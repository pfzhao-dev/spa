#pragma once

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Node.h"
#include "Procedure.h"

typedef short PROC;

namespace spa {

namespace ast {

class Program : public Node {
 public:
  Program(std::vector<std::shared_ptr<Procedure>> proc_list)
      : proc_list_(proc_list) {}

  const std::vector<std::shared_ptr<Procedure>> GetProcList() const;

  bool operator==(const Program &other_program) const;

 private:
  std::vector<std::shared_ptr<Procedure>> proc_list_;
};

}  // namespace ast

}  // namespace spa
