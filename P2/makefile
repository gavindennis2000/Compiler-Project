# project 2 makefile to remove compilation redundancy
# most of this was copied over from my P0 makefile

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
TARGET = build/P2

# object files to create
OBJS = build/main.o build/parser.o build/scanner.o

# default rule: build the final executable
all: $(TARGET)

# link object files into target executable 
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# compile .cpp source files into .o object files
build/main.o: src/main.cpp
	$(CC) $(CFLAGS) -c src/main.cpp -o ./build/main.o
build/parser.o: src/parser.cpp
	$(CC) $(CFLAGS) -c src/parser.cpp -o build/parser.o
build/scanner.o: src/scanner.cpp
	$(CC) $(CFLAGS) -c src/scanner.cpp -o build/scanner.o

# clean command: clears the build folder
clean: 
	rm -r ./build

# test command: makes and runs the executable with no user input 
test: $(TARGET)
	./build/P2

# none command: makes and runs the executable with nonexistant file (P2_test1) as an argument
none: $(TARGET)
	./build/P2 tests/P2_test1

# test1 command: makes and runs the executable with P2_test1.txt as an argument
test1: $(TARGET)
	./build/P2 tests/P2_test1.txt

# test1e command: makes and runs the executable with P2_test1e.txt as an argument
test1e: $(TARGET)
	./build/P2 tests/P2_test1e.txt

# test2 command: makes and runs the executable with P2_test2.txt as an argument
test2: $(TARGET)
	./build/P2 tests/P2_test2.txt

# test3 command: makes and runs the executable with P2_test3.txt as an argument
test3: $(TARGET)
	./build/P2 tests/P2_test3.txt

# test4 command: makes and runs the executable with P2_test3.txt as an argument
test4: $(TARGET)
	./build/P2 tests/P2_test4.txt

# testGavin command: makes and runs the executable with P2_testGavin.txt as an argument
testGavin: $(TARGET)
	./build/P2 tests/P2_testGavin.txt

# debug command: runs with GNU Debugger
debug: $(TARGET)
	gdb ./build/P2