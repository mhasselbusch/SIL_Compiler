/*
  Maximilian Hasselbusch
  lexer.h
  This header files contains the functions and variables required to
  properly analyze and designate a token.
  Last Updated: 16 January 2016
*/


#ifndef LEXER_H
#define LEXER_H
#include "token.h"
#include <istream>

using namespace std;

class Token;
class Lexer{

public:

  Lexer(istream& st);
  char nextChar();
  Token* nextToken();
  char nChar;

private:  
  
  istream& inStream;
  int lineNum;
  int charPos;
  
};

#endif
