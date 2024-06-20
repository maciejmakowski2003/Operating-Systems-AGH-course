#C compiler
CC=gcc
#dir to store all binary files
BUILD_DIR=build

cflags.default := -std=c17 -Wall
cflags.debug := -g -O0

CFLAGS := ${cflags.debug} ${cflags.default}

.PHONY: all countdown clean 

all: countdown 

#$@- target of the rule $<- first prerequisite of the rule
#-o $(BUILD_DIR)/$@- location of output file
countdown: countdown.c 
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

test:
	./$(BUILD_DIR)/countdown

clean:
	rm -rf $(BUILD_DIR)