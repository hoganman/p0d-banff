#ifndef p0dAirNumuCCSelection_h
#define p0dAirNumuCCSelection_h

#include "SelectionBase.hxx"
#include "numuCCSelection.hxx"
#include "EventBoxUtils.hxx"

class p0dAirNumuCCSelection: public SelectionBase{
public:
  p0dAirNumuCCSelection(bool forceBreak=true);
  virtual ~p0dAirNumuCCSelection(){}

  //========= These are mandatory functions ==================
  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox() {return new ToyBoxTracker();}
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  /// Fill the EventBox with the objects needed by this selection
  virtual void InitializeEvent(AnaEventC&);
  SampleId_h GetSampleId(){return SampleId::kP0DAirNuMuCC;}

  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  //==========================================================
};

class FillSummaryAction_p0dAirNumuCC: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FillSummaryAction_p0dAirNumuCC();}
};

#endif
