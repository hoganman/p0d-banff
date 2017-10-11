//
// Simple example to show how to reweight events from a JnuBeam file
// Run with:
//
// ./genWeightsFromJnuBeam.exe -n nd_inputfile -s sk_inputfile [-e nevents]
//   
// K. Mahn, P. de Perio Apr 13 2011
// 
#include <stdlib.h>
#include <iostream>
#include <cstdlib>

#include "TFile.h"
#include "TTree.h"

#include "T2KReWeight.h"
#include "T2KSyst.h"

#include "T2KJNuBeamReWeight.h"
//#ifdef __T2KRW_JNUBEAM_ENABLED__
#include "JnuBeamRead.h"
//#endif 

using std::cout;
using std::cerr;
using std::endl;

using namespace t2krew;

int fNEvts = -1;
char * fNDFileName;
char * fSKFileName;

void Usage();
void ParseArgs(int argc, char **argv);

int main(int argc, char *argv[])
{
// This example only works when compiled against oaAnalysis
#ifdef __T2KRW_OAANALYSIS_ENABLED__
 ParseArgs(argc, argv);

 cout << "Starting to reweight GENIE events from ND file: NOT USED " << fNDFileName << endl;
 cout << "Starting to reweight JnuBeam events from a JnuBeam file: " << fSKFileName << endl;

 TH1D **hist = new TH1D*[20];
 for(int i=0; i<20; i++){
   char hname[50];
   sprintf(hname,"sk_hist_%d",i);
   hist[i] = new TH1D(hname,hname,100,0,10);
 }

 // Load the JnuBeam tree
 // Instantiate the JnuBeamRead class
 JnuBeamRead *jnubeam = new JnuBeamRead(fSKFileName,"11a",0);
 //syntax: filename, JnuBeamVersion  bool == 0 for SK
 cout << "Warning: JnuBeam version hardcoded to 11a" << endl;

 int Njnubeam = jnubeam->GetEntries(); // number of entries

  // Make a t2kreweighting object and add a GENIE weighting engine. In future 
 // can add NEUT and JNuBeam as well.
 t2krew::T2KReWeight rw; 
 rw.AdoptWghtEngine("jnubeam_rw", new t2krew::T2KJNuBeamReWeight());
 T2KJNuBeamReWeight* rw_ptr = (T2KJNuBeamReWeight*)rw.WghtEngine("jnubeam_rw");
 rw_ptr->MakeThrows(20,20110416);
 std::vector<double> throw_vec;
 //rw->UseParameterSet(1,throw_vec);
 (((T2KJNuBeamReWeight*)(rw.WghtEngine("jnubeam_rw")))->GetWghtCalc("bmpt_kaon"))->SetMode(0);

 ///////////////////////////////////////////////
 
 // Loop over input file entries and calculate weights.
 if(fNEvts < 0) fNEvts = jnubeam->GetEntries(); // number of entries in JnuBeam file
 cout << "Will reweight: " << fNEvts << endl;
 for(int i = 0; i < fNEvts; i++){
   jnubeam->GetEntry(i);
   double weight;
   for(int j=0; j<20; j++){
      rw_ptr->UseParameterSet(j, throw_vec );
      weight = rw.CalcWeight(jnubeam);
      //cout << "    -->  weight = " << weight << " "  << jnubeam->Enu << endl;
      hist[j]->Fill(jnubeam->Enu,weight*jnubeam->norm);
   } 
 }

 TFile *outfile = new TFile("test.root","RECREATE");
 for(int i=0; i<20; i++) hist[i]->Write();
 outfile->Close();
 
#endif
 //#endif // __T2KRW_JNUBEAM_ENABLED__ // need to set up ifdefs
 return 0;
}

// Print the cmd line syntax
void Usage(){
 
  std::cout << "Cmd line syntax should be:" << std::endl;
  std::cout << "genWeightsFromNJnuBeam.exe -n nd_inputfile -s sk_inputfile [-e nevents]" << std::endl;
 
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

