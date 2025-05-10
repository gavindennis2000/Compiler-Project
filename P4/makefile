# P4 Makefile
# Most of this was copied over from my P0 makefile

# Makefile documentation from GeeksForGeeks
# https://www.geeksforgeeks.org/makefile-in-c-and-its-applications/

# g++ cheatsheet I used for documentation on compiler flags: 
# https://bytes.usc.edu/cs104/wiki/gcc 

# Build folder solution provided by MadScientist on Stack Overflow
# https://stackoverflow.com/questions/16344719/how-to-create-directory-if-needed

# Compiler: g++ compiler from GNU (free software foundation, inc)
CC = g++

# create build directory if it doesn't already exist 
_build := $(shell mkdir -p build)

# compiler flags
CFLAGS = -g -Wall -std=c++11 -Iinclude

# target executable directory and name
TARGET = build/P4

# object files to create
OBJS = build/main.o build/parser.o build/scanner.o build/testTree.o build/staticSemantics.o

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
build/testTree.o: src/testTree.cpp
	$(CC) $(CFLAGS) -c src/testTree.cpp -o build/testTree.o
build/staticSemantics.o: src/staticSemantics.cpp
	$(CC) $(CFLAGS) -c src/staticSemantics.cpp -o build/staticSemantics.o

# clean command: clears the build folder
clean: 
	rm -r ./build

# test command: makes and runs the executable with no user input 
test: $(TARGET)
	./build/P4

# none command: makes and runs the executable with nonexistant file (P3_test1) as an argument
none: $(TARGET)
	./build/P4 tests/P4_test1

# test1 command: makes and runs the executable with P3_test1.txt as an argument
test1: $(TARGET)
	./build/P4 tests/P4_test1.txt

# test2 command: makes and runs the executable with P3_test2.txt as an argument
test2: $(TARGET)
	./build/P4 tests/P4_test2.txt

# test3 command: makes and runs the executable with P3_test3.txt as an argument
test3: $(TARGET)
	./build/P4 tests/P4_test3.txt

# test4 command: makes and runs the executable with P3_test3.txt as an argument
test4: $(TARGET)
	./build/P4 tests/P4_test4.txt

# testGavin command: makes and runs the executable with P3_testGavin.txt as an argument
testGavin: $(TARGET)
	./build/P4 tests/P4_testGavin.txt

testAll: $(TARGET)
	./build/P4 tests/P4_test1.txt
	./build/P4 tests/P4_test2.txt
	./build/P4 tests/P4_test3.txt
	./build/P4 tests/P4_test4.txt
	./build/P4 tests/P4_testGavin.txt

# debug command: runs with GNU Debugger
debug: $(TARGET)
	gdb ./build/P4