#include "Procedure.h"

#include <iostream>

namespace spa {

namespace ast {

Procedure::Procedure(std::string name, std::shared_ptr<StatementList> stmt_list)
    : name_(name), stmt_list_(stmt_list) {}

bool Procedure::operator==(const Procedure &other_procedure) const {
  return this->name_ == other_procedure.name_ &&
         *this->stmt_list_ == *(other_procedure.stmt_list_);
}

const std::string Procedure::GetName() const { return name_; }

const std::shared_ptr<StatementList> Procedure::GetStmtList() const {
  return stmt_list_;
}

}  // namespace ast

}  // namespace spa
