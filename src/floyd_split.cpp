#include "cvrp/floyd_split.hpp"
#include <chrono>
#include <limits>
#include <algorithm>

namespace cvrp {

Solution floyd_split(const Instance& inst) {
    auto start_time = std::chrono::high_resolution_clock::now();

    Solution sol;
    sol.algorithm_name = "Floyd-Warshall DAG";

    int V = inst.num_customers + 1;
    const double INF = 1e9;

    std::vector<std::vector<double>> adj(V, std::vector<double>(V, INF));
    std::vector<std::vector<int>> next_node(V, std::vector<int>(V, -1));

    for (int i = 0; i < V; ++i) {
        adj[i][i] = 0.0;
    }

    for (int u = 0; u < inst.num_customers; ++u) {
        int load = 0;
        for (int v = u + 1; v <= inst.num_customers; ++v) {
            int cust = inst.sequence[v - 1];
            load += inst.demands[cust];
            if (load > inst.vehicle_capacity) break;

            std::vector<int> subroute = {0};
            for (int k = u; k < v; ++k) {
                subroute.push_back(inst.sequence[k]);
            }
            subroute.push_back(0);

            adj[u][v] = calculate_route_cost(subroute, inst.distance_matrix);
            next_node[u][v] = v;
        }
    }

    std::vector<std::vector<double>> dist = adj;
    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            if (dist[i][k] >= INF) continue;
            for (int j = 0; j < V; ++j) {
                if (dist[k][j] >= INF) continue;
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next_node[i][j] = next_node[i][k];
                }
            }
        }
    }

    int curr = 0;
    int target = inst.num_customers;

    if (dist[curr][target] >= INF) {
        sol.is_feasible = false;
        sol.validation_message = "No feasible route combination found by Floyd-Warshall.";
    } else {
        while (curr != target) {
            int nxt = next_node[curr][target];
            if (nxt == -1) break;

            Route r;
            r.stops.push_back(0);
            for (int k = curr; k < nxt; ++k) {
                int c = inst.sequence[k];
                r.stops.push_back(c);
                r.load += inst.demands[c];
            }
            r.stops.push_back(0);
            r.cost = calculate_route_cost(r.stops, inst.distance_matrix);
            sol.routes.push_back(r);

            curr = nxt;
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    sol.elapsed_seconds = elapsed.count();

    validate_solution(sol, inst);
    return sol;
}

} // namespace cvrp
