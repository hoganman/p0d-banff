//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Mar 29 22:02:16 2013 by ROOT version 5.28/00c
// from TTree sample_sum/Tracker Numu Summary
// found on file: /home/mhartz/work/trackerNuMufiles/BANFFTests/numuCCBANFF_5Emcp_run2_neut0_0_spline.root
//////////////////////////////////////////////////////////

#ifndef WeightFile_h
#define WeightFile_h
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TGraph.h>
#include <TSpline.h>
#include <TClonesArray.h>
#include "SampleId.hxx"

enum XsecSysts { NORM = -1,
                 MAQE = 0, 
                 MARES = 1, 
                 DISMPISHP = 2,
                 SF = 3,
                 EB = 4,
                 PF = 5,
                 PDD = 6,
                 FSI_INEL_LO = 7,
                 FSI_INEL_HI = 8,
                 FSI_PI_PROD = 9,
                 FSI_PI_ABS = 10,
                 FSI_CEX_LO = 11,
                 FSI_CEX_HI = 12,
                 PDD_MEC = 13,
                 MEC = 14,

                 //Newly added for 2014.
                 MEC_C = 15,
                 PF_C = 16,
                 EB_C = 17,
                 MEC_O = 18,
                 PF_O = 19,
                 EB_O = 20,
                 CA5 = 21,
                 BgSclRes = 22,
                 SCCV = 23,
                 SCCA = 24,
                 RPA = 25,
                 SF_RFG = 26,
                 CCNuE = 27,

                 // Add Kendall's weight for 2015
                 COH_BS = 28,

                 // 2016 Winter analysis
                 MEC_shape_C = 29,
                 MEC_shape_O = 30	
};

///A function for taking the parameter name and returning the XsecSysts enum
///value corresponding to that parameter.
XsecSysts ParamNameToXsecSysts(std::string paramName);

class WeightFile {
public :
  TChain          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain

  ///The filetype.
  ///0 for 2013
  ///1 for 2014
  ///Increment as needed.
  Int_t fileType;


  ///An array to hold the TGraphs

  // Declaration of leaf types

  //Tree entries common to 2013 and 2014.
  Int_t            NSamples;
  Double_t*        Enu;
  Double_t*        Pmu;
  Double_t*        CosThetamu;
  Int_t*           ReactionCode;
  Int_t*           NeutrinoCode;
  Double_t*        FluxWeight;
  Double_t*        DetWeight;
  Double_t*        POTWeight;
  Double_t*        PileUpWeight;
  Double_t         DetNomWeight;
  // Kendall's coherent tuning weight
  Double_t*        CohTuningWeight;
 
  Double_t*        TruePmu;
  Int_t*           TrueLepPDG;
  Double_t*        TrueCosThetamu;
  Double_t*        Q2;
  Double_t*        Q2QE;
  Char_t*          HaveTruth;
  Int_t*           TgtMat;
  Int_t*           NuParent;
  Int_t           Run;
  Int_t*           EventType;
  Int_t*           TruthVtx;
  TClonesArray    *MAQEGraph;
  TClonesArray    *MARESGraph;
  TClonesArray    *DISMPISHPGraph;
  TClonesArray    *FSI_PI_ABSGraph;
  TClonesArray    *FSI_CEX_LOGraph;
  TClonesArray    *FSI_INEL_LOGraph;
  TClonesArray    *FSI_PI_PRODGraph;
  TClonesArray    *FSI_CEX_HIGraph;
  TClonesArray    *FSI_INEL_HIGraph;
  //TODO: TGraph          *PDD_MECGraph;

  //Tree entries unique to 2013.
  TClonesArray    *MECGraph;
  TClonesArray    *SFGraph;
  TClonesArray    *EBGraph;
  TClonesArray    *PFGraph;
  TClonesArray    *PDDGraph;


  //Tree entries unique to 2014.
  TClonesArray*   MEC_CGraph;
  TClonesArray*   PF_CGraph;
  TClonesArray*   EB_CGraph;

  TClonesArray*   MEC_OGraph;
  TClonesArray*   PF_OGraph;
  TClonesArray*   EB_OGraph;

