#include "xtr/ParentExtractor.h"

#include <unordered_set>

#include "pkb/PKBRoot.h"

namespace spa {
namespace xtr {
void ParentExtractor::Extract(std::shared_ptr<ast::Program> prog) {
  auto procs = prog->GetProcList();

  for (auto p : procs) {
    std::vector<uint32_t> parents;
    ExtractStatementList(p->GetStmtList(), parents);
  }
}

void ParentExtractor::ExtractStatementList(
    std::shared_ptr<ast::StatementList> stmt_list,
    std::vector<uint32_t> &parents) {
  auto stmts = stmt_list->GetStatements();

  auto pkb_root = pkb::PKBRoot::GetInstance();

  for (auto stmt : stmts) {
    auto succ = stmt->GetStmtNo();
    for (auto pred : parents) {
      pkb_root->AddParentT(pred, succ);
    }
    // if parent exist
    if (parents.size()) {
      auto pred = parents.back();
      pkb_root->AddParent(pred, succ);
    }

    using stmt_type = ast::Statement::Type;
    auto type = stmt->GetType();
    if (type == stmt_type::kIf) {
      auto if_stmt = std::static_pointer_cast<ast::If>(stmt);
      parents.push_back(stmt->GetStmtNo());
      ExtractStatementList(if_stmt->GetConsequent(), parents);
      ExtractStatementList(if_stmt->GetAlternative(), parents);
      parents.pop_back();
    } else if (type == stmt_type::kWhile) {
      parents.push_back(stmt->GetStmtNo());
      auto while_stmt = std::static_pointer_cast<ast::While>(stmt);
      ExtractStatementList(while_stmt->GetStatements(), parents);
      parents.pop_back();
    }
  }
}

}  // namespace xtr
}  // namespace spa
