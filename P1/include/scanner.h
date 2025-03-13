/*
    scanner.h

    parses a single lexeme and returns its token ID, string instance,
    and line number
*/

#include <iostream>

#ifndef SCANNER_H 
#define SCANNER_H

#include <iostream>
#include "token.h"

token scanner(std::string lexeme);

#endif