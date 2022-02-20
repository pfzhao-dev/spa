#include <iostream>

#include "ASTHelper.h"
#include "ast/Procedure.h"
#include "ast/parser/Parser.h"
#include "catch.hpp"
#include "pkb/PKBRoot.h"
#include "pql/Declaration.h"
#include "pql/Evaluator.h"
#include "pql/clause/Affects.h"
#include "pql/clause/Calls.h"
#include "pql/clause/Clause.h"
#include "pql/clause/Compare.h"
#include "pql/clause/Follows.h"
#include "pql/clause/Modifies.h"
#include "pql/clause/Next.h"
#include "pql/clause/Parent.h"
#include "pql/clause/Pattern.h"
#include "pql/clause/Uses.h"
#include "xtr/SourceExtractor.h"


// commented out tests
#if 0
TEST_CASE("Test parent relationship") {
  std::string procName = "Procedure_name";
  spa::ast::Procedure* p = new spa::ast::Procedure(procName);
  std::shared_ptr<spa::ast::Procedure> ptr(p);
  spa::PKBRootk::ClearRoot();
  // spa::pkb::Populator::GetInstance().AddProcedureToAST(ptr);
  std::shared_ptr<spa::ast::Read> ifread = buildRead("x", 3);
  std::shared_ptr<spa::ast::Read> elseread = buildRead("y", 4);
  std::vector<std::shared_ptr<spa::ast::Statement>> statements1{ifread};
  std::vector<std::shared_ptr<spa::ast::Statement>> statements2{elseread};
  std::shared_ptr<spa::ast::Variable> var = buildVariable("x");
  std::shared_ptr<spa::ast::Conditional> cond =
      buildRelational(spa::ast::Relational::OpType::kEq, var, var);
  std::shared_ptr<spa::ast::StatementList> ifStmtLst =
      buildStatementList({ifread});
  std::shared_ptr<spa::ast::StatementList> elseStmtLst =
      buildStatementList({elseread});
  std::shared_ptr<spa::ast::If> IfConditional =
      buildIf(cond, ifStmtLst, elseStmtLst, 6);

  // spa::pkb::Populator::GetInstance().AddStmtLstToAST(ifStmtLst, procName);
  // spa::pkb::Populator::GetInstance().AddStmtLstToAST(elseStmtLst,
  procName); ifStmtLst->AddStmt(ifread); elseStmtLst->AddStmt(elseread);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(ifread, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(elseread, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(IfConditional,
  procName);
  /*REQUIRE(spa::pql::Parent(IfConditional->GetStmtNo(),
                                                    ifread->GetStmtNo()));
  REQUIRE(spa::pql::Parent(IfConditional->GetStmtNo(),
                                                    elseread->GetStmtNo()));
  REQUIRE(!spa::pql::Parent(elseread->GetStmtNo(),
                                                     ifread->GetStmtNo()));*/
}

TEST_CASE("Test parent* relationship") {
  /*
  * procedure Main {
  *1   if (i == i) {
  *2      print i;
  *   } else {
   3     while(i == i) {
   4        read i;
        }
      }
    }
  */
  std::string procName = "Main";
  spa::ast::Procedure* p = new spa::ast::Procedure(procName);
  std::shared_ptr<spa::ast::Procedure> ptr(p);
  spa::pql::ClearRoot();
  // spa::pkb::Populator::GetInstance().AddProcedureToAST(ptr);

  std::shared_ptr<spa::ast::Variable> var = buildVariable("i");
  std::shared_ptr<spa::ast::Conditional> cond =
      buildRelational(spa::ast::Relational::OpType::kEq, var, var);
  std::shared_ptr<spa::ast::Print> ifRead = buildPrint("i", 2);
  std::shared_ptr<spa::ast::Read> innerRead = buildRead("i", 4);

  std::shared_ptr<spa::ast::While> innerWhile =
      buildWhile(cond, buildStatementList({innerRead}), 3);
  std::shared_ptr<spa::ast::If> ifStmt = buildIf(
      cond, buildStatementList({ifRead}), buildStatementList({innerWhile}),
      1);

  // spa::pkb::Populator::GetInstance().AddStmtToAST(ifRead, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(innerRead, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(innerWhile, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(ifStmt, procName);

  /*REQUIRE(spa::pql::Parent(ifStmt->GetStmtNo(),
                                                    ifRead->GetStmtNo(),
                                                    true));
  REQUIRE(spa::pql::Parent(
      ifStmt->GetStmtNo(), innerRead->GetStmtNo(), true));
  REQUIRE(spa::pql::Parent(
      ifStmt->GetStmtNo(), innerWhile->GetStmtNo(), true));
  REQUIRE(spa::pql::Parent(
      innerWhile->GetStmtNo(), innerRead->GetStmtNo(), true));
  REQUIRE(!spa::pql::Parent(
      innerWhile->GetStmtNo(), ifRead->GetStmtNo(), true));
  REQUIRE(!spa::pql::Parent(
      ifRead->GetStmtNo(), innerRead->GetStmtNo(), true));
  REQUIRE(!spa::pql::Parent(
      ifRead->GetStmtNo(), innerWhile->GetStmtNo(), true));
  REQUIRE(!spa::pql::Parent(
      innerRead->GetStmtNo(), ifRead->GetStmtNo(), true));
  REQUIRE(!spa::pql::Parent(
      innerRead->GetStmtNo(), ifStmt->GetStmtNo(), true));*/
}

TEST_CASE("Test follows relationship") {
  /*
       procedure computeCentroid {
01        count = 0;
02        cenX = 0;
03        cenY = 0;
04        call readPoint;
05        while ((x != 0) && (y != 0)) {
06            count = count + 1;
07            cenX = cenX + x;
08            cenY = cenY + y;
09            call readPoint;
      }
10        if (count == 0) then {
11            flag = 1;
      } else {
12            cenX = cenX / count;
13            cenY = cenY / count;
      }
14        normSq = cenX * cenX + cenY * cenY;
  }
   */

  std::string procName = "computeCentroid";
  spa::ast::Procedure* p = new spa::ast::Procedure(procName);
  std::shared_ptr<spa::ast::Procedure> ptr(p);
  spa::pql::ClearRoot();
  // spa::pkb::Populator::GetInstance().AddProcedureToAST(ptr);

  std::shared_ptr<spa::ast::Variable> var_count = buildVariable("count");
  std::shared_ptr<spa::ast::Constant> const_0 = buildConstant(0);
  std::shared_ptr<spa::ast::Assign> stmt1 = buildAssign(var_count, const_0,
  1);

  std::shared_ptr<spa::ast::Variable> var_cenX = buildVariable("cenX");
  std::shared_ptr<spa::ast::Assign> stmt2 = buildAssign(var_cenX, const_0,
  2);

  std::shared_ptr<spa::ast::Variable> var_cenY = buildVariable("cenY");
  std::shared_ptr<spa::ast::Assign> stmt3 = buildAssign(var_cenY, const_0,
  3);

  std::shared_ptr<spa::ast::Call> stmt4 = buildCall(procName, "readPoint",
  4);

  std::shared_ptr<spa::ast::Constant> const_1 = buildConstant(1);
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt6 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, var_count,
      const_1);
  std::shared_ptr<spa::ast::Assign> stmt6 =
      buildAssign(var_count, arith_stmt6, 6);

  std::shared_ptr<spa::ast::Variable> var_x = buildVariable("x");
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt7 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, var_cenX, var_x);
  std::shared_ptr<spa::ast::Assign> stmt7 =
      buildAssign(var_cenX, arith_stmt7, 7);

  std::shared_ptr<spa::ast::Variable> var_y = buildVariable("y");
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt8 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, var_cenY, var_y);
  std::shared_ptr<spa::ast::Assign> stmt8 =
      buildAssign(var_cenY, arith_stmt8, 8);

  std::shared_ptr<spa::ast::Call> stmt9 = buildCall(procName, "readPoint",
  9);

  std::shared_ptr<spa::ast::Conditional> cond_stmt5_l =
      buildRelational(spa::ast::Relational::OpType::kNeq, var_x, const_0);
  std::shared_ptr<spa::ast::Conditional> cond_stmt5_r =
      buildRelational(spa::ast::Relational::OpType::kNeq, var_y, const_0);
  std::shared_ptr<spa::ast::Logical> logic_stmt5 =
      buildLogical(spa::ast::Logical::OpType::kAnd, cond_stmt5_l,
      cond_stmt5_r);
  std::shared_ptr<spa::ast::While> stmt5 = buildWhile(
      logic_stmt5, buildStatementList({stmt6, stmt7, stmt8, stmt9}), 5);

  std::shared_ptr<spa::ast::Variable> var_flag = buildVariable("flag");
  std::shared_ptr<spa::ast::Assign> stmt11 = buildAssign(var_flag, const_1,
  11);

  std::shared_ptr<spa::ast::Arithmetic> arith_stmt12 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kDiv, var_cenX,
      var_count);
  std::shared_ptr<spa::ast::Assign> stmt12 =
      buildAssign(var_cenX, arith_stmt12, 12);

  std::shared_ptr<spa::ast::Arithmetic> arith_stmt13 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kDiv, var_cenY,
      var_count);
  std::shared_ptr<spa::ast::Assign> stmt13 =
      buildAssign(var_cenY, arith_stmt13, 13);

  std::shared_ptr<spa::ast::Conditional> cond_stmt10 =
      buildRelational(spa::ast::Relational::OpType::kEq, var_count, const_0);
  std::shared_ptr<spa::ast::If> stmt10 =
      buildIf(cond_stmt10, buildStatementList({stmt11}),
              buildStatementList({stmt12, stmt13}), 10);

  std::shared_ptr<spa::ast::Variable> var_normSq = buildVariable("normSq");
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt14_1 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kTimes, var_cenX,
      var_cenX);
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt14_2 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kTimes, var_cenY,
      var_cenY);
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt14 = buildArithmetic(
      spa::ast::Arithmetic::OpType::kTimes, arith_stmt14_1, arith_stmt14_2);
  std::shared_ptr<spa::ast::Assign> stmt14 =
      buildAssign(var_normSq, arith_stmt14, 14);

  std::shared_ptr<spa::ast::StatementList> s1 =
      buildStatementList({stmt1, stmt2, stmt3, stmt4, stmt5, stmt10,
      stmt14});
  std::shared_ptr<spa::ast::StatementList> s2 =
      buildStatementList({stmt6, stmt7, stmt8, stmt9});
  std::shared_ptr<spa::ast::StatementList> s3 = buildStatementList({stmt11});
  std::shared_ptr<spa::ast::StatementList> s4 =
      buildStatementList({stmt12, stmt13});

  stmt1->SetParent(s1);
  stmt2->SetParent(s1);
  stmt3->SetParent(s1);
  stmt4->SetParent(s1);
  stmt5->SetParent(s1);
  stmt6->SetParent(s2);
  stmt7->SetParent(s2);
  stmt8->SetParent(s2);
  stmt9->SetParent(s2);
  stmt10->SetParent(s1);
  stmt11->SetParent(s3);
  stmt12->SetParent(s4);
  stmt13->SetParent(s4);
  stmt14->SetParent(s1);

  // spa::pkb::Populator::GetInstance().AddStmtLstToAST(s1, procName);
  // spa::pkb::Populator::GetInstance().AddStmtLstToAST(s2, procName);
  // spa::pkb::Populator::GetInstance().AddStmtLstToAST(s3, procName);
  // spa::pkb::Populator::GetInstance().AddStmtLstToAST(s4, procName);

  // spa::pkb::Populator::GetInstance().AddStmtsToAST(
  //     {stmt1, stmt2, stmt3, stmt4, stmt5, stmt6, stmt7, stmt8, stmt9,
  stmt10,
  //      stmt11, stmt12, stmt13, stmt14});

  /*REQUIRE(spa::pql::Follows(stmt1->GetStmtNo(),
                                                     stmt2->GetStmtNo()));
  REQUIRE(spa::pql::Follows(stmt4->GetStmtNo(),
                                                     stmt5->GetStmtNo()));
  REQUIRE(!spa::pql::Follows(stmt5->GetStmtNo(),
                                                      stmt6->GetStmtNo()));
  REQUIRE(!spa::pql::Follows(stmt9->GetStmtNo(),
                                                      stmt10->GetStmtNo()));
  REQUIRE(spa::pql::Follows(stmt5->GetStmtNo(),
                                                     stmt10->GetStmtNo()));
  REQUIRE(!spa::pql::Follows(stmt11->GetStmtNo(),
                                                      stmt12->GetStmtNo()));
  REQUIRE(!spa::pql::Follows(stmt3->GetStmtNo(),
                                                      stmt10->GetStmtNo()));
  REQUIRE(!spa::pql::Follows(stmt12->GetStmtNo(),
                                                      stmt14->GetStmtNo()));
  REQUIRE(!spa::pql::Follows(stmt1->GetStmtNo(),
                                                      stmt14->GetStmtNo()));
  REQUIRE(!spa::pql::Follows(stmt1->GetStmtNo(),
                                                      stmt1->GetStmtNo()));*/
}

