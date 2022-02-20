#include "TestPattern.h"

#include "../data/QueryDataUtil.h"
#include "catch.hpp"
#include "pql/QueryException.h"

using namespace test::pql::clause;

TEST_CASE("Test constructor of Expression, [pql][expression][pattern]") {
  SECTION("Positive") {
    INFO("Spec is non-empty");
    REQUIRE_NOTHROW(Pattern::Expression());
    REQUIRE_NOTHROW(Pattern::Expression(kSpec1));
    REQUIRE_NOTHROW(Pattern::Expression(kSpec2, true));
  }
  SECTION("Negative") {
    INFO("Spec is empty");
    REQUIRE_THROWS_AS(Pattern::Expression(test::pql::data::kEmptyStr),
                      QueryParseException);
  }
}

TEST_CASE("Test == of Expression", "[pql][expression][pattern][operator==]") {
  SECTION("Positive") {
    INFO("Same Wildcard, Partial and Spec");
    REQUIRE(KExprWild == Pattern::Expression());
    REQUIRE(KExprSpec1 == Pattern::Expression(kSpec1));
    REQUIRE(KExprPartialSpec1 == Pattern::Expression(kSpec1, true));
  }

  SECTION("Negative") {
    INFO("Different Partial, Same Wildcard and Spec");
    REQUIRE_FALSE(KExprSpec1 == KExprPartialSpec1);

    INFO("Different Spec, Same Wildcard and Partial");
    REQUIRE_FALSE(KExprPartialSpec1 == KExprPartialSpec2);
  }
}

TEST_CASE("Test constructor of Pattern, [pql][pattern]") {
  SECTION("Positive") {
    INFO("Ref for type and var is non-empty");

    INFO("Ref for type and var is declared");
    REQUIRE_NOTHROW(Pattern(kStmtRefPtrDeclAssignAssignName,
                            kEntityRefPtrDeclVarVarName, KExprWild));
    REQUIRE_NOTHROW(
        Pattern(kStmtRefPtrDeclIfIfName, kEntityRefPtrIdX, KExprWild));

    INFO("Entity ref for type is declared");
    REQUIRE_NOTHROW(
        Pattern(kStmtRefPtrDeclWhileWhileName, kEntityRefPtrWild, KExprWild));
    REQUIRE_NOTHROW(
        Pattern(kStmtRefPtrDeclAssignAssignName, kEntityRefPtrWild, KExprWild));
  }
  SECTION("Negative") {
    INFO("Statement ref for type is empty");
    REQUIRE_THROWS_AS(Pattern(kStmtRefPtrEmpty, kEntityRefPtrWild, KExprWild),
                      QueryParseException);

    INFO("Entity ref for var is empty");
    REQUIRE_THROWS_AS(
        Pattern(kStmtRefPtrDeclAssignAssignName, kEntityRefPtrEmpty, KExprWild),
        QueryParseException);

    INFO("Statement ref for type is of an invalid type");
    REQUIRE_THROWS_AS(
        Pattern(kStmtRefPtrDeclStmtStmtName, kEntityRefPtrWild, KExprWild),
        QueryParseException);

    INFO("Statement ref for var is of an invalid type");
    REQUIRE_THROWS_AS(Pattern(kStmtRefPtrDeclAssignAssignName,
                              kEntityRefPtrDeclStmtStmtName, KExprWild),
                      QueryParseException);
  }
}

TEST_CASE("Test == of Pattern", "[pql][pattern][operator==]") {
  SECTION("Positive") {
    INFO("Same type, var and expression");

    INFO("Same type and var pointers");
    REQUIRE(
        kPatternTypeAssignVarWildExprWild ==
        Pattern(kStmtRefPtrDeclAssignAssignName, kEntityRefPtrWild, KExprWild));
    REQUIRE(
        kPatternTypeAssignVarXExprWild ==
        Pattern(kStmtRefPtrDeclAssignAssignName, kEntityRefPtrIdX, KExprWild));
    REQUIRE(kPatternTypeAssignVarWildExprPartialSpec1 ==
            Pattern(kStmtRefPtrDeclAssignAssignName, kEntityRefPtrWild,
                    KExprPartialSpec1));

    INFO("Different type and var pointers, same contents");
    REQUIRE(kPatternTypeAssignVarWildExprPartialSpec1 ==
            Pattern(kStmtRefPtrDeclAssignAssignNameDuplicate,
                    kEntityRefPtrWildDuplicate, KExprPartialSpec1));
  }

  SECTION("Negative") {
    INFO("Different type, Same var and expression");
    REQUIRE_FALSE(kPatternTypeWhileVarWildExprWild ==
                  kPatternTypeIfVarWildExprWild);

    INFO("Different var, Same type and expression");
    REQUIRE_FALSE(kPatternTypeAssignVarWildExprWild ==
                  kPatternTypeAssignVarXExprWild);

    INFO("Different expression, Same var and type");
    REQUIRE_FALSE(kPatternTypeAssignVarWildExprWild ==
                  kPatternTypeAssignVarWildExprPartialSpec1);
  }
}

TEST_CASE("Test != of Pattern", "[pql][pattern][operator!=]") {
  SECTION("Positive") {
    INFO("Different type, Same var and expression");
    REQUIRE(kPatternTypeWhileVarWildExprWild != kPatternTypeIfVarWildExprWild);

    INFO("Different var, Same type and expression");
    REQUIRE(kPatternTypeAssignVarWildExprWild !=
            kPatternTypeAssignVarXExprWild);

    INFO("Different expression, Same var and type");
    REQUIRE(kPatternTypeAssignVarWildExprWild !=
            kPatternTypeAssignVarWildExprPartialSpec1);
  }

  SECTION("Negative") {
    INFO("Same type, var and expression");

    INFO("Same type and var pointers");
    REQUIRE_FALSE(
        kPatternTypeAssignVarWildExprWild !=
        Pattern(kStmtRefPtrDeclAssignAssignName, kEntityRefPtrWild, KExprWild));
    REQUIRE_FALSE(
        kPatternTypeAssignVarXExprWild !=
        Pattern(kStmtRefPtrDeclAssignAssignName, kEntityRefPtrIdX, KExprWild));
    REQUIRE_FALSE(kPatternTypeAssignVarWildExprPartialSpec1 !=
                  Pattern(kStmtRefPtrDeclAssignAssignName, kEntityRefPtrWild,
                          KExprPartialSpec1));

    INFO("Different type and var pointers, same contents");
    REQUIRE_FALSE(kPatternTypeAssignVarWildExprPartialSpec1 !=
                  Pattern(kStmtRefPtrDeclAssignAssignNameDuplicate,
                          kEntityRefPtrWildDuplicate, KExprPartialSpec1));
  }
}
