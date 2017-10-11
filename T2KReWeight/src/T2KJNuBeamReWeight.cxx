//____________________________________________________________________________
/*

 For the class documentation see the corresponding header file.

*/
//____________________________________________________________________________
#include <cstdlib>
#include <iostream>

#include "T2KBuild.h"
#include "T2KJNuBeamReWeight.h"

#include "T2KSystSet.h"
#include "T2KJNuBeamUtils.h" 


using namespace t2krew;
#ifdef __T2KRW_JNUBEAM_ENABLED__
using namespace jnubeam::rew;
#include "JSystUncertainty.h"
#endif
using std::cout;
using std::endl;
using std::cerr;

//_______________________________________________________________________________________
T2KJNuBeamReWeight::T2KJNuBeamReWeight() : 
  fAtLeastOneAbsTwkDial(false) 
{
#ifdef __T2KRW_JNUBEAM_ENABLED__
  fJNuBeamReWeight = new JReWeight(); 
  // Adopt all weight calculators.
  //fJNuBeamReWeight->AdoptWghtCalc( "na61_pion",       new JReWeightNA61pion      );
  //fJNuBeamReWeight->AdoptWghtCalc( "bmpt_kaon",       new JReWeightKaonBMPT      );
  //fJNuBeamReWeight->AdoptWghtCalc( "tot_xsec",        new JReWeightTotXsec       );
  //fJNuBeamReWeight->AdoptWghtCalc( "p_beam",        new JReWeightPbeam       ); 
  //fJNuBeamReWeight->AdoptWghtCalc( "enu2012a",        new JReWeightEnu2012a       ); 
  fJNuBeamReWeight->AdoptWghtCalc( "enu2013a",        new JReWeightEnu2013a       ); 

#else
   std::cerr << "WARNING!! Trying to use the JNUBEAM reweighting class without "<<
                "configuring T2KReWeight with the --enable-jnubeam option." << std::endl;
#endif
}
//_______________________________________________________________________________________
T2KJNuBeamReWeight::~T2KJNuBeamReWeight()
{
#ifdef __T2KRW_JNUBEAM_ENABLED__
   if(fJNuBeamReWeight) delete fJNuBeamReWeight;
#endif
}


#ifdef __T2KRW_JNUBEAM_ENABLED__

jnubeam::rew::JReWeightI* T2KJNuBeamReWeight::GetWghtCalc(string name){

  return fJNuBeamReWeight->WghtCalc(name);
   
}

#endif


