#include "Clause.h"

#include <cmath>

#include "ast/Statement.h"
#include "pql/Declaration.h"
#include "pql/Estimation.h"
#include "pql/Evaluator.h"
#include "pql/QueryException.h"
#include "utils/Translator.h"

namespace spa {

namespace pql {

Clause::Clause(Type type) : type_{type} {}

Clause::Type Clause::GetType() { return type_; }

bool Clause::operator==(const Clause& other) const {
  return this->Equals(other);
}

bool Clause::operator!=(const Clause& other) const { return !(*this == other); }

std::ostream& operator<<(std::ostream& out, const Clause& clause) {
  return clause.ToString(out);
}

void Clause::PrepareEval() {
  if (has_prepared_) {
    return;
  }

  has_prepared_ = true;
  PrepareComputation();
}

void Clause::PrepareComputation() {
  // By default, this function does nothing.
  return;
}

std::vector<utils::Attribute> Clause::GetAttrs() {
  if (has_attrs_cache_) {
    return attrs_;
  }

  has_attrs_cache_ = true;
  auto decl_refs = GetDeclaredRefs();
  for (const auto& ref : decl_refs) {
    attrs_.push_back(utils::Attribute(ref->GetDeclaration()));
  }

  return attrs_;
}

int Clause::GetPriorityScore() { return 0; }

Estimation Clause::GetEstimation() {
  if (has_query_estimation_cache_) {
    return query_estimation_;
  }

  has_query_estimation_cache_ = true;
  query_estimation_ = ComputeEstimation();

  return query_estimation_;
}

Relational::Relational(RelType type)
    : Clause(Type::kRelational), rel_type_(type) {}

Relational::RelType Relational::GetRelType() { return rel_type_; }

bool Relational::IsRelTypeIn(std::vector<RelType> rel_types) {
  for (auto type : rel_types) {
    if (type == rel_type_) return true;
  }
  return false;
}

int Relational::GetPriorityScore() {
  auto& evaluator = Evaluator::GetInstance();
  auto default_score = kRelTypeToPriorityScore.at(rel_type_);
  if (default_score == 0) return default_score;

  // If the default score is not zero, but turns out the result has been
  // precomputed (e.g., due to the presence of a wildcard first arg),
  // then we just adjust the score to 0.

  if (rel_type_ == RelType::kAffects) {
    if (evaluator.HasPopulateAffects(false, false)) return 0;
  } else if (rel_type_ == RelType::kAffectsT) {
    if (evaluator.HasPopulateAffects(true, false)) return 0;
  } else if (rel_type_ == RelType::kAffectsBip) {
    if (evaluator.HasPopulateAffects(false, true)) return 0;
  } else if (rel_type_ == RelType::kAffectsBipT) {
    if (evaluator.HasPopulateAffects(true, true)) return 0;
  } else if (rel_type_ == RelType::kNextT) {
    if (evaluator.HasPopulateNext(true, false)) return 0;
  } else if (rel_type_ == RelType::kNextBip) {
    if (evaluator.HasPopulateNext(false, true)) return 0;
  } else if (rel_type_ == RelType::kNextBipT) {
    if (evaluator.HasPopulateNext(true, true)) return 0;
  }

  return default_score;
}

const std::unordered_map<std::string, Relational::RelType>
    kStringToRelationalType = {
        {"Follows", Relational::RelType::kFollows},
        {"Follows*", Relational::RelType::kFollowsT},
        {"Parent", Relational::RelType::kParent},
        {"Parent*", Relational::RelType::kParentT},
        {"Uses", Relational::RelType::kUses},
        {"Modifies", Relational::RelType::kModifies},
        {"Affects", Relational::RelType::kAffects},
        {"Affects*", Relational::RelType::kAffectsT},
        {"AffectsBip", Relational::RelType::kAffectsBip},
        {"AffectsBip*", Relational::RelType::kAffectsBipT},
        {"Calls", Relational::RelType::kCalls},
        {"Calls*", Relational::RelType::kCallsT},
        {"Next", Relational::RelType::kNext},
        {"Next*", Relational::RelType::kNextT},
        {"NextBip", Relational::RelType::kNextBip},
        {"NextBip*", Relational::RelType::kNextBipT},
};

const std::unordered_map<Relational::RelType, int> kRelTypeToPriorityScore = {
    // Pre-computed clauses
    {Relational::RelType::kFollows, 0},
    {Relational::RelType::kFollowsT, 0},
    {Relational::RelType::kParent, 0},
    {Relational::RelType::kParentT, 0},
    {Relational::RelType::kUses, 0},
    {Relational::RelType::kModifies, 0},
    {Relational::RelType::kCalls, 0},
    {Relational::RelType::kCallsT, 0},
    // Next is very lightweight, hence can be considered precomputed
    {Relational::RelType::kNext, 0},
    // Based on order
    {Relational::RelType::kNextBip, 1},
    {Relational::RelType::kNextT, 2},
    {Relational::RelType::kNextBipT, 3},
    {Relational::RelType::kAffects, 4},
    {Relational::RelType::kAffectsBip, 5},
    // Latest priority because we may need to construct another graph.
    {Relational::RelType::kAffectsT, 6},
    {Relational::RelType::kAffectsBipT, 7},
};

}  // namespace pql

}  // namespace spa
