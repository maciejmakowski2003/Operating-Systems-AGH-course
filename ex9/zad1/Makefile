CC=gcc
BUILD_DIR=build

cflags.default := -std=c17 -Wall
cflags.debug := -g -O0
cflags.release := -g -O2

CFLAGS := ${cflags.release} ${cflags.default}

.PHONY: all clean

all: life
	
life: grid.c life.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) grid.c life.c -o $(BUILD_DIR)/life -lncursesw -lpthread -lm

test: life
	./$(BUILD_DIR)/life 15
	
clean:
	rm -rf $(BUILD_DIR)
