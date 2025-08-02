CXX ?= clang++
CXXFLAGS ?= -std=c++23 -Wall -Werror -Wextra -O3
LDFLAGS ?= 

SRC_DIR ?= src
BUILD_DIR ?= build
MODULES = cli checker assembler csv_writer #aliases_and_concepts/ is header only
TARGET = csvls

SRCS := $(SRC_DIR)/main.cpp $(foreach m,$(MODULES),$(wildcard $(SRC_DIR)/$(m)/*.cpp))
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

prefix ?= /usr
bin_dir ?= $(prefix)/bin
DEST_DIR ?= 

.PHONY: all clean install
.DEFAULT_GOAL := all

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

install: all
	@echo "Installing to $(DEST_DIR)$(bin_dir)..."
	mkdir -p $(DEST_DIR)$(bin_dir)
	install -m 0755 $(BUILD_DIR)/$(TARGET) $(DEST_DIR)$(bin_dir)

clean: 
	rm -rfv $(BUILD_DIR)/*