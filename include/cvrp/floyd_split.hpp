#ifndef CVRP_FLOYD_SPLIT_HPP
#define CVRP_FLOYD_SPLIT_HPP

#include "cvrp/instance.hpp"
#include "cvrp/solution.hpp"

namespace cvrp {

/**
 * Floyd-Warshall / DAG Shortest Path Tour-Splitting:
 * Constructs an explicit auxiliary graph where nodes are split boundaries (0 to N).
 * Computes all-pairs shortest paths or topological shortest path to evaluate
 * route decisions and trade-offs.
 *
 * Also provides the lookahead routing heuristic drafted in initial exploratory work.
 * Time Complexity: O(V^3) where V = N + 1
 * Space Complexity: O(V^2)
 */
Solution floyd_split(const Instance& inst);

} // namespace cvrp

#endif // CVRP_FLOYD_SPLIT_HPP
