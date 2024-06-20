CC=gcc
BUILD_DIR=build
PIPE_DIR=tmp

cflags.default := -std=c17 -Wall
cflags.debug := -g -O0
cflags.release := -g -O2

CFLAGS := ${cflags.release} ${cflags.default}

.PHONY: all clean

all: manager integrate

manager: manager.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

integrate: integrate.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

test_manager:
	make manager 
	$(BUILD_DIR)/manager 0 1 0.000000001
	make clean

test_integrate:
	mkdir -p $(PIPE_DIR)
	make integrate 
	$(BUILD_DIR)/integrate

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(PIPE_DIR)