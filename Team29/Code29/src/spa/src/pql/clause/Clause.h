#pragma once

#include <cmath>
#include <iostream>
#include <memory>
#include <unordered_set>
#include <vector>

#include "pkb/Cache.h"
#include "pql/Estimation.h"
#include "pql/Ref.h"
#include "utils/Attribute.h"
#include "utils/Batch.h"
#include "utils/Element.h"

namespace spa {

namespace pql {

class Clause {
 public:
  enum class Type {
    kCompare,
    kPattern,
    kRelational,
  };

  Clause(Type type);

  bool operator==(const Clause& other) const;
  bool operator!=(const Clause& other) const;

  friend std::ostream& operator<<(std::ostream& out, const Clause& ref);

  // Prepare the clause evaluation. For example, some clauses like
  // Affects or Next may want to precompute their value if necessary.
  //
  // The function call will be cached, and each clause can only prepare
  // evaluation once.
  void PrepareEval();

  // Fetch batch of possible attribute values for this particular clause.
  virtual std::shared_ptr<utils::Batch> Fetch() = 0;

  // Get table attributes that could be retrieved from this clause.
  std::vector<utils::Attribute> GetAttrs();

  // Estimate the computation cost to fully execute this clause.
  Estimation GetEstimation();

  Type GetType();

  // Higher priority score indicates that a clause may take longer to be
  // fetched. All the clauses that can be precomputed will have priority 0.
  // Meanwhile, a clause like AffectsBip* may have one of the largest values.
  virtual int GetPriorityScore();

  // Test whether the given tuple satisfy the constraint represented by the
  // the clause object. This function takes in the reference to the tuple data
  // and attribute indices that correspond to the clause based on the `GetAttrs`
  // function for efficiency reasons.
  //
  // TODO: Re-evaluate the design decision on what to do when there is a
  //       wildcard entity in the clause.
  //
  // NOTE: This function should not be used at the moment.
  virtual bool Test(std::vector<utils::Element>& tuple,
                    std::vector<size_t>& attr_idx) = 0;

  // Convenience function for testing. All of the references must be concrete.
  bool T() {
    PrepareEval();

    // NOTE: The references must be concrete, hence the following vectors
    //       should not be accessed at all during evaluation. This is a
    //       convenience function for unit testing.
    std::vector<utils::Element> dummy_tuple;
    std::vector<size_t> dummy_attr_idx;
    return Test(dummy_tuple, dummy_attr_idx);
  }

 protected:
  virtual void PrepareComputation();

  virtual std::vector<std::shared_ptr<Ref>> GetDeclaredRefs() = 0;

  virtual Estimation ComputeEstimation() = 0;

  virtual bool Equals(Clause const& other) const = 0;

  virtual std::ostream& ToString(std::ostream& out) const = 0;

  // Helper functions

  template <typename T1, typename T2>
  Estimation CalcEstimation(pkb::Cache<T1, T2>& cache, std::shared_ptr<Ref> r1,
                            std::shared_ptr<Ref> r2) {
    bool r1_wild = r1->IsWild();
    bool r1_decl = r1->IsDeclared();
    bool r2_wild = r2->IsWild();
    bool r2_decl = r2->IsDeclared();

    Estimation estimation = {};  // default prediction: 0 row, 0 col

    // Exhaustively consider all possible cases
    // Let use assume the following notation for the clause arguments:
    // - _: wildcard
    // - s: synonym
    // - v: value

    if (r1_wild && r2_wild) {                    // Handle (_, _)
      if (!cache.Empty()) estimation = {1, {}};  // Add empty tuple

    } else if (r1_decl && r2_wild) {  // Handle (s, _)
      auto& values = cache.GetOriginalKeys();

      // Estimate the number of rows. If the ref is of type statement, this
      // will scale based on the ratio of stmt type w.r.t. total statement.
      EstimationValue rows =
          std::ceil((double)values.size() * r1->GetEstimationScale());
      estimation = {rows, {rows}};

    } else if (!r1_decl && r2_wild) {  // Handle (v, _)
      auto elem = r1->GetValue();
      if (cache.CountOriginal(std::get<T1>(elem))) {
        estimation = {1, {}};
      }

    } else if (r1_wild && r2_decl) {  // Handle (_, s)
      auto& values = cache.GetReverseKeys();

      EstimationValue rows =
          std::ceil((double)values.size() * r2->GetEstimationScale());
      estimation = {rows, {rows}};

    } else if (r1_wild && !r2_decl) {  // Handle (_, v)
      auto elem = r2->GetValue();
      if (cache.CountReverse(std::get<T2>(elem))) {
        estimation = {1, {}};
      }

    } else if (r1_decl && !r2_decl) {  // Handle (s, v)
      auto r2_elem = r2->GetValue();
      if (cache.CountReverse(std::get<T2>(r2_elem))) {
        auto& values = cache.GetReverseValues(std::get<T2>(r2_elem));

        EstimationValue rows =
            std::ceil((double)values.size() * r1->GetEstimationScale());
        estimation = {rows, {rows}};
      } else {
        estimation = {0, {0}};
      }

    } else if (!r1_decl && r2_decl) {  // Handle (v, s)
      auto r1_elem = r1->GetValue();
      if (cache.CountOriginal(std::get<T1>(r1_elem))) {
        auto& values = cache.GetOriginalValues(std::get<T1>(r1_elem));

        EstimationValue rows =
            std::ceil((double)values.size() * r2->GetEstimationScale());
        estimation = {rows, {rows}};
      } else {
        estimation = {0, {0}};
      }

    } else if (!r1_decl && !r2_decl) {  // Handle (v, v)
      auto r1_elem = r1->GetValue();
      auto r2_elem = r2->GetValue();
      if (cache.Count(std::get<T1>(r1_elem), std::get<T2>(r2_elem))) {
        estimation = {1, {}};
      }

    } else {  // Handle (s, s)
      auto size = cache.Size();
      auto& r1_values = cache.GetOriginalKeys();
      auto& r2_values = cache.GetReverseKeys();

      // Scaler
      auto s1 = r1->GetEstimationScale();
      auto s2 = r2->GetEstimationScale();

      // Adjust the exected number of rows and distinct values per column
      EstimationValue rows = std::ceil((double)size * s1 * s2);
      EstimationValue c1 = std::ceil((double)r1_values.size() * s1);
      EstimationValue c2 = std::ceil((double)r2_values.size() * s2);

      if (*r1 == *r2) {
        rows = std::min(c1, c2);
        estimation = {rows, {rows}};
      } else {
        estimation = {rows, {c1, c2}};
      }
    }

    return estimation;
  }

