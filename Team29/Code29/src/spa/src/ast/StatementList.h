#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Node.h"
#include "Statement.h"

namespace spa {

namespace ast {

// Forward declaration to avoid cyclic dependency
class Statement;

class StatementList : public Node {
 public:
  StatementList(std::vector<std::shared_ptr<Statement>> statements);

  bool operator==(const spa::ast::StatementList& other) const;

  std::vector<std::shared_ptr<Statement>>& GetStatements();

 private:
  std::vector<std::shared_ptr<Statement>> statements_;
};

}  // namespace ast

}  // namespace spa
