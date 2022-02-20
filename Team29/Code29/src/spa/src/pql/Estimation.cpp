#include "Estimation.h"

#include <algorithm>
#include <iostream>
#include <string>

namespace spa {

namespace pql {

Estimation::Estimation() : num_rows{0}, col_unique_values{} {}

Estimation::Estimation(EstimationValue num_rows,
                       std::vector<EstimationValue> col_unique_values) {
  this->num_rows = num_rows;
  this->col_unique_values = col_unique_values;
}

Estimation Estimation::Project(Estimation base, std::vector<size_t> base_idx) {
  // If the base table is empty, the projected table must also be empty.
  EstimationValue new_num_rows = std::min(EstimationValue(1), base.num_rows);
  std::vector<EstimationValue> new_col_unique_values;
  new_col_unique_values.reserve(base_idx.size());

  for (const auto idx : base_idx) {
    if (new_num_rows < base.num_rows) {
      new_num_rows *= base.col_unique_values[idx];
      new_col_unique_values.push_back(base.col_unique_values[idx]);

      // The projected table is guaranteed to have at most the same number
      // of rows with its base.
      new_num_rows = std::min(new_num_rows, base.num_rows);
    }
  }

  return Estimation(new_num_rows, new_col_unique_values);
}

EstimationValue Estimation::GetCost() {
  // Cost to construct a table is equal to num rows multiplied by num cols.
  return num_rows * col_unique_values.size();
}

std::ostream& operator<<(std::ostream& out, const Estimation& ref) {
  out << "[ Estimation : " << std::to_string(ref.num_rows);
  out << ", { ";
  for (auto& v : ref.col_unique_values) {
    out << std::to_string(v);
    out << ", ";
  }
  out << "} ]";
  return out;
}

}  // namespace pql

}  // namespace spa
