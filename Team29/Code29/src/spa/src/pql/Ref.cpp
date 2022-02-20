#include "Ref.h"

#include <iostream>

#include "Comparators.h"
#include "QueryException.h"
#include "pql/Evaluator.h"

namespace spa {

namespace pql {

Ref::Ref() : wildcard_{true}, decl_{std::shared_ptr<Declaration>()} {};

Ref::Ref(std::shared_ptr<Declaration> decl) : wildcard_{false}, decl_{decl} {};

bool Ref::IsWild() const { return wildcard_; }

bool Ref::IsDeclTypeIn(std::vector<Declaration::Type> types) const {
  return this->IsDeclared() && this->decl_.get()->IsTypeIn(types);
}

bool Ref::IsDeclared() const { return !wildcard_ && decl_; }

bool Ref::IsStatement() const {
  return wildcard_ || (decl_ && decl_->IsStatement());
}

double Ref::GetEstimationScale() {
  if (!decl_ || !(decl_->IsStatement())) return 1.0;
  if (decl_->GetType() == Declaration::Type::kStmt) return 1.0;

  // If statement of a specific type
  auto& evaluator = Evaluator::GetInstance();
  auto& stmts = evaluator.GetStatements(Declaration::Type::kStmt);
  auto& s_stmts = evaluator.GetStatements(decl_->GetType());

  return (double)s_stmts.size() / (double)stmts.size();
}

std::shared_ptr<StatementRef> Ref::ToStatementRef() {
  if (!this->IsStatement()) {
    throw QueryException("Unable to convert `Ref` object into `StatementRef`.");
  }

  if (this->IsWild()) {
    return std::make_shared<StatementRef>();
  } else {
    return std::make_shared<StatementRef>(this->decl_);
  }
}

std::shared_ptr<Declaration> Ref::GetDeclaration() const { return decl_; }

std::ostream& operator<<(std::ostream& out, const Ref& ref) {
  return ref.ToString(out);
}

bool Ref::operator==(const Ref& other) const { return this->Equals(other); }

bool Ref::operator!=(const Ref& other) const { return !(*this == other); }

bool Ref::Equals(Ref const& other) const {
  PtrComparator<Declaration> decl_comparator;
  return typeid(*this) == typeid(other) && this->wildcard_ == other.wildcard_ &&
         decl_comparator(this->decl_, other.decl_);
}

std::ostream& Ref::ToString(std::ostream& out) const {
  out << "Ref: wild: " << this->wildcard_ << " decl: ";
  if (this->decl_) {
    out << *this->decl_;
  } else {
    out << "empty";
  }
  return out;
}

EntityRef::EntityRef(std::string id)
    : Ref{std::shared_ptr<Declaration>()}, id_{id} {};

std::string EntityRef::GetId() { return id_; }

bool EntityRef::IsDeclared() const {
  return Ref::IsDeclared() && this->id_.empty();
}

bool EntityRef::Equals(Ref const& other) const {
  if (typeid(*this) != typeid(other)) return false;
  auto that = static_cast<EntityRef const&>(other);
  return this->id_ == that.id_ && Ref::Equals(other);
}

std::ostream& EntityRef::ToString(std::ostream& out) const {
  out << "[Entity: ";
  Ref::ToString(out);
  out << " id: " << this->id_ << "]";
  return out;
}

StatementRef::StatementRef(std::shared_ptr<Declaration> decl)
    : Ref{decl}, stmt_num_{0} {
  if (!this->decl_ || !this->decl_->IsStatement()) {
    throw QueryParseException("Declaration is not a statement");
  }
}

StatementRef::StatementRef(uint32_t stmt_num)
    : Ref{std::shared_ptr<Declaration>()}, stmt_num_{stmt_num} {};

uint32_t StatementRef::GetStmtNo() { return stmt_num_; }

bool StatementRef::IsDeclared() const {
  return Ref::IsDeclared() && this->stmt_num_ == 0;
}

bool StatementRef::IsStatement() const { return true; }

std::shared_ptr<StatementRef> StatementRef::ToStatementRef() {
  return shared_from_this();
}

bool StatementRef::Equals(Ref const& other) const {
  if (typeid(*this) != typeid(other)) return false;
  auto that = static_cast<StatementRef const&>(other);
  return this->stmt_num_ == that.stmt_num_ && Ref::Equals(other);
}

std::ostream& StatementRef::ToString(std::ostream& out) const {
  out << "[Statement: ";
  Ref::ToString(out);
  out << " stmt_num: " << this->stmt_num_ << "]";
  return out;
}

bool Ref::Test(std::string& s) {
  if (wildcard_) return true;

  auto& evaluator = Evaluator::GetInstance();

  auto decl_type = decl_->GetType();
  if (decl_type == Declaration::Type::kVariable) {
    return evaluator.IsVariable(s);
  } else if (decl_type == Declaration::Type::kProcedure) {
    return evaluator.IsProcedure(s);
  } else {
    return false;
  }
}

bool EntityRef::Test(std::string& s) {
  if (!wildcard_ && !decl_) {
    return s == id_;
  } else {
    return Ref::Test(s);
  }
}

bool StatementRef::Test(std::string& s) {
  if (!wildcard_ && !decl_) {
    return false;  // cannot match
  } else {
    return Ref::Test(s);
  }
}

bool Ref::Test(uint32_t& value) {
  if (wildcard_) return true;

  auto& evaluator = Evaluator::GetInstance();

  auto decl_type = decl_->GetType();
  if (decl_type == Declaration::Type::kConstant) {
    return evaluator.IsConstant(value);
  } else if (decl_type == Declaration::Type::kVariable ||
             decl_type == Declaration::Type::kProcedure) {
    return false;  // does not match
  } else {
    return evaluator.IsStatement(value, decl_type);
  }
}

bool EntityRef::Test(uint32_t& value) {
  if (!wildcard_ && !decl_) {
    return false;  // cannot match
  } else {
    return Ref::Test(value);
  }
}

bool StatementRef::Test(uint32_t& value) {
  if (!wildcard_ && !decl_) {
    return stmt_num_ == value;
  } else {
    return Ref::Test(value);
  }
}

bool Ref::Test(utils::Element elem) {
  if (uint32_t* v = std::get_if<uint32_t>(&elem)) {
    return Ref::Test(*v);
  } else {
    return Ref::Test(std::get<std::string>(elem));
  }
}

bool EntityRef::Test(utils::Element elem) {
  if (!wildcard_ && !decl_) {
    if (std::string* s = std::get_if<std::string>(&elem)) {
      return id_ == *s;
    } else {
      return false;  // cannot match
    }
  } else {
    return Ref::Test(elem);
  }
}

bool StatementRef::Test(utils::Element elem) {
  if (!wildcard_ && !decl_) {
    if (uint32_t* v = std::get_if<uint32_t>(&elem)) {
      return stmt_num_ == *v;
    } else {
      return false;  // cannot match
    }
  } else {
    return Ref::Test(elem);
  }
}

utils::Element StatementRef::GetValue() { return stmt_num_; }

utils::Element EntityRef::GetValue() { return id_; }

}  // namespace pql

}  // namespace spa
