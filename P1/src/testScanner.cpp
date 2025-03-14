/*
    testScanner.cpp

    iteratively calls scanner() and prints the returned token
    until EOF
*/

#include "testScanner.h"  // header file
#include "scanner.h"  // for getToken
#include "token.h"  // for token struct
#include <string>  // for getline
#include <fstream>  // for file handling
#include <iostream> 

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
        default:
            // invalid tokens
            tokenTypeStr = "invalid"; 
            break;
    }
    if (tokenTypeStr == "invalid") {
        std::cout << "\nInvalid token\n";
    }
    std::cout << tokenTypeStr << "\t" << tok.tokenStr << "\t" << tok.lineNum << "\n";

    return;
}

void scanIfReady(std::string& stringArg, int lineNumber) {
    /* this is ran when end of line is reached or the comment flag is set.
    checks if stringArg has contents, and if it does, feeds it to the scanner
    iteratively until it is empty. */

    while (stringArg != "") {
        /* before setting the comment flag, if there is a string in the buffer, 
        give it to the scanner before setting the flag and continuing */

        char lookahead = stringArg[0];
        std::cout << "\nString sent: " << stringArg << "\n";
        token tok = FADriver(stringArg, lineNumber, lookahead);
        
        // print the returned token
        printToken(tok);

        // remove token string from stringArg, then loop the scanner til stringArg is empty
        int amountToErase = tok.tokenStr.length();
        // if lookahead was a space, the amount to erase will be one greater than expected
        if (lookahead == ' ') { amountToErase++; }
        stringArg.erase(0, tok.tokenStr.length());
        std::cout << "\nAfter erasure: " << stringArg << "\n";
    }

    return;
}

void testScanner(std::ifstream& file) {
    /* filters text from file (counts lines, skips spaces and comments), then repeatedly provides it to
    scanner, printing out each token, until end-of-file token is returned */

    token tok;  // token to be returned by the scanner
    std::string stringArg = "";  // string provided to scanner; filtered from input file
    int lineNumber = 1;  // line number provided to scanner, kept track of by this tester
    bool commentFlag = false;  // flag that checks if current character is part of a comment
    char lookahead;  // lookahead is set before string is sent to scanner

    // filter the file before you pass it to scanner
    std::string line;
    while (std::getline(file, line)) {

        for (int i = 0; i < line.length(); i++) {
            // parse every character in line to remove extra white spaces and comments

            char ch = line[i];
            char nextChar = (i < line.length() - 1) ? line[i + 1] : '\n';  // nextchar is either the next char or a new line if the line is over

            // check for comments
            if (ch == '*') {
                // if there is contents in stringArg, process the tokens before setting the flag
                scanIfReady(stringArg, lineNumber);

                // if commentFlag is up, put it down; otherwise put it up
                commentFlag = (commentFlag) ? false : true;
                continue;
            }

            // with new lines, run the scanner if stringArg has contents, increment the line number, then remove the new line
            if (ch == '\n') {
                scanIfReady(stringArg, lineNumber);
                lineNumber++;
                continue;
            }

            // if commentFlag is up, keep continuing until the end of the comment
            if (commentFlag) { continue; }

            // remove all extra spaces
            if (ch == ' ' && nextChar == ' ') { continue; }

            // append all other characters to stringArg
            stringArg += ch;
        }

        // run the scanner after every line if stringArg has contents
        scanIfReady(stringArg, lineNumber);
    }

    // run the scanner one more time for the empty file token
    scanIfReady(stringArg, lineNumber);
}