//____________________________________________________________________________
/*

 For the class documentation see the corresponding header file.

*/
//____________________________________________________________________________
#include <cstdlib>
#include <iostream>

#include "T2KBuild.h"
#include "T2KGEANTReWeight.h"

#include "T2KSystSet.h"
#include "T2KGEANTUtils.h" 


using namespace t2krew;
#ifdef __T2KRW_GEANT_ENABLED__
#include "GEANTSystUncertainty.h"
using namespace geant::rew;
#endif
using std::cout;
using std::endl;
using std::cerr;

//_______________________________________________________________________________________
T2KGEANTReWeight::T2KGEANTReWeight() : 
  fAtLeastOneAbsTwkDial(false) 
{
#ifdef __T2KRW_GEANT_ENABLED__
  fGEANTReWeight = new GEANTReWeight(); 
  // Adopt all weight calculators.
  fGEANTReWeight->AdoptWghtCalc( "geant_pion",       new GEANTReWeightPionScndInt      );


#else
   std::cerr << "WARNING!! Trying to use the GEANT reweighting class without "<<
                "configuring T2KReWeight with the --enable-geant option." << std::endl;
#endif
}
//_______________________________________________________________________________________
T2KGEANTReWeight::~T2KGEANTReWeight()
{
#ifdef __T2KRW_GEANT_ENABLED__
   if(fGEANTReWeight) delete fGEANTReWeight;
#endif
}


#ifdef __T2KRW_GEANT_ENABLED__

geant::rew::GEANTReWeightI* T2KGEANTReWeight::GetWghtCalc(string name){

  return fGEANTReWeight->WghtCalc(name);
   
}

#endif


