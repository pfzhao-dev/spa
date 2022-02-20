#include "TestRef.h"

#include "catch.hpp"
#include "data/QueryDataUtil.h"
#include "pql/QueryException.h"

using namespace test::pql::ref;

TEST_CASE("Test IsWild of Ref", "[pql][ref][iswild]") {
  SECTION("Positive") {
    REQUIRE(kEntityRefWild.IsWild());
    REQUIRE(kStmtRefWild.IsWild());
  }
  SECTION("Negative") {
    REQUIRE_FALSE(kEntityRefDeclVarNoName.IsWild());
    REQUIRE_FALSE(kStmtRefStmtNo3.IsWild());
  }
}

TEST_CASE("Test IsDeclTypeIn of Ref", "[pql][ref][matchesdecltype]") {
  SECTION("POSITIVE") {
    REQUIRE(kEntityRefDeclStmtStmtName.IsDeclTypeIn(
        {spa::pql::Declaration::Type::kStmt,
         spa::pql::Declaration::Type::kProcedure}));
    REQUIRE(kStmtRefDeclAssignAssignName.IsDeclTypeIn(
        {spa::pql::Declaration::Type::kAssign}));
  }
  SECTION("NEGATIVE") {
    REQUIRE_FALSE(
        kEntityRefWild.IsDeclTypeIn({spa::pql::Declaration::Type::kStmt}));
    REQUIRE_FALSE(
        kStmtRefWild.IsDeclTypeIn({spa::pql::Declaration::Type::kStmt}));
    REQUIRE_FALSE(kEntityRefDeclStmtStmtName.IsDeclTypeIn(
        {spa::pql::Declaration::Type::kProcedure}));
    REQUIRE_FALSE(kStmtRefDeclAssignAssignName.IsDeclTypeIn(
        {spa::pql::Declaration::Type::kCall}));
  }
}

TEST_CASE("Test IsDeclared of Ref", "[pql][ref][isdeclared]") {
  SECTION("POSITIVE") {
    REQUIRE(kEntityRefDeclStmtStmtName.IsDeclared());
    REQUIRE(kStmtRefDeclAssignAssignName.IsDeclared());
  }
  SECTION("NEGATIVE") {
    REQUIRE_FALSE(kEntityRefWild.IsDeclared());
    REQUIRE_FALSE(kStmtRefWild.IsDeclared());
    REQUIRE_FALSE(kEntityRefDeclEmpty.IsDeclared());
  }
}

TEST_CASE("Test == of Ref", "[pql][ref][operator==]") {
  SECTION("Positive") {
    SECTION("Same Ref Type, Same WildCard and Same Declaration") {
      INFO("Same Declaration pointer");
      REQUIRE(kEntityRefDeclVarNoName == EntityRef(kDeclPtrVarNoName));
      REQUIRE(kStmtRefDeclStmtStmtName == StatementRef(kDeclPtrStmtStmtName));

      INFO("Both null declaration pointer");
      REQUIRE(kEntityRefIdX == EntityRef(kIdX));
      REQUIRE(kEntityRefWild == EntityRef());

      REQUIRE(kStmtRefWild == StatementRef());

      INFO("Same Declaration, Different Declaration pointer");
      REQUIRE(kEntityRefDeclVarNoName == EntityRef(kDeclPtrVarNoNameDuplicate));
      REQUIRE(kStmtRefDeclStmtStmtName ==
              StatementRef(kDeclPtrStmtStmtNameDuplicate));
    }
  }

  SECTION("Negative") {
    SECTION("Different Ref Type, Same Wildcard and Declaration") {
      REQUIRE_FALSE(kStmtRefWild == kEntityRefWild);
      REQUIRE_FALSE(kStmtRefDeclStmtStmtName == kEntityRefDeclStmtStmtName);
    }

    SECTION("Same Ref Type, Different WildCard and Declaration") {
      REQUIRE_FALSE(kStmtRefWild == kStmtRefDeclStmtStmtName);
      REQUIRE_FALSE(kEntityRefWild == kEntityRefDeclVarNoName);
    }

    SECTION("Different Declaration, Same Ref Type and WildCard") {
      INFO("Different Declaration");

      REQUIRE_FALSE(kEntityRefDeclVarNoName == kEntityRefDeclStmtStmtName);
      REQUIRE_FALSE(kStmtRefDeclStmtStmtName == kStmtRefDeclAssignAssignName);

      INFO("1 null declaration pointer");
      REQUIRE_FALSE(kEntityRefDeclEmpty == kEntityRefDeclVarNoName);
    }
  }
}

