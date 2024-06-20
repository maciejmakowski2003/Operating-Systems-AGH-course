CC=gcc
BUILD_DIR=build
TEST_DIR=test

cflags.default := -std=c17 -Wall
cflags.debug := -g -O0
cflags.release := -g -O2

CFLAGS := ${cflags.release} ${cflags.default}

.PHONY: all clean

all: exec_process

exec_process: exec_process.c 
	mkdir -p $(BUILD_DIR) 
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

test:
	make exec_process
	mkdir -p $(TEST_DIR)
	touch $(TEST_DIR)/file

	./$(BUILD_DIR)/exec_process $(TEST_DIR)
	echo "Return value of the first call: $$?"

	./$(BUILD_DIR)/exec_process akkaka
	echo "Return value of the second call:  $$?"
	
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(TEST_DIR)