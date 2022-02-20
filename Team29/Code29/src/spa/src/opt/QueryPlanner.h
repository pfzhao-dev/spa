#pragma once

#include <memory>
#include <unordered_set>
#include <vector>

#include "opr/Operator.h"
#include "pql/Query.h"

namespace spa {

namespace opt {

class QueryPlanner {
 public:
  // Creates an operator plan to execute a particular query.
  //
  // If strict is true, the result will exactly follow the query select
  // entities.
  //
  // Otherwise, the result may be in any order, remove duplicate entries,
  // and not evaluate the translation to attr. For example, if the select
  // entities are "<s1, p.varName, s1, s2>", the result schema attrs may be
  // "<s1, p, s2>". This "not-strict" mode is useful when the planner
  // is used by another planner which will do the strict translation there.
  virtual std::shared_ptr<opr::Operator> Plan(pql::Query query,
                                              bool strict = true) = 0;

 protected:
  // Prepare query.
  void Prepare(pql::Query& query);

  // Finalize query operator.
  // This function will scan attrs that occur in the select entities but not
  // present in the current operator. It will also perform the necessary
  // projection or convertion into boolean depending on the select entities.
  void Finalize(std::shared_ptr<opr::Operator>& op, pql::Query& query,
                bool strict = true);

  // Merge two operators together.
  //
  // The function will try to perform clause projection from one to another
  // if possible (due to fully overlap attrs). Otherwise, it will just join
  // the clauses.
  std::shared_ptr<opr::Operator> Merge(std::shared_ptr<opr::Operator> lhs,
                                       std::shared_ptr<opr::Operator> rhs);

  // Convert an operator into clause if possible. Otherwise will return
  // a shared_ptr of null.
  std::shared_ptr<pql::Clause> ConvertibleToClause(
      std::shared_ptr<opr::Operator> op);

  // This function will scan the select attrs that have not been fetched by the
  // given operator and join them together. If the given operator has already
  // fetched all the select attrs, this function is a no-op.
  void HandleSelectAttrs(std::shared_ptr<opr::Operator>& op,
                         std::vector<utils::Attribute>& select_attrs);

  // Helper function to potentially filter out unnecessary attrs from op schema.
  // If the schema only contains all the required attrs, this function does
  // not change anything.
  void FilterAttrs(std::shared_ptr<opr::Operator>& op,
                   std::unordered_set<utils::Attribute>& required_attrs);
};

}  // namespace opt

}  // namespace spa
