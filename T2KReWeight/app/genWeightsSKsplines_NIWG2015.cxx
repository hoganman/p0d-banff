//
// 2015 NIWG-oscillation analysis dials for spline generation.
// See TN192 for details
//
//  Run with: 
//
// ./generateWeightsFromSK_NIWG2015.exe -s sk_inputfile [optional nevents] 
//
//
// Uses NIWGReWeight2015 and NEUT 3.3.3. Source the correct versions of 
// everything by doing:
// source /data/t2k/duffy/software/T2KReWeight/T2KReWeight2015/T2KReWeight_HEAD2015/Setup/setup.sh





// 
// Systematics for splines (in order):
//
// These ones have 13 dials (-3 sigma to 3 sigma in 0.5 sigma steps):
// MaQE
// Fermi momentum, pF (O)
// Binding energy, Eb (O)
// CA5
// 1pi axial form factor, MaNFFRES
// Isospin=1/2 background normalisation (BgSclRES)
// CC other shape uncertainty, dismpishp
// Second class currents (vector), SCCVecQE
// Second class currents (axial), SCCAxlQE
//
// This one has 1 dial only (tuning on):
// Rel RPA (note: calculate weights relative to untuned MC. Eventually will be applied on top of SF->RFG tuning and Non-rel RPA tuning, but we do not consider the errors associated with these two)
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
  
  const int ndraws = 13*9+2+7+21*2+16; // -3 sigma ... +3 sigma in 0.5sigma steps (13 variations) for 9 parameters, plus 2 RPA tunings, plus 7 extra tweaks (-7 sigma to -3.5 sigma) for MaQE, plus SF-mode MaQE and pFsf (both 10 sigma in 0.5 sigma steps), plus MEC q3 (16 steps)
  
  
