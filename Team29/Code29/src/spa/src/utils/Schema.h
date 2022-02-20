#pragma once

#include <exception>
#include <memory>
#include <string>
#include <vector>

#include "Attribute.h"
#include "pql/clause/Clause.h"

namespace spa {

namespace utils {

class SchemaException : public std::exception {
 public:
  SchemaException(const char* msg) : msg_{msg} {}

  const char* what() const throw() { return msg_; }

 private:
  const char* msg_;
};

class Schema {
 public:
  Schema(std::vector<Attribute> attrs);

  // Convenience constructor that represents a table with a single column.
  Schema(Attribute attr);

  // Convenience constructor that represents a table with zero column.
  // The main usecase is to represent the reselt of a select boolean query.
  Schema();

  static Schema Join(Schema& left, Schema& right);

  static std::vector<std::pair<size_t, size_t>> GetJoinIdx(Schema& left,
                                                           Schema& right);

  static Schema ProjectAttr(Schema& base, std::vector<Attribute> attrs);

  static Schema ProjectClause(Schema& base,
                              std::shared_ptr<pql::Clause> clause);

  // NOTE: This function has a quadratic time complexity for simplicity
  //       since the number of attributes is expected to be small and
  //       using more efficient data structures like hash table could be
  //       less performant due to its overhead cost.
  bool HasAttrs(std::vector<Attribute> attrs);

  // NOTE: This function has a linear time complexity for simplicity since
  //       the number of attrs should be relatively small.
  bool HasAttr(Attribute& attrs);

  std::vector<Attribute>& GetAttrs();

  size_t GetIdx(Attribute attr);

  size_t Size();

  friend std::ostream& operator<<(std::ostream& out, const Schema& ref);

 private:
  std::vector<Attribute> attrs_;
};

}  // namespace utils

}  // namespace spa
