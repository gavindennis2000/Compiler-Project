/*
    token.h 

    Declares the token struct and its members:
        token ID,
        string associated with token,
        token's line number
*/

#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>

enum tokenType {
    /* different possible tokens:
        t1_tk - symbols; specifically ascii numbers 33 to 41
        t2_tk - begin with a single '+', followed by one or more digits (e.g. +34)
        t3_tk - begin with a single letter followed by one or more digits (e.g. z421)
        EOF_tk - signifies end of file
        invalid - signifies that lookahead is an invalid character
    */
   t1_tk, t2_tk, t3_tk, EOF_tk
};

struct token {
    // token data type with specific ID, string instance, and the
    // associated line number

    tokenType tokenID;  // each token can be one of the enum types listed above
    std::string tokenStr;  // lexeme associated with the token
    int lineNum;  // line number where the token is found
};

#endif