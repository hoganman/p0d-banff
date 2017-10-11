//____________________________________________________________________________
/*!

\class    t2krew::T2KGEANTUtils

\brief    Set of utilities to convert various inputs into GEANT native event 
          format. Possible inputs include:
            * An SK "h1" tree
            * An NRooTrackerVtx obj as defined in oaAnalysis output.
            * An entry from nRooTracker tree, the GEANT bare-ROOT format used
              by ND280Software (not yet implemented)

\author   Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
          STFC, Rutherford Appleton Laboratory

          Jim Dobson <J.Dobson07 \at imperial.ac.uk>
          Imperial College London

	  Patrick de Perio <pdeperio \at physics.utoronto.ca>
	  University of Toronto

\created  Apr. 1, 2011
*/
//____________________________________________________________________________

#ifndef _T2KRW_GEANT_UTILS_H_
#define _T2KRW_GEANT_UTILS_H_

#include <string>

#include "T2KBuild.h"
#include "T2KSyst.h"

#ifdef __T2KRW_GEANT_ENABLED__
#include "GEANTEvent.h"
#include "GEANTSyst.h"
#endif

#ifdef __T2KRW_OAANALYSIS_ENABLED__
#include "ND__RooTrackerVtxBase.h"
#include "ND__GRooTrackerVtx.h"
#include "ND__NRooTrackerVtx.h"
#endif

#include "SK__h1.h"

class TFile;
class TTree;
class TBranch;
class TBits;
class TObjString;

using std::string;

namespace t2krew  {

 class T2KGEANTUtils
 {
 public:
#ifdef __T2KRW_GEANT_ENABLED__
   static geant::rew::GEANTEvent * GetGEANTEvent   (TTree * intree, int entry);  
   static geant::rew::GEANTEvent * GetGEANTEvent   (const SK::SK__h1 * sktree);
   static geant::rew::GEANTSyst_t  T2KtoGEANTSyst(T2KSyst_t syst);
#ifdef __T2KRW_OAANALYSIS_ENABLED__
   static geant::rew::GEANTEvent * GetGEANTEvent   (const ND::RooTrackerVtxBase * vtx);

   //static int    TgtA                (const ND::NRooTrackerVtx & evt);        ///< target mass number A
   //static int    TgtA                (const ND::GRooTrackerVtx & evt);        ///< target mass number A

#endif

#endif

 };
}
#endif // _T2KRW_GEANT_UTILS_H_

