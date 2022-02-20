#include "SequentialPlanner.h"

#include <cstdio>

#include "opr/Join.h"
#include "opr/Project.h"
#include "opr/Scan.h"
#include "utils/Attribute.h"

namespace spa {

namespace opt {

std::shared_ptr<opr::Operator> SequentialPlanner::Plan(pql::Query query,
                                                       bool strict) {
  Prepare(query);  // prepare the query

  const auto clauses = query.GetClauses();
  std::shared_ptr<opr::Operator> root_op;

  // Fetch and merge clause tables first.
  for (size_t i = 0; i < clauses.size(); ++i) {
    const auto scan_op = std::make_shared<opr::ScanClause>(clauses[i]);

    if (!root_op) {
      root_op = scan_op;
    } else {
      root_op = Merge(root_op, scan_op);
    }
  }

  Finalize(root_op, query, strict);
  return root_op;
}

}  // namespace opt

}  // namespace spa
