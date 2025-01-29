# project 0 makefile to remove compilation redundancy
#
# makefile documentation from geeksforgeeks
# https://www.geeksforgeeks.org/makefile-in-c-and-its-applications/
#
# g++ cheatsheet I used for documentation on compiler flags: 
# https://bytes.usc.edu/cs104/wiki/gcc 
#
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

# default rule: build executable and run 
all: $(TARGET) run 

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

# run the executable after final compilation
run: $(TARGET)
	$(TARGET)