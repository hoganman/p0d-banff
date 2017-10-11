//____________________________________________________________________________
/*

 For the class documentation see the corresponding header file.

 Authors:
   Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
   STFC, Rutherford Appleton Laboratory

   Jim Dobson <J.Dobson07 \at imperial.ac.uk>
   Imperial College London

   Patrick de Perio <pdeperio \at physics.utoronto.ca>
   University of Toronto

*/
//____________________________________________________________________________
#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLorentzVector.h>

#include "T2KNIWGUtils.h"
#include "T2KSyst.h"

#ifdef __T2KRW_NIWG_ENABLED__
#endif

#ifdef __T2KRW_NEUT_ENABLED__
#include "neutrootTreeSingleton.h"
#endif

using std::cerr;
using std::cout;
using std::endl;

using namespace t2krew;

//#define _DEBUG_NIWG_UTILS__ //!!

#ifdef __T2KRW_NIWG_ENABLED__
//_______________________________________________________________________________________
niwg::rew::NIWGEvent * T2KNIWGUtils::GetNIWGEvent(const SK::SK__h1 * sktree)
{

  niwg::rew::NIWGEvent * fDummyNIWGEvent = NULL;
  if(!sktree){
    cout << "Cannot convert a null SK__h1 tree to NIWG event!" << endl;
    return fDummyNIWGEvent;
  } 

  fDummyNIWGEvent = new niwg::rew::NIWGEvent();
  fDummyNIWGEvent->detid = 1;   // SK
  fDummyNIWGEvent->neutmode = sktree->mode;
  fDummyNIWGEvent->recenu_ccqe_sk = sktree->fRecEnu; // (GeV)
  fDummyNIWGEvent->targetA = sktree->Numatom;
  if (sktree->Ibound==0) fDummyNIWGEvent->targetA = 1;//RT: identifies as H, rather than O16

  // Fill initial particle stack
  for (int ip=0; ip<sktree->Npvc; ip++) {
    
    niwg::rew::NIWGPartStack fDummyPartStack;

    double mass = GetPDGMass(sktree->Ipvc[ip]); 
    
    //double E_old = 0;
    //for (int i=0; i<3; i++)
    //  E_old += pow(sktree->pnu[ip]*sktree->dirnu[ip][i],2);
    //E_old += pow(mass,2);
    //E_old = sqrt(E_old);
    
    double E = 0;
    E += pow(sktree->Abspvc[ip]/1000,2);
    E += pow(mass,2);
    E = sqrt(E);

    //cout << ip << " E: " << E_old << " " << E << endl;

    //(fDummyPartStack.p).SetPxPyPzE(sktree->pnu[ip]*sktree->dirnu[ip][0],
    //				   sktree->pnu[ip]*sktree->dirnu[ip][1],
    //				   sktree->pnu[ip]*sktree->dirnu[ip][2],
    //				   E_old);
    //(fDummyPartStack.p).Print();
    
    (fDummyPartStack.p).SetPxPyPzE(sktree->Pvc[ip][0]/1000,
				   sktree->Pvc[ip][1]/1000,
				   sktree->Pvc[ip][2]/1000,
				   E);
    //(fDummyPartStack.p).Print();
    
    fDummyPartStack.pdg = sktree->Ipvc[ip];
    fDummyPartStack.chase = sktree->Ichvc[ip];
    fDummyPartStack.parent = sktree->Iorgvc[ip]-1;  // WARNING: this needs to be tested with an SK file

    (fDummyNIWGEvent->part_stack).push_back(fDummyPartStack);    
  }
  fDummyNIWGEvent->CalcKinematics();

#ifdef _DEBUG_NIWG_UTILS__
  fDummyNIWGEvent->Print();
#endif
  
  return fDummyNIWGEvent;
  
}

#ifdef __T2KRW_NEUT_ENABLED__
//_______________________________________________________________________________________
niwg::rew::NIWGEvent * T2KNIWGUtils::GetNIWGEvent(NeutVect* nvect)
{
  niwg::rew::NIWGEvent * fDummyNIWGEvent = NULL;

  fDummyNIWGEvent = new niwg::rew::NIWGEvent();
  fDummyNIWGEvent->detid = 1;   // MiniBooNE (apply CCQE LowE variations)
  fDummyNIWGEvent->neutmode = nvect->Mode;
  fDummyNIWGEvent->targetA = nvect->TargetA;
  fDummyNIWGEvent->recenu_ccqe_sk = -1;
  if (nvect->Ibound==0) fDummyNIWGEvent->targetA = 1;//RT: identifies as H, rather than O16

  // Fill initial particle stack
  for (int ip=0; ip<nvect->Npart(); ++ip) {

    niwg::rew::NIWGPartStack fDummyPartStack;

    fDummyPartStack.p = (nvect->PartInfo(ip)->fP)*0.001;  // Convert to GeV

    fDummyPartStack.pdg = nvect->PartInfo(ip)->fPID;
    fDummyPartStack.chase = nvect->PartInfo(ip)->fIsAlive;
    fDummyPartStack.parent = nvect->ParentIdx(ip)-1;       // WARNING: this needs to be tested with a NeutRoot file

    fDummyNIWGEvent->part_stack.push_back(fDummyPartStack);
  }
  fDummyNIWGEvent->CalcKinematics();

  return fDummyNIWGEvent;
}
#endif // neut enabled

