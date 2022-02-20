#include "TestParser.h"

#include "TestQuery.h"
#include "catch.hpp"
#include "data/DeclarationData.h"
#include "data/QueryData.h"
#include "data/QueryStringBuilder.h"
#include "pql/Parser.h"
#include "pql/QueryException.h"

namespace test {

namespace pql {

using namespace spa::pql;

void TestParserPositive(
    std::function<std::vector<data::ClausesPair>(Declaration::Type)>
        get_clauses) {
  std::shared_ptr<Declaration> expected_declaration(nullptr);
  Parser pql_parser = Parser();

  for (auto const& [declaration_type, declaration_name] :
       data::declaration::kDeclTypeNamePairs) {
    expected_declaration =
        std::make_shared<Declaration>(declaration_type, declaration_name);

    for (data::ClausesPair relation_pair : get_clauses(declaration_type)) {
      data::MakeAssignString(declaration_type, declaration_name);

      std::string input_query =
          data::declaration::kDefaultDecls +
          data::MakeAssignString(declaration_type, declaration_name) +
          data::MakeSelectString(declaration_name) +
          relation_pair.string_func_(declaration_name);

      INFO("Test case: \"" << input_query << "\"");

      std::shared_ptr<Result> expected_result =
          std::make_shared<DeclaredResult>(
              DeclaredResult({expected_declaration}));
      Query expected_query(expected_result,
                           relation_pair.clauses_func_(expected_declaration));
      REQUIRE(pql_parser.Parse(input_query) == expected_query);
    }
  }
}

void TestParserNegative(
    std::function<std::vector<data::StringToStringFunc>(Declaration::Type)>
        get_clauses_str) {
  std::shared_ptr<Declaration> expected_declaration(nullptr);
  Parser pql_parser = Parser();

  for (auto const& [declaration_type, declaration_name] :
       data::declaration::kDeclTypeNamePairs) {
    expected_declaration =
        std::make_shared<Declaration>(declaration_type, declaration_name);

    for (data::StringToStringFunc get_query_string :
         get_clauses_str(declaration_type)) {
      std::string input_query =
          data::declaration::kDefaultDecls +
          data::MakeAssignString(declaration_type, declaration_name) +
          data::MakeSelectString(declaration_name) +
          get_query_string(declaration_name);

      INFO("Test case: \"" << input_query << "\"");

      REQUIRE_THROWS_AS(pql_parser.Parse(input_query), QueryParseException);
    }
  }
}

}  //  namespace pql

};  // namespace test

using namespace test::pql;

TEST_CASE("Test parser Declaration", "[pql][parser][parse]") {
  Parser parser;
  std::shared_ptr<Declaration> expected_declaration(nullptr);

  SECTION("POSITIVE") {
    expected_declaration = std::make_shared<Declaration>(
        Declaration(Declaration::Type::kAssign, "a"));
    std::shared_ptr<Result> expected_result = std::make_shared<DeclaredResult>(
        DeclaredResult({expected_declaration}));

    REQUIRE(parser.Parse("assign a; assign assign;\nstmt c, d12, "
                         "Select,pattern,Follows,Modifies,suchthat"
                         "   ;stmt e,stmt;  procedure p ;Select a 	") ==
            Query(expected_result, {}));
  }

  SECTION("NEGATIVE") {
    SECTION("No space between declaration type and name") {
      REQUIRE_THROWS_AS(parser.Parse("assigna; stmt c; Select c "),
                        QueryParseException);
    }

    SECTION("No ;") {
      REQUIRE_THROWS_AS(parser.Parse("stmt c Select c "), QueryParseException);
    }

    SECTION("No declaration") {
      REQUIRE_THROWS_AS(parser.Parse(" Select c "), QueryParseException);
    }

    SECTION("Declaration is invalid") {
      REQUIRE_THROWS_AS(parser.Parse("prog a; Select a "), QueryParseException);

      INFO("Declaration name starts with a number");
      REQUIRE_THROWS_AS(parser.Parse("assign 1; stmt c; Select c "),
                        QueryParseException);
      REQUIRE_THROWS_AS(parser.Parse("assign 1f1; stmt c; Select c "),
                        QueryParseException);
    }
  }
}

TEST_CASE("Test parser Select", "[pql][parser][parse]") {
  Parser parser;
  std::shared_ptr<Declaration> expected_declaration(nullptr);

  SECTION("POSITIVE") {
    SECTION("Select boolean") {
      REQUIRE(parser.Parse("Select BOOLEAN") == query::kQueryBoolEmpty);
      REQUIRE(parser.Parse("Select  BOOLEAN ") == query::kQueryBoolEmpty);
    }

    SECTION("Select tuple") {
      REQUIRE(parser.Parse("stmt s; constant c;assign a, b;Select<s,c,"
                           "c.value,a>such that Follows(2, _)") ==
              query::kQueryMultipleDecls);
      REQUIRE(parser.Parse("stmt s; constant c;assign a, b; Select < s ,  c ,  "
                           " c . value ,  a > such that Follows (  2 , _ )") ==
              query::kQueryMultipleDecls);
      REQUIRE_NOTHROW(parser.Parse("stmt BOOLEAN; Select <BOOLEAN>"));
    }
  }

  SECTION("NEGATIVE") {
    SECTION("No Select") {
      REQUIRE_THROWS_AS(parser.Parse("assign a;"), QueryParseException);
    }

    SECTION("No space between \"Select\" and selected entities") {
      REQUIRE_THROWS_AS(parser.Parse("assigna; stmt c; Selectc "),
                        QueryParseException);
      REQUIRE_THROWS_AS(parser.Parse("assigna; stmt c; SelectBOOLEAN "),
                        QueryParseException);
    }

    SECTION("Declaration selected is invalid") {
      INFO("Declaration selected starts with a number");
      REQUIRE_THROWS_AS(parser.Parse("assign a; Select 1 "),
                        QueryParseException);
      REQUIRE_THROWS_AS(parser.Parse("assign a; Select 1fd "),
                        QueryParseException);
    }

    SECTION("No space after declaration selected ") {
      REQUIRE_THROWS_AS(
          parser.Parse("assign a;Select asuch that Follows(1, 2) "),
          QueryParseException);
      REQUIRE_THROWS_AS(
          parser.Parse("assign a;Select BOOLEANsuch that Follows(1, 2) "),
          QueryParseException);
      REQUIRE_THROWS_AS(
          parser.Parse("call call; Select call.stmt#such that Follows(1, 2)"),
          QueryParseException);
    }

    SECTION("Declaration value selected is invalid") {
      REQUIRE_THROWS_AS(parser.Parse("assign a; Select a.stmt "),
                        QueryParseException);
      REQUIRE_THROWS_AS(parser.Parse("procedure p; Select p.procname "),
                        QueryParseException);
    }
  }
}