TEST_CASE("Test follows* relationship") {
  /*
  *example in lect slides, slide 32/57, basic SPA
       procedure computeCentroid {
01        count = 0;
02        cenX = 0;
03        cenY = 0;
04        call readPoint;
05        while ((x != 0) && (y != 0)) {
06            count = count + 1;
07            cenX = cenX + x;
08            cenY = cenY + y;
09            call readPoint;
      }
10        if (count == 0) then {
11            flag = 1;
      } else {
12            cenX = cenX / count;
13            cenY = cenY / count;
      }
14        normSq = cenX * cenX + cenY * cenY;
  }
   */

  std::string procName = "computeCentroid";
  spa::ast::Procedure* p = new spa::ast::Procedure(procName);
  std::shared_ptr<spa::ast::Procedure> ptr(p);
  spa::pql::ClearRoot();
  // spa::pkb::Populator::GetInstance().AddProcedureToAST(ptr);

  std::shared_ptr<spa::ast::Variable> var_count = buildVariable("count");
  std::shared_ptr<spa::ast::Constant> const_0 = buildConstant(0);
  std::shared_ptr<spa::ast::Assign> stmt1 = buildAssign(var_count, const_0,
  1);

  std::shared_ptr<spa::ast::Variable> var_cenX = buildVariable("cenX");
  std::shared_ptr<spa::ast::Assign> stmt2 = buildAssign(var_cenX, const_0,
  2);

  std::shared_ptr<spa::ast::Variable> var_cenY = buildVariable("cenY");
  std::shared_ptr<spa::ast::Assign> stmt3 = buildAssign(var_cenY, const_0,
  3);

  std::shared_ptr<spa::ast::Call> stmt4 = buildCall(procName, "readPoint",
  4);

  std::shared_ptr<spa::ast::Constant> const_1 = buildConstant(1);
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt6 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, var_count,
      const_1);
  std::shared_ptr<spa::ast::Assign> stmt6 =
      buildAssign(var_count, arith_stmt6, 6);

  std::shared_ptr<spa::ast::Variable> var_x = buildVariable("x");
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt7 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, var_cenX, var_x);
  std::shared_ptr<spa::ast::Assign> stmt7 =
      buildAssign(var_cenX, arith_stmt7, 7);

  std::shared_ptr<spa::ast::Variable> var_y = buildVariable("y");
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt8 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, var_cenY, var_y);
  std::shared_ptr<spa::ast::Assign> stmt8 =
      buildAssign(var_cenY, arith_stmt8, 8);

  std::shared_ptr<spa::ast::Call> stmt9 =
      buildCall("readPoint", std::string(), 9);

  std::shared_ptr<spa::ast::Conditional> cond_stmt5_l =
      buildRelational(spa::ast::Relational::OpType::kNeq, var_x, const_0);
  std::shared_ptr<spa::ast::Conditional> cond_stmt5_r =
      buildRelational(spa::ast::Relational::OpType::kNeq, var_y, const_0);
  std::shared_ptr<spa::ast::Logical> logic_stmt5 =
      buildLogical(spa::ast::Logical::OpType::kAnd, cond_stmt5_l,
      cond_stmt5_r);
  std::shared_ptr<spa::ast::While> stmt5 = buildWhile(
      logic_stmt5, buildStatementList({stmt6, stmt7, stmt8, stmt9}), 5);

  std::shared_ptr<spa::ast::Variable> var_flag = buildVariable("flag");
  std::shared_ptr<spa::ast::Assign> stmt11 = buildAssign(var_flag, const_1,
  11);

  std::shared_ptr<spa::ast::Arithmetic> arith_stmt12 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kDiv, var_cenX,
      var_count);
  std::shared_ptr<spa::ast::Assign> stmt12 =
      buildAssign(var_cenX, arith_stmt12, 12);

  std::shared_ptr<spa::ast::Arithmetic> arith_stmt13 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kDiv, var_cenY,
      var_count);
  std::shared_ptr<spa::ast::Assign> stmt13 =
      buildAssign(var_cenY, arith_stmt13, 13);

  std::shared_ptr<spa::ast::Conditional> cond_stmt10 =
      buildRelational(spa::ast::Relational::OpType::kEq, var_count, const_0);
  std::shared_ptr<spa::ast::If> stmt10 =
      buildIf(cond_stmt10, buildStatementList({stmt11}),
              buildStatementList({stmt12, stmt13}), 10);

  std::shared_ptr<spa::ast::Variable> var_normSq = buildVariable("normSq");
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt14_1 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kTimes, var_cenX,
      var_cenX);
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt14_2 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kTimes, var_cenY,
      var_cenY);
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt14 = buildArithmetic(
      spa::ast::Arithmetic::OpType::kTimes, arith_stmt14_1, arith_stmt14_2);
  std::shared_ptr<spa::ast::Assign> stmt14 =
      buildAssign(var_normSq, arith_stmt14, 14);

  std::shared_ptr<spa::ast::StatementList> s1 =
      buildStatementList({stmt1, stmt2, stmt3, stmt4, stmt5, stmt10,
      stmt14});
  std::shared_ptr<spa::ast::StatementList> s2 =
      buildStatementList({stmt6, stmt7, stmt8, stmt9});
  std::shared_ptr<spa::ast::StatementList> s3 = buildStatementList({stmt11});
  std::shared_ptr<spa::ast::StatementList> s4 =
      buildStatementList({stmt12, stmt13});

  stmt1->SetParent(s1);
  stmt2->SetParent(s1);
  stmt3->SetParent(s1);
  stmt4->SetParent(s1);
  stmt5->SetParent(s1);
  stmt6->SetParent(s2);
  stmt7->SetParent(s2);
  stmt8->SetParent(s2);
  stmt9->SetParent(s2);
  stmt10->SetParent(s1);
  stmt11->SetParent(s3);
  stmt12->SetParent(s4);
  stmt13->SetParent(s4);
  stmt14->SetParent(s1);

  // spa::pkb::Populator::GetInstance().AddStmtLstToAST(s1, procName);
  // spa::pkb::Populator::GetInstance().AddStmtLstToAST(s2, procName);
  // spa::pkb::Populator::GetInstance().AddStmtLstToAST(s3, procName);
  // spa::pkb::Populator::GetInstance().AddStmtLstToAST(s4, procName);

  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt1, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt2, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt3, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt4, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt5, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt6, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt7, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt8, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt9, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt10, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt11, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt12, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt13, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt14, procName);

  /*REQUIRE(spa::pql::Follows(stmt1->GetStmtNo(),
                                                     stmt2->GetStmtNo(),
                                                     true));
  REQUIRE(spa::pql::Follows(stmt4->GetStmtNo(),
                                                     stmt5->GetStmtNo(),
                                                     true));
  REQUIRE(!spa::pql::Follows(
      stmt5->GetStmtNo(), stmt6->GetStmtNo(), true));
  REQUIRE(!spa::pql::Follows(
      stmt9->GetStmtNo(), stmt10->GetStmtNo(), true));
  REQUIRE(spa::pql::Follows(
      stmt5->GetStmtNo(), stmt10->GetStmtNo(), true));
  REQUIRE(!spa::pql::Follows(
      stmt11->GetStmtNo(), stmt12->GetStmtNo(), true));
  REQUIRE(spa::pql::Follows(
      stmt3->GetStmtNo(), stmt10->GetStmtNo(), true));
  REQUIRE(!spa::pql::Follows(
      stmt12->GetStmtNo(), stmt14->GetStmtNo(), true));
  REQUIRE(spa::pql::Follows(
      stmt1->GetStmtNo(), stmt14->GetStmtNo(), true));
  REQUIRE(!spa::pql::Follows(
      stmt1->GetStmtNo(), stmt1->GetStmtNo(), true));*/
}

