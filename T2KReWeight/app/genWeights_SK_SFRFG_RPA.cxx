// Generates a single set of weights for SF->RFG tuning, as well as two sets
// of RPA weights (one for non-rel RPA and one for rel RPA).
//
// Note: calculate both RPA weights relative to untuned MC. Eventually will be 
// applied on top of SF->RFG tuning (and, in the case of rel RPA, non-rel
// RPA tuning), but we do not consider the errors associated with these two.
// (Assume weights are multiplicative)
//
// Run with: 
//
// ./genWeights_SK_SFRFG_RPA.exe -s sk_inputfile
//
//
// Uses NIWGReWeight2015 and NEUT 3.3.3. Source the correct versions of 
// everything by doing:
// source ~/T2KReWeight2015/T2KReWeight_HEAD2015/Setup/setup.sh


#include <stdlib.h>
#include <cstdlib>

#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TString.h"

#include "T2KReWeight.h"
#include "T2KSyst.h"

#include "T2KGenieReWeight.h" 
#include "T2KNeutReWeight.h"
#include "T2KJNuBeamReWeight.h"
#include "T2KNIWGReWeight.h"

#include "SK__h1.h"
// For weight storer class
#include "T2KWeightsStorer.h"


using std::cout;
using std::cerr;
using std::endl;

using namespace t2krew;

int fNskEvts = -1;
TString fSKFileName;

void Usage();
int ParseArgs(int argc, char *argv[]);

int main(int argc, char *argv[])
{
 
  // process the arguments
  int args = ParseArgs(argc, argv);
  if(args != 0){
    std::cerr << "Usage " << std::endl;
    return 0;
  }
  T2KWeightsStorer *storer = new T2KWeightsStorer("testweights_skexample_nue.root"); //forweightstorer
  
  cout << "Starting to reweight NIWG events from SK file: " << fSKFileName << endl;
  
  
  // Load the SK "h1" tree
  TFile * SK_infile = new TFile(fSKFileName, "OPEN");
  if(!SK_infile){
    cerr << "Cannot open SK file!" << endl;
   exit(1);
  }
  TTree * SK_tree;
  SK_tree = (TTree*) SK_infile->Get("h1");
  if(!SK_tree){
    cerr << "Cannot find tree h1! Looking for mtuple instead" << endl;
    SK_tree = (TTree*) SK_infile->Get("mtuple");
  }
  if(!SK_tree){
    cerr << "Cannot find tree mtuple! Looking for minituple instead" << endl;
    SK_tree = (TTree*) SK_infile->Get("minituple");
  }
  if(!SK_tree){
    cerr << "Cannot find tree mtuple! This is a real problem." << endl;
  }
  
  std::cout << "Number of entries in SK_tree: " << SK_tree->GetEntries() << std::endl;
  
  // Instantiate the reader of the SK tree class
  SK::SK__h1 *skVtxs = new SK::SK__h1(SK_tree,1);
  std::cout << "Number of entries in skVtxs: " << skVtxs->GetEntries() << std::endl;
 
#ifdef __T2KRW_NIWG_ENABLED__
#ifdef __T2KRW_NEUT_ENABLED__

  // Make a t2kreweighting object and add a NIWG weighting engine. 
  t2krew::T2KReWeight rw; 
  rw.AdoptWghtEngine("niwg_rw", new t2krew::T2KNIWGReWeight());
  
  // Include systematics
  rw.Systematics().Include(t2krew::kNIWG2014a_SF_RFG);
  rw.Systematics().Include(t2krew::kNIWG_rpaCCQE_norm);
  rw.Systematics().Include(t2krew::kNIWG_rpaCCQE_shape);

  // Set Abs Twk for RPA dials only
  rw.Systematics().SetAbsTwk(t2krew::kNIWG_rpaCCQE_norm);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG_rpaCCQE_shape);


  // Create 3 variations (one for SF->RFG, one for non-rel RPA and one for rel RPA)
  for (int dial=0; dial<3; dial++)
    {
      if (dial==0) // SF->RFG tuning
	rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_SF_RFG,1); 
      else
	rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_SF_RFG,0);
      
      if (dial==1) // non-rel RPA tuning
	{
	  rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_norm,1);
	  rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_shape,-1);
	}

      if (dial==2) // rel RPA tuning
	{
	  rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_norm,1);
	  rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_shape,0);
	}
      
      rw.Reconfigure();
      storer->NewSystSet(rw.Systematics()); // save the current twk dial values to the weight storer class
      
      
      // Loop over SK entries and calculate weights.
      if(fNskEvts < 0) fNskEvts = skVtxs->GetEntries();
      
      for(int i = 0; i < fNskEvts; i++){
	
	skVtxs->GetEntry(i);
	Double_t weight =1.0;
	weight = rw.CalcWeight(skVtxs);
	storer->AddWeight(weight); // add weight for each
      } // event loop
    } // loop over variations index


  cout << "Outputfile testweights_skexample_nue.root has weight storer tree"  << endl;


 storer->SaveToFile(); // save the weights to a file
 #endif // __T2KRW_NIWG_ENABLED__
 #endif // __T2KRW_NEUT_ENABLED__
 delete storer; 

 
 return 0;
}

// Print the cmd line syntax
void Usage(){
  cout << "Cmd line syntax should be:" << endl;
  cout << "generateWeightsFromSK_NIWGexample.exe -s sk_inputfile [-e nevents]" << endl;
}

int ParseArgs(int argc, char **argv){

  while( (argc > 1) && (argv[1][0] == '-') ){

    switch(argv[1][1]){
    case 's':
      fSKFileName    = argv[2];
      ++argv; --argc;
      if (argc <= 2) break;
      else if ( argv[2][0] != '-' ){
	fNskEvts = atoi(argv[2]);
        ++argv; --argc;
      }
      break;

    }
    ++argv; --argc;
  }
  return 0;
}

