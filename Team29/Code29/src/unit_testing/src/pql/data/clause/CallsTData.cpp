#include "CallsTData.h"

#include "../DeclarationData.h"
#include "../QueryStringBuilder.h"
#include "../RefBuilder.h"
#include "catch.hpp"

namespace test {

namespace pql {

namespace data {

namespace callst {

using namespace spa::pql;

// make shared pointer to calls* based on specified arguments and argument type
// pass in type of uint32_t and argument 0 for wildcard
template <typename T, typename V>
std::shared_ptr<Calls> MakeCalls(T t, V v) {
  EntityRef entity_ref_caller = MakeEntityRef(t);
  EntityRef entity_ref_callee = MakeEntityRef(v);

  Calls calls(std::make_shared<EntityRef>(entity_ref_caller),
              std::make_shared<EntityRef>(entity_ref_callee), true);
  return std::make_shared<Calls>(calls);
}

const StringToStringFunc kStrFuncVarDecl = [](std::string declaration_name) {
  return MakeRelationString(kCallsT, "\"x\"", declaration_name);
};
const StringToStringFunc kStrFuncVarVar = [](std::string declaration_name) {
  return MakeRelationString(kCallsT, "\"x\"", "\"y\"");
};
const StringToStringFunc kStrFuncDeclVar = [](std::string declaration_name) {
  return MakeRelationString(kCallsT, declaration_name, "\"z\"");
};
const StringToStringFunc kStrFuncDeclDecl = [](std::string declaration_name) {
  return MakeRelationString(kCallsT, declaration_name, declaration_name);
};
const StringToStringFunc kStrFuncDefaultDeclDecl =
    [](std::string declaration_name) {
      return MakeRelationString(kCallsT, declaration::kDefaultDeclProcedure,
                                declaration_name);
    };
const StringToStringFunc kStrFuncWildVar = [](std::string declaration_name) {
  return MakeRelationString(kCallsT, kWildcardStr, "\"z\"");
};
const StringToStringFunc kStrFuncWildWild = [](std::string declaration_name) {
  return MakeRelationString(kCallsT, kWildcardStr, kWildcardStr);
};
const StringToStringFunc kStrFuncVarWild = [](std::string declaration_name) {
  return MakeRelationString(kCallsT, "\"y\"", kWildcardStr);
};

// declaration name selected not defined
const StringToStringFunc kStrFuncInvalidNameVar =
    [](std::string declaration_name) {
      return MakeRelationString(kCallsT, "invaliddeclarationname", "\"y\"");
    };

// no matching '"' for entity ref
const StringToStringFunc kStrFuncVarMissingQuoteVar =
    [](std::string declaration_name) {
      return MakeRelationString(kCallsT, "\"y\"", "\"x");
    };

// whitespace between Calls and *
const StringToStringFunc kStrFuncInvalidWhiteSpaceAfterCalls =
    [](std::string declaration_name) {
      return "Select " + declaration_name + " such that Calls *( \"x\" , _ )";
    };

const DeclPtrToClausePtrVecFunc kClausesFuncVarDecl =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeCalls<std::string, std::shared_ptr<Declaration>>(
      "x", expected_declaration)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncVarVar =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeCalls<std::string, std::string>("x", "y")};
};
const DeclPtrToClausePtrVecFunc kClausesFuncDeclVar =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeCalls<std::shared_ptr<Declaration>, std::string>(
      expected_declaration, "z")};
};
const DeclPtrToClausePtrVecFunc kClausesFuncDefaultDeclDecl =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeCalls<std::shared_ptr<Declaration>, std::shared_ptr<Declaration>>(
      declaration::kDefaultDeclNameToDeclPtr.at(
          declaration::kDefaultDeclProcedure),
      expected_declaration)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncDeclDecl =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeCalls<std::shared_ptr<Declaration>, std::shared_ptr<Declaration>>(
      expected_declaration, expected_declaration)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncWildVar =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeCalls<std::string, std::string>("", "z")};
};
const DeclPtrToClausePtrVecFunc kClausesFuncWildWild =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeCalls<std::string, std::string>("", "")};
};
const DeclPtrToClausePtrVecFunc kClausesFuncVarWild =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeCalls<std::string, std::string>("y", "")};
};

// pairs of functions that construct query strings and expected clauses
const ClausesPair kPairVarDecl(kStrFuncVarDecl, kClausesFuncVarDecl);
const ClausesPair kPairVarVar(kStrFuncVarVar, kClausesFuncVarVar);
const ClausesPair kPairDeclVar(kStrFuncDeclVar, kClausesFuncDeclVar);
const ClausesPair kPairDeclDecl(kStrFuncDeclDecl, kClausesFuncDeclDecl);
const ClausesPair kPairDefaultDeclDecl(kStrFuncDefaultDeclDecl,
                                       kClausesFuncDefaultDeclDecl);
const ClausesPair kPairWildVar(kStrFuncWildVar, kClausesFuncWildVar);
const ClausesPair kPairWildWild(kStrFuncWildWild, kClausesFuncWildWild);
const ClausesPair kPairVarWild(kStrFuncVarWild, kClausesFuncVarWild);

// clauses pairs for positive calls* test cases
const std::vector<ClausesPair> kPairsPositiveProc = {
    kPairVarDecl,         kPairVarVar,  kPairDeclVar,  kPairDeclDecl,
    kPairDefaultDeclDecl, kPairWildVar, kPairWildWild, kPairVarWild};
const std::vector<ClausesPair> kPairsPositive = {kPairVarVar, kPairWildVar,
                                                 kPairWildWild, kPairVarWild};

// functions for negative calls* test cases
const std::vector<StringToStringFunc> kStrFuncsNegativeProc = {
    kStrFuncInvalidNameVar, kStrFuncVarMissingQuoteVar,
    kStrFuncInvalidWhiteSpaceAfterCalls};
const std::vector<StringToStringFunc> kStrFuncsNegative = {
    kStrFuncVarDecl,
    kStrFuncDeclDecl,
    kStrFuncDefaultDeclDecl,
    kStrFuncDeclVar,
    kStrFuncInvalidNameVar,
    kStrFuncVarMissingQuoteVar,
    kStrFuncInvalidWhiteSpaceAfterCalls};

const std::vector<ClausesPair> kGetClausesPositive(
    Declaration::Type declaration_type) {
  switch (declaration_type) {
    case Declaration::Type::kProcedure:
      return kPairsPositiveProc;
    default:
      return kPairsPositive;
  }
}

const std::vector<StringToStringFunc> kGetFuncsNegative(
    Declaration::Type declaration_type) {
  switch (declaration_type) {
    case Declaration::Type::kProcedure:
      return kStrFuncsNegativeProc;
    default:
      return kStrFuncsNegative;
  }
}

}  // namespace callst

}  // namespace data

}  // namespace pql

}  // namespace test
