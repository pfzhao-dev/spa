#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "cfg/Graph.h"
#include "cfg/Node.h"
#include "cfg/Visitor.h"

namespace spa {

namespace cfg {

class AffectsGraph : public Graph {
 public:
  AffectsGraph();

  void Add(uint32_t affector, uint32_t affected);

  void Traverse(uint32_t stmt_no, std::shared_ptr<Visitor> visitor, bool bip);

 private:
  std::unordered_map<uint32_t, std::shared_ptr<Node>> nodes_;
};

}  // namespace cfg

}  // namespace spa
