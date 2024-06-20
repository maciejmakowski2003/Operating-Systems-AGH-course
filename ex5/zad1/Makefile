CC=gcc
BUILD_DIR=build

cflags.default := -std=c17 -Wall
cflags.debug := -g -O0
cflags.release := -g -O2

CFLAGS := ${cflags.release} ${cflags.default}

.PHONY: all clean

all: sig_demo

sig_demo: sig_demo.c
	mkdir -p $(BUILD_DIR) 
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

test: sig_demo
	make sig_demo
	./$(BUILD_DIR)/sig_demo ignore
	./$(BUILD_DIR)/sig_demo handler
	./$(BUILD_DIR)/sig_demo mask	
	./$(BUILD_DIR)/sig_demo none
	
clean:
	rm -rf $(BUILD_DIR)