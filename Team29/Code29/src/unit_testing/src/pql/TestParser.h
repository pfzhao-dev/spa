#pragma once

#include "catch.hpp"
#include "data/QueryDataUtil.h"

namespace test {

namespace pql {

// Test all positive cases of construction of query objects based on relation.
// get_clauses should supply clauses pairs to construct input query string and
// expected clauses
void TestParserPositive(
    std::function<std::vector<data::ClausesPair>(spa::pql::Declaration::Type)>
        get_clauses);

// Test all negative cases of construction of query objects based on relation.
// get_clauses_str should supply query string to construct input query string
void TestParserNegative(
    std::function<
        std::vector<data::StringToStringFunc>(spa::pql::Declaration::Type)>
        get_clauses_str);

}  // namespace pql

}  // namespace test
