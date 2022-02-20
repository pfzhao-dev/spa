#pragma once

#include <memory>
#include <unordered_set>
#include <vector>

namespace spa {

namespace cfg {

class Node {
 public:
  Node(uint32_t stmt_no, bool is_dummy = false)
      : stmt_no_(stmt_no), is_dummy_(is_dummy), children_{} {}

  void AddChild(std::shared_ptr<Node> node);

  void AddReverseChild(std::shared_ptr<Node> node);

  void AddNeighborIn(std::shared_ptr<Node> node, uint32_t label);

  void AddReverseNeighborIn(std::shared_ptr<Node> node, uint32_t label);

  void AddNeighborOut(std::shared_ptr<Node> node, uint32_t label);

  uint32_t GetStmtNo();

  bool IsDummy();

  std::vector<std::shared_ptr<Node>>& GetChildren();
  std::vector<std::shared_ptr<Node>>& GetReverseChildren();

  using NeighborEdge = std::pair<std::shared_ptr<Node>, uint32_t>;

  bool HasNeighbor();
  std::vector<NeighborEdge>& GetNeighborIn();
  std::vector<NeighborEdge>& GetReverseNeighborIn();
  std::vector<NeighborEdge>& GetNeighborOut();

 private:
  uint32_t stmt_no_;
  bool is_dummy_;

  std::vector<std::shared_ptr<Node>> children_;
  std::vector<std::shared_ptr<Node>> reverse_children_;

  std::vector<NeighborEdge> neighbor_in_;
  std::vector<NeighborEdge> reverse_neighbor_in_;
  std::vector<NeighborEdge> neighbor_out_;
};

}  // namespace cfg

}  // namespace spa
