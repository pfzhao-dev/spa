#pragma once

#include "pql/Estimation.h"
#include "pql/clause/Clause.h"

namespace spa {

namespace pql {

class Parent : public Relational {
 public:
  Parent(std::shared_ptr<StatementRef> pred, std::shared_ptr<StatementRef> succ,
         bool transitive = false);

  Parent(uint32_t pred, uint32_t succ, bool transitive = false);

  std::shared_ptr<utils::Batch> Fetch() override;

  bool Test(std::vector<utils::Element>& tuple,
            std::vector<size_t>& attr_idx) override;

 protected:
  std::vector<std::shared_ptr<Ref>> GetDeclaredRefs() override;

  Estimation ComputeEstimation() override;

  bool Equals(Clause const& other) const override;

  std::ostream& ToString(std::ostream& out) const override;

 private:
  bool transitive_;
  std::shared_ptr<StatementRef> pred_;
  std::shared_ptr<StatementRef> succ_;

  bool equal_decl_;
};

}  // namespace pql

}  // namespace spa
