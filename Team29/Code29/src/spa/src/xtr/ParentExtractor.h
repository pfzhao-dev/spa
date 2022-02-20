#pragma once

#include <memory>
#include <vector>

#include "ast/Program.h"
#include "ast/Statement.h"
#include "ast/StatementList.h"

namespace spa {
namespace xtr {
class ParentExtractor {
 public:
  void Extract(std::shared_ptr<ast::Program> program);

 private:
  void ExtractStatementList(std::shared_ptr<ast::StatementList> stmts,
                            std::vector<uint32_t>& parents);
};

}  // namespace xtr
}  // namespace spa
