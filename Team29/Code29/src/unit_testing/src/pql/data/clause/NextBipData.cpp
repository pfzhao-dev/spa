#include "NextBipData.h"

#include "../DeclarationData.h"
#include "../QueryStringBuilder.h"
#include "../RefBuilder.h"
#include "catch.hpp"

namespace test {

namespace pql {

namespace data {

namespace nextbip {

using namespace spa::pql;

// make shared pointer to nextbip based on specified arguments and argument type
// pass in type of uint32_t and argument 0 for wildcard
template <typename T, typename V>
std::shared_ptr<Next> MakeNextBip(T t, V v) {
  StatementRef statement_ref_pred = MakeStatementRef(t);
  StatementRef statement_ref_succ = MakeStatementRef(v);

  Next nextbip(std::make_shared<StatementRef>(statement_ref_pred),
               std::make_shared<StatementRef>(statement_ref_succ), false, true);
  return std::make_shared<Next>(nextbip);
}

const StringToStringFunc kStrFuncStmtNoDecl = [](std::string declaration_name) {
  return MakeRelationString(kNextBip, "3", declaration_name);
};
const StringToStringFunc kStrFuncStmtNoStmtNo =
    [](std::string declaration_name) {
      return MakeRelationString(kNextBip, "3", "4");
    };
const StringToStringFunc kStrFuncDeclStmtNo = [](std::string declaration_name) {
  return MakeRelationString(kNextBip, declaration_name, "5");
};
const StringToStringFunc kStrFuncDeclDecl = [](std::string declaration_name) {
  return MakeRelationString(kNextBip, declaration_name, declaration_name);
};
const StringToStringFunc kStrFuncDefaultDeclDecl =
    [](std::string declaration_name) {
      return MakeRelationString(kNextBip, declaration::kDefaultDeclIf,
                                declaration_name);
    };
const StringToStringFunc kStrFuncWildStmtNo = [](std::string declaration_name) {
  return MakeRelationString(kNextBip, kWildcardStr, "5");
};
const StringToStringFunc kStrFuncWildWild = [](std::string declaration_name) {
  return MakeRelationString(kNextBip, kWildcardStr, kWildcardStr);
};
const StringToStringFunc kStrFuncStmtNoWild = [](std::string declaration_name) {
  return MakeRelationString(kNextBip, "4", kWildcardStr);
};

// 0 stmt number
const StringToStringFunc kStrFuncZeroStmtNoStmtNo =
    [](std::string declaration_name) {
      return MakeRelationString(kNextBip, "0", "2");
    };

// negative stmt number
const StringToStringFunc kStrFuncNegativeStmtNoStmtNo =
    [](std::string declaration_name) {
      return MakeRelationString(kNextBip, "-1", "4");
    };

// invalid stmt number
const StringToStringFunc kStrFuncInvalidStmtNoStmtNo =
    [](std::string declaration_name) {
      return MakeRelationString(kNextBip, "4.0", "4");
    };

// declaration name selected not defined
const StringToStringFunc kStrFuncInvalidNameStmtNo =
    [](std::string declaration_name) {
      return MakeRelationString(kNextBip, "invaliddeclarationname", "4");
    };

const DeclPtrToClausePtrVecFunc kClausesFuncStmtNoDecl =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeNextBip<uint32_t, std::shared_ptr<Declaration>>(
      3, expected_declaration)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncStmtNoStmtNo =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeNextBip<uint32_t, uint32_t>(3, 4)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncDeclStmtNo =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeNextBip<std::shared_ptr<Declaration>, uint32_t>(
      expected_declaration, 5)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncDefaultDeclDecl =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {
      MakeNextBip<std::shared_ptr<Declaration>, std::shared_ptr<Declaration>>(
          declaration::kDefaultDeclNameToDeclPtr.at(
              declaration::kDefaultDeclIf),
          expected_declaration)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncDeclDecl =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {
      MakeNextBip<std::shared_ptr<Declaration>, std::shared_ptr<Declaration>>(
          expected_declaration, expected_declaration)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncWildStmtNo =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeNextBip<uint32_t, uint32_t>(0, 5)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncWildWild =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeNextBip<uint32_t, uint32_t>(0, 0)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncStmtNoWild =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeNextBip<uint32_t, uint32_t>(4, 0)};
};

// pairs of functions that construct query strings and expected clauses
const ClausesPair kPairStmtNoDecl(kStrFuncStmtNoDecl, kClausesFuncStmtNoDecl);
const ClausesPair kPairStmtNoStmtNo(kStrFuncStmtNoStmtNo,
                                    kClausesFuncStmtNoStmtNo);
const ClausesPair kPairDeclStmtNo(kStrFuncDeclStmtNo, kClausesFuncDeclStmtNo);
const ClausesPair kPairDeclDecl(kStrFuncDeclDecl, kClausesFuncDeclDecl);
const ClausesPair kPairDefaultDeclDecl(kStrFuncDefaultDeclDecl,
                                       kClausesFuncDefaultDeclDecl);
const ClausesPair kPairWildStmtNo(kStrFuncWildStmtNo, kClausesFuncWildStmtNo);
const ClausesPair kPairWildWild(kStrFuncWildWild, kClausesFuncWildWild);
const ClausesPair kPairStmtNoWild(kStrFuncStmtNoWild, kClausesFuncStmtNoWild);

// clauses pairs for positive nextbip test cases
const std::vector<ClausesPair> kPairsPositiveStmts = {
    kPairStmtNoDecl,      kPairStmtNoStmtNo, kPairDeclStmtNo, kPairDeclDecl,
    kPairDefaultDeclDecl, kPairWildStmtNo,   kPairWildWild,   kPairStmtNoWild};
const std::vector<ClausesPair> kPairsPositive = {
    kPairStmtNoStmtNo, kPairWildStmtNo, kPairWildWild, kPairStmtNoWild};

// functions for negative nextbip test cases
const std::vector<StringToStringFunc> kStrFuncsNegativeStmts = {
    kStrFuncZeroStmtNoStmtNo, kStrFuncNegativeStmtNoStmtNo,
    kStrFuncInvalidStmtNoStmtNo, kStrFuncInvalidNameStmtNo};
const std::vector<StringToStringFunc> kStrFuncsNegative = {
    kStrFuncStmtNoDecl,          kStrFuncDeclDecl,
    kStrFuncDefaultDeclDecl,     kStrFuncDeclStmtNo,
    kStrFuncZeroStmtNoStmtNo,    kStrFuncNegativeStmtNoStmtNo,
    kStrFuncInvalidStmtNoStmtNo, kStrFuncInvalidNameStmtNo};

const std::vector<ClausesPair> kGetClausesPositive(
    Declaration::Type declaration_type) {
  switch (declaration_type) {
    case Declaration::Type::kStmt:
    case Declaration::Type::kRead:
    case Declaration::Type::kPrint:
    case Declaration::Type::kCall:
    case Declaration::Type::kWhile:
    case Declaration::Type::kIf:
    case Declaration::Type::kAssign:
      return kPairsPositiveStmts;
    default:
      return kPairsPositive;
  }
}

const std::vector<StringToStringFunc> kGetFuncsNegative(
    Declaration::Type declaration_type) {
  switch (declaration_type) {
    case Declaration::Type::kStmt:
    case Declaration::Type::kRead:
    case Declaration::Type::kPrint:
    case Declaration::Type::kCall:
    case Declaration::Type::kWhile:
    case Declaration::Type::kIf:
    case Declaration::Type::kAssign:
      return kStrFuncsNegativeStmts;
    default:
      return kStrFuncsNegative;
  }
}

}  // namespace nextbip

}  // namespace data

}  // namespace pql

}  // namespace test
