/*
    testTree.h
*/

#ifndef TESTTREE_H
#define TESTTREE_H

#include <iostream>
#include <node.h>

node_t * getNode(std::string label, int level, std::string decoration = "");
void destroyTree(node_t * root);

#endif