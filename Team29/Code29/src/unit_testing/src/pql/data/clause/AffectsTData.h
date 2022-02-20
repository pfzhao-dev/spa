#pragma once

#include "../QueryDataUtil.h"
#include "catch.hpp"

namespace test {

namespace pql {

namespace data {

namespace affectst {

const std::string kAffectsT = "Affects*";

// functions that constructs query strings for affects* test cases
extern const StringToStringFunc kStrFuncStmtNoDecl, kStrFuncStmtNoStmtNo,
    kStrFuncDeclStmtNo, kStrFuncDeclDecl, kStrFuncDefaultDeclDecl,
    kStrFuncWildStmtNo, kStrFuncWildWild, kStrFuncStmtNoWild,
    kStrFuncZeroStmtNoStmtNo, kStrFuncNegativeStmtNoStmtNo,
    kStrFuncInvalidStmtNoStmtNo, kStrFuncInvalidNameStmtNo,
    kStrFuncInvalidWhiteSpaceAfterAffects;

// functions that constructs expected clauses for affects* test cases
extern const DeclPtrToClausePtrVecFunc kClausesFuncStmtNoDecl,
    kClausesFuncStmtNoStmtNo, kClausesFuncDeclStmtNo, kClausesFuncDeclDecl,
    kClausesFuncDefaultDeclDecl, kClausesFuncWildStmtNo, kClausesFuncWildWild,
    kClausesFuncStmtNoWild;

// clauses pairs for positive affects* test cases
// to be used for statements (stmt, assign, prog_line)
extern const std::vector<ClausesPair> kPairsPositiveStmts;
// to be used for other entities
extern const std::vector<ClausesPair> kPairsPositive;

// functions for negative affects* test cases
// to be used for statements (stmt, assign, prog_line)
extern const std::vector<StringToStringFunc> kStrFuncsNegativeStmts;
// to be used for other entities
extern const std::vector<StringToStringFunc> kStrFuncsNegative;

// returns the clauses pairs based on declaration type for affects* positive
// test cases
extern const std::vector<ClausesPair> kGetClausesPositive(
    spa::pql::Declaration::Type declaration_type);

// returns query strings based on declaration type for affects* negative test
// cases
extern const std::vector<StringToStringFunc> kGetFuncsNegative(
    spa::pql::Declaration::Type declaration_type);

}  // namespace affectst

}  // namespace data

}  // namespace pql

}  // namespace test
