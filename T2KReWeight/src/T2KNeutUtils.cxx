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

 @ Apr 11, 2011 - JD
   Use correct formula in CosTheta 
   
 @ Nov 04, 2014 - Martin Hierholzer
   Added nucleon FSI history

*/
//____________________________________________________________________________
#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLorentzVector.h>

#include "T2KNeutUtils.h"
#include "T2KSyst.h"

#ifdef __T2KRW_NEUT_ENABLED__

#include "NFortFns.h" // Contains all the NEUT common blocks
#include "nefillverC.h"
#include "necardC.h"
#include "neutmodelC.h"
#include "neutparamsC.h"
#include "neworkC.h"
#include "fsihistC.h"
//Comment out as NEUT does not have the necessary proton FSI information yet
//#include "nucleonfsihistC.h"
#include "neutcrsC.h"

#include "neutvect.h"
#include "neutpart.h"
#include "neutfsipart.h"
#include "neutfsivert.h"
//#include "neutnucfsivert.h"
//#include "neutnucfsistep.h"
#include "neutrootTreeSingleton.h"


#include "NModeDefn.h"
#endif

using std::cerr;
using std::cout;
using std::endl;

using namespace t2krew;

//#define _DEBUG_NEUT_UTILS__

#ifdef __T2KRW_NEUT_ENABLED__
//_______________________________________________________________________________________
void T2KNeutUtils::fill_neut_commons_with_defaults() {
  neut::rew::NFortFns *fortFns = neut::rew::NFortFns::Instance();
  fortFns->SetDefaults();
}
//_______________________________________________________________________________________
void T2KNeutUtils::fill_neut_commons(const SK::SK__h1 * sktree)
{
  fill_neut_commons_with_defaults();
  if (sktree->f_sk==1) { // Neutrino MC
 
    // Documentation: See nework.h
    nework_.modene = sktree->mode;
    nework_.numne = sktree->numnu;
  
    for ( int i = 0; i<nework_.numne; i++ ) { 
      nework_.ipne[i] = sktree->ipnu[i];
      for ( int j = 0 ; j < 3 ; j++ ){
	nework_.pne[i][j] = (float)sktree->pnu[i]*sktree->dirnu[i][j];  // VC(NE)WORK in M(G)eV
      }
    }
  
    neutcrscom_.crsx = sktree->Crsx;
    neutcrscom_.crsy = sktree->Crsy;
    neutcrscom_.crsz = sktree->Crsz;
    neutcrscom_.crsphi = sktree->Crsphi;
  }

  // Below is applicable for both neutrino and piscat/gampi MC

  // fsihist.h
  fsihist_.nvert = (int)sktree->Nvert;
  for (int ivert=0; ivert<sktree->Nvert; ivert++) {
    fsihist_.iflgvert[ivert] = sktree->Iflgvert[ivert];
    for (int j=0; j<3; j++)
      fsihist_.posvert[ivert][j] = (float)sktree->Posvert[ivert][j];
  }
  
  fsihist_.nvcvert = sktree->Nvcvert;
  for (int ip=0; ip<sktree->Nvcvert; ip++) {
    fsihist_.abspvert[ip] = (float)sktree->Abspvert[ip];
    fsihist_.abstpvert[ip] = (float)sktree->Abstpvert[ip];
    fsihist_.ipvert[ip] = sktree->Ipvert[ip];
    fsihist_.iverti[ip] = sktree->Iverti[ip];
    fsihist_.ivertf[ip] = sktree->Ivertf[ip];
    for (int j=0; j<3; j++)
      fsihist_.dirvert[ip][j] = (float)sktree->Dirvert[ip][j];
  }

  fsihist_.fsiprob = sktree->Fsiprob;

  // Set target nucleus type (water only for SK, this will need to 
  // be modified if OD sample or pion scattering is used)
  neuttarget_.numbndn =  sktree->Numbndn;
  neuttarget_.numbndp =  sktree->Numbndp;
  neuttarget_.numfrep =  sktree->Numfrep;
  neuttarget_.numatom =  sktree->Numatom; 
  posinnuc_.ibound    =  sktree->Ibound; 

  // Workaround to deduce Ibound since it was not calculated properly in SK_11b_official_candidate MC
  //if (sktree->f_sk==1) { // Neutrino MC
  //  neut::rew::NModeDefn neutModeDefn;
  //  if ((neutModeDefn.isCC1PI(nework_.modene) || neutModeDefn.isNC1PI(nework_.modene)) && fsihist_.nvert==0) {
  //    posinnuc_.ibound = 0;
  //  }
  //}

  //neut::rew::NFortFns *fortFns = neut::rew::NFortFns::Instance();
  //fortFns->print_allevent();
  
  // put empty nucleon FSI history (since it is not saved in the sktree format)														
//Comment out as NEUT does not have the necessary proton FSI information yet
//  nucleonfsihist_.nfnvert = 0;
//  nucleonfsihist_.nfnstep = 0;


}

//_______________________________________________________________________________________							   
void T2KNeutUtils::fill_neut_commons(TTree * a_intree, int entry)	
{ 																     
  // WARNING: This has only been implemented for a neuttree and not GENIE	   
  // This should be kept in sync with T2KNIWGUtils::GetNIWGEvent(TTree)

  // A simple test case assuming we have a "neuttree" tree.
	   
  TBranch *br_neutvect = a_intree->GetBranch("vectorbranch");	   
  NeutVect *nvect = NULL;				
  NeutrootTreeSingleton * neutrootTreeObj;		
	   
  if(br_neutvect){
    neutrootTreeObj = NeutrootTreeSingleton::Instance(a_intree);
    nvect = neutrootTreeObj->GetNeutVectAddress();
  } else {
    cout << "Error: T2KNeutUtils::fill_neut_commons(TTree) cannot find branch \"vectorbranch\". Are you using a neutroot generated file?" << endl; 
    exit (-1);	
  }
  neutrootTreeObj->GetEntry(entry);		   

  // CW: changed to rely on new function below instead of repeating code
  fill_neut_commons(nvect);			   
}

