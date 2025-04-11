/*
    testTree.h
*/

#ifndef TESTTREE_H
#define TESTTREE_H

#include <iostream>
#include <node.h>

node_t * getNode(std::string label, std::string decoration = "");
void printTree(node_t * root);
void destroyTree(node_t * root);

#endif