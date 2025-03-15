/*
    scanner.h 
*/

#include <iostream>

#ifndef SCANNER_H 
#define SCANNER_H

#include <iostream>
#include "token.h"

token FADriver(std::string, int, char);
void invalidTokenError(std::string fileStr, int lineNum);
int getLookaheadValue(char lookahead);

#endif