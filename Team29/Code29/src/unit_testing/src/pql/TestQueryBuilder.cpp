#include "TestQuery.h"
#include "catch.hpp"
#include "data/QueryDataUtil.h"
#include "pql/QueryBuilder.h"
#include "pql/QueryException.h"

using namespace test::pql;
using namespace query;

TEST_CASE("Test AddDeclaration and AddDeclarationProgLine of QueryBuilder",
          "[pql][querybuilder][adddeclaration]") {
  QueryBuilder query_builder;
  SECTION("POSITIVE") {
    SECTION("AddDeclaration") {
      query_builder.AddDeclaration(Declaration::Type::kStmt,
                                   declaration::kNameStmt);
      query_builder.SelectDeclaration(declaration::kNameStmt);
      REQUIRE(query_builder.Build() == kQueryStmtNameEmpty);
    }

    SECTION("AddDeclarationProgLine") {
      query_builder.AddDeclarationProgLine(declaration::kNameStmt);
      query_builder.SelectDeclaration(declaration::kNameStmt);
      REQUIRE(query_builder.Build() == kQueryStmtNameEmpty);
    }
  }

  SECTION("NEGATIVE") {
    SECTION("No declaration name") {
      REQUIRE_THROWS_AS(query_builder.AddDeclaration(Declaration::Type::kPrint,
                                                     data::kEmptyStr),
                        QueryParseException);
    }
    SECTION("Declaration already added") {
      query_builder.AddDeclaration(Declaration::Type::kConstant,
                                   declaration::kNameConst);
      REQUIRE_THROWS_AS(query_builder.AddDeclaration(Declaration::Type::kPrint,
                                                     declaration::kNameConst),
                        QueryParseException);
    }
  }
}

TEST_CASE("Test SelectBoolean of QueryBuilder",
          "[pql][querybuilder][selectboolean]") {
  QueryBuilder query_builder;
  SECTION("POSITIVE") {
    query_builder.SelectBoolean();
    REQUIRE(query_builder.Build() == kQueryBoolEmpty);
  }
}

