#include "Variables.hh"
#include "OptimizationConstants.hh"
#include "VarCut.hh"
#include <algorithm>
#include "assert.h"
#include <string>
#include <vector>

const TString barrelTimeName ="20140203_224900_";
const TString endcapTimeName ="20140203_225900_";


int nVarsInTable = Vars::nVariables +1; // +1 means add passConversionVeto
const int nWP = 4;

TString  cutsFiles[nWP];
TFile  * cutFiles[nWP];
VarCut * cutObjects[nWP] ;


const bool printBarrelTable = false;

vector<string> doHorizontConcat(vector<string> & vec1, vector<string> & vec2, vector<string> & vec3, vector<string> & vec4 );


void printWIKItable(){


  for (int iwp =0; iwp < nWP; ++iwp){

    cutsFiles[iwp] = 
      Opt::cutRepositoryDir + TString("/") 
      + (printBarrelTable ? TString("cuts_barrel_") : TString("cuts_endcap_") ) 
      + (printBarrelTable ? barrelTimeName : endcapTimeName)
      + Opt::wpNames[iwp]
      + TString(".root");    

    cutFiles[iwp] = new TFile(cutsFiles[iwp]);

    /* #1 */
    if (cutFiles[iwp]->IsZombie()) {
      std::cout << "Error opening file" << std::endl;
       exit(-1);
    }
    
    //same protection alternative:
    /* #2 */
    if( !cutFiles[iwp] )
      assert(0);

    cutObjects[iwp] = (VarCut*)cutFiles[iwp]->Get("cuts");
    if( !cutObjects[iwp] )
      assert(0);
    
  }//end for-loop

  vector<string> table_V = cutObjects[0]->VarCut::printTable();
  vector<string> table_L = cutObjects[1]->VarCut::printTable();
  vector<string> table_M = cutObjects[2]->VarCut::printTable();
  vector<string> table_T = cutObjects[3]->VarCut::printTable();

  vector<string> vecToPrint =  doHorizontConcat(table_V, table_L, table_M, table_T);

  vector<string>::size_type vecst =0;
  std::cout << (printBarrelTable ? "\n PHYS14 selection, conditions: PU20 bx25, barrel cuts ( |eta supercluster| <= 1.479)" : "\n PHYS14 selection, conditions: PU20 bx25, endcap cuts (1.479 < |eta supercluster| < 2.5)") << std::endl;
  
  while (vecst != vecToPrint.size() ){
    std::cout<<vecToPrint[vecst]<<std::endl;
    ++vecst;
  }



}//end of main fcn



bool digit(char c){
  return isdigit(c);
}


bool space(char c){
  return isspace(c);
}


vector<string> doHorizontConcat(vector<string> & vec1, vector<string> & vec2, vector<string> & vec3, vector<string> & vec4 ){
  
  /* problem with static and ROOT */ 
  const string comparSignInSep = " < |";    

  vector<string> wikiTable;
  vector<string>::size_type vecst =0;
  
  //just few checks
  if (vec1.size() != vec2.size() || vec3.size() != vec4.size() || vec1.size() != vec3.size() )
    assert(0);
  
  while( vecst != vec1.size() ){
    
    string str;
    typedef string::const_iterator iter;
    iter Vbeg = vec1[2].begin();
    iter Vend = vec1[2].end();
    
    string::size_type maxlen = strlen(Vars::variables[8]->nameTmva.Data()) + 6 ; // because it is "| " on the left plus " < |" on the right    
    
    if(vecst==0) {
      str   =  string  ("| Variables                  |") 
	+ string(" Veto     |") 
	+ string(" Loose    |")
	+ string(" Medium   |")
	+ string(" Tight    |")   ;
    
    }
    
    //last line regarding passConversionVeto
    else if(vecst == vec1.size() -1) {
      str = 	"| passConversionVeto" 
	+ string(maxlen - strlen("| passConversionVeto "),' ')
	+ string("|")
	
	+" YES"
	+ string(maxlen - 24,' ') //to make string equal 11 chars
	+ string("|")

	+" YES"
	+ string(maxlen - 24,' ') //to make string equal 11 chars
	+ string("|")

	+" YES"
	+ string(maxlen - 24,' ') //to make string equal 11 chars
	+ string("|")

	+" YES"
	+ string(maxlen - 24,' ') //to make string equal 11 chars
	+ string("|");    

    }
    
    
    else {
      
      // iter Vcur = search(Vbeg, Vend, comparSignInSep.begin(), comparSignInSep.end());
      // iter beg = find_if(Vcur, Vend, digit);
      // iter end = find_if(beg, Vend, space);
      
      string str1,str2, str3, str4;
      str1 = vec1[vecst]; 
      str2 = vec2[vecst]; 
      str3 = vec3[vecst]; 
      str4 = vec4[vecst]; 
      
      //sanity checks
      if (str1.compare(str2) == 0)
	std::cout << str1 << " is  " << str2 << '\n';
      if (str3.compare(str4) == 0)
	std::cout << str3 << " is  " << str4 << '\n';
      if (str1.compare(str3) == 0)
	std::cout << str1 << " is  " << str3 << '\n';
      if (str2.compare(str4) == 0)
	std::cout << str2 << " is  " << str4 << '\n';

      string::size_type start = 31;
      string::size_type finish = 40;

      // start = beg - str1.begin();
      // finish = end - str1.begin();
      //      str +=	str1; //the whole string from the 1st vector
      str = str.append (str1.begin(), str1.end() ) ;
      //      std::cout<<"\nlengths is "<<str.length();
      str.append (str2.begin()+ start, str2.begin() + finish ) ;
      str+= string(" |");
      str.append (str3.begin()+ start, str3.begin() + finish ) ;
      str+= string(" |");
      str.append (str4.begin()+ start, str4.begin() + finish ) ;
      str+= string(" |");
      
    } //end of else
    
    wikiTable.push_back(str);
    
    ++vecst;
  }//end while-loop
    
  return wikiTable;
}//end fcn
  