//_______________________________________________________________________________________
niwg::rew::NIWGEvent * T2KNIWGUtils::GetNIWGEvent   (TTree * a_intree, int entry)
{
#ifdef __T2KRW_NEUT_ENABLED__
  // WARNING: This has only been implemented for a neuttree and not GENIE
  // This should be kept in sync with T2KNeutUtils::fill_neut_commons(TTree)

  // A simple test case assuming we have a "neuttree" tree.
  TBranch *br_neutvect = a_intree->GetBranch("vectorbranch");
  NeutVect *nvect = NULL;
  NeutrootTreeSingleton * neutrootTreeObj;

  if(br_neutvect){

    neutrootTreeObj = NeutrootTreeSingleton::Instance(a_intree);
    nvect = neutrootTreeObj->GetNeutVectAddress();

  } else {
    cout << "Error: T2KNIWGUtils::GetNIWGEvent(TTree) cannot find branch \"vectorbranch\". Are you using a neutroot generated file?" << endl;
    exit (-1);
  }
  neutrootTreeObj->GetEntry(entry);

  return T2KNIWGUtils::GetNIWGEvent(nvect);
#else
  return NULL;
#endif
}





//_______________________________________________________________________________________
double T2KNIWGUtils::GetPDGMass(int ID) {

//       Returns particle mass in GeV/c^2
//
//       Taken from ${NEUT_ROOT}/src/skmcsvc/mcmass.F
//
//       PARTICLE       ID
//
//       GAMMA          22
//       Z              23      
//       W              24
//     
//       E-             11
//       MU-            13
//       TAU-           15
//       NEU-E          12
//       NEU-MU         14
//       NEU-TAU        16
//
//       PI+           211
//       PI0           111
//       ETA           221
//       ETA'          331
//       RO+(770)      213
//       RO0(770)      113
//       OMEGA(783)    223
//       a1(1260)    20113,20213
//       K+            321
//       K0            311
//       K0S           310
//       K0L           130
//       K*+(892)      323
//       K*0(892)      313
//  
//       P            2212
//       N            2112
//       LAMBDA       3122
//       
//     Deuteron        100045
//     Tritium         100046
//     Alpha           100047
//     Geantino        100048
//     He3             100049
//     O16             100069
//
//
  double XMASS=0.0;
  int ID2=abs(ID);
  if (ID2==  11) XMASS=0.511      ;
  else if (ID2==  12) XMASS=0.0	;
  else if (ID2==  13) XMASS=105.658	;
  else if (ID2==  14) XMASS=0.0	;
  else if (ID2==  15) XMASS=1776.99	;
  else if (ID2==  16) XMASS=0.0	;
   
  else if (ID2==  22) XMASS=0.0	;
  else if (ID2==  23) XMASS=91.173E+3	;
  else if (ID2==  24) XMASS=80.22E+3	;
  
  else if (ID2== 211) XMASS=139.568	;
  else if (ID2== 111) XMASS=134.973	;
  else if (ID2== 221) XMASS=548.8	;
  else if (ID2== 213) XMASS=770.0	;
  else if (ID2== 113) XMASS=770.0	;
  else if (ID2== 223) XMASS=782.0	;
  else if (ID2==20113) XMASS=1260.0	;
  else if (ID2==20213) XMASS=1260.0	;
  else if (ID2== 321) XMASS=493.646	;
  else if (ID2== 311) XMASS=497.671	;
  else if (ID2== 310) XMASS=497.671	;
  else if (ID2== 130) XMASS=497.671	;
  else if (ID2== 323) XMASS=892.1	;
  else if (ID2== 313) XMASS=892.1	;
  else if (ID2== 331) XMASS=957.77	;
    
  else if (ID2==2212) XMASS=938.272	;
  else if (ID2==2112) XMASS=939.566	;
  else if (ID2==3122) XMASS=1115.68	;
    
  else if (ID2==100045) XMASS=1875.61	;
  else if (ID2==100046) XMASS=2808.0	;
  else if (ID2==100047) XMASS=3727.42	;
  else if (ID2==100048) XMASS=0.0	;
  else if (ID2==100049) XMASS=2814.48	;
  else if (ID2==100069) XMASS=14899.1   ;

  return XMASS/1000.;
}



