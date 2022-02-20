#pragma once

#include <string>
#include <unordered_set>

#include "pkb/Cache.h"
#include "pkb/PKBRoot.h"
#include "pql/Declaration.h"

namespace spa {

namespace pql {

class Evaluator {
 public:
  // Singleton
  static Evaluator& GetInstance() {
    static Evaluator kInstance;
    return kInstance;
  }

  // Basic values.

  // Get all constants.
  std::unordered_set<uint32_t>& GetConstants();

  // Check whether the given value is recorded as a constant.
  bool IsConstant(uint32_t value);

  // Get all procedures.
  std::unordered_set<std::string>& GetProcedures();

  // Check whether the given string is a procedure name.
  bool IsProcedure(std::string proc_name);

  // Get all statements, potentially of a particular type. It accepts a
  // declaration type for convenience between differentiating a generic
  // statement type (i.e., kStmt) and a specific statement type
  // (e.g., kRead, kPrint).
  //
  // NOTE: The declaration type must represent a statement type.
  std::unordered_set<uint32_t>& GetStatements(pql::Declaration::Type type);

  // Check whether the given value represents a statement number, potentially of
  // a particular type. It accepts a declaration type for convenience between
  // differentiating a generic statement type (i.e., kStmt) and a specific
  // statement type (e.g., kRead, kPrint).
  //
  // NOTE: The declaration type must represent a statement type.
  bool IsStatement(uint32_t stmt_no, pql::Declaration::Type type);

  // Get all variables.
  std::unordered_set<std::string>& GetVariables();

  // Get the variable associated with a particular statement.
  //
  // NOTE: The statement type must either be call, read, or print.
  std::string GetAttrByStmt(uint32_t stmt_no);

  // Check whether the given string is a variable name.
  bool IsVariable(std::string var_name);

  // Affects clause

  // Populate the entire Affects cache based on the specs
  // (transitive and/or bip).
  void PopulateAffects(bool transitive = false, bool bip = false);

  // Populate the Affects cache for the given starting point based on the specs
  // (transitive and/or bip).
  void PopulateAffects(uint32_t pred, bool transitive = false,
                       bool bip = false);

  // Check whether everything has been populated or not.
  bool HasPopulateAffects(bool transitive = false, bool bip = false);

  // Check whether the `affector` statement affects the `affected` statement.
  //
  // NOTE: We need to pass the predecessor to allow lazy evaluation.
  pkb::Cache<uint32_t, uint32_t>& GetAffectsCache(bool transitive = false,
                                                  bool bip = false);

  // Calls clause

  // Check whether the `caller` procedure calls the `callee` procedure.
  pkb::Cache<std::string, std::string>& GetCallsCache(bool transitive = false);

  // Follows clause

  // Check whether the succeeding statement follows the preceding statement.
  pkb::Cache<uint32_t, uint32_t>& GetFollowsCache(bool transitive = false);

  // Modifies clause

  // Check whether a statement modifies a variable.
  pkb::Cache<uint32_t, std::string>& GetModifiesSCache();

  // Check whether a procedure modifies a variable.
  pkb::Cache<std::string, std::string>& GetModifiesPCache();

  // Next clause

  // Populate the entire Next cache.
  void PopulateNext(bool transitive = false, bool bip = false);

  // Populate the Next cache for the given starting point.
  void PopulateNext(uint32_t pred, bool transitive = false, bool bip = false);

  // Check whether everything has been populated or not.
  bool HasPopulateNext(bool transitive = false, bool bip = false);

  // Check whether the succeeding statement is after/reachable from the
  // preceding statement.
  //
  // NOTE: We need to pass the predecessor to allow lazy evaluation.
  pkb::Cache<uint32_t, uint32_t>& GetNextCache(bool transitive = false,
                                               bool bip = false);

  // Parent clause

  // Check whether the preceding statement is a parent of the
  // succeeding statement.
  pkb::Cache<uint32_t, uint32_t>& GetParentCache(bool transitive = false);

  // Pattern clause

  // Parse expression string into an expression node. This API is exposed so
  // that the fetch function can just parse the string once for multiple
  // assign pattern evaluations.
  std::shared_ptr<ast::Expression> ParseExpression(std::string s);

  // Check whether a statement resembles a (potentially partial) expression.
  //
  // NOTE: This function should only be used for an assign statement. Also,
  //       the `expr` value should not be a null pointer.
  bool PatternExpr(uint32_t stmt_no, std::shared_ptr<ast::Expression> expr,
                   bool partial = false);

  // Check whether a statement has a particular control variable. For an assign
  // statement, the control variable is the variable that appears on the left
  // hand side. Meanwhile, for if and while statements, a control variable is a
  // variable used in their conditional expression.
  //
  // NOTE: This function should only be used for assign, while, and if
  //       statements.
  pkb::Cache<uint32_t, std::string>& GetPatternVarCache(
      pql::Declaration::Type type);

  // Uses clause

  // Check whether a statement uses a variable.
  pkb::Cache<uint32_t, std::string>& GetUsesSCache();

  // Check whether a procedure uses a variable.
  pkb::Cache<std::string, std::string>& GetUsesPCache();

 private:
  // Private constructor because the class is supposed to be a singleton.
  Evaluator() { this->root = pkb::PKBRoot::GetInstance(); }

  static pkb::PKBRoot* root;
};

}  // namespace pql

}  // namespace spa
