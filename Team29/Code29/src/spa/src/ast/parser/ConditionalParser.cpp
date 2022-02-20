#include "ConditionalParser.h"

#include <regex>

#include "ast/parser/ExpressionParser.h"
#include "ast/parser/ParseException.h"
#include "ast/parser/Parser.h"

namespace spa {

namespace ast {

std::shared_ptr<Conditional> ConditionalParser::Parse(std::string s) {
  std::string ss = Parser::Strip(s);

  try {
    return ParseRelExpr(ss);
  } catch (...) {
    // try other options

    std::regex not_regex(R"(^!\s*\(.*\)$)");
    std::regex and_regex(R"(^(\(.+\))\s?&&\s?(\(.+\)))");
    std::regex or_regex(R"(^(\(.+\))\s?\|\|\s?(\(.+\)))");

    std::smatch sm;
    if (std::regex_match(ss, sm, not_regex)) {
      std::string new_expr =
          Parser::Strip(ss.substr(1, ss.length() - 2)).substr(1);

      return std::make_shared<Logical>(Logical::ToOpType("not"),
                                       Parse(new_expr));
    }
    if (std::regex_match(ss, sm, and_regex)) {
      for (int i = 0; i < ss.length() - 1; i++) {
        if (ss[i] == '&' && ss[i + 1] == '&') {
          std::string front_cond = RemoveBrackets(ss.substr(0, i));
          std::string back_cond =
              RemoveBrackets(ss.substr(i + 2, ss.length() - i - 1));
          try {
            return std::make_shared<Logical>(
                Logical::ToOpType("and"), Parse(front_cond), Parse(back_cond));
          } catch (...) {
            continue;
          }
        }
      }
    }

    if (regex_match(ss, sm, or_regex)) {
      for (int i = 0; i < ss.length() - 1; i++) {
        if (ss[i] == '|' && ss[i + 1] == '|') {
          std::string front_cond = RemoveBrackets(ss.substr(0, i));
          std::string back_cond =
              RemoveBrackets(ss.substr(i + 2, ss.length() - i - 1));
          try {
            return std::make_shared<Logical>(
                Logical::ToOpType("or"), Parse(front_cond), Parse(back_cond));
          } catch (...) {
            continue;
          }
        }
      }
    }
  }
  throw ParseException();
}

std::shared_ptr<Relational> ConditionalParser::ParseRelExpr(std::string s) {
  const std::string compare_regex_symbols = R"(<=|>=|==|!=)";
  const std::string strict_compare_regex_symbols = R"(<|>)";
  auto compare_regex = std::regex(compare_regex_symbols);
  auto strict_compare_regex = std::regex(strict_compare_regex_symbols);

  std::sregex_token_iterator iter(s.begin(), s.end(), compare_regex, -1);
  std::sregex_token_iterator end;
  std::vector<std::string> rel_factors;

  int occ = 0;

  for (; iter != end; ++iter) {
    rel_factors.push_back(*iter);
  }

  if (rel_factors.size() == 1) {
    iter = std::sregex_token_iterator(s.begin(), s.end(), strict_compare_regex,
                                      -1);
    rel_factors.clear();
    for (; iter != end; ++iter) {
      rel_factors.push_back(*iter);
    }
  }

  if (rel_factors.size() != 2) {
    throw ParseException();
  }

  std::smatch sm;  // same as std::match_results<string::const_iterator> sm;
  std::regex_search(s, sm, compare_regex);

  if (sm.size() == 0) {
    std::regex_search(s, sm, strict_compare_regex);
  }
  if (sm.size() == 0) {
    throw ParseException();
  }

  std::string type_str = sm.str(0);
  auto type = Relational::ToOpType(type_str);

  return std::make_shared<Relational>(type, ParseRelFactor(rel_factors[0]),
                                      ParseRelFactor(rel_factors[1]));
}

std::shared_ptr<Absolute> ConditionalParser::ParseRelFactor(std::string s) {
  std::string ss = Parser::Strip(s);
  try {
    return ParseVariable(ss);
  } catch (...) {
    try {
      return ParseConstant(ss);
    } catch (...) {
      ExpressionParser expr_parser = {stmts, open_close, stmt_nos};
      return expr_parser.ParseExpressionTerm(ss, true);
    }
  }
}

std::string ConditionalParser::RemoveBrackets(std::string s) {
  std::string ss = Parser::Strip(s);
  if (ss[0] != '(' || ss[ss.length() - 1] != ')') {
    throw ParseException();
  }
  return ss.substr(1, ss.length() - 2);
}

}  // namespace ast
}  // namespace spa
