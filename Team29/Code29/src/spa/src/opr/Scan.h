#pragma once

#include <memory>
#include <string>

#include "opr/Operator.h"
#include "pql/clause/Clause.h"
#include "utils/Attribute.h"

namespace spa {

namespace opr {

class Scan : public Operator {
 public:
  enum class ScType {
    kAttr,
    kBoolean,
    kClause,
  };

  Scan(ScType type, utils::Schema schema);

  ScType GetScType();

 private:
  ScType sc_type_;
};

class ScanAttribute : public Scan {
 public:
  ScanAttribute(utils::Attribute attr);

  std::shared_ptr<utils::Batch> Fetch() override;

 protected:
  // Compute the estimation cost.
  uint64_t ComputeEstCost() override;

  // Compute the estimation result.
  pql::Estimation ComputeEstResult() override;

  std::ostream& ToString(std::ostream& out) const override;

 private:
  utils::Attribute attr_;
};

class ScanBoolean : public Scan {
 public:
  ScanBoolean();

  std::shared_ptr<utils::Batch> Fetch() override;

 protected:
  // Compute the estimation cost.
  uint64_t ComputeEstCost() override;

  // Compute the estimation result.
  pql::Estimation ComputeEstResult() override;

  std::ostream& ToString(std::ostream& out) const override;
};

class ScanClause : public Scan {
 public:
  ScanClause(std::shared_ptr<pql::Clause> clause);

  std::shared_ptr<utils::Batch> Fetch() override;

  std::shared_ptr<pql::Clause> GetClause();

 protected:
  // Compute the estimation cost.
  uint64_t ComputeEstCost() override;

  // Compute the estimation result.
  pql::Estimation ComputeEstResult() override;

  std::ostream& ToString(std::ostream& out) const override;

 private:
  std::shared_ptr<pql::Clause> clause_;
};

}  // namespace opr

}  // namespace spa
