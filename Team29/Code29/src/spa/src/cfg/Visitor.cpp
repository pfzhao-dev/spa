#include "Visitor.h"

namespace spa {

namespace cfg {

bool Visitor::VisitRoot(std::shared_ptr<Node> node) { return false; }

bool Visitor::VisitBranch(std::shared_ptr<Node> node, uint32_t label) {
  return false;
}

bool Visitor::ExploreRoot(std::shared_ptr<Node> node) { return false; }

bool Visitor::ExploreBranch(std::shared_ptr<Node> node, uint32_t label,
                            bool forward) {
  return false;
}

}  // namespace cfg

}  // namespace spa
