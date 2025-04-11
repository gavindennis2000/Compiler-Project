/*
    parser.h
*/

#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include "token.h"
#include "node.h"

node_t* parser(std::ifstream& filteredFile);
void printToken(token tok);

// functions for BNF
node_t* S();
node_t* A();
node_t* B();
node_t* C();
node_t* D();
node_t* E();
node_t* F();
node_t* G();

#endif