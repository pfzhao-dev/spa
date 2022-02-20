#include "xtr/UsesExtractor.h"

#include "pkb/PKBRoot.h"

namespace spa {
namespace xtr {
UsesExtractor::UsesExtractor() {}
void UsesExtractor::Extract(std::shared_ptr<ast::Program> program,
                            std::vector<std::string> order) {
  std::unordered_map<std::string, std::shared_ptr<ast::Procedure>> proc_map;
  for (auto p : program->GetProcList()) {
    proc_map[p->GetName()] = p;
  }

  for (auto s : order) {
    ExtractStatementList(s, proc_map[s]->GetStmtList());
  }
}

void UsesExtractor::ExtractStatementList(
    std::string proc_name, std::shared_ptr<ast::StatementList> stmt_list) {
  auto stmts = stmt_list->GetStatements();

  auto pkb_root = pkb::PKBRoot::GetInstance();

  for (auto stmt : stmts) {
    using stmt_type = ast::Statement::Type;
    auto type = stmt->GetType();
    if (type == stmt_type::kIf) {
      auto if_stmt = std::static_pointer_cast<ast::If>(stmt);
      ExtractExpression(proc_name, stmt->GetStmtNo(), if_stmt->GetCond());
      ExtractStatementList(proc_name, if_stmt->GetConsequent());
      ExtractStatementList(proc_name, if_stmt->GetAlternative());

    } else if (type == stmt_type::kWhile) {
      auto while_stmt = std::static_pointer_cast<ast::While>(stmt);
      ExtractExpression(proc_name, stmt->GetStmtNo(), while_stmt->GetCond());
      ExtractStatementList(proc_name, while_stmt->GetStatements());

    } else if (type == stmt_type::kAssign) {
      auto assign_stmt = std::static_pointer_cast<ast::Assign>(stmt);
      ExtractExpression(proc_name, stmt->GetStmtNo(),
                        assign_stmt->GetExpression());

    } else if (type == stmt_type::kPrint) {
      auto print_stmt = std::static_pointer_cast<ast::Print>(stmt);
      auto var = print_stmt->GetVariable();
      ExtractExpression(proc_name, stmt->GetStmtNo(), var);

    } else if (type == stmt_type::kCall) {
      auto call_stmt = std::static_pointer_cast<ast::Call>(stmt);

      auto uses_cache = pkb_root->GetUsesProcedureCache();
      auto call_proc = call_stmt->GetProcName();
      for (auto var : uses_cache.GetOriginalValues(call_proc)) {
        auto par_cache = pkb_root->GetParentCache(true);
        for (auto par : par_cache.GetReverseValues(stmt->GetStmtNo())) {
          pkb_root->AddUsesStatement(par, var);
        }
        pkb_root->AddUsesStatement(stmt->GetStmtNo(), var);
        pkb_root->AddUsesProcedure(proc_name, var);
      }
    }
  }
}

void UsesExtractor::ExtractExpression(std::string proc_name, uint32_t stmt_no,
                                      std::shared_ptr<ast::Expression> expr) {
  auto pkb_root = pkb::PKBRoot::GetInstance();

  using expr_type = ast::Expression::Type;
  auto type = expr->GetType();

  if (type == expr_type::kVariable) {
    auto var = std::static_pointer_cast<ast::Variable>(expr);
    auto par_cache = pkb_root->GetParentCache(true);
    for (auto par : par_cache.GetReverseValues(stmt_no)) {
      pkb_root->AddUsesStatement(par, var->GetName());
    }
    pkb_root->AddUsesStatement(stmt_no, var->GetName());
    pkb_root->AddUsesProcedure(proc_name, var->GetName());

  } else if (type == expr_type::kArithmetic) {
    auto arith = std::static_pointer_cast<ast::Arithmetic>(expr);
    ExtractExpression(proc_name, stmt_no, arith->GetLHS());
    ExtractExpression(proc_name, stmt_no, arith->GetRHS());

  } else if (type == expr_type::kRelational) {
    auto relational = std::static_pointer_cast<ast::Relational>(expr);
    ExtractExpression(proc_name, stmt_no, relational->GetLHS());
    ExtractExpression(proc_name, stmt_no, relational->GetRHS());

  } else if (type == expr_type::kLogical) {
    auto logical = std::static_pointer_cast<ast::Logical>(expr);
    using LogicalType = ast::Logical::OpType;
    auto log_type = logical->GetOpType();
    ExtractExpression(proc_name, stmt_no, logical->GetLHS());
    if (log_type != LogicalType::kNot) {
      ExtractExpression(proc_name, stmt_no, logical->GetRHS());
    }
  }
}

}  // namespace xtr
}  // namespace spa
