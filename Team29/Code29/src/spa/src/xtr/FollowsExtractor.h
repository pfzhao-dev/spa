#pragma once

#include <memory>

#include "ast/Program.h"
#include "ast/StatementList.h"

namespace spa {
namespace xtr {
class FollowsExtractor {
 public:
  FollowsExtractor();
  void Extract(std::shared_ptr<ast::Program> program);

 private:
  void ExtractStatementList(std::shared_ptr<ast::StatementList> stmts);
};

}  // namespace xtr
}  // namespace spa
