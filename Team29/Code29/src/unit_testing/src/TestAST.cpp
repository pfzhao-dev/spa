#include <regex>

#include "catch.hpp"

#include <ast/parser/ParseException.h>
#include <ast/parser/Parser.h>
#include "ASTHelper.h"

void require(bool b) { REQUIRE(b); }

TEST_CASE("Single Call") {
  std::string str =
      "procedure main {"
      "call main2;"
      "}"
      "procedure main2 {"
      "print x;"
      "}";
  std::shared_ptr<spa::ast::Call> call = buildCall("main", "main2", 1);
  std::shared_ptr<spa::ast::StatementList> stmtList =
      buildStatementList({call});
  std::shared_ptr<spa::ast::Procedure> procedure =
      buildProcedure("main", stmtList);

  std::shared_ptr<spa::ast::Print> print = buildPrint("x", 2);
  std::shared_ptr<spa::ast::StatementList> stmtList2 =
      buildStatementList({print});
  std::shared_ptr<spa::ast::Procedure> procedure2 =
      buildProcedure("main2", stmtList2);

  std::shared_ptr<spa::ast::Program> program =
      buildProgram({procedure, procedure2});

  REQUIRE(*spa::ast::Parser(str).Parse() == *program);
}

TEST_CASE("Test Read") {
  SECTION("Positive") {
    std::string str =
        "procedure main {"
        "read X;"
        "}";
    std::shared_ptr<spa::ast::Read> read = buildRead("X", 1);
    std::shared_ptr<spa::ast::StatementList> stmtList =
        buildStatementList({read});
    std::shared_ptr<spa::ast::Procedure> procedure =
        buildProcedure("main", stmtList);
    std::shared_ptr<spa::ast::Program> program = buildProgram({procedure});
    REQUIRE(*spa::ast::Parser(str).Parse() == *program);
  }SECTION("Negative") {
    SECTION("More than 1 argument to read") {
      std::string str =
          "procedure main {"
          "read X y;"
          "}";
      REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                        spa::ast::ParseException);
    }

    SECTION("Read statement in conditional") {
      std::string str =
          "procedure main {"
          "if (read x) {"
          "x = x + 2;"
          ";"
          "}";
      REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                        spa::ast::ParseException);
    }

    SECTION("Multiple Read call") {
      std::string str =
          "procedure main {"
          "read read x;"
          "}";
      REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                        spa::ast::ParseException);
    }SECTION("Parentheses between read") {
      std::string str =
          "procedure main {"
          "(read x);"
          "}";
      REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                        spa::ast::ParseException);
    }

    SECTION("Read statement in conditional") {
      std::string str =
          "procedure main {"
          "if (read x) {"
          "x = x + 2;"
          ";"
          "}";
      REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                        spa::ast::ParseException);
    }
  }
}

TEST_CASE("Test Print") {
  SECTION("Positive") {
    std::string str =
        "procedure main {"
        "print X;"
        "}";
    std::shared_ptr<spa::ast::Print> print = buildPrint("X", 1);
    std::shared_ptr<spa::ast::StatementList> stmtList =
        buildStatementList({print});
    std::shared_ptr<spa::ast::Procedure> procedure =
        buildProcedure("main", stmtList);
    std::shared_ptr<spa::ast::Program> program = buildProgram({procedure});
    REQUIRE(*spa::ast::Parser(str).Parse() == *program);
  }SECTION("Negative") {
    std::string str =
        "procedure main {"
        "print X y;"
        "}";
    REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(), spa::ast::ParseException);
  }
}

TEST_CASE("Test Print and Read") {
  SECTION("Positive") {
    std::string str =
        "procedure main {"
        "print X;"
        "read Y;"
        "}";
    std::shared_ptr<spa::ast::Print> print = buildPrint("X", 1);
    std::shared_ptr<spa::ast::Read> read = buildRead("Y", 2);

    std::shared_ptr<spa::ast::StatementList> stmtList =
        buildStatementList({print, read});
    std::shared_ptr<spa::ast::Procedure> procedure =
        buildProcedure("main", stmtList);
    std::shared_ptr<spa::ast::Program> program = buildProgram({procedure});
    REQUIRE(*spa::ast::Parser(str).Parse() == *program);
  }
}

