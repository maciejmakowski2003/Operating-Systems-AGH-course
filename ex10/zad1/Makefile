CC=gcc
BUILD_DIR=build

cflags.default := -std=c17 -Wall
cflags.debug := -g -O0
cflags.release := -g -O2

CFLAGS := ${cflags.release} ${cflags.default}

.PHONY: all clean

all: santa_claus_problem
	
santa_claus_problem: santa_claus_problem.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/santa_claus_problem -lpthread -lm

test: santa_claus_problem
	./$(BUILD_DIR)/santa_claus_problem
	
clean:
	rm -rf $(BUILD_DIR)
