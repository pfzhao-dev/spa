#pragma once

#include "QueryDataUtil.h"
#include "catch.hpp"
#include "pql/Declaration.h"

namespace test {

namespace pql {

namespace data {

namespace declaration {

// names of default declarations used in kDefaultDecls
const std::string kDefaultDeclStmt = "s";
const std::string kDefaultDeclRead = "r";
const std::string kDefaultDeclPrint = "pri";
const std::string kDefaultDeclCall = "ca";
const std::string kDefaultDeclWhile = "w";
const std::string kDefaultDeclIf = "i";
const std::string kDefaultDeclAssign = "a";
const std::string kDefaultDeclVariable = "v";
const std::string kDefaultDeclConstant = "co";
const std::string kDefaultDeclProcedure = "pro";
const std::string kDefaultDeclProgLine = "prog";

// string of default declarations to be used in each test
extern const std::string kDefaultDecls;

// key is the default declaraion name, value is the declaration pointer
extern const std::unordered_map<std::string, std::shared_ptr<Declaration>>
    kDefaultDeclNameToDeclPtr;

// key is the declaration type, value is the name to be assigned to the
// declaration
extern const std::vector<std::pair<Declaration::Type, std::string>>
    kDeclTypeNamePairs;

}  // namespace declaration

}  // namespace data

}  // namespace pql

}  // namespace test
