//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Oct 12 00:16:51 2017 by ROOT version 5.34/09
// from TTree sample_sum/Tracker Numu Summary
// found on file: /physics/data/BANFF/asg2018oa/Splines_testing_psyche_v3r26/MCrun5w_Splines.root
//////////////////////////////////////////////////////////

#ifndef MakeClSampleSummary_h
#define MakeClSampleSummary_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <TClonesArray.h>

// Fixed size dimensions of array or collections stored in the TTree if any.

class MakeClSampleSummary : public TObject {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           NSamples;
   Int_t           SelectedSample;
   Double_t        Enu[89];   //[NSamples]
   Double_t        Pmu[89];   //[NSamples]
   Double_t        CosThetamu[89];   //[NSamples]
   Int_t           ReactionCode[89];   //[NSamples]
   Int_t           NeutrinoCode[89];   //[NSamples]
   Double_t        FluxWeight[89];   //[NSamples]
   Double_t        TruePmu[89];   //[NSamples]
   Double_t        TrueCosThetamu[89];   //[NSamples]
   Double_t        Q2[89];   //[NSamples]
   Double_t        Q2QE[89];   //[NSamples]
   Char_t          HaveTruth[89];   //[NSamples]
   Int_t           TgtMat[89];   //[NSamples]
   Int_t           NuParent[89];   //[NSamples]
   Int_t           Run;
   Int_t           TruthVtx[89];   //[NSamples]
   Double_t        CCQETuningWeight[89];   //[NSamples]
   Double_t        CohTuningWeight[89];   //[NSamples]
   Double_t        RFGTuningWeight[89];   //[NSamples]
   Double_t        RelRPATuningWeight[89];   //[NSamples]
   Double_t        NonRelRPATuningWeight[89];   //[NSamples]
   TClonesArray    *MAQEGraph;
   TClonesArray    *Q3CUTGraph;
   TClonesArray    *MEC_CGraph;
   TClonesArray    *PF_CGraph;
   TClonesArray    *MEC_OGraph;
   TClonesArray    *PF_OGraph;
   TClonesArray    *CA5Graph;
   TClonesArray    *MARESGraph;
   TClonesArray    *BgSclResGraph;
   TClonesArray    *CCNuEGraph;
   TClonesArray    *DISMPiShpGraph;
   TClonesArray    *FSI_PI_ABSGraph;
   TClonesArray    *FSI_CEX_LOGraph;
   TClonesArray    *FSI_INEL_LOGraph;
   TClonesArray    *FSI_PI_PRODGraph;
   TClonesArray    *FSI_CEX_HIGraph;
   TClonesArray    *FSI_INEL_HIGraph;
   TClonesArray    *PDD_CGraph;
   TClonesArray    *PDD_OGraph;
   TClonesArray    *SF_RFGGraph;
   TClonesArray    *RPAGraph;

