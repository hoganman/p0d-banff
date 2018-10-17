#include "baseSelection.hxx"
#include "p0dAntiNumuCCSelection.hxx"
#include "CutUtils.hxx"
#include "trackerSelectionUtils.hxx"
#include "EventBoxUtils.hxx"

bool _IsAntiNu;

//********************************************************************
p0dAntiNumuCCSelection::p0dAntiNumuCCSelection(bool forceBreak): SelectionBase(forceBreak, EventBoxId::kEventBoxTracker) {
//********************************************************************
  _IsAntiNu = ND::params().GetParameterI("p0dAntiNumuCCAnalysis.IsAntiNu");

}

//********************************************************************
void p0dAntiNumuCCSelection::DefineSteps(){
//********************************************************************

    // Cuts must be added in the right order
    // last "true" means the step sequence is broken if cut is not passed (default is "false")
    AddStep(StepBase::kCut,    "event quality",      new EventQualityCut(),           true);
    AddStep(StepBase::kCut,    "> 0 tracks ",        new TotalMultiplicityCut(),      true);  
    AddStep(StepBase::kAction, "find leading tracks",new FindP0DLeadingTracksAction());  
    AddStep(StepBase::kAction, "find vertex",        new FindVertexAction());  
    AddStep(StepBase::kCut,    "quality+fiducial",   new TrackQualityFiducialCut(),   true);  
    AddStep(StepBase::kCut,    "high momentum",      new TrackHighMomentumCut(),   _IsAntiNu);  
    AddStep(StepBase::kCut,    "P0D muon PID",       new P0DMuonPIDCut(), true);

    SetBranchAlias(0,"trunk");
}

//**************************************************
//olod//void p0dAntiNumuCCSelection::InitializeEvent(AnaEventB& event){
void p0dAntiNumuCCSelection::InitializeEvent(AnaEventC& eventC){
//**************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventC); 
  // Create the appropriate EventBox if it does not exist yet
  if (!event.EventBoxes[EventBoxId::kEventBoxTracker])
    event.EventBoxes[EventBoxId::kEventBoxTracker] = new EventBoxTracker();

  boxUtils::FillTracksWithTPC(event,             static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTracksWithP0D(event);
  boxUtils::FillTrajsChargedInTPC(event);
  boxUtils::FillTrajsChargedInP0D(event);

  //old
  // Create the appropriate EventBox if it does not exist yet
  //if (!event.EventBoxes[EventBoxId::kEventBoxTracker])
    //event.EventBoxes[EventBoxId::kEventBoxTracker] = new EventBoxP0D();

  //p0dBoxUtils::InitializeEventBox(event);
}

//********************************************************************
void p0dAntiNumuCCSelection::DefineDetectorFV(){
//********************************************************************

  SetDetectorFV(SubDetId::kP0D);
}

//**************************************************
bool FindP0DLeadingTracksAction::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  // Cast the ToyBox to the appropriate type
  ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

  trackerSelUtils::FindLeadingTracks(event, box);

  // For this selection the main track is the HMN track
  if(_IsAntiNu) box.MainTrack = box.HMPtrack;
  else box.MainTrack = box.HMNtrack;

  //box.MainTrack = box.HMNtrack;
  //box.MainTrack = box.HMPtrack;
  return true;
}


//old
//**************************************************
//bool FindP0DLeadingTracksAction::Apply(AnaEventB& event, ToyBoxB& boxB) const{
//**************************************************
/*
  // Cast the ToyBox to the appropriate type
  ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 

  // For now, fill using old method because it allows specifying the
  // detector.  In the future the new cutUtils::FindLeadingTracks
  // method should check for detector and fill using the appropriate
  // track groupID
  trackerSelUtils::FindLeadingTracksOld(event, box, true, SubDetId::kP0D);

  // For this selection the main track is the HMN track
  //box.MainTrack = box.HMPtrack;
  box.MainTrack = box.HMNtrack;
  return true;
}*/
//**************************************************
//bool TrackHighMomentumCut::Apply(AnaEventB& event, ToyBoxB& boxB) const{
bool TrackHighMomentumCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************
  (void)event;
  ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB); 
  if(box.HMPtrack){
    if(box.HMPtrack->Momentum>=box.HMtrack->Momentum) return true;
    else return false;
  }
  else return false;
}
//**************************************************
bool P0DMuonPIDCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

    (void)event;

    // cast the box to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB);
    Float_t PIDLikelihood[4];

    if (!box.MainTrack) return false;
    if (box.MainTrack->Momentum < 0.) return false;
    anaUtils::GetPIDLikelihood(*(box.MainTrack), PIDLikelihood);
    if(PIDLikelihood[0]>0.05) return true;
    else return false;
}



