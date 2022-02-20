#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "cfg/Node.h"
#include "cfg/Visitor.h"

namespace spa {

namespace cfg {

class Graph {
 public:
  Graph();

 protected:
  void Traverse(std::shared_ptr<Node> node, std::shared_ptr<Visitor> visitor,
                bool bip = false);

 private:
  void TraverseRecursive(std::shared_ptr<Node> node,
                         std::shared_ptr<Visitor> visitor, bool bip = false);

  void ExploreForward(std::shared_ptr<Node> node,
                      std::shared_ptr<Visitor> visitor);

  void ExploreBackward(std::shared_ptr<Node> node,
                       std::shared_ptr<Visitor> visitor);
};

}  // namespace cfg

}  // namespace spa
