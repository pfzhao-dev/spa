#include "ExpressionParser.h"

#include <regex>

#include "ast/parser/ParseException.h"
#include "ast/parser/Parser.h"

namespace spa {

namespace ast {

// Combine ParseExpression and ParseTerm into one function
std::shared_ptr<Absolute> ExpressionParser::ParseExpressionTerm(
    std::string s, bool is_expression) {
  std::string ss = Parser::Strip(s);
  if (ss.length() == 0) {
    throw ParseException();
  }

  std::unordered_set<char> term_op = {'*', '%', '/'};
  std::unordered_set<char> expr_op = {'+', '-'};

  // term ‘*’ factor | term ‘/’ factor | term ‘%’ factor
  int brackets_ctr = 0;
  for (int idx = ss.length() - 1; idx > 0; idx--) {
    if (ss[idx] == ')') {
      brackets_ctr++;
    } else if (ss[idx] == '(') {
      brackets_ctr--;
    } else if ((brackets_ctr == 0) &&
               (is_expression ? expr_op.count(ss[idx])
                              : term_op.count(ss[idx]))) {
      std::string lhs = ss.substr(0, idx);
      std::string rhs = ss.substr(idx + 1);
      auto left_expr = ParseExpressionTerm(lhs, is_expression);
      auto right_expr =
          is_expression ? ParseExpressionTerm(rhs, false) : ParseFactor(rhs);

      return std::make_shared<Arithmetic>(Arithmetic::ToOpType(ss[idx]),
                                          left_expr, right_expr);
    }
  }
  return is_expression ? ParseExpressionTerm(s, false) : ParseFactor(s);
}

std::shared_ptr<Absolute> ExpressionParser::ParseFactor(std::string s) {
  std::string ss = Parser::Strip(s);
  try {
    return ParseVariable(ss);
  } catch (...) {
    try {
      return ParseConstant(ss);
    } catch (...) {
      std::regex bracketsRegex = std::regex(R"(\(.*\))");
      std::smatch sm;
      if (std::regex_match(ss, sm, bracketsRegex)) {
        std::string tmp = ss.substr(1, ss.length() - 2);
        return ParseExpressionTerm(tmp, true);
      } else {
        throw ParseException();
      }
    }
  }
}

}  // namespace ast

}  // namespace spa
