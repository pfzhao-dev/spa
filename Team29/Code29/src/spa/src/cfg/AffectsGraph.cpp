#include "AffectsGraph.h"

namespace spa {

namespace cfg {

AffectsGraph::AffectsGraph() : Graph(), nodes_{} {}

void AffectsGraph::Add(uint32_t pred, uint32_t succ) {
  if (!nodes_.count(pred)) nodes_[pred] = std::make_shared<Node>(pred);
  if (!nodes_.count(succ)) nodes_[succ] = std::make_shared<Node>(succ);

  nodes_[pred]->AddChild(nodes_[succ]);
}

void AffectsGraph::Traverse(uint32_t stmt_no,
                            std::shared_ptr<Visitor> visitor, bool bip) {
  // If there's no such node to be traversed, just stop.
  if (!nodes_.count(stmt_no)) return;

  Graph::Traverse(nodes_[stmt_no], visitor);
}

}  // namespace cfg

}  // namespace spa
