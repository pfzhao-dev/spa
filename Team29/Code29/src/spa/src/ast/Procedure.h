#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Node.h"
#include "ast/StatementList.h"

namespace spa {

namespace ast {

class Procedure : public Node {
 public:
  Procedure(std::string name, std::shared_ptr<StatementList> stmt_list);

  bool operator==(const spa::ast::Procedure &other_procedure) const;

  const std::string GetName() const;

  const std::shared_ptr<StatementList> GetStmtList() const;

 private:
  std::string name_;
  std::shared_ptr<StatementList> stmt_list_;
};

}  // namespace ast

}  // namespace spa
