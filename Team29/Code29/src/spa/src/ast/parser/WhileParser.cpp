#include "WhileParser.h"

#include "ast/parser/ParseException.h"
#include "ast/parser/ConditionalParser.h"
#include "ast/parser/StatementListParser.h"

namespace spa {

namespace ast {

std::shared_ptr<While> WhileParser::Parse(int idx, std::string proc_name) {
  std::string cond_str = (*stmts)[idx].substr(5);
  ConditionalParser cond_parser = {stmts, open_close, stmt_nos};
  auto cond = cond_parser.Parse(cond_str);

  int close = (*open_close)[idx];
  StatementListParser stmt_lst_parser = {stmts, open_close, stmt_nos};
  auto consequent = stmt_lst_parser.Parse(idx, close, proc_name);

  return std::make_shared<While>(cond, consequent, (*stmt_nos)[idx]);
}

}  // namespace ast

}  // namespace spa