#ifdef __T2KRW_NIWG_ENABLED__
#ifdef __T2KRW_NEUT_ENABLED__
  // Make a t2kreweighting object and add a NIWG and NEUT weighting engine. 
  t2krew::T2KReWeight rw; 
  rw.AdoptWghtEngine("neut_rw", new t2krew::T2KNeutReWeight());
  rw.AdoptWghtEngine("niwg_rw", new t2krew::T2KNIWGReWeight());
  
  // NIWG 2015
  // Uncertainties
  rw.Systematics().Include(t2krew::kNXSec_MaCCQE);
  rw.Systematics().Include(t2krew::kNIWG2014a_pF_O16);
  rw.Systematics().Include(t2krew::kNIWG2014a_Eb_O16); 
  rw.Systematics().Include(t2krew::kNXSec_CA5RES);
  rw.Systematics().Include(t2krew::kNXSec_MaNFFRES);
  rw.Systematics().Include(t2krew::kNXSec_BgSclRES);
  rw.Systematics().Include(t2krew::kNIWG2012a_dismpishp);
  rw.Systematics().Include(t2krew::kNXSec_SCCVecQE);
  rw.Systematics().Include(t2krew::kNXSec_SCCAxlQE);
  rw.Systematics().Include(t2krew::kNIWGSpectralFunc_pFsfO16_smooth);
  rw.Systematics().Include(t2krew::kNIWGMEC_q3Cut);
  
  // Tunings
  rw.Systematics().Include(t2krew::kNIWG_rpaCCQE_norm);
  rw.Systematics().Include(t2krew::kNIWG_rpaCCQE_shape);
  rw.Systematics().Include(t2krew::kNXSec_VecFFCCQE); // used to set MAQE to act according to for RFG MC (2) or SF MC (402). Should always be set to 2 to ensure that we get the correct splines when SF -> RFG tuning is applied later, except for in the case of SF mode MAQE.
  
  
  // Absolute tweak dials set the fractional uncertainty, instead of 
  // in units of "sigma", defined in the code.
  // Useful so that you define the uncertainty within the code, as what is
  // hardcoded may not be the same as what is used for analysis. 
  
  // Uncertanties:
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_MaCCQE);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2014a_pF_O16);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2014a_Eb_O16); 
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_CA5RES);
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_MaNFFRES);
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_BgSclRES);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_dismpishp);
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_SCCVecQE);
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_SCCAxlQE);
  rw.Systematics().SetAbsTwk(t2krew::kNIWGSpectralFunc_pFsfO16_smooth);
  rw.Systematics().SetAbsTwk(t2krew::kNIWGMEC_q3Cut);
   
  // Tunings:
  rw.Systematics().SetAbsTwk(t2krew::kNIWG_rpaCCQE_norm);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG_rpaCCQE_shape);
  

  // Set 1 sigma step for each systematic (uncertainties only)
  Double_t maqeerror = 0.14631; //Taken from Xsec covariance matrix 29/1/15
  Double_t pfoerror = 0.073733; // Nominal is 225 MeV/c for O16 and same fractional error as C12. Nominal is 217 MeV/c for C12.  Fit result was 239 +/- 16 MeV/c. Apply fractional change of 16/217 = 0.073733.
  Double_t eboerror = 0.3333; //Nominal is 27+/-9 MeV for O16. 9/27= 0.3333 fractional error
  Double_t caerror = 0.12; //See TN-197 (confirmed by Ryan Terri 6/1/15)
  Double_t manreserror = 0.1579; //Need to confirm the nominal. Fit result is 0.95+/-0.15. 0.15/0.95 = 0.1579
  Double_t bkgerror = 0.1538; //Need to confirm the nominal. Fit result is 1.3+/-0.2. 0.2/1.3 = 0.1538
  Double_t dismpierror = 0.4; // Same as 2012/2013. 0+/-0.4
  Double_t sccvecerror = 1.0; // Is this right?
  Double_t sccaxlerror = 1.0; // Is this right?
  Double_t pfsferror = 0.0191; // Nominal is 209 MeV. Fit result is 234+/-4. Apply fractional change of 4/209 = 0.0191
  Double_t q3error = 0.0333; // Even spacing: 15 points with fractional changes between -0.25 and 1, at spacing of 1/60 (so 'sigma' = 1/30).

  // Note: decided to put in pF/Eb points by hand to ensure they cover the 'valid' range in the correct way. Valid range is: 
  // Eb: NEUT nominal = 27MeV. Valid for 12-42 (so tweaks of -0.5556 - 0.5556)
  // pF: NEUT nominal = 225MeV/c. Valid for 200 - 275 (so tweaks of -0.1111 - 0.2222)

  std::cout << std::endl
            << "-------------------------------------------------" << std::endl
            << "1-sigma errors are set as: " << std::endl
            << "MaQE: " << maqeerror << std::endl
            << "pF (O): put in by hand between boundaries" /*<< pfoerror*/ << std::endl
            << "Eb (O): put in by hand between boundaries" /*<< eboerror*/ << std::endl
            << "Ca5: " << caerror << std::endl
            << "MaNFFRES: " << manreserror << std::endl
            << "BgSclRES: " << bkgerror << std::endl
            << "Dismpishp: " << dismpierror << std::endl
	    << "SCCVec: " << sccvecerror << std::endl
	    << "SCCAxl: " << sccaxlerror << std::endl
	    << "pF(SF): " << pfsferror << std::endl
	    << "MEC q3 cut: " << q3error << std::endl
            << "-------------------------------------------------"
            << std::endl << std::endl;


  // loop over total variations, changing each tweak dial accordingly
  for(int dial=0; dial<=ndraws; dial++){
  
    // Create dials for each systematic
    // Uncertainties
    Double_t maqedial = 0.;
    Double_t pfodial = 0.;
    Double_t ebodial = 0.;
    Double_t cadial = 0.;
    Double_t manresdial = 0.;
    Double_t bkgdial = 0.;
    Double_t dismpidial = 0.;
    Double_t sccvecdial = 0.;
    Double_t sccaxldial = 0.;
    Double_t pfsfdial = 0.;
    Double_t q3dial = 0.;

    rw.Systematics().SetTwkDial(t2krew::kNXSec_VecFFCCQE, 2); // Note that when we set MAQE, we need to set NXSec_VecFFCCQE to 2 for SF->RFG MC. Should be set to 2 all the time, except for SF MAQE variations (127 - 147) to ensure we get the correct behaviour when SF->RFG tuning is applied later.

    // Set uncertainties
    if(dial>=0&&dial<=12){
      maqedial = (dial-6)*0.5*maqeerror; 
    }else if(dial>=13&&dial<=25){
      //pfodial = ((dial-13)-6)*0.5*pfoerror; 
      if (dial==13) pfodial = -0.1333;
      else if (dial==14) pfodial = -0.1111;
      else if (dial==15) pfodial = -0.0889;
      else if (dial==16) pfodial = -0.0667;
      else if (dial==17) pfodial = -0.0444;
      else if (dial==18) pfodial = -0.0222;
      else if (dial==19) pfodial = 0.0;
      else if (dial==20) pfodial = 0.0444;
      else if (dial==21) pfodial = 0.0889;
      else if (dial==22) pfodial = 0.1333;
      else if (dial==23) pfodial = 0.1778;
      else if (dial==24) pfodial = 0.2222;
      else if (dial==25) pfodial = 0.2667;
    }else if(dial>=26&&dial<=38){
      //ebodial = ((dial-26)-6)*0.5*eboerror; 
      if (dial==26) ebodial = -0.6667;
      else if (dial==27) ebodial = -0.5556;
      else if (dial==28) ebodial = -0.4444;
      else if (dial==29) ebodial = -0.3333;
      else if (dial==30) ebodial = -0.2222;
      else if (dial==31) ebodial = -0.1111;
      else if (dial==32) ebodial = 0.0;
      else if (dial==33) ebodial = 0.1111;
      else if (dial==34) ebodial = 0.2222;
      else if (dial==35) ebodial = 0.3333;
      else if (dial==36) ebodial = 0.4444;
      else if (dial==37) ebodial = 0.5556;
      else if (dial==38) ebodial = 0.6667;
    }else if(dial>=39&&dial<=51){
      cadial = ((dial-39)-6)*0.5*caerror; 
    }else if(dial>=52&&dial<=64){
      manresdial = ((dial-52)-6)*0.5*manreserror; 
    }else if(dial>=65&&dial<=77){
      bkgdial = ((dial-65)-6)*0.5*bkgerror; 
    }else if(dial>=78&&dial<=90){
      dismpidial = ((dial-78)-6)*0.5*dismpierror; 
    }
    else if(dial>=91&&dial<=103){
      sccvecdial = ((dial-91)-6)*0.5*sccvecerror;
    }
    else if(dial>=104&&dial<=116){
      sccaxldial = ((dial-104)-6)*0.5*sccaxlerror;
    }
    // dial=117 is rel RPA (see below) [Everything is a bit of a mess from here, sorry!]
    else if(dial>=118 && dial<=125){ // Extra MaQE tweaks (-7 sigma to -3.5 sigma)
      maqedial = ((dial-118)-14)*0.5*maqeerror;
    }
    // dial=126 is non-rel RPA (see below)
    else if(dial>=127 && dial<=147){ // SF-mode MaQE (-7 sigma to +3 sigma)
      maqedial = ((dial-127)-14)*0.5*maqeerror;
      rw.Systematics().SetTwkDial(t2krew::kNXSec_VecFFCCQE, 402);
    }
    else if(dial>=148 && dial<=168){ // pFsf (SF only) (0 to +10 sigma)
      pfsfdial = (dial-148)*0.5*pfsferror;
    }
    else if(dial>=169 && dial<=184){ // MEC q3 cut (16 equally-spaced points)
      q3dial = ((dial-169)-15)*0.5*q3error;
    }

    // Set tuning

    if (dial==117){ // rel RPA tuning
      rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_norm,1);
      rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_shape,0);
    }
    else if (dial==126){ // non-rel RPA tuning
      rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_norm,1);
      rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_shape,-1);
    }
    else {
      rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_norm,0);
      rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_shape,0);
    }

    // Set uncertainty dials

    rw.Systematics().SetTwkDial(t2krew::kNXSec_MaCCQE, maqedial);
    rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_pF_O16, pfodial);
    rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_Eb_O16, ebodial);
    rw.Systematics().SetTwkDial(t2krew::kNXSec_CA5RES, cadial);
    rw.Systematics().SetTwkDial(t2krew::kNXSec_MaNFFRES, manresdial);
    rw.Systematics().SetTwkDial(t2krew::kNXSec_BgSclRES, bkgdial);
    rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_dismpishp, dismpidial);
    rw.Systematics().SetTwkDial(t2krew::kNXSec_SCCVecQE, sccvecdial);
    rw.Systematics().SetTwkDial(t2krew::kNXSec_SCCAxlQE, sccaxldial);
    rw.Systematics().SetTwkDial(t2krew::kNIWGSpectralFunc_pFsfO16_smooth, pfsfdial);
    rw.Systematics().SetTwkDial(t2krew::kNIWGMEC_q3Cut, q3dial);

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

