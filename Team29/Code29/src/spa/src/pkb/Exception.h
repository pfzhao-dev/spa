#pragma once

#include <exception>
#include <string>

namespace spa {

namespace pkb {

class Exception : public std::exception {
 public:
  Exception(const char* msg) : msg_{msg} {}

  const char* what() const throw() { return msg_; }

 private:
  const char* msg_;
};

class InternalException : public Exception {
 public:
  InternalException(std::string s) : Exception(s.c_str()) {}
};

}  // namespace pkb

}  // namespace spa
