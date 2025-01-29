/*
Gavin Dennis CS 4280-001 Project 0

main file

started on 2025/01/29
finished on tbd #todo

resources consulted:
    w3 schools
        std::cerr documentation: https://www.w3schools.com/cpp/ref_iostream_cerr.asp
    geeksforgeeks.org
        makefile documentation: https://www.geeksforgeeks.org/makefile-in-c-and-its-applications/
          
*/

#include "buildTree.h"  // for binary tree and node structs and methods
#include <iostream>  // for standard input and output streams
#include <fstream>  // for file handling

int main(int argc, char* argv[]) {
    /**/

    int numberOfFiles = argc - 1;
    if (numberOfFiles > 1) {
        // when there is more than one argument provided, display an error message and exit
        std::cerr   << "ERROR: invalid number of arguments." 
            << "\nInput should be provided from one file as a commandline argument, or from the keyboard with no arguments."
            << "Exiting program." << std::endl;
        return 1;
    }

    // the program is finished!
    return 0;
}