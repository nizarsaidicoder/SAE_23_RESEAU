CC = gcc

TARGET_EXEC ?= main

BUILD_DIR ?= ./build
TARGET_DIR ?= ./bin
SRC_DIRS ?= .

SRCS := $(shell find $(SRC_DIRS) -name "*.c")
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS ?= -Wall -Wextra -Iinclude -O3 -Wno-unused-parameter -Wno-unused-return
$(TARGET_DIR)/$(TARGET_EXEC): $(OBJS)
	$(MKDIR_P) $(dir $@)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR) $(TARGET_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
