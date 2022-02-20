#include "TestParser.h"
#include "catch.hpp"
#include "data/clause/AffectsBipData.h"
#include "data/clause/AffectsBipTData.h"
#include "data/clause/AffectsData.h"
#include "data/clause/AffectsTData.h"
#include "data/clause/CallsData.h"
#include "data/clause/CallsTData.h"
#include "data/clause/FollowsData.h"
#include "data/clause/FollowsTData.h"
#include "data/clause/ModifiesData.h"
#include "data/clause/NextBipData.h"
#include "data/clause/NextBipTData.h"
#include "data/clause/NextData.h"
#include "data/clause/NextTData.h"
#include "data/clause/ParentData.h"
#include "data/clause/ParentTData.h"
#include "data/clause/UsesData.h"

using namespace test::pql;

TEST_CASE("Test parsing of Follows", "[pql][parser][follows]") {
  SECTION("Positive") {
    TestParserPositive(data::follows::kGetClausesPositive);
  }
  SECTION("Negative") { TestParserNegative(data::follows::kGetFuncsNegative); }
}

TEST_CASE("Test parsing of Follows*", "[pql][parser][followst]") {
  SECTION("Positive") {
    TestParserPositive(data::followst::kGetClausesPositive);
  }
  SECTION("Negative") { TestParserNegative(data::followst::kGetFuncsNegative); }
}

TEST_CASE("Test parsing of Parent", "[pql][parser][parent]") {
  SECTION("Positive") { TestParserPositive(data::parent::kGetClausesPositive); }
  SECTION("Negative") { TestParserNegative(data::parent::kGetFuncsNegative); }
}

TEST_CASE("Test parsing of Parent*", "[pql][parser][parentt]") {
  SECTION("Positive") {
    TestParserPositive(data::parentt::kGetClausesPositive);
  }
  SECTION("Negative") { TestParserNegative(data::parentt::kGetFuncsNegative); }
}

TEST_CASE("Test parsing of Uses", "[pql][parser][uses]") {
  SECTION("Positive") { TestParserPositive(data::uses::kGetClausesPositive); }
  SECTION("Negative") { TestParserNegative(data::uses::kGetFuncsNegative); }
}

TEST_CASE("Test parsing of Modifies", "[pql][parser][modifies]") {
  SECTION("Positive") {
    TestParserPositive(data::modifies::kGetClausesPositive);
  }
  SECTION("Negative") { TestParserNegative(data::modifies::kGetFuncsNegative); }
}

TEST_CASE("Test parsing of Affects", "[pql][parser][affectsbip]") {
  SECTION("Positive") {
    TestParserPositive(data::affectsbip::kGetClausesPositive);
  }
  SECTION("Negative") {
    TestParserNegative(data::affectsbip::kGetFuncsNegative);
  }
}

TEST_CASE("Test parsing of Affects*", "[pql][parser][affectsbipt]") {
  SECTION("Positive") {
    TestParserPositive(data::affectsbipt::kGetClausesPositive);
  }
  SECTION("Negative") {
    TestParserNegative(data::affectsbipt::kGetFuncsNegative);
  }
}

TEST_CASE("Test parsing of AffectsBip", "[pql][parser][affectsbip]") {
  SECTION("Positive") {
    TestParserPositive(data::affectsbip::kGetClausesPositive);
  }
  SECTION("Negative") {
    TestParserNegative(data::affectsbip::kGetFuncsNegative);
  }
}

TEST_CASE("Test parsing of AffectsBip*", "[pql][parser][affectsbipt]") {
  SECTION("Positive") {
    TestParserPositive(data::affectsbipt::kGetClausesPositive);
  }
  SECTION("Negative") {
    TestParserNegative(data::affectsbipt::kGetFuncsNegative);
  }
}

TEST_CASE("Test parsing of Calls", "[pql][parser][calls]") {
  SECTION("Positive") { TestParserPositive(data::calls::kGetClausesPositive); }
  SECTION("Negative") { TestParserNegative(data::calls::kGetFuncsNegative); }
}

TEST_CASE("Test parsing of Calls*", "[pql][parser][callst]") {
  SECTION("Positive") { TestParserPositive(data::callst::kGetClausesPositive); }
  SECTION("Negative") { TestParserNegative(data::callst::kGetFuncsNegative); }
}

TEST_CASE("Test parsing of NextBip", "[pql][parser][nextbip]") {
  SECTION("Positive") {
    TestParserPositive(data::nextbip::kGetClausesPositive);
  }
  SECTION("Negative") { TestParserNegative(data::nextbip::kGetFuncsNegative); }
}

TEST_CASE("Test parsing of NextBip*", "[pql][parser][nextbipt]") {
  SECTION("Positive") {
    TestParserPositive(data::nextbipt::kGetClausesPositive);
  }
  SECTION("Negative") { TestParserNegative(data::nextbipt::kGetFuncsNegative); }
}

TEST_CASE("Test parsing of Next", "[pql][parser][next]") {
  SECTION("Positive") { TestParserPositive(data::next::kGetClausesPositive); }
  SECTION("Negative") { TestParserNegative(data::next::kGetFuncsNegative); }
}

TEST_CASE("Test parsing of Next*", "[pql][parser][nextt]") {
  SECTION("Positive") { TestParserPositive(data::nextt::kGetClausesPositive); }
  SECTION("Negative") { TestParserNegative(data::nextt::kGetFuncsNegative); }
}
