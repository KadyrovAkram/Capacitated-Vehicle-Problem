CXX ?= clang++
CXXFLAGS ?= -std=c++17 -O3 -Wall -Wextra -Iinclude
SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin
TARGET = $(BIN_DIR)/cvrp_solver

SRCS = $(SRC_DIR)/instance.cpp \
       $(SRC_DIR)/solution.cpp \
       $(SRC_DIR)/greedy_split.cpp \
       $(SRC_DIR)/dp_split.cpp \
       $(SRC_DIR)/floyd_split.cpp \
       $(SRC_DIR)/main.cpp

OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

.PHONY: all clean benchmark test

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

benchmark: $(TARGET)
	@./$(TARGET)

test: $(TARGET)
	@./$(TARGET) --instance 1 --verbose

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
