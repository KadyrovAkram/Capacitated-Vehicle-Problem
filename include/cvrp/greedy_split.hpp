#ifndef CVRP_GREEDY_SPLIT_HPP
#define CVRP_GREEDY_SPLIT_HPP

#include "cvrp/instance.hpp"
#include "cvrp/solution.hpp"

namespace cvrp {

/**
 * Greedy Tour-Splitting Algorithm:
 * Iteratively appends customers from the predefined sequence into the current
 * vehicle route until the vehicle capacity Q is exceeded. Once exceeded, the
 * route returns to the depot (0) and a new vehicle route is initialized.
 *
 * Time Complexity: O(N)
 * Space Complexity: O(N)
 */
Solution greedy_split(const Instance& inst);

} // namespace cvrp

#endif // CVRP_GREEDY_SPLIT_HPP
