#include "Translator.h"

namespace spa {

namespace utils {

const std::unordered_map<pql::Declaration::Type, ast::Statement::Type>
    kDeclTypeToStmtType = {
        {pql::Declaration::Type::kAssign, ast::Statement::Type::kAssign},
        {pql::Declaration::Type::kCall, ast::Statement::Type::kCall},
        {pql::Declaration::Type::kIf, ast::Statement::Type::kIf},
        {pql::Declaration::Type::kPrint, ast::Statement::Type::kPrint},
        {pql::Declaration::Type::kRead, ast::Statement::Type::kRead},
        {pql::Declaration::Type::kWhile, ast::Statement::Type::kWhile},
};

}  // namespace utils

}  // namespace spa
