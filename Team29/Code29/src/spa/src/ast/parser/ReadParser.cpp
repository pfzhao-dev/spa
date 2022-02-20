#include "ReadParser.h"

#include "ast/parser/ParseException.h"

namespace spa {

namespace ast {

std::shared_ptr<Read> ReadParser::Parse(int idx) {
  if ((*stmts)[idx].size() < 5) {
    throw ParseException();
  }
  std::string var = (*stmts)[idx].substr(5);
  auto var_node = ParseVariable(var);
  return std::make_shared<Read>(var_node, (*stmt_nos)[idx]);
}

}  // namespace ast

}  // namespace spa
