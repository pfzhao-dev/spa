#include "CompareData.h"

#include "../DeclarationData.h"
#include "../QueryStringBuilder.h"
#include "../RefBuilder.h"
#include "catch.hpp"

namespace test {

namespace pql {

namespace data {

namespace compare {

using namespace spa::pql;

// make shared pointer to compare clause with entity ref for lhs and rhs based
// on specified arguments and argument type
template <typename T, typename V>
std::shared_ptr<Compare> MakeCompareEntityEntity(T t, V v) {
  EntityRef entity_ref_lhs = MakeEntityRef(t);
  EntityRef entity_ref_rhs = MakeEntityRef(v);

  Compare compare(std::make_shared<EntityRef>(entity_ref_lhs),
                  std::make_shared<EntityRef>(entity_ref_rhs));
  return std::make_shared<Compare>(compare);
}

// make shared pointer to compare clause with statement ref for lhs and entity
// ref for rhs based on specified arguments and argument type
template <typename T, typename V>
std::shared_ptr<Compare> MakeCompareStatementEntity(T t, V v) {
  StatementRef statement_ref_lhs = MakeStatementRef(t);
  EntityRef entity_ref_rhs = MakeEntityRef(v);

  Compare compare(std::make_shared<StatementRef>(statement_ref_lhs),
                  std::make_shared<EntityRef>(entity_ref_rhs));
  return std::make_shared<Compare>(compare);
}

// make shared pointer to compare clause with entity ref for lhs and statement
// ref for rhs based on specified arguments and argument type
template <typename T, typename V>
std::shared_ptr<Compare> MakeCompareEntityStatement(T t, V v) {
  EntityRef entity_ref_lhs = MakeEntityRef(t);
  StatementRef statement_ref_rhs = MakeStatementRef(v);

  Compare compare(std::make_shared<EntityRef>(entity_ref_lhs),
                  std::make_shared<StatementRef>(statement_ref_rhs));
  return std::make_shared<Compare>(compare);
}

const StringToStringFunc kStrFuncVarDeclProcName =
    [](std::string declaration_name) {
      // equivalent to MakeCompareString("\"x\"", declaration_name, "",
      // kDeclValTypeToString.at(Declaration::ValueType::kProcName);
      return " with \" x \" = " + declaration_name + " . procName ";
    };
const StringToStringFunc kStrFuncVarDeclVarName =
    [](std::string declaration_name) {
      // equivalent to MakeCompareString("\"x\"", declaration_name, "",
      // kDeclValTypeToString.at(Declaration::ValueType::kVarName);
      return " with \"x\"=" + declaration_name + ".varName ";
    };
const StringToStringFunc kStrFuncDeclConstValueStmtNo =
    [](std::string declaration_name) {
      return MakeCompareString(
          declaration_name, "12",
          kDeclValTypeToString.at(Declaration::ValueType::kValue));
    };
const StringToStringFunc kStrFuncStmtNoDeclStmtNum =
    [](std::string declaration_name) {
      return MakeCompareString(
          "11", declaration_name, kEmptyStr,
          kDeclValTypeToString.at(Declaration::ValueType::kStmtNum));
    };
const StringToStringFunc kStrFuncDeclDefaultDeclDefault =
    [](std::string declaration_name) {
      return MakeCompareString(
          declaration::kDefaultDeclStmt, declaration::kDefaultDeclProgLine,
          kDeclValTypeToString.at(Declaration::ValueType::kStmtNum));
    };

// 0 stmt number
const StringToStringFunc kStrFuncZeroStmtNoVar =
    [](std::string declaration_name) {
      return MakeCompareString("0", "\"x\"");
    };

// negative stmt number
const StringToStringFunc kStrFuncNegativeStmtNoVar =
    [](std::string declaration_name) {
      return MakeCompareString("-1", "\"x\"");
    };

// invalid stmt number
const StringToStringFunc kStrFuncInvalidStmtNoVar =
    [](std::string declaration_name) {
      return MakeCompareString("4.0", "\"x\"");
    };

// invalid ref
const StringToStringFunc kStrFuncInvalidRefVar =
    [](std::string declaration_name) {
      return MakeCompareString(kWildcardStr, "\"x\"");
    };

// invalid value
const StringToStringFunc kStrFuncInvalidValueVar =
    [](std::string declaration_name) {
      return " with " + declaration_name + ". =\"x\" ";
    };

// invalid declaration type
const StringToStringFunc kStrFuncDelVar = [](std::string declaration_name) {
  return MakeCompareString(declaration_name, declaration_name);
};

// declaration name selected not defined
const StringToStringFunc kStrFuncInvalidDeclVar =
    [](std::string declaration_name) {
      return MakeCompareString("\"x\"", "invalideclarationname");
    };

// no matching '"' for entity ref
const StringToStringFunc kStrFuncStmtNoMissingQuoteVar =
    [](std::string declaration_name) { return MakeCompareString("1", "\"x"); };

// no whitespace between alphanumeric tokens
// no space between with and lhs
const StringToStringFunc kStrFuncInvalidWhiteSpaceWith =
    [](std::string declaration_name) { return " with1 = 1 "; };

const DeclPtrToClausePtrVecFunc kClausesFuncVarDeclProcName =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  auto rhs_decl = Declaration(expected_declaration->GetType(),
                              Declaration::ValueType::kProcName,
                              expected_declaration->GetName());