//_______________________________________________________________________________________
niwg::rew::NIWGSyst_t T2KNIWGUtils::T2KtoNIWGSyst(T2KSyst_t syst)
// map T2KSyst_t to the corresponding NIWG NIWGSyst_t so that we can call functions
// from NIWG
{
  switch(syst) {

  case ( kNIWG2010a_ccqenorm   ) : return niwg::rew::k2010aTwkDial_ccqenorm ; break;
  case ( kNIWG2010a_ccqe       ) : return niwg::rew::k2010aTwkDial_ccqe     ; break;
  case ( kNIWG2010a_ccnue      ) : return niwg::rew::k2010aTwkDial_ccnue    ; break;
  case ( kNIWG2010a_cc1pi      ) : return niwg::rew::k2010aTwkDial_cc1pi    ; break;
  case ( kNIWG2010a_cccoh      ) : return niwg::rew::k2010aTwkDial_cccoh    ; break;
  case ( kNIWG2010a_ccoth      ) : return niwg::rew::k2010aTwkDial_ccoth    ; break;
  case ( kNIWG2010a_nc         ) : return niwg::rew::k2010aTwkDial_nc       ; break;
  case ( kNIWG2010a_nc1pi0     ) : return niwg::rew::k2010aTwkDial_nc1pi0   ; break;
  case ( kNIWG2010a_nccoh      ) : return niwg::rew::k2010aTwkDial_nccoh    ; break;
  case ( kNIWG2010a_ncoth      ) : return niwg::rew::k2010aTwkDial_ncoth    ; break;
  case ( kNIWG2010a_fsi        ) : return niwg::rew::k2010aTwkDial_fsi      ; break;

  case ( kNIWG2011a_CCQESplineE0  ) : return niwg::rew::k2011aTwkDial_CCQESplineE0 ; break;
  case ( kNIWG2011a_CCQESplineE1  ) : return niwg::rew::k2011aTwkDial_CCQESplineE1 ; break;
  case ( kNIWG2011a_CCQESplineE2  ) : return niwg::rew::k2011aTwkDial_CCQESplineE2 ; break;
  case ( kNIWG2011a_CCQESplineE3  ) : return niwg::rew::k2011aTwkDial_CCQESplineE3 ; break;
  case ( kNIWG2011a_CCQESplineE4  ) : return niwg::rew::k2011aTwkDial_CCQESplineE4 ; break;
  case ( kNIWG2011a_CCQESplineE5  ) : return niwg::rew::k2011aTwkDial_CCQESplineE5 ; break;

  case ( kNIWG2011a_CCRESSplineE0 ) : return niwg::rew::k2011aTwkDial_CCRESSplineE0; break;
  case ( kNIWG2011a_CCRESSplineE1 ) : return niwg::rew::k2011aTwkDial_CCRESSplineE1; break;
  case ( kNIWG2011a_CCRESSplineE2 ) : return niwg::rew::k2011aTwkDial_CCRESSplineE2; break;
  case ( kNIWG2011a_CCRESSplineE3 ) : return niwg::rew::k2011aTwkDial_CCRESSplineE3; break;
  case ( kNIWG2011a_CCRESSplineE4 ) : return niwg::rew::k2011aTwkDial_CCRESSplineE4; break;
  case ( kNIWG2011a_CCRESSplineE5 ) : return niwg::rew::k2011aTwkDial_CCRESSplineE5; break;

  case ( kNIWG2011a_NCRESSplineE0 ) : return niwg::rew::k2011aTwkDial_NCRESSplineE0; break;
  case ( kNIWG2011a_NCRESSplineE1 ) : return niwg::rew::k2011aTwkDial_NCRESSplineE1; break;
  case ( kNIWG2011a_NCRESSplineE2 ) : return niwg::rew::k2011aTwkDial_NCRESSplineE2; break;
  case ( kNIWG2011a_NCRESSplineE3 ) : return niwg::rew::k2011aTwkDial_NCRESSplineE3; break;
  case ( kNIWG2011a_NCRESSplineE4 ) : return niwg::rew::k2011aTwkDial_NCRESSplineE4; break;
  case ( kNIWG2011a_NCRESSplineE5 ) : return niwg::rew::k2011aTwkDial_NCRESSplineE5; break;

  case ( kNIWG2012a_eb ) : return niwg::rew::k2012aTwkDial_eb; break;
  case ( kNIWG2012a_pf ) : return niwg::rew::k2012aTwkDial_pf; break;
  case ( kNIWG2012a_dismpishp ) : return niwg::rew::k2012aTwkDial_dismpishp; break;
  case ( kNIWG2012a_sf ) : return niwg::rew::k2012aTwkDial_sf; break;
  case ( kNIWG2012a_ccqeE0 ) : return niwg::rew::k2012aTwkDial_ccqeE0; break;
  case ( kNIWG2012a_ccqeE1 ) : return niwg::rew::k2012aTwkDial_ccqeE1; break;
  case ( kNIWG2012a_ccqeE2 ) : return niwg::rew::k2012aTwkDial_ccqeE2; break;
  case ( kNIWG2012a_cc1piE0 ) : return niwg::rew::k2012aTwkDial_cc1piE0; break;
  case ( kNIWG2012a_cc1piE1 ) : return niwg::rew::k2012aTwkDial_cc1piE1; break;
  case ( kNIWG2012a_nc1piE0 ) : return niwg::rew::k2012aTwkDial_nc1piE0; break;
  case ( kNIWG2012a_nc1pi0E0 ) : return niwg::rew::k2012aTwkDial_nc1pi0E0; break;
  case ( kNIWG2012a_1gamE0 ) : return niwg::rew::k2012aTwkDial_1gamE0; break;
  case ( kNIWG2012a_ccmultipiE0 ) : return niwg::rew::k2012aTwkDial_ccmultipiE0; break;
  case ( kNIWG2012a_ccdisE0 ) : return niwg::rew::k2012aTwkDial_ccdisE0; break;
  case ( kNIWG2012a_cccohE0 ) : return niwg::rew::k2012aTwkDial_cccohE0; break;
  case ( kNIWG2012a_nccohE0 ) : return niwg::rew::k2012aTwkDial_nccohE0; break;
  case ( kNIWG2012a_ncotherE0 ) : return niwg::rew::k2012aTwkDial_ncotherE0; break;
  case ( kNIWG2012a_ccnueE0 ) : return niwg::rew::k2012aTwkDial_ccnueE0; break;
  case ( kNIWG2012a_antinu ) : return niwg::rew::k2012aTwkDial_antinu; break;

  case ( kNIWGDeltaMass_mean ) : return niwg::rew::kDeltaMassTwkDial_mean; break;
  case ( kNIWGDeltaMass_width ) : return niwg::rew::kDeltaMassTwkDial_width; break;

  case ( kNIWG2012a_mbcc1pi_enushp ) : return niwg::rew::k2012aTwkDial_mbcc1pi_enushp; break;

  case ( kNIWG2012a_1piAngle ) : return niwg::rew::k2012aTwkDial_1piAngle; break;
    
  case ( kNIWG2014a_SF_RFG )   : return niwg::rew::k2014aTwkDial_SF_RFG; break;
  case ( kNIWG2014a_pF_C12 )   : return niwg::rew::k2014aTwkDial_pF_C12; break;
  case ( kNIWG2014a_pF_O16 )   : return niwg::rew::k2014aTwkDial_pF_O16; break;
  case ( kNIWG2014a_pF_Al27 )  : return niwg::rew::k2014aTwkDial_pF_Al27; break;
  case ( kNIWG2014a_pF_Fe56 )  : return niwg::rew::k2014aTwkDial_pF_Fe56; break;
  case ( kNIWG2014a_pF_Cu63 )  : return niwg::rew::k2014aTwkDial_pF_Cu63; break;
  case ( kNIWG2014a_pF_Zn64 )  : return niwg::rew::k2014aTwkDial_pF_Zn64; break;
  case ( kNIWG2014a_pF_Pb208 ) : return niwg::rew::k2014aTwkDial_pF_Pb208; break;
  case ( kNIWG2014a_Eb_C12 )   : return niwg::rew::k2014aTwkDial_Eb_C12; break;
  case ( kNIWG2014a_Eb_O16 )   : return niwg::rew::k2014aTwkDial_Eb_O16; break;
  case ( kNIWG2014a_Eb_Al27 )  : return niwg::rew::k2014aTwkDial_Eb_Al27; break;
  case ( kNIWG2014a_Eb_Fe56 )  : return niwg::rew::k2014aTwkDial_Eb_Fe56; break;
  case ( kNIWG2014a_Eb_Cu63 )  : return niwg::rew::k2014aTwkDial_Eb_Cu63; break;
  case ( kNIWG2014a_Eb_Zn64 )  : return niwg::rew::k2014aTwkDial_Eb_Zn64; break;
  case ( kNIWG2014a_Eb_Pb208 ) : return niwg::rew::k2014aTwkDial_Eb_Pb208; break;


  case ( kNIWGPiMult_CorrSwitch ) : return niwg::rew::kPiMultTwkDial_CorrSwitch; break;
  
  case ( kNIWGSpectralFunc_corrNormC12 ) : return niwg::rew::kSpectralFunc_corrNormC12; break;
  case ( kNIWGSpectralFunc_pFsfC12 ) : return niwg::rew::kSpectralFunc_pFsfC12 ; break;
  case ( kNIWGSpectralFunc_pFsfC12_smooth ) : return niwg::rew::kSpectralFunc_pFsfC12_smooth ; break;
  case ( kNIWGSpectralFunc_MFwidthC12 ) : return niwg::rew::kSpectralFunc_MFwidthC12 ; break;
  case ( kNIWGSpectralFunc_corrNormO16 ) : return niwg::rew::kSpectralFunc_corrNormO16; break;
  case ( kNIWGSpectralFunc_pFsfO16 ) : return niwg::rew::kSpectralFunc_pFsfO16 ; break;
  case ( kNIWGSpectralFunc_pFsfO16_smooth ) : return niwg::rew::kSpectralFunc_pFsfO16_smooth ; break;
  case ( kNIWGSpectralFunc_MFwidthO16 ) : return niwg::rew::kSpectralFunc_MFwidthO16 ; break;
  case ( kNIWGSpectralFunc_corrNormFe56 ) : return niwg::rew::kSpectralFunc_corrNormFe56; break;
  case ( kNIWGSpectralFunc_pFsfFe56 ) : return niwg::rew::kSpectralFunc_pFsfFe56 ; break;
  case ( kNIWGSpectralFunc_pFsfFe56_smooth ) : return niwg::rew::kSpectralFunc_pFsfFe56_smooth ; break;
  case ( kNIWGSpectralFunc_MFwidthFe56 ) : return niwg::rew::kSpectralFunc_MFwidthFe56 ; break;

  case ( kNIWGMEC_Norm_C12   ) : return niwg::rew::kMECTwkDial_Norm_C12; break;
  case ( kNIWGMEC_Norm_O16   ) : return niwg::rew::kMECTwkDial_Norm_O16; break;
  case ( kNIWGMEC_Norm_Other ) : return niwg::rew::kMECTwkDial_Norm_Other; break;
  case ( kNIWGMEC_q3Cut      ) : return niwg::rew::kMECTwkDial_q3Cut; break;
  case ( kNIWGMEC_Diff       ) : return niwg::rew::kMECTwkDial_Diff; break;

  case ( kNIWGMEC_PDDInterp     ) : return niwg::rew::kMECTwkDial_PDDInterp; break;
  case ( kNIWGMEC_PDDWeight_C12 ) : return niwg::rew::kMECTwkDial_PDDWeight_C12; break;
  case ( kNIWGMEC_PDDWeight_O16 ) : return niwg::rew::kMECTwkDial_PDDWeight_O16; break;
    
  case ( kNIWG_rpaCCQE_norm ) : return niwg::rew::kTwkDial_rpaCCQE_norm; break;
  case ( kNIWG_rpaCCQE_shape ) : return niwg::rew::kTwkDial_rpaCCQE_shape; break;
  
  case ( kNIWG_rpaCCQE_norm_C12 ) : return niwg::rew::kTwkDial_rpaCCQE_norm_C12; break;
  case ( kNIWG_rpaCCQE_shape_C12 ) : return niwg::rew::kTwkDial_rpaCCQE_shape_C12; break;
  case ( kNIWG_rpaCCQE_norm_O16 ) : return niwg::rew::kTwkDial_rpaCCQE_norm_O16; break;
  case ( kNIWG_rpaCCQE_shape_O16 ) : return niwg::rew::kTwkDial_rpaCCQE_shape_O16; break;

  case ( kNIWG_Effective_rpaCCQE_Norm ) : return niwg::rew::kTwkDial_Effective_rpaCCQE_Norm; break;
  case ( kNIWG_Effective_rpaCCQE_U ) : return niwg::rew::kTwkDial_Effective_rpaCCQE_U; break;
  case ( kNIWG_Effective_rpaCCQE_A ) : return niwg::rew::kTwkDial_Effective_rpaCCQE_A; break;
  case ( kNIWG_Effective_rpaCCQE_B ) : return niwg::rew::kTwkDial_Effective_rpaCCQE_B; break;
  case ( kNIWG_Effective_rpaCCQE_C ) : return niwg::rew::kTwkDial_Effective_rpaCCQE_C; break;
  case ( kNIWG_Effective_rpaCCQE_D ) : return niwg::rew::kTwkDial_Effective_rpaCCQE_D; break;
  case ( kNIWG_Effective_rpaCCQE_Unom ) : return niwg::rew::kTwkDial_Effective_rpaCCQE_Unom; break;
  case ( kNIWG_Effective_rpaCCQE_Anom ) : return niwg::rew::kTwkDial_Effective_rpaCCQE_Anom; break;
  case ( kNIWG_Effective_rpaCCQE_Bnom ) : return niwg::rew::kTwkDial_Effective_rpaCCQE_Bnom; break;
  case ( kNIWG_Effective_rpaCCQE_Cnom ) : return niwg::rew::kTwkDial_Effective_rpaCCQE_Cnom; break;
  case ( kNIWG_Effective_rpaCCQE_Dnom ) : return niwg::rew::kTwkDial_Effective_rpaCCQE_Dnom; break;

  case ( kNIWG_Effective_rpaCCQE_Norm_forweight ) : return niwg::rew::kTwkDial_Effective_rpaCCQE_Norm_forweight; break;
  case ( kNIWG_Effective_rpaCCQE_U_forweight ) : return niwg::rew::kTwkDial_Effective_rpaCCQE_U_forweight; break;
  case ( kNIWG_Effective_rpaCCQE_A_forweight ) : return niwg::rew::kTwkDial_Effective_rpaCCQE_A_forweight; break;
  case ( kNIWG_Effective_rpaCCQE_B_forweight ) : return niwg::rew::kTwkDial_Effective_rpaCCQE_B_forweight; break;
  case ( kNIWG_Effective_rpaCCQE_C_forweight ) : return niwg::rew::kTwkDial_Effective_rpaCCQE_C_forweight; break;
  case ( kNIWG_Effective_rpaCCQE_D_forweight ) : return niwg::rew::kTwkDial_Effective_rpaCCQE_D_forweight; break;

   
  case ( kNIWG_protonFSI_bugfix ) : return niwg::rew::kProtonFSI_bugfix; break;
  
  case ( kNIWGHadronMultSwitch ) : return niwg::rew::kHadronMultSwitch; break;

  default:
    return niwg::rew::kNullSystematic;
    break;
  }
  return niwg::rew::kNullSystematic;
}
//_______________________________________________________________________________________
#ifdef __T2KRW_OAANALYSIS_ENABLED__
//_______________________________________________________________________________________
niwg::rew::NIWGEvent * T2KNIWGUtils::GetNIWGEvent(const ND::RooTrackerVtxBase * vtx)
{
  niwg::rew::NIWGEvent * fDummyNIWGEvent = NULL;

  const ND::NRooTrackerVtx * nrootrac_vtx = dynamic_cast<const ND::NRooTrackerVtx*>(vtx);
  const ND::GRooTrackerVtx * grootrac_vtx = dynamic_cast<const ND::GRooTrackerVtx*>(vtx);
  bool from_neut  = !!nrootrac_vtx;
  bool from_genie = !!grootrac_vtx;
  if (!from_neut && !from_genie) {
    cout << "Cannot cast from ND::RooTrackerVtxBase to ND::G(N)RooTrackerVtx objects!" << endl;
    return fDummyNIWGEvent;
  }

  fDummyNIWGEvent = new niwg::rew::NIWGEvent();
  if (from_genie) {
    // Genie stores a string describing what reaction occured in EvtCode.
    // The Neut equivalent integer is stored in G2NeutEvtCode
    //fDummyNIWGEvent->neutmode = (grootrac_vtx->EvtCode->GetString()).Atoi();
    fDummyNIWGEvent->neutmode = grootrac_vtx->G2NeutEvtCode;

    int AA = (int)TgtA(*grootrac_vtx);
    fDummyNIWGEvent->targetA = AA;

    // Fill initial particle stack
    for (int ip=0; ip<grootrac_vtx->StdHepN; ip++) {

      // Skip nucleus
      if (ip==1) continue;

      niwg::rew::NIWGPartStack fDummyPartStack;

      // WARNING: This needs to be checked by GENIE experts
      (fDummyPartStack.p).SetPxPyPzE(grootrac_vtx->StdHepP4[ip][0],
                                     grootrac_vtx->StdHepP4[ip][1],
                                     grootrac_vtx->StdHepP4[ip][2],
                                     grootrac_vtx->StdHepP4[ip][3]);

      fDummyPartStack.pdg = grootrac_vtx->StdHepPdg[ip];

      fDummyPartStack.parent = -1;                          // WARNING: not yet implemented

      (fDummyNIWGEvent->part_stack).push_back(fDummyPartStack);    
    }
    
  } else if (from_neut) {

    fDummyNIWGEvent = new niwg::rew::NIWGEvent();
    fDummyNIWGEvent->neutmode = (nrootrac_vtx->EvtCode->GetString()).Atoi();

    int AA = (int)TgtA(*nrootrac_vtx);
    fDummyNIWGEvent->targetA = AA;

    // Fill initial particle stack
    // Note: Do not use StdHep stack since it skips particles
    //          that did not escape the nucleus
    for (int ip=0; ip<nrootrac_vtx->NEnvc; ip++) {

      niwg::rew::NIWGPartStack fDummyPartStack;
      double mass = GetPDGMass(nrootrac_vtx->NEipvc[ip]);
      double E = mass*mass;
      for (int i=0; i<3; i++) {
        const double p_i = nrootrac_vtx->NEpvc[ip][i]/1000.;
        E += p_i*p_i;
      }
      E = sqrt(E);

      (fDummyPartStack.p).SetPxPyPzE(nrootrac_vtx->NEpvc[ip][0]/1000.,
                                     nrootrac_vtx->NEpvc[ip][1]/1000.,
                                     nrootrac_vtx->NEpvc[ip][2]/1000.,
                                     E);

      fDummyPartStack.pdg = nrootrac_vtx->NEipvc[ip];

      fDummyPartStack.chase = nrootrac_vtx->NEicrnvc[ip];

      // Convert from FORTRAN indexing to C++ indexing with the -1.
      fDummyPartStack.parent = nrootrac_vtx->NEiorgvc[ip]-1;

      (fDummyNIWGEvent->part_stack).push_back(fDummyPartStack);    
    }

  }
  
  // Generator independent variables
  fDummyNIWGEvent->detid    = 0;   // ND
  fDummyNIWGEvent->recenu_ccqe_sk = -1;  // Does not apply to ND


  fDummyNIWGEvent->CalcKinematics();
  
#ifdef _DEBUG_NIWG_UTILS__
  fDummyNIWGEvent->Print();
#endif
	    
  return fDummyNIWGEvent;
  
}

