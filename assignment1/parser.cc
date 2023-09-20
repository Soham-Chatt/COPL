#include "parser.h"
#include <map>
#include <stack>
#include <string>
#include <cctype>
#include <vector>

enum Symbols {
	// the symbols:
	// Terminal symbols:
	TS_L_PARENS,	// (
	TS_R_PARENS,	// )
	TS_VAR,		// var (eg. x)
	TS_POINT,	// .
	TS_LAMBDA,	// \lambda
	TS_EOS,		// $, in this case corresponds to '\0'
	TS_INVALID,	// invalid token

	// Non-terminal symbols:
	NTS_M,		// M
	NTS_N,		// N
	NTS_L		// L
};

Symbols lexer(std::string c){
  if(c == "("){
    return TS_L_PARENS;
  }
  else if(c == ")"){
    return TS_R_PARENS;
  }
  else if(isalpha(c[0])){
    return TS_VAR;
  }
  else if(c == "+"){
    return TS_POINT;
  }
  else if(c == "\\"){
    return TS_LAMBDA;
  }
  else if(c == "$"){
    return TS_EOS;
  } else {
    return TS_INVALID;
  }
}

bool Parser(std::string expr){
  std::map< Symbols, std::map<Symbols, int> > ParsingTable;
  std::stack<Symbols> SymbolStack;
  std::vector<std::string> tokens;
  
  SymbolStack.push(TS_EOS);
  SymbolStack.push(NTS_M);

  ParsingTable[NTS_M][TS_VAR] = 1;
  ParsingTable[NTS_M][TS_L_PARENS] = 2;
  ParsingTable[NTS_N][TS_VAR] = 4;
  ParsingTable[NTS_N][TS_L_PARENS] = 4;
  ParsingTable[NTS_N][TS_LAMBDA] = 3;
  ParsingTable[NTS_L][TS_VAR] = 5;
  ParsingTable[NTS_L][TS_L_PARENS] = 5;
  ParsingTable[NTS_L][TS_R_PARENS] = 6;

  for(size_t i = 0; i < expr.length(); i++){
    if(expr[i] == ' '){
      continue;
    }
    else{
      std::string temp = "";
      while(expr[i] != ' ' && i < expr.length()){
	temp += expr[i];
	i++;
			}
      tokens.push_back(temp);
    }
  }
  
  int pos(0);
  while(SymbolStack.size() > 0){
    if(lexer(tokens[pos]) == SymbolStack.top()){
      std::cout << "Matched symbols: " << lexer(tokens[pos]) << std::endl;
      pos++;
      SymbolStack.pop();
    } else {
      std::cout << "Rule " << ParsingTable[SymbolStack.top()][lexer(tokens[pos])] << std::endl;
      switch (ParsingTable[SymbolStack.top()][lexer(tokens[pos])]){
      case 1:
	SymbolStack.pop();
	SymbolStack.push(TS_VAR);
	break;
      case 2:
	SymbolStack.pop();
	SymbolStack.push(TS_L_PARENS);
	SymbolStack.push(NTS_N);
	SymbolStack.push(TS_R_PARENS);
	break;
      case 3:
	SymbolStack.pop();
	SymbolStack.push(TS_LAMBDA);
	SymbolStack.push(TS_VAR);
	SymbolStack.push(TS_POINT);
	SymbolStack.push(NTS_M);
	break;
      case 4:
	SymbolStack.pop();
	SymbolStack.push(NTS_M);
	SymbolStack.push(NTS_L);
	break;
      case 5:
	SymbolStack.pop();
	SymbolStack.push(NTS_M);
	break;
      case 6:
	SymbolStack.pop();
	break;
      default:
	std::cout << "parsing table defaulted" << std::endl;
	return false;
      }
    }
  }
  return true;
}
