#pragma once

#include "../QueryDataUtil.h"
#include "catch.hpp"

namespace test {

namespace pql {

namespace data {

namespace parent {

const std::string kParent = "Parent";

// functions that constructs query strings for parent test cases
extern const StringToStringFunc kStrFuncStmtNoDecl, kStrFuncStmtNoStmtNo,
    kStrFuncDeclStmtNo, kStrFuncDeclDecl, kStrFuncDefaultDeclDecl,
    kStrFuncWildStmtNo, kStrFuncWildWild, kStrFuncStmtNoWild,
    kStrFuncZeroStmtNoStmtNo, kStrFuncNegativeStmtNoStmtNo,
    kStrFuncInvalidStmtNoStmtNo, kStrFuncInvalidNameStmtNo;

// functions that constructs expected clauses for parent test cases
extern const DeclPtrToClausePtrVecFunc kClausesFuncStmtNoDecl,
    kClausesFuncStmtNoStmtNo, kClausesFuncDeclStmtNo, kClausesFuncDeclDecl,
    kClausesFuncDefaultDeclDecl, kClausesFuncWildStmtNo, kClausesFuncWildWild,
    kClausesFuncStmtNoWild;

// clauses pairs for positive parent test cases
// to be used for statements (stmt, read, print, call, while, if, assign,
// prog_line)
extern const std::vector<ClausesPair> kPairsPositiveStmts;
// to be used for other entities
extern const std::vector<ClausesPair> kPairsPositive;

// functions for negative parent test cases
// to be used for statements (stmt, read, print, call, while, if, assign,
// prog_line)
extern const std::vector<StringToStringFunc> kStrFuncsNegativeStmts;
// to be used for other entities
extern const std::vector<StringToStringFunc> kStrFuncsNegative;

// returns the clauses pairs based on declaration type for parent positive test
// cases
extern const std::vector<ClausesPair> kGetClausesPositive(
    spa::pql::Declaration::Type declaration_type);

// returns query strings based on declaration type for parent negative test
// cases
extern const std::vector<StringToStringFunc> kGetFuncsNegative(
    spa::pql::Declaration::Type declaration_type);

}  // namespace parent

}  // namespace data

}  // namespace pql

}  // namespace test
