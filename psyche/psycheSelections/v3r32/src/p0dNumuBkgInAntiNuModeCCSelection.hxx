#ifndef p0dNumuBkgInAntiNuModeCCSelection_h
#define p0dNumuBkgInAntiNuModeCCSelection_h

#include "p0dNumuCCSelection.hxx"

class p0dNumuBkgInAntiNuModeCCSelection: public SelectionBase{
public:
  p0dNumuBkgInAntiNuModeCCSelection(bool forceBreak=true);
  virtual ~p0dNumuBkgInAntiNuModeCCSelection(){}

  //========= These are mandatory functions ==================
  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox() {return new ToyBoxTracker();}
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  /// Fill the EventBox with the objects needed by this selection
  virtual void InitializeEvent(AnaEventC&);
  SampleId_h GetSampleId(){return SampleId::kP0DNuMuBkgInAntiNuModeCC;}

  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  //==========================================================
};

class FillSummaryAction_p0dNumuBkgInAntiNuModeCC: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FillSummaryAction_p0dNumuBkgInAntiNuModeCC();}
};


#endif
