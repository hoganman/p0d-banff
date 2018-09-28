#ifndef p0dWaterNumuCCSelection_h
#define p0dWaterNumuCCSelection_h

#include "SelectionBase.hxx"
#include "numuCCSelection.hxx"
#include "EventBoxUtils.hxx"

class p0dWaterNumuCCSelection: public SelectionBase{
public:
  p0dWaterNumuCCSelection(bool forceBreak=true);
  virtual ~p0dWaterNumuCCSelection(){}

  //========= These are mandatory functions ==================

  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox() {return new ToyBoxTracker();}
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  /// Fill the EventBox with the objects needed by this selection
  virtual void InitializeEvent(AnaEventC&);
  SampleId_h GetSampleId(){return SampleId::kP0DWaterNuMuCC;}

  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;


  //==========================================================
};

class FillSummaryAction_p0dWaterNumuCC: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FillSummaryAction_p0dWaterNumuCC();}
};

class FindP0DVetoAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindP0DVetoAction();}
};

class P0DSelectionVetoCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new P0DSelectionVetoCut();}
};

#endif
