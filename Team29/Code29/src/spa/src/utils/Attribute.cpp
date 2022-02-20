#include "Attribute.h"

#include <iostream>

namespace spa {

namespace utils {

Attribute::Attribute(std::shared_ptr<pql::Declaration> decl) : decl_(decl) {}

bool Attribute::operator==(const Attribute& other) const {
  return decl_->GetName() == other.GetName();
}

std::shared_ptr<pql::Declaration> Attribute::GetDecl() const { return decl_; }

std::string Attribute::GetName() const { return decl_->GetName(); }

pql::Declaration::Type Attribute::GetType() const { return decl_->GetType(); }

std::ostream& operator<<(std::ostream& out, const Attribute& ref) {
  out << "[attr: " << ref.GetName();
  auto decl = ref.GetDecl();
  if (decl->HasValue()) {
    out << "has value";
  }
  out << "]";
  return out;
}

}  // namespace utils

}  // namespace spa