TEST_CASE("Test SelectDeclaration of QueryBuilder",
          "[pql][querybuilder][selectdeclaration]") {
  QueryBuilder query_builder;
  query_builder.AddDeclaration(Declaration::Type::kRead,
                               declaration::kNameRead);
  query_builder.AddDeclaration(Declaration::Type::kPrint,
                               declaration::kNamePrint);
  query_builder.AddDeclaration(Declaration::Type::kCall,
                               declaration::kNameCall);
  query_builder.AddDeclaration(Declaration::Type::kWhile,
                               declaration::kNameWhile);
  query_builder.AddDeclaration(Declaration::Type::kIf, declaration::kNameIf);
  query_builder.AddDeclaration(Declaration::Type::kAssign,
                               declaration::kNameAssign);
  query_builder.AddDeclaration(Declaration::Type::kVariable,
                               declaration::kNameVar);
  query_builder.AddDeclaration(Declaration::Type::kConstant,
                               declaration::kNameConst);
  query_builder.AddDeclaration(Declaration::Type::kProcedure,
                               declaration::kNameProc);

  SECTION("POSITIVE") {
    SECTION("No value") {
      query_builder.SelectDeclaration(declaration::kNameConst);
      REQUIRE(query_builder.Build() == kQueryConstNameEmpty);
    }

    SECTION("Valid value") {
      query_builder.AddDeclaration(Declaration::Type::kStmt,
                                   declaration::kNameStmt);

      // names
      // procName val type
      INFO("procedure.procName, call.procName");
      std::string val_type =
          kDeclValTypeToString.at(Declaration::ValueType::kProcName);
      std::vector<std::string> names_of_valid_types = {declaration::kNameProc,
                                                       declaration::kNameCall};
      for (auto name : names_of_valid_types) {
        REQUIRE_NOTHROW(query_builder.SelectDeclaration(name, val_type));
      }

      // varName val type
      INFO("variable.varName, read.varName, print.varName");
      val_type = kDeclValTypeToString.at(Declaration::ValueType::kVarName);
      names_of_valid_types = {declaration::kNameVar, declaration::kNameRead,
                              declaration::kNamePrint};
      for (auto name : names_of_valid_types) {
        REQUIRE_NOTHROW(query_builder.SelectDeclaration(name, val_type));
      }

      // integers
      // value val type
      INFO("constant.value");
      val_type = kDeclValTypeToString.at(Declaration::ValueType::kValue);
      names_of_valid_types = {declaration::kNameConst};
      for (auto name : names_of_valid_types) {
        REQUIRE_NOTHROW(query_builder.SelectDeclaration(name, val_type));
      }

      // stmt# val type
      val_type = kDeclValTypeToString.at(Declaration::ValueType::kStmtNum);
      INFO(
          "stmt.stmt#, read.stmt#, print.stmt#, call.stmt#, while.stmt#, "
          "if.stmt#, assign.stmt#");
      names_of_valid_types = {declaration::kNameStmt,  declaration::kNameRead,
                              declaration::kNamePrint, declaration::kNameCall,
                              declaration::kNameWhile, declaration::kNameIf,
                              declaration::kNameAssign};
      for (auto name : names_of_valid_types) {
        REQUIRE_NOTHROW(query_builder.SelectDeclaration(name, val_type));
      }
    }
  }

  SECTION("NEGATIVE") {
    query_builder.AddDeclaration(Declaration::Type::kStmt,
                                 declaration::kNameStmt);

    SECTION("No declaration name") {
      REQUIRE_THROWS_AS(query_builder.SelectDeclaration(data::kEmptyStr),
                        QueryParseException);
    }

    SECTION("Declaration to select not declared") {
      REQUIRE_THROWS_AS(query_builder.SelectDeclaration("undeclared"),
                        QueryParseException);
    }

    SECTION("Invalid attribute") {
      // procName val type
      std::string val_type =
          kDeclValTypeToString.at(Declaration::ValueType::kProcName);
      std::vector<std::string> names_of_invalid_types = {
          declaration::kNameStmt,  declaration::kNameRead,
          declaration::kNamePrint, declaration::kNameWhile,
          declaration::kNameIf,    declaration::kNameAssign,
          declaration::kNameVar,   declaration::kNameConst};
      for (auto name : names_of_invalid_types) {
        REQUIRE_THROWS_AS(query_builder.SelectDeclaration(name, val_type),
                          QueryParseException);
      }

      // varName val type
      val_type = kDeclValTypeToString.at(Declaration::ValueType::kVarName);
      names_of_invalid_types = {
          declaration::kNameStmt,   declaration::kNameCall,
          declaration::kNameWhile,  declaration::kNameIf,
          declaration::kNameAssign, declaration::kNameConst,
          declaration::kNameProc};
      for (auto name : names_of_invalid_types) {
        REQUIRE_THROWS_AS(query_builder.SelectDeclaration(name, val_type),
                          QueryParseException);
      }

      // value val type
      val_type = kDeclValTypeToString.at(Declaration::ValueType::kValue);
      names_of_invalid_types = {
          declaration::kNameStmt,   declaration::kNameRead,
          declaration::kNamePrint,  declaration::kNameCall,
          declaration::kNameWhile,  declaration::kNameIf,
          declaration::kNameAssign, declaration::kNameVar,
          declaration::kNameProc};
      for (auto name : names_of_invalid_types) {
        REQUIRE_THROWS_AS(query_builder.SelectDeclaration(name, val_type),
                          QueryParseException);
      }

      // stmt# val type
      val_type = kDeclValTypeToString.at(Declaration::ValueType::kStmtNum);
      names_of_invalid_types = {declaration::kNameVar, declaration::kNameConst,
                                declaration::kNameProc};
      for (auto name : names_of_invalid_types) {
        REQUIRE_THROWS_AS(query_builder.SelectDeclaration(name, val_type),
                          QueryParseException);
      }
    }
  }
}

