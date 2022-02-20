#include "AffectsVisitor.h"

#include "ast/Statement.h"
#include "pkb/PKBRoot.h"

namespace spa {

namespace pkb {

AffectsVisitor::AffectsVisitor(bool bip) : bip_(bip) {}

bool AffectsVisitor::ExploreRoot(std::shared_ptr<cfg::Node> node) {
  return bip_;
}

bool AffectsVisitor::VisitRoot(std::shared_ptr<cfg::Node> node) {
  auto stmt_no = node->GetStmtNo();

  // Stop if not assign statement.
  auto pkb_root = pkb::PKBRoot::GetInstance();
  if (!pkb_root->IsStatementType(stmt_no, ast::Statement::Type::kAssign)) {
    return false;
  }

  auto modifies_cache = pkb_root->GetModifiesStatementCache();
  auto modifies_set = modifies_cache.GetOriginalValues(node->GetStmtNo());
  modified_var_ = *modifies_set.begin();  // guaranteed for assign stmt

  return true;
}

bool AffectsVisitor::ExploreBranch(std::shared_ptr<cfg::Node> node,
                                   uint32_t label, bool forward) {
  auto stmt_no = node->GetStmtNo();

  // Allow label if managed to be explored.
  if (label > 0) allowed_labels_.insert(label);

  if (explore_visited_[forward].count(stmt_no)) return false;
  explore_visited_[forward].insert(stmt_no);

  return true;
}

bool AffectsVisitor::VisitBranch(std::shared_ptr<cfg::Node> node,
                                 uint32_t label) {
  auto stmt_no = node->GetStmtNo();

  if (visited_stmts_.count(stmt_no)) {
    return false;  // stop traversal if already visited
  }

  visited_stmts_.insert(stmt_no);

  auto pkb_root = pkb::PKBRoot::GetInstance();

  if (bip_ && label > 0) {
    // Prevent traversing using unallowed label in transitive bip.
    if (!allowed_labels_.count(label)) return false;
  }

  // If assign statement and uses the variable, then add
  // it to the list of affected statements.
  if (pkb_root->IsStatementType(stmt_no, ast::Statement::Type::kAssign)) {
    auto& uses_cache = pkb_root->GetUsesStatementCache();
    if (uses_cache.Count(stmt_no, modified_var_)) {
      affected_stmts_.insert(node->GetStmtNo());
    }
  }

  using StmtType = ast::Statement::Type;
  if (!(pkb_root->IsStatementType(stmt_no, StmtType::kIf) ||
        pkb_root->IsStatementType(stmt_no, StmtType::kWhile)) &&
      // In the bip mode, we should not consider call stmt
      (!bip_ || !pkb_root->IsStatementType(stmt_no, StmtType::kCall))) {
    // If not an if or while statement and modifies the variable,
    // then stop traversing.
    auto modifies_cache = pkb_root->GetModifiesStatementCache();
    if (modifies_cache.Count(stmt_no, modified_var_)) {
      return false;
    }
  }

  return true;  // continue traversal
}

std::unordered_set<uint32_t>& AffectsVisitor::GetAffectedStmts() {
  return affected_stmts_;
}

}  // namespace pkb

}  // namespace spa
