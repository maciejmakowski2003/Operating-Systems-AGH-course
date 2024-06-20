CC=gcc
BUILD_DIR=build

cflags.default := -std=c17 -Wall
cflags.debug := -g -O0
cflags.release := -g -O2

CFLAGS := ${cflags.release} ${cflags.default}

.PHONY: all clean

all: catcher sender

catcher: catcher.c
	mkdir -p $(BUILD_DIR) 
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

sender: sender.c
	mkdir -p $(BUILD_DIR) 
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

test_catcher:
	make catcher 
	./$(BUILD_DIR)/catcher
	
clean:
	rm -rf $(BUILD_DIR)