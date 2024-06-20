CC=gcc
BUILD_DIR=build

cflags.default := -std=c17 -Wall
cflags.debug := -g -O0
cflags.release := -g -O2

CFLAGS := ${cflags.debug} ${cflags.default}

.PHONY: all clean

all: printers users

printers: printers.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $< -lrt -lpthread

users: users.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $< -lrt -lpthread

run_printers: printers 
	./$(BUILD_DIR)/printers 3

run_users: users
	./$(BUILD_DIR)/users 6
	
clean:
	rm -rf $(BUILD_DIR)
	rm -rf /dev/mqueue/*