//_______________________________________________________________________________________
void T2KGEANTReWeight::SetSystTwkDial(T2KSyst_t syst, double val)
{
#ifdef __T2KRW_GEANT_ENABLED__
   // JIMTODO - Add functionality to complain or exit if try to add non existent geantsyst etc...

   GEANTSyst_t geantsyst = T2KGEANTUtils::T2KtoGEANTSyst(syst);

   //if(geantsyst == kNullSystematic){ 
   //  cout << "Cannot convert to geant syst, exiting!" << endl;
   //  exit(1);
   //}

   bool can_handle = fGEANTReWeight->Systematics().Added(geantsyst); 
   if(!can_handle){ 
     fGEANTReWeight->Systematics().Init(geantsyst);
     can_handle = fGEANTReWeight->Systematics().Added(geantsyst);
   }

   if(can_handle) fGEANTReWeight->Systematics().Set(T2KGEANTUtils::T2KtoGEANTSyst(syst), val); 
#endif
}
//_______________________________________________________________________________________
void T2KGEANTReWeight::SetSystFracErr(T2KSyst_t syst, double val, bool absolute)
{
 #ifdef __T2KRW_GEANT_ENABLED__
  if(absolute){

    fAtLeastOneAbsTwkDial = true; // CalcChisq function will be meaningless if absolute tweak dial is used for any systematics
    GEANTSyst_t geantsyst = T2KGEANTUtils::T2KtoGEANTSyst(syst);
    bool can_handle = fGEANTReWeight->Systematics().Added(geantsyst); 

    // Override the default values fractional error for the uncertainty with 'val'
    // val is set to 1.0 when SetAbsTweak is called
    // Variation in T2KWghtEngineI is: tweakedvalue = nominal*(1.0+fractional_error * newvalue)
    // becomes an absolute (relative) variation: tweakedvalue = nominal*(1.0+ newvalue)
    if(can_handle)     GEANTSystUncertainty::Instance()->SetUncertainty(geantsyst, val, val);
    //cerr << "Warning: T2KGEANTReWeight absolute variations will have no effect currently. " << endl;

    
  }
#endif
}
//_______________________________________________________________________________________
void T2KGEANTReWeight::Reconfigure(void)
{
#ifdef __T2KRW_GEANT_ENABLED__
  fGEANTReWeight->Reconfigure();
#endif
}
//_______________________________________________________________________________________
void T2KGEANTReWeight::Reset(void)
{
#ifdef __T2KRW_GEANT_ENABLED__
#endif
}
//_______________________________________________________________________________________
#ifdef __T2KRW_OAANALYSIS_ENABLED__
double T2KGEANTReWeight::CalcWeight(const ND::RooTrackerVtxBase * vtx)
{
// If we make the RooTrackerVtxBase object contain the beam pass-through information 
// then as both GRooTrackerVtx and NRooTrackerVtx inherit from this the beam reweighting 
// does not care which it gets.    
  double weight = 1.0;
#ifdef __T2KRW_GEANT_ENABLED__
// Call GEANT reweighting function and reweight using information in ND::GRooTrackerVtx. 
// Either convert to some native GEANT format first (see example in T2KGenieReWeight) or
// use the RooTrackerVtxBase object directly

  geant::rew::GEANTEvent * event = (geant::rew::GEANTEvent *) T2KGEANTUtils::GetGEANTEvent(vtx);

  if(event){
    weight = fGEANTReWeight->CalcWeight(*event);
  }
  else {
    cout << "Unable to convert ND::RooTrackerVtx to geant::rew::GEANTEvent. Returning weight of 1.0!" << endl;
  }
  
  if(event) delete event;
  
#endif

  return weight;
}
#endif
//_______________________________________________________________________________________
double T2KGEANTReWeight::CalcWeight(const SK::SK__h1 * sktree)
{
  double weight = 1.0;
#ifdef __T2KRW_GEANT_ENABLED__

  geant::rew::GEANTEvent * event = (geant::rew::GEANTEvent *) T2KGEANTUtils::GetGEANTEvent(sktree);

  if(event){
    weight = fGEANTReWeight->CalcWeight(*event);
  }
  else {
    cout << "Unable to convert ND::RooTrackerVtx to geant::rew::GEANTEvent. Returning weight of 1.0!" << endl;
  }
  
  if(event) delete event;

#endif

  return weight;
}
//_______________________________________________________________________________________
double T2KGEANTReWeight::CalcWeight(const JnuBeamRead * jnutree)
{

  return 1;
}
//_______________________________________________________________________________________
double T2KGEANTReWeight::CalcWeight(TTree * intree, int entry) 
{
  double weight = 1.0;
#ifdef __T2KRW_GEANT_ENABLED__

  geant::rew::GEANTEvent * event = (geant::rew::GEANTEvent *) T2KGEANTUtils::GetGEANTEvent(intree,entry);

  if(event){
    weight = fGEANTReWeight->CalcWeight(*event);
  }
  else {
    cout << "Unable to convert ND::RooTrackerVtx to geant::rew::GEANTEvent. Returning weight of 1.0!" << endl;
  }
  
  if(event) {
    delete event;
  }

#endif

  return weight;
}

#ifdef __T2KRW_NEUT_ENABLED__
double T2KGEANTReWeight::CalcWeight     (NeutVect *nvect){
  return 1.0;
}
#endif

//_______________________________________________________________________________________
double T2KGEANTReWeight::CalcChisq()
{
  // This function needs to retrieve the chisq from the external reweighing 
  // library. If fAtLeastOneAbsTwkDial is set to true then it makes no sense to be calculating
  // the chisq so this function needs to print a warning and return 0.0  
  double chisq = 0.0;
#ifdef __T2KRW_GEANT_ENABLED__
  if(fAtLeastOneAbsTwkDial){
    cerr << "Warning: T2KGEANTReWeight::CalcChisq is meaningless if using "<<
            " absolute tweak dial for any GEANT params, returning 0!" << endl;
  }
  else {
    chisq = fGEANTReWeight->CalcChisq();
  }
#endif
  return chisq;
}
//_______________________________________________________________________________________
void T2KGEANTReWeight::Print(void)
{
#ifdef __T2KRW_GEANT_ENABLED__
#endif
}
