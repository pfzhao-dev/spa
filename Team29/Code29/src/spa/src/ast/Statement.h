#pragma once

#include <memory>
#include <string>

#include "Expression.h"
#include "Node.h"

namespace spa {

namespace ast {

// Forward declaration to avoid cyclic dependency
class StatementList;

class Statement : public Node {
 public:
  enum class Type {
    kRead,
    kPrint,
    kCall,
    kWhile,
    kIf,
    kAssign,
  };

  bool operator==(const spa::ast::Statement &other_stmt) const;

  uint32_t GetStmtNo();

  Type GetType();

 protected:
  Statement(Type type, uint32_t stmt_no) : type_(type), stmt_no_(stmt_no) {}

  virtual bool Equals(const spa::ast::Statement &other_stmt) const;

 private:
  uint32_t stmt_no_;
  Type type_;
};

class Read : public Statement {
 public:
  Read(std::shared_ptr<Variable> variable, int stmt_no)
      : Statement(Type::kRead, stmt_no), variable_(variable) {}

  std::shared_ptr<Variable> GetVariable();

 protected:
  bool Equals(const spa::ast::Statement &other_stmt) const override;

 private:
  std::shared_ptr<Variable> variable_;
};

class Print : public Statement {
 public:
  Print(std::shared_ptr<Variable> variable, int stmt_no)
      : Statement(Type::kPrint, stmt_no), variable_(variable) {}

  std::shared_ptr<Variable> GetVariable();

 protected:
  bool Equals(const spa::ast::Statement &other_stmt) const override;

 private:
  std::shared_ptr<Variable> variable_;
};

class Call : public Statement {
 public:
  Call(std::string proc_name, int stmt_no)
      : Statement(Type::kCall, stmt_no), proc_name_(proc_name) {}

  std::string GetProcName();

 protected:
  bool Equals(const spa::ast::Statement &other_stmt) const override;

 private:
  // Store only the procedure name instead of the pointer to the actual
  // `Procedure` node due to the PKB structure.
  std::string proc_name_;
};

class While : public Statement {
 public:
  While(std::shared_ptr<Conditional> cond,
        std::shared_ptr<StatementList> stmt_list, int stmt_no)
      : Statement(Type::kWhile, stmt_no), cond_(cond), stmt_list_(stmt_list) {}

  std::shared_ptr<Conditional> GetCond();
  std::shared_ptr<StatementList> GetStatements();

  const std::shared_ptr<spa::ast::StatementList> &GetStmtList() const;

 protected:
  bool Equals(const spa::ast::Statement &other_stmt) const override;

 private:
  std::shared_ptr<Conditional> cond_;
  std::shared_ptr<StatementList> stmt_list_;
};

class If : public Statement {
 public:
  If(std::shared_ptr<Conditional> cond,
     std::shared_ptr<StatementList> consequent,
     std::shared_ptr<StatementList> alternative, int stmt_no)
      : Statement(Type::kIf, stmt_no),
        cond_(cond),
        consequent_(consequent),
        alternative_(alternative) {}

  std::shared_ptr<Conditional> GetCond();
  std::shared_ptr<StatementList> GetConsequent();
  std::shared_ptr<StatementList> GetAlternative();

 protected:
  bool Equals(const spa::ast::Statement &other_stmt) const override;

 private:
  std::shared_ptr<Conditional> cond_;
  std::shared_ptr<StatementList> consequent_;
  std::shared_ptr<StatementList> alternative_;
};

class Assign : public Statement {
 public:
  Assign(std::shared_ptr<Variable> variable, std::shared_ptr<Absolute> expr,
         int stmt_no)
      : Statement(Type::kAssign, stmt_no), variable_(variable), expr_(expr) {}

  std::shared_ptr<Variable> GetVariable();
  std::shared_ptr<Absolute> GetExpression();

 protected:
  bool Equals(const spa::ast::Statement &other_stmt) const override;

 private:
  std::shared_ptr<Variable> variable_;
  std::shared_ptr<Absolute> expr_;
};

}  // namespace ast

}  // namespace spa
