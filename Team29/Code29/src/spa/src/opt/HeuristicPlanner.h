#pragma once

#include "QueryPlanner.h"

namespace spa {

namespace opt {

class HeuristicPlanner : public QueryPlanner {
 public:
  std::shared_ptr<opr::Operator> Plan(pql::Query query,
                                      bool strict = true) override;

 private:
  // Threshold of the number of clauses to process subquery with bushy tree.
  const static int kBushyTreeThreshold = 7;

  typedef std::vector<std::shared_ptr<pql::Clause>> ClauseList;

  std::pair<ClauseList, ClauseList> SplitHeavyLight(ClauseList& clauses);

  pql::Query BuildSubQuery(pql::Query& query, ClauseList& heavy_clauses,
                           ClauseList& light_clauses);
};

}  // namespace opt

}  // namespace spa
