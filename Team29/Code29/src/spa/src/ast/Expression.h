#pragma once

#include <memory>
#include <string>

#include "Node.h"

namespace spa {

namespace ast {

class Expression : public Node {
 public:
  enum class Type {
    kVariable,
    kConstant,
    kArithmetic,
    kLogical,
    kRelational,
  };

  Expression(Type type) : type_(type) {}

  bool operator==(const spa::ast::Expression &other_expr) const;

  Type GetType() const;

 protected:
  virtual bool Equals(const spa::ast::Expression &other_expr) const;

 private:
  Type type_;
};

class Absolute : public Expression {
 public:
  Absolute(Type type) : Expression(type) {}
};

class Variable : public Absolute {
 public:
  Variable(std::string name) : Absolute(Type::kVariable) { this->name_ = name; }

  std::string GetName();

 protected:
  bool Equals(const spa::ast::Expression &other_expr) const override;

 private:
  std::string name_;
};

class Constant : public Absolute {
 public:
  Constant(uint32_t value) : Absolute(Type::kConstant) { this->value_ = value; }
  uint32_t GetValue();

 private:
  bool Equals(const spa::ast::Expression &other_expr) const override;

 private:
  // The value is guaranteed to be non-negative according to the
  // Concrete Syntax Grammar (CSG) for SIMPLE.
  uint32_t value_;
};

class Arithmetic : public Absolute {
 public:
  // Type of arithmetic operators according to the Abstract Syntax Grammar
  // (ASG) for SIMPLE.
  enum class OpType {
    kPlus,
    kMinus,
    kTimes,
    kDiv,
    kMod,
  };

  static OpType ToOpType(char s);

  Arithmetic(OpType op_type, std::shared_ptr<Absolute> lhs,
             std::shared_ptr<Absolute> rhs)
      : Absolute(Type::kArithmetic) {
    this->op_type_ = op_type;
    this->lhs_ = lhs;
    this->rhs_ = rhs;
  }

  OpType GetOpType();

  std::shared_ptr<Absolute> GetLHS();

  std::shared_ptr<Absolute> GetRHS();

 protected:
  bool Equals(const spa::ast::Expression &other_expr) const override;

 private:
  OpType op_type_;
  std::shared_ptr<Absolute> lhs_;
  std::shared_ptr<Absolute> rhs_;
};

class Conditional : public Expression {
 public:
  Conditional(Type type) : Expression(type) {}
};

class Logical : public Conditional {
 public:
  // Type of logical operators according to the Abstract Syntax Grammar
  // (ASG) for SIMPLE.
  enum class OpType {
    kNot,
    kAnd,
    kOr,
  };

  static OpType ToOpType(std::string s);

  // For `OpType::kNot`.
  // The rhs will be empty.
  //
  // TODO: Throw error if the op_type is invalid.
  Logical(OpType op_type, std::shared_ptr<Conditional> lhs)
      : Conditional(Type::kLogical) {
    op_type_ = op_type;
    lhs_ = lhs;
  }

  // For `OpType::kAnd` and `OpType::kOr`.
  // The rhs will be empty.
  //
  // TODO: Throw error if the op_type is invalid.
  Logical(OpType op_type, std::shared_ptr<Conditional> lhs,
          std::shared_ptr<Conditional> rhs)
      : Conditional(Type::kLogical) {
    op_type_ = op_type;
    lhs_ = lhs;
    rhs_ = rhs;
  }

  OpType GetOpType();

  std::shared_ptr<Conditional> GetLHS();

  std::shared_ptr<Conditional> GetRHS();

 protected:
  bool Equals(const spa::ast::Expression &other_exp) const override;

 private:
  OpType op_type_;
  std::shared_ptr<Conditional> lhs_;
  std::shared_ptr<Conditional> rhs_;
};

class Relational : public Conditional {
 public:
  // Type of relational operators according to the Abstract Syntax Grammar
  // (ASG) for SIMPLE.
  enum class OpType {
    kGt,
    kGte,
    kLt,
    kLte,
    kEq,
    kNeq,
  };

  static OpType ToOpType(std::string s);

  Relational(OpType op_type, std::shared_ptr<Absolute> lhs,
             std::shared_ptr<Absolute> rhs)
      : Conditional(Type::kRelational) {
    op_type_ = op_type;
    lhs_ = lhs;
    rhs_ = rhs;
  }

  OpType GetOpType();

  std::shared_ptr<Absolute> GetLHS();

  std::shared_ptr<Absolute> GetRHS();

 protected:
  bool Equals(const spa::ast::Expression &other_expr) const override;

 private:
  OpType op_type_;
  std::shared_ptr<Absolute> lhs_;
  std::shared_ptr<Absolute> rhs_;
};

}  // namespace ast

}  // namespace spa
