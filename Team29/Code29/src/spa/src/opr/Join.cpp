#include "Join.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <variant>

#include "utils/Schema.h"

namespace spa {

namespace opr {

Join::Join(std::shared_ptr<Operator> left, std::shared_ptr<Operator> right)
    : left_(left),
      right_(right),
      Operator(Operator::Type::kJoin,
               utils::Schema::Join(left->GetSchema(), right->GetSchema())) {
  join_idx_ = utils::Schema::GetJoinIdx(left->GetSchema(), right->GetSchema());

  // Pre-compute the skipped idx for the right batch
  // This is used to create the join values.
  std::vector<size_t> right_sorted_join_idx;
  for (const auto [_, r_join_idx] : join_idx_) {
    right_sorted_join_idx.push_back(r_join_idx);
  }
  std::sort(right_sorted_join_idx.begin(), right_sorted_join_idx.end());

  // Take all the values from the right tuple in order except the join keys.
  auto right_schema = right_->GetSchema();
  auto right_tuple_size = right_schema.Size();

  size_t skipped_pivot = 0;
  for (size_t i = 0; i < right_tuple_size; ++i) {
    if (skipped_pivot < right_sorted_join_idx.size() &&
        right_sorted_join_idx[skipped_pivot] == i) {
      // Skip the join keys for the right batch
      skipped_pivot++;
      continue;
    }

    right_ext_idx_.push_back(i);
  }
}

uint64_t Join::ComputeEstCost() {
  auto left_cost = left_->EstimateCost();
  auto right_cost = right_->EstimateCost();

  auto cur_res = EstimateResult();
  return left_cost + right_cost + cur_res.GetCost();
}

pql::Estimation Join::ComputeEstResult() {
  auto left_res = left_->EstimateResult();
  auto right_res = right_->EstimateResult();

  auto col_estimates = left_res.col_unique_values;  // copy
  for (auto idx : right_ext_idx_)
    col_estimates.push_back(right_res.col_unique_values[idx]);

  auto est_rows = left_res.num_rows * right_res.num_rows;
  for (auto& [left_idx, right_idx] : join_idx_) {
    auto max_d = std::max(left_res.col_unique_values[left_idx],
                          right_res.col_unique_values[right_idx]);
    auto min_d = std::max(left_res.col_unique_values[left_idx],
                          right_res.col_unique_values[right_idx]);

    est_rows = std::ceil((double)est_rows / (double)max_d);
    col_estimates[left_idx] = min_d;
  }

  return {est_rows, col_estimates};
}

std::shared_ptr<utils::Batch> Join::Fetch() {
  auto batch = std::make_shared<utils::Batch>();

  auto left_batch = left_->Fetch();
  auto right_batch = right_->Fetch();

  auto&& left_index = BuildIndex(left_batch, true);

  for (size_t j = 0; j < right_batch->Size(); ++j) {
    auto& right_tuple = right_batch->At(j);
    auto&& keys = GetJoinKeys(right_tuple, false);

    if (!left_index.count(keys)) {
      // If there's no matching keys in the index, just continue to the next
      // iteration. Note that we use std::map::count here to avoid unnecessary
      // memory allocation.
      continue;
    }

    auto& match_left_idx = left_index[keys];
    for (const auto left_idx : match_left_idx) {
      auto& left_tuple = left_batch->At(left_idx);
      auto&& merged_tuple = Merge(left_tuple, right_tuple);
      batch->Add(std::move(merged_tuple));
    }
  }

#ifdef DEBUG_BUILD
  std::cout << "Join Result: " << *batch << std::endl;
#endif

  return batch;
}

std::map<std::vector<utils::Element>, std::vector<size_t>> Join::BuildIndex(
    std::shared_ptr<utils::Batch> batch, bool is_left) {
  std::map<std::vector<utils::Element>, std::vector<size_t>> result;

  for (size_t i = 0; i < batch->Size(); ++i) {
    auto& tuple = batch->At(i);
    auto&& keys = GetJoinKeys(tuple, is_left);

    // Note that std::map in C++ will return a default value constructed with
    // the default constructor (in this case will be an empty std::vector).
    result[keys].push_back(i);
  }

  return std::move(result);
}

std::vector<utils::Element> Join::GetJoinKeys(
    std::vector<utils::Element>& tuple, bool is_left) {
  std::vector<utils::Element> keys;
  keys.reserve(join_idx_.size());

  for (const auto [l_join_idx, r_join_idx] : join_idx_) {
    keys.push_back(tuple.at(is_left ? l_join_idx : r_join_idx));
  }

  return keys;
}

std::vector<utils::Element> Join::Merge(
    std::vector<utils::Element>& left_tuple,
    std::vector<utils::Element>& right_tuple) {
  std::vector<utils::Element> merged_tuple;

  // Take all the values from the left tuple in order.
  for (const auto& v : left_tuple) {
    merged_tuple.push_back(v);
  }

  // Take all the values from the right tuple in order except the join keys.
  size_t skipped_pivot = 0;
  for (const auto& idx : right_ext_idx_) {
    merged_tuple.push_back(right_tuple[idx]);
  }

  return merged_tuple;
}

std::ostream& Join::ToString(std::ostream& out) const {
  out << "[Join: " << *left_ << ", " << *right_ << "]";
  return out;
}

}  // namespace opr

}  // namespace spa
