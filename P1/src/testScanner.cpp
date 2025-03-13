/*
    testScanner.cpp

    iteratively calls scanner() and prints the returned token
    until EOF
*/

#include "testScanner.h"
#include <iostream>
#include <fstream>

void testScanner(std::ifstream& file) {
    /* opens file provided as argument, parses*/

    std::string word;
    char ch;
    while (file.get(ch)) {
        std::cout << ch << " ";
    }
}