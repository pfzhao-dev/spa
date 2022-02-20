#include "Project.h"

#include <cmath>
#include <cstdio>
#include <iostream>
#include <variant>

#include "pql/Evaluator.h"
#include "utils/Element.h"
#include "utils/Schema.h"

namespace spa {

namespace opr {

Project::Project(std::shared_ptr<Operator> base, utils::Schema schema)
    : Operator(Operator::Type::kProject, schema), base_(base) {}

uint64_t Project::ComputeEstCost() {
  // NOTE: In terms of computation cost, we consider Project to not give any
  //       cost since it is just filtering unused rows/cols. This is mainly
  //       to avoid penalizing such operation that could be beneficial in the
  //       long run of the query evaluation.
  return base_->EstimateCost();
}

pql::Estimation ProjectAttr::ComputeEstResult() {
  auto base_result = base_->EstimateResult();

  std::vector<pql::EstimationValue> col_unique_values;
  pql::EstimationValue max_rows = 1;

  for (auto idx : attrs_idx_) {
    col_unique_values.push_back(base_result.col_unique_values[idx]);
    max_rows *= base_result.col_unique_values[idx];
  }

  // should not exceed the theoretical max value.
  auto est_rows = std::min(base_result.num_rows, max_rows);

  return {est_rows, col_unique_values};
}

ProjectAttr::ProjectAttr(std::shared_ptr<Operator> base,
                         std::vector<utils::Attribute> attrs,
                         bool eval_attr_value)
    : Project(base, utils::Schema::ProjectAttr(base->GetSchema(), attrs)),
      attrs_(attrs),
      eval_attr_value_(eval_attr_value) {
  auto base_schema = base->GetSchema();
  for (const auto& attr : attrs_) {
    attrs_idx_.push_back(base_schema.GetIdx(attr));
  }

  if (eval_attr_value_) {
    // Note that the evaluation here is always in the form of converting
    // a statement (call/read/print) to its related var name.
    requires_eval_idx_.reserve(attrs_.size());
    for (const auto& attr : attrs_) {
      auto decl = attr.GetDecl();
      using DType = pql::Declaration::Type;
      using DValType = pql::Declaration::ValueType;
      if (decl->IsTypeIn({DType::kCall, DType::kPrint, DType::kRead}) &&
          decl->HasValue() && decl->GetValueType() != DValType::kStmtNum) {
        requires_eval_idx_.push_back(true);
      } else {
        requires_eval_idx_.push_back(false);
      }
    }
  }
}

std::shared_ptr<utils::Batch> ProjectAttr::Fetch() {
  // If we project by attribute, we are also expected to check for uniqueness
  // of the new batch.
  auto batch = std::make_shared<utils::Batch>(true);
  auto base_batch = base_->Fetch();

  for (size_t i = 0; i < base_batch->Size(); ++i) {
    auto& base_tuple = base_batch->At(i);
    auto new_tuple = ProjectTuple(base_tuple);
    batch->Add(std::move(new_tuple));
  }

#ifdef DEBUG_BUILD
  std::cout << "ProjectAttr Result: " << *batch << std::endl;
#endif
  return batch;
}

std::vector<utils::Element> ProjectAttr::ProjectTuple(
    std::vector<utils::Element>& base) {
  // Construct the projected tuple
  std::vector<utils::Element> new_tuple;
  for (size_t i = 0; i < attrs_idx_.size(); ++i) {
    auto& idx = attrs_idx_[i];
    if (!eval_attr_value_ || (eval_attr_value_ && !requires_eval_idx_[i])) {
      new_tuple.push_back(base[idx]);
    } else {
      auto& evaluator = pql::Evaluator::GetInstance();
      auto stmt_num = std::get<uint32_t>(base[idx]);
      new_tuple.push_back(evaluator.GetAttrByStmt(stmt_num));
    }
  }

  return std::move(new_tuple);
}

ProjectClause::ProjectClause(std::shared_ptr<Operator> base,
                             std::shared_ptr<pql::Clause> clause)
    : Project(base, utils::Schema::ProjectClause(base->GetSchema(), clause)),
      clause_(clause) {
  const auto clause_attrs = clause->GetAttrs();
  for (const auto& clause_attr : clause_attrs) {
    clause_attrs_idx_.push_back(schema_.GetIdx(clause_attr));
  }
}

pql::Estimation ProjectClause::ComputeEstResult() {
  auto result = base_->EstimateResult();  // baseline result

  if (clause_->GetPriorityScore() > 0) {
    // This means that the clause is not precomputed.
    // In this case, we should not try to access the clause estimation
    // which could trigger a precomputation.
    return result;  // just return the expected result as it is.
  }

  auto clause_est = clause_->GetEstimation();

  for (size_t i = 0; i < clause_attrs_idx_.size(); ++i) {
    auto& base_val = result.col_unique_values[clause_attrs_idx_[i]];

    // Minimize based on the expected distinct values.
    base_val = std::min(base_val, clause_est.col_unique_values[i]);
  }

  pql::EstimationValue max_rows = 1;
  for (auto& v : result.col_unique_values) max_rows *= v;

  // potentially update theoretical max number of rows.
  result.num_rows = std::min(result.num_rows, max_rows);

  return result;
}

std::shared_ptr<utils::Batch> ProjectClause::Fetch() {
  auto base_batch = base_->Fetch();

  auto batch = std::make_shared<utils::Batch>();
  for (size_t i = 0; i < base_batch->Size(); ++i) {
    auto& base_tuple = base_batch->At(i);
    if (clause_->Test(base_tuple, clause_attrs_idx_)) {
      batch->Add(std::move(base_tuple));
    }
  }

#ifdef DEBUG_BUILD
  std::cout << "ProjectClause Result: " << *batch << std::endl;
#endif

  return batch;
}

std::ostream& ProjectAttr::ToString(std::ostream& out) const {
  out << "[ProjectAttr: <";
  for (size_t i = 0; i < attrs_.size(); ++i) {
    if (i > 0) out << ", ";
    out << attrs_[i];
  }
  out << ">, "
      << "eval: " << eval_attr_value_ << ", base: " << *base_;

  return out;
}

std::ostream& ProjectClause::ToString(std::ostream& out) const {
  out << "[ProjectClause: " << *clause_ << ", base: " << *base_ << "]";
  return out;
}

}  // namespace opr

}  // namespace spa
