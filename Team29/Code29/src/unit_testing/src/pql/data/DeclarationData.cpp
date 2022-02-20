#include "DeclarationData.h"

#include <memory>

#include "catch.hpp"

namespace test {

namespace pql {

namespace data {

namespace declaration {

using namespace spa::pql;

const std::string kDefaultDecls =
    "stmt s; read r; print pri; call ca; while w; if i; assign a; variable v; "
    "constant co; procedure pro; prog_line prog;";

const std::unordered_map<std::string, std::shared_ptr<Declaration>>
    kDefaultDeclNameToDeclPtr = {
        {kDefaultDeclStmt, std::make_shared<Declaration>(Declaration(
                               Declaration::Type::kStmt, kDefaultDeclStmt))},
        {kDefaultDeclRead, std::make_shared<Declaration>(Declaration(
                               Declaration::Type::kRead, kDefaultDeclRead))},
        {kDefaultDeclPrint, std::make_shared<Declaration>(Declaration(
                                Declaration::Type::kPrint, kDefaultDeclPrint))},
        {kDefaultDeclCall, std::make_shared<Declaration>(Declaration(
                               Declaration::Type::kCall, kDefaultDeclCall))},
        {kDefaultDeclWhile, std::make_shared<Declaration>(Declaration(
                                Declaration::Type::kWhile, kDefaultDeclWhile))},
        {kDefaultDeclIf, std::make_shared<Declaration>(Declaration(
                             Declaration::Type::kIf, kDefaultDeclIf))},
        {kDefaultDeclAssign,
         std::make_shared<Declaration>(
             Declaration(Declaration::Type::kAssign, kDefaultDeclAssign))},
        {kDefaultDeclVariable,
         std::make_shared<Declaration>(
             Declaration(Declaration::Type::kVariable, kDefaultDeclVariable))},
        {kDefaultDeclConstant,
         std::make_shared<Declaration>(
             Declaration(Declaration::Type::kConstant, kDefaultDeclConstant))},
        {kDefaultDeclProcedure,
         std::make_shared<Declaration>(Declaration(
             Declaration::Type::kProcedure, kDefaultDeclProcedure))},
        {kDefaultDeclProgLine,
         std::make_shared<Declaration>(
             Declaration(Declaration::Type::kStmt, kDefaultDeclProgLine))},
};

const std::vector<std::pair<Declaration::Type, std::string>>
    kDeclTypeNamePairs = {{Declaration::Type::kStmt, "Select"},
                          {Declaration::Type::kRead, "read"},
                          {Declaration::Type::kPrint, "pattern"},
                          {Declaration::Type::kCall, "call"},
                          {Declaration::Type::kWhile, "while"},
                          {Declaration::Type::kIf, "if"},
                          {Declaration::Type::kAssign, "assign"},
                          {Declaration::Type::kVariable, "var"},
                          {Declaration::Type::kConstant, "const"},
                          {Declaration::Type::kProcedure, "procedure"},
                          {Declaration::Type::kStmt, "program"}};

}  // namespace declaration

}  // namespace data

}  // namespace pql

}  // namespace test