TEST_CASE("Test Assign and Arithmetic") {
  SECTION("Positive") {
    SECTION("Simple") {
      std::string str =
          "procedure main {"
          "x = 3; }";
      std::shared_ptr<spa::ast::Variable> x = buildVariable("x");
      std::shared_ptr<spa::ast::Constant> three = buildConstant(3);
      std::shared_ptr<spa::ast::Assign> a = buildAssign(x, three, 1);
      std::shared_ptr<spa::ast::StatementList> stmtList =
          buildStatementList({a});
      std::shared_ptr<spa::ast::Procedure> procedure =
          buildProcedure("main", stmtList);
      std::shared_ptr<spa::ast::Program> program = buildProgram({procedure});
      REQUIRE(*spa::ast::Parser(str).Parse() == *program);
    }

    SECTION("Arithmetic") {
      SECTION("Positive") {
        SECTION("Single statement") {
          std::string str =
              "procedure main {"
              "x = y * 3; }";
          std::shared_ptr<spa::ast::Variable> x = buildVariable("x");
          std::shared_ptr<spa::ast::Variable> y = buildVariable("y");
          std::shared_ptr<spa::ast::Constant> three = buildConstant(3);
          std::shared_ptr<spa::ast::Arithmetic> ari =
              buildArithmetic(spa::ast::Arithmetic::OpType::kTimes, y, three);
          std::shared_ptr<spa::ast::Assign> a = buildAssign(x, ari, 1);
          std::shared_ptr<spa::ast::StatementList> stmtList =
              buildStatementList({a});
          std::shared_ptr<spa::ast::Procedure> procedure =
              buildProcedure("main", stmtList);
          std::shared_ptr<spa::ast::Program> program =
              buildProgram({procedure});
          REQUIRE(*spa::ast::Parser(str).Parse() == *program);
        }

        SECTION("Single statement 2") {
          std::string str =
              "procedure main {"
              "x = (y) * 3; }";
          std::shared_ptr<spa::ast::Variable> x = buildVariable("x");
          std::shared_ptr<spa::ast::Variable> y = buildVariable("y");
          std::shared_ptr<spa::ast::Constant> three = buildConstant(3);
          std::shared_ptr<spa::ast::Arithmetic> ari =
              buildArithmetic(spa::ast::Arithmetic::OpType::kTimes, y, three);
          std::shared_ptr<spa::ast::Assign> a = buildAssign(x, ari, 1);
          std::shared_ptr<spa::ast::StatementList> stmtList =
              buildStatementList({a});
          std::shared_ptr<spa::ast::Procedure> procedure =
              buildProcedure("main", stmtList);
          std::shared_ptr<spa::ast::Program> program =
              buildProgram({procedure});
          REQUIRE(*spa::ast::Parser(str).Parse() == *program);
        }

        SECTION("Single statement 3") {
          std::string str =
              "procedure main {"
              "x = x + 2 * y + z;"
              "}";
          std::shared_ptr<spa::ast::Constant> constant = buildConstant(2);
          std::shared_ptr<spa::ast::Variable> x = buildVariable("x");
          std::shared_ptr<spa::ast::Variable> y = buildVariable("y");
          std::shared_ptr<spa::ast::Variable> z = buildVariable("z");

          std::shared_ptr<spa::ast::Arithmetic> a1 = buildArithmetic(
              spa::ast::Arithmetic::OpType::kTimes, constant, y);
          std::shared_ptr<spa::ast::Arithmetic> a2 =
              buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, x, a1);
          std::shared_ptr<spa::ast::Arithmetic> a3 =
              buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, a2, z);
          std::shared_ptr<spa::ast::Assign> assign = buildAssign(x, a3, 1);
          std::shared_ptr<spa::ast::StatementList> stmtList =
              buildStatementList({assign});
          std::shared_ptr<spa::ast::Procedure> procedure =
              buildProcedure("main", stmtList);
          std::shared_ptr<spa::ast::Program> program =
              buildProgram({procedure});
          REQUIRE(*spa::ast::Parser(str).Parse() == *program);
        }

        SECTION("Single statement 4") {
          std::string str =
              "procedure main {"
              "x = x / 2 * y - z;"
              "}";
          std::shared_ptr<spa::ast::Constant> two = buildConstant(2);
          std::shared_ptr<spa::ast::Variable> x = buildVariable("x");
          std::shared_ptr<spa::ast::Variable> y = buildVariable("y");
          std::shared_ptr<spa::ast::Variable> z = buildVariable("z");

          std::shared_ptr<spa::ast::Arithmetic> a1 =
              buildArithmetic(spa::ast::Arithmetic::OpType::kDiv, x, two);
          std::shared_ptr<spa::ast::Arithmetic> a2 =
              buildArithmetic(spa::ast::Arithmetic::OpType::kTimes, a1, y);
          std::shared_ptr<spa::ast::Arithmetic> a3 =
              buildArithmetic(spa::ast::Arithmetic::OpType::kMinus, a2, z);
          std::shared_ptr<spa::ast::Assign> assign = buildAssign(x, a3, 1);
          std::shared_ptr<spa::ast::StatementList> stmtList =
              buildStatementList({assign});
          std::shared_ptr<spa::ast::Procedure> procedure =
              buildProcedure("main", stmtList);
          std::shared_ptr<spa::ast::Program> program =
              buildProgram({procedure});
          REQUIRE(*spa::ast::Parser(str).Parse() == *program);
        }

        SECTION("Multiple statement") {
          std::string str =
              "procedure main {"
              "x = y * 3;"
              "y = x + 3;"
              " }";
          std::shared_ptr<spa::ast::Variable> x = buildVariable("x");
          std::shared_ptr<spa::ast::Variable> y = buildVariable("y");
          std::shared_ptr<spa::ast::Constant> three = buildConstant(3);
          std::shared_ptr<spa::ast::Arithmetic> ari1 =
              buildArithmetic(spa::ast::Arithmetic::OpType::kTimes, y, three);
          std::shared_ptr<spa::ast::Arithmetic> ari2 =
              buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, x, three);
          std::shared_ptr<spa::ast::Assign> a1 = buildAssign(x, ari1, 1);
          std::shared_ptr<spa::ast::Assign> a2 = buildAssign(y, ari2, 2);
          std::shared_ptr<spa::ast::StatementList> stmtList =
              buildStatementList({a1, a2});
          std::shared_ptr<spa::ast::Procedure> procedure =
              buildProcedure("main", stmtList);
          std::shared_ptr<spa::ast::Program> program =
              buildProgram({procedure});
          REQUIRE(*spa::ast::Parser(str).Parse() == *program);
        }

        SECTION("Arithmetic with Parentheses") {
          SECTION("Single Statement 1") {
            std::string str =
                "procedure main {"
                "x = (z + y) * 3 + z; }";
            std::shared_ptr<spa::ast::Variable> x = buildVariable("x");
            std::shared_ptr<spa::ast::Variable> y = buildVariable("y");
            std::shared_ptr<spa::ast::Variable> z = buildVariable("z");
            std::shared_ptr<spa::ast::Constant> three = buildConstant(3);
            std::shared_ptr<spa::ast::Arithmetic> ari =
                buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, z, y);
            std::shared_ptr<spa::ast::Arithmetic> ari2 = buildArithmetic(
                spa::ast::Arithmetic::OpType::kTimes, ari, three);
            std::shared_ptr<spa::ast::Arithmetic> ari3 =
                buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, ari2, z);
            std::shared_ptr<spa::ast::Assign> a = buildAssign(x, ari3, 1);

            std::shared_ptr<spa::ast::StatementList> stmtList =
                buildStatementList({a});
            std::shared_ptr<spa::ast::Procedure> procedure =
                buildProcedure("main", stmtList);
            std::shared_ptr<spa::ast::Program> program =
                buildProgram({procedure});
            REQUIRE(*spa::ast::Parser(str).Parse() == *program);
          }

          SECTION("Single Statement 2") {
            std::string str =
                "procedure main {"
                "x = z + (y * 3); }";
            std::shared_ptr<spa::ast::Variable> x = buildVariable("x");
            std::shared_ptr<spa::ast::Variable> y = buildVariable("y");
            std::shared_ptr<spa::ast::Variable> z = buildVariable("z");
            std::shared_ptr<spa::ast::Constant> three = buildConstant(3);
            std::shared_ptr<spa::ast::Arithmetic> ari1 =
                buildArithmetic(spa::ast::Arithmetic::OpType::kTimes, y, three);
            std::shared_ptr<spa::ast::Arithmetic> ari2 =
                buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, z, ari1);

            std::shared_ptr<spa::ast::Assign> a = buildAssign(x, ari2, 1);
            std::shared_ptr<spa::ast::StatementList> stmtList =
                buildStatementList({a});
            std::shared_ptr<spa::ast::Procedure> procedure =
                buildProcedure("main", stmtList);
            std::shared_ptr<spa::ast::Program> program =
                buildProgram({procedure});
            REQUIRE(*spa::ast::Parser(str).Parse() == *program);
          }

          SECTION("Nested parentheses") {
            SECTION("Test 1") {
              std::string str =
                  "procedure main {"
                  "x = (z + y) / ((z + y) - 3); }";
              std::shared_ptr<spa::ast::Variable> x = buildVariable("x");
              std::shared_ptr<spa::ast::Variable> y = buildVariable("y");
              std::shared_ptr<spa::ast::Variable> z = buildVariable("z");
              std::shared_ptr<spa::ast::Constant> three = buildConstant(3);
              std::shared_ptr<spa::ast::Arithmetic> ari =
                  buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, z, y);
              std::shared_ptr<spa::ast::Arithmetic> ari2 = buildArithmetic(
                  spa::ast::Arithmetic::OpType::kMinus, ari, three);
              std::shared_ptr<spa::ast::Arithmetic> ari3 = buildArithmetic(
                  spa::ast::Arithmetic::OpType::kDiv, ari, ari2);
              std::shared_ptr<spa::ast::Assign> a = buildAssign(x, ari3, 1);
              std::shared_ptr<spa::ast::StatementList> stmtList =
                  buildStatementList({a});
              std::shared_ptr<spa::ast::Procedure> procedure =
                  buildProcedure("main", stmtList);
              std::shared_ptr<spa::ast::Program> program =
                  buildProgram({procedure});
              REQUIRE(*spa::ast::Parser(str).Parse() == *program);
            }

            SECTION("Test 2") {
              std::string str =
                  "procedure main {"
                  "x = (y + 3) / (x + (z - 3) * y) + 5; }";

              std::shared_ptr<spa::ast::Variable> x = buildVariable("x");
              std::shared_ptr<spa::ast::Variable> y = buildVariable("y");
              std::shared_ptr<spa::ast::Variable> z = buildVariable("z");
              std::shared_ptr<spa::ast::Constant> three = buildConstant(3);
              std::shared_ptr<spa::ast::Constant> five = buildConstant(5);

              std::shared_ptr<spa::ast::Arithmetic> ari1 = buildArithmetic(
                  spa::ast::Arithmetic::OpType::kPlus, y, three);

              std::shared_ptr<spa::ast::Arithmetic> ari2 = buildArithmetic(
                  spa::ast::Arithmetic::OpType::kMinus, z, three);
              std::shared_ptr<spa::ast::Arithmetic> ari3 = buildArithmetic(
                  spa::ast::Arithmetic::OpType::kTimes, ari2, y);
              std::shared_ptr<spa::ast::Arithmetic> ari4 =
                  buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, x, ari3);

              std::shared_ptr<spa::ast::Arithmetic> ari5 = buildArithmetic(
                  spa::ast::Arithmetic::OpType::kDiv, ari1, ari4);

              std::shared_ptr<spa::ast::Arithmetic> ari6 = buildArithmetic(
                  spa::ast::Arithmetic::OpType::kPlus, ari5, five);

              std::shared_ptr<spa::ast::Assign> a = buildAssign(x, ari6, 1);
              std::shared_ptr<spa::ast::StatementList> stmtList =
                  buildStatementList({a});
              std::shared_ptr<spa::ast::Procedure> procedure =
                  buildProcedure("main", stmtList);
              std::shared_ptr<spa::ast::Program> program =
                  buildProgram({procedure});
              REQUIRE(*spa::ast::Parser(str).Parse() == *program);
            }
          }
        }
      }

      SECTION("Negative") {
        SECTION("Operator at end") {
          std::string str =
              "procedure main {"
              "x = y + 3 *;"
              "}";
          REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                            spa::ast::ParseException);
        }

        SECTION("Consecutive Operator") {
          std::string str =
              "procedure main {"
              "x = y ++ 3;"
              "}";
          REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                            spa::ast::ParseException);
        }

        SECTION("Invalid Operator") {
          std::string str =
              "procedure main {"
              "x == y;"
              "}";
          REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                            spa::ast::ParseException);
        }

        SECTION("No Right hand side") {
          std::string str =
              "procedure main {"
              "x = y + ;"
              "}";
          REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                            spa::ast::ParseException);
        }

        SECTION("No Left hand side") {
          std::string str =
              "procedure main {"
              "x = + y;"
              "}";
          REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                            spa::ast::ParseException);
        }

        SECTION("Operator in Parentheses") {
          std::string str =
              "procedure main {"
              "x (+) y ;"
              "}";
          REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                            spa::ast::ParseException);
        }

        SECTION("Empty Parentheses") {
          std::string str =
              "procedure main {"
              "x = ();"
              "}";
          REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                            spa::ast::ParseException);
        }

        SECTION("No assignee") {
          std::string str =
              "procedure main {"
              " = y;"
              "}";
          REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                            spa::ast::ParseException);
        }

        SECTION("Assignee is constant") {
          std::string str =
              "procedure main {"
              "3 = y;"
              "}";
          REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                            spa::ast::ParseException);
        }

        SECTION("Imbalanced Parentheses") {
          SECTION("Test 1") {
            std::string str =
                "procedure main {"
                "(x + (y);"
                "}";
            REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                              spa::ast::ParseException);
          }SECTION("TEST 2") {
            std::string str =
                "procedure main {"
                "(x + (y);"
                "}";
            REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                              spa::ast::ParseException);
          }SECTION("TEST 3") {
            std::string str =
                "procedure main {"
                ") x + y (;"
                "}";
            REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                              spa::ast::ParseException);
          }SECTION("TEST 4") {
            std::string str =
                "procedure main {"
                "(x + y ;"
                "}";
            REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                              spa::ast::ParseException);
          }SECTION("TEST 5") {
            std::string str =
                "procedure main {"
                "(x + y) * (y / (5 + z);"
                "}";
            REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                              spa::ast::ParseException);
          }

          SECTION("TEST 6") {
            std::string str =
                "procedure main {"
                "(x + y) * (y / 5 + z));"
                "}";
            REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                              spa::ast::ParseException);
          }

          SECTION("TEST 7") {
            std::string str =
                "procedure main {"
                "(x / y) - (y + 5 * z));"
                "}";
            REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                              spa::ast::ParseException);
          }

          SECTION("TEST 8") {
            std::string str =
                "procedure main {"
                "(x / y) - (y + (5) * (z + 25)));"
                "}";
            REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                              spa::ast::ParseException);
          }
        }
      }
    }
  }
}

