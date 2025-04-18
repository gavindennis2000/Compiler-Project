/*
    staticSemantics.cpp 

    Verifies static semantics of parse tree.
    Creates a symbol table of defined variables
    and prints out table if no errors.
*/

#include "staticSemantics.h"
#include "token.h"  // for token structure
#include <vector>  // for resizable token array
#include <iostream>

std::vector<std::string> symbolTable;  // initialize symbol table as a global vector
bool declareVariable = false;  // flag used to check if variable is being declared or used

bool checkStaticSemantics(node_t * root) {
    // modified from previous printTree function in testTree
    // uses preorder traversal to verify static semantics of parse tree

    // check for variable declaration tokens
    if (root->label == "t1" && (root->decoration == "#" || root->decoration == "\"")) {
        declareVariable = true;
    }
    // add t2s to symbol table
    if (root->label == "t2") {
        if (declareVariable) {
            declareVariable = false;
            insert(root->decoration);
        }
        else {
            // verify variable is in symbol table before it can be used
            if (!verify(root->decoration)) {
                std::cerr << "ERROR: Undefined variable \"" << root->decoration << "\". Terminating program.\n";
                exit(EXIT_FAILURE);
            }
        }
    }

    // recursively traverse tree
    for (long long unsigned int i = 0; i < root->children.size(); i++) {
        // note: long long unsigned int is the type used for vector size
        checkStaticSemantics(root->children[i]);
    }

    // if it's made it this far, there are no errors
    return true;
}

void insert(std::string tokenStr) {
    /* checks if string is not in symbol table, inserts it if it isn't or
    throws an error if it is */

    for (long long unsigned int i = 0; i < symbolTable.size(); i++) {
        if (symbolTable[i] == tokenStr) {
            std::cerr << "ERROR: Variable " << tokenStr << " has already been defined. Terminating program.\n";
            exit(EXIT_FAILURE);
        }
    }

    // add the variable to the symbol table
    symbolTable.push_back(tokenStr);

    return;
}
bool verify(std::string tokenStr) {
    /* used to verify that variable is defined in process table
    before it is used. returns true if variable is in table */

    // check every entry in symbol table to verify token isn't there
    for (long long unsigned int i = 0; i < symbolTable.size(); i++) {
        if (symbolTable[i] == tokenStr) {
            return true;
        }
    }

    // if nothing matches, return false
    return false;
}

void printSymbolTable() {
    /* prints symbol table after static semantics have been verified */

    std::cout << "\nSymbol Table: \n";

    for (long long unsigned int i = 0; i < symbolTable.size(); i++) {
        std::cout << symbolTable[i] << "\n";
    }
}