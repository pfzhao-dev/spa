#include "TestDeclaration.h"

#include "catch.hpp"
#include "pql/QueryException.h"

using namespace test::pql::declaration;

TEST_CASE("Test == of Declaration", "[pql][declaration][operator==]") {
  SECTION("Positive") {
    INFO("Type: stmt, Name: \"\"");
    REQUIRE(kDeclStmtNoName == Declaration(Declaration::Type::kStmt, ""));

    INFO("Type: stmt, Name: \"" + kNameStmt + "\"");
    REQUIRE(kDeclStmtStmtName ==
            Declaration(Declaration::Type::kStmt, kNameStmt));

    INFO("Type: constant, Type: Value, Name: \"" + kNameConst + "\"");
    REQUIRE(kDeclConstConstNameConstValue ==
            Declaration(Declaration::Type::kConstant,
                        Declaration::ValueType::kValue, kNameConst));
  }
  SECTION("Negative") {
    INFO("Different Type, Same Value and Name");
    REQUIRE_FALSE(kDeclStmtNoName == kDeclVarNoName);

    INFO("Different Name, Same Type and Value");
    REQUIRE_FALSE(kDeclVarNoName == kDeclVarVarName);

    INFO("Different Value, Same Name and Type");
    REQUIRE_FALSE(kDeclConstConstName == kDeclConstConstNameConstValue);
  }
}

TEST_CASE("Test != of Declaration", "[pql][declaration][operator!=]") {
  SECTION("Positive") {
    INFO("Different Type, Same Value and Name");
    REQUIRE(kDeclStmtNoName != kDeclVarNoName);

    INFO("Different Name, Same Type and Value");
    REQUIRE(kDeclVarNoName != kDeclVarVarName);

    INFO("Different Value, Same Name and Type");
    REQUIRE(kDeclConstConstName != kDeclConstConstNameConstValue);
  }
  SECTION("Negative") {
    INFO("Type: stmt, Name: \"\"");
    REQUIRE_FALSE(kDeclStmtNoName != Declaration(Declaration::Type::kStmt, ""));

    INFO("Type: stmt, Name: \"" + kNameStmt + "\"");
    REQUIRE_FALSE(kDeclStmtStmtName !=
                  Declaration(Declaration::Type::kStmt, kNameStmt));

    INFO("Type: constant, Type: Value, Name: \"" + kNameConst + "\"");
    REQUIRE_FALSE(kDeclConstConstNameConstValue !=
                  Declaration(Declaration::Type::kConstant,
                              Declaration::ValueType::kValue, kNameConst));
  }
}

TEST_CASE("Test IsStatement of Declaration",
          "[pql][declaration][isstatement]") {
  SECTION("Positive") { REQUIRE(kDeclStmtNoName.IsStatement()); }
  SECTION("Negative") {
    REQUIRE_FALSE(kDeclVarVarName.IsStatement());
    REQUIRE_FALSE(kDeclProcNoName.IsStatement());
    REQUIRE_FALSE(kDeclConstConstName.IsStatement());
  }
}

TEST_CASE("Test IsTypeIn of Declaration", "[pql][declaration][istypein]") {
  SECTION("Positive") {
    REQUIRE(kDeclStmtNoName.IsTypeIn({Declaration::Type::kStmt}));
    REQUIRE(kDeclProcNoName.IsTypeIn(
        {Declaration::Type::kProcedure, Declaration::Type::kStmt}));
  }
  SECTION("Negative") {
    REQUIRE_FALSE(kDeclVarVarName.IsTypeIn({}));
    REQUIRE_FALSE(kDeclProcNoName.IsTypeIn({Declaration::Type::kStmt}));
    REQUIRE_FALSE(kDeclConstConstName.IsTypeIn(
        {Declaration::Type::kStmt, Declaration::Type::kCall}));
  }
}

TEST_CASE("Test IsValueTypeIn of Declaration",
          "[pql][declaration][isvaluetypein]") {
  SECTION("Positive") {
    REQUIRE(kDeclConstConstNameConstValue.IsValueTypeIn(
        {Declaration::ValueType::kValue, Declaration::ValueType::kProcName}));
  }
  SECTION("Negative") {
    REQUIRE_FALSE(kDeclVarVarName.IsValueTypeIn({}));
    REQUIRE_FALSE(
        kDeclProcNoName.IsValueTypeIn({Declaration::ValueType::kStmtNum}));
  }
}

TEST_CASE("Test HasValue of Declaration", "[pql][declaration][hasvalue]") {
  SECTION("Positive") { REQUIRE(kDeclConstConstNameConstValue.HasValue()); }
  SECTION("Negative") {
    REQUIRE_FALSE(kDeclConstConstName.HasValue());
    REQUIRE_FALSE(kDeclProcNoName.HasValue());
  }
}

TEST_CASE("Test HasName of Declaration", "[pql][declaration][hasname]") {
  SECTION("Positive") {
    REQUIRE(kDeclStmtStmtName.HasName());
    REQUIRE(kDeclVarVarName.HasName());
    REQUIRE(kDeclConstConstName.HasName());
  }
  SECTION("Negative") {
    REQUIRE_FALSE(kDeclStmtNoName.HasName());
    REQUIRE_FALSE(kDeclVarNoName.HasName());
    REQUIRE_FALSE(kDeclProcNoName.HasName());
  }
}

