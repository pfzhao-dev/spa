#pragma once

#include <memory>

#include "ast/Procedure.h"
#include "ast/Program.h"
#include "cfg/Node.h"
#include "cfg/ProgramGraph.h"

namespace spa {

namespace xtr {

class CfgExtractor {
 public:
  CfgExtractor();

  void Extract(std::shared_ptr<ast::Program> program,
               std::vector<std::string> order);

 private:
  uint32_t dummy_counter_;
  uint32_t neighbor_counter_;

  using CfgNodePtr = std::shared_ptr<cfg::Node>;
  using CfgHeadTail = std::pair<CfgNodePtr, CfgNodePtr>;

  // Returns head and all tails of the graph.
  CfgHeadTail BuildCfg(std::shared_ptr<ast::StatementList> stmt_list,
                       std::shared_ptr<cfg::ProgramGraph> graph);

  // Returns head and up to two tails of the graph.
  // Only if statement can return two tails.
  CfgHeadTail BuildCfg(std::shared_ptr<ast::Statement> stmt,
                       std::shared_ptr<cfg::ProgramGraph> graph);
};

}  // namespace xtr

}  // namespace spa