   // List of branches
   TBranch        *b_NSamples;   //!
   TBranch        *b_SelectedSample;   //!
   TBranch        *b_Enu;   //!
   TBranch        *b_Pmu;   //!
   TBranch        *b_CosThetamu;   //!
   TBranch        *b_ReactionCode;   //!
   TBranch        *b_NeutrinoCode;   //!
   TBranch        *b_FluxWeight;   //!
   TBranch        *b_TruePmu;   //!
   TBranch        *b_TrueCosThetamu;   //!
   TBranch        *b_Q2;   //!
   TBranch        *b_Q2QE;   //!
   TBranch        *b_HaveTruth;   //!
   TBranch        *b_TgtMat;   //!
   TBranch        *b_NuParent;   //!
   TBranch        *b_Run;   //!
   TBranch        *b_TruthVtx;   //!
   TBranch        *b_CCQETuningWeight;   //!
   TBranch        *b_CohTuningWeight;   //!
   TBranch        *b_RFGTuningWeight;   //!
   TBranch        *b_RelRPATuningWeight;   //!
   TBranch        *b_NonRelRPATuningWeight;   //!
   TBranch        *b_MAQEGraph;   //!
   TBranch        *b_Q3CUTGraph;   //!
   TBranch        *b_MEC_CGraph;   //!
   TBranch        *b_PF_CGraph;   //!
   TBranch        *b_MEC_OGraph;   //!
   TBranch        *b_PF_OGraph;   //!
   TBranch        *b_CA5Graph;   //!
   TBranch        *b_MARESGraph;   //!
   TBranch        *b_BgSclResGraph;   //!
   TBranch        *b_CCNuEGraph;   //!
   TBranch        *b_DISMPiShpGraph;   //!
   TBranch        *b_FSI_PI_ABSGraph;   //!
   TBranch        *b_FSI_CEX_LOGraph;   //!
   TBranch        *b_FSI_INEL_LOGraph;   //!
   TBranch        *b_FSI_PI_PRODGraph;   //!
   TBranch        *b_FSI_CEX_HIGraph;   //!
   TBranch        *b_FSI_INEL_HIGraph;   //!
   TBranch        *b_PDD_CGraph;   //!
   TBranch        *b_PDD_OGraph;   //!
   TBranch        *b_SF_RFGGraph;   //!
   TBranch        *b_RPAGraph;   //!

   MakeClSampleSummary(TTree *tree=0);
   virtual ~MakeClSampleSummary();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   ClassDef(MakeClSampleSummary,1)
};

#endif

#ifdef MakeClSampleSummary_cxx
MakeClSampleSummary::MakeClSampleSummary(TTree *tree) : fChain(0)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/physics/data/BANFF/asg2018oa/Splines_testing_psyche_v3r26/MCrun5w_Splines.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/physics/data/BANFF/asg2018oa/Splines_testing_psyche_v3r26/MCrun5w_Splines.root");
      }
      f->GetObject("sample_sum",tree);

   }
   Init(tree);
}

