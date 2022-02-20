#include "AssignParser.h"

#include "ast/parser/ExpressionParser.h"
#include "ast/parser/ParseException.h"
#include "ast/parser/Parser.h"

namespace spa {

namespace ast {

std::shared_ptr<Assign> AssignParser::Parse(int idx) {
  std::size_t equal_pos = (*stmts)[idx].find('=');
  if (equal_pos >= (*stmts)[idx].size()) {
    // An assign statement must have one equal sign
    throw ParseException();
  }
  auto var =
      ParseVariable(Parser::RemoveTrailing((*stmts)[idx].substr(0, equal_pos)));
  std::string expr = (*stmts)[idx].substr(equal_pos + 1);
  ExpressionParser expr_parser = {stmts, open_close, stmt_nos};
  return std::make_shared<Assign>(
      var, expr_parser.ParseExpressionTerm(expr, true), (*stmt_nos)[idx]);
}

}  // namespace ast

}  // namespace spa
