#include "catch.hpp"
#include <ast/parser/Parser.h>
#include <xtr/ExtractException.h>
#include "xtr/SourceExtractor.h"

TEST_CASE("Test Call") {
  SECTION("Positive") {
      SECTION("No cycle 1") {
      std::string str =
          "procedure main {"
          "call main2;"
          "}"
          "procedure main2 {"
          "call main3;"
          "}"
          "procedure main3 {"
          "print x;"
          "}";

      spa::xtr::SourceExtractor source_extractor;
      REQUIRE_NOTHROW(source_extractor.Extract(spa::ast::Parser(str).Parse()));
    }

    SECTION("No cycle 2") {
      std::string str =
          "procedure main {"
          "call main3;"
          "}"
          "procedure main2 {"
          "call main3;"
          "}"
          "procedure main3 {"
          "print x;"
          "}";
      spa::xtr::SourceExtractor source_extractor;
      REQUIRE_NOTHROW(source_extractor.Extract(spa::ast::Parser(str).Parse()));
    }

    SECTION("No cycle 3") {
      std::string str =
          "procedure a {"
          "call b;"
          "call d;"
          "}"
          "procedure b {"
          "call d;"
          "}"
          "procedure c {"
          "call d;"
          "}"
          "procedure d {"
          "call f;"
          "}"
          "procedure f {"
          "print x;"
          "}";
      spa::ast::Parser(str).Parse();
     spa::xtr::SourceExtractor source_extractor;
     REQUIRE_NOTHROW(source_extractor.Extract(spa::ast::Parser(str).Parse()));
    }

    SECTION("Two separate no cycles") {
      std::string str =
          "procedure a {"
          "call b;"
          "call d;"
          "}"
          "procedure b {"
          "call d;"
          "}"
          "procedure c {"
          "call d;"
          "}"
          "procedure d {"
          "call f;"
          "}"
          "procedure f {"
          "print x;"
          "}"
          "procedure main {"
          "call main2;"
          "}"
          "procedure main2 {"
          "call main3;"
          "}"
          "procedure main3 {"
          "print x;"
          "}";
      spa::xtr::SourceExtractor source_extractor;
      REQUIRE_NOTHROW(source_extractor.Extract(spa::ast::Parser(str).Parse()));
    }
  }SECTION("Negative") {
   SECTION("Call self") {
      std::string str =
          "procedure main {"
          "call main;"
          "}";
      spa::xtr::SourceExtractor source_extractor;
      REQUIRE_THROWS_AS(source_extractor.Extract(spa::ast::Parser(str).Parse()),
                        spa::xtr::ExtractException);
    }

    SECTION("Call non-existent procedure") {
      std::string str =
          "procedure a {"
          "call b;"
          "}"
          "procedure b {"
          "call d;"
          "}"
          "procedure d {"
          "call f;"
          "}";
    }

    SECTION("Cycle 1") {
      std::string str =
          "procedure a {"
          "call b;"
          "}"
          "procedure b {"
          "call d;"
          "}"
          "procedure d {"
          "call b;"
          "}";
      spa::xtr::SourceExtractor source_extractor;
      REQUIRE_THROWS_AS(source_extractor.Extract(spa::ast::Parser(str).Parse()),
                        spa::xtr::ExtractException);
    }SECTION("Cycle 2") {
      std::string str =
          "procedure a {"
          "call b;"
          "call d;"
          "}"
          "procedure b {"
          "call d;"
          "}"
          "procedure c {"
          "call d;"
          "}"
          "procedure d {"
          "call f;"
          "}"
          "procedure f {"
          "call b;"
          "}";
      spa::xtr::SourceExtractor source_extractor;
      REQUIRE_THROWS_AS(source_extractor.Extract(spa::ast::Parser(str).Parse()),
                        spa::xtr::ExtractException);
    }

    SECTION("Cycle 3") {
      std::string str =
          "procedure a {"
          "call b;"
          "call d;"
          "}"
          "procedure b {"
          "call d;"
          "}"
          "procedure c {"
          "call d;"
          "}"
          "procedure d {"
          "call f;"
          "}"
          "procedure f {"
          "call b;"
          "}";
       spa::xtr::SourceExtractor source_extractor;
      REQUIRE_THROWS_AS(source_extractor.Extract(spa::ast::Parser(str).Parse()),
                        spa::xtr::ExtractException);
    }

    SECTION("Cycle 4") {
      std::string str =
          "procedure a {"
          "call b;"
          "call d;"
          "}"
          "procedure b {"
          "call d;"
          "}"
          "procedure c {"
          "call d;"
          "}"
          "procedure d {"
          "call f;"
          "}"
          "procedure f {"
          "call b;"
          "}";
       spa::xtr::SourceExtractor source_extractor;
      REQUIRE_THROWS_AS(source_extractor.Extract(spa::ast::Parser(str).Parse()),
                        spa::xtr::ExtractException);
    }

    SECTION("Cycle 5") {
      std::string str =
          "procedure a {"
          "call b;"
          "}"
          "procedure b {"
          "call c;"
          "call d;"
          "}"
          "procedure c {"
          "print x;"
          "}"
          "procedure d {"
          "call e;"
          "}"
          "procedure e {"
          "call b;"
          "}";
       spa::xtr::SourceExtractor source_extractor;
      REQUIRE_THROWS_AS(source_extractor.Extract(spa::ast::Parser(str).Parse()),
                        spa::xtr::ExtractException);
    }

    SECTION("Two separate, 1 without cycle, 1 with cycle") {
      std::string str =
          "procedure main {"
          "call main2;"
          "}"
          "procedure main2 {"
          "print x;"
          ""
          "procedure a {"
          "call b;"
          "}"
          "procedure b {"
          "call d;"
          "}"
          "procedure d {"
          "call b;"
          "}";
       spa::xtr::SourceExtractor source_extractor;
      REQUIRE_THROWS_AS(source_extractor.Extract(spa::ast::Parser(str).Parse()),
                        spa::xtr::ExtractException);
    }
  }
}
