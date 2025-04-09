/*
    parser.cpp

    Filters input file from main and passes it line by line
    to scanner, then prints out the returned token.
*/

#include "parser.h"  // header file
#include "scanner.h"  // for getToken
#include "token.h"  // for token struct
#include <string>  // for getline
#include <fstream>  // for file handling
#include <iostream> 

token tok;  // global token variable to be accessed by all functions
int lineNum = 0;

void parser(std::ifstream& filteredFile) {
    /* */

    tok = scanner(filteredFile, lineNum);
    S(filteredFile);

    if (tok.tokenID == EOF_tk) {
        std::cout <<"you win";
    }
    else {
        std::cout << "error\n";
    }
    return;
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

// functions for BNF
void S(std::ifstream& filteredFile) {
    // FIRST(S) == { " ( empty }
    printToken(tok);

    // predicts S -> A ( B B )
    if (tok.tokenStr == "\"" || tok.tokenStr == "(") {
        tok = scanner(filteredFile, lineNum);
        A(filteredFile);
        return;
    }
}

void A(std::ifstream& filteredFile) {

}

void B(std::ifstream& filteredFile) {

}

void C(std::ifstream& filteredFile) {

}

void D(std::ifstream& filteredFile) {

}

void E(std::ifstream& filteredFile) {

}

void F(std::ifstream& filteredFile) {

}

void G(std::ifstream& filteredFile) {

}
