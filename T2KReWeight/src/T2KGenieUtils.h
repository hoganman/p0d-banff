//____________________________________________________________________________
/*!

\class    t2krew::T2KGenieUtils

\brief    Set of utilities to convert various inputs into GENIE native event 
          format. Possible inputs include:
            * A TGRooTrackerVtx obj as defined in oaAnalysis output.
            * An entry from gRooTracker tree, the GENIE bare-ROOT format used
              by ND280Software.  
            * An entry from gtree, the GENIE native event format tree.  

\author   Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
          STFC, Rutherford Appleton Laboratory

          Jim Dobson <J.Dobson07 \at imperial.ac.uk>
          Imperial College London

\created  Nov 1, 2010
*/
//____________________________________________________________________________

#ifndef _T2KRW_GENIE_UTILS_H_
#define _T2KRW_GENIE_UTILS_H_

#include <string>

#include "T2KBuild.h"
#include "T2KSyst.h"

#ifdef __T2KRW_GENIE_ENABLED__
#include "EVGCore/EventRecord.h"
#include "ReWeight/GSyst.h"
#include "GHEP/GHepParticle.h"
#include "Conventions/KinePhaseSpace.h"
#include "PDG/PDGCodes.h"
#include "PDG/PDGUtils.h"
#endif

#ifdef __T2KRW_OAANALYSIS_ENABLED__
#include "ND__GRooTrackerVtx.h"
#endif

class TFile;
class TTree;
class TBranch;
class TBits;
class TObjString;

using std::string;

namespace t2krew  {

  const int kGStdHepNPmax = 100;
  const int kGStdHepIdxPx = 0;
  const int kGStdHepIdxPy = 1;
  const int kGStdHepIdxPz = 2;
  const int kGStdHepIdxE  = 3;
  const int kGStdHepIdxX  = 0;
  const int kGStdHepIdxY  = 1;
  const int kGStdHepIdxZ  = 2;
  const int kGStdHepIdxT  = 3;

