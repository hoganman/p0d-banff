#ifndef p0dAntiNumuCCSelection_h
#define p0dAntiNumuCCSelection_h

#include "SelectionBase.hxx"
#include "numuCCSelection.hxx"

class p0dAntiNumuCCSelection: public SelectionBase{
public:
  p0dAntiNumuCCSelection(bool forceBreak=true);
  virtual ~p0dAntiNumuCCSelection(){}

  //========= These are mandatory functions ==================

  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox() {return new ToyBoxTracker();}
  /// Fill the EventBox with the objects needed by this selection
  //old//virtual void InitializeEvent(AnaEventB&);
  virtual void InitializeEvent(AnaEventC&);
  //bool FillEventSummary(AnaEventC&, Int_t*);//added Feb22
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  //void p0dAntiNumuCCSelection::FillEventSummary(AnaEventC&, Int_t*);

  //==========================================================

  SampleId::SampleEnum GetSampleEnum(){return SampleId::kFGD1NuMuCC;}
};

class FindP0DLeadingTracksAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindP0DLeadingTracksAction();}
};

class TrackHighMomentumCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& boxB) const;
  StepBase* MakeClone(){return new TrackHighMomentumCut();}
};

class P0DMuonPIDCut: public StepBase{
 public:
  using StepBase::Apply;
  //old//bool Apply(AnaEventB& event, ToyBoxB& box) const;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new P0DMuonPIDCut();}
};
class P0DCC0piCut: public StepBase{
 public:
  using StepBase::Apply;
  //old//bool Apply(AnaEventB& event, ToyBoxB& box) const;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new P0DCC0piCut();}
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
