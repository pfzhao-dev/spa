#include "TestResult.h"

#include "TestRef.h"
#include "catch.hpp"
#include "pql/QueryException.h"

using namespace test::pql::result;

TEST_CASE("Test constructor of DeclaredResult", "[pql][declaredresult]") {
  SECTION("Positive") {
    INFO("Declaration has a name");
    REQUIRE_NOTHROW(DeclaredResult({test::pql::ref::kDeclPtrStmtStmtName}));
  }
  SECTION("Negative") {
    INFO("Declaration does not have a name");
    REQUIRE_THROWS_AS(DeclaredResult({test::pql::ref::kDeclPtrVarNoName}),
                      QueryParseException);
  }
}

TEST_CASE("Test == of Result", "[pql][result][operator==]") {
  SECTION("NEGATIVE") {
    REQUIRE_FALSE(kBoolResult == kDeclResultConstConstName);
  }
}

TEST_CASE("Test != of Result", "[pql][result][operator==]") {
  SECTION("POSITIVE") { REQUIRE(kBoolResult != kDeclResultConstConstName); }
}

TEST_CASE("Test == of DeclaredResult",
          "[pql][result][declaredresult][operator==]") {
  SECTION("Positive") {
    REQUIRE(kDeclResultStmtStmtName == DeclaredResult(kDeclPtrVecStmtStmtName));

    REQUIRE(kDeclResultStmtStmtNameConstConstName ==
            DeclaredResult(kDeclPtrVecStmtStmtNameConstConstName));
  }

  SECTION("Negative") {
    INFO("Reordered declarations");
    REQUIRE_FALSE(kDeclResultMultiple == kDeclResultMultipleReordered);

    INFO("Declarations of different length");
    REQUIRE_FALSE(kDeclResultMultiple == kDeclResultStmtStmtNameConstConstName);
  }
}

TEST_CASE("Test != of DeclaredResult",
          "[pql][result][declaredresult][operator!=]") {
  SECTION("Positive") {
    INFO("Reordered declarations");
    REQUIRE(kDeclResultMultiple != kDeclResultMultipleReordered);

    INFO("Declarations of different length");
    REQUIRE(kDeclResultMultiple != kDeclResultStmtStmtNameConstConstName);
  }

  SECTION("Negative") {
    REQUIRE_FALSE(kDeclResultStmtStmtName !=
                  DeclaredResult(kDeclPtrVecStmtStmtName));

    REQUIRE_FALSE(kDeclResultStmtStmtNameConstConstName !=
                  DeclaredResult(kDeclPtrVecStmtStmtNameConstConstName));
  }
}

TEST_CASE("Test == of BooleanResult",
          "[pql][result][booleanresult][operator==]") {
  SECTION("Positive") { REQUIRE(kBoolResult == BooleanResult()); }
}

TEST_CASE("Test != of BooleanResult",
          "[pql][result][booleanresult][operator!=]") {
  SECTION("Negative") { REQUIRE_FALSE(kBoolResult != BooleanResult()); }
}
