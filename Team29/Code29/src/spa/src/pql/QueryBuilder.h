#pragma once

#include "Query.h"
#include "pql/clause/Affects.h"
#include "pql/clause/Calls.h"
#include "pql/clause/Compare.h"
#include "pql/clause/Follows.h"
#include "pql/clause/Modifies.h"
#include "pql/clause/Next.h"
#include "pql/clause/Parent.h"
#include "pql/clause/Pattern.h"
#include "pql/clause/Uses.h"

namespace spa {

namespace pql {

// builds the query and handles semantic errors
class QueryBuilder {
 public:
  QueryBuilder();

  // add declarations
  void AddDeclaration(Declaration::Type decl_type, std::string decl_name);
  void AddDeclarationProgLine(std::string decl_name);

  void SelectBoolean();

  // add declaration to builder before selecting it
  // select declarations in order
  void SelectDeclaration(std::string selected_decl,
                         std::string selected_decl_val_type = kEmptyStr);

  // add relational clauses
  void AddRelationFollows(std::string pred, std::string succ, bool transitive);
  void AddRelationParent(std::string pred, std::string succ, bool transitive);
  void AddRelationUses(std::string subject, std::string target);
  void AddRelationModifies(std::string subject, std::string target);
  void AddRelationAffects(std::string affector, std::string affected,
                          bool transitive, bool is_bip);
  void AddRelationCalls(std::string caller, std::string callee,
                        bool transitive);
  void AddRelationNext(std::string pred, std::string succ, bool transitive,
                       bool is_bip);

  // add pattern clauses
  void AddPattern(std::string type, std::string var, std::string expr_spec,
                  std::string expr_spec_2);

  // add with clauses
  void AddWithCompare(std::string lhs_name, std::string rhs_name,
                      std::string lhs_val_type = kEmptyStr,
                      std::string rhs_val_type = kEmptyStr);

  void Reset();
  Query Build() const;

 private:
  std::unordered_map<std::string, std::shared_ptr<Declaration>,
                     std::hash<std::string>, std::equal_to<std::string>>
      name_to_decl_;
  std::vector<std::string> prog_line_decls;
  std::vector<std::shared_ptr<Declaration>> selected_decls_;
  bool selected_boolean_;
  std::vector<std::shared_ptr<Clause>> clauses_;

  static const std::string kEmptyStr;
  const std::string kWildcardStr = "_";
  const std::string kBooleanStr = "BOOLEAN";
  const char kWildcardChar = '_';
  const char kQuoteChar = '"';

  const std::vector<Declaration::Type> kDeclTypesUsesSubject = {
      Declaration::Type::kStmt,     Declaration::Type::kPrint,
      Declaration::Type::kCall,     Declaration::Type::kWhile,
      Declaration::Type::kIf,       Declaration::Type::kAssign,
      Declaration::Type::kProcedure};
  const std::vector<Declaration::Type> kDeclTypesModifiesSubject = {
      Declaration::Type::kStmt,     Declaration::Type::kRead,
      Declaration::Type::kCall,     Declaration::Type::kWhile,
      Declaration::Type::kIf,       Declaration::Type::kAssign,
      Declaration::Type::kProcedure};
  const std::vector<Declaration::Type> kDeclTypesUsesModifiesTargetPatternVar =
      {Declaration::Type::kVariable};
  const std::vector<Declaration::Type> kDeclTypesPatternType = {
      Declaration::Type::kAssign, Declaration::Type::kIf,
      Declaration::Type::kWhile};
  const std::vector<Declaration::Type> kDeclTypesAffects = {
      Declaration::Type::kAssign, Declaration::Type::kStmt};
  const std::vector<Declaration::Type> kDeclTypesCalls = {
      Declaration::Type::kProcedure};

  // declarations
  bool HasDeclaration(std::string decl) const;
  bool IsProgLineDeclaration(std::string decl) const;
  std::shared_ptr<Declaration> GetDeclaration(std::string decl) const;
  std::shared_ptr<Declaration> MakeDeclaration(std::string decl,
                                               std::string decl_val_type) const;
  std::shared_ptr<Declaration> MakeDeclarationWithValue(
      std::string decl, std::string decl_val_type) const;

  // return type
  Declaration::ReturnType GetReturnType(std::shared_ptr<Ref> ref,
                                        std::string name);

  // make refs
  // empty allowed_decl_types implies all types are allowed
  std::shared_ptr<StatementRef> MakeStatementRef(
      std::string s, std::vector<Declaration::Type> allowed_decl_types = {},
      uint32_t min = 1) const;
  std::shared_ptr<EntityRef> MakeEntityRef(
      std::string s, std::vector<Declaration::Type> allowed_decl_types = {},
      std::string val = "") const;
  std::shared_ptr<EntityRef> MakeDeclaredEntityRef(
      std::string s, std::vector<Declaration::Type> allowed_decl_types = {},
      std::string val = "") const;
  std::shared_ptr<Ref> MakeRef(
      std::string s, std::vector<Declaration::Type> allowed_decl_types = {},
      std::string val = "", uint32_t min = 1) const;

  // make expression
  Pattern::Expression MakeExpression(const std::string expr_spec) const;

  // add patterns
  void AddPatternAssign(std::shared_ptr<StatementRef> type_ref,
                        std::shared_ptr<EntityRef> var_ref,
                        std::string expr_spec, std::string expr_spec_2);
  void AddPatternIf(std::shared_ptr<StatementRef> type_ref,
                    std::shared_ptr<EntityRef> var_ref, std::string expr_spec,
                    std::string expr_spec_2);
  void AddPatternWhile(std::shared_ptr<StatementRef> type_ref,
                       std::shared_ptr<EntityRef> var_ref,
                       std::string expr_spec, std::string expr_spec_2);

  bool CanBuild() const;
};

}  // namespace pql

}  // namespace spa
