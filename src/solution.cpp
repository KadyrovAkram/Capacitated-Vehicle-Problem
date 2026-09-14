#include "cvrp/solution.hpp"
#include "cvrp/instance.hpp"
#include <unordered_set>

namespace cvrp {

double calculate_route_cost(const std::vector<int>& route, const std::vector<std::vector<double>>& distances) {
    if (route.size() < 2) return 0.0;
    double cost = 0.0;
    for (size_t i = 0; i < route.size() - 1; ++i) {
        cost += distances[route[i]][route[i + 1]];
    }
    return cost;
}

bool validate_solution(Solution& sol, const Instance& inst) {
    std::unordered_set<int> visited;
    double recalculated_cost = 0.0;

    for (size_t r_idx = 0; r_idx < sol.routes.size(); ++r_idx) {
        const auto& r = sol.routes[r_idx];
        if (r.stops.empty() || r.stops.front() != 0 || r.stops.back() != 0) {
            sol.is_feasible = false;
            sol.validation_message = "Route " + std::to_string(r_idx + 1) + " does not start and end at depot 0.";
            return false;
        }

        int route_load = 0;
        for (size_t i = 1; i < r.stops.size() - 1; ++i) {
            int customer = r.stops[i];
            if (customer < 1 || customer > inst.num_customers) {
                sol.is_feasible = false;
                sol.validation_message = "Invalid customer ID " + std::to_string(customer);
                return false;
            }
            if (visited.count(customer)) {
                sol.is_feasible = false;
                sol.validation_message = "Customer " + std::to_string(customer) + " visited multiple times.";
                return false;
            }
            visited.insert(customer);
            route_load += inst.demands[customer];
        }

        if (route_load > inst.vehicle_capacity) {
            sol.is_feasible = false;
            sol.validation_message = "Route " + std::to_string(r_idx + 1) + " exceeds capacity (" +
                                     std::to_string(route_load) + " > " + std::to_string(inst.vehicle_capacity) + ").";
            return false;
        }

        recalculated_cost += calculate_route_cost(r.stops, inst.distance_matrix);
    }

    if (visited.size() != static_cast<size_t>(inst.num_customers)) {
        sol.is_feasible = false;
        sol.validation_message = "Not all customers visited (" + std::to_string(visited.size()) + " / " +
                                 std::to_string(inst.num_customers) + ").";
        return false;
    }

    sol.total_cost = recalculated_cost;
    sol.is_feasible = true;
    sol.validation_message = "Feasible";
    return true;
}

void Solution::print() const {
    std::cout << "--------------------------------------------------\n";
    std::cout << " Algorithm: " << algorithm_name << "\n";
    std::cout << " Feasible: " << (is_feasible ? "YES" : "NO (" + validation_message + ")") << "\n";
    std::cout << " Total Distance (Cost): " << std::fixed << std::setprecision(2) << total_cost << "\n";
    std::cout << " Number of Routes: " << num_routes() << "\n";
    std::cout << " Execution Time: " << std::scientific << elapsed_seconds << " s (" 
              << std::fixed << std::setprecision(4) << (elapsed_seconds * 1000.0) << " ms)\n";
    std::cout << " Routes:\n";
    for (size_t i = 0; i < routes.size(); ++i) {
        std::cout << "   Vehicle " << (i + 1) << " (Load: " << routes[i].load << ", Cost: "
                  << routes[i].cost << "): ";
        for (size_t j = 0; j < routes[i].stops.size(); ++j) {
            std::cout << routes[i].stops[j] << (j + 1 < routes[i].stops.size() ? " -> " : "\n");
        }
    }
    std::cout << "--------------------------------------------------\n";
}

} // namespace cvrp
