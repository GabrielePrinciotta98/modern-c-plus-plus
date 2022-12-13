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

void SqlValidator::handle(Token token){
  state_ = std::visit([&](auto cur) -> State { return transition(cur, token);}, state_);
}

struct TransitionFromStartVisitor {
  State operator()(token::Select) const { return state::SelectStmt{}; }
  /// All the other tokens, put it in the invalid state
  State operator()(auto) const { return state::Invalid{}; }
};

struct TransitionFromValidVisitor {
  State operator()(token::Semicolon) const { return state::Valid{}; }
  /// All the other tokens, put it in the invalid state
  State operator()(auto) const { return state::Invalid{}; }
};


struct TransitionFromSelectStmtVisitor {
  State operator()(token::Asterisks) const { return state::AllColumns{}; }
  State operator()(token::Identifier) const { return state::NamedColumn{}; }
  /// All the other tokens, put it in the invalid state
  State operator()(auto) const { return state::Invalid{}; }
};

struct TransitionFromAllColumnsVisitor {
  State operator()(token::From) const { return state::FromClause{}; }
  /// All the other tokens, put it in the invalid state
  State operator()(auto) const { return state::Invalid{}; }
};

struct TransitionFromNamedColumnVisitor {
  State operator()(token::From) const { return state::FromClause{}; }
  State operator()(token::Comma) const { return state::MoreColumns{}; }
  /// All the other tokens, put it in the invalid state
  State operator()(auto) const { return state::Invalid{}; }
};

struct TransitionFromMoreColumnsVisitor {
  State operator()(token::Identifier) const { return state::NamedColumn{}; }
  /// All the other tokens, put it in the invalid state
  State operator()(auto) const { return state::Invalid{}; }
};

struct TransitionFromFromClauseVisitor {
  State operator()(token::Identifier) const { return state::TableName{}; }
  /// All the other tokens, put it in the invalid state
  State operator()(auto) const { return state::Invalid{}; }
};

struct TransitionFromTableNameVisitor {
  State operator()(token::Semicolon) const { return state::Valid{}; }
  /// All the other tokens, put it in the invalid state
  State operator()(auto) const { return state::Invalid{}; }
};

State transition(state::Start, Token token) {
  return std::visit(TransitionFromStartVisitor{}, token.value());
}

State transition(state::Valid, Token token) {
  return std::visit(TransitionFromValidVisitor{}, token.value());
}

State transition(state::Invalid, Token) {
  return state::Invalid{};
}

State transition(state::SelectStmt, Token token) {
  return std::visit(TransitionFromSelectStmtVisitor{}, token.value());
}

State transition(state::AllColumns, Token token) {
  return std::visit(TransitionFromAllColumnsVisitor{}, token.value());
}

State transition(state::NamedColumn, Token token) {
  return std::visit(TransitionFromNamedColumnVisitor{}, token.value());
}

State transition(state::MoreColumns, Token token) {
  return std::visit(TransitionFromMoreColumnsVisitor{}, token.value());
}

State transition(state::FromClause, Token token) {
  return std::visit(TransitionFromFromClauseVisitor{}, token.value());
}

State transition(state::TableName, Token token) {
  return std::visit(TransitionFromTableNameVisitor{}, token.value());
}

} // namespace sql
