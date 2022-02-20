#include "Query.h"

#include <algorithm>
#include <iostream>
#include <string>

#include "Comparators.h"
#include "QueryException.h"

namespace spa {

namespace pql {

Query::Query(std::shared_ptr<Result> result,
             std::vector<std::shared_ptr<Clause>> clauses)
    : result_{result}, clauses_{clauses} {};

bool Query::operator==(const Query& other) const {
  // check whether declaration is equal
  PtrComparator<Result> result_comparator;
  if (!result_comparator(this->result_, other.result_)) return false;

  // check whether vector of clauses are equal
  return std::is_permutation(this->clauses_.begin(), this->clauses_.end(),
                             other.clauses_.begin(), other.clauses_.end(),
                             PtrComparator<Clause>());
}

bool Query::operator!=(const Query& other) const { return !(*this == other); }

std::ostream& operator<<(std::ostream& out, const Query& query) {
  out << *query.result_ << " Clauses: {";
  for (std::shared_ptr<Clause> clause_ptr : query.clauses_) {
    out << *clause_ptr;
  }

  out << "}";
  return out;
}

std::vector<std::shared_ptr<Clause>>& Query::GetClauses() { return clauses_; }

std::vector<std::shared_ptr<Declaration>> Query::GetSelectEntities() {
  return result_->GetSelectEntities();
}

}  // namespace pql

}  // namespace spa
