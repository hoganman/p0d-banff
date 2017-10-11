//____________________________________________________________________________
/*!

\class    t2krew::T2KNIWGUtils

\brief    Set of utilities to convert various inputs into NIWG native event 
          format. Possible inputs include:
            * An SK "h1" tree
            * An NRooTrackerVtx obj as defined in oaAnalysis output.
            * An entry from nRooTracker tree, the NIWG bare-ROOT format used
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

#ifndef _T2KRW_NIWG_UTILS_H_
#define _T2KRW_NIWG_UTILS_H_

#include <string>

#include "T2KBuild.h"
#include "T2KSyst.h"

#ifdef __T2KRW_NIWG_ENABLED__
#include "NIWGEvent.h"
#include "NIWGSyst.h"
#endif

#ifdef __T2KRW_OAANALYSIS_ENABLED__
#include "ND__RooTrackerVtxBase.h"
#include "ND__GRooTrackerVtx.h"
#include "ND__NRooTrackerVtx.h"
#endif

#ifdef __T2KRW_NEUT_ENABLED__
#include "neutvect.h"
#include "neutpart.h"
#include "neutfsipart.h"
#include "neutfsivert.h"
#endif

#include "SK__h1.h"

class TFile;
class TTree;
class TBranch;
class TBits;
class TObjString;

using std::string;

namespace t2krew  {

 class T2KNIWGUtils
 {
 public:
#ifdef __T2KRW_NIWG_ENABLED__
   static niwg::rew::NIWGEvent * GetNIWGEvent(TTree * intree, int entry);  
#ifdef __T2KRW_NEUT_ENABLED__
   static niwg::rew::NIWGEvent * GetNIWGEvent(NeutVect* nvect);
#endif
   static niwg::rew::NIWGEvent * GetNIWGEvent(const SK::SK__h1 * sktree);
   static niwg::rew::NIWGSyst_t  T2KtoNIWGSyst(T2KSyst_t syst);

#ifdef __T2KRW_OAANALYSIS_ENABLED__
   static niwg::rew::NIWGEvent * GetNIWGEvent   (const ND::RooTrackerVtxBase * vtx);
   static double Q2(const ND::RooTrackerVtxBase * vtx);
   static double Q2QE(const ND::RooTrackerVtxBase * vtx);
   static double Plep(const ND::RooTrackerVtxBase * vtx);
   static double q0(const ND::RooTrackerVtxBase * vtx);
   static double q3(const ND::RooTrackerVtxBase * vtx);

   static int    TgtA                (const ND::NRooTrackerVtx & evt);        ///< target mass number A
   static int    TgtA                (const ND::GRooTrackerVtx & evt);        ///< target mass number A
   static int    IdxFsl              (const ND::NRooTrackerVtx & evt);        /// Id of outgoing lepton
   static int    IdxFsl              (const ND::GRooTrackerVtx & evt);        /// Id of outgoing lepton

#endif

   static double GetPDGMass(int ID);

#endif

 };
}
#endif // _T2KRW_NIWG_UTILS_H_

