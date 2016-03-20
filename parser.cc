/*
  Maximilian Hasselbusch
  parser.cc
  This file contains all the function bodies for the parser class
  Last Updated: 16 January 2016
*/

#include "parser.h"
#include "token.h"
#include <cstring>

//String array that contains all of the commands that will be printed out at code generation
const string Parser::ops[] = {"ADD", "SUB", "AND", "DIV", "REM", "ISEQ", "ISGE", "ISGT", "ISLE",
			      "ISLT", "ISNE", "MULT", "OR", "LOADL", "LOADV", "STOREV", "JUMPF",
			      "JUMP", "INSLABEL", "PRINT", "SEQ", "NULLX", "PRINTLN", "PRINTS"};

//parser constructor
Parser::Parser(Lexer& lexerx, ostream& outx): lexer(lexerx), out(outx), lindex(1), tindex(1){
  token = lexer.nextToken();
}

//parser destructor
Parser::~Parser() {
}

void Parser::genCode() {
  TreeNode* programNode = program();
  generateCode(programNode);
}

//generates the code for a specific node
void Parser::gen(TreeNode* node) {
  switch (node->op) {
    case SEQ:
    case NULLX:
    break;
    case LOADL:
    case LOADV:
    case STOREV:
    case JUMPF:
    case JUMP:
      emit(node->op, node->val);
      break;
    case PRINTS:
      emit(node->op, "\"" + node->val + "\"");
      break;
    case INSLABEL:
      emit(node->val);
      break;
    default:
      emit(node->op);
  }
}

//generate the code from the tree
void Parser::generateCode(TreeNode* node) {
  if (node != NULL) {
    generateCode(node->leftChild);
    generateCode(node->rightChild);
    gen(node);
  }
}

//prints error messages
void Parser::error(string message) {
  cerr << message << " Found " << token->lexeme() << " at line " << token->line() << " position " << token->pos() << endl;
  exit(1);
}

//check the next token to make sure it matches the grammar
void Parser::check(int tokenType, string message) {
  if (token->type() != tokenType)
    error(message);
}

//parse a Factor
Parser::TreeNode* Parser::factor() {

  TreeNode* t;
  
  if((token->type() == Token::LPAREN) | (token->type() == Token::INTLIT) | (token->type() == Token::FLOATLIT) | (token->type() == Token::IDENT)){

    switch(token->type()){
    case Token::LPAREN:
      token = lexer.nextToken();
      t = expression();
      check(Token::RPAREN, "Missing RParen.");
      break;
    case Token::INTLIT:
    case Token::FLOATLIT:
      t = new TreeNode(LOADL, token->lexeme());
      break;
    case Token::IDENT:
      t = new TreeNode(LOADV, token->lexeme());
      break;
    }
    token = lexer.nextToken();
  }
  else{
    t = new TreeNode(SEQ, " ");
    error("Invalid factor. ");
  }
  return t;
}

//parse a Term
Parser::TreeNode* Parser::term() {

  TreeNode* t = factor();
  TreeNode* f;
  Token* saveTok;

  while((token->type() == Token::TIMES) | (token->type() == Token::DIVIDE) | (token->type() == Token::REM)){

    saveTok = token;
    token = lexer.nextToken();
    f = factor();

    switch(saveTok->type()){
    case Token::TIMES:
      t = new TreeNode(MULT, t, f);
      break;
    case Token::DIVIDE:
      t = new TreeNode(DIV, t, f);
      break;
    case Token::REM:
      t = new TreeNode(REM, t, f);
      break;
    }
  }
    
    return t;
}

//parse an Expression
Parser::TreeNode* Parser::expression() {

  TreeNode* t = term();
  TreeNode* f;
  Token* saveTok;
  
  while((token->type() == Token::PLUS) | (token->type() == Token::MINUS)){
    saveTok = token;
    token = lexer.nextToken();
    f = term();
    switch(saveTok->type()){
    case Token::PLUS:
      t = new TreeNode(ADD, t, f);
      break;
    case Token::MINUS:
      t = new TreeNode(SUB, t, f);
      break;
    }
    if(token->type() == Token::RPAREN){
      return t;
    }
  }
  return t;
}
 
//parse a SetStatement
Parser::TreeNode* Parser::setStatement() {

  TreeNode* t;
  TreeNode* f;
  string ident;
  
  check(Token::SET, "Missing set keyword. ");
  token = lexer.nextToken();

  check(Token::IDENT, "Missing Ident. ");
  t = new TreeNode(STOREV, token->lexeme());
  token = lexer.nextToken();

  check(Token::ASSIGN, "Missing assignment operator. ");
  token = lexer.nextToken();

  f = expression();
  t = new TreeNode(SEQ, " ", f, t);

  return t;
}

//parse a PrintExpression
Parser::TreeNode* Parser::printExpression() {

  TreeNode* t;
  
  if((token->type() == Token::LPAREN) | (token->type() == Token::INTLIT) | (token->type() == Token::FLOATLIT) | (token->type() == Token::IDENT)){
    t = expression();
    return t;
  }
  if(token->type() == Token::STRLIT){
    t = new TreeNode(PRINTS, token->lexeme());
    cout << token->lexeme() << endl;
    token = lexer.nextToken();
    return t;
  }
  error("Invalid print expression. ");
  return t = NULL;
}

//parse a PrintStatement
Parser::TreeNode* Parser::printStatement() {
  
  TreeNode* t;
  TreeNode* f;

  check(Token::PRINT, "Missing print keyword. ");
  token = lexer.nextToken();
  t = printExpression();
  t = new TreeNode(PRINT, t, NULL);
  
  while(token->type() == Token::COMMA){  
    token = lexer.nextToken();
    f = printExpression();
    t = new TreeNode(SEQ, f, t);
  }
  return t;
}