//_______________________________________________________________________________________
//Calculate Q2 from rootracker
double T2KNIWGUtils::Q2(const ND::RooTrackerVtxBase * vtx)
{

  // Really messy way to distinguish between NEUT or GENIE RooTracker

  const ND::NRooTrackerVtx * nrootrac_vtx = dynamic_cast<const ND::NRooTrackerVtx*>(vtx);
  if(nrootrac_vtx == NULL){

    const ND::GRooTrackerVtx * grootrac_vtx = dynamic_cast<const ND::GRooTrackerVtx*>(vtx);
 
    if (grootrac_vtx == NULL){
      cout << "Cannot cast from ND::RooTrackerVtxBase to ND::G(N)RooTrackerVtx objects!" << endl;
      return -1.;
    } else {

      int id = T2KNIWGUtils::IdxFsl(*grootrac_vtx);
      
      TLorentzVector in_lep(grootrac_vtx->StdHepP4[0][0],grootrac_vtx->StdHepP4[0][1],
			    grootrac_vtx->StdHepP4[0][2],grootrac_vtx->StdHepP4[0][3]);
      TLorentzVector out_lep(grootrac_vtx->StdHepP4[id][0],grootrac_vtx->StdHepP4[id][1],
			     grootrac_vtx->StdHepP4[id][2],grootrac_vtx->StdHepP4[id][3]);

      return -(out_lep-in_lep)*(out_lep-in_lep); //(GeV)

    }
  } else {

    int id = T2KNIWGUtils::IdxFsl(*nrootrac_vtx);
    
    TLorentzVector in_lep(nrootrac_vtx->StdHepP4[0][0],nrootrac_vtx->StdHepP4[0][1],
			  nrootrac_vtx->StdHepP4[0][2],nrootrac_vtx->StdHepP4[0][3]);
    TLorentzVector out_lep(nrootrac_vtx->StdHepP4[id][0],nrootrac_vtx->StdHepP4[id][1],
			   nrootrac_vtx->StdHepP4[id][2],nrootrac_vtx->StdHepP4[id][3]);
    
    return -(out_lep-in_lep)*(out_lep-in_lep); //(GeV)
    
  }
}
    
