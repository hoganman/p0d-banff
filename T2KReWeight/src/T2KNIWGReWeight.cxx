//____________________________________________________________________________
/*

 For the class documentation see the corresponding header file.

*/
//____________________________________________________________________________
#include <cstdlib>
#include <iostream>

#include "T2KBuild.h"
#include "T2KNIWGReWeight.h"

#include "T2KSystSet.h"
#include "T2KNIWGUtils.h" 


using namespace t2krew;
#ifdef __T2KRW_NIWG_ENABLED__
#include "NIWGSystUncertainty.h"
using namespace niwg::rew;
#endif
using std::cout;
using std::endl;
using std::cerr;

//_______________________________________________________________________________________
T2KNIWGReWeight::T2KNIWGReWeight(std::string SF2RFG_filename) : 
  fAtLeastOneAbsTwkDial(false) 
{
#ifdef __T2KRW_NIWG_ENABLED__
  fNIWGReWeight = new NIWGReWeight(); 
  // Adopt all weight calculators.
  //fNIWGReWeight->AdoptWghtCalc( "niwg_2010a",       new NIWGReWeight2010a      );
  //fNIWGReWeight->AdoptWghtCalc( "niwg_splineenu",       new NIWGReWeightSplineEnu      );
  fNIWGReWeight->AdoptWghtCalc( "niwg_2012a",       new NIWGReWeight2012a      );
  fNIWGReWeight->AdoptWghtCalc( "niwg_2014a",       new NIWGReWeight2014a      );

  // Mark Scott - Comment out DeltaMass and SpectralFunc reweight calculators, since they are not used in the 2015 analysis and take a long time to configure
  //fNIWGReWeight->AdoptWghtCalc( "niwg_dmass",       new NIWGReWeightDeltaMass  );
  //fNIWGReWeight->AdoptWghtCalc( "niwg_1piAngle",    new NIWGReWeight1piAngle   );
  fNIWGReWeight->AdoptWghtCalc( "niwg_pimult",    new NIWGReWeightPiMult   );
  //fNIWGReWeight->AdoptWghtCalc( "niwg_spectralFunc", new NIWGReWeightSpectralFunc ); 
  fNIWGReWeight->AdoptWghtCalc( "niwg_mec",          new NIWGReWeightMEC          );
  fNIWGReWeight->AdoptWghtCalc( "niwg_rpa",          new NIWGReWeightRPA          );
  fNIWGReWeight->AdoptWghtCalc( "niwg_eff_rpa",      new NIWGReWeightEffectiveRPA );
  fNIWGReWeight->AdoptWghtCalc( "niwg_eff_rpa_norm", new NIWGReWeightEffectiveRPANorm );
  fNIWGReWeight->AdoptWghtCalc( "niwg_protonFSIbug", new NIWGReWeightProtonFSIbug ); 
  fNIWGReWeight->AdoptWghtCalc( "niwg_HadronMultSwitch", new NIWGReWeightHadronMultSwitch );
  

#else
   std::cerr << "WARNING!! Trying to use the NIWG reweighting class without "<<
                "configuring T2KReWeight with the --enable-niwg option." << std::endl;
#endif
}
//_______________________________________________________________________________________
T2KNIWGReWeight::~T2KNIWGReWeight()
{
#ifdef __T2KRW_NIWG_ENABLED__
   if(fNIWGReWeight) delete fNIWGReWeight;
#endif
}


#ifdef __T2KRW_NIWG_ENABLED__

niwg::rew::NIWGReWeightI* T2KNIWGReWeight::GetWghtCalc(string name){

  return fNIWGReWeight->WghtCalc(name);
   
}

#endif


