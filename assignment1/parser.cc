#include "parser.h"
#include <map>
#include <stack>

enum Symbols {
	// the symbols:
	// Terminal symbols:
	TS_L_PARENS,	// (
	TS_R_PARENS,	// )
	TS_VAR,		// var (eg. x)
	TS_POINT,	// .
	TS_LAMBDA,	// \lambda
	TS_EPSILON, 	// \epsilon
	TS_EOS,		// $, in this case corresponds to '\0'
	TS_INVALID,	// invalid token

	// Non-terminal symbols:
	NTS_M,		// S
	NTS_N,		// F
	NTS_L		// L
};

int Parser(string expr){
  map< Symbols, map<Symbols, int> > ParsingTable;
  stack<Symbols> SymbolStack;
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
  
  while(SymbolStack.size() > 0){
    
  }
}
