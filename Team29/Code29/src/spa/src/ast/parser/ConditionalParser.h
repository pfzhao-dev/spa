#pragma once

#include <map>
#include <string>
#include <vector>

#include "ast/Statement.h"
#include "ast/parser/StatementParser.h"

namespace spa {

namespace ast {

class ConditionalParser : public StatementParser {
 public:
  ConditionalParser(std::shared_ptr<std::vector<std::string>> stmts,
                    std::shared_ptr<std::map<int, int>> open_close,
                    std::shared_ptr<std::vector<int>> stmt_nos_) {
    this->stmts = stmts;
    this->open_close = open_close;
    this->stmt_nos = stmt_nos_;
  }

  std::shared_ptr<Conditional> Parse(std::string s);
  std::shared_ptr<Relational> ParseRelExpr(std::string s);
  std::shared_ptr<Absolute> ParseRelFactor(std::string s);
  std::string RemoveBrackets(std::string s);

 private:
  std::shared_ptr<std::vector<std::string>> stmts;
  std::shared_ptr<std::map<int, int>> open_close;
  std::shared_ptr<std::vector<int>> stmt_nos;
};

}  // namespace ast

}  // namespace spa
