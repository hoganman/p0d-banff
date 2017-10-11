///////////////////////////////////////////////////////////////////////////////////
//
// Title: genWeights_2013a.cxx
//
// Purpose: Generate weights for any ND280 or SK sample, using the BANFF
//          parameters fitted central values (CVs) and covariance
//
// Usage:
//   ./genWeights_2013a.exe -i <inputfile> -p <banff_parameter_file> -o <weight_outputfile>
//                          -t <# of throws> -r <random seed for throws> 
//                          [--use-prefit --drop-flux --drop-xsec]
//
//      where,
//
//             <inputfile>: ND280 or SK MC file. Currently supported:
//                          1) NuMu CC (irods://QMULZone/home/asg/banff/numu/CC_MC_*.root)
//                          2) P0D NuE (http://nngroup.physics.sunysb.edu/~itaylor/T2K_Files/NuEAnalysisFiles/complete-Selection111024-[1-3].root
//                          3) Raw oaAnalysis file (only 1st vertex weight is stored)
//                          4) Raw SK tree (h1)
//                          5) NEUT neutroot file for e.g. MiniBooNE simulations (currently disabled because of issue with gcc4.6.3)
//
//  <banff_parameter_file>: Parameter file output by the BANFF fit
//
//           <# of throws>: Number of weights to generate for every event, each based
//                          on a throw from the fitted covariance matrix.
//                          Default is 0, only CV weights stored.
//
//     <weight_outputfile>: ROOT file containing TTree of weights for each event.
//                          First set in the weight array ( weights->At(0) ) is the
//                          fitted central value. The following sets in the array
//                          ( weights->At(i, i>0) ) correspond to each throw from
//                          the covariance matrix, if any (set by '-t'). Use these
//                          weights and evaluate the RMS in each bin of your distributions
//                          to build an error envelope.
//      
//      --use-prefit: Use parameters and covariance that are inputs to BANFF fit
//      --drop-flux: Don't apply any flux weights, even if flux parameters are defined
//      --drop-xsec: Don't apply any xsec weights, even if xsec parameters are defined
//
// History:
//
// 2012-01-14: Creation - Patrick de Perio (pdeperio@physics.utoronto.ca)
// 2012-01-17: Functionality to throw from covariance matrix -pdeperio
// 2012-03-09: Functionality to drop xsec or flux weights -mhartz
//             Updated parameter list -pdeperio
// 2013-06-27: Functionality for customizable flux binning (pdeperio)
//
///////////////////////////////////////////////////////////////////////////////////

//#define DEBUG

#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TAxis.h"

#include "T2KReWeight.h"
#include "T2KSyst.h"

#include "T2KGenieReWeight.h"
#include "T2KGenieUtils.h"

#include "T2KNeutReWeight.h"
#include "T2KNeutUtils.h"

#include "T2KNIWGReWeight.h"
#include "T2KNIWGUtils.h"

#include "T2KJNuBeamReWeight.h"

#ifdef __T2KRW_OAANALYSIS_ENABLED__
#include "ND__NRooTrackerVtx.h"
#endif

#include "T2KWeightsStorer.h"

#include "SK__h1.h"

#ifdef __T2KRW_NEUT_ENABLED__
//#define NEUTROOT
#ifdef NEUTROOT
#include "neutrootTreeSingleton.h"
#endif
#endif

using namespace std;

///////////////////////////////////////////////////////////////////////////////////

// Define Supported Analyses
const int nAnas = 5;
string anaTreeNames[nAnas] = {
  // Keep all RooTrackerVtx based files below here
  "SummaryTree/SummaryTree", // numuCC
  "AnalysisP0DNuE",          // P0DNuE
  "TruthDir/NRooTrackerVtx", // raw_oaAnalysis (this should be last in this section)
  // Keep all RooTrackerVtx based files above here
  
  // Keep all SK__h1 based files below here
  "h1",                      // raw_sk
  // Keep all SK__h1 based files above here
  
  // Keep all neutroot based files below here
  "neuttree"                 // raw_neutroot
  // Keep all neutroot based files above here
};

