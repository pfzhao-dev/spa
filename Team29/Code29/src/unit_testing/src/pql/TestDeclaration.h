#pragma once

#include "catch.hpp"
#include "pql/Declaration.h"

namespace test {

namespace pql {

namespace declaration {

using namespace spa::pql;

// names
const std::string kNameStmt = "s";
const std::string kNameRead = "rr0";
const std::string kNamePrint = "prr";
const std::string kNameCall = "BOOLEAN";
const std::string kNameWhile = "w";
const std::string kNameIf = "i";
const std::string kNameAssign = "a";
const std::string kNameVar = "v";
const std::string kNameConst = "c";
const std::string kNameProc = "procName";

// declarations with type stmt
const Declaration kDeclStmtNoName = Declaration(Declaration::Type::kStmt);
const Declaration kDeclStmtStmtName =
    Declaration(Declaration::Type::kStmt, kNameStmt);
const Declaration kDeclStmtStmtNameStmtNum = Declaration(
    Declaration::Type::kStmt, Declaration::ValueType::kStmtNum, kNameStmt);

// declarations with type read
const Declaration kDeclReadReadNameVarName = Declaration(
    Declaration::Type::kRead, Declaration::ValueType::kVarName, kNameRead);
const Declaration kDeclReadReadNameStmtNum = Declaration(
    Declaration::Type::kRead, Declaration::ValueType::kStmtNum, kNameRead);

// declarations with type print
const Declaration kDeclPrintPrintNameVarName = Declaration(
    Declaration::Type::kPrint, Declaration::ValueType::kVarName, kNamePrint);
const Declaration kDeclPrintPrintNameStmtNum = Declaration(
    Declaration::Type::kPrint, Declaration::ValueType::kStmtNum, kNamePrint);

// declarations with type call
const Declaration kDeclCallCallNameProcName = Declaration(
    Declaration::Type::kCall, Declaration::ValueType::kProcName, kNameCall);
const Declaration kDeclCallCallNameStmtNum = Declaration(
    Declaration::Type::kCall, Declaration::ValueType::kStmtNum, kNameCall);

// declarations with type while
const Declaration kDeclWhileWhileName =
    Declaration(Declaration::Type::kWhile, kNameWhile);
const Declaration kDeclWhileWhileNameStmtNum = Declaration(
    Declaration::Type::kWhile, Declaration::ValueType::kStmtNum, kNameWhile);

// declarations with type if
const Declaration kDeclIfIfName = Declaration(Declaration::Type::kIf, kNameIf);
const Declaration kDeclIfPrintNameStmtNum = Declaration(
    Declaration::Type::kPrint, Declaration::ValueType::kStmtNum, kNamePrint);

// declarations with type assign
const Declaration kDeclAssignAssignName =
    Declaration(Declaration::Type::kAssign, kNameAssign);
const Declaration kDeclAssignAssignNameStmtNum = Declaration(
    Declaration::Type::kAssign, Declaration::ValueType::kStmtNum, kNameAssign);

// declarations with type variable
const Declaration kDeclVarNoName = Declaration(Declaration::Type::kVariable);
const Declaration kDeclVarVarName =
    Declaration(Declaration::Type::kVariable, kNameVar);
const Declaration kDeclVarVarNameVarName = Declaration(
    Declaration::Type::kVariable, Declaration::ValueType::kVarName, kNameVar);

// declarations with type constant
const Declaration kDeclConstConstName =
    Declaration(Declaration::Type::kConstant, kNameConst);
const Declaration kDeclConstConstNameConstValue = Declaration(
    Declaration::Type::kConstant, Declaration::ValueType::kValue, kNameConst);

// declarations with type procedure
const Declaration kDeclProcNoName = Declaration(Declaration::Type::kProcedure);
const Declaration kDeclProcProcName =
    Declaration(Declaration::Type::kProcedure, kNameProc);
const Declaration kDeclProcProcNameProcName =
    Declaration(Declaration::Type::kProcedure,
                Declaration::ValueType::kProcName, kNameProc);

}  // namespace declaration

}  // namespace pql

}  // namespace test
