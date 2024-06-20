CC=gcc
BUILD_DIR=build

cflags.default := -std=c17 -Wall
cflags.debug := -g -O0
cflags.release := -g -O2

CFLAGS := ${cflags.release} ${cflags.default}

.PHONY: all clean

all: server client

server: server.c
	mkdir -p ./$(BUILD_DIR)
	$(CC) $(CFLAGS) server.c -o ./$(BUILD_DIR)/server -lpthread

client: client.c
	mkdir -p ./$(BUILD_DIR)
	$(CC) $(CFLAGS) client.c -o ./$(BUILD_DIR)/client -lpthread
	
run_server: server
	./$(BUILD_DIR)/server 33333

run_client: client
	./$(BUILD_DIR)/client client3 127.0.0.1 33333
	
clean:
	rm -rf $(BUILD_DIR)
