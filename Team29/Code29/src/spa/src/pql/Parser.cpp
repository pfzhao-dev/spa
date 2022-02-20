#include "Parser.h"

#include "QueryException.h"

namespace spa {

namespace pql {

Parser::Parser() : query_builder_{} {}

Query Parser::Parse(std::string s) {
  this->query_builder_.Reset();

  // remove extra and trailing whitespace
  s = this->Remove(s, {kTrailingWhitespaceRegexStr, kMultipleWhitespaceRegexStr,
                       kNewLineRegexStr});

  // parse declarations
  s = this->TokenizeDecls(s);

  // parse selected declaration
  s = this->TokenizeSelectedDecl(s);

  // parse relational clauses
  s = this->TokenizeRelationalClauses(s);

  // parse pattern clauses
  s = this->TokenizePatterns(s);

  // parse with clauses
  s = this->TokenizeCompare(s);

  if (!this->IsStrEmpty(s)) {
    throw QueryParseException("Parsing is incomplete");
  }

  // build query object using query builder
  return this->query_builder_.Build();
}

// regexes

const std::string Parser::kNewLineRegexStr = R"(\n)";

// whitespace
const std::string Parser::kWhitespaceRegexStr = R"(\s)";
const std::string Parser::kMultipleWhitespaceRegexStr = R"((\s)\s+)";
const std::string Parser::kTrailingWhitespaceRegexStr = R"(^\s+|\s+$)";

// keywords
const std::string Parser::kDeclsTypeRegexStr =
    R"((stmt|read|print|call|while|if|assign|variable|constant|procedure|prog_line))";
const std::string Parser::kDeclsValueTypeRegexStr =
    R"((stmt#|procName|varName|value))";
const std::string Parser::kSelectRegexStr = R"(Select)";
const std::string Parser::kBoolRegexStr = R"(BOOLEAN)";
const std::string Parser::kSuchThatRegexStr = R"(such\s+that)";
const std::string Parser::kPatternKeywordRegexStr = R"(pattern)";
const std::string Parser::kWithKeywordRegexStr = R"(with)";
const std::string Parser::kAndRegexStr = R"(and)";
const std::string Parser::kEqualsRegexStr = R"(=)";
const std::string Parser::kRelationTypeRegexStr =
    R"(((Follows(\*)?)|(Parent(\*)?)|Uses|Modifies|(Affects(Bip)?(\*)?))"
    R"(|(Calls(\*)?)|(Next(Bip)?(\*)?)))";
const std::string Parser::kProgLineKeywordRegexStr = R"(prog_line)";
const std::string Parser::kWildRegexStr = R"(_)";

// integers, names and patterns
const std::string Parser::kIntRegexStr = R"([0-9]+)";
const std::string Parser::kNameRegexStr = R"([a-zA-Z][a-zA-Z0-9]*)";
const std::string Parser::kExprRegexStr = R"([a-zA-Z0-9+\-*/%()\s]+)";
const std::string Parser::kDeclRefRegexStr =
    kNameRegexStr + R"((\s*.\s*)" + kDeclsValueTypeRegexStr + R"()?)";
const std::string Parser::kEntityRefRegexStr = R"(()" + kWildRegexStr + R"(|)" +
                                               kNameRegexStr + R"(|(\"\s*)" +
                                               kNameRegexStr + R"(\s*\")))";
const std::string Parser::kRefRegexStr =
    R"(()" + kWildRegexStr + R"(|)" + kIntRegexStr + R"(|)" + kNameRegexStr +
    R"(|(\"\s*)" + kNameRegexStr + R"(\s*\")))";
const std::string Parser::kCompareRefRegexStr =
    R"(()" + kIntRegexStr + R"(|)" + kDeclRefRegexStr + R"(|(\"\s*)" +
    kNameRegexStr + R"(\s*\")))";
const std::string Parser::kRelationRegexStr =
    kRelationTypeRegexStr + R"(\s*\(\s*)" + kRefRegexStr + R"(\s*,\s*)" +
    kRefRegexStr + R"(\s*\))";
const std::string Parser::kPatternTypeRegexStr = R"(\s)" + kNameRegexStr;
const std::string Parser::kPatternExprRegexStr =
    R"(()" + kWildRegexStr + R"(\s*"\s*)" + kExprRegexStr + R"(\s*"\s*)" +
    kWildRegexStr + R"(|)" + kWildRegexStr + R"(|"\s*)" + kExprRegexStr +
    R"(\s*"))";
