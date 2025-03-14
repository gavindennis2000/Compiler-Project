#include "scanner.h"  // header file
#include "token.h"  // for token structure
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
    int state;
    int nextState;

    do {
        // keep finding the next state using the driver table


    } while (state < 1000);

    newToken.tokenID = t1_tk;
    newToken.tokenStr = "+12345";

    return newToken;
}