  TClonesArray* CA5Graph;
  TClonesArray* BgSclResGraph;
  TClonesArray* SCCVGraph;
  TClonesArray* SCCAGraph;
  TClonesArray* SF_RFGGraph;
  TClonesArray* RPAGraph;
  TClonesArray* CCNuEGraph;

  //Tree entries unique to 2016.
  TClonesArray* PDD_CGraph;
  TClonesArray* PDD_OGraph;


  //Splines to convert the TGraphs to.
  TSpline3**      MAQESpline;
  TSpline3**      MARESSpline;
  TSpline3**      DISMPISHPSpline;
  TSpline3**      SFSpline;
  TSpline3**      EBSpline;
  TSpline3**      PFSpline;
  TSpline3**      PDDSpline;
  TSpline3**      FSI_PI_ABSSpline;
  TSpline3**      FSI_CEX_LOSpline;
  TSpline3**      FSI_INEL_LOSpline;
  TSpline3**      FSI_PI_PRODSpline;
  TSpline3**      FSI_CEX_HISpline;
  TSpline3**      FSI_INEL_HISpline;
  TSpline3**      PDD_MECSpline;
  TSpline3**      MECSpline;


  TSpline3**       MEC_CSpline;
  TSpline3**       PF_CSpline;
  TSpline3**       EB_CSpline;

  TSpline3**       MEC_OSpline;
  TSpline3**       PF_OSpline;
  TSpline3**       EB_OSpline;

  TSpline3**       CA5Spline;
  TSpline3**       BgSclResSpline;
  TSpline3**       SCCVSpline;
  TSpline3**       SCCASpline;
  TSpline3**       SF_RFGSpline;
  TSpline3**       RPASpline;
  TSpline3**       CCNuESpline;

  TSpline3**       PDD_CSpline;
  TSpline3**       PDD_OSpline;

  // List of branches
  TBranch        *b_Enu;   //!
  TBranch        *b_Pmu;   //!
  TBranch        *b_CosThetamu;   //!
  TBranch        *b_ReactionCode;   //!
  TBranch        *b_NeutrinoCode;   //!
  TBranch        *b_FluxWeight;   //!
  TBranch        *b_DetWeight;   //!
  TBranch        *b_DetNomWeight;   //!
  TBranch        *b_POTWeight;   //!
  TBranch        *b_PileUpWeight;   //!
  //Kendall CCCoh weight
  TBranch        *b_CohTuningWeight;   //!
  TBranch        *b_TruePmu;   //!
  TBranch        *b_TrueLepPDG;   //!
  TBranch        *b_TrueCosThetamu;   //!
  TBranch        *b_Q2;   //!
  TBranch        *b_Q2QE;   //!
  TBranch        *b_HaveTruth;   //!
  TBranch        *b_TgtMat;   //!
  TBranch        *b_NuParent;   //!
  TBranch        *b_Run;   //!
  TBranch        *b_EventType;   //!
  TBranch        *b_TruthVtx;   //!
  TBranch        *b_MAQEGraph;   //!
  TBranch        *b_MARESGraph;   //!
  TBranch        *b_DISMPISHPGraph;   //!
  TBranch        *b_SFGraph;   //!
  TBranch        *b_EBGraph;   //!
  TBranch        *b_PFGraph;   //!
  TBranch        *b_PDDGraph;   //!
  TBranch        *b_FSI_PI_ABSGraph;
  TBranch        *b_FSI_CEX_LOGraph;
  TBranch        *b_FSI_INEL_LOGraph;
  TBranch        *b_FSI_PI_PRODGraph;
  TBranch        *b_FSI_CEX_HIGraph;
  TBranch        *b_FSI_INEL_HIGraph;
  //TODO: TBranch        *b_PDD_MECGraph;
  TBranch        *b_MECGraph;

  TBranch* b_MEC_CGraph;
  TBranch* b_PF_CGraph;
  TBranch* b_EB_CGraph;

  TBranch* b_MEC_OGraph;
  TBranch* b_PF_OGraph;
  TBranch* b_EB_OGraph;

  TBranch* b_CA5Graph;
  TBranch* b_BgSclResGraph;
  TBranch* b_SCCVGraph;
  TBranch* b_SCCAGraph;
  TBranch* b_SF_RFGGraph;
  TBranch* b_RPAGraph;
  TBranch* b_CCNuEGraph;