const std::string Parser::kPatternRegexStr =
    kNameRegexStr + R"(\s*\(\s*)" + kEntityRefRegexStr + R"(\s*,\s*)" +
    kPatternExprRegexStr + R"((\s*,\s*)" + kWildRegexStr + R"()?\s*\))";
const std::string Parser::kCompareRegexStr = kCompareRefRegexStr + R"(\s*)" +
                                             kEqualsRegexStr + R"(\s*)" +
                                             kCompareRefRegexStr;

// used for tokenization
const std::string Parser::kTokenizeDeclRegexStr =
    kDeclsTypeRegexStr + R"(\s+)" + kNameRegexStr + R"(\s*(,\s*)" +
    kNameRegexStr + R"()*\s*;)";
const std::string Parser::kTokenizeSelectRegexStr =
    kSelectRegexStr + R"(\s*(\s)" + kBoolRegexStr + R"(|\s)" +
    kDeclRefRegexStr + R"(|<\s*)" + kDeclRefRegexStr + R"((\s*,\s*)" +
    kDeclRefRegexStr + R"()*\s*>))";
const std::string Parser::kTokenizeSuchThatRegexStr =
    kSuchThatRegexStr + R"(\s+)" + kRelationRegexStr + R"((\s*)" +
    kAndRegexStr + R"(\s+)" + kRelationRegexStr + R"()*)";
const std::string Parser::kTokenizePatternRegexStr =
    kPatternKeywordRegexStr + R"(\s+)" + kPatternRegexStr + R"((\s*)" +
    kAndRegexStr + R"(\s+)" + kPatternRegexStr + R"()*)";
const std::string Parser::kTokenizeCompareRegexStr =
    kWithKeywordRegexStr + R"(\s+)" + kCompareRegexStr + R"((\s*)" +
    kAndRegexStr + R"(\s+)" + kCompareRegexStr + R"()*)";

const std::string Parser::Remove(const std::string s,
                                 std::vector<std::string> regexes,
                                 bool replace_first_match) const {
  std::string regex_str = "";
  for (auto regex : regexes) {
    if (!regex_str.empty()) regex_str += "|";
    regex_str += regex;
  }

  if (replace_first_match) {
    return std::regex_replace(s, std::regex(regex_str), "$1",
                              std::regex_constants::format_first_only);
  }
  return std::regex_replace(s, std::regex(regex_str), "$1");
}

bool Parser::IsStrEmpty(const std::string s) const {
  std::regex empty_regex("(" + kWhitespaceRegexStr + "|" + kNewLineRegexStr +
                         ")" + "*");
  std::smatch empty_match;
  return std::regex_match(s, empty_match, empty_regex);
}

std::pair<std::string, std::string> Parser::ParseDeclWithVal(
    std::string s) const {
  s = this->Remove(s, {kWhitespaceRegexStr});
  std::string::size_type dot_index = s.find_first_of(".");
  std::string value = "";
  std::string decl = s;
  if (dot_index != std::string::npos) {
    value = s.substr(dot_index + 1);
    decl = s.substr(0, dot_index);
  }
  return std::make_pair(decl, value);
}

