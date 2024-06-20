CC=gcc
CFLAGS=-I. -Wall -pthread 
DEPS = buffer.h
BUILD_DIR=build

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o  
	mkdir -p $(BUILD_DIR) 
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/main  main.c

test: main
	./$(BUILD_DIR)/main

clean:
	rm -rf $(BUILD_DIR) *.o