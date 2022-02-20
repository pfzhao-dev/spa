#include "HeuristicPlanner.h"

#include <algorithm>

#include "opr/Join.h"
#include "opr/Project.h"
#include "opr/Scan.h"
#include "opt/BushyTreesPlanner.h"
#include "opt/GreedyPlanner.h"
#include "pql/Query.h"
#include "pql/clause/Clause.h"
#include "utils/Attribute.h"

namespace spa {

namespace opt {

std::shared_ptr<opr::Operator> HeuristicPlanner::Plan(pql::Query query,
                                                      bool strict) {
  Prepare(query);  // prepare the query

  auto [heavy_clauses, light_clauses] = SplitHeavyLight(query.GetClauses());
  auto sub_query = BuildSubQuery(query, heavy_clauses, light_clauses);

  std::shared_ptr<opr::Operator> result_op;

  if (light_clauses.size() <= kBushyTreeThreshold) {
    BushyTreesPlanner planner;
    result_op = planner.Plan(sub_query, false);
  } else {
    GreedyPlanner planner;
    result_op = planner.Plan(sub_query, false);
  }

  for (auto& clause : heavy_clauses) {
    // Guaranteed to be possible based how we split the heavy & light clauses
    result_op = std::make_shared<opr::ProjectClause>(result_op, clause);
  }

  Finalize(result_op, query, strict);
  return result_op;
}

std::pair<HeuristicPlanner::ClauseList, HeuristicPlanner::ClauseList>
HeuristicPlanner::SplitHeavyLight(ClauseList& clauses) {
  // Constraint: the attribute of the heavy clauses must be fully covered
  // by the light clauses. Hence, we need to first find the candidates
  // before performing further check.
  ClauseList cand_heavy_clauses;
  ClauseList light_clauses;

  for (auto& clause : clauses) {
    if (clause->GetPriorityScore() == 0) {
      // By default, this will be for every clauses except:
      // Affects, AffectsT, AffectsBip, AffectsBipT, NextT, NextBip, NextBipT
      //
      // However, if any of those clauses have been precomputed (e.g., due
      // to the presence of wildcard first arg), then we consider them as
      // light clause as well.
      light_clauses.push_back(clause);
    } else {
      cand_heavy_clauses.push_back(clause);
    }
  }

  // Initialize available attrs based on the light clauses
  std::unordered_set<utils::Attribute> available_attrs;
  for (auto clause : light_clauses)
    for (auto attr : clause->GetAttrs()) available_attrs.insert(attr);

  // Sort heavy candidates based on their priority score
  std::sort(cand_heavy_clauses.begin(), cand_heavy_clauses.end(),
            [](const std::shared_ptr<pql::Clause>& a,
               const std::shared_ptr<pql::Clause>& b) {
              return a->GetPriorityScore() < b->GetPriorityScore();
            });

  // Pick heavy clauses based on the candidates
  ClauseList heavy_clauses;
  for (auto clause : cand_heavy_clauses) {
    // Check whether the attributes have been covered by the light clauses.
    // If so, we can just perform a projection later.
    bool ok = true;
    for (auto attr : clause->GetAttrs())
      if (!available_attrs.count(attr)) ok = false;

    if (ok) {  // if all attrs alr available, then can consider heavy.
      heavy_clauses.push_back(clause);
    } else {  // else consider as light clause, and insert into available attrs.
      light_clauses.push_back(clause);
      for (auto attr : clause->GetAttrs()) available_attrs.insert(attr);
    }
  }

  return {heavy_clauses, light_clauses};
}

pql::Query HeuristicPlanner::BuildSubQuery(pql::Query& query,
                                           ClauseList& heavy_clauses,
                                           ClauseList& light_clauses) {
  // Retrieve the attributes from query select entities and those that
  // are required by the heavy clauses.
  std::unordered_set<utils::Attribute> heavy_or_select_attrs;
  for (auto& select_entity : query.GetSelectEntities()) {
    heavy_or_select_attrs.insert(select_entity);
  }
  for (auto& clause : heavy_clauses) {
    for (auto& attr : clause->GetAttrs()) {
      heavy_or_select_attrs.insert(attr);
    }
  }

  // Sub-query
  std::vector<std::shared_ptr<pql::Declaration>> subq_select_;
  for (auto& clause : light_clauses) {
    for (auto& attr : clause->GetAttrs()) {
      // Only return back light attrs that need to be used by either the
      // select entities or the heavy clauses.
      if (heavy_or_select_attrs.count(attr)) {
        heavy_or_select_attrs.erase(attr);
        subq_select_.push_back(attr.GetDecl());
      }
    }
  }

  auto subq_result = std::make_shared<pql::DeclaredResult>(subq_select_);
  auto sub_query = pql::Query(subq_result, light_clauses);

  return sub_query;
}

}  // namespace opt

}  // namespace spa
