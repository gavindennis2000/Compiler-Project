/*
    parser.h
*/

#include <iostream>
#include <fstream>
#include "token.h"

#ifndef PARSER_H
#define PARSER_H

void parser(std::ifstream& filteredFile);
void printToken(token tok);

// functions for BNF
void S(std::ifstream& filteredFile);
void A(std::ifstream& filteredFile);
void B(std::ifstream& filteredFile);
void C(std::ifstream& filteredFile);
void D(std::ifstream& filteredFile);
void E(std::ifstream& filteredFile);
void F(std::ifstream& filteredFile);
void G(std::ifstream& filteredFile);

#endif