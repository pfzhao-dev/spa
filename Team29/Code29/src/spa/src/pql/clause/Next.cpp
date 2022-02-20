#include "Next.h"

#include <algorithm>

#include "pql/Comparators.h"
#include "pql/Estimation.h"
#include "pql/Evaluator.h"
#include "pql/QueryException.h"
#include "utils/Element.h"

namespace spa {

namespace pql {

Next::Next(std::shared_ptr<StatementRef> pred,
           std::shared_ptr<StatementRef> succ, bool transitive, bool is_bip)
    : Relational(transitive ? (is_bip ? RelType::kNextBipT : RelType::kNextT)
                            : (is_bip ? RelType::kNextBip : RelType::kNext)),
      pred_{pred},
      succ_{succ},
      transitive_{transitive},
      is_bip_{is_bip} {
  if (!this->pred_ || !this->succ_) {
    throw QueryParseException("Predecessor or successor is empty");
  }

  pred_declared_ = pred_->IsDeclared();
  succ_declared_ = succ_->IsDeclared();
  equal_decl_ = pred_declared_ && succ_declared_ && *pred == *succ_;
};

Next::Next(uint32_t pred, uint32_t succ, bool transitive, bool is_bip)
    : Next(std::make_shared<StatementRef>(pred),
           std::make_shared<StatementRef>(succ), transitive, is_bip) {}

void Next::PrepareComputation() {
  auto& evaluator = Evaluator::GetInstance();
  if (pred_->IsWild()) {
    // If it is a wildcard, just precompute everything.
    evaluator.PopulateNext(transitive_, is_bip_);

  } else if (!pred_declared_ && !pred_->IsWild()) {
    // If it is a concrete value, just populate from the beginning.
    evaluator.PopulateNext(pred_->GetStmtNo(), transitive_, is_bip_);
  }
}

std::vector<std::shared_ptr<Ref>> Next::GetDeclaredRefs() {
  std::vector<std::shared_ptr<Ref>> refs;

  if (pred_declared_) refs.push_back(pred_);
  if (!equal_decl_ && succ_declared_) refs.push_back(succ_);

  return refs;
}

Estimation Next::ComputeEstimation() {
  if (equal_decl_ && !transitive_) {
    return {0, {0}};  // no statement can be a direct next to itself.
  }

  auto& evaluator = Evaluator::GetInstance();

  // If we need to perform estimation based on this, then all the values
  // should be precomputed.
  //
  // NOTE: For clause projection, this estimation function should not be
  //       called at all.
  evaluator.PopulateNext(transitive_, is_bip_);

  auto& cache = evaluator.GetNextCache(transitive_, is_bip_);
  return CalcEstimation(cache, pred_, succ_);
}

bool Next::Test(std::vector<utils::Element>& tuple,
                std::vector<size_t>& attr_idx) {
  if (equal_decl_ && !transitive_) return false;

  auto& evaluator = Evaluator::GetInstance();

  if (pred_declared_) {
    // We only need to call populate if this is an individual task.
    // If wildcard or specific value, the populate function should alr
    // be called from the beginning (i.e., upon preparing evaluation).
    auto pred_value = std::get<uint32_t>(tuple[attr_idx[0]]);
    evaluator.PopulateNext(pred_value, transitive_, is_bip_);
  }

  auto& cache = evaluator.GetNextCache(transitive_, is_bip_);
  return TestTuple(cache, pred_, succ_, tuple, attr_idx);
}

std::shared_ptr<utils::Batch> Next::Fetch() {
  if (equal_decl_ && !transitive_) return std::make_shared<utils::Batch>();

  auto& evaluator = Evaluator::GetInstance();

  evaluator.PopulateNext(transitive_, is_bip_);  // populate all

  auto& cache = evaluator.GetNextCache(transitive_, is_bip_);
  return BuildBatch(cache, pred_, succ_);
}

bool Next::Equals(Clause const& other) const {
  if (typeid(*this) != typeid(other)) return false;
  auto that = static_cast<Next const&>(other);
  PtrComparator<StatementRef> statement_ref_comparator;
  return this->transitive_ == that.transitive_ &&
         this->is_bip_ == that.is_bip_ &&
         statement_ref_comparator(this->pred_, that.pred_) &&
         statement_ref_comparator(this->succ_, that.succ_);
}

std::ostream& Next::ToString(std::ostream& out) const {
  out << "[Next: transitive: " << this->transitive_ << " bip: " << this->is_bip_
      << " pred: " << *this->pred_ << " succ: " << *this->succ_ << "]";
  return out;
}

}  // namespace pql

}  // namespace spa
