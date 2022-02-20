#pragma once

#include <memory>

#include "ast/Expression.h"
#include "pql/Estimation.h"
#include "pql/clause/Clause.h"

namespace spa {

namespace pql {

class Pattern : public Clause {
 public:
  class Expression {
   public:
    // Wildcard pattern expression.
    Expression();

    // throws an exception if spec is empty
    Expression(std::string spec, bool partial = false);

    bool operator==(const Expression& other) const;
    bool operator!=(const Expression& other) const;

    std::string ToString() const;

    friend std::ostream& operator<<(std::ostream& out, const Expression& ref);

    std::shared_ptr<ast::Expression> GetSpecNode();

    bool IsWildcard() const;

    bool IsPartial() const;

   private:
    bool wildcard_;
    bool partial_;
    std::string spec_;

    // Cached spec node to be used for estimation and evaluation.
    std::shared_ptr<ast::Expression> spec_node_;
  };

  Pattern(std::shared_ptr<StatementRef> type, std::shared_ptr<EntityRef> var,
          Expression expr);

  Pattern(uint32_t stmt_no, std::string var, Expression expr = {});

  std::shared_ptr<utils::Batch> Fetch() override;

  bool Test(std::vector<utils::Element>& tuple,
            std::vector<size_t>& attr_idx) override;

 protected:
  std::vector<std::shared_ptr<Ref>> GetDeclaredRefs() override;

  Estimation ComputeEstimation() override;

  bool Equals(Clause const& other) const override;

  std::ostream& ToString(std::ostream& out) const override;

 private:
  std::shared_ptr<StatementRef> type_;  // e.g. a of pattern a (v, _)
  std::shared_ptr<EntityRef> var_;      // e.g. v of pattern a (v, _)
  // expression is wildcard for if and while type
  Expression expr_;

  // To differentiate between different pattern types.
  // The value will be based on the declaration for `type_`.
  pql::Declaration::Type pattern_type_;
};

}  // namespace pql

}  // namespace spa