TEST_CASE("Test uses relationship") {
  /*
  example in lect slides, slide 32/57, basic SPA
       procedure computeCentroid {
01        count = 0;
02        cenX = 0;
03        cenY = 0;
04        call readPoint;
05        while ((x != 0) && (y != 0)) {
06            count = count + 1;
07            cenX = cenX + x;
08            cenY = cenY + y;
09            call readPoint;
      }
10        if (count == 0) then {
11            flag = 1;
      } else {
12            cenX = cenX / count;
13            cenY = cenY / count;
      }
14        normSq = cenX * cenX + cenY * cenY;
  }
   */

  std::string procName = "computeCentroid";
  spa::ast::Procedure* p = new spa::ast::Procedure(procName);
  std::shared_ptr<spa::ast::Procedure> ptr(p);
  spa::pql::ClearRoot();
  // spa::pkb::Populator::GetInstance().AddProcedureToAST(ptr);

  std::shared_ptr<spa::ast::Variable> var_count = buildVariable("count");
  std::shared_ptr<spa::ast::Constant> const_0 = buildConstant(0);
  std::shared_ptr<spa::ast::Assign> stmt1 = buildAssign(var_count, const_0,
  1);

  std::shared_ptr<spa::ast::Variable> var_cenX = buildVariable("cenX");
  std::shared_ptr<spa::ast::Assign> stmt2 = buildAssign(var_cenX, const_0,
  2);

  std::shared_ptr<spa::ast::Variable> var_cenY = buildVariable("cenY");
  std::shared_ptr<spa::ast::Assign> stmt3 = buildAssign(var_cenY, const_0,
  3);

  std::shared_ptr<spa::ast::Call> stmt4 =
      buildCall("readPoint", std::string(), 4);

  std::shared_ptr<spa::ast::Constant> const_1 = buildConstant(1);
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt6 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, var_count,
      const_1);
  std::shared_ptr<spa::ast::Assign> stmt6 =
      buildAssign(var_count, arith_stmt6, 6);

  std::shared_ptr<spa::ast::Variable> var_x = buildVariable("x");
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt7 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, var_cenX, var_x);
  std::shared_ptr<spa::ast::Assign> stmt7 =
      buildAssign(var_cenX, arith_stmt7, 7);

  std::shared_ptr<spa::ast::Variable> var_y = buildVariable("y");
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt8 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, var_cenY, var_y);
  std::shared_ptr<spa::ast::Assign> stmt8 =
      buildAssign(var_cenY, arith_stmt8, 8);

  std::shared_ptr<spa::ast::Call> stmt9 =
      buildCall("readPoint", std::string(), 9);

  std::shared_ptr<spa::ast::Conditional> cond_stmt5_l =
      buildRelational(spa::ast::Relational::OpType::kNeq, var_x, const_0);
  std::shared_ptr<spa::ast::Conditional> cond_stmt5_r =
      buildRelational(spa::ast::Relational::OpType::kNeq, var_y, const_0);
  std::shared_ptr<spa::ast::Logical> logic_stmt5 =
      buildLogical(spa::ast::Logical::OpType::kAnd, cond_stmt5_l,
      cond_stmt5_r);
  std::shared_ptr<spa::ast::While> stmt5 = buildWhile(
      logic_stmt5, buildStatementList({stmt6, stmt7, stmt8, stmt9}), 5);

  std::shared_ptr<spa::ast::Variable> var_flag = buildVariable("flag");
  std::shared_ptr<spa::ast::Assign> stmt11 = buildAssign(var_flag, const_1,
  11);

  std::shared_ptr<spa::ast::Arithmetic> arith_stmt12 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kDiv, var_cenX,
      var_count);
  std::shared_ptr<spa::ast::Assign> stmt12 =
      buildAssign(var_cenX, arith_stmt12, 12);

  std::shared_ptr<spa::ast::Arithmetic> arith_stmt13 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kDiv, var_cenY,
      var_count);
  std::shared_ptr<spa::ast::Assign> stmt13 =
      buildAssign(var_cenY, arith_stmt13, 13);

  std::shared_ptr<spa::ast::Conditional> cond_stmt10 =
      buildRelational(spa::ast::Relational::OpType::kEq, var_count, const_0);
  std::shared_ptr<spa::ast::If> stmt10 =
      buildIf(cond_stmt10, buildStatementList({stmt11}),
              buildStatementList({stmt12, stmt13}), 10);

  std::shared_ptr<spa::ast::Variable> var_normSq = buildVariable("normSq");
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt14_1 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kTimes, var_cenX,
      var_cenX);
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt14_2 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kTimes, var_cenY,
      var_cenY);
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt14 = buildArithmetic(
      spa::ast::Arithmetic::OpType::kTimes, arith_stmt14_1, arith_stmt14_2);
  std::shared_ptr<spa::ast::Assign> stmt14 =
      buildAssign(var_normSq, arith_stmt14, 14);

  std::shared_ptr<spa::ast::StatementList> s1 =
      buildStatementList({stmt1, stmt2, stmt3, stmt4, stmt5, stmt10,
      stmt14});
  std::shared_ptr<spa::ast::StatementList> s2 =
      buildStatementList({stmt6, stmt7, stmt8, stmt9});
  std::shared_ptr<spa::ast::StatementList> s3 = buildStatementList({stmt11});
  std::shared_ptr<spa::ast::StatementList> s4 =
      buildStatementList({stmt12, stmt13});

  stmt1->SetParent(s1);
  stmt2->SetParent(s1);
  stmt3->SetParent(s1);
  stmt4->SetParent(s1);
  stmt5->SetParent(s1);
  stmt6->SetParent(s2);
  stmt7->SetParent(s2);
  stmt8->SetParent(s2);
  stmt9->SetParent(s2);
  stmt10->SetParent(s1);
  stmt11->SetParent(s3);
  stmt12->SetParent(s4);
  stmt13->SetParent(s4);
  stmt14->SetParent(s1);

  // spa::pkb::Populator::GetInstance().AddStmtLstToAST(s1, procName);
  // spa::pkb::Populator::GetInstance().AddStmtLstToAST(s2, procName);
  // spa::pkb::Populator::GetInstance().AddStmtLstToAST(s3, procName);
  // spa::pkb::Populator::GetInstance().AddStmtLstToAST(s4, procName);

  // spa::pkb::Populator::GetInstance().AddStmtsToAST(
  //     {stmt1, stmt2, stmt3, stmt4, stmt5, stmt6, stmt7, stmt8, stmt9,
  stmt10,
  //      stmt11, stmt12, stmt13, stmt14});
  /*
       procedure computeCentroid {
01        count = 0;
02        cenX = 0;
03        cenY = 0;
04        call readPoint;
05        while ((x != 0) && (y != 0)) {
06            count = count + 1;
07            cenX = cenX + x;
08            cenY = cenY + y;
09            call readPoint;
      }
10        if (count == 0) then {
11            flag = 1;
      } else {
12            cenX = cenX / count;
13            cenY = cenY / count;
      }
14        normSq = cenX * cenX + cenY * cenY;
  }
  */

  REQUIRE(!spa::pql::Uses(stmt1->GetStmtNo(), "count"));
  REQUIRE(spa::pql::Uses(stmt7->GetStmtNo(), "x"));
  REQUIRE(!spa::pql::Uses(stmt9->GetStmtNo(), "y"));
  REQUIRE(!spa::pql::Uses(stmt10->GetStmtNo(), "flag"));
  REQUIRE(spa::pql::Uses(stmt10->GetStmtNo(), "count"));
}

TEST_CASE("Test modifies relationship") {
  /*
  example in lect slides, slide 32/57, basic SPA
       procedure computeCentroid {
01        count = 0;
02        cenX = 0;
03        cenY = 0;
04        call readPoint;
05        while ((x != 0) && (y != 0)) {
06            count = count + 1;
07            cenX = cenX + x;
08            cenY = cenY + y;
09            call readPoint;
        }
10        if (count == 0) then {
11            flag = 1;
        } else {
12            cenX = cenX / count;
13            cenY = cenY / count;
        }
14        normSq = cenX * cenX + cenY * cenY;
    }
     */

  std::string procName = "computeCentroid";
  spa::ast::Procedure* p = new spa::ast::Procedure(procName);
  std::shared_ptr<spa::ast::Procedure> ptr(p);
  spa::pql::ClearRoot();
  // spa::pkb::Populator::GetInstance().AddProcedureToAST(ptr);

  std::shared_ptr<spa::ast::Variable> var_count = buildVariable("count");
  std::shared_ptr<spa::ast::Constant> const_0 = buildConstant(0);
  std::shared_ptr<spa::ast::Assign> stmt1 = buildAssign(var_count, const_0,
  1);

  std::shared_ptr<spa::ast::Variable> var_cenX = buildVariable("cenX");
  std::shared_ptr<spa::ast::Assign> stmt2 = buildAssign(var_cenX, const_0,
  2);

  std::shared_ptr<spa::ast::Variable> var_cenY = buildVariable("cenY");
  std::shared_ptr<spa::ast::Assign> stmt3 = buildAssign(var_cenY, const_0,
  3);

  std::shared_ptr<spa::ast::Call> stmt4 =
      buildCall("readPoint", std::string(), 4);

  std::shared_ptr<spa::ast::Constant> const_1 = buildConstant(1);
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt6 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, var_count,
      const_1);
  std::shared_ptr<spa::ast::Assign> stmt6 =
      buildAssign(var_count, arith_stmt6, 6);

  std::shared_ptr<spa::ast::Variable> var_x = buildVariable("x");
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt7 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, var_cenX, var_x);
  std::shared_ptr<spa::ast::Assign> stmt7 =
      buildAssign(var_cenX, arith_stmt7, 7);

  std::shared_ptr<spa::ast::Variable> var_y = buildVariable("y");
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt8 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, var_cenY, var_y);
  std::shared_ptr<spa::ast::Assign> stmt8 =
      buildAssign(var_cenY, arith_stmt8, 8);

  std::shared_ptr<spa::ast::Call> stmt9 =
      buildCall("readPoint", std::string(), 9);

  std::shared_ptr<spa::ast::Conditional> cond_stmt5_l =
      buildRelational(spa::ast::Relational::OpType::kNeq, var_x, const_0);
  std::shared_ptr<spa::ast::Conditional> cond_stmt5_r =
      buildRelational(spa::ast::Relational::OpType::kNeq, var_y, const_0);
  std::shared_ptr<spa::ast::Logical> logic_stmt5 =
      buildLogical(spa::ast::Logical::OpType::kAnd, cond_stmt5_l,
      cond_stmt5_r);
  std::shared_ptr<spa::ast::While> stmt5 = buildWhile(
      logic_stmt5, buildStatementList({stmt6, stmt7, stmt8, stmt9}), 5);

  std::shared_ptr<spa::ast::Variable> var_flag = buildVariable("flag");
  std::shared_ptr<spa::ast::Assign> stmt11 = buildAssign(var_flag, const_1,
  11);

  std::shared_ptr<spa::ast::Arithmetic> arith_stmt12 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kDiv, var_cenX,
      var_count);
  std::shared_ptr<spa::ast::Assign> stmt12 =
      buildAssign(var_cenX, arith_stmt12, 12);

  std::shared_ptr<spa::ast::Arithmetic> arith_stmt13 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kDiv, var_cenY,
      var_count);
  std::shared_ptr<spa::ast::Assign> stmt13 =
      buildAssign(var_cenY, arith_stmt13, 13);

  std::shared_ptr<spa::ast::Conditional> cond_stmt10 =
      buildRelational(spa::ast::Relational::OpType::kEq, var_count, const_0);
  std::shared_ptr<spa::ast::If> stmt10 =
      buildIf(cond_stmt10, buildStatementList({stmt11}),
              buildStatementList({stmt12, stmt13}), 10);

  std::shared_ptr<spa::ast::Variable> var_normSq = buildVariable("normSq");
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt14_1 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kTimes, var_cenX,
      var_cenX);
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt14_2 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kTimes, var_cenY,
      var_cenY);
  std::shared_ptr<spa::ast::Arithmetic> arith_stmt14 = buildArithmetic(
      spa::ast::Arithmetic::OpType::kTimes, arith_stmt14_1, arith_stmt14_2);
  std::shared_ptr<spa::ast::Assign> stmt14 =
      buildAssign(var_normSq, arith_stmt14, 14);

  std::shared_ptr<spa::ast::StatementList> s1 =
      buildStatementList({stmt1, stmt2, stmt3, stmt4, stmt5, stmt10,
      stmt14});
  std::shared_ptr<spa::ast::StatementList> s2 =
      buildStatementList({stmt6, stmt7, stmt8, stmt9});
  std::shared_ptr<spa::ast::StatementList> s3 = buildStatementList({stmt11});
  std::shared_ptr<spa::ast::StatementList> s4 =
      buildStatementList({stmt12, stmt13});

  stmt1->SetParent(s1);
  stmt2->SetParent(s1);
  stmt3->SetParent(s1);
  stmt4->SetParent(s1);
  stmt5->SetParent(s1);
  stmt6->SetParent(s2);
  stmt7->SetParent(s2);
  stmt8->SetParent(s2);
  stmt9->SetParent(s2);
  stmt10->SetParent(s1);
  stmt11->SetParent(s3);
  stmt12->SetParent(s4);
  stmt13->SetParent(s4);
  stmt14->SetParent(s1);

  // spa::pkb::Populator::GetInstance().AddStmtLstToAST(s1, procName);
  // spa::pkb::Populator::GetInstance().AddStmtLstToAST(s2, procName);
  // spa::pkb::Populator::GetInstance().AddStmtLstToAST(s3, procName);
  // spa::pkb::Populator::GetInstance().AddStmtLstToAST(s4, procName);

  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt1, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt2, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt3, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt4, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt5, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt6, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt7, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt8, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt9, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt10, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt11, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt12, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt13, procName);
  // spa::pkb::Populator::GetInstance().AddStmtToAST(stmt14, procName);

  /*
       procedure computeCentroid {
01        count = 0;
02        cenX = 0;
03        cenY = 0;
04        call readPoint;
05        while ((x != 0) && (y != 0)) {
06            count = count + 1;
07            cenX = cenX + x;
08            cenY = cenY + y;
09            call readPoint;
      }
10        if (count == 0) then {
11            flag = 1;
      } else {
12            cenX = cenX / count;
13            cenY = cenY / count;
      }
14        normSq = cenX * cenX + cenY * cenY;
  }
  */

  REQUIRE(!spa::pql::Modifies(stmt1->GetStmtNo(), "x"));
  REQUIRE(spa::pql::Modifies(stmt7->GetStmtNo(), "cenX"));

  // supposed to fail, this programme doesn't have readPoint method
  REQUIRE(!spa::pql::Modifies(stmt9->GetStmtNo(), "x"));

  REQUIRE(spa::pql::Modifies(stmt10->GetStmtNo(), "flag"));
  REQUIRE(spa::pql::Modifies(stmt10->GetStmtNo(), "cenX"));
  REQUIRE(spa::pql::Modifies(stmt10->GetStmtNo(), "cenY"));
  REQUIRE(!spa::pql::Modifies(stmt10->GetStmtNo(), "count"));
  REQUIRE(!spa::pql::Modifies(stmt5->GetStmtNo(), "flag"));

  // supposed to fail, no main in the programme
  REQUIRE(!spa::pql::Modifies("main", "y"));  // proc as argument
}