TEST_CASE(
    "Test AddRelationFollows, AddRelationParent and AddRelationNext of "
    "QueryBuilder",
    "[pql][querybuilder][addrelationfollows][addrelationparent]["
    "addrelationnext]") {
  QueryBuilder query_builder;
  query_builder.AddDeclaration(Declaration::Type::kStmt,
                               declaration::kNameStmt);

  SECTION("POSITIVE") {
    query_builder.SelectDeclaration(declaration::kNameStmt);

    SECTION("Follows(2, _)") {
      query_builder.AddRelationFollows("2", data::kWildcardStr, false);
      REQUIRE(query_builder.Build() == kQueryStmtNameFollowsStmt2Wild);
    }
    SECTION("Parent*(2, s)") {
      query_builder.AddRelationParent("2", declaration::kNameStmt, true);
      REQUIRE(query_builder.Build() == kQueryStmtNameParentTStmt2StmtName);
    }
    SECTION("Next*(3, _)") {
      query_builder.AddRelationNext("3", data::kWildcardStr, true, false);
      REQUIRE(query_builder.Build() == kQueryStmtNameNextTStmt3Wild);
    }
    SECTION("NextBip*(2, _)") {
      query_builder.AddRelationNext("2", data::kWildcardStr, true, true);
      REQUIRE(query_builder.Build() == kQueryStmtNameNextBipTStmt2Wild);
    }
    SECTION("NextBip(2, s)") {
      query_builder.AddRelationNext("2", declaration::kNameStmt, false, true);
      REQUIRE(query_builder.Build() == kQueryStmtNameNextBipStmt2StmtName);
    }
  }

  SECTION("NEGATIVE") {
    SECTION("Invalid statement number") {
      REQUIRE_THROWS_AS(
          query_builder.AddRelationFollows("0", declaration::kNameStmt, true),
          QueryParseException);
      REQUIRE_THROWS_AS(query_builder.AddRelationNext(
                            "0", declaration::kNameStmt, false, false),
                        QueryParseException);
    }

    SECTION("Declaration used is not declared") {
      REQUIRE_THROWS_AS(
          query_builder.AddRelationParent("1", "undeclaredref", true),
          QueryParseException);
      REQUIRE_THROWS_AS(
          query_builder.AddRelationNext("1", "undeclaredref", false, false),
          QueryParseException);
    }
  }
}

TEST_CASE("Test AddRelationUses and AddRelationModifies of QueryBuilder",
          "[pql][querybuilder][addrelationuses][addrelationmodifies]") {
  QueryBuilder query_builder;
  query_builder.AddDeclaration(Declaration::Type::kStmt,
                               declaration::kNameStmt);

  SECTION("POSITIVE") {
    query_builder.SelectDeclaration(declaration::kNameStmt);

    SECTION("Uses(s, _)") {
      query_builder.AddRelationUses(declaration::kNameStmt, data::kWildcardStr);
      REQUIRE(query_builder.Build() == kQueryStmtNameUsesStmtNameWild);
    }

    SECTION("Modifies(2, v)") {
      query_builder.AddDeclaration(Declaration::Type::kVariable,
                                   declaration::kNameVar);
      query_builder.AddRelationModifies("2", declaration::kNameVar);
      REQUIRE(query_builder.Build() == kQueryStmtNameModifiesStmtNo2VarVarName);
    }

    SECTION("Uses(\"x\", \"y\")") {
      query_builder.AddRelationUses("\"x\"", "\" y \"");
      REQUIRE(query_builder.Build() == kQueryStmtNameUsesIdXIdY);
    }
  }

  SECTION("NEGATIVE") {
    SECTION("Invalid subject") {
      REQUIRE_THROWS_AS(
          query_builder.AddRelationUses(data::kWildcardStr, "\"x\""),
          QueryParseException);
    }

    SECTION("Invalid statement number in subject") {
      REQUIRE_THROWS_AS(query_builder.AddRelationUses("0", "\"x\""),
                        QueryParseException);
    }

    SECTION("Declaration type used in subject is invalid") {
      query_builder.AddDeclaration(Declaration::Type::kVariable,
                                   declaration::kNameVar);

      REQUIRE_THROWS_AS(
          query_builder.AddRelationUses(declaration::kNameVar, "\"x\""),
          QueryParseException);
    }

    SECTION("Declaration used in subject is not declared") {
      REQUIRE_THROWS_AS(query_builder.AddRelationUses("undeclaredref", "\"x\""),
                        QueryParseException);
    }

    SECTION("Declaration type used in target is invalid") {
      REQUIRE_THROWS_AS(
          query_builder.AddRelationUses("1", declaration::kNameStmt),
          QueryParseException);
    }

    SECTION("Declaration used in target is not declared") {
      REQUIRE_THROWS_AS(query_builder.AddRelationUses("1", "undeclaredref"),
                        QueryParseException);
    }
  }
}

