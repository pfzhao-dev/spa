#pragma once

#include <map>
#include <memory>
#include <vector>

#include "ast/Procedure.h"

namespace spa {

namespace ast {

class ProcedureParser {
 public:
  ProcedureParser(std::shared_ptr<std::vector<std::string>> stmts,
                  std::shared_ptr<std::map<int, int>> open_close,
                  std::shared_ptr<std::vector<int>> stmt_nos_) {
    this->stmts = stmts;
    this->open_close = open_close;
    this->stmt_nos = stmt_nos_;
  }

  std::shared_ptr<Procedure> Parse(int start_idx, int end_idx);

 private:
  std::shared_ptr<std::vector<std::string>> stmts;
  std::shared_ptr<std::map<int, int>> open_close;
  std::shared_ptr<std::vector<int>> stmt_nos;
};

}  // namespace ast
}  // namespace spa
