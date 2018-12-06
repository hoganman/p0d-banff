#ifndef P0DWATERNUMUCCMULTITRACKSELECTION_HXX
#define P0DWATERNUMUCCMULTITRACKSELECTION_HXX

#include "SelectionBase.hxx"
#include "numuCCSelection.hxx"
#include "EventBoxUtils.hxx"

class p0dWaterNumuCCMultiTrackSelection: public SelectionBase{
public:
  p0dWaterNumuCCMultiTrackSelection(bool forceBreak=true);
  virtual ~p0dWaterNumuCCMultiTrackSelection(){}

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
  static Int_t GetNP0DTracks(AnaEventC& eventC);
};

class FillSummaryAction_p0dWaterNumuCCMultiTrack: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FillSummaryAction_p0dWaterNumuCCMultiTrack();}
};

class SingleP0DTrackCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new SingleP0DTrackCut();}
};

class MultipleP0DTracksCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new MultipleP0DTracksCut();}
};

#endif
