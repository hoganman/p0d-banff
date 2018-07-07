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
#include <TGraph.h>

// Header file for the classes stored in the TTree if any.
#include <TClonesArray.h>

// Fixed size dimensions of array or collections stored in the TTree if any.

class MakeClSampleSummary : public TObject {
public :
    TTree             *fChain;    //!pointer to the analyzed TTree or TChain
    Int_t              fCurrent; //!current Tree number in a TChain

    // Declaration of leaf types
    Int_t              NSamples;
    Int_t              SelectedSample;
    Double_t          Enu[89];    //[NSamples]
    Double_t          Pmu[89];    //[NSamples]
    Double_t          CosThetamu[89];    //[NSamples]
    Int_t              ReactionCode[89];    //[NSamples]
    Int_t              NeutrinoCode[89];    //[NSamples]
    Double_t          FluxWeight[89];    //[NSamples]
    Double_t          TruePmu[89];    //[NSamples]
    Double_t          TrueCosThetamu[89];    //[NSamples]
    Double_t          Q2[89];    //[NSamples]
    Double_t          Q2QE[89];    //[NSamples]
    Char_t             HaveTruth[89];    //[NSamples]
    Int_t              TgtMat[89];    //[NSamples]
    Int_t              NuParent[89];    //[NSamples]
    Int_t              Run;
    Int_t              TruthVtx[89];    //[NSamples]
    Double_t          CCQETuningWeight[89];    //[NSamples]
    Double_t          CohTuningWeight[89];    //[NSamples]
    Double_t          RFGTuningWeight[89];    //[NSamples]
    Double_t          RelRPATuningWeight[89];    //[NSamples]
    Double_t          NonRelRPATuningWeight[89];    //[NSamples]
    TClonesArray     *MAQEGraph;
    TClonesArray     *Q3CUTGraph;
    TClonesArray     *MEC_CGraph;
    TClonesArray     *PF_CGraph;
    TClonesArray     *MEC_OGraph;
    TClonesArray     *PF_OGraph;
    TClonesArray     *CA5Graph;
    TClonesArray     *MARESGraph;
    TClonesArray     *BgSclResGraph;
    TClonesArray     *CCNuEGraph;
    TClonesArray     *DISMPiShpGraph;
    TClonesArray     *FSI_PI_ABSGraph;
    TClonesArray     *FSI_CEX_LOGraph;
    TClonesArray     *FSI_INEL_LOGraph;
    TClonesArray     *FSI_PI_PRODGraph;
    TClonesArray     *FSI_CEX_HIGraph;
    TClonesArray     *FSI_INEL_HIGraph;
    TClonesArray     *PDD_CGraph;
    TClonesArray     *PDD_OGraph;
    TClonesArray     *SF_RFGGraph;
    TClonesArray     *RPAGraph;

