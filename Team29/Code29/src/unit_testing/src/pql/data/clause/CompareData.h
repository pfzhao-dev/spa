#pragma once

#include "../QueryDataUtil.h"
#include "catch.hpp"

namespace test {

namespace pql {

namespace data {

namespace compare {

// functions that constructs query strings for compare test cases
// kStrFuncVarDeclProcName, kStrFuncVarDeclVarName includes whitespace tests
extern const StringToStringFunc kStrFuncVarDeclProcName, kStrFuncVarDeclVarName,
    kStrFuncDeclConstValueStmtNo, kStrFuncStmtNoDeclStmtNum,
    kStrFuncDeclDefaultDeclDefault, kStrFuncZeroStmtNoVar,
    kStrFuncNegativeStmtNoVar, kStrFuncInvalidStmtNoVar, kStrFuncInvalidRefVar,
    kStrFuncInvalidValueVar, kStrFuncDelVar, kStrFuncInvalidDeclVar,
    kStrFuncStmtNoMissingQuoteVar, kStrFuncInvalidWhiteSpaceWith;

// functions that constructs expected clauses for compare test cases
extern const DeclPtrToClausePtrVecFunc kClausesFuncVarDeclProcName,
    kClausesFuncVarDeclVarName, kClausesFuncDeclConstValueStmtNo,
    kClausesFuncStmtNoDeclStmtNum, kClausesFuncDeclDefaultDeclDefault;

// clauses pairs for positive compare test cases
// to be used for procedure
extern const std::vector<ClausesPair> kPairsPositiveProc;
// to be used for call
extern const std::vector<ClausesPair> kPairsPositiveCall;
// to be used for variable
extern const std::vector<ClausesPair> kPairsPositiveVar;
// to be used for read and print
extern const std::vector<ClausesPair> kPairsPositiveReadPrint;
// to be used for constant
extern const std::vector<ClausesPair> kPairsPositiveConst;
// to be used for stmt and prog_line
extern const std::vector<ClausesPair> kPairsPositiveStmtProgLine;
// to be used for statements (while, if, assign)
extern const std::vector<ClausesPair> kPairsPositiveStatements;

// functions for negative compare test cases
// // to be used for procedure
extern const std::vector<StringToStringFunc> kStrFuncsNegativeProc;
// to be used for call
extern const std::vector<StringToStringFunc> kStrFuncsNegativeCall;
// to be used for variable
extern const std::vector<StringToStringFunc> kStrFuncsNegativeVar;
// to be used for read and print
extern const std::vector<StringToStringFunc> kStrFuncsNegativeReadPrint;
// to be used for constant
extern const std::vector<StringToStringFunc> kStrFuncsNegativeConst;
// to be used for stmt and prog_line
extern const std::vector<StringToStringFunc> kStrFuncsNegativeStmtProgLine;
// to be used for statements (while, if, assign)
extern const std::vector<StringToStringFunc> kStrFuncsNegativeStatements;

// returns the clauses pairs based on declaration type for compare positive
// test cases
extern const std::vector<ClausesPair> kGetClausesPositive(
    spa::pql::Declaration::Type declaration_type);

// returns query strings based on declaration type for compare negative test
// cases
extern const std::vector<StringToStringFunc> kGetFuncsNegative(
    spa::pql::Declaration::Type declaration_type);

}  // namespace compare

}  // namespace data

}  // namespace pql

}  // namespace test