//parse a Statement
Parser::TreeNode* Parser::statement() {

  TreeNode* t;
  
  if(token->type() == Token::SET){
    t = setStatement();
    return t;
  }
  if(token->type() == Token::PRINT){
    t = printStatement();
    return t;
  }
  if(token->type() == Token::WHILE){
    t = whileStatement();
    return t;
  }
  if(token->type() == Token::IF){
    t = ifStatement();
    return t;
  }
  else{
    error("Statement error. ");
    return t = NULL;
  }  
}

//parse a CompoundStatement
Parser::TreeNode* Parser::compoundStatement() {

  TreeNode* t = statement();
  TreeNode* f;
 
  while((token->type() == Token::SET) || (token->type() == Token::PRINT) || (token->type() == Token::WHILE) || (token->type() == Token::IF)){

    f = statement();    
    t = new TreeNode(SEQ, " ", t, f);
  }
  return t;
}

//parse a WhileStatement
Parser::TreeNode* Parser::whileStatement(){

  string l1 = makeLabel();
  check(Token::WHILE, "Missing while keyword. ");
  token = lexer.nextToken();
  
  TreeNode* t = logicalExpression();
  check(Token::DO, "Missing do keyword. ");
  token = lexer.nextToken();

  TreeNode* f = compoundStatement();
  check(Token::END, "Missing end keyword. ");
  token = lexer.nextToken();
  string l2 = makeLabel();

  //below is a series of new nodes being created in order to properly
  //order the instructions for the while loop
  
  TreeNode* g = new TreeNode(INSLABEL, l1);
  g = new TreeNode(SEQ, g, t);
  TreeNode* k = new TreeNode(JUMPF, l2);
  g = new TreeNode(SEQ, g, k);
  g = new TreeNode(SEQ, g, f);
  TreeNode* j = new TreeNode(JUMP, l1);
  g = new TreeNode(SEQ, g, j);
  TreeNode* p = new TreeNode(INSLABEL, l2);
  g = new TreeNode(SEQ, g, p);
  return g;

}

//parse an IfStatement
Parser::TreeNode* Parser::ifStatement(){

  check(Token::IF, "Missing if keyword. ");
  token = lexer.nextToken();
  TreeNode* t = logicalExpression();
  check(Token::THEN, "Missing then keyword. ");
  token = lexer.nextToken();

  TreeNode* f = compoundStatement();
  string l1 = makeLabel();
  string l2;
  TreeNode* k;
  
  //if the if statement has an else statemet immediately following it,
  //the below tree is created.
  if(token->type() == Token::ELSE){
    
    token = lexer.nextToken();
    l2 = makeLabel();
    
    //k is the compound statement inside the else block
    k = compoundStatement();

    check(Token::ENDIF, "Missing endif keyword. ");

    //construct the if statement
    TreeNode* j = new TreeNode(JUMPF, l2);
    j = new TreeNode(SEQ, t, j);
    j = new TreeNode(SEQ, j, f);
    TreeNode* p = new TreeNode(JUMP, l1);
    j = new TreeNode(SEQ, j, p);
    TreeNode* l = new TreeNode(INSLABEL, l2);
    j = new TreeNode(SEQ, j, l);
    j = new TreeNode(SEQ, j, k);
    TreeNode* o = new TreeNode(INSLABEL, l1);
    j = new TreeNode(SEQ, j, o);

    //reassign the treenode inside this scope to one outside so it can
    //be returned
    t = j;
  }
  //if it doesn't, the below tree is created
  else{
    
    TreeNode* j = new TreeNode(JUMPF, l1);
    j = new TreeNode(SEQ, t, j);
    j = new TreeNode(SEQ, j, f);
    TreeNode* p = new TreeNode(INSLABEL, l1);
    j = new TreeNode(SEQ, j, p);
    check(Token::ENDIF, "Missing endif keyword. ");

    //reassign j
    t = j;
  }

  token = lexer.nextToken();
  return t;
}

//parse a LogicalExpression
Parser::TreeNode* Parser::logicalExpression(){

  TreeNode* t = relationalExpression();
  TreeNode* f;

  while((token->type() == Token::AND) | (token->type() == Token:: OR)){

    if(token->type() == Token::AND){
      f = relationalExpression();
      t = new TreeNode(AND, " ", t, f);
    }
    if(token->type() == Token::OR){
      f = relationalExpression();
      t = new TreeNode(OR, " ", t, f);
    }
  }
  return t;
}

//parse a RelationalExpression
Parser::TreeNode* Parser::relationalExpression(){

  TreeNode* t = expression();
  int x = token->type();
  token = lexer.nextToken();
  TreeNode* f = expression();

  switch(x){
  case Token::EQ:
    t = new TreeNode(ISEQ, " ", t, f);
    break;

  case Token::LT:
    t = new TreeNode(ISLT, " ", t, f);
    break;

  case Token::LE:
    t = new TreeNode(ISLE, " ", t, f);
    break;

  case Token::GT:
    t = new TreeNode(ISGT, " ", t, f);
    break;

  case Token::GE:
    t = new TreeNode(ISGE, " ", t, f);
    break;

  case Token::NE:
    t = new TreeNode(ISNE, " ", t, f);
    break;

  default:
    error("Invalid relational operator. ");
    break;
  }
  
  return t;
}

//parse a Program
Parser::TreeNode* Parser::program() {

  check(Token::PROGRAM, "Missing program keyword.");

  token = lexer.nextToken();
  check(Token::IDENT, "Missing program name.");

  token = lexer.nextToken();
  TreeNode* programNode = compoundStatement();
  check(Token::END, "Missing END keyword.");

  token = lexer.nextToken();
  check(Token::PROGRAM, "Missing PROGRAM keyword.");
  return programNode;
}
