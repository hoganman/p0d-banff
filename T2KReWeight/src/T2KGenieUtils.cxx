//____________________________________________________________________________
/*

 For the class documentation see the corresponding header file.

 Authors:
   Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
   STFC, Rutherford Appleton Laboratory

   Jim Dobson <J.Dobson07 \at imperial.ac.uk>
   Imperial College London

 @ Apr 11, 2011 - JD
   Use correct formula in CosTheta 

 @ Mar 20, 2012 - JD
   Fixed invalid conversion from ND::GRooTrackerVtx to genie::EventRecord
   due to incorrect setting of kinx and kiny as well as a number of other
   variables which needed to be set for DIS events. This fixes problems
   with DIS reweighting.
 @ Mar 23, 2012 - JD 
   Adapted T2KGenieUtils::KineXbj and T2KGenieUtils::KineNu to calculate
   the same values for x and y as are set in the GENIE code when dealing
   with COH events. Previously they just returned 0 as no hit nucleon was 
   found.
   Also removed the work around T2KGenieUtils::KineXbj_nrf and 
   T2KGenieUtils::KineNu_nrf methods now that calculate correctly in 
   T2KGenieUtils::KineXbj and T2KGenieUtils::KineNu.
   Also set the hit nucleon 4-momenta for QEL events - this fixes problem 
   with GXSec_VecFFCCQEshape weights so that now calculate correct weights
   to better than 1 part in 100 (typically better than 1/1000).
*/
//____________________________________________________________________________
#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

#include "T2KGenieUtils.h"
#include "T2KSyst.h"

#ifdef __T2KRW_GENIE_ENABLED__
#include "Ntuple/NtpMCEventRecord.h"
#include "Conventions/Units.h"
#include "Conventions/Constants.h"
#include "Conventions/GVersion.h"
#include "PDG/PDGCodes.h"
#include "PDG/PDGUtils.h"
#include "BaryonResonance/BaryonResonance.h"
#endif

using std::cerr;
using std::cout;
using std::endl;

using namespace t2krew;

//#define _DEBUG_GENIE_UTILS__

