#pragma once

#include <memory>
#include <vector>

#include "ast/Expression.h"
#include "ast/Program.h"
#include "ast/StatementList.h"

namespace spa {
namespace xtr {
class PatternExtractor {
 public:
  PatternExtractor();
  void Extract(std::shared_ptr<ast::Program> program);

 private:
  void ExtractStatementList(std::shared_ptr<ast::StatementList> stmts);

  void ExtractExpression(uint32_t stmt_no,
                         std::shared_ptr<ast::Expression> expr,
                         ast::Statement::Type stmt_type);
};
}  // namespace xtr
}  // namespace spa