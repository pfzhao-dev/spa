#include "Compare.h"

#include <algorithm>
#include <cstdio>

#include "pql/Comparators.h"
#include "pql/Declaration.h"
#include "pql/Estimation.h"
#include "pql/Evaluator.h"
#include "pql/QueryException.h"
#include "utils/Element.h"
#include "utils/Translator.h"

namespace spa {

namespace pql {

Compare::Compare(std::shared_ptr<Ref> lhs, std::shared_ptr<Ref> rhs)
    : Clause(Type::kCompare), lhs_{lhs}, rhs_{rhs} {
  if (!this->lhs_ || !this->rhs_) {
    throw QueryParseException("Lhs or rhs ref is empty");
  }

  lhs_declared_ = lhs_->IsDeclared();
  rhs_declared_ = rhs_->IsDeclared();
  equal_decl_ = lhs_declared_ && rhs_declared_ && *lhs_ == *rhs_;

  // Note that the lhs and rhs return types are guaranteed to be the same
  // by the query parser.
  if (lhs_declared_) {
    auto lhs_decl = lhs_->GetDeclaration();
    if (lhs_decl->GetReturnType() == pql::Declaration::ReturnType::kInteger) {
      is_compare_int_ = true;
    } else {
      is_compare_int_ = false;
    }
  } else {
    if (lhs_->IsStatement()) {
      // Statement ref is guaranteed to hold an integer for non-declared ref.
      is_compare_int_ = true;
    } else {
      is_compare_int_ = false;
    }
  }
};

Compare::Compare(uint32_t lhs, uint32_t rhs)
    : Compare(std::make_shared<StatementRef>(lhs),
              std::make_shared<StatementRef>(rhs)) {}

Compare::Compare(std::string lhs, std::string rhs)
    : Compare(std::make_shared<EntityRef>(lhs),
              std::make_shared<EntityRef>(rhs)) {}

std::vector<std::shared_ptr<Ref>> Compare::GetDeclaredRefs() {
  std::vector<std::shared_ptr<Ref>> refs;

  if (lhs_declared_) refs.push_back(lhs_);
  if (!equal_decl_ && rhs_declared_) refs.push_back(rhs_);

  return refs;
}

Estimation Compare::ComputeEstimation() {
  // Compare clause cannot have wildcard. Hence we can just all
  // possible 4 cases where each arg can either be synonym or value.

  if (!lhs_declared_ & !rhs_declared_) {  // Handle (v, v)
    if (rhs_->Test(lhs_->GetValue()))
      return {1, {}};
    else
      return {0, {}};

  } else if (!lhs_declared_) {  // Handle (v, s)
    if (rhs_->Test(lhs_->GetValue()))
      return {1, {1}};
    else
      return {0, {0}};

  } else if (!rhs_declared_) {  // Handle (s, v)
    if (lhs_->Test(rhs_->GetValue()))
      return {1, {1}};
    else
      return {0, {0}};
  }

  // Handle (s, s)
  //
  // NOTE: Even if we use attr for call, print, or read stmt, the estimation
  //       value is still based on the number of such statements.

  auto lhs_est = GetEstimationValue(lhs_);
  auto rhs_est = GetEstimationValue(rhs_);

  // The expected num of rows is the minimum between lhs and rhs, and we expect
  // lhs and rhs to have the same number of distinct values.
  //
  // NOTE: This prediction may be inaccurate upon using attr for comparison.
  auto num_rows = std::min(lhs_est, rhs_est);

  if (*lhs_ == *rhs_) {
    return {num_rows, {num_rows}};
  } else {
    return {num_rows, {num_rows, num_rows}};
  }
}

bool Compare::Test(std::vector<utils::Element>& tuple,
                   std::vector<size_t>& attr_idx) {
  if (is_compare_int_) {
    uint32_t v1;
    uint32_t v2;

    if (!lhs_declared_)
      v1 = std::get<uint32_t>(lhs_->GetValue());
    else
      v1 = std::get<uint32_t>(tuple[attr_idx[0]]);

    if (!rhs_declared_)
      v2 = std::get<uint32_t>(rhs_->GetValue());
    else
      v2 = std::get<uint32_t>(tuple[attr_idx.back()]);

    return v1 == v2;

  } else {
    std::string s1;
    std::string s2;

    if (!lhs_declared_)
      s1 = std::get<std::string>(lhs_->GetValue());
    else
      s1 = ExtractString(tuple[attr_idx[0]], lhs_);

    if (!rhs_declared_)
      s2 = std::get<std::string>(rhs_->GetValue());
    else
      s2 = ExtractString(tuple[attr_idx.back()], rhs_);

    return s1 == s2;
  }
}

std::shared_ptr<utils::Batch> Compare::Fetch() {
  if (is_compare_int_)
    return HandleFetchInteger();
  else
    return HandleFetchString();
}

std::shared_ptr<utils::Batch> Compare::HandleFetchInteger() {
  auto lhs_values = GetIntegerValues(lhs_);
  auto rhs_values = GetIntegerValues(rhs_);

  auto batch = std::make_shared<utils::Batch>();

  for (const auto& lhs_value : lhs_values) {
    if (rhs_values.count(lhs_value)) {
      std::vector<utils::Element> tuple;
      if (lhs_declared_) tuple.push_back(lhs_value);
      if (!equal_decl_ && rhs_declared_) tuple.push_back(lhs_value);  // l = r
      batch->Add(std::move(tuple));
    }
  }

  return batch;
}

std::shared_ptr<utils::Batch> Compare::HandleFetchString() {
  auto lhs_key_to_tuples = GetStringTuples(lhs_);
  auto rhs_key_to_tuples = GetStringTuples(rhs_);

  auto batch = std::make_shared<utils::Batch>();

  for (auto& [lhs_key, lhs_tuple] : lhs_key_to_tuples) {
    if (!rhs_key_to_tuples.count(lhs_key)) continue;

    auto& rhs_tuples = rhs_key_to_tuples[lhs_key];

    // Cross-product the elements if the given keys alr match
    for (auto& lhs_elem : lhs_tuple)
      for (auto& rhs_elem : rhs_tuples) {
        std::vector<utils::Element> tuple;
        if (lhs_declared_) tuple.push_back(lhs_elem);
        if (!equal_decl_ && rhs_declared_) tuple.push_back(rhs_elem);
        batch->Add(std::move(tuple));
      }
  }

  return batch;
}

std::unordered_set<uint32_t> Compare::GetIntegerValues(
    std::shared_ptr<Ref> ref) {
  auto& evaluator = Evaluator::GetInstance();

  if (!ref->IsDeclared()) {
    // Guaranteed to be a statement ref because an entity ref cannot hold
    // an integer value.
    auto stmt_ref = ref->ToStatementRef();
    std::unordered_set<uint32_t> values = {stmt_ref->GetStmtNo()};
    return values;

  } else if (ref->IsStatement()) {
    auto stmt_ref = ref->ToStatementRef();
    auto stmt_decl = stmt_ref->GetDeclaration();
    auto stmt_type = stmt_decl->GetType();
    return evaluator.GetStatements(stmt_type);

  } else {
    // Guaranteed to be a constant.
    auto entity_ref = std::static_pointer_cast<EntityRef>(ref);
    return evaluator.GetConstants();
  }
}

std::unordered_map<std::string, utils::Tuple> Compare::GetStringTuples(
    std::shared_ptr<Ref> ref) {
  std::unordered_map<std::string, utils::Tuple> result;
  auto& evaluator = Evaluator::GetInstance();

  if (!ref->IsDeclared()) {
    // Guaranteed to be an entity ref because a statement ref cannot hold
    // a string value.
    auto entity_ref = std::dynamic_pointer_cast<EntityRef>(ref);
    result[entity_ref->GetId()] = {entity_ref->GetId()};

  } else {
    auto decl = ref->GetDeclaration();
    if (decl->GetType() == Declaration::Type::kProcedure) {
      // The key and value are both the procedure name here since we
      // always refer to a procedure by its string value directly.
      for (const auto& proc_name : evaluator.GetProcedures()) {
        result[proc_name] = {proc_name};
      }

    } else if (decl->GetType() == Declaration::Type::kVariable) {
      // The key and value are both the variable name here since we
      // always refer to a variable by its string value directly.
      for (const auto& var_name : evaluator.GetVariables()) {
        result[var_name] = {var_name};
      }

    } else if (decl->IsTypeIn({Declaration::Type::kCall,
                               Declaration::Type::kPrint,
                               Declaration::Type::kRead})) {
      // The key is the associated variable name, meanwhile the value is the
      // statement number.
      for (const auto& stmt : evaluator.GetStatements(decl->GetType())) {
        // the default value is an empty vector, so we can just push back
        result[evaluator.GetAttrByStmt(stmt)].push_back(stmt);
      }

    } else {
      throw InvalidQueryException(
          "Unable to convert reference into its string elements and values");
    }
  }

  return result;
}

EstimationValue Compare::GetEstimationValue(std::shared_ptr<Ref> ref) {
  if (!ref->IsDeclared()) {
    return 1;
  }

  auto& evaluator = Evaluator::GetInstance();

  auto decl = ref->GetDeclaration();
  if (decl->GetType() == Declaration::Type::kConstant) {
    return evaluator.GetConstants().size();
  } else if (decl->GetType() == Declaration::Type::kProcedure) {
    return evaluator.GetProcedures().size();
  } else if (decl->GetType() == Declaration::Type::kVariable) {
    return evaluator.GetConstants().size();
  } else {
    return evaluator.GetStatements(decl->GetType()).size();
  }
}

std::string Compare::ExtractString(utils::Element elem,
                                   std::shared_ptr<Ref> ref) {
  if (ref->IsStatement()) {
    auto stmt_no = std::get<uint32_t>(elem);
    auto& evaluator = Evaluator::GetInstance();
    return evaluator.GetAttrByStmt(stmt_no);
  } else {
    return std::get<std::string>(elem);
  }
}

bool Compare::Equals(Clause const& other) const {
  if (typeid(*this) != typeid(other)) return false;
  auto that = static_cast<Compare const&>(other);
  PtrComparator<Ref> ref_comparator;
  return ref_comparator(this->lhs_, that.lhs_) &&
         ref_comparator(this->rhs_, that.rhs_);
}

std::ostream& Compare::ToString(std::ostream& out) const {
  out << "[Compare: lhs: " << *this->lhs_ << " rhs: " << *this->rhs_ << "]";
  return out;
}

}  // namespace pql

}  // namespace spa
