#include "AffectsTData.h"

#include "../DeclarationData.h"
#include "../QueryStringBuilder.h"
#include "../RefBuilder.h"
#include "catch.hpp"

namespace test {

namespace pql {

namespace data {

namespace affectst {

using namespace spa::pql;

// make shared pointer to affects* based on specified arguments and argument
// type
// pass in type of uint32_t and argument 0 for wildcard
template <typename T, typename V>
std::shared_ptr<Affects> MakeAffectsT(T t, V v) {
  StatementRef statement_ref_affector = MakeStatementRef(t);
  StatementRef statement_ref_affected = MakeStatementRef(v);

  Affects affectst(std::make_shared<StatementRef>(statement_ref_affector),
                   std::make_shared<StatementRef>(statement_ref_affected),
                   true);
  return std::make_shared<Affects>(affectst);
}

const StringToStringFunc kStrFuncStmtNoDecl = [](std::string declaration_name) {
  return MakeRelationString(kAffectsT, "3", declaration_name);
};
const StringToStringFunc kStrFuncStmtNoStmtNo =
    [](std::string declaration_name) {
      return MakeRelationString(kAffectsT, "3", "4");
    };
const StringToStringFunc kStrFuncDeclStmtNo = [](std::string declaration_name) {
  return MakeRelationString(kAffectsT, declaration_name, "5");
};
const StringToStringFunc kStrFuncDefaultDeclDecl =
    [](std::string declaration_name) {
      return MakeRelationString(kAffectsT, declaration::kDefaultDeclStmt,
                                declaration_name);
    };
const StringToStringFunc kStrFuncDeclDecl = [](std::string declaration_name) {
  return MakeRelationString(kAffectsT, declaration_name, declaration_name);
};
const StringToStringFunc kStrFuncWildStmtNo = [](std::string declaration_name) {
  return MakeRelationString(kAffectsT, kWildcardStr, "5");
};
const StringToStringFunc kStrFuncWildWild = [](std::string declaration_name) {
  return MakeRelationString(kAffectsT, kWildcardStr, kWildcardStr);
};
const StringToStringFunc kStrFuncStmtNoWild = [](std::string declaration_name) {
  return MakeRelationString(kAffectsT, "4", kWildcardStr);
};

// 0 stmt number
const StringToStringFunc kStrFuncZeroStmtNoStmtNo =
    [](std::string declaration_name) {
      return MakeRelationString(kAffectsT, "0", "2");
    };

// negative stmt number
const StringToStringFunc kStrFuncNegativeStmtNoStmtNo =
    [](std::string declaration_name) {
      return MakeRelationString(kAffectsT, "-1", "4");
    };

// invalid stmt number
const StringToStringFunc kStrFuncInvalidStmtNoStmtNo =
    [](std::string declaration_name) {
      return MakeRelationString(kAffectsT, "4.0", "4");
    };

// declaration name selected not defined
const StringToStringFunc kStrFuncInvalidNameStmtNo =
    [](std::string declaration_name) {
      return MakeRelationString(kAffectsT, "invaliddeclarationname", "4");
    };

// whitespace between Affects and *
const StringToStringFunc kStrFuncInvalidWhiteSpaceAfterAffects =
    [](std::string declaration_name) {
      return "Select " + declaration_name + " such that Affects *( 3 , 4 )";
    };

const DeclPtrToClausePtrVecFunc kClausesFuncStmtNoDecl =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeAffectsT<uint32_t, std::shared_ptr<Declaration>>(
      3, expected_declaration)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncStmtNoStmtNo =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeAffectsT<uint32_t, uint32_t>(3, 4)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncDeclStmtNo =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeAffectsT<std::shared_ptr<Declaration>, uint32_t>(
      expected_declaration, 5)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncDefaultDeclDecl =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {
      MakeAffectsT<std::shared_ptr<Declaration>, std::shared_ptr<Declaration>>(
          declaration::kDefaultDeclNameToDeclPtr.at(
              declaration::kDefaultDeclStmt),
          expected_declaration)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncDeclDecl =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {
      MakeAffectsT<std::shared_ptr<Declaration>, std::shared_ptr<Declaration>>(
          expected_declaration, expected_declaration)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncWildStmtNo =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeAffectsT<uint32_t, uint32_t>(0, 5)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncWildWild =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeAffectsT<uint32_t, uint32_t>(0, 0)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncStmtNoWild =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakeAffectsT<uint32_t, uint32_t>(4, 0)};
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

// clauses pairs for positive affects* test cases
const std::vector<ClausesPair> kPairsPositiveStmts = {
    kPairStmtNoDecl,      kPairStmtNoStmtNo, kPairDeclStmtNo, kPairDeclDecl,
    kPairDefaultDeclDecl, kPairWildStmtNo,   kPairWildWild,   kPairStmtNoWild};
const std::vector<ClausesPair> kPairsPositive = {
    kPairStmtNoStmtNo, kPairWildStmtNo, kPairWildWild, kPairStmtNoWild};

// functions for negative affects* test cases
const std::vector<StringToStringFunc> kStrFuncsNegativeStmts = {
    kStrFuncZeroStmtNoStmtNo, kStrFuncNegativeStmtNoStmtNo,
    kStrFuncInvalidStmtNoStmtNo, kStrFuncInvalidNameStmtNo,
    kStrFuncInvalidWhiteSpaceAfterAffects};
const std::vector<StringToStringFunc> kStrFuncsNegative = {
    kStrFuncStmtNoDecl,
    kStrFuncDeclDecl,
    kStrFuncDefaultDeclDecl,
    kStrFuncDeclStmtNo,
    kStrFuncZeroStmtNoStmtNo,
    kStrFuncNegativeStmtNoStmtNo,
    kStrFuncInvalidStmtNoStmtNo,
    kStrFuncInvalidNameStmtNo,
    kStrFuncInvalidWhiteSpaceAfterAffects};

const std::vector<ClausesPair> kGetClausesPositive(
    Declaration::Type declaration_type) {
  switch (declaration_type) {
    case Declaration::Type::kStmt:
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
    case Declaration::Type::kAssign:
      return kStrFuncsNegativeStmts;
    default:
      return kStrFuncsNegative;
  }
}

}  // namespace affectst

}  // namespace data

}  // namespace pql

}  // namespace test
