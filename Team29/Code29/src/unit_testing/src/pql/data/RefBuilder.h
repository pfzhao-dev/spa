#pragma once

#include "catch.hpp"

namespace test {

namespace pql {

namespace data {

using namespace spa::pql;

template <typename T>
EntityRef MakeEntityRef(T t) {
  return EntityRef(t);
};
template <>
inline EntityRef MakeEntityRef<std::string>(std::string s) {
  return s.empty() ? EntityRef() : EntityRef(s);
};

template <typename T>
StatementRef MakeStatementRef(T t) {
  return StatementRef(t);
};
template <>
inline StatementRef MakeStatementRef<uint32_t>(uint32_t i) {
  return i == 0 ? StatementRef() : StatementRef(i);
};

}  // namespace data

}  // namespace pql

}  // namespace test
