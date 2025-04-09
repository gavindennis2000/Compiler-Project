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

void parser(std::string& stringArg, int lineNumber, bool eofReached = false) {
    /* this is ran when end of line is reached or the comment flag is set.
    checks if stringArg has contents, and if it does, feeds it to the scanner
    iteratively until it is empty. */

    while (stringArg != "" || eofReached) {
        /* before setting the comment flag, if there is a string in the buffer, 
        give it to the scanner before setting the flag and continuing */

        // set the lookahead to the beginning of the string and make sure it's not a space
        char lookahead = stringArg[0];
        while (lookahead == ' ') {
            stringArg.erase(0, 1);
            lookahead = stringArg[0];
            
        }

        // make sure empty lookaheads due to comments don't trigger an
        // eof token
        if (lookahead == '\0' && !eofReached) {
            break;
        }
        // run the scanner
        token tok = FADriver(stringArg, lineNumber, lookahead);
        
        // print the returned token
        printToken(tok);

        // if eof reached, break out of while loop
        if (tok.tokenID == EOF_tk) {
            break;
        }

        // remove token string from stringArg, then loop the scanner til stringArg is empty
        stringArg.erase(0, tok.tokenStr.length());
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
void S() {

}

void A() {

}

void B() {

}

void C() {

}

void D() {

}

void E() {

}

void F() {

}

void G() {

}
