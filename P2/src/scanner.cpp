/*
    scanner.cpp 

    Uses driver table to determine tokenID, token string,
    and line number for an inputted string.

    Throws an error and halts for invalid tokens.
*/

#include "scanner.h"  // header file
#include "token.h"  // for token structure
#include <cstdlib>  // for premature termination
#include <fstream>  // for file handling functions
#include <iostream> 

// driver table
const int TABLE_COLUMNS = 6;
const int TABLE_ROWS = 6;
int driverTable[TABLE_COLUMNS][TABLE_ROWS] = {
/*        ws     ascii   letter  digit   eof     +   */
/* q0 */ {0,     5,      1,      -1,     1004,   3   },
/* q1 */ {-2,    -2,     -2,     2,      -2,     -2  },
/* q2 */ {1003,  1003,   1003,   2,      1003,   1003},
/* q3 */ {-3,    -3,     -3,     4,      -3,     -3  },
/* q4 */ {1002,  1002,   1002,   4,      1002,   1002},
/* q5 */ {1001,  1001,   1001,   1001,   1001,   1001}

/* t1 = 1001, t2 = 1002, t3 = 1003, eofTK = 1004 */
};

token scanner(std::ifstream& filteredFile, int& lineNum) {
    /* takes in string, line number, and lookahead from tester
    as arguments. uses driver table to return the correct token
    for the file string. */

    // create a new token with null values
    token newToken;
    newToken.tokenStr = "";
    newToken.lineNum = lineNum;
    int state = 0, nextState;

    // set the initial lookahead
    char lookahead = filteredFile.peek();

    // remove all white spaces at the beginning
    if (lookahead == '\n') {
        std::cout << "wow\n";
    }
    while (lookahead == ' ' || lookahead == '\n') {
        // TODO
        if (lookahead == '\n') {
            lineNum++;
            std::cout << "line num++\n";
            // discard the current whitespace, then set the next lookahead
            filteredFile.ignore(1);  
            lookahead = filteredFile.peek();
            continue;
        }
        else if (lookahead == ' ') {
            // discard the current whitespace, then set the next lookahead
            filteredFile.ignore(1);
            lookahead = filteredFile.peek();
            continue;
        }
    }

    // keep parsing characters until a token is formed
    while (state >= 0 && state <= 1000) {

        // // if EOF is reached, return an EOF token
        // if (filteredFile.eof()) {
        //     state = 1004;
        // }

        // keep finding the next state using the driver table
        int lookaheadValue = getLookaheadValue(lookahead);
        nextState = driverTable[state][lookaheadValue];

        if (nextState < 0) {
            // if next state is an error, return the invalid token with a message
            newToken.tokenStr += lookahead;
            invalidTokenError(newToken.tokenStr, lineNum);
        }
        else if (nextState > 1000) {
            // if final state is reached, set its ID

            state = nextState; 
            switch (state) {
                case 1001:
                    newToken.tokenID = t1_tk;
                    break;
                case 1002:
                    newToken.tokenID = t2_tk;
                    break;
                case 1003:
                    newToken.tokenID = t3_tk;
                    break;
                case 1004:
                    newToken.tokenID = EOF_tk;
                    break;
            }
        }
        else {
            // if state isn't final, append the character to the token string
            // then update the lookahead
            state = nextState;
            newToken.tokenStr += filteredFile.get();
            lookahead = filteredFile.peek();
        }
    }

    return newToken;
}

int getLookaheadValue(char lookahead) {
    /* gets driver column associated with lookahead character */

    // whitespace
    if (lookahead == ' ') {
        return 0;
    }

    // symbols
    else if (lookahead >= 33 && lookahead <= 41) {
        return 1;
    }

    // letters
    else if ( (lookahead >= 'a' && lookahead <= 'z') || (lookahead >= 'A' && lookahead <= 'Z') ) {
        return 2;
    }

    // digits
    else if (lookahead >= 48 && lookahead <= 57) {
        return 3;
    }

    // +
    else if (lookahead == '+') {
        return 5;
    }

    // eof
    return 4;
}

void invalidTokenError(std::string tokenStr, int lineNum) {
    /* displays a message when handling an invalid token then halts */

    std::cout << "SCANNER ERROR:" << "\t" << tokenStr << "\t" << lineNum << "\n" << "Terminating program.\n";
    exit(EXIT_FAILURE);
}