/*
    parser.cpp

    Filters input file from main and passes it line by line
    to scanner, then prints out the returned token.
*/

#include "parser.h"  // header file
#include "scanner.h"  // for scanning tokens
#include "token.h"  // for token struct
#include <string>  // for getline
#include <fstream>  // for file handling
#include <iostream> 

token tok;  // global token variable to be accessed by all functions
int lineNum = 0;

void parser(std::ifstream& filteredFile) {
    /* */

    tok = scanner(filteredFile, lineNum);
    printToken(tok);
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

void parserError() {
    /* prints an error message with token and line number when parsed token
    doesn't match BNF grammar */

    std::cout << "PARSING ERROR: " << tok.tokenStr << " " << lineNum << ".\n Terminating.\n";
    exit(EXIT_FAILURE);
}

// functions for BNF
void S(std::ifstream& filteredFile) {
    // S -> A ( B B )

    // A
    A(filteredFile);

    // (
    if (tok.tokenStr == "(") {
        tok = scanner(filteredFile, lineNum);
        printToken(tok);
    }
    else {
        parserError();
    }

    // Two B's
    B(filteredFile);
    B(filteredFile);

    // )
    if (tok.tokenStr == ")") {
        tok = scanner(filteredFile, lineNum);
        printToken(tok);
    }
    else {
        parserError();
    }

    // parser is finished
    return;
}

void A(std::ifstream& filteredFile) {
    // A -> " t2 | empty

    if (tok.tokenStr == "\"") {
        tok = scanner(filteredFile, lineNum);
        printToken(tok);
        if (tok.tokenID == t2_tk) {
            tok = scanner(filteredFile, lineNum);
            printToken(tok);
            return;
        }
        else {
            parserError();
        }
    }
    else {
        return;
    }
}

void B(std::ifstream& filteredFile) {
    // B -> S | C | D | E | G

    if (tok.tokenStr == "#" || tok.tokenStr == "!") {
        C(filteredFile);
        return;
    }

    else if (tok.tokenStr == "$") {
        D(filteredFile);
        return;
    }

    else if (tok.tokenStr == "'") {
        E(filteredFile);
        return;
    }

    else if (tok.tokenID == t2_tk) {
        G(filteredFile);
        return;
    }
    else {
        // since FIRST(S) contains empty, we use it for all other option
        S(filteredFile);
        return;
    }
}

void C(std::ifstream& filteredFile) {
    // C -> # t2 | ! F

    if (tok.tokenStr == "#") {
        tok = scanner(filteredFile, lineNum);
        printToken(tok);
        if (tok.tokenID == t2_tk) {
            tok = scanner(filteredFile, lineNum);
            printToken(tok);
            return;
        }
        else {
            parserError();
        }
    }
    else if (tok.tokenStr == "!") {
        tok = scanner(filteredFile, lineNum);
        printToken(tok);
        F(filteredFile);
        return;
    }
    else {
        parserError();
    }
}

void D(std::ifstream& filteredFile) {
    // D -> $ F
    if (tok.tokenStr == "$") {
        tok = scanner(filteredFile, lineNum);
        printToken(tok);
        F(filteredFile);
        return;
    }
    else {
        parserError();
    }
}

void E(std::ifstream& filteredFile) {
    // E -> ' F F F B

    if (tok.tokenStr == "\'") {
        tok = scanner(filteredFile, lineNum);
        printToken(tok);
        F(filteredFile);
        F(filteredFile);
        F(filteredFile);
        B(filteredFile);
        return;
    }
    else {
        parserError();
    }
}

void F(std::ifstream& filteredFile) {
    // F -> t2 | t3 | & F F

    if (tok.tokenID == t2_tk) {
        tok = scanner(filteredFile, lineNum);
        printToken(tok);
        return;
    }
    else if (tok.tokenID == t3_tk) {
        tok = scanner(filteredFile, lineNum);
        printToken(tok);
        return;
    }
    else if (tok.tokenStr == "&") {
        tok = scanner(filteredFile, lineNum);
        printToken(tok);
        F(filteredFile);
        F(filteredFile);
        return;
    }
    else {
        parserError();
    }
}

void G(std::ifstream& filteredFile) {
    // G -> t2 % F
    if (tok.tokenID == t2_tk) {
        tok = scanner(filteredFile, lineNum);
        printToken(tok);
        if (tok.tokenStr == "%") {
            tok = scanner(filteredFile, lineNum);
            printToken(tok);
            F(filteredFile);
        }
        else {
            parserError();
        }
    }
    else {
        parserError();
    }
}