  return {MakeCompareEntityEntity<std::string, std::shared_ptr<Declaration>>(
      "x", std::make_shared<Declaration>(rhs_decl))};
};
const DeclPtrToClausePtrVecFunc kClausesFuncVarDeclVarName =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  auto rhs_decl = Declaration(expected_declaration->GetType(),
                              Declaration::ValueType::kVarName,
                              expected_declaration->GetName());

  return {MakeCompareEntityEntity<std::string, std::shared_ptr<Declaration>>(
      "x", std::make_shared<Declaration>(rhs_decl))};
};
const DeclPtrToClausePtrVecFunc kClausesFuncDeclConstValueStmtNo =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  auto lhs_decl = Declaration(expected_declaration->GetType(),
                              Declaration::ValueType::kValue,
                              expected_declaration->GetName());

  return {MakeCompareEntityStatement<std::shared_ptr<Declaration>, uint32_t>(
      std::make_shared<Declaration>(lhs_decl), 12)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncStmtNoDeclStmtNum =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  auto rhs_decl = Declaration(expected_declaration->GetType(),
                              Declaration::ValueType::kStmtNum,
                              expected_declaration->GetName());

  return {MakeCompareStatementEntity<uint32_t, std::shared_ptr<Declaration>>(
      11, std::make_shared<Declaration>(rhs_decl))};
};
const DeclPtrToClausePtrVecFunc kClausesFuncDeclDefaultDeclDefault =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  auto lhs_decl =
      Declaration(Declaration::Type::kStmt, Declaration::ValueType::kStmtNum,
                  declaration::kDefaultDeclStmt);

