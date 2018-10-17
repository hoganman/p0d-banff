#ifndef p0dAntiNumuCCAnalysis_h
#define p0dAntiNumuCCAnalysis_h
#include "baseTrackerAnalysis.hxx"
#include "p0dNumuCCAnalysis.hxx"
#include "numuCCAnalysis.hxx"
#include "ND280AnalysisUtils.hxx"

class p0dAntiNumuCCAnalysis: public baseTrackerAnalysis {
 public:
  p0dAntiNumuCCAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~p0dAntiNumuCCAnalysis(){delete _numuCCAnalysis;}

  //---- These are mandatory functions  
  /// overload the function from the base class
  // virtual void InitializeConfiguration();

  void DefineSelections();
  void DefineCorrections();
  void DefineSystematics();
  void DefineMicroTrees(bool addBase=true);
  void DefineTruthTree();
  void DefineConfigurations();

  void FillCategories();

  void FillMicroTrees(bool addBase=true);
  void FillToyVarsInMicroTrees(bool addBase=true);

  bool CheckFillTruthTree(const AnaTrueVertex& vtx);

  void FillTruthTree(const AnaTrueVertex& vtx);
  //--------------------

  // Set parameters
  bool Initialize();

  enum enumSyst_p0dAntiNumuCCAnalysis{
    kP0DELossResolVar,
    kP0DELossScaleVar,
    kP0dMomScale,
    kP0dMomResol,
    kTrackerMomResol,
    kTrackerMomScale,
    kTrackerChargeIDEff,
    kTrackerTPCPID,
    kTrackerBFieldDistortion,
    kTrackerTPCClusterEff,
    kTrackerTPCTrackEff,
    kP0dMass,
    kP0dOOFV,
    kP0dVeto,
    kTestResol,
    kTestScale,
    enumSystLast_p0dAntiNumuCCAnalysis
  };

  enum enumConf_p0dAntiNumuCCAnalysis{
    all_syst,
    test_syst,
    p0dmomscale_syst,
    p0dmomresol_syst,
    trackermomresol_syst,
    trackermomscale_syst,
    trackerchargeideff_syst,
    trackertpcpid_syst,
    trackerbfielddistortion_syst,
    trackertpcclustereff_syst,
    trackertpctrackeff_syst,
    p0dmass_syst,
    p0doofv_syst,
    p0dveto_syst,
    enumConfLast_p0dAntiNumuCCAnalysis
  };

  enum enumStandardMicroTrees_p0dAntiNumuCCAnalysis{
    p0dTPC_PID=numuCCAnalysis::enumStandardMicroTreesLast_numuCCAnalysis + 1,
    np0dNew,
    nNodesShort,
    TwoTrackSepSqrd,
    TwoTrackLength,
    newmu_mom,
    newmu_costheta,
    TagMichel1,
    TagMichel2,
    TrkChargeRatio,
    CloseTrkChargeRatio,
    TrkChargeRatioErr,
    TrkChargeRatioPDGs,
    TrkChargeRatioNClusters,
    PIDnShowers,
    selmu_momNew,
    p0dELoss,
    trueP0dELoss,
    eCalVeto,
    TCR_PosX,
    TCR_PosY,
    TCR_PosZ,
    nMuLike,
    TrkChargeMin,
    enumStandardMicroTreesLast_p0dAntiNumuCCAnalysis
  };


protected:

  numuCCAnalysis* _numuCCAnalysis;
  p0dNumuCCAnalysis* _p0dNumuCCAnalysis;

  bool _saveAllNumuCC;
  bool _isUsingReconDirP0DNew;
  bool _IsAntiNu;
  
};


#endif
