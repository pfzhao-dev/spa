#pragma once

#include "TestClause.h"
#include "catch.hpp"
#include "pql/clause/Compare.h"

namespace test {

namespace pql {

namespace clause {

using namespace spa::pql;

// compare integers
const Compare kCompareStmtStmtNumStmtNameStmtNo2 =
    Compare(kEntityRefDeclPtrStmtStmtNumStmtName, kStmtRefPtrStmtNo2);
const Compare kCompareStmtStmtNumStmtNameStmtNo3 =
    Compare(kEntityRefDeclPtrStmtStmtNumStmtName, kStmtRefPtrStmtNo3);
const Compare kCompareStmtStmtNameConstNameConstValue = Compare(
    kEntityRefPtrDeclStmtStmtName, kEntityRefPtrDeclConstConstNameConstValue);

// compare names
const Compare kCompareIdXPrintPrintNameVarName =
    Compare(kEntityRefPtrIdX, kEntityRefPtrDeclPrintPrintNameVarName);
const Compare kCompareIdXProcProcNameProcName =
    Compare(kEntityRefPtrIdX, kEntityRefPtrDeclProcProcNameProcName);

}  // namespace clause

}  // namespace pql

}  // namespace test
