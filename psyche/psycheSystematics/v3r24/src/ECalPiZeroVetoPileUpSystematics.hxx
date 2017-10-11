#ifndef ECalPiZeroVetoPileUpSystematics_h
#define ECalPiZeroVetoPileUpSystematics_h

#include "EventWeightBase.hxx"
#include "BinnedParams.hxx"
#include "FluxWeighting.hxx"

/// This is a normalization systematic. It takes into account the uncertainty on the 
/// probability to select a "fake" ECal-shower candidate used for pi-zero
/// tagging

class ECalPiZeroVetoPileUpSystematics: public EventWeightBase, public BinnedParams {
  public:

    ECalPiZeroVetoPileUpSystematics();  
    virtual ~ECalPiZeroVetoPileUpSystematics() {}

    /// Apply this systematic
    using EventWeightBase::ComputeWeight;
    Weight_h ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box);
};


#endif
