#pragma once

#include "TestRef.h"
#include "catch.hpp"
#include "pql/Result.h"

namespace test {

namespace pql {

namespace result {

using namespace spa::pql;

// vector of declaration ptrs
const std::vector<std::shared_ptr<Declaration>> kDeclPtrVecStmtStmtName = {
    ref::kDeclPtrStmtStmtName};
const std::vector<std::shared_ptr<Declaration>> kDeclPtrVecConstConstName = {
    ref::kDeclPtrConstConstName};
const std::vector<std::shared_ptr<Declaration>> kDeclPtrVecAssignAssignName = {
    ref::kDeclPtrAssignAssignName};
const std::vector<std::shared_ptr<Declaration>> kDeclPtrVecVarVarName = {
    ref::kDeclPtrVarVarName};
const std::vector<std::shared_ptr<Declaration>> kDeclPtrVecProcProcName = {
    ref::kDeclPtrProcProcName};
const std::vector<std::shared_ptr<Declaration>>
    kDeclPtrVecStmtStmtNameConstConstName = {ref::kDeclPtrStmtStmtName,
                                             ref::kDeclPtrConstConstName};
const std::vector<std::shared_ptr<Declaration>> kDeclPtrVecMultiple = {
    ref::kDeclPtrStmtStmtName, ref::kDeclPtrConstConstName,
    ref::kDeclPtrConstConstNameConstValue, ref::kDeclPtrAssignAssignName};
const std::vector<std::shared_ptr<Declaration>> kDeclPtrVecMultipleReordered = {
    ref::kDeclPtrStmtStmtName, ref::kDeclPtrConstConstNameConstValue,
    ref::kDeclPtrConstConstName, ref::kDeclPtrAssignAssignName};

// declared results
const DeclaredResult kDeclResultStmtStmtName =
    DeclaredResult(kDeclPtrVecStmtStmtName);
const DeclaredResult kDeclResultConstConstName =
    DeclaredResult(kDeclPtrVecConstConstName);
const DeclaredResult kDeclResultAssignAssignName =
    DeclaredResult(kDeclPtrVecAssignAssignName);
const DeclaredResult kDeclResultProcProcName =
    DeclaredResult(kDeclPtrVecProcProcName);
const DeclaredResult kDeclResultVarVarName =
    DeclaredResult(kDeclPtrVecVarVarName);
const DeclaredResult kDeclResultStmtStmtNameConstConstName =
    DeclaredResult(kDeclPtrVecStmtStmtNameConstConstName);
const DeclaredResult kDeclResultMultiple = DeclaredResult(kDeclPtrVecMultiple);
const DeclaredResult kDeclResultMultipleReordered =
    DeclaredResult(kDeclPtrVecMultipleReordered);

// boolean results
const BooleanResult kBoolResult = BooleanResult();

}  // namespace result

}  // namespace pql

}  // namespace test