//_______________________________________________________________________________________
//Calculate Q2QE from rootracker
double T2KNIWGUtils::Q2QE(const ND::RooTrackerVtxBase * vtx)
{

  // Really messy way to distinguish between NEUT or GENIE RooTracker
  double mnp = 0.939565-0.220;
  double mp = 0.938272;
  double mmu = 0.105658366;

  const ND::NRooTrackerVtx * nrootrac_vtx = dynamic_cast<const ND::NRooTrackerVtx*>(vtx);
  if(nrootrac_vtx == NULL){

    const ND::GRooTrackerVtx * grootrac_vtx = dynamic_cast<const ND::GRooTrackerVtx*>(vtx);

    if (grootrac_vtx == NULL){
      cout << "Cannot cast from ND::RooTrackerVtxBase to ND::G(N)RooTrackerVtx objects!" << endl;
      return -1.;
    } else {
      
      int id = T2KNIWGUtils::IdxFsl(*grootrac_vtx);
	
      TLorentzVector in_lep(grootrac_vtx->StdHepP4[0][0],grootrac_vtx->StdHepP4[0][1],
			    grootrac_vtx->StdHepP4[0][2],grootrac_vtx->StdHepP4[0][3]);
      TLorentzVector out_lep(grootrac_vtx->StdHepP4[id][0],grootrac_vtx->StdHepP4[id][1],
			     grootrac_vtx->StdHepP4[id][2],grootrac_vtx->StdHepP4[id][3]);
	
      double hoge = sqrt(pow(out_lep(3),2)-mmu*mmu)*(in_lep.Vect())*(out_lep.Vect())/in_lep.Vect().Mag()/out_lep.Vect().Mag();
      double Eqe = (2.*mnp*out_lep(3)-(mnp*mnp+mmu*mmu-mp*mp))/2./(mnp-out_lep(3)+hoge);
      
      return -mmu*mmu+2.*Eqe*(out_lep(3)-hoge); //(GeV)
    }
  } else {

    int id = T2KNIWGUtils::IdxFsl(*nrootrac_vtx);

    TLorentzVector in_lep(nrootrac_vtx->StdHepP4[0][0],nrootrac_vtx->StdHepP4[0][1],
			  nrootrac_vtx->StdHepP4[0][2],nrootrac_vtx->StdHepP4[0][3]);
    TLorentzVector out_lep(nrootrac_vtx->StdHepP4[id][0],nrootrac_vtx->StdHepP4[id][1],
			   nrootrac_vtx->StdHepP4[id][2],nrootrac_vtx->StdHepP4[id][3]);
    
    double hoge = sqrt(pow(out_lep(3),2)-mmu*mmu)*(in_lep.Vect())*(out_lep.Vect())/in_lep.Vect().Mag()/out_lep.Vect().Mag();
    double Eqe = (2.*mnp*out_lep(3)-(mnp*mnp+mmu*mmu-mp*mp))/2./(mnp-out_lep(3)+hoge);
    
    return -mmu*mmu+2.*Eqe*(out_lep(3)-hoge); //(GeV)
  }
}