TEST_CASE("Test pattern matching for assign statements") {
  /*
procedure getUses {
1       print i;
2       read x;
3       print y;
4       read x;
5       y = y + 0;
6       if (x == 0) {
7           y = x + y;
8           while (y > 0) {
9               y = z + k;
10               while (z < 0) {
11                   z = z + x;
12                   x = x + 2 * y + z;"
             }
         }
       } else {
13           read x;
       }
   }
   procedure printThis {
14       read x;
15       print y;
  }
  */
  std::string procName1 = "getUses";
  std::string procName2 = "printThis";
  std::shared_ptr<spa::ast::Procedure> p1 =
      std::shared_ptr<spa::ast::Procedure>(new
      spa::ast::Procedure(procName1));
  std::shared_ptr<spa::ast::Procedure> p2 =
      std::shared_ptr<spa::ast::Procedure>(new
      spa::ast::Procedure(procName2));
  spa::pql::ClearRoot();
  // spa::pkb::Populator::GetInstance().AddProcedureToAST(p1);
  // spa::pkb::Populator::GetInstance().AddProcedureToAST(p2);
  std::shared_ptr<spa::ast::Constant> zero = buildConstant(0);
  std::shared_ptr<spa::ast::Variable> x = buildVariable("x");
  std::shared_ptr<spa::ast::Variable> y = buildVariable("y");
  std::shared_ptr<spa::ast::Variable> k = buildVariable("k");
  std::shared_ptr<spa::ast::Variable> z = buildVariable("z");

  std::shared_ptr<spa::ast::Arithmetic> ar1 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, y, 0);
  std::shared_ptr<spa::ast::Arithmetic> ar2 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, x, y);
  std::shared_ptr<spa::ast::Arithmetic> ar3 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, z, k);
  std::shared_ptr<spa::ast::Arithmetic> ar4 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, z, x);

  std::shared_ptr<spa::ast::Relational> outerCond =
      buildRelational(spa::ast::Relational::OpType::kEq, x, zero);
  std::shared_ptr<spa::ast::Relational> innerCond =
      buildRelational(spa::ast::Relational::OpType::kGt, y, zero);
  std::shared_ptr<spa::ast::Relational> innerWhileCond =
      buildRelational(spa::ast::Relational::OpType::kLt, z, zero);

  std::shared_ptr<spa::ast::Print> line1 = buildPrint("i", 1);
  std::shared_ptr<spa::ast::Read> line2 = buildRead("x", 2);
  std::shared_ptr<spa::ast::Print> line3 = buildPrint("y", 3);
  std::shared_ptr<spa::ast::Read> line4 = buildRead("x", 4);
  std::shared_ptr<spa::ast::Assign> line5 = buildAssign(y, ar1, 5);
  std::shared_ptr<spa::ast::Assign> line7 = buildAssign(y, ar2, 7);
  std::shared_ptr<spa::ast::Assign> line9 = buildAssign(y, ar3, 9);
  std::shared_ptr<spa::ast::Assign> line11 = buildAssign(z, ar4, 11);
  std::shared_ptr<spa::ast::Read> line13 = buildRead("x", 1);

  //"x = x + 2 * y + z"
  std::shared_ptr<spa::ast::Arithmetic> a1 = buildArithmetic(
      spa::ast::Arithmetic::OpType::kTimes, buildConstant(2), y);
  std::shared_ptr<spa::ast::Arithmetic> a2 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, x, a1);
  std::shared_ptr<spa::ast::Arithmetic> a3 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, a2, z);
  std::shared_ptr<spa::ast::Assign> line12 = buildAssign(x, a3, 12);

  std::shared_ptr<spa::ast::While> innerWhile =
      buildWhile(innerWhileCond, buildStatementList({line11, line12}), 10);
  std::shared_ptr<spa::ast::While> outerWhile =
      buildWhile(innerCond, buildStatementList({innerWhile, line9}), 8);
  std::shared_ptr<spa::ast::If> outerIf =
      buildIf(outerCond, buildStatementList({innerWhile, outerWhile, line5}),
              buildStatementList({line13}), 6);

  // spa::pkb::Populator::GetInstance().AddStmtsToAST(
  //     {line1, line2, line3, line4, line5, line7, line9, line11, line12,
  line13,
  //      innerWhile, outerWhile, outerIf});

  // REQUIRE(
  //     spa::pql::PatternExpr(12, spa::pql::ParseExpression("x + 2 * y +
  // z")));
  // REQUIRE(!(spa::pql::PatternExpr(12,
  spa::pql::ParseExpression("2*y+z"))));

  // REQUIRE(spa::pql::PatternExpr(5, spa::pql::ParseExpression("y"), true));
}

TEST_CASE("Test pattern matching for if/while statements") {
  /*
procedure getUses {
1       print i;
2       read x;
3       print y;
4       read x;
5       y = y + 0;
6       if (x == 0) {
7           y = x + y;
8           while (y > 0) {
9               y = z + k;
10               while (z < 0) {
11                   z = z + x;
12                   x = x + 2 * y + z;"
             }
         }
       } else {
13           read x;
       }
   }
   procedure printThis {
14       read x;
15       print y;
  }
  */
  std::string procName1 = "getUses";
  std::string procName2 = "printThis";
  std::shared_ptr<spa::ast::Procedure> p1 =
      std::shared_ptr<spa::ast::Procedure>(new
      spa::ast::Procedure(procName1));
  std::shared_ptr<spa::ast::Procedure> p2 =
      std::shared_ptr<spa::ast::Procedure>(new
      spa::ast::Procedure(procName2));
  spa::pql::ClearRoot();
  // spa::pkb::Populator::GetInstance().AddProcedureToAST(p1);
  // spa::pkb::Populator::GetInstance().AddProcedureToAST(p2);

  std::shared_ptr<spa::ast::Constant> zero = buildConstant(0);
  std::shared_ptr<spa::ast::Variable> x = buildVariable("x");
  std::shared_ptr<spa::ast::Variable> y = buildVariable("y");
  std::shared_ptr<spa::ast::Variable> k = buildVariable("k");
  std::shared_ptr<spa::ast::Variable> z = buildVariable("z");

  std::shared_ptr<spa::ast::Arithmetic> ar1 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, y, 0);
  std::shared_ptr<spa::ast::Arithmetic> ar2 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, x, y);
  std::shared_ptr<spa::ast::Arithmetic> ar3 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, z, k);
  std::shared_ptr<spa::ast::Arithmetic> ar4 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, z, x);

  std::shared_ptr<spa::ast::Relational> outerCond =
      buildRelational(spa::ast::Relational::OpType::kEq, x, zero);
  std::shared_ptr<spa::ast::Relational> innerCond =
      buildRelational(spa::ast::Relational::OpType::kGt, y, zero);
  std::shared_ptr<spa::ast::Relational> innerWhileCond =
      buildRelational(spa::ast::Relational::OpType::kLt, z, zero);

  std::shared_ptr<spa::ast::Print> line1 = buildPrint("i", 1);
  std::shared_ptr<spa::ast::Read> line2 = buildRead("x", 2);
  std::shared_ptr<spa::ast::Print> line3 = buildPrint("y", 3);
  std::shared_ptr<spa::ast::Read> line4 = buildRead("x", 4);
  std::shared_ptr<spa::ast::Assign> line5 = buildAssign(y, ar1, 5);
  std::shared_ptr<spa::ast::Assign> line7 = buildAssign(y, ar2, 7);
  std::shared_ptr<spa::ast::Assign> line9 = buildAssign(y, ar3, 9);
  std::shared_ptr<spa::ast::Assign> line11 = buildAssign(z, ar4, 11);
  std::shared_ptr<spa::ast::Read> line13 = buildRead("x", 1);

  //"x = x + 2 * y + z"
  std::shared_ptr<spa::ast::Arithmetic> a1 = buildArithmetic(
      spa::ast::Arithmetic::OpType::kTimes, buildConstant(2), y);
  std::shared_ptr<spa::ast::Arithmetic> a2 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, x, a1);
  std::shared_ptr<spa::ast::Arithmetic> a3 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, a2, z);
  std::shared_ptr<spa::ast::Assign> line12 = buildAssign(x, a3, 12);

  std::shared_ptr<spa::ast::While> innerWhile =
      buildWhile(innerWhileCond, buildStatementList({line11, line12}), 10);
  std::shared_ptr<spa::ast::While> outerWhile =
      buildWhile(innerCond, buildStatementList({innerWhile, line9}), 8);
  std::shared_ptr<spa::ast::If> outerIf =
      buildIf(outerCond, buildStatementList({innerWhile, outerWhile, line5}),
              buildStatementList({line13}), 6);

  // spa::pkb::Populator::GetInstance().AddStmtsToAST(
  //     {line1, line2, line3, line4, line5, line7, line9, line11, line12,
  line13,
  //      innerWhile, outerWhile, outerIf});
  // TODO: Pattern
  // REQUIRE(spa::pql::PatternVar(8, "y"));
  // REQUIRE(!spa::pql::PatternVar(8, "x"));
  // REQUIRE(spa::pql::PatternVar(6, "x"));
  // REQUIRE(!spa::pql::PatternVar(6, "z"));
}
#endif

