#ifndef CVRP_DP_SPLIT_HPP
#define CVRP_DP_SPLIT_HPP

#include "cvrp/instance.hpp"
#include "cvrp/solution.hpp"

namespace cvrp {

/**
 * Dynamic Programming Tour-Splitting Algorithm (Prins' Split):
 * Formulates the sequence partition as finding the shortest path on a Directed
 * Acyclic Graph (DAG) of customer prefixes.
 *
 * Recurrence:
 *   DP[0] = 0
 *   DP[i] = min_{1 <= j <= i, sum_{k=j}^i d_k <= Q} (DP[j - 1] + cost(0 -> seq[j]...seq[i] -> 0))
 *
 * Guarantees the globally optimal partition of the given sequence.
 * Time Complexity: O(N * B) where B is the max customers per vehicle (worst-case O(N^2))
 * Space Complexity: O(N)
 */
Solution dp_split(const Instance& inst);

} // namespace cvrp

#endif // CVRP_DP_SPLIT_HPP
