#ifndef __THROWPARAM_HH__
#define __THROWPARAM_HH__

#include <iostream>
#include <assert.h>
#include <algorithm>
#include <math.h>

#include <TMath.h>
#include <TMatrixT.h>
#include <TMatrixTSym.h>
#include <TVectorT.h>
#include <TDecompChol.h>

#include <vector>
#include <iostream>
#include <TRandom3.h>
#include <TF1.h>

class ThrowParms 
{
 private:
  typedef TVectorT<double> TVectorD;
  typedef TMatrixTSym<double> TMatrixDSym;
  typedef TMatrixT<double> TMatrixD;

  TVectorD    *pvals;
  TMatrixDSym *covar; 
  TMatrixD    *chel_dec;
  int         npars;
  TRandom3    rand; 
  TF1         *gauss;

 public:
  ThrowParms(TVectorD &parms, TMatrixDSym &covm);
  /*  {  
      npars = parms.GetNrows();
      pvals = new TVectorD(npars);
      covar = new TMatrixDSym(npars);
      //parms.Print();
      (*pvals) = parms;
      (*covar) = covm;
      TDecompChol chdcmp(*covar);
      if(!chdcmp.Decompose())
	{
	  std::cerr<<"Cholesky decomposition failed"<<std::endl;
	  exit(-1);
	}
      chel_dec = new TMatrixD(chdcmp.GetU());
    };*/
   
  ~ThrowParms();
   /* {
      if(pvals!=NULL)    delete pvals;
      if(covar!=NULL)    delete covar;
      if(chel_dec!=NULL) delete chel_dec;
    };*/
   
  void SetSeed(int seed = 1867) {rand.SetSeed(seed);};
  int GetSize() {return npars;};
  void ThrowSet(std::vector<double> &parms);
  /*{
    if(!parms.empty()) parms.clear();
    parms.resize(npars);
    int half_pars = npars/2+npars%2;
    TVectorD std_rand(npars);
    for(int j=0; j<half_pars; j++){
      double z[2];
      StdNormRand(z);
      std_rand(j) = z[0];
      if(npars%2==0 || j!=half_pars-1)
	std_rand(j+half_pars) = z[1];
    }
    TVectorD prod = (*chel_dec)*std_rand;
    for(int i=0;i<npars;i++)
      parms[i] = prod(i) + (*pvals)(i);
  };*/

 private:
  void CheloskyDecomp(TMatrixD &chel_mat);
  void StdNormRand(double *z);
  /*{

    double u = 2.*rand.Rndm()-1.;
    double v = 2.*rand.Rndm()-1.;
       
    double s = u*u+v*v;

    while(s==0 || s>=1.){
      u = 2.*rand.Rndm()-1.;
      v = 2.*rand.Rndm()-1.;
      s = u*u+v*v;
    }
       
    z[0] = u*sqrt(-2.*TMath::Log(s)/s);
    z[1] = v*sqrt(-2.*TMath::Log(s)/s);
  };*/
   
};
#endif