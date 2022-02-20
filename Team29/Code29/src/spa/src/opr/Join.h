#pragma once

#include <map>
#include <vector>

#include "opr/Operator.h"
#include "pql/clause/Clause.h"
#include "utils/Element.h"

namespace spa {

namespace opr {

class Join : public Operator {
 public:
  Join(std::shared_ptr<Operator> left, std::shared_ptr<Operator> right);

  std::shared_ptr<utils::Batch> Fetch() override;

 protected:
  // Compute the estimation cost.
  uint64_t ComputeEstCost() override;

  // Compute the estimation result.
  pql::Estimation ComputeEstResult() override;

  std::ostream& ToString(std::ostream& out) const override;

 private:
  std::shared_ptr<Operator> left_;
  std::shared_ptr<Operator> right_;

  // Join indices for the left and right opr.
  std::vector<std::pair<size_t, size_t>> join_idx_;

  // Helper values to help the merging of two elements. The values in this list
  // are equal to the indices of the right batch that are not join keys.
  std::vector<size_t> right_ext_idx_;

  // Helper function to build the index of a particular batch.
  std::map<std::vector<utils::Element>, std::vector<size_t>> BuildIndex(
      std::shared_ptr<utils::Batch> batch, bool is_left);

  // Get the join keys from a list of elements.
  std::vector<utils::Element> GetJoinKeys(std::vector<utils::Element>& tuple,
                                          bool is_left);

  // Merge the two tuples that come the left and right batches.
  std::vector<utils::Element> Merge(std::vector<utils::Element>& left_tuple,
                                    std::vector<utils::Element>& right_tuple);
};

}  // namespace opr

}  // namespace spa
