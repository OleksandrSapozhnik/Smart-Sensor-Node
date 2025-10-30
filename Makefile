CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -std=c99 -g
LDFLAGS :=
TARGET := iot_client

SRC_DIR := src
BUILD_DIR := build

# Recursively find all .c files in src/ and subdirectories
SRC := $(shell find $(SRC_DIR) -name '*.c')
OBJ := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

# ====== DEFAULT RULE ======
all: $(BUILD_DIR)/$(TARGET)

# ====== BUILD ======
$(BUILD_DIR)/$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Build object files (with subdirectory support)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

# ====== RUN ======
run: all
	@./$(BUILD_DIR)/$(TARGET)

# ====== CLEAN ======
clean:
	rm -rf $(BUILD_DIR)
	@echo "Clean complete"

.PHONY: all run clean