 class T2KGenieUtils
 {
 public:
#ifdef __T2KRW_GENIE_ENABLED__
   static genie::EventRecord * GetEventRecord(TTree * intree, int entry);  
   static genie::rew::GSyst_t  T2KtoGSyst(T2KSyst_t syst);
#ifdef __T2KRW_OAANALYSIS_ENABLED__
   static genie::EventRecord * GetEventRecord(const ND::GRooTrackerVtx * vtx);  
   static genie::KinePhaseSpace_t GetKinePhaseSpace(const ND::GRooTrackerVtx * vtx);  
   static bool   IsCC                (const ND::GRooTrackerVtx & evt);        ///< is it a CC event?
   static bool   IsNC                (const ND::GRooTrackerVtx & evt);        ///< is it a NC event?
   static bool   IsQELCC             (const ND::GRooTrackerVtx & evt);        ///< is it a v - (free or bound) nucleon QEL CC event
   static bool   IsELNC              (const ND::GRooTrackerVtx & evt);        ///< is it a v - (free or bound) nucleon NC elastic event
   static bool   IsRES               (const ND::GRooTrackerVtx & evt);        ///< is it a v - (free or bound) nucleon RES (CC+NC) event?
   static bool   IsRESCC             (const ND::GRooTrackerVtx & evt);        ///< is it a v - (free or bound) nucleon RES CC event?
   static bool   IsRESNC             (const ND::GRooTrackerVtx & evt);        ///< is it a v - (free or bound) nucleon RES NC event?
   static bool   IsDIS               (const ND::GRooTrackerVtx & evt);        ///< is it a v - (free or bound) nucleon DIS event?
   static bool   IsDISCC             (const ND::GRooTrackerVtx & evt);        ///< is it a v - (free or bound) nucleon DIS CC event?
   static bool   IsDISNC             (const ND::GRooTrackerVtx & evt);        ///< is it a v - (free or bound) nucleon(free or bound) DIS NC event?
   static bool   IsCOH               (const ND::GRooTrackerVtx & evt);        ///< is it a v - nucleus COH event (CC+NC, COH elastic + 1pi)?
   static bool   IsCOHPi             (const ND::GRooTrackerVtx & evt);        ///< is it a v - nucleus COH pi event (CC+NC)?
   static bool   IsCOHPiCC           (const ND::GRooTrackerVtx & evt);        ///< is it a v - nucleus COH CC pi event?
   static bool   IsCOHPiNC           (const ND::GRooTrackerVtx & evt);        ///< is it a v - nucleus COH NC pi event?
   static bool   IsAMNuGamma         (const ND::GRooTrackerVtx & evt);        ///<
   static bool   IsNuEEL             (const ND::GRooTrackerVtx & evt);        ///<
   static bool   IsIMD               (const ND::GRooTrackerVtx & evt);        ///<
   static bool   IsCharmProd         (const ND::GRooTrackerVtx & evt);        ///<
   static bool   IsQELCharm          (const ND::GRooTrackerVtx & evt);        ///<
   static bool   IsDISCharm          (const ND::GRooTrackerVtx & evt);        ///<	
   static bool   IsSingleK           (const ND::GRooTrackerVtx & evt);        ///< is it a v - (free or bound) nucleon single kaon event?
   static bool   NucIsHit            (const ND::GRooTrackerVtx & evt);        ///<
   static bool   QuarkIsHit          (const ND::GRooTrackerVtx & evt);        ///<
   static bool   SeaQuarkIsHit       (const ND::GRooTrackerVtx & evt);        ///<
   static bool   ValQuarkIsHit       (const ND::GRooTrackerVtx & evt);        ///<
   static double KineXbj             (const ND::GRooTrackerVtx & evt);        ///< bjorken x 
   static double KineNu              (const ND::GRooTrackerVtx & evt);        ///< energy transfer to the nucleus v (GeV)
   static double KineY               (const ND::GRooTrackerVtx & evt);        ///< inelasticity y
   static double KineTau             (const ND::GRooTrackerVtx & evt);        ///< t, for COH events only
   static double KineW               (const ND::GRooTrackerVtx & evt);        ///< hadronic invariant mass (GeV)
   static double KineQ2              (const ND::GRooTrackerVtx & evt);        ///< momentum transfer (GeV^2)
   static int    PdgNu               (const ND::GRooTrackerVtx & evt);        ///< initial state neutrino pdg code
   static int    PdgFsl              (const ND::GRooTrackerVtx & evt);        ///< final state primary lepton pdg code
   static int    PdgTgt              (const ND::GRooTrackerVtx & evt);        ///< (nuclear) target pdg code
   static int    PdgHitNuc           (const ND::GRooTrackerVtx & evt);        ///< hit nucleon pdg code
   static int    TgtZ                (const ND::GRooTrackerVtx & evt);        ///< target atomic number Z
   static int    TgtA                (const ND::GRooTrackerVtx & evt);        ///< target mass number A
   static int    ResId               (const ND::GRooTrackerVtx & evt);        ///< resonance id
   static int    NParticles          (const ND::GRooTrackerVtx & evt);        ///< number of particles at the event record
   static TLorentzVector P4Idx       (const ND::GRooTrackerVtx & evt, int idx); ///< 4-momenta of i'th particle in StdHep array
   static TLorentzVector P4Nu        (const ND::GRooTrackerVtx & evt);        ///< 4-momenta of the incoming neutrino 
   static int    IdxNu               (const ND::GRooTrackerVtx & evt);        ///< incoming neutrino index in particle array
   static int    IdxTgt              (const ND::GRooTrackerVtx & evt);        ///< (nuclear) target index in particle array
   static int    IdxFsl              (const ND::GRooTrackerVtx & evt);        ///< final state primary lepton index in particle array
   static int    IdxHitNuc           (const ND::GRooTrackerVtx & evt);        ///< hit nucleon index in particle array

   static double ENu                 (const ND::GRooTrackerVtx & evt);        ///< incoming neutrino energy (GeV, at the LAB)
   static double ENu_nrf             (const ND::GRooTrackerVtx & evt);        ///< incoming neutrino energy (GeV, at the nucleon rest frame)
   static double ENu_reco            (const ND::GRooTrackerVtx & evt);        ///< incoming neutrino energy (GeV, 'reconstructed-like' energy from the ~QEL formula)
   static double EFsl                (const ND::GRooTrackerVtx & evt);        ///< final state primary lepton energy (GeV, at the LAB)
   static double MomentumFsl         (const ND::GRooTrackerVtx & evt);        ///< final state primary lepton |momentum| (GeV, at the LAB)
   static double Momentum            (const ND::GRooTrackerVtx & evt, int i); ///< ith stdhep entry |momentum| (GeV, at the LAB)
   static double E                   (const ND::GRooTrackerVtx & evt, int i); ///< ith stdhep entry kinetic energy (GeV)
   static double KE                  (const ND::GRooTrackerVtx & evt, int i); ///< ith stdhep entry kinetic energy (GeV)
   static double CosTheta            (const ND::GRooTrackerVtx & evt, int i); ///< ith stdhep entry cos(scattering angle in respect to the neutrino direction)
   static double FermiMomentum       (const ND::GRooTrackerVtx & evt);        ///< hit nucleon (fermi) |momentum| (GeV, at the LAB)
#endif

#endif
 };
}
#endif // _T2KRW_GENIE_UTILS_H_

