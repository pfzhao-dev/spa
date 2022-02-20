#include "UsesData.h"

#include "../DeclarationData.h"
#include "../QueryStringBuilder.h"
#include "../RefBuilder.h"
#include "catch.hpp"

namespace test {

namespace pql {

namespace data {

namespace uses {

using namespace spa::pql;

// make shared pointer to uses with entity ref for subject based on
// specified arguments and argument type
// pass in argument "" for entity wildcard
template <typename T, typename V>
std::shared_ptr<Uses> MakeUsesEntity(T t, V v) {
  EntityRef entity_ref_subject = MakeEntityRef(t);
  EntityRef entity_ref_target = MakeEntityRef(v);

  Uses uses(std::make_shared<EntityRef>(entity_ref_subject),
            std::make_shared<EntityRef>(entity_ref_target));
  return std::make_shared<Uses>(uses);
}

// make shared pointer to uses with statement ref for subject based on
// specified arguments and argument type
// pass in argument "" for entity wildcard
template <typename T, typename V>
std::shared_ptr<Uses> MakeUsesStatement(T t, V v) {
  StatementRef statement_ref_subject = MakeStatementRef(t);
  EntityRef entity_ref_target = MakeEntityRef(v);

  Uses uses(std::make_shared<StatementRef>(statement_ref_subject),
            std::make_shared<EntityRef>(entity_ref_target));
  return std::make_shared<Uses>(uses);
}

const StringToStringFunc kStrFuncStmtNoVar = [](std::string declaration_name) {
  return MakeRelationString(kUses, "3", "\"x\"");
};
const StringToStringFunc kStrFuncStmtNoDecl = [](std::string declaration_name) {
  return MakeRelationString(kUses, "4", declaration_name);
};
const StringToStringFunc kStrFuncStmtNoWild = [](std::string declaration_name) {
  return MakeRelationString(kUses, "5", kWildcardStr);
};
const StringToStringFunc kStrFuncDeclVar = [](std::string declaration_name) {
  return MakeRelationString(kUses, declaration_name, "\"x\"");
};
const StringToStringFunc kStrFuncDeclWild = [](std::string declaration_name) {
  return MakeRelationString(kUses, declaration_name, kWildcardStr);
};
const StringToStringFunc kStrFuncProcVar = [](std::string declaration_name) {
  return MakeRelationString(kUses, "\"proc\"", "\"x\"");
};
const StringToStringFunc kStrFuncProcDecl = [](std::string declaration_name) {
  return MakeRelationString(kUses, "\"proc\"", declaration_name);
};
const StringToStringFunc kStrFuncProcWild = [](std::string declaration_name) {
  return MakeRelationString(kUses, "\"proc\"", kWildcardStr);
};
const StringToStringFunc kStrFuncDeclDefaultDecl =
    [](std::string declaration_name) {
      return MakeRelationString(kUses, declaration_name,
                                declaration::kDefaultDeclVariable);
    };

// first argument is '_'
const StringToStringFunc kStrFuncWildVar = [](std::string declaration_name) {
  return MakeRelationString(kUses, kWildcardStr, "\"x\"");
};
const StringToStringFunc kStrFuncWildWild = [](std::string declaration_name) {
  return MakeRelationString(kUses, kWildcardStr, kWildcardStr);
};
const StringToStringFunc kStrFuncWildDecl = [](std::string declaration_name) {
  return MakeRelationString(kUses, kWildcardStr, declaration_name);
};

// 0 stmt number
const StringToStringFunc kStrFuncZeroStmtNoVar =
    [](std::string declaration_name) {
      return MakeRelationString(kUses, "0", "\"x\"");
    };

// negative stmt number
const StringToStringFunc kStrFuncNegativeStmtNoVar =
    [](std::string declaration_name) {
      return MakeRelationString(kUses, "-1", "\"x\"");
    };

// invalid stmt number
const StringToStringFunc kStrFuncInvalidStmtNoVar =
    [](std::string declaration_name) {
      return MakeRelationString(kUses, "4.0", "\"x\"");
    };

// invalid declaration type
const StringToStringFunc kStrFuncDeclDecl = [](std::string declaration_name) {
  return MakeRelationString(kUses, declaration_name, declaration_name);
};

// declaration name selected not defined
const StringToStringFunc kStrFuncInvalidDeclVar =
    [](std::string declaration_name) {
      return MakeRelationString(kUses, "invaliddeclarationname", "\"x\"");
    };

// variable selected not defined
const StringToStringFunc kStrFuncStmtNoInvalidVar =
    [](std::string declaration_name) {
      return MakeRelationString(kUses, "1", "xyz");
    };

// no matching '"' for entity ref
const StringToStringFunc kStrFuncStmtNoMissingQuoteVar =
    [](std::string declaration_name) {
      return MakeRelationString(kUses, "1", "\"x");
    };

const DeclPtrToClausePtrVecFunc kClausesFuncStmtNoVar =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeUsesStatement<uint32_t, std::string>(3, "x")};
};
const DeclPtrToClausePtrVecFunc kClausesFuncStmtNoDecl =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeUsesStatement<uint32_t, std::shared_ptr<Declaration>>(
      4, expected_declaration)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncStmtNoWild =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeUsesStatement<uint32_t, std::string>(5, "")};
};
const DeclPtrToClausePtrVecFunc kClausesFuncDeclVar =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeUsesEntity<std::shared_ptr<Declaration>, std::string>(
      expected_declaration, "x")};
};
const DeclPtrToClausePtrVecFunc kClausesFuncDeclWild =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeUsesEntity<std::shared_ptr<Declaration>, std::string>(
      expected_declaration, "")};
};
const DeclPtrToClausePtrVecFunc kClausesFuncProcVar =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeUsesEntity<std::string, std::string>("proc", "x")};
};
const DeclPtrToClausePtrVecFunc kClausesFuncProcDecl =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeUsesEntity<std::string, std::shared_ptr<Declaration>>(
      "proc", expected_declaration)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncProcWild =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeUsesEntity<std::string, std::string>("proc", "")};
};
const DeclPtrToClausePtrVecFunc kClausesFuncDeclDefaultDecl =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeUsesEntity<std::shared_ptr<Declaration>,
                         std::shared_ptr<Declaration>>(
      expected_declaration, declaration::kDefaultDeclNameToDeclPtr.at(
                                declaration::kDefaultDeclVariable))};
};

