#include "ast/parser/ParseException.h"
#include "ast/parser/Parser.h"

#include <regex>

#include "catch.hpp"


void test_tokenizer_positive(std::string s, std::vector<std::string> expected) {
  REQUIRE(spa::ast::Parser::Tokenize(s) == expected);
}

void test_tokenizer_negative(std::string s) {
  REQUIRE_THROWS_AS(spa::ast::Parser::Tokenize(s), spa::ast::ParseException);
}

TEST_CASE("Tokenize No Conditionals") {
  SECTION("Positive") {
    SECTION("Standard Test") {
      test_tokenizer_positive(
          "procedure main {"
          "x =   5;"
          "y = y + 2;"
          "}",
          {"procedure main", "x = 5", "y = y + 2", "}"});
    }

    SECTION("Whitespace Test") {
      test_tokenizer_positive(
          "procedure        main {"
          "x    =       5     ;"
          "y  =  y  +   2;"
          "   } ",
          {"procedure main", "x = 5", "y = y + 2", "}"});
    }

    SECTION("Inline Test") {
      test_tokenizer_positive("procedure main {x=5;y=y+2;}",
                              {"procedure main", "x=5", "y=y+2", "}"});
    }

    SECTION("Inline Test with Whitespace") {
      test_tokenizer_positive("procedure main {x = 5; y = y + 2 ;}",
                              {"procedure main", "x = 5", "y = y + 2", "}"});
    }

    SECTION("Brackets") {
      test_tokenizer_positive(
          "procedure main {"
          "x = (x + 2 * (3 / 6) * 5) + 9;"
          "}",
          {"procedure main", "x = (x + 2 * (3 / 6) * 5) + 9", "}"});
    }

    SECTION("More than 1 procedure") {
      test_tokenizer_positive(
          "procedure main { print x; } procedure sub { print y;}",
          {"procedure main", "print x", "}", "procedure sub", "print y", "}"});
    }
  }

  SECTION("Negative") {
    SECTION("No semicolon") {
      test_tokenizer_negative("procedure main {x = 5; y = y + 2}");
    }
    SECTION("Empty statement") {
      test_tokenizer_negative("procedure main{ ; x = 4;}");
    }
    SECTION("More than 1 semicolon") {
      test_tokenizer_negative("procedure main{ print x; x = 4;;}");
    }
    SECTION("Parenthesis in invalid position 1") {
      test_tokenizer_negative("procedure main{ { print x; x= 4;}");
    }
    SECTION("Parenthesis in invalid position 2") {
      test_tokenizer_negative("procedure main{ print x; {  x= 4;}");
    }
  }
}

TEST_CASE("Tokenize While") {
  SECTION("Positive") {
    SECTION("Standard While Loop") {
      test_tokenizer_positive(
          "procedure main {"
          "while (x == 0) {"
          "x = x + 3;"
          "print Y;"
          "}"
          "}",
          {"procedure main", "while x == 0", "x = x + 3", "print Y", "}", "}"});
    }
    SECTION("Whitespace Test") {
      test_tokenizer_positive(
          "procedure main {"
          "while    (  x     ==     0       )          {"
          "x = x + 3;"
          "print Y;"
          "   }"
          "          }",
          {"procedure main", "while x == 0", "x = x + 3", "print Y", "}", "}"});
    }

    SECTION("Nested While loops") {
      test_tokenizer_positive(
          "procedure main {"
          "while (x == 0) {"
          "x = x + 3;"
          "while (y > 5) {"
          "call X;"
          "}"
          "print Y;"
          "}"
          "}",
          {"procedure main", "while x == 0", "x = x + 3", "while y > 5",
           "call X", "}", "print Y", "}", "}"});
    }

    SECTION("Nested While loops with whitespace") {
      test_tokenizer_positive(
          "procedure main {"
          "while    (  x     ==     0       )          {"
          "x = x + 3;"
          "while (   y > 5   ) {"
          "   call X;   }"
          "print Y;"
          "   }"
          "          }",
          {"procedure main", "while x == 0", "x = x + 3", "while y > 5",
           "call X", "}", "print Y", "}", "}"});
    }

    SECTION("Brackets") {
      test_tokenizer_positive(
          "procedure main {"
          "while (x + 3 > (y * 5)) {"
          "x = x + 3;"
          "while ((y) > 5) {"
          "call X;"
          "}"
          "print Y;"
          "}"
          "}",
          {"procedure main", "while x + 3 > (y * 5)", "x = x + 3",
           "while (y) > 5", "call X", "}", "print Y", "}", "}"});
    }
  }

  SECTION("Negative") {
    SECTION("Invalid while 1") {
      test_tokenizer_negative(
          "procedure main {"
          "while ( x  ==  0 {"
          "x = x + 3;"
          "print Y;"
          " }"
          " }");
    }

    SECTION("Invalid while 2") {
      test_tokenizer_negative(
          "procedure main {"
          "while ( x  ==  0 "
          "x = x + 3;"
          "print Y;"
          " }"
          " }");
    }
  }
}

TEST_CASE("Tokenize If Statements") {
  SECTION("Standard If Then Else") {
    test_tokenizer_positive(
        "procedure main {"
        "if (x == 0) then {"
        "x = x + 3;"
        "} else {"
        "call X;"
        "}"
        "print Y;"
        "}",
        {"procedure main", "if x == 0", "x = x + 3", "}", "else", "call X", "}",
         "print Y", "}"});
  }
  SECTION("Whitespace Test") {
    test_tokenizer_positive(
        "procedure main {"
        "if  (  x   ==  0 )    then {"
        "x  =   x + 3 ;"
        "  }   else   {   "
        "    call X;  "
        "   }"
        "  print Y;"
        "}   ",
        {"procedure main", "if x == 0", "x = x + 3", "}", "else", "call X", "}",
         "print Y", "}"});
  }
  SECTION("No then statement") {
    test_tokenizer_negative(
        "procedure main {"
        "if (x == 0){"
        "x = x + 3;"
        "} else {"
        "call X;"
        "}"
        "print Y;"
        "}");
  }

  SECTION("No else statement") {
    test_tokenizer_negative(
        "procedure main {"
        "if (x == 0) {"
        "x = x + 3;"
        "}"
        "print Y;"
        "}");
  }

  SECTION("Nested if") {
    test_tokenizer_positive(
        "procedure main {"
        "if (x == 0) then {"
        "x = x + 3;"
        "if (y > 3) then {"
        "z = z + 2;"
        "} else {"
        "call X;"
        "}"
        "y = 1;"
        "} else {"
        "x = 2;"
        "}"
        "}"
        "print Y;"
        "}",
        {"procedure main", "if x == 0", "x = x + 3", "if y > 3", "z = z + 2",
         "}", "else", "call X", "}", "y = 1", "}", "else", "x = 2", "}", "}",
         "print Y", "}"});
  }
}

TEST_CASE("Tokenize") {
  SECTION("test") {
    test_tokenizer_positive(
        "procedure main {"
        "if ((x + 1) > 2) then {"
        "x = x + 3;"
        "} else {"
        "call X;"
        "}"
        "print Y;}",
        {"procedure main", "if (x + 1) > 2", "x = x + 3", "}", "else", "call X",
         "}", "print Y", "}"});
  }
  SECTION("test2") {
    test_tokenizer_positive(
        "procedure main {"
        "while ((x + 1) > 2) {"
        "x = x + 3;"
        "}"
        "print Y;}",
        {"procedure main", "while (x + 1) > 2", "x = x + 3", "}", "print Y",
         "}"});
  }
}
