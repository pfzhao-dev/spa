#pragma once

#include <string>
#include <vector>

namespace spa {

namespace pql {

typedef uint64_t EstimationValue;

class Estimation {
 public:
  // Perform projection on the estimation value.
  static Estimation Project(Estimation base, std::vector<size_t> idx);

  // Default constructor for convenience to represent a query result table that
  // is expected to be empty.
  Estimation();

  // Constructor that takes in the estimated number of rows along with the
  // estimated number of distinct values per column in a query result table.
  Estimation(EstimationValue num_rows,
             std::vector<EstimationValue> col_unique_values);

  // Compute the cost to construct the current table.
  EstimationValue GetCost();

  // Estimated number of rows for a particular query result.
  EstimationValue num_rows;

  // Estimated number of unique values per column for a particular query result.
  std::vector<EstimationValue> col_unique_values;

  friend std::ostream& operator<<(std::ostream& out, const Estimation& ref);
};

}  // namespace pql

}  // namespace spa