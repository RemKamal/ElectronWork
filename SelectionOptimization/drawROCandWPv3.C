#include "TLatex.h"
#include "TTree.h"
#include "TLegend.h"
#include "TMarker.h"
#include "TCanvas.h"
#include "TString.h"
#include "TH1F.h"
#include "TFile.h"
#include <iostream>
#include "OptimizationConstants.hh"
#include "VarCut.hh"
#include "TStyle.h"

using std::cout;
using std::endl;

//
//           this script draws combined ROC and 4 Wps
//

const bool verbose = true;
// Draw barrel or endcap
const bool drawBarrel = false;
const int nROCs = 4;

// Files with signal and background trees (ideally the ntuples
// that were used for TMVA optimization
const TString fnameSignal = "DYJetsToLL_phys14_v1.root";
const TString signalTreeName = "ntupler/ElectronTree";
const TString fnameBackground = "TTJets_phys14_v1.root";
const TString backgroundTreeName = "ntupler/ElectronTree";

// Name TMVA output file that contains the pre-computed ROC, etc
const TString tmvaFileNameBarrel[nROCs] = {
  "../../barrel/SelectionOptimization/trainingData/training_results_barrel_pass1_20140203_224900/TMVA_training_results_barrel_pass1_20140203_224900.root",
  "../../barrel/SelectionOptimization/trainingData/training_results_barrel_pass2_20140203_224900/TMVA_training_results_barrel_pass2_20140203_224900.root",
  "../../barrel/SelectionOptimization/trainingData/training_results_barrel_pass3_20140203_224900/TMVA_training_results_barrel_pass3_20140203_224900.root",
  "../../barrel/SelectionOptimization/trainingData/training_results_barrel_pass4_20140203_224900/TMVA_training_results_barrel_pass4_20140203_224900.root"
};



const TString tmvaFileNameEndcap[nROCs] ={"trainingData/training_results_endcap_pass1_20140203_225900/TMVA_training_results_endcap_pass1_20140203_225900.root",
					  "trainingData/training_results_endcap_pass2_20140203_225900/TMVA_training_results_endcap_pass2_20140203_225900.root",
					  "trainingData/training_results_endcap_pass3_20140203_225900/TMVA_training_results_endcap_pass3_20140203_225900.root",
					  "trainingData/training_results_endcap_pass4_20140203_225900/TMVA_training_results_endcap_pass4_20140203_225900.root"
};


//
// Names of ROOT files that contain working points
//
const int nWorkingPointSets = 4;

// Set 1
const int markerColorSet1 = kRed;
const int markerStyleSet1 = 20;
const TString legendSet1 = "WP_Veto";
const int nWP = 1;
const TString cutFileNamesBarrelSet1[nWP] = { 
  "../../barrel/SelectionOptimization/cut_repository/cuts_barrel_20140203_224900_WP_Veto.root"

};
const TString cutFileNamesEndcapSet1[nWP] = {
  "cut_repository/cuts_endcap_20140203_225900_WP_Veto.root"

};


// Set 2
const int markerColorSet2 = kOrange;
const int markerStyleSet2 = 20;
const TString legendSet2 = "WP_Loose";
const TString cutFileNamesBarrelSet2[nWP] = { 
"../../barrel/SelectionOptimization/cut_repository/cuts_barrel_20140203_224900_WP_Loose.root"

};
const TString cutFileNamesEndcapSet2[nWP] = {
  "cut_repository/cuts_endcap_20140203_225900_WP_Loose.root"
};


// Set 3
const int markerColorSet3 = kBlue;
const int markerStyleSet3 = 20;
const TString legendSet3 = "WP_Medium";
const TString cutFileNamesBarrelSet3[nWP] = { 
"../../barrel/SelectionOptimization/cut_repository/cuts_barrel_20140203_224900_WP_Medium.root"

};
const TString cutFileNamesEndcapSet3[nWP] = {
  "cut_repository/cuts_endcap_20140203_225900_WP_Medium.root"
};


// Set 4
const int markerColorSet4 = kGreen;
const int markerStyleSet4 = 20;
const TString legendSet4 = "WP_Tight";
const TString cutFileNamesBarrelSet4[nWP] = { 
 "../../barrel/SelectionOptimization/cut_repository/cuts_barrel_20140203_224900_WP_Tight.root"
};
const TString cutFileNamesEndcapSet4[nWP] = {
  "cut_repository/cuts_endcap_20140203_225900_WP_Tight.root"
};



// Forward declarations
TTree *getTreeFromFile(TString fname, TString tname);
void overlayWorkingPoints(TCanvas *c1, 
			  TTree *signalTree, TTree *backgroundTree, 
			  const TString *cutFileNames,
			  int markerColor, int markerStyle, 
			  TLegend *leg, const TString legendText);
void   findEfficiencies(TTree *signalTree, TTree *backgroundTree,
			float &effSignal, float &effBackground, 
			VarCut *cutObject);