  TBranch* b_PDD_CGraph;
  TBranch* b_PDD_OGraph;


  Int_t          numbSysts;

  WeightFile(std::map<std::string, std::vector<std::pair<std::string,std::string > > > mcFiles);
  virtual ~WeightFile();
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init();
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);
  virtual void     MakeSplines(int entry);
  TClonesArray*   GetResponseGraph(XsecSysts systPar);
  TSpline3** GetResponseSpline(XsecSysts systPar);
  Int_t GetNumberSysts(){return numbSysts;};
  double* GetXSecWeightArray(XsecSysts systPar);
};

#endif

#ifdef WeightFile_cxx

//Constructor takes the pre-parsed input from BANFFInterfaceBase::ParseInput
//and adds those files to the tree.
WeightFile::WeightFile(std::map<std::string, std::vector<std::pair<std::string,std::string > > > mcFiles){

  fChain = new TChain("sample_sum");


  //Define an iterator over the map
  std::map<std::string, std::vector<std::pair<std::string,std::string > > >::iterator it;

  //For each MC file in the map, add it to the summary tree.
  for(it = mcFiles.begin(); it != mcFiles.end(); it++){

    std::vector<std::pair<std::string,std::string> > fileList = it->second;

    for(unsigned int i = 0 ; i < fileList.size(); i++){

      fChain->AddFile((fileList[i].second).c_str());
    }
  }

  Init();
}

