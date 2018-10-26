#ifndef PSYCHEOBSERVABLES_HXX
#define PSYCHEOBSERVABLES_HXX

#include "BANFFObservableBase.hxx"
#include "BANFFEventBase.hxx"

class psycheLeptonCandidateMomentum: public BANFFObservableBase{

public:

  psycheLeptonCandidateMomentum();
  virtual ~psycheLeptonCandidateMomentum(){};
  float GetCurrentValue(BANFFEventBase* event);

private:
  bool   ApplyCoulombShift;
  double ShiftMuCarbon ;
  double ShiftAMuCarbon;
  double ShiftMuOxygen ;
  double ShiftAMuOxygen;

public:
  void SetApplyCoulombShift(const bool apply=true){ ApplyCoulombShift = apply; };
  void SetShiftMuCarbon    (const double s=-3.6)  { ShiftMuCarbon  = s; };
  void SetShiftAMuCarbon   (const double s=+2.6)  { ShiftAMuCarbon = s; };
  void SetShiftMuOxygen    (const double s=-4.3)  { ShiftMuOxygen  = s; };
  void SetShiftAMuOxygen   (const double s=+3.3)  { ShiftAMuOxygen = s; };

  bool   GetApplyCoulombShift()const { return ApplyCoulombShift; };
  double GetShiftMuCarbon    ()const { return ShiftMuCarbon ;    };
  double GetShiftAMuCarbon   ()const { return ShiftAMuCarbon;    };
  double GetShiftMuOxygen    ()const { return ShiftMuOxygen ;    };
  double GetShiftAMuOxygen   ()const { return ShiftAMuOxygen;    };
  
};

class psycheLeptonCandidateCosTheta: public BANFFObservableBase{

    public:

        psycheLeptonCandidateCosTheta();
        virtual ~psycheLeptonCandidateCosTheta(){};

        float GetCurrentValue(BANFFEventBase* event);


};


#endif
