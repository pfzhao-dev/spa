#pragma once

#include <cstdlib>
#include <memory>

#include "pql/Estimation.h"
#include "pql/clause/Clause.h"

namespace spa {

namespace pql {

class Affects : public Relational {
 public:
  Affects(std::shared_ptr<StatementRef> affector,
          std::shared_ptr<StatementRef> affected, bool transitive = false,
          bool is_bip = false);

  Affects(uint32_t affector, uint32_t affected, bool transitive = false,
          bool is_bip = false);

  std::shared_ptr<utils::Batch> Fetch() override;

  bool Test(std::vector<utils::Element>& tuple,
            std::vector<size_t>& attr_idx) override;

 protected:
  void PrepareComputation() override;

  std::vector<std::shared_ptr<Ref>> GetDeclaredRefs() override;

  Estimation ComputeEstimation() override;

  bool Equals(Clause const& other) const override;

  std::ostream& ToString(std::ostream& out) const override;

 private:
  bool transitive_;
  bool is_bip_;
  // assignment statement that affects the other assignment statement
  std::shared_ptr<StatementRef> affector_;
  // assignment statement that is affected
  std::shared_ptr<StatementRef> affected_;

  // convenience variables to avoid unnecessary computations.
  bool affector_declared_;
  bool affected_declared_;
  bool equal_decl_;
};

}  // namespace pql

}  // namespace spa