WeightFile::~WeightFile()
{

  for(int i = 0; i < SampleId::kNSamples; i++){


    //Delete the individual splines.
    // Uses delete rather than ->Delete(), since this is for the interpreter
    // as advised here: https://root.cern.ch/doc/master/classTObject.html#ab53971e1e6c731a22fd5c16455a82665
    if(MAQESpline!=NULL){ 
      if(MAQESpline[i] != NULL){
        delete MAQESpline[i];
        MAQESpline[i]=NULL;
      }
    }

    if(MARESSpline!=NULL){
      if(MARESSpline[i] != NULL){
        delete MARESSpline[i];
        MARESSpline[i]=NULL;
      }
    }

    if(DISMPISHPSpline!=NULL){
      if(DISMPISHPSpline[i] != NULL){
        delete DISMPISHPSpline[i];
        DISMPISHPSpline[i]=NULL;
      }
    }

    if(SFSpline!=NULL){
      if(SFSpline[i] != NULL){
        delete SFSpline[i];
        SFSpline[i]=NULL;
      }
    }
    if(EBSpline!=NULL){
      if(EBSpline[i] != NULL){
        delete EBSpline[i];
        EBSpline[i]=NULL;
      }
    }

    if(PFSpline!=NULL){
      if(PFSpline[i] != NULL){
        delete PFSpline[i];
        PFSpline[i]=NULL;
      }
    }

    if(PDDSpline!=NULL){
      if(PDDSpline[i] != NULL){
        delete PDDSpline[i];
        PDDSpline[i]=NULL;
      }
    }

    if(FSI_PI_ABSSpline!=NULL){
      if(FSI_PI_ABSSpline[i] != NULL){
        delete FSI_PI_ABSSpline[i];
        FSI_PI_ABSSpline[i]=NULL;
      }
    }

    if(FSI_CEX_LOSpline!=NULL){
      if(FSI_CEX_LOSpline[i] != NULL){
        delete FSI_CEX_LOSpline[i];
        FSI_CEX_LOSpline[i]=NULL;
      }
    }

    if(FSI_INEL_LOSpline!=NULL){
      if(FSI_INEL_LOSpline[i] != NULL){
        delete FSI_INEL_LOSpline[i];
        FSI_INEL_LOSpline[i]=NULL;
      }
    }

    if(FSI_PI_PRODSpline!=NULL){
      if(FSI_PI_PRODSpline[i] != NULL){
        delete FSI_PI_PRODSpline[i];
        FSI_PI_PRODSpline[i]=NULL;
      }
    }

    if(FSI_CEX_HISpline!=NULL){
      if(FSI_CEX_HISpline[i] != NULL){
        delete FSI_CEX_HISpline[i];
        FSI_CEX_HISpline[i]=NULL;
      }
    }

    if(FSI_INEL_HISpline!=NULL){
      if(FSI_INEL_HISpline[i] != NULL){
        delete FSI_INEL_HISpline[i];
        FSI_INEL_HISpline[i]=NULL;
      }
    }

    if(PDD_MECSpline!=NULL){
      if(PDD_MECSpline[i] != NULL){
        delete PDD_MECSpline[i];
        PDD_MECSpline[i]=NULL;
      }
    }

    if(MECSpline!=NULL){
      if(MECSpline[i] != NULL){
        delete MECSpline[i];
        MECSpline[i]=NULL;
      }
    }

    if(MEC_CSpline != NULL){ 
      if(MEC_CSpline[i] != NULL){
        delete MEC_CSpline[i];
        MEC_CSpline[i]=NULL;
      }
    }

    if(PF_CSpline != NULL){
      if(PF_CSpline[i] != NULL){
        delete PF_CSpline[i];
        PF_CSpline[i]=NULL;
      }
    }

    if(EB_CSpline != NULL){
      if(EB_CSpline[i] != NULL){
        delete EB_CSpline[i];
        EB_CSpline[i]=NULL;
      }
    }

    if(MEC_OSpline != NULL){
      if(MEC_OSpline[i] != NULL){
        delete MEC_OSpline[i];
        MEC_OSpline[i]=NULL;
      }
    }

    if(PF_OSpline != NULL){
      if(PF_OSpline[i] != NULL){
        delete PF_OSpline[i];
        PF_OSpline[i]=NULL;
      }
    }

    if(EB_OSpline != NULL){ 
      if(EB_OSpline[i] != NULL){
        delete EB_OSpline[i];
        EB_OSpline[i]=NULL;
      }
    }

    if(CA5Spline != NULL){ 
      if(CA5Spline[i] != NULL){
        delete CA5Spline[i];
        CA5Spline[i]=NULL;
      }
    }

    if(BgSclResSpline != NULL){
      if(BgSclResSpline[i] != NULL){
        delete BgSclResSpline[i];
        BgSclResSpline[i]=NULL;
      }
    }

    if(SCCVSpline != NULL){ 
      if(SCCVSpline[i] != NULL){
        delete SCCVSpline[i];
        SCCVSpline[i]=NULL;
      }
    }

    if(SCCASpline != NULL){
      if(SCCASpline[i] != NULL){
        delete SCCASpline[i];
        SCCASpline[i]=NULL;
      }
    }

    if(SF_RFGSpline != NULL){
      if(SF_RFGSpline[i] != NULL){
        delete SF_RFGSpline[i];
        SF_RFGSpline[i]=NULL;
      }
    }

    if(RPASpline != NULL){
      if(RPASpline[i] != NULL){
        delete RPASpline[i];
        RPASpline[i]=NULL;
      }
    }

    if(CCNuESpline != NULL){
      if(CCNuESpline[i] != NULL){
        delete CCNuESpline[i];
        CCNuESpline[i]=NULL;
      }
    }
		
    if(PDD_CSpline != NULL){
      if(PDD_CSpline[i] != NULL){
        delete PDD_CSpline[i];
        PDD_CSpline[i]=NULL;
      }
    }
		
    if(PDD_OSpline != NULL){
      if(PDD_OSpline[i] != NULL){
        delete PDD_OSpline[i];
        PDD_OSpline[i]=NULL;
      }
    }
  }

  //Delete the pointers to the arrays.
  //The individual elements have already been deleted, so use delete
  //instead of delete[].
  // ... but you still need to use this since we did new[] (otherwise valgrind complains) PL
  if(MAQESpline        != NULL){ delete[] MAQESpline;        MAQESpline        = NULL; }
  if(MARESSpline       != NULL){ delete[] MARESSpline;       MARESSpline       = NULL; }
  if(DISMPISHPSpline   != NULL){ delete[] DISMPISHPSpline;   DISMPISHPSpline   = NULL; }
  if(SFSpline          != NULL){ delete[] SFSpline;          SFSpline          = NULL; }
  if(EBSpline          != NULL){ delete[] EBSpline;          EBSpline          = NULL; }
  if(PFSpline          != NULL){ delete[] PFSpline;          PFSpline          = NULL; }
  if(PDDSpline         != NULL){ delete[] PDDSpline;         PDDSpline         = NULL; }
  if(FSI_PI_ABSSpline  != NULL){ delete[] FSI_PI_ABSSpline;  FSI_PI_ABSSpline  = NULL; }
  if(FSI_CEX_LOSpline  != NULL){ delete[] FSI_CEX_LOSpline;  FSI_CEX_LOSpline  = NULL; }
  if(FSI_INEL_LOSpline != NULL){ delete[] FSI_INEL_LOSpline; FSI_INEL_LOSpline = NULL; }
  if(FSI_PI_PRODSpline != NULL){ delete[] FSI_PI_PRODSpline; FSI_PI_PRODSpline = NULL; }
  if(FSI_CEX_HISpline  != NULL){ delete[] FSI_CEX_HISpline;  FSI_CEX_HISpline  = NULL; }
  if(FSI_INEL_HISpline != NULL){ delete[] FSI_INEL_HISpline; FSI_INEL_HISpline = NULL; }
  if(PDD_MECSpline     != NULL){ delete[] PDD_MECSpline;     PDD_MECSpline     = NULL; }
  if(MECSpline         != NULL){ delete[] MECSpline;         MECSpline         = NULL; }
  if(MEC_CSpline       != NULL){ delete[] MEC_CSpline;       MEC_CSpline       = NULL; }
  if(PF_CSpline        != NULL){ delete[] PF_CSpline;        PF_CSpline        = NULL; }
  if(EB_CSpline        != NULL){ delete[] EB_CSpline;        EB_CSpline        = NULL; }
  if(MEC_OSpline       != NULL){ delete[] MEC_OSpline;       MEC_OSpline       = NULL; }
  if(PF_OSpline        != NULL){ delete[] PF_OSpline;        PF_OSpline        = NULL; }
  if(EB_OSpline        != NULL){ delete[] EB_OSpline;        EB_OSpline        = NULL; }
  if(CA5Spline         != NULL){ delete[] CA5Spline;         CA5Spline         = NULL; }
  if(BgSclResSpline    != NULL){ delete[] BgSclResSpline;    BgSclResSpline    = NULL; }
  if(SCCVSpline        != NULL){ delete[] SCCVSpline;        SCCVSpline        = NULL; }
  if(SCCASpline        != NULL){ delete[] SCCASpline;        SCCASpline        = NULL; }
  if(SF_RFGSpline      != NULL){ delete[] SF_RFGSpline;      SF_RFGSpline      = NULL; }
  if(RPASpline         != NULL){ delete[] RPASpline;         RPASpline         = NULL; }
  if(CCNuESpline       != NULL){ delete[] CCNuESpline;       CCNuESpline       = NULL; }
  if(PDD_CSpline       != NULL){ delete[] PDD_CSpline;       PDD_CSpline       = NULL; }
  if(PDD_OSpline       != NULL){ delete[] PDD_OSpline;       PDD_OSpline       = NULL; }

  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t WeightFile::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}
