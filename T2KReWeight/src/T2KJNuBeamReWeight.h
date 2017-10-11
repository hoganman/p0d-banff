//____________________________________________________________________________
/*!

\class    T2KJNuBeamReWeight

\brief    Concrete implementation of T2K reweighting engine for the JNUBEAM weight
          calculator. 

*/
//____________________________________________________________________________

#ifndef _T2K_JNUBEAM_REWEIGHT_H_
#define _T2K_JNUBEAM_REWEIGHT_H_

#include "T2KBuild.h"
#include "T2KWghtEngineI.h"
#include "JnuBeamRead.h"
//
// JNUBEAM headers
//
#ifdef __T2KRW_JNUBEAM_ENABLED__
#include "JReWeight.h"
#include "JReWeightI.h"
#include "JEvent.h"
#include "JReWeightNA61pion.h"
#include "JReWeightKaonBMPT.h"
#include "JReWeightTotXsec.h"
#include "JReWeightPbeam.h"
#include "JReWeightEnu2012a.h"
#include "JReWeightEnu2013a.h"
#endif

#ifdef __T2KRW_NEUT_ENABLED__
#include "neutvect.h"
#endif

namespace t2krew {

 class T2KJNuBeamReWeight : public T2KWghtEngineI 
 {
 public:
   T2KJNuBeamReWeight();
  ~T2KJNuBeamReWeight();

   // implement the T2KWghtEngineI interface
   void   SetSystTwkDial (T2KSyst_t syst, double val);
   void   SetSystFracErr (T2KSyst_t syst, double val, bool absolute = false); 
   void   Reset          (void);
   void   Reconfigure    (void);
#ifdef __T2KRW_OAANALYSIS_ENABLED__
   double CalcWeight     (const ND::RooTrackerVtxBase * vtx);
#endif
   double CalcWeight     (const SK::SK__h1 * sktree);
   double CalcWeight     (const JnuBeamRead * jnutree);
   double CalcWeight     (TTree * intree, int entry);
#ifdef __T2KRW_NEUT_ENABLED__
   double CalcWeight     (NeutVect *nvect);
#endif
   double CalcChisq      (void);
   void Print(void);
   void MakeThrows(int nthrows, int seed);
   //void UseParameterSet(int nset, std::vector<double> &thrwvec);
   void UseParameterSet(int nset);

#ifdef __T2KRW_JNUBEAM_ENABLED__
   void SetRunPeriod(jnubeam::rew::runPeriod_t period);
   jnubeam::rew::JReWeightI* GetWghtCalc(string name);
#endif

 private:
#ifdef __T2KRW_JNUBEAM_ENABLED__
   jnubeam::rew::JReWeight  * fJNuBeamReWeight;   ///< handles all JNuBeam reweighting
#endif
   // TOIMPLEMENT: See how this is done in T2KGenieReWeight, please remove after implemented. 
   bool fAtLeastOneAbsTwkDial;   ///< Set to true if any systematics for this engine set to absolute tweak dial
 };

} // t2krew namespace

#endif

