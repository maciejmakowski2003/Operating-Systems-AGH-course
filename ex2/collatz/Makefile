CC=gcc
BUILD_DIR=build

cflags.default := -std=c17 -Wall
cflags.debug := -g -O0

CFLAGS := ${cflags.debug} ${cflags.default}

.PHONY: all collatz_static collatz_shared clean

all: 
	make collatz_static
	make collatz_shared

collatz_static: collatz.c 
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/$@
	ar rcs $(BUILD_DIR)/lib$@.a $(BUILD_DIR)/$@


collatz_shared: collatz.c 
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -fPIC -c $< -o $(BUILD_DIR)/$@.o
	$(CC) -shared -o $(BUILD_DIR)/lib$@.so $(BUILD_DIR)/$@.o

clean:
	rm -rf $(BUILD_DIR)