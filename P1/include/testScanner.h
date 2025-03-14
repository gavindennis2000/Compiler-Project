/*
    testScanner.h
*/

#include <iostream>
#include "token.h"

#ifndef TESTSCANNER_H
#define TESTSCANNER_H

void printToken(token tok);
void scanIfReady(std::string& stringArg, int lineNumber);
void testScanner(std::ifstream& file);

#endif