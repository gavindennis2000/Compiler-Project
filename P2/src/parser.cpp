/*
    parser.cpp

    Filters input file from main and passes it line by line
    to scanner, then prints out the returned token.
*/

#include "parser.h"  // header file
#include "scanner.h"  // for scanning tokens
#include "token.h"  // for token struct
#include "node.h"  // for parse tree node struct
#include "testTree.h"  // for node and parse tree functions
#include <string>  // for getline
#include <fstream>  // for file handling
#include <iostream> 

token tok;  // global token variable to be accessed by all functions
std::ifstream * filePtr;  // global variable that points to filtered file from main
int lineNum = 0;

node_t* parser(std::ifstream& filteredFile) {
    /* */

    // set the global file variable to the filteredFile passed by main
    filePtr = &filteredFile;

    tok = scanner(*filePtr, lineNum);
    printToken(tok);
    node_t* root = S();

    if (tok.tokenID == EOF_tk) {
        return root;
    }

    // throw error if unsuccessful parse
    std::cout << "ERROR: EOF token not returned. Terminating.\n";
    exit(EXIT_FAILURE);
}

std::string getLabelFromEnum(tokenType tokenID) {
    // takes in token type enum as argument and returns the converted string

    switch(tokenID) {
        case t1_tk:
            return "t1";
        case t2_tk:
            return "t2";
        case t3_tk:
            return "t3";
        case EOF_tk:
        default:
            return "EOF";
    }
}

void printToken(token tok) {
    /* prints out token information returned by scanner:
    token type, its string, and its line number */

    // get the token type
    std::string tokenTypeStr;
    switch (tok.tokenID) {
        case t1_tk:
            tokenTypeStr = "t1 token";
            break;
        case t2_tk:
            tokenTypeStr = "t2 token";
            break;
        case t3_tk:
            tokenTypeStr = "t3 token";
            break;
        case EOF_tk:
            tokenTypeStr = "EOFTk";
            break;
    }

    // output the token description
    std::cout << tokenTypeStr << "\t" << tok.tokenStr << "\t" << tok.lineNum << "\n";

    return;
}

void parserError() {
    /* prints an error message with token and line number when parsed token
    doesn't match BNF grammar */

    std::cout << "PARSER ERROR: " << tok.tokenStr << " " << lineNum << ".\n Terminating.\n";
    exit(EXIT_FAILURE);
}

// functions for BNF
node_t* S() {
    // S -> A ( B B )

    node_t* root = getNode("S");

    // A
    root->children.push_back( A() );

    // (
    if (tok.tokenStr == "(") {
        root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), tok.tokenStr ) );
        tok = scanner(*filePtr, lineNum);
        printToken(tok);
    }
    else {
        parserError();
    }

    // Two B's
    root->children.push_back( B() );
    root->children.push_back( B() );

    // )
    if (tok.tokenStr == ")") {
        root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), tok.tokenStr ) );
        tok = scanner(*filePtr, lineNum);
        printToken(tok);
    }
    else {
        parserError();
    }

    // parser is finished
    return root;
}

node_t* A() {
    // A -> " t2 | empty

    node_t* root = getNode("A");

    if (tok.tokenStr == "\"") {
        root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), tok.tokenStr ) );
        tok = scanner(*filePtr, lineNum);
        printToken(tok);
        if (tok.tokenID == t2_tk) {
            root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), tok.tokenStr ) );
            tok = scanner(*filePtr, lineNum);
            printToken(tok);
        }
        else {
            parserError();
        }
    }
    else {
        root->children.push_back( getNode("empty") );
    }

    return root;
}

node_t* B() {
    // B -> S | C | D | E | G
    // TODO switch

    node_t* root = getNode("B");

    if (tok.tokenStr == "#" || tok.tokenStr == "!") {
        root->children.push_back( C() );
    }

    else if (tok.tokenStr == "$") {
        root->children.push_back( D() );
    }

    else if (tok.tokenStr == "'") {
        root->children.push_back( E() );
    }

    else if (tok.tokenID == t2_tk) {
        root->children.push_back( G() );
    }
    else {
        // since FIRST(S) contains empty, we use it for all other option
        root->children.push_back( S() );
    }

    return root;
}

node_t* C() {
    // C -> # t2 | ! F

    node_t* root = getNode("C");

    if (tok.tokenStr == "#") {
        root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), tok.tokenStr ) );
        tok = scanner(*filePtr, lineNum);
        printToken(tok);
        if (tok.tokenID == t2_tk) {
            root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), tok.tokenStr ) );
            tok = scanner(*filePtr, lineNum);
            printToken(tok);
        }
        else {
            parserError();
        }
    }
    else if (tok.tokenStr == "!") {
        root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), tok.tokenStr ) );
        tok = scanner(*filePtr, lineNum);
        printToken(tok);
        root->children.push_back( F() );
    }
    else {
        parserError();
    }

    return root;
}

node_t* D() {
    // D -> $ F

    node_t* root = getNode("D");

    if (tok.tokenStr == "$") {
        root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), tok.tokenStr ) );
        tok = scanner(*filePtr, lineNum);
        printToken(tok);
        root->children.push_back( F() );
    }
    else {
        parserError();
    }

    return root;
}

node_t* E() {
    // E -> ' F F F B

    node_t* root = getNode("E");

    if (tok.tokenStr == "\'") {
        root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), tok.tokenStr ) );
        tok = scanner(*filePtr, lineNum);
        printToken(tok);
        root->children.push_back( F() );
        root->children.push_back( F() );
        root->children.push_back( F() );
        root->children.push_back( B() );
    }
    else {
        parserError();
    }

    return root;
}

node_t* F() {
    // F -> t2 | t3 | & F F

    node_t* root = getNode("F");

    if (tok.tokenID == t2_tk) {
        root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), tok.tokenStr ) );
        tok = scanner(*filePtr, lineNum);
        printToken(tok);
    }
    else if (tok.tokenID == t3_tk) {
        root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), tok.tokenStr ) );
        tok = scanner(*filePtr, lineNum);
        printToken(tok);
    }
    else if (tok.tokenStr == "&") {
        root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), tok.tokenStr ) );
        tok = scanner(*filePtr, lineNum);
        printToken(tok);
        root->children.push_back( F() );
        root->children.push_back( F() );
    }
    else {
        parserError();
    }

    return root;
}

node_t* G() {
    // G -> t2 % F

    node_t* root = getNode("G");

    if (tok.tokenID == t2_tk) {
        root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), tok.tokenStr ) );
        tok = scanner(*filePtr, lineNum);
        printToken(tok);
        if (tok.tokenStr == "%") {
            root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), tok.tokenStr ) );
            tok = scanner(*filePtr, lineNum);
            printToken(tok);
            root->children.push_back( F() );
        }
        else {
            parserError();
        }
    }
    else {
        parserError();
    }

    return root;
}
