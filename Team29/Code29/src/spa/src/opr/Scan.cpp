#include "Scan.h"

#include <cstdio>
#include <iostream>
#include <variant>

#include "pql/Declaration.h"
#include "pql/Evaluator.h"
#include "utils/Batch.h"
#include "utils/Element.h"
#include "utils/Translator.h"

namespace spa {

namespace opr {

Scan::Scan(ScType type, utils::Schema schema)
    : Operator(Operator::Type::kScan, schema), sc_type_{type} {}

Scan::ScType Scan::GetScType() { return sc_type_; }

ScanAttribute::ScanAttribute(utils::Attribute attr)
    : attr_(attr), Scan(ScType::kAttr, utils::Schema(attr)) {}

ScanBoolean::ScanBoolean() : Scan(ScType::kBoolean, utils::Schema()) {}

ScanClause::ScanClause(std::shared_ptr<pql::Clause> clause)
    : clause_(clause), Scan(ScType::kClause, clause->GetAttrs()) {}

std::shared_ptr<pql::Clause> ScanClause::GetClause() { return clause_; }

uint64_t ScanAttribute::ComputeEstCost() {
  auto est_result = EstimateResult();
  return est_result.GetCost();
}

pql::Estimation ScanAttribute::ComputeEstResult() {
  const auto type = attr_.GetType();
  auto& evaluator = pql::Evaluator::GetInstance();

  pql::EstimationValue est_rows;

  if (type == pql::Declaration::Type::kVariable) {
    auto& vars = evaluator.GetVariables();
    est_rows = vars.size();

  } else if (type == pql::Declaration::Type::kConstant) {
    auto& const_values = evaluator.GetConstants();
    est_rows = const_values.size();

  } else if (type == pql::Declaration::Type::kProcedure) {
    auto& proc_names = evaluator.GetProcedures();
    est_rows = proc_names.size();

  } else {
    auto& stmts = evaluator.GetStatements(type);
    est_rows = stmts.size();
  }

  return {est_rows, {est_rows}};
}

uint64_t ScanBoolean::ComputeEstCost() { return 0; }

pql::Estimation ScanBoolean::ComputeEstResult() { return {1, {}}; }

uint64_t ScanClause::ComputeEstCost() {
  auto est_result = EstimateResult();
  return est_result.GetCost();
}

pql::Estimation ScanClause::ComputeEstResult() {
  // NOTE: Be careful upon calling this, because for some clause like Affects
  //       this may trigger a precomputation.
  return clause_->GetEstimation();
}

std::shared_ptr<utils::Batch> ScanClause::Fetch() {
  auto batch = clause_->Fetch();

#ifdef DEBUG_BUILD
  std::cout << "ScanClause " << *clause_ << "Result: " << *batch << std::endl;
#endif

  return batch;
}

std::shared_ptr<utils::Batch> ScanBoolean::Fetch() {
  auto batch = std::make_shared<utils::Batch>();

  // Add an empty vector to represents a boolean true.
  std::vector<utils::Element> tuple;
  batch->Add(std::move(tuple));

#ifdef DEBUG_BUILD
  std::cout << "ScanBoolean Result: " << *batch << std::endl;
#endif

  return batch;
}

std::shared_ptr<utils::Batch> ScanAttribute::Fetch() {
  const auto type = attr_.GetType();

  auto& evaluator = pql::Evaluator::GetInstance();
  std::shared_ptr<utils::Batch> batch;

  if (type == pql::Declaration::Type::kVariable) {
    batch = std::make_shared<utils::Batch>(evaluator.GetVariables());

  } else if (type == pql::Declaration::Type::kConstant) {
    batch = std::make_shared<utils::Batch>(evaluator.GetConstants());

  } else if (type == pql::Declaration::Type::kProcedure) {
    batch = std::make_shared<utils::Batch>(evaluator.GetProcedures());

  } else {
    batch = std::make_shared<utils::Batch>(evaluator.GetStatements(type));
  }

#ifdef DEBUG_BUILD
  std::cout << "ScanAttribute " << attr_ << "Result: " << *batch << std::endl;
#endif

  return batch;
}

std::ostream& ScanAttribute::ToString(std::ostream& out) const {
  out << "[ScanAttribute: " << attr_ << "]";
  return out;
}

std::ostream& ScanBoolean::ToString(std::ostream& out) const {
  out << "[ScanBoolean ()]";
  return out;
}

std::ostream& ScanClause::ToString(std::ostream& out) const {
  out << "[ScanClause: " << *clause_ << "]";
  return out;
}

}  // namespace opr

}  // namespace spa
