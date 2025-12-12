
TARGET_LIB := inject.so
CC       := gcc

CFLAGS   := -Wall -Wextra -O2 -fPIC -m32 

CPPFLAGS := -Iinclude
LDFLAGS  := -shared -m32
LDLIBS   := -ldl -lGL -lX11

SRC_DIR   := src
BUILD_DIR := build
OBJ_DIR   := $(BUILD_DIR)/obj

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

.PHONY: all
all: $(BUILD_DIR)/$(TARGET_LIB)

$(BUILD_DIR)/$(TARGET_LIB): $(OBJS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
