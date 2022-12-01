#include "validator.h"

#include <variant>
#include <vector>

#include "token.h"

namespace sql {

bool is_valid_sql_query(std::vector<Token> tokens) {
  SqlValidator sqlValidator{};
  auto i = tokens.begin();
  while (!sqlValidator.is_invalid() && i != tokens.end()) //exit the loop iff arriving in an ending state 
  {
    sqlValidator.handle(*i);
    i++;
  }
  return sqlValidator.is_valid(); //true if current state is `Valid`, false if current state is `Invalid`
}


bool SqlValidator::is_valid() const {
  return std::holds_alternative<state::Valid>(state_);
}

bool SqlValidator::is_invalid() const {
  return std::holds_alternative<state::Invalid>(state_);
}


void SqlValidator::handle(Token token) {
  if (std::holds_alternative<state::Start>(state_)){
    state_ = transition(std::get<state::Start>(state_), token);
    return;
  }

  if (std::holds_alternative<state::Valid>(state_)){
    state_ = transition(std::get<state::Valid>(state_), token);
    return;
  }

  if (std::holds_alternative<state::Invalid>(state_)){
    state_ = transition(std::get<state::Invalid>(state_), token);
    return;
  }

  if (std::holds_alternative<state::SelectStmt>(state_)){
    state_ = transition(std::get<state::SelectStmt>(state_), token);
    return;
  }
    
  if (std::holds_alternative<state::AllColumns>(state_)){
    state_ = transition(std::get<state::AllColumns>(state_), token);
    return;
  }
  
  if (std::holds_alternative<state::NamedColumn>(state_)){
    state_ = transition(std::get<state::NamedColumn>(state_), token);
    return;
  }

  if (std::holds_alternative<state::MoreColumns>(state_)){
    state_ = transition(std::get<state::MoreColumns>(state_), token);
    return;
  }

  if (std::holds_alternative<state::FromClause>(state_)){
    state_ = transition(std::get<state::FromClause>(state_), token);
    return;
  }

  if (std::holds_alternative<state::TableName>(state_)){
    state_ = transition(std::get<state::TableName>(state_), token);
    return;
  }

}


struct TransitionFromStartVisitor {
  /// Only the `Select` token advances the FSM from the `Start` state
  /// TODO: Correct the behaviour (DONE?)
  State operator()(token::Select) const { return state::SelectStmt{}; }

  /// All the other tokens, put it in the invalid state
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
  State operator()(token::Identifier) const { return state::Invalid{}; }
};

struct TransitionFromValidVisitor {
  /// Only the `Select` token advances the FSM from the `Start` state
  /// TODO: Correct the behaviour (DONE?)
  State operator()(token::Semicolon) const { return state::Valid{}; }

  /// All the other tokens, put it in the invalid state
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Identifier) const { return state::Invalid{}; }
};


struct TransitionFromSelectStmtVisitor {
  State operator()(token::Asterisks) const { return state::AllColumns{}; }
  State operator()(token::Identifier) const { return state::NamedColumn{}; }

  /// All the other tokens, put it in the invalid state
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
};

struct TransitionFromAllColumnsVisitor {
  State operator()(token::From) const { return state::FromClause{}; }

  /// All the other tokens, put it in the invalid state
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Identifier) const { return state::Invalid{}; }
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
};

struct TransitionFromNamedColumnVisitor {
  State operator()(token::From) const { return state::FromClause{}; }
  State operator()(token::Comma) const { return state::MoreColumns{}; }

  /// All the other tokens, put it in the invalid state
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Identifier) const { return state::Invalid{}; }
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
};

struct TransitionFromMoreColumnsVisitor {
  State operator()(token::Identifier) const { return state::NamedColumn{}; }

  /// All the other tokens, put it in the invalid state
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
};

struct TransitionFromFromClauseVisitor {
  State operator()(token::Identifier) const { return state::TableName{}; }

  /// All the other tokens, put it in the invalid state
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Select) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
};

struct TransitionFromTableNameVisitor {
  State operator()(token::Semicolon) const { return state::Valid{}; }

  /// All the other tokens, put it in the invalid state
  State operator()(token::Identifier) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Select) const { return state::Invalid{}; }
};

State transition(state::Start, Token token) {
  return std::visit(TransitionFromStartVisitor{}, token.value());
}

State transition(state::Valid, Token token) {
  // TODO: Implement //maybe not since they are ending states (?)
  return std::visit(TransitionFromValidVisitor{}, token.value());
}

State transition(state::Invalid, Token token) {
  // TODO: Implement //maybe not since they are ending states (?)
  return state::Invalid{};
}

State transition(state::SelectStmt, Token token) {
  // TODO: Implement
  return std::visit(TransitionFromSelectStmtVisitor{}, token.value());
}

State transition(state::AllColumns, Token token) {
  // TODO: Implement
  return std::visit(TransitionFromAllColumnsVisitor{}, token.value());
}

State transition(state::NamedColumn, Token token) {
  // TODO: Implement
  return std::visit(TransitionFromNamedColumnVisitor{}, token.value());
}

State transition(state::MoreColumns, Token token) {
  // TODO: Implement
  return std::visit(TransitionFromMoreColumnsVisitor{}, token.value());
}

State transition(state::FromClause, Token token) {
  // TODO: Implement
  return std::visit(TransitionFromFromClauseVisitor{}, token.value());
}

State transition(state::TableName, Token token) {
  // TODO: Implement
  return std::visit(TransitionFromTableNameVisitor{}, token.value());
}

} // namespace sql
