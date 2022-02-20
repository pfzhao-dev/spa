#pragma once

#include <memory>
#include <unordered_set>
#include <vector>

#include "ast/Program.h"

namespace spa {
namespace xtr {
class SourceExtractor {
 public:
  void Extract(std::shared_ptr<ast::Program> program);

 private:
  void PopulatePKBProg(std::shared_ptr<ast::Program> program);

  void PopulatePKBStatementList(std::string proc_name,
                                std::shared_ptr<ast::StatementList> stmt_list);
  void PopulatePKBStatement(
      std::string proc_name, std::shared_ptr<ast::Statement> stmt_list);
  void PopulatePKBExpression(std::shared_ptr<ast::Expression> expr);

  void ToposortUtil(std::string cur, std::unordered_set<std::string>& visited,
                    std::vector<std::string>& stack);
  std::vector<std::string> Toposort(std::shared_ptr<ast::Program> program);
};
}  // namespace xtr
}  // namespace spa
