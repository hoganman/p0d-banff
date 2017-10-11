//
// 2015 NIWG-oscillation analysis dials for spline generation.
// See TN192 for details
//
//  Run with: 
//
// ./generateWeightsFromSK_NIWG2015.exe -s sk_inputfile [optional nevents] 
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
 TTree * SK_tree;
 SK_tree = (TTree*) SK_infile->Get("h1");
 if(!SK_tree){
   cerr << "Cannot find tree h1! Looking for minituple instead" << endl;
   SK_tree = (TTree*) SK_infile->Get("minituple");
 }
 

 // Instantiate the reader of the SK tree class
 SK::SK__h1 *skVtxs = new SK::SK__h1(SK_tree,1);

 const int ndraws = 7*18+3+6; // -3 sigma ... +3 sigma (7 variations, 18 dials + 9 tunings - 7 individual, one with all CCQE tunings, and one where all tunings are applied)


#ifdef __T2KRW_NIWG_ENABLED__
#ifdef __T2KRW_NEUT_ENABLED__
 // Make a t2kreweighting object and add a NIWG and NEUT weighting engine. 
 t2krew::T2KReWeight rw; 
 rw.AdoptWghtEngine("neut_rw", new t2krew::T2KNeutReWeight());
 rw.AdoptWghtEngine("niwg_rw", new t2krew::T2KNIWGReWeight());
 
 // NIWG 2015
 // Tuning for CCQE
 rw.Systematics().Include(t2krew::kNIWG2014a_SF_RFG);
 rw.Systematics().Include(t2krew::kNIWG_rpaCCQE_norm);
  rw.Systematics().Include(t2krew::kNIWG_rpaCCQE_shape);

 // Uncertainties
  // CCQE:
  rw.Systematics().Include(t2krew::kNIWG2014a_pF_C12);
  rw.Systematics().Include(t2krew::kNIWG2014a_pF_O16);
  rw.Systematics().Include(t2krew::kNIWG2014a_Eb_C12);
  rw.Systematics().Include(t2krew::kNIWG2014a_Eb_O16);
  rw.Systematics().Include(t2krew::kNIWGMEC_Norm_C12);
  rw.Systematics().Include(t2krew::kNIWGMEC_Norm_O16);  
  rw.Systematics().Include(t2krew::kNXSec_MaCCQE);
  rw.Systematics().Include(t2krew::kNXSec_VecFFCCQE); // used to set MAQE to act according to for RFG MC (2) or SF MC (402)
 
  // CC and NC single pion resonance:
  rw.Systematics().Include(t2krew::kNXSec_CA5RES);
  rw.Systematics().Include(t2krew::kNXSec_MaNFFRES);
  rw.Systematics().Include(t2krew::kNXSec_BgSclRES);
  
  // nue/numu uncertainties
  rw.Systematics().Include(t2krew::kNXSec_SCCVecQE);
  rw.Systematics().Include(t2krew::kNXSec_SCCAxlQE);
  rw.Systematics().Include(t2krew::kNXSec_PsFF);
  rw.Systematics().Include(t2krew::kNIWG2012a_ccnueE0); 

  // All other CC and NC
  rw.Systematics().Include(t2krew::kNIWG2012a_dismpishp);
  rw.Systematics().Include(t2krew::kNIWG2012a_cccohE0);
  rw.Systematics().Include(t2krew::kNIWG2012a_nccohE0);
  rw.Systematics().Include(t2krew::kNIWG2012a_ncotherE0); 

   // Absolute tweak dials set the fractional uncertainty, instead of 
   // in units of "sigma", defined in the code.
   // Useful so that you define the uncertainty within the code, as what is
   // hardcoded may not be the same as what is used for analysis. 

  // CCQE:
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2014a_pF_C12);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2014a_pF_O16);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2014a_Eb_C12);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2014a_Eb_O16);
  rw.Systematics().SetAbsTwk(t2krew::kNIWGMEC_Norm_C12);
  rw.Systematics().SetAbsTwk(t2krew::kNIWGMEC_Norm_O16);  
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_MaCCQE);
   
  // CC and NC single pion resonance:
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_CA5RES);
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_MaNFFRES);
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_BgSclRES);
  
  // nue/numu uncertainties
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_SCCVecQE);
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_SCCAxlQE);
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_PsFF);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_ccnueE0); 

  // All other CC and NC
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_dismpishp);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_cccohE0);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_nccohE0);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_ncotherE0);

  // RPA tuning takes AbsTwk too
  rw.Systematics().SetAbsTwk(t2krew::kNIWG_rpaCCQE_norm);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG_rpaCCQE_shape);

  // loop over total variations, changing each tweak dial accordingly
  // (start at dial=126 for tunings only)
  for(int dial=0; dial<ndraws; dial++){
  
    Double_t pfcdial = 0.;
    Double_t pfodial = 0.;
    Double_t ebcdial = 0.;
    Double_t ebodial = 0.;
    Double_t meccdial = 0.;
    Double_t mecodial = 0.;
    Double_t maqedial = 0.;

    Double_t cadial = 0.;
    Double_t manresdial = 0.;
    Double_t bkgdial = 0.;

    Double_t sccvdial = 0.;
    Double_t sccadial = 0.;
    Double_t fpdial = 0.;
    Double_t ccnuedial = 0.;

    Double_t dismpidial = 0.;
    Double_t cccohdial = 0.;
    Double_t nccohdial = 0.;
    Double_t ncothdial = 0.;

    if(dial>=0&&dial<=6){
      pfcdial = (dial-3-7*0.)*0.073733; // Nominal is 217 MeV/c for C12.  Fit result was 239 +/- 16 MeV/c. Apply fractional change of 16/217 = 0.073733.
    }else if(dial>=7&&dial<=13){
      pfodial = (dial-3-7*1.)*0.073733; // Nominal is 225 MeV/c for O16 and same fractional error as C12. 
    }else if(dial>=14&&dial<=20){
      ebcdial = (dial-3-7*2.)*0.36; //Nominal is 25+/-9 MeV for C12. 9/25= 0.36 fractional error
    }else if(dial>=21&&dial<=27){
      ebodial = (dial-3-7*3.)*0.33333; //Nominal is 27+/-9 MeV for O16. 9/27= 0.3333 fractional error
    }else if(dial>=28&&dial<=34){
      meccdial = (dial-3-7*4.)*0.25; //Nominal is 1. Fit result is 0.58+/-0.25. Use 0.25 increments, revisit this.
    }else if(dial>=35&&dial<=41){
      mecodial = (dial-3-7*5.)*0.25; //Nominal is 1. Use 0.25 increments.
    }else if(dial>=42&&dial<=48){
      maqedial = (dial-3-7*6.)*0.06667; //Nominal is 1.2. Fit result is 1.02+/-0.08. Apply fractional change of 0.08/1.2 = 0.06667
    }else if(dial>=49&&dial<=55){
      cadial = (dial-3-7*7.)*0.12; // Error is 0.12 from TN-197 (confirmed by Ryan Terri 6/1/15) 
    }else if(dial>=56&&dial<=62){
      manresdial = (dial-3-7*8.)*0.1579; //Need to confirm the nominal. Fit result is 0.95+/-0.15. 
    }else if(dial>=63&&dial<=69){
      bkgdial = (dial-3-7*9.)*0.1538; //Need to confirm the nominal. Fit result is 1.3+/-0.2. 
    }else if(dial>=70&&dial<=76){
      sccvdial = (dial-3-7*10.)*4.4; // 0 (off) to 1 (on). Bounded par? should check if >1 or <1 sigma is meaningful.
    }else if(dial>=77&&dial<=83){
      sccadial = (dial-3-7*11.)*0.15; // 0 (off) to 1 (on). Bounded par? should check if >1 or <1 sigma is meaningful. 
    }else if(dial>=84&&dial<=90){
      fpdial = (dial-3-7*12.)*0.03; // 1+/- 0.03 
    }else if(dial>=91&&dial<=97){
      ccnuedial = (dial-3-7*13.)*0.02; // TN205,1+/-0.02 uncertainty
    }else if(dial>=98&&dial<=104){
      dismpidial = (dial-3-7*14.)*0.4; // Same as 2012/2013. 0+/-0.4
    }else if(dial>=105&&dial<=111){
      cccohdial = (dial-3-7*15.)*1.0; // Same as 2012/2013. 1+/-1
    }else if(dial>=112&&dial<=118){
      nccohdial = (dial-3-7*16.)*0.30; // Same as 2012/2013. 1+/-0.30
    }else if(dial>=119&&dial<=125){
      ncothdial = (dial-3-7*17.)*0.30; // Same as 2012/2013. 1+/-0.30
    }

    // Special dials for applying NIWG 2015 tuning (Note: MEC_O and pF_O instead of MEC_C and pF_C, but using the same fractional change)
    else if(dial==129){
      maqedial = -0.15; // MaQE tuning: 1.2 -> 1.02 (1.02/1.2 - 1 = -0.15)
    }
    else if(dial==130){
      mecodial = -0.42; // MEC_norm_C tuning: 1 -> 0.58 (0.58/1 - 1 = -0.42)
    }
    else if(dial==131){
      pfodial = 0.1014; // pF_C tuning: 217 -> 239 (239/217 - 1 = 0.1014)
    }
    else if(dial==132){
      maqedial = -0.15; // All 3 of the above tunings applied together
      mecodial = -0.42;
      pfodial = 0.1014;
    }
    else if(dial==133){
      maqedial = -0.15; // All tunings applied (including the three below)
      mecodial = -0.42;
      pfodial = 0.1014;
    }


    rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_pF_C12, pfcdial);
    rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_pF_O16, pfodial);
    rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_Eb_C12, ebcdial);
    rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_Eb_O16, ebodial);
    rw.Systematics().SetTwkDial(t2krew::kNIWGMEC_Norm_C12, meccdial);
    rw.Systematics().SetTwkDial(t2krew::kNIWGMEC_Norm_O16, mecodial);
    rw.Systematics().SetTwkDial(t2krew::kNXSec_MaCCQE, maqedial);
    rw.Systematics().SetTwkDial(t2krew::kNXSec_VecFFCCQE, 2); // Note that when we set MAQE, we need to set NXSec_VecFFCCQE to 2 for SF->RFG MC

    rw.Systematics().SetTwkDial(t2krew::kNXSec_CA5RES, cadial);
    rw.Systematics().SetTwkDial(t2krew::kNXSec_MaNFFRES, manresdial);
    rw.Systematics().SetTwkDial(t2krew::kNXSec_BgSclRES, bkgdial);

    rw.Systematics().SetTwkDial(t2krew::kNXSec_SCCVecQE, sccvdial);
    rw.Systematics().SetTwkDial(t2krew::kNXSec_SCCAxlQE, sccadial);
    rw.Systematics().SetTwkDial(t2krew::kNXSec_PsFF, fpdial);
    rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_ccnueE0, ccnuedial);
    
    rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_dismpishp, dismpidial);
    rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_cccohE0, cccohdial);
    rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_nccohE0, nccohdial);
    rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_ncotherE0, ncothdial);
    
    // Relative tweak dials used for the tuning only set for tuning cases.
    if(dial==126 || dial==133){
      rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_SF_RFG,1); // SF->RFG tuning
    }else{
      rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_SF_RFG,0);
    }

    if(dial==127 || dial==133){       // Non Rel RPA correction, default tuning
      rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_SF_RFG,1);
      rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_norm,1);
      rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_shape,-1);
    }else if(dial==128 || dial==134){ // Rel RPA correction
      rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_SF_RFG,1);
      rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_norm,1);
      rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_shape,0);
    }else{
      rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_norm,0);
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

   // If SF->RFG weight is larger than 5, print out everything
   /* if ((dial==126 || dial==133) && weight>5)
     {
       cout << "Entry " << i << endl
	    << "weight = " << weight << endl
	    << "Values from SK__h1:" << endl
	    << "pnu[0] = " << skVtxs->pnu[0] << endl
	    << "P = " << pow(skVtxs->Pvc[2][0]*skVtxs->Pvc[2][0]+skVtxs->Pvc[2][1]*skVtxs->Pvc[2][1]+skVtxs->Pvc[2][2]*skVtxs->Pvc[2][2],0.5)/1000 << endl
	    << "costh = " << (skVtxs->Pvc[2][0]*skVtxs->Pvc[0][0]+skVtxs->Pvc[2][1]*skVtxs->Pvc[0][1]+skVtxs->Pvc[2][2]*skVtxs->Pvc[0][2])/(pow(skVtxs->Pvc[0][0]*skVtxs->Pvc[0][0]+skVtxs->Pvc[0][1]*skVtxs->Pvc[0][1]+skVtxs->Pvc[0][2]*skVtxs->Pvc[0][2],0.5)*pow(skVtxs->Pvc[2][0]*skVtxs->Pvc[2][0]+skVtxs->Pvc[2][1]*skVtxs->Pvc[2][1]+skVtxs->Pvc[2][2]*skVtxs->Pvc[2][2],0.5)) <<endl << "---------" << endl << endl;

       Enu->Fill(skVtxs->pnu[0]*1000);
       P->Fill(skVtxs->Pvc[2][0]);
       costh->Fill(((skVtxs->dirv[0][0])*(skVtxs->dir[0][0]))+((skVtxs->dirv[0][1])*(skVtxs->dir[0][1]))+((skVtxs->dirv[0][2])*(skVtxs->dir[0][2])));
	 }*/
   } // event loop

 } // index of tweak dial changes


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

