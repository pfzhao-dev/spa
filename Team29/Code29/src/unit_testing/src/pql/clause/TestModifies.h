#pragma once

#include "TestClause.h"
#include "catch.hpp"
#include "pql/clause/Modifies.h"

namespace test {

namespace pql {

namespace clause {

using namespace spa::pql;

const Modifies kModifiesStmtStmtNameWild =
    Modifies(kEntityRefPtrDeclStmtStmtName, kEntityRefPtrWild);
const Modifies kModifiesStmtNo2VarVarName =
    Modifies(kStmtRefPtrStmtNo2, kEntityRefPtrDeclVarVarName);
const Modifies kModifiesStmtNo2Wild =
    Modifies(kStmtRefPtrStmtNo2, kEntityRefPtrWild);
const Modifies kModifiesIdXWild = Modifies(kEntityRefPtrIdX, kEntityRefPtrWild);
const Modifies kModifiesIdXIdY = Modifies(kEntityRefPtrIdX, kEntityRefPtrIdY);

}  // namespace clause

}  // namespace pql

}  // namespace test
