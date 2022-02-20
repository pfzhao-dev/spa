#include "ASTHelper.h"

std::shared_ptr<spa::ast::Variable> buildVariable(std::string varName) {
  return std::make_shared<spa::ast::Variable>(spa::ast::Variable(varName));
}

std::shared_ptr<spa::ast::Print> buildPrint(std::string varName, int stmt_no) {
  return std::make_shared<spa::ast::Print>(spa::ast::Print(buildVariable(varName), stmt_no));
}

std::shared_ptr<spa::ast::Call> buildCall(std::string proc_name, std::string called_proc_name, int stmt_no) {
  return std::make_shared<spa::ast::Call>(
      spa::ast::Call(called_proc_name, stmt_no));
}

std::shared_ptr<spa::ast::Read> buildRead(std::string varName, int stmt_no) {
  return std::make_shared<spa::ast::Read>(buildVariable(varName), stmt_no);
}

std::shared_ptr<spa::ast::StatementList> buildStatementList(std::vector<std::shared_ptr<spa::ast::Statement>> stmtPtrs) {
  return std::make_shared<spa::ast::StatementList>(spa::ast::StatementList(stmtPtrs));
}

std::shared_ptr<spa::ast::Procedure> buildProcedure(std::string procName,
                                                    std::shared_ptr<spa::ast::StatementList> stmtListPtr) {
  return std::make_shared<spa::ast::Procedure>(spa::ast::Procedure(procName, stmtListPtr));
}

std::shared_ptr<spa::ast::Program> buildProgram(std::vector<std::shared_ptr<spa::ast::Procedure>> procPtrs) {
  return std::make_shared<spa::ast::Program>(spa::ast::Program(procPtrs));
}

std::shared_ptr<spa::ast::Constant> buildConstant(uint32_t const_val) {
  return std::make_shared<spa::ast::Constant>(spa::ast::Constant(const_val));
}

std::shared_ptr<spa::ast::Arithmetic> buildArithmetic(spa::ast::Arithmetic::OpType type,
                                                      std::shared_ptr<spa::ast::Absolute> lhs,
                                                      std::shared_ptr<spa::ast::Absolute> rhs) {
  return std::make_shared<spa::ast::Arithmetic>(spa::ast::Arithmetic(type, lhs, rhs));
}

std::shared_ptr<spa::ast::Assign> buildAssign(std::shared_ptr<spa::ast::Variable> variable,
                                              std::shared_ptr<spa::ast::Absolute> expr,
                                              int stmt_no) {
  return std::make_shared<spa::ast::Assign>(spa::ast::Assign(variable,
                                                             expr,
                                                             stmt_no));
}

std::shared_ptr<spa::ast::Relational> buildRelational(spa::ast::Relational::OpType type,
                                                      std::shared_ptr<spa::ast::Absolute> left,
                                                      std::shared_ptr<spa::ast::Absolute> right) {
  return std::make_shared<spa::ast::Relational>(spa::ast::Relational(type, left, right));
}

std::shared_ptr<spa::ast::If> buildIf(std::shared_ptr<spa::ast::Conditional> cond,
                                      std::shared_ptr<spa::ast::StatementList> cons,
                                      std::shared_ptr<spa::ast::StatementList> alt, int stmt_no) {
  return std::make_shared<spa::ast::If>(spa::ast::If(cond, cons, alt, stmt_no));
}

std::shared_ptr<spa::ast::While> buildWhile(std::shared_ptr<spa::ast::Conditional> cond,
                                            std::shared_ptr<spa::ast::StatementList> cons,
                                           int stmt_no) {
  return std::make_shared<spa::ast::While>(spa::ast::While(cond, cons, stmt_no));
}

std::shared_ptr<spa::ast::Logical> buildLogical(spa::ast::Logical::OpType type,
                                                std::shared_ptr<spa::ast::Conditional> left,
                                                std::shared_ptr<spa::ast::Conditional> right) {
  return std::make_shared<spa::ast::Logical>(type, left, right);
}