//_______________________________________________________________________________________
//Calculate Plep from rootracker
double T2KNIWGUtils::Plep(const ND::RooTrackerVtxBase * vtx) {
  // Really messy way to distinguish between NEUT or GENIE RooTracker

  const ND::NRooTrackerVtx * nrootrac_vtx = dynamic_cast<const ND::NRooTrackerVtx*>(vtx);
  if(nrootrac_vtx == NULL) {
    const ND::GRooTrackerVtx * grootrac_vtx = dynamic_cast<const ND::GRooTrackerVtx*>(vtx);
 
    if (grootrac_vtx == NULL) {
      cout << "Cannot cast from ND::RooTrackerVtxBase to ND::G(N)RooTrackerVtx objects!" << endl;
      return -1.;
    } 
    else { //Is GENIE RooTracker
      int id = T2KNIWGUtils::IdxFsl(*grootrac_vtx);
      int lep = grootrac_vtx->StdHepPdg[id];
      
      if (lep == 11 || lep == -11 || lep == 13 || lep == -13 || lep == 15 || lep == -15) { //e- , e+, mu-, mu+, tau-, tau+
         TLorentzVector out_lep(grootrac_vtx->StdHepP4[id][0],grootrac_vtx->StdHepP4[id][1],
                                grootrac_vtx->StdHepP4[id][2],grootrac_vtx->StdHepP4[id][3]);

         return out_lep.Vect().Mag(); //(GeV)
      }
      else { // is a tau or not a lepton
         //cout << "This particle is not a muon" << endl;
         //cout << grootrac_vtx->StdHepPdg[0] <<endl;
         return -999;
      }
    } //end GENIE RooTracker
  }
  else { //Is NEUT RooTracker
    int id = T2KNIWGUtils::IdxFsl(*nrootrac_vtx);
      int lep = nrootrac_vtx->StdHepPdg[id];
    
    if (lep == 11 || lep == -11 || lep == 13 || lep == -13 || lep == 15 || lep == -15) { //e- , e+, mu-, mu+, tau-, tau+
      TLorentzVector out_lep(nrootrac_vtx->StdHepP4[id][0],nrootrac_vtx->StdHepP4[id][1],
                             nrootrac_vtx->StdHepP4[id][2],nrootrac_vtx->StdHepP4[id][3]);
      return out_lep.Vect().Mag(); //(GeV)
      }
    else { //is a tau or not a lepton
       //cout << "This particle is not a muon" << endl;
       //cout << nrootrac_vtx->StdHepPdg[0] <<endl;
       return -999;
    }
  } //end NEUT RooTracker
} //end Pmu function
    
