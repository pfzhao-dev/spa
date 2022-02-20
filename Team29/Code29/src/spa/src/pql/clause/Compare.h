#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "pql/Estimation.h"
#include "pql/clause/Clause.h"

namespace spa {

namespace pql {

class Compare : public Clause {
 public:
  Compare(std::shared_ptr<Ref> lhs, std::shared_ptr<Ref> rhs);

  Compare(uint32_t lhs, uint32_t rhs);

  Compare(std::string lhs, std::string rhs);

  std::shared_ptr<utils::Batch> Fetch() override;

  bool Test(std::vector<utils::Element>& tuple,
            std::vector<size_t>& attr_idx) override;

 protected:
  std::vector<std::shared_ptr<Ref>> GetDeclaredRefs() override;

  Estimation ComputeEstimation() override;

  bool Equals(Clause const& other) const override;

  std::ostream& ToString(std::ostream& out) const override;

 private:
  std::shared_ptr<Ref> lhs_;
  std::shared_ptr<Ref> rhs_;

  bool lhs_declared_;
  bool rhs_declared_;
  bool equal_decl_;

  bool is_compare_int_;

  std::shared_ptr<utils::Batch> HandleFetchInteger();

  std::shared_ptr<utils::Batch> HandleFetchString();

  // Fetch all integer values based on a reference.
  std::unordered_set<uint32_t> GetIntegerValues(std::shared_ptr<Ref> ref);

  // Map string to tuples based on a reference.
  // NOTE: The tuple here is rather different from original tuple term
  //       used along the tables. Because the tuple is basically all the values
  //       that correspond to a particular string as their key.
  std::unordered_map<std::string, utils::Tuple> GetStringTuples(
      std::shared_ptr<Ref> ref);

  EstimationValue GetEstimationValue(std::shared_ptr<Ref> ref);

  std::string ExtractString(utils::Element elem, std::shared_ptr<Ref> ref);
};

}  // namespace pql

}  // namespace spa