//// a lot of assertions
//// stress test to run only when needed
// TEST_CASE("Test parsing of with 1 relation and 1 pattern and 1 with",
//          "[pql][parser]") {
//  // interleaves pattern, such that and with test cases
//  SECTION("Positive") { TestParserPositive(data::kGetClausesPositive); }
//  SECTION("Negative") { TestParserNegative(data::kGetFuncsNegative); }
//}

TEST_CASE("Test parsing of multiple relations and patterns",
          "[pql][parser][parse]") {
  Parser parser;
  std::shared_ptr<Declaration> expected_declaration(nullptr);

  SECTION("POSITIVE") {
    expected_declaration = std::make_shared<Declaration>(
        Declaration(Declaration::Type::kAssign, "a"));
    std::string input_query;

    SECTION("1 clause in each pattern, such that and with") {
      input_query =
          "assign a; prog_line s; constant c; print prr;procedure p, procName; "
          "variable v; while w;if i;Select a	such that Parent* (2, s) such "
          "that Next* ( 3, _) such that Affects*(2, a) pattern a (v, _) with "
          "\"x\"=prr.varName such that Uses(s,_) such that Affects(2, _) "
          "pattern a (_, _\"(x * y % 5) + z / 2\"_) such that "
          "Calls*(\"x\", _)pattern i (\"x\", _ , _) pattern w (v, _) such that "
          "Calls(_, procName)with   s = c.value";
      INFO("Test case: \"" << input_query << "\"");

      Query actual_query = parser.Parse(input_query);
      REQUIRE(actual_query == query::kQueryMultipleClauses);
    }

    SECTION(
        "multiple clauses in each pattern and such that, minimal whitespace") {
      input_query =
          "assign a; prog_line s;constant c;while w;print prr;procedure "
          "p,procName;if i;variable v;Select a with s=c.value and "
          "\"x\"=prr.varName such that Parent*(2, s)and Uses(s,_)and "
          "Next*(3,_)and Affects*(2,a)pattern a(v,_)and w(v,_)and "
          "a(_,_\"(x * y % 5) + z / 2\"_)and i(\"x\",_,_)such that "
          "Affects(2,_)and Calls*(\"x\",_)and Calls(_,procName)";
      INFO("Test case: \"" << input_query << "\"");

      Query actual_query = parser.Parse(input_query);
      REQUIRE(actual_query == query::kQueryMultipleClauses);
    }

    SECTION(
        "multiple clauses in each pattern and such that, extra "
        "whitespace") {
      input_query =
          "assign a ; prog_line s ; constant c ; while  w ;    if i ; "
          "procedure p , procName ; variable v ; print prr ;    Select a such "
          "that Affects* ( 2 , a )  and Affects ( 2 ,  _ ) such  that   "
          "Parent* ( 2 , s )\tand Uses ( s , _ )\n \tpattern a ( v ,  _ ) and "
          "w   ( v , _ )   and a ( _ ,  _ \"  (x * y % 5) + z / 2\"  _ ) and "
          "i ( \" \n\t x \t \"  , _ , _ ) with \"\n x \t\" = prr \t.\nvarName "
          "and s = c . value such that Next* ( 3 , _ )  and "
          "Calls* ( \" x \" ,  _ )  and Calls ( _ , procName ) ";
      INFO("Test case: \"" << input_query << "\"");

      Query actual_query = parser.Parse(input_query);
      REQUIRE(actual_query == query::kQueryMultipleClauses);
    }

    SECTION("BOOLEAN keyword") {
      REQUIRE_NOTHROW(parser.Parse("stmt BOOLEAN; Select BOOLEAN"));
    }

    SECTION("and keyword") {
      REQUIRE_NOTHROW(parser.Parse(
          "stmt and; Select and such that Follows(and, and) and "
          "Parent(and, and) with and.stmt# = 2 and and.stmt# = 2"));
      REQUIRE_NOTHROW(
          parser.Parse("assign and; Select and pattern and (_, _) and  "
                       "and (_, _) with and.stmt# = 2 and and.stmt# = 2"));
    }

    SECTION("with keyword") {
      REQUIRE_NOTHROW(
          parser.Parse("assign with; Select with pattern with (_, _) and  "
                       "with (_, _) with with.stmt# = 2 and with.stmt# = 2"));
    }

    SECTION("multiple with clauses") {
      REQUIRE_NOTHROW(parser.Parse(
          "prog_line n; stmt s, s1; Select s.stmt# such that "
          "Follows* (s, s1) with s1.stmt#=n and n=13 with s.stmt#=0"));
    }
  }
}