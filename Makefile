CXX ?= clang++
CXXFLAGS ?= -std=c++23 -Wall -Werror -Wextra -O3
LDFLAGS ?= 

SRC_DIR ?= src
BUILD_DIR ?= build
MODULES = cli checker assembler csv_writer #aliases_and_concepts/ is header only
TARGET = csvls

SRCS := $(foreach m,$(MODULES),$(wildcard $(SRC_DIR)/$(m)/*.cpp))
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

.PHONY: all clean

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/main.cpp $^ $(LDFLAGS) -o $@

clean: 
	rm -rfv $(BUILD_DIR)/*