TEST_CASE("Test If") {
  SECTION("Positive") {
    SECTION("Basic") {
      std::string str =
          "procedure main {"
          "if (x == 0) then {"
          "print x;"
          "} else {"
          "read y;"
          "}"
          "}";
      std::shared_ptr<spa::ast::Variable> x = buildVariable("x");
      std::shared_ptr<spa::ast::Constant> zero = buildConstant(0);

      std::shared_ptr<spa::ast::Relational> rel =
          buildRelational(spa::ast::Relational::OpType::kEq, x, zero);
      std::shared_ptr<spa::ast::Print> print = buildPrint("x", 2);
      std::shared_ptr<spa::ast::Read> read = buildRead("y", 3);
      std::shared_ptr<spa::ast::StatementList> cons =
          buildStatementList({print});
      std::shared_ptr<spa::ast::StatementList> alt = buildStatementList({read});
      std::shared_ptr<spa::ast::If> ifs = buildIf(rel, cons, alt, 1);
      std::shared_ptr<spa::ast::StatementList> stmtList =
          buildStatementList({ifs});
      std::shared_ptr<spa::ast::Procedure> procedure =
          buildProcedure("main", stmtList);
      std::shared_ptr<spa::ast::Program> program = buildProgram({procedure});
      REQUIRE(*spa::ast::Parser(str).Parse() == *program);
    }

    SECTION("Nested If") {
      std::string str =
          "procedure main {"
          "if (x == 0) then {"
          "print x;"
          "if (x != 0) then {"
          "read x;"
          "} else {"
          "read x;"
          "} }"
          "else {"
          "print y;"
          "}"
          "}";
      std::shared_ptr<spa::ast::Variable> x = buildVariable("x");
      std::shared_ptr<spa::ast::Constant> zero = buildConstant(0);

      std::shared_ptr<spa::ast::Relational> eq =
          buildRelational(spa::ast::Relational::OpType::kEq, x, zero);
      std::shared_ptr<spa::ast::Relational> neq =
          buildRelational(spa::ast::Relational::OpType::kNeq, x, zero);
      std::shared_ptr<spa::ast::Print> print = buildPrint("x", 2);
      std::shared_ptr<spa::ast::Read> read2 = buildRead("x", 4);
      std::shared_ptr<spa::ast::Read> read = buildRead("x", 5);
      std::shared_ptr<spa::ast::Print> printY = buildPrint("y", 6);

      std::shared_ptr<spa::ast::StatementList> cons =
          buildStatementList({read2});
      std::shared_ptr<spa::ast::StatementList> alt = buildStatementList({read});
      std::shared_ptr<spa::ast::If> ifs = buildIf(neq, cons, alt, 3);

      std::shared_ptr<spa::ast::StatementList> cons2 =
          buildStatementList({print, ifs});
      std::shared_ptr<spa::ast::StatementList> alt2 =
          buildStatementList({printY});
      std::shared_ptr<spa::ast::If> ifs2 = buildIf(eq, cons2, alt2, 1);

      std::shared_ptr<spa::ast::StatementList> stmtList =
          buildStatementList({ifs2});
      std::shared_ptr<spa::ast::Procedure> procedure =
          buildProcedure("main", stmtList);
      std::shared_ptr<spa::ast::Program> program = buildProgram({procedure});
      REQUIRE(*spa::ast::Parser(str).Parse() == *program);
    }
  }SECTION("Negative") {
    SECTION("No then clause") {
      std::string str =
          "procedure main {"
          "if (x == 0) {"
          "print x;}"
          "}";
      REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                        spa::ast::ParseException);
    }SECTION("No else clause") {
      std::string str =
          "procedure main {"
          "if (x == 0) then {"
          "print x;"
          "}"
          "}";
      REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                        spa::ast::ParseException);
    }SECTION("no parentheses") {
      std::string str =
          "procedure main {"
          "if x == 0 then {"
          "print x;}"
          "}";
      REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                        spa::ast::ParseException);
    }SECTION("Missing parentheses 1") {
      std::string str =
          "procedure main {"
          "if (x == 0) then {"
          "print x;"
          "}";
      REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                        spa::ast::ParseException);
    }SECTION("Missing parentheses 2") {
      std::string str =
          "procedure main {"
          "if (x == 0) then {"
          "print x;"
          "} else {"
          "call x;"
          "}";
      REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                        spa::ast::ParseException);
    }

    SECTION("Extra parentheses 1") {
      std::string str =
          "procedure main {"
          "if (x == 0) then {"
          "print x;"
          "} else {"
          "call x;"
          "} }"
          "}";
      REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                        spa::ast::ParseException);
    }

    SECTION("Extra parentheses 2") {
      std::string str =
          "procedure main {"
          "if (x == 0) then {"
          "print x;"
          "} } else {"
          "call x;"
          "}"
          "}";
      REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                        spa::ast::ParseException);
    }

    SECTION("Wrong order") {
      std::string str =
          "procedure main {"
          "if (x == 0) then {"
          "print x;"
          "{ else }"
          "call x;"
          "}"
          "}";
      REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                        spa::ast::ParseException);
    }

    SECTION("Nested invalid") {
      std::string str =
          "procedure main {"
          "if (x == 0) then {"
          "print x;"
          "if (x != 3) then {"
          "print y;"
          "} else {"
          "call x;"
          "}"
          "}";
      REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                        spa::ast::ParseException);
    }

    SECTION("Nested invalid 2") {
      std::string str =
          "procedure main {"
          "if (x == 0) then {"
          "print x;"
          "if (x != 3) then {"
          "print y;"
          "} else {"
          "call x;"
          "}"
          "}";
      REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                        spa::ast::ParseException);
    }
  }
}

