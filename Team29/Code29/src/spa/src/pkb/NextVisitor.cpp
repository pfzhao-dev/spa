#include "NextVisitor.h"

#include <cstdio>

#include "pkb/Exception.h"
#include "pkb/PKBRoot.h"

namespace spa {

namespace pkb {

NextVisitor::NextVisitor(bool transitive, bool bip)
    : transitive_(transitive), bip_(bip) {}

bool NextVisitor::ExploreRoot(std::shared_ptr<cfg::Node> node) {
  if (bip_ && transitive_)
    return true;
  else
    return false;
}

bool NextVisitor::VisitRoot(std::shared_ptr<cfg::Node> node) {
  return true;
}

bool NextVisitor::ExploreBranch(std::shared_ptr<cfg::Node> node, uint32_t label,
                                bool forward) {
  auto stmt_no = node->GetStmtNo();

  if (label > 0) allowed_labels_.insert(label);

  if (explore_visited_[forward].count(stmt_no)) return false;
  explore_visited_[forward].insert(stmt_no);

  return true;
}

bool NextVisitor::VisitBranch(std::shared_ptr<cfg::Node> node, uint32_t label) {
  auto stmt_no = node->GetStmtNo();

  if (visited_stmts_.count(stmt_no)) {
    return false;  // stop traversal if already visited
  }

  visited_stmts_.insert(stmt_no);

  // label > 0 only when using neighbor edge
  if (transitive_ && bip_ && label > 0) {
    // Prevent traversing using unallowed label in transitive bip.
    if (!allowed_labels_.count(label)) return false;
  }

  if (!node->IsDummy()) {
    next_stmts_.insert(stmt_no);

    // Both of bip and non-bip have to terminate.
    // NOTE: The total time complexity over all computations (i.e., start DFS
    //       from every single node) will be equal to the number of edges,
    //       which is O(N).
    if (!transitive_) return false;
  }

  return true;  // continue traversal
}

std::unordered_set<uint32_t>& NextVisitor::GetNextStmts() {
  return next_stmts_;
}

}  // namespace pkb

}  // namespace spa
