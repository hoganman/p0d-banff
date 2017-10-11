//
// Example to show how to reweight events from the SK tree with NIWG2010a systematics//  Run with: 
//
// ./generateWeightsFromSK_NIWGexample.exe -s sk_inputfile [optional nevents] 
// 
// Jim Dobson       - 28/03/2011
// Patrick de Perio - 15/04/2011
// Kendall Mahn - 29/08/2011
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
#include "T2KGEANTReWeight.h"

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
 T2KWeightsStorer *storer = new T2KWeightsStorer("fsi_wtstorer.root"); //forweightstorer

 cout << "Starting to reweight events from SK file: " << fSKFileName << endl;

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
 
 const int fsidraw = 30-7+1+1; //1 default and 7 through 30

 // Table 13 from TN032 of FSI parameter variations.
 // Only last 16 variations are used for generation of 2012a uncertainties
 Double_t fsi[fsidraw][7] = {
{0 ,1.0 ,1.8 ,1 ,1.1 ,1.0 ,1.8},
{7 ,0.6 ,1.8 ,1 ,0.7 ,0.5 ,1.8},
{8 ,0.6 ,1.8 ,1 ,0.7 ,1.6 ,1.8},
{9 ,0.7 ,1.8 ,1 ,1.6 ,0.4 ,1.8},
{10 ,0.7 ,1.8 ,1 ,1.6 ,1.6 ,1.8},
{11 ,1.4 ,1.8 ,1 ,0.6 ,0.6 ,1.8},
{12 ,1.3 ,1.8 ,1 ,0.7 ,1.6 ,1.8},
{13 ,1.5 ,1.8 ,1 ,1.5 ,0.4 ,1.8},
{14 ,1.6 ,1.8 ,1 ,1.6 ,1.6 ,1.8},
{15,0.6 ,1.1 ,1.5 ,0.7 ,0.5 ,2.3},
{16 ,0.6 ,1.1 ,1.5 ,0.7 ,1.6 ,2.3},
{17 ,0.7 ,1.1 ,1.5 ,1.6 ,0.4 ,2.3},
{18 ,0.7 ,1.1 ,1.5 ,1.6 ,1.6 ,2.3},
{19 ,1.4 ,1.1 ,1.5 ,0.6 ,0.6 ,2.3},
{20 ,1.3 ,1.1 ,1.5 ,0.7 ,1.6 ,2.3},
{21 ,1.5 ,1.1 ,1.5 ,1.5 ,0.4 ,2.3},
{22 ,1.6 ,1.1 ,1.5 ,1.6 ,1.6 ,2.3},
{23 ,0.6 ,2.3 ,0.5 ,0.7 ,0.5 ,1.3},
{24 ,0.6 ,2.3 ,0.5 ,0.7 ,1.6 ,1.3},
{25 ,0.7 ,2.3 ,0.5 ,1.6 ,0.4 ,1.3},
{26 ,0.7 ,2.3 ,0.5 ,1.6 ,1.6 ,1.3},
{27 ,1.4 ,2.3 ,0.5 ,0.6 ,0.6 ,1.3},
{28 ,1.3 ,2.3 ,0.5 ,0.7 ,1.6 ,1.3},
{29 ,1.5 ,2.3 ,0.5 ,1.5 ,0.4 ,1.3},
{30 ,1.6 ,2.3 ,0.5 ,1.6 ,1.6 ,1.3}
};

const int ndraws = fsidraw;


 // Instantiate the reader of the SK tree class
 SK::SK__h1 *skVtxs = new SK::SK__h1(SK_tree,1);


