/*
    main.cpp

    Project 0
    CS 4280-001
    Gavin Dennis 

    gets input from the user from either the command-line or cin; validates input;
    uses the input to create a binary tree structure; then traverses the tree using
    pre-order, post-order, and level-order traversal while outputting the results into
    the three respective files.

    resources consulted:
        w3 schools
            std::cerr documentation: https://www.w3schools.com/cpp/ref_iostream_cerr.asp
        geeksforgeeks.org
            makefile documentation: https://www.geeksforgeeks.org/makefile-in-c-and-its-applications/
        cplusplus.com
            std::remove documentation: https://cplusplus.com/reference/cstdio/remove/
            cctype documentation: https://cplusplus.com/reference/cctype/
*/

#include "buildTree.h"  // for binary tree and node structs and methods
#include "node.h"  // for root node which is returned by buildTree
#include "traversals.h"  // for binary tree traversals
#include <iostream>  // for standard input and output streams
#include <fstream>  // for file handling
#include <cctype>  // for input character validation
#include <stdio.h>  // for remove function from c library (used to remove temp input file)

// function prototypes (there's just one)
bool isValidInput(std::ifstream&);  // used to validate input file before creating the binary tree

// main program
int main(int argc, char* argv[]) {
    /* either takes in text file as argument, or reads user input into a file.
    Then validates it, builds a binary tree from the data, and finally traverses it
    while writing the results to three files:
        filename.levelorder - the binary tree traversed using a breadth-first level-order traversal algorithm
        filename.preorder - the binary tree traversed using a depth-first pre-order traversal algorithm
        filename.postorder - the binary tree traversed using a depth-first post-order traversal algorithm
    where 'filename' is either the name of the inputted file, or 'out' when using std::cin  */

    std::string filename;  // stores name of inputted file
    // note: this is not the ideal way to create a temporary file in c++, but for this use case it is simpler and was okayed by the instructor
    char* tempFilename = "temp1234abcd.txt";  // filename for temp file that stores user input.
    std::ofstream temp;  // temp file that stores user input. deleted after the tree is built
    bool changeOutFilename = false;  // if flagged, will change the output file names to "out.xxxorder"

    // intro message to user
    std::cout   << "* * * * * * * * * * * * * * * * * * * * \n"
                << "Project 0 \n"
                << "\n" 
                << "Takes in one file as an argument, otherwise it reads in user input from keyboard.\n\n";

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
        changeOutFilename = true;  // flag to make sure filename is changed to "out.xxxorder"
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

    // time to validate
    std::cout << "Validating user input... \n";
    if (!isValidInput(file)) {
        // validate the input file's characters and halt if there's an unnacceptable symbol

        std::cerr << "ERROR: File validation unsuccessful. \n" << "Exiting program.\n";
        return 1;
    }
    else {
        // we're dandy. moving on

        std::cout << "Successfully validated! \n";
    }

    // if file is valid, create the binary tree using data read from the input file
    std::cout << "Creating binary tree with data from \"" << filename << "\".\n";
    node_t* root = buildTree(file);
    file.close();  // close the opened file

    if (changeOutFilename) {
        // delete the temporary user input file and rename the output files

        if (std::remove(tempFilename) == 0) {
            std::cout << "Temporary file successfully deleted.\n";
        }
        else {
            std::cerr << "ERROR: Could not delete temporary input file. \n" << "Exiting program.\n";
        }
        filename = "out";
    }

    // verify that the tree has been created
    if (root == nullptr) {
        // if the root is null, the file/input was blank or a related error
        // send the user a message and exit

        std::cout << "ERROR: Unable to create binary tree. Missing data.\n Exiting program.\n";
        return 1;
    }
    else {
        // tree was successfully created. let the user know

        std::cout << "\nBinary tree successfully created!\n" << "Performing traversals.\n";
    }

    // open the output files for each traversal method
    std::string levelFileName = filename + ".levelorder";
    std::ofstream levelFile(levelFileName, std::ofstream::trunc);
    std::string preFileName = filename + ".preorder";
    std::ofstream preFile(preFileName, std::ofstream::trunc);
    std::string postFileName = filename + ".postorder";
    std::ofstream postFile(postFileName, std::ofstream::trunc);

    // traverse the tree
    std::cout << "\nPerforming level-order traversal.\n";
    traverseLevelOrder(root, levelFile);
    std::cout << "Level order traversal has been written to \"" << levelFileName << "\".\n";
    std::cout << "\nPerforming pre-order traversal.\n";
    traversePreOrder(root, preFile);
    std::cout << "pre-order traversal has been written to \"" << preFileName << "\".\n";
    std::cout << "\nPerforming post-order traversal.\n";
    traversePostOrder(root, postFile);
    std::cout << "post-order traversal has been written to \"" << postFileName << "\".\n";

    // close the output files
    levelFile.close();
    preFile.close();
    postFile.close();

    // delete the binary tree so that there aren't any dangling pointers
    std::cout << "\nTraversals complete! Deleting binary tree.\n";
    destroyTree(root);

    // the program is finished!
    std::cout << "Binary tree has been successfully deleted. \nProgram end.\n";
    return 0;
}

bool isValidInput(std::ifstream& iFile) {
    /* takes read-only file as input. Parses every character and
    returns false if an unacceptable character is found. 
    
    acceptable characters:
    all letters
    all numbers
    the following symbols:
        ! " # $ & ' ( ) * + 
    
    returns true if all characters are acceptable 
    
    I originally used regex for this function, but the compile times were
    far too long. Thus I switched to using the cctype library to 
    */

    std::string line;  // line that we will use to get text from the file source
    
    if (!iFile.is_open()) {
        // unable to validate input if the file won't open. halt the program

        return false;
    }

    if (iFile.peek() == std::ifstream::traits_type::eof()) {
        // check if file is blank and halt if it is

        std::cerr << "Inputted file is empty. \n";
        return false;
    }

    while (std::getline(iFile, line)) {
        // check that every line contains only acceptable characters
        
        for (int i = 0; i < line.length(); i++) {
            // use cctype to parse every character in the current line
            /* i know this could've been done with less lines of code, but i thought this
            would be easier to read and follow along with */

            char character = line[i];
            int ascii = static_cast<int>(character);  // gets the ascii value of the char

            if (isalnum(character) || isspace(character)) {
                // continues loop if character is a letter, number, or space

                continue;
            }
            else if (ascii >= 33 && ascii <= 43) {
                // continues loop if character is one of the accepted punctation characters
                
                continue;
            }
            else {
                // halt the program with a message about which unacceptable character was found

                std::cout << "Unacceptable character found: \"" << character << "\"\n";
                return false;
            }
        }
    }
    
    // every character is acceptable; reset the file to the beginning and return true
    iFile.clear();  
    iFile.seekg(0);
    return true;
}