// CW: New function to make reweighting NEUT events much quicker
void T2KNeutUtils::fill_neut_commons(NeutVect *nvect)
{
  fill_neut_commons_with_defaults();
  // WARNING: This has only been implemented for a neuttree and not GENIE
  // This should be kept in sync with T2KNIWGUtils::GetNIWGEvent(TTree)
	
  //NEUT version info.  Can't get it to compile properly with this yet
  neutversion_.corev  =   nvect->COREVer;
  neutversion_.nucev  =   nvect->NUCEVer;
  neutversion_.nuccv  =   nvect->NUCCVer;

  // Documentation: See nework.h
  nework_.modene = nvect->Mode;
  nework_.numne = nvect->Npart();

  nemdls_.mdlqeaf = nvect->QEVForm;
  nemdls_.mdlqe = nvect->QEModel;
  nemdls_.mdlspi = nvect->SPIModel;
  nemdls_.mdldis = nvect->DISModel;
  nemdls_.mdlcoh = nvect->COHModel;
  neutcoh_.necohepi = nvect->COHModel;

  nemdls_.xmaqe = nvect->QEMA;
  nemdls_.xmvqe = nvect->QEMV;
  nemdls_.kapp  = nvect->KAPPA;

  //nemdls_.sccfv = SCCFVdef;
  //nemdls_.sccfa = SCCFAdef;
  //nemdls_.fpqe = FPQEdef;

  nemdls_.xmaspi = nvect->SPIMA;
  nemdls_.xmvspi = nvect->SPIMV;
  nemdls_.xmares = nvect->RESMA;
  nemdls_.xmvres = nvect->RESMV;

  neut1pi_.xmanffres = nvect->SPIMA;
  neut1pi_.xmvnffres = nvect->SPIMV;
  neut1pi_.xmarsres = nvect->RESMA;
  neut1pi_.xmvrsres = nvect->RESMV;
  neut1pi_.neiff    = nvect->SPIForm;
  neut1pi_.nenrtype = nvect->SPINRType;
  neut1pi_.rneca5i  = nvect->SPICA5I;
  neut1pi_.rnebgscl = nvect->SPIBGScale;

  nemdls_.xmacoh = nvect->COHMA;
  nemdls_.rad0nu = nvect->COHR0;
  //nemdls_.fa1coh = nvect->COHA1err;
  //nemdls_.fb1coh = nvect->COHb1err;

  //neutdis_.nepdf = NEPDFdef;
  //neutdis_.nebodek = NEBODEKdef;

  neutcard_.nefrmflg  = nvect->FrmFlg;
  neutcard_.nepauflg  = nvect->PauFlg;
  neutcard_.nenefo16  = nvect->NefO16;
  neutcard_.nemodflg  = nvect->ModFlg;
  //neutcard_.nenefmodl = 1;
  //neutcard_.nenefmodh = 1;
  //neutcard_.nenefkinh = 1;
  //neutpiabs_.neabspiemit = 1;
	   
  nenupr_.iformlen    =  nvect->FormLen;

  neutpiless_.ipilessdcy = nvect->IPilessDcy;
  neutpiless_.rpilessdcy = nvect->RPilessDcy;

  neffpr_.fefqe   = nvect->NuceffFactorPIQE;
  neffpr_.fefqeh  = nvect->NuceffFactorPIQEH;
  neffpr_.fefinel = nvect->NuceffFactorPIInel;
  neffpr_.fefabs  = nvect->NuceffFactorPIAbs;
  neffpr_.fefcx   = nvect->NuceffFactorPICX;
  neffpr_.fefcxh  = nvect->NuceffFactorPICXH;

  neffpr_.fefcoh =  nvect->NuceffFactorPICoh;
  neffpr_.fefqehf = nvect->NuceffFactorPIQEHKin;
  neffpr_.fefcxhf = nvect->NuceffFactorPICXKin;
  neffpr_.fefcohf = nvect->NuceffFactorPIQELKin;

  for ( int i = 0; i<nework_.numne; i++ ) {
    nework_.ipne[i] = nvect->PartInfo(i)->fPID;	   
    nework_.pne[i][0] = (float)nvect->PartInfo(i)->fP.X()/1000;  // VC(NE)WORK in M(G)eV
    nework_.pne[i][1] = (float)nvect->PartInfo(i)->fP.Y()/1000;  // VC(NE)WORK in M(G)eV
    nework_.pne[i][2] = (float)nvect->PartInfo(i)->fP.Z()/1000;  // VC(NE)WORK in M(G)eV
  }	   
  // fsihist.h															
  // neutroot fills a dummy object for events with no FSI to prevent memory leak when
  // reading the TTree, so check for it here
	   
  if ( (int)nvect->NfsiVert() == 1 ) { // An event with FSI must have at least two vertices
    if (nvect->NfsiPart()!=1 || nvect->Fsiprob!=-1)
      cout << "Warning: T2KNeutUtils::fill_neut_commons(TTree) NfsiPart!=1 or Fsiprob!=-1 when NfsiVert==1" << endl; 	   
    fsihist_.nvert = 0;
    fsihist_.nvcvert = 0;
    fsihist_.fsiprob = 1;
  }	   
  else { // Real FSI event
    fsihist_.nvert = (int)nvect->NfsiVert();
    for (int ivert=0; ivert<fsihist_.nvert; ivert++) {
      fsihist_.iflgvert[ivert] = nvect->FsiVertInfo(ivert)->fVertID;
      fsihist_.posvert[ivert][0] = (float)nvect->FsiVertInfo(ivert)->fPos.X();
      fsihist_.posvert[ivert][1] = (float)nvect->FsiVertInfo(ivert)->fPos.Y();
      fsihist_.posvert[ivert][2] = (float)nvect->FsiVertInfo(ivert)->fPos.Z();
    }

    fsihist_.nvcvert = nvect->NfsiPart();
    for (int ip=0; ip<fsihist_.nvcvert; ip++) {
      fsihist_.abspvert[ip] = (float)nvect->FsiPartInfo(ip)->fMomLab;
      fsihist_.abstpvert[ip] = (float)nvect->FsiPartInfo(ip)->fMomNuc;
      fsihist_.ipvert[ip] = nvect->FsiPartInfo(ip)->fPID;
      fsihist_.iverti[ip] = nvect->FsiPartInfo(ip)->fVertStart;
      fsihist_.ivertf[ip] = nvect->FsiPartInfo(ip)->fVertEnd;
      fsihist_.dirvert[ip][0] = (float)nvect->FsiPartInfo(ip)->fDir.X();
      fsihist_.dirvert[ip][1] = (float)nvect->FsiPartInfo(ip)->fDir.Y();
      fsihist_.dirvert[ip][2] = (float)nvect->FsiPartInfo(ip)->fDir.Z();
    }
    fsihist_.fsiprob = nvect->Fsiprob;
  }
					
  neutcrscom_.crsx = nvect->Crsx;
  neutcrscom_.crsy = nvect->Crsy;
  neutcrscom_.crsz = nvect->Crsz;
  neutcrscom_.crsphi = nvect->Crsphi;
  neutcrscom_.crsq2 = nvect->Crsq2;

  neuttarget_.numbndn = nvect->TargetA - nvect->TargetZ;
  neuttarget_.numbndp = nvect->TargetZ;
  neuttarget_.numfrep = nvect->TargetH;
  neuttarget_.numatom = nvect->TargetA;
  posinnuc_.ibound = nvect->Ibound;

  // put empty nucleon FSI history (since it is not saved in the NeutVect format)
  //Comment out as NEUT does not have the necessary proton FSI information yet
  //  nucleonfsihist_.nfnvert = 0;
  //  nucleonfsihist_.nfnstep = 0;
  
}