//_______________________________________________________________________________________
void T2KJNuBeamReWeight::SetSystTwkDial(T2KSyst_t syst, double val)
{
#ifdef __T2KRW_JNUBEAM_ENABLED__
   // JIMTODO - Add functionality to complain or exit if try to add non existent jsyst etc...

   JSyst_t jsyst = T2KJNuBeamUtils::T2KtoJSyst(syst);
   //if(jsyst == kNullSystematic){ 
   //  cout << "Cannot convert to jnubeam syst, exiting!" << endl;
   //  exit(1);
   //}

   bool can_handle = fJNuBeamReWeight->Systematics().Added(jsyst); 
   if(!can_handle){ 
     fJNuBeamReWeight->Systematics().Init(jsyst);
     can_handle = fJNuBeamReWeight->Systematics().Added(jsyst);
   }

   if(can_handle) fJNuBeamReWeight->Systematics().Set(T2KJNuBeamUtils::T2KtoJSyst(syst), val); 
#endif
}
//_______________________________________________________________________________________
void T2KJNuBeamReWeight::SetSystFracErr(T2KSyst_t syst, double val, bool absolute)
{
 #ifdef __T2KRW_JNUBEAM_ENABLED__
  if(absolute){

    fAtLeastOneAbsTwkDial = true; // CalcChisq function will be meaningless if absolute tweak dial is used for any systematics
    JSyst_t jsyst = T2KJNuBeamUtils::T2KtoJSyst(syst);   
    bool can_handle = fJNuBeamReWeight->Systematics().Added(jsyst); 

    // Override the default values fractional error for the uncertainty with 'val'
    // val is set to 1.0 when SetAbsTweak is called
    // Variation in T2KWghtEngineI is: tweakedvalue = nominal*(1.0+fractional_error * newvalue)
    // becomes an absolute (relative) variation: tweakedvalue = nominal*(1.0+ newvalue)
    if(can_handle)     JSystUncertainty::Instance()->SetUncertainty(jsyst, val, val);
    
  }
#endif
}
//_______________________________________________________________________________________
void T2KJNuBeamReWeight::Reconfigure(void)
{
#ifdef __T2KRW_JNUBEAM_ENABLED__
  fJNuBeamReWeight->Reconfigure();
#endif
}
//_______________________________________________________________________________________
void T2KJNuBeamReWeight::Reset(void)
{
#ifdef __T2KRW_JNUBEAM_ENABLED__
#endif
}
//_______________________________________________________________________________________
#ifdef __T2KRW_OAANALYSIS_ENABLED__
double T2KJNuBeamReWeight::CalcWeight(const ND::RooTrackerVtxBase * vtx)
{
// If we make the RooTrackerVtxBase object contain the beam pass-through information 
// then as both GRooTrackerVtx and NRooTrackerVtx inherit from this the beam reweighting 
// does not care which it gets.    
  double weight = 1.0;
#ifdef __T2KRW_JNUBEAM_ENABLED__
// Call JNuBeam reweighting function and reweight using information in ND::GRooTrackerVtx. 
// Either convert to some native JNuBeam format first (see example in T2KGenieReWeight) or
// use the RooTrackerVtxBase object directly

  jnubeam::rew::JEvent * event = (jnubeam::rew::JEvent *) T2KJNuBeamUtils::GetJNuBeamEvent(vtx);

  if(event){
    weight = fJNuBeamReWeight->CalcWeight(*event);
  }
  else {
    cout << "Unable to convert ND::RooTrackerVtx to jnubeam::rew::JEvent. Returning weight of 1.0!" << endl;
  }
  
  if(event) delete event;
  
#endif
  return weight;
}
#endif
//_______________________________________________________________________________________
double T2KJNuBeamReWeight::CalcWeight(const SK::SK__h1 * sktree)
{
  double weight = 1.0;
#ifdef __T2KRW_JNUBEAM_ENABLED__

  jnubeam::rew::JEvent * event = (jnubeam::rew::JEvent *) T2KJNuBeamUtils::GetJNuBeamEvent(sktree);

  if(event){
    weight = fJNuBeamReWeight->CalcWeight(*event);
  }
  else {
    cout << "Unable to convert ND::RooTrackerVtx to jnubeam::rew::JEvent. Returning weight of 1.0!" << endl;
  }
  
  if(event) delete event;

#endif
  return weight;
}
//_______________________________________________________________________________________
double T2KJNuBeamReWeight::CalcWeight(const JnuBeamRead * jnutree)
{
  double weight = 1.0;
#ifdef __T2KRW_JNUBEAM_ENABLED__

  jnubeam::rew::JEvent * event = (jnubeam::rew::JEvent *) T2KJNuBeamUtils::GetJNuBeamEvent(jnutree);

  if(event){
    weight = fJNuBeamReWeight->CalcWeight(*event);
  }
  else {
    cout << "Unable to convert ND::RooTrackerVtx to jnubeam::rew::JEvent. Returning weight of 1.0!" << endl;
  }
  
  if(event) delete event;

#endif
  return weight;
}
//_______________________________________________________________________________________
double T2KJNuBeamReWeight::CalcWeight(TTree * intree, int entry) 
{
  double weight = 1.0;
#ifdef __T2KRW_JNUBEAM_ENABLED__
// Call JNuBeam reweighting function and reweight using information entry in intree-> 
// Either convert to some native JNuBeam format (see example in T2KGenieReWeight) or
// use the information from the intree directly.
#endif
  return weight;
}

#ifdef __T2KRW_NEUT_ENABLED__
double T2KJNuBeamReWeight::CalcWeight(NeutVect *nvect)
{
  double weight = 1.0;
  return weight;
}
#endif

//_______________________________________________________________________________________
double T2KJNuBeamReWeight::CalcChisq()
{
  // TOIMPLEMENT: This function needs to retrieve the chisq from the external reweighing 
  // library. If fAtLeastOneAbsTwkDial is set to true then it makes no sense to be calculating
  // the chisq so this function needs to print a warning and return 0.0  
  double chisq = 0.0;
#ifdef __T2KRW_JNUBEAM_ENABLED__
  if(fAtLeastOneAbsTwkDial){
    std::cerr << "Warning: T2KJnuBeamReWeight::CalcChisq is meaningless if using "<<
            " absolute tweak dial for any JnuBeam params, returning 0!" << endl;
  }
  else {
    chisq = fJNuBeamReWeight->CalcChisq();
  }
#endif
  return chisq;
}
//_______________________________________________________________________________________
void T2KJNuBeamReWeight::Print(void)
{
#ifdef __T2KRW_JNUBEAM_ENABLED__
#endif
}
//_______________________________________________________________________________________
void T2KJNuBeamReWeight::MakeThrows(int nthrows, int seed)
{
#ifdef __T2KRW_JNUBEAM_ENABLED__
  fJNuBeamReWeight->MakeThrows(nthrows,seed);
#endif
}

void T2KJNuBeamReWeight::UseParameterSet(int nset)
{
#ifdef __T2KRW_JNUBEAM_ENABLED__
  fJNuBeamReWeight->UseParameterSet(nset);
#endif
}

#ifdef __T2KRW_JNUBEAM_ENABLED__
void T2KJNuBeamReWeight::SetRunPeriod(jnubeam::rew::runPeriod_t period){
   fJNuBeamReWeight->SetRunPeriod(period);
}
#endif

