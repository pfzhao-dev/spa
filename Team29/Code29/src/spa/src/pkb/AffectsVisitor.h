#pragma once

#include <memory>
#include <string>
#include <unordered_set>

#include "cfg/Node.h"
#include "cfg/Visitor.h"

namespace spa {

namespace pkb {

class AffectsVisitor : public cfg::Visitor {
 public:
  AffectsVisitor(bool bip);

  bool VisitRoot(std::shared_ptr<cfg::Node> node) override;

  bool VisitBranch(std::shared_ptr<cfg::Node> node, uint32_t label) override;

  bool ExploreRoot(std::shared_ptr<cfg::Node> node) override;

  bool ExploreBranch(std::shared_ptr<cfg::Node> node, uint32_t label,
                     bool forward) override;

  std::unordered_set<uint32_t>& GetAffectedStmts();

 private:
  bool bip_;
  std::string modified_var_;

  std::unordered_set<uint32_t> visited_stmts_;
  std::unordered_set<uint32_t> affected_stmts_;

  // Extra visited & allowed labels for transitive bip exploration.
  std::unordered_set<uint32_t> explore_visited_[2];
  std::unordered_set<uint32_t> allowed_labels_;
};

}  // namespace pkb

}  // namespace spa
