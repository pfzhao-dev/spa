#include "ModifiesData.h"

#include "../DeclarationData.h"
#include "../QueryStringBuilder.h"
#include "../RefBuilder.h"
#include "catch.hpp"

namespace test {

namespace pql {

namespace data {

namespace modifies {

using namespace spa::pql;

// make shared pointer to modifies with entity ref for subject based on
// specified arguments and argument type
// pass in argument "" for entity wildcard
template <typename T, typename V>
std::shared_ptr<Modifies> MakeModifiesEntity(T t, V v) {
  EntityRef entity_ref_subject = MakeEntityRef(t);
  EntityRef entity_ref_target = MakeEntityRef(v);

  Modifies modifies(std::make_shared<EntityRef>(entity_ref_subject),
                    std::make_shared<EntityRef>(entity_ref_target));
  return std::make_shared<Modifies>(modifies);
}

// make shared pointer to modifies with statement ref for subject based on
// specified arguments and argument type
// pass in argument "" for entity wildcard
template <typename T, typename V>
std::shared_ptr<Modifies> MakeModifiesStatement(T t, V v) {
  StatementRef statement_ref_subject = MakeStatementRef(t);
  EntityRef entity_ref_target = MakeEntityRef(v);

  Modifies modifies(std::make_shared<StatementRef>(statement_ref_subject),
                    std::make_shared<EntityRef>(entity_ref_target));
  return std::make_shared<Modifies>(modifies);
}

const StringToStringFunc kStrFuncStmtNoVar = [](std::string declaration_name) {
  // equivalent to MakeRelationString(kModifies, "3", "\"x\"");
  return "such that Modifies ( 3 , \" x \")";
};
const StringToStringFunc kStrFuncStmtNoDecl = [](std::string declaration_name) {
  return MakeRelationString(kModifies, "4", declaration_name);
};
const StringToStringFunc kStrFuncStmtNoWild = [](std::string declaration_name) {
  return MakeRelationString(kModifies, "5", kWildcardStr);
};
const StringToStringFunc kStrFuncDeclVar = [](std::string declaration_name) {
  return MakeRelationString(kModifies, declaration_name, "\"x\"");
};
const StringToStringFunc kStrFuncDeclWild = [](std::string declaration_name) {
  return MakeRelationString(kModifies, declaration_name, kWildcardStr);
};
const StringToStringFunc kStrFuncProcVar = [](std::string declaration_name) {
  return MakeRelationString(kModifies, "\"proc\"", "\"x\"");
};
const StringToStringFunc kStrFuncProcDecl = [](std::string declaration_name) {
  return MakeRelationString(kModifies, "\"proc\"", declaration_name);
};
const StringToStringFunc kStrFuncProcWild = [](std::string declaration_name) {
  return MakeRelationString(kModifies, "\"proc\"", kWildcardStr);
};
const StringToStringFunc kStrFuncDeclDefaultDecl =
    [](std::string declaration_name) {
      return MakeRelationString(kModifies, declaration_name,
                                declaration::kDefaultDeclVariable);
    };

// first argument is '_'
const StringToStringFunc kStrFuncWildVar = [](std::string declaration_name) {
  return MakeRelationString(kModifies, kWildcardStr, "\"x\"");
};
const StringToStringFunc kStrFuncWildWild = [](std::string declaration_name) {
  return MakeRelationString(kModifies, kWildcardStr, kWildcardStr);
};
const StringToStringFunc kStrFuncWildDecl = [](std::string declaration_name) {
  return MakeRelationString(kModifies, kWildcardStr, declaration_name);
};

// 0 stmt number
const StringToStringFunc kStrFuncZeroStmtNoVar =
    [](std::string declaration_name) {
      return MakeRelationString(kModifies, "0", "\"x\"");
    };

// negative stmt number
const StringToStringFunc kStrFuncNegativeStmtNoVar =
    [](std::string declaration_name) {
      return MakeRelationString(kModifies, "-1", "\"x\"");
    };

// invalid stmt number
const StringToStringFunc kStrFuncInvalidStmtNoVar =
    [](std::string declaration_name) {
      return MakeRelationString(kModifies, "4.0", "\"x\"");
    };

// invalid declaration type
const StringToStringFunc kStrFuncDelDel = [](std::string declaration_name) {
  return MakeRelationString(kModifies, declaration_name, declaration_name);
};

// declaration name selected not defined
const StringToStringFunc kStrFuncInvalidDeclVar =
    [](std::string declaration_name) {
      return MakeRelationString(kModifies, "invalideclarationname", "\"x\"");
    };

// variable selected not defined
const StringToStringFunc kStrFuncStmtNoInvalidVar =
    [](std::string declaration_name) {
      return MakeRelationString(kModifies, "1", "xyz");
    };

// no matching '"' for entity ref
const StringToStringFunc kStrFuncStmtNoMissingQuoteVar =
    [](std::string declaration_name) {
      return MakeRelationString(kModifies, "1", "\"x");
    };

const DeclPtrToClausePtrVecFunc kClausesFuncStmtNoVar =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeModifiesStatement<uint32_t, std::string>(3, "x")};
};
const DeclPtrToClausePtrVecFunc kClausesFuncStmtNoDecl =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeModifiesStatement<uint32_t, std::shared_ptr<Declaration>>(
      4, expected_declaration)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncStmtNoWild =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeModifiesStatement<uint32_t, std::string>(5, "")};
};
const DeclPtrToClausePtrVecFunc kClausesFuncDeclVar =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeModifiesEntity<std::shared_ptr<Declaration>, std::string>(
      expected_declaration, "x")};
};
const DeclPtrToClausePtrVecFunc kClausesFuncDeclWild =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeModifiesEntity<std::shared_ptr<Declaration>, std::string>(
      expected_declaration, "")};
};
const DeclPtrToClausePtrVecFunc kClausesFuncProcVar =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeModifiesEntity<std::string, std::string>("proc", "x")};
};
const DeclPtrToClausePtrVecFunc kClausesFuncProcDecl =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeModifiesEntity<std::string, std::shared_ptr<Declaration>>(
      "proc", expected_declaration)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncProcWild =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeModifiesEntity<std::string, std::string>("proc", "")};
};
const DeclPtrToClausePtrVecFunc kClausesFuncDeclDefaultDecl =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeModifiesEntity<std::shared_ptr<Declaration>,
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

