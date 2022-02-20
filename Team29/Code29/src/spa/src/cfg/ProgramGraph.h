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

class ProgramGraph : public Graph {
 public:
  ProgramGraph();

  void Add(std::string proc_name, std::shared_ptr<Node> head,
           std::shared_ptr<Node> tail);

  void Traverse(uint32_t stmt_no, std::shared_ptr<Visitor> visitor, bool bip);

  void Traverse(std::string proc_name, std::shared_ptr<Visitor> visitor, bool bip);

  std::shared_ptr<Node> GetProcHead(std::string proc_name);
  std::shared_ptr<Node> GetProcTail(std::string proc_name);

 private:
  std::unordered_map<std::string, std::shared_ptr<Node>> procs_head_;
  std::unordered_map<std::string, std::shared_ptr<Node>> procs_tail_;

  std::vector<std::shared_ptr<Node>> nodes_;

  void IndexNode(std::shared_ptr<Node> node);
};

}  // namespace cfg

}  // namespace spa
