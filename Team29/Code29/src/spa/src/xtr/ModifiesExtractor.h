#pragma once

#include <memory>
#include <vector>

#include "ast/Expression.h"
#include "ast/Program.h"
#include "ast/StatementList.h"

namespace spa {
namespace xtr {
class ModifiesExtractor {
 public:
  ModifiesExtractor();
  void Extract(std::shared_ptr<ast::Program> program,
               std::vector<std::string> order);

 private:
  void ExtractStatementList(std::string proc_name,
                            std::shared_ptr<ast::StatementList> stmts);

  void ExtractExpression(std::string proc_name, uint32_t stmt_no,
                         std::shared_ptr<ast::Variable> expr);
};
}  // namespace xtr
}  // namespace spa