#ifdef __T2KRW_GENIE_ENABLED__
//_______________________________________________________________________________________
#ifdef __T2KRW_OAANALYSIS_ENABLED__
genie::EventRecord * T2KGenieUtils::GetEventRecord(const ND::GRooTrackerVtx * vtx)
{
  // Reconstruct as much of the GHEP record as we can using the 
  // information saved in a GRooTrackerVtx object. Note this will 
  // probably not be complete but is intended only so that it can be used
  // for reweighting.   
  genie::EventRecord * fDummyGHEP = NULL;
  if(!vtx){
    cout << "Cannot convert a null GRooTrackerVtx to GHEP record!" << endl;
    return fDummyGHEP;
  }

  // First set the interaction information.
  genie::Interaction * fInteraction = NULL;
  fDummyGHEP   = new genie::EventRecord();
  fDummyGHEP -> SetWeight(vtx->EvtWght); // Don't think this is actually need but just for completeness
  #if __GENIE_RELEASE_CODE__ >= GRELCODE(2,8,0)
    // In recent versions of GENIE (>= 2.8.0), SetDiffXSec() requires a second argument
    genie::KinePhaseSpace_t fKPS = T2KGenieUtils::GetKinePhaseSpace(vtx);
    if (fKPS == NULL) fDummyGHEP -> SetWeight(1.);
    fDummyGHEP -> SetDiffXSec (vtx->EvtDXSec / (1E+38 / genie::units::cm2), fKPS);
  #else
    fDummyGHEP -> SetDiffXSec (vtx->EvtDXSec / (1E+38 / genie::units::cm2));
  #endif
  
  int target = T2KGenieUtils::PdgTgt(*vtx);
  int hitnucl = T2KGenieUtils::PdgHitNuc(*vtx);
  int idx_hitnuc = T2KGenieUtils::IdxHitNuc(*vtx);
  int nu = T2KGenieUtils::PdgNu(*vtx);
  TLorentzVector P4v = T2KGenieUtils::P4Nu(*vtx);
  TLorentzVector P4_hitnucl = T2KGenieUtils::P4Idx(*vtx, idx_hitnuc);
  double kineQ2 = T2KGenieUtils::KineQ2(*vtx);
  double kineW = T2KGenieUtils::KineW(*vtx);
  double kinex = T2KGenieUtils::KineXbj(*vtx);
  double kiney = T2KGenieUtils::KineY(*vtx);
  bool is_charm = T2KGenieUtils::IsCharmProd(*vtx);
  bool is_cc = T2KGenieUtils::IsCC(*vtx);
  int fZ = genie::pdg::IonPdgCodeToZ(target);
  int fA = genie::pdg::IonPdgCodeToA(target);
  if(target == 2112){
    fA = 1; 
    fZ = 0;
  } 
  if(target == 2212){
    fA = 1; 
    fZ = 1;
  } 
  genie::Target tgt;
  tgt.SetId(fZ, fA);
  int target_pdg = tgt.Pdg(); 

  bool degenerate = 0;
  
#ifdef _DEBUG_GHEPCONV__
  std::cout << "Event: "<< vtx->EvtCode->GetString() << std::endl;
#endif

  // For QEL and elastic scattering. 
  bool is_qel = vtx->EvtCode->GetString().Contains("QES");
  if(is_qel && !is_charm){
    if(is_cc){
      degenerate++;
      fInteraction = genie::Interaction::QELCC(target_pdg, hitnucl, nu, P4v);
    }
    else{
      degenerate++;
      fInteraction = genie::Interaction::QELNC(target_pdg, hitnucl, nu, P4v);
    }
    fInteraction->KinePtr()->SetQ2(kineQ2, true);
    // Need to set the hit nucleon as used when calculating events for 
    // the GXSec_VecFFCCQEshape systematic
    fInteraction->InitStatePtr()->TgtPtr()->SetHitNucP4(P4_hitnucl);
  }

  // For RES events.
  if(T2KGenieUtils::IsRES(*vtx)){
    degenerate++;
    if(is_cc) fInteraction = genie::Interaction::RESCC(target_pdg, hitnucl, nu, P4v);  
    else fInteraction = genie::Interaction::RESNC(target_pdg, hitnucl, nu, P4v);
    // Set the kinematical variables.
    fInteraction->KinePtr()->SetQ2(kineQ2, true);
    fInteraction->KinePtr()->SetW(kineW, true);
    // Set charm flag. Do not set the charm Id. This is done in RES event
    // generator, don't think matters as reweighting code only asks if charm.
    if(is_charm) fInteraction->ExclTagPtr()->SetCharm();  
    genie::Resonance_t res = (genie::Resonance_t) T2KGenieUtils::ResId(*vtx);
    assert(res != genie::kNoResonance);
    fInteraction->ExclTagPtr()->SetResonance(res);
    fInteraction->InitStatePtr()->TgtPtr()->SetHitNucP4(P4_hitnucl);
    // Need to decide whether it is necessry to se the following? Don't think
    // so as ReWeighting code does not seem to use info. 
    // fInteraction->ExclTagPtr()->SetNNucleons (nproton, nneutron);
    // fInteraction->ExclTagPtr()->SetNPions    (npiplus, npi0, npiminus);
  }

  // For DIS events (as required, IsDIS does not include DIS charmed-production)
  if(T2KGenieUtils::IsDIS(*vtx)){
    degenerate++;
    if(is_cc) fInteraction = genie::Interaction::DISCC(target_pdg, hitnucl, nu, P4v);  
    else fInteraction = genie::Interaction::DISNC(target_pdg, hitnucl, nu, P4v);
    // Set the kinematical variables.
    fInteraction->KinePtr()->SetQ2(kineQ2, true);
    fInteraction->KinePtr()->SetW (kineW, true);
    fInteraction->KinePtr()->Setx (kinex, true);
    fInteraction->KinePtr()->Sety (kiney, true);
    fInteraction->InitStatePtr()->TgtPtr()->SetHitNucP4(P4_hitnucl);
    // Set the hit quark info
    TString eventcode = vtx->EvtCode->GetString(); 
    if(eventcode.Index("q:") >= 0){
      bool is_valence = eventcode.Index("(v)") >= 0;
      int hitquarkpdg = std::atoi(eventcode(eventcode.Index("q:")+2,1).Data());
      fInteraction->InitStatePtr()->TgtPtr()->SetHitQrkPdg(hitquarkpdg);
      fInteraction->InitStatePtr()->TgtPtr()->SetHitSeaQrk(!is_valence);
    }
  }

  bool is_coh = T2KGenieUtils::IsCOH(*vtx);
  // For COH scattering
  if(is_coh){
    degenerate++;
    if(is_cc) fInteraction = genie::Interaction::COHCC(target_pdg, nu, P4v);  
    else fInteraction = genie::Interaction::COHNC(target_pdg, nu, P4v);;
    fInteraction->KinePtr()->SetQ2(kineQ2, true);
    // Currently unable to calculate the following for COH events
    fInteraction->KinePtr()->Setx (kinex, true);
    fInteraction->KinePtr()->Sety (kiney, true);
    fInteraction->KinePtr()->SetW (kineW, true);
  }

  // NuEElastic scattering. 
  bool is_nueel = vtx->EvtCode->GetString().Contains("NuEEL");
  if(is_nueel){
    degenerate++;
  
    genie::InitialState init = genie::InitialState(target_pdg, nu);  
    init.TgtPtr()->SetHitNucPdg(0);
  
    // NC
    if(nu == genie::kPdgNuMu  || nu == genie::kPdgAntiNuMu ||
       nu == genie::kPdgNuTau || nu == genie::kPdgAntiNuTau) {
       genie::ProcessInfo   proc_info(genie::kScNuElectronElastic, genie::kIntWeakNC);
       fInteraction = new genie::Interaction(init, proc_info);
    } 
    // CC+NC+interference
    if(nu == genie::kPdgNuE  || nu == genie::kPdgAntiNuE) {
       genie::ProcessInfo   proc_info(genie::kScNuElectronElastic,  genie::kIntWeakMix);
       fInteraction = new genie::Interaction(init, proc_info);
    }

  }
  
  // Single kaon production.
  if(T2KGenieUtils::IsSingleK(*vtx)){
    degenerate++;
    fInteraction = genie::Interaction::ASK(target_pdg, nu, P4v);;
    fInteraction->KinePtr()->SetQ2(kineQ2, true);
    fInteraction->KinePtr()->Setx (kinex, true);
    fInteraction->KinePtr()->Sety (kiney, true);
    fInteraction->KinePtr()->SetW (kineW, true);
    fInteraction->InitStatePtr()->TgtPtr()->SetHitNucP4(P4_hitnucl);
  }
  
  // Charm production. Does not have a separate genie::Interaction type, 
  // so we use the same objects as for QEL/DIS instead.
  if (is_charm) {
    if (T2KGenieUtils::IsQELCharm(*vtx)) {  // same as for QEL events
      if(is_cc){
        degenerate++;
        fInteraction = genie::Interaction::QELCC(target_pdg, hitnucl, nu, P4v);
      }
      else{
        degenerate++;
        fInteraction = genie::Interaction::QELNC(target_pdg, hitnucl, nu, P4v);
      }
      fInteraction->KinePtr()->SetQ2(kineQ2, true);
      fInteraction->InitStatePtr()->TgtPtr()->SetHitNucP4(P4_hitnucl);
    }
    if (T2KGenieUtils::IsDISCharm(*vtx)) {  // same as for DIS events
      if(is_cc) {
        degenerate++;
        fInteraction = genie::Interaction::DISCC(target_pdg, hitnucl, nu, P4v);  
      }
      else {
        degenerate++;
        fInteraction = genie::Interaction::DISNC(target_pdg, hitnucl, nu, P4v);
      }
      // Set the kinematical variables.
      fInteraction->KinePtr()->SetQ2(kineQ2, true);
      fInteraction->KinePtr()->SetW (kineW, true);
      fInteraction->KinePtr()->Setx (kinex, true);
      fInteraction->KinePtr()->Sety (kiney, true);
      fInteraction->InitStatePtr()->TgtPtr()->SetHitNucP4(P4_hitnucl);
      // Set the hit quark info
      TString eventcode = vtx->EvtCode->GetString(); 
      if(eventcode.Index("q:") >= 0){
        bool is_valence = eventcode.Index("(v)") >= 0;
        int hitquarkpdg = std::atoi(eventcode(eventcode.Index("q:")+2,1).Data());
        fInteraction->InitStatePtr()->TgtPtr()->SetHitQrkPdg(hitquarkpdg);
        fInteraction->InitStatePtr()->TgtPtr()->SetHitSeaQrk(!is_valence);
      }
    }
  }
  
  if(degenerate > 1){
    std::cerr << "Degenerate choice for process type - exiting!" << std::endl;
    exit(1);
  }
  else if(degenerate < 1){
    std::cerr << "Unable to categorise event: "<< vtx->EvtCode->GetString() << std::endl;
    return NULL;
  }

  // Add the interaction to the GHEPRecord.
  fDummyGHEP->AttachSummary(fInteraction);

  // Now loop over all particles and add to EventRecord.
  for(int ip = 0; ip < vtx->StdHepN; ip++){
    // Construct a GHepParticle with most of the information. 
    genie::GHepParticle p(vtx->StdHepPdg[ip], genie::GHepStatus_t(vtx->StdHepStatus[ip]), 
                   vtx->StdHepFm[ip], vtx->StdHepLm[ip],
                   vtx->StdHepFd[ip], vtx->StdHepLd[ip],
                   vtx->StdHepP4[ip][0], vtx->StdHepP4[ip][1], 
                   vtx->StdHepP4[ip][2], vtx->StdHepP4[ip][3], 
                   vtx->StdHepX4[ip][0], vtx->StdHepX4[ip][1], 
                   vtx->StdHepX4[ip][2], vtx->StdHepX4[ip][3]);

    // Fill in the extra information.
    p.SetRescatterCode(vtx->StdHepRescat[ip]);

    // For moment leave out polarisation as is not used by reweighting
    // TVector3 polz(vtx->StdHepPolz[ip][0], vtx->StdHepPolz[ip][1], vtx->StdHepPolz[ip][2]);
    // p.SetPolarization(polz);
    // Also leave out the following for the moment as not used by any of current parameters
    // SetBound(), SetRemovalEnergy() 

    // Add to the event.
    fDummyGHEP->AddParticle(p);
  }
  return fDummyGHEP;
}
//_______________________________________________________________________________________
genie::KinePhaseSpace_t T2KGenieUtils::GetKinePhaseSpace(const ND::GRooTrackerVtx * vtx)
{
  // Unique reaction types (defined in $GENIE/src/Interaction/ScatteringType.h)
  bool is_qel = vtx->EvtCode->GetString().Contains("QES");
  bool is_res = vtx->EvtCode->GetString().Contains("RES");
  bool is_dis = vtx->EvtCode->GetString().Contains("DIS");
  bool is_coh = vtx->EvtCode->GetString().Contains("COH");
  bool is_nue = vtx->EvtCode->GetString().Contains("NuEEL");
  bool is_cel = vtx->EvtCode->GetString().Contains("COHEl");
  bool is_dfr = vtx->EvtCode->GetString().Contains("DFR");
  bool is_sk  = vtx->EvtCode->GetString().Contains("1Kaon");
  bool is_ibd = vtx->EvtCode->GetString().Contains("IBD");
  
  // Set kinetic phase space corresponding to reaction
  genie::KinePhaseSpace_t kps = genie::kPSNull;
  if      (is_qel) kps = genie::kPSQ2fE;       // quasi-elastic (QEL)
  else if (is_res) kps = genie::kPSWQ2fE;      // resonant (RES)
  else if (is_dis) kps = genie::kPSxyfE;       // deep inelastic (DIS)
  else if (is_coh) kps = genie::kPSxytfE;      // coherent (COH)
  else if (is_nue) kps = genie::kPSyfE;        // nu-e scattering (NuE)
  else if (is_cel) kps = genie::kPSyfE;        // coherent elastic (COHEl)
  else if (is_dfr) kps = genie::kPSxytfE;      // diffractive reaction (DFR)
  else if (is_sk)  kps = genie::kPSTkTlctl;    // single kaon (SK)
  else if (is_ibd) kps = genie::kPSQ2fE;       // inverse beta decay (IBD)
  else std::cout << "*** WARNING: KinePhaseSpace could not be found!" << std::endl;
  
  // Return KinePhaseSpace_t object pointer for given GRooTrackerVtx
  return kps;  
}
#endif
//_______________________________________________________________________________________
genie::EventRecord * T2KGenieUtils::GetEventRecord(TTree * intree, int entry)
{
  // A simple test case assuming we have a GHEP tree.
  genie::EventRecord * event = NULL; 
  TBranch * br_gmcrec = intree->GetBranch("gmcrec");
  if(br_gmcrec){
    genie::NtpMCEventRecord * mcrec = NULL;
    br_gmcrec->SetAddress(&mcrec);
    intree->GetEntry(entry);
    if(mcrec) {
      event = new genie::EventRecord(*(mcrec->event));
      delete mcrec;
    }
  }  
  return event;
  // JIMTODO - Resetting the branch address every time is inefficient - could
  // make branch and variables datamembers of this class and only set them once
}
//_______________________________________________________________________________________
genie::rew::GSyst_t T2KGenieUtils::T2KtoGSyst(T2KSyst_t syst)
// map T2KSyst_t to the corresponding GENIE GSyst_t so that we can call functions
// from GENIE
{
  switch(syst) {
    case ( kGXSec_MaNCEL         ) : return genie::rew::kXSecTwkDial_MaNCEL;         break;
    case ( kGXSec_EtaNCEL        ) : return genie::rew::kXSecTwkDial_EtaNCEL;        break;
    case ( kGXSec_NormCCQE       ) : return genie::rew::kXSecTwkDial_NormCCQE;       break;
    case ( kGXSec_NormCCQEenu    ) : return genie::rew::kXSecTwkDial_NormCCQEenu;    break;
    case ( kGXSec_MaCCQEshape    ) : return genie::rew::kXSecTwkDial_MaCCQEshape;    break;
    case ( kGXSec_MaCCQE         ) : return genie::rew::kXSecTwkDial_MaCCQE;         break;
    case ( kGXSec_VecFFCCQEshape ) : return genie::rew::kXSecTwkDial_VecFFCCQEshape; break;
    case ( kGXSec_NormCCRES      ) : return genie::rew::kXSecTwkDial_NormCCRES;      break;
    case ( kGXSec_MaCCRESshape   ) : return genie::rew::kXSecTwkDial_MaCCRESshape;   break;
    case ( kGXSec_MvCCRESshape   ) : return genie::rew::kXSecTwkDial_MvCCRESshape;   break;
    case ( kGXSec_MaCCRES        ) : return genie::rew::kXSecTwkDial_MaCCRES;        break;
    case ( kGXSec_MvCCRES        ) : return genie::rew::kXSecTwkDial_MvCCRES;        break;
    case ( kGXSec_NormNCRES      ) : return genie::rew::kXSecTwkDial_NormNCRES;      break;
    case ( kGXSec_MaNCRESshape   ) : return genie::rew::kXSecTwkDial_MaNCRESshape;   break;
    case ( kGXSec_MvNCRESshape   ) : return genie::rew::kXSecTwkDial_MvNCRESshape;   break;
    case ( kGXSec_MaNCRES        ) : return genie::rew::kXSecTwkDial_MaNCRES;        break;
    case ( kGXSec_MvNCRES        ) : return genie::rew::kXSecTwkDial_MvNCRES;        break;
    case ( kGXSec_MaCOHpi        ) : return genie::rew::kXSecTwkDial_MaCOHpi;        break;
    case ( kGXSec_R0COHpi        ) : return genie::rew::kXSecTwkDial_R0COHpi;        break;
    case ( kGXSec_RvpCC1pi       ) : return genie::rew::kXSecTwkDial_RvpCC1pi;       break;
    case ( kGXSec_RvpCC2pi       ) : return genie::rew::kXSecTwkDial_RvpCC2pi;       break;
    case ( kGXSec_RvpNC1pi       ) : return genie::rew::kXSecTwkDial_RvpNC1pi;       break;
    case ( kGXSec_RvpNC2pi       ) : return genie::rew::kXSecTwkDial_RvpNC2pi;       break;
    case ( kGXSec_RvnCC1pi       ) : return genie::rew::kXSecTwkDial_RvnCC1pi;       break;
    case ( kGXSec_RvnCC2pi       ) : return genie::rew::kXSecTwkDial_RvnCC2pi;       break;
    case ( kGXSec_RvnNC1pi       ) : return genie::rew::kXSecTwkDial_RvnNC1pi;       break;
    case ( kGXSec_RvnNC2pi       ) : return genie::rew::kXSecTwkDial_RvnNC2pi;       break;
    case ( kGXSec_RvbarpCC1pi    ) : return genie::rew::kXSecTwkDial_RvbarpCC1pi;    break;
    case ( kGXSec_RvbarpCC2pi    ) : return genie::rew::kXSecTwkDial_RvbarpCC2pi;    break;
    case ( kGXSec_RvbarpNC1pi    ) : return genie::rew::kXSecTwkDial_RvbarpNC1pi;    break;
    case ( kGXSec_RvbarpNC2pi    ) : return genie::rew::kXSecTwkDial_RvbarpNC2pi;    break;
    case ( kGXSec_RvbarnCC1pi    ) : return genie::rew::kXSecTwkDial_RvbarnCC1pi;    break;
    case ( kGXSec_RvbarnCC2pi    ) : return genie::rew::kXSecTwkDial_RvbarnCC2pi;    break;
    case ( kGXSec_RvbarnNC1pi    ) : return genie::rew::kXSecTwkDial_RvbarnNC1pi;    break;
    case ( kGXSec_RvbarnNC2pi    ) : return genie::rew::kXSecTwkDial_RvbarnNC2pi;    break;
    case ( kGXSec_AhtBY          ) : return genie::rew::kXSecTwkDial_AhtBY;          break;
    case ( kGXSec_BhtBY          ) : return genie::rew::kXSecTwkDial_BhtBY;          break;
    case ( kGXSec_CV1uBY         ) : return genie::rew::kXSecTwkDial_CV1uBY;         break;
    case ( kGXSec_CV2uBY         ) : return genie::rew::kXSecTwkDial_CV2uBY;         break;
    case ( kGXSec_AhtBYshape     ) : return genie::rew::kXSecTwkDial_AhtBYshape;     break;
    case ( kGXSec_BhtBYshape     ) : return genie::rew::kXSecTwkDial_BhtBYshape;     break;
    case ( kGXSec_CV1uBYshape    ) : return genie::rew::kXSecTwkDial_CV1uBYshape;    break;
    case ( kGXSec_CV2uBYshape    ) : return genie::rew::kXSecTwkDial_CV2uBYshape;    break;
    case ( kGXSec_NormDISCC      ) : return genie::rew::kXSecTwkDial_NormDISCC;      break;
    case ( kGXSec_RnubarnuCC     ) : return genie::rew::kXSecTwkDial_RnubarnuCC;     break;
    case ( kGXSec_DISNuclMod     ) : return genie::rew::kXSecTwkDial_DISNuclMod;     break;
    case ( kGXSec_NC             ) : return genie::rew::kXSecTwkDial_NC;             break;
    case ( kGHadrAGKY_xF1pi    ) : return genie::rew::kHadrAGKYTwkDial_xF1pi;    break;
    case ( kGHadrAGKY_pT1pi    ) : return genie::rew::kHadrAGKYTwkDial_pT1pi;    break;
    case ( kGHadrNucl_FormZone ) : return genie::rew::kHadrNuclTwkDial_FormZone; break;
    case ( kGINuke_MFP_pi      ) : return genie::rew::kINukeTwkDial_MFP_pi;      break;
    case ( kGINuke_MFP_N       ) : return genie::rew::kINukeTwkDial_MFP_N;       break;
    case ( kGINuke_FrCEx_pi    ) : return genie::rew::kINukeTwkDial_FrCEx_pi;    break;
    case ( kGINuke_FrElas_pi   ) : return genie::rew::kINukeTwkDial_FrElas_pi;   break;
    case ( kGINuke_FrInel_pi   ) : return genie::rew::kINukeTwkDial_FrInel_pi;   break;
    case ( kGINuke_FrAbs_pi    ) : return genie::rew::kINukeTwkDial_FrAbs_pi;    break;
    case ( kGINuke_FrPiProd_pi ) : return genie::rew::kINukeTwkDial_FrPiProd_pi; break;
    case ( kGINuke_FrCEx_N     ) : return genie::rew::kINukeTwkDial_FrCEx_N;     break;
    case ( kGINuke_FrElas_N    ) : return genie::rew::kINukeTwkDial_FrElas_N;    break;
    case ( kGINuke_FrInel_N    ) : return genie::rew::kINukeTwkDial_FrInel_N;    break;
    case ( kGINuke_FrAbs_N     ) : return genie::rew::kINukeTwkDial_FrAbs_N;     break;
    case ( kGINuke_FrPiProd_N  ) : return genie::rew::kINukeTwkDial_FrPiProd_N;  break;
    case ( kGNucl_CCQEPauliSupViaKF   ) : return genie::rew::kSystNucl_CCQEPauliSupViaKF;   break;
    case ( kGNucl_CCQEMomDistroFGtoSF ) : return genie::rew::kSystNucl_CCQEMomDistroFGtoSF; break;
    case ( kGRDcy_BR1gamma        ) : return genie::rew::kRDcyTwkDial_BR1gamma;        break;
    case ( kGRDcy_BR1eta          ) : return genie::rew::kRDcyTwkDial_BR1eta;          break;
    case ( kGRDcy_Theta_Delta2Npi ) : return genie::rew::kRDcyTwkDial_Theta_Delta2Npi; break;
    default:
      return genie::rew::kNullSystematic;
      break;
  }
  return genie::rew::kNullSystematic;
}
//_______________________________________________________________________________________
#ifdef __T2KRW_OAANALYSIS_ENABLED__
bool T2KGenieUtils::IsCC(const ND::GRooTrackerVtx & evt) 
{
// Is it a CC event?
//
  string code = evt.EvtCode->GetString().Data();
  return (code.find("Weak[CC]") != string::npos);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::IsNC(const ND::GRooTrackerVtx & evt) 
{
// Is it a NC event?
//
  string code = evt.EvtCode->GetString().Data();
  return (code.find("Weak[NC]") != string::npos);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::IsQELCC(const ND::GRooTrackerVtx & evt) 
{
// Is it a QEL CC event?
// Note: Excludes QEL charm production -
//
  string code = evt.EvtCode->GetString().Data();
  return (!T2KGenieUtils::IsCharmProd(evt) && code.find("Weak[CC],QES") != string::npos);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::IsELNC(const ND::GRooTrackerVtx & evt) 
{
// Is it an Elastic NC event?
//
  string code = evt.EvtCode->GetString().Data();
  return (code.find("Weak[NC],QES") != string::npos);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::IsRES(const ND::GRooTrackerVtx & evt)
{
// Is it a Resonance (CC+NC) event?
//
  string code = evt.EvtCode->GetString().Data();
  return (code.find("RES") != string::npos);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::IsRESCC(const ND::GRooTrackerVtx & evt) 
{
// Is it a Resonance CC event?
//
  string code = evt.EvtCode->GetString().Data();
  return (code.find("Weak[CC],RES") != string::npos);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::IsRESNC(const ND::GRooTrackerVtx & evt) 
{
// Is it a Resonance NC event?
//
  string code = evt.EvtCode->GetString().Data();
  return (code.find("Weak[NC],RES") != string::npos);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::IsDIS(const ND::GRooTrackerVtx & evt) 
{
// Is it a DIS (CC+NC) event?
// Note: Excludes DIS charm production -
//
  string code = evt.EvtCode->GetString().Data();
  return (!T2KGenieUtils::IsCharmProd(evt) && code.find("DIS") != string::npos);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::IsDISCC(const ND::GRooTrackerVtx & evt) 
{
// Is it a DIS CC event?
// Note: Excludes DIS charm production -
//
  string code = evt.EvtCode->GetString().Data();
  return (!T2KGenieUtils::IsCharmProd(evt) && code.find("Weak[CC],DIS") != string::npos);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::IsDISNC(const ND::GRooTrackerVtx & evt) 
{
// Is it a DIS NC event?
// Note: Excludes DIS charm production -
//
  string code = evt.EvtCode->GetString().Data();
  return (!T2KGenieUtils::IsCharmProd(evt) && code.find("Weak[NC],DIS") != string::npos);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::IsCOH(const ND::GRooTrackerVtx & evt)
{
// Is it a COH (CC+NC, elastic + pi prod + ...)  event?
//
  string code = evt.EvtCode->GetString().Data();
  return (code.find("COH") != string::npos);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::IsCOHPi(const ND::GRooTrackerVtx & evt) 
{
// Is it a COH pion production event (CC+NC)?
//
  bool is_coh  = T2KGenieUtils::IsCOH(evt);
  bool has_1pi = true; 

  return (is_coh && has_1pi);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::IsCOHPiCC(const ND::GRooTrackerVtx & evt) 
{
// Is it a COH CC pion production event?
//
  bool is_cohpi = T2KGenieUtils::IsCOHPi(evt);
  bool is_cc    = T2KGenieUtils::IsCC   (evt);

  return (is_cohpi && is_cc);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::IsCOHPiNC(const ND::GRooTrackerVtx & evt) 
{
// Is it a COH NC pion production event?
//
  bool is_cohpi = T2KGenieUtils::IsCOHPi(evt);
  bool is_nc    = T2KGenieUtils::IsNC   (evt);

  return (is_cohpi && is_nc);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::IsAMNuGamma(const ND::GRooTrackerVtx & evt) 
{
// Is it an anomaly-mediated nu-gamma interaction?
//
  string code = evt.EvtCode->GetString().Data();
  return (code.find("AMNuGamma") != string::npos);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::IsNuEEL(const ND::GRooTrackerVtx & evt) 
{
// Is it a (anti)neutrino - electron elastic event?
//
  string code = evt.EvtCode->GetString().Data();
  return (code.find("NuEEL") != string::npos);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::IsIMD(const ND::GRooTrackerVtx & evt) 
{
// Is it an inverse muon decay event?
//
  string code = evt.EvtCode->GetString().Data();
  return (code.find("IMD") != string::npos);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::IsCharmProd(const ND::GRooTrackerVtx & evt) 
{
// Is it a charm production event?
//
  string code = evt.EvtCode->GetString().Data();
  return (code.find("charm") != string::npos);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::IsQELCharm(const ND::GRooTrackerVtx & evt) 
{
// Is it a QEL charm production event?
//
  string code = evt.EvtCode->GetString().Data();
  return (code.find("QES;charm") != string::npos);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::IsDISCharm(const ND::GRooTrackerVtx & evt) 
{
// Is it a DIS charm production event?
//
  string code = evt.EvtCode->GetString().Data();
  return (code.find("DIS;charm") != string::npos);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::IsSingleK(const ND::GRooTrackerVtx & evt) 
{
// Is it a single kaon production event?
//
  string code = evt.EvtCode->GetString().Data();
  return (code.find("Weak[CC],1Kaon") != string::npos);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::NucIsHit(const ND::GRooTrackerVtx & evt) 
{
  string code = evt.EvtCode->GetString().Data();
  return (code.find("N:2112") != string::npos ||
          code.find("N:2212") != string::npos);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::QuarkIsHit(const ND::GRooTrackerVtx & evt) 
{
  string code = evt.EvtCode->GetString().Data();
  return (code.find("N:2112;q:") != string::npos ||
          code.find("N:2212;q:") != string::npos);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::SeaQuarkIsHit(const ND::GRooTrackerVtx & evt) 
{
  string code = evt.EvtCode->GetString().Data();
  return (code.find("(s)") != string::npos);
}
//_______________________________________________________________________________________
bool T2KGenieUtils::ValQuarkIsHit(const ND::GRooTrackerVtx & evt) 
{
  string code = evt.EvtCode->GetString().Data();
  return (code.find("(v)") != string::npos);
}
//_______________________________________________________________________________________
double T2KGenieUtils::KineXbj(const ND::GRooTrackerVtx & evt) 
{
// Bjorken scaling var, xbj = Q^2/(2*q.p), where p is the 4-momenta of 
// the hit nucleon and q is transfered 4-momenta. This is equivalent to
// calculating as xbj = 0.5*Q2/(Mnucleon*v)
//
  int iv = T2KGenieUtils::IdxNu (evt);
  int il = T2KGenieUtils::IdxFsl(evt);

  if(iv==-1 || il==-1) return 0;

  // get the neutrino, final state prim lepton & hit nucleon 4-momenta
  TLorentzVector p4v(evt.StdHepP4[iv][kGStdHepIdxPx],
                     evt.StdHepP4[iv][kGStdHepIdxPy],
                     evt.StdHepP4[iv][kGStdHepIdxPz],
                     evt.StdHepP4[iv][kGStdHepIdxE]);
  TLorentzVector p4l(evt.StdHepP4[il][kGStdHepIdxPx],
                     evt.StdHepP4[il][kGStdHepIdxPy],
                     evt.StdHepP4[il][kGStdHepIdxPz],
                     evt.StdHepP4[il][kGStdHepIdxE]);

  TLorentzVector q = p4v - p4l;

  // if no hit nucleon and not coherent scattering then cannot calculate x
  bool is_coh = T2KGenieUtils::IsCOH(evt);
  int idx_nucl = T2KGenieUtils::IdxHitNuc(evt);
  if(idx_nucl < 0 && !is_coh) return 0;

  TLorentzVector p4nucl;
 
  // Use the hit nucleon 4-momenta in the lab frame  
  if(is_coh == false){
      p4nucl.SetXYZT(evt.StdHepP4[idx_nucl][kGStdHepIdxPx],
                     evt.StdHepP4[idx_nucl][kGStdHepIdxPy], 
                     evt.StdHepP4[idx_nucl][kGStdHepIdxPz], 
                     evt.StdHepP4[idx_nucl][kGStdHepIdxE]);
  }
  // For COH events use the 4-momenta of a hit nucleon in its own
  // rest frame. This results in a value of x consistent definition 
  // given in D. Rein, L. M. Sehgal, Coherent pi0 production in neutrino
  // reactions, Nucl. Phys. B223 (1983) 29
  else p4nucl.SetXYZT(0.0, 0.0, 0.0, genie::constants::kNucleonMass);

  double Q2 = T2KGenieUtils::KineQ2(evt);
  double pdotq = q.Dot(p4nucl);
  double xbj = 0.5*Q2/pdotq;

  return xbj;
}
//_______________________________________________________________________________________
double T2KGenieUtils::KineNu(const ND::GRooTrackerVtx & evt) 
{
// Energy transfer, v (GeV)
// This is the energy loss of the leptons in the nucleon rest frams
// and is equivalent to q.P/M_nucl where q is the 4-momentum transfer 
// and P is the 4-momentum of the hit nucleon
//
  int iv = T2KGenieUtils::IdxNu (evt); 
  int il = T2KGenieUtils::IdxFsl(evt);

  if(iv==-1 || il==-1) return 0;

  // get the neutrino, final state prim lepton & hit nucleon 4-momenta
  TLorentzVector p4v(evt.StdHepP4[iv][kGStdHepIdxPx], 
                     evt.StdHepP4[iv][kGStdHepIdxPy],
                     evt.StdHepP4[iv][kGStdHepIdxPz], 
                     evt.StdHepP4[iv][kGStdHepIdxE]);
  TLorentzVector p4l(evt.StdHepP4[il][kGStdHepIdxPx], 
                     evt.StdHepP4[il][kGStdHepIdxPy], 
                     evt.StdHepP4[il][kGStdHepIdxPz], 
                     evt.StdHepP4[il][kGStdHepIdxE]);

  TLorentzVector q = p4v - p4l;

  int iN = T2KGenieUtils::IdxHitNuc(evt);
  if(iN<0) return q.Energy(); // no hit nuc - same as Ev at LAB
  double bx = evt.StdHepP4[iN][kGStdHepIdxPx] /  evt.StdHepP4[iN][kGStdHepIdxE];
  double by = evt.StdHepP4[iN][kGStdHepIdxPy] /  evt.StdHepP4[iN][kGStdHepIdxE];
  double bz = evt.StdHepP4[iN][kGStdHepIdxPz] /  evt.StdHepP4[iN][kGStdHepIdxE];

  q.Boost(-bx,-by,-bz);
  
  return q.Energy();
}
//_______________________________________________________________________________________
double T2KGenieUtils::KineY(const ND::GRooTrackerVtx & evt) 
{
// Inelasticity, y = (p4_nu - p4_l).p4_nucl / p4_nu.p4_nucl 
// Previously was being calculated in the wrong rest frame as 
// y = v/Ev. 
// 
  int iv = T2KGenieUtils::IdxNu (evt); 
  int il = T2KGenieUtils::IdxFsl(evt);

  if(iv==-1 || il==-1) return 0;

  // get the neutrino, final state prim lepton & hit nucleon 4-momenta
  TLorentzVector p4v(evt.StdHepP4[iv][kGStdHepIdxPx], 
                     evt.StdHepP4[iv][kGStdHepIdxPy],
                     evt.StdHepP4[iv][kGStdHepIdxPz], 
                     evt.StdHepP4[iv][kGStdHepIdxE]);
  TLorentzVector p4l(evt.StdHepP4[il][kGStdHepIdxPx], 
                     evt.StdHepP4[il][kGStdHepIdxPy], 
                     evt.StdHepP4[il][kGStdHepIdxPz], 
                     evt.StdHepP4[il][kGStdHepIdxE]);

  TLorentzVector q = p4v - p4l;

  // if no hit nucleon and not coherent scattering then cannot calculate x
  bool is_coh = T2KGenieUtils::IsCOH(evt);
  int idx_nucl = T2KGenieUtils::IdxHitNuc(evt);
  if(idx_nucl < 0 && !is_coh) return 0;

  TLorentzVector p4nucl;
 
  // Use the hit nucleon 4-momenta in the lab frame  
  if(is_coh == false){
      p4nucl.SetXYZT(evt.StdHepP4[idx_nucl][kGStdHepIdxPx],
                     evt.StdHepP4[idx_nucl][kGStdHepIdxPy], 
                     evt.StdHepP4[idx_nucl][kGStdHepIdxPz], 
                     evt.StdHepP4[idx_nucl][kGStdHepIdxE]);
  }
  // For COH events use the 4-momenta of a hit nucleon in its own
  // rest frame. This results in a value of y consistent with the
  // definition of x given in D. Rein, L. M. Sehgal, Coherent pi0 
  // production in neutrino reactions, Nucl. Phys. B223 (1983) 29
  else p4nucl.SetXYZT(0.0, 0.0, 0.0, genie::constants::kNucleonMass);

  double y = q.Dot(p4nucl)/(p4v.Dot(p4nucl)); 

  return y; 
}
//_______________________________________________________________________________________
double T2KGenieUtils::KineTau(const ND::GRooTrackerVtx & /*evt*/)
{
// t, for COH events only
//
  return 0;
}
//_______________________________________________________________________________________
double T2KGenieUtils::KineW(const ND::GRooTrackerVtx & evt) 
{
// Hadronic invariant mass, W (GeV)
//
  int iv = T2KGenieUtils::IdxNu     (evt);              
  int il = T2KGenieUtils::IdxFsl    (evt);
  int in = T2KGenieUtils::IdxHitNuc (evt);              

  // Return 0 if cannot find incoming or outgoing lepton or hit nucleon. 
  // In the case of COH scattering there will be no hit nucleon, we could 
  // copy what is done in the GENIE code and set W to genie::constants::kPionMass.
  // For now will return 0 as physical meaning of using kPionMass is not clear
  // and a value of 0 will force analysers to think about the meaning of W for
  // COH events
  if(iv==-1 || il==-1 || in==-1) return 0;

  TLorentzVector p4v(evt.StdHepP4[iv][kGStdHepIdxPx], 
                     evt.StdHepP4[iv][kGStdHepIdxPy],
                     evt.StdHepP4[iv][kGStdHepIdxPz], 
                     evt.StdHepP4[iv][kGStdHepIdxE]);
  TLorentzVector p4l(evt.StdHepP4[il][kGStdHepIdxPx], 
                     evt.StdHepP4[il][kGStdHepIdxPy], 
                     evt.StdHepP4[il][kGStdHepIdxPz], 
                     evt.StdHepP4[il][kGStdHepIdxE]);
  TLorentzVector p4n(evt.StdHepP4[in][kGStdHepIdxPx], 
                     evt.StdHepP4[in][kGStdHepIdxPy], 
                     evt.StdHepP4[in][kGStdHepIdxPz], 
                     evt.StdHepP4[in][kGStdHepIdxE]);

  TLorentzVector q = p4v - p4l;
  TLorentzVector w = p4n + q;;

  return w.Mag();
}
//_______________________________________________________________________________________
double T2KGenieUtils::KineQ2(const ND::GRooTrackerVtx & evt) 
{
// Momentum transfer, Q2
//
  int iv = T2KGenieUtils::IdxNu (evt);              
  int il = T2KGenieUtils::IdxFsl(evt);

  if(iv==-1 || il==-1) return 0;

  // get the neutrino, final state prim lepton & hit nucleon 4-momenta
  TLorentzVector p4v(evt.StdHepP4[iv][kGStdHepIdxPx], 
                     evt.StdHepP4[iv][kGStdHepIdxPy],
                     evt.StdHepP4[iv][kGStdHepIdxPz], 
                     evt.StdHepP4[iv][kGStdHepIdxE]);
  TLorentzVector p4l(evt.StdHepP4[il][kGStdHepIdxPx], 
                     evt.StdHepP4[il][kGStdHepIdxPy], 
                     evt.StdHepP4[il][kGStdHepIdxPz], 
                     evt.StdHepP4[il][kGStdHepIdxE]);

  TLorentzVector q = p4v - p4l;

  return (-1.*q.M2());

}
//_______________________________________________________________________________________
int T2KGenieUtils::ResId(const ND::GRooTrackerVtx & evt) 
{
  string code = evt.EvtCode->GetString().Data();

  // do not change order of below if statements as
  // they only work in current order, otherwise a 'code.find("RES;res:1")'  
  // will return true when it should be 'code.find("RES;res:1?")'

  if (code.find("RES;res:0" ) != string::npos) return  0; // kP33_1232
  if (code.find("RES;res:10") != string::npos) return 10; // kP13_1720
  if (code.find("RES;res:11") != string::npos) return 11; // kF15_1680
  if (code.find("RES;res:12") != string::npos) return 12; // kP31_1910
  if (code.find("RES;res:13") != string::npos) return 13; // kP33_1920
  if (code.find("RES;res:14") != string::npos) return 14; // kF35_1905
  if (code.find("RES;res:15") != string::npos) return 15; // kF37_1950
  if (code.find("RES;res:16") != string::npos) return 16; // kP11_1710
  if (code.find("RES;res:17") != string::npos) return 17; // kF17_1970
  if (code.find("RES;res:1" ) != string::npos) return  1; // kS11_1535
  if (code.find("RES;res:2" ) != string::npos) return  2; // kD13_1520
  if (code.find("RES;res:3" ) != string::npos) return  3; // kS11_1650
  if (code.find("RES;res:4" ) != string::npos) return  4; // kD13_1700
  if (code.find("RES;res:5" ) != string::npos) return  5; // kD15_1675
  if (code.find("RES;res:6" ) != string::npos) return  6; // kS31_1620
  if (code.find("RES;res:7" ) != string::npos) return  7; // kD33_1700
  if (code.find("RES;res:8" ) != string::npos) return  8; // kP11_1440
  if (code.find("RES;res:9" ) != string::npos) return  9; // kP33_1600

  return -1;
}
//_______________________________________________________________________________________
int T2KGenieUtils::PdgNu(const ND::GRooTrackerVtx & evt) 
{
// Return the neutrino PDG code
//
  int iv = T2KGenieUtils::IdxNu(evt);
  if(iv<0) return 0;
  
  return evt.StdHepPdg[iv];
}
//_______________________________________________________________________________________
int T2KGenieUtils::PdgFsl(const ND::GRooTrackerVtx & evt) 
{
// Return the final state primarly lepton PDG code
//
  int il = T2KGenieUtils::IdxNu(evt);
  if(il<0) return 0;
  
  return evt.StdHepPdg[il];
}
//_______________________________________________________________________________________
int T2KGenieUtils::PdgTgt(const ND::GRooTrackerVtx & evt) 
{
// Return the hit target pdg code
//
  int it = T2KGenieUtils::IdxTgt(evt);
  if(it<0) return 0;
  
  return evt.StdHepPdg[it];
}
//_______________________________________________________________________________________
int T2KGenieUtils::PdgHitNuc(const ND::GRooTrackerVtx & evt) 
{
// Return the hit nucleon pdg code (if a nucleon was hit)
//
  int idx = T2KGenieUtils::IdxHitNuc(evt);
  if(idx<0) return 0;

  return evt.StdHepPdg[idx];
}
//_______________________________________________________________________________________
int T2KGenieUtils::TgtZ(const ND::GRooTrackerVtx & evt) 
{
// Return the hit target Z 
//
  int tgtpdg = T2KGenieUtils::PdgTgt(evt);

  if(tgtpdg == genie::kPdgProton ) return 1;
  if(tgtpdg == genie::kPdgNeutron) return 0;

  // nuclear pdg codes of the form: 10LZZZAAAI
  // extract ZZZ
  if(tgtpdg >  1000000000 ) {
   int Z = (tgtpdg/10000) - 1000*(tgtpdg/10000000); // don't factor out - bit shifts!
   return Z;
  }

  return 0;
}
//_______________________________________________________________________________________
int T2KGenieUtils::TgtA(const ND::GRooTrackerVtx & evt) 
{
// Return the hit target A
//
  int tgtpdg = T2KGenieUtils::PdgTgt(evt);

  if(tgtpdg == genie::kPdgProton ) return 1;
  if(tgtpdg == genie::kPdgNeutron) return 1;

  // nuclear pdg codes of the form: 10LZZZAAAI
  // extract AAA
  if(tgtpdg >  1000000000 ) {
   int A = (tgtpdg/10) - 1000*(tgtpdg/10000); // don't factor out - bit shifts!
   return A;
  }

  return 0;
}
//_______________________________________________________________________________________
int T2KGenieUtils::NParticles(const ND::GRooTrackerVtx & evt) 
{
  int N=0;
  while(evt.StdHepPdg[N] != 0) {
    N++;
  }
  return N;
}
//_______________________________________________________________________________________
TLorentzVector T2KGenieUtils::P4Idx(const ND::GRooTrackerVtx & evt, int idx){
// Return the neutrino P4
//
  TLorentzVector p4v(0.0,0.0,0.0,0.0);
  if(idx>=0){
    p4v.SetXYZT(evt.StdHepP4[idx][kGStdHepIdxPx],
                evt.StdHepP4[idx][kGStdHepIdxPy],
                evt.StdHepP4[idx][kGStdHepIdxPz],
                evt.StdHepP4[idx][kGStdHepIdxE]);
  }
  return p4v;
}
//_______________________________________________________________________________________
TLorentzVector T2KGenieUtils::P4Nu(const ND::GRooTrackerVtx & evt){
  int iv = T2KGenieUtils::IdxNu(evt);              
  return T2KGenieUtils::P4Idx(evt, iv);
}
//_______________________________________________________________________________________
double T2KGenieUtils::ENu(const ND::GRooTrackerVtx & evt) 
{
// Return the neutrino energy
//
  int iv = T2KGenieUtils::IdxNu(evt);              
  if(iv<0) return 0;

  return evt.StdHepP4[iv][kGStdHepIdxE];
}
//_______________________________________________________________________________________
double T2KGenieUtils::ENu_nrf(const ND::GRooTrackerVtx & evt)
{
// Return the neutrino energy at the hit nucleon rest frame
//
  int iN = T2KGenieUtils::IdxHitNuc(evt);
  if(iN<0) return T2KGenieUtils::ENu(evt); // no hit nuc - same as Ev at LAB

  int iv = T2KGenieUtils::IdxNu(evt);

  double bx = evt.StdHepP4[iN][kGStdHepIdxPx] /  evt.StdHepP4[iN][kGStdHepIdxE];
  double by = evt.StdHepP4[iN][kGStdHepIdxPy] /  evt.StdHepP4[iN][kGStdHepIdxE];
  double bz = evt.StdHepP4[iN][kGStdHepIdxPz] /  evt.StdHepP4[iN][kGStdHepIdxE];

  TLorentzVector p4v(evt.StdHepP4[iv][kGStdHepIdxPx],
                     evt.StdHepP4[iv][kGStdHepIdxPy],
                     evt.StdHepP4[iv][kGStdHepIdxPz],
                     evt.StdHepP4[iv][kGStdHepIdxE]);

  p4v.Boost(-bx,-by,-bz);

  return p4v.Energy();
}
//_______________________________________________________________________________________
double T2KGenieUtils::ENu_reco(const ND::GRooTrackerVtx & evt) 
{
// Return the reconstructed-like neutrino energy for QELCC
// Ev_reco = (M*El - ml^2/2) / (M-El+pl*costheta)
//
  int iv = T2KGenieUtils::IdxNu(evt);              
  if(iv<0) return 0;
  int il = T2KGenieUtils::IdxFsl(evt);        
  if(il<0) return 0;

  double El     = T2KGenieUtils::E        (evt,il);
  double pl     = T2KGenieUtils::Momentum (evt,il);
  double costhl = T2KGenieUtils::CosTheta (evt,il);
  double M      = genie::constants::kNucleonMass;
  double ml2    = TMath::Power(genie::constants::kMuonMass, 2.);

  double Ev_reco = (M*El - 0.5*ml2) / (M - El + pl*costhl);
  return Ev_reco;
}
//_______________________________________________________________________________________
double T2KGenieUtils::EFsl(const ND::GRooTrackerVtx & evt) 
{
// Return the final state primary lepton energy
//
  int il = T2KGenieUtils::IdxFsl(evt);              
  return T2KGenieUtils::E(evt,il);
}
//_______________________________________________________________________________________
double T2KGenieUtils::MomentumFsl(const ND::GRooTrackerVtx & evt) 
{
// Return the final state primary lepton momentum
//
  int il = T2KGenieUtils::IdxFsl(evt);              
  return T2KGenieUtils::Momentum(evt,il);
}
//_______________________________________________________________________________________
double T2KGenieUtils::Momentum(const ND::GRooTrackerVtx & evt, int i) 
{
// Return the final state ith stdhep entry momentum
  
  int Np = T2KGenieUtils::NParticles(evt);
  if(i >= Np || i < 0){
    return 0.0; 
  }

  return TMath::Sqrt(
            TMath::Power(evt.StdHepP4[i][kGStdHepIdxPx], 2) +
            TMath::Power(evt.StdHepP4[i][kGStdHepIdxPy], 2) +
            TMath::Power(evt.StdHepP4[i][kGStdHepIdxPz], 2)
	    );
}
//_______________________________________________________________________________________
double T2KGenieUtils::E(const ND::GRooTrackerVtx & evt, int i) 
{
// Return the final state ith stdhep entry momentum
  
  int Np = T2KGenieUtils::NParticles(evt);
  if(i >= Np || i < 0){
    return 0.0; 
  }

  return evt.StdHepP4[i][kGStdHepIdxE];
}
//_______________________________________________________________________________________
double T2KGenieUtils::KE(const ND::GRooTrackerVtx & evt, int i) 
{
  double E  = T2KGenieUtils::E(evt,i);
  double P  = T2KGenieUtils::Momentum(evt,i);
  double KE = E - TMath::Sqrt(TMath::Max(0., E*E - P*P));

  return KE;
}
//_______________________________________________________________________________________
double T2KGenieUtils::CosTheta(const ND::GRooTrackerVtx & evt, int i) 
{
// Return the i^th particle cos(theta) with respect to the neutrino direction
//
  int Np = T2KGenieUtils::NParticles(evt);
  if(i >= Np || i < 0){
    return 0.0; 
  }

  int iv = T2KGenieUtils::IdxNu(evt);  
  if(iv<0) return 0;

  TVector3 p3v(evt.StdHepP4[iv][kGStdHepIdxPx],
               evt.StdHepP4[iv][kGStdHepIdxPy],
               evt.StdHepP4[iv][kGStdHepIdxPz]);

  TVector3 p3i(evt.StdHepP4[i][kGStdHepIdxPx],
               evt.StdHepP4[i][kGStdHepIdxPy],
               evt.StdHepP4[i][kGStdHepIdxPz]);

  double costh = p3v.Dot(p3i)/(p3v.Mag()*p3i.Mag()); // <-- cos(theta) = a.b/(|a||b|) 

  return costh;
}
//_______________________________________________________________________________________
double T2KGenieUtils::FermiMomentum(const ND::GRooTrackerVtx & evt) 
{
// Return the hit nucleon Fermi momentum
//
  int in = T2KGenieUtils::IdxHitNuc(evt);              
  if(in<0) return 0;

  return TMath::Sqrt(
            TMath::Power(evt.StdHepP4[in][kGStdHepIdxPx], 2) +
            TMath::Power(evt.StdHepP4[in][kGStdHepIdxPy], 2) +
            TMath::Power(evt.StdHepP4[in][kGStdHepIdxPz], 2)
         );
}
//_______________________________________________________________________________________
int T2KGenieUtils::IdxNu(const ND::GRooTrackerVtx & /*evt*/) 
{
// Return the input neutrino index in the STDHEP array
// Always at 0
//
  return 0;
}
//_______________________________________________________________________________________
int T2KGenieUtils::IdxTgt(const ND::GRooTrackerVtx & /*evt*/) 
{
// Return the input neutrino index in the STDHEP array
// Always at 1
//
  return 1;
}
//_______________________________________________________________________________________
int T2KGenieUtils::IdxFsl(const ND::GRooTrackerVtx & evt) 
{
// Return the final state prim lepton index in the STDHEP array
// Look for the input neutrino's daughter
//
  return evt.StdHepFd[0]; 
}
//_______________________________________________________________________________________
int T2KGenieUtils::IdxHitNuc(const ND::GRooTrackerVtx & evt) 
{
// Return the hit nucleon index in the STDHEP array
// Look for particle at idx 1 for interactions off free nucleon targets
// Look at the daughters ofparticle at idx 1 for interactions off nuclear targets

  int pdg = evt.StdHepPdg[1];
  if(pdg == genie::kPdgProton || pdg == genie::kPdgNeutron) return 1;

  for(int i = evt.StdHepFd[1]; i <= evt.StdHepLd[1]; i++) {
   if(i<=0) continue;
   if(evt.StdHepStatus[i] == genie::kIStNucleonTarget) return i;
  }

  return -1;
}
#endif // __T2KRW_OAANALYSIS_ENABLED__
//_______________________________________________________________________________________
#endif // __T2KRW_GENIE_ENABLED__ 
 
