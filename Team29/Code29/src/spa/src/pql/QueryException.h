#pragma once

#include <exception>

namespace spa {

namespace pql {

class QueryException : public std::exception {
 public:
  QueryException(const char* msg) : msg_{msg} {}

  const char* what() const throw() { return msg_; }

 private:
  const char* msg_;
};

class QueryParseException : public QueryException {
 public:
  using QueryException::QueryException;

  QueryParseException() : QueryException("Error in parsing query") {}
};

class InvalidQueryException : public QueryException {
  public:
  using QueryException::QueryException;

  InvalidQueryException() : QueryException("Invalid query") {}
};

}  // namespace pql

}  // namespace spa
