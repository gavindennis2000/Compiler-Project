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
node_t* S(int);
node_t* A(int);
node_t* B(int);
node_t* C(int);
node_t* D(int);
node_t* E(int);
node_t* F(int);
node_t* G(int);

#endif