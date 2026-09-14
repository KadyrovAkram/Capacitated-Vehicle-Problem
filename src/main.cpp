#include "cvrp/instance.hpp"
#include "cvrp/solution.hpp"
#include "cvrp/greedy_split.hpp"
#include "cvrp/dp_split.hpp"
#include "cvrp/floyd_split.hpp"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <string>

void print_banner() {
    std::cout << "========================================================================\n";
    std::cout << "  CVRP Shortest Path Tour-Splitting Benchmark Suite\n";
    std::cout << "  Research in Combinatorial Optimization & Operations Research\n";
    std::cout << "========================================================================\n";
}

void print_benchmark_table_header() {
    std::cout << std::left 
              << std::setw(6)  << "Inst"
              << std::setw(6)  << "N"
              << std::setw(6)  << "Q"
              << std::setw(8)  << "Demand"
              << "|"
              << std::setw(10) << "Greedy(C)"
              << std::setw(14) << "Greedy(T,s)"
              << "|"
              << std::setw(10) << "DP(Cost)"
              << std::setw(14) << "DP(Time,s)"
              << "|"
              << std::setw(10) << "Floyd(C)"
              << std::setw(14) << "Floyd(T,s)"
              << "\n";
    std::cout << std::string(86, '-') << "\n";
}

void run_single_instance(int inst_id, bool verbose) {
    cvrp::Instance inst = cvrp::Instance::create_benchmark(inst_id);
    if (verbose) {
        inst.print_summary();
    }

    cvrp::Solution sol_greedy = cvrp::greedy_split(inst);
    cvrp::Solution sol_dp = cvrp::dp_split(inst);
    cvrp::Solution sol_floyd = cvrp::floyd_split(inst);

    if (verbose) {
        sol_greedy.print();
        sol_dp.print();
        sol_floyd.print();
    } else {
        std::cout << std::left 
                  << std::setw(6)  << inst_id
                  << std::setw(6)  << inst.num_customers
                  << std::setw(6)  << inst.vehicle_capacity
                  << std::setw(8)  << inst.get_total_demand()
                  << "|"
                  << std::setw(10) << std::fixed << std::setprecision(1) << sol_greedy.total_cost
                  << std::setw(14) << std::scientific << sol_greedy.elapsed_seconds
                  << "|"
                  << std::setw(10) << std::fixed << std::setprecision(1) << sol_dp.total_cost
                  << std::setw(14) << std::scientific << sol_dp.elapsed_seconds
                  << "|"
                  << std::setw(10) << std::fixed << std::setprecision(1) << sol_floyd.total_cost
                  << std::setw(14) << std::scientific << sol_floyd.elapsed_seconds
                  << "\n";
    }
}

void run_all_benchmarks(const std::string& export_csv = "") {
    print_banner();
    print_benchmark_table_header();

    std::ofstream csv;
    if (!export_csv.empty()) {
        csv.open(export_csv);
        csv << "Instance,N,Q,TotalDemand,GreedyCost,GreedyTime,DPCost,DPTime,FloydCost,FloydTime\n";
    }

    for (int id = 1; id <= 10; ++id) {
        cvrp::Instance inst = cvrp::Instance::create_benchmark(id);
        cvrp::Solution sol_greedy = cvrp::greedy_split(inst);
        cvrp::Solution sol_dp = cvrp::dp_split(inst);
        cvrp::Solution sol_floyd = cvrp::floyd_split(inst);

        std::cout << std::left 
                  << std::setw(6)  << id
                  << std::setw(6)  << inst.num_customers
                  << std::setw(6)  << inst.vehicle_capacity
                  << std::setw(8)  << inst.get_total_demand()
                  << "|"
                  << std::setw(10) << std::fixed << std::setprecision(1) << sol_greedy.total_cost
                  << std::setw(14) << std::scientific << sol_greedy.elapsed_seconds
                  << "|"
                  << std::setw(10) << std::fixed << std::setprecision(1) << sol_dp.total_cost
                  << std::setw(14) << std::scientific << sol_dp.elapsed_seconds
                  << "|"
                  << std::setw(10) << std::fixed << std::setprecision(1) << sol_floyd.total_cost
                  << std::setw(14) << std::scientific << sol_floyd.elapsed_seconds
                  << "\n";

        if (csv.is_open()) {
            csv << id << ","
                << inst.num_customers << ","
                << inst.vehicle_capacity << ","
                << inst.get_total_demand() << ","
                << sol_greedy.total_cost << "," << sol_greedy.elapsed_seconds << ","
                << sol_dp.total_cost << "," << sol_dp.elapsed_seconds << ","
                << sol_floyd.total_cost << "," << sol_floyd.elapsed_seconds << "\n";
        }
    }

    if (csv.is_open()) {
        std::cout << "\n[INFO] Results successfully saved to: " << export_csv << "\n";
    }
}

int main(int argc, char* argv[]) {
    bool verbose = false;
    int instance_id = -1;
    std::string export_csv = "";

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--verbose") == 0 || std::strcmp(argv[i], "-v") == 0) {
            verbose = true;
        } else if (std::strcmp(argv[i], "--instance") == 0 && i + 1 < argc) {
            instance_id = std::stoi(argv[++i]);
        } else if (std::strcmp(argv[i], "--export-csv") == 0 && i + 1 < argc) {
            export_csv = argv[++i];
        } else if (std::strcmp(argv[i], "--help") == 0 || std::strcmp(argv[i], "-h") == 0) {
            std::cout << "Usage: " << argv[0] << " [options]\n"
                      << "Options:\n"
                      << "  --all                Run all 10 benchmark instances (default)\n"
                      << "  --instance <1-10>    Run a specific benchmark instance\n"
                      << "  --verbose, -v        Print detailed route breakdowns\n"
                      << "  --export-csv <path>  Export benchmark results to CSV\n"
                      << "  --help, -h           Show this help message\n";
            return 0;
        }
    }

    if (instance_id != -1) {
        print_banner();
        run_single_instance(instance_id, verbose);
    } else {
        run_all_benchmarks(export_csv);
    }

    return 0;
}
