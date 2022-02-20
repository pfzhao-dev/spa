#pragma once

#include "ast/Statement.h"
#include "pql/Declaration.h"

namespace spa {

namespace utils {

extern const std::unordered_map<pql::Declaration::Type, ast::Statement::Type>
    kDeclTypeToStmtType;

}  // namespace utils

}  // namespace spa
