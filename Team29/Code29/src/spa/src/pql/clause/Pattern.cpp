#include "Pattern.h"

#include "pql/Comparators.h"
#include "pql/Estimation.h"
#include "pql/Evaluator.h"
#include "pql/QueryException.h"
#include "utils/Element.h"

namespace spa {

namespace pql {

Pattern::Expression::Expression(std::string spec, bool partial)
    : wildcard_{false}, partial_{partial}, spec_{spec} {
  if (this->spec_.empty()) {
    throw QueryParseException("Expression spec is empty");
  }
};

Pattern::Expression::Expression()
    : wildcard_{true}, partial_{false}, spec_{""} {};

std::ostream& operator<<(std::ostream& out, const Pattern::Expression& expr) {
  out << "Expression: wild: " << expr.wildcard_ << " partial: " << expr.partial_
      << " spec: " << expr.spec_;
  return out;
}

bool Pattern::Expression::operator==(const Expression& other) const {
  return this->wildcard_ == other.wildcard_ &&
         this->partial_ == other.partial_ && this->spec_ == other.spec_;
}

bool Pattern::Expression::operator!=(const Expression& other) const {
  return !(*this == other);
}

std::string Pattern::Expression::ToString() const {
  if (wildcard_) {
    return "_";
  } else if (partial_) {
    return "_" + spec_ + "_";
  } else {
    return spec_;
  }
}

std::shared_ptr<ast::Expression> Pattern::Expression::GetSpecNode() {
  if (wildcard_) {
    return spec_node_;  // empty ptr
  }

  if (!spec_node_) {
    auto& evaluator = Evaluator::GetInstance();
    spec_node_ = evaluator.ParseExpression(spec_);
  }
  return spec_node_;
}

bool Pattern::Expression::IsWildcard() const { return wildcard_; }

bool Pattern::Expression::IsPartial() const { return partial_; }

Pattern::Pattern(std::shared_ptr<StatementRef> type,
                 std::shared_ptr<EntityRef> var, Expression expr)
    : Clause(Type::kPattern), type_{type}, var_{var}, expr_{expr} {
  if (!this->type_                   // empty ptr
      || (this->type_->IsDeclared()  // statement with a declaration
          && !this->type_->IsDeclTypeIn(
                 {Declaration::Type::kAssign, Declaration::Type::kWhile,
                  Declaration::Type::kIf}))  // correct declaration type
  ) {
    throw QueryParseException(
        "Statement ref for pattern type is empty or invalid");
  }

  if (!this->var_                   // empty ptr
      || (this->var_->IsDeclared()  // entity with a declaration
          && !this->var_->IsDeclTypeIn(
                 {Declaration::Type::kVariable})  // correct declaration type
          )) {
    throw QueryParseException(
        "Entity ref for pattern variable is empty or invalid");
  }

  auto type_decl_ = type_->GetDeclaration();
  pattern_type_ = type_decl_->GetType();
};

Pattern::Pattern(uint32_t type, std::string var, Expression expr)
    : Pattern(std::make_shared<StatementRef>(type),
              std::make_shared<EntityRef>(var), expr) {}

std::vector<std::shared_ptr<Ref>> Pattern::GetDeclaredRefs() {
  std::vector<std::shared_ptr<Ref>> refs;

  if (type_->IsDeclared()) refs.push_back(type_);
  if (var_->IsDeclared()) refs.push_back(var_);

  return refs;
}

Estimation Pattern::ComputeEstimation() {
  auto& evaluator = Evaluator::GetInstance();
  auto& cache = evaluator.GetPatternVarCache(pattern_type_);

  // Since the type is guaranteed to be a declaration, we can just
  // get the map directly and verify.

  if (var_->IsWild()) {  // Handle (s, _)
    auto& stmts = cache.GetOriginalKeys();
    return {stmts.size(), {stmts.size()}};

  } else if (!var_->IsDeclared()) {  // Handle (s, v)
    if (!cache.CountReverse(var_->GetId())) {
      return {0, {0}};  // special case if there's no such variable
    }

    auto& stmts = cache.GetReverseValues(var_->GetId());
    return {stmts.size(), {stmts.size()}};

  } else {
    auto rows = cache.Size();
    auto& c1 = cache.GetOriginalKeys();
    auto& c2 = cache.GetReverseKeys();

    // NOTE: We don't filter this based on the expression.
    //       Maybe can consider the unfiltered values as the extra cost
    //       to verify an expression.
    return {rows, {c1.size(), c2.size()}};
  }
}

bool Pattern::Test(std::vector<utils::Element>& tuple,
                   std::vector<size_t>& attr_idx) {
  auto& evaluator = Evaluator::GetInstance();
  auto& cache = evaluator.GetPatternVarCache(pattern_type_);
  auto no_expr_result = TestTuple(cache, type_, var_, tuple, attr_idx);

  // If the result is false or there is no expression, just returns.
  if (!no_expr_result || expr_.IsWildcard()) return no_expr_result;

  // Otherwise check the expression. This is guaranteed to be an assign pattern
  // by the pql parser.
  auto stmt_no = std::get<uint32_t>(tuple[attr_idx[0]]);
  return evaluator.PatternExpr(stmt_no, expr_.GetSpecNode(), expr_.IsPartial());
}

std::shared_ptr<utils::Batch> Pattern::Fetch() {
  auto& evaluator = Evaluator::GetInstance();
  auto& cache = evaluator.GetPatternVarCache(pattern_type_);

  // Since the type is guaranteed to be a declaration, we can just
  // get the map directly and verify.

  auto& stmt_to_vars = cache.GetOriginalMap();
  bool var_declared_ = var_->IsDeclared();
  auto batch = std::make_shared<utils::Batch>();

  for (auto& [stmt, vars] : stmt_to_vars) {
    if (!expr_.IsWildcard() &&
        !evaluator.PatternExpr(stmt, expr_.GetSpecNode(), expr_.IsPartial())) {
      // Skip if the expression is not a wildcard and does not match.
      continue;
    }

    for (auto& var : vars) {
      if (!var_->Test(var)) continue;  // skip if the variable does not match

      std::vector<utils::Element> tuple = {stmt};
      if (var_declared_) tuple.push_back(var);
      batch->Add(std::move(tuple));
    }
  }

  return batch;
}

bool Pattern::Equals(Clause const& other) const {
  if (typeid(*this) != typeid(other)) return false;
  auto that = static_cast<Pattern const&>(other);
  PtrComparator<StatementRef> statement_ref_comparator;
  PtrComparator<EntityRef> entity_ref_comparator;
  return this->expr_ == that.expr_ &&
         statement_ref_comparator(this->type_, that.type_) &&
         entity_ref_comparator(this->var_, that.var_);
}

std::ostream& Pattern::ToString(std::ostream& out) const {
  out << "[Pattern: type: " << *this->type_ << " var: " << *this->var_
      << " expr: " << this->expr_ << "]";
  return out;
}

}  // namespace pql

}  // namespace spa