TEST_CASE("Test GetReturnType of Declaration",
          "[pql][declaration][getreturntype]") {
  SECTION("Positive") {
    SECTION("Declaration has no value") {
      INFO("Return type is int");
      REQUIRE(kDeclStmtNoName.GetReturnType() ==
              Declaration::ReturnType::kInteger);
      REQUIRE(kDeclStmtStmtName.GetReturnType() ==
              Declaration::ReturnType::kInteger);
      REQUIRE(kDeclWhileWhileName.GetReturnType() ==
              Declaration::ReturnType::kInteger);
      REQUIRE(kDeclIfIfName.GetReturnType() ==
              Declaration::ReturnType::kInteger);
      REQUIRE(kDeclAssignAssignName.GetReturnType() ==
              Declaration::ReturnType::kInteger);
      REQUIRE(kDeclConstConstName.GetReturnType() ==
              Declaration::ReturnType::kInteger);

      INFO("Return type is name");
      REQUIRE(kDeclVarNoName.GetReturnType() == Declaration::ReturnType::kName);
      REQUIRE(kDeclProcNoName.GetReturnType() ==
              Declaration::ReturnType::kName);
      REQUIRE(kDeclProcProcName.GetReturnType() ==
              Declaration::ReturnType::kName);
    }

    SECTION("Declaration has value") {
      INFO("Return type is name");
      REQUIRE(kDeclProcProcNameProcName.GetReturnType() ==
              Declaration::ReturnType::kName);
      REQUIRE(kDeclCallCallNameProcName.GetReturnType() ==
              Declaration::ReturnType::kName);
      REQUIRE(kDeclVarVarNameVarName.GetReturnType() ==
              Declaration::ReturnType::kName);
      REQUIRE(kDeclReadReadNameVarName.GetReturnType() ==
              Declaration::ReturnType::kName);
      REQUIRE(kDeclPrintPrintNameVarName.GetReturnType() ==
              Declaration::ReturnType::kName);

      INFO("Return type is int");
      REQUIRE(kDeclConstConstNameConstValue.GetReturnType() ==
              Declaration::ReturnType::kInteger);
      REQUIRE(kDeclStmtStmtNameStmtNum.GetReturnType() ==
              Declaration::ReturnType::kInteger);
      REQUIRE(kDeclReadReadNameStmtNum.GetReturnType() ==
              Declaration::ReturnType::kInteger);
      REQUIRE(kDeclPrintPrintNameStmtNum.GetReturnType() ==
              Declaration::ReturnType::kInteger);
      REQUIRE(kDeclCallCallNameStmtNum.GetReturnType() ==
              Declaration::ReturnType::kInteger);
      REQUIRE(kDeclWhileWhileNameStmtNum.GetReturnType() ==
              Declaration::ReturnType::kInteger);
      REQUIRE(kDeclIfPrintNameStmtNum.GetReturnType() ==
              Declaration::ReturnType::kInteger);
      REQUIRE(kDeclAssignAssignNameStmtNum.GetReturnType() ==
              Declaration::ReturnType::kInteger);
    }
  }
  SECTION("Negative") {
    SECTION("Declaration has value") {
      INFO("Invalid value type");
      auto invalid_decl = Declaration(Declaration::Type::kStmt,
                                      Declaration::ValueType::kProcName, "s");
      REQUIRE_THROWS_AS(invalid_decl.GetReturnType(), QueryParseException);
    }
  }
}

TEST_CASE("Test GetReturnTypeWithValue of Declaration",
          "[pql][declaration][getreturntype]") {
  SECTION("Positive") {
    INFO("Return type is name");
    REQUIRE(kDeclProcProcNameProcName.GetReturnTypeWithValue() ==
            Declaration::ReturnType::kName);
    REQUIRE(kDeclCallCallNameProcName.GetReturnTypeWithValue() ==
            Declaration::ReturnType::kName);
    REQUIRE(kDeclVarVarNameVarName.GetReturnTypeWithValue() ==
            Declaration::ReturnType::kName);
    REQUIRE(kDeclReadReadNameVarName.GetReturnTypeWithValue() ==
            Declaration::ReturnType::kName);
    REQUIRE(kDeclPrintPrintNameVarName.GetReturnTypeWithValue() ==
            Declaration::ReturnType::kName);

    INFO("Return type is int");
    REQUIRE(kDeclConstConstNameConstValue.GetReturnTypeWithValue() ==
            Declaration::ReturnType::kInteger);
    REQUIRE(kDeclStmtStmtNameStmtNum.GetReturnTypeWithValue() ==
            Declaration::ReturnType::kInteger);
    REQUIRE(kDeclReadReadNameStmtNum.GetReturnTypeWithValue() ==
            Declaration::ReturnType::kInteger);
    REQUIRE(kDeclPrintPrintNameStmtNum.GetReturnTypeWithValue() ==
            Declaration::ReturnType::kInteger);
    REQUIRE(kDeclCallCallNameStmtNum.GetReturnTypeWithValue() ==
            Declaration::ReturnType::kInteger);
    REQUIRE(kDeclWhileWhileNameStmtNum.GetReturnTypeWithValue() ==
            Declaration::ReturnType::kInteger);
    REQUIRE(kDeclIfPrintNameStmtNum.GetReturnTypeWithValue() ==
            Declaration::ReturnType::kInteger);
    REQUIRE(kDeclAssignAssignNameStmtNum.GetReturnTypeWithValue() ==
            Declaration::ReturnType::kInteger);
  }
  SECTION("Negative") {
    INFO("No value");
    REQUIRE_THROWS_AS(kDeclStmtStmtName.GetReturnTypeWithValue(),
                      QueryParseException);

    INFO("Invalid value type");
    auto invalid_decl = Declaration(Declaration::Type::kStmt,
                                    Declaration::ValueType::kProcName, "s");
    REQUIRE_THROWS_AS(invalid_decl.GetReturnTypeWithValue(),
                      QueryParseException);
  }
}
