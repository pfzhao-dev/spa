#pragma once
#include <ast/Node.h>
#include <ast/Program.h>
#include <ast/Procedure.h>
#include <ast/Statement.h>

std::shared_ptr<spa::ast::Variable> buildVariable(std::string varName);

std::shared_ptr<spa::ast::Print> buildPrint(std::string varName, int stmt_no);

std::shared_ptr<spa::ast::Call> buildCall(std::string proc_name, std::string called_proc_name, int stmt_no);

std::shared_ptr<spa::ast::Read> buildRead(std::string varName, int stmt_no);

// change to pointer for polymorphic
std::shared_ptr<spa::ast::StatementList> buildStatementList(std::vector<std::shared_ptr<spa::ast::Statement>> stmtPtrs);

std::shared_ptr<spa::ast::Procedure> buildProcedure(std::string procName, std::shared_ptr<spa::ast::StatementList> stmtList);

std::shared_ptr<spa::ast::Program> buildProgram(std::vector<std::shared_ptr<spa::ast::Procedure>> procedures);

std::shared_ptr<spa::ast::Constant> buildConstant(uint32_t const_val);

std::shared_ptr<spa::ast::Arithmetic> buildArithmetic(spa::ast::Arithmetic::OpType type,
                                                      std::shared_ptr<spa::ast::Absolute> lhs,
                                                      std::shared_ptr<spa::ast::Absolute> rhs);

std::shared_ptr<spa::ast::Assign> buildAssign(std::shared_ptr<spa::ast::Variable> variable,
                                              std::shared_ptr<spa::ast::Absolute> expr, int stmt_no);

std::shared_ptr<spa::ast::Relational> buildRelational(spa::ast::Relational::OpType type,
                                                      std::shared_ptr<spa::ast::Absolute> left,
                                                      std::shared_ptr<spa::ast::Absolute> right);

std::shared_ptr<spa::ast::If> buildIf(std::shared_ptr<spa::ast::Conditional> cond, std::shared_ptr<spa::ast::StatementList> cons,
                                      std::shared_ptr<spa::ast::StatementList> alt, int stmt_no);

std::shared_ptr<spa::ast::While> buildWhile(std::shared_ptr<spa::ast::Conditional> cond,
                                            std::shared_ptr<spa::ast::StatementList> cons,
                                            int stmt_no);
std::shared_ptr<spa::ast::Logical> buildLogical(spa::ast::Logical::OpType type,
                                                std::shared_ptr<spa::ast::Conditional> left,
                                                std::shared_ptr<spa::ast::Conditional> right);
