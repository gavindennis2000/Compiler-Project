/*
    staticSemantics.cpp 

    Verifies static semantics of parse tree.
    Creates a symbol table of defined variables
    and prints out table if no errors.

    For P4, this function also generates the assembly
    code for the .asm file.
*/

#include "staticSemantics.h"
#include "token.h"  // for token structure
#include <vector>  // for resizable token array
#include <queue>  // for keeping track of nested operators
#include <stack>  // for nested addition
#include <iostream>

std::vector<std::string> symbolTable;  // initialize symbol table as a global vector
int numOfTempVars = 0;  // a list of temp variables used for for loops
int numOfLoops = 0;  // how many loops have been created in the assembly code
int loopArgs = -1;  // keeps track of arguments for conditional for loops; -1 implies no loop at the moment
int backtrace = 0;  // used to return through a loop. holds the value of the loop temp variable
bool declareVariable = false;  // flag used to check if variable is being declared or used
bool printNext = false;  // used when operations are nested inside the WRITE operator
bool continueSum = false;  //  used to ensure accumulator isn't overwritten during nested addition
bool negateNext = false;  // used to negate the result of a sum or several sums
std::string assignTo = "";  // used to store an identifier where the result of the next operation is assigned
std::vector<std::string> tokenList;  // keeps track of previous tokens. Used for sums, assignments, and loops mostly
std::stack<std::string> addNest;  // keeps track of nested addition operations

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
    else std::cout << "Empty tokenlist\n";

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
            // make sure loop arg counter is setup
            if (loopArgs == -1) {
                loopArgs = 0;
            }
            // if an operation is found after a for loop, clear the token list up to this point
            tokenList.clear();
            tokenList.push_back(root->decoration);
            // after the next operation is performed, set up a return to the loop
            if (loopArgs >= 3) {
                backtrace = numOfLoops;
                // assembly << "\tSetting backtrace to " << backtrace << "\n";
            }
            else {
                // assembly << "\tLoopArg is: " << loopArgs << "\n";
            }
        }
        else if (tokenList.front() == "$" && tokenList.size() >= 2) {
            // if the next token hasn't been written, remove the $
            // token from the vector and print the next operation
            tokenList.erase(tokenList.begin());
            printNext = true;
        }
        else if (tokenList.size() == 2 && tokenList.back() == "%") {
            // prepare to assign the next result of the next operation to an identifier
            assignTo = tokenList.front();
            tokenList.clear();
        }
        else if (tokenList.front() == "!" && tokenList.back() == "&") {
            // handle the negation of a sum (or several sums)
            tokenList.erase(tokenList.begin(), tokenList.begin() + 1);
            // negation will be added after the sum is taken care of
            negateNext = true;
            
        }
        else if (tokenList.front() == "&" && tokenList.back() == "&" && tokenList.size() > 1) {
            // handle nested addition
            if (tokenList[1] == "&") {
                // e.g. & & x13 a13 +14 -> The sum of (the sum of two numbers) and a single number
                // store the first addition operator in the stack
                addNest.push(tokenList.front());
                tokenList.erase(tokenList.begin(), tokenList.begin() + 1);
            }
            else {
                // e.g. & x13 & a13 +14 -> The sum of a number and (the sum of two numbers)
                addNest.push(tokenList[1]);
                // remove the first two elements of tokenList since they're stored
                tokenList.erase(tokenList.begin(), tokenList.begin() + 2);
            }
            std::stack<std::string> tempAddNest = addNest;
            for (int i = 0; i < tempAddNest.size(); i++) {
                tempAddNest.pop();
            }
            // these two cases should handle any nested addition scenarios
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
                // negate operator multiplies variable by negative one
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

                // if there is nested addition, make sure the result isn't overrided
                if (continueSum) {
                    assembly << "STORE TEMP" << ++numOfTempVars << "\n";
                }
                // perform the normal addition
                assembly << "LOAD " << tokenList[1] << "\n"
                         << "ADD " << tokenList[2] << "\n";
                // if a temp variable was created, add it to the result now
                if (continueSum) {
                    assembly << "ADD TEMP" << numOfTempVars << "\n";
                }
                // check if the result is assigned to anything
                if (!assignTo.empty()) {
                    assembly << "STORE " << assignTo << "\n";
                    assignTo = "";
                }
                // reset token list
                tokenList.clear();
            }
            else if (tokenList.size() == 1 && !assignTo.empty()) {
                // assign the value of this variable to the variable stored in assignTo
                assembly << "LOAD " << tokenList.front() << "\n"
                         << "STORE " << assignTo << "\n";
                assignTo = "";
                tokenList.clear();
            }
            else if (tokenList.front() == "\'") {
                // gather the temporary variables for the conditional for loop
                if (loopArgs == 0) {
                    // the first argument
                    assembly << "LOAD " << tokenList[1] << "\n"
                             << "STORE ARG" << ++loopArgs << "\n";
                    // clear the token from the list
                    tokenList.erase(tokenList.end());
                }
                else if (loopArgs == 1) {
                    // the second argument
                    assembly << "LOAD " << tokenList[1] << "\n"
                             << "STORE ARG" << ++loopArgs << "\n";
                    // skip the for loop if the accumulator isn't positive
                    assembly << "LOAD ARG1" << "\n"
                             << "SUB ARG2" << "\n"
                             << "BRZNEG SKIP" << ++numOfLoops << "\n";
                    // clear the token from the list
                    tokenList.erase(tokenList.end());
                }
                else if (loopArgs == 2) {
                    // third arg
                    // find out how many times to run the loop
                    assembly << "LOAD " << tokenList.back() << "\n"
                             << "STORE ITER" << numOfLoops << "\n"
                             // skip the loop if the third argument is non-positive
                             << "BRZNEG SKIP" << numOfLoops << "\n"
                             << "LOOP" << numOfLoops << ": SUB 1" << "\n"
                             << "STORE ITER" << numOfLoops<< "\n";
                    // increment the number of loop args
                    loopArgs++;
                    // clear the token from the list
                    tokenList.erase(tokenList.end());
                }
                else if (loopArgs >= 3) {
                    // the fourth arg is an operation, so there's nothing here
                    loopArgs++;
                    tokenList.clear();//todoTODO
                    tokenList.push_back("\'");
                }
                // assembly << "\tLoopargs: " << loopArgs << "\n";
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

        // fix numbers that have a starting 0 (e.g. 05)
        // this is purely for aesthetics as the program works fine without it
        if (tokenList.back().length() > 1) {
            if (tokenList.back()[0] == '0') {
                tokenList.back().erase(0, 1);
            }
            else if (tokenList.back()[0] == '-' && tokenList.back()[1] == '0') {
                tokenList.back().erase(1, 1);
            }
        }

        // check the current operator and see what work needs to be done
        if (tokenList.front() == "!") {
            // negate operator multiplies integer by negative one
            assembly << "LOAD " << tokenList.back() << "\n"
                     << "MULT -1" << "\n";
                    //  << "STORE " << tokenList.back() << "\n";
            tokenList.clear();
        }
        else if (tokenList.size() >= 3 && tokenList[1] == "%") {
            // write assembly code for assigning an integer to a variable
            assembly << "LOAD " << tokenList[2] << "\n"
                     << "STORE " << tokenList[0] << "\n";
            // reset token list
            tokenList.clear();
        }
        else if (tokenList.front() == "&" && tokenList.size() == 3) {
            // if there is nested addition, make sure the result isn't overrided
            if (continueSum) {
                assembly << "STORE TEMP" << ++numOfTempVars << "\n";
            }
            // perform the normal addition
            assembly << "LOAD " << tokenList[1] << "\n"
                     << "ADD " << tokenList[2] << "\n";
            // if a temp variable was created, add it to the result now
            if (continueSum) {
                assembly << "ADD TEMP" << numOfTempVars << "\n";
            }
            // check if the result is assigned to anything
            if (!assignTo.empty()) {
                assembly << "STORE " << assignTo << "\n";
                assignTo = "";
            }
            // reset token list
            tokenList.clear();
        }
        else if (tokenList.front() == "$" && tokenList.size() == 2) {
            // if the number is alone with a write operator, write to the screen
            // TODO - take care of the Fs
            assembly << "WRITE " << tokenList.back() << "\n";
            // reset token list
            tokenList.clear();
        }         
        else if (tokenList.size() == 1 && !assignTo.empty()) {
            // assign the value of this integer to the variable stored in assignTo
            assembly << "LOAD " << tokenList.front() << "\n"
                     << "STORE " << assignTo << "\n";
            assignTo = "";
            tokenList.clear();
        }
        else if (tokenList.front() == "\'") {
            // gather the temporary variables for the conditional for loop
            if (loopArgs == 0) {
                // the first argument
                assembly << "LOAD " << tokenList[1] << "\n"
                         << "STORE ARG" << ++loopArgs << "\n";
                // clear the token from the list
                tokenList.erase(tokenList.end());
            }
            else if (loopArgs == 1) {
                // the second argument
                assembly << "LOAD " << tokenList[1] << "\n"
                         << "STORE ARG" << ++loopArgs << "\n";
                // skip the for loop if the accumulator isn't positive
                assembly << "LOAD ARG1" << "\n"
                         << "SUB ARG2" << "\n"
                         << "BRZNEG SKIP" << ++numOfLoops << "\n";
                // clear the token from the list
                tokenList.erase(tokenList.end());
            }
            else if (loopArgs == 2) {
                // third arg
                // find out how many times to run the loop
                assembly << "LOAD " << tokenList.back() << "\n"
                         << "STORE ITER" << numOfLoops << "\n"
                         // skip the loop if the third argument is non-positive
                         << "BRZNEG SKIP" << numOfLoops << "\n"
                         << "LOOP" << numOfLoops << ": SUB 1" << "\n"
                         << "STORE ITER" << numOfLoops<< "\n";
                // increment the number of loop args
                loopArgs++;
                // clear the token from the list
                tokenList.erase(tokenList.end());
            }
            else if (loopArgs >= 3) {
                // the fourth arg is an operation, so there's nothing here
                loopArgs++;
                tokenList.clear();//todoTODO
                tokenList.push_back("\'");
            }
            // assembly << "\tLoopargs: " << loopArgs << "\n";
        }
    }

    // handle nested operations and intraloop operations
    while (!addNest.empty()) {
        // handle any nested additions remaining
        if (!tokenList.empty() && addNest.top() == "&" && tokenList.front() != "&") {
            // if two addition ops were back to back, store the result of the rightmost
            // operation in a temp variable, then add them to the token vector
            addNest.pop();
            assembly << "ADD " << tokenList.front() << "\n";
            tokenList.clear();
        }
        else if (tokenList.empty() && addNest.top() != "&") {
            // if an integer or identifier was wedged between two additions, just add it to
            // the current result
            assembly << "ADD " << addNest.top() << "\n";
            addNest.pop();
        }
        else {
            // if neither of these can happen, break out of the infinite loop
            break;
        }
        // if an operation goes through, make sure the accumulator isn't overwritten
        if (!addNest.empty()) {
            continueSum = true;
        }
        else {
            continueSum = false;
        }
    }

    if (tokenList.empty() && loopArgs != -1) {
        switch(loopArgs) {
            case 0:
                assembly << "STORE ARG" << ++loopArgs << "\n";
                // clear the token from the list
                // tokenList.erase(tokenList.end());
                tokenList.push_back("\'");
                break;
            case 1:
                assembly << "STORE ARG" << ++loopArgs << "\n";
                // skip the for loop if the accumulator isn't positive
                assembly << "LOAD ARG1" << "\n"
                         << "SUB ARG2" << "\n"
                         << "BRZNEG SKIP" << ++numOfLoops << "\n";
                // clear the token from the list
                // tokenList.erase(tokenList.end());
                tokenList.push_back("\'");
                break;
            case 2:
                // find the difference of arg 1 and arg2, then calculate arg3 (the iterator)
                loopArgs++;
                assembly << "STORE ITER" << numOfLoops << "\n";
                tokenList.push_back("\'");
                // skip the loop if the third argument is non-positive
                assembly << "BRZNEG SKIP" << numOfLoops << "\n"
                         << "LOOP" << numOfLoops << ": SUB 1" << "\n"
                         << "STORE ITER" << numOfLoops<< "\n";
                break;
        }
    }

    if (negateNext && tokenList.empty() && addNest.empty()) {
        // multiply the result of a sum by negative 1 and leave
        // it in the accumulator
        negateNext = false;
        assembly << "MULT -1" << "\n";
    }

    if (printNext && tokenList.empty() && addNest.empty()) {
        // print the result of a nested operation
        printNext = false;
        assembly << "STORE TEMP" << ++numOfTempVars << "\n"
                 << "WRITE TEMP" << numOfTempVars << "\n";
    }

    // handle loop returns
    if (backtrace > 0 && tokenList.empty() && addNest.empty()) {
        // set up a return to the loop
        assembly << "LOAD ITER" << backtrace << "\n"
                 << "BRPOS LOOP" << numOfLoops << "\n"
                 << "SKIP" << numOfLoops << ": NOOP" << "\n";
        backtrace = 0;
        loopArgs = -1;
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
    // loop argument temps
    if (numOfLoops > 0) {
        assembly << "ARG1 0" << "\n"
                 << "ARG2 0" << "\n";
    }
    // loop iterators
    for (int i = 1; i <= numOfLoops; i++) {
        assembly << "ITER" << i << " 0" << "\n";
    }
    // temp variables
    for (int i = 1; i <= numOfTempVars; i++) {
        assembly << "TEMP" << i << " 0" << "\n";
    }
}