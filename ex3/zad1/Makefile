GCC=gcc
BUILD_DIR=build
RAPORT_DIR=report
FILES_DIR=files
INPUT_FILE=input.txt
OUTPUT_FILE=output.txt
TMP_FILE=tmp.txt
TIME_LOG=pomiar_zad_2.txt
TIME=/usr/bin/time -f
TIME_FORMAT="%e seconds elapsed, %U user, %S system"


cflags.default := -std=c17 -Wall
cflags.debug := -g -O0
cflags.release := -g -O2

CFLAGS := ${cflags.release} ${cflags.default}

.PHONY: all reverse_byte reverse

all: reverse_byte reverse

reverse_byte: reverse.c 
	mkdir -p $(BUILD_DIR)
	$(GCC) $(CFLAGS) $< -o $(BUILD_DIR)/$@ -D BYTE

reverse: reverse.c
	mkdir -p $(BUILD_DIR)
	$(GCC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

reverse_test:
	make reverse 
	touch ./$(FILES_DIR)/$(OUTPUT_FILE)
	touch ./$(FILES_DIR)/$(TMP_FILE)
	./$(BUILD_DIR)/reverse ./$(FILES_DIR)/$(INPUT_FILE) ./$(FILES_DIR)/$(TMP_FILE)
	./$(BUILD_DIR)/reverse ./$(FILES_DIR)/$(TMP_FILE) ./$(FILES_DIR)/$(OUTPUT_FILE)
	diff -s ./$(FILES_DIR)/$(INPUT_FILE) ./$(FILES_DIR)/$(OUTPUT_FILE)

reverse_byte_test:
	make reverse_byte 
	touch ./$(FILES_DIR)/$(OUTPUT_FILE)
	touch ./$(FILES_DIR)/$(TMP_FILE)
	./$(BUILD_DIR)/reverse_byte ./$(FILES_DIR)/$(INPUT_FILE) ./$(FILES_DIR)/$(TMP_FILE)
	./$(BUILD_DIR)/reverse_byte ./$(FILES_DIR)/$(TMP_FILE) ./$(FILES_DIR)/$(OUTPUT_FILE)
	diff -s ./$(FILES_DIR)/$(INPUT_FILE) ./$(FILES_DIR)/$(OUTPUT_FILE)

time_test:
	mkdir -p $(RAPORT_DIR)
	touch ./$(RAPORT_DIR)/$(TIME_LOG)
	$(TIME) $(TIME_FORMAT) make reverse_test 2>> ./$(RAPORT_DIR)/$(TIME_LOG)
	$(TIME) $(TIME_FORMAT) make reverse_byte_test 2>> ./$(RAPORT_DIR)/$(TIME_LOG)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(RAPORT_DIR)
	rm -f $(FILES_DIR)/$(OUTPUT_FILE)
	rm -f $(FILES_DIR)/$(TMP_FILE)

