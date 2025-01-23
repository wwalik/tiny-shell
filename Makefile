INCLUDE_DIR := include
SRC_DIR := src
BUILD_DIR := build

CC := gcc
CFLAGS := --std=gnu99 -iquote$(INCLUDE_DIR) -MMD
DBGFLAGS := -ggdb

.PHONY: clean all


all: tsh tsh-debug


# get src/*.c and out build/*.o
src_c_files := $(wildcard $(SRC_DIR)/*.c)
src_o_files := $(patsubst %.c,%.o,$(src_c_files))
build_o_files := $(subst $(SRC_DIR),$(BUILD_DIR),$(src_o_files))

# Build main executable
tsh: $(build_o_files)
	$(CC) $(CFLAGS) $^ -o $@

# Build a debuggable executable
tsh-debug: $(build_o_files)
	$(CC) $(CFLAGS) $(DBGFLAGS) $^ -o $@

# Build .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm tsh* 
	rm $(BUILD_DIR)/*.o
	rm $(BUILD_DIR)/*.d


-include $(BUILD_DIR)/*.d
