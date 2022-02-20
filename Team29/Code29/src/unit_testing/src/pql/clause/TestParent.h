#pragma once

#include "TestClause.h"
#include "catch.hpp"
#include "pql/clause/Parent.h"

namespace test {

namespace pql {

namespace clause {

using namespace spa::pql;

const Parent kParentStmt2Wild = Parent(kStmtRefPtrStmtNo2, kStmtRefPtrWild);
const Parent kParentTStmt2Wild =
    Parent(kStmtRefPtrStmtNo2, kStmtRefPtrWild, true);
const Parent kParentTStmt3Wild =
    Parent(kStmtRefPtrStmtNo3, kStmtRefPtrWild, true);
const Parent kParentTStmt2StmtName =
    Parent(kStmtRefPtrStmtNo2, kStmtRefPtrDeclStmtStmtName, true);


}  // namespace clause

}  // namespace pql

}  // namespace test
