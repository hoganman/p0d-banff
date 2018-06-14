#ifndef FGDPIDVariation_h
#define FGDPIDVariation_h

#include "BinnedParams.hxx"
#include "BaseDataClasses.hxx"

class ToyExperiment;


/// This variation smears PID pull parameters of the FGD tracks
/// 
class FGDPIDVariation{
public:

  /// Instantiate the PID variation,  
  FGDPIDVariation(){ 
    _fgd1mean  = NULL;
    _fgd1sigma = NULL;
    _fgd2mean  = NULL;
    _fgd2sigma = NULL;

  }

  virtual ~FGDPIDVariation() {

    if (_fgd1mean)  delete _fgd1mean;  _fgd1mean  = NULL;
    if (_fgd1sigma) delete _fgd1sigma; _fgd1sigma = NULL;
    if (_fgd2mean)  delete _fgd2mean;  _fgd2mean  = NULL;
    if (_fgd2sigma) delete _fgd2sigma; _fgd2sigma = NULL;

  }

  /// Apply variation for a track, the most general case given a certain ToyExperiment
  virtual void ApplyVariation(AnaTrackB* track, const ToyExperiment& exp);

  /// Get inputs for the variation
  /// Three params to retrieve needed to apply the variation: 
  /// 1. pull_mean -- mean of the pull so to vary the sigma given the ratio,
  /// e.g. pull value for the MC analysis sample
  /// 2. mean_var -- variation for the mean
  /// 3. sigma_var -- variation for the sigma ratio
  virtual bool GetVariation(
      const AnaParticleB& track,
      Float_t& pull_mean, 
      Float_t& mean_var, 
      Float_t& sigma_var, 
      const ToyExperiment& exp) = 0;


protected:

  /// Expect that the derived classes should use some data to store the params for the variations
  /// so keep the data-storage here

  BinnedParams* _fgd1mean;
  BinnedParams* _fgd1sigma;
  BinnedParams* _fgd2mean;
  BinnedParams* _fgd2sigma;
};

#endif
