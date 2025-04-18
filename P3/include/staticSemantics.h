/*
    staticSematics.h
*/

#include "node.h"
#include <iostream>

#ifndef STATIC_SEMANTICS_H
#define STATIC_SEMANTICS_H

bool checkStaticSemantics(node_t *);
void insert(std::string);
void printSymbolTable();
bool verify(std::string);

#endif