#include "xtr/ModifiesExtractor.h"

#include "pkb/PKBRoot.h"

namespace spa {
namespace xtr {
ModifiesExtractor::ModifiesExtractor() {}
void ModifiesExtractor::Extract(std::shared_ptr<ast::Program> program,
                                std::vector<std::string> order) {
  std::unordered_map<std::string, std::shared_ptr<ast::Procedure>> proc_map;
  for (auto p : program->GetProcList()) {
    proc_map[p->GetName()] = p;
  }

  for (auto s : order) {
    ExtractStatementList(s, proc_map[s]->GetStmtList());
  }
}

void ModifiesExtractor::ExtractStatementList(
    std::string proc_name, std::shared_ptr<ast::StatementList> stmt_list) {
  auto stmts = stmt_list->GetStatements();

  auto pkb_root = pkb::PKBRoot::GetInstance();

  for (auto stmt : stmts) {
    using stmt_type = ast::Statement::Type;
    auto type = stmt->GetType();
    if (type == stmt_type::kIf) {
      auto if_stmt = std::static_pointer_cast<ast::If>(stmt);
      ExtractStatementList(proc_name, if_stmt->GetConsequent());
      ExtractStatementList(proc_name, if_stmt->GetAlternative());
    } else if (type == stmt_type::kWhile) {
      auto while_stmt = std::static_pointer_cast<ast::While>(stmt);
      ExtractStatementList(proc_name, while_stmt->GetStatements());
    } else if (type == stmt_type::kAssign) {
      auto assign_stmt = std::static_pointer_cast<ast::Assign>(stmt);
      ExtractExpression(proc_name, stmt->GetStmtNo(),
                        assign_stmt->GetVariable());
    } else if (type == stmt_type::kRead) {
      auto read_stmt = std::static_pointer_cast<ast::Read>(stmt);
      auto var = read_stmt->GetVariable();
      ExtractExpression(proc_name, stmt->GetStmtNo(), var);
    } else if (type == stmt_type::kCall) {
      auto call_stmt = std::static_pointer_cast<ast::Call>(stmt);

      // TODO: double check this part
      auto modifies_cache = pkb_root->GetModifiesProcedureCache();
      auto call_proc = call_stmt->GetProcName();
      auto par_cache = pkb_root->GetParentCache(true);
      auto par_set = par_cache.GetReverseValues(stmt->GetStmtNo());

      // This will cost amortized O(N^2) for the whole computation.
      for (auto var : modifies_cache.GetOriginalValues(call_proc)) {
        for (auto par : par_set) {
          pkb_root->AddModifiesStatement(par, var);
        }
        pkb_root->AddModifiesStatement(stmt->GetStmtNo(), var);
        pkb_root->AddModifiesProcedure(proc_name, var);
      }
    }
  }
}

void ModifiesExtractor::ExtractExpression(std::string proc_name,
                                          uint32_t stmt_no,
                                          std::shared_ptr<ast::Variable> var) {
  auto pkb_root = pkb::PKBRoot::GetInstance();

  auto par_cache = pkb_root->GetParentCache(true);
  for (auto par : par_cache.GetReverseValues(stmt_no)) {
    pkb_root->AddModifiesStatement(par, var->GetName());
  }
  pkb_root->AddModifiesStatement(stmt_no, var->GetName());
  pkb_root->AddModifiesProcedure(proc_name, var->GetName());
}

}  // namespace xtr
}  // namespace spa
