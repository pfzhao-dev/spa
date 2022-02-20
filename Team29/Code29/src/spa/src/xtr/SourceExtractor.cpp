#include "xtr/SourceExtractor.h"

#include "pkb/PKBRoot.h"
#include "xtr/CallsExtractor.h"
#include "xtr/CfgExtractor.h"
#include "xtr/FollowsExtractor.h"
#include "xtr/ModifiesExtractor.h"
#include "xtr/ParentExtractor.h"
#include "xtr/PatternExtractor.h"
#include "xtr/UsesExtractor.h"

namespace spa {
namespace xtr {

void SourceExtractor::Extract(std::shared_ptr<ast::Program> program) {
  PopulatePKBProg(program);

  FollowsExtractor follows_extractor;
  follows_extractor.Extract(program);

  ParentExtractor parent_extractor;
  parent_extractor.Extract(program);

  CallsExtractor calls_extractor;
  calls_extractor.Extract(program);

  std::vector<std::string> topo_order = Toposort(program);

  UsesExtractor uses_extractor;
  uses_extractor.Extract(program, topo_order);

  ModifiesExtractor modifies_extractor;
  modifies_extractor.Extract(program, topo_order);

  PatternExtractor pattern_extractor;
  pattern_extractor.Extract(program);

  CfgExtractor cfg_extractor;
  cfg_extractor.Extract(program, topo_order);
}

void SourceExtractor::PopulatePKBProg(std::shared_ptr<ast::Program> program) {
  auto pkb_root = pkb::PKBRoot::GetInstance();
  for (auto p : program->GetProcList()) {
    pkb_root->AddProcedure(p);
    PopulatePKBStatementList(p->GetName(), p->GetStmtList());
  }
}

void SourceExtractor::PopulatePKBStatementList(std::string proc_name,
    std::shared_ptr<ast::StatementList> stmt_list) {
  for (auto stmt : stmt_list->GetStatements()) {
    PopulatePKBStatement(proc_name, stmt);
  }
}

void SourceExtractor::PopulatePKBStatement(std::string proc_name,
    std::shared_ptr<ast::Statement> stmt) {
  auto pkb_root = pkb::PKBRoot::GetInstance();
  pkb_root->AddStmt(stmt, proc_name);

  using stmt_type = ast::Statement::Type;
  auto type = stmt->GetType();
  if (type == stmt_type::kIf) {
    auto if_stmt = std::static_pointer_cast<ast::If>(stmt);
    PopulatePKBExpression(if_stmt->GetCond());
    PopulatePKBStatementList(proc_name, if_stmt->GetConsequent());
    PopulatePKBStatementList(proc_name, if_stmt->GetAlternative());
  } else if (type == stmt_type::kWhile) {
    auto while_stmt = std::static_pointer_cast<ast::While>(stmt);
    PopulatePKBExpression(while_stmt->GetCond());
    PopulatePKBStatementList(proc_name, while_stmt->GetStatements());
  } else if (type == stmt_type::kAssign) {
    auto assign_stmt = std::static_pointer_cast<ast::Assign>(stmt);
    PopulatePKBExpression(assign_stmt->GetVariable());
    PopulatePKBExpression(assign_stmt->GetExpression());
  } else if (type == stmt_type::kPrint) {
    auto print_stmt = std::static_pointer_cast<ast::Print>(stmt);
    auto var = print_stmt->GetVariable();
    PopulatePKBExpression(var);
  } else if (type == stmt_type::kRead) {
    auto read_stmt = std::static_pointer_cast<ast::Read>(stmt);
    auto var = read_stmt->GetVariable();
    PopulatePKBExpression(var);
  }
}

void SourceExtractor::PopulatePKBExpression(
    std::shared_ptr<ast::Expression> expr) {
  auto pkb_root = pkb::PKBRoot::GetInstance();

  using expr_type = ast::Expression::Type;
  auto type = expr->GetType();

  if (type == expr_type::kVariable) {
    auto var = std::static_pointer_cast<ast::Variable>(expr);
    pkb_root->AddVariable(var);
  } else if (type == expr_type::kConstant) {
    auto c = std::static_pointer_cast<ast::Constant>(expr);
    pkb_root->AddConstant(c);
  } else if (type == expr_type::kArithmetic) {
    auto arith = std::static_pointer_cast<ast::Arithmetic>(expr);
    PopulatePKBExpression(arith->GetLHS());
    PopulatePKBExpression(arith->GetRHS());
  } else if (type == expr_type::kRelational) {
    auto relational = std::static_pointer_cast<ast::Relational>(expr);
    PopulatePKBExpression(relational->GetLHS());
    PopulatePKBExpression(relational->GetRHS());
  } else if (type == expr_type::kLogical) {
    auto logical = std::static_pointer_cast<ast::Logical>(expr);
    using LogicalType = ast::Logical::OpType;
    auto log_type = logical->GetOpType();
    PopulatePKBExpression(logical->GetLHS());
    if (log_type != LogicalType::kNot) {
      PopulatePKBExpression(logical->GetRHS());
    }
  }
}

void SourceExtractor::ToposortUtil(std::string cur,
                                   std::unordered_set<std::string>& visited,
                                   std::vector<std::string>& stack) {
  visited.insert(cur);
  /* need to be called after CallsExtractor */
  auto pkb_root = pkb::PKBRoot::GetInstance();
  auto calls_cache = pkb_root->GetCallsCache(false);
  for (auto nx : calls_cache.GetOriginalValues(cur)) {
    if (!visited.count(nx)) {
      ToposortUtil(nx, visited, stack);
    }
  }

  stack.push_back(cur);
}

std::vector<std::string> SourceExtractor::Toposort(
    std::shared_ptr<ast::Program> program) {
  auto pkb_root = pkb::PKBRoot::GetInstance();

  std::vector<std::string> stack;
  std::unordered_set<std::string> visited;

  for (auto key : pkb_root->GetProcNames()) {
    if (!visited.count(key)) {
      ToposortUtil(key, visited, stack);
    }
  }

  return stack;
}

}  // namespace xtr
}  // namespace spa