TEST_CASE("Test While") {
  SECTION("Positive") {
    SECTION("Basic") {
      std::string str =
          "procedure main {"
          "while (x == 0) {"
          "print x;"
          "}"
          "}";
      std::shared_ptr<spa::ast::Variable> x = buildVariable("x");
      std::shared_ptr<spa::ast::Constant> zero = buildConstant(0);

      std::shared_ptr<spa::ast::Relational> rel =
          buildRelational(spa::ast::Relational::OpType::kEq, x, zero);
      std::shared_ptr<spa::ast::Print> print = buildPrint("x", 2);
      std::shared_ptr<spa::ast::StatementList> cons =
          buildStatementList({print});

      std::shared_ptr<spa::ast::While> whiles = buildWhile(rel, cons, 1);
      std::shared_ptr<spa::ast::StatementList> stmtList =
          buildStatementList({whiles});
      std::shared_ptr<spa::ast::Procedure> procedure =
          buildProcedure("main", stmtList);
      std::shared_ptr<spa::ast::Program> program = buildProgram({procedure});
      REQUIRE(*spa::ast::Parser(str).Parse() == *program);
    }

    SECTION("Nested While") {
      std::string str =
          "procedure main {"
          "while (x == 0) {"
          "print x;"
          "while (x != 0) {"
          "read x;"
          "}"
          "}"
          "}";
      std::shared_ptr<spa::ast::Variable> x = buildVariable("x");
      std::shared_ptr<spa::ast::Constant> zero = buildConstant(0);

      std::shared_ptr<spa::ast::Relational> eq =
          buildRelational(spa::ast::Relational::OpType::kEq, x, zero);

      std::shared_ptr<spa::ast::Relational> neq =
          buildRelational(spa::ast::Relational::OpType::kNeq, x, zero);
      std::shared_ptr<spa::ast::Print> print = buildPrint("x", 2);
      std::shared_ptr<spa::ast::Read> read = buildRead("x", 4);

      std::shared_ptr<spa::ast::StatementList> stmts =
          buildStatementList({read});

      std::shared_ptr<spa::ast::While> whiles = buildWhile(neq, stmts, 3);
      std::shared_ptr<spa::ast::StatementList> stmts2 =
          buildStatementList({print, whiles});
      std::shared_ptr<spa::ast::While> whiles2 = buildWhile(eq, stmts2, 1);

      std::shared_ptr<spa::ast::StatementList> stmtList =
          buildStatementList({whiles2});
      std::shared_ptr<spa::ast::Procedure> procedure =
          buildProcedure("main", stmtList);
      std::shared_ptr<spa::ast::Program> program = buildProgram({procedure});
      REQUIRE(*spa::ast::Parser(str).Parse() == *program);
    }
  }SECTION("Negative") {
    SECTION("No parentheses") {
      std::string str =
          "procedure main {"
          "while x == 0 {"
          "print x;"
          "}"
          "}";
      REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                        spa::ast::ParseException);
    }

    SECTION("Missing parentheses") {
      std::string str =
          "procedure main {"
          "while x == 0 "
          "print x;"
          "}"
          "}";
      REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                        spa::ast::ParseException);
    }SECTION("Nested invalid") {
      std::string str =
          "procedure main {"
          "while (x == 0) {"
          "print x;"
          "while (x != 0) {"
          "print y;"
          "}";
      REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                        spa::ast::ParseException);
    }

    SECTION("Nested invalid 2") {
      std::string str =
          "procedure main {"
          "while (x == 0) {"
          "print x;"
          "while (x != 0) {"
          "print y;"
          "while (x == 0) {"
          "print z;"
          "}"
          "}"
          "}"
          "}"
          "}";

      REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                        spa::ast::ParseException);
    }SECTION("Empty while") {
      std::string str =
          "procedure main {"
          "while (x == 0) {"
          "}"
          "}";

      REQUIRE_THROWS_AS(*spa::ast::Parser(str).Parse(),
                        spa::ast::ParseException);
    }
  }
}

