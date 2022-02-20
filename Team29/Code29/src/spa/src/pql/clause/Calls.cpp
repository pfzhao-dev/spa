#include "Calls.h"

#include "pql/Comparators.h"
#include "pql/Estimation.h"
#include "pql/Evaluator.h"
#include "pql/QueryException.h"
#include "utils/Element.h"

namespace spa {

namespace pql {

Calls::Calls(std::shared_ptr<EntityRef> caller,
             std::shared_ptr<EntityRef> callee, bool transitive)
    : Relational(transitive ? RelType::kCallsT : RelType::kCalls),
      caller_{caller},
      callee_{callee},
      transitive_{transitive} {
  if (!this->caller_ || !this->callee_) {
    throw QueryParseException("Caller or callee ref is empty");
  }

  equal_decl_ =
      caller_->IsDeclared() && callee_->IsDeclared() && *caller_ == *callee_;
};

Calls::Calls(std::string caller, std::string callee, bool transitive)
    : Calls(std::make_shared<EntityRef>(caller),
            std::make_shared<EntityRef>(callee), transitive) {}

std::vector<std::shared_ptr<Ref>> Calls::GetDeclaredRefs() {
  std::vector<std::shared_ptr<Ref>> refs;

  if (caller_->IsDeclared()) refs.push_back(caller_);

  // Only add the callee if it is declared and not equal to the
  // caller (if the caller is also declared).
  if (callee_->IsDeclared() && !equal_decl_) refs.push_back(callee_);

  return refs;
}

Estimation Calls::ComputeEstimation() {
  if (equal_decl_) {
    return {0, {0}};  // no procedure can call itself
  }

  auto& evaluator = Evaluator::GetInstance();
  auto& cache = evaluator.GetCallsCache(transitive_);
  return CalcEstimation(cache, caller_, callee_);
}

bool Calls::Test(std::vector<utils::Element>& tuple,
                 std::vector<size_t>& attr_idx) {
  if (equal_decl_) return false;

  auto& evaluator = Evaluator::GetInstance();
  auto& cache = evaluator.GetCallsCache(transitive_);
  return TestTuple(cache, caller_, callee_, tuple, attr_idx);
}

std::shared_ptr<utils::Batch> Calls::Fetch() {
  if (equal_decl_) return std::make_shared<utils::Batch>();

  auto& evaluator = Evaluator::GetInstance();
  auto& cache = evaluator.GetCallsCache(transitive_);
  return BuildBatch(cache, caller_, callee_);
}

bool Calls::Equals(Clause const& other) const {
  if (typeid(*this) != typeid(other)) return false;
  auto that = static_cast<Calls const&>(other);
  PtrComparator<EntityRef> entity_ref_comparator;
  return this->transitive_ == that.transitive_ &&
         entity_ref_comparator(this->caller_, that.caller_) &&
         entity_ref_comparator(this->callee_, that.callee_);
}

std::ostream& Calls::ToString(std::ostream& out) const {
  out << "[Calls: transitive: " << this->transitive_
      << " caller: " << *this->caller_ << " callee: " << *this->callee_ << "]";
  return out;
}

}  // namespace pql

}  // namespace spa
