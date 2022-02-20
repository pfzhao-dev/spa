#include "Parser.h"

#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include "ast/Expression.h"
#include "ast/Node.h"
#include "ast/Program.h"
#include "ast/StatementList.h"
#include "ast/parser/ExpressionParser.h"
#include "ast/parser/ParseException.h"
#include "ast/parser/ProcedureParser.h"

namespace spa {

namespace ast {

std::string Parser::RemoveTrailing(std::string s) {
  std::regex trailing_whitespace(R"(^\s*(.+)?\s*$)");
  std::smatch smatch;
  // remove prefix and suffix whitespaces
  s.erase(0, s.find_first_not_of(' '));
  s.erase(s.find_last_not_of(' ') + 1);
  return s;
}

std::string Parser::Strip(std::string s) {
  std::regex whitespace_regex(R"(\s+)");
  // remove space in between
  return std::regex_replace(RemoveTrailing(s), whitespace_regex, " ");
}

std::string Parser::RemoveNewLines(std::string s) {
  std::regex newline_reg(R"(\n+|\t+)");
  return std::regex_replace(s, newline_reg, " ");
}

std::string Parser::GetFirstWord(std::string s) {
  return s.substr(0, s.find(" "));
}

std::vector<std::string> Parser::Tokenize(std::string s) {
  bool has_match = true;
  std::vector<std::string> stmts;
  std::smatch stmt_match;

  std::regex proc_regex(R"(^\s*procedure\s+([A-Za-z][A-Za-z0-9]*)\s*\{\s*)");
  std::regex if_regex(R"(^\s*if\s*\((.+?)\)\s*then\s*\{\s*)");
  std::regex else_regex(R"(^\s*else\s*\{\s*)");
  std::regex while_regex(R"(^\s*while\s*\((.+?)\)\s*\{\s*)");
  std::regex end_cond_regex(R"(^\s*(\})\s*)");
  std::regex stmt_regex(R"(^\s*([^;{}]+);\s*)");

  std::vector<std::regex> regexes = {end_cond_regex, proc_regex,  if_regex,
                                     else_regex,     while_regex, stmt_regex};
  std::vector<std::string> regex_names = {"",     "procedure ", "if ",
                                          "else", "while ",     ""};
  s = Parser::Strip(RemoveNewLines(s));

  // Tries to match 1 of these regular expressions
  while (has_match) {
    for (int i = 0; i < regex_names.size(); i++) {
      has_match = std::regex_search(s, stmt_match, regexes[i]);
      if (has_match) {
        stmts.push_back(regex_names[i] + RemoveTrailing(stmt_match[1]));
        s = stmt_match.suffix();
        break;
      } else if (s.length() > 0 &&
                 GetFirstWord(s) == RemoveTrailing(regex_names[i])) {
        // unmatched syntax for above regexes
        throw ParseException();
      }
    }
  }

  if (s.length() > 0) {
    throw ParseException();
  }

  return stmts;
}

std::shared_ptr<Program> Parser::Parse() {
  // Do bracket matching
  std::vector<int> open_brackets;

  int stmt_no = 1;
  for (int i = 0; i < this->stmts.size(); i++) {
    std::string first_word = GetFirstWord(stmts[i]);
    if (first_word == "}") {
      if (open_brackets.empty()) {
        // No open bracket to match close bracket
        throw ParseException();
      }
      int close = open_brackets.back();
      open_close[close] = i;
      close_open[i] = close;
      open_brackets.pop_back();
    }
    if (first_word == "if" || first_word == "while" || first_word == "else" ||
        first_word == "procedure") {
      open_brackets.push_back(i);
    }
    stmt_nos_[i] = stmt_no;
    // update stmt no
    if (first_word != "procedure" && first_word != "}" &&
        first_word != "else") {
      stmt_no += 1;
    }
  }

  if (!open_brackets.empty()) {
    // Imbalanced brackets
    throw ParseException();
  }

  std::vector<std::shared_ptr<Procedure>> proc_list;

  // count number of procedures, if we are doing dynamic resizing of lists it
  // would take more time
  int num_proc = 0;
  std::unordered_map<std::string, int> proc_index_map;
  std::unordered_map<int, std::string> index_proc_map;

  for (int i = 0; i < stmts.size(); i++) {
    if (GetFirstWord(stmts[i]) == "procedure") {
      std::string proc_name = stmts[i].substr(10);
      if (proc_index_map.count(proc_name)) {
        throw ParseException();
      }
      proc_index_map[proc_name] = num_proc;
      index_proc_map[num_proc] = proc_name;
      ++num_proc;
    }
  }

  int proc_end_idx = -1;
  int i = 0;
  ProcedureParser procedure_parser = {
      std::make_shared<std::vector<std::string>>(stmts),
      std::make_shared<std::map<int, int>>(open_close),
      std::make_shared<std::vector<int>>(stmt_nos_)};
  while (i < stmts.size()) {
    if (GetFirstWord(stmts[i]) == "procedure") {
      proc_list.push_back(procedure_parser.Parse(i, open_close[i]));
      proc_end_idx = open_close[i];
      i = proc_end_idx;
    } else if (i > proc_end_idx) {
      // Throw parse exception is statement is not in a procedure
      throw ParseException();
    } else {
      ++i;
    }
  }

  return std::make_shared<Program>(proc_list);
}

std::shared_ptr<Absolute> Parser::ParseExpression(std::string s) {
  ExpressionParser expr_parser;
  return expr_parser.ParseExpressionTerm(s, true);
}

}  // namespace ast

}  // namespace spa
