#include "Follows.h"

#include "pql/Comparators.h"
#include "pql/Estimation.h"
#include "pql/Evaluator.h"
#include "pql/QueryException.h"
#include "utils/Element.h"

namespace spa {

namespace pql {

Follows::Follows(std::shared_ptr<StatementRef> pred,
                 std::shared_ptr<StatementRef> succ, bool transitive)
    : Relational(transitive ? RelType::kFollowsT : RelType::kFollows),
      pred_{pred},
      succ_{succ},
      transitive_{transitive} {
  if (!this->pred_ || !this->succ_) {
    throw QueryParseException("Predecessor or successor is empty");
  }

  equal_decl_ = pred_->IsDeclared() && succ_->IsDeclared() && *pred == *succ_;
};

Follows::Follows(uint32_t pred, uint32_t succ, bool transitive)
    : Follows(std::make_shared<StatementRef>(pred),
              std::make_shared<StatementRef>(succ), transitive) {}

std::vector<std::shared_ptr<Ref>> Follows::GetDeclaredRefs() {
  std::vector<std::shared_ptr<Ref>> refs;

  if (pred_->IsDeclared()) refs.push_back(pred_);

  // Only add the second argument if it is declared and not equal to the
  // predecessor (if the predecessor is also declared).
  if (succ_->IsDeclared() && !equal_decl_) refs.push_back(succ_);

  return refs;
}

Estimation Follows::ComputeEstimation() {
  if (equal_decl_) {
    return {0, {0}};  // no statement can follow itself
  }

  auto& evaluator = Evaluator::GetInstance();
  auto& cache = evaluator.GetFollowsCache(transitive_);
  return CalcEstimation(cache, pred_, succ_);
}

bool Follows::Test(std::vector<utils::Element>& tuple,
                   std::vector<size_t>& attr_idx) {
  if (equal_decl_) return false;

  auto& evaluator = Evaluator::GetInstance();
  auto& cache = evaluator.GetFollowsCache(transitive_);
  return TestTuple(cache, pred_, succ_, tuple, attr_idx);
}

std::shared_ptr<utils::Batch> Follows::Fetch() {
  if (equal_decl_) return std::make_shared<utils::Batch>();

  auto& evaluator = Evaluator::GetInstance();
  auto& cache = evaluator.GetFollowsCache(transitive_);
  return BuildBatch(cache, pred_, succ_);
}

bool Follows::Equals(Clause const& other) const {
  if (typeid(*this) != typeid(other)) return false;
  auto that = static_cast<Follows const&>(other);
  PtrComparator<StatementRef> statement_ref_comparator;
  return this->transitive_ == that.transitive_ &&
         statement_ref_comparator(this->pred_, that.pred_) &&
         statement_ref_comparator(this->succ_, that.succ_);
}

std::ostream& Follows::ToString(std::ostream& out) const {
  out << "[Follows: transitive: " << this->transitive_
      << " pred: " << *this->pred_ << " succ: " << *this->succ_ << "]";
  return out;
}

}  // namespace pql

}  // namespace spa
