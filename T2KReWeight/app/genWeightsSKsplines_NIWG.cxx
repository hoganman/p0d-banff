//
// 2016 NIWG-oscillation analysis dials for spline generation.
// See TNXXX for details
//
//  Run with: 
//
// ./generateWeightsFromSK_NIWG2016.exe -s sk_inputfile [optional nevents] 
//
//
// Uses NIWGReWeight2016 and NEUT 3.3.3. Source the correct versions of 
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
  TTree * SK_tree = NULL;
  SK_infile->GetObject("h1", SK_tree);
  if(!SK_tree){
    cerr << "Cannot find tree h1! Looking for mtuple instead" << endl;
    SK_infile->GetObject("mtuple", SK_tree);
  }
  if(!SK_tree){
    cerr << "Cannot find tree mtuple! Looking for minituple instead" << endl;
    SK_infile->GetObject("minituple", SK_tree);
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
  rw.Systematics().Include(t2krew::kNNucl_CCQEBindingEnergy_O16);
  rw.Systematics().Include(t2krew::kNXSec_CA5RES);
  rw.Systematics().Include(t2krew::kNXSec_MaNFFRES);
  rw.Systematics().Include(t2krew::kNXSec_BgSclRES);
  rw.Systematics().Include(t2krew::kNIWG2012a_dismpishp);
  rw.Systematics().Include(t2krew::kNIWGMEC_PDDWeight_O16);
  
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
  rw.Systematics().SetAbsTwk(t2krew::kNNucl_CCQEBindingEnergy_O16);
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_CA5RES);
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_MaNFFRES);
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_BgSclRES);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_dismpishp);
  rw.Systematics().SetAbsTwk(t2krew::kNIWGMEC_PDDWeight_O16);
   
  // Tunings:
  rw.Systematics().SetAbsTwk(t2krew::kNIWG_rpaCCQE_norm);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG_rpaCCQE_shape);
  

  // Set 1 sigma step for each systematic (uncertainties only)
  Double_t maqeerror = 0.14631; //Taken from Xsec covariance matrix 29/1/15
  Double_t caerror = 0.12; //See TN-197 (confirmed by Ryan Terri 6/1/15)
  Double_t manreserror = 0.1579; //Need to confirm the nominal. Fit result is 0.95+/-0.15. 0.15/0.95 = 0.1579
  Double_t bkgerror = 0.1538; //Need to confirm the nominal. Fit result is 1.3+/-0.2. 0.2/1.3 = 0.1538
  Double_t dismpierror = 0.4; // Same as 2012/2013. 0+/-0.4

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
    Double_t mecpddodial = 0.;
    Double_t vecffccqe = 2; // Note that when we set MAQE, we need to set NXSec_VecFFCCQE to 2 for SF->RFG MC. Should be set to 2 all the time, except for SF MAQE variations (127 - 147) to ensure we get the correct behaviour when SF->RFG tuning is applied later.
    Double_t rpanorm = 0;
    Double_t rpashape = 0;


    // Set uncertainties
    switch (dial) { 
      case   0:
      case   1:
      case   2:
      case   3:
      case   4:
      case   5:
      case   6: //< nominal
      case   7:
      case   8:
      case   9:
      case  10:
      case  11:
      case  12:
        maqedial = (dial-6)*0.5*maqeerror; 
        break;

      case  13: { pfodial = -0.1333; break; }
      case  14: { pfodial = -0.1111; break; }
      case  15: { pfodial = -0.0889; break; }
      case  16: { pfodial = -0.0667; break; }
      case  17: { pfodial = -0.0444; break; }
      case  18: { pfodial = -0.0222; break; }
      case  19: { pfodial =  0.    ; break; }
      case  20: { pfodial =  0.0444; break; }
      case  21: { pfodial =  0.0889; break; }
      case  22: { pfodial =  0.1333; break; }
      case  23: { pfodial =  0.1778; break; }
      case  24: { pfodial =  0.2222; break; }
      case  25: { pfodial =  0.2667; break; }

      case  26: { ebodial = -0.6667; break; }
      case  27: { ebodial = -0.5556; break; }
      case  28: { ebodial = -0.4444; break; }
      case  29: { ebodial = -0.3333; break; }
      case  30: { ebodial = -0.2222; break; }
      case  31: { ebodial = -0.1111; break; }
      case  32: { ebodial =  0.    ; break; }
      case  33: { ebodial =  0.1111; break; }
      case  34: { ebodial =  0.2222; break; }
      case  35: { ebodial =  0.3333; break; }
      case  36: { ebodial =  0.4444; break; }
      case  37: { ebodial =  0.5556; break; }
      case  38: { ebodial =  0.6667; break; }

      case  39:
      case  40:
      case  41:
      case  42:
      case  43:
      case  44:
      case  45:
      case  46:
      case  47:
      case  48:
      case  49:
      case  50:
      case  51:
        cadial = ((dial-39)-6)*0.5*caerror; 
        break;

      case  52:
      case  53:
      case  54:
      case  55:
      case  56:
      case  57:
      case  58:
      case  59:
      case  60:
      case  61:
      case  62:
      case  63:
      case  64:
        manresdial = ((dial-52)-6)*0.5*manreserror; 
        break;

      case  65:
      case  66:
      case  67:
      case  68:
      case  69:
      case  70:
      case  71:
      case  72:
      case  73:
      case  74:
      case  75:
      case  76:
      case  77:
        bkgdial = ((dial-65)-6)*0.5*bkgerror; 
        break;

      case  78:
      case  79:
      case  80:
      case  81:
      case  82:
      case  83:
      case  84:
      case  85:
      case  86:
      case  87:
      case  88:
      case  89:
      case  90:
        dismpidial = ((dial-78)-6)*0.5*dismpierror; 
        break;

      case  91:
      case  92:
      case  93:
      case  94:
      case  95:
      case  96:
      case  97:
      case  98:
      case  99:
      case 100:
      case 101:
      case 102:
      case 103:
        // Extra MaQE tweaks (-7 sigma to -3.5 sigma)
        maqedial = ((dial-91)-14)*0.5*maqeerror;

      case 104:
      case 105:
      case 106:
      case 107:
      case 108:
      case 109:
      case 110:
      case 111:
      case 112:
      case 113:
      case 114:
      case 115:
      case 116:
        // SF-mode MaQE (-7 sigma to +3 sigma)
        maqedial = ((dial-104)-14)*0.5*maqeerror;
        rw.Systematics().SetTwkDial(t2krew::kNXSec_VecFFCCQE, 402);
        break;

      case 117:
      case 118:
      case 119: //< nominal
      case 120:
      case 121:
        mecpddodial = (dial-119)*0.5; // {-1.0, -0.5, 0, +0.5, +1}
        break;

      case 122:
        // rel RPA tuning
        rpanorm = 1;
        rpashape = 0;
        break;

      case 123:
        // non-rel RPA tuning
        rpanorm = 1;
        rpashape = -1;
        break;

      default:
        break;
    }

    // Set uncertainty dials
    rw.Systematics().SetTwkDial(t2krew::kNXSec_MaCCQE, maqedial);
    rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_pF_O16, pfodial);
    rw.Systematics().SetTwkDial(t2krew::kNNucl_CCQEBindingEnergy_O16, ebodial);
    rw.Systematics().SetTwkDial(t2krew::kNXSec_CA5RES, cadial);
    rw.Systematics().SetTwkDial(t2krew::kNXSec_MaNFFRES, manresdial);
    rw.Systematics().SetTwkDial(t2krew::kNXSec_BgSclRES, bkgdial);
    rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_dismpishp, dismpidial);
    rw.Systematics().SetTwkDial(t2krew::kNIWGMEC_PDDWeight_O16, mecpddodial);
    rw.Systematics().SetTwkDial(t2krew::kNXSec_VecFFCCQE, vecffccqe);
    rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_norm,rpanorm);
    rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_shape,rpashape);

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

