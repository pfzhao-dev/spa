#include "StatementParser.h"

#include <regex>
#include <string>

#include "ast/parser/ParseException.h"
#include "ast/parser/AssignParser.h"
#include "ast/parser/CallParser.h"
#include "ast/parser/IfParser.h"
#include "ast/parser/PrintParser.h"
#include "ast/parser/ReadParser.h"
#include "ast/parser/WhileParser.h"

namespace spa {

namespace ast {

std::string StatementParser::GetFirstWord(std::string s) {
  return s.substr(0, s.find(" "));
}

std::shared_ptr<Statement> StatementParser::Parse(int start_idx, int end_idx,
                                                  std::string proc_name) {
  std::string firstWord = GetFirstWord((*stmts)[start_idx]);

  std::shared_ptr<Statement> stmt_ptr;

  if (firstWord == "print") {
    PrintParser print_parser = {stmts, open_close, stmt_nos};
    stmt_ptr = print_parser.Parse(start_idx);
  } else if (firstWord == "call") {
    CallParser call_parser = {stmts, open_close, stmt_nos};
    stmt_ptr = call_parser.Parse(start_idx);
  } else if (firstWord == "read") {
    ReadParser read_parser = {stmts, open_close, stmt_nos};
    stmt_ptr = read_parser.Parse(start_idx);
  } else if (firstWord == "if") {
    IfParser if_parser = {stmts, open_close, stmt_nos};
    stmt_ptr = if_parser.Parse(start_idx, proc_name);
  } else if (firstWord == "while") {
    WhileParser while_parser = {stmts, open_close, stmt_nos};
    stmt_ptr = while_parser.Parse(start_idx, proc_name);
  } else {
    // Should be assign. Need to verify correctness.
    AssignParser assign_parser = {stmts, open_close, stmt_nos};
    stmt_ptr = assign_parser.Parse(start_idx);
  }

  return stmt_ptr;
}

std::shared_ptr<Variable> StatementParser::ParseVariable(std::string var_str) {
  const std::string var_name_regex = "[A-Za-z][A-Za-z0-9]*";
  std::regex var_regex = std::regex(var_name_regex);
  if (!std::regex_match(var_str, var_regex)) {
    throw ParseException();
  } else {
    return std::make_shared<Variable>(var_str);
  }
}

std::shared_ptr<Constant> StatementParser::ParseConstant(
    std::string const_str) {
  const std::string const_value_regex = "0|[1-9][0-9]*";
  std::regex const_regex = std::regex(const_value_regex);
  if (!std::regex_match(const_str, const_regex)) {
    throw ParseException();
  } else {
    auto const_value = std::stoi(const_str);
    return std::make_shared<Constant>(const_value);
  }
}

}  // namespace ast
}  // namespace spa
