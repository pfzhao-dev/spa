#pragma once

#include "../QueryDataUtil.h"
#include "catch.hpp"

namespace test {

namespace pql {

namespace data {

namespace callst {

const std::string kCallsT = "Calls*";

// functions that constructs query strings for calls* test cases
extern const StringToStringFunc kStrFuncVarDecl, kStrFuncVarVar,
    kStrFuncDeclVar, kStrFuncDeclDecl, kStrFuncDefaultDeclDecl, kStrFuncWildVar,
    kStrFuncWildWild, kStrFuncVarWild, kStrFuncInvalidNameVar,
    kStrFuncVarMissingQuoteVar, kStrFuncInvalidWhiteSpaceAfterCalls;

// functions that constructs expected clauses for calls* test cases
extern const DeclPtrToClausePtrVecFunc kClausesFuncVarDecl, kClausesFuncVarVar,
    kClausesFuncDeclVar, kClausesFuncDeclDecl, kClausesFuncDefaultDeclDecl,
    kClausesFuncWildVar, kClausesFuncWildWild, kClausesFuncVarWild;

// clauses pairs for positive calls* test cases
// to be used for procedure
extern const std::vector<ClausesPair> kPairsPositiveProc;
// to be used for other entities
extern const std::vector<ClausesPair> kPairsPositive;

// functions for negative calls* test cases
// to be used for procedure
extern const std::vector<StringToStringFunc> kStrFuncsNegativeProc;
// to be used for other entities
extern const std::vector<StringToStringFunc> kStrFuncsNegative;

// returns the clauses pairs based on declaration type for calls* positive test
// cases
extern const std::vector<ClausesPair> kGetClausesPositive(
    spa::pql::Declaration::Type declaration_type);

// returns query strings based on declaration type for calls* negative test
// cases
extern const std::vector<StringToStringFunc> kGetFuncsNegative(
    spa::pql::Declaration::Type declaration_type);

}  // namespace callst

}  // namespace data

}  // namespace pql

}  // namespace test
