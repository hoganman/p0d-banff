#ifndef p0dNumubarBkgCCSelection_h
#define p0dNumubarBkgCCSelection_h

#include "SelectionBase.hxx"
#include "numuCCSelection.hxx"
#include "EventBoxUtils.hxx"

class p0dNumubarBkgCCSelection: public SelectionBase{
public:
  p0dNumubarBkgCCSelection(bool forceBreak=true);
  virtual ~p0dNumubarBkgCCSelection(){}

  //========= These are mandatory functions ==================

  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox() {return new ToyBoxTracker();}
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  /// Fill the EventBox with the objects needed by this selection
  virtual void InitializeEvent(AnaEventC&);
  SampleId_h GetSampleId(){return SampleId::kP0DNuMuBarBkgCC;}

  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;


  //==========================================================
};

class FillSummaryAction_p0dNumubarBkgCC: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FillSummaryAction_p0dNumubarBkgCC();}
};

class FindP0DLeadingTracksAction_p0dNumubarBkgCC: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindP0DLeadingTracksAction_p0dNumubarBkgCC();}
};

//class FindP0DVetoAction_p0dNumubarBkgCC: public StepBase{
// public:
//  using StepBase::Apply;
//  bool Apply(AnaEventC& event, ToyBoxB& box) const;
//  StepBase* MakeClone(){return new FindP0DVetoAction_p0dNumubarBkgCC();}
//};
//
//class P0DSelectionVetoCut_p0dNumubarBkgCC: public StepBase{
// public:
//  using StepBase::Apply;
//  bool Apply(AnaEventC& event, ToyBoxB& box) const;
//  StepBase* MakeClone(){return new P0DSelectionVetoCut_p0dNumubarBkgCC();}
//};

#endif