TEST_CASE("Sample Simple Program") {
  std::string str =
      "procedure main {"
      "while (i > 0) {"
      "x = x + z * 5;"
      "z = 2;"
      "}"
      "print x;"
      "}";

  std::shared_ptr<spa::ast::Variable> x = buildVariable("x");
  std::shared_ptr<spa::ast::Variable> z = buildVariable("z");
  std::shared_ptr<spa::ast::Variable> i = buildVariable("i");

  std::shared_ptr<spa::ast::Constant> zero = buildConstant(0);
  std::shared_ptr<spa::ast::Constant> five = buildConstant(5);
  std::shared_ptr<spa::ast::Constant> two = buildConstant(2);

  std::shared_ptr<spa::ast::Arithmetic> a1 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kTimes, z, five);
  std::shared_ptr<spa::ast::Arithmetic> a2 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, x, a1);

  std::shared_ptr<spa::ast::Assign> assign1 = buildAssign(x, a2, 2);

  std::shared_ptr<spa::ast::Assign> assign2 = buildAssign(z, two, 3);
  std::shared_ptr<spa::ast::StatementList> stmtLst =
      buildStatementList({assign1, assign2});
  std::shared_ptr<spa::ast::Relational> cond =
      buildRelational(spa::ast::Relational::OpType::kGt, i, zero);
  std::shared_ptr<spa::ast::While> whiles = buildWhile(cond, stmtLst, 1);
  std::shared_ptr<spa::ast::Print> print = buildPrint("x", 4);
  std::shared_ptr<spa::ast::StatementList> procStmtList =
      buildStatementList({whiles, print});

  std::shared_ptr<spa::ast::Procedure> procedure =
      buildProcedure("main", procStmtList);
  std::shared_ptr<spa::ast::Program> program = buildProgram({procedure});

  REQUIRE(*spa::ast::Parser(str).Parse() == *program);
}

