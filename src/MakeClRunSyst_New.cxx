#define MakeClRunSyst_New_cxx
#include "MakeClRunSyst_New.hxx"
#include "P0DBANFFInterface.hxx"
ClassImp(MakeClRunSyst_New)


MakeClRunSyst_New::MakeClRunSyst_New(const TString &name) : fChain(0)
{
    TFile* _file0 = TFile::Open(name.Data());
    TTree* tree = NULL;
    if(_file0->Get("all"))
        tree = static_cast<TTree*>(_file0->Get("all"));
    else if(_file0->Get("nominal"))
        tree = static_cast<TTree*>(_file0->Get("nominal"));
    else
        P0DBANFFInterface::Error(this, TString::Format("Unable to find \"all\" nor \"nominal\" tree in input file %s", name.Data()));
    Init(tree);

}

MakeClRunSyst_New::MakeClRunSyst_New(TTree *tree) : fChain(0)
{
   Init(tree);
}

MakeClRunSyst_New::~MakeClRunSyst_New()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t MakeClRunSyst_New::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t MakeClRunSyst_New::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
   }
   return centry;
}

void MakeClRunSyst_New::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Run", &Run, &b_Run);
   fChain->SetBranchAddress("SubRun", &SubRun, &b_SubRun);
   fChain->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber);
   fChain->SetBranchAddress("RooVertexIndex", &RooVertexIndex, &b_RooVertexIndex);
   fChain->SetBranchAddress("SelectionNom", &SelectionNom, &b_SelectionNom);
   fChain->SetBranchAddress("TrueEnuNom", &TrueEnuNom, &b_TrueEnuNom);
   fChain->SetBranchAddress("TrueNuPDGNom", &TrueNuPDGNom, &b_TrueNuPDGNom);
   fChain->SetBranchAddress("TrueVertexIDNom", &TrueVertexIDNom, &b_TrueVertexIDNom);
   fChain->SetBranchAddress("LeptonMomNom", &LeptonMomNom, &b_LeptonMomNom);
   fChain->SetBranchAddress("LeptonTPC1MomNom", &LeptonTPC1MomNom, &b_LeptonTPC1MomNom);
   fChain->SetBranchAddress("LeptonCosNom", &LeptonCosNom, &b_LeptonCosNom);
   fChain->SetBranchAddress("WeightNom", &WeightNom, &b_WeightNom);
   fChain->SetBranchAddress("FluxWeightNom", &FluxWeightNom, &b_FluxWeightNom);
   fChain->SetBranchAddress("tLeptonPDG", &tLeptonPDG, &b_tLeptonPDG);
   fChain->SetBranchAddress("tLeptonParentPDG", &tLeptonParentPDG, &b_tLeptonParentPDG);
   fChain->SetBranchAddress("tLeptonGParentPDG", &tLeptonGParentPDG, &b_tLeptonGParentPDG);
   fChain->SetBranchAddress("tLeptonMomentum", &tLeptonMomentum, &b_tLeptonMomentum);
   fChain->SetBranchAddress("tLeptonCosTheta", &tLeptonCosTheta, &b_tLeptonCosTheta);
   fChain->SetBranchAddress("tLeptonPhi", &tLeptonPhi, &b_tLeptonPhi);
   fChain->SetBranchAddress("vtxX", &vtxX, &b_vtxX);
   fChain->SetBranchAddress("vtxY", &vtxY, &b_vtxY);
   fChain->SetBranchAddress("vtxZ", &vtxZ, &b_vtxZ);
   fChain->SetBranchAddress("LeptonPositionX", &LeptonPositionX, &b_LeptonPositionX);
   fChain->SetBranchAddress("LeptonPositionY", &LeptonPositionY, &b_LeptonPositionY);
   fChain->SetBranchAddress("LeptonPositionZ", &LeptonPositionZ, &b_LeptonPositionZ);
   fChain->SetBranchAddress("tLeptonPositionX", &tLeptonPositionX, &b_tLeptonPositionX);
   fChain->SetBranchAddress("tLeptonPositionY", &tLeptonPositionY, &b_tLeptonPositionY);
   fChain->SetBranchAddress("tLeptonPositionZ", &tLeptonPositionZ, &b_tLeptonPositionZ);
   fChain->SetBranchAddress("tVtxX", &tVtxX, &b_tVtxX);
   fChain->SetBranchAddress("tVtxY", &tVtxY, &b_tVtxY);
   fChain->SetBranchAddress("tVtxZ", &tVtxZ, &b_tVtxZ);
   fChain->SetBranchAddress("tReactionCode", &tReactionCode, &b_tReactionCode);
   fChain->SetBranchAddress("tW2", &tW2, &b_tW2);
   fChain->SetBranchAddress("tQ2", &tQ2, &b_tQ2);
   fChain->SetBranchAddress("tXbj", &tXbj, &b_tXbj);
   fChain->SetBranchAddress("tNu", &tNu, &b_tNu);
   fChain->SetBranchAddress("tYbj", &tYbj, &b_tYbj);
   fChain->SetBranchAddress("NumberOfTracks", &NumberOfTracks, &b_NumberOfTracks);
   fChain->SetBranchAddress("NPrimaryParticles", NPrimaryParticles, &b_NPrimaryParticles);
   fChain->SetBranchAddress("inFGD1", &inFGD1, &b_inFGD1);
   fChain->SetBranchAddress("inFGD2", &inFGD2, &b_inFGD2);
   fChain->SetBranchAddress("tOnWaterTarget", &tOnWaterTarget, &b_tOnWaterTarget);
   fChain->SetBranchAddress("nToys", &nToys, &b_nToys);
   fChain->SetBranchAddress("Toy", Toy, &b_Toy);
   fChain->SetBranchAddress("TrueVertexIDToy", TrueVertexIDToy, &b_TrueVertexIDToy);
   fChain->SetBranchAddress("SelectionToy", SelectionToy, &b_SelectionToy);
   fChain->SetBranchAddress("TrueEnuToy", TrueEnuToy, &b_TrueEnuToy);
   fChain->SetBranchAddress("TrueNuPDGToy", TrueNuPDGToy, &b_TrueNuPDGToy);
   fChain->SetBranchAddress("LeptonMomToy", LeptonMomToy, &b_LeptonMomToy);
   fChain->SetBranchAddress("LeptonCosToy", LeptonCosToy, &b_LeptonCosToy);
   fChain->SetBranchAddress("FluxWeightToy", FluxWeightToy, &b_FluxWeightToy);
   fChain->SetBranchAddress("WeightToy", WeightToy, &b_WeightToy);
   Notify();
}