// This must correspond to the above "anaTreeNames"
enum anaEnum {numuCC,
	      P0DNuE,
	      raw_oaAnalysis,
	      raw_sk,
              raw_neutroot
};
// Note: "raw_oaAnalysis" is last in the ND280 trees since some analyses have
//       the same "TruthDir/NRooTrackerVtx" tree structure

int iana=0; // Counter and flag for determining analysis

// Tree types T2KReWeight input
enum tree_type_enum {NRooTrackerVtx, // iana=0,1,2
		     SK__h1,         // iana=3
		     neutroot        // iana=4
};    
int tree_type = -1;

// Parameters as Defined in the BANFF fit
// WARNING: Be careful when adding new parameters, make sure they are grouped
//          properly and ordered similarly to the BANFF input file,
//          since some conditionals in this program depend on it
const int nJnuPars = 8;
const int nNeutPars = 6;
const int nNiwgPars = 13;
const int nPars = nJnuPars+nNeutPars+nNiwgPars; // 26

                          // JNuBeam Flux parameters
string parNames[nPars] = {"nd_numu_bins", "nd_numub_bins", "nd_nue_bins", "nd_nueb_bins",
                          "sk_numu_bins", "sk_numub_bins", "sk_nue_bins", "sk_nueb_bins",

                          // NEUT parameters
                          "x_maqe", "x_1overmaqe2", "x_mares", "x_kappa","x_byonoff", "x_pdd",

                          // NIWG parameters
                          "x_dismpishp", "x_sf", "x_eb", "x_pf", "x_wshape", "x_1pishp",
                          "ccqe_ebins", "cc1pi_ebins", "ccmultipi_ebins", 
                          "ccdis_ebins", "cccoh_ebins", 
			  "ncother_ebins", "nc1pi0_ebins"};

// JNuBeam Flux parameters
enum parEnum {nd_numu_bins, nd_numub_bins, nd_nue_bins, nd_nueb_bins,
              sk_numu_bins, sk_numub_bins, sk_nue_bins, sk_nueb_bins,

              // NEUT parameters
              x_maqe, x_1overmaqe2, x_mares, x_kappa,x_byonoff,x_pdd,

              // NIWG parameters
              x_dismpishp, x_sf, x_eb, x_pf, x_wshape, x_1pishp,
              ccqe_ebins, cc1pi_ebins, ccmultipi_ebins,
              ccdis_ebins, cccoh_ebins, 
	      ncother_ebins, nc1pi0_ebins};

// Make sure to change these when adding parameters
const int lastFluxPar = sk_nueb_bins;
const int lastNeutPar = x_pdd;

enum jnuDetEnum {jnu_nd5, jnu_sk};
string jnuDet[2] = {"nd5","sk"};

const int nNus = 4;
enum jnuFluxEnum {jnu_numu, jnu_numub, jnu_nue, jnu_nueb};
string jnuFlux[nNus] = {"numu", "numub", "nue", "nueb"};

const int nModes = 7;
enum niwgModesEnum {ccqe,cc1pi,ccmultipi,
                    ccdis,cccoh,nc1pi0, ncother};
string niwgModes[nModes] = {"ccqe","cc1pi","ccmultipi",
                            "ccdis","cccoh","nc1pi0","ncother"};


// For determining if a parameter was included in BANFF fit and
// ordering of bins in parameter vector/covariance matrix
int parIncluded[nPars] = {0};

// For checking binning against that hardcoded in reweighting engines
TAxis *parBins[nPars];

// Number of events in file to loop over (default is all events)
// Can be modified with '-e' command line argument
int fNEvts = -1;

// Filename containers
TString fInputFileName = "";
TString fOutputFileName = "";
TString fInputParsFileName = "";

// Configuration of inputs
bool fUseFlux = true;
bool fUseXsec = true;
bool fUsePrefit = false;

// For matching to RooTrackerVtx
int anaNeutmode, NeutGenieIdx;
double anaEnu;

// For throwing parameters from covariance
int nThrows=0;
int rndSeed = 1867;

void Usage();
void ParseArgs(int argc, char **argv);
vector<string> separate(string input_uptions);

