#pragma once

#include "TestDeclaration.h"
#include "catch.hpp"
#include "pql/Ref.h"

namespace test {

namespace pql {

namespace ref {

using namespace spa::pql;

// statement numbers
const uint32_t kStmtNo2 = 2;
const uint32_t kStmtNo3 = 3;

// ids
const std::string kIdX = "x";
const std::string kIdY = "y";

// pointers of declarations
const std::shared_ptr<Declaration> kDeclPtrEmpty =
    std::shared_ptr<Declaration>();
const std::shared_ptr<Declaration> kDeclPtrStmtStmtName =
    std::make_shared<Declaration>(declaration::kDeclStmtStmtName);
const std::shared_ptr<Declaration> kDeclPtrWhileWhileName =
    std::make_shared<Declaration>(declaration::kDeclWhileWhileName);
const std::shared_ptr<Declaration> kDeclPtrIfIfName =
    std::make_shared<Declaration>(declaration::kDeclIfIfName);
const std::shared_ptr<Declaration> kDeclPtrStmtStmtNumStmtName =
    std::make_shared<Declaration>(declaration::kDeclStmtStmtNameStmtNum);
const std::shared_ptr<Declaration> kDeclPtrAssignAssignName =
    std::make_shared<Declaration>(declaration::kDeclAssignAssignName);
const std::shared_ptr<Declaration> kDeclPtrVarNoName =
    std::make_shared<Declaration>(declaration::kDeclVarNoName);
const std::shared_ptr<Declaration> kDeclPtrVarVarName =
    std::make_shared<Declaration>(declaration::kDeclVarVarName);
const std::shared_ptr<Declaration> kDeclPtrConstConstName =
    std::make_shared<Declaration>(declaration::kDeclConstConstName);
const std::shared_ptr<Declaration> kDeclPtrConstConstNameConstValue =
    std::make_shared<Declaration>(declaration::kDeclConstConstNameConstValue);
const std::shared_ptr<Declaration> kDeclPtrProcProcName =
    std::make_shared<Declaration>(declaration::kDeclProcProcName);
const std::shared_ptr<Declaration> kDeclPtrProcProcNameProcName =
    std::make_shared<Declaration>(declaration::kDeclProcProcNameProcName);
const std::shared_ptr<Declaration> kDeclPtrPrintPrintNameVarName =
    std::make_shared<Declaration>(declaration::kDeclPrintPrintNameVarName);

// duplicate pointers of declarations
const std::shared_ptr<Declaration> kDeclPtrStmtStmtNameDuplicate =
    std::make_shared<Declaration>(declaration::kDeclStmtStmtName);
const std::shared_ptr<Declaration> kDeclPtrVarNoNameDuplicate =
    std::make_shared<Declaration>(declaration::kDeclVarNoName);
const std::shared_ptr<Declaration> kDeclPtrConstConstNameDuplicate =
    std::make_shared<Declaration>(declaration::kDeclConstConstName);

// entity refs
const EntityRef kEntityRefDeclEmpty = EntityRef(kDeclPtrEmpty);
const EntityRef kEntityRefWild = EntityRef();
const EntityRef kEntityRefIdX = EntityRef(kIdX);
const EntityRef kEntityRefIdY = EntityRef(kIdY);
const EntityRef kEntityRefDeclAssignAssignName =
    EntityRef(kDeclPtrAssignAssignName);
const EntityRef kEntityRefDeclStmtStmtName = EntityRef(kDeclPtrStmtStmtName);
const EntityRef kEntityRefDeclStmtStmtNumStmtName =
    EntityRef(kDeclPtrStmtStmtNumStmtName);
const EntityRef kEntityRefDeclVarNoName = EntityRef(kDeclPtrVarNoName);
const EntityRef kEntityRefDeclVarVarName = EntityRef(kDeclPtrVarVarName);
const EntityRef kEntityRefDeclProcProcName = EntityRef(kDeclPtrProcProcName);
const EntityRef kEntityRefDeclProcProcNameProcName =
    EntityRef(kDeclPtrProcProcNameProcName);
const EntityRef kEntityRefDeclConstConstNameConstValue =
    EntityRef(kDeclPtrConstConstNameConstValue);
const EntityRef kEntityRefDeclPrintPrintNameVarName =
    EntityRef(kDeclPtrPrintPrintNameVarName);

// statement refs
const StatementRef kStmtRefWild = StatementRef();
const StatementRef kStmtRefStmtNo2 = StatementRef(kStmtNo2);
const StatementRef kStmtRefStmtNo3 = StatementRef(kStmtNo3);
const StatementRef kStmtRefDeclAssignAssignName =
    StatementRef(kDeclPtrAssignAssignName);
const StatementRef kStmtRefDeclStmtStmtName =
    StatementRef(kDeclPtrStmtStmtName);
const StatementRef kStmtRefDeclWhileWhileName =
    StatementRef(kDeclPtrWhileWhileName);
const StatementRef kStmtRefDeclIfIfName = StatementRef(kDeclPtrIfIfName);

}  // namespace ref

}  // namespace pql

}  // namespace test
