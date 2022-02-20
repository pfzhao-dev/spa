#include "Batch.h"

#include <iostream>
#include <variant>

namespace spa {

namespace utils {

Batch::Batch(bool distinct) : distinct_(distinct), data_{}, memo_{} {}

Batch::Batch(std::unordered_set<std::string>& values, bool distinct)
    : data_{}, memo_{} {
  for (const auto& v : values) {
    std::vector<utils::Element> tuple;
    tuple.push_back(v);
    if (distinct_ && memo_.count(tuple)) {
      continue;
    }

    if (distinct_) memo_.insert(tuple);
    data_.push_back(std::move(tuple));
  }
}

Batch::Batch(std::unordered_set<uint32_t>& values, bool distinct)
    : data_{}, memo_{} {
  for (const auto& v : values) {
    std::vector<utils::Element> tuple;
    tuple.push_back(v);
    if (distinct_ && memo_.count(tuple)) {
      continue;
    }

    if (distinct_) memo_.insert(tuple);
    data_.push_back(std::move(tuple));
  }
}

void Batch::Add() {
  std::vector<Element> tuple;
  Add(std::move(tuple));
}

void Batch::Add(Element elem) {
  std::vector<Element> tuple{elem};
  Add(std::move(tuple));
}

void Batch::Add(std::vector<Element>&& tuple) {
  if (distinct_ && memo_.count(tuple)) {
    // Ensure that the stored tuples are all unique.
    return;
  }

  if (distinct_) memo_.insert(tuple);
  data_.push_back(std::move(tuple));
}

size_t Batch::Size() const { return data_.size(); }

std::vector<Element>& Batch::At(size_t idx) { return data_.at(idx); }
std::vector<Element> Batch::Get(size_t idx) const { return data_.at(idx); }

std::ostream& operator<<(std::ostream& out, const Batch& ref) {
  out << "[Batch:\n";
  for (size_t i = 0; i < ref.Size(); i++) {
    auto tuple = ref.Get(i);
    out << " - (";
    bool first = true;
    for (auto elem : tuple) {
      if (first)
        first = false;
      else
        out << ", ";
      if (uint32_t* v = std::get_if<uint32_t>(&elem)) {
        out << std::to_string(*v);
      } else {
        out << std::get<std::string>(elem);
      }
    }
    out << ")\n";
  }
  out << "]";
  return out;
}

}  // namespace utils

}  // namespace spa
