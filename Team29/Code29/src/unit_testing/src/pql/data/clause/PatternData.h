#pragma once

#include "../QueryDataUtil.h"
#include "catch.hpp"

namespace test {

namespace pql {

namespace data {

namespace pattern {

// functions that constructs query strings for pattern test cases
// kStrFuncTypeDeclVarNameExprWild, kStrFuncTypeDeclVarNameExprPartial,
// kStrFuncTypeDeclVarNameExprFull includes whitespace tests
extern const StringToStringFunc kStrFuncTypeDeclVarNameExprWild,
    kStrFuncTypeDeclVarNameExprPartial, kStrFuncTypeDeclVarNameExprFull,
    kStrFuncTypeDeclVarNameExprWildExprWild, kStrFuncTypeDeclVarWildExprWild,
    kStrFuncTypeDeclVarWildExprWildExprWild, kStrFuncTypeDeclVarWildExprPartial,
    kStrFuncTypeDeclVarWildExprFull, kStrFuncTypeDeclVarDeclExprWild,
    kStrFuncTypeDeclVarDeclExprPartial, kStrFuncTypeDeclVarDeclExprFull,
    kStrFuncTypeDeclVarDeclExprWildExprWild,
    kStrFuncInvalidWhiteSpaceAfterPattern,
    kStrFuncInvalidTypeDeclInvalidVarDeclExprWild,
    kStrFuncInvalidTypeDeclVarNameExprWild,
    kStrFuncTypeDeclInvalidVarDeclExprWild,
    kStrFuncTypeDeclMissingQuoteVarNameExprWild,
    kStrFuncTypeDeclVarNameInvaidExpr, kStrFuncTypeDeclVarNameMissingQuoteExpr,
    kStrFuncTypeDeclVarNameMissingUnderWild, kStrFuncTypeDeclVarNameInvaidExpr2;

// functions that constructs expected clauses for pattern test cases
extern const DeclPtrToClausePtrVecFunc kClausesFuncTypeDeclVarNameExprWild,
    kClausesFuncTypeDeclVarNameExprPartial, kClausesFuncTypeDeclVarNameExprFull,
    kClausesFuncTypeDeclVarNameExprWildExprWild,
    kClausesFuncTypeDeclVarWildExprWild,
    kClausesFuncTypeDeclVarWildExprWildExprWild,
    kClausesFuncTypeDeclVarWildExprPartial, kClausesFuncTypeDeclVarWildExprFull,
    kClausesFuncTypeDeclVarDeclExprWild, kClausesFuncTypeDeclVarDeclExprPartial,
    kClausesFuncTypeDeclVarDeclExprFull,
    kClausesFuncTypeDeclVarDeclExprWildExprWild;

// clauses pairs for positive pattern test cases
// to be used for assign
extern const std::vector<ClausesPair> kPairsPositiveAssign;
// to be used for while
extern const std::vector<ClausesPair> kPairsPositiveWhile;
// to be used for if
extern const std::vector<ClausesPair> kPairsPositiveIf;
// to be used for var
extern const std::vector<ClausesPair> kPairsPositiveVar;
// to be used for other entities
extern const std::vector<ClausesPair> kPairsPositive;

// functions for negative pattern test cases
// to be used for assign
extern const std::vector<StringToStringFunc> kStrFuncsNegativeAssign;
// to be used for while
extern const std::vector<StringToStringFunc> kStrFuncsNegativeWhile;
// to be used for if
extern const std::vector<StringToStringFunc> kStrFuncsNegativeIf;
// to be used for var
extern const std::vector<StringToStringFunc> kStrFuncsNegativeVar;
// to be used for other entities
extern const std::vector<StringToStringFunc> kStrFuncsNegative;

// returns the clauses pairs based on declaration type for pattern positive test
// cases
extern const std::vector<ClausesPair> kGetClausesPositive(
    spa::pql::Declaration::Type declaration_type);

// returns query strings based on declaration type for pattern negative test
// cases
extern const std::vector<StringToStringFunc> kGetFuncsNegative(
    spa::pql::Declaration::Type declaration_type);

}  // namespace pattern

}  // namespace data

}  // namespace pql

}  // namespace test
