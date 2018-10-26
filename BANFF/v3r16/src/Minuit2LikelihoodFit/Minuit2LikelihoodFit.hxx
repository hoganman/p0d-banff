#ifndef MINUIT2LIKELIHOODFIT_HXX
#define MINUIT2LIKELIHOODFIT_HXX
#include <vector>
#include <iostream>
#include <string>
#include "Minuit2LikelihoodFitFCN.hxx"
#include "TMatrixDSym.h"
#include "TVectorD.h"
#include "TAxis.h"
#include "Minuit2/MnUserParameters.h"
#include "TFitterMinuit.h"


///A class to handle all the setup needed for performing a likelihood fit using
///Minuit2
class Minuit2LikelihoodFit {

public:
  Minuit2LikelihoodFit(BANFFInterfaceBase* inter, std::string outputFileName);
  ~Minuit2LikelihoodFit();

  void Reset();

  void InitializeParameters();

  void ScanParameters();

  void DoFit();

  void InitializeParametersFromFile(std::string inputFileName);
  //Class variables
        
  ///A pointer to the BANFFInterfaceBase with all the information needed
  ///to perform the fit.
  BANFFInterfaceBase* interface;

  TFile* outputFile;

  ///The fit function used in the fit.
  Minuit2LikelihoodFitFCN* minuit2_likelihood_fit_fcn;

  ///The covariance matrix from migrad.
  TMatrixDSym* migrad_covariance;

  ///The Hessian matrix from migrad.
  TMatrixDSym* migrad_hessian;

  ///The parameters fed to Minuit2.
  ROOT::Minuit2::MnUserParameters *params;

  ///The Minuit fitter.
  TFitterMinuit *fminuit;

  ///A counter used to count how many times DoFit() has been called with
  ///this instance of Minuit2LikelihoodFit.  Primarly useful for
  ///validation purposes (don't need to reload MC when want to fit
  ///against different toys.
  int iteration;

  TVectorD *fcnMin;

};
#endif
