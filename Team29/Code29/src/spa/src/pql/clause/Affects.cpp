#include "Affects.h"

#include "pql/Comparators.h"
#include "pql/Estimation.h"
#include "pql/Evaluator.h"
#include "pql/QueryException.h"
#include "utils/Element.h"

namespace spa {

namespace pql {

Affects::Affects(std::shared_ptr<StatementRef> affector,
                 std::shared_ptr<StatementRef> affected, bool transitive,
                 bool is_bip)
    : Relational(transitive
                     ? (is_bip ? RelType::kAffectsBipT : RelType::kAffectsT)
                     : (is_bip ? RelType::kAffectsBip : RelType::kAffects)),
      affector_{affector},
      affected_{affected},
      transitive_{transitive},
      is_bip_{is_bip} {
  if (!this->affector_ || !this->affected_) {
    throw QueryParseException("Affector or affected ref is empty");
  }

  affector_declared_ = affector_->IsDeclared();
  affected_declared_ = affected_->IsDeclared();
  equal_decl_ =
      affector_declared_ && affected_declared_ && *affector_ == *affected_;
};

Affects::Affects(uint32_t affector, uint32_t affected, bool transitive,
                 bool is_bip)
    : Affects(std::make_shared<StatementRef>(affector),
              std::make_shared<StatementRef>(affected), transitive, is_bip) {}

void Affects::PrepareComputation() {
  auto& evaluator = Evaluator::GetInstance();

  if (transitive_) {
    // A transitive Affects depend on the non-transitive one.
    evaluator.PopulateAffects(false, is_bip_);
  }
  if (transitive_ && is_bip_) {
    evaluator.PopulateNext(true, true);  // AffectsBip* depends on NextBip*
  }

  if (affector_->IsWild()) {
    // If it is a wildcard, just precompute everything.
    evaluator.PopulateAffects(transitive_, is_bip_);

  } else if (!affector_declared_ && !affector_->IsWild()) {
    // If it is a concrete value, just populate from the beginning.
    evaluator.PopulateAffects(affector_->GetStmtNo(), transitive_, is_bip_);
  }
}

std::vector<std::shared_ptr<Ref>> Affects::GetDeclaredRefs() {
  std::vector<std::shared_ptr<Ref>> refs;

  if (affector_declared_) refs.push_back(affector_);
  if (!equal_decl_ && affected_declared_) refs.push_back(affected_);

  return refs;
}

Estimation Affects::ComputeEstimation() {
  auto& evaluator = Evaluator::GetInstance();

  // If we need to perform estimation based on this, then all the values
  // should be precomputed.
  //
  // NOTE: For clause projection, this estimation function should not be
  //       called at all.
  evaluator.PopulateAffects(transitive_, is_bip_);

  auto& cache = evaluator.GetAffectsCache(transitive_, is_bip_);
  return CalcEstimation(cache, affector_, affected_);
}

bool Affects::Test(std::vector<utils::Element>& tuple,
                   std::vector<size_t>& attr_idx) {
  auto& evaluator = Evaluator::GetInstance();

  if (affector_declared_) {
    // We only need to call populate if this is an individual task.
    // If wildcard or specific value, the populate function should alr
    // be called from the beginning (i.e., upon preparing evaluation).
    auto affector_value = std::get<uint32_t>(tuple[attr_idx[0]]);
    evaluator.PopulateAffects(affector_value, transitive_, is_bip_);
  }

  auto& cache = evaluator.GetAffectsCache(transitive_, is_bip_);
  return TestTuple(cache, affector_, affected_, tuple, attr_idx);
}

std::shared_ptr<utils::Batch> Affects::Fetch() {
  auto& evaluator = Evaluator::GetInstance();

  evaluator.PopulateAffects(transitive_, is_bip_);  // populate all

  auto& cache = evaluator.GetAffectsCache(transitive_, is_bip_);
  return BuildBatch(cache, affector_, affected_);
}

bool Affects::Equals(Clause const& other) const {
  if (typeid(*this) != typeid(other)) return false;
  auto that = static_cast<Affects const&>(other);
  PtrComparator<StatementRef> statement_ref_comparator;
  return this->transitive_ == that.transitive_ &&
         this->is_bip_ == that.is_bip_ &&
         statement_ref_comparator(this->affector_, that.affector_) &&
         statement_ref_comparator(this->affected_, that.affected_);
}

std::ostream& Affects::ToString(std::ostream& out) const {
  out << "[Affects: transitive: " << this->transitive_
      << " bip: " << this->is_bip_ << " affector: " << *this->affector_
      << " affected: " << *this->affected_ << "]";
  return out;
}

}  // namespace pql

}  // namespace spa
