#pragma once

#include "TestClause.h"
#include "catch.hpp"
#include "pql/clause/Pattern.h"

namespace test {

namespace pql {

namespace clause {

using namespace spa::pql;

// expression spec
const std::string kSpec1 = "(x * y % 5) + z / 2";
const std::string kSpec2 = "x * z % 5 + y / 4";
const std::string kSpec1Partial = "_\"(x * y % 5) + z / 2\"_";

// expression
const Pattern::Expression KExprWild = Pattern::Expression();
const Pattern::Expression KExprSpec1 = Pattern::Expression(kSpec1);
const Pattern::Expression KExprPartialSpec1 = Pattern::Expression(kSpec1, true);
const Pattern::Expression KExprPartialSpec2 = Pattern::Expression(kSpec2, true);

// assign type
const Pattern kPatternTypeAssignVarWildExprWild =
    Pattern(kStmtRefPtrDeclAssignAssignName, kEntityRefPtrWild, KExprWild);
const Pattern kPatternTypeAssignVarVarNameExprWild = Pattern(
    kStmtRefPtrDeclAssignAssignName, kEntityRefPtrDeclVarVarName, KExprWild);
const Pattern kPatternTypeAssignVarXExprWild =
    Pattern(kStmtRefPtrDeclAssignAssignName, kEntityRefPtrIdX, KExprWild);
const Pattern kPatternTypeAssignVarWildExprPartialSpec1 = Pattern(
    kStmtRefPtrDeclAssignAssignName, kEntityRefPtrWild, KExprPartialSpec1);
const Pattern kPatternTypeAssignVarXExprSpec1 =
    Pattern(kStmtRefPtrDeclAssignAssignName, kEntityRefPtrIdX, KExprSpec1);

// while type
const Pattern kPatternTypeWhileVarWildExprWild =
    Pattern(kStmtRefPtrDeclWhileWhileName, kEntityRefPtrWild, KExprWild);
const Pattern kPatternTypeWhileVarVarNameExprWild = Pattern(
    kStmtRefPtrDeclWhileWhileName, kEntityRefPtrDeclVarVarName, KExprWild);
const Pattern kPatternTypeWhileVarXExprWild =
    Pattern(kStmtRefPtrDeclWhileWhileName, kEntityRefPtrIdX, KExprWild);

// if type
const Pattern kPatternTypeIfVarWildExprWild =
    Pattern(kStmtRefPtrDeclIfIfName, kEntityRefPtrWild, KExprWild);
const Pattern kPatternTypeIfVarVarNameExprWild =
    Pattern(kStmtRefPtrDeclIfIfName, kEntityRefPtrDeclVarVarName, KExprWild);
const Pattern kPatternTypeIfVarXExprWild =
    Pattern(kStmtRefPtrDeclIfIfName, kEntityRefPtrIdX, KExprWild);

}  // namespace clause

}  // namespace pql

}  // namespace test
