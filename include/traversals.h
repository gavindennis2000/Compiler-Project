/*
traversals.h 

function prototypes for traversal methods

*/

#ifndef TRAVERSALS_H
#define TRAVERSALS_H

#include "node.h"
#include <fstream>

void traverseLevelOrder(node_t* root, std::ofstream& levelFile);  // travserses from left to right for each level of the tree
void traversePreOrder(node_t* root, std::ofstream& preFile);  // recursively travserses starting with the root, then the left subtree, then the right subtree
void traversePostOrder(node_t* root, std::ofstream& postFile);  // traverses starting with the left subtree, then the right, then the root

#endif