// pairs of functions that construct query strings and expected clauses
const ClausesPair kPairStmtNoVar(kStrFuncStmtNoVar, kClausesFuncStmtNoVar);
const ClausesPair kPairStmtNoDecl(kStrFuncStmtNoDecl, kClausesFuncStmtNoDecl);
const ClausesPair kPairStmtNoWild(kStrFuncStmtNoWild, kClausesFuncStmtNoWild);
const ClausesPair kPairDeclVar(kStrFuncDeclVar, kClausesFuncDeclVar);
const ClausesPair kPairDeclWild(kStrFuncDeclWild, kClausesFuncDeclWild);
const ClausesPair kPairProcVar(kStrFuncProcVar, kClausesFuncProcVar);
const ClausesPair kPairProcDecl(kStrFuncProcDecl, kClausesFuncProcDecl);
const ClausesPair kPairProcWild(kStrFuncProcWild, kClausesFuncProcWild);
const ClausesPair kPairDeclDefaultDecl(kStrFuncDeclDefaultDecl,
                                       kClausesFuncDeclDefaultDecl);

// clauses pairs for positive uses test cases
const std::vector<ClausesPair> kPairsPositiveVar = {
    kPairStmtNoVar, kPairStmtNoDecl, kPairStmtNoWild,
    kPairProcVar,   kPairProcDecl,   kPairProcWild};
const std::vector<ClausesPair> kPairsPositiveStmts = {
    kPairStmtNoVar, kPairStmtNoWild, kPairDeclVar,        kPairDeclWild,
    kPairProcVar,   kPairProcWild,   kPairDeclDefaultDecl};
const std::vector<ClausesPair> kPairsPositive = {
    kPairStmtNoVar, kPairStmtNoWild, kPairProcVar, kPairProcWild};

// functions for negative uses test cases
const std::vector<StringToStringFunc> kStrFuncsNegativeVar = {
    kStrFuncDeclVar,
    kStrFuncDeclWild,
    kStrFuncWildVar,
    kStrFuncWildWild,
    kStrFuncWildDecl,
    kStrFuncDeclDefaultDecl,
    kStrFuncZeroStmtNoVar,
    kStrFuncNegativeStmtNoVar,
    kStrFuncInvalidStmtNoVar,
    kStrFuncDeclDecl,
    kStrFuncInvalidDeclVar,
    kStrFuncStmtNoInvalidVar,
    kStrFuncStmtNoMissingQuoteVar};
const std::vector<StringToStringFunc> kStrFuncsNegativeStmts = {
    kStrFuncStmtNoDecl,        kStrFuncProcDecl,
    kStrFuncWildVar,           kStrFuncWildWild,
    kStrFuncWildDecl,          kStrFuncZeroStmtNoVar,
    kStrFuncNegativeStmtNoVar, kStrFuncInvalidStmtNoVar,
    kStrFuncDeclDecl,          kStrFuncInvalidDeclVar,
    kStrFuncStmtNoInvalidVar,  kStrFuncStmtNoMissingQuoteVar};
const std::vector<StringToStringFunc> kStrFuncsNegative = {
    kStrFuncStmtNoDecl,
    kStrFuncDeclVar,
    kStrFuncDeclWild,
    kStrFuncProcDecl,
    kStrFuncWildVar,
    kStrFuncWildWild,
    kStrFuncWildDecl,
    kStrFuncDeclDefaultDecl,
    kStrFuncZeroStmtNoVar,
    kStrFuncNegativeStmtNoVar,
    kStrFuncInvalidStmtNoVar,
    kStrFuncDeclDecl,
    kStrFuncInvalidDeclVar,
    kStrFuncStmtNoInvalidVar,
    kStrFuncStmtNoMissingQuoteVar};

const std::vector<ClausesPair> kGetClausesPositive(
    Declaration::Type declaration_type) {
  switch (declaration_type) {
    case Declaration::Type::kVariable:
      return kPairsPositiveVar;
    case Declaration::Type::kStmt:
    case Declaration::Type::kPrint:
    case Declaration::Type::kCall:
    case Declaration::Type::kWhile:
    case Declaration::Type::kIf:
    case Declaration::Type::kAssign:
    case Declaration::Type::kProcedure:
      return kPairsPositiveStmts;
    default:
      return kPairsPositive;
  }
}

const std::vector<StringToStringFunc> kGetFuncsNegative(
    Declaration::Type declaration_type) {
  switch (declaration_type) {
    case Declaration::Type::kVariable:
      return kStrFuncsNegativeVar;
    case Declaration::Type::kStmt:
    case Declaration::Type::kPrint:
    case Declaration::Type::kCall:
    case Declaration::Type::kWhile:
    case Declaration::Type::kIf:
    case Declaration::Type::kAssign:
    case Declaration::Type::kProcedure:
      return kStrFuncsNegativeStmts;
    default:
      return kStrFuncsNegative;
  }
}
}  // namespace uses
}  // namespace data

}  // namespace pql

}  // namespace test
