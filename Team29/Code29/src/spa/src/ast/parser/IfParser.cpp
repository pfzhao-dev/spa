#include "IfParser.h"

#include <regex>

#include "ast/parser/ParseException.h"
#include "ast/parser/ConditionalParser.h"
#include "ast/parser/StatementListParser.h"

namespace spa {

namespace ast {

std::shared_ptr<If> IfParser::Parse(int idx, std::string proc_name) {
  std::string cond_str = (*stmts)[idx].substr(2);
  ConditionalParser cond_parser = {stmts, open_close, stmt_nos};
  auto cond = cond_parser.Parse(cond_str);

  int close1 = (*open_close)[idx];

  StatementListParser stmt_lst_parser = {stmts, open_close, stmt_nos};
  auto consequent = stmt_lst_parser.Parse(idx, close1, proc_name);

  std::string closing_bracket = (*stmts)[close1];

  // The string has already been trimmed. Hence, we can just can match "else".
  std::regex else_regex(R"(^\s*else\s*)");

  std::smatch stmt_match;
  int else_idx = close1 + 1;
  bool has_match =
      std::regex_search((*stmts)[else_idx], stmt_match, else_regex);

  if (has_match) {
    auto alternative =
        stmt_lst_parser.Parse(else_idx, (*open_close)[else_idx], proc_name);
    return std::make_shared<If>(cond, consequent, alternative,
                                (*stmt_nos)[idx]);
  } else {
    throw ParseException();
  }
}

}  // namespace ast

}  // namespace spa
