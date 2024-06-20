CC=gcc
BUILD_DIR=build

CFLAGS= -Wall

pipe: pipe.c
	mkdir -p $(BUILD_DIR) 
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

test: pipe
	./$(BUILD_DIR)/pipe
	
	
clean:
	rm -rf $(BUILD_DIR)