TEST_CASE("Test integration between parser and spa::pkb") {
  std::string s =
      "procedure main{\
	x = x + y;\
	y = y + z;\
	print x;\
	read y;\
	if (z == 0) then            {\
		if ((y == 1) && (x != (y + z))) then     {\
			k = x + p;\
			while (((k != (p + y)) || (z == (k * k))) && (l == (p + p))) {\
				k = y * y;\
				read k;\
				print zz;\
				while ((k == (k)) && (pop != pop)) {\
					zz = pq * zk + kz - pz / pl;\
				}\
			}\
		}\
 else {\
			read k;\
			z = (z + x) * (x + y) / p;\
		}\
	}\
 else {\
		print z;\
		p = lp + pq;\
	}\
 print myName;\
}";
  spa::pkb::PKBRoot::GetInstance()->ClearRoot();
  spa::ast::Parser parser = spa::ast::Parser(s);
  auto prog = parser.Parse();

  spa::xtr::SourceExtractor extractor;
  extractor.Extract(prog);
  /*
  "procedure main{\
      1 x = x + y;\
      2 y = y + z;\
      3 print x;\
      4 read y;\
      5 if (z == 0) then            {\
      6	if ((y == 1) && (x != (y + z))) then     {\
      7		k = x + p;\
      8		while (((k != (p + y)) || (z == (k * k))) && (l == (p + p))) {\
      9			k = y * y;\
      10			read k;\
      11			print zz;\
      12			while ((k == (k)) && (pop != pop)) {\
      13				zz = pq * zk + kz - pz / pl;\
                              }\
                      }\
              }\
      else {\
      14		read k;\
      15		z = (z + x) * (x + y) / p;\
          }\
  }\
  else {\
      16	print z;\
      17	p = lp + pq;\
      }\
  18 print myName;\
*/
  REQUIRE(spa::pql::Uses("main", "x").T());
  REQUIRE(spa::pql::Uses("main", "y").T());
  REQUIRE(spa::pql::Uses("main", "k").T());
  REQUIRE(spa::pql::Uses("main", "z").T());
  REQUIRE(spa::pql::Uses("main", "zz").T());
  REQUIRE(spa::pql::Uses("main", "pq").T());
  REQUIRE(spa::pql::Uses("main", "zk").T());
  REQUIRE(spa::pql::Uses("main", "kz").T());
  REQUIRE(spa::pql::Uses("main", "pz").T());
  REQUIRE(spa::pql::Uses("main", "pl").T());
  REQUIRE(!spa::pql::Uses("main", "pp").T());
  REQUIRE(!spa::pql::Uses("pf", "pl").T());
  REQUIRE(!spa::pql::Uses("pl", "pl").T());
  REQUIRE(!spa::pql::Uses("z", "pl").T());

  REQUIRE(spa::pql::Modifies("main", "x").T());
  REQUIRE(spa::pql::Modifies("main", "y").T());
  REQUIRE(spa::pql::Modifies("main", "k").T());
  REQUIRE(spa::pql::Modifies("main", "z").T());

  REQUIRE(spa::pql::Parent(5, 13, true).T());
  REQUIRE(spa::pql::Parent(12, 13).T());
  REQUIRE(spa::pql::Parent(5, 16).T());
  REQUIRE(spa::pql::Parent(5, 16, true).T());
  REQUIRE(spa::pql::Parent(5, 15, true).T());
  REQUIRE(spa::pql::Parent(5, 17).T());
  REQUIRE(spa::pql::Parent(8, 12).T());
  REQUIRE(spa::pql::Parent(8, 13, true).T());

  REQUIRE(spa::pql::Follows(10, 11).T());
  REQUIRE(spa::pql::Follows(14, 15).T());
  REQUIRE(!spa::pql::Follows(1, 1).T());
  REQUIRE(spa::pql::Follows(1, 2).T());
  REQUIRE(spa::pql::Follows(2, 3).T());
  REQUIRE(spa::pql::Follows(4, 5).T());
  REQUIRE(spa::pql::Follows(5, 18).T());
  REQUIRE(spa::pql::Follows(5, 18, true).T());
  REQUIRE(!spa::pql::Follows(6, 7).T());
  REQUIRE(!spa::pql::Follows(13, 14).T());
  REQUIRE(!spa::pql::Follows(14, 16).T());
  REQUIRE(!spa::pql::Follows(16, 18).T());
  REQUIRE(spa::pql::Follows(16, 17).T());
  REQUIRE(spa::pql::Follows(16, 17, true).T());
  REQUIRE(spa::pql::Follows(1, 18, true).T());
  REQUIRE(!spa::pql::Follows(1, 17, true).T());
  REQUIRE(!spa::pql::Follows(1, 1, true).T());
  REQUIRE(spa::pql::Follows(1, 2, true).T());
  REQUIRE(spa::pql::Follows(1, 4, true).T());
  REQUIRE(spa::pql::Follows(1, 5, true).T());
  REQUIRE(!spa::pql::Follows(1, 6, true).T());
  REQUIRE(spa::pql::Follows(7, 8, true).T());
  REQUIRE(!spa::pql::Follows(8, 9, true).T());
  REQUIRE(spa::pql::Follows(7, 8).T());
  REQUIRE(!spa::pql::Follows(8, 9).T());

  REQUIRE(spa::pql::Uses(1, "x").T());  // x = x + y;
  REQUIRE(spa::pql::Uses(1, "y").T());
  REQUIRE(!spa::pql::Uses(1, "z").T());
  REQUIRE(!spa::pql::Modifies(1, "z").T());
  REQUIRE(spa::pql::Modifies(1, "x").T());
  REQUIRE(spa::pql::Uses(3, "x").T());       // print x;
  REQUIRE(!spa::pql::Modifies(3, "x").T());  // print x;
  REQUIRE(!spa::pql::Uses(4, "y").T());      // read y;
  REQUIRE(spa::pql::Modifies(4, "y").T());   // read y;
  REQUIRE(spa::pql::Uses(6, "x").T());  // if ((y == 1) && (x != (y + z))) then
  REQUIRE(spa::pql::Uses(6, "y").T());
  REQUIRE(spa::pql::Uses(6, "z").T());
  REQUIRE(!spa::pql::Modifies(6, "x").T());
  REQUIRE(!spa::pql::Modifies(6, "y").T());
  REQUIRE(spa::pql::Modifies(6, "z").T());   // 15: z = (z + x) * (x + y) / p;
  REQUIRE(spa::pql::Uses(5, "zz").T());      // 11 print zz;
  REQUIRE(spa::pql::Modifies(5, "zz").T());  // 13 zz = pq * zk + kz - pz / pl;
  REQUIRE(spa::pql::Uses(5, "l").T());
  REQUIRE(spa::pql::Uses(6, "l").T());
  REQUIRE(!spa::pql::Uses(7, "l").T());
  REQUIRE(spa::pql::Uses(8, "l").T());
  REQUIRE(!spa::pql::Uses(9, "l").T());
  REQUIRE(!spa::pql::Modifies(5, "l").T());
  REQUIRE(!spa::pql::Modifies(6, "l").T());
  REQUIRE(!spa::pql::Modifies(7, "l").T());
  REQUIRE(!spa::pql::Modifies(8, "l").T());
  REQUIRE(!spa::pql::Modifies(9, "l").T());

  REQUIRE(!spa::pql::Uses(4, "pq").T());
  REQUIRE(spa::pql::Uses(5, "pq").T());
  REQUIRE(spa::pql::Uses(6, "pq").T());
  REQUIRE(!spa::pql::Uses(7, "pq").T());
  REQUIRE(spa::pql::Uses(8, "pq").T());
  REQUIRE(!spa::pql::Uses(9, "pq").T());
  REQUIRE(!spa::pql::Uses(10, "pq").T());
  REQUIRE(!spa::pql::Uses(11, "pq").T());
  REQUIRE(spa::pql::Uses(12, "pq").T());
  REQUIRE(spa::pql::Uses(13, "pq").T());

  REQUIRE(!spa::pql::Modifies(4, "pq").T());
  REQUIRE(!spa::pql::Modifies(5, "pq").T());
  REQUIRE(!spa::pql::Modifies(6, "pq").T());
  REQUIRE(!spa::pql::Modifies(7, "pq").T());
  REQUIRE(!spa::pql::Modifies(8, "pq").T());
  REQUIRE(!spa::pql::Modifies(9, "pq").T());
  REQUIRE(!spa::pql::Modifies(10, "pq").T());
  REQUIRE(!spa::pql::Modifies(11, "pq").T());
  REQUIRE(!spa::pql::Modifies(12, "pq").T());
  REQUIRE(!spa::pql::Modifies(13, "pq").T());
  REQUIRE(!spa::pql::Modifies(14, "pq").T());
  REQUIRE(spa::pql::Uses(15, "z").T());

  REQUIRE(spa::pql::Uses(18, "myName").T());
  REQUIRE(!spa::pql::Modifies(18, "myName").T());

  REQUIRE(!spa::pql::Uses(10, "k").T());
  REQUIRE(spa::pql::Modifies(10, "k").T());
  REQUIRE(spa::pql::Uses(5, "k").T());
  REQUIRE(spa::pql::Uses(6, "k").T());
  REQUIRE(spa::pql::Modifies(5, "k").T());
  REQUIRE(spa::pql::Modifies(6, "k").T());
  REQUIRE(spa::pql::Uses(11, "zz").T());
  REQUIRE(spa::pql::Uses(5, "zz").T());
  REQUIRE(spa::pql::Uses(6, "zz").T());
  REQUIRE(spa::pql::Uses(8, "zz").T());
  REQUIRE(!spa::pql::Modifies(11, "zz").T());
  REQUIRE(spa::pql::Modifies(5, "zz").T());
  REQUIRE(spa::pql::Modifies(6, "zz").T());
  REQUIRE(!spa::pql::Modifies(7, "zz").T());
  REQUIRE(spa::pql::Modifies(8, "zz").T());

  // REQUIRE(spa::pql::Pattern(13, spa::pql::ParseExpression("   pz   /   pl"),
  //                               true)
  //             .T());
  // REQUIRE(spa::pql::Pattern(
  //             13, spa::pql::ParseExpression("  pq    *     zk  "), true)
  //             .T());
  // REQUIRE(
  //     spa::pql::Pattern(
  //         13, spa::pql::ParseExpression("   pq    *     zk +     kz  "),
  //         true) .T());
  // REQUIRE(
  //     spa::pql::Patternpr(17, spa::pql::ParseExpression("lp"), true).T());
  // REQUIRE(spa::pql::PatternExpr(13, spa::pql::ParseExpression("pz/pl"), true)
  //             .T());

  REQUIRE(spa::pql::Evaluator::GetInstance()
              .GetStatements(spa::pql::Declaration::Type::kStmt)
              .size() == 18);
  REQUIRE(spa::pql::Evaluator::GetInstance()
              .GetStatements(spa::pql::Declaration::Type::kWhile)
              .size() == 2);
  REQUIRE(spa::pql::Evaluator::GetInstance()
              .GetStatements(spa::pql::Declaration::Type::kRead)
              .size() == 3);
  REQUIRE(spa::pql::Evaluator::GetInstance()
              .GetStatements(spa::pql::Declaration::Type::kAssign)
              .size() == 7);
}

TEST_CASE("Another integration test") {
  std::string s =
      "\
        procedure main{\
          flag = 0;\
          call computeCentroid;\
          call printResults;\
    }\
       procedure readPoint{\
            read x;\
            read y;\
   }\
       procedure printResults{\
            print flag;\
            print cenX;\
            print cenY;\
            print normSq;\
    }\
        procedure computeCentroid{\
            count = 0;\
            cenX = 0;\
            cenY = 0;\
            call readPoint;\
            while ((x != 0) && (y != 0)) {\
                count = count + 1;\
                cenX = cenX + x;\
                cenY = cenY + y;\
                call readPoint;\
            }\
            if (count == 0) then {\
                flag = 1;\
            }\
     else {\
      cenX = cenX / count;\
      cenY = cenY / count;\
  }\
  normSq = cenX * cenX + cenY * cenY;\
    }\
    ";
  spa::pkb::PKBRoot::GetInstance()->ClearRoot();
  spa::ast::Parser parser = spa::ast::Parser(s);
  auto prog = parser.Parse();

  spa::xtr::SourceExtractor extractor;
  extractor.Extract(prog);

  // TODO: For pattern testing, we need to check both the variable and expr
  //       directly.
  // REQUIRE(!spa::pql::PatternExpr(
  //     23, spa::pql::ParseExpression("cenX+cenY*cenY"), true));
  // REQUIRE(spa::pql::PatternExpr(23, spa::pql::ParseExpression("cenX *
  // cenX"),
  //                                 true));
  // REQUIRE(spa::pql::PatternExpr(
  //     23, spa::pql::ParseExpression("cenX * cenX + cenY * cenY")));

  REQUIRE(spa::pql::Evaluator::GetInstance().GetVariables().size() == 7);
  REQUIRE(spa::pql::Evaluator::GetInstance().GetConstants().size() == 2);
  // spa::pql::clearRoot();
}