TEST_CASE("Test AddRelationAffects of QueryBuilder",
          "[pql][querybuilder][addrelationaffects]") {
  QueryBuilder query_builder;
  query_builder.AddDeclaration(Declaration::Type::kAssign,
                               declaration::kNameAssign);

  SECTION("POSITIVE") {
    query_builder.SelectDeclaration(declaration::kNameAssign);

    SECTION("Affects(2, _)") {
      query_builder.AddRelationAffects("2", data::kWildcardStr, false, false);
      REQUIRE(query_builder.Build() == kQueryAssignNameAffectsStmt2Wild);
    }
    SECTION("Affects*(2, a)") {
      query_builder.AddRelationAffects("2", declaration::kNameAssign, true,
                                       false);
      REQUIRE(query_builder.Build() == kQueryAssignNameAffectsTStmt2AssignName);
    }
    SECTION("AffectsBip(3, _)") {
      query_builder.AddRelationAffects("3", data::kWildcardStr, false, true);
      REQUIRE(query_builder.Build() == kQueryAssignNameAffectsBipStmt3Wild);
    }
    SECTION("AffectsBip*(2, a)") {
      query_builder.AddRelationAffects("2", declaration::kNameAssign, true,
                                       true);
      REQUIRE(query_builder.Build() ==
              kQueryAssignNameAffectsBipTStmt2AssignName);
    }
  }

  SECTION("NEGATIVE") {
    SECTION("Invalid statement number") {
      REQUIRE_THROWS_AS(query_builder.AddRelationAffects(
                            "0", declaration::kNameAssign, true, false),
                        QueryParseException);
    }

    SECTION("Declaration type is invalid") {
      query_builder.AddDeclaration(Declaration::Type::kPrint,
                                   declaration::kNamePrint);

      REQUIRE_THROWS_AS(query_builder.AddRelationAffects(
                            declaration::kNamePrint, "1", false, false),
                        QueryParseException);
    }

    SECTION("Declaration used is not declared") {
      REQUIRE_THROWS_AS(
          query_builder.AddRelationAffects("1", "undeclaredref", true, false),
          QueryParseException);
    }
  }
}

TEST_CASE("Test AddRelationCalls of QueryBuilder",
          "[pql][querybuilder][addrelationcalls]") {
  QueryBuilder query_builder;
  query_builder.AddDeclaration(Declaration::Type::kProcedure,
                               declaration::kNameProc);

  SECTION("POSITIVE") {
    query_builder.SelectDeclaration(declaration::kNameProc);

    SECTION("Calls(_, procName)") {
      query_builder.AddRelationCalls(data::kWildcardStr, declaration::kNameProc,
                                     false);
      REQUIRE(query_builder.Build() == kQueryProcNameCallsWildProcProcName);
    }

    SECTION("Calls*(\"x\", _)") {
      query_builder.AddRelationCalls("\"x\"", data::kWildcardStr, true);
      REQUIRE(query_builder.Build() == kQueryProcNameCallsIdXWild);
    }
  }

  SECTION("NEGATIVE") {
    SECTION("Declaration type used is invalid") {
      query_builder.AddDeclaration(Declaration::Type::kVariable,
                                   declaration::kNameVar);

      REQUIRE_THROWS_AS(
          query_builder.AddRelationCalls(declaration::kNameVar, "\"x\"", false),
          QueryParseException);
    }

    SECTION("Declaration used is not declared") {
      REQUIRE_THROWS_AS(
          query_builder.AddRelationCalls("\"x\"", "undeclaredref", true),
          QueryParseException);
    }
  }
}

