#include "antiNumuCCMultiPiAnalysis.hxx"
#include "antiNumuCCMultiPiSelection.hxx"
#include "antiNumuCCMultiPiFGD2Selection.hxx"
#include "antiNumuCCMultiTrackSelection.hxx"
#include "antiNumuCCMultiTrackFGD2Selection.hxx"

//constructor
//********************************************************************
antiNumuCCMultiPiAnalysis::antiNumuCCMultiPiAnalysis(AnalysisAlgorithm* ana) : baseTrackerAnalysis(ana) {
//********************************************************************

  // Add the package version
  ND::versioning().AddPackage("antiNumuCCMultiPiAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("ANTINUMUCCMULTIPIANALYSISROOT")));

  // Create a numuCCAnalysis passing this analysis to the constructor. In that way the same managers are used
  _antiNumuCCAnalysis = new antiNumuCCAnalysis(this);

  // Use the antiNumuCCAnalysis (in practice that means that the same box and event will be used for the antiNumuCCAnalysis as for this analysis)
  UseAnalysis(_antiNumuCCAnalysis);
}

//********************************************************************
bool antiNumuCCMultiPiAnalysis::Initialize() {
//********************************************************************

  // Initialize the baseAnalysis instead of numuCCAnalysis
  // since we do not want to call and overload numu categories
  if(!baseTrackerAnalysis::Initialize()) return false;

  // Minimum accum level to save event into the output tree
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("antiNumuCCMultiPiAnalysis.MinAccumLevelToSave"));

  // which analysis: FGD1, FGD2 or FGDs
  _whichFGD = ND::params().GetParameterI("antiNumuCCMultiPiAnalysis.Selections.whichFGD");
  if (_whichFGD == 3) {
    std::cout << "----------------------------------------------------" << std::endl;
    std::cout << "WARNING: only for events with accum_level > 6 the vars in the output microtree will surely refer to the muon candidate in that FGD" << std::endl;
    std::cout << "----------------------------------------------------" << std::endl;
  }

  // CC-multi-pi or CC-multi-track
   _runMultiTrack = ND::params().GetParameterI("antiNumuCCMultiPiAnalysis.Selections.RunMultiTrack");
  
  // Define antinu categories (different legend from numuCC)
  // for FGD2 same categories with prefix "fgd2", i,e, "fgd2reaction" etc.)
  anaUtils::AddStandardAntiNumuCategories();
  anaUtils::AddStandardAntiNumuCategories("fgd2");
  //TODO: add categories

  return true;
}

//********************************************************************
void antiNumuCCMultiPiAnalysis::DefineSelections(){
//********************************************************************
  
  // FGD1 antineutrino analysis:
  if(_whichFGD==1 || _whichFGD==3){
    // ---- Inclusive CC ----
    if(!_runMultiTrack)
      sel().AddSelection("kTrackerAntiNumuCCMultiPi",    "antiNumu FGD1 CC Multiple Pion selection",  new antiNumuCCMultiPiSelection(false));
    // ---- CC Multi Pion Samples ----
    else
      sel().AddSelection("kTrackerAntiNumuCCMultiTrack", "antiNumu FGD1 CC Multiple Track selection", new antiNumuCCMultiTrackSelection(false));
  }
  // FGD2 antineutrino analysis:
  if (_whichFGD==2 || _whichFGD==3){
    // ---- CC Inclusive ----
    if(!_runMultiTrack)
      sel().AddSelection("kTrackerAntiNumuCCMultiPiFGD2",  "antiNumu FGD2 CC Multiple Pion selection",  new antiNumuCCMultiPiFGD2Selection(false));
    // ---- CC Multi Track Samples ----
    else
      sel().AddSelection("kTrackerAntiNumuMultiTrackFGD2", "antiNumu FGD2 CC Multiple Track selection", new antiNumuCCMultiTrackFGD2Selection(false));
  }

  
}

//********************************************************************
void antiNumuCCMultiPiAnalysis::DefineMicroTrees(bool addBase){
//********************************************************************

  // -------- Add variables to the analysis tree ----------------------

  // Variables from antiNumuCCAnalysis
  _antiNumuCCAnalysis->DefineMicroTrees(addBase);
 
}

//********************************************************************
void antiNumuCCMultiPiAnalysis::DefineTruthTree(){
//********************************************************************

  // Variables from numuCCAnalysis
  _antiNumuCCAnalysis->DefineTruthTree();

}

//********************************************************************
void antiNumuCCMultiPiAnalysis::FillMicroTrees(bool addBase){
//********************************************************************
 
  _antiNumuCCAnalysis->FillMicroTrees(addBase);
}

//********************************************************************
void antiNumuCCMultiPiAnalysis::FillToyVarsInMicroTrees(bool addBase){
//********************************************************************

  _antiNumuCCAnalysis->FillToyVarsInMicroTrees(addBase);
}

//********************************************************************
bool antiNumuCCMultiPiAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  return _antiNumuCCAnalysis->CheckFillTruthTree(vtx);
}

//********************************************************************
void antiNumuCCMultiPiAnalysis::FillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  _antiNumuCCAnalysis->FillTruthTreeBase(vtx);

}

//********************************************************************
void antiNumuCCMultiPiAnalysis::FillCategories(){
//********************************************************************

  _antiNumuCCAnalysis->FillCategories();

}