//_______________________________________________________________________________________
neut::rew::NSyst_t T2KNeutUtils::T2KtoNSyst(T2KSyst_t syst)
// map T2KSyst_t to the corresponding NEUT NSyst_t so that we can call functions
// from NEUT
{
  switch(syst) {
    case ( kNNucl_CCQEBindingEnergy_C12   ) : return neut::rew::kSystNucl_CCQEBindingEnergy_C12;    break;
    case ( kNNucl_CCQEBindingEnergy_O16   ) : return neut::rew::kSystNucl_CCQEBindingEnergy_O16;    break;
    case ( kNNucl_CCQEBindingEnergy_Al27  ) : return neut::rew::kSystNucl_CCQEBindingEnergy_Al27;   break;
    case ( kNNucl_CCQEBindingEnergy_Fe56  ) : return neut::rew::kSystNucl_CCQEBindingEnergy_Fe56;   break;
    case ( kNNucl_CCQEBindingEnergy_Cu63  ) : return neut::rew::kSystNucl_CCQEBindingEnergy_Cu63;   break;
    case ( kNNucl_CCQEBindingEnergy_Zn64  ) : return neut::rew::kSystNucl_CCQEBindingEnergy_Zn64;   break;
    case ( kNNucl_CCQEBindingEnergy_Pb208 ) : return neut::rew::kSystNucl_CCQEBindingEnergy_Pb208;  break;
    
    case ( kNXSec_MaNCEL         ) : return neut::rew::kXSecTwkDial_MaNCEL;       break;
      //case ( kNXSec_EtaNCEL        ) : return neut::rew::kXSecTwkDial_EtaNCEL;      break;
    case ( kNXSec_NormNCEL       ) : return neut::rew::kXSecTwkDial_NormNCEL;       break;
    case ( kNXSec_MaNCELshape    ) : return neut::rew::kXSecTwkDial_MaNCELshape;    break;
    case ( kNXSec_1overMaNCEL2    ) : return neut::rew::kXSecTwkDial_1overMaNCEL2;    break;
    case ( kNXSec_AxlFFNCEL ) : return neut::rew::kXSecTwkDial_AxlFFNCEL; break;
    case ( kNXSec_VecFFNCEL ) : return neut::rew::kXSecTwkDial_VecFFNCEL; break;

    case ( kNXSec_NormCCQE       ) : return neut::rew::kXSecTwkDial_NormCCQE;       break;
      //case ( kNXSec_NormCCQEenu    ) : return neut::rew::kXSecTwkDial_NormCCQEenu;    break;
    case ( kNXSec_MaCCQEshape    ) : return neut::rew::kXSecTwkDial_MaCCQEshape;    break;
    case ( kNXSec_1overMaCCQE2    ) : return neut::rew::kXSecTwkDial_1overMaCCQE2;    break;
    case ( kNXSec_MaCCQE         ) : return neut::rew::kXSecTwkDial_MaCCQE;         break;
    case ( kNXSec_AxlFFCCQE ) : return neut::rew::kXSecTwkDial_AxlFFCCQE; break;
    case ( kNXSec_VecFFCCQE ) : return neut::rew::kXSecTwkDial_VecFFCCQE; break;
    case ( kNXSec_VecFFCCQE_out ) : return neut::rew::kXSecTwkDial_VecFFCCQE_out; break;
      //case ( kNXSec_VecFFCCQEshape ) : return neut::rew::kXSecTwkDial_VecFFCCQEshape; break;

    case ( kNXSec_SCCVecQE ) : return neut::rew::kXSecTwkDial_SCCVecQE; break;
    case ( kNXSec_SCCAxlQE ) : return neut::rew::kXSecTwkDial_SCCAxlQE; break;
    case ( kNXSec_PsFF ) : return neut::rew::kXSecTwkDial_PsFF; break;

      //case ( kNXSec_CCQEFermiSurfMom   ) : return neut::rew::kSystNucl_CCQEFermiSurfMom;   break;

    case ( kNXSec_NormRES      ) : return neut::rew::kXSecTwkDial_NormRES;      break;
    case ( kNXSec_MaRESshape   ) : return neut::rew::kXSecTwkDial_MaRESshape;   break;
    case ( kNXSec_MaRES        ) : return neut::rew::kXSecTwkDial_MaRES;        break;
    case ( kNXSec_MvRES        ) : return neut::rew::kXSecTwkDial_MvRES;        break;

    case ( kNXSec_FFRES      ) : return neut::rew::kXSecTwkDial_FFRES;      break;
    case ( kNXSec_TypeRES    ) : return neut::rew::kXSecTwkDial_TypeRES;    break;
    case ( kNXSec_CA5RES     ) : return neut::rew::kXSecTwkDial_CA5RES;     break;
    case ( kNXSec_BgSclRES   ) : return neut::rew::kXSecTwkDial_BgSclRES;   break;
    case ( kNXSec_MaNFFRES   ) : return  neut::rew::kXSecTwkDial_MaNFFRES;  break;
    case ( kNXSec_MvNFFRES   ) : return  neut::rew::kXSecTwkDial_MvNFFRES;  break;
    case ( kNXSec_MaRSRES    ) : return  neut::rew::kXSecTwkDial_MaRSRES;   break;
    case ( kNXSec_MvRSRES    ) : return  neut::rew::kXSecTwkDial_MvRSRES;   break;

    case ( kNXSec_NormCCRES      ) : return neut::rew::kXSecTwkDial_NormCCRES;      break;
    case ( kNXSec_MaCCRESshape   ) : return neut::rew::kXSecTwkDial_MaCCRESshape;   break;
      //case ( kNXSec_MvCCRESshape   ) : return neut::rew::kXSecTwkDial_MvCCRESshape;   break;
    case ( kNXSec_MaCCRES        ) : return neut::rew::kXSecTwkDial_MaCCRES;        break;
    case ( kNXSec_MvCCRES        ) : return neut::rew::kXSecTwkDial_MvCCRES;        break;

    case ( kNXSec_FFCCRES      ) : return neut::rew::kXSecTwkDial_FFCCRES;      break;
    case ( kNXSec_TypeCCRES    ) : return neut::rew::kXSecTwkDial_TypeCCRES;    break;
    case ( kNXSec_CA5CCRES     ) : return neut::rew::kXSecTwkDial_CA5CCRES;     break;
    case ( kNXSec_BgSclCCRES   ) : return neut::rew::kXSecTwkDial_BgSclCCRES;   break;
    case ( kNXSec_MaNFFCCRES   ) : return  neut::rew::kXSecTwkDial_MaNFFCCRES;  break;
    case ( kNXSec_MvNFFCCRES   ) : return  neut::rew::kXSecTwkDial_MvNFFCCRES;  break;
    case ( kNXSec_MaRSCCRES    ) : return  neut::rew::kXSecTwkDial_MaRSCCRES;   break;
    case ( kNXSec_MvRSCCRES    ) : return  neut::rew::kXSecTwkDial_MvRSCCRES;   break;

    case ( kNXSec_NormNCRES      ) : return neut::rew::kXSecTwkDial_NormNCRES;      break;
    case ( kNXSec_MaNCRESshape   ) : return neut::rew::kXSecTwkDial_MaNCRESshape;   break;
      //case ( kNXSec_MvNCRESshape   ) : return neut::rew::kXSecTwkDial_MvNCRESshape;   break;
    case ( kNXSec_MaNCRES        ) : return neut::rew::kXSecTwkDial_MaNCRES;        break;
    case ( kNXSec_MvNCRES        ) : return neut::rew::kXSecTwkDial_MvNCRES;        break;

    case ( kNXSec_FFNCRES      ) : return neut::rew::kXSecTwkDial_FFNCRES;      break;
    case ( kNXSec_TypeNCRES    ) : return neut::rew::kXSecTwkDial_TypeNCRES;    break;
    case ( kNXSec_CA5NCRES     ) : return neut::rew::kXSecTwkDial_CA5NCRES;     break;
    case ( kNXSec_BgSclNCRES   ) : return neut::rew::kXSecTwkDial_BgSclNCRES;   break;
    case ( kNXSec_MaNFFNCRES   ) : return  neut::rew::kXSecTwkDial_MaNFFNCRES;  break;
    case ( kNXSec_MvNFFNCRES   ) : return  neut::rew::kXSecTwkDial_MvNFFNCRES;  break;
    case ( kNXSec_MaRSNCRES    ) : return  neut::rew::kXSecTwkDial_MaRSNCRES;   break;
    case ( kNXSec_MvRSNCRES    ) : return  neut::rew::kXSecTwkDial_MvRSNCRES;   break;
    
    //case ( kNXSec_NECOHEPI     ) : return neut::rew::kXSecTwkDial_NECOHEPI;     break;
    case ( kNXSec_MaCOHpi      ) : return neut::rew::kXSecTwkDial_MaCOHpi;      break;
    case ( kNXSec_R0COHpi      ) : return neut::rew::kXSecTwkDial_R0COHpi;      break;
    //case ( kNXSec_fA1COHpi     ) : return neut::rew::kXSecTwkDial_fA1COHpi;     break;
    //case ( kNXSec_fb1COHpi     ) : return neut::rew::kXSecTwkDial_fb1COHpi;     break;

    case ( kNXSec_NormDIS        ) : return neut::rew::kXSecTwkDial_NormDIS;        break;
    case ( kNXSec_BYOnOffDIS        ) : return neut::rew::kXSecTwkDial_BYOnOffDIS;        break;
      //case ( kNXSec_RnubarnuCC     ) : return neut::rew::kXSecTwkDial_RnubarnuCC;     break;
      //case ( kNXSec_DISNuclMod     ) : return neut::rew::kXSecTwkDial_DISNuclMod;     break;
    case ( kNXSec_NC             ) : return neut::rew::kXSecTwkDial_NC;             break;

    // P.S (26.01.17) AxialFF Patch 
    case ( kNXSec_AxlDipToAlt ) : return neut::rew::kXSecTwkDial_AxlDipToAlt; break;
    
    case ( kNXSec_FAxlCCQEAlpha     ) : return neut::rew::kXSecTwkDial_FAxlCCQEAlpha; break;
    case ( kNXSec_FAxlCCQEGamma     ) : return neut::rew::kXSecTwkDial_FAxlCCQEGamma; break;
    case ( kNXSec_FAxlCCQEBeta      ) : return neut::rew::kXSecTwkDial_FAxlCCQEBeta;  break;
    case ( kNXSec_FAxlCCQETheta     ) : return neut::rew::kXSecTwkDial_FAxlCCQETheta; break;

    case ( kNXSec_FAZExp_NTerms    ) : return neut::rew::kXSecTwkDial_FAZExp_NTerms; break;
    case ( kNXSec_FAZExp_TCut      ) : return neut::rew::kXSecTwkDial_FAZExp_TCut;  break;
    case ( kNXSec_FAZExp_T0        ) : return neut::rew::kXSecTwkDial_FAZExp_T0;    break;
    case ( kNXSec_FAZExp_Q4Cut     ) : return neut::rew::kXSecTwkDial_FAZExp_Q4Cut; break;
    case ( kNXSec_FAZExp_A0        ) : return neut::rew::kXSecTwkDial_FAZExp_A0;    break;
    case ( kNXSec_FAZExp_A1        ) : return neut::rew::kXSecTwkDial_FAZExp_A1;  break;
    case ( kNXSec_FAZExp_A2        ) : return neut::rew::kXSecTwkDial_FAZExp_A2;  break;
    case ( kNXSec_FAZExp_A3        ) : return neut::rew::kXSecTwkDial_FAZExp_A3;  break;
    case ( kNXSec_FAZExp_A4        ) : return neut::rew::kXSecTwkDial_FAZExp_A4;  break;
    case ( kNXSec_FAZExp_A5        ) : return neut::rew::kXSecTwkDial_FAZExp_A5;  break;
    case ( kNXSec_FAZExp_A6        ) : return neut::rew::kXSecTwkDial_FAZExp_A6;  break;
    case ( kNXSec_FAZExp_A7        ) : return neut::rew::kXSecTwkDial_FAZExp_A7;  break;
    case ( kNXSec_FAZExp_A8        ) : return neut::rew::kXSecTwkDial_FAZExp_A8;  break;
    case ( kNXSec_FAZExp_A9        ) : return neut::rew::kXSecTwkDial_FAZExp_A9;  break;

      //case ( kNHadrAGKY_xF1pi    ) : return neut::rew::kHadrAGKYTwkDial_xF1pi;    break;
      //case ( kNHadrAGKY_pT1pi    ) : return neut::rew::kHadrAGKYTwkDial_pT1pi;    break;
      //case ( kNHadrNucl_FormZone ) : return neut::rew::kHadrNuclTwkDial_FormZone; break;

    case ( kNCasc_FrAbs_pi       ) : return neut::rew::kCascTwkDial_FrAbs_pi;       break;
    case ( kNCasc_FrInelLow_pi   ) : return neut::rew::kCascTwkDial_FrInelLow_pi;   break;
    case ( kNCasc_FrCExLow_pi    ) : return neut::rew::kCascTwkDial_FrCExLow_pi;    break;
    case ( kNCasc_FrInelHigh_pi  ) : return neut::rew::kCascTwkDial_FrInelHigh_pi;  break;
    case ( kNCasc_FrCExHigh_pi   ) : return neut::rew::kCascTwkDial_FrCExHigh_pi;   break;
    case ( kNCasc_FrPiProd_pi    ) : return neut::rew::kCascTwkDial_FrPiProd_pi;    break;

    //Comment out as NEUT does not have the necessary proton FSI information yet
    //case ( kNINuke_MFP_N       ) : return neut::rew::kINukeTwkDial_MFP_N;       break;
    // Was previously commented out
    //case ( kNINuke_FrCEx_N     ) : return neut::rew::kINukeTwkDial_FrCEx_N;     break;
    //case ( kNINuke_FrElas_N    ) : return neut::rew::kINukeTwkDial_FrElas_N;    break;
    //case ( kNINuke_FrInel_N    ) : return neut::rew::kINukeTwkDial_FrInel_N;    break;
    //case ( kNINuke_FrAbs_N     ) : return neut::rew::kINukeTwkDial_FrAbs_N;     break;
    //case ( kNINuke_FrPiProd_N  ) : return neut::rew::kINukeTwkDial_FrPiProd_N;  break;

    case ( kNNucl_CCQEPauliSupViaKF   ) : return neut::rew::kSystNucl_CCQEPauliSupViaKF;   break;
    case ( kNNucl_CCQEFermiSurfMom   ) : return neut::rew::kSystNucl_CCQEFermiSurfMom;   break;
    case ( kNNucl_CCQEBindingEnergy   ) : return neut::rew::kSystNucl_CCQEBindingEnergy;   break;
    //case ( kNNucl_CCQEMomDistroFGtoSF ) : return neut::rew::kSystNucl_CCQEMomDistroFGtoSF; break;
    //case ( kNRDcy_BR1gamma        ) : return neut::rew::kRDcyTwkDial_BR1gamma;        break;
    //case ( kNRDcy_BR1eta          ) : return neut::rew::kRDcyTwkDial_BR1eta;          break;
    //case ( kNRDcy_Theta_Delta2Npi ) : return neut::rew::kRDcyTwkDial_Theta_Delta2Npi; break;
    case ( kNNucl_PilessDcyRES ) : return neut::rew::kSystNucl_PilessDcyRES; break;
      //case ( kNNucl_PilessDcyOnlyRES ) : return neut::rew::kSystNucl_PilessDcyOnlyRES; break;

    default:
      return neut::rew::kNullSystematic;
      break;
  }
  return neut::rew::kNullSystematic;
}
//_______________________________________________________________________________________
#ifdef __T2KRW_OAANALYSIS_ENABLED__
//_______________________________________________________________________________________
void T2KNeutUtils::fill_neut_commons(const ND::NRooTrackerVtx * vtx)
{
  fill_neut_commons_with_defaults();
  // Documentation: See nework.h
  nework_.numne  = vtx->NEnvc; 
  nework_.modene = (vtx->EvtCode->GetString()).Atoi();
  for ( int i = 0; i<nework_.numne; i++ ) { 
    nework_.ipne[i] = vtx->NEipvc[i];
    for ( int j = 0 ; j < 3 ; j++ ){
      nework_.pne[i][j] = (float)vtx->NEpvc[i][j]/1000;  // VC(NE)WORK in M(G)eV
    }
  }

  neutcrscom_.crsx = vtx->NEcrsx;
  neutcrscom_.crsy = vtx->NEcrsy;
  neutcrscom_.crsz = vtx->NEcrsz;
  neutcrscom_.crsphi = vtx->NEcrsphi;

  // fsihist.h
  fsihist_.nvert = vtx->NEnvert;
  for (int ivert=0; ivert<vtx->NEnvert; ivert++) {
    fsihist_.iflgvert[ivert] = vtx->NEiflgvert[ivert];
    for (int j=0; j<3; j++)
      fsihist_.posvert[ivert][j] = (float)vtx->NEposvert[ivert][j];
  }
  
  fsihist_.nvcvert = vtx->NEnvcvert;
  for (int ip=0; ip<vtx->NEnvcvert; ip++) {
    fsihist_.abspvert[ip] = (float)vtx->NEabspvert[ip];
    fsihist_.abstpvert[ip] = (float)vtx->NEabstpvert[ip];
    fsihist_.ipvert[ip] = vtx->NEipvert[ip];
    fsihist_.iverti[ip] = vtx->NEiverti[ip];
    fsihist_.ivertf[ip] = vtx->NEivertf[ip];
    for (int j=0; j<3; j++)
      fsihist_.dirvert[ip][j] = (float)vtx->NEdirvert[ip][j];
  }

  //fsihist_.fsiprob = vtx->NEfsiprob;
  // Default FSIPROB calculation not calculated properly in <= MCP4 
  // so set to -1 to have NReWeightCasc.cc calculate it on the fly
  fsihist_.fsiprob = -1;
 
  // Get target nucleus type
  int Z = (int)TgtZ(*vtx);
  int AA = (int)TgtA(*vtx);

  // Workaround to deduce Ibound since it was not stored in <=MCP4
  if (Z==1) {
    neuttarget_.numatom = 1; 
    neuttarget_.numbndp = 0;
    neuttarget_.numbndn = 0;
    neuttarget_.numfrep = 1; 
    posinnuc_.ibound = 0;
  }
  else if (Z>1){
    neuttarget_.numatom = AA; 
    neuttarget_.numbndp = Z;
    neuttarget_.numbndn = AA-Z;
    neuttarget_.numfrep = 0; 
    posinnuc_.ibound = 1;
  }
  else {
    cout << "T2KNeutUtils Error: Unknown Z = " << Z << endl;
    exit (-1);
  }
  
  // Nucleon FSI history
  //Comment out as NEUT does not have the necessary proton FSI information yet
//  nucleonfsihist_.nfnvert = vtx->NFnvert;
//  for(int i=0; i < vtx->NFnvert; i++) {
//    nucleonfsihist_.nfiflag[i] = vtx->NFiflag[i];
//    nucleonfsihist_.nfx[i] = vtx->NFx[i];
//    nucleonfsihist_.nfy[i] = vtx->NFy[i];
//    nucleonfsihist_.nfz[i] = vtx->NFz[i];
//    nucleonfsihist_.nfpx[i] = vtx->NFpx[i];
//    nucleonfsihist_.nfpy[i] = vtx->NFpy[i];
//    nucleonfsihist_.nfpz[i] = vtx->NFpz[i];
//    nucleonfsihist_.nfe[i] = vtx->NFe[i];
//    nucleonfsihist_.nffirststep[i] = vtx->NFfirststep[i];
//  }
//  nucleonfsihist_.nfnstep = vtx->NFnstep;
//  for(int i=0; i < vtx->NFnstep; i++) {
//    nucleonfsihist_.nfecms2[i] = vtx->NFecms2[i];
//  }
  
//  cout << "NEUT commons filled for event: " << vtx->EvtNum << endl;
    

}
//_______________________________________________________________________________________
int T2KNeutUtils::PdgNu(const ND::NRooTrackerVtx & evt) 
{
// Return the neutrino PDG code
//
  int iv = T2KNeutUtils::IdxNu(evt);
  if(iv<0) return 0;
  
  return evt.StdHepPdg[iv];
}
//_______________________________________________________________________________________
int T2KNeutUtils::PdgFsl(const ND::NRooTrackerVtx & evt) 
{
// Return the final state primary lepton PDG code
//
  int il = T2KNeutUtils::IdxNu(evt);
  if(il<0) return 0;
  
  return evt.StdHepPdg[il];
}
//_______________________________________________________________________________________
int T2KNeutUtils::PdgTgt(const ND::NRooTrackerVtx & evt) 
{
// Return the hit target pdg code
//
  int it = T2KNeutUtils::IdxTgt(evt);
  if(it<0) return 0;
  
  return evt.StdHepPdg[it];
}
//_______________________________________________________________________________________
int T2KNeutUtils::PdgHitNuc(const ND::NRooTrackerVtx & evt) 
{
// Return the hit nucleon pdg code (if a nucleon was hit)
//
  int idx = T2KNeutUtils::IdxHitNuc(evt);
  if(idx<0) return 0;

  return evt.StdHepPdg[idx];
}
//_______________________________________________________________________________________
int T2KNeutUtils::TgtZ(const ND::NRooTrackerVtx & evt) 
{
// Return the hit target Z 
//
  int tgtpdg = T2KNeutUtils::PdgTgt(evt);

  if(tgtpdg == 2212) return 1;
  if(tgtpdg == 2112) return 0;

  // nuclear pdg codes of the form: 10LZZZAAAI
  // extract ZZZ
  if(tgtpdg >  1000000000 ) {
   int Z = (tgtpdg/10000) - 1000*(tgtpdg/10000000); // don't factor out - bit shifts!
   return Z;
  }

  return 0;
}
//_______________________________________________________________________________________
int T2KNeutUtils::TgtA(const ND::NRooTrackerVtx & evt) 
{
// Return the hit target A
//
  int tgtpdg = T2KNeutUtils::PdgTgt(evt);

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
int T2KNeutUtils::NParticles(const ND::NRooTrackerVtx & evt) 
{
  int N=0;
  while(evt.StdHepPdg[N] != 0) {
    N++;
  }
  return N;
}
////_______________________________________________________________________________________
double T2KNeutUtils::ENu(const ND::NRooTrackerVtx & evt) 
{
// Return the neutrino energy
//
  int iv = T2KNeutUtils::IdxNu(evt);              
  if(iv<0) return 0;

  return evt.StdHepP4[iv][kNStdHepIdxE];
}
//_______________________________________________________________________________________
double T2KNeutUtils::ENu_nrf(const ND::NRooTrackerVtx & evt)
{
// Return the neutrino energy at the hit nucleon rest frame
//
  int iN = T2KNeutUtils::IdxHitNuc(evt);
  if(iN<0) return T2KNeutUtils::ENu(evt); // no hit nuc - same as Ev at LAB

  int iv = T2KNeutUtils::IdxNu(evt);

  double bx = evt.StdHepP4[iN][kNStdHepIdxPx] /  evt.StdHepP4[iN][kNStdHepIdxE];
  double by = evt.StdHepP4[iN][kNStdHepIdxPy] /  evt.StdHepP4[iN][kNStdHepIdxE];
  double bz = evt.StdHepP4[iN][kNStdHepIdxPz] /  evt.StdHepP4[iN][kNStdHepIdxE];

  TLorentzVector p4v(evt.StdHepP4[iv][kNStdHepIdxPx],
                     evt.StdHepP4[iv][kNStdHepIdxPy],
                     evt.StdHepP4[iv][kNStdHepIdxPz],
                     evt.StdHepP4[iv][kNStdHepIdxE]);

  p4v.Boost(-bx,-by,-bz);

  return p4v.Energy();
}
//_______________________________________________________________________________________
double T2KNeutUtils::ENu_reco(const ND::NRooTrackerVtx & evt) 
{
// Return the reconstructed-like neutrino energy for QELCC
// Ev_reco = (M*El - ml^2/2) / (M-El+pl*costheta)
//
  int iv = T2KNeutUtils::IdxNu(evt);              
  if(iv<0) return 0;
  int il = T2KNeutUtils::IdxFsl(evt);        
  if(il<0) return 0;

  double El     = T2KNeutUtils::E        (evt,il);
  double pl     = T2KNeutUtils::Momentum (evt,il);
  double costhl = T2KNeutUtils::CosTheta (evt,il);
  double M      = 0.938919;
  double ml2    = TMath::Power(0.105658, 2.);

  double Ev_reco = (M*El - 0.5*ml2) / (M - El + pl*costhl);
  return Ev_reco;
}
//_______________________________________________________________________________________
double T2KNeutUtils::EFsl(const ND::NRooTrackerVtx & evt) 
{
// Return the final state primary lepton energy
//
  int il = T2KNeutUtils::IdxFsl(evt);              
  return T2KNeutUtils::E(evt,il);
}
//_______________________________________________________________________________________
double T2KNeutUtils::MomentumFsl(const ND::NRooTrackerVtx & evt) 
{
// Return the final state primary lepton momentum
//
  int il = T2KNeutUtils::IdxFsl(evt);              
  return T2KNeutUtils::Momentum(evt,il);
}
//_______________________________________________________________________________________
double T2KNeutUtils::Momentum(const ND::NRooTrackerVtx & evt, int i) 
{
// Return the final state ith stdhep entry momentum
  
  int Np = T2KNeutUtils::NParticles(evt);
  if(i >= Np || i < 0){
    return 0.0; 
  }

  return TMath::Sqrt(
            TMath::Power(evt.StdHepP4[i][kNStdHepIdxPx], 2) +
            TMath::Power(evt.StdHepP4[i][kNStdHepIdxPy], 2) +
            TMath::Power(evt.StdHepP4[i][kNStdHepIdxPz], 2)
	    );
}
//_______________________________________________________________________________________
double T2KNeutUtils::E(const ND::NRooTrackerVtx & evt, int i) 
{
// Return the final state ith stdhep entry momentum
  
  int Np = T2KNeutUtils::NParticles(evt);
  if(i >= Np || i < 0){
    return 0.0; 
  }

  return evt.StdHepP4[i][kNStdHepIdxE];
}
//_______________________________________________________________________________________
double T2KNeutUtils::KE(const ND::NRooTrackerVtx & evt, int i) 
{
  double E  = T2KNeutUtils::E(evt,i);
  double P  = T2KNeutUtils::Momentum(evt,i);
  double KE = E - TMath::Sqrt(TMath::Max(0., E*E - P*P));

  return KE;
}
//_______________________________________________________________________________________
double T2KNeutUtils::CosTheta(const ND::NRooTrackerVtx & evt, int i) 
{
// Return the i^th particle cos(theta) with respect to the neutrino direction
//
  int Np = T2KNeutUtils::NParticles(evt);
  if(i >= Np || i < 0){
    return 0.0; 
  }

  int iv = T2KNeutUtils::IdxNu(evt);  
  if(iv<0) return 0;

  TVector3 p3v(evt.StdHepP4[iv][kNStdHepIdxPx],
               evt.StdHepP4[iv][kNStdHepIdxPy],
               evt.StdHepP4[iv][kNStdHepIdxPz]);

  TVector3 p3i(evt.StdHepP4[i][kNStdHepIdxPx],
               evt.StdHepP4[i][kNStdHepIdxPy],
               evt.StdHepP4[i][kNStdHepIdxPz]);

  double costh = p3v.Dot(p3i)/(p3v.Mag()*p3i.Mag()); // <-- cos(theta) = a.b/(|a||b|) 

  return costh;
}
//_______________________________________________________________________________________
double T2KNeutUtils::FermiMomentum(const ND::NRooTrackerVtx & evt) 
{
// Return the hit nucleon Fermi momentum
//
  int in = T2KNeutUtils::IdxHitNuc(evt);              
  if(in<0) return 0;

  return TMath::Sqrt(
            TMath::Power(evt.StdHepP4[in][kNStdHepIdxPx], 2) +
            TMath::Power(evt.StdHepP4[in][kNStdHepIdxPy], 2) +
            TMath::Power(evt.StdHepP4[in][kNStdHepIdxPz], 2)
         );
}
//_______________________________________________________________________________________
int T2KNeutUtils::IdxNu(const ND::NRooTrackerVtx & /*evt*/) 
{
// Return the input neutrino index in the STDHEP array
// Always at 0
//
  return 0;
}
//_______________________________________________________________________________________
int T2KNeutUtils::IdxTgt(const ND::NRooTrackerVtx & /*evt*/) 
{
// Return the input target index in the STDHEP array
// Always at 1
//
  return 1;
}
//_______________________________________________________________________________________
int T2KNeutUtils::IdxFsl(const ND::NRooTrackerVtx & evt) 
{
  // Return the final state prim lepton index in the STDHEP array
  // Look for the input neutrino's daughter
  // return evt.StdHepFd[0]; 
  
  // StdHepFd not currently filled for NEUT, so search for first outgoing lepton instead
  for (int i = 3; i < evt.StdHepN; ++i){
    if (abs(evt.StdHepPdg[i]) < 11 || abs(evt.StdHepPdg[i]) > 16) continue;
    return i;
  }
  
  return 0;

}
//_______________________________________________________________________________________
int T2KNeutUtils::IdxHitNuc(const ND::NRooTrackerVtx & evt) 
{
// For NEUT, this is always the i=2 (following the nucleus index)
  return 2;
}
#endif // __T2KRW_OAANALYSIS_ENABLED__
//_______________________________________________________________________________________
#endif // __T2KRW_NEUT_ENABLED__ 
 
