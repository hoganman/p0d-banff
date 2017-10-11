#ifndef __EIGENDECOMP_HH__
#define __EIGENDECOMP_HH__

#include <iostream>
#include <assert.h>
#include <algorithm>
#include <math.h>

#include <TMath.h>
#include <TMatrixDSym.h>
#include <TMatrixD.h>
#include <TVectorD.h>

#include <vector>
#include <iostream>

class EigenDecomp 
{
 private:
  TMatrixDSym *orig_cov;
  TMatrixD *eigen_vectors;
  TMatrixD *eigen_vectorsT;
  TMatrixDSym *eigen_cov;
  TVectorD *beta_nom;
  TVectorD *orig_params;

 public:
  EigenDecomp(TVectorD &params, TMatrixDSym &cov);
   
  ~EigenDecomp();
   
  void GetDecomposition(TVectorD &decomp_params, TMatrixDSym &decomp_cov);
  void GetOriginalParams(TVectorD &params_in, TVectorD &params_out, int start_param=0, int nparams=0);
  void GetOriginalCovariance(TMatrixDSym &cov_in, TMatrixDSym &cov_out, int start_param=0, int nparams=0);
  double BetaNom(int index){ return (*beta_nom)(index); };
  double EigenValue(int index){ return (*eigen_cov)(index,index); };
  
 private:

  void Decompose();
  int npars;
   
};
#endif
