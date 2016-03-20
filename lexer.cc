/*
  Maximilian Hasselbusch
  lexer.cc
  This file contains all the function bodies for the Lexer class
  Last updated: 16 January 2016
*/

#include "parser.h"
#include "lexer.h"
#include "token.h"

using namespace std;

Lexer::Lexer(istream& st):inStream(st){

  nChar = inStream.get();
  lineNum = 1;
  charPos = 0;
}

char Lexer::nextChar(){
  charPos++;
  
  char ch = inStream.get(); 
  if(ch == '\n'){
    lineNum++;
    charPos = 0;
  }
  if(ch == '#'){
    cout << "Comment" << endl;
    while((ch = inStream.get()) != '#'){
      charPos++;
      if(ch == '\n'){
	lineNum++;
      }
    }
    if(ch == '#'){
      charPos++;
      ch = nextChar();
    }
  }
  else if(inStream.eof()){
    ch = '$';
  }
  return ch;
}

//nextToken is the main function for this class.  It is used to get tokens from the input file and turn them into token objects, rather than just chars and strings.
Token* Lexer::nextToken(){
  
  Token* tok;
  string specChar("+-*/%=),<>!");
  char cChar;
  string str;
  
  //if the next char is a newline character, it will be skipped and the program will get the next one
  if(nChar == '\n'){
    nChar = nextChar();
  }
  char ch = nChar;

  //convert chars to strings
  stringstream ss;
  string charStr;
  ss << ch;
  ss >> charStr;
  
  //this switch statement is used to decide which kind of token will be next.  Once the first character is read in, the program will decide which kind of token to create based on what character comes after it.
  switch(nChar){
   
  //these cases are used to create single character tokens

  case '\"':
    while((cChar = nextChar()) != '\"'){
      str += cChar;
    }
    tok = new Token(Token::STRLIT, str, lineNum, charPos);
    break;
    
  case '+':
    tok = new Token(Token::PLUS, charStr, lineNum, charPos);
    break;
    
  case '-':
    str[0] = ch;
    tok = new Token(Token::MINUS, charStr, lineNum, charPos); 
    break;
    
  case '*':
    str[0] = ch;
    tok = new Token(Token::TIMES, charStr, lineNum, charPos);
    break;
    
  case '/':
    str[0] = ch;
    tok = new Token(Token::DIVIDE, charStr, lineNum, charPos);
    break;
    
  case '%':
    str[0] = ch;
    tok = new Token(Token::REM, charStr, lineNum, charPos);
    break;
    
  case '=':
    str += ch;
    if((cChar = nextChar()) == '='){
      str += cChar; 
      tok = new Token(Token::EQ, str, lineNum, charPos);
    }
    else{
      tok = new Token(Token::ASSIGN, charStr, lineNum, charPos);
    }
    break;
    
  case '(':
    str[0] = ch;
    tok = new Token(Token::LPAREN, charStr, lineNum, charPos);
    break;
    
  case ')':
    str[0] = ch;
    tok = new Token(Token::RPAREN, charStr, lineNum, charPos);
    break;
    
  case ',':
    str[0] = ch;
    tok = new Token(Token::COMMA, charStr, lineNum, charPos);
    break;
    
  case '<':
    str += ch;
    if((cChar = nextChar()) == '='){
      str += cChar;
      tok = new Token(Token::LE, str, lineNum, charPos);
    }
    else{
      tok = new Token(Token::LT, charStr, lineNum, charPos);
    }
    break;
    
  case '>':
    str += ch;
    if((cChar = nextChar()) == '='){
      str += cChar;
      tok = new Token(Token::GE, str, lineNum, charPos);
    }
    else{
      tok = new Token(Token::GT, charStr, lineNum, charPos);
    }
    break;
    
  case '!':
    str += ch;
    if((cChar = nextChar()) == '='){
      str += cChar;
      tok = new Token(Token::NE, str, lineNum, charPos);
    }
    else{
      tok = new Token(Token::ERROR, charStr, lineNum, charPos);
    }
    break;
  
  //the default case is used when the token is more than one character long.  Because of this, the program automatically decides the type of token and creates a new object.

  default:   
    
    if(isdigit(ch)){
      str += ch;
            
      while(isdigit(cChar = nextChar())){
	str += cChar;
      }
      if(cChar == '.'){
	str += cChar;
	while(isdigit(cChar = nextChar())){
            str += cChar;
	}
	tok = new Token(Token::FLOATLIT, str, lineNum, charPos);

      }
      else if((cChar == ' ') || (cChar == '\n')){
       	tok = new Token(Token::INTLIT, str, lineNum, charPos);
      }
      else{
	tok = new Token(Token::ERROR, str, lineNum, charPos);
       
      }
    }
    
    //The code inside the else block will create full strings from individual characters.  This is used to make an IDENT token type.
    else{
      str += ch;
      if(str != "$"){
	while((cChar = nextChar()) != ' '){
	  if(cChar == '\n'){
	    break;
	  }
	  int x;
	  int y = 0;

	  //check to see if the character is a special char
	  for(x = 0; x < 13; x++){
	    if(cChar == specChar[x]){
	      y = 1;
	    }
	  }
	  if(y != 1){
	    str += cChar;
	  }
	  else{
	    nChar = cChar;
	    tok = new Token(Token::IDENT, str, lineNum, charPos);
	    return tok;
	  }
	}
      }
      
      //the following are all keyword cases of IDENT tokens and 'tok' will be reassigned to a new token if the string matches any of the below keywords 
      if(str == "set"){
	tok = new Token(Token::SET, str, lineNum, charPos);
      }
      else if(str == "print"){
	tok = new Token(Token::PRINT, str, lineNum, charPos);
      }
      else if(str == "while"){
	tok = new Token(Token::WHILE, str, lineNum, charPos);
      }
      else if(str == "do"){
	tok = new Token(Token::DO, str, lineNum, charPos);
      }
      else if(str == "end"){
	tok = new Token(Token::END, str, lineNum, charPos);
      }
      else if(str == "and"){
	tok = new Token(Token::AND, str, lineNum, charPos);
      }
      else if(str == "or"){
	tok = new Token(Token::OR, str, lineNum, charPos);
      }
      else if(str == "if"){
	tok = new Token(Token::IF, str, lineNum, charPos);
      }
      else if(str == "then"){
	tok = new Token(Token::THEN, str, lineNum, charPos);
      }
      else if(str == "else"){
	tok = new Token(Token::ELSE, str, lineNum, charPos);
      }
      else if(str == "endif"){
	tok = new Token(Token::ENDIF, str, lineNum, charPos);
      }
      else if(str == "program"){
	tok = new Token(Token::PROGRAM, str, lineNum, charPos);
      }
      else{
	tok = new Token(Token::IDENT, str, lineNum, charPos);
      }	 
    }
    break;
  }
  
  //nextToken gets the next character before it returns the token so the lexer always remain one character ahead
  nChar = nextChar();
  return tok;
}
      



