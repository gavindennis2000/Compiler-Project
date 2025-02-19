/*
    node.h 

    defines the node structure which represents the leaves of a binary tree
    structure. Members represent the parent and children of the nodes, as well
    as their character length and level in the tree.
*/

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>

struct node_t {
    /* a binary tree leaf with two child nodes, an int containing
    the number of characters per string, an int containing its level
    in the tree, and a string-list of all the words of that length */

    node_t* left = nullptr;  // points to the left child of the leaf node
    node_t* right = nullptr;  // points to the right child of the leaf node

    int numberOfChars;  // number of choaracters in the string
    int level;  // level of the current leaf

    // strings already seen
    std::vector<std::string> stringsAlreadySeen;
};

#endif