#include "Statement.h"

#include <iostream>

#include "StatementList.h"

namespace spa {
namespace ast {

bool Statement::operator==(const Statement &other) const {
  return this->Equals(other);
}

bool Statement::Equals(const Statement &other) const {
  return stmt_no_ == other.stmt_no_;
}

bool Print::Equals(const Statement &other_stmt) const {
  if (typeid(*this) != typeid(other_stmt)) return false;
  Print other_print = static_cast<Print const &>(other_stmt);
  return Statement::Equals(other_stmt) && *variable_ == *other_print.variable_;
}

bool Assign::Equals(const Statement &other_stmt) const {
  if (typeid(*this) != typeid(other_stmt)) return false;
  Assign other_assign = static_cast<Assign const &>(other_stmt);
  return Statement::Equals(other_stmt) &&
         *variable_ == *other_assign.variable_ && *expr_ == *other_assign.expr_;
}

bool Call::Equals(const Statement &other_stmt) const {
  if (typeid(*this) != typeid(other_stmt)) return false;
  Call other_call = static_cast<Call const &>(other_stmt);
  return Statement::Equals(other_stmt) && proc_name_ == other_call.proc_name_;
}

bool If::Equals(const Statement &other_stmt) const {
  if (typeid(*this) != typeid(other_stmt)) return false;
  If other_if = static_cast<If const &>(other_stmt);
  return Statement::Equals(other_stmt) && *this->cond_ == *other_if.cond_ &&
         *consequent_ == *(other_if.consequent_) &&
         *alternative_ == *(other_if.alternative_);
}

bool While::Equals(const Statement &other_stmt) const {
  if (typeid(*this) != typeid(other_stmt)) return false;
  While other_while = static_cast<While const &>(other_stmt);
  return Statement::Equals(other_stmt) && *this->cond_ == *other_while.cond_ &&
         *this->stmt_list_ == *(other_while.stmt_list_);
}
const std::shared_ptr<StatementList> &While::GetStmtList() const {
  return stmt_list_;
}

bool Read::Equals(const Statement &other_stmt) const {
  if (typeid(*this) != typeid(other_stmt)) return false;
  Read other_read = static_cast<Read const &>(other_stmt);

  return Statement::Equals(other_stmt) && *variable_ == *other_read.variable_;
}

Statement::Type Statement::GetType() { return type_; }

uint32_t Statement::GetStmtNo() { return stmt_no_; }

std::shared_ptr<Variable> Read::GetVariable() { return variable_; }

std::shared_ptr<Variable> Print::GetVariable() { return variable_; }

std::shared_ptr<Variable> Assign::GetVariable() { return variable_; }

std::shared_ptr<Absolute> Assign::GetExpression() { return expr_; }

std::shared_ptr<Conditional> While::GetCond() { return cond_; }

std::shared_ptr<StatementList> While::GetStatements() { return stmt_list_; }

std::shared_ptr<Conditional> If::GetCond() { return cond_; }

std::shared_ptr<StatementList> If::GetConsequent() { return consequent_; }

std::shared_ptr<StatementList> If::GetAlternative() { return alternative_; }

std::string Call::GetProcName() { return proc_name_; }

}  // namespace ast

}  // namespace spa
