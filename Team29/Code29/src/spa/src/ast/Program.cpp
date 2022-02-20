#include "Program.h"

#include <algorithm>
#include <iostream>
#include <set>
#include <unordered_map>

namespace spa {

namespace ast {

bool Program::operator==(const Program& other) const {
  if (proc_list_.size() != other.proc_list_.size()) {
    return false;
  }
  for (size_t i = 0; i < proc_list_.size(); i++) {
    if (!(*proc_list_[i] == *other.proc_list_[i])) {
      return false;
    }
  }
  return true;
}

const std::vector<std::shared_ptr<Procedure>> Program::GetProcList() const {
  return proc_list_;
}

}  // namespace ast

}  // namespace spa
