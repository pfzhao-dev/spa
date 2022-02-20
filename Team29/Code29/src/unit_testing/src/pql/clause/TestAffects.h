#pragma once

#include "TestClause.h"
#include "catch.hpp"
#include "pql/clause/Affects.h"

namespace test {

namespace pql {

namespace clause {

using namespace spa::pql;

const Affects kAffectsStmt2Wild = Affects(kStmtRefPtrStmtNo2, kStmtRefPtrWild);
const Affects kAffectsTStmt2Wild =
    Affects(kStmtRefPtrStmtNo2, kStmtRefPtrWild, true);
const Affects kAffectsTStmt3Wild =
    Affects(kStmtRefPtrStmtNo3, kStmtRefPtrWild, true);
const Affects kAffectsTStmt2AssignName =
    Affects(kStmtRefPtrStmtNo2, kStmtRefPtrDeclAssignAssignName, true);

// AffectsBip
const Affects kAffectsBipStmt2Wild =
    Affects(kStmtRefPtrStmtNo2, kStmtRefPtrWild, false, true);
const Affects kAffectsBipStmt3Wild =
    Affects(kStmtRefPtrStmtNo3, kStmtRefPtrWild, false, true);
const Affects kAffectsBipTStmt2Wild =
    Affects(kStmtRefPtrStmtNo2, kStmtRefPtrWild, true, true);
const Affects kAffectsBipTStmt2AssignName =
    Affects(kStmtRefPtrStmtNo2, kStmtRefPtrDeclAssignAssignName, true, true);

}  // namespace clause

}  // namespace pql

}  // namespace test
