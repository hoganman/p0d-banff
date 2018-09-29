#ifndef p0dAirNumuBkgInAntiNuModeCCSelection_h
#define p0dAirNumuBkgInAntiNuModeCCSelection_h

#include "SelectionBase.hxx"
#include "numuCCSelection.hxx"
#include "EventBoxUtils.hxx"

class p0dAirNumuBkgInAntiNuModeCCSelection: public SelectionBase{
public:
  p0dAirNumuBkgInAntiNuModeCCSelection(bool forceBreak=true);
  virtual ~p0dAirNumuBkgInAntiNuModeCCSelection(){}

  //========= These are mandatory functions ==================
  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox() {return new ToyBoxTracker();}
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  /// Fill the EventBox with the objects needed by this selection
  virtual void InitializeEvent(AnaEventC&);
  SampleId_h GetSampleId(){return SampleId::kP0DAirNuMuBkgInAntiNuModeCC;}

  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  //==========================================================
};

class FillSummaryAction_p0dAirNumuBkgInAntiNuModeCC: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FillSummaryAction_p0dAirNumuBkgInAntiNuModeCC();}
};


#endif
