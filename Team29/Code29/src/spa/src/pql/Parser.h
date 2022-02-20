#pragma once

#include <regex>

#include "QueryBuilder.h"

namespace spa {

namespace pql {

class Parser {
 public:
  Parser();

  Query Parse(std::string s);

 private:
  QueryBuilder query_builder_;

  // regexes
  // maintain at least 1 whitespace between alphanumeric tokens
  // whitespace is not complusory between other tokens
  // all refs such as "x", s.stmt#, 2, and _ are considered to be alphanumberic
  // tokens

  static const std::string kNewLineRegexStr;

  // whitespace
  static const std::string kWhitespaceRegexStr, kMultipleWhitespaceRegexStr,
      kTrailingWhitespaceRegexStr;

  // keywords
  static const std::string kDeclsTypeRegexStr, kDeclsValueTypeRegexStr,
      kSelectRegexStr, kBoolRegexStr, kSuchThatRegexStr,
      kPatternKeywordRegexStr, kWithKeywordRegexStr, kAndRegexStr,
      kEqualsRegexStr, kRelationTypeRegexStr, kProgLineKeywordRegexStr,
      kWildRegexStr;

  // integers, names and patterns
  static const std::string kIntRegexStr, kNameRegexStr, kExprRegexStr,
      kDeclRefRegexStr, kEntityRefRegexStr, kRefRegexStr, kCompareRefRegexStr,
      kRelationRegexStr, kPatternTypeRegexStr, kPatternExprRegexStr,
      kPatternRegexStr, kCompareRegexStr;

  // used for tokenization
  static const std::string kTokenizeDeclRegexStr, kTokenizeSelectRegexStr,
      kTokenizeSuchThatRegexStr, kTokenizePatternRegexStr,
      kTokenizeCompareRegexStr;

  // remove from string s based on regexes
  const std::string Remove(const std::string s,
                           std::vector<std::string> regexes,
                           bool replace_first_match = false) const;

  // returns true if string only contains spaces, tabs or newlines
  bool IsStrEmpty(const std::string s) const;

  // pair returned is (decl_type, decl_value)
  std::pair<std::string, std::string> ParseDeclWithVal(std::string s) const;

  // tokenize all declarations, e.g. "stmt s; assign a1, a2;"
  // return remaining string
  const std::string TokenizeDecls(const std::string s);
  // parse a declarations, e.g. "assign a1, a2;"
  void ParseDecl(const std::string decl);

  // tokenize selected declaration
  // return remaining string
  const std::string TokenizeSelectedDecl(const std::string s);
  // parse selected declaration, e.g. "Select v"
  void ParseSelectDecl(const std::string select_decl);

  // tokenize relational clauses
  // return remaining string
  const std::string TokenizeRelationalClauses(const std::string s);
  // parse relations, e.g. "Follows(1, 2)"
  std::string ParseRelationalClause(const std::string relation);

  // tokenize pattern clauses
  // return remaining string
  const std::string TokenizePatterns(const std::string s);
  // parse patterns, e.g. "pattern a (v, _)"
  std::string ParsePattern(const std::string pattern);

  // tokenize with clauses
  // return remaining string
  const std::string TokenizeCompare(const std::string s);
  // parse with, e.g. with p.procName = v.varName
  std::string ParseCompare(const std::string compare);
};

}  // namespace pql

}  // namespace spa
