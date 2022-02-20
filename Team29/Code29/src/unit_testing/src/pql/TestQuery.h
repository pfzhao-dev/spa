#pragma once

#include "TestResult.h"
#include "catch.hpp"
#include "clause/TestAffects.h"
#include "clause/TestCalls.h"
#include "clause/TestCompare.h"
#include "clause/TestFollows.h"
#include "clause/TestModifies.h"
#include "clause/TestNext.h"
#include "clause/TestParent.h"
#include "clause/TestPattern.h"
#include "clause/TestUses.h"
#include "pql/Query.h"

namespace test {

namespace pql {

namespace query {

using namespace spa::pql;

// pointers of results
const std::shared_ptr<Result> kResultPtrBool =
    std::make_shared<BooleanResult>(result::kBoolResult);
const std::shared_ptr<Result> kResultPtrStmtStmtName =
    std::make_shared<DeclaredResult>(result::kDeclResultStmtStmtName);
const std::shared_ptr<Result> kResultPtrConstConstName =
    std::make_shared<DeclaredResult>(result::kDeclResultConstConstName);
const std::shared_ptr<Result> kResultPtrAssignAssignName =
    std::make_shared<DeclaredResult>(result::kDeclResultAssignAssignName);
const std::shared_ptr<Result> kResultPtrProcProcName =
    std::make_shared<DeclaredResult>(result::kDeclResultProcProcName);
const std::shared_ptr<Result> kResultPtrVarVarName =
    std::make_shared<DeclaredResult>(result::kDeclResultVarVarName);
const std::shared_ptr<Result> kResultPtrMultiple =
    std::make_shared<DeclaredResult>(result::kDeclResultMultiple);

// duplicate pointers of results
const std::shared_ptr<Result> kResultPtrStmtStmtNameDuplicate =
    std::make_shared<DeclaredResult>(result::kDeclResultStmtStmtName);
const std::shared_ptr<Result> kResultPtrConstConstNameDuplicate =
    std::make_shared<DeclaredResult>(result::kDeclResultConstConstName);

// pointers of clauses
const std::shared_ptr<Clause> kFollowsPtrStmt2Wild =
    std::make_shared<Follows>(clause::kFollowsStmt2Wild);
const std::shared_ptr<Clause> kParentTPtrStmt2StmtName =
    std::make_shared<Parent>(clause::kParentTStmt2StmtName);
const std::shared_ptr<Clause> kModifiesPtrStmtNo2VarVarName =
    std::make_shared<Modifies>(clause::kModifiesStmtNo2VarVarName);
const std::shared_ptr<Clause> kUsesPtrStmtStmtNameWild =
    std::make_shared<Uses>(clause::kUsesStmtStmtNameWild);
const std::shared_ptr<Clause> kUsesPtrIdXIdY =
    std::make_shared<Uses>(clause::kUsesIdXIdY);
const std::shared_ptr<Clause> kAffectsPtrStmt2Wild =
    std::make_shared<Affects>(clause::kAffectsStmt2Wild);
const std::shared_ptr<Clause> kAffectsTPtrStmt2AssignName =
    std::make_shared<Affects>(clause::kAffectsTStmt2AssignName);
const std::shared_ptr<Clause> kAffectsBipPtrStmt3Wild =
    std::make_shared<Affects>(clause::kAffectsBipStmt3Wild);
const std::shared_ptr<Clause> kAffectsBipTPtrStmt2AssignName =
    std::make_shared<Affects>(clause::kAffectsBipTStmt2AssignName);
const std::shared_ptr<Clause> kCallsPtrWildProcProcName =
    std::make_shared<Calls>(clause::kCallsWildProcProcName);
const std::shared_ptr<Clause> kCallsTPtrIdXWild =
    std::make_shared<Calls>(clause::kCallsTIdXWild);
const std::shared_ptr<Clause> kNextTPtrStmt3Wild =
    std::make_shared<Next>(clause::kNextTStmt3Wild);
const std::shared_ptr<Clause> kNextBipTPtrStmt2Wild =
    std::make_shared<Next>(clause::kNextBipTStmt2Wild);
const std::shared_ptr<Clause> kNextBipPtrStmt2StmtName =
    std::make_shared<Next>(clause::kNextBipStmt2StmtName);
const std::shared_ptr<Clause> kPatternPtrTypeAssignVarWildExprPartialSpec1 =
    std::make_shared<Pattern>(
        clause::kPatternTypeAssignVarWildExprPartialSpec1);
const std::shared_ptr<Clause> kPatternPtrTypeAssignVarVarNameExprWild =
    std::make_shared<Pattern>(clause::kPatternTypeAssignVarVarNameExprWild);
const std::shared_ptr<Clause> kPatternPtrTypeAssignVarXExprSpec1 =
    std::make_shared<Pattern>(clause::kPatternTypeAssignVarXExprSpec1);
const std::shared_ptr<Clause> kPatternPtrTypeWhileVarWildExprWild =
    std::make_shared<Pattern>(clause::kPatternTypeWhileVarWildExprWild);
const std::shared_ptr<Clause> kPatternPtrTypeWhileVarVarNameExprWild =
    std::make_shared<Pattern>(clause::kPatternTypeWhileVarVarNameExprWild);
const std::shared_ptr<Clause> kPatternPtrTypeWhileVarXExprWild =
    std::make_shared<Pattern>(clause::kPatternTypeWhileVarXExprWild);
const std::shared_ptr<Clause> kPatternPtrTypeIfVarWildExprWild =
    std::make_shared<Pattern>(clause::kPatternTypeIfVarWildExprWild);
const std::shared_ptr<Clause> kPatternPtrTypeIfVarVarNameExprWild =
    std::make_shared<Pattern>(clause::kPatternTypeIfVarVarNameExprWild);
const std::shared_ptr<Clause> kPatternPtrTypeIfVarXExprWild =
    std::make_shared<Pattern>(clause::kPatternTypeIfVarXExprWild);
const std::shared_ptr<Clause> kComparePtrStmtStmtNumStmtNameStmtNo2 =
    std::make_shared<Compare>(clause::kCompareStmtStmtNumStmtNameStmtNo2);
const std::shared_ptr<Clause> kComparePtrStmtStmtNameConstNameConstValue =
    std::make_shared<Compare>(clause::kCompareStmtStmtNameConstNameConstValue);
const std::shared_ptr<Clause> kComparePtrIdXPrintPrintNameVarName =
    std::make_shared<Compare>(clause::kCompareIdXPrintPrintNameVarName);

// duplicate pointers of clauses
const std::shared_ptr<Clause> kFollowsPtrStmt2WildDuplicate =
    std::make_shared<Follows>(clause::kFollowsStmt2Wild);
const std::shared_ptr<Clause> kUsesPtrStmtStmtNameWildDuplicate =
    std::make_shared<Uses>(clause::kUsesStmtStmtNameWild);
const std::shared_ptr<Clause>
    kPatternPtrTypeAssignVarWildExprPartialSpec1Duplicate =
        std::make_shared<Pattern>(
            clause::kPatternTypeAssignVarWildExprPartialSpec1);

// vector of clauses
const std::vector<std::shared_ptr<Clause>> kClausesEmpty = {};
const std::vector<std::shared_ptr<Clause>> kClausesFollowsStmt2Wild = {
    kFollowsPtrStmt2Wild};
const std::vector<std::shared_ptr<Clause>> kClausesParentTStmt2StmtName = {
    kParentTPtrStmt2StmtName};
const std::vector<std::shared_ptr<Clause>> kClausesModifiesStmtNo2VarVarName = {
    kModifiesPtrStmtNo2VarVarName};
const std::vector<std::shared_ptr<Clause>> kClausesUsesStmtNameWild = {
    kUsesPtrStmtStmtNameWild};
const std::vector<std::shared_ptr<Clause>> kClausesUsesIdXIdY = {
    kUsesPtrIdXIdY};
const std::vector<std::shared_ptr<Clause>> kClausesAffectsStmt2Wild = {
    kAffectsPtrStmt2Wild};
const std::vector<std::shared_ptr<Clause>> kClausesAffectsTStmt2AssignName = {
    kAffectsTPtrStmt2AssignName};
const std::vector<std::shared_ptr<Clause>> kClausesAffectsBipStmt3Wild = {
    kAffectsBipPtrStmt3Wild};
const std::vector<std::shared_ptr<Clause>> kClausesAffectsBipTStmt2AssignName =
    {kAffectsBipTPtrStmt2AssignName};
const std::vector<std::shared_ptr<Clause>> kClausesCallsWildProcProcName = {
    kCallsPtrWildProcProcName};
const std::vector<std::shared_ptr<Clause>> kClausesCallsIdXWild = {
    kCallsTPtrIdXWild};
const std::vector<std::shared_ptr<Clause>> kClausesNextTStmt3Wild = {
    kNextTPtrStmt3Wild};
const std::vector<std::shared_ptr<Clause>> kClausesNextBipTStmt2Wild = {
    kNextBipTPtrStmt2Wild};
const std::vector<std::shared_ptr<Clause>> kClausesNextBipStmt2StmtName = {
    kNextBipPtrStmt2StmtName};
const std::vector<std::shared_ptr<Clause>>
    kClausesPatternTypeAssignVarWildExprPartialSpec1 = {
        kPatternPtrTypeAssignVarWildExprPartialSpec1};
const std::vector<std::shared_ptr<Clause>>
    kClausesPatternTypeAssignVarVarNameExprWild = {
        kPatternPtrTypeAssignVarVarNameExprWild};
const std::vector<std::shared_ptr<Clause>>
    kClausesPatternTypeAssignVarXExprSpec1 = {
        kPatternPtrTypeAssignVarXExprSpec1};
const std::vector<std::shared_ptr<Clause>>
    kClausesPatternTypeWhileVarWildExprWild = {
        kPatternPtrTypeWhileVarWildExprWild};
const std::vector<std::shared_ptr<Clause>>
    kClausesPatternTypeWhileVarVarNameExprWild = {
        kPatternPtrTypeWhileVarVarNameExprWild};
const std::vector<std::shared_ptr<Clause>>
    kClausesPatternTypeWhileVarXExprWild = {kPatternPtrTypeWhileVarXExprWild};
const std::vector<std::shared_ptr<Clause>>
    kClausesPatternTypeIfVarWildExprWild = {kPatternPtrTypeIfVarWildExprWild};
const std::vector<std::shared_ptr<Clause>>
    kClausesPatternTypeIfVarVarNameExprWild = {
        kPatternPtrTypeIfVarVarNameExprWild};
const std::vector<std::shared_ptr<Clause>> kClausesPatternTypeIfVarXExprWild = {
    kPatternPtrTypeIfVarXExprWild};
const std::vector<std::shared_ptr<Clause>>
    kClausesCompareStmtStmtNumStmtNameStmtNo2 = {
        kComparePtrStmtStmtNumStmtNameStmtNo2};
const std::vector<std::shared_ptr<Clause>>
    kClausesCompareStmtStmtNameConstNameConstValue = {
        kComparePtrStmtStmtNameConstNameConstValue};
const std::vector<std::shared_ptr<Clause>>
    kClausesCompareIdXPrintPrintNameVarName = {
        kComparePtrIdXPrintPrintNameVarName};
const std::vector<std::shared_ptr<Clause>>
    kClausesUsesStmtNameWildPatternTypeAssignVarWildExprPartialSpec1 = {
        kUsesPtrStmtStmtNameWild, kPatternPtrTypeAssignVarWildExprPartialSpec1};
const std::vector<std::shared_ptr<Clause>> kClausesMultiple = {
    kPatternPtrTypeAssignVarVarNameExprWild,
    kPatternPtrTypeAssignVarWildExprPartialSpec1,
    kUsesPtrStmtStmtNameWild,
    kParentTPtrStmt2StmtName,
    kPatternPtrTypeWhileVarVarNameExprWild,
    kPatternPtrTypeIfVarXExprWild,
    kComparePtrStmtStmtNameConstNameConstValue,
    kComparePtrIdXPrintPrintNameVarName,
    kAffectsPtrStmt2Wild,
    kAffectsTPtrStmt2AssignName,
    kCallsPtrWildProcProcName,
    kCallsTPtrIdXWild,
    kNextTPtrStmt3Wild};

// duplicate vector of clauses
const std::vector<std::shared_ptr<Clause>> kClausesEmptyDuplicate = {};
const std::vector<std::shared_ptr<Clause>> kClausesFollowsStmt2WildDuplicate = {
    kFollowsPtrStmt2WildDuplicate};
const std::vector<std::shared_ptr<Clause>> kClausesUsesStmtNameWildDuplicate = {
    kUsesPtrStmtStmtNameWildDuplicate};
const std::vector<std::shared_ptr<Clause>>
    kClausesPatternTypeAssignVarWildExprPartialSpec1Duplicate = {
        kPatternPtrTypeAssignVarWildExprPartialSpec1Duplicate};
const std::vector<std::shared_ptr<Clause>>
    kClausesUsesStmtNameWildPatternTypeAssignVarWildExprPartialSpec1Duplicate =
        {
            kPatternPtrTypeAssignVarWildExprPartialSpec1Duplicate,
            kUsesPtrStmtStmtNameWildDuplicate,
};

// query
const Query kQueryBoolEmpty = Query(kResultPtrBool, kClausesEmpty);
const Query kQueryStmtNameEmpty = Query(kResultPtrStmtStmtName, kClausesEmpty);
const Query kQueryStmtNameFollowsStmt2Wild =
    Query(kResultPtrStmtStmtName, kClausesFollowsStmt2Wild);
const Query kQueryStmtNameParentTStmt2StmtName =
    Query(kResultPtrStmtStmtName, kClausesParentTStmt2StmtName);
const Query kQueryStmtNameUsesStmtNameWild =
    Query(kResultPtrStmtStmtName, kClausesUsesStmtNameWild);
const Query kQueryStmtNameModifiesStmtNo2VarVarName =
    Query(kResultPtrStmtStmtName, kClausesModifiesStmtNo2VarVarName);
const Query kQueryStmtNameUsesIdXIdY =
    Query(kResultPtrStmtStmtName, kClausesUsesIdXIdY);
const Query kQueryConstNameUsesStmtNameWild =
    Query(kResultPtrConstConstName, kClausesUsesStmtNameWild);
const Query kQueryAssignNameAffectsStmt2Wild =
    Query(kResultPtrAssignAssignName, kClausesAffectsStmt2Wild);
const Query kQueryAssignNameAffectsTStmt2AssignName =
    Query(kResultPtrAssignAssignName, kClausesAffectsTStmt2AssignName);
const Query kQueryAssignNameAffectsBipStmt3Wild =
    Query(kResultPtrAssignAssignName, kClausesAffectsBipStmt3Wild);
const Query kQueryAssignNameAffectsBipTStmt2AssignName =
    Query(kResultPtrAssignAssignName, kClausesAffectsBipTStmt2AssignName);
const Query kQueryProcNameCallsWildProcProcName =
    Query(kResultPtrProcProcName, kClausesCallsWildProcProcName);
const Query kQueryProcNameCallsIdXWild =
    Query(kResultPtrProcProcName, kClausesCallsIdXWild);
const Query kQueryStmtNameNextTStmt3Wild =
    Query(kResultPtrStmtStmtName, kClausesNextTStmt3Wild);
const Query kQueryStmtNameNextBipTStmt2Wild =
    Query(kResultPtrStmtStmtName, kClausesNextBipTStmt2Wild);
const Query kQueryStmtNameNextBipStmt2StmtName =
    Query(kResultPtrStmtStmtName, kClausesNextBipStmt2StmtName);
const Query kQueryStmtNamePatternTypeAssignVarWildExprPartialSpec1 = Query(
    kResultPtrStmtStmtName, kClausesPatternTypeAssignVarWildExprPartialSpec1);
const Query kQueryStmtNamePatternTypeAssignVarVarNameExprWild =
    Query(kResultPtrStmtStmtName, kClausesPatternTypeAssignVarVarNameExprWild);
const Query kQueryStmtNamePatternTypeAssignVarXExprSpec1 =
    Query(kResultPtrStmtStmtName, kClausesPatternTypeAssignVarXExprSpec1);
const Query
    kQueryStmtNameUsesStmtNameWildPatternTypeAssignVarWildExprPartialSpec1 =
        Query(kResultPtrStmtStmtName,
              kClausesUsesStmtNameWildPatternTypeAssignVarWildExprPartialSpec1);
const Query kQueryConstNameEmpty =
    Query(kResultPtrConstConstName, kClausesEmpty);
const Query
    kQueryConstNameUsesStmtNameWildPatternTypeAssignVarWildExprPartialSpec1 =
        Query(kResultPtrConstConstName,
              kClausesUsesStmtNameWildPatternTypeAssignVarWildExprPartialSpec1);
const Query kQueryVarNamePatternTypeWhileVarWildExprWild =
    Query(kResultPtrVarVarName, kClausesPatternTypeWhileVarWildExprWild);
const Query kQueryVarNamePatternTypeWhileVarVarNameExprWild =
    Query(kResultPtrVarVarName, kClausesPatternTypeWhileVarVarNameExprWild);
const Query kQueryVarNamePatternTypeWhileVarXExprWild =
    Query(kResultPtrVarVarName, kClausesPatternTypeWhileVarXExprWild);
const Query kQueryVarNamePatternTypeIfVarWildExprWild =
    Query(kResultPtrVarVarName, kClausesPatternTypeIfVarWildExprWild);
const Query kQueryVarNamePatternTypeIfVarVarNameExprWild =
    Query(kResultPtrVarVarName, kClausesPatternTypeIfVarVarNameExprWild);
const Query kQueryVarNamePatternTypeIfVarXExprWild =
    Query(kResultPtrVarVarName, kClausesPatternTypeIfVarXExprWild);
const Query kQueryStmtNameCompareStmtStmtNumStmtNameStmtNo2 =
    Query(kResultPtrStmtStmtName, kClausesCompareStmtStmtNumStmtNameStmtNo2);
const Query kQueryStmtNameCompareStmtStmtNameConstNameConstValue = Query(
    kResultPtrStmtStmtName, kClausesCompareStmtStmtNameConstNameConstValue);
const Query kQueryStmtNameCompareIdXPrintPrintNameVarName =
    Query(kResultPtrStmtStmtName, kClausesCompareIdXPrintPrintNameVarName);
const Query kQueryMultipleClauses =
    Query(kResultPtrAssignAssignName, kClausesMultiple);
const Query kQueryMultipleDecls =
    Query(kResultPtrMultiple, kClausesFollowsStmt2Wild);

}  // namespace query

}  // namespace pql

}  // namespace test
