/*
    scanner.cpp 

    Uses driver table to determine tokenID, token string,
    and line number for an inputted string.

    Throws an error and halts for invalid tokens.
*/

#include "scanner.h"  // header file
#include "token.h"  // for token structure
#include <cstdlib>  // for premature termination
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

token FADriver(std::string fileStr, int lineNum, char lookahead) {
    /* takes in string, line number, and lookahead from tester
    as arguments. uses driver table to return the correct token
    for the file string. */

    // create a new token with null values
    token newToken;
    newToken.tokenStr = "";
    newToken.lineNum = lineNum;
    int state = 0, nextState;

    // set the lookahead index
    int lookaheadIndex = 0;
    lookahead = fileStr[lookaheadIndex];

    do {
        // keep finding the next state using the driver table
        int lookaheadValue = getLookaheadValue(lookahead);
        nextState = driverTable[state][lookaheadValue];

        if (nextState < 0) {
            // if next state is an error, return invalid
            invalidTokenError(fileStr, lineNum);
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
                default:
                    newToken.tokenID = EOF_tk;
                    break;
            }
        }
        else {
            // if state isn't final, append the character to the token string
            // then increment the lookahead
            state = nextState;
            newToken.tokenStr += lookahead;
            lookahead = fileStr[++lookaheadIndex];
        }
    } while (state >= 0 && state <= 1000);

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

void invalidTokenError(std::string fileStr, int lineNum) {
    /* displays a message when handling an invalid token then halts */

    std::cout << "SCANNER ERROR:" << "\t" << fileStr << "\t" << lineNum << "\n" << "Terminating program.\n";
    exit(EXIT_FAILURE);
}