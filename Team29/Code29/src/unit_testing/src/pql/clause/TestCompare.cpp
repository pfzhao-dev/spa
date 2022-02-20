#include "TestCompare.h"

#include "../data/QueryDataUtil.h"
#include "catch.hpp"
#include "pql/QueryException.h"

using namespace test::pql::clause;

TEST_CASE("Test constructor of Compare, [pql][compare]") {
  SECTION("Positive") {
    INFO("Lhs and Rhs is non-empty");
    REQUIRE_NOTHROW(
        Compare(kEntityRefDeclPtrStmtStmtNumStmtName, kStmtRefPtrStmtNo2));
    REQUIRE_NOTHROW(
        Compare(kEntityRefPtrIdX, kEntityRefPtrDeclProcProcNameProcName));
  }
  SECTION("Negative") {
    INFO("Lhs is empty");
    REQUIRE_THROWS_AS(
        Compare(kEntityRefPtrEmpty, kEntityRefDeclPtrStmtStmtNumStmtName),
        QueryParseException);
    REQUIRE_THROWS_AS(Compare(kEntityRefPtrEmpty, kEntityRefPtrIdX),
                      QueryParseException);

    INFO("Rhs is empty");
    REQUIRE_THROWS_AS(
        Compare(kEntityRefPtrDeclProcProcName, kEntityRefPtrEmpty),
        QueryParseException);
    REQUIRE_THROWS_AS(
        Compare(kEntityRefPtrDeclConstConstNameConstValue, kEntityRefPtrEmpty),
        QueryParseException);
  }
}

TEST_CASE("Test == of Compare", "[pql][compare][operator==]") {
  SECTION("Positive") {
    INFO("Same Lhs and Rhs");
    INFO("Same Lhs and Rhs pointers");
    REQUIRE(kCompareStmtStmtNameConstNameConstValue ==
            Compare(kEntityRefPtrDeclStmtStmtName,
                    kEntityRefPtrDeclConstConstNameConstValue));
    REQUIRE(kCompareIdXProcProcNameProcName ==
            Compare(kEntityRefPtrIdX, kEntityRefPtrDeclProcProcNameProcName));

    INFO("Different Lhs and Rhs pointers, same contents");
    REQUIRE(kCompareStmtStmtNameConstNameConstValue ==
            Compare(kEntityRefPtrDeclStmtStmtNameDuplicate,
                    kEntityRefPtrDeclConstNameConstValueDuplicate));

    SECTION("Negative") {
      INFO("Different Lhs, Same Rhs");
      REQUIRE_FALSE(kCompareIdXPrintPrintNameVarName ==
                    kCompareIdXProcProcNameProcName);

      INFO("Different Rhs, Same Lhs");
      REQUIRE_FALSE(kCompareStmtStmtNumStmtNameStmtNo2 ==
                    kCompareStmtStmtNumStmtNameStmtNo3);
    }
  }
}

TEST_CASE("Test != of Compare", "[pql][compare][operator!=]") {
  SECTION("Positive") {
    INFO("Different Lhs, Same Rhs");
    REQUIRE(kCompareIdXPrintPrintNameVarName !=
            kCompareIdXProcProcNameProcName);

    INFO("Different Rhs, Same Lhs");
    REQUIRE(kCompareStmtStmtNumStmtNameStmtNo2 !=
            kCompareStmtStmtNumStmtNameStmtNo3);
  }

  SECTION("Negative") {
    INFO("Same Lhs and Rhs");
    INFO("Same Lhs and Rhs pointers");
    REQUIRE_FALSE(kCompareStmtStmtNameConstNameConstValue !=
                  Compare(kEntityRefPtrDeclStmtStmtName,
                          kEntityRefPtrDeclConstConstNameConstValue));
    REQUIRE_FALSE(
        kCompareIdXProcProcNameProcName !=
        Compare(kEntityRefPtrIdX, kEntityRefPtrDeclProcProcNameProcName));

    INFO("Different Lhs and Rhs pointers, same contents");
    REQUIRE_FALSE(kCompareStmtStmtNameConstNameConstValue !=
                  Compare(kEntityRefPtrDeclStmtStmtNameDuplicate,
                          kEntityRefPtrDeclConstNameConstValueDuplicate));
  }
}
