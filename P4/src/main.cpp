/*
    main.cpp

    P4
    CS 4280-001
    Gavin Dennis 

    Documentation/resources, test cases, and compilation instructions can be found
    in the README.

    Gets input from the user from either the command-line or cin. Filters input, then
    passes to parser. Parser repeatedly calls scanner while ensuring that BNF grammar
    is followed. parsed tokens are converted into nodes in a parse tree structure.
    After a successful parse, the tree root is returned to main where the static
    semantics are verified. Throughout semantic verification, assembly code will be
    written and added to the file [inputFile].asm, where inputFile is the name of
    the argument provided to the program (out.asm if user input from cin is used).
    The generated code should run on the UMSL ASM Interpreter.

*/

#include "parser.h"  // for token parser
#include "node.h"  // for parse tree nodes
#include "testTree.h"  // for parse tree printing and deleting
#include "staticSemantics.h"  // for check static semantics of parse tree
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
    for (long long unsigned int i = 0; i < fileLines.size(); i++) {
        for (long long unsigned int j = 0; j < fileLines[i].length(); j++) {
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
        if (i != fileLines.size() - 1) {
            // add new lines until the end of file
            filteredFile << "\n";
        }
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
    std::ofstream assemblyCode;  // file where assembly code is outputted
    std::string assemblyFilename = "out.asm";  // filename of assembly file object

    // introductory message to user
    std::cout   << "* * * * * * * * * * * * * * * * * * * * \n"
                << "P4 \n"
                << "\n" 
                << "Takes in one file as an argument, otherwise it reads in user input from keyboard. \n"
                << "Filters file contents, then passes to parser which determines if tokens returned by \n"
                << "scanner match BNF Grammer. If tokens match, a parse tree is returned, then the static \n"
                << "semantics of the program are verified while tokens are converted into assembly code. \n"
                << "If the semantics are successfully verified, the code will be outputted to a file called \n"
                << "[inputfile].asm, where \"inputfile\" is the name of the file passed as argument (out.asm for cin). \n";

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
        assemblyFilename = filename + ".asm";
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
    std::cout << "\n" << "Parsing input...\n";
    node_t* parseTreeRoot = parser(filteredFileRead, filterFilename);

    // open the file for assembly code output
    assemblyCode.open(assemblyFilename, std::fstream::out);

    // call static semantic function on tree
    // this is where the assembly code will be generated
    std::cout << "\n" << "Checking static semantics...\n";
    if (checkStaticSemantics(parseTreeRoot, assemblyCode)) {
        // add the process table to the end of the assembly code file
        std::cout << "Static semantics verified! \n";
        printSymbolTable(assemblyCode);
    }

    // tell the user where their new assembly code
    std::cout << "\n" << "Assembly code outputted to \"" << assemblyFilename << "\".\n";

    // delete the parse tree
    destroyTree(parseTreeRoot);

    // the scanner is finished; close the read-only filtered file
    // and the assembly code file
    filteredFileRead.close();
    assemblyCode.close();

    // remove the temp user input file if it exists as well as the filtered file
    std::remove(tempFilename.c_str());
    std::remove(filterFilename.c_str());

    // the program is finished!
    std::cout << "\n" << "Program is finished.\n";
    return 0;
}