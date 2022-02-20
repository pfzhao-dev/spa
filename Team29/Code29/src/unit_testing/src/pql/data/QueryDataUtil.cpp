#include "QueryDataUtil.h"

#include "catch.hpp"

namespace test {

namespace pql {

namespace data {

using namespace spa::pql;

StringToStringFunc CombineStrFunc(StringToStringFunc func1,
                                  StringToStringFunc func2) {
  return [=](std::string decl_name) {
    return func1(decl_name) + " " + func2(decl_name);
  };
}

ClausesPair::ClausesPair(StringToStringFunc string_func,
                         DeclPtrToClausePtrVecFunc clauses_func)
    : string_func_{string_func}, clauses_func_{clauses_func} {}

ClausesPair ClausesPair::Combine(const ClausesPair& cp1,
                                 const ClausesPair& cp2) {
  StringToStringFunc new_string_func =
      CombineStrFunc(cp1.string_func_, cp2.string_func_);
  DeclPtrToClausePtrVecFunc new_clauses_func =
      [=](std::shared_ptr<Declaration> expected_declaration)
      -> std::vector<std::shared_ptr<Clause>> {
    auto clauses1 = cp1.clauses_func_(expected_declaration);
    auto clauses2 = cp2.clauses_func_(expected_declaration);
    return CombineVectors(clauses1, clauses2);
  };
  return ClausesPair(new_string_func, new_clauses_func);
}

}  // namespace data

}  // namespace pql

}  // namespace test
