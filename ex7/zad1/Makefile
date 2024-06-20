CC=gcc
BUILD_DIR=build

cflags.default := -std=c17 -Wall
cflags.debug := -g -O0
cflags.release := -g -O2

CFLAGS := ${cflags.debug} ${cflags.default}

.PHONY: all clean

all:

server: server.c
	make clean
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

client: client.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

test_server: server 
	./$(BUILD_DIR)/server 

test_client: client 
	./$(BUILD_DIR)/client 

clean:
	rm -rf $(BUILD_DIR)
	rm -f /dev/mqueue/*