int main(int argc, char *argv[])
{
#if defined (__T2KRW_NEUT_ENABLED__) && defined (__T2KRW_JNUBEAM_ENABLED__) && defined (__T2KRW_NIWG_ENABLED__)
  ParseArgs(argc, argv);

  // Check that filenames were specified
  if (!fInputFileName.Length() || !fOutputFileName.Length() || !fInputParsFileName.Length()) Usage();

  // Open the input file
  TFile * infile = new TFile(fInputFileName, "OPEN");
  if(!infile->IsOpen()) exit(1);

  cout << endl << "Loading input tree file: " << fInputFileName.Data() << endl << endl;

  TTree *input_tree = 0;
  for (iana=0; iana<nAnas; iana++) {
    input_tree = (TTree*) infile->Get(anaTreeNames[iana].c_str());
    if (input_tree) {
      cout << "Found " << anaTreeNames[iana] << " tree" << endl;
      break;
    }
  }

  if (!input_tree) {
    cerr << "Error: Cannot find input tree" << endl;
    exit (1);
  }

  // Add tree distinguishing here if implementing a new tree format
  if (iana<=raw_oaAnalysis) {
    tree_type = NRooTrackerVtx;

#ifndef __T2KRW_OAANALYSIS_ENABLED__
    cout << "Error: oaAnalysis must be enabled upon build if using ND280 files" << endl;
    exit (1);
#endif

    cout << "Loading NRooTrackerVtx tree ";
  }

  else if (iana<=raw_sk) {
    tree_type = SK__h1;
    cout << "Loading SK tree ";
  }

  else if (iana<=raw_neutroot) {
    tree_type = neutroot;
    cout << "Loading neutroot tree ";
  }

  else {
    cerr << "Error: Unknown analysis type, iana=" << iana << endl;
    exit (1);
  }

  // Specific analysis tree
  TTree *ana_tree = input_tree;

  // For matching to RooTrackerVtx
  TBranch *b_anaNeutmode, *b_anaEnu, *b_NeutGenieIdx;

  // Initialize numuCC analysis tree
  if (iana == numuCC) {
    input_tree = (TTree*)infile->Get("TruthDir/NRooTrackerVtx");

    ana_tree->SetBranchAddress("selNeutGenie", &NeutGenieIdx, &b_NeutGenieIdx);
    ana_tree->SetBranchAddress("ReactionCode", &anaNeutmode, &b_anaNeutmode);

  }

  // Initialize P0DNuE analysis tree
  else if (iana == P0DNuE) {
    input_tree = (TTree*)infile->Get("NRooTrackerVtx");

    ana_tree->SetBranchAddress("NEUTCode", &anaNeutmode, &b_anaNeutmode);
    ana_tree->SetBranchAddress("NuEnergy", &anaEnu,      &b_anaEnu);
  }

  if (!input_tree) {
    cout << "Error: Cannot find NRooTrackerVtx in analysis file" << endl;
    exit (1);
  }

  ana_tree->AddFriend(input_tree);

  // Initialize input tree (need to implement new tree formats here)
  int NVtx;
  TClonesArray * nRooVtxs;
  SK::SK__h1 *skVtxs;

  if (tree_type == NRooTrackerVtx) {
    nRooVtxs = new TClonesArray("ND::NRooTrackerVtx", 100);
    input_tree->SetBranchAddress("Vtx", &nRooVtxs);
    input_tree->SetBranchAddress("NVtx", &NVtx);
  }
  else if (tree_type == SK__h1) {
    skVtxs = new SK::SK__h1(input_tree,1);

    // Check for SK signal nue/nuebar file (by filename only for now, since no ntuple variable)
    if (fInputFileName.Contains("nue_x_numuflx")) skVtxs->filetype = 1;
    else if (fInputFileName.Contains("nuebar_x_numubarflx")) skVtxs->filetype = 1;
  }
#ifdef NEUTROOT
  else if (tree_type == neutroot) {
    // Create global tree object 
    NeutrootTreeSingleton * neutrootTreeObj = NeutrootTreeSingleton::Instance(fInputFileName.Data());
  }
#endif

  // Get number of events in tree
  if(fNEvts < 0) fNEvts = input_tree->GetEntries();
  if (tree_type!=neutroot)
    cout << "with " << fNEvts << " events." << endl << endl;

  if (input_tree->GetEntries() != ana_tree->GetEntries()) {
    cout << "Error: Number of events in input_tree != ana_tree (" << ana_tree->GetEntries() << ")" << endl;
    exit (1);
  }

#ifdef __T2KRW_OAANALYSIS_ENABLED__
  vector<ND::NRooTrackerVtx*> allRooTrackerVtxs;
#endif

  vector<SK::SK__h1*> allSKVtxs;

  //====================================================================
  // Loop over events to store in memory
  for(int i = 0; i < fNEvts; i++){

    // Currently do not store neutroot events since it's difficult 
    // in the current tree implementation
    if (tree_type == neutroot) break;

    ana_tree->GetEntry(i);

    // RooTrackerVtx
    if (tree_type == NRooTrackerVtx) {
#ifdef __T2KRW_OAANALYSIS_ENABLED__
      // Count number of matched vertices
      // (only useful if matching index "NeutGenieIdx" is not used)
      int nMatchedVertices=0;

      ND::NRooTrackerVtx * vtx=0;
      // Check if matching index was set properly
      if (NeutGenieIdx<0) {
        cout << "Warning: No matched RooTrackerVtx for event " << i << ", NeutGenieIdx = " << NeutGenieIdx << endl;

        // Temporarily assume there was a matched vertex but assign it a weight of 1
        nMatchedVertices=1;

      } else {

        // Loop over RooTrackerVtx's
        for(int j = 0; j<NVtx; j++){

          ND::NRooTrackerVtx* thisVtx = (ND::NRooTrackerVtx*) nRooVtxs->At(j);
          if(!thisVtx){
            cout << "Error: Cannot find NRooTrackerVtx object for event " << i << ", vertex " << j << endl;
            exit (1);
          }

          // For P0DNuE analysis, match vertex using NEUT mode and Enu
          if (iana==P0DNuE &&
              !(atoi(thisVtx->EvtCode->GetString().Data()) == anaNeutmode &&
                thisVtx->StdHepP4[0][3] == anaEnu))
            continue;

          // For numu CC analysis, use index variable
          else if ( iana==numuCC && j!=NeutGenieIdx)
            continue;

          vtx=thisVtx;

          nMatchedVertices++;

          // Only weight first vertex in raw oaAnalysis file
          // Since T2KWeightStorer tree is flat
          if (iana==raw_oaAnalysis) break;
        }
      }

      // Check number of matched vertices
      if (nMatchedVertices < 1) {
        cout << "Error: No matched RooTrackerVtx found in event " << i << endl;
        exit (1);
      }
      else if (nMatchedVertices > 1) {
        cout << "Error: " << nMatchedVertices << " matched RooTrackerVtx found in event " << i << endl;
        exit (1);
      }

      // Need to clone the vertex because TTree owns vtx (due to SetBranchAddress)
      if (vtx)
	allRooTrackerVtxs.push_back((ND::NRooTrackerVtx*)vtx->Clone());
      else
	allRooTrackerVtxs.push_back(0);
#endif

    } // end if tree_type==NRooTrackerVtx

      // SK tree
    else if (tree_type == SK__h1) {
      allSKVtxs.push_back(new SK::SK__h1(*skVtxs));
    }
  } // end loop over events

  //================================================================================

  // Initialize output weights tree and file
  t2krew::T2KWeightsStorer *wt_storer = new t2krew::T2KWeightsStorer(fOutputFileName.Data());

  if (iana==raw_oaAnalysis)
    cout << "Warning: Only first vertex in each event of raw oaAnalysis will be weighted" << endl << endl;


  // Load input parameters file
  TFile *parfile = new TFile(fInputParsFileName, "OPEN");
  if(!parfile->IsOpen()) exit(1);

  cout << "Loading input parameters file: " << fInputParsFileName << endl << endl;

  // Get included parameters from file
  TObjString *param_list_obj = (TObjString*) parfile->Get("param_list");
  string param_list = (param_list_obj->GetString()).Data();
  vector<string> param_vec = separate(param_list);

  for (unsigned int i=0; i<param_vec.size(); i++) {

    bool parFound = 0;
    for (int ipar=0; ipar<nPars; ipar++) {
      if (!parNames[ipar].compare(param_vec[i])) {
        parIncluded[ipar] = i+1;
        parFound = 1;
      }
    }

    if (!parFound) {
      cout << "Error: " << param_vec[i].c_str() << " has not been implemented in this executable" << endl;
      exit (1);
    }
  }

  // Initialize T2KReWeight and engines
  t2krew::T2KReWeight rw;
  rw.AdoptWghtEngine("jnubeam_rw", new t2krew::T2KJNuBeamReWeight());
  rw.AdoptWghtEngine("neut_rw", new t2krew::T2KNeutReWeight());
  rw.AdoptWghtEngine("niwg_rw", new t2krew::T2KNIWGReWeight());

  jnubeam::rew::JReWeightEnu2013a* jrw_ptr = (jnubeam::rew::JReWeightEnu2013a*)((t2krew::T2KJNuBeamReWeight*)rw.WghtEngine("jnubeam_rw"))->GetWghtCalc("enu2013a");
  if (!jrw_ptr) {
    cout << endl << "Error: \"enu2013a\" WghtCalc not enabled in \"jnubeam_rw\" WghtEngine" << endl;
    exit (-1);
  }

  niwg::rew::NIWGReWeight2012a* niwgrw_ptr = (niwg::rew::NIWGReWeight2012a*)((t2krew::T2KNIWGReWeight*)rw.WghtEngine("niwg_rw"))->GetWghtCalc("niwg_2012a");
  if (!niwgrw_ptr) {
    cout << endl << "Error: \"niwg_2012a\" WghtCalc not enabled in \"niwg_rw\" WghtEngine" << endl;
    exit (-1);
  }


  // Check that ordering of parameter list is as we expect for assigning to T2KSyst's later
  int last_ipar=0;
  for (int ipar=0; ipar<nPars; ipar++) {
    if (!parIncluded[ipar]) continue;

    if (parIncluded[ipar] < last_ipar) {
      cout << "Error: Unexpected parameter ordering for " << parNames[ipar].c_str() << "(" << parIncluded[ipar] << ")" << endl;
      exit (1);
    }
    else
      last_ipar = parIncluded[ipar];
  }

  // Get axes for parameters that are binned
  for (int ipar=0; ipar<nPars; ipar++) {
    if (parNames[ipar].find("bins")==string::npos) continue;
    else if (parIncluded[ipar]) {

      parBins[ipar] = (TAxis*)parfile->Get(parNames[ipar].c_str());

      if (!parBins[ipar]) {
        cout << "Error: " << parNames[ipar] << " does not have associated TAxis in file." << endl;
        exit (1);
      }
    }
  }

  // WARNING: There is no check of neutrino type, detector nor mode ordering
  // (assumed order is as given by parNames[] defined above)
  for (int ipar=0; ipar<nPars; ipar++) {
    if (!parBins[ipar]) continue;

    TAxis *axis;
    if (ipar<=lastFluxPar) jrw_ptr->axis[ipar] = axis = parBins[ipar]; // Copy axis to jrw instead, so checks below are redundant
    else if (ipar>=ccqe_ebins) axis = niwgrw_ptr->axis[ipar-(nPars-nModes)];

    // Check number of bins 
    //(Warning: bins dropped for SK so ignore this check)
    if (tree_type!=SK__h1) {
      if (parBins[ipar]->GetNbins() != axis->GetNbins()) {
	cout << "Error: " << parNames[ipar] << " has " << parBins[ipar]->GetNbins()
	     << " number of bins, should be " << axis->GetNbins() << endl;
	exit (1);
      }
    }

    // Check bin boundaries
    cout << endl;
    for (int ibin=1; ibin<=parBins[ipar]->GetNbins()+1; ibin++) {
      if ( TMath::Abs(parBins[ipar]->GetBinLowEdge(ibin)-axis->GetBinLowEdge(ibin))>1.e-6 ) {
        cout << "Error: " << parNames[ipar] << " bin low edge " << parBins[ipar]->GetBinLowEdge(ibin)
             << " should be  " << axis->GetBinLowEdge(ibin) << endl;
        exit (1);
      }
#ifdef DEBUG
      cout << "Matched " << parNames[ipar] << " low edge = " << parBins[ipar]->GetBinLowEdge(ibin) << endl;
#endif

      if (ibin!=parBins[ipar]->GetNbins()+1) // Do not print check of overflow bin
	cout << "Binning for " << parNames[ipar] << ibin-1 << ": [" << parBins[ipar]->GetBinLowEdge(ibin) << ", " << parBins[ipar]->GetBinLowEdge(ibin)+parBins[ipar]->GetBinWidth(ibin) << "]" << endl;

    }
#ifdef DEBUG
    cout << endl;
#endif
  }

  // Get fitted parameter central values
  TVectorD postfit_params(*(TVectorD*)parfile->Get(Form("%s_params",(fUsePrefit ? "prefit":"postfit"))));

  // Get fitted parameter covariance matrix
  TMatrixDSym *postfit_cov = (TMatrixDSym*)(parfile->Get(Form("%s_cov",(fUsePrefit ? "prefit":"postfit"))))->Clone();

  // Throwing class
  ThrowParms *par_throws = 0;
  if (nThrows>0) {
    par_throws = new ThrowParms(postfit_params, (*postfit_cov));
    par_throws->SetSeed(rndSeed);

    cout << "Starting " << nThrows << " throws with random seed = " << rndSeed << endl << endl;
  }


  for (int ithrow=0; ithrow<nThrows+1; ithrow++) {

    TVectorD curr_parset = postfit_params;

    // First parameter set is the fitted central value
    // Else get a random throw based on covariance
    if (ithrow!=0) {
      std::vector<double> par_throw;
      par_throws->ThrowSet(par_throw);
      for(int j=0; j<(int)par_throw.size(); j++)
        curr_parset(j) = par_throw[j];
    }


    // Enable and set T2KSyst values
    int nParsXnBins=0;
    for (int ipar=0; ipar<nPars; ipar++) {

      if (!parIncluded[ipar]) continue;

      t2krew::T2KSyst_t t2ksyst = t2krew::kSystNull;

      // Flux parameters
      if (ipar<=lastFluxPar) {

        // Get detector type
        int jnuDetType;
        if (parNames[ipar].find("nd")!=string::npos) jnuDetType = jnu_nd5;
        else if (parNames[ipar].find("sk")!=string::npos) jnuDetType = jnu_sk;
        else {
          cout << "Error: Can't find detector type in " << parNames[ipar].c_str() << endl;
          exit (1);
        }

	// Keep count of flux bins for the other detector, if present in parameter file
        if (nd_numu_bins<=ipar && ipar<=nd_nueb_bins && tree_type == SK__h1){
           nParsXnBins += parBins[ipar]->GetNbins();
           continue; 
        } 
	else if (sk_numu_bins<=ipar && ipar<=sk_nueb_bins && tree_type == NRooTrackerVtx) {
           nParsXnBins += parBins[ipar]->GetNbins();
           continue;
        }  	
	else if (tree_type == neutroot) {
	  nParsXnBins += parBins[ipar]->GetNbins();
	  continue;
	}

        // Get neutrino type
        int jnuFluxType=-1;
        for (int inu=0; inu<nNus; inu++)
          if (parNames[ipar].find(jnuFlux[inu])!=string::npos) jnuFluxType = inu;

        if (jnuFluxType<0) {
          cout << "Error: Can't find neutrino type in " << parNames[ipar].c_str() << endl;
          exit (1);
        }

        // Enable T2KSyst's
        for (int ibin=0; ibin<parBins[ipar]->GetNbins(); ibin++) {

          // See ${T2KREWEIGHT}/src/T2KSyst.h for naming convention
          string systString = Form("JEnu2013a_%s%s%d",jnuDet[jnuDetType].c_str(),jnuFlux[jnuFluxType].c_str(),ibin);

          t2ksyst = t2krew::T2KSyst::FromString(systString);

          if (t2ksyst == t2krew::kSystNull) {
            cout << "Error: Unknown T2KSyst: " << systString.c_str() << endl;
            exit (1);
          }

          //Only if using flux parameters
          if(fUseFlux){ 
            rw.Systematics().Include(t2ksyst);
            rw.Systematics().SetAbsTwk(t2ksyst);
            rw.Systematics().SetTwkDial(t2ksyst,curr_parset[nParsXnBins] - 1);
          }

          nParsXnBins++;

        }
      } // End flux parameters

      // NEUT parameters
      else if (ipar<=lastNeutPar) {

        if (ipar==x_maqe) t2ksyst = t2krew::kNXSec_MaCCQE;
	else if (ipar==x_1overmaqe2) { t2ksyst = t2krew::kNXSec_1overMaCCQE2;
	  t2krew::T2KNeutReWeight * neut_rw_ptr = (t2krew::T2KNeutReWeight*) rw.WghtEngine("neut_rw");
 	  ((NReWeightNuXSecCCQE*) neut_rw_ptr->GetWghtCalc("xsec_ccqe"))->SetMode(NReWeightNuXSecCCQE::kMode1overMa2);
	}
        else if (ipar==x_mares) t2ksyst = t2krew::kNXSec_MaRES;
        else if (ipar==x_kappa) t2ksyst = t2krew::kNNucl_CCQEPauliSupViaKF;
        else if (ipar==x_byonoff) t2ksyst = t2krew::kNXSec_BYOnOffDIS;
	else if (ipar==x_pdd) t2ksyst = t2krew::kNNucl_PilessDcyRES;

        else {
          cout << "Error: Unknown parameter in NEUT block: " << parNames[ipar].c_str() << endl;
          exit (1);
        }

        //Only if using xsec parameters
        if(fUseXsec){
          rw.Systematics().Include(t2ksyst);
          rw.Systematics().SetAbsTwk(t2ksyst);

          // Parameters passed straight to NEUT
          if (ipar==x_byonoff || ipar==x_pdd)
            rw.Systematics().SetTwkDial(t2ksyst,curr_parset[nParsXnBins]);
  
          else
            rw.Systematics().SetTwkDial(t2ksyst,curr_parset[nParsXnBins] - 1);
        }

        nParsXnBins++;

      } // End NEUT parameters

      // NIWG parameters
      else {

        // Enu binned parameters
        if (parNames[ipar].find("bins")!=string::npos) {

          // Get mode
          int niwgModeType=-1;
          for (int imode=0; imode<nModes; imode++)
            if (parNames[ipar].find(niwgModes[imode])!=string::npos) niwgModeType = imode;

          if (niwgModeType<0) {
            cout << "Error: Can't find mode in " << parNames[ipar].c_str() << endl;
            exit (1);
          }

          // Enable T2KSyst's
          for (int ibin=0; ibin<parBins[ipar]->GetNbins(); ibin++) {

            // See ${T2KREWEIGHT}/src/T2KSyst.h for naming convention
            string systString = Form("NIWG2012a_%sE%d",niwgModes[niwgModeType].c_str(),ibin);

            t2ksyst = t2krew::T2KSyst::FromString(systString);

            if (t2ksyst == t2krew::kSystNull) {
              cout << "Error: Unknown T2KSyst: " << systString.c_str() << endl;
              exit (1);
            }

            //Only if using xsec parameters
            if(fUseXsec){
              rw.Systematics().Include(t2ksyst);
              rw.Systematics().SetAbsTwk(t2ksyst);
              rw.Systematics().SetTwkDial(t2ksyst,curr_parset[nParsXnBins] - 1);
            }  

            nParsXnBins++;

          }
        }

        // Non-Enu binned parameters
        else {

          if (ipar==x_dismpishp)   t2ksyst = t2krew::kNIWG2012a_dismpishp;
          else if (ipar==x_sf)     t2ksyst = t2krew::kNIWG2012a_sf;
          else if (ipar==x_eb)     t2ksyst = t2krew::kNIWG2012a_eb;
          else if (ipar==x_pf)     t2ksyst = t2krew::kNIWG2012a_pf;
	  else if (ipar==x_wshape) t2ksyst = t2krew::kNIWGDeltaMass_width;
	  else if (ipar==x_1pishp) t2ksyst = t2krew::kNIWG2012a_mbcc1pi_enushp;
          else {
            cout << "Error: Unknown parameter in NIWG block: " << parNames[ipar].c_str() << endl;
            exit (1);
          }

          //Only if using xsec parameters
          if(fUseXsec){
            rw.Systematics().Include(t2ksyst);
            rw.Systematics().SetAbsTwk(t2ksyst);

            // Parameters passed straight to NIWGReWeight
            if (ipar==x_dismpishp || ipar==x_sf || ipar==x_1pishp)
              rw.Systematics().SetTwkDial(t2ksyst,curr_parset[nParsXnBins]);
            else
              rw.Systematics().SetTwkDial(t2ksyst,curr_parset[nParsXnBins] - 1);
          }  

          nParsXnBins++;

        }

      } // End NIWG Parameters

    }

    rw.Reconfigure();

    // Store the values of the current set of parameters
    wt_storer->NewSystSet(rw.Systematics());


    //////////////////////////////////////////////////////////


    // Loop over events in memory
    for(int i = 0; i < fNEvts; i++){

      //if (i%10000==0) cout << "Event " << i << endl;

      Double_t weight = 1.0;

      if (tree_type == NRooTrackerVtx) {
#ifdef __T2KRW_OAANALYSIS_ENABLED__
        ND::NRooTrackerVtx* vtx=allRooTrackerVtxs[i];
        if(vtx) weight = rw.CalcWeight(vtx);
#endif
      }

      else if (tree_type == SK__h1) {
        SK::SK__h1* vtx=allSKVtxs[i];
        if(vtx) weight = rw.CalcWeight(vtx);
      }

      else if (tree_type == neutroot) {
	weight = rw.CalcWeight(input_tree, i);
      }
      
      // Store weight for the event
      wt_storer->AddWeight(weight);

    } // end loop over events

  } // end loop over throws

  if (par_throws) delete par_throws;

  wt_storer->SaveToFile(); // Save the weights to a file
  delete wt_storer;

#ifdef __T2KRW_OAANALYSIS_ENABLED__
  for(int i=0; i<(int)allRooTrackerVtxs.size(); ++i) delete allRooTrackerVtxs[i];
#endif

  for(int i=0; i<(int)allSKVtxs.size(); ++i) delete allSKVtxs[i];

#else

  cout << "The NEUT, JNuBeam and NIWG reweighting engines must all be enabled upon building." << endl;

#endif
  return 0;
}

