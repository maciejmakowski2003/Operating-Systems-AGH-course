CC=gcc
BUILD_DIR=build
RAPORT_DIR=raport

cflags.default := -std=c17 -Wall
cflags.debug := -g -O0
cflags.release := -g -O2

CFLAGS := ${cflags.release} ${cflags.default}

.PHONY: all clean

all: integrate

integrate: integrate.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $< -lm

test:
	make integrate
	mkdir -p $(RAPORT_DIR)
	time -f '%e' ./$(BUILD_DIR)/integrate 0.0000000001 100 2>&1 | tee -a $(RAPORT_DIR)/raport.txt
	time -f '%e' ./$(BUILD_DIR)/integrate 0.0000000001 10 2>&1 | tee -a $(RAPORT_DIR)/raport.txt
	time -f '%e' ./$(BUILD_DIR)/integrate 0.000000001 100 2>&1 | tee -a $(RAPORT_DIR)/raport.txt
	time -f '%e' ./$(BUILD_DIR)/integrate 0.000000001 10 2>&1 | tee -a $(RAPORT_DIR)/raport.txt
	time -f '%e' ./$(BUILD_DIR)/integrate 0.5 1 2>&1 | tee -a $(RAPORT_DIR)/raport.txt
		

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(RAPORT_DIR)