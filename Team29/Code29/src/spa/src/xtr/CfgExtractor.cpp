#include "CfgExtractor.h"

#include "ast/Statement.h"
#include "cfg/Graph.h"
#include "cfg/ProgramGraph.h"
#include "pkb/PKBRoot.h"

namespace spa {

namespace xtr {

CfgExtractor::CfgExtractor() {}

void CfgExtractor::Extract(std::shared_ptr<ast::Program> program,
                           std::vector<std::string> order) {
  auto graph = std::make_shared<cfg::ProgramGraph>();

  auto procedures = program->GetProcList();
  auto pkb_root = pkb::PKBRoot::GetInstance();

  // set dummy counter to start from max stmt num + 1
  dummy_counter_ = pkb_root->GetStmtNos().size() + 1;

  // always start from 1, since 0 indicates a children edge
  neighbor_counter_ = 1;

  std::unordered_map<std::string, std::shared_ptr<ast::Procedure>> proc_map;
  for (auto p : program->GetProcList()) {
    proc_map[p->GetName()] = p;
  }

  for (auto s : order) {
    auto procedure = proc_map[s];
    auto stmt_list = procedure->GetStmtList();
    auto [head, tail] = BuildCfg(stmt_list, graph);
    graph->Add(procedure->GetName(), head, tail);
  }

  pkb_root->AddProgramGraph(graph);
}

CfgExtractor::CfgHeadTail CfgExtractor::BuildCfg(
    std::shared_ptr<ast::StatementList> stmt_list,
    std::shared_ptr<cfg::ProgramGraph> graph) {
  CfgNodePtr head;
  CfgNodePtr tail;

  bool is_call_tail_exist = false;
  uint32_t call_neighbor_out_label;
  std::shared_ptr<cfg::Node> call_tail;

  // Construct the graph for statement list
  for (auto stmt : stmt_list->GetStatements()) {
    using StmtType = ast::Statement::Type;
    auto [stmt_head, stmt_tail] = BuildCfg(stmt, graph);
    if (!head) {  // Only for the first statement
      head = stmt_head;
      tail = stmt_tail;

    } else {  // For the second statement onwards
      tail->AddChild(stmt_head);
      stmt_head->AddReverseChild(tail);

      tail = stmt_tail;
    }

    if (is_call_tail_exist) {
      call_tail->AddNeighborOut(stmt_head, call_neighbor_out_label);
      is_call_tail_exist = false;
    }

    if (stmt->GetType() == StmtType::kCall) {
      auto call_stmt = std::static_pointer_cast<ast::Call>(stmt);

      auto proc_head = graph->GetProcHead(call_stmt->GetProcName());
      stmt_head->AddNeighborIn(proc_head, neighbor_counter_);
      proc_head->AddReverseNeighborIn(stmt_head, neighbor_counter_);

      // increase the counter to guarantee uniqueness among all neighbor labels.
      call_neighbor_out_label = neighbor_counter_++;

      call_tail = graph->GetProcTail(call_stmt->GetProcName());
      is_call_tail_exist = true;
    }
  }

  if (is_call_tail_exist) {
    // create a dummy node to avoid unnecessary branching
    auto dummy = std::make_shared<cfg::Node>(dummy_counter_++, true);

    tail->AddChild(dummy);
    dummy->AddReverseChild(tail);

    call_tail->AddNeighborOut(dummy, call_neighbor_out_label);

    tail = dummy;
  }

  return {head, tail};
}

CfgExtractor::CfgHeadTail CfgExtractor::BuildCfg(
    std::shared_ptr<ast::Statement> stmt,
    std::shared_ptr<cfg::ProgramGraph> graph) {
  auto head = std::make_shared<cfg::Node>(stmt->GetStmtNo());
  auto tail = head;

  using StmtType = ast::Statement::Type;
  if (stmt->GetType() == StmtType::kIf) {
    // We create a dummy tail for if statement to avoid branching
    // that can cause complicated issues for bip clause.
    tail = std::make_shared<cfg::Node>(dummy_counter_++, true);

    auto if_stmt = std::static_pointer_cast<ast::If>(stmt);

    auto [con_head, con_tail] = BuildCfg(if_stmt->GetConsequent(), graph);
    auto [alt_head, alt_tail] = BuildCfg(if_stmt->GetAlternative(), graph);

    head->AddChild(con_head);
    con_head->AddReverseChild(head);
    head->AddChild(alt_head);
    alt_head->AddReverseChild(head);

    con_tail->AddChild(tail);
    tail->AddReverseChild(con_tail);
    alt_tail->AddChild(tail);
    tail->AddReverseChild(alt_tail);

  } else if (stmt->GetType() == StmtType::kWhile) {
    auto while_stmt = std::static_pointer_cast<ast::While>(stmt);

    auto [child_head, child_tail] = BuildCfg(while_stmt->GetStmtList(), graph);

    head->AddChild(child_head);
    child_head->AddReverseChild(head);

    child_tail->AddChild(head);
    head->AddReverseChild(child_tail);
  }

  return {head, tail};
}

}  // namespace xtr

}  // namespace spa
