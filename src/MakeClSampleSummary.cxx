#define MAKECLSAMPLESUMMARY_CXX
#include "MakeClSampleSummary.hxx"
ClassImp(MakeClSampleSummary)

MakeClSampleSummary::MakeClSampleSummary(TString filename) : fChain(0)
{
    TFile* f = TFile::Open(filename.Data());
    TTree* tree = static_cast<TTree*>(f->Get("sample_sum"));
    Init(tree);
}

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

    fChain->SetBranchAddress("SelectedSample", &SelectedSample, &b_SelectedSample);
    fChain->SetBranchAddress("Pmu", Pmu, &b_Pmu);
    fChain->SetBranchAddress("CosThetamu", CosThetamu, &b_CosThetamu);

    //fChain->SetBranchAddress("NSamples", &NSamples, &b_NSamples);
    //fChain->SetBranchAddress("Run", &Run, &b_Run);
    //fChain->SetBranchAddress("DetNomWeight", &DetNomWeight, &b_DetNomWeight);
    //fChain->SetBranchAddress("NeutrinoCode", NeutrinoCode, &b_NeutrinoCode);
    //fChain->SetBranchAddress("Enu", Enu, &b_Enu);
    //fChain->SetBranchAddress("TrueNeutrinoDirX", &TrueNeutrinoDirX, &b_TrueNeutrinoDirX);
    //fChain->SetBranchAddress("TrueNeutrinoDirY", &TrueNeutrinoDirY, &b_TrueNeutrinoDirY);
    //fChain->SetBranchAddress("TrueNeutrinoDirZ", &TrueNeutrinoDirZ, &b_TrueNeutrinoDirZ);
    //fChain->SetBranchAddress("TrueLepPDG", &TrueLepPDG, &b_TrueLepPDG);
    //fChain->SetBranchAddress("TruePmu", TruePmu, &b_TruePmu);
    //fChain->SetBranchAddress("TrueLepDirX", &TrueLepDirX, &b_TrueLepDirX);
    //fChain->SetBranchAddress("TrueLepDirY", &TrueLepDirY, &b_TrueLepDirY);
    //fChain->SetBranchAddress("TrueLepDirZ", &TrueLepDirZ, &b_TrueLepDirZ);
    //fChain->SetBranchAddress("TrueCosThetamu", TrueCosThetamu, &b_TrueCosThetamu);
    //fChain->SetBranchAddress("RecoLepDirX", &RecoLepDirX, &b_RecoLepDirX);
    //fChain->SetBranchAddress("RecoLepDirY", &RecoLepDirY, &b_RecoLepDirY);
    //fChain->SetBranchAddress("RecoLepDirZ", RecoLepDirZ, &b_RecoLepDirZ);
    //fChain->SetBranchAddress("TruePostFSIPiPDG", &TruePostFSIPiPDG, &b_TruePostFSIPiPDG);
    //fChain->SetBranchAddress("TruePostFSIPiMom", &TruePostFSIPiMom, &b_TruePostFSIPiMom);
    //fChain->SetBranchAddress("TruePostFSIPiDirX", &TruePostFSIPiDirX, &b_TruePostFSIPiDirX);
    //fChain->SetBranchAddress("TruePostFSIPiDirY", &TruePostFSIPiDirY, &b_TruePostFSIPiDirY);
    //fChain->SetBranchAddress("TruePostFSIPiDirZ", &TruePostFSIPiDirZ, &b_TruePostFSIPiDirZ);
    //fChain->SetBranchAddress("TruePreFSIPiPDG", &TruePreFSIPiPDG, &b_TruePreFSIPiPDG);
    //fChain->SetBranchAddress("TruePreFSIPiMom", &TruePreFSIPiMom, &b_TruePreFSIPiMom);
    //fChain->SetBranchAddress("TruePreFSIPiDirX", &TruePreFSIPiDirX, &b_TruePreFSIPiDirX);
    //fChain->SetBranchAddress("TruePreFSIPiDirY", &TruePreFSIPiDirY, &b_TruePreFSIPiDirY);
    //fChain->SetBranchAddress("TruePreFSIPiDirZ", &TruePreFSIPiDirZ, &b_TruePreFSIPiDirZ);
    //fChain->SetBranchAddress("RecoPiMom", &RecoPiMom, &b_RecoPiMom);
    //fChain->SetBranchAddress("RecoPiDirX", &RecoPiDirX, &b_RecoPiDirX);
    //fChain->SetBranchAddress("RecoPiDirY", &RecoPiDirY, &b_RecoPiDirY);
    //fChain->SetBranchAddress("RecoPiDirZ", &RecoPiDirZ, &b_RecoPiDirZ);
    //fChain->SetBranchAddress("ReactionCode", ReactionCode, &b_ReactionCode);
    //fChain->SetBranchAddress("NuParent", NuParent, &b_NuParent);
    //fChain->SetBranchAddress("Q2", Q2, &b_Q2);
    //fChain->SetBranchAddress("Q2QE", Q2QE, &b_Q2QE);
    //fChain->SetBranchAddress("HaveTruth", HaveTruth, &b_HaveTruth);
    //fChain->SetBranchAddress("TgtMat", TgtMat, &b_TgtMat);
    //fChain->SetBranchAddress("FluxWeight", FluxWeight, &b_FluxWeight);
    //fChain->SetBranchAddress("TruthVtx", TruthVtx, &b_TruthVtx);
    //fChain->SetBranchAddress("CCQETuningWeight", CCQETuningWeight, &b_CCQETuningWeight);
    //fChain->SetBranchAddress("CohTuningWeight", CohTuningWeight, &b_CohTuningWeight);
    //fChain->SetBranchAddress("RFGTuningWeight", RFGTuningWeight, &b_RFGTuningWeight);
    //fChain->SetBranchAddress("RelRPATuningWeight", RelRPATuningWeight, &b_RelRPATuningWeight);
    //fChain->SetBranchAddress("NonRelRPATuningWeight", NonRelRPATuningWeight, &b_NonRelRPATuningWeight);
    //fChain->SetBranchAddress("pFTuningWeight", pFTuningWeight, &b_pFTuningWeight);
    //fChain->SetBranchAddress("MECCTuningWeight", MECCTuningWeight, &b_MECCTuningWeight);
    //fChain->SetBranchAddress("MECOTuningWeight", MECOTuningWeight, &b_MECOTuningWeight);
    //fChain->SetBranchAddress("MaQETuningWeight", MaQETuningWeight, &b_MaQETuningWeight);
    //fChain->SetBranchAddress("MAQEGraph", &MAQEGraph, &b_MAQEGraph);
    //fChain->SetBranchAddress("Q3CUTGraph", &Q3CUTGraph, &b_Q3CUTGraph);
    //fChain->SetBranchAddress("MEC_CGraph", &MEC_CGraph, &b_MEC_CGraph);
    //fChain->SetBranchAddress("PF_CGraph", &PF_CGraph, &b_PF_CGraph);
    //fChain->SetBranchAddress("MEC_OGraph", &MEC_OGraph, &b_MEC_OGraph);
    //fChain->SetBranchAddress("PF_OGraph", &PF_OGraph, &b_PF_OGraph);
    //fChain->SetBranchAddress("CA5Graph", &CA5Graph, &b_CA5Graph);
    //fChain->SetBranchAddress("MARESGraph", &MARESGraph, &b_MARESGraph);
    //fChain->SetBranchAddress("BgSclResGraph", &BgSclResGraph, &b_BgSclResGraph);
    //fChain->SetBranchAddress("CCNuEGraph", &CCNuEGraph, &b_CCNuEGraph);
    //fChain->SetBranchAddress("DISMPiShpGraph", &DISMPiShpGraph, &b_DISMPiShpGraph);
    //fChain->SetBranchAddress("FSI_PI_ABSGraph", &FSI_PI_ABSGraph, &b_FSI_PI_ABSGraph);
    //fChain->SetBranchAddress("FSI_CEX_LOGraph", &FSI_CEX_LOGraph, &b_FSI_CEX_LOGraph);
    fChain->SetBranchAddress("FSI_INEL_LOGraph", &FSI_INEL_LOGraph, &b_FSI_INEL_LOGraph);
    //fChain->SetBranchAddress("FSI_PI_PRODGraph", &FSI_PI_PRODGraph, &b_FSI_PI_PRODGraph);
    //fChain->SetBranchAddress("FSI_CEX_HIGraph", &FSI_CEX_HIGraph, &b_FSI_CEX_HIGraph);
    //fChain->SetBranchAddress("FSI_INEL_HIGraph", &FSI_INEL_HIGraph, &b_FSI_INEL_HIGraph);
    //fChain->SetBranchAddress("PDD_CGraph", &PDD_CGraph, &b_PDD_CGraph);
    //fChain->SetBranchAddress("PDD_OGraph", &PDD_OGraph, &b_PDD_OGraph);
    //fChain->SetBranchAddress("SF_RFGGraph", &SF_RFGGraph, &b_SF_RFGGraph);
    //fChain->SetBranchAddress("RPAGraph", &RPAGraph, &b_RPAGraph);
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
void MakeClSampleSummary::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L MakeClSampleSummary.C
//      Root > MakeClSampleSummary t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   }
}
