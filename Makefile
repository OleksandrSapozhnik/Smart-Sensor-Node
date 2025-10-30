CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -std=c99 -g
LDFLAGS :=
TARGET := iot_client

SRC_DIR := src
BUILD_DIR := build

# Recursively find all .c files in src/ and subdirectories
SRC := $(shell find $(SRC_DIR) -name '*.c')
OBJ := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

# Automatically find all directories containing .h files
INCLUDE_DIRS := $(shell find $(SRC_DIR) -type d)
INCLUDE_FLAGS := $(addprefix -I,$(INCLUDE_DIRS))

# DEFAULT RULE 
all: $(BUILD_DIR)/$(TARGET)

# BUILD 
$(BUILD_DIR)/$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Build object files (with automatic include paths)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

# RUN 
run: all
	@./$(BUILD_DIR)/$(TARGET)

# CLEAN 
clean:
	rm -rf $(BUILD_DIR)
	@echo "Clean complete"

# DEBUG INFO 
info:
	@echo "Source files:"
	@echo "$(SRC)"
	@echo ""
	@echo "Include directories:"
	@echo "$(INCLUDE_DIRS)"
	@echo ""
	@echo "Include flags:"
	@echo "$(INCLUDE_FLAGS)"

.PHONY: all run clean info