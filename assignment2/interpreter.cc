#include "interpreter.h"
#include "parser.h"

bool BetaReduction(Node* node){
  if(!node){
    return false;
  }
  if(auto a = dynamic_cast<ApplicationNode*>(node)){
    //hier komt nog iets
  }
}
