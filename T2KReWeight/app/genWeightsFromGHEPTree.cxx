//
// Simple example to show how to reweight GENIE events from a native ghep file. 
// ./genWeightsFromGHEPTree.exe -f inputfile [-n nevents]
//
// Will replace this with a proper example as the package developes!
//
// ./genWeightsFromGHEPTree.exe -f inputfile [-n nevents]
//   
// Jim Dobson - 28/01/2011
// Kendall Mahn - 29/08/2011 
#include <stdlib.h>
#include <iostream>
#include <cstdlib>

#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"

#include "T2KReWeight.h"
#include "T2KSyst.h"
#include "T2KGenieReWeight.h" 

#include "T2KGenieUtils.h"
#include "T2KNeutReWeight.h"
#include "T2KJNuBeamReWeight.h"

using std::cout;
using std::cerr;
using std::endl;

using namespace t2krew;

int fNEvts = -1;
char * fFileName;

void Usage();
void ParseArgs(int argc, char **argv);

int main(int argc, char *argv[])
{
 ParseArgs(argc, argv);

 cout << "Starting to reweight GENIE events from file: " << fFileName << endl;
 if(fNEvts >= 0) cout << "Will reweight nevents: " << fNEvts << endl;

 // Load the GHEP events from "gtree" 
 TFile * infile = new TFile(fFileName, "OPEN");
 if(!infile){
   cerr << "Cannot open file!" << endl;
   exit(1);
 }
 TTree * tree = (TTree*) infile->Get("gtree");
 if(!tree){
   cerr << "Cannot find tree!" << endl;
 }
 
 // Make a t2kreweighting object and add a GENIE weighting engine. 
 t2krew::T2KReWeight rw; 
 rw.AdoptWghtEngine("genie_rw", new t2krew::T2KGenieReWeight());

 // Include some GENIE specific T2K systematics.
 rw.Systematics().Include(t2krew::kGXSec_MaCCQE);
 rw.Systematics().Include(t2krew::kGXSec_MaNCRES);
 rw.Systematics().Include(t2krew::kGXSec_MaCCQE);
 rw.Systematics().Include(t2krew::kGINuke_MFP_pi);

 // Fine-tune GENIE weight calculators. 
#ifdef __T2KRW_GENIE_ENABLED__
 T2KGenieReWeight * rw_ptr = (T2KGenieReWeight*) rw.WghtEngine("genie_rw"); 
 ((GReWeightNuXSecCCQE*) rw_ptr->GetWghtCalc("xsec_ccqe"))->SetMode(GReWeightNuXSecCCQE::kModeMa);
 ((GReWeightNuXSecCCRES*) rw_ptr->GetWghtCalc("xsec_ccres"))->SetMode(GReWeightNuXSecNCRES::kModeMaMv);
 ((GReWeightNuXSecNCRES*) rw_ptr->GetWghtCalc("xsec_ncres"))->SetMode(GReWeightNuXSecNCRES::kModeMaMv);
 ((GReWeightNuXSecDIS*) rw_ptr->GetWghtCalc("xsec_dis"))->SetMode(GReWeightNuXSecDIS::kModeABCV12u);
#endif

 // Set tweaked systematics values in units of 1sigma deviation.
 rw.Systematics().SetTwkDial(t2krew::kGXSec_MaNCRES, 1.0);
 rw.Systematics().SetTwkDial(t2krew::kGXSec_MaCCQE, 1.0);
 //rw.Systematics().SetTwkDial(t2krew::kGINuke_MFP_pi, 1.0);
 //rw.Systematics().SetTwkDial(t2krew::kGXSec_AhtBY, 1.0);
 rw.Reconfigure();

 // Loop over entries and calculate weights.
 for(int i = 0; i < tree->GetEntries(); i++){
   cout << "Getting weight for GHEP tree entry number = "<< i << ":"  << endl;
   double weight = rw.CalcWeight(tree, i);
   cout << "    -->  weight = " << weight << endl;
 }
 
 return 0;
}

// Print the cmd line syntax
void Usage(){
  cout << "Cmd line syntax should be:" << endl;
  cout << "genWeightsFromGHEPTree.exe -f inputfile [-n nevents]" << endl;
}

// Messy way to process cmd line arguments.
void ParseArgs(int argc, char **argv){
  int nargs = 1; 
  if(argc<(nargs*2+1)){ Usage(); exit(1); }
  for(int i = 1; i < argc; i+=2){
    cout << "Comapring "<< string(argv[i]) << ":"<<"-f" <<" results in:"<<(string(argv[i]) == "-f")<< endl;
    if(string(argv[i]) == "-f") fFileName = argv[i+1];
    else if(string(argv[i]) == "-n") fNEvts = std::atoi(argv[i+1]);
    else {  
        cout << "Invalid argument:" << argv[i] << " "<< argv[i+1] << endl;
        Usage();
        exit(1);
    }
  } 
}

