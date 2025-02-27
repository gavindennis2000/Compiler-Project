/*
    token.h 

    declares the token struct and its members
*/

#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>

typedef struct token {
    // token data type with specific ID, string instance, and the
    // associated line number
    
    enum tokenType {
        /* different possible tokens:
            t1_tk - symbols; specifically ascii numbers 33 to 41
            t2_tk - begin with a single '+', followed by one or more digits (e.g. +34)
            t3_tk - begin with a single letter followed by one or more digits (e.g. z421)
        */
       t1_tk, t2_tk, t3_tk
    };

    tokenType tokenID;
    std::string tokenInstance;  // lexeme associated with the token
    int lineNum;  // line number where the token is found
};

#endif