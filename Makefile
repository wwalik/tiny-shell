INCLUDE_DIR := include
SRC_DIR := src
BUILD_DIR := build

CC := gcc
CFLAGS := --std=c99 -iquote$(INCLUDE_DIR)
DBGFLAGS := -ggdb

.PHONY: clean


all: tsh debug


# get src/*.c and out build/*.o
src_c_files := $(wildcard $(SRC_DIR)/*.c)
src_o_files := $(patsubst %.c,%.o,$(src_c_files))
build_o_files := $(subst $(SRC_DIR),$(BUILD_DIR),$(src_o_files))

tsh: $(build_o_files)
	$(CC) $(CFLAGS) $^ -o $@

debug: $(build_o_files)
	$(CC) $(CFLAGS) $^ -o tsh-$@

# Build .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm tsh* 
	rm $(BUILD_DIR)/$(wildcard *.o)
