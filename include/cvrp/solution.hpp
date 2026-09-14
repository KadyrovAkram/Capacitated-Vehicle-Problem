#ifndef CVRP_SOLUTION_HPP
#define CVRP_SOLUTION_HPP

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

namespace cvrp {

class Instance;

struct Route {
    std::vector<int> stops; // Starts and ends with 0 (depot)
    int load = 0;
    double cost = 0.0;
};

struct Solution {
    std::string algorithm_name;
    std::vector<Route> routes;
    double total_cost = 0.0;
    double elapsed_seconds = 0.0;
    bool is_feasible = true;
    std::string validation_message = "Feasible";

    int num_routes() const { return static_cast<int>(routes.size()); }
    void print() const;
};

double calculate_route_cost(const std::vector<int>& route, const std::vector<std::vector<double>>& distances);
bool validate_solution(Solution& sol, const Instance& inst);

} // namespace cvrp

#endif // CVRP_SOLUTION_HPP
