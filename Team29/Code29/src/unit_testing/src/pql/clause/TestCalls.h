#pragma once

#include "TestClause.h"
#include "catch.hpp"
#include "pql/clause/Calls.h"

namespace test {

namespace pql {

namespace clause {

using namespace spa::pql;

const Calls kCallsProcProcNameWild =
    Calls(kEntityRefPtrDeclProcProcName, kEntityRefPtrWild);
const Calls kCallsWildProcProcName =
    Calls(kEntityRefPtrWild, kEntityRefPtrDeclProcProcName);
const Calls kCallsTWildProcProcName =
    Calls(kEntityRefPtrWild, kEntityRefPtrDeclProcProcName, true);
const Calls kCallsWildWild = Calls(kEntityRefPtrWild, kEntityRefPtrWild);
const Calls kCallsIdXIdY = Calls(kEntityRefPtrIdX, kEntityRefPtrIdY);
const Calls kCallsIdXWild = Calls(kEntityRefPtrIdX, kEntityRefPtrWild);
const Calls kCallsTIdXWild = Calls(kEntityRefPtrIdX, kEntityRefPtrWild, true);

}  // namespace clause

}  // namespace pql

}  // namespace test
