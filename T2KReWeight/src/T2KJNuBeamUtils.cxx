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

#include "T2KJNuBeamUtils.h"
#include "T2KSyst.h"

#ifdef __T2KRW_JNUBEAM_ENABLED__
#endif

using std::cerr;
using std::cout;
using std::endl;

using namespace t2krew;

//#define _DEBUG_JNUBEAM_UTILS__

#ifdef __T2KRW_JNUBEAM_ENABLED__
//_______________________________________________________________________________________
jnubeam::rew::JEvent * T2KJNuBeamUtils::GetJNuBeamEvent(const SK::SK__h1 * sktree)
{

  jnubeam::rew::JEvent * fDummyJEvent = NULL;
  if(!sktree){
    cout << "Cannot convert a null SK__h1 tree to JNuBeam event!" << endl;
    return fDummyJEvent;
  } 

  fDummyJEvent = new jnubeam::rew::JEvent();
  fDummyJEvent->enu = sktree->pnu[0];
  
  // WARNING: These are not the actual JNuBeam mode.
  //          The first available mode producing each neutrino type
  //          is used just for use in JReWeightEnu2011a.cc
  if (sktree->ipnu[0]==14) fDummyJEvent->mode = 11;
  else if (sktree->ipnu[0]==-14) fDummyJEvent->mode = 21;
  else if (sktree->ipnu[0]==12) {
    
    // T2K signal nue file
    if (sktree->filetype==1) fDummyJEvent->mode = 11;

    // Beam nue
    else fDummyJEvent->mode = 31;
    
  }
  else if (sktree->ipnu[0]==-12) {

    // T2K signal nuebar file
    if (sktree->filetype==1) fDummyJEvent->mode = 21;

    // Beam nuebar
    else fDummyJEvent->mode = 41;

  }

  fDummyJEvent->idfd = -1;  // No ID for SK

  return fDummyJEvent;
  
}

//_______________________________________________________________________________________
jnubeam::rew::JEvent * T2KJNuBeamUtils::GetJNuBeamEvent(const JnuBeamRead * jnutree)
{
  // Read in a JNuBeam file and set the appropriate variables
  
  jnubeam::rew::JEvent * fDummyJEvent = NULL;

  // need to distinguish between SK and ND JnuBeam files

  if(!jnutree){
    cout << "Cannot convert a null JnuBeam  tree to JNuBeam event!" << endl;
    return fDummyJEvent;
  }
      
  fDummyJEvent = new jnubeam::rew::JEvent();
  fDummyJEvent->enu = jnutree->Enu;
  fDummyJEvent->xnu[0] = jnutree->xnu;// check me
  fDummyJEvent->xnu[1] = jnutree->ynu; //check me
  fDummyJEvent->mode = jnutree->mode; //check me
  fDummyJEvent->idfd = jnutree->idfd; //check me
  fDummyJEvent->norm = jnutree->norm; //check me

  fDummyJEvent->bpos[0] = jnutree->bpos[0];
  fDummyJEvent->bpos[1] = jnutree->bpos[1];
  fDummyJEvent->btilt[0] = jnutree->btilt[0];
  fDummyJEvent->btilt[1] = jnutree->btilt[1];
  fDummyJEvent->brms[0] = jnutree->brms[0];
  fDummyJEvent->brms[1] = jnutree->brms[1];
  fDummyJEvent->emit[0] = jnutree->emit[0];
  fDummyJEvent->emit[1] = jnutree->emit[1];
  fDummyJEvent->alpha[0] = jnutree->alpha[0];
  fDummyJEvent->alpha[1] = jnutree->alpha[1];
  fDummyJEvent->hcur[0] = jnutree->hcur[0];
  fDummyJEvent->hcur[1] = jnutree->hcur[1];
  fDummyJEvent->hcur[2] = jnutree->hcur[2];


  for (int ig=0; ig<jnutree->ng; ig++) {

    JIntLink fDummyJIntChain;
    
    fDummyJIntChain.pos[0] = jnutree->gvx[ig];
    fDummyJIntChain.pos[1] = jnutree->gvy[ig];
    fDummyJIntChain.pos[2] = jnutree->gvz[ig];

    fDummyJIntChain.mom[0] = jnutree->gpx[ig];
    fDummyJIntChain.mom[1] = jnutree->gpy[ig];
    fDummyJIntChain.mom[2] = jnutree->gpz[ig];

    fDummyJIntChain.pid = jnutree->gpid[ig];
    fDummyJIntChain.mec = jnutree->gmec[ig];
    fDummyJIntChain.mat = jnutree->gmat[ig];
    fDummyJIntChain.distc = jnutree->gdistc[ig];
    fDummyJIntChain.distal = jnutree->gdistal[ig];
    fDummyJIntChain.distti = jnutree->gdistti[ig];
    fDummyJIntChain.distfe = jnutree->gdistfe[ig];
    
    (fDummyJEvent->int_chain).push_back (fDummyJIntChain);
    
  }
  fDummyJEvent->CalcTheta(); //check me necessary?
  
  
  return fDummyJEvent;
  
}