TEST_CASE("Test AddPattern of QueryBuilder",
          "[pql][querybuilder][addpattern]") {
  QueryBuilder query_builder;
  query_builder.AddDeclaration(Declaration::Type::kAssign,
                               declaration::kNameAssign);
  query_builder.AddDeclaration(Declaration::Type::kVariable,
                               declaration::kNameVar);
  query_builder.AddDeclaration(Declaration::Type::kStmt,
                               declaration::kNameStmt);
  query_builder.AddDeclaration(Declaration::Type::kWhile,
                               declaration::kNameWhile);
  query_builder.AddDeclaration(Declaration::Type::kIf, declaration::kNameIf);

  SECTION("POSITIVE") {
    SECTION("Pattern type is assign") {
      query_builder.SelectDeclaration(declaration::kNameStmt);

      SECTION("a (v, _)") {
        query_builder.AddPattern(declaration::kNameAssign,
                                 declaration::kNameVar, data::kWildcardStr,
                                 data::kEmptyStr);
        REQUIRE(query_builder.Build() ==
                kQueryStmtNamePatternTypeAssignVarVarNameExprWild);
      }

      SECTION("a (_, _\"(x * y % 5) + z / 2\"_)") {
        query_builder.AddPattern(declaration::kNameAssign, data::kWildcardStr,
                                 "_ \" (x * y % 5) + z / 2  \" _",
                                 data::kEmptyStr);

        REQUIRE(query_builder.Build() ==
                kQueryStmtNamePatternTypeAssignVarWildExprPartialSpec1);
      }

      SECTION("a (\"x\", \"(x * y % 5) + z / 2\")") {
        query_builder.AddPattern(declaration::kNameAssign, "\" x \"",
                                 "\" (x * y % 5) + z / 2  \"", data::kEmptyStr);

        REQUIRE(query_builder.Build() ==
                kQueryStmtNamePatternTypeAssignVarXExprSpec1);
      }
    }

    SECTION("Pattern type is while") {
      query_builder.SelectDeclaration(declaration::kNameVar);

      SECTION("while (v, _)") {
        query_builder.AddPattern(declaration::kNameWhile, declaration::kNameVar,
                                 data::kWildcardStr, data::kEmptyStr);
        REQUIRE(query_builder.Build() ==
                kQueryVarNamePatternTypeWhileVarVarNameExprWild);
      }

      SECTION("while (\"x\", _)") {
        query_builder.AddPattern(declaration::kNameWhile, "\"x\"",
                                 data::kWildcardStr, data::kEmptyStr);
        REQUIRE(query_builder.Build() ==
                kQueryVarNamePatternTypeWhileVarXExprWild);
      }

      SECTION("while (_, _)") {
        query_builder.AddPattern(declaration::kNameWhile, data::kWildcardStr,
                                 data::kWildcardStr, data::kEmptyStr);
        REQUIRE(query_builder.Build() ==
                kQueryVarNamePatternTypeWhileVarWildExprWild);
      }
    }

    SECTION("Pattern type is if") {
      query_builder.SelectDeclaration(declaration::kNameVar);

      SECTION("if (v, _, _)") {
        query_builder.AddPattern(declaration::kNameIf, declaration::kNameVar,
                                 data::kWildcardStr, data::kWildcardStr);
        REQUIRE(query_builder.Build() ==
                kQueryVarNamePatternTypeIfVarVarNameExprWild);
      }

      SECTION("if (\"x\", _, _)") {
        query_builder.AddPattern(declaration::kNameIf, "\"x\"",
                                 data::kWildcardStr, data::kWildcardStr);
        REQUIRE(query_builder.Build() ==
                kQueryVarNamePatternTypeIfVarXExprWild);
      }

      SECTION("if (_, _, _)") {
        query_builder.AddPattern(declaration::kNameIf, data::kWildcardStr,
                                 data::kWildcardStr, data::kWildcardStr);
        REQUIRE(query_builder.Build() ==
                kQueryVarNamePatternTypeIfVarWildExprWild);
      }
    }
  }

  SECTION("NEGATIVE") {
    SECTION("Declaration type used in pattern type is invalid") {
      REQUIRE_THROWS_AS(
          query_builder.AddPattern(declaration::kNameStmt, data::kWildcardStr,
                                   clause::kSpec1Partial, data::kEmptyStr),
          QueryParseException);
    }

    SECTION("Declaration used in pattern type is not declared") {
      REQUIRE_THROWS_AS(
          query_builder.AddPattern("undeclaredassign", data::kWildcardStr,
                                   clause::kSpec1Partial, data::kEmptyStr),
          QueryParseException);
    }

    SECTION("Declaration type used in pattern var is invalid") {
      REQUIRE_THROWS_AS(query_builder.AddPattern(
                            declaration::kNameAssign, declaration::kNameStmt,
                            clause::kSpec1Partial, data::kEmptyStr),
                        QueryParseException);
    }

    SECTION("Declaration used in pattern var is not declared") {
      REQUIRE_THROWS_AS(
          query_builder.AddPattern(declaration::kNameAssign, "undeclaredvar",
                                   clause::kSpec1Partial, data::kEmptyStr),
          QueryParseException);
    }

    SECTION("Expression spec 2 for pattern type assign is inavlid") {
      REQUIRE_THROWS_AS(query_builder.AddPattern(
                            declaration::kNameAssign, declaration::kNameVar,
                            clause::kSpec1Partial, data::kWildcardStr),
                        QueryParseException);
    }

    SECTION("Expression spec for pattern type while is inavlid") {
      REQUIRE_THROWS_AS(query_builder.AddPattern(
                            declaration::kNameWhile, declaration::kNameVar,
                            clause::kSpec1Partial, data::kEmptyStr),
                        QueryParseException);
    }

    SECTION("Expression spec 2 for pattern type while is inavlid") {
      REQUIRE_THROWS_AS(query_builder.AddPattern(
                            declaration::kNameWhile, declaration::kNameVar,
                            data::kWildcardStr, data::kWildcardStr),
                        QueryParseException);
    }

    SECTION("Expression spec for pattern type if is inavlid") {
      REQUIRE_THROWS_AS(
          query_builder.AddPattern(declaration::kNameIf, declaration::kNameVar,
                                   clause::kSpec1Partial, data::kWildcardStr),
          QueryParseException);
    }

    SECTION("Expression spec 2 for pattern type if is inavlid") {
      REQUIRE_THROWS_AS(
          query_builder.AddPattern(declaration::kNameIf, declaration::kNameVar,
                                   data::kWildcardStr, data::kEmptyStr),
          QueryParseException);
    }
  }
}