void debug( std::string message){
  if(verbose)
    std::cout<< message <<std::endl;
}


//
// Main function
//

void drawROCandWPv3(){
debug("I am here1");
  TCanvas *c1 = new TCanvas("c1","",10,10,600,600);
 
  c1->cd();

  TString tmvaFileName1 = tmvaFileNameBarrel[0];
  TString tmvaFileName2 = tmvaFileNameBarrel[1];
  TString tmvaFileName3 = tmvaFileNameBarrel[2];
  TString tmvaFileName4 = tmvaFileNameBarrel[3];

  const TString *cutFileNamesSet1 = cutFileNamesBarrelSet1;
  const TString *cutFileNamesSet2 = cutFileNamesBarrelSet2;
  const TString *cutFileNamesSet3 = cutFileNamesBarrelSet3;
  const TString *cutFileNamesSet4 = cutFileNamesBarrelSet4;
  if( !drawBarrel ){
    tmvaFileName1 = tmvaFileNameEndcap[0];
    tmvaFileName2 = tmvaFileNameEndcap[1];
    tmvaFileName3 = tmvaFileNameEndcap[2];
    tmvaFileName4 = tmvaFileNameEndcap[3];

    cutFileNamesSet1 = cutFileNamesEndcapSet1;
    cutFileNamesSet2 = cutFileNamesEndcapSet2;
    cutFileNamesSet3 = cutFileNamesEndcapSet3;
    cutFileNamesSet4 = cutFileNamesEndcapSet4;
  }

debug("I am here2");

  // Open the file with TMVA output
  TFile *tmvaFile1 = new TFile(tmvaFileName1);
  TFile *tmvaFile2 = new TFile(tmvaFileName2);
  TFile *tmvaFile3 = new TFile(tmvaFileName3);
  TFile *tmvaFile4 = new TFile(tmvaFileName4);
  if( !(tmvaFile1 || tmvaFile2 ||tmvaFile3 || tmvaFile4))
    assert(0);

  // 
  //  Draw the ROC curve
  //
  TH1F *hROC1 = (TH1F*)tmvaFile1->Get("Method_Cuts/Cuts/MVA_Cuts_rejBvsS");
  TH1F *hROC2 = (TH1F*)tmvaFile2->Get("Method_Cuts/Cuts/MVA_Cuts_rejBvsS");
  TH1F *hROC3 = (TH1F*)tmvaFile3->Get("Method_Cuts/Cuts/MVA_Cuts_rejBvsS");
  TH1F *hROC4 = (TH1F*)tmvaFile4->Get("Method_Cuts/Cuts/MVA_Cuts_rejBvsS");
  if( !(hROC1 || hROC2 || hROC3|| hROC3 ))
    assert(0);

debug("I am here3");


  TH1F * hROC = new TH1F("hROC", "", 100, 0, 100);
  for(int iBin = 1; iBin <=100; ++iBin){
   
    if(1<=iBin && iBin<71){
      hROC->SetBinContent(iBin, 100*hROC4->GetBinContent(iBin)) ;
      debug("I am here3A");
    }
    else if(71 <= iBin && iBin<77){  //early cut-off at 77%
      hROC->SetBinContent(iBin, 100*hROC3->GetBinContent(iBin)) ;
      debug("I am here3B");
    }
    else if(77 <=iBin && iBin<90){
      hROC->SetBinContent(iBin, 100*hROC2->GetBinContent(iBin)) ;
      debug("I am here3C");
    }
    else if(90<=iBin && iBin <=100){
      hROC->SetBinContent(iBin, 100*hROC1->GetBinContent(iBin)) ;
      debug("I am here3D");
    }
    else 
      printf("\nThis should never happen!");
  }
  
  //Set histogram attributes and draw the ROC curve
  hROC->SetDirectory(0);
  hROC->SetStats(0);
  hROC->SetLineWidth(2);
  hROC->SetTitle("");
  hROC->GetXaxis()->SetTitle("signal efficiency, %");
  hROC->GetYaxis()->SetTitle("background rejection, %");
  hROC->GetYaxis()->SetTitleOffset(1.4);
  if( drawBarrel ){
    hROC->GetXaxis()->SetRangeUser(60, 100);
    hROC->GetYaxis()->SetRangeUser(95, 100);
  }else{
    hROC->GetXaxis()->SetRangeUser(60, 100);
    hROC->GetYaxis()->SetRangeUser(80, 100);
  }
 


  // gStyle->SetStatBorderSize(1);
  //  c1->SetFillColor(0);
  gStyle->SetFillColor(0) ;
  hROC->Draw("L");

debug("I am here4");
  TString commentText = "barrel electrons";
  if( !drawBarrel )
    commentText = "endcap electrons";
  TLatex *comment = new TLatex(0.2, 0.2, commentText);
  comment->SetNDC(kTRUE);
  comment->Draw();

  c1->Update();

  // 
  // Overlay the cuts
  //
debug("I am here5");
  // First find the TestTree for measuring efficiency and rejection
  printf("\n Take true electrons from %s   tree %s\n", 
	 fnameSignal.Data(), signalTreeName.Data());
  TTree *signalTree = getTreeFromFile( fnameSignal, signalTreeName);
  // Input background tree  
  printf("\n Take background electrons from %s   tree %s\n", 
	 fnameBackground.Data(), backgroundTreeName.Data());
  TTree *backgroundTree = getTreeFromFile( fnameBackground, backgroundTreeName);
  debug("I am here6");
  // Next, draw all working point sets
  if( nWorkingPointSets >=1 ){

    TLegend *leg = new TLegend(0.15, 0.45, 0.5, 0.7);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    overlayWorkingPoints(c1, signalTree, backgroundTree, cutFileNamesSet1, 
			 markerColorSet1, markerStyleSet1, leg, legendSet1);
    
debug("I am here7");
    overlayWorkingPoints(c1, signalTree, backgroundTree, cutFileNamesSet2, 
    			 markerColorSet2, markerStyleSet2, leg, legendSet2);
    
    overlayWorkingPoints(c1, signalTree, backgroundTree, cutFileNamesSet3,
    			 markerColorSet3, markerStyleSet3, leg, legendSet3);
    overlayWorkingPoints(c1, signalTree, backgroundTree, cutFileNamesSet4,  			   
    			 markerColorSet4, markerStyleSet4, leg, legendSet4);
    leg->Draw("same");
    
  }
  
  debug("I am here8");
  // Save the figure into a file
  TString outname = "figures/plot_ROCandWP_barrel_v3_feb2015.png";
  if( !drawBarrel )
    outname = "figures/plot_ROCandWP_endcap_v3_feb2015.png";
  c1->Print(outname);
debug("I am here9");
  return;
};

