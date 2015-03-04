#include "VarCut.hh"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cstring>
#include "assert.h"

using std::vector;
using std::string;

const int UNDEFCUT = -999;

VarCut::VarCut()
{
  // All cuts are initialized to an unlikely value
  for(int i=0; i<Vars::nVariables; i++)
    _cuts[i]= UNDEFCUT;
};

// Construct TCut object for all cuts joined with &&
TCut *VarCut::getCut(){

  TCut *cut = 0;

  // Die if something appears uninitialized
  for(int i=0; i<Vars::nVariables; i++){
    if( _cuts[i] == UNDEFCUT ){
      printf("VarCut:: not all cuts are set! Die!\n");
      assert(0);
    }
  }

  cut = new TCut("");
  for(int i=0; i<Vars::nVariables; i++){
    // The += adds all cuts with &&:
    (*cut) += TString::Format(" %s < %f ",
			      Vars::variables[i]->nameTmva.Data(),
			      _cuts[i]);
  }
  
  return cut;
}

TCut *VarCut::getCutsForMVAcomparison(){

  TCut *cut = 0;

  // Die if something appears uninitialized
  for(int i=0; i<Vars::nVariables; i++){
    if( _cuts[i] == UNDEFCUT ){
      printf("VarCut:: not all cuts are set! Die!\n");
      assert(0);
    }
  }

  cut = new TCut("");
  for(int i=0; i<Vars::nVariables; i++){
    // The += adds all cuts with &&:
    if( i==getVariableIndex("relIsoWithEA") || i==getVariableIndex("d0") || i==getVariableIndex("dz") || i==8 ) 
      continue;
    (*cut) += TString::Format(" %s < %f ",
			      Vars::variables[i]->nameTmva.Data(),
			      _cuts[i]);
  }
  
  return cut;
}


TCut *VarCut::getCutDB(){

  TCut *cut = 0;

  // Die if something appears uninitialized
  for(int i=0; i<VarsDB::nVariables; i++){
    if( _cuts[i] == UNDEFCUT ){
      printf("VarCut:: not all cuts are set! Die!\n");
      assert(0);
    }
  }

  cut = new TCut("");
  for(int i=0; i<VarsDB::nVariables; i++){
    // The += adds all cuts with &&:
    (*cut) += TString::Format(" %s < %f ",
			      VarsDB::variables[i]->nameTmva.Data(),
			      _cuts[i]);
  }
  
  return cut;
}


TCut *VarCut::getCutNminusOne(int varIndex){

  TCut *cut = 0;

  // Die if something appears uninitialized
  for(int i=0; i<Vars::nVariables; i++){
    if( _cuts[i] == UNDEFCUT ){
      printf("VarCut:: not all cuts are set! Die!\n");
      assert(0);
    }


  }

  cut = new TCut("");
  for(int i=0; i<Vars::nVariables; i++){
    // The += adds all cuts with &&:
    if(i == varIndex) continue;
    (*cut) += TString::Format(" %s < %f ",
			      Vars::variables[i]->nameTmva.Data(),
			      _cuts[i]);
  }
  
  return cut;
}


TCut *VarCut::getCutOneVar(int varIndex){

  TCut *cut = 0;

  // Die if something appears uninitialized
  for(int i=0; i<Vars::nVariables; i++){
    if( _cuts[i] == UNDEFCUT ){
      printf("VarCut:: not all cuts are set! Die!\n");
      assert(0);
    }
  }

  cut = new TCut("");
  for(int i=0; i<Vars::nVariables; i++){
    // The += adds all cuts with &&:
    if(i != varIndex) continue;
    (*cut) += TString::Format(" %s < %f ",
			      Vars::variables[i]->nameTmva.Data(),
			      _cuts[i]);
  }
  
  return cut;
}


void VarCut::setCutValue(TString varName, float val){

  int index = getVariableIndex(varName);

  if( index != -1 ){
    _cuts[index] = val;
  }else{
    printf("VarCut::setCutValue: requested variable is not known!!!\n");
  }

  return;
}

void VarCut::setCutValueTmvaName(TString varNameTmva, float val){

  int index = getVariableIndexTmvaName(varNameTmva);

  if( index != -1 ){
    _cuts[index] = val;
  }else{
    printf("VarCut::setCutValue: requested variable is not known!!!\n");
  }

  return;
}

float VarCut::getCutValue(TString variable){

  float cutVal = UNDEFCUT;
  int index = getVariableIndex(variable);

  if( index != -1 ){
    cutVal = _cuts[index];
  }else{
    printf("VarCut::getCutValue: requested variable is not known!!!\n");
  }

  return cutVal;
}


float VarCut::getCutValueTmvaName(TString variable){

  float cutVal = UNDEFCUT;
  int index = getVariableIndexTmvaName(variable);

  if( index != -1 ){
    cutVal = _cuts[index];
  }else{
    printf("VarCut::getCutValue: requested variable is not known!!!\n");
  }

  return cutVal;
}




int VarCut::getVariableIndex(TString variable){

  int index = -1;
  for(int i=0; i<Vars::nVariables; i++){
    if( variable == Vars::variables[i]->name ){
      index = i;
      break;
    } 
  }

  return index;
}

int VarCut::getVariableIndexTmvaName(TString variableTmva){

  int index = -1;
  for(int i=0; i<Vars::nVariables; i++){
    if( variableTmva == Vars::variables[i]->nameTmva ){
      index = i;
      break;
    } 
  }

  return index;
}

bool VarCut::isSymmetric(TString variable){

  bool result = false;
  int index = getVariableIndex(variable);

  if( index != -1 ){
    result = Vars::variables[index]->symmetric;
  }else{
    printf("VarCut::isSymmetric: requestd variable is not known!!!\n");
  }

  return result;
}

// Print all cut values to stdout
void VarCut::print(){

  printf("VarCut::print: Cut values are\n");
  for(int i=0; i<Vars::nVariables; i++){
    printf("  %30s < %f\n", Vars::variables[i]->nameTmva.Data(), _cuts[i]);
  }

}


vector <string> VarCut::printTable(){

  string::size_type maxlen = strlen(Vars::variables[8]->nameTmva.Data()) + 6 ; // because it is "| " on the left plus " < |" on the right
  
  vector<string> ret;

  //  std::cout<<maxlen;
  //maxlen =30

  for(int iVar =0; iVar < Vars::nVariables; ++iVar){
      
    std::string currStr, cutStr;
    char currStr_buff[100], cutStr_buff[100];
    
    if(iVar==0){
      currStr = 
	"| Variables" 
	+ string(maxlen - strlen("| Variables "),' ') + string("|");
      ret.push_back(currStr);
      
    }
    
    sprintf(currStr_buff, "| %s" ,Vars::variables[iVar]->nameTmva.Data() );
    currStr = currStr_buff;
    currStr += string(maxlen- strlen(Vars::variables[iVar]->nameTmva.Data()) -6,' '); //exclude 2 spaces +  4 for " < |"
    currStr += string(" < |");

    sprintf(cutStr_buff, " %1.6f" , _cuts[iVar] );
    cutStr= cutStr_buff;
    cutStr += string(maxlen - 29,' '); //exclude 2 spaces + 8 chars number
    cutStr += string("|");
    currStr += cutStr;
    //currStr length is 11   " '8digits' |"

    ret.push_back(currStr);
  } //end of for-loop

  return ret;
} //end of printTable fcn
