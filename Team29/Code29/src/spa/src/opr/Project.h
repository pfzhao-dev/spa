#pragma once

#include <vector>

#include "opr/Operator.h"
#include "pql/clause/Clause.h"
#include "utils/Attribute.h"

namespace spa {

namespace opr {

class Project : public Operator {
 public:
  Project(std::shared_ptr<Operator> base, utils::Schema schema);

 protected:
  std::shared_ptr<Operator> base_;

  // Compute the estimation cost.
  uint64_t ComputeEstCost() override;
};

class ProjectAttr : public Project {
 public:
  //
  ProjectAttr(std::shared_ptr<Operator> base,
              std::vector<utils::Attribute> attrs,
              bool eval_attr_value = false);

  std::shared_ptr<utils::Batch> Fetch() override;

 protected:
  // Compute the estimation result.
  pql::Estimation ComputeEstResult() override;

  std::ostream& ToString(std::ostream& out) const override;

 private:
  std::vector<size_t> attrs_idx_;

  std::vector<utils::Attribute> attrs_;

  bool eval_attr_value_;
  std::vector<bool> requires_eval_idx_;

  std::vector<utils::Element> ProjectTuple(std::vector<utils::Element>& base);
};

// TODO: Re-evaluate the design whether having a projection by clause is
//       feasible. The problem is that when there is a wildcard entity in the
//       clause, things become complicated since we will need to handle
//       many potential cases as compared to just fetching the clause table
//       and perform a join operation.
//
// NOTE: This class should not be used since the undelying Clause::Test
//       function is still not properly implemented.
class ProjectClause : public Project {
 public:
  ProjectClause(std::shared_ptr<Operator> base,
                std::shared_ptr<pql::Clause> clause);

  std::shared_ptr<utils::Batch> Fetch() override;

 protected:
  // Compute the estimation result.
  pql::Estimation ComputeEstResult() override;

  std::ostream& ToString(std::ostream& out) const override;

 private:
  std::vector<size_t> clause_attrs_idx_;

  std::shared_ptr<pql::Clause> clause_;
};

}  // namespace opr

}  // namespace spa
