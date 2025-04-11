/*
    main.cpp

    P1
    CS 4280-001
    Gavin Dennis 

    Documentation/resources, test cases, and compilation instructions can be found
    in the README.

    Gets input from the user from either the command-line or cin, provides input to 
    testScanner, which iteratively calls scanner for one token at a time
    until eof, while printing out each token.

*/

#include "parser.h"  // for token parser
#include "node.h"  // for parse tree nodes
#include "testTree.h"  // for parse tree printing and deleting
#include <string>  // for getline functionality
#include <fstream>  // for file handling
#include <stdio.h>  // for remove function from c library (used to remove temp input file)
#include <cstdlib>  // for premature termination with exit
#include <vector>  // used for file filter function
#include <iostream>

void filter(std::ifstream& file, std::ofstream& filteredFile) {
    // filters file contents for comments before passing to parser

    // make sure files are open and ready
    if (!file.is_open() || !filteredFile.is_open()) {
        std::cerr << "ERROR: Unable to open file for filtering. \nTerminating program.\n";
        exit(EXIT_FAILURE);
    }

    // add contents of original file to vector
    std::string line;
    std::vector<std::string> fileLines;
    while (std::getline(file, line)) {
        fileLines.push_back(line);
    }

    // filter each line in vector
    bool commentFlag = false;
    for (int i = 0; i < fileLines.size(); i++) {
        for (int j = 0; j < fileLines[i].length(); j++) {
            // switch the comment flag when reaching an asterisk
            if (fileLines[i][j] == '*') {
                commentFlag = (!commentFlag);
                fileLines[i][j] = ' ';
                continue;
            }
            if (commentFlag) {
                // if comment flag is checked, replace any char with a space
                fileLines[i][j] = ' ';
            }
        }
        // add filtered line to filteredFile
        filteredFile << fileLines[i];
    }

    // finished filtering
    return;
}

int main(int argc, char* argv[]) {
    /* either takes in text file as argument, or reads user input into a file.
    Then passes it to testScanner which provides one token at a time to scanner. */

    std::string filename;  // stores name of inputted file
    std::string tempFilename = "tempUserInput.txt";  // filename for temp file that stores user input.
    std::ofstream temp;  // temp file that stores user input. deleted after the tree is built

    // introductory message to user
    std::cout   << "* * * * * * * * * * * * * * * * * * * * \n"
                << "P2 \n"
                << "\n" 
                << "Takes in one file as an argument, otherwise it reads in user input from keyboard. \n"
                << "Filters file contents, then passes to parser which determines if tokens returned by \n"
                << "scanner match BNF Grammer. If tokens match, parser prints out a parse tree.\n\n";

    // check for command line arguments
    int numberOfFiles = argc - 1;
    if (numberOfFiles > 1) {
        // there can only be one file provided as an arument.
        // when there is more than one argument provided, display an error message and halt

        std::cerr   << "ERROR: invalid number of arguments." 
                    << "\nInput should be provided from one file as a command-line argument, or from the keyboard with no arguments."
                    << "\nExiting program." << std::endl;
        return 1;
    }

    else if (numberOfFiles == 1) {
        // if there's exactly one file, store it's name

        filename = argv[1];
    }

    else {
        // if there's no command line argument, read from the keyboard until 'ctr + d' is pressed 
        // and set the filename as 'out'

        // message to user
        std::cout   << "Reading user input. The program will read until the user enters \"ctr + d\" (\"ctr + z\" on Windows). \n\n"
                    << "Reading user input:\n";
        
        // open and write to the temporary file
        temp.open(tempFilename, std::fstream::trunc);  
        filename = tempFilename;
        std::string line;  // string that holds getline input
        while (std::getline(std::cin, line)) {
            // take each line of user input and write it to the temp file

            temp << line;
        }
        temp.close();
    }

    // attempt to open the target file and validate its contents
    std::ifstream file(filename);
    if (!file.is_open()) {
        // give an error message if file doesn't exist or just won't open

        std::cerr << "ERROR: File: \"" << filename << "\" won't open or doesn't exist. \n" << "Exiting program. \n";
        return 1;
    }

    // filter file before giving it to scanner
    std::string filterFilename = filename + ".filter";
    std::ofstream filteredFileWrite(filterFilename, std::fstream::out);
    filter(file, filteredFileWrite);

    // close the open files, then open a read-only version to pass to scanner
    file.close();
    filteredFileWrite.close();
    std::ifstream filteredFileRead(filterFilename);

    // provide the file to parser and let it handle the rest
    // parser returns parse tree if successful
    node_t* parseTreeRoot = parser(filteredFileRead);

    // print the parse tree, then delete it
    printTree(parseTreeRoot);
    destroyTree(parseTreeRoot);

    // the scanner is finished; close the read-only filtered file
    filteredFileRead.close();

    // remove the temp user input file if it exists as well as the filtered file
    std::remove(tempFilename.c_str());
    std::remove(filterFilename.c_str());

    // the program is finished!
    std::cout << "\n" << "Program is finished.\n";
    return 0;
}