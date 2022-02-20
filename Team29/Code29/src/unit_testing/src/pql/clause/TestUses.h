#pragma once

#include "TestClause.h"
#include "catch.hpp"
#include "pql/clause/Uses.h"

namespace test {

namespace pql {

namespace clause {

using namespace spa::pql;

const Uses kUsesStmtStmtNameWild =
    Uses(kEntityRefPtrDeclStmtStmtName, kEntityRefPtrWild);
const Uses kUsesStmtNo2Wild = Uses(kStmtRefPtrStmtNo2, kEntityRefPtrWild);
const Uses kUsesIdXWild = Uses(kEntityRefPtrIdX, kEntityRefPtrWild);
const Uses kUsesIdXIdY = Uses(kEntityRefPtrIdX, kEntityRefPtrIdY);


}  // namespace clause

}  // namespace pql

}  // namespace test
