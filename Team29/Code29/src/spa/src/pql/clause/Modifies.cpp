#include "Modifies.h"

#include "pql/Comparators.h"
#include "pql/Estimation.h"
#include "pql/Evaluator.h"
#include "pql/QueryException.h"
#include "utils/Element.h"

namespace spa {

namespace pql {

Modifies::Modifies(std::shared_ptr<Ref> subject,
                   std::shared_ptr<EntityRef> target)
    : Relational(RelType::kModifies), subject_{subject}, target_{target} {
  if (!this->subject_ || !this->target_) {
    throw QueryParseException("Subject or target is empty");
  }

  subject_is_stmt_ = subject_->IsStatement();
};

Modifies::Modifies(uint32_t stmt_no, std::string var_name)
    : Modifies(std::make_shared<StatementRef>(stmt_no),
               std::make_shared<EntityRef>(var_name)) {}

Modifies::Modifies(std::string proc_name, std::string var_name)
    : Modifies(std::make_shared<EntityRef>(proc_name),
               std::make_shared<EntityRef>(var_name)) {}

std::vector<std::shared_ptr<Ref>> Modifies::GetDeclaredRefs() {
  std::vector<std::shared_ptr<Ref>> refs;

  if (subject_->IsDeclared()) refs.push_back(subject_);
  if (target_->IsDeclared()) refs.push_back(target_);

  return refs;
}

Estimation Modifies::ComputeEstimation() {
  auto& evaluator = Evaluator::GetInstance();
  if (subject_is_stmt_) {
    auto& cache = evaluator.GetModifiesSCache();
    return CalcEstimation(cache, subject_, target_);
  } else {
    auto& cache = evaluator.GetModifiesPCache();
    return CalcEstimation(cache, subject_, target_);
  }
}

bool Modifies::Test(std::vector<utils::Element>& tuple,
                    std::vector<size_t>& attr_idx) {
  auto& evaluator = Evaluator::GetInstance();
  if (subject_is_stmt_) {
    auto& cache = evaluator.GetModifiesSCache();
    return TestTuple(cache, subject_, target_, tuple, attr_idx);
  } else {
    auto& cache = evaluator.GetModifiesPCache();
    return TestTuple(cache, subject_, target_, tuple, attr_idx);
  }
}

std::shared_ptr<utils::Batch> Modifies::Fetch() {
  auto& evaluator = Evaluator::GetInstance();
  if (subject_is_stmt_) {
    auto& cache = evaluator.GetModifiesSCache();
    return BuildBatch(cache, subject_, target_);
  } else {
    auto& cache = evaluator.GetModifiesPCache();
    return BuildBatch(cache, subject_, target_);
  }
}

bool Modifies::Equals(Clause const& other) const {
  if (typeid(*this) != typeid(other)) return false;
  auto that = static_cast<Modifies const&>(other);
  PtrComparator<Ref> ref_comparator;
  return typeid(this->subject_) ==
             typeid(that.subject_)  // compare subject type
         && ref_comparator(this->subject_, that.subject_) &&
         ref_comparator(this->target_, that.target_);
}

std::ostream& Modifies::ToString(std::ostream& out) const {
  out << "[Modifies: subject: " << *this->subject_
      << " target: " << *this->target_ << "]";
  return out;
}

}  // namespace pql

}  // namespace spa
