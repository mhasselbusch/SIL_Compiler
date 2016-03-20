/*
  Maximilian Hasselbusch and Dr. James Femister
  parser.h
  This file contains all the function and field declarations for the
  parser class.
  Last Updated: 16 January 2016
*/

#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "lexer.h"
#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <sstream>

using namespace std;

class Parser {
  
private:
  
  //this enumeration contains all commands that are printed out at code generation

  //The SEQ command is a command used to link other ones together.  It is the parent node to two leaf nodes containing actual commands.  Print out the generated tree to see SEQ nodes in action.
  enum Operation {
    ADD, SUB, AND, DIV, REM, ISEQ, ISGE, ISGT, ISLE, ISLT,
    ISNE, MULT, OR,
    LOADL, LOADV, STOREV, JUMPF, JUMP, INSLABEL,
    PRINT, SEQ, NULLX, PRINTLN, PRINTS
  };

public:  

  class TreeNode {
    
  public:
    
    //TreeNode data fields
    Operation op;
    string val;
    TreeNode *leftChild;
    TreeNode *rightChild;
    
    //this is the function that assigns values to fields inside a TreeNode
    void init(Operation opx, string valx, TreeNode *leftChildx, TreeNode *rightChildx) {
      op = opx;
      val = valx;
      leftChild = leftChildx;
      rightChild = rightChildx;
    }
    
    //These are different constructors for TreeNodes.  Each one is used for different types of commands.  
    TreeNode(Operation op, string val) {
      init(op, val, NULL, NULL);
    }
    
    TreeNode(Operation op, string val, TreeNode *leftChild, TreeNode *rightChild) {
      init(op, val, leftChild, rightChild);
    }

    TreeNode(Operation op) {
      init(op, "", NULL, NULL);
    }

    TreeNode(Operation op, TreeNode *leftChild, TreeNode *rightChild) {
      init(op, "", leftChild, rightChild);
    }
    
    //this function formats appends forward slashes to node values if the node operator (command) is PRINTS.  If not, it    only returns the value
    static string operandFormat(TreeNode *node) {
       if (node->op == PRINTS) 
          return "\"" + node->val + "\"";
       return node->val;      
    }
    
    //calls the toString0 function below
    static string toString(TreeNode *node) {
      return toString0(node, 0);
    }
    
    //converts TreeNodes and their children into strings
    static string toString0(TreeNode *node, int spaces) {
      static string blanks = "                                        ";
      string left = "";
      string right = "";
      bool isLeaf = true;
      if (node->leftChild != NULL) {
	left = toString0(node->leftChild, spaces+2);
	isLeaf = false;
      }
      if (node->rightChild != NULL) {
	right = toString0(node->rightChild, spaces+2);
	isLeaf = false;	
      }
      string ret;
      if (isLeaf) {
	ret = blanks.substr(0, spaces) + ops[node->op] + "[" + operandFormat(node) + "]";
      } else {
	ret = blanks.substr(0, spaces) + ops[node->op] + "(\n" + left + ",\n" + right + "\n" + 
	      blanks.substr(0, spaces) + ")";
      }
      return ret;
    }
    
  };
  
private:
  Lexer lexer;
  Token* token;
  ostream& out;
  int lindex;
  int tindex;
  
  //converts ints to strings
  string itos(int i) { stringstream ss; ss << i; string res = ss.str(); return res;}
  
  //labels are used for loops.  Lables allow the virtual machine to move from the end of a loop back to the front of it
  string makeLabel() { string tmp = "L"; stringstream ss; ss << ++lindex; string res = ss.str(); tmp = tmp + res; return tmp;}

  //create a temporary label
  string makeTemp() { string tmp = "$TEMP"; stringstream ss; ss << ++tindex; string res = ss.str(); tmp = tmp + res; return tmp;}
  
  static const string ops[];  
  
  //emit will print out the nodes to standard output 
  void emit(Operation op, string object) { out << ops[op] << " " <<  object << endl; }
  void emit(Operation op) { out << ops[op] << endl; }
  void emit(string label) { out << label << endl; }
  
  void gen(TreeNode* node);
  void error(string message);
  void check(int tokenType, string message);
  
 public:

  //function declarations for the nonterminals
  TreeNode* program();
  TreeNode* compoundStatement();
  TreeNode* statement();
  TreeNode* setStatement();
  TreeNode* printExpression();
  TreeNode* printStatement();
  TreeNode* whileStatement();
  TreeNode* ifStatement();
  TreeNode* logicalExpression();
  TreeNode* relationalExpression();
  TreeNode* expression();
  TreeNode* term();
  TreeNode* factor();

  Parser(Lexer& lexer, ostream& out);
  ~Parser(); 

  void generateCode(TreeNode* node);  
  void genCode();
};

#endif
