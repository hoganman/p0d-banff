//____________________________________________________________________________
/*!

\class    t2krew::T2KReWeight

\brief    Interface to T2KReWeight event reweighting enginess

\author   Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
          STFC, Rutherford Appleton Laboratory

          Jim Dobson <j.dobson07 \at imperial.ac.uk>
          Imperial College London

\created  Aug 1, 2008

 @ July 4, 2011 - JD
   Update the Reocnfigure method so it correctly handles cases where the
   default 1 sigma fractional errors have been flagged as overridden in the 
   T2KSystSet.  

*/
//____________________________________________________________________________

#ifndef _T2KRW_REWEIGHT_H_
#define _T2KRW_REWEIGHT_H_

#include "T2KSystSet.h"
#include "T2KWghtEngineI.h"
#include "JnuBeamRead.h"

#ifdef __T2KRW_OAANALYSIS_ENABLED__
#include "ND__JNuBeamFlux.h"
#endif

#ifdef __T2KRW_NEUT_ENABLED__
#include "neutvect.h"
#endif

namespace t2krew {

class T2KReWeight
{
public:
   T2KReWeight();
  ~T2KReWeight();

   void          AdoptWghtEngine (string name, T2KWghtEngineI* wcalc); ///< add concrete weight calculator, transfers ownership
   T2KWghtEngineI* WghtEngine      (string name);                      ///< access a weight calculator by name
   T2KSystSet &  Systematics (void);                               ///< return current systematics list
   void          Reconfigure (void);                               ///< propergate syst changes to weight calculators
#ifdef __T2KRW_OAANALYSIS_ENABLED__
   double        CalcWeight  (const ND::RooTrackerVtxBase * event);   ///< calc weight based on rootracker vtx obj
#endif
   double        CalcWeight     (const SK::SK__h1 * sktree);  //< calc weight based on input SK tree 
   double        CalcWeight     (const JnuBeamRead * jnutree); //< calc weight based on input JnuBeam file
   double        CalcWeight  (TTree * intree, int entry);    ///< calc weight based on input tree and entry
#ifdef __T2KRW_NEUT_ENABLED__
   double        CalcWeight  (NeutVect *nvect);              /// Rather than setting addresses and finding events constantly, pass a neutvect
#endif
   double        CalcChisq   (void);                               ///< return chi2 penality for current syst set
   void          Print       (void);                             ///< print

private:

   void CleanUp (void);

   T2KSystSet               fSystSet;   ///< set of enabled nuisance parameters
   map<string, T2KWghtEngineI *> fWghtCalc;  ///< concrete weight calculators 
};

}       // namespace t2krew
#endif  // _T2KRW_REWEIGHT_H_