TEST_CASE("Test Calls Relationship") {
  SECTION("Program 1") {
    std::string s =
        "\
        procedure main{\
          flag = 0;\
          call computeCentroid;\
          call printResults;\
    }\
       procedure readPoint{\
            read x;\
            read y;\
   }\
       procedure printResults{\
            print flag;\
            print cenX;\
            print cenY;\
            print normSq;\
    }\
        procedure computeCentroid{\
            count = 0;\
            cenX = 0;\
            cenY = 0;\
            call readPoint;\
            while ((x != 0) && (y != 0)) {\
                count = count + 1;\
                cenX = cenX + x;\
                cenY = cenY + y;\
                call readPoint;\
            }\
            if (count == 0) then {\
                flag = 1;\
            }\
     else {\
      cenX = cenX / count;\
      cenY = cenY / count;\
  }\
  normSq = cenX * cenX + cenY * cenY;\
    }\
        ";
    spa::pkb::PKBRoot::GetInstance()->ClearRoot();
    auto prog = spa::ast::Parser(s).Parse();

    spa::xtr::SourceExtractor extractor;
    extractor.Extract(prog);

    REQUIRE(spa::pql::Follows(1, 3, true).T());
    REQUIRE(spa::pql::Follows(16, 18, true).T());
    REQUIRE(spa::pql::Evaluator::GetInstance()
                .GetStatements(spa::pql::Declaration::Type::kAssign)
                .size() == 11);
    REQUIRE(spa::pql::Evaluator::GetInstance()
                .GetStatements(spa::pql::Declaration::Type::kCall)
                .size() == 4);
    REQUIRE(spa::pql::Uses("computeCentroid", "count").T());
    REQUIRE(!spa::pql::Uses(10, "x").T());
    REQUIRE(!spa::pql::Uses(5, "x").T());
    REQUIRE(!spa::pql::Uses(4, "y").T());
    REQUIRE(!spa::pql::Uses(3, "x").T());
    REQUIRE(!spa::pql::Modifies(1, "cenX").T());
    REQUIRE(!spa::pql::Uses(1, "cenX").T());
    REQUIRE(!spa::pql::Uses(1, "flag").T());
    REQUIRE(spa::pql::Uses(2, "count").T());
    REQUIRE(spa::pql::Uses(3, "flag").T());
    REQUIRE(spa::pql::Uses(3, "cenX").T());
    REQUIRE(spa::pql::Uses(3, "cenY").T());
    REQUIRE(spa::pql::Uses(3, "normSq").T());
    REQUIRE(spa::pql::Uses("main", "flag").T());
    REQUIRE(spa::pql::Uses("main", "cenX").T());
    REQUIRE(spa::pql::Uses("main", "cenY").T());
    REQUIRE(spa::pql::Uses("main", "normSq").T());

    REQUIRE(spa::pql::Uses(2, "x").T());
    REQUIRE(spa::pql::Uses(2, "y").T());
    REQUIRE(spa::pql::Uses(14, "x").T());
    REQUIRE(spa::pql::Uses(14, "y").T());
    REQUIRE(spa::pql::Uses(14, "count").T());
    REQUIRE(spa::pql::Uses(14, "cenX").T());
    REQUIRE(spa::pql::Uses(14, "cenY").T());

    REQUIRE(spa::pql::Uses("computeCentroid", "count").T());
    REQUIRE(spa::pql::Uses("computeCentroid", "cenX").T());
    REQUIRE(spa::pql::Uses("computeCentroid", "cenY").T());
    REQUIRE(spa::pql::Uses("computeCentroid", "x").T());
    REQUIRE(spa::pql::Uses("computeCentroid", "y").T());
    REQUIRE(spa::pql::Uses("main", "flag").T());
    REQUIRE(spa::pql::Uses("main", "cenX").T());
    REQUIRE(spa::pql::Uses("main", "cenY").T());
    REQUIRE(spa::pql::Uses("main", "normSq").T());
    REQUIRE(spa::pql::Uses("main", "x").T());
    REQUIRE(spa::pql::Uses("main", "y").T());
    REQUIRE(!spa::pql::Uses("main", "readPoint").T());

    REQUIRE(spa::pql::Modifies(1, "flag").T());
    REQUIRE(spa::pql::Modifies(2, "count").T());
    REQUIRE(!spa::pql::Modifies(3, "flag").T());
    REQUIRE(spa::pql::Modifies(2, "normSq").T());
    REQUIRE(spa::pql::Modifies(2, "x").T());
    REQUIRE(spa::pql::Modifies(2, "y").T());
    REQUIRE(spa::pql::Modifies("readPoint", "x").T());
    REQUIRE(spa::pql::Modifies("readPoint", "y").T());
    REQUIRE(spa::pql::Modifies(18, "x").T());
    REQUIRE(spa::pql::Modifies(18, "y").T());
    REQUIRE(spa::pql::Modifies(14, "x").T());
    REQUIRE(spa::pql::Modifies(14, "y").T());

    REQUIRE(spa::pql::Modifies("computeCentroid", "count").T());
    REQUIRE(spa::pql::Modifies("computeCentroid", "cenX").T());
    REQUIRE(spa::pql::Modifies("computeCentroid", "cenY").T());
    REQUIRE(spa::pql::Modifies("computeCentroid", "x").T());
    REQUIRE(spa::pql::Modifies("computeCentroid", "y").T());
    REQUIRE(spa::pql::Modifies("main", "flag").T());
    REQUIRE(spa::pql::Modifies("main", "count").T());
    REQUIRE(spa::pql::Modifies("main", "normSq").T());
    REQUIRE(!spa::pql::Modifies("main", "normsq").T());

    REQUIRE(spa::pql::Calls("main", "computeCentroid").T());
    REQUIRE(spa::pql::Calls("main", "printResults").T());
    REQUIRE(spa::pql::Calls("main", "computeCentroid").T());
    REQUIRE(spa::pql::Calls("computeCentroid", "readPoint").T());

    REQUIRE(!spa::pql::Calls("main", "readPoint").T());
    REQUIRE(!spa::pql::Calls("readPoint", "main").T());
    REQUIRE(!spa::pql::Calls("computeCentroid", "printResults").T());
    REQUIRE(!spa::pql::Calls("computeCentroid", "main").T());
    REQUIRE(!spa::pql::Calls("printResults", "computeCentroid").T());
    REQUIRE(!spa::pql::Calls("printResults", "main").T());
    REQUIRE(!spa::pql::Calls("printResults", "readPoint").T());
    REQUIRE(!spa::pql::Calls("main", "main").T());

    // non existing procedure
    REQUIRE(!spa::pql::Calls("printResults", "x").T());
    REQUIRE(!spa::pql::Calls("x", "main").T());

    REQUIRE(spa::pql::Calls("main", "computeCentroid", true).T());
    REQUIRE(spa::pql::Calls("main", "printResults", true).T());
    REQUIRE(spa::pql::Calls("main", "computeCentroid", true).T());
    REQUIRE(spa::pql::Calls("computeCentroid", "readPoint", true).T());
    REQUIRE(spa::pql::Calls("main", "readPoint", true).T());

    REQUIRE(!spa::pql::Calls("readPoint", "main", true).T());
    REQUIRE(!spa::pql::Calls("computeCentroid", "printResults", true).T());
    REQUIRE(!spa::pql::Calls("computeCentroid", "main", true).T());
    REQUIRE(!spa::pql::Calls("printResults", "computeCentroid", true).T());
    REQUIRE(!spa::pql::Calls("printResults", "main", true).T());
    REQUIRE(!spa::pql::Calls("printResults", "readPoint", true).T());
    REQUIRE(!spa::pql::Calls("main", "main", true).T());

    // non existing procedure
    REQUIRE(!spa::pql::Calls("printResults", "x", true).T());
    REQUIRE(!spa::pql::Calls("x", "main", true).T());
  }
  SECTION("Program 2") {
    std::string s =
        "procedure a {"
        "call b;"
        "call d;"
        "}"
        "procedure b {"
        "call d;"
        "}"
        "procedure c {"
        "call d;"
        "}"
        "procedure d {"
        "call f;"
        "}"
        "procedure f {"
        "print x;"
        "}"
        "procedure main {"
        "call main2;"
        "}"
        "procedure main2 {"
        "call main3;"
        "}"
        "procedure main3 {"
        "print x;"
        "}";
    spa::pkb::PKBRoot::GetInstance()->ClearRoot();
    spa::ast::Parser parser = spa::ast::Parser(s);
    auto prog = parser.Parse();

    spa::xtr::SourceExtractor extractor;
    extractor.Extract(prog);

    REQUIRE(spa::pql::Calls("a", "b").T());
    REQUIRE(spa::pql::Calls("a", "d").T());
    REQUIRE(spa::pql::Calls("b", "d").T());
    REQUIRE(spa::pql::Calls("c", "d").T());
    REQUIRE(spa::pql::Calls("d", "f").T());

    REQUIRE(!spa::pql::Calls("a", "f").T());

    REQUIRE(spa::pql::Calls("a", "b", true).T());
    REQUIRE(spa::pql::Calls("a", "d", true).T());
    REQUIRE(spa::pql::Calls("b", "d", true).T());
    REQUIRE(spa::pql::Calls("c", "d", true).T());
    REQUIRE(spa::pql::Calls("d", "f", true).T());
    REQUIRE(spa::pql::Calls("a", "f", true).T());

    REQUIRE(spa::pql::Calls("main", "main2", true).T());
    REQUIRE(spa::pql::Calls("main", "main3", true).T());

    REQUIRE(!spa::pql::Calls("a", "main", true).T());
    REQUIRE(!spa::pql::Calls("main2", "f", true).T());
  }
}

TEST_CASE("New Test") {
  SECTION("Program 1") {
    std::string s =
        "\
  procedure main {\
    if (i == 3) then {\
      while ((k == 3) || (pop != 3)) {\
        zz = pq * zk + kz - pz / pl;\
        call tree;\
      }\
      read xx;\
      xx = py + yz;\
    }\
    else {\
      a = a + b;\
    }\
    while (x == y) {\
      call prune;\
      if (y == (x % z)) then {\
        call ride;\
      }\
      else {\
        x = x % y;\
      }\
    }\
    call sign;\
    }\
      \
  procedure sign {\
    call prune;\
    read x;\
    print y;\
  }\
      \
  procedure tree {\
    if (x == y) then {\
      print zz;\
    }\
    else {\
      y = zz;\
    }\
  }\
      \
  procedure ride {\
    while (x == (y % (x + 4))) {\
      call tree;\
    }\
    x = x + y + z * z - 1;\
  }\
  \
  procedure prune {\
    print pr;\
    read x;\
    x = kx + zy;\
  }\
        ";
    spa::pkb::PKBRoot::GetInstance()->ClearRoot();
    spa::ast::Parser parser = spa::ast::Parser(s);
    auto prog = parser.Parse();

    spa::xtr::SourceExtractor extractor;
    extractor.Extract(prog);

    REQUIRE(spa::pql::Uses(4, "x").T());
    REQUIRE(spa::pql::Uses(4, "y").T());
    REQUIRE(spa::pql::Uses(4, "zz").T());
    REQUIRE(spa::pql::Uses(2, "x").T());
    REQUIRE(spa::pql::Uses(2, "y").T());
    REQUIRE(spa::pql::Uses(2, "zz").T());
    REQUIRE(spa::pql::Uses("main", "x").T());
    REQUIRE(spa::pql::Uses("main", "y").T());
    REQUIRE(spa::pql::Uses("main", "zz").T());
    REQUIRE(spa::pql::Uses(1, "x").T());
    REQUIRE(spa::pql::Uses(1, "y").T());
    REQUIRE(spa::pql::Uses(1, "zz").T());
    REQUIRE(spa::pql::Follows(1, 8).T());
    REQUIRE(!spa::pql::Follows(11, 18, true).T());
    // REQUIRE(spa::pql::Pattern(8, "x").T());
    // TODO: What does the following mean?
    // REQUIRE(spa::pql::GetStatementsByType(spa::ast::Statement::Type::kIf)[0]
    // ==
    //         1);
    REQUIRE(spa::pql::Evaluator::GetInstance()
                .GetStatements(spa::pql::Declaration::Type::kIf)
                .size() == 3);
    REQUIRE(spa::pql::Evaluator::GetInstance()
                .GetStatements(spa::pql::Declaration::Type::kAssign)
                .size() == 7);
    // REQUIRE(
    //     spa::pql::Pattern(3, "zz", spa::pql::Pattern::Expression("pq*zk",
    //     true))
    //         .T());
    /*REQUIRE(spa::pql::PatternExpr(
        22, spa::pql::ParseExpression("z*z-1"),
        true));*/
    // REQUIRE(!spa::pql::Pattern(3, "zz",
    //                            spa::pql::Pattern::Expression("kz-pz", true))
    //              .T());
    REQUIRE(spa::pql::Follows(9, 10, true).T());
    // REQUIRE(spa::pql::Pattern(
    //     22, spa::pql::ParseExpression("x + y + z*z-1"), true));
    REQUIRE(spa::pql::Modifies(22, "x").T());
    // REQUIRE(spa::pql::Pattern(17, "y").T());
    REQUIRE(spa::pql::Modifies(17, "y").T());
  }
}

