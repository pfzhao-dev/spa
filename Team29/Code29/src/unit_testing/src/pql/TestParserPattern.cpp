#include "TestParser.h"
#include "catch.hpp"
#include "data/clause/PatternData.h"
#include "pql/Parser.h"
#include "pql/Result.h"

TEST_CASE("can parse single clause pattern query string",
          "[pql][parser][pattern]") {
  // Assign entity
  auto assign_entity_decl = std::make_shared<spa::pql::Declaration>(
      spa::pql::Declaration::Type::kAssign, "a");
  auto assign_statement_ref =
      std::make_shared<spa::pql::StatementRef>(assign_entity_decl);

  auto pql_parser = spa::pql::Parser();

  // List of possible types of variable entity reference
  std::vector<std::tuple<std::string, std::string, spa::pql::EntityRef>>
      entity_ref_options = {
          {"variable reference", "v",
           spa::pql::EntityRef(std::make_shared<spa::pql::Declaration>(
               spa::pql::Declaration::Type::kVariable, "v"))},
          {"variable name", "\"varId\"", spa::pql::EntityRef("varId")},
          {"wildcard", "_", spa::pql::EntityRef()}};

  // List of possible types of expression
  std::string expr_equation = "(x * y % 5) + z / 2";
  std::vector<
      std::tuple<std::string, std::string, spa::pql::Pattern::Expression>>
      expr_options = {{"non-partial spec", "\"" + expr_equation + "\"",
                       spa::pql::Pattern::Expression(expr_equation)},
                      {"partial spec", "_\"" + expr_equation + "\"_",
                       spa::pql::Pattern::Expression(expr_equation, true)},
                      {"wildcard", "_", spa::pql::Pattern::Expression()}};

  // Builder of query string
  auto query_str_builder = [](std::string var_rep, std::string expr_rep) {
    return "assign a; variable v;\n"
           "Select a pattern a (" +
           var_rep + ", " + expr_rep + ")";
  };

  for (auto &[var_desc, var_rep, var_entity_ref] : entity_ref_options) {
    SECTION("from " + var_desc) {
      for (auto &[expr_desc, expr_rep, expr] : expr_options) {
        SECTION("to " + expr_desc) {
          auto pattern = spa::pql::Pattern(
              assign_statement_ref,
              std::make_shared<spa::pql::EntityRef>(var_entity_ref), expr);
          std::vector<std::shared_ptr<spa::pql::Clause>> clauses = {
              std::make_shared<spa::pql::Pattern>(pattern)};

          std::shared_ptr<spa::pql::Result> expected_result =
              std::make_shared<spa::pql::DeclaredResult>(
                  spa::pql::DeclaredResult({assign_entity_decl}));
          auto query = spa::pql::Query(expected_result, clauses);

          std::string query_string = query_str_builder(var_rep, expr_rep);
          INFO("Test case: \"" << query_string << "\"");

          REQUIRE(pql_parser.Parse(query_string) == query);
        };
      }
    }
  }
}

using namespace test::pql;

TEST_CASE("Test parsing of Pattern", "[pql][parser][pattern]") {
  SECTION("Positive") {
    TestParserPositive(data::pattern::kGetClausesPositive);
  }
  SECTION("Negative") { TestParserNegative(data::pattern::kGetFuncsNegative); }
}
