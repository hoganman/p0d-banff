#include "numuBkgInAntiNuModeCCMultiTrackSelection.hxx"
#include "numuBkgInAntiNuModeCCSelection.hxx"
#include "antiNumuCCMultiTrackSelection.hxx"
#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "Parameters.hxx"
#include "SubDetId.hxx"
#include "EventBoxId.hxx"
#include "SystId.hxx"

//********************************************************************
numuBkgInAntiNuModeCCMultiTrackSelection::numuBkgInAntiNuModeCCMultiTrackSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxTracker) {
//********************************************************************
    _numuBkgInAntiNuModeCCSelection.Initialize();
}

//********************************************************************
void numuBkgInAntiNuModeCCMultiTrackSelection::DefineSteps(){
//********************************************************************

    // Copy all steps from the numuBkgInAntiNuModeCCSelection
    CopySteps(_numuBkgInAntiNuModeCCSelection);

    //Additional actions for the multi-pi selection.
    AddStep(StepBase::kAction, "fill summary numuBkg_CCMultiTrack",       new FillSummaryAction_numuBkg_CCMultiTrack());  

    //Add a split to the trunk with 2 branches.
    AddSplit(2);

    //First branch is for CC1Track enhanced sample 
    AddStep(0, StepBase::kCut, "CC1Track", new CCQEcut_antinu(SubDetId::kFGD1));

    //Second branch is for CCNTracks enhanced sample 
    AddStep(1, StepBase::kCut, "CCNTracks", new CCnQEcut_antinu(SubDetId::kFGD1));

    // Set the branch aliases to the three branches
    SetBranchAlias(0,"CC1Track",0);
    SetBranchAlias(1,"CCNTracks",1);

    // By default the preselection correspond to cuts 0-2
    SetPreSelectionAccumLevel(2);
}

//********************************************************************
void numuBkgInAntiNuModeCCMultiTrackSelection::DefineDetectorFV(){
//********************************************************************

    // The detector in which the selection is applied
    SetDetectorFV(SubDetId::kFGD1);
}

//********************************************************************
bool numuBkgInAntiNuModeCCMultiTrackSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

    //CC1Track
    if(allCutsPassed[0]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuBkgInAntiNuModeCC1Track;
    }  
    //CCNTracks
    else if (allCutsPassed[1]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuBkgInAntiNuModeCCNTracks;
    }

    // otherwise kUnassigned is used from the EventSummary constructor
    return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}

//*********************************************************************
bool FillSummaryAction_numuBkg_CCMultiTrack::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//*********************************************************************

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

    if(!box.HMNtrack) return 1;

    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD1NuMuBkgInAntiNuModeCC1Track] = box.HMNtrack;
    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kFGD1NuMuBkgInAntiNuModeCCNTracks] = box.HMNtrack;

    for(int i = 0; i < 4; ++i){
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD1NuMuBkgInAntiNuModeCC1Track][i] = box.HMNtrack->PositionStart[i];
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kFGD1NuMuBkgInAntiNuModeCCNTracks][i] = box.HMNtrack->PositionStart[i];
    }
    if(box.HMNtrack->GetTrueParticle()){
        static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD1NuMuBkgInAntiNuModeCC1Track] = box.HMNtrack->GetTrueParticle()->TrueVertex;
        static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kFGD1NuMuBkgInAntiNuModeCCNTracks] = box.HMNtrack->GetTrueParticle()->TrueVertex;
    }
    return 1;
}


//**************************************************
bool numuBkgInAntiNuModeCCMultiTrackSelection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* track, SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuBkgInAntiNuModeCCSelection.IsRelevantRecObjectForSystematic(event,track,systId,branch);
}

//**************************************************
bool numuBkgInAntiNuModeCCMultiTrackSelection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const{
//**************************************************

  return _numuBkgInAntiNuModeCCSelection.IsRelevantTrueObjectForSystematic(event,trueTrack,systId,branch);
}

//**************************************************
bool numuBkgInAntiNuModeCCMultiTrackSelection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
//**************************************************

  
  (void)event;
  (void)branch;
  (void)box;

 switch (systId){
    // Fall through the relevant ones
    // TPC reco
    case SystId::kChargeIDEff:
    case SystId::kTpcClusterEff:
    case SystId::kTpcTrackEff:
    // TPC matching
    case SystId::kTpcFgdMatchEff:
    // Normalization
    case SystId::kPileUp:
    case SystId::kFgdMass:
    case SystId::kOOFV:
    case SystId::kSandMu:
    // Flux
    case SystId::kFluxWeight:
    // SI
    case SystId::kSIPion:
    case SystId::kSIProton:
      return true;
      break;
    default:
     return false;
     break;
  }
  
  return false;
}

//**************************************************
Int_t numuBkgInAntiNuModeCCMultiTrackSelection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _numuBkgInAntiNuModeCCSelection.GetRelevantRecObjectGroupsForSystematic(systId, IDs, branch);
}

//**************************************************
Int_t numuBkgInAntiNuModeCCMultiTrackSelection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  return _numuBkgInAntiNuModeCCSelection.GetRelevantTrueObjectGroupsForSystematic(systId, IDs, branch);
}

//**************************************************
void numuBkgInAntiNuModeCCMultiTrackSelection::InitializeEvent(AnaEventC& event){
//**************************************************

  _numuBkgInAntiNuModeCCSelection.InitializeEvent(event);
}

//********************************************************************
bool numuBkgInAntiNuModeCCMultiTrackSelection::CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep){
//********************************************************************

  // Must redo selection if _numuBkgInAntiNuModeCCSelection decides so
  if( _numuBkgInAntiNuModeCCSelection.CheckRedoSelection(event,PreviousToyBox,redoFromStep)) return true;

  // Otherwise selection should not be redone since the number of tracks with TPC and FGD will not be changed by systematics
  return false;
}

