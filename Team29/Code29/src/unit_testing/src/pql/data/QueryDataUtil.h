#pragma once

#include "catch.hpp"
#include "pql/clause/Affects.h"
#include "pql/clause/Calls.h"
#include "pql/clause/Clause.h"
#include "pql/clause/Compare.h"
#include "pql/clause/Follows.h"
#include "pql/clause/Modifies.h"
#include "pql/clause/Next.h"
#include "pql/clause/Parent.h"
#include "pql/clause/Pattern.h"
#include "pql/clause/Uses.h"

namespace test {

namespace pql {

namespace data {

using namespace spa::pql;

using StringToStringFunc = std::function<std::string(std::string)>;

using DeclPtrToClausePtrVecFunc =
    std::function<std::vector<std::shared_ptr<Clause>>(
        std::shared_ptr<Declaration>)>;

StringToStringFunc CombineStrFunc(StringToStringFunc func1,
                                  StringToStringFunc func2);

template <typename T>
std::vector<T> CombineVectors(std::vector<T> v1, std::vector<T> v2) {
  std::vector<T> new_vector;
  new_vector.reserve(v1.size() + v2.size());
  new_vector.insert(new_vector.end(), v1.begin(), v1.end());
  new_vector.insert(new_vector.end(), v2.begin(), v2.end());
  return new_vector;
}

class ClausesPair {
 public:
  ClausesPair(StringToStringFunc string_func,
              DeclPtrToClausePtrVecFunc clauses_func);

  // function that constructs query string
  StringToStringFunc string_func_;
  // function that constructs the expected clauses
  DeclPtrToClausePtrVecFunc clauses_func_;

  static ClausesPair Combine(const ClausesPair& cp1, const ClausesPair& cp2);
};

const std::string kWildcardStr = "_";
const std::string kEmptyStr = "";

}  // namespace data

}  // namespace pql

}  // namespace test
