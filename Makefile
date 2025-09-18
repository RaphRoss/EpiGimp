TARGET = MiniGIMP

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./src -I./src/core -I./src/tools

SRC_DIR = src
BUILD_DIR = build

SRCS = $(shell find $(SRC_DIR) -name "*.cpp")
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

LIBS = -lsfml-graphics -lsfml-window -lsfml-system

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