// TEST_CASE("Test PKB Pointer") {
//    // same test case as before, but testing pointer
//    shared_ptr<spa::ast::Variable> x = buildVariable("x");
//    shared_ptr<spa::ast::Variable> z = buildVariable("z");
//    shared_ptr<spa::ast::Variable> i = buildVariable("i");
//
//    shared_ptr<spa::ast::Constant> zero = buildConstant(0);
//    shared_ptr<spa::ast::Constant> five = buildConstant(5);
//    shared_ptr<spa::ast::Constant> two = buildConstant(2);
//
//    shared_ptr<spa::ast::Arithmetic> a1 =
//    buildArithmetic(spa::ast::Arithmetic::OpType::kTimes, z, five);
//    shared_ptr<spa::ast::Arithmetic> a2 =
//    buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, x, a1);
//
//    shared_ptr<spa::ast::Assign> assign1 = buildAssign(x, a2, 2);
//
//    shared_ptr<spa::ast::Assign> assign2 = buildAssign(z, two, 3);
//    shared_ptr<spa::ast::StatementList> stmtLst = buildStatementList({ assign1
//    , assign2 }); shared_ptr<spa::ast::Relational> cond =
//    buildRelational(spa::ast::Relational::OpType::kGt, i, zero);
//    shared_ptr<spa::ast::While> whiles = buildWhile(cond, stmtLst, 1);
//    shared_ptr<spa::ast::Print> print = buildPrint("x", 4);
//    shared_ptr<spa::ast::StatementList> procStmtList = buildStatementList({
//    whiles, print });
//
//    std::shared_ptr<spa::ast::Procedure> procedure = buildProcedure("main",
//    procStmtList); std::shared_ptr<spa::ast::Program> program = buildProgram({
//    procedure });
//
//    std::string str = "procedure main {"
//        "while (i > 0) {"
//        "x = x + z * 5;"
//        "z = 2;"
//        "}"
//        "print x;"
//        "}";
//
//    Program program2 = *spa::ast::parser(str).Parse();
//    cout << (program2.GetProcList())[0]->GetName() << endl;
//    shared_ptr<StatementList> sl =
//    ((program2.GetProcList())[0]->GetStmtList());
//
//    spa::ast::While astWhile = static_cast<spa::ast::While
//    const&>(*(sl->GetStatements())[0]); shared_ptr<spa::ast::Statement>
//    astAssign1 = (astWhile.GetStmtList())->GetStatements()[0];
//    REQUIRE(astAssign1 != nullptr);
//    REQUIRE(*spa::ast::parser(str).Parse() == program2);
//    REQUIRE(*astWhile.GetParent() == *procStmtList);
//    REQUIRE(*astWhile.GetStmtList() == *stmtLst);
//    REQUIRE(*astAssign1 == *assign1);
//    REQUIRE(*astAssign1->GetParent() == *stmtLst);
//}

TEST_CASE("Test equals metod") {
  // same test case as before, but testing pointer

  std::shared_ptr<spa::ast::Variable> x = buildVariable("x");
  std::shared_ptr<spa::ast::Variable> z = buildVariable("z");
  std::shared_ptr<spa::ast::Variable> i = buildVariable("i");

  std::shared_ptr<spa::ast::Constant> zero = buildConstant(0);
  std::shared_ptr<spa::ast::Constant> five = buildConstant(5);
  std::shared_ptr<spa::ast::Constant> two = buildConstant(2);

  std::shared_ptr<spa::ast::Arithmetic> a1 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kTimes, z, five);
  std::shared_ptr<spa::ast::Arithmetic> a2 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, x, a1);

  std::shared_ptr<spa::ast::Assign> assign1 = buildAssign(x, a2, 2);

  std::shared_ptr<spa::ast::Assign> assign2 = buildAssign(z, two, 3);
  std::shared_ptr<spa::ast::StatementList> stmtLst =
      buildStatementList({assign1, assign2});
  std::shared_ptr<spa::ast::Relational> cond =
      buildRelational(spa::ast::Relational::OpType::kGt, i, zero);
  std::shared_ptr<spa::ast::While> whiles = buildWhile(cond, stmtLst, 1);
  std::shared_ptr<spa::ast::Print> print = buildPrint("x", 4);
  std::shared_ptr<spa::ast::StatementList> procStmtList =
      buildStatementList({whiles, print});

  std::shared_ptr<spa::ast::Procedure> procedure =
      buildProcedure("main", procStmtList);
  std::shared_ptr<spa::ast::Program> program = buildProgram({procedure});

  std::string str =
      "procedure main {"
      "while (i > 0) {"
      "x = x + z * 5;"
      "z = 2;"
      "}"
      "print x;"
      "}";

  REQUIRE(*spa::ast::Parser(str).Parse() == *program);
}

TEST_CASE("System test1") {
  std::string str =
      "procedure main {"
      "      flag = 0;"
      "      call computeCentroid;"
      "      call printResults;"
      "    }"
      "    procedure readPoint {"
      "        read x;"
      "        read y;"
      "    }"
      "    procedure printResults {"
      "        print flag;"
      "        print cenX;"
      "        print cenY;"
      "        print normSq;"
      "    }"

      "    procedure computeCentroid {"
      "        count = 0;"
      "        cenX = 0;"
      "        cenY = 0;"
      "        call readPoint;"
      "        normSq = cenX * cenX + cenY * cenY;"
      "    }";

  std::shared_ptr<spa::ast::Variable> flag = buildVariable("flag");
  std::shared_ptr<spa::ast::Variable> x = buildVariable("x");
  std::shared_ptr<spa::ast::Variable> y = buildVariable("y");
  std::shared_ptr<spa::ast::Variable> count = buildVariable("count");
  std::shared_ptr<spa::ast::Variable> cenX = buildVariable("cenX");
  std::shared_ptr<spa::ast::Variable> cenY = buildVariable("cenY");
  std::shared_ptr<spa::ast::Variable> normSq = buildVariable("normSq");

  std::shared_ptr<spa::ast::Constant> zero = buildConstant(0);
  std::shared_ptr<spa::ast::Constant> one = buildConstant(1);

  std::shared_ptr<spa::ast::Assign> a = buildAssign(flag, zero, 1);
  std::shared_ptr<spa::ast::Call> callCompute = buildCall("main", "computeCentroid", 2);
  std::shared_ptr<spa::ast::Call> callPrint = buildCall("main", "printResults", 3);
  std::shared_ptr<spa::ast::StatementList> mainStmtLst =
      buildStatementList({a, callCompute, callPrint});
  std::shared_ptr<spa::ast::Procedure> main =
      buildProcedure("main", mainStmtLst);

  std::shared_ptr<spa::ast::Read> readX = buildRead("x", 4);
  std::shared_ptr<spa::ast::Read> readY = buildRead("y", 5);
  std::shared_ptr<spa::ast::StatementList> readPtStmtLst =
      buildStatementList({readX, readY});
  std::shared_ptr<spa::ast::Procedure> readPoint =
      buildProcedure("readPoint", readPtStmtLst);

  std::shared_ptr<spa::ast::Print> printFlag = buildPrint("flag", 6);
  std::shared_ptr<spa::ast::Print> printCenX = buildPrint("cenX", 7);
  std::shared_ptr<spa::ast::Print> printCenY = buildPrint("cenY", 8);
  std::shared_ptr<spa::ast::Print> printNormSq = buildPrint("normSq", 9);
  std::shared_ptr<spa::ast::StatementList> printStmtLst =
      buildStatementList({printFlag, printCenX, printCenY, printNormSq});
  std::shared_ptr<spa::ast::Procedure> printResults =
      buildProcedure("printResults", printStmtLst);

  std::shared_ptr<spa::ast::Assign> a2 = buildAssign(count, zero, 10);
  std::shared_ptr<spa::ast::Assign> a3 = buildAssign(cenX, zero, 11);
  std::shared_ptr<spa::ast::Assign> a4 = buildAssign(cenY, zero, 12);
  std::shared_ptr<spa::ast::Call> callReadPoint = buildCall("computeCentroid", "readPoint", 13);

  std::shared_ptr<spa::ast::Arithmetic> ar6 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kTimes, cenX, cenX);
  std::shared_ptr<spa::ast::Arithmetic> ar7 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kTimes, cenY, cenY);
  std::shared_ptr<spa::ast::Arithmetic> ar8 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, ar6, ar7);
  std::shared_ptr<spa::ast::Assign> a11 = buildAssign(normSq, ar8, 14);

  std::shared_ptr<spa::ast::StatementList> comStmtLst =
      buildStatementList({a2, a3, a4, callReadPoint, a11});
  std::shared_ptr<spa::ast::Procedure> comCentroid =
      buildProcedure("computeCentroid", comStmtLst);
  std::shared_ptr<spa::ast::Program> program =
      buildProgram({main, readPoint, printResults, comCentroid});

  REQUIRE(*spa::ast::Parser(str).Parse() == *program);
}