//_______________________________________________________________________________________
jnubeam::rew::JSyst_t T2KJNuBeamUtils::T2KtoJSyst(T2KSyst_t syst)
// map T2KSyst_t to the corresponding JNUBEAM JSyst_t so that we can call functions
// from JNUBEAM
{
  switch(syst) {
    case ( kJNA61pi_dedx      ) : return jnubeam::rew::kNA61piTwkDial_dedx     ; break;
    case ( kJNA61pi_dedxtof      ) : return jnubeam::rew::kNA61piTwkDial_dedxtof     ; break;
    case ( kJNA61pi_feeddown ) : return jnubeam::rew::kNA61piTwkDial_feeddown; break;
    case ( kJNA61pi_trktop   ) : return jnubeam::rew::kNA61piTwkDial_trktop  ; break;
    case ( kJNA61pi_trkcut   ) : return jnubeam::rew::kNA61piTwkDial_trkcut  ; break;
    case ( kJNA61pi_trkmrg   ) : return jnubeam::rew::kNA61piTwkDial_trkmrg  ; break;
    case ( kJNA61pi_hcont    ) : return jnubeam::rew::kNA61piTwkDial_hcont   ; break;
    case ( kJNA61pi_receff   ) : return jnubeam::rew::kNA61piTwkDial_receff  ; break;
    case ( kJNA61pi_tofeff   ) : return jnubeam::rew::kNA61piTwkDial_tofeff  ; break;
    case ( kJNA61pi_piloss   ) : return jnubeam::rew::kNA61piTwkDial_piloss  ; break;
    case ( kJNA61pi_norm     ) : return jnubeam::rew::kNA61piTwkDial_norm    ; break;
    case ( kJkaon_bmptA      ) : return jnubeam::rew::kKaonTwkDial_bmptA     ; break;
    case ( kJkaon_bmptB      ) : return jnubeam::rew::kKaonTwkDial_bmptB     ; break;
    case ( kJkaon_bmptalpha  ) : return jnubeam::rew::kKaonTwkDial_bmptalpha ; break;
    case ( kJkaon_bmptbeta   ) : return jnubeam::rew::kKaonTwkDial_bmptbeta  ; break;
    case ( kJkaon_bmpta1     ) : return jnubeam::rew::kKaonTwkDial_bmpta1    ; break;
    case ( kJkaon_bmptgamma  ) : return jnubeam::rew::kKaonTwkDial_bmptgamma ; break;
    case ( kJkaon_bmptdelta  ) : return jnubeam::rew::kKaonTwkDial_bmptdelta ; break;
    case ( kJTXsec_pLow      ) : return jnubeam::rew::kTXsecTwkDial_pLow     ; break;
    case ( kJTXsec_ppiMid    ) : return jnubeam::rew::kTXsecTwkDial_ppiMid   ; break;
    case ( kJTXsec_pHigh     ) : return jnubeam::rew::kTXsecTwkDial_pHigh    ; break;
    case ( kJTXsec_piLow     ) : return jnubeam::rew::kTXsecTwkDial_piLow    ; break;
    case ( kJTXsec_kLow      ) : return jnubeam::rew::kTXsecTwkDial_kLow     ; break;
    case ( kJTXsec_pikHigh   ) : return jnubeam::rew::kTXsecTwkDial_pikHigh  ; break;
    case ( kJTXsec_kHigh     ) : return jnubeam::rew::kTXsecTwkDial_kHigh    ; break;
    case ( kJPbeam_xpos      ) : return jnubeam::rew::kPbeamTwkDial_xpos     ; break;
    case ( kJPbeam_ypos      ) : return jnubeam::rew::kPbeamTwkDial_ypos     ; break;
    case ( kJPbeam_xdir      ) : return jnubeam::rew::kPbeamTwkDial_xdir     ; break;
    case ( kJPbeam_ydir      ) : return jnubeam::rew::kPbeamTwkDial_ydir     ; break;
    case ( kJPbeam_xsig      ) : return jnubeam::rew::kPbeamTwkDial_xsig     ; break;
    case ( kJPbeam_ysig      ) : return jnubeam::rew::kPbeamTwkDial_ysig     ; break;
    case ( kJPbeam_xalph     ) : return jnubeam::rew::kPbeamTwkDial_xalph    ; break;
    case ( kJPbeam_yalph     ) : return jnubeam::rew::kPbeamTwkDial_yalph    ; break;
    case ( kJPbeam_xemit     ) : return jnubeam::rew::kPbeamTwkDial_xemit    ; break;
    case ( kJPbeam_yemit     ) : return jnubeam::rew::kPbeamTwkDial_yemit    ; break;

    case ( kJEnu2012a_nd5numu0     ) : return jnubeam::rew::kEnu2012aTwkDial_nd5numu0   ; break;	    
    case ( kJEnu2012a_nd5numu1     ) : return jnubeam::rew::kEnu2012aTwkDial_nd5numu1   ; break;	    
    case ( kJEnu2012a_nd5numu2     ) : return jnubeam::rew::kEnu2012aTwkDial_nd5numu2   ; break;	    
    case ( kJEnu2012a_nd5numu3     ) : return jnubeam::rew::kEnu2012aTwkDial_nd5numu3   ; break;	    
    case ( kJEnu2012a_nd5numu4     ) : return jnubeam::rew::kEnu2012aTwkDial_nd5numu4   ; break;	    
    case ( kJEnu2012a_nd5numu5     ) : return jnubeam::rew::kEnu2012aTwkDial_nd5numu5   ; break;	    
    case ( kJEnu2012a_nd5numu6     ) : return jnubeam::rew::kEnu2012aTwkDial_nd5numu6   ; break;	    
    case ( kJEnu2012a_nd5numu7     ) : return jnubeam::rew::kEnu2012aTwkDial_nd5numu7   ; break;	    
    case ( kJEnu2012a_nd5numu8     ) : return jnubeam::rew::kEnu2012aTwkDial_nd5numu8   ; break;	    
    case ( kJEnu2012a_nd5numu9     ) : return jnubeam::rew::kEnu2012aTwkDial_nd5numu9   ; break;	    
    case ( kJEnu2012a_nd5numu10    ) : return jnubeam::rew::kEnu2012aTwkDial_nd5numu10  ; break;	   
    case ( kJEnu2012a_nd5numub0    ) : return jnubeam::rew::kEnu2012aTwkDial_nd5numub0  ; break;	   
    case ( kJEnu2012a_nd5numub1    ) : return jnubeam::rew::kEnu2012aTwkDial_nd5numub1  ; break;	   
    case ( kJEnu2012a_nd5numub2    ) : return jnubeam::rew::kEnu2012aTwkDial_nd5numub2  ; break;	   
    case ( kJEnu2012a_nd5numub3    ) : return jnubeam::rew::kEnu2012aTwkDial_nd5numub3  ; break;	   
    case ( kJEnu2012a_nd5numub4    ) : return jnubeam::rew::kEnu2012aTwkDial_nd5numub4  ; break;	   
    case ( kJEnu2012a_nd5nue0      ) : return jnubeam::rew::kEnu2012aTwkDial_nd5nue0    ; break;	     
    case ( kJEnu2012a_nd5nue1      ) : return jnubeam::rew::kEnu2012aTwkDial_nd5nue1    ; break;	     
    case ( kJEnu2012a_nd5nue2      ) : return jnubeam::rew::kEnu2012aTwkDial_nd5nue2    ; break;	     
    case ( kJEnu2012a_nd5nue3      ) : return jnubeam::rew::kEnu2012aTwkDial_nd5nue3    ; break;	     
    case ( kJEnu2012a_nd5nue4      ) : return jnubeam::rew::kEnu2012aTwkDial_nd5nue4    ; break;	     
    case ( kJEnu2012a_nd5nue5      ) : return jnubeam::rew::kEnu2012aTwkDial_nd5nue5    ; break;	     
    case ( kJEnu2012a_nd5nue6      ) : return jnubeam::rew::kEnu2012aTwkDial_nd5nue6    ; break;	     
    case ( kJEnu2012a_nd5nueb0     ) : return jnubeam::rew::kEnu2012aTwkDial_nd5nueb0   ; break;	    
    case ( kJEnu2012a_nd5nueb1     ) : return jnubeam::rew::kEnu2012aTwkDial_nd5nueb1   ; break;	    
    case ( kJEnu2012a_sknumu0      ) : return jnubeam::rew::kEnu2012aTwkDial_sknumu0    ; break;	     
    case ( kJEnu2012a_sknumu1      ) : return jnubeam::rew::kEnu2012aTwkDial_sknumu1    ; break;	     
    case ( kJEnu2012a_sknumu2      ) : return jnubeam::rew::kEnu2012aTwkDial_sknumu2    ; break;	     
    case ( kJEnu2012a_sknumu3      ) : return jnubeam::rew::kEnu2012aTwkDial_sknumu3    ; break;	     
    case ( kJEnu2012a_sknumu4      ) : return jnubeam::rew::kEnu2012aTwkDial_sknumu4    ; break;	     
    case ( kJEnu2012a_sknumu5      ) : return jnubeam::rew::kEnu2012aTwkDial_sknumu5    ; break;	     
    case ( kJEnu2012a_sknumu6      ) : return jnubeam::rew::kEnu2012aTwkDial_sknumu6    ; break;	     
    case ( kJEnu2012a_sknumu7      ) : return jnubeam::rew::kEnu2012aTwkDial_sknumu7    ; break;	     
    case ( kJEnu2012a_sknumu8      ) : return jnubeam::rew::kEnu2012aTwkDial_sknumu8    ; break;	     
    case ( kJEnu2012a_sknumu9      ) : return jnubeam::rew::kEnu2012aTwkDial_sknumu9    ; break;	     
    case ( kJEnu2012a_sknumu10     ) : return jnubeam::rew::kEnu2012aTwkDial_sknumu10   ; break;	    
    case ( kJEnu2012a_sknumub0     ) : return jnubeam::rew::kEnu2012aTwkDial_sknumub0   ; break;	    
    case ( kJEnu2012a_sknumub1     ) : return jnubeam::rew::kEnu2012aTwkDial_sknumub1   ; break;	    
    case ( kJEnu2012a_sknumub2     ) : return jnubeam::rew::kEnu2012aTwkDial_sknumub2   ; break;	    
    case ( kJEnu2012a_sknumub3     ) : return jnubeam::rew::kEnu2012aTwkDial_sknumub3   ; break;	    
    case ( kJEnu2012a_sknumub4     ) : return jnubeam::rew::kEnu2012aTwkDial_sknumub4   ; break;	    
    case ( kJEnu2012a_sknue0       ) : return jnubeam::rew::kEnu2012aTwkDial_sknue0     ; break;	      
    case ( kJEnu2012a_sknue1       ) : return jnubeam::rew::kEnu2012aTwkDial_sknue1     ; break;	      
    case ( kJEnu2012a_sknue2       ) : return jnubeam::rew::kEnu2012aTwkDial_sknue2     ; break;	      
    case ( kJEnu2012a_sknue3       ) : return jnubeam::rew::kEnu2012aTwkDial_sknue3     ; break;	      
    case ( kJEnu2012a_sknue4       ) : return jnubeam::rew::kEnu2012aTwkDial_sknue4     ; break;	      
    case ( kJEnu2012a_sknue5       ) : return jnubeam::rew::kEnu2012aTwkDial_sknue5     ; break;	      
    case ( kJEnu2012a_sknue6       ) : return jnubeam::rew::kEnu2012aTwkDial_sknue6     ; break;	      
    case ( kJEnu2012a_sknueb0      ) : return jnubeam::rew::kEnu2012aTwkDial_sknueb0    ; break;	     
    case ( kJEnu2012a_sknueb1      ) : return jnubeam::rew::kEnu2012aTwkDial_sknueb1    ; break;

    case ( kJEnu2013a_nd5numu0          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numu0;      break;          
    case ( kJEnu2013a_nd5numu1          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numu1;      break;          
    case ( kJEnu2013a_nd5numu2          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numu2;      break;          
    case ( kJEnu2013a_nd5numu3          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numu3;      break;          
    case ( kJEnu2013a_nd5numu4          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numu4;      break;          
    case ( kJEnu2013a_nd5numu5          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numu5;      break;          
    case ( kJEnu2013a_nd5numu6          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numu6;      break;          
    case ( kJEnu2013a_nd5numu7          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numu7;      break;          
    case ( kJEnu2013a_nd5numu8          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numu8;      break;          
    case ( kJEnu2013a_nd5numu9          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numu9;      break;          
    case ( kJEnu2013a_nd5numu10         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numu10;      break;
    case ( kJEnu2013a_nd5numu11         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numu11;      break;         
    case ( kJEnu2013a_nd5numu12         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numu12;      break;         
    case ( kJEnu2013a_nd5numu13         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numu13;      break;         
    case ( kJEnu2013a_nd5numu14         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numu14;      break;         
    case ( kJEnu2013a_nd5numu15         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numu15;      break;         
    case ( kJEnu2013a_nd5numu16         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numu16;      break;         
    case ( kJEnu2013a_nd5numu17         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numu17;      break;         
    case ( kJEnu2013a_nd5numu18         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numu18;      break;         
    case ( kJEnu2013a_nd5numu19         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numu19;      break;         
    case ( kJEnu2013a_nd5numu20         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numu20;      break;        
    case ( kJEnu2013a_nd5numub0         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numub0;      break;         
    case ( kJEnu2013a_nd5numub1         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numub1;      break;         
    case ( kJEnu2013a_nd5numub2         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numub2;      break;         
    case ( kJEnu2013a_nd5numub3         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numub3;      break;         
    case ( kJEnu2013a_nd5numub4         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numub4;      break;         
    case ( kJEnu2013a_nd5numub5         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numub5;      break;         
    case ( kJEnu2013a_nd5numub6         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numub6;      break;         
    case ( kJEnu2013a_nd5numub7         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numub7;      break;         
    case ( kJEnu2013a_nd5numub8         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numub8;      break;         
    case ( kJEnu2013a_nd5numub9         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numub9;      break;         
    case ( kJEnu2013a_nd5numub10        ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numub10;      break;     
    case ( kJEnu2013a_nd5numub11        ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numub11;      break;        
    case ( kJEnu2013a_nd5numub12        ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numub12;      break;        
    case ( kJEnu2013a_nd5numub13        ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numub13;      break;        
    case ( kJEnu2013a_nd5numub14        ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numub14;      break;        
    case ( kJEnu2013a_nd5numub15        ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numub15;      break;        
    case ( kJEnu2013a_nd5numub16        ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numub16;      break;        
    case ( kJEnu2013a_nd5numub17        ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numub17;      break;        
    case ( kJEnu2013a_nd5numub18        ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numub18;      break;        
    case ( kJEnu2013a_nd5numub19        ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numub19;      break;        
    case ( kJEnu2013a_nd5numub20        ) : return jnubeam::rew::kEnu2013aTwkDial_nd5numub20;      break;       
    case ( kJEnu2013a_nd5nue0           ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nue0;      break;           
    case ( kJEnu2013a_nd5nue1           ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nue1;      break;           
    case ( kJEnu2013a_nd5nue2           ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nue2;      break;           
    case ( kJEnu2013a_nd5nue3           ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nue3;      break;           
    case ( kJEnu2013a_nd5nue4           ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nue4;      break;           
    case ( kJEnu2013a_nd5nue5           ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nue5;      break;           
    case ( kJEnu2013a_nd5nue6           ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nue6;      break;           
    case ( kJEnu2013a_nd5nue7           ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nue7;      break;           
    case ( kJEnu2013a_nd5nue8           ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nue8;      break;           
    case ( kJEnu2013a_nd5nue9           ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nue9;      break;           
    case ( kJEnu2013a_nd5nue10          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nue10;      break;	     
    case ( kJEnu2013a_nd5nue11          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nue11;      break;          
    case ( kJEnu2013a_nd5nue12          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nue12;      break;          
    case ( kJEnu2013a_nd5nue13          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nue13;      break;          
    case ( kJEnu2013a_nd5nue14          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nue14;      break;          
    case ( kJEnu2013a_nd5nue15          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nue15;      break;          
    case ( kJEnu2013a_nd5nue16          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nue16;      break;          
    case ( kJEnu2013a_nd5nue17          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nue17;      break;          
    case ( kJEnu2013a_nd5nue18          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nue18;      break;          
    case ( kJEnu2013a_nd5nue19          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nue19;      break;          
    case ( kJEnu2013a_nd5nue20          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nue20;      break;         
    case ( kJEnu2013a_nd5nueb0          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nueb0;      break;          
    case ( kJEnu2013a_nd5nueb1          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nueb1;      break;          
    case ( kJEnu2013a_nd5nueb2          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nueb2;      break;          
    case ( kJEnu2013a_nd5nueb3          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nueb3;      break;          
    case ( kJEnu2013a_nd5nueb4          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nueb4;      break;          
    case ( kJEnu2013a_nd5nueb5          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nueb5;      break;          
    case ( kJEnu2013a_nd5nueb6          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nueb6;      break;          
    case ( kJEnu2013a_nd5nueb7          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nueb7;      break;          
    case ( kJEnu2013a_nd5nueb8          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nueb8;      break;          
    case ( kJEnu2013a_nd5nueb9          ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nueb9;      break;          
    case ( kJEnu2013a_nd5nueb10         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nueb10;      break;
    case ( kJEnu2013a_nd5nueb11         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nueb11;      break;         
    case ( kJEnu2013a_nd5nueb12         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nueb12;      break;         
    case ( kJEnu2013a_nd5nueb13         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nueb13;      break;         
    case ( kJEnu2013a_nd5nueb14         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nueb14;      break;         
    case ( kJEnu2013a_nd5nueb15         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nueb15;      break;         
    case ( kJEnu2013a_nd5nueb16         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nueb16;      break;         
    case ( kJEnu2013a_nd5nueb17         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nueb17;      break;         
    case ( kJEnu2013a_nd5nueb18         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nueb18;      break;         
    case ( kJEnu2013a_nd5nueb19         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nueb19;      break;         
    case ( kJEnu2013a_nd5nueb20         ) : return jnubeam::rew::kEnu2013aTwkDial_nd5nueb20;      break;        
    case ( kJEnu2013a_sknumu0           ) : return jnubeam::rew::kEnu2013aTwkDial_sknumu0;      break;           
    case ( kJEnu2013a_sknumu1           ) : return jnubeam::rew::kEnu2013aTwkDial_sknumu1;      break;           
    case ( kJEnu2013a_sknumu2           ) : return jnubeam::rew::kEnu2013aTwkDial_sknumu2;      break;           
    case ( kJEnu2013a_sknumu3           ) : return jnubeam::rew::kEnu2013aTwkDial_sknumu3;      break;           
    case ( kJEnu2013a_sknumu4           ) : return jnubeam::rew::kEnu2013aTwkDial_sknumu4;      break;           
    case ( kJEnu2013a_sknumu5           ) : return jnubeam::rew::kEnu2013aTwkDial_sknumu5;      break;           
    case ( kJEnu2013a_sknumu6           ) : return jnubeam::rew::kEnu2013aTwkDial_sknumu6;      break;           
    case ( kJEnu2013a_sknumu7           ) : return jnubeam::rew::kEnu2013aTwkDial_sknumu7;      break;           
    case ( kJEnu2013a_sknumu8           ) : return jnubeam::rew::kEnu2013aTwkDial_sknumu8;      break;           
    case ( kJEnu2013a_sknumu9           ) : return jnubeam::rew::kEnu2013aTwkDial_sknumu9;      break;           
    case ( kJEnu2013a_sknumu10          ) : return jnubeam::rew::kEnu2013aTwkDial_sknumu10;      break;	     
    case ( kJEnu2013a_sknumu11          ) : return jnubeam::rew::kEnu2013aTwkDial_sknumu11;      break;          
    case ( kJEnu2013a_sknumu12          ) : return jnubeam::rew::kEnu2013aTwkDial_sknumu12;      break;          
    case ( kJEnu2013a_sknumu13          ) : return jnubeam::rew::kEnu2013aTwkDial_sknumu13;      break;          
    case ( kJEnu2013a_sknumu14          ) : return jnubeam::rew::kEnu2013aTwkDial_sknumu14;      break;          
    case ( kJEnu2013a_sknumu15          ) : return jnubeam::rew::kEnu2013aTwkDial_sknumu15;      break;          
    case ( kJEnu2013a_sknumu16          ) : return jnubeam::rew::kEnu2013aTwkDial_sknumu16;      break;          
    case ( kJEnu2013a_sknumu17          ) : return jnubeam::rew::kEnu2013aTwkDial_sknumu17;      break;          
    case ( kJEnu2013a_sknumu18          ) : return jnubeam::rew::kEnu2013aTwkDial_sknumu18;      break;          
    case ( kJEnu2013a_sknumu19          ) : return jnubeam::rew::kEnu2013aTwkDial_sknumu19;      break;          
    case ( kJEnu2013a_sknumu20          ) : return jnubeam::rew::kEnu2013aTwkDial_sknumu20;      break;         
    case ( kJEnu2013a_sknumub0          ) : return jnubeam::rew::kEnu2013aTwkDial_sknumub0;      break;          
    case ( kJEnu2013a_sknumub1          ) : return jnubeam::rew::kEnu2013aTwkDial_sknumub1;      break;          
    case ( kJEnu2013a_sknumub2          ) : return jnubeam::rew::kEnu2013aTwkDial_sknumub2;      break;          
    case ( kJEnu2013a_sknumub3          ) : return jnubeam::rew::kEnu2013aTwkDial_sknumub3;      break;          
    case ( kJEnu2013a_sknumub4          ) : return jnubeam::rew::kEnu2013aTwkDial_sknumub4;      break;          
    case ( kJEnu2013a_sknumub5          ) : return jnubeam::rew::kEnu2013aTwkDial_sknumub5;      break;          
    case ( kJEnu2013a_sknumub6          ) : return jnubeam::rew::kEnu2013aTwkDial_sknumub6;      break;          
    case ( kJEnu2013a_sknumub7          ) : return jnubeam::rew::kEnu2013aTwkDial_sknumub7;      break;          
    case ( kJEnu2013a_sknumub8          ) : return jnubeam::rew::kEnu2013aTwkDial_sknumub8;      break;          
    case ( kJEnu2013a_sknumub9          ) : return jnubeam::rew::kEnu2013aTwkDial_sknumub9;      break;          
    case ( kJEnu2013a_sknumub10         ) : return jnubeam::rew::kEnu2013aTwkDial_sknumub10;      break;	  
    case ( kJEnu2013a_sknumub11         ) : return jnubeam::rew::kEnu2013aTwkDial_sknumub11;      break;         
    case ( kJEnu2013a_sknumub12         ) : return jnubeam::rew::kEnu2013aTwkDial_sknumub12;      break;         
    case ( kJEnu2013a_sknumub13         ) : return jnubeam::rew::kEnu2013aTwkDial_sknumub13;      break;         
    case ( kJEnu2013a_sknumub14         ) : return jnubeam::rew::kEnu2013aTwkDial_sknumub14;      break;         
    case ( kJEnu2013a_sknumub15         ) : return jnubeam::rew::kEnu2013aTwkDial_sknumub15;      break;         
    case ( kJEnu2013a_sknumub16         ) : return jnubeam::rew::kEnu2013aTwkDial_sknumub16;      break;         
    case ( kJEnu2013a_sknumub17         ) : return jnubeam::rew::kEnu2013aTwkDial_sknumub17;      break;         
    case ( kJEnu2013a_sknumub18         ) : return jnubeam::rew::kEnu2013aTwkDial_sknumub18;      break;         
    case ( kJEnu2013a_sknumub19         ) : return jnubeam::rew::kEnu2013aTwkDial_sknumub19;      break;         
    case ( kJEnu2013a_sknumub20         ) : return jnubeam::rew::kEnu2013aTwkDial_sknumub20;      break;        
    case ( kJEnu2013a_sknue0            ) : return jnubeam::rew::kEnu2013aTwkDial_sknue0;      break;  
    case ( kJEnu2013a_sknue1            ) : return jnubeam::rew::kEnu2013aTwkDial_sknue1;      break;   	  
    case ( kJEnu2013a_sknue2            ) : return jnubeam::rew::kEnu2013aTwkDial_sknue2;      break;   	  
    case ( kJEnu2013a_sknue3            ) : return jnubeam::rew::kEnu2013aTwkDial_sknue3;      break;   	       case ( kJEnu2013a_sknue4            ) : return jnubeam::rew::kEnu2013aTwkDial_sknue4;      break;   	          case ( kJEnu2013a_sknue5            ) : return jnubeam::rew::kEnu2013aTwkDial_sknue5;      break;   	        
    case ( kJEnu2013a_sknue6            ) : return jnubeam::rew::kEnu2013aTwkDial_sknue6;      break;   	       case ( kJEnu2013a_sknue7            ) : return jnubeam::rew::kEnu2013aTwkDial_sknue7;      break;   	          case ( kJEnu2013a_sknue8            ) : return jnubeam::rew::kEnu2013aTwkDial_sknue8;      break;   	        
    case ( kJEnu2013a_sknue9            ) : return jnubeam::rew::kEnu2013aTwkDial_sknue9;      break;   	       case ( kJEnu2013a_sknue10           ) : return jnubeam::rew::kEnu2013aTwkDial_sknue10;      break;	     
    case ( kJEnu2013a_sknue11           ) : return jnubeam::rew::kEnu2013aTwkDial_sknue11;      break;           
    case ( kJEnu2013a_sknue12           ) : return jnubeam::rew::kEnu2013aTwkDial_sknue12;      break;           
    case ( kJEnu2013a_sknue13           ) : return jnubeam::rew::kEnu2013aTwkDial_sknue13;      break;           
    case ( kJEnu2013a_sknue14           ) : return jnubeam::rew::kEnu2013aTwkDial_sknue14;      break;           
    case ( kJEnu2013a_sknue15           ) : return jnubeam::rew::kEnu2013aTwkDial_sknue15;      break;           
    case ( kJEnu2013a_sknue16           ) : return jnubeam::rew::kEnu2013aTwkDial_sknue16;      break;           
    case ( kJEnu2013a_sknue17           ) : return jnubeam::rew::kEnu2013aTwkDial_sknue17;      break;           
    case ( kJEnu2013a_sknue18           ) : return jnubeam::rew::kEnu2013aTwkDial_sknue18;      break;           
    case ( kJEnu2013a_sknue19           ) : return jnubeam::rew::kEnu2013aTwkDial_sknue19;      break;           
    case ( kJEnu2013a_sknue20           ) : return jnubeam::rew::kEnu2013aTwkDial_sknue20;      break;
    case ( kJEnu2013a_sknueb0           ) : return jnubeam::rew::kEnu2013aTwkDial_sknueb0;      break;           
    case ( kJEnu2013a_sknueb1           ) : return jnubeam::rew::kEnu2013aTwkDial_sknueb1;      break;           
    case ( kJEnu2013a_sknueb2           ) : return jnubeam::rew::kEnu2013aTwkDial_sknueb2;      break;           
    case ( kJEnu2013a_sknueb3           ) : return jnubeam::rew::kEnu2013aTwkDial_sknueb3;      break;           
    case ( kJEnu2013a_sknueb4           ) : return jnubeam::rew::kEnu2013aTwkDial_sknueb4;      break;           
    case ( kJEnu2013a_sknueb5           ) : return jnubeam::rew::kEnu2013aTwkDial_sknueb5;      break;           
    case ( kJEnu2013a_sknueb6           ) : return jnubeam::rew::kEnu2013aTwkDial_sknueb6;      break;           
    case ( kJEnu2013a_sknueb7           ) : return jnubeam::rew::kEnu2013aTwkDial_sknueb7;      break;           
    case ( kJEnu2013a_sknueb8           ) : return jnubeam::rew::kEnu2013aTwkDial_sknueb8;      break;           
    case ( kJEnu2013a_sknueb9           ) : return jnubeam::rew::kEnu2013aTwkDial_sknueb9;      break;           
    case ( kJEnu2013a_sknueb10          ) : return jnubeam::rew::kEnu2013aTwkDial_sknueb10;      break;	     
    case ( kJEnu2013a_sknueb11          ) : return jnubeam::rew::kEnu2013aTwkDial_sknueb11;      break;          
    case ( kJEnu2013a_sknueb12          ) : return jnubeam::rew::kEnu2013aTwkDial_sknueb12;      break;          
    case ( kJEnu2013a_sknueb13          ) : return jnubeam::rew::kEnu2013aTwkDial_sknueb13;      break;          
    case ( kJEnu2013a_sknueb14          ) : return jnubeam::rew::kEnu2013aTwkDial_sknueb14;      break;          
    case ( kJEnu2013a_sknueb15          ) : return jnubeam::rew::kEnu2013aTwkDial_sknueb15;      break;          
    case ( kJEnu2013a_sknueb16          ) : return jnubeam::rew::kEnu2013aTwkDial_sknueb16;      break;          
    case ( kJEnu2013a_sknueb17          ) : return jnubeam::rew::kEnu2013aTwkDial_sknueb17;      break;          
    case ( kJEnu2013a_sknueb18          ) : return jnubeam::rew::kEnu2013aTwkDial_sknueb18;      break;          
    case ( kJEnu2013a_sknueb19          ) : return jnubeam::rew::kEnu2013aTwkDial_sknueb19;      break;          
    case ( kJEnu2013a_sknueb20          ) : return jnubeam::rew::kEnu2013aTwkDial_sknueb20;      break;        

    default:
      return jnubeam::rew::kNullSystematic;
      break;
  }
  return jnubeam::rew::kNullSystematic;
}
//_______________________________________________________________________________________
#ifdef __T2KRW_OAANALYSIS_ENABLED__
//_______________________________________________________________________________________
jnubeam::rew::JEvent * T2KJNuBeamUtils::GetJNuBeamEvent(const ND::RooTrackerVtxBase * vtx)
{

  jnubeam::rew::JEvent * fDummyJEvent = NULL;

  // Really messy way to distinguish between NEUT or GENIE RooTracker

  const ND::NRooTrackerVtx * nrootrac_vtx = dynamic_cast<const ND::NRooTrackerVtx*>(vtx);
  if(nrootrac_vtx == NULL){
    
    const ND::GRooTrackerVtx * grootrac_vtx = dynamic_cast<const ND::GRooTrackerVtx*>(vtx);
    
    if (grootrac_vtx == NULL){
      cout << "Cannot cast from ND::RooTrackerVtxBase to ND::G(N)RooTrackerVtx objects!" << endl;
      return fDummyJEvent;
    } else {
      
      
      fDummyJEvent = new jnubeam::rew::JEvent();
      fDummyJEvent->enu = grootrac_vtx->StdHepP4[0][3];
      fDummyJEvent->xnu[0] = grootrac_vtx->NuXnu[0];
      fDummyJEvent->xnu[1] = grootrac_vtx->NuXnu[1];
      fDummyJEvent->mode = grootrac_vtx->NuParentDecMode;
      fDummyJEvent->idfd = grootrac_vtx->NuIdfd;
      fDummyJEvent->norm = grootrac_vtx->NuNorm;
      fDummyJEvent->bpos[0] = grootrac_vtx->NuBpos[0];
      fDummyJEvent->bpos[1] = grootrac_vtx->NuBpos[1];
      fDummyJEvent->btilt[0] = grootrac_vtx->NuBtilt[0];
      fDummyJEvent->btilt[1] = grootrac_vtx->NuBtilt[1];
      fDummyJEvent->brms[0] = grootrac_vtx->NuBrms[0];
      fDummyJEvent->brms[1] = grootrac_vtx->NuBrms[1];
      //fDummyJEvent->emit[0] = grootrac_vtx->NuEmit[0];
      //fDummyJEvent->emit[1] = grootrac_vtx->NuEmit[1];
      //Set to 0 automatically for now
      fDummyJEvent->emit[0] = 0.;
      fDummyJEvent->emit[1] = 0.;
      fDummyJEvent->alpha[0] = grootrac_vtx->NuAlpha[0];
      fDummyJEvent->alpha[1] = grootrac_vtx->NuAlpha[1];
      fDummyJEvent->hcur[0] = grootrac_vtx->NuHcur[0];
      fDummyJEvent->hcur[1] = grootrac_vtx->NuHcur[1];
      fDummyJEvent->hcur[2] = grootrac_vtx->NuHcur[2];

      //      std::cout << "Int chain lengths in grootrac " << grootrac_vtx->NuNg << std::endl;
      for (int ig=0; ig<grootrac_vtx->NuNg; ig++) {

	JIntLink fDummyJIntChain;

	for (int i=0; i<3; i++) {
	  fDummyJIntChain.pos[i] = grootrac_vtx->NuGv[ig][i];
	  fDummyJIntChain.mom[i] = grootrac_vtx->NuGp[ig][i];
	}

	fDummyJIntChain.pid = grootrac_vtx->NuGpid[ig];
	fDummyJIntChain.mec = grootrac_vtx->NuGmec[ig];
	fDummyJIntChain.mat = grootrac_vtx->NuGmat[ig];
	fDummyJIntChain.distc = grootrac_vtx->NuGdistc[ig];
	fDummyJIntChain.distal = grootrac_vtx->NuGdistal[ig];
	fDummyJIntChain.distti = grootrac_vtx->NuGdistti[ig];
	fDummyJIntChain.distfe = grootrac_vtx->NuGdistfe[ig];
    
	(fDummyJEvent->int_chain).push_back (fDummyJIntChain);
  
      }
      fDummyJEvent->CalcTheta();
    }
  } else {
    
    fDummyJEvent = new jnubeam::rew::JEvent();
    fDummyJEvent->enu = nrootrac_vtx->StdHepP4[0][3];
    fDummyJEvent->xnu[0] = nrootrac_vtx->NuXnu[0];
    fDummyJEvent->xnu[1] = nrootrac_vtx->NuXnu[1];
    fDummyJEvent->mode = nrootrac_vtx->NuParentDecMode;
    fDummyJEvent->idfd = nrootrac_vtx->NuIdfd;
    fDummyJEvent->norm = nrootrac_vtx->NuNorm;
    fDummyJEvent->bpos[0] = nrootrac_vtx->NuBpos[0];
    fDummyJEvent->bpos[1] = nrootrac_vtx->NuBpos[1];
    fDummyJEvent->btilt[0] = nrootrac_vtx->NuBtilt[0];
    fDummyJEvent->btilt[1] = nrootrac_vtx->NuBtilt[1];
    fDummyJEvent->brms[0] = nrootrac_vtx->NuBrms[0];
    fDummyJEvent->brms[1] = nrootrac_vtx->NuBrms[1];
    //fDummyJEvent->emit[0] = nrootrac_vtx->NuEmit[0];
    //fDummyJEvent->emit[1] = nrootrac_vtx->NuEmit[1];
    //Set to 0 automatically for now
    fDummyJEvent->emit[0] = 0.;
    fDummyJEvent->emit[1] = 0.;
    fDummyJEvent->alpha[0] = nrootrac_vtx->NuAlpha[0];
    fDummyJEvent->alpha[1] = nrootrac_vtx->NuAlpha[1];
    fDummyJEvent->hcur[0] = nrootrac_vtx->NuHcur[0];
    fDummyJEvent->hcur[1] = nrootrac_vtx->NuHcur[1];
    fDummyJEvent->hcur[2] = nrootrac_vtx->NuHcur[2];

    for (int ig=0; ig<nrootrac_vtx->NuNg; ig++) {

      JIntLink fDummyJIntChain;

      for (int i=0; i<3; i++) {
	fDummyJIntChain.pos[i] = nrootrac_vtx->NuGv[ig][i];
	fDummyJIntChain.mom[i] = nrootrac_vtx->NuGp[ig][i];
      }

      fDummyJIntChain.pid = nrootrac_vtx->NuGpid[ig];
      fDummyJIntChain.mec = nrootrac_vtx->NuGmec[ig];
      fDummyJIntChain.mat = nrootrac_vtx->NuGmat[ig];
      fDummyJIntChain.distc = nrootrac_vtx->NuGdistc[ig];
      fDummyJIntChain.distal = nrootrac_vtx->NuGdistal[ig];
      fDummyJIntChain.distti = nrootrac_vtx->NuGdistti[ig];
      fDummyJIntChain.distfe = nrootrac_vtx->NuGdistfe[ig];
    
      (fDummyJEvent->int_chain).push_back (fDummyJIntChain);
  
    }
    fDummyJEvent->CalcTheta();
  }
  
  return fDummyJEvent;
  
}

#endif // __T2KRW_OAANALYSIS_ENABLED__
//_______________________________________________________________________________________
#endif // __T2KRW_JNUBEAM_ENABLED__ 
 
