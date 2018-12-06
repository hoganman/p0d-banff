//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Oct 26 15:25:08 2018 by ROOT version 5.34/09
// from TTree all/all
// found on file: ../test.root
//////////////////////////////////////////////////////////

#ifndef MakeClRunSyst_New_h
#define MakeClRunSyst_New_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class MakeClRunSyst_New : public TObject {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           Run;
   Int_t           SubRun;
   Int_t           EventNumber;
   UInt_t          RooVertexIndex;
   Int_t           SelectionNom;
   Double_t        TrueEnuNom;
   Int_t           TrueNuPDGNom;
   Int_t           TrueVertexIDNom;
   Double_t        LeptonMomNom;
   Double_t        LeptonTPC1MomNom;
   Double_t        LeptonCosNom;
   Double_t        WeightNom;
   Double_t        FluxWeightNom;
   Int_t           tLeptonPDG;
   Int_t           tLeptonParentPDG;
   Int_t           tLeptonGParentPDG;
   Double_t        tLeptonMomentum;
   Double_t        tLeptonCosTheta;
   Double_t        tLeptonPhi;
   Double_t        vtxX;
   Double_t        vtxY;
   Double_t        vtxZ;
   Double_t        LeptonPositionX;
   Double_t        LeptonPositionY;
   Double_t        LeptonPositionZ;
   Double_t        tLeptonPositionX;
   Double_t        tLeptonPositionY;
   Double_t        tLeptonPositionZ;
   Double_t        tVtxX;
   Double_t        tVtxY;
   Double_t        tVtxZ;
   Int_t           tReactionCode;
   Double_t        tW2;
   Double_t        tQ2;
   Double_t        tXbj;
   Double_t        tNu;
   Double_t        tYbj;
   Int_t           NumberOfTracks;
   Int_t           NPrimaryParticles[36];
   Int_t           inFGD1;
   Int_t           inFGD2;
   Int_t           tOnWaterTarget;
   Int_t           nToys;
   Int_t           Toy[1000];
   Int_t           TrueVertexIDToy[1000];
   Int_t           SelectionToy[1000];
   Double_t        TrueEnuToy[1000];
   Int_t           TrueNuPDGToy[1000];
   Double_t        LeptonMomToy[1000];
   Double_t        LeptonCosToy[1000];
   Double_t        FluxWeightToy[1000];
   Double_t        WeightToy[1000];

   // List of branches
   TBranch        *b_Run;   //!
   TBranch        *b_SubRun;   //!
   TBranch        *b_EventNumber;   //!
   TBranch        *b_RooVertexIndex;   //!
   TBranch        *b_SelectionNom;   //!
   TBranch        *b_TrueEnuNom;   //!
   TBranch        *b_TrueNuPDGNom;   //!
   TBranch        *b_TrueVertexIDNom;   //!
   TBranch        *b_LeptonMomNom;   //!
   TBranch        *b_LeptonTPC1MomNom;   //!
   TBranch        *b_LeptonCosNom;   //!
   TBranch        *b_WeightNom;   //!
   TBranch        *b_FluxWeightNom;   //!
   TBranch        *b_tLeptonPDG;   //!
   TBranch        *b_tLeptonParentPDG;   //!
   TBranch        *b_tLeptonGParentPDG;   //!
   TBranch        *b_tLeptonMomentum;   //!
   TBranch        *b_tLeptonCosTheta;   //!
   TBranch        *b_tLeptonPhi;   //!
   TBranch        *b_vtxX;   //!
   TBranch        *b_vtxY;   //!
   TBranch        *b_vtxZ;   //!
   TBranch        *b_LeptonPositionX;   //!
   TBranch        *b_LeptonPositionY;   //!
   TBranch        *b_LeptonPositionZ;   //!
   TBranch        *b_tLeptonPositionX;   //!
   TBranch        *b_tLeptonPositionY;   //!
   TBranch        *b_tLeptonPositionZ;   //!
   TBranch        *b_tVtxX;   //!
   TBranch        *b_tVtxY;   //!
   TBranch        *b_tVtxZ;   //!
   TBranch        *b_tReactionCode;   //!
   TBranch        *b_tW2;   //!
   TBranch        *b_tQ2;   //!
   TBranch        *b_tXbj;   //!
   TBranch        *b_tNu;   //!
   TBranch        *b_tYbj;   //!
   TBranch        *b_NumberOfTracks;   //!
   TBranch        *b_NPrimaryParticles;   //!
   TBranch        *b_inFGD1;   //!
   TBranch        *b_inFGD2;   //!
   TBranch        *b_tOnWaterTarget;   //!
   TBranch        *b_nToys;   //!
   TBranch        *b_Toy;   //!
   TBranch        *b_TrueVertexIDToy;   //!
   TBranch        *b_SelectionToy;   //!
   TBranch        *b_TrueEnuToy;   //!
   TBranch        *b_TrueNuPDGToy;   //!
   TBranch        *b_LeptonMomToy;   //!
   TBranch        *b_LeptonCosToy;   //!
   TBranch        *b_FluxWeightToy;   //!
   TBranch        *b_WeightToy;   //!

   MakeClRunSyst_New(TTree *tree=0);
   MakeClRunSyst_New(const TString &name);
   virtual ~MakeClRunSyst_New();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);

   ClassDef(MakeClRunSyst_New,1)
};

#endif
