//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Nov 29 17:18:53 2018 by ROOT version 5.34/34
// from TTree sample_sum/Tracker Numu Summary
// found on file: /home/mhogan/Downloads/Run4_Water_splines.root
//////////////////////////////////////////////////////////

#ifndef MAKECLSAMPLESUMMARY_HXX
#define MAKECLSAMPLESUMMARY_HXX

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TGraph.h>
#include <TString.h>

// Header file for the classes stored in the TTree if any.
#include <TClonesArray.h>

// Fixed size dimensions of array or collections stored in the TTree if any.

class MakeClSampleSummary : public TObject {
public :
    TTree          *fChain;   //!pointer to the analyzed TTree or TChain
    Int_t           fCurrent; //!current Tree number in a TChain

    // Declaration of leaf types
    Char_t          HaveTruth[97];   //[NSamples]
    Double_t        CCQETuningWeight[97];   //[NSamples]
    Double_t        CohTuningWeight[97];   //[NSamples]
    Double_t        CosThetamu[97];   //[NSamples]
    Double_t        DetNomWeight;
    Double_t        Enu[97];   //[NSamples]
    Double_t        FluxWeight[97];   //[NSamples]
    Double_t        MECCTuningWeight[97];   //[NSamples]
    Double_t        MECOTuningWeight[97];   //[NSamples]
    Double_t        MaQETuningWeight[97];   //[NSamples]
    Double_t        NonRelRPATuningWeight[97];   //[NSamples]
    Double_t        Pmu[97];   //[NSamples]
    Double_t        Q2QE[97];   //[NSamples]
    Double_t        Q2[97];   //[NSamples]
    Double_t        RFGTuningWeight[97];   //[NSamples]
    Double_t        RecoLepDirX;
    Double_t        RecoLepDirY;
    Double_t        RecoLepDirZ[97];   //[NSamples]
    Double_t        RecoPiDirX;
    Double_t        RecoPiDirY;
    Double_t        RecoPiDirZ;
    Double_t        RecoPiMom;
    Double_t        RelRPATuningWeight[97];   //[NSamples]
    Double_t        TrueCosThetamu[97];   //[NSamples]
    Double_t        TrueLepDirX;
    Double_t        TrueLepDirY;
    Double_t        TrueLepDirZ;
    Double_t        TrueNeutrinoDirX;
    Double_t        TrueNeutrinoDirY;
    Double_t        TrueNeutrinoDirZ;
    Double_t        TruePmu[97];   //[NSamples]
    Double_t        TruePostFSIPiDirX;
    Double_t        TruePostFSIPiDirY;
    Double_t        TruePostFSIPiDirZ;
    Double_t        TruePostFSIPiMom;
    Double_t        TruePreFSIPiDirX;
    Double_t        TruePreFSIPiDirY;
    Double_t        TruePreFSIPiDirZ;
    Double_t        TruePreFSIPiMom;
    Double_t        pFTuningWeight[97];   //[NSamples]
    Int_t           NSamples;
    Int_t           NeutrinoCode[97];   //[NSamples]
    Int_t           NuParent[97];   //[NSamples]
    Int_t           ReactionCode[97];   //[NSamples]
    Int_t           Run;
    Int_t           SelectedSample;
    Int_t           TgtMat[97];   //[NSamples]
    Int_t           TrueLepPDG;
    Int_t           TruePostFSIPiPDG;
    Int_t           TruePreFSIPiPDG;
    Int_t           TruthVtx[97];   //[NSamples]
    TClonesArray    *BgSclResGraph;
    TClonesArray    *CA5Graph;
    TClonesArray    *CCNuEGraph;
    TClonesArray    *DISMPiShpGraph;
    TClonesArray    *FSI_CEX_HIGraph;
    TClonesArray    *FSI_CEX_LOGraph;
    TClonesArray    *FSI_INEL_HIGraph;
    TClonesArray    *FSI_INEL_LOGraph;
    TClonesArray    *FSI_PI_ABSGraph;
    TClonesArray    *FSI_PI_PRODGraph;
    TClonesArray    *MAQEGraph;
    TClonesArray    *MARESGraph;
    TClonesArray    *MEC_CGraph;
    TClonesArray    *MEC_OGraph;
    TClonesArray    *PDD_CGraph;
    TClonesArray    *PDD_OGraph;
    TClonesArray    *PF_CGraph;
    TClonesArray    *PF_OGraph;
    TClonesArray    *Q3CUTGraph;
    TClonesArray    *RPAGraph;
    TClonesArray    *SF_RFGGraph;

