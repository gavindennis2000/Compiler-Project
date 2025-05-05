/*
    parser.cpp

    Repeatedly passes input to scanner, ensuring that returned tokens match
    BNF. If a token doesn't match, Tokens that match are converted into nodes
    of a parse tree structure. After a successful parse, the root node is 
    returned to Main.
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
std::string filePtrName;  // name of filter file. used for remove temporary file after parser error
int lineNum = 1;  // starting line number
node_t* root;  // root node for parse tree

node_t* parser(std::ifstream& filteredFile, std::string filterFilename) {
    /* begins calls to scanner and passes off to the nonterminal 
    functions starting with S */

    // set the global file variable to the filteredFile passed by main
    filePtr = &filteredFile;
    filePtrName = filterFilename;

    tok = scanner(*filePtr, lineNum);
    root = S(0);

    if (tok.tokenID == EOF_tk) {
        std::cout << "Parse successful! \n";
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

void parserError() {
    /* prints an error message with token and line number when parsed token
    doesn't match BNF grammar */

    std::cout << "PARSER ERROR:\t" << tok.tokenStr << "\t" << lineNum << ".\nTerminating program. \n";

    // free pointer memory if root points to something
    if (root != nullptr) {
        destroyTree(root);
    }

    // close the filtered file and delete it
    if (filePtr->is_open()) {
        filePtr->close();
        std::remove(filePtrName.c_str());
    }

    // terminate program
    exit(EXIT_FAILURE);
}

// functions for BNF nonterminals
node_t* S(int level) {
    // S -> A ( B B )
    // returns root after everything is called and returned

    node_t* root = getNode("S", level);

    // A
    root->children.push_back( A(level + 1) );

    // ( t1 token
    if (tok.tokenStr == "(") {
        root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), level + 1, tok.tokenStr ) );
        tok = scanner(*filePtr, lineNum);
    }
    else {
        parserError();
    }

    // first B
    root->children.push_back( B(level + 1) );

    // second B
    root->children.push_back( B(level + 1) );

    // ) t1 token
    if (tok.tokenStr == ")") {
        root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), level + 1, tok.tokenStr ) );
        tok = scanner(*filePtr, lineNum);
    }
    else {
        parserError();
    }

    // parser is finished
    return root;
}

node_t* A(int level) {
    // A -> " t2 | empty

    node_t* root = getNode("A", level);

    // match token to a possible outcome
    if (tok.tokenStr == "\"") {
        root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), level + 1, tok.tokenStr ) );
        tok = scanner(*filePtr, lineNum);
        if (tok.tokenID == t2_tk) {
            root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), level + 1, tok.tokenStr ) );
            tok = scanner(*filePtr, lineNum);
        }
        else {
            parserError();
        }
    }
    else {
        // since A is nullable, epsilon will be returned if there's no token to consume
        root->children.push_back( getNode("empty", level + 1) );
    }

    return root;
}

node_t* B(int level) {
    // B -> S | C | D | E | G

    /* B itself doesn't consume any tokens. instead,
    it resolves to another nonterminal */

    node_t* root = getNode("B", level);

    // match token to a possible outcome
    if (tok.tokenStr == "#" || tok.tokenStr == "!") {
        root->children.push_back( C(level + 1) );
    }

    else if (tok.tokenStr == "$") {
        root->children.push_back( D(level + 1) );
    }

    else if (tok.tokenStr == "'") {
        root->children.push_back( E(level + 1) );
    }

    else if (tok.tokenID == t2_tk) {
        root->children.push_back( G(level + 1) );
    }
    else {
        // since FIRST(S) contains empty, we use it for all other options
        root->children.push_back( S(level + 1) );
    }

    return root;
}

node_t* C(int level) {
    // C -> # t2 | ! F

    node_t* root = getNode("C", level);

    // match token to a possible outcome, otherwise throw an error
    if (tok.tokenStr == "#") {
        root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), level + 1, tok.tokenStr ) );
        tok = scanner(*filePtr, lineNum);
        if (tok.tokenID == t2_tk) {
            root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), level + 1, tok.tokenStr ) );
            tok = scanner(*filePtr, lineNum);
        }
        else {
            parserError();
        }
    }
    else if (tok.tokenStr == "!") {
        root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), level + 1, tok.tokenStr ) );
        tok = scanner(*filePtr, lineNum);
        root->children.push_back( F(level + 1) );
    }
    else {
        parserError();
    }

    return root;
}

node_t* D(int level) {
    // D -> $ F

    node_t* root = getNode("D", level);

    // match token to a possible outcome, otherwise throw an error
    if (tok.tokenStr == "$") {
        root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), level + 1, tok.tokenStr ) );
        tok = scanner(*filePtr, lineNum);
        root->children.push_back( F(level + 1) );
    }
    else {
        parserError();
    }

    return root;
}

node_t* E(int level) {
    // E -> ' F F F B

    node_t* root = getNode("E", level);

    // match token to a possible outcome, otherwise throw an error
    if (tok.tokenStr == "\'") {
        root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), level + 1, tok.tokenStr ) );
        tok = scanner(*filePtr, lineNum);
        root->children.push_back( F(level + 1) );
        root->children.push_back( F(level + 1) );
        root->children.push_back( F(level + 1) );
        root->children.push_back( B(level + 1) );
    }
    else {
        parserError();
    }

    return root;
}

node_t* F(int level) {
    // F -> t2 | t3 | & F F

    node_t* root = getNode("F", level);

    // match token to a possible outcome, otherwise throw an error
    if (tok.tokenID == t2_tk) {
        root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), level + 1, tok.tokenStr ) );
        tok = scanner(*filePtr, lineNum);
    }
    else if (tok.tokenID == t3_tk) {
        root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), level + 1, tok.tokenStr ) );
        tok = scanner(*filePtr, lineNum);
    }
    else if (tok.tokenStr == "&") {
        root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), level + 1, tok.tokenStr ) );
        tok = scanner(*filePtr, lineNum);
        root->children.push_back( F(level + 1) );
        root->children.push_back( F(level + 1) );
    }
    else {
        parserError();
    }

    return root;
}

node_t* G(int level) {
    // G -> t2 % F

    node_t* root = getNode("G", level);

    // match token to a possible outcome, otherwise throw an error
    if (tok.tokenID == t2_tk) {
        root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), level + 1, tok.tokenStr ) );
        tok = scanner(*filePtr, lineNum);
        if (tok.tokenStr == "%") {
            root->children.push_back( getNode( getLabelFromEnum(tok.tokenID), level + 1, tok.tokenStr ) );
            tok = scanner(*filePtr, lineNum);
            root->children.push_back( F(level + 1) );
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