  template <typename T1, typename T2>
  std::shared_ptr<utils::Batch> BuildBatch(pkb::Cache<T1, T2>& cache,
                                           std::shared_ptr<Ref> r1,
                                           std::shared_ptr<Ref> r2) {
    bool r1_wild = r1->IsWild();
    bool r1_decl = r1->IsDeclared();
    bool r2_wild = r2->IsWild();
    bool r2_decl = r2->IsDeclared();

    auto batch = std::make_shared<utils::Batch>();

    // Exhaustively consider all possible cases
    // Let use assume the following notation for the clause arguments:
    // - _: wildcard
    // - s: synonym
    // - v: value

    if (r1_wild && r2_wild) {            // Handle (_, _)
      if (!cache.Empty()) batch->Add();  // Add empty tuple

    } else if (r1_decl && r2_wild) {  // Handle (s, _)
      auto& values = cache.GetOriginalKeys();
      for (auto& value : values)
        if (r1->Test(value)) batch->Add(value);

    } else if (!r1_decl && r2_wild) {  // Handle (v, _)
      auto elem = r1->GetValue();
      if (cache.CountOriginal(std::get<T1>(elem))) batch->Add();

    } else if (r1_wild && r2_decl) {  // Handle (_, s)
      auto& values = cache.GetReverseKeys();
      for (auto& value : values)
        if (r2->Test(value)) batch->Add(value);

    } else if (r1_wild && !r2_decl) {  // Handle (_, v)
      auto elem = r2->GetValue();
      if (cache.CountReverse(std::get<T2>(elem))) batch->Add();

    } else if (r1_decl && !r2_decl) {  // Handle (s, v)
      auto r2_elem = r2->GetValue();
      if (cache.CountReverse(std::get<T2>(r2_elem))) {
        auto& values = cache.GetReverseValues(std::get<T2>(r2_elem));
        for (auto& value : values)
          if (r1->Test(value)) batch->Add(value);
      }

    } else if (!r1_decl && r2_decl) {  // Handle (v, s)
      auto r1_elem = r1->GetValue();
      if (cache.CountOriginal(std::get<T1>(r1_elem))) {
        auto& values = cache.GetOriginalValues(std::get<T1>(r1_elem));
        for (auto& value : values)
          if (r2->Test(value)) batch->Add(value);
      }

    } else if (!r1_decl && !r2_decl) {  // Handle (v, v)
      auto r1_elem = r1->GetValue();
      auto r2_elem = r2->GetValue();
      if (cache.Count(std::get<T1>(r1_elem), std::get<T2>(r2_elem)))
        batch->Add();

    } else {  // Handle (s, s)
      auto result_map = cache.GetOriginalMap();

      // This is actually only possible when T1 = T2
      bool equal_refs = *r1 == *r2;

      for (auto& [r1_value, r2_values] : result_map) {
        if (!r1->Test(r1_value)) continue;

        for (auto& r2_value : r2_values) {
          if (!r2->Test(r2_value)) continue;

          utils::Element r1_elem = r1_value;
          utils::Element r2_elem = r2_value;

          // Continue if they share the same ref, but not have equal value.
          // TODO: This is kinda inefficient that we need to convert to elem
          //       first. Maybe can use preprocessor to compare?
          if (equal_refs && r1_elem != r2_elem) continue;

          std::vector<utils::Element> tuple;
          if (equal_refs)
            tuple = {r1_elem};
          else
            tuple = {r1_elem, r2_elem};

          batch->Add(std::move(tuple));
        }
      }
    }

    return batch;
  }