  return {MakeCompareEntityEntity<std::shared_ptr<Declaration>,
                                  std::shared_ptr<Declaration>>(
      std::make_shared<Declaration>(lhs_decl),
      declaration::kDefaultDeclNameToDeclPtr.at(
          declaration::kDefaultDeclProgLine))};
};

// pairs of functions that construct query strings and expected clauses
const ClausesPair kPairVarDeclProcName(kStrFuncVarDeclProcName,
                                       kClausesFuncVarDeclProcName);
const ClausesPair kPairVarDeclVarName(kStrFuncVarDeclVarName,
                                      kClausesFuncVarDeclVarName);
const ClausesPair kPairDeclConstValueStmtNo(kStrFuncDeclConstValueStmtNo,
                                            kClausesFuncDeclConstValueStmtNo);
const ClausesPair kPairStmtNoDeclStmtNum(kStrFuncStmtNoDeclStmtNum,
                                         kClausesFuncStmtNoDeclStmtNum);
const ClausesPair kPairDeclDefaultDeclDefault(
    kStrFuncDeclDefaultDeclDefault, kClausesFuncDeclDefaultDeclDefault);

// clauses pairs for positive compare test cases
const std::vector<ClausesPair> kPairsPositiveProc = {
    kPairVarDeclProcName, kPairDeclDefaultDeclDefault};
const std::vector<ClausesPair> kPairsPositiveCall = {
    kPairVarDeclProcName, kPairStmtNoDeclStmtNum, kPairDeclDefaultDeclDefault};
const std::vector<ClausesPair> kPairsPositiveVar = {
    kPairVarDeclVarName, kPairDeclDefaultDeclDefault};
const std::vector<ClausesPair> kPairsPositiveReadPrint = {
    kPairVarDeclVarName, kPairStmtNoDeclStmtNum, kPairDeclDefaultDeclDefault};
const std::vector<ClausesPair> kPairsPositiveConst = {
    kPairDeclConstValueStmtNo, kPairDeclDefaultDeclDefault};
const std::vector<ClausesPair> kPairsPositiveStmtProgLine = {
    kPairDeclDefaultDeclDefault};
const std::vector<ClausesPair> kPairsPositiveStatements = {
    kPairStmtNoDeclStmtNum, kPairDeclDefaultDeclDefault};

// functions for negative compare test cases
const std::vector<StringToStringFunc> kStrFuncsNegativeProc = {
    kStrFuncVarDeclVarName,
    kStrFuncDeclConstValueStmtNo,
    kStrFuncZeroStmtNoVar,
    kStrFuncNegativeStmtNoVar,
    kStrFuncInvalidStmtNoVar,
    kStrFuncInvalidRefVar,
    kStrFuncInvalidValueVar,
    kStrFuncDelVar,
    kStrFuncInvalidDeclVar,
    kStrFuncStmtNoMissingQuoteVar,
    kStrFuncInvalidWhiteSpaceWith};
const std::vector<StringToStringFunc> kStrFuncsNegativeCall = {
    kStrFuncVarDeclVarName,       kStrFuncDeclConstValueStmtNo,
    kStrFuncZeroStmtNoVar,        kStrFuncNegativeStmtNoVar,
    kStrFuncInvalidStmtNoVar,     kStrFuncInvalidRefVar,
    kStrFuncInvalidValueVar,      kStrFuncDelVar,
    kStrFuncInvalidDeclVar,       kStrFuncStmtNoMissingQuoteVar,
    kStrFuncInvalidWhiteSpaceWith};
const std::vector<StringToStringFunc> kStrFuncsNegativeVar = {
    kStrFuncVarDeclProcName,
    kStrFuncDeclConstValueStmtNo,
    kStrFuncZeroStmtNoVar,
    kStrFuncNegativeStmtNoVar,
    kStrFuncInvalidStmtNoVar,
    kStrFuncInvalidRefVar,
    kStrFuncInvalidValueVar,
    kStrFuncDelVar,
    kStrFuncInvalidDeclVar,
    kStrFuncStmtNoMissingQuoteVar,
    kStrFuncInvalidWhiteSpaceWith};
const std::vector<StringToStringFunc> kStrFuncsNegativeReadPrint = {
    kStrFuncVarDeclProcName,      kStrFuncDeclConstValueStmtNo,
    kStrFuncZeroStmtNoVar,        kStrFuncNegativeStmtNoVar,
    kStrFuncInvalidStmtNoVar,     kStrFuncInvalidRefVar,
    kStrFuncInvalidValueVar,      kStrFuncDelVar,
    kStrFuncInvalidDeclVar,       kStrFuncStmtNoMissingQuoteVar,
    kStrFuncInvalidWhiteSpaceWith};
const std::vector<StringToStringFunc> kStrFuncsNegativeConst = {
    kStrFuncVarDeclProcName,
    kStrFuncVarDeclVarName,
    kStrFuncZeroStmtNoVar,
    kStrFuncNegativeStmtNoVar,
    kStrFuncInvalidStmtNoVar,
    kStrFuncInvalidRefVar,
    kStrFuncInvalidValueVar,
    kStrFuncDelVar,
    kStrFuncInvalidDeclVar,
    kStrFuncStmtNoMissingQuoteVar,
    kStrFuncInvalidWhiteSpaceWith};
const std::vector<StringToStringFunc> kStrFuncsNegativeStmtProgLine = {
    kStrFuncVarDeclProcName,
    kStrFuncVarDeclVarName,
    kStrFuncDeclConstValueStmtNo,
    kStrFuncZeroStmtNoVar,
    kStrFuncNegativeStmtNoVar,
    kStrFuncInvalidStmtNoVar,
    kStrFuncInvalidRefVar,
    kStrFuncInvalidValueVar,
    kStrFuncDelVar,
    kStrFuncInvalidDeclVar,
    kStrFuncStmtNoMissingQuoteVar,
    kStrFuncInvalidWhiteSpaceWith};
const std::vector<StringToStringFunc> kStrFuncsNegativeStatements = {
    kStrFuncVarDeclProcName,
    kStrFuncVarDeclVarName,
    kStrFuncDeclConstValueStmtNo,
    kStrFuncZeroStmtNoVar,
    kStrFuncNegativeStmtNoVar,
    kStrFuncInvalidStmtNoVar,
    kStrFuncInvalidRefVar,
    kStrFuncInvalidValueVar,
    kStrFuncDelVar,
    kStrFuncInvalidDeclVar,
    kStrFuncStmtNoMissingQuoteVar,
    kStrFuncInvalidWhiteSpaceWith};

const std::vector<ClausesPair> kGetClausesPositive(
    Declaration::Type declaration_type) {
  switch (declaration_type) {
    case Declaration::Type::kProcedure:
      return kPairsPositiveProc;
    case Declaration::Type::kCall:
      return kPairsPositiveCall;
    case Declaration::Type::kVariable:
      return kPairsPositiveVar;
    case Declaration::Type::kRead:
    case Declaration::Type::kPrint:
      return kPairsPositiveReadPrint;
    case Declaration::Type::kConstant:
      return kPairsPositiveConst;
    case Declaration::Type::kStmt:
      return kPairsPositiveStmtProgLine;
    case Declaration::Type::kWhile:
    case Declaration::Type::kIf:
    case Declaration::Type::kAssign:
      return kPairsPositiveStatements;
    default:
      return {};
  }
}

const std::vector<StringToStringFunc> kGetFuncsNegative(
    Declaration::Type declaration_type) {
  switch (declaration_type) {
    case Declaration::Type::kProcedure:
      return kStrFuncsNegativeProc;
    case Declaration::Type::kCall:
      return kStrFuncsNegativeCall;
    case Declaration::Type::kVariable:
      return kStrFuncsNegativeVar;
    case Declaration::Type::kRead:
    case Declaration::Type::kPrint:
      return kStrFuncsNegativeReadPrint;
    case Declaration::Type::kConstant:
      return kStrFuncsNegativeConst;
    case Declaration::Type::kStmt:
      return kStrFuncsNegativeStmtProgLine;
    case Declaration::Type::kWhile:
    case Declaration::Type::kIf:
    case Declaration::Type::kAssign:
      return kStrFuncsNegativeStatements;
    default:
      return {};
  }
}

}  // namespace compare

}  // namespace data

}  // namespace pql

}  // namespace test