//_______________________________________________________________________________________
void T2KNIWGReWeight::SetSystTwkDial(T2KSyst_t syst, double val)
{
#ifdef __T2KRW_NIWG_ENABLED__
   // JIMTODO - Add functionality to complain or exit if try to add non existent niwgsyst etc...

   NIWGSyst_t niwgsyst = T2KNIWGUtils::T2KtoNIWGSyst(syst);

   //if(niwgsyst == kNullSystematic){ 
   //  cout << "Cannot convert to niwg syst, exiting!" << endl;
   //  exit(1);
   //}

   bool can_handle = fNIWGReWeight->Systematics().Added(niwgsyst); 
   if(!can_handle){ 
     fNIWGReWeight->Systematics().Init(niwgsyst);
     can_handle = fNIWGReWeight->Systematics().Added(niwgsyst);
   }

   if(can_handle) fNIWGReWeight->Systematics().Set(T2KNIWGUtils::T2KtoNIWGSyst(syst), val); 
#endif
}
//_______________________________________________________________________________________
void T2KNIWGReWeight::SetSystFracErr(T2KSyst_t syst, double val, bool absolute)
{
 #ifdef __T2KRW_NIWG_ENABLED__
  if(absolute){

    fAtLeastOneAbsTwkDial = true; // CalcChisq function will be meaningless if absolute tweak dial is used for any systematics
    NIWGSyst_t niwgsyst = T2KNIWGUtils::T2KtoNIWGSyst(syst);
    bool can_handle = fNIWGReWeight->Systematics().Added(niwgsyst); 

    // Override the default values fractional error for the uncertainty with 'val'
    // val is set to 1.0 when SetAbsTweak is called
    // Variation in T2KWghtEngineI is: tweakedvalue = nominal*(1.0+fractional_error * newvalue)
    // becomes an absolute (relative) variation: tweakedvalue = nominal*(1.0+ newvalue)
    if(can_handle)     NIWGSystUncertainty::Instance()->SetUncertainty(niwgsyst, val, val);
    //cerr << "Warning: T2KNIWGReWeight absolute variations will have no effect currently. " << endl;

    
  }
#endif
}
//_______________________________________________________________________________________
void T2KNIWGReWeight::Reconfigure(void)
{
#ifdef __T2KRW_NIWG_ENABLED__
  fNIWGReWeight->Reconfigure();
#endif
}
//_______________________________________________________________________________________
void T2KNIWGReWeight::Reset(void)
{
#ifdef __T2KRW_NIWG_ENABLED__
#endif
}
//_______________________________________________________________________________________
#ifdef __T2KRW_OAANALYSIS_ENABLED__
double T2KNIWGReWeight::CalcWeight(const ND::RooTrackerVtxBase * vtx)
{
// If we make the RooTrackerVtxBase object contain the beam pass-through information 
// then as both GRooTrackerVtx and NRooTrackerVtx inherit from this the beam reweighting 
// does not care which it gets.    
  double weight = 1.0;
#ifdef __T2KRW_NIWG_ENABLED__
// Call NIWG reweighting function and reweight using information in ND::GRooTrackerVtx. 
// Either convert to some native NIWG format first (see example in T2KGenieReWeight) or
// use the RooTrackerVtxBase object directly

  niwg::rew::NIWGEvent * event = (niwg::rew::NIWGEvent *) T2KNIWGUtils::GetNIWGEvent(vtx);

  if(event){
    weight = fNIWGReWeight->CalcWeight(*event);
  }
  else {
    cout << "Unable to convert ND::RooTrackerVtx to niwg::rew::NIWGEvent. Returning weight of 1.0!" << endl;
  }
  
  if(event) delete event;
  
#endif
  fCalcedWeight = weight;
  return weight;
}
#endif
//_______________________________________________________________________________________
double T2KNIWGReWeight::CalcWeight(const SK::SK__h1 * sktree)
{
  double weight = 1.0;
#ifdef __T2KRW_NIWG_ENABLED__

  niwg::rew::NIWGEvent * event = (niwg::rew::NIWGEvent *) T2KNIWGUtils::GetNIWGEvent(sktree);

  if(event){
    weight = fNIWGReWeight->CalcWeight(*event);
  }
  else {
    cout << "Unable to convert ND::RooTrackerVtx to niwg::rew::NIWGEvent. Returning weight of 1.0!" << endl;
  }
  
  if(event) delete event;

#endif
  fCalcedWeight = weight;
  return weight;
}
//_______________________________________________________________________________________
double T2KNIWGReWeight::CalcWeight(const JnuBeamRead * jnutree)
{
  fCalcedWeight = 1;
  return 1;
}
//_______________________________________________________________________________________
double T2KNIWGReWeight::CalcWeight(TTree * intree, int entry) 
{
  double weight = 1.0;
#ifdef __T2KRW_NIWG_ENABLED__

  niwg::rew::NIWGEvent * event = (niwg::rew::NIWGEvent *) T2KNIWGUtils::GetNIWGEvent(intree,entry);

  if(event){
    weight = fNIWGReWeight->CalcWeight(*event);
  }
  else {
    cout << "Unable to convert ND::RooTrackerVtx to niwg::rew::NIWGEvent. Returning weight of 1.0!" << endl;
  }
  
  if(event) {
    delete event;
  }

#endif
  fCalcedWeight = weight;
  return weight;
}

#ifdef __T2KRW_NEUT_ENABLED__
double T2KNIWGReWeight::CalcWeight(NeutVect *nvect)
{
  double weight = 1.0;

  niwg::rew::NIWGEvent * event = (niwg::rew::NIWGEvent *) T2KNIWGUtils::GetNIWGEvent(nvect);

  if(event){
    weight = fNIWGReWeight->CalcWeight(*event);
  }
  else {
    cout << "Unable to convert ND::RooTrackerVtx to niwg::rew::NIWGEvent. Returning weight of 1.0!" << endl;
  }

  if(event) {
    delete event;
  }

  fCalcedWeight = weight;
  return weight;
}
#endif

//_______________________________________________________________________________________
#ifdef __T2KRW_NIWG_ENABLED__
double T2KNIWGReWeight::CalcWeight(niwg::rew::NIWGEvent *event)
{
  double weight = 1.0;
  if(event){
    weight = fNIWGReWeight->CalcWeight(*event);
  }
  fCalcedWeight = weight;
  return weight;
}
#endif
//_______________________________________________________________________________________
double T2KNIWGReWeight::CalcChisq()
{
  // This function needs to retrieve the chisq from the external reweighing 
  // library. If fAtLeastOneAbsTwkDial is set to true then it makes no sense to be calculating
  // the chisq so this function needs to print a warning and return 0.0  
  double chisq = 0.0;
#ifdef __T2KRW_NIWG_ENABLED__
  if(fAtLeastOneAbsTwkDial){
    cerr << "Warning: T2KNIWGReWeight::CalcChisq is meaningless if using "<<
            " absolute tweak dial for any NIWG params, returning 0!" << endl;
  }
  else {
    chisq = fNIWGReWeight->CalcChisq();
  }
#endif
  return chisq;
}
//_______________________________________________________________________________________
void T2KNIWGReWeight::Print(void)
{
#ifdef __T2KRW_NIWG_ENABLED__
#endif
}
