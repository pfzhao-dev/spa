#pragma once

#include <exception>

namespace spa {

namespace ast {

class ParseException : public std::exception {
  const char* what() const throw() { return "Error in parsing source code"; }
};

}  // namespace ast

}  // namespace spa
