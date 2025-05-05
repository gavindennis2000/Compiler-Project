/*
    staticSematics.h
*/

#include "node.h"
#include <iostream>
#include <fstream>

#ifndef STATIC_SEMANTICS_H
#define STATIC_SEMANTICS_H

bool checkStaticSemantics(node_t *, std::ofstream&);
void insert(std::string);
void printSymbolTable(std::ofstream&);
bool verify(std::string);

#endif