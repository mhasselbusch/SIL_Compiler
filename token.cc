/*
  Maximilian Hasselbusch
  token.cc
  This file contains all of the function bodies for the token class
  Last Updated: 16 January 2016
*/

#include "token.h"
#include "lexer.h"

using namespace std;

//token constructor (assigns parameters to fields)
Token::Token(int type, string lex, int line, int pos):tokType(type),lexStr(lex),lineNum(line),charPos(pos){  
}

//token destructor
Token::~Token(){
}

//return the type of the token
int Token::type(){
  return tokType;
}

//return the lexeme for the token (string, int value, etc.)
string Token::lexeme(){
  return lexStr;
}

//the line number where the token is found
int Token::line(){
  return lineNum;
}

//the character position where the token begins
int Token::pos(){
  return charPos;
}

