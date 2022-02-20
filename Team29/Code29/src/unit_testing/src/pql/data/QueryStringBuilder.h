#pragma once

#include "catch.hpp"
#include "pql/Declaration.h"

namespace test {

namespace pql {

namespace data {

// return assignment string based declaration type and name in
const std::string MakeAssignString(spa::pql::Declaration::Type type,
                                   std::string name);

// return select string based on declaration_name
const std::string MakeSelectString(std::string declaration_name);

// return relation string based on type of relation and arguments
const std::string MakeRelationString(std::string relation_name,
                                     std::string arg1, std::string arg2);

// return pattern string based on type_name, var_name, expr and expr_2
const std::string MakePatternString(std::string type_name, std::string var,
                                    std::string expr, std::string expr_2 = "");

// return with string based on lhs_name, rhs_name, lhs_value and rhs_value
const std::string MakeCompareString(std::string lhs_name, std::string rhs_name,
                                    std::string lhs_value = "",
                                    std::string rhs_value = "");

}  // namespace data

}  // namespace pql

}  // namespace test
