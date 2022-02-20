#pragma once

#include <memory>
#include <vector>

#include "opt/QueryPlanner.h"

namespace spa {

namespace opt {

class GreedyPlanner : public QueryPlanner {
 public:
  std::shared_ptr<opr::Operator> Plan(pql::Query query,
                                      bool strict = true) override;

 private:
  // Get the best pair that minimize the total cost after being merged
  // among the given operators.
  std::pair<size_t, size_t> GetBestPair(
      std::vector<std::shared_ptr<opr::Operator>>& oprs);
};

}  // namespace opt

}  // namespace spa
