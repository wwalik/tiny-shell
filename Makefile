SHELL := /bin/bash

INCLUDE_DIR := include
SRC_DIR := src
BUILD_DIR := build
INSTALL_DIR := /bin

CC := gcc
CFLAGS := --std=gnu99 -iquote$(INCLUDE_DIR) -MMD
DBGFLAGS := -g

.PHONY: all clean install remove 

all: tsh 

clean:
	rm tsh* 
	rm $(BUILD_DIR)/*.o
	rm $(BUILD_DIR)/*.d

install: tsh
	cp /etc/shells scripts/shells.old
	export TSH_INSTALL_DIR=${INSTALL_DIR}
	echo "${TSH_INSTALL_DIR}"
	sudo -E ./scripts/install.sh
	unset TSH_INSTALL_DIR

remove:
	cp /etc/shells scripts/shells.old
	sudo -E ./scripts/remove.sh


# transform all src/*.c paths into build/*.o paths
src_c_files := $(wildcard $(SRC_DIR)/*.c)
src_o_files := $(patsubst %.c,%.o,$(src_c_files))
build_o_files := $(subst $(SRC_DIR),$(BUILD_DIR),$(src_o_files))

# Build main executable
tsh: $(build_o_files)
	$(CC) $(CFLAGS) $^ -o $@

# Build .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) ${DBGFLAGS} -c $< -o $@


# include dependencies magic
-include $(BUILD_DIR)/*.d