TEST_CASE("Test AddWithCompare of QueryBuilder",
          "[pql][querybuilder][addwithcompare]") {
  QueryBuilder query_builder;
  query_builder.AddDeclaration(Declaration::Type::kRead,
                               declaration::kNameRead);
  query_builder.AddDeclaration(Declaration::Type::kPrint,
                               declaration::kNamePrint);
  query_builder.AddDeclaration(Declaration::Type::kCall,
                               declaration::kNameCall);
  query_builder.AddDeclaration(Declaration::Type::kWhile,
                               declaration::kNameWhile);
  query_builder.AddDeclaration(Declaration::Type::kIf, declaration::kNameIf);
  query_builder.AddDeclaration(Declaration::Type::kAssign,
                               declaration::kNameAssign);
  query_builder.AddDeclaration(Declaration::Type::kVariable,
                               declaration::kNameVar);
  query_builder.AddDeclaration(Declaration::Type::kConstant,
                               declaration::kNameConst);
  query_builder.AddDeclaration(Declaration::Type::kProcedure,
                               declaration::kNameProc);

  SECTION("POSITIVE") {
    SECTION("with s.stmt# = 2") {
      query_builder.AddDeclaration(Declaration::Type::kStmt,
                                   declaration::kNameStmt);
      query_builder.SelectDeclaration(declaration::kNameStmt);

      query_builder.AddWithCompare(
          declaration::kNameStmt, "2",
          kDeclValTypeToString.at(Declaration::ValueType::kStmtNum));
      REQUIRE(query_builder.Build() ==
              kQueryStmtNameCompareStmtStmtNumStmtNameStmtNo2);
    }

    SECTION("with s = c.value") {
      query_builder.AddDeclarationProgLine(declaration::kNameStmt);
      query_builder.SelectDeclaration(declaration::kNameStmt);

      query_builder.AddWithCompare(
          declaration::kNameStmt, declaration::kNameConst, data::kEmptyStr,
          kDeclValTypeToString.at(Declaration::ValueType::kValue));
      REQUIRE(query_builder.Build() ==
              kQueryStmtNameCompareStmtStmtNameConstNameConstValue);
    }

    SECTION("with \"x\" = prr.varName") {
      query_builder.AddDeclaration(Declaration::Type::kStmt,
                                   declaration::kNameStmt);
      query_builder.SelectDeclaration(declaration::kNameStmt);

      query_builder.AddWithCompare(
          "\" x \"", declaration::kNamePrint, data::kEmptyStr,
          kDeclValTypeToString.at(Declaration::ValueType::kVarName));
      REQUIRE(query_builder.Build() ==
              kQueryStmtNameCompareIdXPrintPrintNameVarName);
    }

    SECTION("Declaration value type is valid") {
      query_builder.AddDeclaration(Declaration::Type::kStmt,
                                   declaration::kNameStmt);
      query_builder.SelectDeclaration(declaration::kNameStmt);

      // names
      // procName val type
      std::string val_type =
          kDeclValTypeToString.at(Declaration::ValueType::kProcName);
      std::vector<std::string> names_of_valid_types = {declaration::kNameProc,
                                                       declaration::kNameCall};
      for (auto name : names_of_valid_types) {
        REQUIRE_NOTHROW(query_builder.AddWithCompare(name, "\"x\"", val_type));
      }

      // varName val type
      val_type = kDeclValTypeToString.at(Declaration::ValueType::kVarName);
      names_of_valid_types = {declaration::kNameVar, declaration::kNameRead,
                              declaration::kNamePrint};
      for (auto name : names_of_valid_types) {
        REQUIRE_NOTHROW(query_builder.AddWithCompare(
            "\"x\"", name, data::kEmptyStr, val_type));
      }

      // integers
      // value val type
      val_type = kDeclValTypeToString.at(Declaration::ValueType::kValue);
      names_of_valid_types = {declaration::kNameConst};
      for (auto name : names_of_valid_types) {
        REQUIRE_NOTHROW(
            query_builder.AddWithCompare("1", name, data::kEmptyStr, val_type));
      }

      // stmt# val type
      val_type = kDeclValTypeToString.at(Declaration::ValueType::kStmtNum);
      names_of_valid_types = {declaration::kNameStmt,  declaration::kNameRead,
                              declaration::kNamePrint, declaration::kNameCall,
                              declaration::kNameWhile, declaration::kNameIf,
                              declaration::kNameAssign};
      for (auto name : names_of_valid_types) {
        REQUIRE_NOTHROW(query_builder.AddWithCompare(name, "1", val_type));
      }
    }
  }

  SECTION("NEGATIVE") {
    SECTION("Invalid lhs") {
      REQUIRE_THROWS_AS(
          query_builder.AddWithCompare(data::kWildcardStr, "\"x\""),
          QueryParseException);
    }

    SECTION("Invalid statement number in lhs") {
      REQUIRE_THROWS_AS(query_builder.AddWithCompare("0", "\"x\""),
                        QueryParseException);
    }

    SECTION("Declaration used in lhs is not declared") {
      REQUIRE_THROWS_AS(query_builder.AddWithCompare("undeclaredref", "\"x\""),
                        QueryParseException);
    }

    SECTION("Declaration used in lhs is invalid") {
      query_builder.AddDeclarationProgLine(declaration::kNameStmt);

      REQUIRE_THROWS_AS(
          query_builder.AddWithCompare(
              declaration::kNameStmt, "\"x\"",
              kDeclValTypeToString.at(Declaration::ValueType::kStmtNum)),
          QueryParseException);
    }

    SECTION("Declaration used in rhs is invalid") {
      REQUIRE_THROWS_AS(
          query_builder.AddWithCompare(declaration::kNameConst, "1"),
          QueryParseException);
    }

    SECTION("Declaration value type in lhs is invalid") {
      // procName val type
      std::string val_type =
          kDeclValTypeToString.at(Declaration::ValueType::kProcName);
      std::vector<std::string> names_of_invalid_types = {
          declaration::kNameStmt,  declaration::kNameRead,
          declaration::kNamePrint, declaration::kNameWhile,
          declaration::kNameIf,    declaration::kNameAssign,
          declaration::kNameVar,   declaration::kNameConst};
      for (auto name : names_of_invalid_types) {
        REQUIRE_THROWS_AS(query_builder.AddWithCompare(
                              "\"x\"", name, data::kEmptyStr, val_type),
                          QueryParseException);
      }

      // varName val type
      val_type = kDeclValTypeToString.at(Declaration::ValueType::kVarName);
      names_of_invalid_types = {
          declaration::kNameStmt,   declaration::kNameCall,
          declaration::kNameWhile,  declaration::kNameIf,
          declaration::kNameAssign, declaration::kNameConst,
          declaration::kNameProc};
      for (auto name : names_of_invalid_types) {
        REQUIRE_THROWS_AS(query_builder.AddWithCompare(name, "\"x\"", val_type),
                          QueryParseException);
      }

      // value val type
      val_type = kDeclValTypeToString.at(Declaration::ValueType::kValue);
      names_of_invalid_types = {
          declaration::kNameStmt,   declaration::kNameRead,
          declaration::kNamePrint,  declaration::kNameCall,
          declaration::kNameWhile,  declaration::kNameIf,
          declaration::kNameAssign, declaration::kNameVar,
          declaration::kNameProc};
      for (auto name : names_of_invalid_types) {
        REQUIRE_THROWS_AS(
            query_builder.AddWithCompare("1", name, data::kEmptyStr, val_type),
            QueryParseException);
      }

      // stmt# val type
      val_type = kDeclValTypeToString.at(Declaration::ValueType::kStmtNum);
      names_of_invalid_types = {declaration::kNameVar, declaration::kNameConst,
                                declaration::kNameProc};
      for (auto name : names_of_invalid_types) {
        REQUIRE_THROWS_AS(query_builder.AddWithCompare(name, "1", val_type),
                          QueryParseException);
      }
    }

    SECTION("Return type of lhs does not match return type of rhs") {
      SECTION("s = procName.procName") {
        query_builder.AddDeclarationProgLine(declaration::kNameStmt);

        REQUIRE_THROWS_AS(
            query_builder.AddWithCompare(
                declaration::kNameStmt, declaration::kNameProc, data::kEmptyStr,
                kDeclValTypeToString.at(Declaration::ValueType::kProcName)),
            QueryParseException);
      }

      SECTION("\"x\" = 1") {
        REQUIRE_THROWS_AS(query_builder.AddWithCompare("\"x\"", "1"),
                          QueryParseException);
      }
    }
  }
}

