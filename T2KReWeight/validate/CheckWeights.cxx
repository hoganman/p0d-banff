
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TAxis.h"

#include "T2KReWeight.h"
#include "T2KSyst.h"

#include "T2KGenieReWeight.h"
#include "T2KGenieUtils.h"

#include "T2KNeutReWeight.h"
#include "T2KNeutUtils.h"

#include "T2KNIWGReWeight.h"
#include "T2KNIWGUtils.h"

#include "T2KGEANTReWeight.h"

#include "T2KJNuBeamReWeight.h"

#ifdef __T2KRW_OAANALYSIS_ENABLED__
#include "ND__NRooTrackerVtx.h"
#include "ND__GRooTrackerVtx.h"
#endif

#include "T2KWeightsStorer.h"

#include "SK__h1.h"

#ifdef __T2KRW_NEUT_ENABLED__
//#define NEUTROOT
#ifdef NEUTROOT
#include "neutrootTreeSingleton.h"
#endif
#endif

using namespace std;

///////////////////////////////////////////////////////////////////////////////////

const int iVerbose = 0;

// Define Supported Analyses
const int nAnas = 6;
string anaTreeNames[nAnas] = {
  // Keep all RooTrackerVtx based files below here
  "GRooTrackerVtx", // GENIE Highland
  "TruthDir/GRooTrackerVtx", // GENIE oaAnalysis
  "NRooTrackerVtx", // NEUT Highland
  "TruthDir/NRooTrackerVtx", // NEUT oaAnalysis
  // Keep all RooTrackerVtx based files above here
  
  // Keep all SK__h1 based files below here
  "h1",                      // raw_sk
  // Keep all SK__h1 based files above here
  
  // Keep all neutroot based files below here
  "neuttree"                 // raw_neutroot
  // Keep all neutroot based files above here
};

// This must correspond to the above "anaTreeNames"
enum anaEnum {
  genie_highland,
  genie_oaAnalysis,
	neut_highland,
	neut_oaAnalysis,
	raw_sk,
  raw_neutroot
};

int iana=0; // Counter and flag for determining analysis

// Tree types T2KReWeight input
enum tree_type_enum {
  GRooTrackerVtx,// iana=0,1
  NRooTrackerVtx,// iana=2,3
	SK__h1,        // iana=4
	neutroot       // iana=5
};    
int tree_type = -1;

// Number of events in file to loop over (default is all events)
// Can be modified with '-e' command line argument
int fNEvts = -1;

// Filename containers
TString fInputFileName = "";
TString fOutputFileName = "";

bool flgSIRW=true;

// For matching to RooTrackerVtx

void Usage();
void ParseArgs(int argc, char **argv);
vector<string> separate(string input_uptions);

int nPars=0;// total number of systematic parameters

//##########################################################################
// Lists of systematic parameters to check

//GENIE, only apply to ND280 files
const int nGENIEPars = 3;
t2krew::T2KSyst_t GENIEsystArr[nGENIEPars] = {
  t2krew::kGXSec_MaCCQE,
//  t2krew::kGXSec_MaCCQEshape,
//  t2krew::kGXSec_NormCCQE,
  t2krew::kGXSec_MaCCRES,
//  t2krew::kGXSec_MaCCRESshape,
//  t2krew::kGXSec_NormCCRES,
  t2krew::kGNucl_CCQEPauliSupViaKF
};

double GENIEsyst1sgm[nGENIEPars] = {
  0.165289256,
//  0.165289256,
//  0.1,
  0.165289256,
//  0.165289256,
//  0.1,
  0.01
};

//NEUT
const int nNEUTPars = 6;
t2krew::T2KSyst_t NEUTsystArr[nNEUTPars] = {
  t2krew::kNXSec_MaCCQE,
  t2krew::kNXSec_1overMaCCQE2,
  t2krew::kNXSec_MaRES,
  t2krew::kNNucl_CCQEPauliSupViaKF,
  t2krew::kNXSec_BYOnOffDIS,
  t2krew::kNNucl_PilessDcyRES
};

