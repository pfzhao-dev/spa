#pragma once

#include "utils/Batch.h"
#include "utils/Schema.h"

namespace spa {

namespace opr {

class Operator {
 public:
  enum class Type {
    kJoin,
    kProject,
    kScan,
  };

  Operator(Type type, utils::Schema schema);

  // Get the schema of the result table.
  utils::Schema& GetSchema();

  // Get the type of the operator.
  Type GetType();

  // Get the estimation cost to execute the operator. The cost will be equal
  // to the sum of expected cost from all nodes in the operator tree.
  uint64_t EstimateCost();

  // Get query estimation result.
  pql::Estimation EstimateResult();

  // Fetch the result table (i.e., execute all of the operations represented
  // by this operator).
  virtual std::shared_ptr<utils::Batch> Fetch() = 0;

  friend std::ostream& operator<<(std::ostream& out, const Operator& ref);

 protected:
  Type type_;
  utils::Schema schema_;

  // Compute the estimation cost.
  virtual uint64_t ComputeEstCost() = 0;

  // Compute the estimation result.
  virtual pql::Estimation ComputeEstResult() = 0;

  virtual std::ostream& ToString(std::ostream& out) const = 0;

  bool has_est_cost_cache_ = false;
  uint64_t est_cost_cache_;

  bool has_est_result_cache_ = false;
  pql::Estimation est_result_cache_;
};

}  // namespace opr

}  // namespace spa
