#include "TestFollows.h"

#include "../data/QueryDataUtil.h"
#include "catch.hpp"
#include "pql/QueryException.h"

using namespace test::pql::clause;

TEST_CASE("Test constructor of Follows, [pql][follows]") {
  SECTION("Positive") {
    INFO("Pred and Succ is non-empty");
    REQUIRE_NOTHROW(Follows(kStmtRefPtrStmtNo2, kStmtRefPtrWild));
  }
  SECTION("Negative") {
    INFO("Pred is empty");
    REQUIRE_THROWS_AS(Follows(kStmtRefPtrEmpty, kStmtRefPtrStmtNo2),
                      QueryParseException);

    INFO("Succ is empty");
    REQUIRE_THROWS_AS(Follows(kStmtRefPtrStmtNo2, kStmtRefPtrEmpty),
                      QueryParseException);
  }
}

TEST_CASE("Test == of Follows", "[pql][follows][operator==]") {
  SECTION("Positive") {
    INFO("Same Transitive, Pred and Succ");
    INFO("Same Pred and Succ pointers");
    REQUIRE(kFollowsStmt2Wild == Follows(kStmtRefPtrStmtNo2, kStmtRefPtrWild));
    REQUIRE(kFollowsStmt2StmtName ==
            Follows(kStmtRefPtrStmtNo2, kStmtRefPtrDeclStmtStmtName));

    INFO("Different Pred and Succ pointers, same contents");
    REQUIRE(kFollowsStmt2Wild ==
            Follows(kStmtRefPtrStmtNo2Duplicate, kStmtRefPtrWildDuplicate));
  }

  SECTION("Negative") {
    INFO("Different Transitive, Same Pred and Succ");
    REQUIRE_FALSE(kFollowsStmt2Wild == kFollowsTStmt2Wild);

    INFO("Different Pred, Same Transitive and Succ");
    REQUIRE_FALSE(kFollowsTStmt2Wild == kFollowsTStmt3Wild);

    INFO("Different Succ, Same Transitive and Pred");
    REQUIRE_FALSE(kFollowsStmt2Wild == kFollowsStmt2StmtName);
  }
}

TEST_CASE("Test != of Follows", "[pql][follows][operator!=]") {
  SECTION("Positive") {
    INFO("Different Transitive, Same Pred and Succ");
    REQUIRE(kFollowsStmt2Wild != kFollowsTStmt2Wild);

    INFO("Different Pred, Same Transitive and Succ");
    REQUIRE(kFollowsTStmt2Wild != kFollowsTStmt3Wild);

    INFO("Different Succ, Same Transitive and Pred");
    REQUIRE(kFollowsStmt2Wild != kFollowsStmt2StmtName);
  }

  SECTION("Negative") {
    INFO("Same Transitive, Pred and Succ");
    INFO("Same Pred and Succ pointers");
    REQUIRE_FALSE(kFollowsStmt2Wild !=
                  Follows(kStmtRefPtrStmtNo2, kStmtRefPtrWild));
    REQUIRE_FALSE(kFollowsStmt2StmtName !=
                  Follows(kStmtRefPtrStmtNo2, kStmtRefPtrDeclStmtStmtName));

    INFO("Different Pred and Succ pointers, same contents");
    REQUIRE_FALSE(kFollowsStmt2Wild != Follows(kStmtRefPtrStmtNo2Duplicate,
                                               kStmtRefPtrWildDuplicate));
  }
}
