/*
    staticSemantics.cpp 

    Verifies static semantics of parse tree.
    Creates a symbol table of defined variables
    and prints out table if no errors.
*/

#include "staticSemantics.h"
#include "token.h"  // for token structure
#include <vector>  // for resizable token array
#include <queue>  // for keeping track of nested operators
#include <iostream>

std::vector<std::string> symbolTable;  // initialize symbol table as a global vector
int numOfTempVars = 0;  // a list of temp variables used for for loops
int numOfLoops = 0;  // how many loops have been created in the assembly code
int loopArg = 1;  // keeps track of arguments for conditional for loops
bool declareVariable = false;  // flag used to check if variable is being declared or used
int backtrace = 0;  // used to return through a loop. holds the value of the loop temp variable
bool printNext = false;  // used when operations are nested inside the WRITE operator
std::vector<std::string> tokenList;  // keeps track of previous tokens. Used for sums, assignments, and loops mostly

bool checkStaticSemantics(node_t * root, std::ofstream& assembly) {
    // modified from previous printTree function in testTree
    // uses preorder traversal to verify static semantics of parse tree
    // while simultaneously generating the related assembly code

    // keep track of the tokens until we know what to do with them
    if (root->label == "t1" || root->label == "t2" || root->label == "t3") {
        tokenList.push_back(root->decoration);
    }
    if (tokenList.size() > 0) {
        std::cout << "\nToken list: " << tokenList.size() << "\n";
        for (long long unsigned int i = 0; i < tokenList.size(); i++) {
            std::cout << tokenList[i] << "\n";
        }
    }

    // store the current operator and predict situations where variables will be declared
    if (root->label == "t1") {
        // check if operator is used to declare a variable
        if (root->decoration == "#" || root->decoration == "\"") {
            declareVariable = true;
        }
        else if (tokenList.front() == "(" || tokenList.back() == ")") {
            // clear delimters
            tokenList.clear();
        }
        else if (tokenList.front() == "\'") {
            // if an operation is found after a for loop, clear the token list up to this point
            tokenList.clear();
            tokenList.push_back(root->decoration);
            // after the next operation is performed, set up a return to the loop
            backtrace = numOfTempVars;
        }
        else if (tokenList.front() == "$" && tokenList.size() >= 2) {
            // if the next token hasn't been written, remove the $
            // token from the vector and print the next operation
            tokenList.erase(tokenList.begin());
            printNext = true;
        }
    }

    // handle variable declarations and operations
    else if (root->label == "t2") {
        // change the name of the variable token from +[xxx] to P[xxx]
        tokenList.back().replace(0, 1, "P");

        // add t2s to symbol table
        if (declareVariable) {
            declareVariable = false;
            insert(root->decoration);

            // determine what is happening with the current variable and
            // generate the assembly code for it
            // I had to use a bunch of "if/else" because c++ can't do switch statements
            // with strings unfortunately
            if (tokenList.front() == "#") {
                // reading in variables
                assembly << "READ " << tokenList.back() << "\n";
                tokenList.clear();
            }
            else if (tokenList.front() == "\"") {
                // store 0 in new variable
                assembly << "LOAD 0" << "\n" << "STORE " << tokenList.back() << "\n";
                tokenList.clear();
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
            if (tokenList.front() == "!") {
                // negate operator multiplies by negative one
                assembly << "LOAD " << tokenList.back() << "\n"
                         << "MULT -1" << "\n"
                         << "STORE " << tokenList.back() << "\n";
                tokenList.clear();
            }
            else if (tokenList.front() == "$" && tokenList.size() == 2) {
                // write the variable to the screen
                // if the number is alone with a write operator, write to the screen
                assembly << "WRITE " << tokenList.back() << "\n";
                // reset token list
                tokenList.clear();
            }
            else if (tokenList.front() == "&" && tokenList.size() == 3) {
                // write assembly code for addition operation. the result
                // gets left in the accumulator
                assembly << "LOAD " << tokenList[1] << "\n"
                         << "ADD " << tokenList[2] << "\n";
                // reset token list
                tokenList.clear();
            }
            else if (tokenList.front() == "%" && tokenList.size() == 3) {
                // assign the value of another variable to this variable
                assembly << "LOAD " << tokenList[1] << " \n"
                         << "STORE " << tokenList.back() << "\n";
                // reset the token list
                tokenList.clear();
            }
            else if (tokenList.front() == "\'") {
                // gather the temporary variables for the conditional for loop
                if (tokenList.size() == 2) {
                    // the first argument
                    assembly << "LOAD " << tokenList[1] << "\n";
                }
                else if (tokenList.size() == 3) {
                    // the second argument
                    assembly << "SUB " << tokenList[2] << "\n";
                }
                else if (tokenList.size() == 4) {
                    // third arg
                    // skip the for loop if the accumulator isn't positive
                    assembly << "BRZNEG SKIP" << "\n";
                    // find out how many times to run the loop
                    numOfTempVars++;
                    numOfLoops++;
                    assembly << "LOAD " << tokenList[3] << "\n"
                                << "STORE TEMP" << numOfTempVars << "\n"
                                // skip the loop if the third argument is non-positive
                                << "BRZNEG SKIP" << "\n"
                                << "LOOP" << numOfLoops << ": SUB1" << "\n"
                                << "STORE TEMP" << numOfTempVars << "\n";
                }
                else if (tokenList.size() == 5) {
                    // the fourth arg is an operation, so there's nothing here
                    backtrace = true;
                    tokenList.clear();//todoTODO
                }
            }
        }
    }

    else if (root->label == "t3") {
        // handle operations that involve integers

        // decide if t3 integer is positive or negative by checking if the first letter is uppercase
        char firstChar = tokenList.back()[0];
        if (tokenList.back().length() >= 3 && tokenList.back()[1] == '0' && tokenList.back()[2] == '0') {
            // tokens proceeded by at least two zeroes represent zero
            tokenList.back() = "0";
        }
        else if (firstChar >= 'a' && firstChar <= 'z') {
            // negative integer
            tokenList.back().replace(0, 1, "-");
        }
        else {
            // positive integer
            tokenList.back().replace(0, 1, "");
        }

        // check the current operator and see what work needs to be done
        if (tokenList.size() == 3 && tokenList[1] == "%") {
            // write assembly code for assigning an integer to a variable
            assembly << "LOAD " << tokenList[2] << "\n"
                     << "STORE " << tokenList[0] << "\n";
            // reset token list
            tokenList.clear();
        }
        else if (tokenList.front() == "&" && tokenList.size() == 3) {
            // write assembly code for addition operation
            assembly << "LOAD " << tokenList[1] << "\n"
                     << "ADD " << tokenList.back() << "\n";
            // reset token list
            tokenList.clear();
        }
        else if (tokenList.front() == "$" && tokenList.size() == 2) {
            // if the number is alone with a write operator, write to the screen
            assembly << "WRITE " << tokenList.back() << "\n";
            // reset token list
            tokenList.clear();
            // else {
            //     // if there is another operator in the mix, take a look at it
            //     assembly << "GOTCHA\n";
            //     // reset token list
            //     tokenList.clear();
            // }
        }
        else if (tokenList.front() == "\'") {
            // gather the temporary variables for the conditional for loop
            if (tokenList.size() == 2) {
                // the first argument
                assembly << "LOAD " << tokenList[1] << "\n";
            }
            else if (tokenList.size() == 3) {
                // the second argument
                assembly << "SUB " << tokenList[2] << "\n";
            }
            else if (tokenList.size() == 4) {
                // third arg
                // skip the for loop if the accumulator isn't positive
                assembly << "BRZNEG SKIP" << numOfLoops + 1 << "\n";
                // find out how many times to run the loop
                numOfTempVars++;
                numOfLoops++;
                assembly << "LOAD " << tokenList[3] << "\n"
                            << "STORE TEMP" << numOfTempVars << "\n"
                            // skip the loop if the third argument is non-positive
                            << "BRZNEG SKIP" << numOfLoops << "\n"
                            << "LOOP" << numOfLoops << ": SUB 1" << "\n"
                            << "STORE TEMP" << numOfTempVars<< "\n";
            }
            else if (tokenList.size() == 5) {
                // the fourth arg is an operation, so there's nothing here
                tokenList.clear();//todoTODO
            }
        }
    }

    // handle nested operations
    if (tokenList.empty() && printNext) {
        // print the result of a nested operation
        printNext = false;
        assembly << "STORE TEMP" << ++numOfTempVars << "\n"
                 << "WRITE TEMP" << numOfTempVars << "\n";
    }

    // handle loop returns
    if (tokenList.empty() && backtrace > 0) {
        // set up a return to the loop
        assembly << "LOAD TEMP" << backtrace << "\n"
                 << "BRPOS LOOP" << numOfLoops << "\n"
                 << "SKIP" << numOfLoops << ": NOOP" << "\n";
        backtrace = 0;
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
    // outputs symbol table and temp variables to the end of assembly code file
    assembly << "\n" << "STOP" << "\n";

    // symbol table
    for (long long unsigned int i = 0; i < symbolTable.size(); i++) {
        // change "+" to a capital P for variables
        std::string symbolToVariable = symbolTable[i];
        symbolToVariable.replace(0, 1, "P");
        // assign each variable the value 0
        assembly << symbolToVariable << " 0" << "\n";
    }
    // temp variables
    for (int i = 1; i <= numOfTempVars; i++) {
        assembly << "TEMP" << i << " 0" << "\n";
    }
}