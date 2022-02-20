#include "TestComparators.h"

#include "catch.hpp"
#include "pql/Comparators.h"

using namespace test::pql::util;

TEST_CASE("Test PtrComparator", "[pql][util][ptrcomparator]") {
  spa::pql::PtrComparator<int> ptr_comparator;

  SECTION("POSITIVE") {
    INFO("Same pointer");
    REQUIRE(ptr_comparator(kIntPtrEmpty, kIntPtrEmpty));
    REQUIRE(ptr_comparator(kIntPtr1, kIntPtr1));

    INFO("Same contents");
    REQUIRE(ptr_comparator(kIntPtrEmpty, kIntPtrEmptyDuplicate));
    REQUIRE(ptr_comparator(kIntPtr1, kIntPtr1Duplicate));
  }

  SECTION("NEGATIVE") {
    INFO("Different contents");
    REQUIRE_FALSE(ptr_comparator(kIntPtrEmpty, kIntPtr1));
    REQUIRE_FALSE(ptr_comparator(kIntPtr4, kIntPtrEmpty));
    REQUIRE_FALSE(ptr_comparator(kIntPtr1, kIntPtr2));
  }
}
