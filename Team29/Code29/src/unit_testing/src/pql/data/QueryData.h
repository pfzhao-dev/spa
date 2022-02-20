#pragma once

#include "QueryDataUtil.h"
#include "catch.hpp"
#include "pql/Declaration.h"

namespace test {

namespace pql {

namespace data {

// returns the clauses pairs based on declaration type for relation + pattern
// positive test cases
extern const std::vector<ClausesPair> kGetClausesPositive(
    spa::pql::Declaration::Type declaration_type);

// returns query strings based on declaration type for relation + pattern
// negative test cases
extern const std::vector<StringToStringFunc> kGetFuncsNegative(
    spa::pql::Declaration::Type declaration_type);

}  // namespace data

}  // namespace pql

}  // namespace test
