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

#include "T2KGEANTUtils.h"
#include "T2KSyst.h"

using std::cerr;
using std::cout;
using std::endl;

using namespace t2krew;

//#define _DEBUG_GEANT_UTILS__

#ifdef __T2KRW_GEANT_ENABLED__
//_______________________________________________________________________________________
geant::rew::GEANTEvent * T2KGEANTUtils::GetGEANTEvent(const SK::SK__h1 * sktree)
{
  /*
          sktree->iflgscnd[ig]:
C                Currently following convention defined in $SKDETSIM/apscndry.h
C                     (*100 for SKDETSIM non-NEFFECT interactions e.g. elastic SGPIEL;
C                          +0 Free Hydrogen, +1 Oxygen)
C                  -1 : ESCAPE
C                   0 : INITIAL (or unmatched parent vertex if I>1)
C                   1 :
C                   2 : 
C                   3 : ABSORPTION
C                   4 : CHARGE EXCHANGE
C                   5 : 
C                   6 : 
C                   7 : HADRON PRODUCTION 
C                   8 : QUASI-ELASTIC SCATTER
C                   9 : FORWARD (ELASTIC-LIKE) SCATTER
C
  */

  geant::rew::GEANTEvent * fDummyGEANTEvent = NULL;
  if(!sktree){
    cout << "Cannot convert a null SK__h1 tree to GEANT event!" << endl;
    return fDummyGEANTEvent;
  } 

  fDummyGEANTEvent = new geant::rew::GEANTEvent();

  
  for (int ig=0; ig<sktree->nscndprt; ig++) {

    // Only store events that use new pion xsec model in SKDETSIM (sgnuceff)
    // Warning: iflgscnd==7 was not correctly stored for pions produced from 
    //          hadron production interactions in any existing MC production
    //          but this is OK since hadron production cross section reweighting
    //          is not implemented yet.
    if (!sktree->iflgscnd[ig]) continue;
    
    geant::rew::GInt fDummyGInt;
    
    for (int j=0; j<3; j++) {
      
      //fDummyGInt.pos[j] = sktree->vtxscnd[ig][j];
      
      fDummyGInt.mom_in[j] = sktree->pprnt[ig][j];
      fDummyGInt.mom_out[j] = sktree->pscnd[ig][j];
      
    }
    
    fDummyGInt.pid_in = sktree->iprntprt[ig];
    fDummyGInt.pid_out = sktree->iprtscnd[ig];
    fDummyGInt.targetA = sktree->Numatom;

    if (sktree->iflgscnd[ig]==3) fDummyGInt.flg = geant::rew::pionNuclABS;

    else if (sktree->iflgscnd[ig]==8 || sktree->iflgscnd[ig]==9) fDummyGInt.flg = geant::rew::pionNuclQEL;

    else if (sktree->iflgscnd[ig]==4) {
      if (fDummyGInt.pid_in==-fDummyGInt.pid_out)
	fDummyGInt.flg = geant::rew::pionNuclDCX;
      else if (fDummyGInt.pid_out==111) 
	fDummyGInt.flg = geant::rew::pionNuclCXL;
      else {
	cout << "T2KGEANTUtils::GetGEANTEvent() Error: PID = " << fDummyGInt.pid_out 
	     << " not expected for CX reaction" << endl;
	exit (-1);
      }	
    }
    else if (sktree->iflgscnd[ig]==7) continue;

    else if (sktree->iflgscnd[ig]==901) fDummyGInt.flg = geant::rew::pionNuclELA;

    else if (sktree->iflgscnd[ig]==900) fDummyGInt.flg = geant::rew::pionFreeQEL;

    else if (sktree->iflgscnd[ig]==400) fDummyGInt.flg = geant::rew::pionFreeCXL;

    else {
      cout << "T2KGEANTUtils::GetGEANTEvent() Error: Unknown iflgscnd = " << sktree->iflgscnd[ig] << endl;
      exit (-1);
    }	
    
    (fDummyGEANTEvent->int_list).push_back (fDummyGInt);
    
  }
  fDummyGEANTEvent->CalcTheta(); 
 
  return fDummyGEANTEvent;
  
}

//_______________________________________________________________________________________
geant::rew::GEANTEvent * T2KGEANTUtils::GetGEANTEvent   (TTree * a_intree, int entry)
{
  geant::rew::GEANTEvent * fDummyGEANTEvent = NULL;

  return fDummyGEANTEvent;
}


//_______________________________________________________________________________________
geant::rew::GEANTSyst_t T2KGEANTUtils::T2KtoGEANTSyst(T2KSyst_t syst)
// map T2KSyst_t to the corresponding GEANT GEANTSyst_t so that we can call functions
// from GEANT
{
  switch(syst) {
    
  case ( kGEANT_PionNuclXSecABS ) : return geant::rew::kPionNuclXSecTwkDial_ABS;   break;
  case ( kGEANT_PionNuclXSecQEL ) : return geant::rew::kPionNuclXSecTwkDial_QEL;   break;
  case ( kGEANT_PionNuclXSecCXL ) : return geant::rew::kPionNuclXSecTwkDial_CXL;   break;
  case ( kGEANT_PionNuclXSecDCX ) : return geant::rew::kPionNuclXSecTwkDial_DCX;   break;
//case ( kGEANT_PionNuclXSecHAD ) : return geant::rew::kPionNuclXSecTwkDial_HAD;   break;

  case ( kGEANT_PionFreeXSecQEL ) : return geant::rew::kPionFreeXSecTwkDial_QEL;   break;
  case ( kGEANT_PionFreeXSecCXL ) : return geant::rew::kPionFreeXSecTwkDial_CXL;   break;
//case ( kGEANT_PionFreeXSecHAD ) : return geant::rew::kPionFreeXSecTwkDial_HAD;   break;

  case ( kGEANT_PionXSecTbl ) : return geant::rew::kPionXSecTwkDial_Tbl;    break;
    
  default:
    return geant::rew::kNullSystematic;
    break;
  }
  return geant::rew::kNullSystematic;
}
//_______________________________________________________________________________________
#ifdef __T2KRW_OAANALYSIS_ENABLED__
//_______________________________________________________________________________________
geant::rew::GEANTEvent * T2KGEANTUtils::GetGEANTEvent(const ND::RooTrackerVtxBase * vtx)
{

  geant::rew::GEANTEvent * fDummyGEANTEvent = NULL;
	    
  return fDummyGEANTEvent;
  
}
#endif // __T2KRW_OAANALYSIS_ENABLED__
//_______________________________________________________________________________________
#endif // __T2KRW_GEANT_ENABLED__ 
 
