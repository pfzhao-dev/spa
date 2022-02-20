#include "Node.h"

namespace spa {

namespace cfg {

void Node::AddChild(std::shared_ptr<Node> node) {
  children_.push_back(node);
}

void Node::AddReverseChild(std::shared_ptr<Node> node) {
  reverse_children_.push_back(node);
}

void Node::AddNeighborIn(std::shared_ptr<Node> node, uint32_t label) {
  neighbor_in_.push_back({node, label});
}

void Node::AddReverseNeighborIn(std::shared_ptr<Node> node, uint32_t label) {
  reverse_neighbor_in_.push_back({node, label});
}

void Node::AddNeighborOut(std::shared_ptr<Node> node, uint32_t label) {
  neighbor_out_.push_back({node, label});
}

uint32_t Node::GetStmtNo() { return stmt_no_; }

bool Node::IsDummy() { return is_dummy_; }

bool Node::HasNeighbor() {
  return !neighbor_in_.empty() || !neighbor_out_.empty();
}

std::vector<std::shared_ptr<Node>>& Node::GetChildren() { return children_; }

std::vector<std::shared_ptr<Node>>& Node::GetReverseChildren() {
  return reverse_children_;
}

std::vector<Node::NeighborEdge>& Node::GetNeighborIn() { return neighbor_in_; }

std::vector<Node::NeighborEdge>& Node::GetReverseNeighborIn() {
  return reverse_neighbor_in_;
}

std::vector<Node::NeighborEdge>& Node::GetNeighborOut() {
  return neighbor_out_;
}

}  // namespace cfg

}  // namespace spa
