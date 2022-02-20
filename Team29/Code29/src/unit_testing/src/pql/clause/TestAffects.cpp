#include "TestAffects.h"

#include "../data/QueryDataUtil.h"
#include "catch.hpp"
#include "pql/QueryException.h"

using namespace test::pql::clause;

TEST_CASE("Test constructor of Affects, [pql][affects]") {
  SECTION("Positive") {
    INFO("Affector and Affected is non-empty");
    REQUIRE_NOTHROW(Affects(kStmtRefPtrStmtNo2, kStmtRefPtrWild));
  }
  SECTION("Negative") {
    INFO("Affector is empty");
    REQUIRE_THROWS_AS(Affects(kStmtRefPtrEmpty, kStmtRefPtrStmtNo2),
                      QueryParseException);

    INFO("Affected is empty");
    REQUIRE_THROWS_AS(Affects(kStmtRefPtrStmtNo2, kStmtRefPtrEmpty),
                      QueryParseException);
  }
}

TEST_CASE("Test == of Affects", "[pql][affects][operator==]") {
  SECTION("Positive") {
    INFO("Same Transitive, Bip, Affector and Affected");
    INFO("Same Affector and Affected pointers");
    REQUIRE(kAffectsStmt2Wild == Affects(kStmtRefPtrStmtNo2, kStmtRefPtrWild));
    REQUIRE(kAffectsTStmt2AssignName ==
            Affects(kStmtRefPtrStmtNo2, kStmtRefPtrDeclAssignAssignName, true));

    INFO("Different Affector and Affected pointers, same contents");
    REQUIRE(kAffectsStmt2Wild ==
            Affects(kStmtRefPtrStmtNo2Duplicate, kStmtRefPtrWildDuplicate));
  }

  SECTION("Negative") {
    INFO("Different Transitive, Same Bip, Affector and Affected");
    REQUIRE_FALSE(kAffectsStmt2Wild == kAffectsTStmt2Wild);

    INFO("Different Affector, Same Bip, Transitive and Affected");
    REQUIRE_FALSE(kAffectsTStmt2Wild == kAffectsTStmt3Wild);

    INFO("Different Affected, Same Bip, Transitive and Affector");
    REQUIRE_FALSE(kAffectsStmt2Wild == kAffectsTStmt2AssignName);

    INFO("Different Bip, Same Bip, Transitive, Affector and Affector");
    REQUIRE_FALSE(kAffectsStmt2Wild == kAffectsBipStmt2Wild);
  }
}

TEST_CASE("Test != of Affects", "[pql][affects][operator!=]") {
  SECTION("Positive") {
    INFO("Different Transitive, Same Bip, Affector and Affected");
    REQUIRE(kAffectsStmt2Wild != kAffectsTStmt2Wild);

    INFO("Different Affector, Same Bip, Transitive and Affected");
    REQUIRE(kAffectsTStmt2Wild != kAffectsTStmt3Wild);

    INFO("Different Affected, Same Bip, Transitive and Affector");
    REQUIRE(kAffectsStmt2Wild != kAffectsTStmt2AssignName);

    INFO("Different Bip, Same Bip, Transitive, Affector and Affector");
    REQUIRE(kAffectsStmt2Wild != kAffectsBipStmt2Wild);
  }

  SECTION("Negative") {
    INFO("Same Transitive, Bip, Affector and Affected");
    INFO("Same Affector and Affected pointers");
    REQUIRE_FALSE(kAffectsStmt2Wild !=
                  Affects(kStmtRefPtrStmtNo2, kStmtRefPtrWild));
    REQUIRE_FALSE(
        kAffectsTStmt2AssignName !=
        Affects(kStmtRefPtrStmtNo2, kStmtRefPtrDeclAssignAssignName, true));

    INFO("Different Affector and Affected pointers, same contents");
    REQUIRE_FALSE(kAffectsStmt2Wild != Affects(kStmtRefPtrStmtNo2Duplicate,
                                               kStmtRefPtrWildDuplicate));
  }
}
