#include "ProgramGraph.h"

#include <cstdio>

namespace spa {

namespace cfg {

ProgramGraph::ProgramGraph()
    : Graph(), nodes_{}, procs_head_{}, procs_tail_{} {}

void ProgramGraph::Add(std::string proc_name, std::shared_ptr<Node> head,
                       std::shared_ptr<Node> tail) {
  procs_head_[proc_name] = head;
  procs_tail_[proc_name] = tail;
  IndexNode(head);
}

void ProgramGraph::IndexNode(std::shared_ptr<Node> node) {
  auto stmt_no = node->GetStmtNo();

  // NOTE: We should not use the 0-th index since the stmt no is 1-based.
  if (nodes_.size() <= stmt_no) {
    nodes_.resize(stmt_no + 1, {});
  }

  if (nodes_[stmt_no]) return;

  nodes_[stmt_no] = node;

  for (auto child : node->GetChildren()) {
    IndexNode(child);
  }
}

void ProgramGraph::Traverse(uint32_t stmt_no,
                            std::shared_ptr<Visitor> visitor, bool bip) {
  if (stmt_no < 1 || nodes_.size() <= stmt_no || !nodes_[stmt_no]) return;
  Graph::Traverse(nodes_[stmt_no], visitor, bip);
}

void ProgramGraph::Traverse(std::string proc_name,
                            std::shared_ptr<Visitor> visitor, bool bip) {
  if (!procs_head_.count(proc_name)) return;
  Graph::Traverse(procs_head_[proc_name], visitor, bip);
}

std::shared_ptr<Node> ProgramGraph::GetProcHead(std::string proc_name) {
  return procs_head_[proc_name];
}

std::shared_ptr<Node> ProgramGraph::GetProcTail(std::string proc_name) {
  return procs_tail_[proc_name];
}

}  // namespace cfg

}  // namespace spa