    // List of branches
    TBranch        *b_NSamples;   //!
    TBranch        *b_SelectedSample;   //!
    TBranch        *b_Run;   //!
    TBranch        *b_DetNomWeight;   //!
    TBranch        *b_NeutrinoCode;   //!
    TBranch        *b_Enu;   //!
    TBranch        *b_TrueNeutrinoDirX;   //!
    TBranch        *b_TrueNeutrinoDirY;   //!
    TBranch        *b_TrueNeutrinoDirZ;   //!
    TBranch        *b_TrueLepPDG;   //!
    TBranch        *b_TruePmu;   //!
    TBranch        *b_TrueLepDirX;   //!
    TBranch        *b_TrueLepDirY;   //!
    TBranch        *b_TrueLepDirZ;   //!
    TBranch        *b_TrueCosThetamu;   //!
    TBranch        *b_Pmu;   //!
    TBranch        *b_RecoLepDirX;   //!
    TBranch        *b_RecoLepDirY;   //!
    TBranch        *b_RecoLepDirZ;   //!
    TBranch        *b_CosThetamu;   //!
    TBranch        *b_TruePostFSIPiPDG;   //!
    TBranch        *b_TruePostFSIPiMom;   //!
    TBranch        *b_TruePostFSIPiDirX;   //!
    TBranch        *b_TruePostFSIPiDirY;   //!
    TBranch        *b_TruePostFSIPiDirZ;   //!
    TBranch        *b_TruePreFSIPiPDG;   //!
    TBranch        *b_TruePreFSIPiMom;   //!
    TBranch        *b_TruePreFSIPiDirX;   //!
    TBranch        *b_TruePreFSIPiDirY;   //!
    TBranch        *b_TruePreFSIPiDirZ;   //!
    TBranch        *b_RecoPiMom;   //!
    TBranch        *b_RecoPiDirX;   //!
    TBranch        *b_RecoPiDirY;   //!
    TBranch        *b_RecoPiDirZ;   //!
    TBranch        *b_ReactionCode;   //!
    TBranch        *b_NuParent;   //!
    TBranch        *b_Q2;   //!
    TBranch        *b_Q2QE;   //!
    TBranch        *b_HaveTruth;   //!
    TBranch        *b_TgtMat;   //!
    TBranch        *b_FluxWeight;   //!
    TBranch        *b_TruthVtx;   //!
    TBranch        *b_CCQETuningWeight;   //!
    TBranch        *b_CohTuningWeight;   //!
    TBranch        *b_RFGTuningWeight;   //!
    TBranch        *b_RelRPATuningWeight;   //!
    TBranch        *b_NonRelRPATuningWeight;   //!
    TBranch        *b_pFTuningWeight;   //!
    TBranch        *b_MECCTuningWeight;   //!
    TBranch        *b_MECOTuningWeight;   //!
    TBranch        *b_MaQETuningWeight;   //!
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

    MakeClSampleSummary(TString filename);
    MakeClSampleSummary(TTree* tree = NULL);
    virtual ~MakeClSampleSummary();
    virtual Int_t     Cut(Long64_t entry);
    virtual Int_t     GetEntry(Long64_t entry);
    virtual Long64_t  LoadTree(Long64_t entry);
    virtual void      Init(TTree *tree);
    virtual void      Loop();
    virtual Bool_t    Notify();
    virtual void      Show(Long64_t entry = -1);


