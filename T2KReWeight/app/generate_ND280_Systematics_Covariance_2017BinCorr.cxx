//
// 2015 ND280 Detector Covariance Throws generator
// See TN192 for details
//
// Jim Dobson       - 28/03/2011
// Patrick de Perio - 15/04/2011
// Kendall Mahn - 29/08/2011
// Kendall Mahn - 9/03/2012
// Kendall Mahn - 9/10/2012
// Mark Scott - 06/01/2015
// Christine Nielsen 05/05/2015
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
#include "T2KNIWGUtils.h"

//#ifdef __T2KRW_PSYCHE_ENABLED__

// The only psyche include we need, woop
#include "AnalysisManager.hxx"
#include "ToyMakerExample.hxx"
#include "SampleId.hxx"
#include "Parameters.hxx"
#include "AnalysisUtils.hxx"

// For weight storer class
#include "T2KWeightsStorer.h"
#include "TDecompChol.h"
#include "TMatrixT.h"
#include "TMatrixTSym.h"
#include "TVectorT.h"

#include <time.h>       /* time_t, struct tm, difftime, time, mktime */

using namespace t2krew;

const int nDecorrSample = 2;
const int nDecorrDet = 2;
const int nSyst = 30;
const int nThrows = 500;
int nDuplicate= 4; // number of times the systematics are duplicated in the matrix
int nRows;
int fNEvts = -1;
int fNThrows = 1;

bool verbose = false;
bool PreLoad = false;
bool onSample[nSyst][4];
bool applySyst[nSyst];
bool Decorr[nSyst][nDecorrDet];
std::map<std::string, int> systMap;
std::map<std::string, int> systFirstIndex;

std::string fFileName;
std::string fOutfile;
std::vector<double> systpull_corr[nThrows];

int Rest(int systIndex, int npar){
  
  if     (!Decorr[systIndex][0] && !Decorr[systIndex][1]) return 0;
  else if(!Decorr[systIndex][0] &&  Decorr[systIndex][1]) return npar;
  else if( Decorr[systIndex][0] && !Decorr[systIndex][1]) return npar;
  else if( Decorr[systIndex][0] &&  Decorr[systIndex][1]) return npar*3;

  return -1;
 
};

void FillCorr(int i, int j, TMatrixTSym<double> *corr, double scorr){
  
  if(i >= corr->GetNcols() || j >= corr->GetNcols()){
    std::cout << "You are trying to fill the correlation with index > size of the matrix." << std::endl;
    exit(0);
  }  

  if(i == j)
    return;
  
  (*corr)(i,j) = scorr;
  
  return;

};

int RowIndex(int systIndex, int sample){

  if     (sample == 0)
    return 0; // numu FGD1

  else if(sample == 1 && !Decorr[systIndex][1])
    return 0; // numuFGD2
  else if(sample == 1 &&  Decorr[systIndex][1])
    return 1; // numuFGD2 in case the detectors were decorrelated

  else if(sample == 2 && !Decorr[systIndex][0] && !Decorr[systIndex][1])
    return 0; // nueFGD1 in case everything is correlated
  else if(sample == 2 &&  Decorr[systIndex][0] && !Decorr[systIndex][1])
    return 1; // nueFGD1 in case samples were correlated and not detector
  else if(sample == 2 && !Decorr[systIndex][0] &&  Decorr[systIndex][1])
    return 1; // nueFGD1 in case detector were correlated and not samples
  else if(sample == 2 &&  Decorr[systIndex][0] &&  Decorr[systIndex][1])
    return 2; // nueFGD1 in case everything is uncorrelated

  else if(sample == 3 && !Decorr[systIndex][0] && !Decorr[systIndex][1])
    return 0; // nueFGD2 in case everything is correlated
  else if(sample == 3 &&  Decorr[systIndex][0] && !Decorr[systIndex][1])
    return 1; // nueFGD2 in case samples were correlated and not detector
  else if(sample == 3 && !Decorr[systIndex][0] &&  Decorr[systIndex][1])
    return 1; // nueFGD2 in case detector were correlated and not samples
  else if(sample == 3 &&  Decorr[systIndex][0] &&  Decorr[systIndex][1])
    return 3; // nueFGD2 in case everything is uncorrelated 

  return -1;

};

class SelectedVertex{

public:

  SelectedVertex(int sampleIDVal, ND::NRooTrackerVtx* rtvPointer, double mom){
    sampleID = sampleIDVal;
    rtv = new ND::NRooTrackerVtx(*rtvPointer);
    Momentum = mom/1000.0;
  }
  virtual ~SelectedVertex(){};

  int sampleID;
  double Momentum;
  ND::NRooTrackerVtx* rtv;

};

//******************************************************************
class BANFFNuMuFGD1ToyMaker: public ToyMaker{ //********************
//******************************************************************

  /// A random generator that can be used to generate throws.
  TRandom3 _RandomGenerator;

  /// The random seed used
  UInt_t _seed;

  /// Apply 0 variation
  bool _zero_var;

  /// A variable to know which toy we are filling
  int CurrentToy;
  
public:

  //******************************************************************
  void FillToyExperiment(ToyExperiment & toy){
  //******************************************************************

    // Set the same weight (1) for al toys. This will be later normalized to the number of toys
    Float_t weight = 1.;
    int previous = 0;

    for(UInt_t isyst = 0; isyst < NMAXSYSTEMATICS; isyst++){
      SystematicBase* syst = _systematics[isyst];
      if(!syst) continue;
      int index = systMap[syst->GetName()];
      if(index < 0) continue;

      int npar  = syst->GetNParameters();
      toy.AddToyVariation(syst->GetIndex(), syst->GetNParameters());

      // Loop over parameters for this systematic (numuFGD1 is always the first so it's easy)

      for(int ipar = 0; ipar < npar; ipar++){
        if(CurrentToy > 1 && applySyst[index]){
          toy.SetToyVariation(syst->GetIndex(), ipar, systpull_corr[CurrentToy][previous + RowIndex(index,0)*npar], weight);
          previous++;
        }else{
          toy.SetToyVariation(syst->GetIndex(), ipar, 0., weight);
        }
      }
      previous = previous + Rest(index, npar);
    }
    CurrentToy++;
  }


  //******************************************************************
  BANFFNuMuFGD1ToyMaker(UInt_t seed, bool zero_var):ToyMaker(){
  //******************************************************************

    // Set the seed
    _RandomGenerator.SetSeed(seed);

    // Save the random Seed
    _seed = seed;

    // Apply 0 variation
    _zero_var = zero_var;
    
    CurrentToy = 0;
  };

};


//******************************************************************
class BANFFNuMuFGD2ToyMaker: public ToyMaker{ //********************
//******************************************************************
  /// A random generator that can be used to generate throws.
  TRandom3 _RandomGenerator;

  /// The random seed used
  UInt_t _seed;

  /// Apply 0 variation
  bool _zero_var;
  
  /// A variable to know which toy we are filling
  int CurrentToy;
  
public:

  //******************************************************************
  void FillToyExperiment(ToyExperiment & toy){
  //******************************************************************

    // Set the same weight (1) for al toys. This will be later normalized to the number of toys
    Float_t weight = 1.;
    int previous = 0;
    for(UInt_t isyst = 0; isyst < NMAXSYSTEMATICS; isyst++){
      SystematicBase* syst = _systematics[isyst];
      if(!syst) continue;

      int index = systMap[syst->GetName()];
      if(index < 0) continue;

      int npar  = syst->GetNParameters();

      //if(onSample[index][1])
      toy.AddToyVariation(syst->GetIndex(), syst->GetNParameters());

      // Loop over parameters for this systematic
      
      for (int ipar = 0; ipar < npar; ipar++){
        if(CurrentToy > 1 && applySyst[index]){
          toy.SetToyVariation(syst->GetIndex(), ipar, systpull_corr[CurrentToy][previous + RowIndex(index,1)*npar], weight);
          previous++;
        }else{
          toy.SetToyVariation(syst->GetIndex(), ipar, 0., weight);
        }
      }
           
      previous = previous + Rest(index, npar);
      
    }
    CurrentToy++;

  }



  //******************************************************************
  BANFFNuMuFGD2ToyMaker(UInt_t seed, bool zero_var):ToyMaker(){
  //******************************************************************

    // Set the seed
    _RandomGenerator.SetSeed(seed);

    // Save the random Seed
    _seed = seed;

    // Apply 0 variation
    _zero_var = zero_var;
    
    CurrentToy = 0;
    
  };

};


//******************************************************************
class BANFFNuEFGD1ToyMaker: public ToyMaker{ //*********************
//******************************************************************

  /// A random generator that can be used to generate throws.
  TRandom3 _RandomGenerator;

  /// The random seed used
  UInt_t _seed;

  /// Apply 0 variation
  bool _zero_var;

