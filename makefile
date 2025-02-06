# project 0 makefile to remove compilation redundancy

# makefile documentation from geeksforgeeks
# https://www.geeksforgeeks.org/makefile-in-c-and-its-applications/

# g++ cheatsheet I used for documentation on compiler flags: 
# https://bytes.usc.edu/cs104/wiki/gcc 

# build folder solution provided by MadScientist on stack overflow
# https://stackoverflow.com/questions/16344719/how-to-create-directory-if-needed

# compiler: g++ compiler from GNU (free software foundation, inc)
CC = g++

# create build directory if it doesn't already exist 
_build := $(shell mkdir -p build)

# compiler flags
CFLAGS = -g -Wall -std=c++11 -Iinclude

# target executable directory and name
TARGET = build/P0

# object files to create
OBJS = build/main.o build/buildTree.o build/traversals.o

# default rule: build the final executable
all: $(TARGET)

# link object files into target executable 
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# compile .cpp source files into .o object files
build/main.o: src/main.cpp
	$(CC) $(CFLAGS) -c src/main.cpp -o ./build/main.o
build/buildTree.o: src/buildTree.cpp
	$(CC) $(CFLAGS) -c src/buildTree.cpp -o build/buildTree.o
build/traversals.o: src/traversals.cpp
	$(CC) $(CFLAGS) -c src/traversals.cpp -o build/traversals.o

# clean command: clears the build folder
clean: 
	rm $(OBJS) $(TARGET)
	rmdir ./build

# test command: makes and runs the executable with no file argument
test: $(TARGET)
	./build/P0

# testGavin command: makes and runs the executable with P0_testGavin as an argument
testGavin: $(TARGET)
	./build/P0 tests/P0_testGavin

# test1 command: makes and runs the executable with P0_test1 as an argument
test1: $(TARGET)
	./build/P0 tests/P0_test1

# test command: makes and runs the executable with P0_test2 as an argument
test2: $(TARGET)
	./build/P0 tests/P0_test2

# test command: makes and runs the executable with P0_test3 as an argument
test3: $(TARGET)
	./build/P0 tests/P0_test3

# test command: makes and runs the executable with P0_test4 as an argument
test4: $(TARGET)
	./build/P0 tests/P0_test4

# debug command: makes and runs gdb with the executable and a test file argument
debug: $(TARGET)
	gdb ./build/P0