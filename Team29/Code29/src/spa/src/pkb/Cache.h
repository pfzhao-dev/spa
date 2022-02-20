#pragma once

#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace spa {
namespace pkb {

template <typename T1, typename T2>
class Cache {
 public:
  Cache() : size_{0} {}

  void AddRelation(T1 lhs, T2 rhs) {
    cache_original_[lhs].insert(rhs);
    cache_original_keys_.insert(lhs);

    cache_reverse_[rhs].insert(lhs);
    cache_reverse_keys_.insert(rhs);

    size_++;
  }

  void Clear() {
    cache_original_.clear();
    cache_original_keys_.clear();

    cache_reverse_.clear();
    cache_reverse_keys_.clear();

    size_ = 0;
  }

  bool Empty() { return size_ == 0; }

  size_t Size() { return size_; }

  bool Count(T1 lhs, T2 rhs) {
    return cache_original_.count(lhs) && cache_original_[lhs].count(rhs);
  }
  bool CountOriginal(T1 elem) { return cache_original_.count(elem); }
  bool CountReverse(T2 elem) { return cache_reverse_.count(elem); }

  std::unordered_map<T1, std::unordered_set<T2>>& GetOriginalMap() {
    return cache_original_;
  }

  std::unordered_map<T2, std::unordered_set<T1>>& GetReverseMap() {
    return cache_reverse_;
  }

  std::unordered_set<T1>& GetOriginalKeys() { return cache_original_keys_; }
  std::unordered_set<T2>& GetReverseKeys() { return cache_reverse_keys_; }

  std::unordered_set<T2>& GetOriginalValues(T1 key) {
    return cache_original_[key];
  }
  std::unordered_set<T1>& GetReverseValues(T2 key) {
    return cache_reverse_[key];
  }

 private:
  std::unordered_map<T1, std::unordered_set<T2>> cache_original_;
  std::unordered_set<T1> cache_original_keys_;

  std::unordered_map<T2, std::unordered_set<T1>> cache_reverse_;
  std::unordered_set<T2> cache_reverse_keys_;

  size_t size_;
};

}  // namespace pkb
}  // namespace spa