    // List of branches
    TBranch          *b_NSamples;    //!
    TBranch          *b_SelectedSample;    //!
    TBranch          *b_Enu;    //!
    TBranch          *b_Pmu;    //!
    TBranch          *b_CosThetamu;    //!
    TBranch          *b_ReactionCode;    //!
    TBranch          *b_NeutrinoCode;    //!
    TBranch          *b_FluxWeight;    //!
    TBranch          *b_TruePmu;    //!
    TBranch          *b_TrueCosThetamu;    //!
    TBranch          *b_Q2;    //!
    TBranch          *b_Q2QE;    //!
    TBranch          *b_HaveTruth;    //!
    TBranch          *b_TgtMat;    //!
    TBranch          *b_NuParent;    //!
    TBranch          *b_Run;    //!
    TBranch          *b_TruthVtx;    //!
    TBranch          *b_CCQETuningWeight;    //!
    TBranch          *b_CohTuningWeight;    //!
    TBranch          *b_RFGTuningWeight;    //!
    TBranch          *b_RelRPATuningWeight;    //!
    TBranch          *b_NonRelRPATuningWeight;    //!
    TBranch          *b_MAQEGraph;    //!
    TBranch          *b_Q3CUTGraph;    //!
    TBranch          *b_MEC_CGraph;    //!
    TBranch          *b_PF_CGraph;    //!
    TBranch          *b_MEC_OGraph;    //!
    TBranch          *b_PF_OGraph;    //!
    TBranch          *b_CA5Graph;    //!
    TBranch          *b_MARESGraph;    //!
    TBranch          *b_BgSclResGraph;    //!
    TBranch          *b_CCNuEGraph;    //!
    TBranch          *b_DISMPiShpGraph;    //!
    TBranch          *b_FSI_PI_ABSGraph;    //!
    TBranch          *b_FSI_CEX_LOGraph;    //!
    TBranch          *b_FSI_INEL_LOGraph;    //!
    TBranch          *b_FSI_PI_PRODGraph;    //!
    TBranch          *b_FSI_CEX_HIGraph;    //!
    TBranch          *b_FSI_INEL_HIGraph;    //!
    TBranch          *b_PDD_CGraph;    //!
    TBranch          *b_PDD_OGraph;    //!
    TBranch          *b_SF_RFGGraph;    //!
    TBranch          *b_RPAGraph;    //!

    MakeClSampleSummary(TTree *tree=0);
    MakeClSampleSummary(TString fileName);
    virtual ~MakeClSampleSummary();
    virtual Int_t     Cut(Long64_t entry);
    virtual Int_t     GetEntry(Long64_t entry);
    virtual Long64_t LoadTree(Long64_t entry);
    virtual void      Init(TTree *tree);
    virtual void      Loop();
    virtual Bool_t    Notify();
    virtual void      Show(Long64_t entry = -1);

    TGraph *GetSelectedSampleMAQEGraph() {return static_cast<TGraph*>( MAQEGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleQ3CUTGraph() {return static_cast<TGraph*>( Q3CUTGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleMEC_CGraph() {return static_cast<TGraph*>( MEC_CGraph->At(SelectedSample));}
    TGraph *GetSelectedSamplePF_CGraph() {return static_cast<TGraph*>( PF_CGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleMEC_OGraph() {return static_cast<TGraph*>( MEC_OGraph->At(SelectedSample));}
    TGraph *GetSelectedSamplePF_OGraph() {return static_cast<TGraph*>( PF_OGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleCA5Graph() {return static_cast<TGraph*>( CA5Graph->At(SelectedSample));}
    TGraph *GetSelectedSampleMARESGraph() {return static_cast<TGraph*>( MARESGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleBgSclResGraph() {return static_cast<TGraph*>( BgSclResGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleCCNuEGraph() {return static_cast<TGraph*>( CCNuEGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleDISMPiShpGraph() {return static_cast<TGraph*>( DISMPiShpGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleFSI_PI_ABSGraph() {return static_cast<TGraph*>( FSI_PI_ABSGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleFSI_CEX_LOGraph() {return static_cast<TGraph*>( FSI_CEX_LOGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleFSI_INEL_LOGraph() {return static_cast<TGraph*>( FSI_INEL_LOGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleFSI_PI_PRODGraph() {return static_cast<TGraph*>( FSI_PI_PRODGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleFSI_CEX_HIGraph() {return static_cast<TGraph*>( FSI_CEX_HIGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleFSI_INEL_HIGraph() {return static_cast<TGraph*>( FSI_INEL_HIGraph->At(SelectedSample));}
    TGraph *GetSelectedSamplePDD_CGraph() {return static_cast<TGraph*>( PDD_CGraph->At(SelectedSample));}
    TGraph *GetSelectedSamplePDD_OGraph() {return static_cast<TGraph*>( PDD_OGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleSF_RFGGraph() {return static_cast<TGraph*>( SF_RFGGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleRPAGraph() {return static_cast<TGraph*>( RPAGraph->At(SelectedSample));}

    ClassDef(MakeClSampleSummary,1)
};

#endif
