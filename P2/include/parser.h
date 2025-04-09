/*
    parser.h
*/

#include <iostream>
#include "token.h"

#ifndef PARSER_H
#define PARSER_H

void filter(std::ifstream& file);
void parser(std::string& stringArg, int lineNumber, bool eofReached);
void printToken(token tok);

// functions for BNF
void S();
void A();
void B();
void C();
void D();
void E();
void F();
void G();

#endif