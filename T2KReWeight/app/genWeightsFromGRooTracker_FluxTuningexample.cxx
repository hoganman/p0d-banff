//
// Example reweights oaAnalysis events from the GRooTrackerVtx
// with the nominal flux tuning.
//
// Run with: 
//
// ./generateWeightsFromJReWeight.exe -n nd_inputfile [-e nevents]
//   
// Jim Dobson - 28/03/2011
// MPH/KM 30/08/2011 - read in OaAnalysis files, do reweighting
// 
#include <stdlib.h>
#include <cstdlib>

#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TClonesArray.h"

#include "T2KReWeight.h"
#include "T2KSyst.h"
#include "T2KJNuBeamReWeight.h"

#ifdef __T2KRW_OAANALYSIS_ENABLED__
#include "ND__GRooTrackerVtx.h"
#endif

using std::cout;
using std::cerr;
using std::endl;

using namespace t2krew;

int fNEvts = -1;
char * fNDFileName;

void Usage();
void ParseArgs(int argc, char **argv);

int main(int argc, char *argv[])
{
// This example only works when compiled against oaAnalysis
#ifdef __T2KRW_OAANALYSIS_ENABLED__
 ParseArgs(argc, argv);

 cout << "Starting to reweight event from OAanalysisFile file: " << fNDFileName << endl;

 // Load the oaAnalysis TGRooTrackerVtx tree containing
 // TClonesArray of objects inheriting from TRooTrackerBase. 
 TFile * ND_infile = new TFile(fNDFileName, "OPEN");
 if(!ND_infile){
   cerr << "Cannot open ND file!" << endl;
   exit(1);
 }
 TTree * ND_tree = (TTree*) ND_infile->Get("TruthDir/GRooTrackerVtx");
 if(!ND_tree){
   cerr << "Cannot find ND_tree!" << endl;
 }
 
 int NVtx; 
 TClonesArray * nRooVtxs = new TClonesArray("ND::GRooTrackerVtx", 100);
 ND_tree->SetBranchAddress("Vtx", &nRooVtxs);
 ND_tree->SetBranchAddress("NVtx", &NVtx);


 // Make a t2kreweighting object and add a JReWeight weighting engine.
 t2krew::T2KReWeight rw; 
 rw.AdoptWghtEngine("jnubeam_rw", new t2krew::T2KJNuBeamReWeight());
 T2KJNuBeamReWeight* rw_ptr = (T2KJNuBeamReWeight*)rw.WghtEngine("jnubeam_rw");

 //Set the run period for any proton beam reweighting
 rw_ptr->SetRunPeriod(jnubeam::rew::RUN1);
 //rw_ptr->SetRunPeriod(jnubeam::rew::RUN1);
 //rw_ptr->SetRunPeriod(jnubeam::rew::RUN1PLUS2);

 //For now, now dials are tweaked, the weights represent the nominal flux tuning
 std::cout << " Weights will represent nominal flux tuning if jnubeam is used, even if no tweak dials are set " << std::endl;
 rw.Reconfigure();

 //Configure histograms
 int nbins = 19;
 double bins[20] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0,
                    1.2, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 5.0, 7.0, 10.0};
 TH1D enu_nom("enu_nom","enu_nom",nbins,bins);
 TH1D enu_tune("enu_tune","enu_tune",nbins,bins);

 // Loop over ND entries and calculate weights.
 if(fNEvts < 0) fNEvts = ND_tree->GetEntries();
 cout << "Will reweight ND nevents: " << fNEvts << endl;
 for(int i = 0; i < fNEvts; i++){
   ND_tree->GetEntry(i);
   for(int j = 0; j<NVtx; j++){
     ND::GRooTrackerVtx * vtx = (ND::GRooTrackerVtx*) nRooVtxs->At(j);
     if(!vtx){
       cerr << "Cannot find GRooTrackerVtx object - skipping weight for this vertex!";
       continue;
     }
     double weight = rw.CalcWeight(vtx);
     enu_nom.Fill(vtx->StdHepP4[0][3]);
     enu_tune.Fill(vtx->StdHepP4[0][3],weight);
     //cout << "    -->  weight = " << weight << endl;
   }
 }
 
 ///////////////////////////////////////////////
 cout << "Finished with weights, saving histograms"  << endl;


 TFile fout("jreweight_example.root","RECREATE");
 enu_nom.Write("enu_nom"); 
 enu_tune.Write("enu_tune"); 
 fout.Close();

 cout << "See jreweight_example.root file for tuned and untuned flux vs. Enu"  << endl;
 
#endif // __T2KRW_OAANALYSIS_ENABLED__
 return 0;
}

// Print the cmd line syntax
void Usage(){
  cout << "Cmd line syntax should be:" << endl;
  cout << "genWeightsFromJReWeight.exe -n nd_OaAnalysisfile [-e nevents]" << endl;
}

// Messy way to process cmd line arguments.
void ParseArgs(int argc, char **argv){
  int nargs = 1; 
  if(argc<(nargs*2+1)){ Usage(); exit(1); }
  for(int i = 1; i < argc; i+=2){
    if(string(argv[i]) == "-n") fNDFileName = argv[i+1];
    else if(string(argv[i]) == "-e") fNEvts = std::atoi(argv[i+1]);
    else {  
        cout << "Invalid argument:" << argv[i] << " "<< argv[i+1] << endl;
        Usage();
        exit(1);
    }
  } 
}

