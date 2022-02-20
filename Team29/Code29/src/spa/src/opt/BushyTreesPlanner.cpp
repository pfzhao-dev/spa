#include "BushyTreesPlanner.h"

#include "opr/Join.h"
#include "opr/Project.h"
#include "opr/Scan.h"
#include "opt/Exception.h"
#include "utils/Attribute.h"

#ifdef _MSC_VER
#include <intrin.h>
#define __builtin_popcount __popcnt
#define __builtin_clz __lzcnt
#endif

namespace spa {

namespace opt {

std::shared_ptr<opr::Operator> BushyTreesPlanner::Plan(pql::Query query,
                                                       bool strict) {
  Prepare(query);  // prepare the query

  auto clauses = query.GetClauses();

  if (clauses.size() > 15) {
    throw Exception("Unable to plan using bushy tree with > 15 clauses.");
  }

  // Integer should suffice since we cannot h
  int limit_mask = (1 << clauses.size());

  std::vector<std::shared_ptr<opr::Operator>> best_op;
  best_op.assign(limit_mask, {});

  // Attribute used by the query for select op
  std::unordered_set<utils::Attribute> select_attrs;
  for (auto attr : query.GetSelectEntities()) select_attrs.insert(attr);

  // Loop over all possible subsets.
  for (int mask = 1; mask < limit_mask; mask++) {
    auto on_bits = __builtin_popcount(mask);
    auto msb_idx = 31 - __builtin_clz(mask);

    if (on_bits == 1) {  // if only one clause is used, just scan clause
      best_op[mask] = std::make_shared<opr::ScanClause>(clauses[msb_idx]);
      continue;
    }

    auto& best = best_op[mask];

    for (int submask = (mask - 1) & mask;
         submask > 0 && ((submask >> msb_idx) & 1);
         submask = (submask - 1) & mask) {
      // Iterate through all possible submasks to find all possible 2-partition.
      // The submask must still share the same most significant bit since
      // the way we join table does not have any diff between L-R and R-L.

      auto merge_op = Merge(best_op[submask], best_op[mask ^ submask]);

      // NOTE: Estimate cost is cached. Hence it is fine to call it repeatedly.
      if (!best || merge_op->EstimateCost() < best->EstimateCost()) {
        best = merge_op;
      }
    }

    // Project out attributes that won't be useful anymore if necessary
    auto needed_attrs = select_attrs;
    for (size_t i = 0; i < clauses.size(); ++i) {
      if (!((mask >> i) & 1)) {  // if the clause is not part of the mask
        for (auto attr : clauses[i]->GetAttrs()) needed_attrs.insert(attr);
      }
    }
    FilterAttrs(best, needed_attrs);
  }

  // NOTE: If there's no clause, this will be an shared_ptr to null.
  auto result = best_op[limit_mask - 1];

  Finalize(result, query, strict);
  return result;
}

}  // namespace opt

}  // namespace spa
