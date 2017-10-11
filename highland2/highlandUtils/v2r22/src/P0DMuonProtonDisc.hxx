#ifndef P0DMUONPROTONDISC_HXX
#define P0DMUONPROTONDISC_HXX

#include "TVector3.h"
#include "TH2F.h"
#include "TFile.h"

#include "DataClasses.hxx"

#include "P0DAnalysisUtils.hxx"
#include "P0DGeometryManager.hxx"

//************************************************************

class P0DMuonProtonDisc{
 public:
  //P0DMuonProtonDisc(){};
  P0DMuonProtonDisc(std::string filename); 

  //Likelihood access methods
  double GetLikelihoodCoarse(AnaP0DParticle *Track,bool isWater,float energyCorrection,float useLength);
  double GetLikelihoodCoarseBeg(AnaP0DParticle *Track,bool isWater,float energyCorrection,float useLength);
  double GetLikelihoodFineCharge(AnaP0DParticle *Track,bool isWater,float energyCorrection,float useLength);
  double GetLikelihoodFineDistance(AnaP0DParticle *Track,bool isWater,float energyCorrection,float useLength);
  double GetLikelihoodFine(AnaP0DParticle *Track,bool isWater,float energyCorrection,float useLength);
  double GetLikelihoodFineBeg(AnaP0DParticle *Track,bool isWater,float energyCorrection,float useLength);

  //Chi2 access methods
  double GetChi2TrackEnd(AnaP0DParticle *Track,bool isWater,float energyCorrection,float useLength,bool noAngle);
  double GetChi2TrackBeg(AnaP0DParticle *Track,bool isWater,float energyCorrection,float tracklength,bool noAngle);

  //Pull access methods
  double GetPullTrackEnd(AnaP0DParticle *Track,bool isWater,float energyCorrection,float useLength,bool noAngle);
  double GetPullTrackBeg(AnaP0DParticle *Track,bool isWater,float energyCorrection,float tracklength,bool noAngle);

 private:  
  //Likelihood PDF histograms
  //Waterin
  TH2F *fLCoarse[4];
  TH2F *fLFineDistance[4];
  TH2F *fLFineCharge[4];
  TH2F *fLFine[4];

  //Waterout
  TH2F *fLCoarseWaterOut[4];
  TH2F *fLFineDistanceWaterOut[4];
  TH2F *fLFineChargeWaterOut[4];
  TH2F *fLFineWaterOut[4];

  //CHi2 method
  //Waterin
  vector<vector<float> >fChargeMeans;
  vector<vector<float> >fChargeSigmas;
  vector<vector<float> >fChargeNorm;
  //Waterout
  vector<vector<float> >fChargeMeansWaterOut;
  vector<vector<float> >fChargeSigmasWaterOut;
  vector<vector<float> >fChargeNormWaterOut;

  static const float fChi2BinSize      = 67.8;  //Optimal for Water Target region
  //static const float fChi2BinSize    = 43.12;//Optimal for ECals
  static const float fChi2GCMBinSize = 2.0;

  //Find declaration in TMuonProton.cxx class constructor
  string fDefaultSoftwareVersion;
  
  void loadPIDValues(vector<vector<float> >&pidconst,
                     vector<vector<float> >&pidmean,
                     vector<vector<float> >&pidsigma,
                     string filename, int binnumber);

  bool CheckIfFileExists(string filename);

  string GetFileSoftwareVersion(string filename);
};

#endif
