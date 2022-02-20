#pragma once

#include <memory>

#include "cfg/Node.h"

namespace spa {

namespace cfg {

class Visitor {
 public:
  // Notifies the visitor that a root node is about to be visited.
  // Returns whether the root node should be visited or not.
  virtual bool VisitRoot(std::shared_ptr<Node> node);

  // Notifies the visitor that a branch node is about to be visited.
  // Returns whether the branch node should be visited or not.
  //
  // If the branch is visited with a neighbor edge, the label should be > 0.
  virtual bool VisitBranch(std::shared_ptr<Node> node, uint32_t label);

  // Notifies the visitor that a root node is about to perform exploration.
  // Returns whether the exploration should be performed or not.
  //
  // This is useful for transitive bip clauses.
  virtual bool ExploreRoot(std::shared_ptr<Node> node);

  // Explore the branch in the beginning by traversing children and neighbor_in
  // in forward and reverse orders based on the root.
  //
  // This is useful for transitive bip clauses.
  virtual bool ExploreBranch(std::shared_ptr<Node> node, uint32_t label,
                             bool forward);
};

}  // namespace cfg

}  // namespace spa