double NEUTsyst1sgm[nNEUTPars] = {
  0.165289256,
  0.2,
  0.165289256,
  0.01,
  1.,
  0.2
};

//NIWG
const int nNIWGPars = 20;
t2krew::T2KSyst_t NIWGsystArr[nNIWGPars] = {
  t2krew::kNIWG2012a_sf,
  t2krew::kNIWG2012a_eb,
  t2krew::kNIWG2012a_pf,
  t2krew::kNIWGDeltaMass_width,
  t2krew::kNIWG2012a_mbcc1pi_enushp,
  t2krew::kNIWG2012a_dismpishp,
  t2krew::kNIWG2012a_ccqeE0,
  t2krew::kNIWG2012a_ccqeE1,
  t2krew::kNIWG2012a_ccqeE2,
  t2krew::kNIWG2012a_cc1piE0,
  t2krew::kNIWG2012a_cc1piE1,
  t2krew::kNIWG2012a_nc1piE0,
  t2krew::kNIWG2012a_nc1pi0E0,
  t2krew::kNIWG2012a_1gamE0,
  t2krew::kNIWG2012a_ccmultipiE0,
  t2krew::kNIWG2012a_ccdisE0,
  t2krew::kNIWG2012a_cccohE0,
  t2krew::kNIWG2012a_nccohE0,
  t2krew::kNIWG2012a_ncotherE0,
  t2krew::kNIWG2012a_ccnueE0//,
//  t2krew::kNIWG2012a_antinu
};

double NIWGsyst1sgm[nNIWGPars] = {
  1.,
  0.36,
  0.13825,
  0.52,
  0.5,
  0.4,
  0.11,
  0.3,
  0.3,
  0.21,
  0.4,
  0.3,
  0.31,
  0.3,
  0.25,
  0.25,
  1.0,
  0.3,
  0.3,
  0.06//,
//  0.4
};

//FSI
const int nFSIPars = 6;
t2krew::T2KSyst_t FSIsystArr[nFSIPars] = {
  t2krew::kNCasc_FrAbs_pi,
  t2krew::kNCasc_FrCExLow_pi,
  t2krew::kNCasc_FrInelLow_pi,
  t2krew::kNCasc_FrCExHigh_pi,
  t2krew::kNCasc_FrInelHigh_pi,
  t2krew::kNCasc_FrPiProd_pi
};

double FSIsyst1sgm[nFSIPars] = {
  0.5,
  0.5,
  0.5,
  0.3,
  0.3,
  0.5
};

//SI, only apply to SK files
const int nSIPars = 5;
t2krew::T2KSyst_t SIsystArr[nSIPars] = {
  t2krew::kGEANT_PionNuclXSecABS,
  t2krew::kGEANT_PionNuclXSecQEL,
  t2krew::kGEANT_PionNuclXSecCXL,
  t2krew::kGEANT_PionNuclXSecDCX,
  t2krew::kGEANT_PionXSecTbl//[1-8]
};

double SIsyst1sgm[nSIPars] = {
  0.3,
  0.2,
  0.5,
  0.5,
  1.0
};

//##########################################################################

void GetSyst(int iSyst, t2krew::T2KSyst_t& t2ksyst, double& onesig) {
  
  t2ksyst = t2krew::kSystNull;
  onesig = 1.;
  
  if (tree_type==GRooTrackerVtx) {
    if (iSyst<nGENIEPars) {
      t2ksyst = GENIEsystArr[iSyst];
      onesig = GENIEsyst1sgm[iSyst];
    }
    else {
      cout << "Error: systematic parameter does not exist!" << endl;
      exit(-1);
    }
  }
  else {
    if (iSyst<nNEUTPars) {
      t2ksyst = NEUTsystArr[iSyst];
      onesig = NEUTsyst1sgm[iSyst];
    }
    else if (iSyst<nNEUTPars+nNIWGPars) {
      t2ksyst = NIWGsystArr[iSyst-nNEUTPars];
      onesig = NIWGsyst1sgm[iSyst-nNEUTPars];
    }
    else if (iSyst<nNEUTPars+nNIWGPars+nFSIPars) {
      t2ksyst = FSIsystArr[iSyst-nNEUTPars-nNIWGPars];
      onesig = FSIsyst1sgm[iSyst-nNEUTPars-nNIWGPars];
    }
    else if (iSyst<nNEUTPars+nNIWGPars+nFSIPars+nSIPars) {
      t2ksyst = SIsystArr[iSyst-nNEUTPars-nNIWGPars-nFSIPars];
      onesig = SIsyst1sgm[iSyst-nNEUTPars-nNIWGPars-nFSIPars];
    }
    else {
      cout << "Error: systematic parameter does not exist!" << endl;
      exit(-1);
    }
  }
  
}

