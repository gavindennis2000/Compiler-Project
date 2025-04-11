/*
    scanner.h 
*/

#ifndef SCANNER_H 
#define SCANNER_H

#include <iostream>
#include "token.h"

token scanner(std::ifstream& filteredFile, int& lineNum);
void invalidTokenError(std::string tokenStr, int lineNum);
int getLookaheadValue(char lookahead);

#endif