#pragma once

#include "../QueryDataUtil.h"
#include "catch.hpp"

namespace test {

namespace pql {

namespace data {

namespace uses {

const std::string kUses = "Uses";

// functions that constructs query strings for uses test cases
extern const StringToStringFunc kStrFuncStmtNoVar, kStrFuncStmtNoDecl,
    kStrFuncStmtNoWild, kStrFuncDeclVar, kStrFuncDeclWild, kStrFuncProcVar,
    kStrFuncProcDecl, kStrFuncProcWild, kStrFuncWildVar, kStrFuncWildWild,
    kStrFuncWildDecl, kStrFuncDeclDefaultDecl, kStrFuncZeroStmtNoVar,
    kStrFuncNegativeStmtNoVar, kStrFuncInvalidStmtNoVar, kStrFuncDeclDecl,
    kStrFuncInvalidDeclVar, kStrFuncStmtNoInvalidVar;

// functions that constructs expected clauses for uses test cases
extern const DeclPtrToClausePtrVecFunc kClausesFuncStmtNoVar,
    kClausesFuncStmtNoDecl, kClausesFuncStmtNoWild, kClausesFuncDeclVar,
    kClausesFuncDeclWild, kClausesFuncProcVar, kClausesFuncProcDecl,
    kClausesFuncProcWild, kClausesFuncDeclDefaultDecl;

// clauses pairs for positive uses test cases
// to be used for statements (variable)
extern const std::vector<ClausesPair> kPairsPositiveVar;
// to be used for statements (stmt, print, call, while, if, assign, procedure, prog_line)
extern const std::vector<ClausesPair> kPairsPositiveStmts;
// to be used for other entities
extern const std::vector<ClausesPair> kPairsPositive;

// functions for negative uses test cases
// to be used for statements (variable)
extern const std::vector<StringToStringFunc> kStrFuncsNegativeVar;
// to be used for statements (stmt, print, call, while, if, assign, procedure, prog_line)
extern const std::vector<StringToStringFunc> kStrFuncsNegativeStmts;
// to be used for other entities
extern const std::vector<StringToStringFunc> kStrFuncsNegative;

// returns the clauses pairs based on declaration type for uses positive test
// cases
extern const std::vector<ClausesPair> kGetClausesPositive(
    spa::pql::Declaration::Type declaration_type);

// returns query strings based on declaration type for uses negative test cases
extern const std::vector<StringToStringFunc> kGetFuncsNegative(
    spa::pql::Declaration::Type declaration_type);

}  // namespace uses

}  // namespace data

}  // namespace pql

}  // namespace test
