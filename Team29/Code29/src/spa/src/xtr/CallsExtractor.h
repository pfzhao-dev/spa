#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "ast/Program.h"

namespace spa {
namespace xtr {

class CallsExtractor {
 public:
  void Extract(std::shared_ptr<ast::Program> program);

 private:
  void ExtractStatementList(std::string proc_name,
                            std::shared_ptr<ast::StatementList> stmts);

  void GenerateCallsT(std::string proc_name, std::string cur,
                      std::unordered_set<std::string> &visited);
};

}  // namespace xtr
}  // namespace spa