//_______________________________________________________________________________________
//Calculate q0 from rootracker
double T2KNIWGUtils::q0(const ND::RooTrackerVtxBase * vtx) {
  // Really messy way to distinguish between NEUT or GENIE RooTracker

  const ND::NRooTrackerVtx * nrootrac_vtx = dynamic_cast<const ND::NRooTrackerVtx*>(vtx);
  if(nrootrac_vtx == NULL) {
    const ND::GRooTrackerVtx * grootrac_vtx = dynamic_cast<const ND::GRooTrackerVtx*>(vtx);
 
    if (grootrac_vtx == NULL) {
      cout << "Cannot cast from ND::RooTrackerVtxBase to ND::G(N)RooTrackerVtx objects!" << endl;
      return -1.;
    } 
    else { //is GENIE RooTracker
      int id = T2KNIWGUtils::IdxFsl(*grootrac_vtx);
      
      TLorentzVector in_lep(grootrac_vtx->StdHepP4[0][0],grootrac_vtx->StdHepP4[0][1],
			    grootrac_vtx->StdHepP4[0][2],grootrac_vtx->StdHepP4[0][3]);
      TLorentzVector out_lep(grootrac_vtx->StdHepP4[id][0],grootrac_vtx->StdHepP4[id][1],
			     grootrac_vtx->StdHepP4[id][2],grootrac_vtx->StdHepP4[id][3]);

      return std::fabs((out_lep-in_lep).E()); //(GeV)
    } //end GENIE RooTracker
  } 
  else { //is NEUT RooTracker
    int id = T2KNIWGUtils::IdxFsl(*nrootrac_vtx);
    
    TLorentzVector in_lep(nrootrac_vtx->StdHepP4[0][0],nrootrac_vtx->StdHepP4[0][1],
			  nrootrac_vtx->StdHepP4[0][2],nrootrac_vtx->StdHepP4[0][3]);
    TLorentzVector out_lep(nrootrac_vtx->StdHepP4[id][0],nrootrac_vtx->StdHepP4[id][1],
			   nrootrac_vtx->StdHepP4[id][2],nrootrac_vtx->StdHepP4[id][3]);
    
    return std::fabs((out_lep-in_lep).E()); //(GeV)
  } //end NEUT RooTracker
}
    
