#pragma once

#include <memory>
#include <string>

#include "pql/Declaration.h"

namespace spa {

namespace utils {

// Attribute class wraps a declaration to be able to represent a column
// name or information.
//
// For example, declaration of "p.stmt#" should be considered the same with
// "p.varName" where "p" is a print statement since the undelying value is
// the statement number represented by "p".
class Attribute {
 public:
  Attribute(std::shared_ptr<pql::Declaration> decl);

  bool operator==(const Attribute& other) const;

  std::shared_ptr<pql::Declaration> GetDecl() const;

  std::string GetName() const;

  pql::Declaration::Type GetType() const;

  friend std::ostream& operator<<(std::ostream& out, const Attribute& ref);

 private:
  std::shared_ptr<pql::Declaration> decl_;
};

}  // namespace utils

}  // namespace spa

namespace std {

// Implement hash function for the Attribute so that it could be used with
// an unordered map / set.
template <>
struct hash<spa::utils::Attribute> {
  std::size_t operator()(const spa::utils::Attribute& attr) const {
    return hash<std::string>()(attr.GetName());
  }
};

}  // namespace std