  /// A variable to know which toy we are filling
  int CurrentToy;
  
public:

  //******************************************************************
  void FillToyExperiment(ToyExperiment & toy){
  //******************************************************************
   
    // Set the same weight (1) for al toys. This will be later normalized to the number of toys
    Float_t weight = 1.;
    int previous = 0;
    for(UInt_t isyst = 0; isyst < NMAXSYSTEMATICS; isyst++){
      SystematicBase* syst = _systematics[isyst];
      if(!syst) continue;

      int index = systMap[syst->GetName()];
      if(index < 0) continue;

      int npar  = syst->GetNParameters();

      //if(onSample[index][2])
      toy.AddToyVariation(syst->GetIndex(), syst->GetNParameters());

      // Loop over parameters for this systematic
      for(int ipar = 0; ipar < npar; ipar++){
        if(CurrentToy > 1 && applySyst[index]){
          toy.SetToyVariation(syst->GetIndex(), ipar, systpull_corr[CurrentToy][previous + RowIndex(index,2)*npar],weight);
          previous++;
        }else{
	  toy.SetToyVariation(syst->GetIndex(), ipar, 0., weight);
        }        

      }
      previous = previous + Rest(index, npar);
      
    }
    CurrentToy++;
  }
  
  
  //******************************************************************
  BANFFNuEFGD1ToyMaker(UInt_t seed, bool zero_var):ToyMaker(){
  //******************************************************************

    // Set the seed
    _RandomGenerator.SetSeed(seed);

    // Save the random Seed
    _seed = seed;

    // Apply 0 variation
    _zero_var = zero_var;
    
    CurrentToy = 0;
  };

};


//******************************************************************
class BANFFNuEFGD2ToyMaker: public ToyMaker{ //*********************
//******************************************************************

  /// A random generator that can be used to generate throws.
  TRandom3 _RandomGenerator;

  /// The random seed used
  UInt_t _seed;

  /// Apply 0 variation
  bool _zero_var;

  /// A variable to know which toy we are filling
  int CurrentToy;
  
public:


  //******************************************************************
  void FillToyExperiment(ToyExperiment & toy){
  //******************************************************************
     
    // Set the same weight (1) for al toys. This will be later normalized to the number of toys
    Float_t weight = 1.;
    int previous = 0;
    for(UInt_t isyst = 0; isyst < NMAXSYSTEMATICS; isyst++){
      SystematicBase* syst = _systematics[isyst];
      if(!syst) continue;
      
      int index = systMap[syst->GetName()];
      if(index < 0) continue;

      int npar  = syst->GetNParameters();

      toy.AddToyVariation(syst->GetIndex(), syst->GetNParameters());

      // Loop over parameters for this systematic
      for(int ipar = 0; ipar < npar; ipar++){
	if(CurrentToy > 1 && applySyst[index]){
          toy.SetToyVariation(syst->GetIndex(), ipar, systpull_corr[CurrentToy][previous + RowIndex(index,3)*npar], weight);
          previous++;
	}else{
	  toy.SetToyVariation(syst->GetIndex(), ipar, 0., weight);
        }
      }
      previous = previous + Rest(index, npar);
    }
    CurrentToy++;

  }


  //******************************************************************
  BANFFNuEFGD2ToyMaker(UInt_t seed, bool zero_var):ToyMaker(){
  //******************************************************************

    // Set the seed
    _RandomGenerator.SetSeed(seed);

    // Save the random Seed
    _seed = seed;

    // Apply 0 variation
    _zero_var = zero_var;
    
    CurrentToy = 0;
  };

};


void Usage();
int ParseArgs(int argc, char *argv[]);

