#include "cvrp/dp_split.hpp"
#include <chrono>
#include <limits>
#include <algorithm>

namespace cvrp {

Solution dp_split(const Instance& inst) {
    auto start_time = std::chrono::high_resolution_clock::now();

    Solution sol;
    sol.algorithm_name = "Dynamic Programming";

    int n = inst.num_customers;
    const double INF = std::numeric_limits<double>::infinity();

    std::vector<double> dp(n + 1, INF);
    std::vector<int> previous(n + 1, -1);
    dp[0] = 0.0;

    for (int i = 1; i <= n; ++i) {
        int load = 0;
        for (int j = i; j >= 1; --j) {
            int cust = inst.sequence[j - 1];
            load += inst.demands[cust];

            if (load > inst.vehicle_capacity) {
                break;
            }

            std::vector<int> subroute = {0};
            for (int k = j - 1; k < i; ++k) {
                subroute.push_back(inst.sequence[k]);
            }
            subroute.push_back(0);

            double route_cost = calculate_route_cost(subroute, inst.distance_matrix);

            if (dp[j - 1] != INF && dp[j - 1] + route_cost < dp[i]) {
                dp[i] = dp[j - 1] + route_cost;
                previous[i] = j - 1;
            }
        }
    }

    int curr = n;
    while (curr > 0) {
        int prev = previous[curr];
        if (prev == -1) {
            sol.is_feasible = false;
            sol.validation_message = "Infeasible partition: cannot satisfy demands within vehicle capacity.";
            break;
        }

        Route r;
        r.stops.push_back(0);
        for (int k = prev; k < curr; ++k) {
            int c = inst.sequence[k];
            r.stops.push_back(c);
            r.load += inst.demands[c];
        }
        r.stops.push_back(0);
        r.cost = calculate_route_cost(r.stops, inst.distance_matrix);
        sol.routes.push_back(r);

        curr = prev;
    }

    std::reverse(sol.routes.begin(), sol.routes.end());

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    sol.elapsed_seconds = elapsed.count();

    validate_solution(sol, inst);
    return sol;
}

} // namespace cvrp