// Get a given tree from a given file name.
TTree *getTreeFromFile(TString fname, TString tname){

  TFile *file = new TFile( fname );
  TTree *tree     = (TTree*) file->Get(tname);
  
  return tree;
}

// Draw on a given canvas the full set of working points
void   overlayWorkingPoints(TCanvas *c1, 
			    TTree *signalTree, TTree *backgroundTree, 
			    const TString *cutFileNames,
			    int markerColor, int markerStyle, 
			    TLegend *leg, const TString legendText){


  // Now loop over working points
  for(int iwp = 0; iwp<nWP; iwp++){
    
    // Load the working point from a ROOT file
    TFile *cutFile = new TFile(cutFileNames[iwp]);
    if( !cutFile )
      assert(0);
    VarCut *cutObject = (VarCut*)cutFile->Get("cuts");
    if( !cutObject )
      assert(0);
    
    // Compute the efficiencies
    float effSignal, effBackground;
    findEfficiencies(signalTree, backgroundTree, effSignal, effBackground,
		     cutObject);
    printf("Computed eff for cut from %s, effS= %.6f effB= %.6f\n",
	   cutFileNames[iwp].Data(), effSignal, effBackground);
    
    // Make a marker and draw it.
    TMarker *marker = new TMarker(100*effSignal, 100-100*effBackground, 20);
    marker->SetMarkerSize(2);
    marker->SetMarkerColor(markerColor);
    marker->SetMarkerStyle(markerStyle);
    marker->Draw("same");

    // Add marker to the legend only once. Do not draw the legend here,
    // it is drawn in the main function later
    if( iwp == 0 ){
      if( !leg )
	assert(0);
      leg->AddEntry(marker, legendText, "p");
    }

    c1->Update();
    
    cutFile->Close();
  }

  
}

// Compute signal and background efficiencies for given cuts
void findEfficiencies(TTree *signalTree, TTree *backgroundTree,
		      float &effSignal, float &effBackground, VarCut *cutObject){

  TCut etaCut = "";
  if( drawBarrel ){
    etaCut = Opt::etaCutBarrel;
  }else{
    etaCut = Opt::etaCutEndcap;
  }
  TCut kinematicCuts = Opt::ptCut && etaCut;

  TCut preselectionCuts = kinematicCuts && Opt::otherPreselectionCuts;
  
  TCut signalCuts = preselectionCuts && Opt::trueEleCut;
  TCut backgroundCuts = preselectionCuts && Opt::fakeEleCut;  
 
  TCut selectionCuts = *(cutObject->getCut());

  // printf("\nSelecton cuts:\n");
  // selectionCuts.Print();
  // printf("\nSignal cuts:\n");
  // signalCuts.Print();
  // printf("\nBackground cuts:\n");
  // backgroundCuts.Print();

  effSignal = (1.0*signalTree->GetEntries(selectionCuts && signalCuts) )
    / signalTree->GetEntries(signalCuts);

  effBackground = (1.0*backgroundTree->GetEntries(selectionCuts 
						  && backgroundCuts) )
    / backgroundTree->GetEntries(backgroundCuts);
  
  return;
}
