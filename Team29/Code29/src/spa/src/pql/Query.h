#pragma once

#include <vector>

#include "Result.h"
#include "pql/clause/Clause.h"

namespace spa {

namespace pql {

class Query {
 public:
  Query(std::shared_ptr<Result> result,
        std::vector<std::shared_ptr<Clause>> clauses);

  bool operator==(const Query& other) const;
  bool operator!=(const Query& other) const;

  friend std::ostream& operator<<(std::ostream& out, const Query& ref);

  std::vector<std::shared_ptr<Clause>>& GetClauses();

  std::vector<std::shared_ptr<Declaration>> GetSelectEntities();

 private:
  std::shared_ptr<Result> result_;
  std::vector<std::shared_ptr<Clause>> clauses_;
};

}  // namespace pql

}  // namespace spa
