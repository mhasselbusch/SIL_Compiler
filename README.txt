Simple Imperative Language Compiler (SILC)
Author: Maximilian Hasselbusch, Dr. James Femister
Documented by: Maximilian Hasselbusch
Lehigh University, Bethlehem PA
Last Updated: 16 January 2016

**Please refer to testfile.sil throughout as a reference for the SIL language.**

**Please refer to the accompanying file, SIL_GRAMMAR, in order to understand the grammar of the SIL language.**

**Dr. Femister provided TreeNode data structure and silc.cc code due to time restraints in class.  The rest of the program is written by Maximilian Hasselbusch.**

##Program Description##

The SILC compiler is made for use with the Simple Imperative Language (SIL) programming language written by Dr. James Femister, Professor of Practice, Lehigh University.  The language was created in order to give students a basic language to write a compiler for, rather than attempting to do one for a more popular language such as Java or C.  The compiler is written in C++ and utilizes version C++14.  The program can be run from any unix/linux command line.  

The compiler includes the following features:
-Lexical, Syntactical and Semantical analysis
-Line number and character position tracking for debugging
-Error catching and printing (includes line and character numbers).

Once the program is run, code is generated from the SIL file.  This code can be run in the virtual machine created for the SIL language.  The virtual machine (Simple Virtual Machine, SVM) is not included in this software package.

##Running the Program##

1. Compile the program 

In order to ensure the compiler runs smoothly, execute a ‘make clean’ command to remove all previous versions of the program from the directory.  Afterwards, execute a ‘make’ command in order to run the Makefile, thus compiling the program into an executable.  The file ’silc’ should appear in the directory.

2. Run the compiler

Run the executable ’silc’ with -h as the first command line argument to get instructions on how to run the program.  The compiler should be run with the provided file ‘testfile.sil’ in order to test its functionality.  

Possible commands:

./silc -h testfile.sil  ->  display more information.
./silc -p testfile.sil  ->  run the compiler and print out a tree of tokens (no code is generated).
./silc testfile.sil  ->  run the compiler and print out the compiled commands for use in a virtual machine.

##Class Descriptions##

The compiler includes 4 classes and object files: Lexer, Parser, Silc and Token. 

Lexer (lexer.cc, lexer.h)

The Lexer class functions as the lexical analyzer for the compiler.  Lexer takes in tokens from the input stream and analyzes them character by character.  Once the token type is identified (see token.h for token types), a new token object will be created and returned.

Parser (parser.cc, parser.h)

The Parser class, written as a recursive decent parser, does the brunt of the work of the compiler.  This class calls the Lexer class in order to get each new token from the input stream and attempts to match them with the SIL grammar.  If the next token does not match the grammar, an error message is printed to standard output and the program quits.

Silc (silc.cc)

Silc simply functions as the main class that creates a lexer object, passes it to a parser object and processes the SIL file. 

Token (token.cc, token.h)

The token class is used to create token objects that hold the data for each inputed token from the SIL file.  Token.h includes all of the token categories as well as the individual types.  There are 33 different token types.    
