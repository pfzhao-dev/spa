#include "StatementList.h"

#include "Statement.h"

namespace spa {
namespace ast {

StatementList::StatementList(std::vector<std::shared_ptr<Statement>> statements)
    : statements_(statements) {}

bool StatementList::operator==(const StatementList& other) const {
  if (statements_.size() != other.statements_.size()) {
    return false;
  }
  for (size_t i = 0; i < statements_.size(); i++) {
    if (!(*statements_[i] == *other.statements_[i])) {
      return false;
    }
  }
  return true;
}

std::vector<std::shared_ptr<Statement>>& StatementList::GetStatements() {
  return statements_;
}

}  // namespace ast

}  // namespace spa
