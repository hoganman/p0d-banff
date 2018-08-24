#ifndef p0dNumubarCCSelection_h
#define p0dNumubarCCSelection_h

#include "SelectionBase.hxx"
#include "numuCCSelection.hxx"
#include "EventBoxUtils.hxx"

class p0dNumubarCCSelection: public SelectionBase{
public:
  p0dNumubarCCSelection(bool forceBreak=true);
  virtual ~p0dNumubarCCSelection(){}

  //========= These are mandatory functions ==================

  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox() {return new ToyBoxTracker();}
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  /// Fill the EventBox with the objects needed by this selection
  virtual void InitializeEvent(AnaEventC&);
  SampleId_h GetSampleId(){return SampleId::kP0DNuMuBarCC;}

  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;


  //==========================================================
};

class FillSummaryAction_p0dNumubarCC: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FillSummaryAction_p0dNumubarCC();}
};

class FindP0DLeadingTracksAction_p0dNumubarCC: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindP0DLeadingTracksAction_p0dNumubarCC();}
};

//class FindP0DVetoAction_p0dNumubarCC: public StepBase{
// public:
//  using StepBase::Apply;
//  bool Apply(AnaEventC& event, ToyBoxB& box) const;
//  StepBase* MakeClone(){return new FindP0DVetoAction_p0dNumubarCC();}
//};
//
//class P0DSelectionVetoCut_p0dNumubarCC: public StepBase{
// public:
//  using StepBase::Apply;
//  bool Apply(AnaEventC& event, ToyBoxB& box) const;
//  StepBase* MakeClone(){return new P0DSelectionVetoCut_p0dNumubarCC();}
//};

#endif
