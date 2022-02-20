#include "xtr/CallsExtractor.h"

#include "pkb/PKBRoot.h"
#include "xtr/ExtractException.h"

namespace spa {
namespace xtr {

void CallsExtractor::Extract(std::shared_ptr<ast::Program> program) {
  auto procs = program->GetProcList();
  for (auto p : procs) {
    ExtractStatementList(p->GetName(), p->GetStmtList());
  }

  for (auto p : procs) {
    std::unordered_set<std::string> visited;
    GenerateCallsT(p->GetName(), p->GetName(), visited);

    if (visited.count(p->GetName())) {
      throw ExtractException("Cyclic calls occured");
    }
  }
}

void CallsExtractor::ExtractStatementList(
    std::string caller, std::shared_ptr<ast::StatementList> stmt_list) {
  auto pkb_root = pkb::PKBRoot::GetInstance();

  auto stmts = stmt_list->GetStatements();
  using stmt_type = ast::Statement::Type;
  for (auto stmt : stmts) {
    auto type = stmt->GetType();
    if (type == stmt_type::kCall) {
      auto call_stmt = std::static_pointer_cast<ast::Call>(stmt);
      std::string callee = call_stmt->GetProcName();
      if (!pkb_root->IsProcedure(callee))
        throw ExtractException("Procedure called does not exist.");

      pkb_root->AddCalls(caller, callee);
    } else if (type == stmt_type::kIf) {
      auto if_stmt = std::static_pointer_cast<ast::If>(stmt);
      ExtractStatementList(caller, if_stmt->GetConsequent());
      ExtractStatementList(caller, if_stmt->GetAlternative());
    } else if (type == stmt_type::kWhile) {
      auto while_stmt = std::static_pointer_cast<ast::While>(stmt);
      ExtractStatementList(caller, while_stmt->GetStmtList());
    }
  }
}

void CallsExtractor::GenerateCallsT(std::string proc_name, std::string cur,
                                    std::unordered_set<std::string>& visited) {
  auto pkb_root = pkb::PKBRoot::GetInstance();
  auto calls_cache = pkb_root->GetCallsCache(false);

  auto children = calls_cache.GetOriginalValues(cur);
  for (auto child : children) {
    if (visited.count(child)) {
      continue;
    }
    visited.insert(child);
    pkb_root->AddCallsT(proc_name, child);
    GenerateCallsT(proc_name, child, visited);
  }
}

}  // namespace xtr
}  // namespace spa