//_______________________________________________________________________________________
//Calculate q3 from rootracker
double T2KNIWGUtils::q3(const ND::RooTrackerVtxBase * vtx) {
  // Really messy way to distinguish between NEUT or GENIE RooTracker

  const ND::NRooTrackerVtx * nrootrac_vtx = dynamic_cast<const ND::NRooTrackerVtx*>(vtx);
  if(nrootrac_vtx == NULL) {
    const ND::GRooTrackerVtx * grootrac_vtx = dynamic_cast<const ND::GRooTrackerVtx*>(vtx);
 
    if (grootrac_vtx == NULL) {
      cout << "Cannot cast from ND::RooTrackerVtxBase to ND::G(N)RooTrackerVtx objects!" << endl;
      return -1.;
    } 
    else { //is GENIE RooTracker
      int id = T2KNIWGUtils::IdxFsl(*grootrac_vtx);
      
      TLorentzVector in_lep(grootrac_vtx->StdHepP4[0][0],grootrac_vtx->StdHepP4[0][1],
			    grootrac_vtx->StdHepP4[0][2],grootrac_vtx->StdHepP4[0][3]);
      TLorentzVector out_lep(grootrac_vtx->StdHepP4[id][0],grootrac_vtx->StdHepP4[id][1],
			     grootrac_vtx->StdHepP4[id][2],grootrac_vtx->StdHepP4[id][3]);

      return (out_lep-in_lep).Vect().Mag(); //(GeV)
    } //end GENIE RooTracker
  } 
  else { //is NEUT RooTracker
    int id = T2KNIWGUtils::IdxFsl(*nrootrac_vtx);
    
    TLorentzVector in_lep(nrootrac_vtx->StdHepP4[0][0],nrootrac_vtx->StdHepP4[0][1],
			  nrootrac_vtx->StdHepP4[0][2],nrootrac_vtx->StdHepP4[0][3]);
    TLorentzVector out_lep(nrootrac_vtx->StdHepP4[id][0],nrootrac_vtx->StdHepP4[id][1],
			   nrootrac_vtx->StdHepP4[id][2],nrootrac_vtx->StdHepP4[id][3]);
    
    return (out_lep-in_lep).Vect().Mag(); //(GeV)
  } //end NEUT RooTracker
}
    
//_______________________________________________________________________________________
int T2KNIWGUtils::TgtA(const ND::NRooTrackerVtx & evt) 
{
// Return the hit target A
//
  int tgtpdg = evt.StdHepPdg[1]; 

  if(tgtpdg == 2212 ) return 1;
  if(tgtpdg == 2112) return 1;

  // nuclear pdg codes of the form: 10LZZZAAAI
  // extract AAA
  if(tgtpdg >  1000000000 ) {
    int AAA = (tgtpdg/10) - 1000*(tgtpdg/10000); // don't factor out - bit shifts!
    return AAA;
  }

  return 0;
}

//_______________________________________________________________________________________
int T2KNIWGUtils::TgtA(const ND::GRooTrackerVtx & evt) 
{
// Return the hit target A
//
  int tgtpdg = evt.StdHepPdg[1]; 

  if(tgtpdg == 2212 ) return 1;
  if(tgtpdg == 2112) return 1;

  // nuclear pdg codes of the form: 10LZZZAAAI
  // extract AAA
  if(tgtpdg >  1000000000 ) {
    int AAA = (tgtpdg/10) - 1000*(tgtpdg/10000); // don't factor out - bit shifts!
    return AAA;
  }

  return 0;
}

//_______________________________________________________________________________________
int T2KNIWGUtils::IdxFsl(const ND::NRooTrackerVtx & evt) 
{
  // StdHepFd not currently filled for NEUT, so search for first outgoing lepton instead
  for (int i = 3; i < evt.StdHepN; ++i){
    if (abs(evt.StdHepPdg[i]) < 11 || abs(evt.StdHepPdg[i]) > 16) continue;
    return i;
  }
  
  return 0;
}

//_______________________________________________________________________________________
int T2KNIWGUtils::IdxFsl(const ND::GRooTrackerVtx & evt) 
{
  // StdHepFd filled for GENIE, so use that
  return evt.StdHepFd[0];
}

#endif // __T2KRW_OAANALYSIS_ENABLED__
//_______________________________________________________________________________________
#endif // __T2KRW_NIWG_ENABLED__ 
 
