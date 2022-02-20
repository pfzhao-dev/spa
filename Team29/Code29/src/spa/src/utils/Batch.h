#pragma once

#include <set>
#include <string>
#include <unordered_set>
#include <vector>

#include "Element.h"

namespace spa {

namespace utils {

class Batch {
 public:
  Batch(bool distinct = false);

  // Convenient constructor to construct a batch based on a vector of string.
  Batch(std::unordered_set<std::string>& values, bool distinct = false);

  // Convenient constructor to construct a batch based on a vector of uint32_t.
  Batch(std::unordered_set<std::uint32_t>& values, bool distinct = false);

  // Convenient function to add a tuple with 0 column.
  void Add();

  // Convenient function to add a tuple with 1 column.
  void Add(Element elem);

  void Add(std::vector<Element>&& tuple);

  size_t Size() const;

  std::vector<Element>& At(size_t idx);

  std::vector<Element> Get(size_t idx) const;

  friend std::ostream& operator<<(std::ostream& out, const Batch& ref);

 private:
  std::vector<std::vector<Element>> data_;

  // Boolean to indicate whether we need to add check for data uniqueness.
  bool distinct_;

  // Memo to guarantee uniqueness of data during insertion.
  // Using `set` instead of `unordered_set` so that we don't need to implement
  // our own hash function.
  std::set<std::vector<Element>> memo_;
};

}  // namespace utils

}  // namespace spa
