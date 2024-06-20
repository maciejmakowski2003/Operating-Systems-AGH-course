GCC=gcc
BUILD_DIR=build
TEST_DIR=test
FUNCTION_OUTPUT=f_output.txt 
OUTPUT=output.txt


cflags.default := -std=c17 -Wall
cflags.debug := -g -O0
cflags.release := -g -O2

CFLAGS := ${cflags.release} ${cflags.default}

.PHONY: all 

all: count_size

count_size: count_size.c
	mkdir -p $(BUILD_DIR)
	$(GCC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

test:
	make count_size 
	mkdir -p $(TEST_DIR)
	./$(BUILD_DIR)/count_size 1>> ./$(TEST_DIR)/$(FUNCTION_OUTPUT)
	./count_test.sh count_size.c Makefile > ./$(TEST_DIR)/$(OUTPUT)
	diff -s ./$(TEST_DIR)/$(FUNCTION_OUTPUT) ./$(TEST_DIR)/$(OUTPUT)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(TEST_DIR)
	