//____________________________________________________________________________
/*

 For the class documentation see the corresponding header file.

*/
//____________________________________________________________________________
#include <cstdlib>
#include <iostream>

#include "T2KBuild.h"
#include "T2KSKReWeight.h"
#include "T2KSystSet.h"
#include "T2KSKUtils.h" 

using namespace t2krew;
#ifdef __T2KRW_NEUT_ENABLED__
#include "NSystUncertainty.h"
using namespace neut::rew;
#endif
using std::cout;
using std::endl;
using std::cerr;

//_______________________________________________________________________________________
T2KSKReWeight::T2KSKReWeight() : 
  fAtLeastOneAbsTwkDial(false) 
{
#ifdef __T2KRW_NEUT_ENABLED__
  
  fNeutReWeight = new NReWeight(); 
  // Adopt all weight calculators.
  fNeutReWeight->AdoptWghtCalc( "nucl_casc",      new NReWeightCasc          );

#else
   std::cerr << "WARNING!! Trying to use the NEUT reweighting class without "<<
                "configuring T2KReWeight with the --enable-neut option." << std::endl;
#endif
}
//_______________________________________________________________________________________
T2KSKReWeight::~T2KSKReWeight()
{
#ifdef __T2KRW_NEUT_ENABLED__
  if(fNeutReWeight) delete fNeutReWeight;
#endif
}
//_______________________________________________________________________________________
void T2KSKReWeight::SetSystTwkDial(T2KSyst_t syst, double val)
{
#ifdef __T2KRW_NEUT_ENABLED__
  // JIMTODO - Add functionality to complain or exit if try to add non existent nsyst etc...

   NSyst_t nsyst = T2KSKUtils::T2KtoNSyst(syst);
   //if(nsyst == kNullSystematic){ 
   //cout << "Cannot convert to neut syst, exiting!" << endl;
   //exit(1);
   //}

   bool can_handle = fNeutReWeight->Systematics().Added(nsyst); 
  
   if(!can_handle){ 
     fNeutReWeight->Systematics().Init(nsyst);
     can_handle = fNeutReWeight->Systematics().Added(nsyst);
   }

   if(can_handle) fNeutReWeight->Systematics().Set(T2KSKUtils::T2KtoNSyst(syst), val); 
#endif
}
void T2KSKReWeight::SetSystFracErr(T2KSyst_t syst, double val, bool absolute)
{

#ifdef __T2KRW_NEUT_ENABLED__
  if(absolute){
    
    fAtLeastOneAbsTwkDial = true; // CalcChisq function will be meaningless if absolute tweak dial is used for any systematics
    
    NSyst_t nsyst = T2KSKUtils::T2KtoNSyst(syst);
    
    bool can_handle = fNeutReWeight->Systematics().Added(nsyst); 
    // Override the default values fractional error for the uncertainty with 'val'
    // val is set to 1.0 when SetAbsTweak is called
    // Variation in T2KWghtEngineI is: tweakedvalue = nominal*(1.0+fractional_error * newvalue)
    // becomes an absolute (relative) variation: tweakedvalue = nominal*(1.0+ newvalue)
  
    if(can_handle){    
      NSystUncertainty::Instance()->SetUncertainty(nsyst, val, val);
    }
  }
#endif

}
//_______________________________________________________________________________________
void T2KSKReWeight::Reconfigure(void)
{
#ifdef __T2KRW_NEUT_ENABLED__
  fNeutReWeight->Reconfigure();
#endif
}
//_______________________________________________________________________________________
void T2KSKReWeight::Reset(void)
{
}
//_______________________________________________________________________________________
#ifdef __T2KRW_OAANALYSIS_ENABLED__
double T2KSKReWeight::CalcWeight(const ND::RooTrackerVtxBase * vtx)
{
  double weight = 1.0;

  return weight;
}
#endif
//_______________________________________________________________________________________
double T2KSKReWeight::CalcWeight(const SK::SK__h1 * sktree)
{
  double weight = 1.0;
#ifdef __T2KRW_NEUT_ENABLED__

  T2KSKUtils::fill_neut_commons(sktree);

  weight = fNeutReWeight->CalcWeight();

#endif
  return weight;
}

//_______________________________________________________________________________________
double T2KSKReWeight::CalcWeight(const JnuBeamRead * jnutree)
{
  double weight = 1.0;

  return weight;
}
//_______________________________________________________________________________________
double T2KSKReWeight::CalcWeight(TTree * intree, int entry) 
{
  double weight = 1.0;

  return weight;
}

#ifdef __T2KRW_NEUT_ENABLED__
double T2KSKReWeight::CalcWeight(NeutVect *nvect)
{
  double weight = 1.0;

  return weight;
}
#endif

//_______________________________________________________________________________________
double T2KSKReWeight::CalcChisq()
{
  // This function needs to retrieve the chisq from the external reweighing 
  // library. If fAtLeastOneAbsTwkDial is set to true then it makes no sense to be calculating
  // the chisq so this function needs to print a warning and return 0.0  
  double chisq = 0.0;
#ifdef __T2KRW_NEUT_ENABLED__
  if(fAtLeastOneAbsTwkDial){
    cerr << "Warning: T2KSKReWeight::CalcChisq is meaningless if using "<<
            " absolute tweak dial for any Neut params, returning 0!" << endl;
  }
  else {
    chisq = fNeutReWeight->CalcChisq();
  }

#endif
  return chisq;
}
//_______________________________________________________________________________________
void T2KSKReWeight::Print(void)
{
#ifdef __T2KRW_NEUT_ENABLED__
  fNeutReWeight->Print();
#endif
}
//_______________________________________________________________________________________
