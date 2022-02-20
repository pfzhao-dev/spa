#include "CallParser.h"

#include <regex>

#include "ast/parser/ParseException.h"

namespace spa {

namespace ast {

std::shared_ptr<Call> CallParser::Parse(int idx) {
  if ((*stmts)[idx].size() < 5) {
    throw ParseException();
  }
  std::string proc_name = (*stmts)[idx].substr(5);
  const std::string proc_name_regex = "[A-Za-z][A-Za-z0-9]*";
  std::regex proc_regex = std::regex(proc_name_regex);
  if (!std::regex_match(proc_name, proc_regex)) {
    throw ParseException();
  }
  return std::make_shared<Call>(proc_name, (*stmt_nos)[idx]);
}

}  // namespace ast

}  // namespace spa
