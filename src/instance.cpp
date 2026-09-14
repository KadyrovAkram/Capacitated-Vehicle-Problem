#include "cvrp/instance.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <random>

namespace cvrp {

void Instance::compute_euclidean_distances() {
    int total_nodes = num_customers + 1;
    distance_matrix.assign(total_nodes, std::vector<double>(total_nodes, 0.0));

    std::vector<Customer> all_nodes;
    all_nodes.push_back(depot);
    for (int i = 1; i <= num_customers; ++i) {
        all_nodes.push_back(customers[i]);
    }

    for (int i = 0; i < total_nodes; ++i) {
        for (int j = 0; j < total_nodes; ++j) {
            double dx = all_nodes[i].x - all_nodes[j].x;
            double dy = all_nodes[i].y - all_nodes[j].y;
            distance_matrix[i][j] = std::round(std::sqrt(dx * dx + dy * dy));
        }
    }
}

int Instance::get_total_demand() const {
    int total = 0;
    for (int i = 1; i <= num_customers; ++i) {
        total += demands[i];
    }
    return total;
}

void Instance::print_summary() const {
    std::cout << "==================================================\n";
    std::cout << " Instance: " << name << "\n";
    std::cout << " Customers: " << num_customers << " | Capacity (Q): " << vehicle_capacity << "\n";
    std::cout << " Total Demand: " << get_total_demand() << "\n";
    std::cout << " Depot Location: (" << depot.x << ", " << depot.y << ")\n";
    std::cout << "==================================================\n";
}

Instance Instance::create_synthetic(int n, int capacity, int seed) {
    Instance inst;
    inst.name = "Synthetic_N" + std::to_string(n) + "_Q" + std::to_string(capacity);
    inst.num_customers = n;
    inst.vehicle_capacity = capacity;

    inst.depot = {0, 50.0, 50.0, 0};

    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist_coords(0.0, 100.0);
    int max_dem = std::max(1, std::min(capacity / 2, 10));
    std::uniform_int_distribution<int> dist_demand(1, std::max(2, max_dem));

    inst.customers.resize(n + 1);
    inst.demands.resize(n + 1, 0);
    inst.sequence.resize(n);

    inst.customers[0] = inst.depot;

    for (int i = 1; i <= n; ++i) {
        inst.customers[i] = {i, dist_coords(rng), dist_coords(rng), dist_demand(rng)};
        inst.demands[i] = inst.customers[i].demand;
        inst.sequence[i - 1] = i;
    }

    inst.compute_euclidean_distances();
    return inst;
}

Instance Instance::create_benchmark(int instance_id) {
    struct BenchmarkConfig {
        int n;
        int q;
        int target_demand;
        int seed;
    };

    std::vector<BenchmarkConfig> configs = {
        {10, 10, 200, 101},  // 1
        {10, 20, 200, 102},  // 2
        {10, 30, 200, 103},  // 3
        {10, 40, 200, 104},  // 4
        {20, 10, 400, 105},  // 5
        {20, 20, 400, 106},  // 6
        {20, 30, 400, 107},  // 7
        {50, 10, 1000, 108}, // 8
        {50, 40, 1000, 109}, // 9
        {100, 20, 2000, 110} // 10
    };

    if (instance_id < 1 || instance_id > 10) {
        throw std::invalid_argument("Instance ID must be between 1 and 10.");
    }

    const auto& cfg = configs[instance_id - 1];

    Instance inst;
    inst.name = "Instance_" + (instance_id < 10 ? std::string("0") : "") + std::to_string(instance_id);
    inst.num_customers = cfg.n;
    inst.vehicle_capacity = cfg.q;
    inst.depot = {0, 50.0, 50.0, 0};

    std::mt19937 rng(cfg.seed);
    std::uniform_real_distribution<double> dist_coords(5.0, 95.0);

    // Customer demands (1 to min(8, Q)) so that individual demand <= Q
    // This matches the demand pattern in the original research files: {1, 2, 3, 4, 5, 6, ...}
    int max_d = std::max(1, std::min(8, cfg.q - 1));
    std::uniform_int_distribution<int> dist_demand(1, max_d);

    inst.customers.resize(cfg.n + 1);
    inst.demands.resize(cfg.n + 1, 0);
    inst.sequence.resize(cfg.n);

    inst.customers[0] = inst.depot;

    for (int i = 1; i <= cfg.n; ++i) {
        int d = dist_demand(rng);
        inst.customers[i] = {i, dist_coords(rng), dist_coords(rng), d};
        inst.demands[i] = d;
        inst.sequence[i - 1] = i;
    }

    inst.compute_euclidean_distances();
    return inst;
}

} // namespace cvrp