#ifdef __T2KRW_NEUT_ENABLED__


 // Make a t2kreweighting object and add a NIWG weighting engine. 
 t2krew::T2KReWeight rw; 

 rw.AdoptWghtEngine("neut_rw", new t2krew::T2KNeutReWeight());
 rw.AdoptWghtEngine("geant_rw", new t2krew::T2KGEANTReWeight());

 rw.Systematics().Include(t2krew::kNCasc_FrAbs_pi);
 rw.Systematics().Include(t2krew::kNCasc_FrCExLow_pi);
 rw.Systematics().Include(t2krew::kNCasc_FrInelLow_pi);
 rw.Systematics().Include(t2krew::kNCasc_FrPiProd_pi);
 rw.Systematics().Include(t2krew::kNCasc_FrCExHigh_pi);
 rw.Systematics().Include(t2krew::kNCasc_FrInelHigh_pi);
 rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrAbs_pi);
 rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrCExLow_pi);
 rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrInelLow_pi);
 rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrPiProd_pi);
 rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrCExHigh_pi);
 rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrInelHigh_pi);

 rw.Systematics().Include(t2krew::kGEANT_PionXSecTbl);

 Double_t dialabs = 1.1; //FSIABS
 Double_t dialcexlo = 1.0; //FSICX
 Double_t dialinelo = 1.0; //FSIQE
 Double_t dialprod = 1.0; //FSIINEL
 Double_t dialcexhi = 1.8;//FSICXH
 Double_t dialinehi = 1.8; //FSIQEH

 Double_t dialabsn   = 1.1; //FSIABS
 Double_t dialcexlon = 1.0; //FSICX
 Double_t dialinelon = 1.0; //FSIQE
 Double_t dialprodn  = 1.0; //FSIINEL
 Double_t dialcexhin = 1.8;//FSICXH
 Double_t dialinehin = 1.8; //FSIQEH

 Double_t dialtbl = 0.; //FSIQEH

 for(int dial=0; dial<ndraws; dial++){

   dialabs   = (fsi[dial][4]-dialabsn  )/dialabsn  ;
   dialcexlo = (fsi[dial][5]-dialcexlon)/dialcexlon;
   dialinelo = (fsi[dial][1]-dialinelon)/dialinelon;
   dialprod  = (fsi[dial][3]-dialprodn )/dialprodn ;
   dialcexhi = (fsi[dial][6]-dialcexhin)/dialcexhin;
   dialinehi = (fsi[dial][2]-dialinehin)/dialinehin;
   if(dial!=0) dialtbl = (dial-1)%8+1;   
   
   rw.Systematics().SetTwkDial(t2krew::kNCasc_FrAbs_pi, dialabs);
   rw.Systematics().SetTwkDial(t2krew::kNCasc_FrCExLow_pi, dialcexlo);
   rw.Systematics().SetTwkDial(t2krew::kNCasc_FrInelLow_pi, dialinelo);
   rw.Systematics().SetTwkDial(t2krew::kNCasc_FrPiProd_pi, dialprod);
   rw.Systematics().SetTwkDial(t2krew::kNCasc_FrCExHigh_pi,dialcexhi);
   rw.Systematics().SetTwkDial(t2krew::kNCasc_FrInelHigh_pi, dialinehi);

   rw.Systematics().SetTwkDial(t2krew::kGEANT_PionXSecTbl,dialtbl);

   
   rw.Reconfigure();
   storer->NewSystSet(rw.Systematics()); // save the current twk dial values to the weight storer class

   // Loop over SK entries and calculate weights.
 if(fNskEvts < 0) fNskEvts = skVtxs->GetEntries();


 for(int i = 0; i < fNskEvts; i++){

   skVtxs->GetEntry(i);
   //   cout << "Getting weight for SK Event = "<< i << " with EvtCode: "<< abs(skVtxs->modee)  << ", NuPDG = " << skVtxs->ipnu[0] << endl;
   double weight = rw.CalcWeight(skVtxs);
   storer->AddWeight(weight);

 }// event loop
 
 } // dial

 storer->SaveToFile(); // save the weights to a file
 delete storer; 
 #endif // __T2KRW_NEUT_ENABLED__
 
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

