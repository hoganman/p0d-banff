#ifndef p0dAntiNumuCCAnalysis_h
#define p0dAntiNumuCCAnalysis_h
#include "baseTrackerAnalysis.hxx"
#include "p0dNumuCCAnalysis.hxx"
#include "ND280AnalysisUtils.hxx"

class p0dAntiNumuCCAnalysis: public baseTrackerAnalysis {
 public:
  p0dAntiNumuCCAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~p0dAntiNumuCCAnalysis(){delete _p0dNumuCCAnalysis;}

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
    enumSystLast_p0dAntiNumuCCAnalysis
  };

  enum enumConf_p0dAntiNumuCCAnalysis{
    all_syst,
    enumConfLast_p0dAntiNumuCCAnalysis
  };

  enum enumStandardMicroTrees_p0dAntiNumuCCAnalysis{
    p0dTPC_PID=p0dNumuCCAnalysis::enumStandardMicroTreesLast_p0dNumuCCAnalysis + 1,
    np0dNew,
    nNodesShort,
    TwoTrackSepSqrd,
    TwoTrackLength,
    newmu_mom,
    newmu_costheta,
    TagMichel1,
    TagMichel2,
    TrkChargeRatio,
    TrkChargeRatioPDGs,
    enumStandardMicroTreesLast_p0dAntiNumuCCAnalysis
  };


protected:

  p0dNumuCCAnalysis* _p0dNumuCCAnalysis;

  bool _saveAllNumuCC;
  bool _isUsingReconDirP0DNew;
  bool _IsAntiNu;
  
};


#endif