TEST_CASE("Test != of Ref", "[pql][ref][operator!=]") {
  SECTION("Positive") {
    SECTION("Different Ref Type, Same Wildcard and Declaration") {
      REQUIRE(kStmtRefWild != kEntityRefWild);
      REQUIRE(kStmtRefDeclStmtStmtName != kEntityRefDeclStmtStmtName);
    }

    SECTION("Same Ref Type, Different WildCard and Declaration") {
      REQUIRE(kStmtRefWild != kStmtRefDeclStmtStmtName);
      REQUIRE(kEntityRefWild != kEntityRefDeclVarNoName);
    }

    SECTION("Different Declaration, Same Ref Type and WildCard") {
      INFO("Different Declaration");

      REQUIRE(kEntityRefDeclVarNoName != kEntityRefDeclStmtStmtName);
      REQUIRE(kStmtRefDeclStmtStmtName != kStmtRefDeclAssignAssignName);

      INFO("1 null declaration pointer");
      REQUIRE(kEntityRefDeclEmpty != kEntityRefDeclVarNoName);
    }
  }
  SECTION("Negative") {
    SECTION("Same Ref Type, Same WildCard and Same Declaration") {
      INFO("Same Declaration pointer");
      REQUIRE_FALSE(kEntityRefDeclVarNoName != EntityRef(kDeclPtrVarNoName));
      REQUIRE_FALSE(kStmtRefDeclStmtStmtName !=
                    StatementRef(kDeclPtrStmtStmtName));

      INFO("Both null declaration pointer");
      REQUIRE_FALSE(kEntityRefIdX != EntityRef(kIdX));
      REQUIRE_FALSE(kEntityRefWild != EntityRef());

      REQUIRE_FALSE(kStmtRefWild != StatementRef());

      INFO("Same Declaration, Different Declaration pointer");
      REQUIRE_FALSE(kEntityRefDeclVarNoName !=
                    EntityRef(kDeclPtrVarNoNameDuplicate));
      REQUIRE_FALSE(kStmtRefDeclStmtStmtName !=
                    StatementRef(kDeclPtrStmtStmtNameDuplicate));
    }
  }
}

TEST_CASE("Test IsDeclared of EntityRef", "[pql][entityref][isdeclared]") {
  SECTION("NEGATIVE") {
    REQUIRE_FALSE(EntityRef(test::pql::data::kEmptyStr).IsDeclared());
  }
}

TEST_CASE("Test == of EntityRef", "[pql][entityref][operator==]") {
  SECTION("Positive") {
    INFO("Same Wildcard, Declaration and id");
    REQUIRE(kEntityRefWild == EntityRef());
    REQUIRE(kEntityRefDeclVarNoName == EntityRef(kDeclPtrVarNoName));
    REQUIRE(kEntityRefDeclStmtStmtName == EntityRef(kDeclPtrStmtStmtName));
    REQUIRE(kEntityRefIdX == EntityRef(test::pql::ref::kIdX));
  }

  SECTION("Negative") {
    INFO("Different id, Same Wildcard and Declaration");
    REQUIRE_FALSE(kEntityRefIdX == kEntityRefIdY);
  }
}

TEST_CASE("Test != of EntityRef", "[pql][entityref][operator!=]") {
  SECTION("Positive") {
    INFO("Different id, Same Wildcard and Declaration");
    REQUIRE(kEntityRefIdX != kEntityRefIdY);
  }

  SECTION("Negative") {
    INFO("Same Wildcard, Declaration and id");
    REQUIRE_FALSE(kEntityRefWild != EntityRef());
    REQUIRE_FALSE(kEntityRefDeclVarNoName != EntityRef(kDeclPtrVarNoName));
    REQUIRE_FALSE(kEntityRefDeclStmtStmtName !=
                  EntityRef(kDeclPtrStmtStmtName));
    REQUIRE_FALSE(kEntityRefIdX != EntityRef(test::pql::ref::kIdX));
  }
}

TEST_CASE("Test constructor of StatementRef", "[pql][statementref]") {
  SECTION("Positive") {
    INFO("Declaration is a statement");
    REQUIRE_NOTHROW(StatementRef(test::pql::ref::kDeclPtrStmtStmtName));
  }
  SECTION("Negative") {
    INFO("Declaration is not a statement");
    REQUIRE_THROWS_AS(StatementRef(test::pql::ref::kDeclPtrVarNoName),
                      QueryParseException);
    REQUIRE_THROWS_AS(StatementRef(kDeclPtrEmpty), QueryParseException);
  }
}

TEST_CASE("Test IsDeclared of StatementRef",
          "[pql][statementref][isdeclared]") {
  SECTION("NEGATIVE") { REQUIRE_FALSE(StatementRef(0).IsDeclared()); }
}

TEST_CASE("Test == of StatementRef", "[pql][statementref][operator==]") {
  SECTION("Positive") {
    INFO("Same Wildcard, Declaration and Statement Number");
    REQUIRE(kStmtRefWild == StatementRef());
    REQUIRE(kStmtRefDeclStmtStmtName ==
            StatementRef(test::pql::ref::kDeclPtrStmtStmtName));
    REQUIRE(kStmtRefStmtNo3 == StatementRef(test::pql::ref::kStmtNo3));
  }

  SECTION("Negative") {
    INFO("Different Statement Number, Same Wildcard and Declaration");
    REQUIRE_FALSE(kStmtRefStmtNo2 == kStmtRefStmtNo3);
  }
}

TEST_CASE("Test != of StatementRef", "[pql][statementref][operator!=]") {
  SECTION("Positive") {
    INFO("Different Statement Number, Same Wildcard and Declaration");
    REQUIRE(kStmtRefStmtNo2 != kStmtRefStmtNo3);
  }

  SECTION("Negative") {
    INFO("Same Wildcard, Declaration and Statement Number");
    REQUIRE_FALSE(kStmtRefWild != StatementRef());
    REQUIRE_FALSE(kStmtRefDeclStmtStmtName !=
                  StatementRef(test::pql::ref::kDeclPtrStmtStmtName));
    REQUIRE_FALSE(kStmtRefStmtNo3 != StatementRef(test::pql::ref::kStmtNo3));
  }
}
