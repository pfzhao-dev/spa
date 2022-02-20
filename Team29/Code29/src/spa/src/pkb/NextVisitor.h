#pragma once

#include <memory>
#include <string>
#include <unordered_set>

#include "cfg/Node.h"
#include "cfg/Visitor.h"

namespace spa {

namespace pkb {

class NextVisitor : public cfg::Visitor {
 public:
  NextVisitor(bool transitive, bool bip);

  bool VisitRoot(std::shared_ptr<cfg::Node> node) override;

  bool VisitBranch(std::shared_ptr<cfg::Node> node, uint32_t label) override;

  bool ExploreRoot(std::shared_ptr<cfg::Node> node) override;

  bool ExploreBranch(std::shared_ptr<cfg::Node> node, uint32_t label,
                     bool forward) override;

  std::unordered_set<uint32_t>& GetNextStmts();

 private:
  bool transitive_;
  bool bip_;

  std::unordered_set<uint32_t> visited_stmts_;
  std::unordered_set<uint32_t> next_stmts_;

  // Extra visited & allowed labels for transitive bip exploration.
  std::unordered_set<uint32_t> explore_visited_[2];
  std::unordered_set<uint32_t> allowed_labels_;

  std::string root_proc_;
};

}  // namespace pkb

}  // namespace spa
