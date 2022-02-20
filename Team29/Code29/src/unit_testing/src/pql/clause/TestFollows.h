#pragma once

#include "TestClause.h"
#include "catch.hpp"
#include "pql/clause/Follows.h"

namespace test {

namespace pql {

namespace clause {

using namespace spa::pql;

const Follows kFollowsStmt2Wild = Follows(kStmtRefPtrStmtNo2, kStmtRefPtrWild);
const Follows kFollowsTStmt2Wild =
    Follows(kStmtRefPtrStmtNo2, kStmtRefPtrWild, true);
const Follows kFollowsTStmt3Wild =
    Follows(kStmtRefPtrStmtNo3, kStmtRefPtrWild, true);
const Follows kFollowsStmt2StmtName =
    Follows(kStmtRefPtrStmtNo2, kStmtRefPtrDeclStmtStmtName);

}  // namespace clause

}  // namespace pql

}  // namespace test
