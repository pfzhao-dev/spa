#include "GreedyPlanner.h"

#include "opr/Join.h"
#include "opr/Project.h"
#include "opr/Scan.h"
#include "opt/Exception.h"
#include "utils/Attribute.h"

namespace spa {

namespace opt {

std::shared_ptr<opr::Operator> GreedyPlanner::Plan(pql::Query query,
                                                   bool strict) {
  Prepare(query);  // prepare the query

  auto clauses = query.GetClauses();

  std::shared_ptr<opr::Operator> result_op;

  // Attribute used by the query for select op
  std::unordered_set<utils::Attribute> select_attrs;
  for (auto attr : query.GetSelectEntities()) select_attrs.insert(attr);

  // Our goal is to merge all the operators together. Initially they only
  // consist of scan clause operators. However, note that we may convert some
  // into the ProjectClause later if possible.
  std::vector<std::shared_ptr<opr::Operator>> oprs;
  for (auto clause : clauses)
    oprs.push_back(std::make_shared<opr::ScanClause>(clause));

  if (oprs.size() == 1) {
    // If there is only one clause, then we can just scan it directly.
    result_op = oprs[0];
  } else if (oprs.size() > 1) {
    // The greedy algortihm will join all of the clauses in (N - 1) rounds.
    // In each round, it will pick the two pair of intermediary results
    // that create the smallest change in total cost.

    auto num_rounds = clauses.size() - 1;

    for (int r = 0; r < num_rounds; ++r) {
      auto [i, j] = GetBestPair(oprs);
      auto new_op = Merge(oprs[i], oprs[j]);

      // Remove the two operators
      if (i > j) std::swap(i, j);
      oprs.erase(oprs.begin() + j);
      oprs.erase(oprs.begin() + i);

      // Project out attributes that won't be useful anymore if necessary
      auto needed_attrs = select_attrs;
      for (auto op : oprs) {
        auto& schema = op->GetSchema();
        for (auto attr : schema.GetAttrs()) needed_attrs.insert(attr);
      }
      FilterAttrs(new_op, needed_attrs);

      oprs.push_back(new_op);
    }

    result_op = oprs[0];
  } else {
    // Do nothing, and the result_op will still be an empty ptr
  }

  Finalize(result_op, query, strict);
  return result_op;
}

std::pair<size_t, size_t> GreedyPlanner::GetBestPair(
    std::vector<std::shared_ptr<opr::Operator>>& oprs) {
  if (oprs.size() < 2) {
    throw Exception("Unable to start a greedy round with less than 2 oprs");
  }

  bool has_best = false;
  size_t best_i;
  size_t best_j;
  // We need int64_t since the diff may be negative if we change a ScanClause
  // to ProjectClause.
  int64_t best_diff;

  for (size_t i = 0; i < oprs.size(); ++i) {
    for (size_t j = i + 1; j < oprs.size(); ++j) {
      int64_t old_cost = oprs[i]->EstimateCost() + oprs[j]->EstimateCost();

      auto new_op = Merge(oprs[i], oprs[j]);
      int64_t new_cost = new_op->EstimateCost();

      int64_t cost_diff = new_cost - old_cost;

      if (!has_best) {
        has_best = true;
        best_i = i;
        best_j = j;
        best_diff = cost_diff;

      } else if (cost_diff < best_diff) {  // found cheaper merging
        best_i = i;
        best_j = j;
        best_diff = cost_diff;
      }
    }
  }

  return {best_i, best_j};
}

}  // namespace opt

}  // namespace spa
