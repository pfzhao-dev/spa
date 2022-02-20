#include "TestParser.h"
#include "catch.hpp"
#include "data/clause/CompareData.h"

using namespace test::pql;

TEST_CASE("Test parsing of with clauses", "[pql][parser][compare]") {
  SECTION("Positive") {
    TestParserPositive(data::compare::kGetClausesPositive);
  }
  SECTION("Negative") { TestParserNegative(data::compare::kGetFuncsNegative); }
}