TEST_CASE("WIKI_Code_5") {
  std::string s =
      "      procedure First {\
      read x;\
  read z;\
  call Second;\
}\
\
procedure Second {\
  x = 0;\
  i = 5;\
  while (i != 0) {\
    x = x + 2 * y;\
    call Third;\
    i = i - 1;\
  }\
  if (x == 1) then {\
      x = x + 1;\
    }\
  else {\
    z = 1;\
  }\
  z = z + x + i;\
  y = z + 2;\
  x = x * y + z;\
}\
\
procedure Third {\
  z = 5;\
  v = z;\
  print v;\
}\
";
  spa::pkb::PKBRoot::GetInstance()->ClearRoot();
  auto prog = spa::ast::Parser(s).Parse();

  spa::xtr::SourceExtractor extractor;
  extractor.Extract(prog);

  REQUIRE(spa::pql::Evaluator::GetInstance()
              .GetStatements(spa::pql::Declaration::Type::kRead)
              .size() == 2);
  REQUIRE(spa::pql::Evaluator::GetInstance()
              .GetStatements(spa::pql::Declaration::Type::kCall)
              .size() == 2);
  // REQUIRE(spa::pql::Follows(16, 18, true));
  // REQUIRE(!spa::pql::Uses(18, "z"));
  // REQUIRE(spa::pql::Uses(18, "v"));
  // REQUIRE(spa::pql::Uses(10, "z"));
  REQUIRE(spa::pql::Uses(6, "z").T());
  // REQUIRE(spa::pql::Follows(1, 1, true));
  // REQUIRE(spa::pql::Follows(1, 1, false));
  // REQUIRE(false);
}

TEST_CASE("Test Affects no conditionals") {
  SECTION("Positive") {
    SECTION("Basic 1") {
      std::string str =
          "procedure main {"
          "x = y + 2;"
          "z = x + 3;"
          "}";

      spa::pkb::PKBRoot::GetInstance()->ClearRoot();
      auto prog = spa::ast::Parser(str).Parse();

      spa::xtr::SourceExtractor extractor;
      extractor.Extract(prog);
      REQUIRE(spa::pql::Affects(1, 2, false).T());
    }

    SECTION("Basic 2") {
      std::string str =
          "procedure main {"
          "x = y + 2;"
          "z = x * 5;"
          "print x;"
          "print z;"
          "z = x + 3;"
          "}";
      spa::pkb::PKBRoot::GetInstance()->ClearRoot();
      auto prog = spa::ast::Parser(str).Parse();

      spa::xtr::SourceExtractor extractor;
      extractor.Extract(prog);
      REQUIRE(spa::pql::Affects(1, 5).T());
    }

    SECTION("Procedure") {
      std::string str =
          "procedure main {"
          "x = y + 2;"
          "call main2;"
          "z = x + 3;"
          "}"
          "procedure main2 {"
          "print z;"
          "z = z * 5 + x;"
          "}";
      spa::pkb::PKBRoot::GetInstance()->ClearRoot();
      auto prog = spa::ast::Parser(str).Parse();

      spa::xtr::SourceExtractor extractor;
      extractor.Extract(prog);

      REQUIRE(spa::pql::Affects(1, 3).T());
    }
  }

  SECTION("Negative") {
    SECTION("Not assign statement") {
      std::string str =
          "procedure main {"
          "x = y + 2;"
          "print y;"
          "}";
      spa::pkb::PKBRoot::GetInstance()->ClearRoot();
      auto prog = spa::ast::Parser(str).Parse();

      spa::xtr::SourceExtractor extractor;
      extractor.Extract(prog);
      REQUIRE(!spa::pql::Affects(1, 2).T());
    }
    SECTION("Basic 1") {
      std::string str =
          "procedure main {"
          "x = y + 2;"
          "x = 6;"
          "z = x + 3;"
          "}";
      spa::pkb::PKBRoot::GetInstance()->ClearRoot();
      auto prog = spa::ast::Parser(str).Parse();

      spa::xtr::SourceExtractor extractor;
      extractor.Extract(prog);
      REQUIRE(!spa::pql::Affects(1, 3).T());
    }

    SECTION("Statement does not exist") {
      std::string str =
          "procedure main {"
          "x = y + 2;"
          "z = x + 3;"
          "}";
      spa::pkb::PKBRoot::GetInstance()->ClearRoot();
      auto prog = spa::ast::Parser(str).Parse();

      spa::xtr::SourceExtractor extractor;
      extractor.Extract(prog);
      REQUIRE(!spa::pql::Affects(1, 3).T());
    }

    SECTION("Procedure") {
      std::string str =
          "procedure main {"
          "x = y + 2;"
          "call main2;"
          "z = x + 3;"
          "}"
          "procedure main2 {"
          "print z;"
          "x = z * 5 + x;"
          "}";
      spa::pkb::PKBRoot::GetInstance()->ClearRoot();
      auto prog = spa::ast::Parser(str).Parse();

      spa::xtr::SourceExtractor extractor;
      extractor.Extract(prog);
      REQUIRE(!spa::pql::Affects(1, 3).T());
    }
  }
}

TEST_CASE("Test Affects with conditionals") {
  SECTION("Positive") {
    SECTION("Basic If else") {
      std::string str =
          "procedure main {"
          "x = y + 2;"
          "if (x == 4) then {"
          "x = 3;"
          "} else {"
          "print x;"
          "y = x + 2;"
          "}"
          "x = x + 3;"
          "}";
      spa::pkb::PKBRoot::GetInstance()->ClearRoot();
      auto prog = spa::ast::Parser(str).Parse();

      spa::xtr::SourceExtractor extractor;
      extractor.Extract(prog);
      REQUIRE(spa::pql::Affects(1, 6).T());
      REQUIRE(spa::pql::Affects(3, 6).T());
      REQUIRE(!spa::pql::Affects(4, 6).T());
      REQUIRE(!spa::pql::Affects(1, 1).T());
      REQUIRE(!spa::pql::Affects(6, 6).T());
    }

    SECTION("Basic while") {
      std::string str =
          "procedure main {"
          "x = y + 2;"
          "while (x == 4) {"
          "x = 3;"
          "y = x + 2 + y;"
          "}"
          "y = x / y;"
          "}";

      spa::pkb::PKBRoot::GetInstance()->ClearRoot();
      auto prog = spa::ast::Parser(str).Parse();

      spa::xtr::SourceExtractor extractor;
      extractor.Extract(prog);

      REQUIRE(spa::pql::Affects(3, 4).T());
      REQUIRE(spa::pql::Affects(4, 5).T());
      REQUIRE(spa::pql::Affects(1, 5).T());
      REQUIRE(spa::pql::Affects(4, 4).T());
      REQUIRE(!spa::pql::Affects(1, 3).T());
      REQUIRE(!spa::pql::Affects(3, 3).T());
    }
  }
}

TEST_CASE("WIKI") {
  SECTION("Wiki test case") {
    std::string str =
        " procedure Second {\n"
        "        x = 0;\n"
        "        i = 5;\n"
        "        while (i!=0) {\n"
        "            x = x + 2*y;\n"
        "            call Third;\n"
        "            i = i - 1; }\n"
        "        if (x==1) then {\n"
        "            x = x+1; }\n"
        "          else {\n"
        "            z = 1; }\n"
        "        z = z + x + i;\n"
        "        y = z + 2;\n"
        "        x = x * y + z; }\n"
        "  \n"
        "      procedure Third {\n"
        "          z = 5;\n"
        "          v = z;\n"
        "          print v; }";

    spa::pkb::PKBRoot::GetInstance()->ClearRoot();
    auto prog = spa::ast::Parser(str).Parse();

    spa::xtr::SourceExtractor extractor;
    extractor.Extract(prog);

    REQUIRE(!spa::pql::Affects(2, 3, false).T());
    REQUIRE(spa::pql::Affects(2, 6, false).T());
    REQUIRE(spa::pql::Affects(4, 8, false).T());
    REQUIRE(spa::pql::Affects(4, 10, false).T());
    REQUIRE(!spa::pql::Affects(9, 6, false).T());
    REQUIRE(!spa::pql::Affects(9, 11, false).T());
    REQUIRE(spa::pql::Affects(6, 6, false).T());
  }
}

TEST_CASE("Affect star") {
  SECTION("Wiki test case") {
    std::string str =
        " procedure Second {\n"
        "        x = 0;\n"
        "        i = 5;\n"
        "        while (i!=0) {\n"
        "            x = x + 2*y;\n"
        "            call Third;\n"
        "            i = i - 1; }\n"
        "        if (x==1) then {\n"
        "            x = x+1; }\n"
        "          else {\n"
        "            z = 1; }\n"
        "        z = z + x + i;\n"
        "        y = z + 2;\n"
        "        x = x * y + z; }\n"
        "  \n"
        "      procedure Third {\n"
        "          z = 5;\n"
        "          v = z;\n"
        "          print v; }";

    spa::pkb::PKBRoot::GetInstance()->ClearRoot();
    auto prog = spa::ast::Parser(str).Parse();

    spa::xtr::SourceExtractor extractor;
    extractor.Extract(prog);

    REQUIRE(!spa::pql::Affects(2, 3, true).T());
    REQUIRE(spa::pql::Affects(2, 6, true).T());
    REQUIRE(spa::pql::Affects(4, 8, true).T());
    REQUIRE(spa::pql::Affects(4, 10, true).T());
    REQUIRE(!spa::pql::Affects(9, 6, true).T());
    REQUIRE(spa::pql::Affects(6, 6, true).T());

    REQUIRE(spa::pql::Affects(1, 4, true).T());
    REQUIRE(spa::pql::Affects(1, 10, true).T());
    REQUIRE(spa::pql::Affects(1, 11, true).T());
    REQUIRE(spa::pql::Affects(9, 12, true).T());
    REQUIRE(spa::pql::Affects(4, 10, true).T());
    REQUIRE(!spa::pql::Affects(4, 14, true).T());
    REQUIRE(spa::pql::Affects(1, 8, true).T());
    REQUIRE(!spa::pql::Affects(12, 8, true).T());
  }

  SECTION("system1") {
    std::string str =
        "procedure main {\n"
        "   a = b + c;\n"
        "   b = c;\n"
        "  read b;\n"
        "  d = a + b;\n"
        "  e = d + c;   \n"
        "   if (x == 3) then {\n"
        "\td = b + c;\n"
        "\twhile (x == 3) {\n"
        "\t\td = e + d;\n"
        "\t\ta = d + a;\n"
        "\t\td = d;\n"
        "  }\n"
        "e = 5;\t\n"
        "} else {\n"
        "\tcall main2;\n"
        "}\n"
        "a = d + 2;\n"
        "}"
        "procedure main2 {\n"
        "read e;\n"
        "}";
    spa::pkb::PKBRoot::GetInstance()->ClearRoot();
    auto prog = spa::ast::Parser(str).Parse();

    spa::xtr::SourceExtractor extractor;
    extractor.Extract(prog);

    REQUIRE(!spa::pql::Affects(1, 2, false).T());
    REQUIRE(spa::pql::Affects(1, 4, false).T());
    REQUIRE(spa::pql::Affects(5, 9, false).T());

    REQUIRE(spa::pql::Affects(9, 11, false).T());
    REQUIRE(spa::pql::Affects(9, 10, false).T());

    REQUIRE(spa::pql::Affects(11, 9, false).T());
    REQUIRE(spa::pql::Affects(10, 10, false).T());
    REQUIRE(spa::pql::Affects(4, 14, false).T());

    REQUIRE(!spa::pql::Affects(11, 13, false).T());
    REQUIRE(!spa::pql::Affects(2, 4, false).T());
    REQUIRE(!spa::pql::Affects(4, 9, false).T());
    REQUIRE(!spa::pql::Affects(5, 7, false).T());
    REQUIRE(!spa::pql::Affects(10, 11, false).T());
    REQUIRE(!spa::pql::Affects(5, 14, false).T());

    REQUIRE(spa::pql::Affects(5, 11, true).T());
    REQUIRE(spa::pql::Affects(1, 14, true).T());
    REQUIRE(spa::pql::Affects(9, 9, true).T());
    REQUIRE(spa::pql::Affects(11, 11, true).T());
    REQUIRE(spa::pql::Affects(5, 9, true).T());
    REQUIRE(spa::pql::Affects(1, 10, true).T());

    REQUIRE(!spa::pql::Affects(10, 11, true).T());
    REQUIRE(!spa::pql::Affects(5, 10, false).T());
    REQUIRE(!spa::pql::Affects(10, 14, false).T());
  }
}