TEST_CASE("Sample If") {
  std::string str =
      "procedure main {"
      "if (count == 0) then {"
      "            flag = 1;"
      "        } else {"
      "            cenX = cenX / count;"
      "            cenY = cenY / count;"
      "        }"
      "}";

  std::shared_ptr<spa::ast::Variable> flag = buildVariable("flag");
  std::shared_ptr<spa::ast::Variable> count = buildVariable("count");
  std::shared_ptr<spa::ast::Variable> cenX = buildVariable("cenX");
  std::shared_ptr<spa::ast::Variable> cenY = buildVariable("cenY");

  std::shared_ptr<spa::ast::Constant> zero = buildConstant(0);
  std::shared_ptr<spa::ast::Constant> one = buildConstant(1);

  std::shared_ptr<spa::ast::Relational> ifCond =
      buildRelational(spa::ast::Relational::OpType::kEq, count, zero);
  std::shared_ptr<spa::ast::Assign> a8 = buildAssign(flag, one, 2);

  std::shared_ptr<spa::ast::Arithmetic> ar4 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kDiv, cenX, count);
  std::shared_ptr<spa::ast::Assign> a9 = buildAssign(cenX, ar4, 3);
  std::shared_ptr<spa::ast::Arithmetic> ar5 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kDiv, cenY, count);
  std::shared_ptr<spa::ast::Assign> a10 = buildAssign(cenY, ar5, 4);

  std::shared_ptr<spa::ast::StatementList> cons = buildStatementList({a8});
  std::shared_ptr<spa::ast::StatementList> alt = buildStatementList({a9, a10});
  std::shared_ptr<spa::ast::If> ifs = buildIf(ifCond, cons, alt, 1);
  std::shared_ptr<spa::ast::StatementList> st = buildStatementList({ifs});
  std::shared_ptr<spa::ast::Procedure> main = buildProcedure("main", st);
  std::shared_ptr<spa::ast::Program> program = buildProgram({main});

  REQUIRE(*spa::ast::Parser(str).Parse() == *program);
}

TEST_CASE("Sample While") {
  std::string str =
      "procedure main {"
      "while ((x != 0) && (y != 0)) {"
      "            count = count + 1;"
      "            cenX = cenX + x;"
      "            cenY = cenY + y;"
      "        }"
      "}";

  std::shared_ptr<spa::ast::Variable> flag = buildVariable("flag");
  std::shared_ptr<spa::ast::Variable> x = buildVariable("x");
  std::shared_ptr<spa::ast::Variable> y = buildVariable("y");
  std::shared_ptr<spa::ast::Variable> count = buildVariable("count");
  std::shared_ptr<spa::ast::Variable> cenX = buildVariable("cenX");
  std::shared_ptr<spa::ast::Variable> cenY = buildVariable("cenY");
  std::shared_ptr<spa::ast::Variable> normSq = buildVariable("normSq");
  std::shared_ptr<spa::ast::Constant> one = buildConstant(1);
  std::shared_ptr<spa::ast::Constant> zero = buildConstant(0);

  std::shared_ptr<spa::ast::Relational> rel1 =
      buildRelational(spa::ast::Relational::OpType::kNeq, x, zero);
  std::shared_ptr<spa::ast::Relational> rel2 =
      buildRelational(spa::ast::Relational::OpType::kNeq, y, zero);
  std::shared_ptr<spa::ast::Logical> whileCond =
      buildLogical(spa::ast::Logical::OpType::kAnd, rel1, rel2);

  std::shared_ptr<spa::ast::Arithmetic> ar1 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, count, one);
  std::shared_ptr<spa::ast::Assign> a5 = buildAssign(count, ar1, 2);
  std::shared_ptr<spa::ast::Arithmetic> ar2 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, cenX, x);
  std::shared_ptr<spa::ast::Assign> a6 = buildAssign(cenX, ar2, 3);
  std::shared_ptr<spa::ast::Arithmetic> ar3 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, cenY, y);
  std::shared_ptr<spa::ast::Assign> a7 = buildAssign(cenY, ar3, 4);

  std::shared_ptr<spa::ast::StatementList> whileStmtLst =
      buildStatementList({a5, a6, a7});
  std::shared_ptr<spa::ast::While> whiles =
      buildWhile(whileCond, whileStmtLst, 1);
  std::shared_ptr<spa::ast::StatementList> stm = buildStatementList({whiles});
  std::shared_ptr<spa::ast::Procedure> main = buildProcedure("main", stm);
  std::shared_ptr<spa::ast::Program> program = buildProgram({main});
  REQUIRE(*spa::ast::Parser(str).Parse() == *program);
}