const std::string Parser::TokenizeDecls(const std::string s) {
  // extract each declaration from query string
  std::regex decl_regex(kTokenizeDeclRegexStr);
  std::smatch decl_match;
  for (std::sregex_iterator i =
           std::sregex_iterator(s.begin(), s.end(), decl_regex);
       i != std::sregex_iterator(); ++i) {
    decl_match = *i;

    if (!this->IsStrEmpty(decl_match.prefix())) {
      throw QueryParseException(
          "Syntax error while trying to parse declarations");
    }

    // parse each declaration
    this->ParseDecl(decl_match.str());
  }

  return decl_match.empty() ? s : decl_match.suffix();
}

void Parser::ParseDecl(const std::string decl) {
  // parse declaration type
  std::regex decl_type_regex(kDeclsTypeRegexStr);
  std::smatch decl_type_match;
  std::regex_search(decl, decl_type_match, decl_type_regex);

  Declaration::Type decl_type = kStringToDeclType.at(decl_type_match.str());
  bool is_prog_line = decl_type_match.str() == kProgLineKeywordRegexStr;
  const std::string decl_names = decl_type_match.suffix();

  // parse declaration names
  std::regex name_regex(kNameRegexStr);
  for (std::sregex_iterator i = std::sregex_iterator(
           decl_names.begin(), decl_names.end(), name_regex);
       i != std::sregex_iterator(); ++i) {
    std::smatch match = *i;
    std::string decl_name = match.str();

    // build each declaration
    if (is_prog_line) {
      this->query_builder_.AddDeclarationProgLine(decl_name);
    } else {
      this->query_builder_.AddDeclaration(decl_type, decl_name);
    }
  }
}

const std::string Parser::TokenizeSelectedDecl(const std::string s) {
  std::regex select_regex(kTokenizeSelectRegexStr);
  std::smatch select_match;
  bool has_select = std::regex_search(s, select_match, select_regex);

  if (!has_select || !this->IsStrEmpty(select_match.prefix())) {
    throw QueryParseException(
        "Syntax error while trying to parse selected declaration");
  }
  std::string decl = select_match.str();
  this->ParseSelectDecl(decl);

  // ensure that there is a space between alphanumeric tokens
  std::string remaining = select_match.suffix();

  if (this->IsStrEmpty(remaining) || decl.back() == '>' ||
      this->IsStrEmpty(remaining.substr(0, 1))) {
    return remaining;
  }

  throw QueryParseException(
      "Syntax error no whitespace after declaration selected");
}

void Parser::ParseSelectDecl(const std::string select_decl) {
  // remove 'Select' and all whitespace or tabs
  std::string selected_decl =
      this->Remove(this->Remove(select_decl, {kSelectRegexStr}, true),
                   {kWhitespaceRegexStr});

  if (selected_decl == kBoolRegexStr) {
    this->query_builder_.SelectBoolean();
  } else {
    // build selected declarations
    std::regex decl_regex(kDeclRefRegexStr);
    for (std::sregex_iterator i = std::sregex_iterator(
             selected_decl.begin(), selected_decl.end(), decl_regex);
         i != std::sregex_iterator(); ++i) {
      std::smatch match = *i;

      // build selected declaration
      auto decl_pair = this->ParseDeclWithVal(match.str());
      this->query_builder_.SelectDeclaration(decl_pair.first, decl_pair.second);
    }
  }
}

const std::string Parser::TokenizeRelationalClauses(const std::string s) {
  std::regex such_that_regex(kTokenizeSuchThatRegexStr);
  std::smatch relation_match;
  std::string remaining;
  for (std::sregex_iterator i =
           std::sregex_iterator(s.begin(), s.end(), such_that_regex);
       i != std::sregex_iterator(); ++i) {
    relation_match = *i;

    remaining += relation_match.prefix();
    // parse each such that
    std::string curr_relations = relation_match.str();
    while (true) {
      // parse each relation
      curr_relations = this->ParseRelationalClause(curr_relations);
      curr_relations =
          this->Remove(curr_relations, {kTrailingWhitespaceRegexStr});

      if (curr_relations.empty() || curr_relations == ")") break;
    }
  }

  if (relation_match.empty()) return s;

  remaining += relation_match.suffix().str();
  return remaining;
}

