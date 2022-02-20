#include "QueryData.h"

#include <algorithm>

#include "catch.hpp"
#include "clause/AffectsBipData.h"
#include "clause/AffectsBipTData.h"
#include "clause/AffectsData.h"
#include "clause/AffectsTData.h"
#include "clause/CallsData.h"
#include "clause/CallsTData.h"
#include "clause/CompareData.h"
#include "clause/FollowsData.h"
#include "clause/FollowsTData.h"
#include "clause/ModifiesData.h"
#include "clause/NextBipData.h"
#include "clause/NextBipTData.h"
#include "clause/NextData.h"
#include "clause/NextTData.h"
#include "clause/ParentData.h"
#include "clause/ParentTData.h"
#include "clause/PatternData.h"
#include "clause/UsesData.h"

namespace test {

namespace pql {

namespace data {

using namespace spa::pql;

const auto kGetStrFunc = [](const ClausesPair& cp) { return cp.string_func_; };

const std::vector<ClausesPair> GetRelationClausesPositive(
    Declaration::Type declaration_type) {
  std::vector<ClausesPair> clauses;
  auto follows_clauses = follows::kGetClausesPositive(declaration_type);
  auto followst_clauses = followst::kGetClausesPositive(declaration_type);
  auto parent_clauses = parent::kGetClausesPositive(declaration_type);
  auto parentt_clauses = parentt::kGetClausesPositive(declaration_type);
  auto uses_clauses = uses::kGetClausesPositive(declaration_type);
  auto modifies_clauses = modifies::kGetClausesPositive(declaration_type);
  auto affectsbip_clauses = affectsbip::kGetClausesPositive(declaration_type);
  auto affectsbipt_clauses = affectsbipt::kGetClausesPositive(declaration_type);
  auto affects_clauses = affects::kGetClausesPositive(declaration_type);
  auto affectst_clauses = affectst::kGetClausesPositive(declaration_type);
  auto calls_clauses = calls::kGetClausesPositive(declaration_type);
  auto callst_clauses = callst::kGetClausesPositive(declaration_type);
  auto nextbip_clauses = nextbip::kGetClausesPositive(declaration_type);
  auto nextbipt_clauses = nextbipt::kGetClausesPositive(declaration_type);
  auto next_clauses = next::kGetClausesPositive(declaration_type);
  auto nextt_clauses = nextt::kGetClausesPositive(declaration_type);

  clauses.insert(clauses.end(), follows_clauses.begin(), follows_clauses.end());
  clauses.insert(clauses.end(), followst_clauses.begin(),
                 followst_clauses.end());
  clauses.insert(clauses.end(), parent_clauses.begin(), parent_clauses.end());
  clauses.insert(clauses.end(), parentt_clauses.begin(), parentt_clauses.end());
  clauses.insert(clauses.end(), uses_clauses.begin(), uses_clauses.end());
  clauses.insert(clauses.end(), modifies_clauses.begin(),
                 modifies_clauses.end());
  clauses.insert(clauses.end(), affectsbip_clauses.begin(),
                 affectsbip_clauses.end());
  clauses.insert(clauses.end(), affectsbipt_clauses.begin(),
                 affectsbipt_clauses.end());
  clauses.insert(clauses.end(), affects_clauses.begin(), affects_clauses.end());
  clauses.insert(clauses.end(), affectst_clauses.begin(),
                 affectst_clauses.end());
  clauses.insert(clauses.end(), calls_clauses.begin(), calls_clauses.end());
  clauses.insert(clauses.end(), callst_clauses.begin(), callst_clauses.end());
  clauses.insert(clauses.end(), nextbip_clauses.begin(), nextbip_clauses.end());
  clauses.insert(clauses.end(), nextbipt_clauses.begin(),
                 nextbipt_clauses.end());
  clauses.insert(clauses.end(), next_clauses.begin(), next_clauses.end());
  clauses.insert(clauses.end(), nextt_clauses.begin(), nextt_clauses.end());

  return clauses;
}

const std::vector<StringToStringFunc> GetRelationFuncsPositive(
    Declaration::Type declaration_type) {
  std::vector<ClausesPair> relational_clauses =
      GetRelationClausesPositive(declaration_type);

  std::vector<StringToStringFunc> relational_funcs_positive;

  std::transform(relational_clauses.begin(), relational_clauses.end(),
                 std::back_inserter(relational_funcs_positive), kGetStrFunc);

  return relational_funcs_positive;
}

const std::vector<StringToStringFunc> GetPatternFuncsPositive(
    Declaration::Type declaration_type) {
  std::vector<ClausesPair> pattern_clauses =
      pattern::kGetClausesPositive(declaration_type);

  std::vector<StringToStringFunc> pattern_funcs_positive;

  std::transform(pattern_clauses.begin(), pattern_clauses.end(),
                 std::back_inserter(pattern_funcs_positive), kGetStrFunc);

  return pattern_funcs_positive;
}

const std::vector<StringToStringFunc> GetCompareFuncsPositive(
    Declaration::Type declaration_type) {
  std::vector<ClausesPair> compare_clauses =
      compare::kGetClausesPositive(declaration_type);

  std::vector<StringToStringFunc> compare_funcs_positive;

  std::transform(compare_clauses.begin(), compare_clauses.end(),
                 std::back_inserter(compare_funcs_positive), kGetStrFunc);

  return compare_funcs_positive;
}

const std::vector<StringToStringFunc> GetRelationFuncsNegative(
    Declaration::Type declaration_type) {
  std::vector<StringToStringFunc> funcs;
  auto follows_funcs = follows::kGetFuncsNegative(declaration_type);
  auto followst_funcs = followst::kGetFuncsNegative(declaration_type);
  auto parent_funcs = parent::kGetFuncsNegative(declaration_type);
  auto parentt_funcs = parentt::kGetFuncsNegative(declaration_type);
  auto uses_funcs = uses::kGetFuncsNegative(declaration_type);
  auto modifies_funcs = modifies::kGetFuncsNegative(declaration_type);
  auto affectsbip_funcs = affectsbip::kGetFuncsNegative(declaration_type);
  auto affectsbipt_funcs = affectsbipt::kGetFuncsNegative(declaration_type);
  auto affects_funcs = affects::kGetFuncsNegative(declaration_type);
  auto affectst_funcs = affectst::kGetFuncsNegative(declaration_type);
  auto calls_funcs = calls::kGetFuncsNegative(declaration_type);
  auto callst_funcs = callst::kGetFuncsNegative(declaration_type);
  auto nextbip_funcs = nextbip::kGetFuncsNegative(declaration_type);
  auto nextbipt_funcs = nextbipt::kGetFuncsNegative(declaration_type);
  auto next_funcs = next::kGetFuncsNegative(declaration_type);
  auto nextt_funcs = nextt::kGetFuncsNegative(declaration_type);

  funcs.insert(funcs.end(), follows_funcs.begin(), follows_funcs.end());
  funcs.insert(funcs.end(), followst_funcs.begin(), followst_funcs.end());
  funcs.insert(funcs.end(), parent_funcs.begin(), parent_funcs.end());
  funcs.insert(funcs.end(), parentt_funcs.begin(), parentt_funcs.end());
  funcs.insert(funcs.end(), uses_funcs.begin(), uses_funcs.end());
  funcs.insert(funcs.end(), modifies_funcs.begin(), modifies_funcs.end());
  funcs.insert(funcs.end(), affectsbip_funcs.begin(), affectsbip_funcs.end());
  funcs.insert(funcs.end(), affectsbipt_funcs.begin(), affectsbipt_funcs.end());
  funcs.insert(funcs.end(), affects_funcs.begin(), affects_funcs.end());
  funcs.insert(funcs.end(), affectst_funcs.begin(), affectst_funcs.end());
  funcs.insert(funcs.end(), calls_funcs.begin(), calls_funcs.end());
  funcs.insert(funcs.end(), callst_funcs.begin(), callst_funcs.end());
  funcs.insert(funcs.end(), nextbip_funcs.begin(), nextbip_funcs.end());
  funcs.insert(funcs.end(), nextbipt_funcs.begin(), nextbipt_funcs.end());
  funcs.insert(funcs.end(), next_funcs.begin(), next_funcs.end());
  funcs.insert(funcs.end(), nextt_funcs.begin(), nextt_funcs.end());

  return funcs;
}

const std::vector<ClausesPair> kGetClausesPositive(
    Declaration::Type declaration_type) {
  std::vector<ClausesPair> clauses;
  std::vector<ClausesPair> relational_clauses =
      GetRelationClausesPositive(declaration_type);
  std::vector<ClausesPair> pattern_clauses =
      pattern::kGetClausesPositive(declaration_type);
  std::vector<ClausesPair> compare_clauses =
      compare::kGetClausesPositive(declaration_type);

  for (auto relational_clause : relational_clauses) {
    for (auto pattern_clause : pattern_clauses) {
      for (auto compare_clause : compare_clauses) {
        // ... such that ... pattern ... with ...
        clauses.push_back(ClausesPair::Combine(
            ClausesPair::Combine(relational_clause, pattern_clause),
            compare_clause));
        // ... pattern ... with ... such that ...
        clauses.push_back(ClausesPair::Combine(
            ClausesPair::Combine(pattern_clause, compare_clause),
            relational_clause));
      }
    }
  }

  return clauses;
}

const std::vector<StringToStringFunc> kGetFuncsNegative(
    Declaration::Type declaration_type) {
  std::vector<StringToStringFunc> funcs;
  std::vector<StringToStringFunc> relational_funcs_positive =
      GetRelationFuncsPositive(declaration_type);
  std::vector<StringToStringFunc> pattern_funcs_positive =
      GetPatternFuncsPositive(declaration_type);
  std::vector<StringToStringFunc> compare_funcs_positive =
      GetCompareFuncsPositive(declaration_type);
  std::vector<StringToStringFunc> relational_funcs_negative =
      GetRelationFuncsNegative(declaration_type);
  std::vector<StringToStringFunc> pattern_funcs_negative =
      pattern::kGetFuncsNegative(declaration_type);
  std::vector<StringToStringFunc> compare_funcs_negative =
      compare::kGetFuncsNegative(declaration_type);

  // negative relation tests
  for (auto relational_func : relational_funcs_negative) {
    for (auto pattern_func : pattern_funcs_positive) {
      for (auto compare_func : compare_funcs_positive) {
        // ... such that ... with ... pattern ...
        funcs.push_back(CombineStrFunc(
            CombineStrFunc(relational_func, compare_func), pattern_func));
        // ... pattern ... such that ... with ...
        funcs.push_back(CombineStrFunc(
            CombineStrFunc(pattern_func, relational_func), compare_func));
      }
    }
  }

  // negative pattern tests
  for (auto relational_func : relational_funcs_positive) {
    for (auto pattern_func : pattern_funcs_negative) {
      for (auto compare_func : compare_funcs_positive) {
        // with ... such that ... pattern ...
        funcs.push_back(CombineStrFunc(
            CombineStrFunc(compare_func, relational_func), pattern_func));
        // ... pattern ... with ... such that ...
        funcs.push_back(CombineStrFunc(
            CombineStrFunc(pattern_func, compare_func), relational_func));
      }
    }
  }

  // negative compare tests
  for (auto relational_func : relational_funcs_positive) {
    for (auto pattern_func : pattern_funcs_positive) {
      for (auto compare_func : compare_funcs_negative) {
        // ... such that ... pattern ... with ...
        funcs.push_back(CombineStrFunc(
            CombineStrFunc(relational_func, pattern_func), compare_func));
        // with ... pattern ... such that ...
        funcs.push_back(CombineStrFunc(
            CombineStrFunc(compare_func, pattern_func), relational_func));
      }
    }
  }

  return funcs;
}

}  // namespace data

}  // namespace pql

}  // namespace test
