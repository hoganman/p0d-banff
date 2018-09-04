#ifndef P0DOOFVSystematics_h
#define P0DOOFVSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"

const UInt_t NMAXP0DOOFVSYSTEMATICSBINS = 9;

class P0DOOFVSystematics: public EventWeightBase {
public:

  using EventWeightBase::ComputeWeight;
  P0DOOFVSystematics();

  virtual ~P0DOOFVSystematics()
  {
    if (_p0d)  delete _p0d;  _p0d = NULL;
    if (_rate) delete _rate; _rate = NULL;
  }

  Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box);


protected:

  ///
  void Initialize();

  ///
  Int_t GetDetNumber(const SubDetId::SubDetEnum& det) const;

  ///
  Int_t GetBeamNumber(const Int_t& runperiod,AnaTrackB* maintrack) const;

protected:

  /// Mean of the reco correction
  Float_t _reco_corr;

  /// Uncertainty on the reco correction
  Float_t _reco_error;

  /// Mean of the rate eff correction
  Float_t _rate_corr[NMAXP0DOOFVSYSTEMATICSBINS];

  /// Uncertainty on the rate eff correction
  Float_t _rate_error[NMAXP0DOOFVSYSTEMATICSBINS];

  /// Index for rate corrections
  Int_t _rate_index[NMAXP0DOOFVSYSTEMATICSBINS];

  /// Index for reco corrections
  Int_t _reco_index;

  /// The OOFV reco systematic param
  BinnedParams* _p0d;

  /// The OOFV other sub-detector rates systematic param
  BinnedParams* _rate;

};

#endif
