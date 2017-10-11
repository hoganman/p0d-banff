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

#include "T2KSKUtils.h"
#include "T2KSyst.h"

#ifdef __T2KRW_NEUT_ENABLED__
#include "necardC.h"
#include "neutmodelC.h"
#include "neutparamsC.h"
#include "neworkC.h"
#include "fsihistC.h"
#endif

using std::cerr;
using std::cout;
using std::endl;

using namespace t2krew;

//#define _DEBUG_NEUT_UTILS__

#ifdef __T2KRW_NEUT_ENABLED__
//_______________________________________________________________________________________
void T2KSKUtils::fill_neut_commons(const SK::SK__h1 * sktree)
{

  // fsihist.h
  fsihist_.nvert = (int)sktree->Nvertsk;
  for (int ivert=0; ivert<sktree->Nvertsk; ivert++) {
    fsihist_.iflgvert[ivert] = sktree->Iflgvertsk[ivert];
    for (int j=0; j<3; j++)
      fsihist_.posvert[ivert][j] = (float)sktree->Posvertsk[ivert][j];
  }
  
  fsihist_.nvcvert = sktree->Nvcvertsk;
  for (int ip=0; ip<sktree->Nvcvertsk; ip++) {
    fsihist_.abspvert[ip] = (float)sktree->Abspvertsk[ip];
    fsihist_.abstpvert[ip] = (float)sktree->Abstpvertsk[ip];
    fsihist_.ipvert[ip] = sktree->Ipvertsk[ip];
    fsihist_.iverti[ip] = sktree->Ivertisk[ip];
    fsihist_.ivertf[ip] = sktree->Ivertfsk[ip];
    for (int j=0; j<3; j++)
      fsihist_.dirvert[ip][j] = (float)sktree->Dirvertsk[ip][j];
  }

  fsihist_.fsiprob = sktree->Fsiprobsk;

  // Set target nucleus type (water only for SK, this will need to 
  // be modified if OD sample or more complicated geometry is used, 
  // will probably also need to create a new variable in SKDETSIM 
  // which stores the current material)  
  neuttarget_.numbndn =  8; //sktree->Numbndn;
  neuttarget_.numbndp =  8; //sktree->Numbndp;
  neuttarget_.numfrep =  2; //sktree->Numfrep;
  neuttarget_.numatom =  16; //sktree->Numatom; 
  posinnuc_.ibound    =  1; //sktree->Ibound; 

}


//_______________________________________________________________________________________
neut::rew::NSyst_t T2KSKUtils::T2KtoNSyst(T2KSyst_t syst)
// map T2KSyst_t to the corresponding NEUT NSyst_t so that we can call functions
// from NEUT
{
  switch(syst) {

    case ( kNCasc_FrCExLow_pi    ) : return neut::rew::kCascTwkDial_FrCExLow_pi;    break;
    case ( kNCasc_FrInelLow_pi   ) : return neut::rew::kCascTwkDial_FrInelLow_pi;   break;
    case ( kNCasc_FrAbs_pi       ) : return neut::rew::kCascTwkDial_FrAbs_pi;       break;
    case ( kNCasc_FrPiProd_pi    ) : return neut::rew::kCascTwkDial_FrPiProd_pi;    break;
    case ( kNCasc_FrCExHigh_pi   ) : return neut::rew::kCascTwkDial_FrCExHigh_pi;   break;
    case ( kNCasc_FrInelHigh_pi  ) : return neut::rew::kCascTwkDial_FrInelHigh_pi;  break;

    default:
      return neut::rew::kNullSystematic;
      break;
  }
  return neut::rew::kNullSystematic;
}
//_______________________________________________________________________________________

#endif // __T2KRW_NEUT_ENABLED__ 
 
