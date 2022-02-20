#pragma once

namespace spa {

namespace pql {

template <typename T>
struct PtrComparator {
  bool operator()(const std::shared_ptr<T>& t1,
                  const std::shared_ptr<T>& t2) const {
    return (t1 == t2           // same pointer
            || (t1 && t2 &&    // check null of pointers
                *t1 == *t2));  // compare contents
  }
};

}  // namespace pql

}  // namespace spa
