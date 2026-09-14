# Mathematical Formulation: Capacitated Vehicle Routing Problem & Tour-Splitting DAG

## 1. Standard Capacitated Vehicle Routing Problem (CVRP)

The Capacitated Vehicle Routing Problem is defined on a complete directed or undirected graph $G = (V, E)$:
- $V = \{0, 1, 2, \dots, N\}$: The vertex set, where vertex $0$ represents the central distribution depot, and $V_c = \{1, 2, \dots, N\}$ represents the set of geographically dispersed customers.
- $E = \{(i, j) : i, j \in V, i \ne j\}$: The set of traversable edges/arcs.
- $C = [c_{ij}]$: The non-negative travel distance or transit cost matrix satisfying the triangle inequality:
  $$c_{ik} + c_{kj} \ge c_{ij} \quad \forall i, j, k \in V$$
- $d_i$: Demands of customer $i \in V_c$, with $d_0 = 0$. We assume $0 < d_i \le Q$.
- $Q$: Uniform physical carrying capacity of each vehicle in the homogeneous fleet.
- $K$: Available fleet size (or minimized as a primary objective).

### Decision Variables
- $x_{ij} \in \{0, 1\}$: Binary variable indicating whether a vehicle travels directly from node $i$ to node $j$.
- $u_i \ge 0$: Continuous auxiliary load variable indicating cumulative cargo delivered or vehicle load after servicing node $i$ (used for MTZ subtour elimination).

### Mixed-Integer Linear Programming (MILP) Formulation
$$\min \sum_{i \in V} \sum_{j \in V, j \ne i} c_{ij} x_{ij}$$

Subject to:

1. **Degree / Assignment Constraints** (Every customer visited exactly once):
   $$\sum_{j \in V, j \ne i} x_{ij} = 1 \quad \forall i \in V_c$$
   $$\sum_{i \in V, i \ne j} x_{ij} = 1 \quad \forall j \in V_c$$

2. **Depot Flow Balance**:
   $$\sum_{j \in V_c} x_{0j} = \sum_{j \in V_c} x_{j0} \le K$$

3. **Capacity & Miller-Tucker-Zemlin (MTZ) Subtour Elimination Constraints**:
   $$u_i - u_j + Q x_{ij} \le Q - d_j \quad \forall i, j \in V_c, i \ne j$$
   $$d_i \le u_i \le Q \quad \forall i \in V_c$$

---

## 2. The Shortest Path Tour-Splitting Subproblem

In metaheuristic frameworks (such as Genetic Algorithms, Memetic Algorithms, and Iterated Local Search), candidate solutions are frequently encoded as a **giant tour**—a permutation $\mathcal{S} = \langle a_1, a_2, \dots, a_N \rangle$ of all customers without explicit depot delimiters.

The **Tour-Splitting Subproblem** determines the optimal sequence of depot insertions into $\mathcal{S}$ to partition the giant tour into feasible vehicle routes while strictly preserving the relative customer ordering.

### Auxiliary Directed Acyclic Graph (DAG) Construction
Let an auxiliary graph $H = (\mathcal{V}_H, \mathcal{A}_H)$ be constructed where:
- Vertices $\mathcal{V}_H = \{0, 1, 2, \dots, N\}$ represent customer prefix indices. Vertex $i$ denotes that the prefix $\langle a_1, \dots, a_i \rangle$ has been partitioned.
- A directed arc $(i, j) \in \mathcal{A}_H$ ($0 \le i < j \le N$) exists if and only if the sub-sequence of customers $\langle a_{i+1}, a_{i+2}, \dots, a_j \rangle$ satisfies the vehicle capacity constraint:
  $$\sum_{k=i+1}^j d_{a_k} \le Q$$
- The arc weight $W(i, j)$ represents the cost of dispatching a single vehicle on the route $0 \to a_{i+1} \to \dots \to a_j \to 0$:
  $$W(i, j) = c_{0, a_{i+1}} + \left(\sum_{k=i+1}^{j-1} c_{a_k, a_{k+1}}\right) + c_{a_j, 0}$$

### Bellman Shortest Path Recurrence (Prins' Split Algorithm)
Because $H$ is a topological Directed Acyclic Graph, the optimal splitting corresponds to the single-source shortest path from node $0$ to node $N$:

$$DP[0] = 0$$

$$DP[j] = \min_{0 \le i < j, \; \sum_{k=i+1}^j d_{a_k} \le Q} \left[ DP[i] + W(i, j) \right] \quad \forall j \in \{1, \dots, N\}$$

With predecessor tracking:

$$\pi(j) = \arg\min_{0 \le i < j, \; \sum_{k=i+1}^j d_{a_k} \le Q} \left[ DP[i] + W(i, j) \right]$$

### Theoretical Complexity
- **Time Complexity**: If $B = \max \{ m : \sum_{k=1}^m d_{(k)} \le Q \}$ denotes the maximum number of consecutive customers that fit in a vehicle, the inner loop explores at most $B$ arcs. Thus, execution time is bounded by:
  $$\mathcal{O}(N \cdot B) \subseteq \mathcal{O}(N^2)$$
  In practical logistics settings where $B \ll N$, this evaluates in **sub-millisecond (microsecond) time**.
- **Space Complexity**: $\mathcal{O}(N)$ memory for memoization arrays $DP[\cdot]$ and $\pi[\cdot]$.
