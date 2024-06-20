CC=gcc
BUILD_DIR=build

CFLAGS= -Wall

shm_write: shm_write.c
	mkdir -p $(BUILD_DIR) 
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

shm_read: shm_read.c
	mkdir -p $(BUILD_DIR) 
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

test: shm_write shm_read
	./$(BUILD_DIR)/shm_write
	./$(BUILD_DIR)/shm_read
	
	
clean:
	rm -rf $(BUILD_DIR)