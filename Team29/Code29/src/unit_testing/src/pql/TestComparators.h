#pragma once

#include "catch.hpp"

namespace test {

namespace pql {

namespace util {

// int shared pointers
const std::shared_ptr<int> kIntPtrEmpty = std::make_shared<int>();
const std::shared_ptr<int> kIntPtr1 = std::make_shared<int>(1);
const std::shared_ptr<int> kIntPtr2 = std::make_shared<int>(2);
const std::shared_ptr<int> kIntPtr3 = std::make_shared<int>(3);
const std::shared_ptr<int> kIntPtr4 = std::make_shared<int>(4);

// int shared pointers duplicate
const std::shared_ptr<int> kIntPtrEmptyDuplicate = std::make_shared<int>();
const std::shared_ptr<int> kIntPtr1Duplicate = std::make_shared<int>(1);
const std::shared_ptr<int> kIntPtr2Duplicate = std::make_shared<int>(2);
const std::shared_ptr<int> kIntPtr3Duplicate = std::make_shared<int>(3);

}  // namespace util

}  // namespace pql

}  // namespace test
