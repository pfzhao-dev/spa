#include "../data/QueryDataUtil.h"
#include "TestFollows.h"
#include "TestModifies.h"
#include "TestParent.h"
#include "TestPattern.h"
#include "TestUses.h"
#include "catch.hpp"
#include "pql/QueryException.h"

using namespace test::pql::clause;

TEST_CASE("Test == of Clause", "[pql][clause][operator==]") {
  SECTION("Negative") {
    INFO("Different Type");
    REQUIRE_FALSE(kFollowsStmt2Wild == kParentStmt2Wild);
    REQUIRE_FALSE(kFollowsStmt2Wild == kUsesStmtStmtNameWild);
    REQUIRE_FALSE(kFollowsStmt2Wild == kModifiesStmtStmtNameWild);
    REQUIRE_FALSE(kFollowsStmt2Wild == kPatternTypeAssignVarWildExprWild);

    REQUIRE_FALSE(kParentTStmt2Wild == kUsesStmtNo2Wild);
    REQUIRE_FALSE(kParentTStmt2Wild == kModifiesStmtNo2Wild);
    REQUIRE_FALSE(kParentTStmt2Wild == kPatternTypeAssignVarXExprWild);

    REQUIRE_FALSE(kUsesIdXWild == kModifiesIdXWild);
    REQUIRE_FALSE(kUsesIdXWild == kPatternTypeAssignVarWildExprPartialSpec1);

    REQUIRE_FALSE(kModifiesIdXIdY == kPatternTypeAssignVarXExprWild);
  }
}
