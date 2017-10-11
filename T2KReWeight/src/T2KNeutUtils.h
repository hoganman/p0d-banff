//____________________________________________________________________________
/*!

\class    t2krew::T2KNeutUtils

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

\created  Apr. 1, 2011
*/
//____________________________________________________________________________

#ifndef _T2KRW_NEUT_UTILS_H_
#define _T2KRW_NEUT_UTILS_H_

#include <string>

#include "TTree.h"

#include "T2KBuild.h"
#include "T2KSyst.h"

#ifdef __T2KRW_NEUT_ENABLED__
#include "NSyst.h"
#include "neutvect.h"
#endif

#ifdef __T2KRW_OAANALYSIS_ENABLED__
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

  const int kNStdHepNPmax = 100;
  const int kNStdHepIdxPx = 0;
  const int kNStdHepIdxPy = 1;
  const int kNStdHepIdxPz = 2;
  const int kNStdHepIdxE  = 3;
  const int kNStdHepIdxX  = 0;
  const int kNStdHepIdxY  = 1;
  const int kNStdHepIdxZ  = 2;
  const int kNStdHepIdxT  = 3;

 class T2KNeutUtils
 {
 public:
#ifdef __T2KRW_NEUT_ENABLED__
 private:
   static void   fill_neut_commons_with_defaults();
 public:
   static void   fill_neut_commons   (const SK::SK__h1 * sktree);
   static void   fill_neut_commons   (TTree * intree, int entry);
   static void   fill_neut_commons   (NeutVect *nvect);

   static neut::rew::NSyst_t  T2KtoNSyst(T2KSyst_t syst);
   
#ifdef __T2KRW_OAANALYSIS_ENABLED__
   static void   fill_neut_commons   (const ND::NRooTrackerVtx * vtx);
   
   static int    PdgNu               (const ND::NRooTrackerVtx & evt);        ///< initial state neutrino pdg code
   static int    PdgFsl              (const ND::NRooTrackerVtx & evt);        ///< final state primary lepton pdg code
   static int    PdgTgt              (const ND::NRooTrackerVtx & evt);        ///< (nuclear) target pdg code
   static int    PdgHitNuc           (const ND::NRooTrackerVtx & evt);        ///< hit nucleon pdg code
   static int    TgtZ                (const ND::NRooTrackerVtx & evt);        ///< target atomic number Z
   static int    TgtA                (const ND::NRooTrackerVtx & evt);        ///< target mass number A
   static int    NParticles          (const ND::NRooTrackerVtx & evt);        ///< number of particles at the event record
   static int    IdxNu               (const ND::NRooTrackerVtx & evt);        ///< incoming neutrino index in particle array
   static int    IdxTgt              (const ND::NRooTrackerVtx & evt);        ///< (nuclear) target index in particle array
   static int    IdxFsl              (const ND::NRooTrackerVtx & evt);        ///< final state primary lepton index in particle array
   static int    IdxHitNuc           (const ND::NRooTrackerVtx & evt);        ///< hit nucleon index in particle array

   static double ENu                 (const ND::NRooTrackerVtx & evt);        ///< incoming neutrino energy (GeV, at the LAB)
   static double ENu_nrf             (const ND::NRooTrackerVtx & evt);        ///< incoming neutrino energy (GeV, at the nucleon rest frame)
   static double ENu_reco            (const ND::NRooTrackerVtx & evt);        ///< incoming neutrino energy (GeV, 'reconstructed-like' energy from the ~QEL formula)
   static double EFsl                (const ND::NRooTrackerVtx & evt);        ///< final state primary lepton energy (GeV, at the LAB)
   static double MomentumFsl         (const ND::NRooTrackerVtx & evt);        ///< final state primary lepton |momentum| (GeV, at the LAB)
   static double Momentum            (const ND::NRooTrackerVtx & evt, int i); ///< ith stdhep entry |momentum| (GeV, at the LAB)
   static double E                   (const ND::NRooTrackerVtx & evt, int i); ///< ith stdhep entry kinetic energy (GeV)
   static double KE                  (const ND::NRooTrackerVtx & evt, int i); ///< ith stdhep entry kinetic energy (GeV)
   static double CosTheta            (const ND::NRooTrackerVtx & evt, int i); ///< ith stdhep entry cos(scattering angle in respect to the neutrino direction)
   static double FermiMomentum       (const ND::NRooTrackerVtx & evt);        ///< hit nucleon (fermi) |momentum| (GeV, at the LAB)

#endif

#endif
 };
}
#endif // _T2KRW_NEUT_UTILS_H_

