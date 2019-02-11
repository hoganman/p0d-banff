#include "baseSelection.hxx"
#include "p0dAntiNumuCCSelection.hxx"
#include "CutUtils.hxx"
#include "trackerSelectionUtils.hxx"
#include "EventBoxUtils.hxx"
#include "p0dNumuCCSelection.hxx"

#include "p0dRatioUtils.hxx"
#include "p0dRatioClasses.hxx"

#include "P0DAnalysisUtils.hxx"

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
    AddStep(StepBase::kCut,    "high momentum",      new TrackHighMomentumCut(),   true);

    // Original ///////////////////////////////////////////////////////////////////////////
    //AddStep(StepBase::kCut,    "P0D muon PID",       new P0DMuonPIDCut(), true);
    //AddStep(StepBase::kCut,    "P0D CC0Pi",          new P0DCC0piCut(), true);
    /////////////////////////////////////////////////////////////////////////////////////

    AddStep(StepBase::kAction, "find veto track",     new FindP0DVetoAction());
    AddStep(StepBase::kCut,    "veto",                new P0DSelectionVetoCut(),     true);

    //Add a split to the trunk with 2 branches. One for single p0d
    //selection and the other for multiple. This is necessary to get the
    //weight systs as ApplyWeightSystematic is only called if a branch
    //passes.
    AddSplit(2);

    // QE selection
    AddStep(0, StepBase::kCut, "single p0d pid", new SingleP0DPIDCut());
    // Other selection. Need this additional "cut" or else the selection crashes
    AddStep(1, StepBase::kCut, "multiple p0d pid", new MultipleP0DPIDCut());

    SetBranchAlias(0,"Single-P0D", 0);
    SetBranchAlias(1,"Multiple-P0D", 1);

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


//added Feb 22
//********************************************************************
bool p0dAntiNumuCCSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
  //********************************************************************

  if(allCutsPassed[0]){
    static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCC;
  }
  return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}

//end added

//**************************************************
bool FindP0DLeadingTracksAction::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

  // Cast the ToyBox to the appropriate type
  ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB);

  trackerSelUtils::FindLeadingTracks(event, box);

  // For this selection the main track is the HMP track unless studying numu bkg
  if(_IsAntiNu)
      box.MainTrack = box.HMPtrack;
  else
      box.MainTrack = box.HMNtrack;

  return true;
}


//**************************************************
bool TrackHighMomentumCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************
  (void)event;
  ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB);
  if(_IsAntiNu && box.HMPtrack && box.HMPtrack->Momentum >= box.HMtrack->Momentum)
    return true;
  else if(box.HMNtrack && box.HMNtrack->Momentum >= box.HMtrack->Momentum)
    return true;
  else
    return false;
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

//**************************************************
bool P0DCC0piCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************

    (void)boxB;

    AnaEventB& eventPass = *static_cast<AnaEventB*>(&event);

    AnaP0DVertex* p0dVertices[NMAXVERTICES];
    //Int_t nVerts = p0dUtils::GetAllP0DVertices(GetEvent(),p0dVertices);
    Int_t nVerts = p0dUtils::GetAllP0DVertices(eventPass,p0dVertices);
    int TagMichelFill[2];
    int TagMichelReturn = p0dRatioUtils::TagMichel(p0dVertices, nVerts, TagMichelFill);

    //Get charge ratios
    int nTracksForChargeRatio = 4;

    p0dRatioPID* RatioPIDOut = new p0dRatioPID[nTracksForChargeRatio];

    //int MipPIDReturn = p0dRatioUtils::MipPID(p0dVertices, nVerts, RatioPIDOut, nTracksForChargeRatio, temp_selmu_pos);
    //cout << RatioPIDOut[1].ChargeRatio << " " << TagMichelFill[1] << " " << RatioPIDOut[0].nShowers <<  " " << nVerts << endl;

    if(RatioPIDOut[1].ChargeRatio>1.5 && TagMichelFill[1]<1 && RatioPIDOut[0].nShowers<1){
      //cout << "Passed!" << endl;
      return true;
    }
    else{
      //cout << "Failed" << endl;
      return false;
    }

}

//**************************************************
bool SingleP0DPIDCut::Apply(AnaEventC& eventC, ToyBoxB& box) const{
//**************************************************

  (void)box;

  AnaEventB& event = *static_cast<AnaEventB*>(&eventC);

  // Get all tracks using P0D
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];
  int nP0D=EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithP0D];

  return (nP0D == 1);
}

//**************************************************
bool MultipleP0DPIDCut::Apply(AnaEventC& eventC, ToyBoxB& box) const{
//**************************************************

  (void)box;

  AnaEventB& event = *static_cast<AnaEventB*>(&eventC);

  // Get all tracks using P0D
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxTracker];
  int nP0D=EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithP0D];

  return (nP0D > 1);
}