//******************************************************************
int main(int argc, char *argv[]){
//******************************************************************
  
  std::cout<<"here"<<std::endl;

  double fSmallNumber = 0.0000001;

  // process the arguments
  int args = ParseArgs(argc, argv);
  if(args != 0){
    std::cerr << "Usage " << std::endl;
    return 0;
  }
  
  time_t timer;
  struct tm y2k = {0};
  double seconds0, seconds1;
  y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
  y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;
  
  // Read the parameters files following the package hierarchy
  // first the top level package. Set the parameters as fixed
  ND::params().LoadParametersFiles(anaUtils::GetPackageHierarchy(), true);

  // Make sure no parameters have been accessed yet
  ND::params().SetReadParamOverrideFilePointPassed();

  TFile* outfile = new TFile(fOutfile.c_str(),"RECREATE");
  TTree* outtree = new TTree("SelectedEventTruth","SelectedEventTruth");

  int nTh = nThrows;
  float* PMu          = new float[nThrows];
  float* ThetaMu      = new float[nThrows];
  float* DetWeight    = new float[nThrows];
  float* FluxWeight   = new float[nThrows];
  float* NIWGWeight   = new float[nThrows];
  int*   Selection    = new int  [nThrows];
  int    EventNumber = 0;
  int    Run = 0;
  int    SubRun = 0;
  

  for(int i = 0; i < nThrows; ++i){
    FluxWeight[i]   = 1.;
    DetWeight[i]    = 1.;
    NIWGWeight[i]   = 1.;
    PMu[i]       = -999;
    ThetaMu[i]   = -999;
    Selection[i] = 0;
  }

  outtree->Branch("nThrows",     &nTh,          "nThrows/I");
  outtree->Branch("PMu",          PMu,         "PMu[nThrows]/F");
  outtree->Branch("ThetaMu",      ThetaMu,     "ThetaMu[nThrows]/F");
  outtree->Branch("DetWeight",    DetWeight,   "DetWeight[nThrows]/F");
  outtree->Branch("FluxWeight",   FluxWeight,  "FluxWeight[nThrows]/F");
  outtree->Branch("NIWGWeight",   NIWGWeight,  "NIWGWeight[nThrows]/F");
  outtree->Branch("Selection",    Selection,   "Selection[nThrows]/I");
  outtree->Branch("EventNumber", &EventNumber, "EventNumber/I");
  outtree->Branch("Run",         &Run,         "Run/I");
  outtree->Branch("SubRun",      &SubRun,      "SubRun/I");
  

  std::cout << "Starting to reweight events from psyche file: " << fFileName << std::endl;

  // Load in the NRooTracker vertex tree.
  TFile * infile = new TFile(fFileName.c_str(), "OPEN");
  if(!infile){
    std::cerr << "Cannot open input file!" << std::endl;
    exit(1);
  }

  applySyst[ 0] = (bool)ND::params().GetParameterI("psycheSteering.Variations.EnableBFieldDist");
  applySyst[ 1] = (bool)ND::params().GetParameterI("psycheSteering.Variations.EnableMomScale");
  applySyst[ 2] = (bool)ND::params().GetParameterI("psycheSteering.Variations.EnableMomResol");
  applySyst[ 3] = (bool)ND::params().GetParameterI("psycheSteering.Variations.EnableTpcPid");
  applySyst[ 4] = (bool)ND::params().GetParameterI("psycheSteering.Variations.EnableFgdPid");
  applySyst[ 5] = (bool)ND::params().GetParameterI("psycheSteering.Variations.EnableToFResol");
  applySyst[ 6] = (bool)ND::params().GetParameterI("psycheSteering.Variations.EnableECalMomScale");
  applySyst[ 7] = (bool)ND::params().GetParameterI("psycheSteering.Variations.EnableECalMomResol");
  applySyst[ 8] = (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableChargeConf");
  applySyst[ 9] = (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableTpcClusterEff");
  applySyst[10] = (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableTpcTrackEff");
  applySyst[11] = (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableTpcFgdMatchEff");
  applySyst[12] = (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableFgdTrackEff");
  applySyst[13] = (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableFgdHybridTrackEff");
  applySyst[14] = (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableMichelEleEff");
  applySyst[15] = (bool)ND::params().GetParameterI("psycheSteering.Weights.EnablePileUp");
  applySyst[16] = (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableFgdMass");
  applySyst[17] = (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableOOFV");
  applySyst[18] = (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableSIPion");
  applySyst[19] = (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableSIProton");
  applySyst[20] = (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableSandMuons");
  applySyst[21] = (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableECalPID");
  applySyst[22] = (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableTpcECalMatchEff");
  applySyst[23] = (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableECalEmHipPID");
  applySyst[24] = (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableFGD2Shower");
  applySyst[25] = (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableNuETPCPileUp");
  applySyst[26] = (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableNuEP0DPileUp");
  applySyst[27] = (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableNuEECalPileUp");
  applySyst[28] = (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableNuEOOFV");
  applySyst[29] = (bool)ND::params().GetParameterI("psycheSteering.Weights.EnableFlux");
  
  // numuFGD1              numuFGD2                 nueFGD1                  nueFGD2
  onSample[ 0][0] = true;  onSample[ 0][1] = true;  onSample[ 0][2] = true;  onSample[ 0][3] = true;  // Variations.BFieldDist
  onSample[ 1][0] = true;  onSample[ 1][1] = true;  onSample[ 1][2] = true;  onSample[ 1][3] = true;  // Variations.MomScale
  onSample[ 2][0] = true;  onSample[ 2][1] = true;  onSample[ 2][2] = true;  onSample[ 2][3] = true;  // Variations.MomResol
  onSample[ 3][0] = true;  onSample[ 3][1] = true;  onSample[ 3][2] = true;  onSample[ 3][3] = true;  // Variations.TpcPid
  onSample[ 4][0] = true;  onSample[ 4][1] = true;  onSample[ 4][2] = false; onSample[ 4][3] = false; // Variations.FgdPid
  onSample[ 5][0] = true;  onSample[ 5][1] = true;  onSample[ 5][2] = true;  onSample[ 5][3] = true;  // Variations.ToFResol
  onSample[ 6][0] = false; onSample[ 6][1] = false; onSample[ 6][2] = true;  onSample[ 6][3] = true;  // Variations.ECalMomScale    
  onSample[ 7][0] = false; onSample[ 7][1] = false; onSample[ 7][2] = true;  onSample[ 7][3] = true;  // Variations.ECalMomResol    

  onSample[ 8][0] = true;  onSample[ 8][1] = true;  onSample[ 8][2] = true;  onSample[ 8][3] = true;  // Weights.ChargeConf
  onSample[ 9][0] = true;  onSample[ 9][1] = true;  onSample[ 9][2] = true;  onSample[ 9][3] = true;  // Weights.TpcClusterEff
  onSample[10][0] = true;  onSample[10][1] = true;  onSample[10][2] = true;  onSample[10][3] = true;  // Weights.TpcTrackEff
  onSample[11][0] = true;  onSample[11][1] = true;  onSample[11][2] = true;  onSample[11][3] = true;  // Weights.TpcFgdMatchEff
  onSample[12][0] = true;  onSample[12][1] = true;  onSample[12][2] = true;  onSample[12][3] = true;  // Weights.FgdTrackEff
  onSample[13][0] = true;  onSample[13][1] = true;  onSample[13][2] = false; onSample[13][3] = false; // Weights.FgdHybridTrackEff  
  onSample[14][0] = true;  onSample[14][1] = true;  onSample[14][2] = false; onSample[14][3] = false; // Weights.MichelEleEff
  onSample[15][0] = true;  onSample[15][1] = true;  onSample[15][2] = true;  onSample[15][3] = true;  // Weights.PileUp
  onSample[16][0] = true;  onSample[16][1] = true;  onSample[16][2] = true;  onSample[16][3] = true;  // Weights.FgdMass
  onSample[17][0] = true;  onSample[17][1] = true;  onSample[17][2] = false; onSample[17][3] = false; // Weights.OOFV
  onSample[18][0] = true;  onSample[18][1] = true;  onSample[18][2] = true;  onSample[18][3] = true;  // Weights.SIPion
  onSample[19][0] = true;  onSample[19][1] = true;  onSample[19][2] = true;  onSample[19][3] = true;  // Weights.SIProton
  onSample[20][0] = true;  onSample[20][1] = true;  onSample[20][2] = true;  onSample[20][3] = true;  // Weights.SandMuons
  
  onSample[21][0] = false; onSample[21][1] = false; onSample[21][2] = true;  onSample[21][3] = true;  // Weights.ECalPID
  onSample[22][0] = false; onSample[22][1] = false; onSample[22][2] = true;  onSample[22][3] = true;  // Weights.TpcECalMatchEff
  onSample[23][0] = false; onSample[23][1] = false; onSample[23][2] = true;  onSample[23][3] = true;  // Weights.ECalEmHipPID
  onSample[24][0] = false; onSample[24][1] = false; onSample[24][2] = true;  onSample[24][3] = true;  // Weights.FGD2Shower
  
  onSample[25][0] = false; onSample[25][1] = false; onSample[25][2] = false; onSample[25][3] = false; // Weights.NuETPCPileUp
  onSample[26][0] = false; onSample[26][1] = false; onSample[26][2] = true;  onSample[26][3] = true;  // Weights.NuEP0DPileUp
  onSample[27][0] = false; onSample[27][1] = false; onSample[27][2] = true;  onSample[27][3] = true;  // Weights.NuEECalPileUp   
  onSample[28][0] = false; onSample[28][1] = false; onSample[28][2] = true;  onSample[28][3] = true;  // Weights.NuEOOFV
  onSample[29][0] = false; onSample[29][1] = false; onSample[29][2] = true;  onSample[29][3] = true;  // Weights.Flux
   
  // nue / numu          FGD1 / FGD2
  Decorr[ 0][0] = false; Decorr[ 0][1] = false; // Variations.BFieldDist
  Decorr[ 1][0] = false; Decorr[ 1][1] = false; // Variations.MomScale	      
  Decorr[ 2][0] = false; Decorr[ 2][1] = false; // Variations.MomResol	      
  Decorr[ 3][0] = false; Decorr[ 3][1] = false; // Variations.TpcPid	      
  Decorr[ 4][0] = false; Decorr[ 4][1] = false; // Variations.FgdPid	      
  Decorr[ 5][0] = false; Decorr[ 5][1] = false; // Variations.ToFResol	      
  Decorr[ 6][0] = false; Decorr[ 6][1] = false; // Variations.ECalMomScale    
  Decorr[ 7][0] = false; Decorr[ 7][1] = false; // Variations.ECalMomResol    
  Decorr[ 8][0] = true;  Decorr[ 8][1] = false; // Weights.ChargeConf	      
  Decorr[ 9][0] = true;  Decorr[ 9][1] = false; // Weights.TpcClusterEff      
  Decorr[10][0] = true;  Decorr[10][1] = false; // Weights.TpcTrackEff	      
  Decorr[11][0] = true;  Decorr[11][1] = false; // Weights.TpcFgdMatchEff     
  Decorr[12][0] = true;  Decorr[12][1] = false; // Weights.FgdTrackEff	      
  Decorr[13][0] = false; Decorr[13][1] = false; // Weights.FgdHybridTrackEff  
  Decorr[14][0] = false; Decorr[14][1] = false; // Weights.MichelEleEff	      
  Decorr[15][0] = false; Decorr[15][1] = false; // Weights.PileUp
  Decorr[16][0] = false; Decorr[16][1] = false; // Weights.FgdMass
  Decorr[17][0] = false; Decorr[17][1] = false; // Weights.OOFV
  Decorr[18][0] = false; Decorr[18][1] = false; // Weights.SIPion
  Decorr[19][0] = false; Decorr[19][1] = false; // Weights.SIProton
  Decorr[20][0] = false; Decorr[20][1] = false; // Weights.SandMuons
  
  Decorr[21][0] = false; Decorr[21][1] = false; // Weights.ECalPID
  Decorr[22][0] = false; Decorr[22][1] = false; // Weights.TpcECalMatchEff
  Decorr[23][0] = false; Decorr[23][1] = false; // Weights.ECalEmHipPID
  Decorr[24][0] = false; Decorr[24][1] = false; // Weights.FGD2Shower
  
  Decorr[25][0] = false; Decorr[25][1] = false; // Weights.NuETPCPileUp
  Decorr[26][0] = false; Decorr[26][1] = false; // Weights.NuEP0DPileUp
  Decorr[27][0] = false; Decorr[27][1] = false; // Weights.NuEECalPileUp   
  Decorr[28][0] = false; Decorr[28][1] = false; // Weights.NuEOOFV
  Decorr[29][0] = false; Decorr[29][1] = false; // Weights.Flux 

  


  systMap["BFieldDist"]        = 0;
  systMap["MomScale"]          = 1;
  systMap["MomResol"]          = 2;
  systMap["TpcPid"]            = 3;
  systMap["FgdPid"]            = 4;
  systMap["ToFResol"]          = 5;
  systMap["ECalEMResol"]       = 6;
  systMap["ECalEMScale"]       = 7;
  systMap["ChargeIDEff"]       = 8;
  systMap["TpcClusterEff"]     = 9;
  systMap["TpcTrackEff"]       = 10;
  systMap["TpcFgdMatchEff"]    = 11;
  systMap["FgdTrackEff"]       = 12;
  systMap["FgdHybridTrackEff"] = 13;
  systMap["MichelEleEff"]      = 14;
  systMap["PileUp"]            = 15;
  systMap["FgdMass"]           = 16;
  systMap["OOFV"]              = 17;
  systMap["SIPion"]            = 18;
  systMap["SIProton"]          = 19;
  systMap["SandMu"]            = 20;
  systMap["ECalPID"]           = 21;
  systMap["TpcECalMatchEff"]   = 22;
  systMap["ECalEmHipPID"]      = 23;
  systMap["FGD2Shower"]        = 24;
  systMap["NuETPCPileUp"]      = 25;
  systMap["NuEP0DPileUp"]      = 26;
  systMap["NuEECalPileUp"]     = 27;
  systMap["NuEOOFV"]           = 28;
  systMap["FluxWeight"]        = 29;
  //Set up the AnalysisManagers.
  AnalysisManager _man;

  if (!_man.input().Initialize(fFileName.c_str(),"", false)) return false;
  int nmax_entries = fNEvts;
  int nmax_events =-1;
  
  // Get the number of entries in the tree
  if ( fNEvts < 0 ) {
    nmax_entries = (int)_man.input().GetEntries();
  }
  else {
    nmax_entries = std::min(nmax_entries, (int)_man.input().GetEntries());
  }
  
  std::cout << " We will process ";
  if (nmax_events==-1){ 
    // Get the numer of events 
    nmax_events=_man.input().GetNEvents(nmax_entries);
    std::cout << nmax_entries << " entries." << std::endl;
  }
  else
    std::cout << nmax_events << " events." << std::endl;
  
  int nv;
  _man.syst().GetVariationSystematics(nv);
  int nw;
  _man.syst().GetWeightSystematics(nw);

  std::vector<SystematicBase*>  allSyst = _man.syst().GetSystematics();

  for(int i = 0; i < allSyst.size(); i++){
    if(allSyst[i])
      std::cout << allSyst[i]->GetName() << " has index " << allSyst[i]->GetIndex() << std::endl;
  }

  
  // I believe these should have the same seed but I'm not entirely sure...
  BANFFNuMuFGD1ToyMaker* toyMakerNuMuFGD1 = new BANFFNuMuFGD1ToyMaker((UInt_t)ND::params().GetParameterI("psycheSteering.Systematics.RandomSeed"), 
								      (bool)  ND::params().GetParameterI("psycheSteering.Systematics.ZeroVariation"));
  
  BANFFNuMuFGD2ToyMaker* toyMakerNuMuFGD2 = new BANFFNuMuFGD2ToyMaker((UInt_t)ND::params().GetParameterI("psycheSteering.Systematics.RandomSeed"), 
								      (bool)  ND::params().GetParameterI("psycheSteering.Systematics.ZeroVariation"));
  
  BANFFNuEFGD1ToyMaker* toyMakerNuEFGD1   = new BANFFNuEFGD1ToyMaker ((UInt_t)ND::params().GetParameterI("psycheSteering.Systematics.RandomSeed"), 
                                                                      (bool)  ND::params().GetParameterI("psycheSteering.Systematics.ZeroVariation"));

  BANFFNuEFGD2ToyMaker* toyMakerNuEFGD2   = new BANFFNuEFGD2ToyMaker ((UInt_t)ND::params().GetParameterI("psycheSteering.Systematics.RandomSeed"), 
                                                                      (bool)  ND::params().GetParameterI("psycheSteering.Systematics.ZeroVariation"));

  std::vector<double> systpull[nThrows];
  TRandom3 random(5923211);

  nRows = 0;
  
  std::vector<SystematicBase*> myAllSyst;
  std::vector<TString> paramStdString;

  for(unsigned int isyst = 0; isyst < allSyst.size(); isyst++){
    SystematicBase *syst = allSyst[isyst];
    if(!syst) continue;

    int index = systMap[syst->GetName()];

    if(!onSample[index][0] &&
       !onSample[index][1] && 
       !onSample[index][2] && 
       !onSample[index][3])
      applySyst[index] = false;

    if(!applySyst[index]){
      if(verbose) std::cout << "not applying " << syst->GetName()<< std::endl;
      continue;
    }

    int npar  = syst->GetNParameters();
    if(verbose) std::cout << syst->GetName() << " -> " << syst->GetNParameters() << std::endl;
    myAllSyst.push_back(syst);
    systFirstIndex[syst->GetName()] = nRows;
    paramStdString.push_back(Form("%s_%i",syst->GetName(),nRows));
    nRows = nRows + npar + Rest(index, npar);
  }	
  
  if(verbose) std::cout << "nRows : " << nRows << std::endl;

  TObjArray *allParam = new TObjArray((int)paramStdString.size()); // DEBUG purpose
  for(int i = 0; i < (int)paramStdString.size(); i++){
    allParam->AddAt(new TObjString(paramStdString[i]), i);
  }

  TMatrixTSym<double> *corr = new TMatrixTSym<double>(nRows);
  //first set diagonal to 1 all other elements to 0 - since Symm only need to set one side
  //note : due to how rounding works diag is slightly above one to let TDecompChol work for fully correlated case.
  for(int i = 0; i < nRows; i++){
    for(int j = 0; j < nRows; j++){
      if(i==j){(*corr)(i,j) = 1 + fSmallNumber; }
      else    {(*corr)(i,j) = 0; }
    }
  }

  double scorr = 1;


  for(unsigned int isyst = 0; isyst < myAllSyst.size(); isyst++){
    SystematicBase* tmp = myAllSyst[isyst];
    std::string sname = tmp->Name();
    int index = systMap[sname];
    int npar = tmp->GetNParameters();

    if(verbose) std::cout << sname << " has " << tmp->GetNParameters() << " random numbers" << std::endl;        

    // Full correlations for all the bins (note that this doesn't take care of correlations inter samples)
    for(int iSample = 0; iSample < 4; iSample++){
      
      int r = RowIndex(index,iSample)*npar + systFirstIndex[sname];
      //std::cout << r << "  " << sname << std::endl;
      if(sname == "MomResol"          ||
	 sname == "ChargeIDEff"       ||
	 sname == "TpcClusterEff"     ||
	 sname == "TpcTrackEff"       ||
	 sname == "TpcFgdMatchEff"    ||
	 sname == "MichelEleEff")
	for(int i1 = 0; i1 < npar; i1++)
	  for(int i2 = 0; i2 < npar; i2++)
	    FillCorr(r+i1, r+i2, corr, scorr);     
      
      //Weird cases
      if(sname == "TpcPid"){
	
   	// TPCPID Muon Sig (24 param)
	for(int i1 = 0; i1 < 24; i1++)
	  for(int i2 = 0; i2 < 24; i2++)
	    FillCorr(r+i1, r+i2, corr, scorr);
	  
	// TPCPID Elec Sig (5 param)
	for(int i1 = 24; i1 < 24+5; i1++)
	  for(int i2 = 24; i2 < 24+5; i2++)
	    FillCorr(r+i1, r+i2, corr, scorr);
	
	// TPCPID Prot Sig (16 param)
	for(int i1 = 24+5; i1 < 24+5+16; i1++)
	  for(int i2 = 24+5; i2 < 24+5+16; i2++)
	    FillCorr(r+i1, r+i2, corr, scorr);

	// TPCPID Muon Mean (24 param)
	for(int i1 = npar/2; i1 < npar/2 + 24; i1++)
	  for(int i2 = npar/2; i2 < npar/2 + 24; i2++)
	    FillCorr(r+i1, r+i2, corr, scorr);
	
	// TPCPID Elec Mean (5 param)
	for(int i1 = npar/2 + 24; i1 < npar/2 + 24+5; i1++)
	  for(int i2 = npar/2 + 24; i2 < npar/2 + 24+5; i2++)
	    FillCorr(r+i1, r+i2, corr, scorr);
	
	// TPCPID Prot Mean (16 param)
	for(int i1 = npar/2 + 24+5; i1 < npar; i1++)
	  for(int i2 = npar/2 + 24+5; i2 < npar; i2++)
	    FillCorr(r+i1, r+i2, corr, scorr);
	  
	// TPCPID Muon Sig and Mean
	for(int i1 = 0; i1 < 24; i1++)
	  for(int i2 = npar/2; i2 < 24 + npar/2; i2++){
	    FillCorr(r+i1, r+i2, corr, scorr);
	    FillCorr(r+i2, r+i1, corr, scorr);
	  }
	
	// TPCPID Elec Sig and Mean
	for(int i1 = 24; i1 < 24+5; i1++)
	  for(int i2 = 24 + npar/2; i2 < 24+5 + npar/2; i2++){
	    FillCorr(r+i1, r+i2, corr, scorr);
	    FillCorr(r+i2, r+i1, corr, scorr);
	  }
	
	// TPCPID Prot Sig and Mean
	for(int i1 = 24+5; i1 < npar/2; i1++)
	  for(int i2 = 24+5 + npar/2; i2 < npar; i2++){
	    FillCorr(r+i1, r+i2, corr, scorr);
	    FillCorr(r+i2, r+i1, corr, scorr);
	  }
	
      }
    
      
      if(sname == "FgdPid"){
	
	// Note: it looks like the FGD1 and FGD2 are already correlated for this syst inside psyche
	// (doesn't read the last 4 parameters and apply FGD1 numbers to FGD2)
	// I'll fill a bug and probably this will be changed at some point
	// The structure is then:
	// 0 Muon FGD1 Mean
	// 1 Proton FGD1 Mean
	// 2 Muon FGD1 Sig
	// 3 Proton FGD1 Sig
	// 4 Muon FGD2 Mean
	// 5 Proton FGD2 Mean
	// 6 Muon FGD2 Sig
	// 7 Proton FGD2 Sig
	// Muon Mean and Sigma FGD1
	FillCorr(r+0, r+2, corr, scorr);
	FillCorr(r+2, r+0, corr, scorr);
	// Muon Mean and Sigma FGD2
	FillCorr(r+4, r+6, corr, scorr);
	FillCorr(r+6, r+4, corr, scorr);
	// Muon Mean and Sigma FGD1 and FGD2
	FillCorr(r+0, r+4, corr, scorr);
	FillCorr(r+4, r+0, corr, scorr);
	FillCorr(r+0, r+6, corr, scorr);
	FillCorr(r+6, r+0, corr, scorr);
	FillCorr(r+2, r+4, corr, scorr);
	FillCorr(r+4, r+2, corr, scorr);
	FillCorr(r+2, r+6, corr, scorr);
	FillCorr(r+6, r+2, corr, scorr);
	// Proton Mean and Sigma FGD1
	FillCorr(r+1, r+3, corr, scorr);
	FillCorr(r+3, r+1, corr, scorr);
	// Proton Mean and Sigma FGD2
	FillCorr(r+5, r+7, corr, scorr);
	FillCorr(r+7, r+5, corr, scorr);
	// Proton Mean and Sigma FGD1 and FGD2
	FillCorr(r+1, r+5, corr, scorr);
	FillCorr(r+5, r+1, corr, scorr);
	FillCorr(r+1, r+7, corr, scorr);
	FillCorr(r+7, r+1, corr, scorr);
	FillCorr(r+3, r+5, corr, scorr);
	FillCorr(r+5, r+3, corr, scorr);
	FillCorr(r+3, r+7, corr, scorr);
	FillCorr(r+7, r+3, corr, scorr);
      }

      if(sname == "OOFV"   ||
	 sname == "NuEOOFV"){
	// Same applies here, reco OOFV are all 100% correlated even if you have 5+5
	// parameters for the FGD1 and FGD2 -> Submitting a bug
	// The structure is then:
	// 5 FGD1 reco
	// 5 FGD2 reco
	// these are independant amonst them but there is correlation FGD1/FGD2
	// 5 rate uncorrelated for both FGDs...
	
	for(int i1 = 0; i1 < 5; i1++){
	  FillCorr(r+i1,   r+i1+5, corr, scorr);
	  FillCorr(r+i1+5, r+i1, corr, scorr);
	}
      }
    
      
      if(sname == "PileUp" ||
	 sname == "NuETPCPileUp"  ||
	 sname == "NuEP0DPileUp"  ||
	 sname == "NuEECalPileUp"){
	for(int i1 = 0; i1 < 8; i1++)
	  for(int i2 = 0; i2 < 8; i2++){
	    // FGD1
	    FillCorr(r+i1, r+i2, corr, scorr);
	    FillCorr(r+i2, r+i1, corr, scorr);
	    // FGD2
	    FillCorr(r+i1+npar/2, r+i2+npar/2, corr, scorr);
	    FillCorr(r+i2+npar/2, r+i1+npar/2, corr, scorr);
	    // FGD1/FGD2
	    FillCorr(r+i1       , r+i2+npar/2, corr, scorr);
	    FillCorr(r+i2+npar/2, r+i1       , corr, scorr);
	  }
	for(int i1 = 8; i1 < npar/2; i1++)
	  for(int i2 = 8; i2 < npar/2; i2++){
	    // FGD1
	    FillCorr(r+i1, r+i2, corr, scorr);
	    FillCorr(r+i2, r+i1, corr, scorr);
	    // FGD2
	    FillCorr(r+i1+npar/2, r+i2+npar/2, corr, scorr);
	    FillCorr(r+i2+npar/2, r+i1+npar/2, corr, scorr);
	    // FGD1/FGD2
	    FillCorr(r+i1       , r+i2+npar/2, corr, scorr);
	    FillCorr(r+i2+npar/2, r+i1       , corr, scorr);
	  }        
      }
      
      
      if(sname == "SandMu"){
	FillCorr(r+1, r+2, corr, scorr);
	FillCorr(r+2, r+1, corr, scorr);
      }
    }
  }
  
  //correlation matrix has been constructed - now decompose to L,U 
  //printout to check corr matrix is therer

  //double_t d1;
  //double_t d2;
  //(*corr).Determinant(d1,d2);
  //std::cout << "Determinant is : " <<  (*corr).Determinant() << std::endl;
  //for testing: make a root outfile to save the matrices and vectors to
  //(*corr).Write("corr0");
  TMatrixT<double> *ltri;
  TMatrixTSym<double> *smallercorr = new TMatrixTSym<double>(corr->GetNrows()); 

  for(int i = 0; i < corr->GetNrows(); i++)
    for(int j = 0; j < corr->GetNrows(); j++)
      (*smallercorr)(i, j) = (*corr)(i, j);

  // TFile* f = new TFile("CorrelationMatrix.root", "RECREATE");
  // corr->Write();
  // f->Close();

  TDecompChol *chdcmp = new TDecompChol(*corr);
  //std::cout << "Condition is : " <<  chdcmp.Condition() << std::endl;    
  //check it decomposes:
  int chuck = 0;
  while(!chdcmp->Decompose()){
    std::cout << "row " << nRows - chuck << std::endl;
    std::cout << "Cholesky decomposition failed!" << std::endl;
    if(smallercorr)
      delete smallercorr;
    
    smallercorr = new TMatrixTSym<double>(corr->GetNrows() - chuck); 
    for(int i = 0; i < corr->GetNrows() - chuck; i++)
      for(int j = 0; j < corr->GetNrows() - chuck; j++)
	(*smallercorr)(i, j) = (*corr)(i, j);

    delete chdcmp;
    chdcmp = new TDecompChol(*smallercorr);
    chuck++;
  }

  std::cout << "Decomposed after attempt " << chuck << std::endl;

  if(chuck != 0){
    std::cout << "Check the line " << nRows - chuck << " there is probably something wrong there in the correlation matrix as the Cholesky decomposition failed." << std::endl;
    std::cout << "For now, exit..." << std::endl;
  }

  //get upper triangular matrix - want lower so transpose
  ltri = new TMatrixT<double>(chdcmp->GetU());
  (*ltri).Transpose(*ltri);
  //for checking purposes
  //(*ltri).Print();
  //(*ltri).Write("ltri0");
	
  /// Define the throws and generate them 
  // First set of throws
  std::vector<TString> systList;
  
  for(int itoy = 0; itoy < nThrows; itoy++){
    for(int isyst = 0; isyst < (int)myAllSyst.size(); isyst++){
      SystematicBase* tmp =  myAllSyst[isyst];
      if(!tmp) continue;
      
      int index = systMap[tmp->GetName()];
      if(index < 0) continue;

      for(int irow = 0; irow < RowIndex(index,3) + 1; irow++)
	for(int ipar = 0; ipar < (int)myAllSyst[isyst]->GetNParameters(); ipar++){
	  Float_t var;
	  if(tmp->PDF() == SystematicBase::kUniform){ var=random.Uniform(0.,1.); }
	  else                                      { var=random.Gaus(0.,1.);    }
	    
	  if(itoy < 2) var = 0;
	  //for testing only
	  //if(itoy>1) var = .5; 
	  if(itoy == 0) systList.push_back(Form("%s_%i", myAllSyst[isyst]->GetName(), ipar));
	  systpull[itoy].push_back(var);
	}
    }
  }

  TObjArray *allParamDetailed = new TObjArray((int)systList.size()); // DEBUG purpose

  for(int i = 0; i < (int)systList.size(); i++){
    allParamDetailed->AddAt(new TObjString(Form("%i_", i) + systList[i]), i);
  }

  std::cout << "Correlation matrix is made" << std::endl;
  if(verbose){
    TFile * testfile = new TFile("test.root", "RECREATE");
    (*corr).Write("theMatrix");
    (*allParam).Write("list", TObject::kSingleKey);
    (*allParamDetailed).Write("listDetailed", TObject::kSingleKey);
    testfile->Close();
    std::cout << "systpull size : " << systpull[0].size() << std::endl;

    for(int i = 0; i < (int)systList.size(); i++)
      std::cout << i << " " <<systList[i] << std::endl;
  }    

  if(chuck != 0)
    exit(1);
  
  

  //we have one large vector per toy
  //now loop over toys and multiply our transposed matrix
  for(int itoy = 0; itoy < nThrows; itoy++){
    //for each vector make it into a TVectorD
    TVectorT<double> vec(systpull[itoy].size(),&systpull[itoy][0]);
    //vec2 is for output purposes and testing
    //TVectorT<double> vec2(systpull[itoy].size(),&systpull[itoy][0]);
    //check that the vector has been filled - for DEBUG
    //if(itoy == 2){
    //vec2.Write("throws1");
    //vec2.Print();
    //}
    //std::cout << "systpull size : " << systpull[itoy].size() << std::endl;
    //multiply by lower triangular matrix
    vec *= (*ltri);
    //check the multiplication was successful - for DEBUG
    //if(itoy == 2){
    //vec.Write("throws2");
    //vec.Print();
    //}
    //values are now correlated
    //put values back into a std::vector for each toy
    //if(itoy > 1){
    //std::cout << "Toy is " << itoy << std::endl;}
    for(int j = 0; j < nRows; j++){
      systpull_corr[itoy].push_back(vec[j]);
      //if(itoy > 1){std::cout << "itoy " << itoy << "   -> "<< j << " : " << systpull_corr[itoy][j] << std::endl;}
      
    }
  }
  //outtest->Close();
	



#ifdef __T2KRW_NIWG_ENABLED__
#ifdef __T2KRW_NEUT_ENABLED__
  // Make a t2kreweighting object and add a NIWG and NEUT weighting engine. 
  std::vector< t2krew::ET2KSyst > dials;
  t2krew::T2KReWeight rw; 
  rw.AdoptWghtEngine("neut_rw", new t2krew::T2KNeutReWeight());
  rw.AdoptWghtEngine("niwg_rw", new t2krew::T2KNIWGReWeight());

  // Uncertainties
  // CCQE:
  std::cout << "Setting the NIWG dials" << std::endl;
  rw.Systematics().Include(t2krew::kNIWG2014a_pF_C12);
  rw.Systematics().Include(t2krew::kNIWG2014a_pF_O16);
  rw.Systematics().Include(t2krew::kNIWG2014a_Eb_C12);
  rw.Systematics().Include(t2krew::kNIWG2014a_Eb_O16);
  rw.Systematics().Include(t2krew::kNIWGMEC_Norm_C12);
  rw.Systematics().Include(t2krew::kNIWGMEC_Norm_O16);  
  rw.Systematics().Include(t2krew::kNIWGMEC_q3Cut);  
  rw.Systematics().Include(t2krew::kNXSec_MaCCQE);
  rw.Systematics().Include(t2krew::kNXSec_VecFFCCQE); // used to set MAQE to act according to for RFG MC (2) or SF MC (402)

  // CC and NC single pion resonance:
  rw.Systematics().Include(t2krew::kNXSec_CA5RES);
  rw.Systematics().Include(t2krew::kNXSec_MaNFFRES);
  rw.Systematics().Include(t2krew::kNXSec_BgSclRES);

  // nue/numu uncertainties
  rw.Systematics().Include(t2krew::kNXSec_SCCVecQE);
  rw.Systematics().Include(t2krew::kNXSec_SCCAxlQE);
  rw.Systematics().Include(t2krew::kNIWG2012a_ccnueE0); 

  // All other CC and NC
  rw.Systematics().Include(t2krew::kNIWG2012a_dismpishp);
  rw.Systematics().Include(t2krew::kNIWG2012a_cccohE0);
  rw.Systematics().Include(t2krew::kNIWG2012a_nccohE0);
  rw.Systematics().Include(t2krew::kNIWG2012a_ncotherE0); 

  rw.Systematics().Include(t2krew::kNIWG2014a_SF_RFG);
  rw.Systematics().Include(t2krew::kNIWG_rpaCCQE_norm);
  rw.Systematics().Include(t2krew::kNIWG_rpaCCQE_shape);

  // Add FSI dials
  rw.Systematics().Include(t2krew::kNCasc_FrAbs_pi);
  rw.Systematics().Include(t2krew::kNCasc_FrCExLow_pi);
  rw.Systematics().Include(t2krew::kNCasc_FrInelLow_pi);
  rw.Systematics().Include(t2krew::kNCasc_FrPiProd_pi);
  rw.Systematics().Include(t2krew::kNCasc_FrCExHigh_pi);
  rw.Systematics().Include(t2krew::kNCasc_FrInelHigh_pi);

  //-- PDD Weights, New Eb dial
  rw.Systematics().Include(t2krew::kNIWGMEC_PDDWeight_C12);
  rw.Systematics().Include(t2krew::kNIWGMEC_PDDWeight_O16);
  rw.Systematics().Include(t2krew::kNNucl_CCQEBindingEnergy_C12);
  rw.Systematics().Include(t2krew::kNNucl_CCQEBindingEnergy_O16);


  dials.push_back(t2krew::kNIWG2014a_pF_C12);
  dials.push_back(t2krew::kNIWG2014a_pF_O16);
  dials.push_back(t2krew::kNIWG2014a_Eb_C12);
  dials.push_back(t2krew::kNIWG2014a_Eb_O16);
  dials.push_back(t2krew::kNIWGMEC_Norm_C12);
  dials.push_back(t2krew::kNIWGMEC_Norm_O16);  
  dials.push_back(t2krew::kNIWGMEC_q3Cut);  
  dials.push_back(t2krew::kNXSec_MaCCQE);
  dials.push_back(t2krew::kNXSec_VecFFCCQE); // used to set MAQE to act according to for RFG MC (2) or SF MC (402)
  dials.push_back(t2krew::kNXSec_CA5RES);
  dials.push_back(t2krew::kNXSec_MaNFFRES);
  dials.push_back(t2krew::kNXSec_BgSclRES);
  dials.push_back(t2krew::kNXSec_SCCVecQE);
  dials.push_back(t2krew::kNXSec_SCCAxlQE);
  dials.push_back(t2krew::kNIWG2012a_ccnueE0); 
  dials.push_back(t2krew::kNIWG2012a_dismpishp);
  dials.push_back(t2krew::kNIWG2012a_cccohE0);
  dials.push_back(t2krew::kNIWG2012a_nccohE0);
  dials.push_back(t2krew::kNIWG2012a_ncotherE0); 
  dials.push_back(t2krew::kNIWG2014a_SF_RFG);
  dials.push_back(t2krew::kNIWG_rpaCCQE_norm);
  dials.push_back(t2krew::kNIWG_rpaCCQE_shape);
  dials.push_back(t2krew::kNCasc_FrAbs_pi);
  dials.push_back(t2krew::kNCasc_FrCExLow_pi);
  dials.push_back(t2krew::kNCasc_FrInelLow_pi);
  dials.push_back(t2krew::kNCasc_FrPiProd_pi);
  dials.push_back(t2krew::kNCasc_FrCExHigh_pi);
  dials.push_back(t2krew::kNCasc_FrInelHigh_pi);
  dials.push_back(t2krew::kNIWGMEC_PDDWeight_C12);
  dials.push_back(t2krew::kNIWGMEC_PDDWeight_O16);
  dials.push_back(t2krew::kNNucl_CCQEBindingEnergy_C12);
  dials.push_back(t2krew::kNNucl_CCQEBindingEnergy_O16);
  
  // CCQE:
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2014a_pF_C12);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2014a_pF_O16);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2014a_Eb_C12);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2014a_Eb_O16);
  rw.Systematics().SetAbsTwk(t2krew::kNIWGMEC_Norm_C12);
  rw.Systematics().SetAbsTwk(t2krew::kNIWGMEC_Norm_O16);  
  rw.Systematics().SetAbsTwk(t2krew::kNIWGMEC_q3Cut);  
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_MaCCQE);

  // CC and NC single pion resonance:
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_CA5RES);
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_MaNFFRES);
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_BgSclRES);

  // nue/numu uncertainties
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_SCCVecQE);
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_SCCAxlQE);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_ccnueE0); 

  // All other CC and NC
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_dismpishp);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_cccohE0);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_nccohE0);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_ncotherE0);

  rw.Systematics().SetAbsTwk(t2krew::kNIWG_rpaCCQE_norm);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG_rpaCCQE_shape);

  // FSI dials
  rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrAbs_pi);
  rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrCExLow_pi);
  rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrInelLow_pi);
  rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrPiProd_pi);
  rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrCExHigh_pi);
  rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrInelHigh_pi);

  //-- PDD Weights, New Eb dial
  rw.Systematics().SetAbsTwk(t2krew::kNIWGMEC_PDDWeight_C12);
  rw.Systematics().SetAbsTwk(t2krew::kNIWGMEC_PDDWeight_O16);
  rw.Systematics().SetAbsTwk(t2krew::kNNucl_CCQEBindingEnergy_C12);
  rw.Systematics().SetAbsTwk(t2krew::kNNucl_CCQEBindingEnergy_O16);
  

  // Initialise dials to nominal
  for(unsigned int sys_iter1=0; sys_iter1<dials.size(); sys_iter1++){
    rw.Systematics().SetTwkDial(dials[sys_iter1], 0.);
  }
  // Apply nominal tuning
  rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_SF_RFG,0); // SF->RFG tuning
  rw.Systematics().SetTwkDial(t2krew::kNXSec_VecFFCCQE, 2); // Note that when we set MAQE, we need to set NXSec_VecFFCCQE to 2 for SF->RFG MC
  rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_norm,1);
  rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_shape,0);

  std::cout << "Setting the NIWG dials -done-" << std::endl;
  rw.Reconfigure();
  std::cout << "Reconfigured NIWG dials -done-" << std::endl;
  int prevTruthID = -1;
  float prevNIWGWeight = -1.;
  int prevRTV = -1;
  // int prevnomRTV = 0;
  //bool foundvtx = false;
  Int_t rtvi = 0;


  for(UInt_t isyst = 0; isyst < NMAXSYSTEMATICS; isyst++){
    SystematicBase* syst = allSyst[isyst];
    if(!syst) continue;
    int index = _man.syst().GetSystematicVariationIndex(syst->GetName());
    if(index != -1)
      _man.syst().EnableSystematic(isyst);
    index = _man.syst().GetSystematicWeightIndex(syst->GetName());
    if(index != -1)
      _man.syst().EnableSystematic(isyst);
  }
  
  toyMakerNuMuFGD1->CreateToyExperiments(nThrows, _man.syst().GetSystematics()); 
  toyMakerNuMuFGD2->CreateToyExperiments(nThrows, _man.syst().GetSystematics()); 
  toyMakerNuEFGD1 ->CreateToyExperiments(nThrows, _man.syst().GetSystematics()); 
  toyMakerNuEFGD2 ->CreateToyExperiments(nThrows, _man.syst().GetSystematics()); 
  std::cout << "Toy Experiments created" << std::endl;
  
  // Make and fill the EventSummary even when the selection is not passed.
  if(ND::params().GetParameterI("psycheSteering.Selections.ForceFillEventSummary"))
    _man.sel().SetForceFillEventSummary(true);

  if(verbose) _man.sel().DumpSelections();

  _man.sel().CreateToyBoxArray(nmax_events);
  // Create the array of SystBox
  _man.syst().Initialize(_man.sel(), nmax_events);
  
  _man.syst().DumpVariationSystematics();
  _man.syst().DumpWeightSystematics();
  
  TTree* RTV = (TTree*)infile->Get("NRooTrackerVtx");
  TClonesArray *nRooVtxs = new TClonesArray("ND::NRooTrackerVtx",100);
  int NRooVtx = 0;
  RTV->SetBranchAddress("Vtx",  &nRooVtxs);
  RTV->SetBranchAddress("NVtx", &NRooVtx);
  Long64_t entry = 0;
  Int_t evt = 0;
  
  while(entry < nmax_entries && evt < nmax_events){

    if(entry%10000 == 0)
      std::cout << "Progress: " << (double)entry / (double)nmax_entries * 100. << "%... nThrows = " << nThrows << std::endl;
    if(entry == 0){
      std::cout << "sampling time from " << entry << std::endl;
      time(&timer);
      seconds0 = difftime(timer,mktime(&y2k));
    }

    AnaEventB* event = NULL;
    if(!_man.LoadEvent(entry))
      continue;

    event = static_cast<AnaEventB*>(_man.input().GetSuperEvent().Event);

    if(!event){
      std::cout << "no event!" << std::endl;
      continue;
    }
    
    _man.sel().InitializeEvent(*event);
    int OverAllSample = 0;
     
    int nSelected = 0;
    bool foundvtx = false;
    
    _man.syst().InitializeEventSystematics(_man.sel(),*event);

    for (Int_t itoy = 0; itoy < nThrows; itoy++){
      FluxWeight[itoy]   = 1;
      DetWeight[itoy]    = 1;
      NIWGWeight[itoy]   = 1;

      PMu[itoy]     = -999;
      ThetaMu[itoy] = -999;

      //Get the event to be modified.

      WeightType totalWeightSyst;
      WeightType fluxWeightSyst;
            
      AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(event->Summary);
      ToyExperiment* toy = toyMakerNuMuFGD1->GetToyExperiment(itoy);
      
      _man.ProcessEvent(*toy, *event, totalWeightSyst, fluxWeightSyst);
      
      if(summary->EventSample != SampleId::kFGD1NuMuCC   &&
	 summary->EventSample != SampleId::kFGD1NuMuCCQE &&
	 
	 summary->EventSample != SampleId::kFGD1NuMuCC0Pi   &&
      	 summary->EventSample != SampleId::kFGD1NuMuCC1Pi   &&
      	 summary->EventSample != SampleId::kFGD1NuMuCCOther &&

         summary->EventSample != SampleId::kFGD1NuMuCC0PiFwd     &&
      	 summary->EventSample != SampleId::kFGD1NuMuCC1PiFwd     &&
      	 summary->EventSample != SampleId::kFGD1NuMuCCOtherFwd   &&
         summary->EventSample != SampleId::kFGD1NuMuCC0PiBwd     &&
      	 summary->EventSample != SampleId::kFGD1NuMuCC1PiBwd     &&
      	 summary->EventSample != SampleId::kFGD1NuMuCCOtherBwd   &&
         summary->EventSample != SampleId::kFGD1NuMuCC0PiHAFwd   &&
      	 summary->EventSample != SampleId::kFGD1NuMuCC1PiHAFwd   &&
      	 summary->EventSample != SampleId::kFGD1NuMuCCOtherHAFwd &&
         summary->EventSample != SampleId::kFGD1NuMuCC0PiHABwd   &&
      	 summary->EventSample != SampleId::kFGD1NuMuCC1PiHABwd   &&
      	 summary->EventSample != SampleId::kFGD1NuMuCCOtherHABwd &&

	 summary->EventSample != SampleId::kFGD1AntiNuMuCC        &&
	 summary->EventSample != SampleId::kFGD1AntiNuMuCC1Track  &&
	 summary->EventSample != SampleId::kFGD1AntiNuMuCCNTracks &&
	 summary->EventSample != SampleId::kFGD1AntiNuMuCC0Pi     &&
	 summary->EventSample != SampleId::kFGD1AntiNuMuCC1Pi     &&
	 summary->EventSample != SampleId::kFGD1AntiNuMuCCOther   &&

	 summary->EventSample != SampleId::kFGD1NuMuBkgInAntiNuModeCC        &&
	 summary->EventSample != SampleId::kFGD1NuMuBkgInAntiNuModeCC1Track  &&
	 summary->EventSample != SampleId::kFGD1NuMuBkgInAntiNuModeCCNTracks &&
	 summary->EventSample != SampleId::kFGD1NuMuBkgInAntiNuModeCC0Pi     &&
	 summary->EventSample != SampleId::kFGD1NuMuBkgInAntiNuModeCC1Pi     &&
	 summary->EventSample != SampleId::kFGD1NuMuBkgInAntiNuModeCCOther){

      	//Process the event.
        toy = toyMakerNuEFGD1->GetToyExperiment(itoy);
        _man.ProcessEvent(*toy, *event, totalWeightSyst, fluxWeightSyst);

        if(summary->EventSample != SampleId::kFGD1NuECC                &&
      	   summary->EventSample != SampleId::kFGD1AntiNuECC            &&
      	   summary->EventSample != SampleId::kFGD1Gamma                &&
           summary->EventSample != SampleId::kFGD1NuEBkgInAntiNuModeCC &&
      	   summary->EventSample != SampleId::kFGD1AntiNuEBkgInNuModeCC &&
      	   summary->EventSample != SampleId::kFGD1GammaInAntiNuMode){

      	  //Process the event.
          toy = toyMakerNuMuFGD2->GetToyExperiment(itoy);
          _man.ProcessEvent(*toy, *event, totalWeightSyst, fluxWeightSyst);

      	  if(summary->EventSample != SampleId::kFGD2NuMuCC   &&
	 
	     summary->EventSample != SampleId::kFGD2NuMuCC0Pi   &&
	     summary->EventSample != SampleId::kFGD2NuMuCC1Pi   &&
	     summary->EventSample != SampleId::kFGD2NuMuCCOther &&

	     summary->EventSample != SampleId::kFGD2NuMuCC0PiFwd     &&
	     summary->EventSample != SampleId::kFGD2NuMuCC1PiFwd     &&
	     summary->EventSample != SampleId::kFGD2NuMuCCOtherFwd   &&
	     summary->EventSample != SampleId::kFGD2NuMuCC0PiBwd     &&
	     summary->EventSample != SampleId::kFGD2NuMuCC1PiBwd     &&
	     summary->EventSample != SampleId::kFGD2NuMuCCOtherBwd   &&
	     summary->EventSample != SampleId::kFGD2NuMuCC0PiHAFwd   &&
	     summary->EventSample != SampleId::kFGD2NuMuCC1PiHAFwd   &&
	     summary->EventSample != SampleId::kFGD2NuMuCCOtherHAFwd &&
	     summary->EventSample != SampleId::kFGD2NuMuCC0PiHABwd   &&
	     summary->EventSample != SampleId::kFGD2NuMuCC1PiHABwd   &&
	     summary->EventSample != SampleId::kFGD2NuMuCCOtherHABwd &&

	     summary->EventSample != SampleId::kFGD2AntiNuMuCC        &&
	     summary->EventSample != SampleId::kFGD2AntiNuMuCC1Track  &&
	     summary->EventSample != SampleId::kFGD2AntiNuMuCCNTracks &&
	     summary->EventSample != SampleId::kFGD2AntiNuMuCC0Pi     &&
	     summary->EventSample != SampleId::kFGD2AntiNuMuCC1Pi     &&
	     summary->EventSample != SampleId::kFGD2AntiNuMuCCOther   &&

	     summary->EventSample != SampleId::kFGD2NuMuBkgInAntiNuModeCC        &&
	     summary->EventSample != SampleId::kFGD2NuMuBkgInAntiNuModeCC1Track  &&
	     summary->EventSample != SampleId::kFGD2NuMuBkgInAntiNuModeCCNTracks &&
	     summary->EventSample != SampleId::kFGD2NuMuBkgInAntiNuModeCC0Pi     &&
	     summary->EventSample != SampleId::kFGD2NuMuBkgInAntiNuModeCC1Pi     &&
	     summary->EventSample != SampleId::kFGD2NuMuBkgInAntiNuModeCCOther){
            
      	    //Process the event.
      	    toy = toyMakerNuEFGD2->GetToyExperiment(itoy);
            _man.ProcessEvent(*toy, *event, totalWeightSyst, fluxWeightSyst);
          }
      	}
      }
      
      Selection[itoy] = summary->EventSample;
                
      //Only interested if the event passed selection.
      if(summary->EventSample != SampleId::kUnassigned){
	if(summary->TrueVertex[summary->EventSample] != NULL){

	  PMu[itoy]     = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->Momentum;
	  ThetaMu[itoy] = summary->LeptonCandidate[summary->EventSample]->DirectionStart[2];

          FluxWeight[itoy] = fluxWeightSyst .Correction;
          DetWeight[itoy]  = totalWeightSyst.Systematic;
          // std::cout << std::endl;
          // std::cout << "Selection [" << itoy << "] " << Selection [itoy] << std::endl;
          // std::cout << "PMu       [" << itoy << "] " << PMu       [itoy] << std::endl;
          // std::cout << "ThetaMu   [" << itoy << "] " << ThetaMu   [itoy] << std::endl;
          // std::cout << "FluxWeight[" << itoy << "] " << FluxWeight[itoy] << std::endl;
          // std::cout << "DetWeight [" << itoy << "] " << DetWeight [itoy] << std::endl;
          
          //What we need to get the RooTrackerVtx is truth vertex identifier
	  //which is given by AnaTrueVertexB::TrueVertexID
	  Int_t TrueVertexID = summary->LeptonCandidate[summary->EventSample]->GetTrueParticle()->VertexID;

	  if(TrueVertexID == prevTruthID){
	    NIWGWeight[itoy] = prevNIWGWeight;
	    nSelected++;
	    continue;
	  }

	  if(event->EventInfo.IsSand){
	    NIWGWeight[itoy] = 1;
	    nSelected++;
	    continue;
	  }

	  //Use the TruthVertexID to get the RooTrackerVertex corresponding to this
	  //event.
	  ND::NRooTrackerVtx * vtx = NULL;
	  prevTruthID = TrueVertexID;
	  //if(itoy == 0){rtvi = prevnomRTV;}
	  prevRTV = rtvi;
	  if(!foundvtx){
	    while(vtx==NULL){
	      //Pull out the correct RooTrackerVtx tree entry.
	      RTV->GetEntry(rtvi);
	      for(int i = 0; i < NRooVtx; ++i){
		vtx = (ND::NRooTrackerVtx*)(*nRooVtxs)[i];
		if(vtx->TruthVertexID == TrueVertexID &&
		   fabs(vtx->StdHepP4[0][3]*1000 - summary->TrueVertex[summary->EventSample]->NuEnergy) < 0.1){
		  foundvtx = true;
		  break;
		}
		vtx = NULL;
	      }
	      if(vtx==NULL) rtvi++;
	      if(rtvi == RTV->GetEntries()){
		std::cout << "Looping to find correct vertex!" << std::endl;
		rtvi = 0;
		//break;
	      }
	      if(rtvi == prevRTV - 1){
		std::cout << "Event failed to find ANY vertex!" << std::endl;
		break;}
	    }
	  }else{
	    //may not work, depending on how RTV->GetEntry(rvti) works
	    for(int i = 0; i > NRooVtx; ++i){
	      vtx = (ND::NRooTrackerVtx*)(*nRooVtxs)[i];
	      if(vtx->TruthVertexID == TrueVertexID &&
		 fabs(vtx->StdHepP4[0][3]*1000 - summary->TrueVertex[summary->EventSample]->NuEnergy) < 0.1){
		break;
	      } 
	    }
	  }
	  if(vtx != NULL){
	    NIWGWeight[itoy] = rw.CalcWeight(vtx);


	    //Add Kendall's pion tuning stuff
	    double piEnergy = -1.0;
	    if(fabs(atoi(((vtx->EvtCode)->String()).Data() )) == 16){
	      for(int i = 0; i < event->nTrueParticles; ++i){
		if(fabs(event->TrueParticles[i]->PDG) != 211) continue;
		if(event->TrueParticles[i]->TrueVertex == NULL) continue;
		if(event->TrueParticles[i]->VertexID == TrueVertexID){
		  piEnergy = sqrt(event->TrueParticles[i]->Momentum * event->TrueParticles[i]->Momentum + 139.57 * 139.57) / 1000.0;
		}
	      }
	      if(piEnergy < 0){
		NIWGWeight[itoy] = 1*NIWGWeight[itoy];
	      }
	      else if(piEnergy < 0.25){
		NIWGWeight[itoy] = 0.135*NIWGWeight[itoy];
	      }
	      else if(piEnergy < 0.5){
		NIWGWeight[itoy] = 0.4*NIWGWeight[itoy];
	      }
	      else if(piEnergy < 0.75){
		NIWGWeight[itoy] = 0.294*NIWGWeight[itoy];
	      }
	      else if(piEnergy < 1.0){
		NIWGWeight[itoy] = 1.206*NIWGWeight[itoy];
	      }
	    }
	  }
	  prevNIWGWeight = NIWGWeight[itoy];
	} //End true vertex exists check.
	nSelected++;
      } //End passed check.
      
    }
    //end loop over toy.
    EventNumber = event->EventInfo.Event;
    Run         = event->EventInfo.Run;
    SubRun      = event->EventInfo.SubRun;
    // Reset the PreviousToyBox
    _man.syst().FinalizeEventSystematics(*event);
    _man.sel().FinalizeEvent(*event);
    evt++;

    if(nSelected > nThrows / 200){
      outtree->Fill();
    }
    if(entry == 1000){
      time(&timer);
      std::cout << "sampling time until " << entry << std::endl;
      seconds1 = difftime(timer,mktime(&y2k));
      std::cout << "time to process 1000 events: " << seconds1 - seconds0 << std::endl;
    }
  }
#endif
#endif

  outfile->cd();
  outtree->Write();
  outfile->Close();
  return 0;
}

// Print the cmd line syntax
void Usage(){
  std::cout << "Cmd line syntax should be:" << std::endl;
  std::cout << "generate_ND280_Systematics_Covariance_2015.cxx -p -s nd280_inputfile -o outputfile.root [-n nevents]" << std::endl;
}

int ParseArgs(int argc, char **argv){

  for (;;) {
    int c = getopt(argc, argv, "n:s:t:o:p");
    if (c < 0)
      break;
    switch (c){
    case 'p':
      {
	PreLoad = true;
	break;
      }
    case 'n':
      {
	std::istringstream tmp(optarg);
	tmp >> fNEvts;
	std::cout << "fNEvts " << fNEvts << std::endl;
	break;
      }
    case 's':
      {
	fFileName = optarg;
	break;
      }
    case 'o':
      {
	fOutfile = optarg;
	break;
      }
    case 'v':
      {
	verbose = true;
	break;
      }
    default:
      {
	std::cerr << "Option " << c << " is not recognised." << std::endl;
	Usage();
      }
    }
  }

  return 0;
}

