CXX := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude
BIN_DIR := bin
SRC_DIR := src
OBJ_DIR := obj
INCLUDE_DIR := include
TARGET := $(BIN_DIR)/main
TARGET2 := $(SRC_DIR)/main.cpp
SOURCE := $(SRC_DIR)/BMP.cpp
INCLUDE := $(INCLUDE_DIR)/BMP.hpp

all: $(TARGET)

$(TARGET): $(TARGET2) $(SOURCE) $(INCLUDE)
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(TARGET2) $(SOURCE)

clean:
	rm -f $(TARGET)
