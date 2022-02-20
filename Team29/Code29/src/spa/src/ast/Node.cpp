#include "Node.h"

namespace spa {

namespace ast {

Node::Node() {}

void Node::SetParent(std::shared_ptr<Node> parent) { parent_ = parent; }

std::shared_ptr<Node> Node::GetParent() { return parent_.lock(); }

}  // namespace ast

}  // namespace spa
