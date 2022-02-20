#include "QueryPlanner.h"

#include <memory>

#include "opr/Join.h"
#include "opr/Project.h"
#include "opr/Scan.h"

namespace spa {

namespace opt {

void QueryPlanner::Prepare(pql::Query& query) {
  for (auto& clause : query.GetClauses()) {
    // Prepare the clause evaluation. Some clauses like Affects/Bip/* and
    // Next/Bip/* may want to do some pre-computation on the query evaluation
    // itself.
    clause->PrepareEval();
  }
}

void QueryPlanner::Finalize(std::shared_ptr<opr::Operator>& op,
                            pql::Query& query, bool strict) {
  // Scan select attributes that have not been fetched.
  std::vector<utils::Attribute> attrs_vec;
  for (auto& attr : query.GetSelectEntities()) attrs_vec.push_back(attr);
  HandleSelectAttrs(op, attrs_vec);

  if (!op) {
    op = std::make_shared<opr::ScanBoolean>();
  }

  // Fetch result according to the select entities in strict mode.
  if (strict) {
    op = std::make_shared<opr::ProjectAttr>(op, attrs_vec, true);
  }
}

std::shared_ptr<opr::Operator> QueryPlanner::Merge(
    std::shared_ptr<opr::Operator> lhs, std::shared_ptr<opr::Operator> rhs) {
  auto& lhs_schema = lhs->GetSchema();
  auto& rhs_schema = rhs->GetSchema();

  if (lhs_schema.HasAttrs(rhs_schema.GetAttrs())) {
    auto clause = ConvertibleToClause(rhs);
    if (clause) {
      return std::make_shared<opr::ProjectClause>(lhs, clause);
    }

  } else if (rhs_schema.HasAttrs(lhs_schema.GetAttrs())) {
    auto clause = ConvertibleToClause(lhs);
    if (clause) {
      return std::make_shared<opr::ProjectClause>(rhs, clause);
    }
  }

  // Easiest way is to just join them together
  return std::make_shared<opr::Join>(lhs, rhs);
}

std::shared_ptr<pql::Clause> QueryPlanner::ConvertibleToClause(
    std::shared_ptr<opr::Operator> op) {
  // Check if the rhs_schema is a scan clause. If so, can just project.
  if (op->GetType() == opr::Operator::Type::kScan) {
    auto scan_op = std::static_pointer_cast<opr::Scan>(op);
    if (scan_op->GetScType() == opr::Scan::ScType::kClause) {
      auto scan_clause_op = std::static_pointer_cast<opr::ScanClause>(op);
      return scan_clause_op->GetClause();
    }
  }
  return {};  // null
}

void QueryPlanner::HandleSelectAttrs(
    std::shared_ptr<opr::Operator>& op,
    std::vector<utils::Attribute>& select_attrs) {
  // Fetch select entities that have not been fetched.
  for (auto attr : select_attrs) {
    if (op) {  // only check if the op is not an empty ptr
      auto& op_schema = op->GetSchema();
      if (op_schema.HasAttr(attr)) continue;
    }

    auto scan_op = std::make_shared<opr::ScanAttribute>(attr);
    if (!op) {
      op = scan_op;
    } else {
      op = std::make_shared<opr::Join>(scan_op, op);
    }
  }
}

void QueryPlanner::FilterAttrs(
    std::shared_ptr<opr::Operator>& op,
    std::unordered_set<utils::Attribute>& required_attrs) {
  auto& schema = op->GetSchema();
  auto& attrs = schema.GetAttrs();

  // Find out the list of attrs needed from the current operator.
  std::vector<utils::Attribute> p_attrs;
  for (auto& attr : attrs)
    if (required_attrs.count(attr)) {
      required_attrs.erase(attr);
      p_attrs.push_back(attr);
    }

  // NOTE: We assume that the schema attr does not have any flaw. Hence,
  //       comparing the size is enough to determine whether the attr sets
  //       are equal or not.
  if (p_attrs.size() < attrs.size()) {
    op = std::make_shared<opr::ProjectAttr>(op, p_attrs);
  }
}

}  // namespace opt

}  // namespace spa
