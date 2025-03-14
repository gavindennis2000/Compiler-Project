/*
    testScanner.cpp

    Filters input file from main and passes it line by line
    to scanner, then prints out the returned token.
*/

#include <iostream>
#include "token.h"

#ifndef TESTSCANNER_H
#define TESTSCANNER_H

void printToken(token tok);
void scanIfReady(std::string& stringArg, int lineNumber, bool eofReached);
void testScanner(std::ifstream& file);

#endif