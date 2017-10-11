//____________________________________________________________________________
/*!

\class    T2KNIWGReWeight

\brief    Concrete implementation of T2K reweighting engine for the NIWG weight
          calculator. 

*/
//____________________________________________________________________________

#ifndef _T2K_NIWG_REWEIGHT_H_
#define _T2K_NIWG_REWEIGHT_H_

#include <string>
#include "T2KBuild.h"
#include "T2KWghtEngineI.h"
//
// NIWG headers
//
#ifdef __T2KRW_NIWG_ENABLED__
#include "NIWGReWeight.h"
#include "NIWGReWeight2012a.h"
#include "NIWGReWeightDeltaMass.h"
#include "NIWGReWeight1piAngle.h"
#include "NIWGReWeightSplineEnu.h"
#include "NIWGReWeight2010a.h"
#include "NIWGReWeightPiMult.h"
#include "NIWGReWeightSpectralFunc.h"
#include "NIWGReWeightMEC.h"
#include "NIWGReWeightRPA.h"
#include "NIWGReWeightEffectiveRPA.h"
#include "NIWGReWeightEffectiveRPANorm.h"
#include "NIWGReWeight2014a.h"
#include "NIWGReWeightProtonFSIbug.h"
#include "NIWGReWeightHadronMultSwitch.h"
#endif

#ifdef __T2KRW_NEUT_ENABLED__
#include "neutvect.h"
#endif

namespace t2krew {

 class T2KNIWGReWeight : public T2KWghtEngineI 
 {
 public:
   explicit T2KNIWGReWeight(std::string SF2RFG_filename="SFToRFGReWeightingTables_pF209MeV.root");
  ~T2KNIWGReWeight();

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
#ifdef __T2KRW_NIWG_ENABLED__
   double CalcWeight     (niwg::rew::NIWGEvent *event);
#endif
   double CalcChisq      (void);
   void Print(void);

#ifdef __T2KRW_NIWG_ENABLED__
   niwg::rew::NIWGReWeightI* GetWghtCalc(string name);
#endif

   double GetWeightXsec() {return fCalcedWeight;}


 private:
#ifdef __T2KRW_NIWG_ENABLED__
   niwg::rew::NIWGReWeight  * fNIWGReWeight;   ///< handles all NIWG reweighting
#endif

   double fCalcedWeight;

   // TOIMPLEMENT: See how this is done in T2KGenieReWeight, please remove after implemented. 
   bool fAtLeastOneAbsTwkDial;   ///< Set to true if any systematics for this engine set to absolute tweak dial
 };

} // t2krew namespace

#endif

