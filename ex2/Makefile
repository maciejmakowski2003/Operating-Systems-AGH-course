CC=gcc
BUILD_DIR=build
VIEW_DIR=view

cflags.default := -std=c17 -Wall
cflags.debug := -g -O0

CFLAGS := ${cflags.debug} ${cflags.default}

.PHONY: all client_static client_shared client_dynamic clean 

all:
	make client_static 
	make client_shared
	make client_dynamic
	

client_static: client_static.c 
	mkdir -p $(BUILD_DIR)
	make -C collatz collatz_static
	$(CC) $(CFLAGS) $< collatz/build/libcollatz_static.a -o $(BUILD_DIR)/$@

client_shared: client_shared.c 
	mkdir -p $(BUILD_DIR)
	make -C collatz collatz_shared 
	$(CC) $(CFLAGS) $< -lcollatz_shared -L ./collatz/build -Wl,-rpath=./collatz/build -o $(BUILD_DIR)/$@

client_dynamic: client_dynamic.c 
	mkdir -p $(BUILD_DIR)
	make -C collatz collatz_shared 
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@ -ldl

test_static:
	./$(BUILD_DIR)/client_static

test_shared:
	./$(BUILD_DIR)/client_shared

test_dynamic:
	./$(BUILD_DIR)/client_dynamic

objdump_static:
	mkdir -p $(VIEW_DIR)
	objdump -d $(BUILD_DIR)/client_static > $(VIEW_DIR)/static.txt

objdump_shared:
	mkdir -p $(VIEW_DIR)
	objdump -d $(BUILD_DIR)/client_shared > $(VIEW_DIR)/shared.txt

objdump_dynamic:
	mkdir -p $(VIEW_DIR)
	objdump -d $(BUILD_DIR)/client_dynamic > $(VIEW_DIR)/dynamic.txt

clean: 
	rm -rf $(BUILD_DIR)
	rm -rf $(VIEW_DIR)
	make -C collatz clean 