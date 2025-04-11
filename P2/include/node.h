/*
    node.h 

    defines the node structure which represents the leaves of a binary tree
    structure. Members represent the parent and children of the nodes, as well
    as their character length and level in the tree.
*/

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <node.h>

struct node_t {
    /**/
    
    std::string label;
    std::string decoration = "";  // specific ID of token or empty
    
    int level = 0;
    std::vector<node_t*> children;
};

#endif