std::string Parser::ParseRelationalClause(const std::string relation) {
  // parse relation type
  std::regex decl_type_regex(kRelationTypeRegexStr);
  std::smatch clause_type_match;
  std::regex_search(relation, clause_type_match, decl_type_regex);

  Relational::RelType clause_type =
      kStringToRelationalType.at(clause_type_match.str());
  const std::string args = clause_type_match.suffix();

  // parse relation arguments
  std::string arg1, arg2;
  std::regex ref_regex(kRefRegexStr);
  std::smatch args_match;
  for (std::sregex_iterator i =
           std::sregex_iterator(args.begin(), args.end(), ref_regex);
       i != std::sregex_iterator(); ++i) {
    args_match = *i;
    if (arg1.empty()) {
      arg1 = args_match.str();
    } else if (arg2.empty()) {
      arg2 = args_match.str();
      break;
    }
  }

  // build relation
  switch (clause_type) {
    case Relational::RelType::kFollows:
      this->query_builder_.AddRelationFollows(arg1, arg2, false);
      break;
    case Relational::RelType::kFollowsT:
      this->query_builder_.AddRelationFollows(arg1, arg2, true);
      break;
    case Relational::RelType::kParent:
      this->query_builder_.AddRelationParent(arg1, arg2, false);
      break;
    case Relational::RelType::kParentT:
      this->query_builder_.AddRelationParent(arg1, arg2, true);
      break;
    case Relational::RelType::kUses:
      this->query_builder_.AddRelationUses(arg1, arg2);
      break;
    case Relational::RelType::kModifies:
      this->query_builder_.AddRelationModifies(arg1, arg2);
      break;
    case Relational::RelType::kAffects:
      this->query_builder_.AddRelationAffects(arg1, arg2, false, false);
      break;
    case Relational::RelType::kAffectsT:
      this->query_builder_.AddRelationAffects(arg1, arg2, true, false);
      break;
    case Relational::RelType::kAffectsBip:
      this->query_builder_.AddRelationAffects(arg1, arg2, false, true);
      break;
    case Relational::RelType::kAffectsBipT:
      this->query_builder_.AddRelationAffects(arg1, arg2, true, true);
      break;
    case Relational::RelType::kCalls:
      this->query_builder_.AddRelationCalls(arg1, arg2, false);
      break;
    case Relational::RelType::kCallsT:
      this->query_builder_.AddRelationCalls(arg1, arg2, true);
      break;
    case Relational::RelType::kNext:
      this->query_builder_.AddRelationNext(arg1, arg2, false, false);
      break;
    case Relational::RelType::kNextT:
      this->query_builder_.AddRelationNext(arg1, arg2, true, false);
      break;
    case Relational::RelType::kNextBip:
      this->query_builder_.AddRelationNext(arg1, arg2, false, true);
      break;
    case Relational::RelType::kNextBipT:
      this->query_builder_.AddRelationNext(arg1, arg2, true, true);
      break;
  }

  return args_match.suffix();
}

const std::string Parser::TokenizePatterns(const std::string s) {
  std::regex pattern_regex(kTokenizePatternRegexStr);
  std::smatch pattern_match;
  std::string remaining;
  for (std::sregex_iterator i =
           std::sregex_iterator(s.begin(), s.end(), pattern_regex);
       i != std::sregex_iterator(); ++i) {
    pattern_match = *i;

    remaining += pattern_match.prefix();
    // parse pattern
    std::string curr_relations = pattern_match.str();
    while (true) {
      // parse each pattern
      curr_relations = this->ParsePattern(curr_relations);
      curr_relations =
          this->Remove(curr_relations, {kTrailingWhitespaceRegexStr});
      curr_relations = this->Remove(curr_relations, {kAndRegexStr}, true);
      if (curr_relations.empty() || curr_relations == ")") break;
    }
  }

  if (pattern_match.empty()) return s;

  remaining += pattern_match.suffix().str();
  return remaining;
}

