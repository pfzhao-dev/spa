#include "Expression.h"

#include <iostream>

#include "ast/parser/ParseException.h"

namespace spa {

namespace ast {

Expression::Type Expression::GetType() const { return type_; }

bool Expression::Equals(const Expression &other_exp) const {
  return type_ == other_exp.type_;
}

std::string Variable::GetName() { return name_; }

bool Variable::Equals(const Expression &other_expr) const {
  if (typeid(*this) != typeid(other_expr)) return false;
  Variable otherVariable = static_cast<Variable const &>(other_expr);
  return Absolute::Equals(other_expr) && this->name_ == otherVariable.name_;
}

Logical::OpType Logical::ToOpType(std::string s) {
  if (s == "not") return Logical::OpType::kNot;
  if (s == "and") return Logical::OpType::kAnd;
  if (s == "or") return Logical::OpType::kOr;
  throw ParseException();
}

Logical::OpType Logical::GetOpType() { return op_type_; }

bool Logical::Equals(const Expression &other_exp) const {
  if (typeid(*this) != typeid(other_exp)) return false;
  Logical otherLog = static_cast<Logical const &>(other_exp);
  return Conditional::Equals(other_exp) && op_type_ == otherLog.GetOpType() &&
         *lhs_ == *otherLog.lhs_ && *rhs_ == *otherLog.rhs_;
}

std::shared_ptr<Absolute> Arithmetic::GetLHS() { return lhs_; }

std::shared_ptr<Absolute> Arithmetic::GetRHS() { return rhs_; }

std::shared_ptr<Conditional> Logical::GetLHS() { return lhs_; }

std::shared_ptr<Conditional> Logical::GetRHS() { return rhs_; }

std::shared_ptr<Absolute> Relational::GetLHS() { return lhs_; }

std::shared_ptr<Absolute> Relational::GetRHS() { return rhs_; }

Arithmetic::OpType Arithmetic::ToOpType(char s) {
  if (s == '+') return Arithmetic::OpType::kPlus;
  if (s == '-') return Arithmetic::OpType::kMinus;
  if (s == '%') return Arithmetic::OpType::kMod;
  if (s == '/') return Arithmetic::OpType::kDiv;
  if (s == '*') return Arithmetic::OpType::kTimes;
  throw ParseException();
}

Arithmetic::OpType Arithmetic::GetOpType() { return op_type_; }

bool Arithmetic::Equals(const Expression &other_expr) const {
  if (typeid(*this) != typeid(other_expr)) return false;
  Arithmetic otherAri = static_cast<Arithmetic const &>(other_expr);
  return Absolute::Equals(other_expr) && op_type_ == otherAri.GetOpType() &&
         (*lhs_ == *otherAri.lhs_) && (*rhs_ == *otherAri.rhs_);
}

Relational::OpType Relational::ToOpType(std::string s) {
  if (s == ">=") return Relational::OpType::kGte;
  if (s == "<=") return Relational::OpType::kLte;
  if (s == ">") return Relational::OpType::kGt;
  if (s == "<") return Relational::OpType::kLt;
  if (s == "==") return Relational::OpType::kEq;
  if (s == "!=") return Relational::OpType::kNeq;
  throw ParseException();
}

Relational::OpType Relational::GetOpType() { return op_type_; }

bool Relational::Equals(const Expression &other_expr) const {
  if (typeid(*this) != typeid(other_expr)) return false;
  Relational otherRel = static_cast<Relational const &>(other_expr);
  return Conditional::Equals(other_expr) && op_type_ == otherRel.GetOpType() &&
         *lhs_ == *otherRel.lhs_ && *rhs_ == *otherRel.rhs_;
}

bool Constant::Equals(const Expression &other_expr) const {
  if (typeid(*this) != typeid(other_expr)) return false;
  Constant otherConstant = static_cast<Constant const &>(other_expr);
  return Absolute::Equals(other_expr) && value_ == otherConstant.value_;
}

uint32_t Constant::GetValue() { return value_; }

bool Expression::operator==(const Expression &other_expr) const {
  return (this != nullptr) && this->Equals(other_expr);
}

}  // namespace ast

}  // namespace spa
