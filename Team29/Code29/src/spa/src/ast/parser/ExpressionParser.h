#pragma once

#include <map>
#include <string>
#include <vector>

#include "ast/Statement.h"
#include "ast/parser/StatementParser.h"

namespace spa {

namespace ast {

class ExpressionParser : public StatementParser {
 public:
  ExpressionParser(std::shared_ptr<std::vector<std::string>> stmts,
                   std::shared_ptr<std::map<int, int>> open_close,
                   std::shared_ptr<std::vector<int>> stmt_nos_) {
    this->stmts = stmts;
    this->open_close = open_close;
    this->stmt_nos = stmt_nos_;
  }

  ExpressionParser() {}

  std::shared_ptr<Absolute> ParseFactor(std::string s);

  std::shared_ptr<Absolute> ParseExpressionTerm(std::string s,
                                                bool is_expression);

 private:
  std::shared_ptr<std::vector<std::string>> stmts;
  std::shared_ptr<std::map<int, int>> open_close;
  std::shared_ptr<std::vector<int>> stmt_nos;
};

}  // namespace ast

}  // namespace spa
