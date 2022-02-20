#pragma once

#include <unordered_set>

#include "opr/Operator.h"
#include "opt/QueryPlanner.h"
#include "utils/Attribute.h"

namespace spa {

namespace opt {

class BushyTreesPlanner : public QueryPlanner {
 public:
  std::shared_ptr<opr::Operator> Plan(pql::Query query,
                                      bool strict = true) override;
};

}  // namespace opt

}  // namespace spa
