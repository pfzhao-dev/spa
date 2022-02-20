#pragma once

#include <exception>

namespace spa {

namespace xtr {

class ExtractException : public std::exception {
 public:
  ExtractException(const char* msg) : msg_{msg} {}

  const char* what() const throw() { return msg_; }

 private:
  const char* msg_;
};

}  // namespace xtr

}  // namespace spa
