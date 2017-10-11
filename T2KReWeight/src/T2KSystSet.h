//____________________________________________________________________________
/*!

\class    t2krew::T2KSystSet

\brief    Set of systematics to be considered by T2KReWeight. Adapted from
          previous version T2KReWeight and the GENIE reweighting tools. 
          A T2KSystSet consists of a mapping between an enumeration of all
          the systematics we consider and an instance of the T2KSystInfo 
          class which stores, among other things, the current and the default
          value of the tweaking dial for the syst. The units of the tweak dial
          values stored in T2KSystInfo are in numbers of 1 sigma deviations 
          from the nominal value for that systematic. The definition of what a
          1sigma deviation is first taken from the generator but can be 
          overridden using the T2KSystSet::SetSigma method and the 
          T2KSystSet::SetCurValue with bool absolute set to true. 

          For example calling:
            T2KSystSet::SetCurValue(t2krew::kGXSec_MaCCQE, twkdialval);
          Would lead to kGXSec_MaCCQE syst to a tweaked value of:
            tweaked = nominal*(1.0+fractional_1sigma_error*twkdialval)
          Where fractional_1sigma_error = 1sigma_error/nominal and is taken
          from the generator. 

          You can override what the definition of the 1sigma fractional error
          using the following:
            T2KSystSet::SetSigma(t2krew::kGXSec_MaCCQE, new_fractional_1sigma_error);
          Which would lead to kGXSec_MaCCQE with a tweaked value of:
            tweaked = nominal(1.0+new_fractional_1sigma_error*twkdialval)

          Calling with the abosulte flag set to true:
             T2KSystSet::SetCurValue(t2krew::kGXSec_MaCCQE, twkdialval, true);
          Would lead to kGXSec_MaCCQE in units of absolute fractions resulting
          in a tweaked value of:
             tweaked = nominal(1.0+twkdialval)
          And is equivalent to calling first calling SetSigma with a value of 1.0.

\author   Jim Dobson <J.Dobson07 \at imperial.ac.uk>
          Imperial College London

\created  Nov 1, 2010

 @ July 4, 2011 - JD
   Added options to override default value of fractional 1 sigma errors taken
   from the generator. Also updated the documentation (see above).   

*/
//____________________________________________________________________________

#ifndef _T2KRW_SET_OF_SYSTEMATICS_H_
#define _T2KRW_SET_OF_SYSTEMATICS_H_

#include <string>
#include <map>
#include <vector>

#include "T2KSyst.h"

using std::string;
using std::map;
using std::vector;

namespace t2krew {

class T2KSystInfo;

class T2KSystSet {

public:  
  T2KSystSet();
  T2KSystSet(const T2KSystSet & syst_set);
 ~T2KSystSet();

  // Methods to add, remove and check if a syst is included 
  void    Include      (T2KSyst_t syst);
  void    Remove       (T2KSyst_t syst);
  void    RemoveAll    (void);
  int     NIncluded    (void) const;
  bool    IsIncluded   (T2KSyst_t syst) const;
  // Methods to set twk dial value (units of 1 sigma fractional err)
  void    SetTwkDial   (T2KSyst_t syst, double val);
  void    ResetTwkDial (T2KSyst_t syst);
  void    ResetAllTwkDials (void);
  // Methods to override the default frac error defined by generator (only 
  // use if you really understand what these do!)
  void    SetFracErr   (T2KSyst_t syst, double val);
  void    SetAbsTwk    (T2KSyst_t syst); 
  // Methods to get the current twk dial and fractional error values 
  double  CurTwkDialVal     (T2KSyst_t syst) const;
  double  CurFracErrVal     (T2KSyst_t syst) const;
  // Methods to see if default generator fractional errors have been overridden
  bool    UsingDefFracErr (T2KSyst_t syst) const;
  bool    AbsTweak     (T2KSyst_t syst) const;
  // Other useful methods
  void    PrintSummary (void);
  void    CopyFrom         (const T2KSystSet & syst_set);

  vector<t2krew::T2KSyst_t> AllIncluded (void);

private:

  map<T2KSyst_t, T2KSystInfo *>  fSystematics;  

};

class T2KSystInfo {
public:
  T2KSystInfo() : 
     CurValue(0), InitValue(0), 
     OverrideDefSigma(false), SigmaOverrideValue(1), AbsTweakDial(false)
  { 
  }
  T2KSystInfo(double init) : 
     CurValue(init), InitValue(init), 
     OverrideDefSigma(false), SigmaOverrideValue(1), AbsTweakDial(false)
  { 
  }
  T2KSystInfo(const T2KSystInfo & info){
    this->CopyFrom(info);
  }
 ~T2KSystInfo() 
  { 
  }

  void CopyFrom(const T2KSystInfo & info){
    CurValue = info.CurValue;
    InitValue = info.InitValue;
    OverrideDefSigma = info.OverrideDefSigma; 
    SigmaOverrideValue = info.SigmaOverrideValue; 
    AbsTweakDial = info.AbsTweakDial;
  }

  double CurValue;
  double InitValue;
  bool OverrideDefSigma; 
  double SigmaOverrideValue; 
  bool AbsTweakDial;
};

} // t2krew namespace

#endif 

