#include "xtr/FollowsExtractor.h"

#include <unordered_set>

#include "ast/Statement.h"
#include "pkb/PKBRoot.h"

namespace spa {
namespace xtr {

FollowsExtractor::FollowsExtractor() {}
void FollowsExtractor::Extract(std::shared_ptr<ast::Program> prog) {
  auto procs = prog->GetProcList();

  for (auto p : procs) {
    ExtractStatementList(p->GetStmtList());
  }
}

void FollowsExtractor::ExtractStatementList(
    std::shared_ptr<ast::StatementList> stmt_list) {
  auto stmts = stmt_list->GetStatements();

  auto pkb_root = pkb::PKBRoot::GetInstance();

  for (auto i = 0; i < stmts.size(); i++) {
    auto succ = stmts[i]->GetStmtNo();
    if (i != 0) {
      auto pred = stmts[i - 1]->GetStmtNo();
      pkb_root->AddFollows(pred, succ);
    }
    for (auto j = 0; j <= i - 1; j++) {
      auto pred = stmts[j]->GetStmtNo();
      pkb_root->AddFollowsT(pred, succ);
    }
    using stmt_type = ast::Statement::Type;
    auto type = stmts[i]->GetType();
    if (type == stmt_type::kIf) {
      auto if_stmt = std::static_pointer_cast<ast::If>(stmts[i]);
      ExtractStatementList(if_stmt->GetConsequent());
      ExtractStatementList(if_stmt->GetAlternative());
    } else if (type == stmt_type::kWhile) {
      auto while_stmt = std::static_pointer_cast<ast::While>(stmts[i]);
      ExtractStatementList(while_stmt->GetStatements());
    }
  }
}

}  // namespace xtr
}  // namespace spa
