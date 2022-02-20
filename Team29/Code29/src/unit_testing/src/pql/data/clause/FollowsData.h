#pragma once

#include "../QueryDataUtil.h"
#include "catch.hpp"

namespace test {

namespace pql {

namespace data {

namespace follows {

const std::string kFollows = "Follows";

// functions that constructs query strings for follows test cases
// kStrFuncStmtNoStmtNo, kStrFuncDeclStmtNo includes whitespace tests
extern const StringToStringFunc kStrFuncStmtNoDecl, kStrFuncStmtNoStmtNo,
    kStrFuncDeclStmtNo, kStrFuncDeclDecl, kStrFuncDeclDefaultDecl,
    kStrFuncWildStmtNo, kStrFuncWildWild, kStrFuncStmtNoWild, kStrFuncZeroStmtN,
    kStrFuncDeclDecloStmtNo, kStrFuncNegativeStmtNoStmtNo,
    kStrFuncInvalidStmtNoStmtNo, kStrFuncInvalidNameStmtNo,
    kStrFuncInvalidWhiteSpaceSuchThat, kStrFuncInvalidWhiteSpaceBeforeSuchThat;

// functions that constructs expected clauses for follows test cases
extern const DeclPtrToClausePtrVecFunc kClausesFuncStmtNoDecl,
    kClausesFuncStmtNoStmtNo, kClausesFuncDeclStmtNo,
    kClausesFuncDeclDefaultDecl, kClausesFuncDeclDecl, kClausesFuncWildStmtNo,
    kClausesFuncWildWild, kClausesFuncStmtNoWild;

// clauses pairs for positive follows test cases
// to be used for statements (stmt, read, print, call, while, if, assign, prog_line)
extern const std::vector<ClausesPair> kPairsPositiveStmts;
// to be used for other entities
extern const std::vector<ClausesPair> kPairsPositive;

// functions for negative follows test cases
// to be used for statements (stmt, read, print, call, while, if, assign, prog_line)
extern const std::vector<StringToStringFunc> kStrFuncsNegativeStmts;
// to be used for other entities
extern const std::vector<StringToStringFunc> kStrFuncsNegative;

// returns the clauses pairs based on declaration type for follows positive test
// cases
extern const std::vector<ClausesPair> kGetClausesPositive(
    spa::pql::Declaration::Type declaration_type);

// returns query strings based on declaration type for follows negative test
// cases
extern const std::vector<StringToStringFunc> kGetFuncsNegative(
    spa::pql::Declaration::Type declaration_type);

}  // namespace follows

}  // namespace data

}  // namespace pql

}  // namespace test
