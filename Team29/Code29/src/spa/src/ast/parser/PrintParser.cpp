#include "PrintParser.h"

#include "ast/parser/ParseException.h"

namespace spa {

namespace ast {

std::shared_ptr<Print> PrintParser::Parse(int idx) {
  if ((*stmts)[idx].size() < 6) {
    throw ParseException();
  }
  std::string var = (*stmts)[idx].substr(6);
  auto var_node = ParseVariable(var);
  return std::make_shared<Print>(var_node, (*stmt_nos)[idx]);
}

}  // namespace ast

}  // namespace spa
