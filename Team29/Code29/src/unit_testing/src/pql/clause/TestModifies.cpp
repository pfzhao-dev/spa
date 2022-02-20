#include "TestModifies.h"

#include "../data/QueryDataUtil.h"
#include "catch.hpp"
#include "pql/QueryException.h"

using namespace test::pql::clause;

TEST_CASE("Test constructor of Modifies, [pql][modifies]") {
  SECTION("Positive") {
    INFO("Subject and Target is non-empty");
    REQUIRE_NOTHROW(Modifies(kEntityRefPtrDeclStmtStmtName, kEntityRefPtrWild));
    REQUIRE_NOTHROW(Modifies(kEntityRefPtrIdY, kEntityRefPtrIdX));
  }
  SECTION("Negative") {
    INFO("Subject is empty");
    REQUIRE_THROWS_AS(Modifies(kStmtRefPtrEmpty, kEntityRefPtrWild),
                      QueryParseException);
    REQUIRE_THROWS_AS(Modifies(kEntityRefPtrEmpty, kEntityRefPtrIdX),
                      QueryParseException);

    INFO("Target is empty");
    REQUIRE_THROWS_AS(Modifies(kEntityRefPtrDeclStmtStmtName, kEntityRefPtrEmpty),
                      QueryParseException);
    REQUIRE_THROWS_AS(Modifies(kEntityRefPtrIdY, kEntityRefPtrEmpty),
                      QueryParseException);
  }
}

TEST_CASE("Test == of Modifies", "[pql][modifies][operator==]") {
  SECTION("Positive") {
    INFO("Same Subject and Target");
    INFO("Same Subject and Target pointers");
    REQUIRE(kModifiesStmtStmtNameWild ==
            Modifies(kEntityRefPtrDeclStmtStmtName, kEntityRefPtrWild));
    REQUIRE(kModifiesIdXIdY == Modifies(kEntityRefPtrIdX, kEntityRefPtrIdY));

    INFO("Different Subject and Target pointers, same contents");
    REQUIRE(
        kModifiesStmtStmtNameWild ==
        Modifies(kEntityRefPtrDeclStmtStmtNameDuplicate, kEntityRefPtrWildDuplicate));
  }

  SECTION("Negative") {
    INFO("Different Subject, Same Target");
    REQUIRE_FALSE(kModifiesStmtStmtNameWild == kModifiesStmtNo2Wild);
    REQUIRE_FALSE(kModifiesStmtNo2Wild == kModifiesIdXWild);

    INFO("Different Target, Same Subject");
    REQUIRE_FALSE(kModifiesIdXWild == kModifiesIdXIdY);
  }
}

TEST_CASE("Test != of Modifies", "[pql][modifies][operator!=]") {
  SECTION("Positive") {
    INFO("Different Subject, Same Target");
    REQUIRE(kModifiesStmtStmtNameWild != kModifiesStmtNo2Wild);
    REQUIRE(kModifiesStmtNo2Wild != kModifiesIdXWild);

    INFO("Different Target, Same Subject");
    REQUIRE(kModifiesIdXWild != kModifiesIdXIdY);
  }

  SECTION("Negative") {
    INFO("Same Subject and Target");
    INFO("Same Subject and Target pointers");
    REQUIRE_FALSE(kModifiesStmtStmtNameWild !=
                  Modifies(kEntityRefPtrDeclStmtStmtName, kEntityRefPtrWild));
    REQUIRE_FALSE(kModifiesIdXIdY !=
                  Modifies(kEntityRefPtrIdX, kEntityRefPtrIdY));

    INFO("Different Subject and Target pointers, same contents");
    REQUIRE_FALSE(kModifiesStmtStmtNameWild !=
                  Modifies(kEntityRefPtrDeclStmtStmtName, kEntityRefPtrWild));
  }
}
