#define P0DAIRNUMUBKGINANTINUMODECCMULTITRACKSELECTION_CXX

#include "p0dAirNumuBkgInAntiNuModeCCMultiTrackSelection.hxx"
#include "p0dWaterNumuCCMultiTrackSelection.hxx"
#include "p0dNumuCCSelection.hxx"
#include "baseSelection.hxx"
#include "numuBkgInAntiNuModeCCMultiTrackSelection.hxx"
#include "CutUtils.hxx"
#include "trackerSelectionUtils.hxx"
#include "EventBoxUtils.hxx"


//********************************************************************
p0dAirNumuBkgInAntiNuModeCCMultiTrackSelection::p0dAirNumuBkgInAntiNuModeCCMultiTrackSelection(bool forceBreak):
    SelectionBase(forceBreak, EventBoxId::kEventBoxTracker) {
//********************************************************************

}

//********************************************************************
void p0dAirNumuBkgInAntiNuModeCCMultiTrackSelection::DefineSteps(){
//********************************************************************

    // Cuts must be added in the right order
    // last "true" means the step sequence is broken if cut is not passed (default is "false")
    AddStep(StepBase::kCut,    "event quality",       new EventQualityCut(),         true);
    AddStep(StepBase::kCut,    "> 0 tracks ",         new TotalMultiplicityCut(),    true);
    AddStep(StepBase::kAction, "find leading tracks", new FindLeadingTracksAction());
    AddStep(StepBase::kAction, "find vertex",         new FindVertexAction());
    AddStep(StepBase::kAction, "fill_summary",        new FillSummaryAction_p0dAirNumuBkgInAntiNuModeCCMultiTrack());
    AddStep(StepBase::kCut,    "quality+fiducial",    new TrackQualityFiducialCut(), true);
    AddStep(StepBase::kCut,    "neg_mult",            new NegativeMultiplicityCut());
    AddStep(StepBase::kAction, "find veto track",     new FindP0DVetoAction());
    AddStep(StepBase::kCut,    "veto",                new P0DSelectionVetoCut(),     true);

    //Add a split to the trunk with 2 branches. One for single p0d
    //selection and the other for multiple. This is necessary to get the
    //weight systs as ApplyWeightSystematic is only called if a branch
    //passes.
    AddSplit(2);

    // QE selection
    AddStep(0, StepBase::kCut, "1 track cut", new SingleP0DTrackCut());
    // Other selection. Need this additional "cut" or else the selection crashes
    AddStep(1, StepBase::kCut, "N track cut", new MultipleP0DTracksCut());

    SetBranchAlias(0,"CC1Track", 0);
    SetBranchAlias(1,"CCNTrack", 1);

    SetPreSelectionAccumLevel(4);

}

//**************************************************
void p0dAirNumuBkgInAntiNuModeCCMultiTrackSelection::InitializeEvent(AnaEventC& eventC){
//**************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventC);

  // Create the appropriate EventBox if it does not exist yet
  if (!event.EventBoxes[EventBoxId::kEventBoxTracker])
    event.EventBoxes[EventBoxId::kEventBoxTracker] = new EventBoxTracker();

  boxUtils::FillTracksWithTPC(event, static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTracksWithP0D(event);
  boxUtils::FillTrajsChargedInTPC(event);
  boxUtils::FillTrajsChargedInP0D(event);

}

//********************************************************************
void p0dAirNumuBkgInAntiNuModeCCMultiTrackSelection::DefineDetectorFV(){
//********************************************************************

  SetDetectorFV(SubDetId::kP0D);

}

//********************************************************************
bool p0dAirNumuBkgInAntiNuModeCCMultiTrackSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

    //CC1Track
    if(allCutsPassed[0]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kP0DAirNuMuBkgInAntiNuModeCC1Track;
    }
    //CCNTracks
    else if (allCutsPassed[1]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kP0DAirNuMuBkgInAntiNuModeCCNTracks;
    }
    // otherwise kUnassigned is used from the EventSummary constructor
    return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);

}

