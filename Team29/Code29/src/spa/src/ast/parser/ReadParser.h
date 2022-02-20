#pragma once

#include <map>
#include <string>

#include "ast/Statement.h"
#include "ast/parser/StatementParser.h"

namespace spa {

namespace ast {

class ReadParser : public StatementParser {
 public:
  ReadParser(std::shared_ptr<std::vector<std::string>> stmts,
             std::shared_ptr<std::map<int, int>> open_close,
             std::shared_ptr<std::vector<int>> stmt_nos_) {
    this->stmts = stmts;
    this->open_close = open_close;
    this->stmt_nos = stmt_nos_;
  }

  std::shared_ptr<Read> Parse(int idx);

 private:
  std::shared_ptr<std::vector<std::string>> stmts;
  std::shared_ptr<std::map<int, int>> open_close;
  std::shared_ptr<std::vector<int>> stmt_nos;
};

}  // namespace ast

}  // namespace spa
