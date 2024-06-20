GCC=gcc
BUILD_DIR=build

cflags.default := -std=c17 -Wall
cflags.debug := -g -O0
cflags.release := -g -O2

CFLAGS := ${cflags.release} ${cflags.default}

.PHONY: all clean

all: process_creator

process_creator: process_creator.c 
	mkdir -p $(BUILD_DIR) 
	$(GCC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

test:
	make process_creator
	./$(BUILD_DIR)/process_creator 5
	
clean:
	rm -rf $(BUILD_DIR)
	
	