//**************************************************
Int_t p0dAirNumuBkgInAntiNuModeCCMultiTrackSelection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  (void)branch;

  std::vector<std::vector<Int_t> > groups(SystId::SystEnumLast_SystId, std::vector<Int_t>(1, -1));
  groups.resize(SystId::SystEnumLast_SystId);


  // Explicitely set the groups for various systeamtics that need them,
  // this allows to have a transparent set/control of the systematics by the
  // selection

  using namespace SystId;
  using namespace anaUtils;
  // --- Systematic         Detectors                                   P0D
  groups[   kBFieldDist     ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPCInP0DFV );
  groups[   kMomScale       ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPCInP0DFV );
  groups[   kMomResol       ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPCInP0DFV );
  groups[   kTpcPid         ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPCInP0DFV );
  groups[   kChargeIDEff    ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPCInP0DFV );
  groups[   kTpcClusterEff  ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPCInP0DFV );
  groups[   kP0DELossScale  ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPCInP0DFV );
  groups[   kP0DELossResol  ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPCInP0DFV );
  groups[   kTpcTrackEff    ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPC        );
  groups[   kTpcP0dMatchEff ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPC        );
  groups[   kP0DVeto        ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPC        );

  // The systematics not mentioned above will get no groups
  Int_t ngroups = 0;
  // Check the systematic index is ok
  try {
    groups.at(systId);
  }
  catch (const std::out_of_range& oor) {
    std::cerr << this->Name() << " GetRelevantRecObjectGroupsForSystematic: syst index beyond limits "<< systId << " Error " << oor.what() << '\n';
    return ngroups;
  }

  if (groups[systId][0] >= 0) IDs[ngroups++] = groups[systId][0];

  return ngroups;
}

//**************************************************
Int_t p0dAirNumuBkgInAntiNuModeCCMultiTrackSelection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
//**************************************************

  (void)branch;

  std::vector<std::vector<Int_t> > groups(SystId::SystEnumLast_SystId, std::vector<Int_t>(1, -1));

  // Explicitely set the groups for various systeamtics that need them,
  // this allows to have a transparent set/control of the systematics by the
  // selection

  using namespace SystId;
  using namespace anaUtils;
  // --- Systematic         Detectors                                   P0D
  groups[ kTpcTrackEff    ] =  CreateVectorI(1, EventBoxTracker::kTrueParticlesChargedInTPCInBunch);
  groups[ kTpcP0dMatchEff ] =  CreateVectorI(1, EventBoxTracker::kTrueParticlesChargedInTPCInBunch);
  groups[ kP0DVeto        ] =  CreateVectorI(1, EventBoxTracker::kTrueParticlesChargedInTPCInBunch);

  // The systematics not mentioned above will get no groups
  Int_t ngroups = 0;
  // Check the systematic index is ok
  try {
    groups.at(systId);
  }
  catch (const std::out_of_range& oor) {
    std::cerr << this->Name() << " GetRelevantTrueObjectGroupsForSystematic: syst index beyond limits "<< systId << " Error " << oor.what() << '\n';
    exit(1);
  }
  if (groups[systId][0] >= 0) IDs[ngroups++] = groups[systId][0];

  return ngroups;
}

//********************************************************************
bool FillSummaryAction_p0dAirNumuBkgInAntiNuModeCCMultiTrack::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//********************************************************************

    // Cast the ToyBox to the appropriate type
    ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB);

    if(!box.MainTrack) return true;

    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kP0DAirNuMuBkgInAntiNuModeCC1Track ] = box.HMNtrack;
    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kP0DAirNuMuBkgInAntiNuModeCCNTracks] = box.HMNtrack;

    for(int i = 0; i < 4; ++i){
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kP0DAirNuMuBkgInAntiNuModeCC1Track ][i] = box.HMNtrack->PositionStart[i];
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kP0DAirNuMuBkgInAntiNuModeCCNTracks][i] = box.HMNtrack->PositionStart[i];
    }
    if(box.HMNtrack->GetTrueParticle()){
        static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kP0DAirNuMuBkgInAntiNuModeCC1Track ] = box.HMNtrack->GetTrueParticle()->TrueVertex;
        static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kP0DAirNuMuBkgInAntiNuModeCCNTracks] = box.HMNtrack->GetTrueParticle()->TrueVertex;
    }
    return true;
}