// Print the cmd line syntax
void Usage(){
  cout << "Error Usage: " << endl;
  cout << "genWeights_2013a.exe -i <Input Filename> -p <Input BANFF Parameter Filename> -o <Output Weight Tree Filename>" << endl;
  exit(1);
}

// Messy way to process cmd line arguments.
void ParseArgs(int argc, char **argv){
  int nargs = 3;
  if(argc<(nargs*2+1)){ Usage(); }
  for(int i = 1; i < argc; i++){
    if(string(argv[i]) == "-i") {fInputFileName = argv[i+1]; i++;}
    else if(string(argv[i]) == "-o") {fOutputFileName = argv[i+1]; i++;}
    else if(string(argv[i]) == "-p") {fInputParsFileName = argv[i+1]; i++;}
    else if(string(argv[i]) == "-t") {nThrows = std::atoi(argv[i+1]); i++;}
    else if(string(argv[i]) == "-r") {rndSeed = std::atoi(argv[i+1]); i++;}
    else if(string(argv[i]) == "-e") {fNEvts = std::atoi(argv[i+1]); i++;}
    else if(string(argv[i]) == "--use-prefit") fUsePrefit = true;
    else if(string(argv[i]) == "--drop-flux") fUseFlux = false;
    else if(string(argv[i]) == "--drop-xsec") fUseXsec = false;
    else {
      cout << "Invalid argument:" << argv[i] << " "<< argv[i+1] << endl;
      Usage();
    }
  }
}




vector<string> separate(string input_options){
  vector<string> options_string;
  istringstream iss(input_options);
  while(iss){
    string sub;
    iss >> sub;
    options_string.push_back(sub);
  }
  options_string.pop_back();
  return options_string;
}
