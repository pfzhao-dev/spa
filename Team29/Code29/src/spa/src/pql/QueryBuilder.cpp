#include "QueryBuilder.h"

#include <regex>
#include <string>

#include "QueryException.h"

namespace spa {

namespace pql {

QueryBuilder::QueryBuilder()
    : name_to_decl_{},
      prog_line_decls{},
      selected_decls_{},
      selected_boolean_{false},
      clauses_{} {};

void QueryBuilder::AddDeclaration(Declaration::Type decl_type,
                                  std::string decl_name) {
  if (decl_name.empty() || this->HasDeclaration(decl_name)) {
    throw QueryParseException(
        "Declaration name is empty or declaration is already added");
  }

  auto decl_ptr =
      std::make_shared<Declaration>(Declaration(decl_type, decl_name));
  this->name_to_decl_.insert({decl_name, decl_ptr});
}

void QueryBuilder::AddDeclarationProgLine(std::string decl_name) {
  this->AddDeclaration(Declaration::Type::kStmt, decl_name);
  this->prog_line_decls.push_back(decl_name);
}

void QueryBuilder::SelectBoolean() {
  if (this->HasDeclaration(kBooleanStr)) {
    this->SelectDeclaration(kBooleanStr, kEmptyStr);
  } else {
    this->selected_boolean_ = true;
  }
}

void QueryBuilder::SelectDeclaration(std::string selected_decl,
                                     std::string selected_decl_val_type) {
  this->selected_decls_.push_back(
      this->MakeDeclaration(selected_decl, selected_decl_val_type));
}

void QueryBuilder::AddRelationFollows(std::string pred, std::string succ,
                                      bool transitive) {
  std::shared_ptr<StatementRef> pred_ref = this->MakeStatementRef(pred);
  std::shared_ptr<StatementRef> succ_ref = this->MakeStatementRef(succ);

  std::shared_ptr<Follows> follows =
      std::make_shared<Follows>(Follows(pred_ref, succ_ref, transitive));
  this->clauses_.push_back(follows);
}

void QueryBuilder::AddRelationParent(std::string pred, std::string succ,
                                     bool transitive) {
  std::shared_ptr<StatementRef> pred_ref = this->MakeStatementRef(pred);
  std::shared_ptr<StatementRef> succ_ref = this->MakeStatementRef(succ);

  std::shared_ptr<Parent> parent =
      std::make_shared<Parent>(Parent(pred_ref, succ_ref, transitive));
  this->clauses_.push_back(parent);
}

void QueryBuilder::AddRelationUses(std::string subject, std::string target) {
  if (subject == kWildcardStr) {
    throw QueryParseException("First argument of Uses cannot be _");
  }

  std::shared_ptr<Ref> subject_ref =
      this->MakeRef(subject, kDeclTypesUsesSubject);
  std::shared_ptr<EntityRef> target_ref =
      this->MakeEntityRef(target, kDeclTypesUsesModifiesTargetPatternVar);

  std::shared_ptr<Uses> uses =
      std::make_shared<Uses>(Uses(subject_ref, target_ref));
  this->clauses_.push_back(uses);
}

void QueryBuilder::AddRelationModifies(std::string subject,
                                       std::string target) {
  if (subject == kWildcardStr) {
    throw QueryParseException("First argument of Modifies cannot be _");
  }

  std::shared_ptr<Ref> subject_ref =
      this->MakeRef(subject, kDeclTypesModifiesSubject);
  std::shared_ptr<EntityRef> target_ref =
      this->MakeEntityRef(target, kDeclTypesUsesModifiesTargetPatternVar);

  std::shared_ptr<Modifies> modifies =
      std::make_shared<Modifies>(Modifies(subject_ref, target_ref));
  this->clauses_.push_back(modifies);
}

void QueryBuilder::AddRelationAffects(std::string affector,
                                      std::string affected, bool transitive,
                                      bool is_bip) {
  std::shared_ptr<StatementRef> affector_ref =
      this->MakeStatementRef(affector, kDeclTypesAffects);
  std::shared_ptr<StatementRef> affected_ref =
      this->MakeStatementRef(affected, kDeclTypesAffects);

  std::shared_ptr<Affects> affects = std::make_shared<Affects>(
      Affects(affector_ref, affected_ref, transitive, is_bip));
  this->clauses_.push_back(affects);
}

void QueryBuilder::AddRelationCalls(std::string caller, std::string callee,
                                    bool transitive) {
  std::shared_ptr<EntityRef> caller_ref =
      this->MakeEntityRef(caller, kDeclTypesCalls);
  std::shared_ptr<EntityRef> callee_ref =
      this->MakeEntityRef(callee, kDeclTypesCalls);

  std::shared_ptr<Calls> calls =
      std::make_shared<Calls>(Calls(caller_ref, callee_ref, transitive));
  this->clauses_.push_back(calls);
}

void QueryBuilder::AddRelationNext(std::string pred, std::string succ,
                                   bool transitive, bool is_bip) {
  std::shared_ptr<StatementRef> pred_ref = this->MakeStatementRef(pred);
  std::shared_ptr<StatementRef> succ_ref = this->MakeStatementRef(succ);

  std::shared_ptr<Next> calls =
      std::make_shared<Next>(Next(pred_ref, succ_ref, transitive, is_bip));
  this->clauses_.push_back(calls);
}

void QueryBuilder::AddPattern(std::string type, std::string var,
                              std::string expr_spec, std::string expr_spec_2) {
  std::shared_ptr<StatementRef> type_ref =
      this->MakeStatementRef(type, kDeclTypesPatternType);
  std::shared_ptr<EntityRef> var_ref =
      this->MakeEntityRef(var, kDeclTypesUsesModifiesTargetPatternVar);

  if (type_ref->IsDeclTypeIn({Declaration::Type::kAssign})) {
    this->AddPatternAssign(type_ref, var_ref, expr_spec, expr_spec_2);
  } else if (type_ref->IsDeclTypeIn({Declaration::Type::kIf})) {
    this->AddPatternIf(type_ref, var_ref, expr_spec, expr_spec_2);
  } else if (type_ref->IsDeclTypeIn({Declaration::Type::kWhile})) {
    this->AddPatternWhile(type_ref, var_ref, expr_spec, expr_spec_2);
  }
}

void QueryBuilder::AddWithCompare(std::string lhs_name, std::string rhs_name,
                                  std::string lhs_val_type,
                                  std::string rhs_val_type) {
  // make ref
  std::shared_ptr<Ref> lhs_ref = this->MakeRef(lhs_name, {}, lhs_val_type, 0);
  std::shared_ptr<Ref> rhs_ref = this->MakeRef(rhs_name, {}, rhs_val_type, 0);

  // check that the only ref without value of type prog_line
  if (lhs_val_type.empty() && lhs_ref->IsDeclared() &&
      !this->IsProgLineDeclaration(lhs_name)) {
    throw QueryParseException(
        "Declaration type of lhs of with clause is invalid");
  }
  if (rhs_val_type.empty() && rhs_ref->IsDeclared() &&
      !this->IsProgLineDeclaration(rhs_name)) {
    throw QueryParseException(
        "Declaration type of rhs of with clause is invalid");
  }

  // check whether lhs value type == rhs value type
  if (this->GetReturnType(lhs_ref, lhs_name) !=
      this->GetReturnType(rhs_ref, rhs_name)) {
    throw QueryParseException(
        "Return type of lhs and rhs of with clause is not equal");
  }

  std::shared_ptr<Compare> compare =
      std::make_shared<Compare>(Compare(lhs_ref, rhs_ref));
  this->clauses_.push_back(compare);
}

void QueryBuilder::Reset() {
  this->name_to_decl_.clear();
  this->prog_line_decls.clear();
  this->selected_decls_.clear();
  this->selected_boolean_ = false;
  this->clauses_.clear();
}

Query QueryBuilder::Build() const {
  if (!this->CanBuild()) {
    throw QueryParseException("Query cannot be built");
  }

  std::shared_ptr<Result> result_ptr;
  if (this->selected_boolean_) {
    result_ptr = std::make_shared<BooleanResult>(BooleanResult());
  } else {
    result_ptr =
        std::make_shared<DeclaredResult>(DeclaredResult(this->selected_decls_));
  }

  return Query(result_ptr, this->clauses_);
}

const std::string QueryBuilder::kEmptyStr = "";

bool QueryBuilder::HasDeclaration(std::string decl) const {
  return this->name_to_decl_.count(decl);
}

bool QueryBuilder::IsProgLineDeclaration(std::string decl) const {
  return std::find(this->prog_line_decls.begin(), this->prog_line_decls.end(),
                   decl) != this->prog_line_decls.end();
}

std::shared_ptr<Declaration> QueryBuilder::GetDeclaration(
    std::string decl) const {
  return this->name_to_decl_.at(decl);
}

std::shared_ptr<Declaration> QueryBuilder::MakeDeclaration(
    std::string decl, std::string decl_val_type) const {
  if (decl.empty() || !this->HasDeclaration(decl)) {
    throw QueryParseException(
        "Declaration name and value type is empty or is not declared");
  }

  if (decl_val_type.empty()) return this->GetDeclaration(decl);

  // decl_val_type is non-empty
  return this->MakeDeclarationWithValue(decl, decl_val_type);
}

std::shared_ptr<Declaration> QueryBuilder::MakeDeclarationWithValue(
    std::string decl, std::string decl_val_type) const {
  if (this->IsProgLineDeclaration(decl)) {
    throw QueryParseException("Declaration value type is invalid");
  }

  auto decl_ptr = this->GetDeclaration(decl);

  // check whether value type is valid for declaration type
  Declaration::ValueType val_type = kStringToDeclValType.at(decl_val_type);
  bool isValid = false;
  switch (val_type) {
    case spa::pql::Declaration::ValueType::kProcName:
      isValid = decl_ptr->IsTypeIn(
          {Declaration::Type::kProcedure, Declaration::Type::kCall});
      break;
    case spa::pql::Declaration::ValueType::kVarName:
      isValid = decl_ptr->IsTypeIn({Declaration::Type::kVariable,
                                    Declaration::Type::kRead,
                                    Declaration::Type::kPrint});
      break;
    case spa::pql::Declaration::ValueType::kValue:
      isValid = decl_ptr->IsTypeIn({Declaration::Type::kConstant});
      break;
    case spa::pql::Declaration::ValueType::kStmtNum:
      isValid = decl_ptr->IsStatement();
      break;
  }

  if (!isValid) {
    throw QueryParseException("Declaration value type is invalid");
  }

  return std::make_shared<Declaration>(
      Declaration(decl_ptr->GetType(), val_type, decl));
}

Declaration::ReturnType QueryBuilder::GetReturnType(
    std::shared_ptr<Ref> ref_ptr, std::string name) {
  if (this->IsProgLineDeclaration(name)) {  // e.g. prog_line
    return Declaration::ReturnType::kInteger;
  }

  auto decl = ref_ptr->GetDeclaration();

  if (ref_ptr->IsDeclared() && decl->HasValue()) {  // e.g. s.stmt#
    return decl->GetReturnTypeWithValue();
  }

  if (ref_ptr->IsStatement()) {  // e.g. 1
    return Declaration::ReturnType::kInteger;
  }

  if (name.front() == kQuoteChar) {  // e.g. "x"
    return Declaration::ReturnType::kName;
  }

  throw QueryParseException("Ref of compare clause is invalid");
}

std::shared_ptr<StatementRef> QueryBuilder::MakeStatementRef(
    std::string s, std::vector<Declaration::Type> allowed_decl_types,
    uint32_t min) const {
  if (s == kWildcardStr) {
    return std::make_shared<StatementRef>(StatementRef());
  }

  // try to parse into a statement number
  try {
    uint32_t stmt_num = std::stoi(s);

    if (stmt_num < min) {
      throw QueryParseException(
          "Statement number used in statement ref is invalid");
    }

    return std::make_shared<StatementRef>(StatementRef(stmt_num));
  } catch (const std::invalid_argument& e) {
  }

  if (HasDeclaration(s)) {  // is a declaration
    auto declaration = this->GetDeclaration(s);

    if (!allowed_decl_types.empty() &&
        !declaration->IsTypeIn(allowed_decl_types)) {
      throw QueryParseException(
          "Declaration type used in statement ref is not valid");
    }

    return std::make_shared<StatementRef>(declaration);
  }
  throw QueryParseException(
      "Declaration used in statement ref is not declared");
}

std::shared_ptr<EntityRef> QueryBuilder::MakeEntityRef(
    std::string s, std::vector<Declaration::Type> allowed_decl_types,
    std::string val) const {
  if (s == kWildcardStr && val.empty()) {
    return std::make_shared<EntityRef>(EntityRef());
  }

  if (s.front() == kQuoteChar && val.empty()) {  // is an identifier
    std::regex quote_regex(R"(\s*\"\s*|\s*\"\s*)");
    std::string id = std::regex_replace(s, quote_regex, "$1");
    return std::make_shared<EntityRef>(EntityRef(id));
  }

  return this->MakeDeclaredEntityRef(s, allowed_decl_types, val);
}

std::shared_ptr<EntityRef> QueryBuilder::MakeDeclaredEntityRef(
    std::string s, std::vector<Declaration::Type> allowed_decl_types,
    std::string val) const {
  auto declaration = this->MakeDeclaration(s, val);

  if (!allowed_decl_types.empty() &&
      !declaration->IsTypeIn(allowed_decl_types)) {
    throw QueryParseException(
        "Declaration type used in entity ref is not valid");
  }

  return std::make_shared<EntityRef>(declaration);
}

std::shared_ptr<Ref> QueryBuilder::MakeRef(
    std::string s, std::vector<Declaration::Type> allowed_decl_types,
    std::string val, uint32_t min) const {
  if (val.empty()) {
    // try to parse into a statement number
    try {
      uint32_t stmt_num = std::stoi(s);

      if (stmt_num < min) {
        throw QueryParseException(
            "Statement number used in statement ref is invalid");
      }

      return std::make_shared<StatementRef>(StatementRef(stmt_num));
    } catch (const std::invalid_argument& e) {
    }
  }

  return this->MakeEntityRef(s, allowed_decl_types, val);
}

Pattern::Expression QueryBuilder::MakeExpression(
    const std::string expr_spec) const {
  if (expr_spec == kWildcardStr) {
    return Pattern::Expression();
  }

  if (expr_spec.front() == kQuoteChar) {  // is of form "expression"
    std::regex quote_regex(R"(\s*\"\s*|\s*\"\s*)");
    std::string spec = std::regex_replace(expr_spec, quote_regex, "$1");
    return Pattern::Expression(spec);
  }

  if (expr_spec.front() == kWildcardChar) {  // is of form _"expression"_
    std::regex underscore_quote_regex(R"(\s*_\s*\"\s*|\s*\"\s*_\s*)");
    std::string spec =
        std::regex_replace(expr_spec, underscore_quote_regex, "$1");
    return Pattern::Expression(spec, true);
  }

  throw QueryParseException("Expression spec is invalid");
}

void QueryBuilder::AddPatternAssign(std::shared_ptr<StatementRef> type_ref,
                                    std::shared_ptr<EntityRef> var_ref,
                                    std::string expr_spec,
                                    std::string expr_spec_2) {
  if (!expr_spec_2.empty()) {
    throw QueryParseException(
        "Pattern with assign type cannot have a third argument");
  }

  Pattern::Expression expr = this->MakeExpression(expr_spec);

  std::shared_ptr<Pattern> pattern =
      std::make_shared<Pattern>(Pattern(type_ref, var_ref, expr));
  this->clauses_.push_back(pattern);
}

void QueryBuilder::AddPatternIf(std::shared_ptr<StatementRef> type_ref,
                                std::shared_ptr<EntityRef> var_ref,
                                std::string expr_spec,
                                std::string expr_spec_2) {
  if (expr_spec != kWildcardStr || expr_spec_2 != kWildcardStr) {
    throw QueryParseException(
        "Second or third argument of pattern with if type is invalid");
  }

  Pattern::Expression expr = this->MakeExpression(expr_spec);

  std::shared_ptr<Pattern> pattern =
      std::make_shared<Pattern>(Pattern(type_ref, var_ref, expr));
  this->clauses_.push_back(pattern);
}

void QueryBuilder::AddPatternWhile(std::shared_ptr<StatementRef> type_ref,
                                   std::shared_ptr<EntityRef> var_ref,
                                   std::string expr_spec,
                                   std::string expr_spec_2) {
  if (!expr_spec_2.empty()) {
    throw QueryParseException(
        "Pattern with while type cannot have a third argument");
  }

  if (expr_spec != kWildcardStr) {
    throw QueryParseException(
        "Second argument of pattern with while type is invalid");
  }

  Pattern::Expression expr = this->MakeExpression(expr_spec);

  std::shared_ptr<Pattern> pattern =
      std::make_shared<Pattern>(Pattern(type_ref, var_ref, expr));
  this->clauses_.push_back(pattern);
}

bool QueryBuilder::CanBuild() const {
  return (this->selected_boolean_ && this->selected_decls_.empty()) ||
         (!this->selected_boolean_ && !this->selected_decls_.empty());
}

}  // namespace pql

}  // namespace spa
