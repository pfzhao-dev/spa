#include "PKBRoot.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <set>
#include <unordered_set>

#include "pkb/AffectsTVisitor.h"
#include "pkb/AffectsVisitor.h"
#include "pkb/Exception.h"
#include "pkb/NextVisitor.h"

namespace spa {

namespace pkb {

void PKBRoot::ClearRoot() {
  // Perform a full reset on the PKBRoot.
  // Useful for unit testing.

  stmts_.clear();
  stmt_nos_.clear();
  stmt_nos_by_type_.clear();

  follows_cache_.Clear();
  follows_t_cache_.Clear();

  // cache for parent family
  parent_cache_.Clear();
  parent_t_cache_.Clear();

  // cache for calls
  calls_cache_.Clear();
  calls_t_cache_.Clear();

  // cache for uses
  uses_stmt_cache_.Clear();
  uses_proc_cache_.Clear();

  // cache for modifies
  modifies_stmt_cache_.Clear();
  modifies_proc_cache_.Clear();

  // cache for pattern
  pattern_if_cache_.Clear();
  pattern_while_cache_.Clear();
  pattern_assign_cache_.Clear();

  proc_names_.clear();
  variables_.clear();
  constants_.clear();

  ClearQueryCache();

  program_graph_ = {};
  affects_graph_ = {};
}

void PKBRoot::ClearQueryCache() {
  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 2; ++j) {
      // cache for next family
      next_cache_[i][j].Clear();
      generated_next_[i][j].clear();
      has_populate_all_next_[i][j] = false;

      // cache for affects family
      affects_cache_[i][j].Clear();
      generated_affects_[i][j].clear();
      has_populate_all_affects_[i][j] = false;
    }
}

void PKBRoot::AddProcedure(std::shared_ptr<ast::Procedure> p) {
  proc_names_.insert(p->GetName());
}

void PKBRoot::AddStmt(std::shared_ptr<ast::Statement> stmt,
                      std::string proc_name) {
  if (stmts_.size() <= stmt->GetStmtNo())
    stmts_.resize((size_t)stmt->GetStmtNo() + 1);

  stmt_nos_.insert(stmt->GetStmtNo());

  // the default value will be an empty set, which we can just insert
  stmt_nos_by_type_[stmt->GetType()].insert(stmt->GetStmtNo());

  stmts_[stmt->GetStmtNo()] = stmt;

  proc_name_by_stmt_nos[stmt->GetStmtNo()] = proc_name;
}

std::unordered_set<uint32_t> &PKBRoot::GetConstValues() { return constants_; }

std::unordered_set<std::string> &PKBRoot::GetProcNames() { return proc_names_; }

std::unordered_set<uint32_t> &PKBRoot::GetStmtNos() { return stmt_nos_; }

std::unordered_set<uint32_t> &PKBRoot::GetStmtNosByType(
    ast::Statement::Type type) {
  return stmt_nos_by_type_[type];
}

std::unordered_set<std::string> &PKBRoot::GetVarNames() { return variables_; }

std::string PKBRoot::GetProcNameByStmtNo(uint32_t stmt_no) {
  return proc_name_by_stmt_nos[stmt_no];
}

std::string PKBRoot::GetAttrByStmt(uint32_t stmt_no) {
  auto stmt = stmts_[stmt_no];

  if (stmt->GetType() == ast::Statement::Type::kCall) {
    auto call_stmt = std::static_pointer_cast<ast::Call>(stmt);
    return call_stmt->GetProcName();

  } else if (stmt->GetType() == ast::Statement::Type::kPrint) {
    auto print_stmt = std::static_pointer_cast<ast::Print>(stmt);
    auto var = print_stmt->GetVariable();
    return var->GetName();

  } else if (stmt->GetType() == ast::Statement::Type::kRead) {
    auto read_stmt = std::static_pointer_cast<ast::Read>(stmt);
    auto var = read_stmt->GetVariable();
    return var->GetName();

  } else {
    throw Exception("Unable to fetch stmt attr if not call, print, or read.");
  }
}

void PKBRoot::AddVariable(std::shared_ptr<ast::Variable> var) {
  variables_.insert(var->GetName());
}

void PKBRoot::AddConstant(std::shared_ptr<ast::Constant> constant) {
  constants_.insert(constant->GetValue());
}

bool PKBRoot::IsConstant(uint32_t constant) {
  return constants_.count(constant);
}

bool PKBRoot::IsStatement(uint32_t stmt_no) { return stmt_nos_.count(stmt_no); }

bool PKBRoot::IsStatementType(uint32_t stmt_no, ast::Statement::Type type) {
  return stmt_nos_by_type_.count(type) &&
         stmt_nos_by_type_[type].count(stmt_no);
}

bool PKBRoot::IsProcedure(std::string proc_name) {
  return proc_names_.count(proc_name);
}

bool PKBRoot::IsVariable(std::string variable) {
  return variables_.count(variable);
}

bool PKBRoot::MatchSubTree(std::shared_ptr<spa::ast::Expression> original,
                           std::shared_ptr<spa::ast::Expression> pattern) {
  if (original->GetType() == spa::ast::Expression::Type::kArithmetic) {
    auto arithmetic = std::static_pointer_cast<spa::ast::Arithmetic>(original);
    if (*arithmetic == *pattern) {
      return true;
    }
    return MatchSubTree(arithmetic->GetLHS(), pattern) ||
           MatchSubTree(arithmetic->GetRHS(), pattern);
  }
  return *original == *pattern;
}

bool PKBRoot::MatchPatternExpr(uint32_t stmt_no,
                               std::shared_ptr<spa::ast::Expression> expr,
                               bool partial) {
  auto stmt = stmts_[stmt_no];
  if (stmt->GetType() != ast::Statement::Type::kAssign) {
    return false;
  }
  if (partial && expr->GetType() == ast::Expression::Type::kVariable) {
    auto var = std::static_pointer_cast<ast::Variable>(expr);
    return uses_stmt_cache_.Count(stmt_no, var->GetName());
  }
  auto assign_stmt = std::static_pointer_cast<ast::Assign>(stmt);
  return partial ? MatchSubTree(assign_stmt->GetExpression(), expr)
                 : *assign_stmt->GetExpression() == *expr;
}

void PKBRoot::AddFollows(uint32_t pred, uint32_t succ) {
  follows_cache_.AddRelation(pred, succ);
}
void PKBRoot::AddFollowsT(uint32_t pred, uint32_t succ) {
  follows_t_cache_.AddRelation(pred, succ);
}

Cache<uint32_t, uint32_t> &PKBRoot::GetFollowsCache(bool transitive) {
  return transitive ? follows_t_cache_ : follows_cache_;
}

void PKBRoot::AddParent(uint32_t pred, uint32_t succ) {
  parent_cache_.AddRelation(pred, succ);
}
void PKBRoot::AddParentT(uint32_t pred, uint32_t succ) {
  parent_t_cache_.AddRelation(pred, succ);
}

Cache<uint32_t, uint32_t> &PKBRoot::GetParentCache(bool transitive) {
  return transitive ? parent_t_cache_ : parent_cache_;
}

void PKBRoot::AddCalls(std::string pred, std::string succ) {
  calls_cache_.AddRelation(pred, succ);
}
void PKBRoot::AddCallsT(std::string pred, std::string succ) {
  calls_t_cache_.AddRelation(pred, succ);
}

Cache<std::string, std::string> &PKBRoot::GetCallsCache(bool transitive) {
  return transitive ? calls_t_cache_ : calls_cache_;
}

void PKBRoot::AddUsesStatement(uint32_t pred, std::string succ) {
  uses_stmt_cache_.AddRelation(pred, succ);
}
void PKBRoot::AddUsesProcedure(std::string pred, std::string succ) {
  uses_proc_cache_.AddRelation(pred, succ);
}

Cache<uint32_t, std::string> &PKBRoot::GetUsesStatementCache() {
  return uses_stmt_cache_;
}
Cache<std::string, std::string> &PKBRoot::GetUsesProcedureCache() {
  return uses_proc_cache_;
}

void PKBRoot::AddModifiesStatement(uint32_t pred, std::string succ) {
  modifies_stmt_cache_.AddRelation(pred, succ);
}
void PKBRoot::AddModifiesProcedure(std::string pred, std::string succ) {
  modifies_proc_cache_.AddRelation(pred, succ);
}

Cache<uint32_t, std::string> &PKBRoot::GetModifiesStatementCache() {
  return modifies_stmt_cache_;
}
Cache<std::string, std::string> &PKBRoot::GetModifiesProcedureCache() {
  return modifies_proc_cache_;
}

void PKBRoot::AddPatternIf(uint32_t pred, std::string succ) {
  pattern_if_cache_.AddRelation(pred, succ);
}

void PKBRoot::AddPatternWhile(uint32_t pred, std::string succ) {
  pattern_while_cache_.AddRelation(pred, succ);
}

void PKBRoot::AddPatternAssign(uint32_t pred, std::string succ) {
  pattern_assign_cache_.AddRelation(pred, succ);
}

Cache<uint32_t, std::string> &PKBRoot::GetPatternIfCache() {
  return pattern_if_cache_;
}

Cache<uint32_t, std::string> &PKBRoot::GetPatternWhileCache() {
  return pattern_while_cache_;
}

Cache<uint32_t, std::string> &PKBRoot::GetPatternAssignCache() {
  return pattern_assign_cache_;
}

bool PKBRoot::HasPopulateNextAll(bool transitive, bool bip) {
  return has_populate_all_next_[transitive][bip];
}

void PKBRoot::PopulateNextAll(bool transitive, bool bip) {
  if (!program_graph_) return;
  if (has_populate_all_next_[transitive][bip]) return;

  has_populate_all_next_[transitive][bip] = true;
  for (auto i = 1; i <= stmts_.size(); i++) PopulateNext(i, transitive, bip);
}

void PKBRoot::PopulateNext(uint32_t idx, bool transitive, bool bip) {
  if (!program_graph_) return;

  auto &generated_set = generated_next_[transitive][bip];

  if (generated_set.count(idx)) return;
  generated_set.insert(idx);

  auto visitor = std::make_shared<NextVisitor>(transitive, bip);
  program_graph_->Traverse(idx, visitor, bip);

  auto &cache = next_cache_[transitive][bip];
  for (auto nx : visitor->GetNextStmts()) {
    cache.AddRelation(idx, nx);
  }
}

Cache<uint32_t, uint32_t> &PKBRoot::GetNextCache(bool transitive, bool bip) {
  return next_cache_[transitive][bip];
}

bool PKBRoot::HasPopulateAffectsAll(bool transitive, bool bip) {
  return has_populate_all_affects_[transitive][bip];
}

void PKBRoot::PopulateAffectsAll(bool transitive, bool bip) {
  if (!program_graph_) return;
  if (has_populate_all_affects_[transitive][bip]) return;
  has_populate_all_affects_[transitive][bip] = true;

  if (!stmt_nos_by_type_.count(ast::Statement::Type::kAssign)) {
    // Skip if there's no assign statement
    return;
  }

  for (auto stmt : stmt_nos_by_type_[ast::Statement::Type::kAssign]) {
    PopulateAffects(stmt, transitive, bip);
  }
}

void PKBRoot::GenerateAffectsGraph(bool bip) {
  if (!program_graph_) return;

  if (!bip && affects_graph_) return;
  if (bip && affects_bip_graph_) return;

  PopulateAffectsAll(false, bip);

  auto &cache = GetAffectsCache(false, bip);
  auto &cache_map = cache.GetOriginalMap();
  auto &graph = bip ? affects_bip_graph_ : affects_graph_;

  graph = std::make_shared<cfg::AffectsGraph>();

  for (auto &[affector, affecteds] : cache_map) {
    for (auto &affected : affecteds) {
      graph->Add(affector, affected);
    }
  }
}

void PKBRoot::PopulateAffects(uint32_t idx, bool transitive, bool bip) {
  if (!program_graph_) return;
  auto &generated_set = generated_affects_[transitive][bip];

  if (generated_set.count(idx)) return;
  generated_set.insert(idx);

  if (!transitive) {
    auto visitor = std::make_shared<AffectsVisitor>(bip);
    program_graph_->Traverse(idx, visitor, bip);

    auto &cache = affects_cache_[transitive][bip];
    for (auto affected : visitor->GetAffectedStmts()) {
      cache.AddRelation(idx, affected);
    }

  } else {
    // NOTE: AffectsT depends on the total precomputation of Affects.
    //       Hence, if there's an AffectsT statement, we can just precompute.
    GenerateAffectsGraph(bip);

    if (bip) {  // special case since this relies on NextBip*
      PopulateNextAll(true, true);
    }

    auto visitor = std::make_shared<AffectsTVisitor>(bip);
    auto &graph = bip ? affects_bip_graph_ : affects_graph_;
    graph->Traverse(idx, visitor, bip);

    auto &cache = affects_cache_[transitive][bip];
    for (auto affected : visitor->GetAffectedStmts()) {
      cache.AddRelation(idx, affected);
    }
  }
}

Cache<uint32_t, uint32_t> &PKBRoot::GetAffectsCache(bool transitive, bool bip) {
  return affects_cache_[transitive][bip];
}

void PKBRoot::AddProgramGraph(std::shared_ptr<cfg::ProgramGraph> graph) {
  program_graph_ = graph;
}

}  // namespace pkb

}  // namespace spa
