#include "contact_list.h"
#include "hw03.h"

#include <iostream>

namespace contact_list {

void test_contact_list() {
    storage abook;
    add(abook, "Best Friend", 421234);
    add(abook, "False Friend", 10032331);
    //std::cout << to_string(abook);
    //std::cout << size(abook) << '\n';
    add(abook, "Luca", 9876);
    add(abook, "Carlo", 1234);
    //std::cout << to_string(abook);
    //remove(abook, "Luca");
    std::cout << to_string(abook);
    //std::cout << size(abook) << '\n';
    //std::cout << get_number_by_name(abook, "False Friend") << '\n';
    //std::cout << get_name_by_number(abook, 9876) << '\n';
    sort(abook);
    std::cout << to_string(abook);

}

} // namespace contact_list


int main() {
    contact_list::test_contact_list();
    std::cout << "program terminated\n";
    return 0;
}
