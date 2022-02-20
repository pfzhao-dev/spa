#include "Result.h"

#include <iostream>

#include "Comparators.h"
#include "QueryException.h"

namespace spa {

namespace pql {

bool Result::operator==(const Result& other) const {
  return this->Equals(other);
}

bool Result::operator!=(const Result& other) const { return !(*this == other); }

bool Result::Equals(Result const& other) const {
  return typeid(*this) == typeid(other);
}

std::ostream& Result::ToString(std::ostream& out) const { return out; }

std::ostream& operator<<(std::ostream& out, const Result& result) {
  out << "Result: ";
  return result.ToString(out);
}

DeclaredResult::DeclaredResult(std::vector<std::shared_ptr<Declaration>> decls)
    : decls_{decls} {
  for (auto decl_ptr : this->decls_) {
    if (!decl_ptr->HasName()) {
      throw QueryParseException("Declaration selected does not have a name");
    }
  }
}

std::vector<std::shared_ptr<Declaration>> DeclaredResult::GetSelectEntities()
    const {
  return this->decls_;
}

bool DeclaredResult::Equals(Result const& other) const {
  if (!Result::Equals(other)) return false;
  auto that = static_cast<DeclaredResult const&>(other);

  if (this->decls_.size() != that.decls_.size()) return false;

  PtrComparator<Declaration> decl_ptr_comparator;
  return std::equal(this->decls_.begin(), this->decls_.end(),
                    that.decls_.begin(), decl_ptr_comparator);
}

std::ostream& DeclaredResult::ToString(std::ostream& out) const {
  out << "Selected Declarations: {";
  for (auto decl_ptr : this->decls_) {
    out << *decl_ptr;
  }

  out << "}";
  return out;
}

BooleanResult::BooleanResult() {}

std::vector<std::shared_ptr<Declaration>> BooleanResult::GetSelectEntities()
    const {
  return std::vector<std::shared_ptr<Declaration>>();
}

bool BooleanResult::Equals(Result const& other) const {
  return Result::Equals(other);
}

std::ostream& BooleanResult::ToString(std::ostream& out) const {
  out << "BOOLEAN";
  return out;
}

}  // namespace pql

}  // namespace spa
