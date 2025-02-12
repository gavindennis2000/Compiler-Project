/*
header file for tree stuff

*/

#ifndef BUILD_TREE_H
#define BUILD_TREE_H

#include "node.h"
#include <iostream>
#include <fstream>

node_t* buildTree(std::ifstream& file);  // returns the location of the root node
void destroyTree(node_t* root);  // recursively deletes all tree nodes

#endif