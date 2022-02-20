#include "Operator.h"

#include <iostream>

namespace spa {

namespace opr {

Operator::Operator(Type type, utils::Schema schema)
    : type_(type), schema_(schema) {}

std::ostream& operator<<(std::ostream& out, const Operator& op) {
  return op.ToString(out);
}

Operator::Type Operator::GetType() { return type_; }

utils::Schema& Operator::GetSchema() { return schema_; }

uint64_t Operator::EstimateCost() {
  if (has_est_cost_cache_) {
    return est_cost_cache_;
  }

  has_est_cost_cache_ = true;
  est_cost_cache_ = ComputeEstCost();
  return est_cost_cache_;
}

pql::Estimation Operator::EstimateResult() {
  if (has_est_result_cache_) {
    return est_result_cache_;
  }

  has_est_result_cache_ = true;
  est_result_cache_ = ComputeEstResult();
  return est_result_cache_;
}

}  // namespace opr

}  // namespace spa