// clauses pairs for positive modifies test cases
const std::vector<ClausesPair> kPairsPositiveVar = {
    kPairStmtNoVar, kPairStmtNoDecl, kPairStmtNoWild,
    kPairProcVar,   kPairProcDecl,   kPairProcWild};
const std::vector<ClausesPair> kPairsPositiveStmts = {
    kPairStmtNoVar, kPairStmtNoWild, kPairDeclVar, kPairDeclDefaultDecl,
    kPairDeclWild,  kPairProcVar,    kPairProcWild};
const std::vector<ClausesPair> kPairsPositive = {
    kPairStmtNoVar, kPairStmtNoWild, kPairProcVar, kPairProcWild};

// functions for negative modifies test cases
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
    kStrFuncDelDel,
    kStrFuncInvalidDeclVar,
    kStrFuncStmtNoInvalidVar,
    kStrFuncStmtNoMissingQuoteVar};
const std::vector<StringToStringFunc> kStrFuncsNegativeStmts = {
    kStrFuncStmtNoDecl,
    kStrFuncProcDecl,
    kStrFuncWildVar,
    kStrFuncWildWild,
    kStrFuncWildDecl,
    kStrFuncZeroStmtNoVar,
    kStrFuncNegativeStmtNoVar,
    kStrFuncInvalidStmtNoVar,
    kStrFuncDelDel,
    kStrFuncInvalidDeclVar,
    kStrFuncStmtNoInvalidVar,
    kStrFuncStmtNoMissingQuoteVar};
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
    kStrFuncDelDel,
    kStrFuncInvalidDeclVar,
    kStrFuncStmtNoInvalidVar,
    kStrFuncStmtNoMissingQuoteVar};

const std::vector<ClausesPair> kGetClausesPositive(
    Declaration::Type declaration_type) {
  switch (declaration_type) {
    case Declaration::Type::kVariable:
      return kPairsPositiveVar;
    case Declaration::Type::kStmt:
    case Declaration::Type::kRead:
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
    case Declaration::Type::kRead:
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

}  // namespace modifies

}  // namespace data

}  // namespace pql

}  // namespace test
