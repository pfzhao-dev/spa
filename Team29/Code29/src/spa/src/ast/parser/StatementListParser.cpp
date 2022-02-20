#include "StatementListParser.h"

#include "ast/Statement.h"
#include "ast/parser/ParseException.h"
#include "ast/parser/StatementParser.h"

namespace spa {

namespace ast {

std::shared_ptr<StatementList> StatementListParser::Parse(
    int start_idx, int end_idx, std::string proc_name) {
  std::vector<std::shared_ptr<Statement>> statement_list;

  int cur_idx = start_idx + 1;

  if (cur_idx >= end_idx) {
    // statement list is empty
    throw ParseException();
  }
  while (cur_idx < end_idx) {
    int close_idx;
    if (open_close->count(cur_idx)) {
      close_idx = (*open_close)[cur_idx];
    } else {
      close_idx = cur_idx;
    }
    // don't parse statement starting from else
    if ((*stmts)[cur_idx] != "else") {
      StatementParser stmt_parser = {stmts, open_close, stmt_nos};
      statement_list.push_back(
          stmt_parser.Parse(cur_idx, close_idx, proc_name));
    }

    cur_idx = close_idx;
    cur_idx++;
  }

  auto stmt_lst_ptr = std::make_shared<StatementList>(statement_list);

  for (int i = 0; i < statement_list.size(); i++) {
    (*statement_list[i]).SetParent(stmt_lst_ptr);
  }

  return stmt_lst_ptr;
}

}  // namespace ast

}  // namespace spa
