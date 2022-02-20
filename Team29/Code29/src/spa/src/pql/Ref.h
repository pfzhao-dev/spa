#pragma once

#include <memory>

#include "pql/Declaration.h"
#include "utils/Element.h"

namespace spa {

namespace pql {

// Forward declaration since the `Ref` class may need to be able to convert
// into a `StatementRef`.
//
// TODO: Figure out a better implementation.
class StatementRef;

class Ref {
 public:
  // Constructor for a wildcard reference.
  Ref();

  // Reference to a declaration in the query.
  Ref(std::shared_ptr<Declaration> decl);

  bool operator==(const Ref& other) const;
  bool operator!=(const Ref& other) const;

  bool IsWild() const;

  // returns true if ref is declared and its declaration
  // type matches the specified type
  bool IsDeclTypeIn(std::vector<Declaration::Type> types) const;

  // returns true if wildcard_ is false and decl_ is non-empty
  virtual bool IsDeclared() const;

  // Returns true if any of the following conditions is satisfied:
  // - `wildcard_` is true
  // - `IsDeclared()` is true and the referenced declaration is a statement
  // - The object is an instance of `StatementRef`.
  virtual bool IsStatement() const;

  // Convert any ref object to a statement ref. Note that `IsStatement()` must
  // be true to be able to call this function. Not using `const` in the function
  // declaration since the returned object may be equal to the current object
  // if it is already a `StatementRef`.
  virtual std::shared_ptr<StatementRef> ToStatementRef();

  virtual utils::Element GetValue() = 0;

  // Returns a multiplier w.r.t. to the number of results produced by this
  // reference. This is useful upon fetching a specific statement type.
  //
  // If the ref represents a statement, this function will return a value
  // equal to the number of statements that could be represented divided by
  // the total number of statements.
  //
  // Otherwise, the return value will be equal to 1.0.
  double GetEstimationScale();

  std::shared_ptr<Declaration> GetDeclaration() const;

  virtual bool Test(std::string& s);

  virtual bool Test(uint32_t& value);

  virtual bool Test(utils::Element elem);

  friend std::ostream& operator<<(std::ostream& out, const Ref& ref);

 protected:
  bool wildcard_;
  std::shared_ptr<Declaration> decl_;

  virtual bool Equals(Ref const& other) const = 0;

  virtual std::ostream& ToString(std::ostream& out) const;
};

class EntityRef : public Ref {
 public:
  // Inherit constructor(s) from `Ref`.
  using Ref::Ref;

  // The reference can also be in the form of a string that represents
  // an entity name.
  EntityRef(std::string id);

  std::string GetId();

  bool IsDeclared() const override;

  bool Test(std::string& s) override;

  bool Test(uint32_t& value) override;

  bool Test(utils::Element elem) override;

  utils::Element GetValue() override;

 protected:
  bool Equals(Ref const& other) const override;

  std::ostream& ToString(std::ostream& out) const override;

 private:
  std::string id_ = "";
};

class StatementRef : public Ref,
                     public std::enable_shared_from_this<StatementRef> {
 public:
  // Inherit constructor(s) from `Ref`.
  using Ref::Ref;

  // Override the constructor that takes reference to a declaration in the
  // query to check for the declaration type. If the provided declaration
  // is not of type statement, the constructor will throw an error.
  StatementRef(std::shared_ptr<Declaration> decl);

  // The reference can also be in the form of a number that represents
  // a statement number.
  StatementRef(uint32_t stmt_num);

  uint32_t GetStmtNo();

  bool IsDeclared() const override;

  bool IsStatement() const override;

  std::shared_ptr<StatementRef> ToStatementRef() override;

  bool Test(std::string& s) override;

  bool Test(uint32_t& value) override;

  bool Test(utils::Element elem) override;

  utils::Element GetValue() override;

 protected:
  bool Equals(Ref const& other) const override;

  std::ostream& ToString(std::ostream& out) const override;

 private:
  uint32_t stmt_num_ = 0;
};

}  // namespace pql

}  // namespace spa
