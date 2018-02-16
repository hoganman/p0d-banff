#ifndef antiNumuCCMultiTrackSelection_h
#define antiNumuCCMultiTrackSelection_h

#include "SelectionBase.hxx"
#include "antiNumuCCSelection.hxx"

class antiNumuCCMultiTrackSelection: public SelectionBase{

public:
  antiNumuCCMultiTrackSelection(bool forceBreak=true);
  virtual ~antiNumuCCMultiTrackSelection(){}

  //---- These are mandatory functions
  void DefineSteps();
  void DefineDetectorFV();
  ToyBoxB* MakeToyBox(){return new ToyBoxTracker();}
  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  SampleId_h GetSampleId(){return SampleId::kFGD1AntiNuMuCC1Track;}
  bool IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const;
  bool IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const;
  void InitializeEvent(AnaEventC& event);
  bool CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep);

  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h systId, Int_t* IDs, Int_t branch) const;

  void SetDetectorFV(SubDetId_h det, Int_t ibranch=-1){
    SelectionBase::SetDetectorFV(det, ibranch);
    // Set to the selection that is used
    _antiNumuCCSelection.SetDetectorFV(det, ibranch); 
  }

protected:
  antiNumuCCSelection _antiNumuCCSelection;

};

class FillSummaryAction_antinu_CCMultiTrack: public StepBase{
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;  
  StepBase* MakeClone(){return new FillSummaryAction_antinu_CCMultiTrack();}
};

class CCQEcut_antinu: public StepBase{

public:
  CCQEcut_antinu(SubDetId::SubDetEnum det = SubDetId::kFGD1){
    if (!SubDetId::IsFGDDetector(det)){
      std::cout << " antuNumuCCMultiTrackSelection, CCQEcut_antinu: det is not FGD" << det << std::endl;
      exit(1);
    }
    _det = det;
  }
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new CCQEcut_antinu(_det);}

protected:
  SubDetId::SubDetEnum _det;
};

class CCnQEcut_antinu: public StepBase{
public:
  CCnQEcut_antinu(SubDetId::SubDetEnum det = SubDetId::kFGD1){
    if (!SubDetId::IsFGDDetector(det)){
      std::cout << " antuNumuCCMultiTrackSelection, CCnQEcut_antinu: det is not FGD" << det << std::endl;
      exit(1);
    }
    _det = det;
  }
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new CCnQEcut_antinu(_det);}
protected:
  SubDetId::SubDetEnum _det;
};
#endif
