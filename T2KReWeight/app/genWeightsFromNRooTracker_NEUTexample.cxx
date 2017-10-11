//
// Example to show how to reweight NEUT events from the NRooTrackerVtx
// objects stored in an oaAnalysis tree. Run with: 
//
// ./generateWeightsFromNRooTracker_NEUTexample.exe -n nd_inputfile
//   
// Jim Dobson - 28/03/2011
// Kendall Mahn - 29/08/2011
// 
#include <stdlib.h>
#include <cstdlib>

#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"

#include "T2KReWeight.h"
#include "T2KSyst.h"

#include "T2KGenieReWeight.h" 
#include "T2KGenieUtils.h"

#include "T2KNeutReWeight.h"
#include "T2KNeutUtils.h"

#include "T2KJNuBeamReWeight.h"

#ifdef __T2KRW_OAANALYSIS_ENABLED__
#include "ND__NRooTrackerVtx.h"
#endif

// For weight storer class
#include "T2KWeightsStorer.h"

#include "SK__h1.h"

using std::cout;
using std::cerr;
using std::endl;

using namespace t2krew;

int fNEvts = -1;
int sNEvts = -1; // summary tree
char * fNDFileName;
char * fSKFileName;

void Usage();
void ParseArgs(int argc, char **argv);

int main(int argc, char *argv[])
{
  //cout<<"blah"<<endl;
// This example only works when compiled against oaAnalysis
#ifdef __T2KRW_OAANALYSIS_ENABLED__
 ParseArgs(argc, argv);

 T2KWeightsStorer *storer = new T2KWeightsStorer("testweights.root"); //forweightstorer

 cout << "Starting to reweight NEUT events from ND file: " << fNDFileName << endl;

 // Load the oaAnalysis TNRooTrackerVtx tree containing
 // TClonesArray of objects inheriting from TRooTrackerBase. 
 TFile * ND_infile = new TFile(fNDFileName, "OPEN");
 if(!ND_infile){
   cerr << "Cannot open ND file!" << endl;
   exit(1);
 }
 TTree * ND_tree = (TTree*) ND_infile->Get("TruthDir/NRooTrackerVtx");
 if(!ND_tree){
   cerr << "Cannot find ND_tree!" << endl;
 }
 
 int NVtx; 
 TClonesArray * nRooVtxs = new TClonesArray("ND::NRooTrackerVtx", 100);
 ND_tree->SetBranchAddress("Vtx", &nRooVtxs);
 ND_tree->SetBranchAddress("NVtx", &NVtx);

 if(fNEvts < 0) fNEvts = ND_tree->GetEntries();

 cout << "Will reweight ND nevents: " << fNEvts << endl;

 // Add NEUT reweighting engine
 t2krew::T2KReWeight rw; 
 rw.AdoptWghtEngine("neut_rw", new t2krew::T2KNeutReWeight());

 // Include some NEUT specific T2K systematics.
 rw.Systematics().Include(t2krew::kNXSec_MaCCQE);
 rw.Systematics().Include(t2krew::kNXSec_MaRES);
 


#ifdef __T2KRW_NEUT_ENABLED__

 Int_t dialccpi = 3;
 Int_t dialccqe = 3;

 // Make a loop where 2 tweak dials are changed from -3 to +3 sigma each in turn // = 14 variations
 for(int dial=0; dial<14; dial++){

   if(dial<7){
     dialccqe = dial;
     dialccpi = 3;

   }else if(dial<14){
     dialccqe = 3;
     dialccpi = dial-7;
   }
   // values of tweak dials
   std::cout << "ccqe " << dialccqe << " ccpi " << dialccpi <<  std::endl;  
   Double_t dialccpivalue = 0.165289*(dialccpi-3); // value is a fractional shift, here it starts at -3*16% = -45% shift. 16% corresponds to 1.21+/-0.2 GeV 
   Double_t dialccqevalue = 0.165289*(dialccqe-3); // value is a fractional shift, here it starts at -3*16% = -45% shift

   // Set absolute tweak dial
   rw.Systematics().SetAbsTwk(t2krew::kNXSec_MaCCQE); // abs
   rw.Systematics().SetAbsTwk(t2krew::kNXSec_MaRES); // abs

   // twkdial = nominaldial * (1 + dialvalue)
   rw.Systematics().SetTwkDial(t2krew::kNXSec_MaCCQE, dialccqevalue); 
   rw.Systematics().SetTwkDial(t2krew::kNXSec_MaRES, dialccpivalue);


 rw.Reconfigure();

 storer->NewSystSet(rw.Systematics()); // save the current twk dial values to the weight storer class

 for(int i = 0; i < fNEvts; i++){
   ND_tree->GetEntry(i);
   Double_t weight = 1.0;
   // The passthrough information saves all neutrino vertexes in an event
   // One has to separately associate these to the reconstructed track which is not done here.
   // This loop calculates the weight for all of these vertices
   for(int j = 0; j<NVtx; j++){
	 ND::NRooTrackerVtx * vtx = (ND::NRooTrackerVtx*) nRooVtxs->At(j);
	 if(!vtx){
	   cerr << "Cannot find NRooTrackerVtx object - skipping weight for this vertex!";
	   continue;
	 }

	 //	 cout << "Getting weight for ND280Event = "<< i << ", vertex number = "<< j << " with interaction mode: "<< vtx->EvtCode->GetString().Data()  << " Enu " << vtx->StdHepP4[0][3] << endl;
	 weight = rw.CalcWeight(vtx);
	 storer->AddWeight(weight); // add weight for each
	 
       } // if correct vertex from summary tree
     } // vertex container
    
   
 } // dial loop


#endif


 storer->SaveToFile(); // save the weights to a file
 delete storer; 

#endif // __T2KRW_OAANALYSIS_ENABLED__
 return 0;
}

// Print the cmd line syntax
void Usage(){
  cout << "Cmd line syntax should be:" << endl;
  cout << "genWeightsFromJReWeight.exe -n nd_inputfile -s sk_inputfile [-e nevents]" << endl;
}

// Messy way to process cmd line arguments.
void ParseArgs(int argc, char **argv){
  int nargs = 1; 
  if(argc<(nargs*2+1)){ Usage(); exit(1); }
  for(int i = 1; i < argc; i+=2){
    if(string(argv[i]) == "-n") fNDFileName = argv[i+1];
    else if(string(argv[i]) == "-s") fSKFileName = argv[i+1];
    else if(string(argv[i]) == "-e") fNEvts = std::atoi(argv[i+1]);
    else {  
        cout << "Invalid argument:" << argv[i] << " "<< argv[i+1] << endl;
        Usage();
        exit(1);
    }
  } 
}

