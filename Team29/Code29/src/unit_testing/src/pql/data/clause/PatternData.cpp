#include "PatternData.h"

#include "../DeclarationData.h"
#include "../QueryStringBuilder.h"
#include "../RefBuilder.h"
#include "catch.hpp"

namespace test {

namespace pql {

namespace data {

namespace pattern {

using namespace spa::pql;

// make shared pointer to pattern based on specified arguments and argument type
// pass in argument "" for entity wildcard
template <typename T>
std::shared_ptr<Pattern> MakePattern(std::shared_ptr<Declaration> type, T var,
                                     bool is_wild, bool is_partial,
                                     std::string spec) {
  StatementRef stmt_ref_type = MakeStatementRef(type);
  EntityRef entity_ref_var = MakeEntityRef(var);

  Pattern::Expression expression;
  if (!is_wild && stmt_ref_type.IsDeclTypeIn({Declaration::Type::kAssign})) {
    expression = Pattern::Expression(spec, is_partial);
  }

  Pattern pattern(std::make_shared<StatementRef>(stmt_ref_type),
                  std::make_shared<EntityRef>(entity_ref_var), expression);
  return std::make_shared<Pattern>(pattern);
}

// expressions used
const std::string kExprWild = kWildcardStr;
const std::string kExprPartial = "_\"pattern/Follows*procedure\"_";
const std::string kExprPartialExpr = "pattern/Follows*procedure";
const std::string kExprFull = "\"Select + fsdf / 2 * (3 % 2) - 0\"";
const std::string kExprFullExpr = "Select + fsdf / 2 * (3 % 2) - 0";

const StringToStringFunc kStrFuncTypeDeclVarNameExprWild =
    [](std::string declaration_name) {
      // equivalent to MakePatternString(declaration_name, "\"x\"", kExprWild)
      return "pattern " + declaration_name + "(\"x\",_)";
    };
const StringToStringFunc kStrFuncTypeDeclVarNameExprPartial =
    [](std::string declaration_name) {
      // equivalent to MakePatternString(declaration_name, "\"x\"",
      // kExprPartial)
      return "pattern  	 	" + declaration_name +
             " ( \" x \" ,  _ \" pattern/Follows*procedure \"  _	)";
    };
const StringToStringFunc kStrFuncTypeDeclVarNameExprFull =
    [](std::string declaration_name) {
      // equivalent to MakePatternString(declaration_name, "\"x\"", kExprFull)
      return "pattern  	 	" + declaration_name +
             " ( \" x \" ,  \"  Select + fsdf / 2 * (3 % 2) - 0  \"	)";
    };
const StringToStringFunc kStrFuncTypeDeclVarNameExprWildExprWild =
    [](std::string declaration_name) {
      // equivalent to MakePatternString(declaration_name, "\"x\"", kExprWild,
      // kExprWild)
      return "pattern  	 	" + declaration_name +
             " ( \" x \" ,  _ , _ 	)";
    };
const StringToStringFunc kStrFuncTypeDeclVarWildExprWild =
    [](std::string declaration_name) {
      return MakePatternString(declaration_name, kWildcardStr, kExprWild);
    };
const StringToStringFunc kStrFuncTypeDeclVarWildExprWildExprWild =
    [](std::string declaration_name) {
      return MakePatternString(declaration_name, kWildcardStr, kExprWild,
                               kExprWild);
    };
const StringToStringFunc kStrFuncTypeDeclVarWildExprPartial =
    [](std::string declaration_name) {
      return MakePatternString(declaration_name, kWildcardStr, kExprPartial);
    };
const StringToStringFunc kStrFuncTypeDeclVarWildExprFull =
    [](std::string declaration_name) {
      return MakePatternString(declaration_name, kWildcardStr, kExprFull);
    };
const StringToStringFunc kStrFuncTypeDeclVarDeclExprWild =
    [](std::string declaration_name) {
      return MakePatternString(declaration::kDefaultDeclWhile, declaration_name,
                               kExprWild);
    };
const StringToStringFunc kStrFuncTypeDeclVarDeclExprPartial =
    [](std::string declaration_name) {
      return MakePatternString(declaration::kDefaultDeclAssign,
                               declaration_name, kExprPartial);
    };
const StringToStringFunc kStrFuncTypeDeclVarDeclExprFull =
    [](std::string declaration_name) {
      return MakePatternString(declaration::kDefaultDeclAssign,
                               declaration_name, kExprFull);
    };
const StringToStringFunc kStrFuncTypeDeclVarDeclExprWildExprWild =
    [](std::string declaration_name) {
      return MakePatternString(declaration::kDefaultDeclIf, declaration_name,
                               kExprWild, kExprWild);
    };

// no whitespace between pattern and type
const StringToStringFunc kStrFuncInvalidWhiteSpaceAfterPattern =
    [](std::string declaration_name) { return "patterna (_, _) "; };

// wrong declaration type for type and/or var
const StringToStringFunc kStrFuncInvalidTypeDeclInvalidVarDeclExprWild =
    [](std::string declaration_name) {
      return MakePatternString(declaration_name, declaration_name, kExprWild);
    };

// declaration name for type is not defined
const StringToStringFunc kStrFuncInvalidTypeDeclVarNameExprWild =
    [](std::string declaration_name) {
      return MakePatternString("invaliddeclarationname", "\"x\"", kExprWild);
    };

// declaration name for var is not defined
const StringToStringFunc kStrFuncTypeDeclInvalidVarDeclExprWild =
    [](std::string declaration_name) {
      return MakePatternString(declaration::kDefaultDeclAssign,
                               "invaliddeclarationname", kExprWild);
    };

// no matching '"' for var entity ref
const StringToStringFunc kStrFuncTypeDeclMissingQuoteVarNameExprWild =
    [](std::string declaration_name) {
      return MakePatternString(declaration::kDefaultDeclAssign, "\"x",
                               kExprWild);
    };

// invalid expr
const StringToStringFunc kStrFuncTypeDeclVarNameInvaidExpr =
    [](std::string declaration_name) {
      return MakePatternString(declaration::kDefaultDeclAssign, "\"x\"",
                               "\"1~[2]\"");
    };

// no matching '"' for expr
const StringToStringFunc kStrFuncTypeDeclVarNameMissingQuoteExpr =
    [](std::string declaration_name) {
      return MakePatternString(declaration::kDefaultDeclAssign, "\"x\"",
                               "1 + 2\"");
    };

// no matching '_' for expr
const StringToStringFunc kStrFuncTypeDeclVarNameMissingUnderWild =
    [](std::string declaration_name) {
      return MakePatternString(declaration::kDefaultDeclAssign, "\"x\"",
                               "_\"1 + 2\"");
    };

// invalid 3rd argument
const StringToStringFunc kStrFuncTypeDeclVarNameInvaidExpr2 =
    [](std::string declaration_name) {
      return MakePatternString(declaration::kDefaultDeclIf, "\"x\"",
                               kWildcardStr, "\"x\"");
    };

const DeclPtrToClausePtrVecFunc kClausesFuncTypeDeclVarNameExprWild =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakePattern<std::string>(expected_declaration, "x", true, false,
                                   kExprWild)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncTypeDeclVarNameExprPartial =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakePattern<std::string>(expected_declaration, "x", false, true,
                                   kExprPartialExpr)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncTypeDeclVarNameExprFull =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakePattern<std::string>(expected_declaration, "x", false, false,
                                   kExprFullExpr)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncTypeDeclVarNameExprWildExprWild =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakePattern<std::string>(expected_declaration, "x", true, false,
                                   kExprWild)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncTypeDeclVarWildExprWild =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakePattern<std::string>(expected_declaration, "", true, false,
                                   kExprWild)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncTypeDeclVarWildExprWildExprWild =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakePattern<std::string>(expected_declaration, "", true, false,
                                   kExprWild)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncTypeDeclVarWildExprPartial =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakePattern<std::string>(expected_declaration, "", false, true,
                                   kExprPartialExpr)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncTypeDeclVarWildExprFull =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakePattern<std::string>(expected_declaration, "", false, false,
                                   kExprFullExpr)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncTypeDeclVarDeclExprWild =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakePattern<std::shared_ptr<Declaration>>(
      declaration::kDefaultDeclNameToDeclPtr.at(declaration::kDefaultDeclWhile),
      expected_declaration, true, false, kExprWild)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncTypeDeclVarDeclExprPartial =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakePattern<std::shared_ptr<Declaration>>(
      declaration::kDefaultDeclNameToDeclPtr.at(
          declaration::kDefaultDeclAssign),
      expected_declaration, false, true, kExprPartialExpr)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncTypeDeclVarDeclExprFull =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakePattern<std::shared_ptr<Declaration>>(
      declaration::kDefaultDeclNameToDeclPtr.at(
          declaration::kDefaultDeclAssign),
      expected_declaration, false, false, kExprFullExpr)};
};
const DeclPtrToClausePtrVecFunc kClausesFuncTypeDeclVarDeclExprWildExprWild =
    [](std::shared_ptr<Declaration> expected_declaration)
    -> std::vector<std::shared_ptr<Clause>> {
  return {MakePattern<std::shared_ptr<Declaration>>(
      declaration::kDefaultDeclNameToDeclPtr.at(declaration::kDefaultDeclIf),
      expected_declaration, false, false, kExprWild)};
};

// pairs of functions that construct query strings and expected clauses
const ClausesPair kPairTypeDeclVarNameExprWild(
    kStrFuncTypeDeclVarNameExprWild, kClausesFuncTypeDeclVarNameExprWild);
const ClausesPair kPairTypeDeclVarNameExprPartial(
    kStrFuncTypeDeclVarNameExprPartial, kClausesFuncTypeDeclVarNameExprPartial);
const ClausesPair kPairTypeDeclVarNameExprFull(
    kStrFuncTypeDeclVarNameExprFull, kClausesFuncTypeDeclVarNameExprFull);
const ClausesPair kPairTypeDeclVarNameExprWildExprWild(
    kStrFuncTypeDeclVarNameExprWildExprWild,
    kClausesFuncTypeDeclVarNameExprWildExprWild);
const ClausesPair kPairTypeDeclVarWildExprWild(
    kStrFuncTypeDeclVarWildExprWild, kClausesFuncTypeDeclVarWildExprWild);
const ClausesPair kPairTypeDeclVarWildExprWildExprWild(
    kStrFuncTypeDeclVarWildExprWildExprWild,
    kClausesFuncTypeDeclVarWildExprWildExprWild);
const ClausesPair kPairTypeDeclVarWildExprPartial(
    kStrFuncTypeDeclVarWildExprPartial, kClausesFuncTypeDeclVarWildExprPartial);
const ClausesPair kPairTypeDeclVarWildExprFull(
    kStrFuncTypeDeclVarWildExprFull, kClausesFuncTypeDeclVarWildExprFull);
const ClausesPair kPairTypeDeclVarDeclExprWild(
    kStrFuncTypeDeclVarDeclExprWild, kClausesFuncTypeDeclVarDeclExprWild);
const ClausesPair kPairTypeDeclVarDeclExprPartial(
    kStrFuncTypeDeclVarDeclExprPartial, kClausesFuncTypeDeclVarDeclExprPartial);
const ClausesPair kPairTypeDeclVarDeclExprFull(
    kStrFuncTypeDeclVarDeclExprFull, kClausesFuncTypeDeclVarDeclExprFull);
const ClausesPair kPairTypeDeclVarDeclExprWildExprWild(
    kStrFuncTypeDeclVarDeclExprWildExprWild,
    kClausesFuncTypeDeclVarDeclExprWildExprWild);

// clauses pairs for positive pattern test cases
const std::vector<ClausesPair> kPairsPositiveAssign = {
    kPairTypeDeclVarNameExprWild,    kPairTypeDeclVarNameExprPartial,
    kPairTypeDeclVarNameExprFull,    kPairTypeDeclVarWildExprWild,
    kPairTypeDeclVarWildExprPartial, kPairTypeDeclVarWildExprFull};
const std::vector<ClausesPair> kPairsPositiveWhile = {
    kPairTypeDeclVarNameExprWild, kPairTypeDeclVarWildExprWild};
const std::vector<ClausesPair> kPairsPositiveIf = {
    kPairTypeDeclVarNameExprWildExprWild, kPairTypeDeclVarWildExprWildExprWild};
const std::vector<ClausesPair> kPairsPositiveVar = {
    kPairTypeDeclVarDeclExprWild, kPairTypeDeclVarDeclExprPartial,
    kPairTypeDeclVarDeclExprFull, kPairTypeDeclVarDeclExprWildExprWild};
const std::vector<ClausesPair> kPairsPositive = {};

// functions for negative pattern test cases
const std::vector<StringToStringFunc> kStrFuncsNegativeAssign = {
    kStrFuncTypeDeclVarNameExprWildExprWild,
    kStrFuncTypeDeclVarWildExprWildExprWild,
    kStrFuncTypeDeclVarDeclExprWild,
    kStrFuncTypeDeclVarDeclExprPartial,
    kStrFuncTypeDeclVarDeclExprFull,
    kStrFuncInvalidWhiteSpaceAfterPattern,
    kStrFuncInvalidTypeDeclInvalidVarDeclExprWild,
    kStrFuncInvalidTypeDeclVarNameExprWild,
    kStrFuncTypeDeclInvalidVarDeclExprWild,
    kStrFuncTypeDeclMissingQuoteVarNameExprWild,
    kStrFuncTypeDeclVarNameInvaidExpr,
    kStrFuncTypeDeclVarNameMissingQuoteExpr,
    kStrFuncTypeDeclVarNameMissingUnderWild,
    kStrFuncTypeDeclVarNameInvaidExpr2};
const std::vector<StringToStringFunc> kStrFuncsNegativeWhile = {
    kStrFuncTypeDeclVarNameExprWildExprWild,
    kStrFuncTypeDeclVarWildExprWildExprWild,
    kStrFuncTypeDeclVarNameExprPartial,
    kStrFuncTypeDeclVarNameExprFull,
    kStrFuncTypeDeclVarWildExprPartial,
    kStrFuncTypeDeclVarWildExprFull,
    kStrFuncTypeDeclVarDeclExprWild,
    kStrFuncTypeDeclVarDeclExprPartial,
    kStrFuncTypeDeclVarDeclExprFull,
    kStrFuncInvalidWhiteSpaceAfterPattern,
    kStrFuncInvalidTypeDeclInvalidVarDeclExprWild,
    kStrFuncInvalidTypeDeclVarNameExprWild,
    kStrFuncTypeDeclInvalidVarDeclExprWild,
    kStrFuncTypeDeclMissingQuoteVarNameExprWild,
    kStrFuncTypeDeclVarNameInvaidExpr,
    kStrFuncTypeDeclVarNameMissingQuoteExpr,
    kStrFuncTypeDeclVarNameMissingUnderWild,
    kStrFuncTypeDeclVarNameInvaidExpr2};
const std::vector<StringToStringFunc> kStrFuncsNegativeIf = {
    kStrFuncTypeDeclVarNameExprWild,
    kStrFuncTypeDeclVarNameExprPartial,
    kStrFuncTypeDeclVarNameExprFull,
    kStrFuncTypeDeclVarWildExprWild,
    kStrFuncTypeDeclVarWildExprPartial,
    kStrFuncTypeDeclVarWildExprFull,
    kStrFuncTypeDeclVarDeclExprWild,
    kStrFuncTypeDeclVarDeclExprPartial,
    kStrFuncTypeDeclVarDeclExprFull,
    kStrFuncInvalidWhiteSpaceAfterPattern,
    kStrFuncInvalidTypeDeclInvalidVarDeclExprWild,
    kStrFuncInvalidTypeDeclVarNameExprWild,
    kStrFuncTypeDeclInvalidVarDeclExprWild,
    kStrFuncTypeDeclMissingQuoteVarNameExprWild,
    kStrFuncTypeDeclVarNameInvaidExpr,
    kStrFuncTypeDeclVarNameMissingQuoteExpr,
    kStrFuncTypeDeclVarNameMissingUnderWild,
    kStrFuncTypeDeclVarNameInvaidExpr2};
const std::vector<StringToStringFunc> kStrFuncsNegativeVar = {
    kStrFuncTypeDeclVarNameExprWildExprWild,
    kStrFuncTypeDeclVarWildExprWildExprWild,
    kStrFuncTypeDeclVarNameExprWild,
    kStrFuncTypeDeclVarNameExprPartial,
    kStrFuncTypeDeclVarNameExprFull,
    kStrFuncTypeDeclVarWildExprWild,
    kStrFuncTypeDeclVarWildExprPartial,
    kStrFuncTypeDeclVarWildExprFull,
    kStrFuncInvalidWhiteSpaceAfterPattern,
    kStrFuncInvalidTypeDeclInvalidVarDeclExprWild,
    kStrFuncInvalidTypeDeclVarNameExprWild,
    kStrFuncTypeDeclInvalidVarDeclExprWild,
    kStrFuncTypeDeclMissingQuoteVarNameExprWild,
    kStrFuncTypeDeclVarNameInvaidExpr,
    kStrFuncTypeDeclVarNameMissingQuoteExpr,
    kStrFuncTypeDeclVarNameMissingUnderWild,
    kStrFuncTypeDeclVarNameInvaidExpr2};
const std::vector<StringToStringFunc> kStrFuncsNegative = {
    kStrFuncTypeDeclVarNameExprWildExprWild,
    kStrFuncTypeDeclVarWildExprWildExprWild,
    kStrFuncTypeDeclVarNameExprWild,
    kStrFuncTypeDeclVarNameExprPartial,
    kStrFuncTypeDeclVarNameExprFull,
    kStrFuncTypeDeclVarWildExprWild,
    kStrFuncTypeDeclVarWildExprPartial,
    kStrFuncTypeDeclVarWildExprFull,
    kStrFuncTypeDeclVarDeclExprWild,
    kStrFuncTypeDeclVarDeclExprPartial,
    kStrFuncTypeDeclVarDeclExprFull,
    kStrFuncInvalidWhiteSpaceAfterPattern,
    kStrFuncInvalidTypeDeclInvalidVarDeclExprWild,
    kStrFuncInvalidTypeDeclVarNameExprWild,
    kStrFuncTypeDeclInvalidVarDeclExprWild,
    kStrFuncTypeDeclMissingQuoteVarNameExprWild,
    kStrFuncTypeDeclVarNameInvaidExpr,
    kStrFuncTypeDeclVarNameMissingQuoteExpr,
    kStrFuncTypeDeclVarNameMissingUnderWild,
    kStrFuncTypeDeclVarNameInvaidExpr2};

const std::vector<ClausesPair> kGetClausesPositive(
    Declaration::Type declaration_type) {
  switch (declaration_type) {
    case Declaration::Type::kAssign:
      return kPairsPositiveAssign;
    case Declaration::Type::kWhile:
      return kPairsPositiveWhile;
    case Declaration::Type::kIf:
      return kPairsPositiveIf;
    case Declaration::Type::kVariable:
      return kPairsPositiveVar;
    default:
      return kPairsPositive;
  }
}

const std::vector<StringToStringFunc> kGetFuncsNegative(
    Declaration::Type declaration_type) {
  switch (declaration_type) {
    case Declaration::Type::kAssign:
      return kStrFuncsNegativeAssign;
    case Declaration::Type::kWhile:
      return kStrFuncsNegativeWhile;
    case Declaration::Type::kIf:
      return kStrFuncsNegativeIf;
    case Declaration::Type::kVariable:
      return kStrFuncsNegativeVar;
    default:
      return kStrFuncsNegative;
  }
}

}  // namespace pattern

}  // namespace data

}  // namespace pql

}  // namespace test
