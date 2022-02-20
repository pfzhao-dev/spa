#pragma once

#include <string>
#include <variant>
#include <vector>

#include <ast/Statement.h>

namespace spa {

namespace utils {

typedef std::variant<uint32_t, std::string> Element;

typedef std::vector<Element> Tuple;

}  // namespace utils

}  // namespace spa
