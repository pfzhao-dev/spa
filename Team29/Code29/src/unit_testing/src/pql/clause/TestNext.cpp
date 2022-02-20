#include "TestNext.h"

#include "../data/QueryDataUtil.h"
#include "catch.hpp"
#include "pql/QueryException.h"

using namespace test::pql::clause;

TEST_CASE("Test constructor of Next, [pql][next]") {
  SECTION("Positive") {
    INFO("Pred and Succ is non-empty");
    REQUIRE_NOTHROW(Next(kStmtRefPtrStmtNo2, kStmtRefPtrWild));
  }
  SECTION("Negative") {
    INFO("Pred is empty");
    REQUIRE_THROWS_AS(Next(kStmtRefPtrEmpty, kStmtRefPtrStmtNo2),
                      QueryParseException);

    INFO("Succ is empty");
    REQUIRE_THROWS_AS(Next(kStmtRefPtrStmtNo2, kStmtRefPtrEmpty),
                      QueryParseException);
  }
}

TEST_CASE("Test == of Next", "[pql][next][operator==]") {
  SECTION("Positive") {
    INFO("Same Transitive, Bip, Pred and Succ");
    INFO("Same Pred and Succ pointers");
    REQUIRE(kNextStmt2Wild == Next(kStmtRefPtrStmtNo2, kStmtRefPtrWild));
    REQUIRE(kNextStmt2StmtName ==
            Next(kStmtRefPtrStmtNo2, kStmtRefPtrDeclStmtStmtName));

    INFO("Different Pred and Succ pointers, same contents");
    REQUIRE(kNextStmt2Wild ==
            Next(kStmtRefPtrStmtNo2Duplicate, kStmtRefPtrWildDuplicate));
  }

  SECTION("Negative") {
    INFO("Different Transitive, Same Bip, Pred and Succ");
    REQUIRE_FALSE(kNextStmt2Wild == kNextTStmt2Wild);

    INFO("Different Pred, Same Transitive, Bip and Succ");
    REQUIRE_FALSE(kNextTStmt2Wild == kNextTStmt3Wild);

    INFO("Different Succ, Same Transitive, Bip and Pred");
    REQUIRE_FALSE(kNextStmt2Wild == kNextStmt2StmtName);

    INFO("Different Bip, Same Transitive, Succ and Pred");
    REQUIRE_FALSE(kNextStmt2StmtName == kNextBipStmt2StmtName);
  }
}

TEST_CASE("Test != of Next", "[pql][next][operator!=]") {
  SECTION("Positive") {
    INFO("Different Transitive, Same Pred and Succ");
    REQUIRE(kNextStmt2Wild != kNextTStmt2Wild);

    INFO("Different Pred, Same Transitive and Succ");
    REQUIRE(kNextTStmt2Wild != kNextTStmt3Wild);

    INFO("Different Succ, Same Transitive and Pred");
    REQUIRE(kNextStmt2Wild != kNextStmt2StmtName);

    INFO("Different Bip, Same Transitive, Succ and Pred");
    REQUIRE(kNextStmt2StmtName != kNextBipStmt2StmtName);
  }

  SECTION("Negative") {
    INFO("Same Transitive, Bip, Pred and Succ");
    INFO("Same Pred and Succ pointers");
    REQUIRE_FALSE(kNextStmt2Wild != Next(kStmtRefPtrStmtNo2, kStmtRefPtrWild));
    REQUIRE_FALSE(kNextStmt2StmtName !=
                  Next(kStmtRefPtrStmtNo2, kStmtRefPtrDeclStmtStmtName));

    INFO("Different Pred and Succ pointers, same contents");
    REQUIRE_FALSE(kNextStmt2Wild !=
                  Next(kStmtRefPtrStmtNo2Duplicate, kStmtRefPtrWildDuplicate));
  }
}
