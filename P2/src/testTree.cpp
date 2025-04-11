/*
    testTree.cpp
*/

#include <iostream> 
#include <testTree.h>
#include <node.h>
#include <vector>

node_t * getNode(std::string label, std::string decoration) {
    // allocates node and labels it

    node_t * node = new node_t;
    node->label = label;
    node->decoration = decoration;
    
    return node;
}

void printTree(node_t * root) {
    // uses preorder traversal to print out parse tree 

    // create the string that will be printed out for the current node
    std::string stringToPrint = "";
    for (int i = 0; i < root->level; i++) {
        // add an indentation (four spaces) for each level
        stringToPrint += "    ";
    }
    stringToPrint += root->label;
    if (root->decoration != "") {
        stringToPrint += " " + root->decoration;
    }

    // print out each node
    std::cout << stringToPrint << "\n";
    for (long long unsigned int i = 0; i < root->children.size(); i++) {
        // note: long long unsigned int is the type used for vector size
        printTree(root->children[i]);
    }

    // parse tree has been printed
    return;
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