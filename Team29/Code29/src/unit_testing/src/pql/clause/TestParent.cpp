#include "TestParent.h"

#include "../data/QueryDataUtil.h"
#include "catch.hpp"
#include "pql/QueryException.h"

using namespace test::pql::clause;

TEST_CASE("Test constructor of Parent, [pql][parent]") {
  SECTION("Positive") {
    INFO("Pred and Succ is non-empty");
    REQUIRE_NOTHROW(Parent(kStmtRefPtrStmtNo2, kStmtRefPtrWild));
  }
  SECTION("Negative") {
    INFO("Pred is empty");
    REQUIRE_THROWS_AS(Parent(kStmtRefPtrEmpty, kStmtRefPtrStmtNo2),
                      QueryParseException);

    INFO("Succ is empty");
    REQUIRE_THROWS_AS(Parent(kStmtRefPtrStmtNo2, kStmtRefPtrEmpty),
                      QueryParseException);
  }
}

TEST_CASE("Test == of Parent", "[pql][parent][operator==]") {
  SECTION("Positive") {
    INFO("Same Transitive, Pred and Succ");
    INFO("Same Pred and Succ pointers");
    REQUIRE(kParentStmt2Wild == Parent(kStmtRefPtrStmtNo2, kStmtRefPtrWild));
    REQUIRE(kParentTStmt2StmtName ==
            Parent(kStmtRefPtrStmtNo2, kStmtRefPtrDeclStmtStmtName, true));

    INFO("Different Pred and Succ pointers, same contents");
    REQUIRE(kParentStmt2Wild ==
            Parent(kStmtRefPtrStmtNo2Duplicate, kStmtRefPtrWildDuplicate));
  }

  SECTION("Negative") {
    INFO("Different Transitive, Same Pred and Succ");
    REQUIRE_FALSE(kParentStmt2Wild == kParentTStmt2Wild);

    INFO("Different Pred, Same Transitive and Succ");
    REQUIRE_FALSE(kParentTStmt2Wild == kParentTStmt3Wild);

    INFO("Different Succ, Same Transitive and Pred");
    REQUIRE_FALSE(kParentStmt2Wild == kParentTStmt2StmtName);
  }
}

TEST_CASE("Test != of Parent", "[pql][parent][operator!=]") {
  SECTION("Positive") {
    INFO("Different Transitive, Same Pred and Succ");
    REQUIRE(kParentStmt2Wild != kParentTStmt2Wild);

    INFO("Different Pred, Same Transitive and Succ");
    REQUIRE(kParentTStmt2Wild != kParentTStmt3Wild);

    INFO("Different Succ, Same Transitive and Pred");
    REQUIRE(kParentStmt2Wild != kParentTStmt2StmtName);
  }

  SECTION("Negative") {
    INFO("Same Transitive, Pred and Succ");
    INFO("Same Pred and Succ pointers");
    REQUIRE_FALSE(kParentStmt2Wild !=
                  Parent(kStmtRefPtrStmtNo2, kStmtRefPtrWild));
    REQUIRE_FALSE(kParentTStmt2StmtName != Parent(kStmtRefPtrStmtNo2,
                                                  kStmtRefPtrDeclStmtStmtName,
                                                  true));

    INFO("Different Pred and Succ pointers, same contents");
    REQUIRE_FALSE(kParentStmt2Wild != Parent(kStmtRefPtrStmtNo2Duplicate,
                                             kStmtRefPtrWildDuplicate));
  }
}