//##########################################################################

int main(int argc, char *argv[])
{
  
  ParseArgs(argc, argv);

  // Check that filenames were specified
  if (!fInputFileName.Length() || !fOutputFileName.Length()) Usage();

  // Open the input file
  TFile * infile = new TFile(fInputFileName, "OPEN");
  if(!infile->IsOpen()) exit(1);

  cout << endl << "Loading input tree file: " << fInputFileName.Data() << endl << endl;

  TTree *input_tree = 0;
  for (iana=0; iana<nAnas; iana++) {
    input_tree = (TTree*) infile->Get(anaTreeNames[iana].c_str());
    if (input_tree) {
      cout << "Found " << anaTreeNames[iana] << " tree" << endl;
      break;
    }
  }

  if (!input_tree) {
    cerr << "Error: Cannot find input tree" << endl;
    exit (1);
  }
  
// Add tree distinguishing here if implementing a new tree format
  if (iana<=genie_oaAnalysis) {
    tree_type = GRooTrackerVtx;
    
#ifndef __T2KRW_OAANALYSIS_ENABLED__
    cout << "Error: oaAnalysis must be enabled upon build if using ND280 files" << endl;
    exit (1);
#endif
    
    cout << "Loading GRooTrackerVtx tree ";
  }
  else if (iana<=neut_oaAnalysis) {
    tree_type = NRooTrackerVtx;
    
#ifndef __T2KRW_OAANALYSIS_ENABLED__
    cout << "Error: oaAnalysis must be enabled upon build if using ND280 files" << endl;
    exit (1);
#endif
    
    cout << "Loading NRooTrackerVtx tree ";
  }
  else if (iana<=raw_sk) {
    tree_type = SK__h1;
    cout << "Loading SK tree ";
  }
  else if (iana<=raw_neutroot) {
    tree_type = neutroot;
    cout << "Loading neutroot tree ";
  }
  else {
    cerr << "Error: Unknown analysis type, iana=" << iana << endl;
    exit (1);
  }
  
  // Get number of events in tree
  if(fNEvts < 0) fNEvts = input_tree->GetEntries();
  cout << "with " << fNEvts << " events." << endl << endl;
  
  
  // Initialize T2KReWeight and engines
  t2krew::T2KReWeight rw;
  
  // Initialize input tree (need to implement new tree formats here)
  int NVtx;
  TClonesArray * nRooVtxs;
  SK::SK__h1 *skVtxs;
  
  if (tree_type == GRooTrackerVtx) {
    nRooVtxs = new TClonesArray("ND::GRooTrackerVtx", 100);
    input_tree->SetBranchAddress("Vtx", &nRooVtxs);
    input_tree->SetBranchAddress("NVtx", &NVtx);
    
    rw.AdoptWghtEngine("genie_rw", new t2krew::T2KGenieReWeight());
    
    nPars = nGENIEPars;// Total # of systematic parameters to examine
    
  }
  else if (tree_type == NRooTrackerVtx) {
    nRooVtxs = new TClonesArray("ND::NRooTrackerVtx", 100);
    input_tree->SetBranchAddress("Vtx", &nRooVtxs);
    input_tree->SetBranchAddress("NVtx", &NVtx);
    
    rw.AdoptWghtEngine("neut_rw", new t2krew::T2KNeutReWeight());
    rw.AdoptWghtEngine("niwg_rw", new t2krew::T2KNIWGReWeight());
    
    nPars = nNEUTPars+nNIWGPars+nFSIPars;// Total # of systematic parameters to examine
    
  }
  else if (tree_type == SK__h1) {
    skVtxs = new SK::SK__h1(input_tree,1);
    
    rw.AdoptWghtEngine("neut_rw", new t2krew::T2KNeutReWeight());
    rw.AdoptWghtEngine("niwg_rw", new t2krew::T2KNIWGReWeight());
    
    nPars = nNEUTPars+nNIWGPars+nFSIPars;// Total # of systematic parameters to examine
    
    if (flgSIRW) {// SI reweighting on SK files only!
      rw.AdoptWghtEngine("geant_rw", new t2krew::T2KGEANTReWeight());
      nPars += nSIPars;
    }
  }
#ifdef NEUTROOT
  else if (tree_type == neutroot) {
    // Create global tree object 
    NeutrootTreeSingleton * neutrootTreeObj = NeutrootTreeSingleton::Instance(fInputFileName.Data());
    
    rw.AdoptWghtEngine("neut_rw", new t2krew::T2KNeutReWeight());
    rw.AdoptWghtEngine("niwg_rw", new t2krew::T2KNIWGReWeight());
    
    nPars = nNEUTPars+nNIWGPars+nFSIPars;// Total # of systematic parameters to examine
    
  }
#endif
  
  // Initialize output weights tree and file
  t2krew::T2KWeightsStorer *wt_storer = new t2krew::T2KWeightsStorer(fOutputFileName.Data());
  
  t2krew::T2KSyst_t t2ksyst = t2krew::kSystNull;
  double dumsgm;
  for (int iSyst=0; iSyst<nPars; iSyst++) {// set up systematic parameters
    
    GetSyst(iSyst,t2ksyst,dumsgm);
    
    rw.Systematics().Include(t2ksyst);
    if (t2ksyst!=t2krew::kGEANT_PionXSecTbl) {
      rw.Systematics().SetAbsTwk(t2ksyst);
    }
    
  }
  
  for (int iSyst=0; iSyst<nPars; iSyst++) {// vary one systematic parameter at a time
    
    t2krew::T2KSyst_t currsyst;
    double curr1sigm;
    GetSyst(iSyst,currsyst,curr1sigm);
    
    if (tree_type == GRooTrackerVtx) {// GENIE file
#ifdef __T2KRW_GENIE_ENABLED__
      t2krew::T2KGenieReWeight * genie_rw_ptr = (t2krew::T2KGenieReWeight*) rw.WghtEngine("genie_rw");
      
      if (currsyst==t2krew::kGXSec_MaCCQEshape || currsyst==t2krew::kGXSec_NormCCQE) {
        ((GReWeightNuXSecCCQE*) genie_rw_ptr->GetWghtCalc("xsec_ccqe"))->SetMode(GReWeightNuXSecCCQE::kModeNormAndMaShape);
      }
      else {
        ((GReWeightNuXSecCCQE*) genie_rw_ptr->GetWghtCalc("xsec_ccqe"))->SetMode(GReWeightNuXSecCCQE::kModeMa);
      }
      
      if (currsyst==t2krew::kGXSec_MaCCRESshape || currsyst==t2krew::kGXSec_NormCCRES) {
        ((GReWeightNuXSecCCRES*) genie_rw_ptr->GetWghtCalc("xsec_ccres"))->SetMode(GReWeightNuXSecCCRES::kModeNormAndMaMvShape);
      }
      else {
        ((GReWeightNuXSecCCRES*) genie_rw_ptr->GetWghtCalc("xsec_ccres"))->SetMode(GReWeightNuXSecCCRES::kModeMaMv);
      }
#endif
    }
    else {// NEUT file
      t2krew::T2KNeutReWeight * neut_rw_ptr = (t2krew::T2KNeutReWeight*) rw.WghtEngine("neut_rw");
      
      if (currsyst==t2krew::kNXSec_1overMaCCQE2) {
        ((NReWeightNuXSecCCQE*) neut_rw_ptr->GetWghtCalc("xsec_ccqe"))->SetMode(NReWeightNuXSecCCQE::kMode1overMa2);
      }
      else {
        ((NReWeightNuXSecCCQE*) neut_rw_ptr->GetWghtCalc("xsec_ccqe"))->SetMode(NReWeightNuXSecCCQE::kModeMa);
      }
      
    }
    
    int nVar;
    double dialArr[100];
    if (currsyst==t2krew::kGEANT_PionXSecTbl) {
      nVar=8;
      for (int iVar=0; iVar<nVar; iVar++) {
        dialArr[iVar]=(double)(iVar+1);
      }
    }
    else {
      nVar=4;
      for (int i=0; i<nVar; i++) {
        dialArr[i]=(i*2.-3.)*curr1sigm;//-3sigma~+3sigma
      }
    }
    
    for (int iVar=0; iVar<nVar; iVar++) {//systematic variations
      
      for (int iSyst2=0; iSyst2<nPars; iSyst2++) {
        GetSyst(iSyst2,t2ksyst,dumsgm);
        rw.Systematics().SetTwkDial(t2ksyst,0.);
      }
      rw.Systematics().SetTwkDial(currsyst,dialArr[iVar]);
      
      rw.Reconfigure();
      
      // Store the values of the current set of parameters
      wt_storer->NewSystSet(rw.Systematics());
      
      // Loop over events
      for(int i = 0; i < fNEvts; i++){
        
        if (i%100==0 || iVerbose) cout << " Event #" << i << endl;
        
        Double_t weight = 1.0;
        
        if (tree_type == GRooTrackerVtx) {
#ifdef __T2KRW_OAANALYSIS_ENABLED__
          
          input_tree->GetEntry(i);
          
          // Loop over RooTrackerVtx's
          for(int j = 0; j<NVtx; j++){
            
            ND::GRooTrackerVtx * vtx = (ND::GRooTrackerVtx*) nRooVtxs->At(j);
            if(!vtx){
              cout << "Error: Cannot find GRooTrackerVtx object for event " << i << ", vertex " << j << endl;
              continue;
            }
            weight = rw.CalcWeight(vtx);
            
            break;
          }
          
#endif
        }
        
        else if (tree_type == NRooTrackerVtx) {
#ifdef __T2KRW_OAANALYSIS_ENABLED__
          
          input_tree->GetEntry(i);
          
          // Loop over RooTrackerVtx's
          for(int j = 0; j<NVtx; j++){
            
            ND::NRooTrackerVtx * vtx = (ND::NRooTrackerVtx*) nRooVtxs->At(j);
            if(!vtx){
              cout << "Error: Cannot find NRooTrackerVtx object for event " << i << ", vertex " << j << endl;
              continue;
            }
            weight = rw.CalcWeight(vtx);
            
            break;
          }
          
#endif
        }
        
        else if (tree_type == SK__h1) {
          skVtxs->GetEntry(i);
          weight = rw.CalcWeight(skVtxs);
        }
        
        else if (tree_type == neutroot) {
          weight = rw.CalcWeight(input_tree, i);
        }
        
        // Store weight for the event
        wt_storer->AddWeight(weight);
        
      } // end loop over events
      
    }
  }
  
  wt_storer->SaveToFile(); // Save the weights to a file
  delete wt_storer;
  
  return 0;
}

// Print the cmd line syntax
void Usage(){
  cout << "Error Usage: " << endl;
  cout << "CheckWeights.exe -i <Input Filename> -o <Output Weight Tree Filename>" << endl;
  exit(1);
}

// Messy way to process cmd line arguments.
void ParseArgs(int argc, char **argv){
  for(int i = 1; i < argc; i++){
    if(string(argv[i]) == "-i") {fInputFileName = argv[i+1]; i++;}
    else if(string(argv[i]) == "-o") {fOutputFileName = argv[i+1]; i++;}
    else if(string(argv[i]) == "--noSI") {flgSIRW=false;}
    else {
      cout << "Invalid argument:" << argv[i] << " "<< argv[i+1] << endl;
      Usage();
    }
  }
}

vector<string> separate(string input_options){
  vector<string> options_string;
  istringstream iss(input_options);
  while(iss){
    string sub;
    iss >> sub;
    options_string.push_back(sub);
  }
  options_string.pop_back();
  return options_string;
}