TEST_CASE("Next Bip") {
  SECTION("Basic") {
    std::string str =
        "procedure p {\n"
        "   p = a;\n"
        "   call c;\n"
        "   call b;\n"
        "   while (a > b){\n"
        "       p = p + 1;\n"
        "   }\n"
        "   call c;\n"
        "}\n"
        "procedure c {\n"
        "   a = p;\n"
        "   while (i > 0) {\n"
        "        while (k == 0){\n"
        "              loop = 1;\n"
        "          }\n"
        "    }\n"
        "}\n"
        "procedure b {\n"
        "     c = x + 1;\n"
        "    x = x + 1;\n"
        "}";

    spa::pkb::PKBRoot::GetInstance()->ClearRoot();
    auto prog = spa::ast::Parser(str).Parse();

    spa::xtr::SourceExtractor extractor;
    extractor.Extract(prog);

    REQUIRE(spa::pql::Next(12, 4, false, true).T());
    REQUIRE(spa::pql::Next(8, 3, false, true).T());
  }

  SECTION("If statement") {
    SECTION("Basic") {
      std::string str =
          "procedure p {\n"
          "   p = a;\n"
          "   call c;\n"
          "  call b;\n"
          "  while (a > b){\n"
          "      p = p + 1;\n"
          "   }\n"
          "  call c;\n"
          "}\n"
          "procedure c {\n"
          "  a = p;\n"
          "  if (i > 0) then {\n"
          "       while (k == 0){\n"
          "              loop = 1;\n"
          "          }\n"
          "   } else {\n"
          "       x = l + p;\n"
          "    }\n"
          "}\n"
          "procedure b {\n"
          "   c = x + 1;\n"
          "   x = x + 1;\n"
          "}";

      spa::pkb::PKBRoot::GetInstance()->ClearRoot();
      auto prog = spa::ast::Parser(str).Parse();

      spa::xtr::SourceExtractor extractor;
      extractor.Extract(prog);
      // if
      REQUIRE(spa::pql::Next(9, 3, false, true).T());
      // last of else
      REQUIRE(spa::pql::Next(11, 3, false, true).T());
      REQUIRE(spa::pql::Next(12, 13, false, true).T());
    }
  }

  SECTION("Wiki") {
    std::string str =
        "procedure Bill {\n"
        "      x = 5;\n"
        "      call Mary;\n"
        "      y = x + 6;\n"
        "      call John;\n"
        "      z = x * y + 2; }\n"
        "\n"
        "procedure Mary {\n"
        "      y = x * 3;\n"
        "     call John;\n"
        "      z = x + y; }\n"
        "\n"
        "procedure John {\n"
        "      if (i > 0) then {\n"
        "              x = x + z;  \n"
        "        } else {\n"
        "              y = x * y; } }";

    spa::pkb::PKBRoot::GetInstance()->ClearRoot();
    auto prog = spa::ast::Parser(str).Parse();

    spa::xtr::SourceExtractor extractor;
    extractor.Extract(prog);
    REQUIRE(spa::pql::Next(1, 2, false, true).T());
    REQUIRE(!spa::pql::Next(2, 3, false, true).T());
    REQUIRE(spa::pql::Next(3, 4, false, true).T());
    REQUIRE(!spa::pql::Next(4, 5, false, true).T());
    REQUIRE(!spa::pql::Next(7, 8, false, true).T());
    REQUIRE(spa::pql::Next(8, 3, false, true).T());

    REQUIRE(spa::pql::Next(10, 8, false, true).T());
    REQUIRE(spa::pql::Next(11, 8, false, true).T());
    REQUIRE(spa::pql::Next(10, 5, false, true).T());
    REQUIRE(spa::pql::Next(11, 5, false, true).T());

    REQUIRE(spa::pql::Next(4, 9, false, true).T());
    REQUIRE(spa::pql::Next(7, 9, false, true).T());
    REQUIRE(!spa::pql::Next(7, 8, false, true).T());
  }

  SECTION("Last stmt call") {
    std::string str = "procedure p {\
        if (x == y) then {\
      if (z == x) then {\
          if (a == x) then {\
              call A;\
            }\
          else {\
            while (x == y) {\
              call A;\
            }\
            call B;\
          }\
        }\
      else {\
        call C;\
      }\
    }\
    else {\
      if (d == x) then {\
          call D;\
        }\
      else {\
        call E;\
      }\
    }\
  }\
\
  procedure A {\
    while (x == y) {\
      print z;\
    }\
    call B;\
  }\
\
  procedure B {\
    x = y;\
    if (x == y) then {\
        x = y;\
      }\
    else {\
      call C;\
    }\
    print p;\
  }\
\
  procedure C {\
    y = z;\
    call E;\
  }\
\
  procedure D { x = x * x;\
  }\
\
  procedure E { y = y * y; \
    }";

    spa::pkb::PKBRoot::GetInstance()->ClearRoot();
    auto prog = spa::ast::Parser(str).Parse();

    spa::xtr::SourceExtractor extractor;
    extractor.Extract(prog);
    REQUIRE(spa::pql::Next(19, 5, false, true).T());
    REQUIRE(spa::pql::Next(23, 19, false, true).T());
    REQUIRE(spa::pql::Next(21, 23, false, true).T());
    REQUIRE(!spa::pql::Next(19, 11, false, true).T());
    REQUIRE(!spa::pql::Next(23, 12, false, true).T());
    

    REQUIRE(spa::pql::Next(21, 20, true, true).T());
    REQUIRE(spa::pql::Next(19, 15, true, true).T());
    REQUIRE(spa::pql::Next(14, 12, true, true).T());
    REQUIRE(spa::pql::Next(23, 23, true, true).T());

    REQUIRE(!spa::pql::Next(22, 23, true, true).T());
    REQUIRE(!spa::pql::Next(10, 11, true, true).T());
  }
}

TEST_CASE("Affects Bip") {
  SECTION("Basic") {
    std::string str =
        "procedure p {\n"
        "   p = a;\n"
        "   call c;\n"
        "   call b;\n"
        "   while (a > b){\n"
        "       p = p + 1;\n"
        "   }\n"
        "   call c;\n"
        "}\n"
        "procedure c {\n"
        "   a = p;\n"
        "   while (i > 0) {\n"
        "        while (k == 0){\n"
        "              loop = 1;\n"
        "          }\n"
        "    }\n"
        "}\n"
        "procedure b {\n"
        "     c = x + 1;\n"
        "    x = x + 1;\n"
        "}";

    spa::pkb::PKBRoot::GetInstance()->ClearRoot();
    auto prog = spa::ast::Parser(str).Parse();

    spa::xtr::SourceExtractor extractor;
    extractor.Extract(prog);

    REQUIRE(spa::pql::Affects(1, 7, false, true).T());
    REQUIRE(spa::pql::Affects(5, 7, false, true).T());
  }

  SECTION("complex cfg") {
    std::string str = "\
        procedure main {\
        while (x < 0) {\
      a = b;\
      b = c;\
      c = a;\
    }\
    call readMe;\
  }\
\
  procedure readMe {\
    while (c > 10) {\
      z = c + x;\
      x = x + 1;\
      if (y == 0) then {\
          while (x < z) {\
            x = z + r;\
            read x;\
            call Two;\
          }\
        }\
      else {\
        call One;\
      }\
    }\
    y = x + z;\
    call One;\
  }\
\
  procedure One {\
    a = b * c + d;\
    while (c > 0) {\
      if (d > 0) then {\
          b = a - 1;\
        }\
      else {\
        c = b * d;\
      }\
      d = b + c;\
      c = d * a;\
      b = d - c;\
    }\
    call Two;\
  }\
\
  procedure Two {\
    s = s + x;\
    x = x + 1;\
    y = y + x;\
    print c;\
    x = x * x;\
  }\
  ";

    spa::pkb::PKBRoot::GetInstance()->ClearRoot();
    auto prog = spa::ast::Parser(str).Parse();

    spa::xtr::SourceExtractor extractor;
    extractor.Extract(prog);
    REQUIRE(spa::pql::Affects(8, 27, false, true).T());
    REQUIRE(spa::pql::Affects(15, 28, false, true).T());
    REQUIRE(spa::pql::Affects(30, 7, false, true).T());
    REQUIRE(!spa::pql::Affects(30, 11, false, true).T());
    REQUIRE(!spa::pql::Affects(15, 15, false, true).T());
    REQUIRE(!spa::pql::Affects(17, 2, false, true).T());

    REQUIRE(!spa::pql::Affects(7, 30, true, true).T());
    REQUIRE(spa::pql::Affects(30, 11, true, true).T());
    REQUIRE(!spa::pql::Affects(15, 15, true, true).T());
    REQUIRE(spa::pql::Affects(24, 15, true, true).T());
  }
}

TEST_CASE("Single_procedure_duplicable") { 
    std::string s =
      "procedure main00 {\
      while (x00 < 0) {\
    a00 = b00;\
    b00 = k00;\
    k00 = a00;\
  }\
  while (k00 > 10) {\
    z00 = k00 + x00;\
    x00 = x00 + 1;\
    if (y00 == 0) then {\
        while (x00 < z00) {\
          x00 = z00 + r00;\
          read x00;\
          x00 = z00 + r00;\
        }\
      }\
    else {\
      x00 = y00 + k00;\
      call main01;\
    }\
  }\
  y00 = x00 + z00;\
  s00 = s00 + x00;\
  x00 = x00 + 1;\
  y00 = y00 + x00;\
  print z00;\
  x00 = x00 * x00;\
  while (y00 == z00) {\
    if (xy00 == z00) then {\
        z00 = z00 + 1;\
        x00 = y00 + 2;\
      }\
    else {\
      while (z00 == (y00 + x00)) {\
        cr00 = cr00 + z00;\
      }\
    }\
  }\
  print x00;\
}\
\
procedure main01 { print x00; }\
";
  spa::pkb::PKBRoot::GetInstance()->ClearRoot();
  auto prog = spa::ast::Parser(s).Parse();

  spa::xtr::SourceExtractor extractor;
  extractor.Extract(prog);

  REQUIRE(spa::pql::Next(14, 12, true, false).T());
}