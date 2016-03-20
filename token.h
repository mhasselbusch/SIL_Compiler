/*
  Maximilian Hasselbusch
  token.h
  This header files contains the member functions and variables that
  hold and manipulate the information stored in a token.
  Last Updated: 16 January 2016
*/

#ifndef TOKEN_H
#define TOKEN_H
#include "lexer.h"
#include <string>

using namespace std;

class Token{

public:
  
  //token constructor
  Token(int type, string lex, int line, int pos);
  
  //token destructor
  ~Token();
  
  int type(); //returns the type

  string lexeme(); //returns the lexeme

  int line(); //returns the line the lexeme is found on

  int pos(); //returns the starting char pos of the lexeme on its line

  //tokens where the lexeme will change with each token read in
  static const int INTLIT = 1;
  static const int FLOATLIT = 2;
  static const int STRLIT = 3;
  static const int IDENT = 4;

  //special characters
  static const int PLUS = 5;
  static const int MINUS = 6;
  static const int TIMES = 7;
  static const int DIVIDE = 8;
  static const int REM = 9;
  static const int ASSIGN = 10;
  static const int LPAREN = 11;
  static const int RPAREN = 12;
  static const int COMMA = 13;
  static const int EQ = 14; //==
  static const int LT = 15; //<
  static const int LE = 16; //<= 
  static const int GT = 17; //>
  static const int GE = 18; //>=
  static const int NE = 19; //!=

  //keywords
  static const int SET = 20;
  static const int PRINT = 21;
  static const int WHILE = 22;
  static const int DO = 23;
  static const int END = 24;
  static const int AND = 25;
  static const int OR = 26;
  static const int IF = 27;
  static const int THEN = 28;
  static const int ELSE = 29;
  static const int ENDIF = 30;
  static const int PROGRAM = 31;

  static const int ENOF = 32; //end-of-file 
  static const int ERROR = 33; //returns when no other token type is recognized
  
private:

  int tokType;
  string lexStr;
  int lineNum;
  int charPos;

};

#endif
