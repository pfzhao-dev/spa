#pragma once

#include <cstring>

#include "pql/Estimation.h"
#include "pql/clause/Clause.h"

namespace spa {

namespace pql {

class Calls : public Relational {
 public:
  Calls(std::shared_ptr<EntityRef> caller, std::shared_ptr<EntityRef> callee,
        bool transitive = false);

  Calls(std::string caller, std::string callee, bool transitive = false);

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
  std::shared_ptr<EntityRef> caller_;
  std::shared_ptr<EntityRef> callee_;

  bool equal_decl_;
};

}  // namespace pql

}  // namespace spa
