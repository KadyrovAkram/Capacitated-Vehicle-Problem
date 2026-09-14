#ifndef CVRP_INSTANCE_HPP
#define CVRP_INSTANCE_HPP

#include <string>
#include <vector>
#include <cmath>
#include <iostream>

namespace cvrp {

struct Customer {
    int id;
    double x;
    double y;
    int demand;
};

class Instance {
public:
    std::string name;
    int num_customers;     // N customers (excluding depot 0)
    int vehicle_capacity;  // Q
    Customer depot;
    std::vector<Customer> customers;                  // 1-indexed (index 0 is depot)
    std::vector<int> demands;                         // demands[0] = 0, demands[1..N]
    std::vector<std::vector<double>> distance_matrix; // (N+1) x (N+1)
    std::vector<int> sequence;                        // Tour permutation: [1, 2, ..., N]

    Instance() : num_customers(0), vehicle_capacity(0) {}

    // Factory methods
    static Instance create_benchmark(int instance_id);
    static Instance create_synthetic(int n, int capacity, int seed = 42);

    void compute_euclidean_distances();
    int get_total_demand() const;
    void print_summary() const;
};

} // namespace cvrp

#endif // CVRP_INSTANCE_HPP
