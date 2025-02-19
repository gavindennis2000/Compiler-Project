/*
    buildTree.cpp

    contains functionality for creating binary tree

    resources consulted:
        geeksforgeeks.org 
            c++ file handling (fstream) documentation: https://www.geeksforgeeks.org/file-handling-c-classes/
            'getline' documentation: https://www.geeksforgeeks.org/getline-string-c/
            'string.find' documentation: https://www.geeksforgeeks.org/string-find-in-cpp/
        stack overflow
            ctr + d solution for getline from Karina Kozarova: https://stackoverflow.com/questions/47612169/c-stop-asking-for-input-on-ctrl-d   
*/

#include "node.h"  // node structure for tree leaves
#include "buildTree.h"  // function and structure prototypes
#include <iostream>  // for getline and stuff
#include <fstream>  // for file handling
#include <string>  // for 'string.find'
#include <vector>  // for holding the list of strings already seen in tree leaf nodes

node_t* buildTree(std::ifstream& file) {
    /* creates a binary tree structure using file inputted from argument. 
    returns the root node of tree */

    bool rootExists = false;  // a flag that checks if a tree root has been assigned
    node_t* root = new node_t;  // dynamically alocated pointer to an empty node that will contain the tree root
    node_t* nodeToCompare = nullptr;  // used for comparing two nodes when creating the tree

    // open the file and start parsing its contents
    std::string word;
    while (file >> word) {
        // add each word to the tree, using 'rootExists' to ensure the
        // first word is its root

        if (!rootExists) {  
            // assign the first word and its characteristics as the root of the tree

            rootExists = true;
            root->numberOfChars = word.length();
            root->level = 0;
            root->stringsAlreadySeen.push_back(word);  // add word to the string list
        }
        else {
            // create the leaf nodes

            node_t* leaf = new node_t;  // create a temporary node pointer for the current leaf
            nodeToCompare = root;
            leaf->numberOfChars = word.length();
            leaf->level = 0;
            leaf->stringsAlreadySeen.push_back(word);

            bool assigned = false;  // flag for new nodes locations
            while (!assigned) {
                // for the current word, keep traversing the tree until it's proper location is found

                if (leaf->numberOfChars > nodeToCompare->numberOfChars) {
                    // if leaf is greater than the current node being compared, check to see if it has child nodes

                    if (nodeToCompare->right == nullptr) {
                        // if the node doesn't have a right child, the leaf becomes the right child

                        leaf->level = nodeToCompare->level + 1;
                        nodeToCompare->right = leaf;
                        assigned = true;
                    }
                    else {
                        // otherwise, the compared node is changed to the right child and the while loop repeats

                        nodeToCompare = nodeToCompare->right;
                    }
                }
                else if (leaf->numberOfChars < nodeToCompare->numberOfChars) {
                    // if leaf is less than the current node being compared, check to see if it has child nodes

                    if (nodeToCompare->left == nullptr) {
                        // if the node doesn't have a left child, the leaf becomes the left child

                        leaf->level = nodeToCompare->level + 1;
                        nodeToCompare->left = leaf;
                        assigned = true;
                    }
                    else {
                        // otherwise, the compared node is changed to the left child and the while loop repeats
                        
                        nodeToCompare = nodeToCompare->left;
                    }
                }
                else {
                    // if the node string lengths are equal, add the string to the list and
                    // delete the current leaf (no need for this node)

                    nodeToCompare->stringsAlreadySeen.push_back(word);
                    assigned = true;
                    delete leaf;
                }
            }
        }
    }
    
    // the tree has been created. return the root to the main function
    return root;
}

void destroyTree(node_t* root) {
    /* takes tree root as argument. recursively deletes every tree node using post-order traversal */ 

    if (root == nullptr) {
        // exit the current function call when a non-existant child is passed as the argument

        return;
    }

    destroyTree(root->left);
    destroyTree(root->right);
    delete root;
}