MakeClSampleSummary::~MakeClSampleSummary()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t MakeClSampleSummary::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t MakeClSampleSummary::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void MakeClSampleSummary::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   MAQEGraph = 0;
   Q3CUTGraph = 0;
   MEC_CGraph = 0;
   PF_CGraph = 0;
   MEC_OGraph = 0;
   PF_OGraph = 0;
   CA5Graph = 0;
   MARESGraph = 0;
   BgSclResGraph = 0;
   CCNuEGraph = 0;
   DISMPiShpGraph = 0;
   FSI_PI_ABSGraph = 0;
   FSI_CEX_LOGraph = 0;
   FSI_INEL_LOGraph = 0;
   FSI_PI_PRODGraph = 0;
   FSI_CEX_HIGraph = 0;
   FSI_INEL_HIGraph = 0;
   PDD_CGraph = 0;
   PDD_OGraph = 0;
   SF_RFGGraph = 0;
   RPAGraph = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("NSamples", &NSamples, &b_NSamples);
   fChain->SetBranchAddress("SelectedSample", &SelectedSample, &b_SelectedSample);
   fChain->SetBranchAddress("Enu", Enu, &b_Enu);
   fChain->SetBranchAddress("Pmu", Pmu, &b_Pmu);
   fChain->SetBranchAddress("CosThetamu", CosThetamu, &b_CosThetamu);
   fChain->SetBranchAddress("ReactionCode", ReactionCode, &b_ReactionCode);
   fChain->SetBranchAddress("NeutrinoCode", NeutrinoCode, &b_NeutrinoCode);
   fChain->SetBranchAddress("FluxWeight", FluxWeight, &b_FluxWeight);
   fChain->SetBranchAddress("TruePmu", TruePmu, &b_TruePmu);
   fChain->SetBranchAddress("TrueCosThetamu", TrueCosThetamu, &b_TrueCosThetamu);
   fChain->SetBranchAddress("Q2", Q2, &b_Q2);
   fChain->SetBranchAddress("Q2QE", Q2QE, &b_Q2QE);
   fChain->SetBranchAddress("HaveTruth", HaveTruth, &b_HaveTruth);
   fChain->SetBranchAddress("TgtMat", TgtMat, &b_TgtMat);
   fChain->SetBranchAddress("NuParent", NuParent, &b_NuParent);
   fChain->SetBranchAddress("Run", &Run, &b_Run);
   fChain->SetBranchAddress("TruthVtx", TruthVtx, &b_TruthVtx);
   fChain->SetBranchAddress("CCQETuningWeight", CCQETuningWeight, &b_CCQETuningWeight);
   fChain->SetBranchAddress("CohTuningWeight", CohTuningWeight, &b_CohTuningWeight);
   fChain->SetBranchAddress("RFGTuningWeight", RFGTuningWeight, &b_RFGTuningWeight);
   fChain->SetBranchAddress("RelRPATuningWeight", RelRPATuningWeight, &b_RelRPATuningWeight);
   fChain->SetBranchAddress("NonRelRPATuningWeight", NonRelRPATuningWeight, &b_NonRelRPATuningWeight);
   fChain->SetBranchAddress("MAQEGraph", &MAQEGraph, &b_MAQEGraph);
   fChain->SetBranchAddress("Q3CUTGraph", &Q3CUTGraph, &b_Q3CUTGraph);
   fChain->SetBranchAddress("MEC_CGraph", &MEC_CGraph, &b_MEC_CGraph);
   fChain->SetBranchAddress("PF_CGraph", &PF_CGraph, &b_PF_CGraph);
   fChain->SetBranchAddress("MEC_OGraph", &MEC_OGraph, &b_MEC_OGraph);
   fChain->SetBranchAddress("PF_OGraph", &PF_OGraph, &b_PF_OGraph);
   fChain->SetBranchAddress("CA5Graph", &CA5Graph, &b_CA5Graph);
   fChain->SetBranchAddress("MARESGraph", &MARESGraph, &b_MARESGraph);
   fChain->SetBranchAddress("BgSclResGraph", &BgSclResGraph, &b_BgSclResGraph);
   fChain->SetBranchAddress("CCNuEGraph", &CCNuEGraph, &b_CCNuEGraph);
   fChain->SetBranchAddress("DISMPiShpGraph", &DISMPiShpGraph, &b_DISMPiShpGraph);
   fChain->SetBranchAddress("FSI_PI_ABSGraph", &FSI_PI_ABSGraph, &b_FSI_PI_ABSGraph);
   fChain->SetBranchAddress("FSI_CEX_LOGraph", &FSI_CEX_LOGraph, &b_FSI_CEX_LOGraph);
   fChain->SetBranchAddress("FSI_INEL_LOGraph", &FSI_INEL_LOGraph, &b_FSI_INEL_LOGraph);
   fChain->SetBranchAddress("FSI_PI_PRODGraph", &FSI_PI_PRODGraph, &b_FSI_PI_PRODGraph);
   fChain->SetBranchAddress("FSI_CEX_HIGraph", &FSI_CEX_HIGraph, &b_FSI_CEX_HIGraph);
   fChain->SetBranchAddress("FSI_INEL_HIGraph", &FSI_INEL_HIGraph, &b_FSI_INEL_HIGraph);
   fChain->SetBranchAddress("PDD_CGraph", &PDD_CGraph, &b_PDD_CGraph);
   fChain->SetBranchAddress("PDD_OGraph", &PDD_OGraph, &b_PDD_OGraph);
   fChain->SetBranchAddress("SF_RFGGraph", &SF_RFGGraph, &b_SF_RFGGraph);
   fChain->SetBranchAddress("RPAGraph", &RPAGraph, &b_RPAGraph);
   Notify();
}

Bool_t MakeClSampleSummary::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void MakeClSampleSummary::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t MakeClSampleSummary::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef MakeClSampleSummary_cxx
