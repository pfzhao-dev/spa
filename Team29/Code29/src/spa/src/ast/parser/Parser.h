#pragma once

#include <map>
#include <string>
#include <unordered_map>

#include "ast/Node.h"
#include "ast/Program.h"
#include "ast/Statement.h"

namespace spa {

namespace ast {

class Parser {
 public:
  Parser() {}

  Parser(std::string s) {
    this->stmts = Tokenize(s);
    this->stmt_nos_ = std::vector<int>(stmts.size(), 0);
  }

  // For unit testing
  static std::vector<std::string> Tokenize(std::string s);

  std::shared_ptr<Program> Parse();
  std::shared_ptr<Absolute> ParseExpression(std::string s);

  static std::string Strip(std::string s);
  static std::string RemoveNewLines(std::string s);
  static std::string RemoveTrailing(std::string s);
  static std::string GetFirstWord(std::string s);

 private:
  std::vector<std::string> stmts;
  std::map<int, int> open_close;
  std::map<int, int> close_open;
  std::vector<int> stmt_nos_;
};

}  // namespace ast

}  // namespace spa