  // TODO: Shall we make this inline?
  // Inline function can avoid the extra cost of calling this so many times.
  // Main reason is because this will be called for almost every tuple.
  template <typename T1, typename T2>
  bool TestTuple(pkb::Cache<T1, T2>& cache, std::shared_ptr<Ref> r1,
                 std::shared_ptr<Ref> r2, std::vector<utils::Element>& tuple,
                 std::vector<size_t>& attr_idx) {
    bool r1_wild = r1->IsWild();
    bool r1_decl = r1->IsDeclared();
    bool r2_wild = r2->IsWild();
    bool r2_decl = r2->IsDeclared();

    // Exhaustively consider all possible cases
    // Let use assume the following notation for the clause arguments:
    // - _: wildcard
    // - s: synonym
    // - v: value

    bool result = false;

    if (r1_wild && r2_wild) {  // Handle (_, _)
      result = true;           // Add empty tuple

    } else if (r1_decl && r2_wild) {  // Handle (s, _)
      auto& values = cache.GetOriginalKeys();
      auto r1_value = std::get<T1>(tuple[attr_idx[0]]);
      result = values.count(r1_value);  // assume alr fetched by synonym

    } else if (!r1_decl && r2_wild) {  // Handle (v, _)
      auto elem = r1->GetValue();
      result = cache.CountOriginal(std::get<T1>(elem));

    } else if (r1_wild && r2_decl) {  // Handle (_, s)
      auto& values = cache.GetReverseKeys();
      auto r2_value = std::get<T2>(tuple[attr_idx[0]]);
      result = values.count(r2_value);

    } else if (r1_wild && !r2_decl) {  // Handle (_, v)
      auto elem = r2->GetValue();
      result = cache.CountReverse(std::get<T2>(elem));

    } else if (r1_decl && !r2_decl) {  // Handle (s, v)
      auto r2_elem = r2->GetValue();
      if (cache.CountReverse(std::get<T2>(r2_elem))) {
        auto& values = cache.GetReverseValues(std::get<T2>(r2_elem));
        auto r1_value = std::get<T1>(tuple[attr_idx[0]]);
        result = values.count(r1_value);
      }

    } else if (!r1_decl && r2_decl) {  // Handle (v, s)
      auto r1_elem = r1->GetValue();
      if (cache.CountOriginal(std::get<T1>(r1_elem))) {
        auto& values = cache.GetOriginalValues(std::get<T1>(r1_elem));
        auto r2_value = std::get<T2>(tuple[attr_idx[0]]);
        result = values.count(r2_value);
      }

    } else if (!r1_decl && !r2_decl) {  // Handle (v, v)
      auto r1_elem = r1->GetValue();
      auto r2_elem = r2->GetValue();
      result = cache.Count(std::get<T1>(r1_elem), std::get<T2>(r2_elem));

    } else {  // Handle (s, s)
      auto result_map = cache.GetOriginalMap();
      auto r1_value = std::get<T1>(tuple[attr_idx[0]]);
      auto r2_value = std::get<T2>(tuple[attr_idx.back()]);
      result = cache.Count(r1_value, r2_value);
    }

    return result;
  }

 private:
  Type type_;

  bool has_prepared_ = false;

  bool has_attrs_cache_ = false;
  std::vector<utils::Attribute> attrs_;

  bool has_query_estimation_cache_ = false;
  Estimation query_estimation_;
};

class Relational : public Clause {
 public:
  enum class RelType {
    kFollows = 0,
    kFollowsT,
    kParent,
    kParentT,
    kUses,
    kModifies,
    kAffects,
    kAffectsT,
    kAffectsBip,
    kAffectsBipT,
    kCalls,
    kCallsT,
    kNext,
    kNextT,
    kNextBip,
    kNextBipT,
  };

  Relational(RelType rel_type);

  RelType GetRelType();

  bool IsRelTypeIn(std::vector<RelType> rel_types);

  int GetPriorityScore() override;

 private:
  RelType rel_type_;
};

extern const std::unordered_map<std::string, Relational::RelType>
    kStringToRelationalType;

extern const std::unordered_map<Relational::RelType, int>
    kRelTypeToPriorityScore;

}  // namespace pql

}  // namespace spa
