//____________________________________________________________________________
/*!

\class    t2krew::T2KSKUtils

\brief    Set of utilities to convert various inputs into NEUT native event 
          format. Possible inputs include:
            * An SK "h1" tree
            * An NRooTrackerVtx obj as defined in oaAnalysis output.
            * An entry from nRooTracker tree, the NEUT bare-ROOT format used
              by ND280Software (not yet implemented)

\author   Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
          STFC, Rutherford Appleton Laboratory

          Jim Dobson <J.Dobson07 \at imperial.ac.uk>
          Imperial College London

	  Patrick de Perio <pdeperio \at physics.utoronto.ca>
	  University of Toronto

\created  Aug. 30, 2011
*/
//____________________________________________________________________________

#ifndef _T2KRW_SK_UTILS_H_
#define _T2KRW_SK_UTILS_H_

#include <string>

#include "TTree.h"

#include "T2KBuild.h"
#include "T2KSyst.h"

#ifdef __T2KRW_NEUT_ENABLED__
#include "NSyst.h"
#include "NFortFns.h" // Contains all the NEUT common blocks
#endif

#include "SK__h1.h"

class TFile;
class TTree;
class TBranch;
class TBits;
class TObjString;

using std::string;

namespace t2krew  {


 class T2KSKUtils
 {
 public:
#ifdef __T2KRW_NEUT_ENABLED__
   static void   fill_neut_commons   (const SK::SK__h1 * sktree);

   static neut::rew::NSyst_t  T2KtoNSyst(T2KSyst_t syst);
#endif
 };
}
#endif // _T2KRW_SK_UTILS_H_

