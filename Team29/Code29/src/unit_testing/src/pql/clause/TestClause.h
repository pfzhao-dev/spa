#pragma once

#include "../TestRef.h"
#include "catch.hpp"

namespace test {

namespace pql {

namespace clause {

using namespace spa::pql;

// statement ref pointers
const std::shared_ptr<StatementRef> kStmtRefPtrEmpty =
    std::shared_ptr<StatementRef>();
const std::shared_ptr<StatementRef> kStmtRefPtrWild =
    std::make_shared<StatementRef>(ref::kStmtRefWild);
const std::shared_ptr<StatementRef> kStmtRefPtrStmtNo2 =
    std::make_shared<StatementRef>(ref::kStmtRefStmtNo2);
const std::shared_ptr<StatementRef> kStmtRefPtrStmtNo3 =
    std::make_shared<StatementRef>(ref::kStmtRefStmtNo3);
const std::shared_ptr<StatementRef> kStmtRefPtrDeclStmtStmtName =
    std::make_shared<StatementRef>(ref::kStmtRefDeclStmtStmtName);
const std::shared_ptr<StatementRef> kStmtRefPtrDeclAssignAssignName =
    std::make_shared<StatementRef>(ref::kStmtRefDeclAssignAssignName);
const std::shared_ptr<StatementRef> kStmtRefPtrDeclWhileWhileName =
    std::make_shared<StatementRef>(ref::kStmtRefDeclWhileWhileName);
const std::shared_ptr<StatementRef> kStmtRefPtrDeclIfIfName =
    std::make_shared<StatementRef>(ref::kStmtRefDeclIfIfName);

// duplicate statement ref pointers
const std::shared_ptr<StatementRef> kStmtRefPtrWildDuplicate =
    std::make_shared<StatementRef>(ref::kStmtRefWild);
const std::shared_ptr<StatementRef> kStmtRefPtrStmtNo2Duplicate =
    std::make_shared<StatementRef>(ref::kStmtRefStmtNo2);
const std::shared_ptr<StatementRef> kStmtRefPtrDeclStmtStmtNameDuplicate =
    std::make_shared<StatementRef>(ref::kStmtRefDeclStmtStmtName);
const std::shared_ptr<StatementRef> kStmtRefPtrDeclAssignAssignNameDuplicate =
    std::make_shared<StatementRef>(ref::kStmtRefDeclAssignAssignName);

// entity ref pointers
const std::shared_ptr<EntityRef> kEntityRefPtrEmpty =
    std::shared_ptr<EntityRef>();
const std::shared_ptr<EntityRef> kEntityRefPtrWild =
    std::make_shared<EntityRef>(ref::kEntityRefWild);
const std::shared_ptr<EntityRef> kEntityRefPtrIdX =
    std::make_shared<EntityRef>(ref::kEntityRefIdX);
const std::shared_ptr<EntityRef> kEntityRefPtrIdY =
    std::make_shared<EntityRef>(ref::kEntityRefIdY);
const std::shared_ptr<EntityRef> kEntityRefPtrDeclStmtStmtName =
    std::make_shared<EntityRef>(ref::kEntityRefDeclStmtStmtName);
const std::shared_ptr<EntityRef> kEntityRefDeclPtrStmtStmtNumStmtName =
    std::make_shared<EntityRef>(ref::kEntityRefDeclStmtStmtNumStmtName);
const std::shared_ptr<EntityRef> kEntityRefPtrDeclVarVarName =
    std::make_shared<EntityRef>(ref::kEntityRefDeclVarVarName);
const std::shared_ptr<EntityRef> kEntityRefPtrDeclProcProcName =
    std::make_shared<EntityRef>(ref::kEntityRefDeclProcProcName);
const std::shared_ptr<EntityRef> kEntityRefPtrDeclProcProcNameProcName =
    std::make_shared<EntityRef>(ref::kEntityRefDeclProcProcNameProcName);
const std::shared_ptr<EntityRef> kEntityRefPtrDeclConstConstNameConstValue =
    std::make_shared<EntityRef>(ref::kEntityRefDeclConstConstNameConstValue);
const std::shared_ptr<EntityRef> kEntityRefPtrDeclPrintPrintNameVarName =
    std::make_shared<EntityRef>(ref::kEntityRefDeclPrintPrintNameVarName);

// duplicate entity ref pointers
const std::shared_ptr<EntityRef> kEntityRefPtrWildDuplicate =
    std::make_shared<EntityRef>(ref::kEntityRefWild);
const std::shared_ptr<EntityRef> kEntityRefPtrDeclStmtStmtNameDuplicate =
    std::make_shared<EntityRef>(ref::kEntityRefDeclStmtStmtName);
const std::shared_ptr<EntityRef> kEntityRefPtrDeclProcProcNameDuplicate =
    std::make_shared<EntityRef>(ref::kEntityRefDeclProcProcName);
const std::shared_ptr<EntityRef> kEntityRefPtrDeclConstNameConstValueDuplicate =
    std::make_shared<EntityRef>(ref::kEntityRefDeclConstConstNameConstValue);

}  // namespace clause

}  // namespace pql

}  // namespace test
