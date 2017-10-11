//____________________________________________________________________________
/*!

\class    T2KGEANTReWeight

\brief    Concrete implementation of T2K reweighting engine for the GEANT weight
          calculator. 

*/
//____________________________________________________________________________

#ifndef _T2K_GEANT_REWEIGHT_H_
#define _T2K_GEANT_REWEIGHT_H_

#include "T2KBuild.h"
#include "T2KWghtEngineI.h"
//
// GEANT headers
//
#ifdef __T2KRW_GEANT_ENABLED__
#include "GEANTReWeight.h"
#include "GEANTReWeightPionScndInt.h"
#endif

#ifdef __T2KRW_NEUT_ENABLED__
#include "neutvect.h"
#endif

namespace t2krew {

 class T2KGEANTReWeight : public T2KWghtEngineI 
 {
 public:
   T2KGEANTReWeight();
  ~T2KGEANTReWeight();

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

#ifdef __T2KRW_GEANT_ENABLED__
   geant::rew::GEANTReWeightI* GetWghtCalc(string name);
#endif

 private:
#ifdef __T2KRW_GEANT_ENABLED__
   geant::rew::GEANTReWeight  * fGEANTReWeight;   ///< handles all GEANT reweighting
#endif

   // TOIMPLEMENT: See how this is done in T2KGenieReWeight, please remove after implemented. 
   bool fAtLeastOneAbsTwkDial;   ///< Set to true if any systematics for this engine set to absolute tweak dial
 };

} // t2krew namespace

#endif

