#include "xtr/PatternExtractor.h"

#include "pkb/PKBRoot.h"

namespace spa {
namespace xtr {
PatternExtractor::PatternExtractor() {}
void PatternExtractor::Extract(std::shared_ptr<ast::Program> program) {
  for (auto p : program->GetProcList()) {
    ExtractStatementList(p->GetStmtList());
  }
}

void PatternExtractor::ExtractStatementList(
    std::shared_ptr<ast::StatementList> stmt_list) {
  auto stmts = stmt_list->GetStatements();

  auto pkb_root = pkb::PKBRoot::GetInstance();

  for (auto stmt : stmts) {
    using StmtType = ast::Statement::Type;
    auto type = stmt->GetType();
    if (type == StmtType::kIf) {
      auto if_stmt = std::static_pointer_cast<ast::If>(stmt);
      ExtractExpression(stmt->GetStmtNo(), if_stmt->GetCond(), type);
      ExtractStatementList(if_stmt->GetConsequent());
      ExtractStatementList(if_stmt->GetAlternative());
    } else if (type == StmtType::kWhile) {
      auto while_stmt = std::static_pointer_cast<ast::While>(stmt);
      ExtractExpression(stmt->GetStmtNo(), while_stmt->GetCond(), type);
      ExtractStatementList(while_stmt->GetStatements());
    } else if (type == StmtType::kAssign) {
      auto assign_stmt = std::static_pointer_cast<ast::Assign>(stmt);
      ExtractExpression(stmt->GetStmtNo(), assign_stmt->GetVariable(), type);
    }
  }
}

void PatternExtractor::ExtractExpression(uint32_t stmt_no,
                                         std::shared_ptr<ast::Expression> expr,
                                         ast::Statement::Type stmt_type) {
  auto pkb_root = pkb::PKBRoot::GetInstance();

  using ExprType = ast::Expression::Type;
  using StmtType = ast::Statement::Type;
  auto type = expr->GetType();

  if (type == ExprType::kVariable) {
    auto var = std::static_pointer_cast<ast::Variable>(expr);
    if (stmt_type == StmtType::kIf) {
      pkb_root->AddPatternIf(stmt_no, var->GetName());
    } else if (stmt_type == StmtType::kWhile) {
      pkb_root->AddPatternWhile(stmt_no, var->GetName());
    } else if (stmt_type == StmtType::kAssign) {
      pkb_root->AddPatternAssign(stmt_no, var->GetName());
    }
  } else if (type == ExprType::kArithmetic) {
    auto arith = std::static_pointer_cast<ast::Arithmetic>(expr);
    ExtractExpression(stmt_no, arith->GetLHS(), stmt_type);
    ExtractExpression(stmt_no, arith->GetRHS(), stmt_type);
  } else if (type == ExprType::kRelational) {
    auto relational = std::static_pointer_cast<ast::Relational>(expr);
    ExtractExpression(stmt_no, relational->GetLHS(), stmt_type);
    ExtractExpression(stmt_no, relational->GetRHS(), stmt_type);
  } else if (type == ExprType::kLogical) {
    auto logical = std::static_pointer_cast<ast::Logical>(expr);
    using LogicalType = ast::Logical::OpType;
    auto log_type = logical->GetOpType();
    ExtractExpression(stmt_no, logical->GetLHS(), stmt_type);
    if (log_type != LogicalType::kNot) {
      ExtractExpression(stmt_no, logical->GetRHS(), stmt_type);
    }
  }
}

}  // namespace xtr
}  // namespace spa
