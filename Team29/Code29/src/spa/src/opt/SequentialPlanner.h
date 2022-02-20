#pragma once

#include "QueryPlanner.h"

namespace spa {

namespace opt {

class SequentialPlanner : public QueryPlanner {
 public:
  std::shared_ptr<opr::Operator> Plan(pql::Query query,
                                      bool strict = true) override;
};

}  // namespace opt

}  // namespace spa
