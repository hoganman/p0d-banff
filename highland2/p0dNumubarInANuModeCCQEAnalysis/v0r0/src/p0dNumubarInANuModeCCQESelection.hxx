#ifndef p0dNumubarInANuModeCCQESelection_h
#define p0dNumubarInANuModeCCQESelection_h

#include "SelectionBase.hxx"

class p0dNumubarInANuModeCCQESelection: public SelectionBase{
public:
  p0dNumubarInANuModeCCQESelection(bool forceBreak=true);
  virtual ~p0dNumubarInANuModeCCQESelection(){}

  //========= These are mandatory functions ==================

  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox() {return new ToyBoxTracker();}
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  /// Fill the EventBox with the objects needed by this selection
  virtual void InitializeEvent(AnaEventC&);
  SampleId_h GetSampleId(){return SampleId::kP0DNuMuCC;}

  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  //==========================================================
};

class SingleP0DPIDCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new SingleP0DPIDCut();}
};

class MultipleP0DPIDCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new MultipleP0DPIDCut();}
};
#endif
