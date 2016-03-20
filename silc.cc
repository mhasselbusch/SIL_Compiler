/*
  Maximilian Hasselbusch and Dr. James Femister
  silc.cc
  This program takes a file as input, lexically analyzes it and
  parses it in order to prepare the code for complilation within the SVM.
  Last Updated: 16 January 2016
*/

#include "token.h"
#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <fstream>

using namespace std;

bool genCode = true;

//processFile creates lexer and parser objects, parses the code from the input file (starting from the <Program> nonterminal, and generates the code
void processFile(istream& in) {
  
  Lexer lexer(in);
  Parser parser(lexer, cout);
  Parser::TreeNode* program = parser.program();

  if (genCode) {
    //generate the code if not command line arguments are entered
    parser.generateCode(program);    
  } 
  else {
    //prints out the entire tree if the user inputs the "-p" command line argument
    cout << Parser::TreeNode::toString(program) << endl;
  }
}

int main(int argc, char **argv) {
  
  ifstream in;
  
  if (argc > 1) {
    
    string a1 = argv[1];
    
    //if the user enters the "-h" command line argument, the following code executes and the program quits
    if (a1 == "-h") {
      
      cout << "Usage: silc [-h | -p ] [<filename>]\nOptions:\n  -h Display this message\n  -p Parse the file and display the tree (no code generation)\nThe compiler will read from standard input if no filename is given.\nAll output goes to standard output.\n";
      return 0;
    }

    //if the user enters the "-p" command line argument, genCode is changed to false so that when the processFile function is called, the code generation is skipped and the tree is printed instead 
    else if (a1 == "-p") {
      genCode = false;
    }

    //if no command line arguments are entered besides the name of the input file, the program will attempt to open it and send it to the processFile function for lexical analysis and parsing
    else {
      in.open(argv[1]);
      processFile(in);
      in.close();
      return 0;
    }
    
    //if multiple command line arguments are entered (2 or more) and the file name is present, the program will attempt to open it and send it to the processFile function for lexical analysis and parsing
    if (argc > 2) {
      in.open(argv[2]);
      processFile(in);
      in.close();
      return 0;
    } 
    
    else {
      processFile(cin);
    }
  } 
  else {
    processFile(cin);
  }
}
