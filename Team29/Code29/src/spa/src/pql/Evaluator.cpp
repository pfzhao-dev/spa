#include "Evaluator.h"

#include <set>

#include "ast/Expression.h"
#include "ast/Statement.h"
#include "ast/parser/Parser.h"
#include "pql/QueryException.h"
#include "utils/Translator.h"

namespace spa {

namespace pql {

// Basic values.
pkb::PKBRoot* Evaluator::root = pkb::PKBRoot::GetInstance();

std::unordered_set<uint32_t>& Evaluator::GetConstants() {
  return root->GetConstValues();
}

bool Evaluator::IsConstant(uint32_t value) { return root->IsConstant(value); }

std::unordered_set<std::string>& Evaluator::GetProcedures() {
  return root->GetProcNames();
}

bool Evaluator::IsProcedure(std::string proc_name) {
  return root->IsProcedure(proc_name);
}

std::unordered_set<uint32_t>& Evaluator::GetStatements(Declaration::Type type) {
  if (type == Declaration::Type::kStmt) {
    return root->GetStmtNos();
  } else {
    if (!utils::kDeclTypeToStmtType.count(type)) {
      throw QueryException("unable to fetch stmt with the given type.");
    } else {
      auto stmt_type = utils::kDeclTypeToStmtType.at(type);
      return root->GetStmtNosByType(stmt_type);
    }
  }
}

bool Evaluator::IsStatement(uint32_t stmt_num, Declaration::Type type) {
  if (type == Declaration::Type::kStmt) {
    return root->IsStatement(stmt_num);
  } else {
    if (!utils::kDeclTypeToStmtType.count(type)) {
      throw QueryException("unable to fetch stmt with the given type.");
    } else {
      auto stmt_type = utils::kDeclTypeToStmtType.at(type);
      return root->IsStatementType(stmt_num, stmt_type);
    }
  }
}

std::unordered_set<std::string>& Evaluator::GetVariables() {
  return root->GetVarNames();
}

std::string Evaluator::GetAttrByStmt(uint32_t stmt_no) {
  return root->GetAttrByStmt(stmt_no);
}

bool Evaluator::IsVariable(std::string var_name) {
  return root->IsVariable(var_name);
}

// Affects clause

void Evaluator::PopulateAffects(bool transitive, bool bip) {
  root->PopulateAffectsAll(transitive, bip);
}

// Populate the Next cache for the given starting point.
void Evaluator::PopulateAffects(uint32_t pred, bool transitive, bool bip) {
  root->PopulateAffects(pred, transitive, bip);
}

bool Evaluator::HasPopulateAffects(bool transitive, bool bip) {
  return root->HasPopulateAffectsAll(transitive, bip);
}

pkb::Cache<uint32_t, uint32_t>& Evaluator::GetAffectsCache(bool transitive,
                                                           bool bip) {
  return root->GetAffectsCache(transitive, bip);
}

// Calls clause

pkb::Cache<std::string, std::string>& Evaluator::GetCallsCache(
    bool transitive) {
  return root->GetCallsCache(transitive);
}

// Parent clause

pkb::Cache<uint32_t, uint32_t>& Evaluator::GetParentCache(bool transitive) {
  return root->GetParentCache(transitive);
}

// Follows clause

pkb::Cache<uint32_t, uint32_t>& Evaluator::GetFollowsCache(bool transitive) {
  return root->GetFollowsCache(transitive);
}

// Uses clause

pkb::Cache<uint32_t, std::string>& Evaluator::GetUsesSCache() {
  return root->GetUsesStatementCache();
}

pkb::Cache<std::string, std::string>& Evaluator::GetUsesPCache() {
  return root->GetUsesProcedureCache();
}

// Modifies clause

pkb::Cache<uint32_t, std::string>& Evaluator::GetModifiesSCache() {
  return root->GetModifiesStatementCache();
}

pkb::Cache<std::string, std::string>& Evaluator::GetModifiesPCache() {
  return root->GetModifiesProcedureCache();
}

// Next clause

void Evaluator::PopulateNext(bool transitive, bool bip) {
  root->PopulateNextAll(transitive, bip);
}

// Populate the Next cache for the given starting point.
void Evaluator::PopulateNext(uint32_t pred, bool transitive, bool bip) {
  root->PopulateNext(pred, transitive, bip);
}

bool Evaluator::HasPopulateNext(bool transitive, bool bip) {
  return root->HasPopulateNextAll(transitive, bip);
}

pkb::Cache<uint32_t, uint32_t>& Evaluator::GetNextCache(bool transitive,
                                                        bool bip) {
  return root->GetNextCache(transitive, bip);
}

// Pattern clause

std::shared_ptr<ast::Expression> Evaluator::ParseExpression(std::string s) {
  std::shared_ptr<ast::Expression> expr = ast::Parser().ParseExpression(s);
  return expr;
}

bool Evaluator::PatternExpr(uint32_t stmt_no,
                            std::shared_ptr<ast::Expression> expr,
                            bool partial) {
  return root->MatchPatternExpr(stmt_no, expr, partial);
}

pkb::Cache<uint32_t, std::string>& Evaluator::GetPatternVarCache(
    Declaration::Type type) {
  if (type == Declaration::Type::kAssign) {
    return root->GetPatternAssignCache();
  } else if (type == Declaration::Type::kIf) {
    return root->GetPatternIfCache();
  } else if (type == Declaration::Type::kWhile) {
    return root->GetPatternWhileCache();
  } else {
    throw QueryException(
        "Pattern var cache is only available for assign, if, and while stmt.");
  }
}

}  // namespace pql

}  // namespace spa
