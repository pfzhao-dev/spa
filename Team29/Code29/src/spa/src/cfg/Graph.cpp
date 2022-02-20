#include "Graph.h"

namespace spa {

namespace cfg {

Graph::Graph() {}

void Graph::Traverse(std::shared_ptr<Node> node,
                     std::shared_ptr<Visitor> visitor, bool bip) {
  if (bip && visitor->ExploreRoot(node)) {
    ExploreBackward(node, visitor);
    ExploreForward(node, visitor);
  }

  if (visitor->VisitRoot(node)) {
    TraverseRecursive(node, visitor, bip);
  }
}

void Graph::TraverseRecursive(std::shared_ptr<Node> node,
                              std::shared_ptr<Visitor> visitor, bool bip) {
  if (bip && node->HasNeighbor()) {
    for (auto [neighbor, label] : node->GetNeighborIn()) {
      if (visitor->VisitBranch(neighbor, label)) {
        TraverseRecursive(neighbor, visitor, bip);
      }
    }
    for (auto [neighbor, label] : node->GetNeighborOut()) {
      if (visitor->VisitBranch(neighbor, label)) {
        TraverseRecursive(neighbor, visitor, bip);
      }
    }
  } else {
    for (auto child : node->GetChildren()) {
      if (visitor->VisitBranch(child, 0)) {
        TraverseRecursive(child, visitor, bip);
      }
    }
  }
}

void Graph::ExploreForward(std::shared_ptr<Node> node,
                           std::shared_ptr<Visitor> visitor) {
  for (auto& child : node->GetChildren()) {
    if (visitor->ExploreBranch(child, 0, true)) {
      ExploreForward(child, visitor);
    }
  }
  for (auto& [child, label] : node->GetNeighborIn()) {
    if (visitor->ExploreBranch(child, label, true)) {
      ExploreForward(child, visitor);
    }
  }
}

void Graph::ExploreBackward(std::shared_ptr<Node> node,
                            std::shared_ptr<Visitor> visitor) {
  for (auto& child : node->GetReverseChildren()) {
    if (visitor->ExploreBranch(child, 0, false)) {
      ExploreBackward(child, visitor);
    }
  }
  for (auto& [child, label] : node->GetReverseNeighborIn()) {
    if (visitor->ExploreBranch(child, label, false)) {
      ExploreBackward(child, visitor);

      // If we move back using a reverse neighbor in, we can explore forward
      // from the node children. Can imagine this as continuing the execution
      // after returning from a function call.
      for (auto nc : child->GetChildren())
        if (visitor->ExploreBranch(nc, label, true)) {
          ExploreForward(nc, visitor);
        }
    }
  }

  // As we move backward, we are also allowed to traverse to the children.
  // Note that this is different with ExploreForward that can also visit
  // the neighbor in.
  for (auto& child : node->GetChildren()) {
    if (visitor->ExploreBranch(child, 0, false)) {
      ExploreBackward(child, visitor);
    }
  }
}

}  // namespace cfg

}  // namespace spa
