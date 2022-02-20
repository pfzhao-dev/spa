#include "QueryStringBuilder.h"

#include <sstream>

#include "catch.hpp"

namespace test {

namespace pql {

namespace data {

const std::string MakeAssignString(spa::pql::Declaration::Type type,
                                   std::string name) {
  std::stringstream string_stream;
  string_stream << spa::pql::kDeclTypeToString.at(type) << " " << name << ";";
  return string_stream.str();
}

const std::string MakeSelectString(std::string declaration_name) {
  std::stringstream string_stream;
  string_stream << "Select " << declaration_name << " ";
  return string_stream.str();
}

const std::string MakeRelationString(std::string relation_name,
                                     std::string arg1, std::string arg2) {
  std::stringstream string_stream;
  string_stream << " such that " << relation_name << " (" << arg1 << ", "
                << arg2 << ")";
  return string_stream.str();
}

const std::string MakePatternString(std::string type_name, std::string var,
                                    std::string expr, std::string expr_2) {
  std::stringstream string_stream;
  string_stream << " pattern " << type_name << "(" << var << ", " << expr;
  if (!expr_2.empty()) {
    string_stream << ", " << expr_2;
  }
  string_stream << ")";
  return string_stream.str();
}

const std::string MakeCompareString(std::string lhs_name, std::string rhs_name,
                                    std::string lhs_value,
                                    std::string rhs_value) {
  std::stringstream string_stream;
  string_stream << " with " << lhs_name;
  if (!lhs_value.empty()) {
    string_stream << "." << lhs_value;
  }
  string_stream << " = " << rhs_name;
  if (!rhs_value.empty()) {
    string_stream << "." << rhs_value;
  }
  string_stream << " ";
  return string_stream.str();
}

}  // namespace data

}  // namespace pql

}  // namespace test