std::string Parser::ParsePattern(const std::string pattern) {
  // parse pattern type
  std::regex type_regex(kPatternTypeRegexStr);
  std::smatch type_match;
  std::regex_search(pattern, type_match, type_regex);
  std::string type =
      this->Remove(type_match.str(), {kTrailingWhitespaceRegexStr});
  const std::string var_expr_str = type_match.suffix();

  // parse pattern variable
  std::regex entity_ref_regex(kEntityRefRegexStr);
  std::smatch ref_match;
  std::regex_search(var_expr_str, ref_match, entity_ref_regex);
  std::string var = ref_match.str();
  const std::string expr_str = ref_match.suffix();

  // parse pattern expression
  std::regex expr_regex(kPatternExprRegexStr);
  std::smatch expr_match;
  std::regex_search(expr_str, expr_match, expr_regex);
  std::string expr = expr_match.str();
  const std::string expr_2_str = expr_match.suffix();

  std::string remaining = expr_match.suffix();

  // parse pattern second expression
  std::string expr_2 = "";
  std::regex expr_2_regex(kWildRegexStr);
  std::smatch expr_2_match;
  bool has_second_expr =
      std::regex_search(expr_2_str, expr_2_match, expr_2_regex);
  if (has_second_expr &&
      this->Remove(expr_2_match.prefix(), {kWhitespaceRegexStr}) == ",") {
    expr_2 = expr_2_match.str();
    remaining = expr_2_match.suffix();
  }

  this->query_builder_.AddPattern(type, var, expr, expr_2);

  return remaining;
}

const std::string Parser::TokenizeCompare(const std::string s) {
  std::regex compare_regex(kTokenizeCompareRegexStr);
  std::smatch compare_match;
  std::string remaining;
  for (std::sregex_iterator i =
           std::sregex_iterator(s.begin(), s.end(), compare_regex);
       i != std::sregex_iterator(); ++i) {
    compare_match = *i;

    remaining += compare_match.prefix();
    // parse with clauses
    std::string curr_relations = this->Remove(
        this->Remove(compare_match.str(), {kWithKeywordRegexStr}, true),
        {kTrailingWhitespaceRegexStr});
    while (true) {
      // parse each with clause
      curr_relations = this->ParseCompare(curr_relations);
      curr_relations = this->Remove(curr_relations, {kAndRegexStr}, true);
      curr_relations =
          this->Remove(curr_relations, {kTrailingWhitespaceRegexStr});
      if (curr_relations.empty()) break;
    }
  }

  if (compare_match.empty()) return s;

  remaining += compare_match.suffix().str();

  // ensure that there is a space between alphanumeric tokens
  if (this->IsStrEmpty(remaining) || this->IsStrEmpty(remaining.substr(0, 1))) {
    return remaining;
  }

  throw QueryParseException("Syntax error no whitespace after with clause");
}

std::string Parser::ParseCompare(const std::string compare) {
  // parse with clause arguments
  std::string lhs, rhs;
  std::regex ref_regex(kCompareRefRegexStr);
  std::smatch args_match;
  for (std::sregex_iterator i =
           std::sregex_iterator(compare.begin(), compare.end(), ref_regex);
       i != std::sregex_iterator(); ++i) {
    args_match = *i;
    if (lhs.empty()) {
      lhs = args_match.str();
    } else if (rhs.empty()) {
      rhs = args_match.str();
      break;
    }
  }

  // build compare clause
  auto lhs_pair = this->ParseDeclWithVal(lhs);
  auto rhs_pair = this->ParseDeclWithVal(rhs);
  this->query_builder_.AddWithCompare(lhs_pair.first, rhs_pair.first,
                                      lhs_pair.second, rhs_pair.second);

  return args_match.suffix();
}

}  // namespace pql

}  // namespace spa