Long64_t WeightFile::LoadTree(Long64_t entry)
{
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (!fChain->InheritsFrom(TChain::Class()))  return centry;
  TChain *chain = (TChain*)fChain;
  if (chain->GetTreeNumber() != fCurrent) {
    fCurrent = chain->GetTreeNumber();
    Notify();
  }
  return centry;
}

void WeightFile::Init()
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  //First let's check the file type.
  //Default to 2013
  fileType = 0;

  //If GetBranch("CA5Graph") does not return NULL, it's a 2014 file.
  if(fChain->GetBranch("CA5Graph")){

    fileType = 1;

  }
  if(fChain->GetBranch("CohTuningWeight")){

    fileType = 2;

  }
	
  if(fChain->GetBranch("PDD_CGraph")){

    fileType = 3;

  }

  // Set object pointer
  MAQEGraph = new TClonesArray("TGraph", SampleId::kNSamples);
  MARESGraph = new TClonesArray("TGraph", SampleId::kNSamples);
  DISMPISHPGraph = new TClonesArray("TGraph", SampleId::kNSamples);
  SFGraph = new TClonesArray("TGraph", SampleId::kNSamples);
  EBGraph = new TClonesArray("TGraph", SampleId::kNSamples);
  PFGraph = new TClonesArray("TGraph", SampleId::kNSamples);
  PDDGraph = new TClonesArray("TGraph", SampleId::kNSamples);
  FSI_PI_ABSGraph = new TClonesArray("TGraph", SampleId::kNSamples);
  FSI_CEX_LOGraph = new TClonesArray("TGraph", SampleId::kNSamples);
  FSI_INEL_LOGraph = new TClonesArray("TGraph", SampleId::kNSamples);
  FSI_PI_PRODGraph = new TClonesArray("TGraph", SampleId::kNSamples);
  FSI_CEX_HIGraph = new TClonesArray("TGraph", SampleId::kNSamples);
  FSI_INEL_HIGraph = new TClonesArray("TGraph", SampleId::kNSamples);
  //TODO: PDD_MECGraph = 0;
  MECGraph = new TClonesArray("TGraph", SampleId::kNSamples);

  //2014 additions.
  MEC_CGraph = new TClonesArray("TGraph", SampleId::kNSamples);
  PF_CGraph = new TClonesArray("TGraph", SampleId::kNSamples);
  EB_CGraph = new TClonesArray("TGraph", SampleId::kNSamples);

  MEC_OGraph = new TClonesArray("TGraph", SampleId::kNSamples);
  PF_OGraph = new TClonesArray("TGraph", SampleId::kNSamples);
  EB_OGraph = new TClonesArray("TGraph", SampleId::kNSamples);

  CA5Graph = new TClonesArray("TGraph", SampleId::kNSamples);
  BgSclResGraph = new TClonesArray("TGraph", SampleId::kNSamples);
  SCCVGraph = new TClonesArray("TGraph", SampleId::kNSamples);
  SCCAGraph = new TClonesArray("TGraph", SampleId::kNSamples);
  SF_RFGGraph = new TClonesArray("TGraph",SampleId::kNSamples);
  RPAGraph = new TClonesArray("TGraph", SampleId::kNSamples);
  CCNuEGraph = new TClonesArray("TGraph",SampleId::kNSamples);

  PDD_CGraph = new TClonesArray("TGraph",SampleId::kNSamples);
  PDD_OGraph = new TClonesArray("TGraph",SampleId::kNSamples);


  //Initialize pointers.
  Enu = new Double_t[SampleId::kNSamples];
  Pmu = new Double_t[SampleId::kNSamples];
  CosThetamu = new Double_t[SampleId::kNSamples];
  ReactionCode = new Int_t[SampleId::kNSamples];
  NeutrinoCode = new Int_t[SampleId::kNSamples];
  FluxWeight = new Double_t[SampleId::kNSamples];
  DetWeight = new Double_t[SampleId::kNSamples];
  POTWeight = new Double_t[SampleId::kNSamples];
  PileUpWeight = new Double_t[SampleId::kNSamples];
  // Kendall CCCoh tuning
  CohTuningWeight = new Double_t[SampleId::kNSamples];
  TruePmu = new Double_t[SampleId::kNSamples];
  TrueLepPDG = new Int_t[SampleId::kNSamples];
  TrueCosThetamu = new Double_t[SampleId::kNSamples];
  Q2 = new Double_t[SampleId::kNSamples];
  Q2QE = new Double_t[SampleId::kNSamples];
  HaveTruth = new Char_t[SampleId::kNSamples];
  TgtMat = new Int_t[SampleId::kNSamples];
  NuParent = new Int_t[SampleId::kNSamples];
  EventType = new Int_t[SampleId::kNSamples];
  TruthVtx = new Int_t[SampleId::kNSamples];



  // Set branch addresses and branch pointers
  fCurrent = -1;
  fChain->SetMakeClass(1);
  numbSysts = 0;

  fChain->SetBranchAddress("NSamples",&NSamples);
  fChain->SetBranchAddress("Enu", Enu, &b_Enu);
  fChain->SetBranchAddress("Pmu", Pmu, &b_Pmu);
  fChain->SetBranchAddress("CosThetamu", CosThetamu, &b_CosThetamu);
  fChain->SetBranchAddress("ReactionCode", ReactionCode, &b_ReactionCode);
  fChain->SetBranchAddress("NeutrinoCode", NeutrinoCode, &b_NeutrinoCode);
  fChain->SetBranchAddress("FluxWeight", FluxWeight, &b_FluxWeight);
  fChain->SetBranchAddress("DetWeight", DetWeight, &b_DetWeight);
  fChain->SetBranchAddress("DetNomWeight", &DetNomWeight, &b_DetNomWeight);
  fChain->SetBranchAddress("POTWeight", POTWeight, &b_POTWeight);
  fChain->SetBranchAddress("PileUpWeight", PileUpWeight, &b_PileUpWeight);
  fChain->SetBranchAddress("TruePmu", TruePmu, &b_TruePmu);
  fChain->SetBranchAddress("TrueLepPDG", TrueLepPDG, &b_TrueLepPDG);
  fChain->SetBranchAddress("TrueCosThetamu", TrueCosThetamu, &b_TrueCosThetamu);
  fChain->SetBranchAddress("Q2", Q2, &b_Q2);
  fChain->SetBranchAddress("Q2QE", Q2QE, &b_Q2QE);
  fChain->SetBranchAddress("HaveTruth", HaveTruth, &b_HaveTruth);
  fChain->SetBranchAddress("TgtMat", TgtMat, &b_TgtMat);
  fChain->SetBranchAddress("NuParent", NuParent, &b_NuParent);
  fChain->SetBranchAddress("Run", &Run, &b_Run);
  fChain->SetBranchAddress("EventType", EventType, &b_EventType);
  fChain->SetBranchAddress("TruthVtx", TruthVtx, &b_TruthVtx);

  //Common branches.
  fChain->SetBranchAddress("MAQEGraph", &MAQEGraph, &b_MAQEGraph);
  numbSysts++;
  fChain->SetBranchAddress("MARESGraph", &MARESGraph, &b_MARESGraph);
  numbSysts++;
  fChain->SetBranchAddress("FSI_PI_ABSGraph", &FSI_PI_ABSGraph, &b_FSI_PI_ABSGraph);
  numbSysts++;
  fChain->SetBranchAddress("FSI_CEX_LOGraph", &FSI_CEX_LOGraph, &b_FSI_CEX_LOGraph);
  numbSysts++;
  fChain->SetBranchAddress("FSI_INEL_LOGraph", &FSI_INEL_LOGraph, &b_FSI_INEL_LOGraph);
  numbSysts++;
  fChain->SetBranchAddress("FSI_PI_PRODGraph", &FSI_PI_PRODGraph, &b_FSI_PI_PRODGraph);
  numbSysts++;
  fChain->SetBranchAddress("FSI_CEX_HIGraph", &FSI_CEX_HIGraph, &b_FSI_CEX_HIGraph);
  numbSysts++;
  fChain->SetBranchAddress("FSI_INEL_HIGraph", &FSI_INEL_HIGraph, &b_FSI_INEL_HIGraph);
  numbSysts++;

  //2013 only branches.
  if(fileType < 1){
    fChain->SetBranchAddress("DISMPISHPGraph", &DISMPISHPGraph, &b_DISMPISHPGraph);
    numbSysts++;
    fChain->SetBranchAddress("SFGraph", &SFGraph, &b_SFGraph);
    numbSysts++;
    fChain->SetBranchAddress("EBGraph", &EBGraph, &b_EBGraph);
    numbSysts++;
    fChain->SetBranchAddress("PFGraph", &PFGraph, &b_PFGraph);
    numbSysts++;
    fChain->SetBranchAddress("PDDGraph", &PDDGraph, &b_PDDGraph);
    numbSysts++;
    //TODO: fChain->SetBranchAddress("PDD_MECGraph", &PDD_MECGraph, &b_PDD_MECGraph);
    //TODO: numbSysts++;
    fChain->SetBranchAddress("MECGraph", &MECGraph, &b_MECGraph);
    numbSysts++;
  }

  //The 2014 parameter treatment.
  else if(fileType < 4 ){
    fChain->SetBranchAddress("MEC_CGraph", &MEC_CGraph, &b_MEC_CGraph);
    numbSysts++;

    fChain->SetBranchAddress("PF_CGraph", &PF_CGraph, &b_PF_CGraph);
    numbSysts++;

    fChain->SetBranchAddress("EB_CGraph", &EB_CGraph, &b_EB_CGraph);
    numbSysts++;

    fChain->SetBranchAddress("MEC_OGraph", &MEC_OGraph, &b_MEC_OGraph);
    numbSysts++;

    fChain->SetBranchAddress("PF_OGraph", &PF_OGraph, &b_PF_OGraph);
    numbSysts++;

    fChain->SetBranchAddress("EB_OGraph", &EB_OGraph, &b_EB_OGraph);
    numbSysts++;

    fChain->SetBranchAddress("CA5Graph", &CA5Graph, &b_CA5Graph);
    numbSysts++;

    fChain->SetBranchAddress("BgSclResGraph", &BgSclResGraph, &b_BgSclResGraph);
    numbSysts++;

    fChain->SetBranchAddress("SCCVGraph", &SCCVGraph, &b_SCCVGraph);
    numbSysts++;

    fChain->SetBranchAddress("SCCAGraph", &SCCAGraph, &b_SCCAGraph);
    numbSysts++;

    fChain->SetBranchAddress("SF_RFGGraph", &SF_RFGGraph, &b_SF_RFGGraph);
    numbSysts++;

    fChain->SetBranchAddress("RPAGraph", &RPAGraph, &b_RPAGraph);
    numbSysts++;

    //DISMPISHP has a different spelling this year.
    fChain->SetBranchAddress("DISMPiShpGraph",&DISMPISHPGraph,&b_DISMPISHPGraph);
    numbSysts++;

    fChain->SetBranchAddress("CCNuEGraph", &CCNuEGraph, &b_CCNuEGraph);

  }
  if(fileType == 2 || fileType == 3){
    //Kendall CCCoh tuning
    fChain->SetBranchAddress("CohTuningWeight", CohTuningWeight, &b_CohTuningWeight);
    numbSysts++;
  }
	
  if(fileType == 3){
    //PDD Shape
    fChain->SetBranchAddress("PDD_CGraph", &PDD_CGraph, &b_PDD_CGraph);
    numbSysts++;
		
    fChain->SetBranchAddress("PDD_OGraph", &PDD_OGraph, &b_PDD_OGraph);
    numbSysts++;
  }

  std::cout << "number of systs " << numbSysts << std::endl;

  MAQESpline = NULL;
  MARESSpline = NULL;
  DISMPISHPSpline = NULL;
  SFSpline = NULL;
  EBSpline = NULL;
  PFSpline = NULL;
  PDDSpline = NULL;
  FSI_PI_ABSSpline=NULL;
  FSI_CEX_LOSpline=NULL;
  FSI_INEL_LOSpline=NULL;
  FSI_PI_PRODSpline=NULL;
  FSI_CEX_HISpline=NULL;
  FSI_INEL_HISpline=NULL;
  PDD_MECSpline = NULL;
  MECSpline = NULL;

  //2014 additions.
  MEC_CSpline = NULL;
  PF_CSpline = NULL;
  EB_CSpline = NULL;
  MEC_OSpline = NULL;
  PF_OSpline = NULL;
  EB_OSpline = NULL;
  CA5Spline = NULL;
  BgSclResSpline = NULL;
  SCCVSpline = NULL;
  SCCASpline = NULL;
  SF_RFGSpline = NULL;
  RPASpline = NULL;
  CCNuESpline = NULL;

  //2016 additions.
  PDD_CSpline = NULL;
  PDD_OSpline = NULL;

  Notify();
}

Bool_t WeightFile::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}

void WeightFile::Show(Long64_t entry)
{
  // Print contents of entry.
  // If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}
Int_t WeightFile::Cut(Long64_t entry)
{
  (void) entry;
  // This function may be called from Loop.
  // returns  1 if entry is accepted.
  // returns -1 otherwise.
  return 1;
}
#endif // #ifdef WeightFile_cxx
