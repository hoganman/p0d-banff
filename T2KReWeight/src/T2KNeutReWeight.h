//____________________________________________________________________________
/*!

\class    T2KNeutReWeight

\brief    Concrete implementation of T2K reweighting engine for the NEUT weight
          calculator. 

*/
//____________________________________________________________________________

#ifndef _T2K_NEUT_REWEIGHT_H_
#define _T2K_NEUT_REWEIGHT_H_

#include "T2KBuild.h"
#include "T2KWghtEngineI.h"

//
// NEUT headers
//
#ifdef __T2KRW_NEUT_ENABLED__
#include "NReWeight.h"
#include "NReWeightNuXSecCCQE.h"
#include "NReWeightNuXSecRES.h"
#include "NReWeightNuXSecCCRES.h"
#include "NReWeightNuXSecCOH.h"
#include "NReWeightNuXSecDIS.h"
#include "NReWeightNuXSecNCEL.h"
#include "NReWeightNuXSecNC.h"
#include "NReWeightNuXSecNCRES.h"
#include "NReWeightCasc.h"
//Comment out for time being as current NEUT version does not have proton FSI information
//#include "NReWeightINuke.h"
#include "NReWeightNuclPiless.h"
#include "neutvect.h"
#endif


namespace t2krew {

 class T2KNeutReWeight : public T2KWghtEngineI 
 {
 public:
   T2KNeutReWeight();
  ~T2KNeutReWeight();

   // implement the T2KWghtEngineI interface
   void   SetSystTwkDial (T2KSyst_t syst, double val);
   void   SetSystFracErr (T2KSyst_t syst, double val, bool absolute = false); 
   void   Reset          (void);
   void   Reconfigure    (void);
#ifdef __T2KRW_OAANALYSIS_ENABLED__
   double CalcWeight     (const ND::RooTrackerVtxBase * vtx);
#endif
   double CalcWeight     (const SK::SK__h1 * sktree);
   double CalcWeight     (TTree * intree, int entry);
#ifdef __T2KRW_NEUT_ENABLED__
   double CalcWeight     (NeutVect *nvect);
#endif
   double CalcWeight     (const JnuBeamRead * jnutree);
   double CalcChisq      (void);
   void Print(void);
#ifdef __T2KRW_NEUT_ENABLED__
   neut::rew::NReWeightI* GetWghtCalc(string name);
   //neut::rew::NReWeightNuXSecCCQE* GetWghtCCQECalc(string name);
   //neut::rew::NReWeightCasc* GetWghtCascCalc(string name);
   double GetWeightXsec  ();
     
#endif
 
 private:

#ifdef __T2KRW_NEUT_ENABLED__
   neut::rew::NReWeight  * fNeutReWeight;   ///< handles all NEUT reweighting
#endif
   // TOIMPLEMENT: See how this is done in T2KGenieReWeight, please remove after implemented. 
   bool fAtLeastOneAbsTwkDial;   ///< Set to true if any systematics for this engine set to absolute tweak dial
 };

} // t2krew namespace

#endif