TEST_CASE("Test System") {
  std::string str =
      "procedure main {"
      "      flag = 0;"
      "      call computeCentroid;"
      "      call printResults;"
      "    }"
      "    procedure readPoint {"
      "        read x;"
      "        read y;"
      "    }"
      "    procedure printResults {"
      "        print flag;"
      "        print cenX;"
      "        print cenY;"
      "        print normSq;"
      "    }"
      "    procedure computeCentroid {"
      "        count = 0;"
      "        cenX = 0;"
      "        cenY = 0;"
      "        call readPoint;"
      "        while ((x != 0) && (y != 0)) {"
      "            count = count + 1;"
      "            cenX = cenX + x;"
      "            cenY = cenY + y;"
      "            call readPoint;"
      "        }"
      "        if (count == 0) then {"
      "            flag = 1;"
      "        } else {"
      "            cenX = cenX / count;"
      "            cenY = cenY / count;"
      "        }"
      "        normSq = cenX * cenX + cenY * cenY;"
      "    }";

  std::shared_ptr<spa::ast::Variable> flag = buildVariable("flag");
  std::shared_ptr<spa::ast::Variable> x = buildVariable("x");
  std::shared_ptr<spa::ast::Variable> y = buildVariable("y");
  std::shared_ptr<spa::ast::Variable> count = buildVariable("count");
  std::shared_ptr<spa::ast::Variable> cenX = buildVariable("cenX");
  std::shared_ptr<spa::ast::Variable> cenY = buildVariable("cenY");
  std::shared_ptr<spa::ast::Variable> normSq = buildVariable("normSq");

  std::shared_ptr<spa::ast::Constant> zero = buildConstant(0);
  std::shared_ptr<spa::ast::Constant> one = buildConstant(1);

  std::shared_ptr<spa::ast::Assign> a = buildAssign(flag, zero, 1);
  std::shared_ptr<spa::ast::Call> callCompute = buildCall("main", "computeCentroid", 2);
  std::shared_ptr<spa::ast::Call> callPrint = buildCall("main", "printResults", 3);
  std::shared_ptr<spa::ast::StatementList> mainStmtLst =
      buildStatementList({a, callCompute, callPrint});
  std::shared_ptr<spa::ast::Procedure> main =
      buildProcedure("main", mainStmtLst);

  std::shared_ptr<spa::ast::Read> readX = buildRead("x", 4);
  std::shared_ptr<spa::ast::Read> readY = buildRead("y", 5);
  std::shared_ptr<spa::ast::StatementList> readPtStmtLst =
      buildStatementList({readX, readY});
  std::shared_ptr<spa::ast::Procedure> readPoint =
      buildProcedure("readPoint", readPtStmtLst);

  std::shared_ptr<spa::ast::Print> printFlag = buildPrint("flag", 6);
  std::shared_ptr<spa::ast::Print> printCenX = buildPrint("cenX", 7);
  std::shared_ptr<spa::ast::Print> printCenY = buildPrint("cenY", 8);
  std::shared_ptr<spa::ast::Print> printNormSq = buildPrint("normSq", 9);
  std::shared_ptr<spa::ast::StatementList> printStmtLst =
      buildStatementList({printFlag, printCenX, printCenY, printNormSq});
  std::shared_ptr<spa::ast::Procedure> printResults =
      buildProcedure("printResults", printStmtLst);

  std::shared_ptr<spa::ast::Assign> a2 = buildAssign(count, zero, 10);
  std::shared_ptr<spa::ast::Assign> a3 = buildAssign(cenX, zero, 11);
  std::shared_ptr<spa::ast::Assign> a4 = buildAssign(cenY, zero, 12);
  std::shared_ptr<spa::ast::Call> callReadPoint = buildCall("computeCentroid",
                                                            "readPoint", 13);

  std::shared_ptr<spa::ast::Relational> rel1 =
      buildRelational(spa::ast::Relational::OpType::kNeq, x, zero);
  std::shared_ptr<spa::ast::Relational> rel2 =
      buildRelational(spa::ast::Relational::OpType::kNeq, y, zero);
  std::shared_ptr<spa::ast::Logical> whileCond =
      buildLogical(spa::ast::Logical::OpType::kAnd, rel1, rel2);

  std::shared_ptr<spa::ast::Arithmetic> ar1 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, count, one);
  std::shared_ptr<spa::ast::Assign> a5 = buildAssign(count, ar1, 15);
  std::shared_ptr<spa::ast::Arithmetic> ar2 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, cenX, x);
  std::shared_ptr<spa::ast::Assign> a6 = buildAssign(cenX, ar2, 16);
  std::shared_ptr<spa::ast::Arithmetic> ar3 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, cenY, y);
  std::shared_ptr<spa::ast::Assign> a7 = buildAssign(cenY, ar3, 17);
  std::shared_ptr<spa::ast::Call> callReadPoint2 = buildCall("computeCentroid", "readPoint", 18);

  std::shared_ptr<spa::ast::StatementList> whileStmtLst =
      buildStatementList({a5, a6, a7, callReadPoint2});
  std::shared_ptr<spa::ast::While> whiles =
      buildWhile(whileCond, whileStmtLst, 14);

  std::shared_ptr<spa::ast::Relational> ifCond =
      buildRelational(spa::ast::Relational::OpType::kEq, count, zero);
  std::shared_ptr<spa::ast::Assign> a8 = buildAssign(flag, one, 20);

  std::shared_ptr<spa::ast::Arithmetic> ar4 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kDiv, cenX, count);
  std::shared_ptr<spa::ast::Assign> a9 = buildAssign(cenX, ar4, 21);
  std::shared_ptr<spa::ast::Arithmetic> ar5 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kDiv, cenY, count);
  std::shared_ptr<spa::ast::Assign> a10 = buildAssign(cenY, ar5, 22);

  std::shared_ptr<spa::ast::StatementList> cons = buildStatementList({a8});
  std::shared_ptr<spa::ast::StatementList> alt = buildStatementList({a9, a10});
  std::shared_ptr<spa::ast::If> ifs = buildIf(ifCond, cons, alt, 19);

  std::shared_ptr<spa::ast::Arithmetic> ar6 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kTimes, cenX, cenX);
  std::shared_ptr<spa::ast::Arithmetic> ar7 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kTimes, cenY, cenY);
  std::shared_ptr<spa::ast::Arithmetic> ar8 =
      buildArithmetic(spa::ast::Arithmetic::OpType::kPlus, ar6, ar7);
  std::shared_ptr<spa::ast::Assign> a11 = buildAssign(normSq, ar8, 23);

  std::shared_ptr<spa::ast::StatementList> comStmtLst =
      buildStatementList({a2, a3, a4, callReadPoint, whiles, ifs, a11});
  std::shared_ptr<spa::ast::Procedure> comCentroid =
      buildProcedure("computeCentroid", comStmtLst);
  std::shared_ptr<spa::ast::Program> program =
      buildProgram({main, readPoint, printResults, comCentroid});

  REQUIRE(*spa::ast::Parser(str).Parse() == *program);
}
