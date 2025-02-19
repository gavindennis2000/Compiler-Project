/*
    traversals.cpp

*/

#include "traversals.h"  // function prototypes
#include "node.h"  // for node structures
#include <iostream>  // for std::string
#include <fstream>  // for writing to files
#include <queue>  // for queue library which is used for breadth first traversal

void traverseLevelOrder(node_t* root, std::ofstream& levelFile) {
    /* takes tree root and output file as arguments.
    traverses each level of the binary tree from left to right 
    starting with the lowest and ending with the highest */
    
    node_t* leaf = root;  // copy the root to a new pointer so you don't lose the tree root
    static std::queue<node_t*> q;  /* create an empty queue of node pointers. it's static 
                                  * so that it isn't redeclared when recursively running the function */
    
    if (leaf == nullptr) {
        // first, stop the function if the leaf doesn't exist
        
        return;
    }

    // set up the string that will be added to the output file
    // string contains indentation, node level, and node string list
    std::string outputStr = "";
    for (int i = 0; i < leaf->level; i++) {
        // indent the string depending on the node's level

        outputStr += "    ";  // four spaces per level
    }
    outputStr += std::to_string(root->level) + " " + std::to_string(root->numberOfChars) + " ";  // add the note level and char count
    int listSize = leaf->stringsAlreadySeen.size();  // find how many strings are in the node's 'strings already seen' list
    for (int i = 0; i < listSize; i++) {
        // add each word in the 'strings already seen' list to the output string

        if (i == 0) {
            // beginning of the list

            outputStr += "{";  
        }
        std::string wordToAdd = leaf->stringsAlreadySeen[i];
        outputStr += " " + wordToAdd;  // add the word and some spaces
        if (i == listSize - 1) {
            // end of the list

            outputStr += " }";  
        }
    }

    // write the finalized string to filename.preorder
    levelFile << outputStr << "\n";

    if (leaf->left != nullptr) {
        // if the node has a left child, add it to the queue

        q.push(leaf->left);
    }
    if (leaf->right != nullptr) {
        // if the node has a right child, add it to the queue

        q.push(leaf->right);
    }

    if (!q.empty()) {
        // if the queue isn't empty, recursively run the function using the next
        // node in line

        node_t* next = q.front();
        q.pop();

        traverseLevelOrder(next, levelFile);
    }
}

void traversePreOrder(node_t* root, std::ofstream& preFile) {
    /* takes tree root and ouput file as arguments.
    traverses the binary tree starting with the root, followed by
    the left sub-tree, ending with the right sub-tree */

    node_t* leaf = root;  // copy the root to a new pointer so you don't lose the tree root

    if (leaf == nullptr) {
        // first, stop the function if the leaf doesn't exist

        return;
    }

    // set up the string that will be added to the output file
    // string contains indentation, node level, and node string list
    std::string outputStr = "";
    for (int i = 0; i < leaf->level; i++) {
        // indent the string depending on the node's level

        outputStr += "    ";  // four spaces per level
    }
    outputStr += std::to_string(root->level) + " " + std::to_string(root->numberOfChars) + " ";  // add the note level and char count
    int listSize = leaf->stringsAlreadySeen.size();  // find how many strings are in the node's 'strings already seen' list
    for (int i = 0; i < listSize; i++) {
        // add each word in the 'strings already seen' list to the output string

        if (i == 0) {
            // beginning of the list

            outputStr += "{";  
        }
        std::string wordToAdd = leaf->stringsAlreadySeen[i];
        outputStr += " " + wordToAdd;  // add the word and some spaces
        if (i == listSize - 1) {
            // end of the list

            outputStr += " }";  
        }
    }

    // write the finalized string to filename.preorder
    preFile << outputStr << "\n";

    // recursively iterate through the linked list
    traversePreOrder(leaf->left, preFile);
    traversePreOrder(leaf->right, preFile);
    
    // traversal complete
}

void traversePostOrder(node_t* root, std::ofstream& postFile) {
    /* takes tree root and ouput file as arguments.
    traverses the binary tree starting with the left sub-tree, followed by
    the right sub-tree, ending with the root 
    
    very similar to preorder function, but with a different recursion order */

    node_t* leaf = root;  // copy the root to a new pointer so you don't lose the tree root

    // first, stop the function if the leaf doesn't exist
    if (leaf == nullptr) {
        return;
    }

    // recursively iterate through the linked list
    traversePostOrder(leaf->left, postFile);
    traversePostOrder(leaf->right, postFile);

    // set up the string that will be added to the output file
    // string contains indentation, node level, and node string list
    std::string outputStr = "";
    for (int i = 0; i < leaf->level; i++) {
        // indent the string depending on the node's level

        outputStr += "    ";  // four spaces per level
    }
    outputStr += std::to_string(root->level) + " " + std::to_string(root->numberOfChars) + " ";  // add the note level and char count
    int listSize = leaf->stringsAlreadySeen.size();
    for (int i = 0; i < listSize; i++) {
        // add each word in the 'strings already seen' list to the output string

        if (i == 0) {
            // beginning of the list

            outputStr += "{";  
        }
        std::string wordToAdd = leaf->stringsAlreadySeen[i];
        outputStr += " " + wordToAdd;  // add the word and some spaces
        if (i == listSize - 1) {
            // end of the list

            outputStr += " }";  
        }
    }

    // write the finalized string to filename.postorder
    postFile << outputStr << "\n";
    
    // traversal complete
}