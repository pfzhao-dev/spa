#pragma once

#include <map>
#include <memory>
#include <vector>
#include <cstring>

#include "ast/StatementList.h"

namespace spa {

namespace ast {

class StatementListParser {
 public:
  StatementListParser(std::shared_ptr<std::vector<std::string>> stmts,
                      std::shared_ptr<std::map<int, int>> open_close,
                      std::shared_ptr<std::vector<int>> stmt_nos_) {
    this->stmts = stmts;
    this->open_close = open_close;
    this->stmt_nos = stmt_nos_;
  }

  std::shared_ptr<StatementList> Parse(int start_idx, int end_idx,
                                       std::string proc_name);

 private:
  std::shared_ptr<std::vector<std::string>> stmts;
  std::shared_ptr<std::map<int, int>> open_close;
  std::shared_ptr<std::vector<int>> stmt_nos;
};

}  // namespace ast

}  // namespace spa
