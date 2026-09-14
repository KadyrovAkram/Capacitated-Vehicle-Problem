#include "cvrp/greedy_split.hpp"
#include <chrono>

namespace cvrp {

Solution greedy_split(const Instance& inst) {
    auto start_time = std::chrono::high_resolution_clock::now();

    Solution sol;
    sol.algorithm_name = "Greedy";

    std::vector<int> current_stops = {0};
    int current_load = 0;

    for (int customer : inst.sequence) {
        int demand = inst.demands[customer];

        if (current_load + demand > inst.vehicle_capacity && current_stops.size() > 1) {
            current_stops.push_back(0);
            Route r;
            r.stops = current_stops;
            r.load = current_load;
            r.cost = calculate_route_cost(r.stops, inst.distance_matrix);
            sol.routes.push_back(r);

            current_stops = {0, customer};
            current_load = demand;
        } else {
            current_stops.push_back(customer);
            current_load += demand;
        }
    }

    if (current_stops.size() > 1) {
        current_stops.push_back(0);
        Route r;
        r.stops = current_stops;
        r.load = current_load;
        r.cost = calculate_route_cost(r.stops, inst.distance_matrix);
        sol.routes.push_back(r);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    sol.elapsed_seconds = elapsed.count();

    validate_solution(sol, inst);
    return sol;
}

} // namespace cvrp
