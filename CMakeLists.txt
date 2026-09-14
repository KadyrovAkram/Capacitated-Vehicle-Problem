cmake_minimum_required(VERSION 3.14)
project(CVRPSplitBenchmark VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optimization flags
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

include_directories(include)

file(GLOB SOURCES
    "src/instance.cpp"
    "src/solution.cpp"
    "src/greedy_split.cpp"
    "src/dp_split.cpp"
    "src/floyd_split.cpp"
    "src/main.cpp"
)

add_executable(cvrp_solver )
target_include_directories(cvrp_solver PUBLIC include)
