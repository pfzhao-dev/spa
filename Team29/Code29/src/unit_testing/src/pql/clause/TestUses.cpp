#include "TestUses.h"

#include "../data/QueryDataUtil.h"
#include "catch.hpp"
#include "pql/QueryException.h"

using namespace test::pql::clause;

TEST_CASE("Test constructor of Uses, [pql][uses]") {
  SECTION("Positive") {
    INFO("Subject and Target is non-empty");
    REQUIRE_NOTHROW(Uses(kEntityRefPtrDeclStmtStmtName, kEntityRefPtrWild));
    REQUIRE_NOTHROW(Uses(kEntityRefPtrIdY, kEntityRefPtrIdX));
  }
  SECTION("Negative") {
    INFO("Subject is empty");
    REQUIRE_THROWS_AS(Uses(kStmtRefPtrEmpty, kEntityRefPtrWild),
                      QueryParseException);
    REQUIRE_THROWS_AS(Uses(kEntityRefPtrEmpty, kEntityRefPtrIdX),
                      QueryParseException);

    INFO("Target is empty");
    REQUIRE_THROWS_AS(Uses(kEntityRefPtrDeclStmtStmtName, kEntityRefPtrEmpty),
                      QueryParseException);
    REQUIRE_THROWS_AS(Uses(kEntityRefPtrIdY, kEntityRefPtrEmpty),
                      QueryParseException);
  }
}

TEST_CASE("Test == of Uses", "[pql][uses][operator==]") {
  SECTION("Positive") {
    INFO("Same Subject and Target");
    INFO("Same Subject and Target pointers");
    REQUIRE(kUsesStmtStmtNameWild ==
            Uses(kEntityRefPtrDeclStmtStmtName, kEntityRefPtrWild));
    REQUIRE(kUsesIdXIdY == Uses(kEntityRefPtrIdX, kEntityRefPtrIdY));

    INFO("Different Subject and Target pointers, same contents");
    REQUIRE(kUsesStmtStmtNameWild ==
            Uses(kEntityRefPtrDeclStmtStmtNameDuplicate,
                 kEntityRefPtrWildDuplicate));
  }

  SECTION("Negative") {
    INFO("Different Subject, Same Target");
    REQUIRE_FALSE(kUsesStmtStmtNameWild == kUsesStmtNo2Wild);
    REQUIRE_FALSE(kUsesStmtNo2Wild == kUsesIdXWild);

    INFO("Different Target, Same Subject");
    REQUIRE_FALSE(kUsesIdXWild == kUsesIdXIdY);
  }
}

TEST_CASE("Test != of Uses", "[pql][uses][operator!=]") {
  SECTION("Positive") {
    INFO("Different Subject, Same Target");
    REQUIRE(kUsesStmtStmtNameWild != kUsesStmtNo2Wild);
    REQUIRE(kUsesStmtNo2Wild != kUsesIdXWild);

    INFO("Different Target, Same Subject");
    REQUIRE(kUsesIdXWild != kUsesIdXIdY);
  }

  SECTION("Negative") {
    INFO("Same Subject and Target");
    INFO("Same Subject and Target pointers");
    REQUIRE_FALSE(kUsesStmtStmtNameWild !=
                  Uses(kEntityRefPtrDeclStmtStmtName, kEntityRefPtrWild));
    REQUIRE_FALSE(kUsesIdXIdY != Uses(kEntityRefPtrIdX, kEntityRefPtrIdY));

    INFO("Different Subject and Target pointers, same contents");
    REQUIRE_FALSE(kUsesStmtStmtNameWild !=
                  Uses(kEntityRefPtrDeclStmtStmtNameDuplicate,
                       kEntityRefPtrWildDuplicate));
  }
}