TEST_CASE("Test Reset of QueryBuilder", "[pql][querybuilder][reset]") {
  QueryBuilder query_builder;
  query_builder.AddDeclaration(Declaration::Type::kStmt,
                               declaration::kNameStmt);
  query_builder.SelectDeclaration(declaration::kNameStmt);
  query_builder.Reset();
  INFO("Can add added declaration after reset");
  REQUIRE_NOTHROW(query_builder.AddDeclaration(Declaration::Type::kStmt,
                                               declaration::kNameStmt));
  INFO("Can select selected declaration after reset");
  REQUIRE_NOTHROW(query_builder.SelectDeclaration(declaration::kNameStmt));
}

TEST_CASE("Test Build of QueryBuilder", "[pql][querybuilder][build]") {
  QueryBuilder query_builder;
  SECTION("POSITIVE") {
    query_builder.AddDeclaration(Declaration::Type::kStmt,
                                 declaration::kNameStmt);
    query_builder.SelectDeclaration(declaration::kNameStmt);
    REQUIRE(query_builder.Build() == kQueryStmtNameEmpty);
  }

  SECTION("NEGATIVE") {
    SECTION("No declaration") {
      REQUIRE_THROWS_AS(query_builder.Build(), QueryParseException);
    }

    SECTION("No selected declaration") {
      query_builder.AddDeclaration(Declaration::Type::kConstant,
                                   declaration::kNameConst);
      REQUIRE_THROWS_AS(query_builder.Build(), QueryParseException);
    }
  }
}
