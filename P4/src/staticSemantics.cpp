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
std::string currentOp;  // keeps track of operation being performed on the current symbol
std::string currentVar;  // keeps track of most recent variable for operations such as assignment

bool checkStaticSemantics(node_t * root, std::ofstream& assembly) {
    // modified from previous printTree function in testTree
    // uses preorder traversal to verify static semantics of parse tree

    // check for variable declaration tokens
    if (root->label == "t1") {
        currentOp = root->decoration;
        if (root->decoration == "#" || root->decoration == "\"") {
            declareVariable = true;
            // generate the right assembly code
            if (root->decoration == "#") {
                assembly << "READ ";
            }
        }
    }

    // handle variable declarations and operations
    if (root->label == "t2") {
        // get variable names for assembly code
        std::string symbolToVariable = root->decoration;
        symbolToVariable.replace(0, 1, "P");

        // add t2s to symbol table
        if (declareVariable) {
            declareVariable = false;
            insert(root->decoration);

            // determine what is happening with the current variable and
            // generate the assembly code for it
            // I had to use a bunch of "if/else" because c++ can't do switch statements
            // with strings unfortunately
            if (currentOp == "#") {
                // reading in variables
                assembly << symbolToVariable << "\n";
            }
            else if (currentOp == "\"") {
                // store 0 in new variable
                assembly << "LOAD 0" << "\n" << "STORE " << symbolToVariable << "\n";
            }
        }
        // if variable is already declared, handle the operation being acted upon it
        else {
            // verify variable is in symbol table before it can be used
            if (!verify(root->decoration)) {
                std::cerr << "ERROR: Undefined variable \"" << root->decoration << "\". Terminating program.\n";
                exit(EXIT_FAILURE);
            }
            // determine what is happening with the current variable and
            // generate the assembly code for it
            if (currentOp == "!") {
                // negate operator multiplies by negative one
                assembly << "LOAD " << symbolToVariable << "\n"
                         << "MULT -1" << "\n"
                         << "STORE " << symbolToVariable << "\n";
            }
            else if (currentOp == "$") {
                // write the variable to the screen
                assembly << "WRITE " << symbolToVariable << "\n";
            }
            else if (currentOp == "(") {
                // There isn't really an operator acting on the variable yet, so just remember it for the next op
                currentVar = symbolToVariable;
            }
        }
    }

    if (root->label == "t3") {
        // handle operations that involve integers

        // decide if t3 integer is positive or negative
        std::string currentInt = root->decoration;
        char firstChar = (root->decoration)[0];
        if (currentInt.length() >= 3 && currentInt[1] == '0' && currentInt[2] == '0') {
            currentInt = "0";
        }
        else if (firstChar >= 'a' && firstChar <= 'z') {
            // negative integer
            currentInt.replace(0, 1, "-");
        }
        else {
            // positive integer
            currentInt.replace(0, 1, "");
        }

        if (currentOp == "%" && !currentVar.empty()) {
            // write assembly code for variable assignment
            assembly << "LOAD " << currentInt << "\n"
                     << "STORE " << currentVar << "\n";
        }
        else if (currentOp == "&" && !currentVar.empty()) {
            // write assembly code for addition operation
            assembly << "LOAD " << currentVar << "\n"
                     << "ADD " << currentInt << "\n"
                     << "STORE " << currentVar << "\n";
        }
        // reset the current operator and variable
        currentOp = "";
        currentVar = "";
    }

    // recursively traverse tree
    for (long long unsigned int i = 0; i < root->children.size(); i++) {
        // note: long long unsigned int is the type used for vector size
        checkStaticSemantics(root->children[i], assembly);
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

void printSymbolTable(std::ofstream& assembly) {
    // outputs symbol table to the end of assembly code file

    assembly << "STOP" << "\n";

    for (long long unsigned int i = 0; i < symbolTable.size(); i++) {
        // change "+" to a capital P for variables
        std::string symbolToVariable = symbolTable[i];
        symbolToVariable.replace(0, 1, "P");
        // assign each variable the value 0
        assembly << symbolToVariable << " 0" << "\n";
    }
}