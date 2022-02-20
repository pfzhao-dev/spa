#include "TestQuery.h"

#include "catch.hpp"
#include "data/QueryDataUtil.h"
#include "pql/QueryException.h"

using namespace test::pql::query;

TEST_CASE("Test == of Query", "[pql][query][operator==]") {
  SECTION("Positive") {
    INFO("Same Declaration and Clauses");

    INFO("Same Declaration and Clauses pointers");
    REQUIRE(kQueryConstNameEmpty ==
            Query(kResultPtrConstConstName, kClausesEmpty));
    REQUIRE(kQueryStmtNameFollowsStmt2Wild ==
            Query(kResultPtrStmtStmtName, kClausesFollowsStmt2Wild));
    REQUIRE(kQueryStmtNameUsesStmtNameWild ==
            Query(kResultPtrStmtStmtName, kClausesUsesStmtNameWild));
    REQUIRE(kQueryStmtNamePatternTypeAssignVarWildExprPartialSpec1 ==
            Query(kResultPtrStmtStmtName,
                  kClausesPatternTypeAssignVarWildExprPartialSpec1));
    REQUIRE(
        kQueryStmtNameUsesStmtNameWildPatternTypeAssignVarWildExprPartialSpec1 ==
        Query(
            kResultPtrStmtStmtName,
            kClausesUsesStmtNameWildPatternTypeAssignVarWildExprPartialSpec1));

    INFO("Different Declaration and Clauses pointers with same contents");
    REQUIRE(kQueryConstNameEmpty ==
            Query(kResultPtrConstConstNameDuplicate, kClausesEmptyDuplicate));
    REQUIRE(
        kQueryStmtNameFollowsStmt2Wild ==
        Query(kResultPtrStmtStmtNameDuplicate, kClausesFollowsStmt2WildDuplicate));
    REQUIRE(
        kQueryStmtNameUsesStmtNameWild ==
        Query(kResultPtrStmtStmtNameDuplicate, kClausesUsesStmtNameWildDuplicate));
    REQUIRE(kQueryStmtNamePatternTypeAssignVarWildExprPartialSpec1 ==
            Query(kResultPtrStmtStmtNameDuplicate,
                  kClausesPatternTypeAssignVarWildExprPartialSpec1Duplicate));
    REQUIRE(
        kQueryStmtNameUsesStmtNameWildPatternTypeAssignVarWildExprPartialSpec1 ==
        Query(
            kResultPtrStmtStmtNameDuplicate,
            kClausesUsesStmtNameWildPatternTypeAssignVarWildExprPartialSpec1Duplicate));
  }

  SECTION("Negative") {
    INFO("Different Declaration, Same Clauses");
    REQUIRE_FALSE(kQueryStmtNameEmpty == kQueryConstNameEmpty);
    REQUIRE_FALSE(kQueryStmtNameUsesStmtNameWild ==
                  kQueryConstNameUsesStmtNameWild);
    REQUIRE_FALSE(
        kQueryStmtNameUsesStmtNameWildPatternTypeAssignVarWildExprPartialSpec1 ==
        kQueryConstNameUsesStmtNameWildPatternTypeAssignVarWildExprPartialSpec1);

    INFO("Different Clauses, Same Declaration");
    REQUIRE_FALSE(
        kQueryStmtNameEmpty ==
        kQueryStmtNameUsesStmtNameWildPatternTypeAssignVarWildExprPartialSpec1);
    REQUIRE_FALSE(kQueryConstNameEmpty == kQueryConstNameUsesStmtNameWild);
    REQUIRE_FALSE(kQueryStmtNameFollowsStmt2Wild ==
                  kQueryStmtNameUsesStmtNameWild);
  }
}

TEST_CASE("Test != of Query", "[pql][query][operator!=]") {
  SECTION("Positive") {
    INFO("Different Declaration, Same Clauses");
    REQUIRE(kQueryStmtNameEmpty != kQueryConstNameEmpty);
    REQUIRE(kQueryStmtNameUsesStmtNameWild != kQueryConstNameUsesStmtNameWild);
    REQUIRE(
        kQueryStmtNameUsesStmtNameWildPatternTypeAssignVarWildExprPartialSpec1 !=
        kQueryConstNameUsesStmtNameWildPatternTypeAssignVarWildExprPartialSpec1);

    INFO("Different Clauses, Same Declaration");
    REQUIRE(
        kQueryStmtNameEmpty !=
        kQueryStmtNameUsesStmtNameWildPatternTypeAssignVarWildExprPartialSpec1);
    REQUIRE(kQueryConstNameEmpty != kQueryConstNameUsesStmtNameWild);
    REQUIRE(kQueryStmtNameFollowsStmt2Wild != kQueryStmtNameUsesStmtNameWild);
  }

  SECTION("Negative") {
    INFO("Same Declaration and Clauses");

    INFO("Same Declaration and Clauses pointers");
    REQUIRE_FALSE(kQueryConstNameEmpty !=
                  Query(kResultPtrConstConstName, kClausesEmpty));
    REQUIRE_FALSE(kQueryStmtNameFollowsStmt2Wild !=
                  Query(kResultPtrStmtStmtName, kClausesFollowsStmt2Wild));
    REQUIRE_FALSE(kQueryStmtNameUsesStmtNameWild !=
                  Query(kResultPtrStmtStmtName, kClausesUsesStmtNameWild));
    REQUIRE_FALSE(kQueryStmtNamePatternTypeAssignVarWildExprPartialSpec1 !=
                  Query(kResultPtrStmtStmtName,
                        kClausesPatternTypeAssignVarWildExprPartialSpec1));
    REQUIRE_FALSE(
        kQueryStmtNameUsesStmtNameWildPatternTypeAssignVarWildExprPartialSpec1 !=
        Query(
            kResultPtrStmtStmtName,
            kClausesUsesStmtNameWildPatternTypeAssignVarWildExprPartialSpec1));

    INFO("Different Declaration and Clauses pointers with same contents");
    REQUIRE_FALSE(kQueryConstNameEmpty != Query(kResultPtrConstConstNameDuplicate,
                                                kClausesEmptyDuplicate));
    REQUIRE_FALSE(
        kQueryStmtNameFollowsStmt2Wild !=
        Query(kResultPtrStmtStmtNameDuplicate, kClausesFollowsStmt2WildDuplicate));
    REQUIRE_FALSE(
        kQueryStmtNameUsesStmtNameWild !=
        Query(kResultPtrStmtStmtNameDuplicate, kClausesUsesStmtNameWildDuplicate));
    REQUIRE_FALSE(
        kQueryStmtNamePatternTypeAssignVarWildExprPartialSpec1 !=
        Query(kResultPtrStmtStmtNameDuplicate,
              kClausesPatternTypeAssignVarWildExprPartialSpec1Duplicate));
    REQUIRE_FALSE(
        kQueryStmtNameUsesStmtNameWildPatternTypeAssignVarWildExprPartialSpec1 !=
        Query(
            kResultPtrStmtStmtNameDuplicate,
            kClausesUsesStmtNameWildPatternTypeAssignVarWildExprPartialSpec1Duplicate));
  }
}
