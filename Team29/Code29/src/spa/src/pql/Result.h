#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Declaration.h"

namespace spa {

namespace pql {

class Result {
 public:
  virtual std::vector<std::shared_ptr<Declaration>> GetSelectEntities()
      const = 0;

  bool operator==(const Result& other) const;
  bool operator!=(const Result& other) const;

  friend std::ostream& operator<<(std::ostream& out, const Result& result);

 protected:
  virtual bool Equals(Result const& other) const = 0;

  virtual std::ostream& ToString(std::ostream& out) const;
};

class DeclaredResult : public Result {
 public:
  DeclaredResult(std::vector<std::shared_ptr<Declaration>> decls);

  std::vector<std::shared_ptr<Declaration>> GetSelectEntities() const override;

 protected:
  bool Equals(Result const& other) const override;

  std::ostream& ToString(std::ostream& out) const override;

 private:
  std::vector<std::shared_ptr<Declaration>> decls_;
};

class BooleanResult : public Result {
 public:
  BooleanResult();

  std::vector<std::shared_ptr<Declaration>> GetSelectEntities() const override;

 protected:
  bool Equals(Result const& other) const override;

  std::ostream& ToString(std::ostream& out) const override;
};

}  // namespace pql

}  // namespace spa
