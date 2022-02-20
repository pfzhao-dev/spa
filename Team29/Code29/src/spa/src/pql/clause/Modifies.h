#pragma once

#include "pql/Estimation.h"
#include "pql/clause/Clause.h"

namespace spa {

namespace pql {

class Modifies : public Relational {
 public:
  Modifies(std::shared_ptr<Ref> subject, std::shared_ptr<EntityRef> target);

  Modifies(uint32_t stmt_no, std::string var_name);

  Modifies(std::string proc_name, std::string var_name);

  std::shared_ptr<utils::Batch> Fetch() override;

  bool Test(std::vector<utils::Element>& tuple,
            std::vector<size_t>& attr_idx) override;

 protected:
  std::vector<std::shared_ptr<Ref>> GetDeclaredRefs() override;

  Estimation ComputeEstimation() override;

  bool Equals(Clause const& other) const override;

  std::ostream& ToString(std::ostream& out) const override;

 private:
  std::shared_ptr<Ref> subject_;
  std::shared_ptr<EntityRef> target_;

  bool subject_is_stmt_;
};

}  // namespace pql

}  // namespace spa
