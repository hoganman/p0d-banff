//
// Example to show how to reweight events from the SK tree with NIWG2012a systematics//  Run with: 
//
// ./generateWeightsFromSK_NIWGexample.exe -s sk_inputfile [optional nevents] 
// 
// Jim Dobson       - 28/03/2011
// Patrick de Perio - 15/04/2011
// Kendall Mahn - 29/08/2011
// Kendall Mahn - 9/03/2012
// Kendall Mahn - 9/10/2012
// 
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
 TTree * SK_tree = (TTree*) SK_infile->Get("h1");
 if(!SK_tree){
   cerr << "Cannot find SK_tree!" << endl;
 }
 

 // Instantiate the reader of the SK tree class
 SK::SK__h1 *skVtxs = new SK::SK__h1(SK_tree,1);

 const int ndraws = 7*9; // -3 sigma ... +3 sigma (7 variations, 9 dials)


#ifdef __T2KRW_NIWG_ENABLED__
 // Make a t2kreweighting object and add a NIWG weighting engine. 
 t2krew::T2KReWeight rw; 
 rw.AdoptWghtEngine("niwg_rw", new t2krew::T2KNIWGReWeight());

 // NIWG 2012a
  rw.Systematics().Include(t2krew::kNIWG2012a_pf);
  rw.Systematics().Include(t2krew::kNIWG2012a_eb);
  rw.Systematics().Include(t2krew::kNIWG2012a_sf);
  rw.Systematics().Include(t2krew::kNIWG2012a_cccohE0);
  rw.Systematics().Include(t2krew::kNIWG2012a_dismpishp);
  rw.Systematics().Include(t2krew::kNIWG2012a_nccohE0);
  rw.Systematics().Include(t2krew::kNIWG2012a_ncotherE0);
  rw.Systematics().Include(t2krew::kNIWG2012a_nc1piE0);
  rw.Systematics().Include(t2krew::kNIWG2012a_ccnueE0);
  rw.Systematics().Include(t2krew::kNIWGDeltaMass_width);

  // loop over total variations, changing each tweak dial accordingly
 for(int dial=0; dial<ndraws; dial++){
   // KM
   // Absolute tweak dials set the fractional uncertainty, instead of 
   // in units of "sigma", defined in the code.
   // Useful so that you define the uncertainty within the code, as what is
   // hardcoded may not be the same as what is used for analysis. 
   // See TN113 for details.
   rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_pf);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_eb);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_sf);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_cccohE0);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_dismpishp);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_nccohE0);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_ncotherE0);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_nc1piE0);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_ccnueE0);
  rw.Systematics().SetAbsTwk(t2krew::kNIWGDeltaMass_width);

    Double_t dismpidial = 0.;
    Double_t sfdial = 0.;
    Double_t ebdial = 0.;
    Double_t pfdial = 0.;
    Double_t cccohdial = 0.;
    Double_t nccohdial = 0.;
    Double_t ncothdial = 0.;
    Double_t ccnuedial = 0.;
    Double_t mdeldial = 0.;

    if(dial>=0&&dial<=6){
     dismpidial = (dial-3-7*0.)*0.4;
    }else if(dial>=7&&dial<=13){
      sfdial = (dial-3-7*1.)*1.0;
    }else if(dial>=14&&dial<=20){
      ebdial = (dial-3-7*2.)*0.33333; //v3 fractional shift 9 MeV/27 MeV for SK O target = 0.333
    }else if(dial>=21&&dial<=27){
      pfdial = (dial-3-7*3.)*0.13333; //v3 fractional shift 30 MeV/c / 225 MeV/c for SK O target = 0.13333
    }else if(dial>=28&&dial<=34){
      cccohdial = (dial-3-7*4)*1.0;
    }else if(dial>=35&&dial<=41){
      nccohdial = (dial-3-7*5)*0.30; //v3 absolute 30% uncertainty
    }else if(dial>=42&&dial<=48){
      ncothdial = (dial-3-7*6)*0.30; //v3  absolute 30% uncertainty
    }else if(dial>=49&&dial<=55){
      ccnuedial = (dial-3-7*7)*0.06; //v3 absolute 6% uncertainty
    }else if(dial>=56&&dial<=62){
      mdeldial = (dial-3-7*8)*0.521; //v2 fractional shift 82 MeV/118 MeV 
    }

   rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_dismpishp, dismpidial);
   rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_sf, sfdial); 
   rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_eb, ebdial);
   rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_pf, pfdial);   
   rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_cccohE0, cccohdial);
   rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_nccohE0, nccohdial);
   rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_ncotherE0, ncothdial);
   rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_nc1piE0, ncothdial);
   rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_ccnueE0, ccnuedial);
   rw.Systematics().SetTwkDial( t2krew::kNIWGDeltaMass_width, mdeldial);
    
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

 } // index of tweak dial changes


 cout << "Outputfile testweights_skexample_nue.root has weight storer tree"  << endl;


 storer->SaveToFile(); // save the weights to a file
 #endif // __T2KRW_NIWG_ENABLED__
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

