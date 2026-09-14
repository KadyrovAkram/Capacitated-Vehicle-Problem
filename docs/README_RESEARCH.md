# Research Overview & Literature Context

This repository is based on academic research conducted by **Ekremzhan Kadyrov** investigating shortest path formulations, tour-splitting dynamics, and metaheuristic solvers for the Capacitated Vehicle Routing Problem (CVRP).

## Associated Research Works

1. **"Optimizing Shortest Paths in the Capacitated Vehicle Routing Problem"**
   - Focus: Investigates the Shortest Path Subproblem (Tour Splitting). Evaluates how greedy search, dynamic programming, and Floyd-Warshall DAG shortest paths perform in terms of route cost and execution time when splitting customer sequences.
   - Core finding: Dynamic Programming (Prins' Split) consistently yields the lowest total distance across small and large problem instances ($N=10$ to $N=100$) while maintaining microsecond computational performance.

2. **"Genetic Algorithms for the Capacitated Vehicle Routing Problem: A 10-Year Survey (2014–2023)"**
   - Focus: Systematic review of 235 publications across Scopus, synthesizing chromosome encodings, crossover operators (PMX, OX, SCX, BCRC, GTBCX), mutation techniques (Swap, 2-opt, Swap*), and hybrid methods.
   - Core finding: Permutation-based representations (giant tours) paired with optimal split procedures (the algorithms implemented in this repository) form the backbone of modern state-of-the-art CVRP metaheuristics (e.g., Vidal's HGS-CVRP).

## Methodological Summary of Literature Survey

| Author(s) | Variant | Representation | Crossover | Local Search / Enhancements |
|---|---|---|---|---|
| **Ahmed et al. (2023)** | Classical | Dummy Depots | PMX, OX, AEX, SCX | Distance-based crossover operators |
| **Arshad et al. (2023)** | Classical | Standard | BRBAX | Self-tuning adaptive mechanisms |
| **Vidal (2022)** | Classical | Giant Tour | OX | HGS-CVRP, SWAP* neighborhood search |
| **Sajid et al. (2021)** | Bi-objective | Standard | GTBCX | Two-opt*, NSGA-II integration |
| **Lin et al. (2019)** | IoT CVRP | Dummy Depots | BCRC | Sweep algorithm initialization |
| **Pop et al. (2018)** | Clustered VRP| Standard | OX | Two-level decomposition with Concorde TSP |
| **Prins (2004)** | Classical | Giant Tour | OX | Pioneered the Split DP algorithm |

## How Tour Splitting Powers Modern Metaheuristics
Instead of requiring genetic algorithm operators to navigate complex multi-vehicle constraint boundaries directly (which frequently yields infeasible offspring), modern algorithms evolve an unconstrained sequence of customers ($\langle a_1, \dots, a_N \rangle$) and invoke an optimal split algorithm as the fitness evaluation decoder.
