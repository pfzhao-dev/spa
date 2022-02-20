#include "AffectsTVisitor.h"

#include "ast/Statement.h"
#include "pkb/Exception.h"
#include "pkb/PKBRoot.h"

namespace spa {

namespace pkb {

AffectsTVisitor::AffectsTVisitor(bool bip) : bip_(bip) {}

bool AffectsTVisitor::VisitRoot(std::shared_ptr<cfg::Node> node) {
  auto stmt_no = node->GetStmtNo();

  // Stop if not assign statement.
  auto pkb_root = pkb::PKBRoot::GetInstance();
  if (!pkb_root->IsStatementType(stmt_no, ast::Statement::Type::kAssign)) {
    return false;
  }

  if (bip_) {
    root_stmt_no_ = stmt_no;
  }
  return true;  // we need to traverse the branch
}

bool AffectsTVisitor::VisitBranch(std::shared_ptr<cfg::Node> node,
                                  uint32_t label) {
  auto stmt_no = node->GetStmtNo();

  if (visited_stmts_.count(stmt_no)) {
    return false;  // stop traversal if already visited
  }

  visited_stmts_.insert(node->GetStmtNo());

  if (bip_) {
    // Prevent traversing to a statement that should not be reachable.
    // Here we rely on NextBip* precomputation.
    auto pkb_root = pkb::PKBRoot::GetInstance();
    auto next_bip_t_cache = pkb_root->GetNextCache(true, true);
    if (!next_bip_t_cache.Count(root_stmt_no_, stmt_no)) {
      return false;
    }
  }

  // Guaranteed to be affected since in the affects graph
  // edges only exist between assign statements.
  //
  // NOTE: This is a bit reduntant since we alr have a set to
  //       keep track of visited stmts. However, having a separate
  //       list to keep track of affected stmts is kinda useful
  //       if we want to extend this in the future.
  affected_stmt_.insert(node->GetStmtNo());

  return true;  // continue traversal
}

std::unordered_set<uint32_t>& AffectsTVisitor::GetAffectedStmts() {
  return affected_stmt_;
}

}  // namespace pkb

}  // namespace spa
