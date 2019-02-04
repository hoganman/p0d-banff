#ifndef p0dNumubarInANuModeCCQEAnalysis_h
#define p0dNumubarInANuModeCCQEAnalysis_h

#include "baseTrackerAnalysis.hxx"
#include "p0dNumuCCAnalysis.hxx"
#include "ND280AnalysisUtils.hxx"

class p0dNumubarInANuModeCCQEAnalysis: public baseTrackerAnalysis {
 public:
  p0dNumubarInANuModeCCQEAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~p0dNumubarInANuModeCCQEAnalysis(){delete _p0dNumuCCAnalysis;}

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

  using baseTrackerAnalysis::FillTruthTree;
  void FillTruthTree(const AnaTrueVertex& vtx);
  //--------------------

  // Set parameters
  bool Initialize();

  enum enumSyst_p0dNumubarInANuModeCCQEAnalysis{
    kp0dCCQEOOFV = p0dNumuCCAnalysis::enumSystLast_p0dNumuCCAnalysis+1,
    enumSystLast_p0dNumubarInANuModeCCQEAnalysis
  };

  enum enumConf_p0dNumubarInANuModeCCQEAnalysis{
    p0dccqeoofv_syst = p0dNumuCCAnalysis::enumConfLast_p0dNumuCCAnalysis+1,
    enumConfLast_p0dNumubarInANuModeCCQEAnalysis
  };

protected:

  p0dNumuCCAnalysis* _p0dNumuCCAnalysis;

  bool _saveAllNumuCC;
  
};

#endif
