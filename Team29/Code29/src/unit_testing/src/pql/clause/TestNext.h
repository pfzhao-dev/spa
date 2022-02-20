#pragma once

#include "TestClause.h"
#include "catch.hpp"
#include "pql/clause/Next.h"

namespace test {

namespace pql {

namespace clause {

using namespace spa::pql;

const Next kNextStmt2Wild = Next(kStmtRefPtrStmtNo2, kStmtRefPtrWild);
const Next kNextTStmt2Wild = Next(kStmtRefPtrStmtNo2, kStmtRefPtrWild, true);
const Next kNextTStmt3Wild = Next(kStmtRefPtrStmtNo3, kStmtRefPtrWild, true);
const Next kNextStmt2StmtName =
    Next(kStmtRefPtrStmtNo2, kStmtRefPtrDeclStmtStmtName);

// NextBip
const Next kNextBipStmt2Wild =
    Next(kStmtRefPtrStmtNo2, kStmtRefPtrWild, false, true);
const Next kNextBipTStmt2Wild =
    Next(kStmtRefPtrStmtNo2, kStmtRefPtrWild, true, true);
const Next kNextBipTStmt3Wild =
    Next(kStmtRefPtrStmtNo3, kStmtRefPtrWild, true, true);
const Next kNextBipStmt2StmtName =
    Next(kStmtRefPtrStmtNo2, kStmtRefPtrDeclStmtStmtName, false, true);

}  // namespace clause

}  // namespace pql

}  // namespace test
