/*
    testTree.cpp

    Contains functions for creating parse tree nodes, printing the parse tree,
    and deleting the tree afterwords.

    2025/04/17 - refactored "printTree" function into "checkStaticSemantics" and 
    moved to staticSemantics.cpp
*/

#include <iostream> 
#include <testTree.h>
#include <node.h>
#include <staticSemantics.h>
#include <vector>

node_t * getNode(std::string label, int level, std::string decoration) {
    // allocates memory for leaf node and labels it

    node_t * node = new node_t;  // allocate memory for new leaf
    node->label = label;
    node->level = level;
    node->decoration = decoration;
    
    return node;
}

void destroyTree(node_t * root) {
    // uses postorder traversal to free memory from tree node pointers

    for (long long unsigned int i = 0; i < root->children.size(); i++) {
        destroyTree(root->children[i]);
    }

    delete(root);

    // parse tree has been deleted
    return;
}