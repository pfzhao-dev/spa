#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "ast/Expression.h"
#include "ast/Statement.h"

namespace spa {
namespace ast {

class StatementParser {
 public:
  StatementParser(){};

  StatementParser(std::shared_ptr<std::vector<std::string>> stmts,
                  std::shared_ptr<std::map<int, int>> open_close,
                  std::shared_ptr<std::vector<int>> stmt_nos_) {
    this->stmts = stmts;
    this->open_close = open_close;
    this->stmt_nos = stmt_nos_;
  }

  std::shared_ptr<Statement> Parse(int start_idx, int end_idx,
                                   std::string proc_name);

  std::string GetFirstWord(std::string s);

 protected:
  std::shared_ptr<std::vector<std::string>> stmts;
  std::shared_ptr<std::map<int, int>> open_close;
  std::shared_ptr<std::vector<int>> stmt_nos;
  std::shared_ptr<Variable> ParseVariable(std::string var_str);
  std::shared_ptr<Constant> ParseConstant(std::string const_str);
};

}  // namespace ast
}  // namespace spa
