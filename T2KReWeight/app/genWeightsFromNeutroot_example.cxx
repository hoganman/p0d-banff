//
// Example to show how to reweight events from a neutroot file with NIWG2010a systematics//  Run with: 
//
// ./genWeightsFromNeutroot_example.exe -i input_neutroot_file.root -h 'NIWG2012a_pf NIWG2012a_eb' -v '0.2 0.1' -o output.root
// 
#include <stdlib.h>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"

#include "T2KReWeight.h"
#include "T2KSyst.h"

#include "T2KNeutReWeight.h"
#include "T2KNIWGReWeight.h"

// For weight storer class
#include "T2KWeightsStorer.h"

#include "neutrootTreeSingleton.h"
using std::cout;
using std::cerr;
using std::endl;

using namespace t2krew;

int fNnrEvts = -1;
string fNRFileName;

void Usage();
int ParseArgs(int argc, char *argv[]);
void include_systematics(t2krew::T2KReWeight *rw, vector<string> handles);
vector<string> separate(string input_uptions);
string handles_input="";
string fixed_values_input="";
string out_string="output.root";

int main(int argc, char *argv[])
{

 
  // process the arguments
  int args = ParseArgs(argc, argv);
  if(args != 0){
    std::cerr << "Usage " << std::endl;
    return 0;
  }

  cout << "Starting to reweight NIWG events from Neutroot file: " << fNRFileName << endl;


  vector<string> handles = separate(handles_input);
  vector<string> fixed_values = separate(fixed_values_input);

  if(fixed_values.size()!=handles.size()){
    cout << "Error: number of handles and number of fixed values are not equal" << endl;
    exit(1);
  }

 // Load the NR "neuttree" tree
 TFile * NR_infile = new TFile(fNRFileName.c_str(), "OPEN");
 if(!NR_infile){
   cerr << "Cannot open neutroot file!" << endl;
   exit(1);
 }
 TTree * NR_tree = (TTree*) NR_infile->Get("neuttree");
 if(!NR_tree){
   cerr << "Cannot find NR_tree!" << endl;
 }

  TH1D* flux_numu = (TH1D*)NR_infile->Get("flux_numu");
  TH1D* evtrt_numu = (TH1D*)NR_infile->Get("evtrt_numu");

  if(!flux_numu){ cout << "Error: couldn't find flux histogram" << endl; exit(1);}  
  if(!evtrt_numu){ cout << "Error: couldn't find event rate histogram" << endl; exit(1);}  


  cout << "Saving output file as: " << out_string << endl;
  T2KWeightsStorer *storer = new T2KWeightsStorer(out_string.c_str()); //forweightstorer

 // Instantiate the reader of the neutroot tree class
  NeutrootTreeSingleton * neutrootTreeObj = NeutrootTreeSingleton::Instance(fNRFileName);
  NeutVect *nvect = neutrootTreeObj->GetNeutVectAddress();

  TChain *f_mc_chain = neutrootTreeObj->tree_neutroot;
  int f_nEvents = neutrootTreeObj->f_nEvents;

#ifdef __T2KRW_NIWG_ENABLED__
 // Make a t2kreweighting object and add a NIWG weighting engine. 
 t2krew::T2KReWeight rw; 
 rw.AdoptWghtEngine("niwg_rw", new t2krew::T2KNIWGReWeight());
 rw.AdoptWghtEngine("neut_rw", new t2krew::T2KNeutReWeight());

 include_systematics(&rw, handles);

 // Set tweaked systematics values in units of 1 sigma deviation.
 // No absolute variation possible for NIWG2010 uncertainties
  for(u_int i = 0; i < handles.size(); i++){
    rw.Systematics().SetTwkDial(T2KSyst::FromString(handles[i]),atof(fixed_values[i].c_str()));
  }

  rw.Reconfigure();
  storer->NewSystSet(rw.Systematics()); // save the current twk dial values to the weight storer class

  // Loop over entries and calculate weights.
  if(fNnrEvts < 0) fNnrEvts = f_nEvents;


  for(int i = 0; i < fNnrEvts; i++){
    f_mc_chain->GetEntry(i);
    
    //  if(abs(nvect->Mode)!=1)continue ; //If you want histograms of CCQE only
    if (i%10000==0) cout << "Getting weight for event = "<< i << " with EvtCode: "<< abs(nvect->Mode)  << endl; //Maybe you want to know the progress
    
    Double_t weight = 1;
    weight = rw.CalcWeight(f_mc_chain,i);
    storer->AddWeight(weight);

  } 

  storer->SaveToFile(); // save the weights to a file
  delete storer; 

 #endif // __T2KRW_NIWG_ENABLED__
 
 return 0;
}

// Print the cmd line syntax
void Usage(){
  cout << "Cmd line syntax should be:" << endl;
  cout << "generateWeightsFromNeutroot.exe -i neutroot_inputfile -h 'T2KReWeight handles' -v 'tweak values' " << endl;
}

int ParseArgs(int argc, char **argv){

  while( (argc > 1) && (argv[1][0] == '-') ){

    switch(argv[1][1]){
    case 'i':
      fNRFileName = argv[2];
      ++argv; --argc;
      break;
    case 'h':
      handles_input = argv[2];
      ++argv; --argc;
      break;
    case 'v':
      fixed_values_input = argv[2];
      ++argv; --argc;
      break;
    case 'o':
      out_string = argv[2];
      ++argv; --argc;
      break;
    }

    ++argv; --argc;
  }

  return 0;

}

vector<string> separate(string input_options){
  vector<string> options_string;
  std::istringstream iss(input_options);
  while(iss){
    string sub;
    iss >> sub;
    options_string.push_back(sub);
    cout << sub << endl;
  }
  options_string.pop_back();
  return options_string;
}

void include_systematics(t2krew::T2KReWeight *rw, vector<string>handles){

  for(unsigned int i=0;i<handles.size();i++){
    rw->Systematics().Include(T2KSyst::FromString(handles[i]));
    rw->Systematics().SetAbsTwk(T2KSyst::FromString(handles[i]));
  }
  return;

}
