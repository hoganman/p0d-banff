//____________________________________________________________________________
/*!

\class    T2KGenieReWeight

\brief    Concrete implementation of T2K reweighting engine for the GENIE weight
          calculator. This class acts as a thin wrapper for the GENIE reweighting
          libraries and just converts (using T2KGenieUtils methods as well) to 
          systematics and event format which the GENIE libraries expect. Adapted
          from previous version of T2KReWeight and the GENIE reweighting tools.  

\author   Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
          STFC, Rutherford Appleton Laboratory

          Jim Dobson <J.Dobson07 \at imperial.ac.uk>
          Imperial College London

\created  Nov 1, 2010
*/
//____________________________________________________________________________

#ifndef _T2K_GENIE_REWEIGHT_H_
#define _T2K_GENIE_REWEIGHT_H_

#include "T2KBuild.h"
#include "T2KWghtEngineI.h"

//
// GENIE headers
//
#ifdef __T2KRW_GENIE_ENABLED__
#include "ReWeight/GReWeight.h"
#include "EVGCore/EventRecord.h"
#include "ReWeight/GReWeightNuXSecNCEL.h"
#include "ReWeight/GReWeightNuXSecCCQE.h"
#include "ReWeight/GReWeightNuXSecCCRES.h"
#include "ReWeight/GReWeightNuXSecCOH.h"
#include "ReWeight/GReWeightNonResonanceBkg.h"
#include "ReWeight/GReWeightFGM.h"
#include "ReWeight/GReWeightDISNuclMod.h"
#include "ReWeight/GReWeightResonanceDecay.h"
#include "ReWeight/GReWeightFZone.h"
#include "ReWeight/GReWeightINuke.h"
#include "ReWeight/GReWeightAGKY.h"
#include "ReWeight/GReWeightNuXSecCCQEvec.h"
#include "ReWeight/GReWeightNuXSecNCRES.h"
#include "ReWeight/GReWeightNuXSecDIS.h"
#include "ReWeight/GReWeightNuXSecNC.h"
#endif

#ifdef __T2KRW_NEUT_ENABLED__
#include "neutvect.h"
#endif


namespace t2krew {

 class T2KGenieReWeight : public T2KWghtEngineI 
 {
 public:
   T2KGenieReWeight();
  ~T2KGenieReWeight();

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
#ifdef __T2KRW_GENIE_ENABLED__
   genie::rew::GReWeightI* GetWghtCalc(string name);
   genie::rew::GReWeightNuXSecCCQE* GetWghtCCQECalc(string name);
#endif
     
 private:

#ifdef __T2KRW_GENIE_ENABLED__
   genie::rew::GReWeight  * fGenieReWeight;   ///< handles all GENIE reweighting
#endif
   bool fAtLeastOneAbsTwkDial;   ///< Set to true if any GENIE systematics set to absolute tweak dial
 };

} // t2krew namespace

#endif

