#ifndef P0DAIRNUMUBARINANTINUMODECCSELECTION_HXX
#define P0DAIRNUMUBARINANTINUMODECCSELECTION_HXX

#include "SelectionBase.hxx"
#include "numuCCSelection.hxx"
#include "EventBoxUtils.hxx"

class p0dAirNumubarInAntiNuModeCCMultiTrackSelection: public SelectionBase{
public:
  p0dAirNumubarInAntiNuModeCCMultiTrackSelection(bool forceBreak=true);
  virtual ~p0dAirNumubarInAntiNuModeCCMultiTrackSelection(){}

  //========= These are mandatory functions ==================

  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox() {return new ToyBoxTracker();}
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  /// Fill the EventBox with the objects needed by this selection
  virtual void InitializeEvent(AnaEventC&);
  SampleId_h GetSampleId(){return SampleId::kP0DAirNuMuBarInAntiNuModeCC;}

  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;


  //==========================================================
};

class FillSummaryAction_p0dAirNumubarInAntiNuModeCCMultiTrack: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FillSummaryAction_p0dAirNumubarInAntiNuModeCCMultiTrack();}
};

#endif
