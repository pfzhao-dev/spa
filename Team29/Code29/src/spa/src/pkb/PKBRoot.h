#pragma once

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "ast/Procedure.h"
#include "cfg/AffectsGraph.h"
#include "cfg/ProgramGraph.h"
#include "pkb/Cache.h"

namespace spa {

namespace pkb {

class PKBRoot {
 public:
  const std::vector<std::shared_ptr<ast::Procedure>> GetProcList() const;

  static PKBRoot* GetInstance() {
    static PKBRoot* kInstance;
    if (kInstance == nullptr) {
      kInstance = new PKBRoot();
    }
    return kInstance;
  }

  void ClearRoot();
  void ClearQueryCache();

  void AddProcedure(std::shared_ptr<ast::Procedure> p);
  void AddStmt(std::shared_ptr<ast::Statement> stmt, std::string proc_name);
  void AddVariable(std::shared_ptr<ast::Variable> variable);
  void AddConstant(std::shared_ptr<ast::Constant> constant);

  std::unordered_set<uint32_t>& GetConstValues();
  std::unordered_set<std::string>& GetProcNames();
  std::unordered_set<uint32_t>& GetStmtNos();
  std::unordered_set<uint32_t>& GetStmtNosByType(ast::Statement::Type type);
  std::unordered_set<std::string>& GetVarNames();
  std::string GetProcNameByStmtNo(uint32_t stmt_no);
  std::string GetAttrByStmt(uint32_t stmt_no);

  bool IsConstant(uint32_t value);
  bool IsProcedure(std::string proc_name);
  bool IsStatement(uint32_t stmt_num);
  bool IsStatementType(uint32_t stmt_num, ast::Statement::Type);
  bool IsVariable(std::string variable);

  bool MatchPatternExpr(uint32_t stmt_num,
                        std::shared_ptr<ast::Expression> expr, bool partial);

  bool MatchSubTree(std::shared_ptr<ast::Expression> original,
                    std::shared_ptr<ast::Expression> pattern);

  void AddFollows(uint32_t pred, uint32_t succ);
  void AddFollowsT(uint32_t pred, uint32_t succ);
  Cache<uint32_t, uint32_t>& GetFollowsCache(bool transitive = false);

  void AddParent(uint32_t pred, uint32_t succ);
  void AddParentT(uint32_t pred, uint32_t succ);
  Cache<uint32_t, uint32_t>& GetParentCache(bool transitive = false);

  void AddCalls(std::string pred, std::string succ);
  void AddCallsT(std::string pred, std::string succ);
  Cache<std::string, std::string>& GetCallsCache(bool transitive = false);

  void AddUsesStatement(uint32_t pred, std::string succ);
  void AddUsesProcedure(std::string pred, std::string succ);
  Cache<uint32_t, std::string>& GetUsesStatementCache();
  Cache<std::string, std::string>& GetUsesProcedureCache();

  void AddModifiesStatement(uint32_t pred, std::string succ);
  void AddModifiesProcedure(std::string pred, std::string succ);
  Cache<uint32_t, std::string>& GetModifiesStatementCache();
  Cache<std::string, std::string>& GetModifiesProcedureCache();

  void AddPatternIf(uint32_t pred, std::string succ);
  void AddPatternWhile(uint32_t pred, std::string succ);
  void AddPatternAssign(uint32_t pred, std::string succ);
  Cache<uint32_t, std::string>& GetPatternIfCache();
  Cache<uint32_t, std::string>& GetPatternWhileCache();
  Cache<uint32_t, std::string>& GetPatternAssignCache();

  bool HasPopulateNextAll(bool transitive = false, bool bip = false);
  void PopulateNextAll(bool transitive = false, bool bip = false);
  void PopulateNext(uint32_t idx, bool transitive = false, bool bip = false);
  Cache<uint32_t, uint32_t>& GetNextCache(bool transitive = false,
                                          bool bip = false);

  bool HasPopulateAffectsAll(bool transitive = false, bool bip = false);
  void PopulateAffectsAll(bool transitive = false, bool bip = false);
  void PopulateAffects(uint32_t idx, bool transitive = false, bool bip = false);
  void GenerateAffectsGraph(bool bip = false);
  Cache<uint32_t, uint32_t>& GetAffectsCache(bool transitive, bool bip = false);

  void AddProgramGraph(std::shared_ptr<cfg::ProgramGraph> graph);

 private:
  PKBRoot(){};

  // vector of statements
  std::vector<std::shared_ptr<ast::Statement>> stmts_;

  std::unordered_set<std::string> proc_names_;
  std::unordered_set<std::string> variables_;
  std::unordered_set<uint32_t> constants_;
  std::unordered_set<uint32_t> stmt_nos_;
  std::unordered_map<ast::Statement::Type, std::unordered_set<uint32_t>>
      stmt_nos_by_type_;
  std::unordered_map<uint32_t, std::string> proc_name_by_stmt_nos;

  // cache for follows family
  Cache<uint32_t, uint32_t> follows_cache_;
  Cache<uint32_t, uint32_t> follows_t_cache_;

  // cache for parent family
  Cache<uint32_t, uint32_t> parent_cache_;
  Cache<uint32_t, uint32_t> parent_t_cache_;

  // cache for calls
  Cache<std::string, std::string> calls_cache_;
  Cache<std::string, std::string> calls_t_cache_;

  // cache for uses
  Cache<uint32_t, std::string> uses_stmt_cache_;
  Cache<std::string, std::string> uses_proc_cache_;

  // cache for modifies
  Cache<uint32_t, std::string> modifies_stmt_cache_;
  Cache<std::string, std::string> modifies_proc_cache_;

  // cache for pattern
  Cache<uint32_t, std::string> pattern_if_cache_;
  Cache<uint32_t, std::string> pattern_while_cache_;
  Cache<uint32_t, std::string> pattern_assign_cache_;

  // cache for next family
  Cache<uint32_t, uint32_t> next_cache_[2][2];
  std::unordered_set<uint32_t> generated_next_[2][2];
  bool has_populate_all_next_[2][2];

  // cache for affects family
  Cache<uint32_t, uint32_t> affects_cache_[2][2];
  std::unordered_set<uint32_t> generated_affects_[2][2];
  bool has_populate_all_affects_[2][2];

  // cfg graph
  std::shared_ptr<cfg::ProgramGraph> program_graph_;

  // affects graph
  std::shared_ptr<cfg::AffectsGraph> affects_graph_;

  // affects graph
  std::shared_ptr<cfg::AffectsGraph> affects_bip_graph_;
};

}  // namespace pkb

}  // namespace spa
