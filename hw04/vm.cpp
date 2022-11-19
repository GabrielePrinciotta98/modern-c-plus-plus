#include "vm.h"

#include <iostream>
#include <limits>

#include "util.h"


namespace vm {


vm_state create_vm(bool debug) {
    vm_state state;

    // enable vm debugging
    state.debug = debug;


    register_instruction(state, "PRINT", [](vm_state& vmstate, const item_t) {
        std::cout << vmstate.stack.top() << std::endl;
        return true;
    });

    // TODO create instructions
    register_instruction(state, "LOAD_CONST", [](vm_state& vmstate, const item_t item){
        vmstate.stack.push(item);
        return true;
    });

    register_instruction(state, "EXIT", [](vm_state& vmstate, const item_t){
        if (vmstate.stack.empty())
        {
            throw vm_stackfail{"empty stack!"};
        }
        return false;
    });

    register_instruction(state, "POP", [](vm_state& vmstate, const item_t){
        if (vmstate.stack.empty())
            throw vm_stackfail{"empty stack!"};
        vmstate.stack.pop();
        return true;
    });

    register_instruction(state, "ADD", [](vm_state& vmstate, const item_t){
        if (vmstate.stack.size() < 2)
            throw vm_stackfail{"stack size not sufficient!"};
        item_t tos{vmstate.stack.top()};
        vmstate.stack.pop();
        item_t tos1{vmstate.stack.top()};
        vmstate.stack.pop();
        vmstate.stack.push(tos1 + tos);
        return true;
    });

    register_instruction(state, "DIV", [](vm_state& vmstate, const item_t){
        if (vmstate.stack.size() < 2)
            throw vm_stackfail{"stack size not sufficient!"};
        item_t tos{vmstate.stack.top()};
        if (tos == 0) 
            throw div_by_zero{"cannot divide by zero!"};
        vmstate.stack.pop();
        item_t tos1{vmstate.stack.top()};
        vmstate.stack.pop();
        vmstate.stack.push(tos1 / tos);
        return true;
    });

    register_instruction(state, "EQ", [](vm_state& vmstate, const item_t){
        if (vmstate.stack.size() < 2)
            throw vm_stackfail{"stack size not sufficient!"};
        item_t tos{vmstate.stack.top()};
        vmstate.stack.pop();
        item_t tos1{vmstate.stack.top()};
        vmstate.stack.pop();
        if (tos == tos1)
            vmstate.stack.push(1);
        else
            vmstate.stack.push(0);
        return true;
    });

    register_instruction(state, "NEQ", [](vm_state& vmstate, const item_t){
        if (vmstate.stack.size() < 2)
            throw vm_stackfail{"stack size not sufficient!"};
        item_t tos{vmstate.stack.top()};
        vmstate.stack.pop();
        item_t tos1{vmstate.stack.top()};
        vmstate.stack.pop();
        if (tos == tos1)
            vmstate.stack.push(0);
        else
            vmstate.stack.push(1);
        return true;
    });

    register_instruction(state, "DUP", [](vm_state& vmstate, const item_t){
        if (vmstate.stack.empty())
            throw vm_stackfail{"empty stack!"};
        item_t tos{vmstate.stack.top()};
        vmstate.stack.push(tos);
        return true;
    });

    register_instruction(state, "JMP", [](vm_state& vmstate, const item_t item){
        vmstate.pc = static_cast<size_t>(item);
        return true;
    });

    register_instruction(state, "JMPZ", [](vm_state& vmstate, const item_t item){
        if (vmstate.stack.empty())
            throw vm_stackfail{"empty stack!"};
        if (vmstate.stack.top() == 0)
            vmstate.pc = static_cast<size_t>(item);
        vmstate.stack.pop();
        return true;
    });

    register_instruction(state, "WRITE", [](vm_state& vmstate, const item_t){
        if (vmstate.stack.empty())
            throw vm_stackfail{"empty stack!"};
        item_t tos{vmstate.stack.top()};
        //append TOS as a number to the vm output string;
        vmstate.output_text.append(std::to_string(tos));
        return true;
    });

    register_instruction(state, "WRITE_CHAR", [](vm_state& vmstate, const item_t){
        if (vmstate.stack.empty())
            throw vm_stackfail{"empty stack!"};
        item_t tos{vmstate.stack.top()};
        //append TOS as a char to the vm output string;
        char ascii_tos = static_cast<char>(tos); //TODO CONVERSIONE INT TO CHAR NON VA
        vmstate.output_text += ascii_tos;
        return true;
    });

    return state;
}


void register_instruction(vm_state& state, std::string_view name,
                          const op_action_t& action) {
    size_t op_id = state.next_op_id;

    // TODO make instruction available to vm

    //mapping instruction name -> op_id
    state.instruction_ids.emplace(std::make_pair(static_cast<std::string>(name), op_id));
    //mapping op_id -> instruction name
    state.instruction_names.emplace(std::make_pair(op_id, static_cast<std::string>(name)));
    //mapping op_id -> action
    state.instruction_actions.emplace(std::make_pair(op_id, action));

    //increment next_op_id for the next instruction to register
    state.next_op_id++;

}


code_t assemble(const vm_state& state, std::string_view input_program) {
    code_t code;

    // convert each line separately
    for (auto& line : util::split(input_program, '\n')) {

        auto line_words = util::split(line, ' ');

        // only support instruction and one argument
        if (line_words.size() >= 3) {
            throw invalid_instruction{std::string{"more than one instruction argument: "} + line};
        }

        // look up instruction id
        auto& op_name = line_words[0];
        auto find_op_id = state.instruction_ids.find(op_name);
        if (find_op_id == std::end(state.instruction_ids)) {
            throw invalid_instruction{std::string{"unknown instruction: "} + op_name};
        }
        op_id_t op_id = find_op_id->second;

        // parse the argument
        item_t argument{0};
        if (line_words.size() == 2) {
            argument = std::stoll(line_words[1]);
        }

        // and save the instruction to the code store
        code.emplace_back(op_id, argument);
    }

    return code;
}


std::tuple<item_t, std::string> run(vm_state& vm, const code_t& code) {
    // to help you debugging the code!
    if (vm.debug) {
        std::cout << "=== running vm ======================" << std::endl;
        std::cout << "disassembly of run code:" << std::endl;
        for (const auto &[op_id, arg] : code) {
            if (not vm.instruction_names.contains(op_id)) {
                std::cout << "could not disassemble - op_id unknown..." << std::endl;
                std::cout << "turning off debug mode." << std::endl;
                vm.debug = false;
                break;
            }
            std::cout << vm.instruction_names[op_id] << " " << arg << std::endl;
        }
        std::cout << "=== end of disassembly" << std::endl << std::endl;
    }
    std::tuple<item_t, std::string> result = {0, ""};
    // execution loop for the machine
    while (true) {
        if (vm.pc > code.size() - 1)
            throw vm_segfault{"seg fault!"};
        auto& [op_id, arg] = code[vm.pc];

        if (vm.debug) {
            std::cout << "-- exec " << vm.instruction_names[op_id] << " arg=" << arg << " at pc=" << vm.pc << std::endl;
        }

        // increase the program counter here so its value can be overwritten
        // by the instruction when it executes!
        vm.pc += 1;

        // TODO execute instruction and stop if the action returns false.
        bool execution_result{vm.instruction_actions[op_id](vm, arg)};
        item_t tos{0};
        
        tos = vm.stack.top();
        std::string out_text{vm.output_text};
        result = std::make_tuple(tos, out_text); 
        if (execution_result == false) 
            break;
    }

    return result; //return tuple(exit value, output text)
}


} // namespace vm
