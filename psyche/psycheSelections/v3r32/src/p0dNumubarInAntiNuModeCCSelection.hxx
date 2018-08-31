#ifndef p0dNumubarInAntiNuModeCCSelection_h
#define p0dNumubarInAntiNuModeCCSelection_h

#include "SelectionBase.hxx"
#include "numuCCSelection.hxx"
#include "EventBoxUtils.hxx"

class p0dNumubarInAntiNuModeCCSelection: public SelectionBase{
public:
  p0dNumubarInAntiNuModeCCSelection(bool forceBreak=true);
  virtual ~p0dNumubarInAntiNuModeCCSelection(){}

  //========= These are mandatory functions ==================

  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox() {return new ToyBoxTracker();}
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  /// Fill the EventBox with the objects needed by this selection
  virtual void InitializeEvent(AnaEventC&);
  SampleId_h GetSampleId(){return SampleId::kP0DNuMuBarInAntiNuModeCC;}

  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;


  //==========================================================
};

class FillSummaryAction_p0dNumubarInAntiNuModeCC: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FillSummaryAction_p0dNumubarInAntiNuModeCC();}
};

class FindP0DLeadingTracksAction_p0dNumubarInAntiNuModeCC: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindP0DLeadingTracksAction_p0dNumubarInAntiNuModeCC();}
};

//class FindP0DVetoAction_p0dNumubarInAntiNuModeCC: public StepBase{
// public:
//  using StepBase::Apply;
//  bool Apply(AnaEventC& event, ToyBoxB& box) const;
//  StepBase* MakeClone(){return new FindP0DVetoAction_p0dNumubarInAntiNuModeCC();}
//};
//
//class P0DSelectionVetoCut_p0dNumubarInAntiNuModeCC: public StepBase{
// public:
//  using StepBase::Apply;
//  bool Apply(AnaEventC& event, ToyBoxB& box) const;
//  StepBase* MakeClone(){return new P0DSelectionVetoCut_p0dNumubarInAntiNuModeCC();}
//};

#endif
