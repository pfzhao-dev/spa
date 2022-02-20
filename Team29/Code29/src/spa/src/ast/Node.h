#pragma once

#include <memory>

namespace spa {

namespace ast {

class Node {
 public:
  // The constructor does not take the reference to the parent node to avoid
  // to avoid cluttering the construction of the subclasses. The parent node
  // needs to be set manually using the `SetParent` method if there is any.
  Node();

  // Even though the parameter is a `shared_ptr`, the `Node` class will only
  // store the reference as a `weak_ptr` to avoid potential memory leak.
  void SetParent(std::shared_ptr<Node> parent);

  // Returns a `shared_ptr` since the caller is expected to require access
  // to the parent node.
  std::shared_ptr<Node> GetParent();

 private:
  // Store the reference to the parent node using a weak pointer to avoid
  // memory leak that could happen due to cyclic dependency otherwise.
  std::weak_ptr<Node> parent_;
};

}  // namespace ast

}  // namespace spa
