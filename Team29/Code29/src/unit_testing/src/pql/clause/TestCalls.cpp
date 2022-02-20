#include "TestCalls.h"

#include "../data/QueryDataUtil.h"
#include "catch.hpp"
#include "pql/QueryException.h"

using namespace test::pql::clause;

TEST_CASE("Test constructor of Calls, [pql][calls]") {
  SECTION("Positive") {
    INFO("Caller and Callee is non-empty");
    REQUIRE_NOTHROW(Calls(kEntityRefPtrDeclProcProcName, kEntityRefPtrWild));
    REQUIRE_NOTHROW(Calls(kEntityRefPtrIdY, kEntityRefPtrIdX));
  }
  SECTION("Negative") {
    INFO("Caller is empty");
    REQUIRE_THROWS_AS(Calls(kEntityRefPtrEmpty, kEntityRefPtrWild),
                      QueryParseException);
    REQUIRE_THROWS_AS(Calls(kEntityRefPtrEmpty, kEntityRefPtrIdX),
                      QueryParseException);

    INFO("Callee is empty");
    REQUIRE_THROWS_AS(Calls(kEntityRefPtrDeclProcProcName, kEntityRefPtrEmpty),
                      QueryParseException);
    REQUIRE_THROWS_AS(Calls(kEntityRefPtrIdY, kEntityRefPtrEmpty),
                      QueryParseException);
  }
}

TEST_CASE("Test == of Calls", "[pql][calls][operator==]") {
  SECTION("Positive") {
    INFO("Same Transitive, Caller and Callee");
    INFO("Same Caller and Callee pointers");
    REQUIRE(kCallsProcProcNameWild ==
            Calls(kEntityRefPtrDeclProcProcName, kEntityRefPtrWild));
    REQUIRE(kCallsIdXIdY == Calls(kEntityRefPtrIdX, kEntityRefPtrIdY));

    INFO("Different Caller and Callee pointers, same contents");
    REQUIRE(kCallsProcProcNameWild ==
            Calls(kEntityRefPtrDeclProcProcNameDuplicate,
                  kEntityRefPtrWildDuplicate));
  }

  SECTION("Negative") {
    INFO("Different Transitive, Same Caller and Callee");
    REQUIRE_FALSE(kCallsWildProcProcName == kCallsTWildProcProcName);
    REQUIRE_FALSE(kCallsIdXWild == kCallsTIdXWild);

    INFO("Different Caller, Same Transitive and Callee");
    REQUIRE_FALSE(kCallsProcProcNameWild == kCallsWildWild);
    REQUIRE_FALSE(kCallsWildWild == kCallsIdXWild);

    INFO("Different Callee, Same Transitive and Caller");
    REQUIRE_FALSE(kCallsIdXWild == kCallsIdXIdY);
  }
}

TEST_CASE("Test != of Calls", "[pql][calls][operator!=]") {
  SECTION("Positive") {
    INFO("Different Transitive, Same Caller and Callee");
    REQUIRE(kCallsWildProcProcName != kCallsTWildProcProcName);
    REQUIRE(kCallsIdXWild != kCallsTIdXWild);

    INFO("Different Caller, Same Transitive and Callee");
    REQUIRE(kCallsProcProcNameWild != kCallsWildWild);
    REQUIRE(kCallsWildWild != kCallsIdXWild);

    INFO("Different Callee, Same Transitive and Caller");
    REQUIRE(kCallsIdXWild != kCallsIdXIdY);
  }

  SECTION("Negative") {
    INFO("Same Transitive, Caller and Callee");
    INFO("Same Caller and Callee pointers");
    REQUIRE_FALSE(kCallsProcProcNameWild !=
                  Calls(kEntityRefPtrDeclProcProcName, kEntityRefPtrWild));
    REQUIRE_FALSE(kCallsIdXIdY != Calls(kEntityRefPtrIdX, kEntityRefPtrIdY));

    INFO("Different Caller and Callee pointers, same contents");
    REQUIRE_FALSE(kCallsProcProcNameWild != Calls(kEntityRefPtrDeclProcProcName,
                                                  kEntityRefPtrWildDuplicate));
  }
}