    TGraph *GetSelectedSampleMAQEGraph()        {return static_cast<TGraph*>(MAQEGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleQ3CUTGraph()       {return static_cast<TGraph*>(Q3CUTGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleMEC_CGraph()       {return static_cast<TGraph*>(MEC_CGraph->At(SelectedSample));}
    TGraph *GetSelectedSamplePF_CGraph()        {return static_cast<TGraph*>(PF_CGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleMEC_OGraph()       {return static_cast<TGraph*>(MEC_OGraph->At(SelectedSample));}
    TGraph *GetSelectedSamplePF_OGraph()        {return static_cast<TGraph*>(PF_OGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleCA5Graph()         {return static_cast<TGraph*>(CA5Graph->At(SelectedSample));}
    TGraph *GetSelectedSampleMARESGraph()       {return static_cast<TGraph*>(MARESGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleBgSclResGraph()    {return static_cast<TGraph*>(BgSclResGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleCCNuEGraph()       {return static_cast<TGraph*>(CCNuEGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleDISMPiShpGraph()   {return static_cast<TGraph*>(DISMPiShpGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleFSI_PI_ABSGraph()  {return static_cast<TGraph*>(FSI_PI_ABSGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleFSI_CEX_LOGraph()  {return static_cast<TGraph*>(FSI_CEX_LOGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleFSI_INEL_LOGraph() {return static_cast<TGraph*>(FSI_INEL_LOGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleFSI_PI_PRODGraph() {return static_cast<TGraph*>(FSI_PI_PRODGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleFSI_CEX_HIGraph()  {return static_cast<TGraph*>(FSI_CEX_HIGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleFSI_INEL_HIGraph() {return static_cast<TGraph*>(FSI_INEL_HIGraph->At(SelectedSample));}
    TGraph *GetSelectedSamplePDD_CGraph()       {return static_cast<TGraph*>(PDD_CGraph->At(SelectedSample));}
    TGraph *GetSelectedSamplePDD_OGraph()       {return static_cast<TGraph*>(PDD_OGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleSF_RFGGraph()      {return static_cast<TGraph*>(SF_RFGGraph->At(SelectedSample));}
    TGraph *GetSelectedSampleRPAGraph()         {return static_cast<TGraph*>(RPAGraph->At(SelectedSample));}

    Double_t EvalMAQEGraph(Double_t point)        {return GetSelectedSampleMAQEGraph()       ->Eval(point);}
    Double_t EvalQ3CUTGraph(Double_t point)       {return GetSelectedSampleQ3CUTGraph()      ->Eval(point);}
    Double_t EvalMEC_CGraph(Double_t point)       {return GetSelectedSampleMEC_CGraph()      ->Eval(point);}
    Double_t EvalPF_CGraph(Double_t point)        {return GetSelectedSamplePF_CGraph()       ->Eval(point);}
    Double_t EvalMEC_OGraph(Double_t point)       {return GetSelectedSampleMEC_OGraph()      ->Eval(point);}
    Double_t EvalPF_OGraph(Double_t point)        {return GetSelectedSamplePF_OGraph()       ->Eval(point);}
    Double_t EvalCA5Graph(Double_t point)         {return GetSelectedSampleCA5Graph()        ->Eval(point);}
    Double_t EvalMARESGraph(Double_t point)       {return GetSelectedSampleMARESGraph()      ->Eval(point);}
    Double_t EvalBgSclResGraph(Double_t point)    {return GetSelectedSampleBgSclResGraph()   ->Eval(point);}
    Double_t EvalCCNuEGraph(Double_t point)       {return GetSelectedSampleCCNuEGraph()      ->Eval(point);}
    Double_t EvalDISMPiShpGraph(Double_t point)   {return GetSelectedSampleDISMPiShpGraph()  ->Eval(point);}
    Double_t EvalFSI_PI_ABSGraph(Double_t point)  {return GetSelectedSampleFSI_PI_ABSGraph() ->Eval(point);}
    Double_t EvalFSI_CEX_LOGraph(Double_t point)  {return GetSelectedSampleFSI_CEX_LOGraph() ->Eval(point);}
    Double_t EvalFSI_INEL_LOGraph(Double_t point) {return GetSelectedSampleFSI_INEL_LOGraph()->Eval(point);}
    Double_t EvalFSI_PI_PRODGraph(Double_t point) {return GetSelectedSampleFSI_PI_PRODGraph()->Eval(point);}
    Double_t EvalFSI_CEX_HIGraph(Double_t point)  {return GetSelectedSampleFSI_CEX_HIGraph() ->Eval(point);}
    Double_t EvalFSI_INEL_HIGraph(Double_t point) {return GetSelectedSampleFSI_INEL_HIGraph()->Eval(point);}
    Double_t EvalPDD_CGraph(Double_t point)       {return GetSelectedSamplePDD_CGraph()      ->Eval(point);}
    Double_t EvalPDD_OGraph(Double_t point)       {return GetSelectedSamplePDD_OGraph()      ->Eval(point);}
    Double_t EvalSF_RFGGraph(Double_t point)      {return GetSelectedSampleSF_RFGGraph()     ->Eval(point);}
    Double_t EvalRPAGraph(Double_t point)         {return GetSelectedSampleRPAGraph()        ->Eval(point);}

    ClassDef(MakeClSampleSummary, 1)
};

#endif
