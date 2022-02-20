#pragma once

#include <map>
#include <string>

#include "ast/Statement.h"
#include "ast/parser/StatementParser.h"

namespace spa {

namespace ast {

class WhileParser : public StatementParser {
 public:
  WhileParser(std::shared_ptr<std::vector<std::string>> stmts,
              std::shared_ptr<std::map<int, int>> open_close,
              std::shared_ptr<std::vector<int>> stmt_nos_) {
    this->stmts = stmts;
    this->open_close = open_close;
    this->stmt_nos = stmt_nos_;
  }

  std::shared_ptr<While> Parse(int idx, std::string proc_name);

 private:
  std::shared_ptr<std::vector<std::string>> stmts;
  std::shared_ptr<std::map<int, int>> open_close;
  std::shared_ptr<std::vector<int>> stmt_nos;
};

}  // namespace ast

}  // namespace spa
