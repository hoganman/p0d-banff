#ifndef P0DWATERNUMUBKGINANTINUMODECCMULTITRACKSELECTION_HXX
#define P0DWATERNUMUBKGINANTINUMODECCMULTITRACKSELECTION_HXX

#include "p0dWaterNumuCCSelection.hxx"

class p0dWaterNumuBkgInAntiNuModeCCMultiTrackSelection: public SelectionBase{
public:
  p0dWaterNumuBkgInAntiNuModeCCMultiTrackSelection(bool forceBreak=true);
  virtual ~p0dWaterNumuBkgInAntiNuModeCCMultiTrackSelection(){}

  //========= These are mandatory functions ==================
  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox() {return new ToyBoxTracker();}
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  /// Fill the EventBox with the objects needed by this selection
  virtual void InitializeEvent(AnaEventC&);
  SampleId_h GetSampleId(){return SampleId::kP0DWaterNuMuBkgInAntiNuModeCC;}

  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  //==========================================================
};

class FillSummaryAction_p0dWaterNumuBkgInAntiNuModeCCMultiTrack: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FillSummaryAction_p0dWaterNumuBkgInAntiNuModeCCMultiTrack();}
};


#endif
