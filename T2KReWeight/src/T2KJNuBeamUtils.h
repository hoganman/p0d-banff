//____________________________________________________________________________
/*!

\class    t2krew::T2KJNuBeamUtils

\brief    Set of utilities to convert various inputs into JNUBEAM native event 
          format. Possible inputs include:
            * An SK "h1" tree
            * An NRooTrackerVtx obj as defined in oaAnalysis output.
            * An entry from nRooTracker tree, the JNUBEAM bare-ROOT format used
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

#ifndef _T2KRW_JNUBEAM_UTILS_H_
#define _T2KRW_JNUBEAM_UTILS_H_

#include <string>

#include "T2KBuild.h"
#include "T2KSyst.h"

#ifdef __T2KRW_JNUBEAM_ENABLED__
#include "JEvent.h"
#include "JnuBeamRead.h"
#include "JSyst.h"
using namespace jnubeam::rew;
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

 class T2KJNuBeamUtils
 {
 public:
#ifdef __T2KRW_JNUBEAM_ENABLED__
   static jnubeam::rew::JEvent * GetJNuBeamEvent   (const SK::SK__h1 * sktree);
   static jnubeam::rew::JEvent * GetJNuBeamEvent   (const JnuBeamRead * jnutree);
   static jnubeam::rew::JSyst_t  T2KtoJSyst(T2KSyst_t syst);
   
#ifdef __T2KRW_OAANALYSIS_ENABLED__
   static jnubeam::rew::JEvent * GetJNuBeamEvent   (const ND::RooTrackerVtxBase * vtx);


#endif

#endif
 };
}
#endif // _T2KRW_JNUBEAM_UTILS_H_

