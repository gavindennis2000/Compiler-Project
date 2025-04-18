/*
    node.h 

    defines the node structure which represents the parse tree leaves.
    Members represent the label, level, and decoration (i.e. token string)
    of the node, as a vector of their children nodes.
*/

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <iostream>
#include <node.h>

struct node_t {
    /* represents a single node of parse tree created from user input */
    
    int level = 0;  // represents depth of current node
    std::string label = "";  // denotes either a nonterminal, epsilon, or terminal token type
    std::string decoration = "";  // string associated with terminal token
    
    std::vector<node_t*> children;  // a resizable array of node's children. vector::push_back adds to these lists
};

#endif