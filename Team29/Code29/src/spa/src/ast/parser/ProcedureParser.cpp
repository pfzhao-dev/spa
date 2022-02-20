#include "ProcedureParser.h"

#include "ast/parser/StatementListParser.h"

namespace spa {

namespace ast {

std::shared_ptr<Procedure> ProcedureParser::Parse(int start_idx, int end_idx) {
  std::string proc_name = (*stmts)[start_idx].substr(10);
  StatementListParser stmt_lst_parser = {stmts, open_close, stmt_nos};
  auto statement_list = stmt_lst_parser.Parse(start_idx, end_idx, proc_name);
  auto procedure = std::make_shared<Procedure>(proc_name, statement_list);

  return procedure;
}

}  // namespace ast

}  // namespace spa
