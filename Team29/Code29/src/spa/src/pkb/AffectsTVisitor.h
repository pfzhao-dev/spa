#pragma once

#include <memory>
#include <string>
#include <unordered_set>

#include "cfg/Node.h"
#include "cfg/Visitor.h"

namespace spa {

namespace pkb {

class AffectsTVisitor : public cfg::Visitor {
 public:
  AffectsTVisitor(bool bip);

  bool VisitRoot(std::shared_ptr<cfg::Node> node) override;

  bool VisitBranch(std::shared_ptr<cfg::Node> node, uint32_t label) override;

  std::unordered_set<uint32_t>& GetAffectedStmts();

 private:
  bool bip_;
  uint32_t root_stmt_no_;

  std::unordered_set<uint32_t> visited_stmts_;
  std::unordered_set<uint32_t> affected_stmt_;
};

}  // namespace pkb

}  // namespace spa
