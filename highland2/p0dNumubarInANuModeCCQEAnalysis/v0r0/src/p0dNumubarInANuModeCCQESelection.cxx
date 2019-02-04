#include "baseSelection.hxx"
#include "p0dNumubarInANuModeCCQESelection.hxx"
#include "p0dWaterNumubarInAntiNuModeCCMultiTrackSelection.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"

//********************************************************************
p0dNumubarInANuModeCCQESelection::p0dNumubarInANuModeCCQESelection(bool forceBreak): SelectionBase(forceBreak, EventBoxId::kEventBoxTracker) {
//********************************************************************

}

//********************************************************************
void p0dNumubarInANuModeCCQESelection::DefineSteps(){
//********************************************************************

  p0dNumubarInANuModeCCQESelection p0dnumubarInANuModeCCMultiTrack;
  CopySteps(p0dnumubarInANuModeCCMultiTrack);

}

//********************************************************************
void p0dNumubarInANuModeCCQESelection::DefineDetectorFV(){
//********************************************************************

  SetDetectorFV(SubDetId::kP0D);
}

//********************************************************************
bool p0dNumubarInANuModeCCQESelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

    if(allCutsPassed[0]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kP0DWaterNuMuBarInAntiNuModeCC1Track;
    }
    if(allCutsPassed[1]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kP0DWaterNuMuBarInAntiNuModeCCNTracks;
    }
    return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}

//**************************************************
void p0dNumubarInANuModeCCQESelection::InitializeEvent(AnaEventC& eventC){
//**************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventC);

  // Create the appropriate EventBox if it does not exist yet
  if (!event.EventBoxes[EventBoxId::kEventBoxTracker])
    event.EventBoxes[EventBoxId::kEventBoxTracker] = new EventBoxTracker();

  boxUtils::FillTracksWithTPC(event,             static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTracksWithP0D(event);
  boxUtils::FillTrajsChargedInTPC(event);
  boxUtils::FillTrajsChargedInP0D(event);


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


//**************************************************
Int_t p0dNumubarInANuModeCCQESelection::GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
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
  groups[   kP0DOOFV        ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPCInP0DFV );
  groups[   kTpcTrackEff    ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPC        );
  groups[   kTpcP0dMatchEff ] = CreateVectorI( 1, EventBoxTracker::kTracksWithTPC        );
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
Int_t p0dNumubarInANuModeCCQESelection::GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const{
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


