//____________________________________________________________________________
/*!

\class    t2krew::T2KSyst_t

\brief    An enumeration of T2K systematic parameters. This includes GENIE, 
          NEUT, JNuBeam and any other weighting engine parameters. This is 
          the list of systematics that the T2K user comes into contact with 
          and it is the responsibility of the reweighting engines to convert
          from these to a native format. Adapted from previous version of 
          T2KReWeight and the GENIE reweighting tools. 

\author   Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
          STFC, Rutherford Appleton Laboratory

          Jim Dobson <J.Dobson07 \at imperial.ac.uk>
          Imperial College London

          Patrick de Perio <pdeperio \at physics.utoronto.ca>
          University of Toronto

\created  Nov 1, 2010
*/
//____________________________________________________________________________

#ifndef _T2KRW_SYSTEMATIC_H_
#define _T2KRW_SYSTEMATIC_H_

#include <iostream>

#include <string>

#include "T2KBuild.h"

using std::string;

namespace t2krew {

typedef enum ET2KSyst {

  kSystNull = 0,

  // ================== Start of GENIE parameters ===================
  //
  // GENIE systematic paramters available for reweighting.  
  // 
  // Each systematic listed here corresponds to a genie reweighting
  // systematic as defined in $GENIE/src/ReWeight/GSyst.h. There are
  // many physics parameters included for reweighting and it is the
  // users responsibility to ensure understanding of the set of
  // parameter being tweaked. For example, some of the parameters
  // provide different ways of reweighting the same physics process
  // and should not be used in conjunction with each other. In order
  // to get meaningful results the user should read the reweighting
  // section in the GENIE Physics and Users Manual (see:
  // http://www.genie-mc.org/ for latest version).
 
  //
  // GENIE Neutrino cross section systematics 
  // NCEL parameters
  kGXSec_MaNCEL,            ///< tweak MA for NCEL, affects shape and normalization
  kGXSec_EtaNCEL,           ///< tweak eta, the F_A^s(0) (strange axial form factor)


  // CCQE tweaking parameters:
  kGXSec_NormCCQE,          ///< tweak CCQE normalization (energy independent)
  kGXSec_NormCCQEenu,       ///< tweak CCQE normalization (maintains dependence on neutrino energy)
  kGXSec_MaCCQEshape,       ///< tweak Ma CCQE, affects dsigma(CCQE)/dQ2 in shape only (normalized to constant integral)
  kGXSec_MaCCQE,            ///< tweak Ma CCQE, affects dsigma(CCQE)/dQ2 both in shape and normalization
  kGXSec_VecFFCCQEshape,    ///< tweak elastic nucleon form factors (BBA/default -> dipole) - shape only effect of dsigma(CCQE)/dQ2
 

  // Resonance neutrino-production tweaking parameters:
  kGXSec_NormCCRES,         ///< tweak CCRES normalization
  kGXSec_MaCCRESshape,      ///< tweak Ma CCRES, affects d2sigma(CCRES)/dWdQ2 in shape only (normalized to constant integral)
  kGXSec_MvCCRESshape,      ///< tweak Mv CCRES, affects d2sigma(CCRES)/dWdQ2 in shape only (normalized to constant integral)
  kGXSec_MaCCRES,           ///< tweak Ma CCRES, affects d2sigma(CCRES)/dWdQ2 both in shape and normalization
  kGXSec_MvCCRES,           ///< tweak Mv CCRES, affects d2sigma(CCRES)/dWdQ2 both in shape and normalization
  kGXSec_NormNCRES,         ///< tweak NCRES normalization
  kGXSec_MaNCRESshape,      ///< tweak Ma NCRES, affects d2sigma(NCRES)/dWdQ2 in shape only (normalized to constant integral)
  kGXSec_MvNCRESshape,      ///< tweak Mv NCRES, affects d2sigma(NCRES)/dWdQ2 in shape only (normalized to constant integral)
  kGXSec_MaNCRES,           ///< tweak Ma NCRES, affects d2sigma(NCRES)/dWdQ2 both in shape and normalization
  kGXSec_MvNCRES,           ///< tweak Mv NCRES, affects d2sigma(NCRES)/dWdQ2 both in shape and normalization
  // Coherent pion production tweaking parameters:
  kGXSec_MaCOHpi,           ///< tweak Ma for COH pion production
  kGXSec_R0COHpi,           ///< tweak R0 for COH pion production
  // Non-resonance background tweaking parameters:
  kGXSec_RvpCC1pi,          ///< tweak the 1pi non-RES bkg in the RES region, for v+p CC
  kGXSec_RvpCC2pi,          ///< tweak the 2pi non-RES bkg in the RES region, for v+p CC
  kGXSec_RvpNC1pi,          ///< tweak the 1pi non-RES bkg in the RES region, for v+p NC
  kGXSec_RvpNC2pi,          ///< tweak the 2pi non-RES bkg in the RES region, for v+p NC
  kGXSec_RvnCC1pi,          ///< tweak the 1pi non-RES bkg in the RES region, for v+n CC
  kGXSec_RvnCC2pi,          ///< tweak the 2pi non-RES bkg in the RES region, for v+n CC
  kGXSec_RvnNC1pi,          ///< tweak the 1pi non-RES bkg in the RES region, for v+n NC
  kGXSec_RvnNC2pi,          ///< tweak the 2pi non-RES bkg in the RES region, for v+n NC
  kGXSec_RvbarpCC1pi,       ///< tweak the 1pi non-RES bkg in the RES region, for vbar+p CC
  kGXSec_RvbarpCC2pi,       ///< tweak the 2pi non-RES bkg in the RES region, for vbar+p CC
  kGXSec_RvbarpNC1pi,       ///< tweak the 1pi non-RES bkg in the RES region, for vbar+p NC
  kGXSec_RvbarpNC2pi,       ///< tweak the 2pi non-RES bkg in the RES region, for vbar+p NC
  kGXSec_RvbarnCC1pi,       ///< tweak the 1pi non-RES bkg in the RES region, for vbar+n CC
  kGXSec_RvbarnCC2pi,       ///< tweak the 2pi non-RES bkg in the RES region, for vbar+n CC
  kGXSec_RvbarnNC1pi,       ///< tweak the 1pi non-RES bkg in the RES region, for vbar+n NC
  kGXSec_RvbarnNC2pi,       ///< tweak the 2pi non-RES bkg in the RES region, for vbar+n NC
  // DIS tweaking parameters - applied for DIS events with (Q2>Q2o, W>Wo), typically Q2o=1GeV^2, Wo=1.7-2.0GeV
  kGXSec_AhtBY,             ///< tweak the Bodek-Yang model parameter A_{ht} - incl. both shape and normalization effect
  kGXSec_BhtBY,             ///< tweak the Bodek-Yang model parameter B_{ht} - incl. both shape and normalization effect 
  kGXSec_CV1uBY,            ///< tweak the Bodek-Yang model parameter CV1u - incl. both shape and normalization effect 
  kGXSec_CV2uBY,            ///< tweak the Bodek-Yang model parameter CV2u - incl. both shape and normalization effect 
  kGXSec_AhtBYshape,        ///< tweak the Bodek-Yang model parameter A_{ht} - shape only effect to d2sigma(DIS)/dxdy
  kGXSec_BhtBYshape,        ///< tweak the Bodek-Yang model parameter B_{ht} - shape only effect to d2sigma(DIS)/dxdy
  kGXSec_CV1uBYshape,       ///< tweak the Bodek-Yang model parameter CV1u - shape only effect to d2sigma(DIS)/dxdy
  kGXSec_CV2uBYshape,       ///< tweak the Bodek-Yang model parameter CV2u - shape only effect to d2sigma(DIS)/dxdy
  kGXSec_NormDISCC,         ///< tweak the inclusive DIS CC normalization
  kGXSec_RnubarnuCC,        ///< tweak the ratio of \sigma(\bar\nu CC) / \sigma(\nu CC)
  kGXSec_DISNuclMod,        ///< tweak DIS nuclear modification (shadowing, anti-shadowing, EMC)
  kGXSec_NC,                ///< 
  //
  // GENIE Hadronization (free nucleon target) parameters
  kGHadrAGKY_xF1pi,         ///< tweak xF distribution for low multiplicity (N + pi) DIS f/s produced by AGKY
  kGHadrAGKY_pT1pi,         ///< tweak pT distribution for low multiplicity (N + pi) DIS f/s produced by AGKY
  //
  // GENIE Medium-effects to hadronization parameters
  kGHadrNucl_FormZone,         ///< tweak formation zone
  //
  // GENIE Intranuclear rescattering systematics.
  // There are 2 sets of parameters:
  // - parameters that control the total rescattering probability, P(total)
  // - parameters that control the fraction of each process (`fate'), given a total rescat. prob., P(fate|total)
  // These parameters are considered separately for pions and nucleons.
  kGINuke_MFP_pi,      ///< tweak mean free path for pions
  kGINuke_MFP_N,       ///< tweak mean free path for nucleons
  kGINuke_FrCEx_pi,    ///< tweak charge exchange probability for pions, for given total rescattering probability
  kGINuke_FrElas_pi,   ///< tweak elastic         probability for pions, for given total rescattering probability
  kGINuke_FrInel_pi,   ///< tweak inelastic       probability for pions, for given total rescattering probability
  kGINuke_FrAbs_pi,    ///< tweak absorption      probability for pions, for given total rescattering probability
  kGINuke_FrPiProd_pi, ///< tweak pion production probability for pions, for given total rescattering probability
  kGINuke_FrCEx_N,     ///< tweak charge exchange probability for nucleons, for given total rescattering probability
  kGINuke_FrElas_N,    ///< tweak elastic         probability for nucleons, for given total rescattering probability
  kGINuke_FrInel_N,    ///< tweak inelastic       probability for nucleons, for given total rescattering probability
  kGINuke_FrAbs_N,     ///< tweak absorption      probability for nucleons, for given total rescattering probability
  kGINuke_FrPiProd_N,  ///< tweak pion production probability for nucleons, for given total rescattering probability
  //
  // GENIE Nuclear model parameters
  kGNucl_CCQEPauliSupViaKF,   ///<
  kGNucl_CCQEMomDistroFGtoSF, ///<
  //
  // GENIE Resonance decay parameters
  kGRDcy_BR1gamma,        ///< tweak Resonance -> X + gamma branching ratio, eg Delta+(1232) -> p gamma
  kGRDcy_BR1eta,          ///< tweak Resonance -> X + eta   branching ratio, eg N+(1440) -> p eta
  kGRDcy_Theta_Delta2Npi,  ///< distort pi angular distribution in Delta -> N + pi
  //
  //
  // ================== End of GENIE parameters ===================


  // ================== Start of NEUT parameters ===================
  //
  // NEUT systematic paramters available for reweighting.  
  // 
  // Each systematic listed here corresponds to a neut reweighting
  // systematic as defined in $NEUT_ROOT/src/reweight/NSyst.h. There are
  // many physics parameters included for reweighting and it is the
  // users responsibility to ensure understanding of the set of
  // parameter being tweaked. For example, some of the parameters
  // provide different ways of reweighting the same physics process
  // and should not be used in conjunction with each other. 
  //
  // Note: This list comes from GENIE systematics, many of which are 
  //       not currently implemented in NEUT (commented out) 
  //
  // NEUT Neutrino cross section systematics 

  // NCEL tweaking parameters:
  kNXSec_MaNCEL,            ///< tweak Ma NCEL, affects dsigma(NCEL)/dQ2 both in shape and normalization
  kNXSec_NormNCEL,          ///< tweak NCEL normalization (energy independent)
  kNXSec_MaNCELshape,       ///< tweak Ma NCEL, affects dsigma(NCEL)/dQ2 in shape
  kNXSec_1overMaNCEL2,       ///< tweak 1/MaNCEL^2, affects dsigma(NCEL)/dQ2. More symmetric response in cross section when assuming Gaussian errors on this parameter
  kNXSec_AxlFFNCEL,         ///< tweak elastic nucleon form factors (dipole/default -> BBBA07) NOT VALIDATED
  kNXSec_VecFFNCEL,         ///< tweak elastic nucleon form factors (dipole/default -> BBBA05)
  //kNXSec_EtaNCEL,           ///< tweak NCEL strange axial form factor eta, affects dsigma(NCEL)/dQ2 both in shape and normalization

  // CCQE tweaking parameters:
  kNXSec_NormCCQE,          ///< tweak CCQE normalization (energy independent)
  //kNXSec_NormCCQEenu,       ///< tweak CCQE normalization (maintains dependence on neutrino energy)
  kNXSec_MaCCQEshape,       ///< tweak Ma CCQE, affects dsigma(CCQE)/dQ2 in shape only (normalized to constant integral)
  kNXSec_1overMaCCQE2,       ///< tweak 1/MaCCQE^2, affects dsigma(CCQE)/dQ2. More symmetric response in cross section when assuming Gaussian errors on this parameter
  kNXSec_MaCCQE,            ///< tweak Ma CCQE, affects dsigma(CCQE)/dQ2 both in shape and normalization
  kNXSec_AxlFFCCQE,         ///< tweak elastic nucleon form factors (dipole/default -> BBBA07) NOT VALIDATED	  
  kNXSec_VecFFCCQE,         ///< tweak elastic nucleon form factors (dipole/default -> BBBA05)	 
  kNXSec_VecFFCCQE_out,     ///< tweak elastic nucleon form factors (dipole/default -> BBBA05)	 
  //kNXSec_VecFFCCQEshape,  ///< tweak elastic nucleon form factors (BBA/default -> dipole) - shape only effect of dsigma(CCQE)/dQ2
  
  kNXSec_SCCVecQE,         ///< tweak vector second class current
  kNXSec_SCCAxlQE,         ///< tweak axial second class current
  kNXSec_PsFF,             ///< tweak pseudoscalar form factor at Q2=0

   
   kNIWG_rpaCCQE_norm,           ///<tweak for RPA norm  correction on Carbon and Oxygen
   kNIWG_rpaCCQE_shape,          ///<tweak for RPA shape correction on Carbon and Oxygen
   kNIWG_rpaCCQE_norm_C12,       ///<tweak for RPA norm  correction on Carbon
   kNIWG_rpaCCQE_shape_C12,      ///<tweak for RPA shape correction on Carbon  
   kNIWG_rpaCCQE_norm_O16,       ///<tweak for RPA norm  correction on Oxygen
   kNIWG_rpaCCQE_shape_O16,      ///<tweak for RPA shape correction on Oxygen   

   kNIWG_Effective_rpaCCQE_Norm,  ///<tweak for ERPA norm and to turn off the ERPA dial. Nominal is off ERPA off
   kNIWG_Effective_rpaCCQE_U,     ///<tweak for ERPA U 
   kNIWG_Effective_rpaCCQE_A,     ///<tweak for ERPA A 
   kNIWG_Effective_rpaCCQE_B,     ///<tweak for ERPA B 
   kNIWG_Effective_rpaCCQE_C,     ///<tweak for ERPA C   
   kNIWG_Effective_rpaCCQE_D,     ///<tweak for ERPA D 
   kNIWG_Effective_rpaCCQE_Unom,     ///<tweak for ERPA Unom 
   kNIWG_Effective_rpaCCQE_Anom,     ///<tweak for ERPA Anom 
   kNIWG_Effective_rpaCCQE_Bnom,     ///<tweak for ERPA Bnom 
   kNIWG_Effective_rpaCCQE_Cnom,     ///<tweak for ERPA Cnom   
   kNIWG_Effective_rpaCCQE_Dnom,     ///<tweak for ERPA Dnom 

   kNIWG_Effective_rpaCCQE_Norm_forweight,  ///<tweak for ERPA norm and to turn off the ERPA dial. Nominal is off ERPA off
   kNIWG_Effective_rpaCCQE_U_forweight,     ///<tweak for ERPA U 
   kNIWG_Effective_rpaCCQE_A_forweight,     ///<tweak for ERPA A 
   kNIWG_Effective_rpaCCQE_B_forweight,     ///<tweak for ERPA B 
   kNIWG_Effective_rpaCCQE_C_forweight,     ///<tweak for ERPA C   
   kNIWG_Effective_rpaCCQE_D_forweight,     ///<tweak for ERPA D 

   
  // Resonance neutrino-production tweaking parameters:

  kNXSec_NormRES,         ///< tweak RES normalization
  kNXSec_MaRESshape,      ///< tweak Ma RES, affects d2sigma(CCRES)/dWdQ2 in shape only (normalized to constant integral)
  kNXSec_MaRES,           ///< tweak Ma RES, affects d2sigma(RES)/dWdQ2 both in shape and normalization
  kNXSec_MvRES,           ///< tweak Mv RES, affects d2sigma(RES)/dWdQ2 both in shape and normalization

  kNXSec_FFRES,          ///< tweak the form factor model used, affects single pion events only
  kNXSec_TypeRES,        ///< tweak form factors in the RS model, should only affect single pion events
  kNXSec_CA5RES,         ///< tweak CA5 for single pion interactions using Graczyk & Sobczyk FF
  kNXSec_BgSclRES,       ///< tweak I=1/2 background scale factor, single pion only
  kNXSec_MaNFFRES,        ///< tweak Ma RES, affects d2sigma(RES)/dWdQ2 both in shape and normalization
  kNXSec_MvNFFRES,        ///< tweak Mv RES, affects d2sigma(RES)/dWdQ2 both in shape and normalization
  kNXSec_MaRSRES,        ///< tweak Ma RES, affects d2sigma(RES)/dWdQ2 both in shape and normalization
  kNXSec_MvRSRES,        ///< tweak Mv RES, affects d2sigma(RES)/dWdQ2 both in shape and normalization

  kNXSec_NormCCRES,         ///< tweak CCRES normalization
  kNXSec_MaCCRESshape,      ///< tweak Ma CCRES, affects d2sigma(CCRES)/dWdQ2 in shape only (normalized to constant integral)
  //kNXSec_MvCCRESshape,      ///< tweak Mv CCRES, affects d2sigma(CCRES)/dWdQ2 in shape only (normalized to constant integral)
  kNXSec_MaCCRES,           ///< tweak Ma CCRES, affects d2sigma(CCRES)/dWdQ2 both in shape and normalization
  kNXSec_MvCCRES,           ///< tweak Mv CCRES, affects d2sigma(CCRES)/dWdQ2 both in shape and normalization

  kNXSec_FFCCRES,          ///< tweak the form factor model used, affects single pion events only
  kNXSec_TypeCCRES,        ///< tweak form factors in the RS model, should only affect single pion events
  kNXSec_CA5CCRES,         ///< tweak CA5 for single pion interactions using Graczyk & Sobczyk FF
  kNXSec_BgSclCCRES,       ///< tweak I=1/2 background scale factor, single pion only
  kNXSec_MaNFFCCRES,        ///< tweak Ma CCRES, affects d2sigma(CCRES)/dWdQ2 both in shape and normalization
  kNXSec_MvNFFCCRES,        ///< tweak Mv CCRES, affects d2sigma(CCRES)/dWdQ2 both in shape and normalization
  kNXSec_MaRSCCRES,        ///< tweak Ma CCRES, affects d2sigma(CCRES)/dWdQ2 both in shape and normalization
  kNXSec_MvRSCCRES,        ///< tweak Mv CCRES, affects d2sigma(CCRES)/dWdQ2 both in shape and normalization

  kNXSec_NormNCRES,         ///< tweak NCRES normalization
  kNXSec_MaNCRESshape,      ///< tweak Ma NCRES, affects d2sigma(NCRES)/dWdQ2 in shape only (normalized to constant integral)
  //kNXSec_MvNCRESshape,      ///< tweak Mv NCRES, affects d2sigma(NCRES)/dWdQ2 in shape only (normalized to constant integral)
  kNXSec_MaNCRES,           ///< tweak Ma NCRES, affects d2sigma(NCRES)/dWdQ2 both in shape and normalization
  kNXSec_MvNCRES,           ///< tweak Mv NCRES, affects d2sigma(NCRES)/dWdQ2 both in shape and normalization

  kNXSec_FFNCRES,          ///< tweak the form factor model used, affects single pion events only
  kNXSec_TypeNCRES,        ///< tweak form factors in the RS model, should only affect single pion events
  kNXSec_CA5NCRES,         ///< tweak CA5 for single pion interactions using Graczyk & Sobczyk FF
  kNXSec_BgSclNCRES,       ///< tweak I=1/2 background scale factor, single pion only
  kNXSec_MaNFFNCRES,        ///< tweak Ma NCRES, affects d2sigma(NCRES)/dWdQ2 both in shape and normalization
  kNXSec_MvNFFNCRES,        ///< tweak Mv NCRES, affects d2sigma(NCRES)/dWdQ2 both in shape and normalization
  kNXSec_MaRSNCRES,        ///< tweak Ma NCRES, affects d2sigma(NCRES)/dWdQ2 both in shape and normalization
  kNXSec_MvRSNCRES,        ///< tweak Mv NCRES, affects d2sigma(NCRES)/dWdQ2 both in shape and normalization

  // Coherent pion production tweaking parameters:
  kNXSec_NECOHEPI,          ///< tweak the model for COH pion production
  kNXSec_MaCOHpi,           ///< tweak Ma for COH pion production
  kNXSec_R0COHpi,           ///< tweak R0 for COH pion production
  kNXSec_fA1COHpi,          ///< tweak A1 error for COH pion production (B&S model)
  kNXSec_fb1COHpi,          ///< tweak b1 error for COH pion production (B&S model)

  kNXSec_NormDIS,            ///< tweak the inclusive DIS CC normalization
  kNXSec_BYOnOffDIS,            ///< Turn Bodek-ang correction on/off
  //kNXSec_RnubarnuCC,        ///< tweak the ratio of \sigma(\bar\nu CC) / \sigma(\nu CC)
  //kNXSec_DISNuclMod,        ///< tweak DIS nuclear modification (shadowing, anti-shadowing, EMC)
  kNXSec_NC,                ///< tweak the inclusive NC normalization

  // P.S (26.01.17) AxialFF Patch 
  kNXSec_AxlDipToAlt,

  kNXSec_FAxlCCQEAlpha,
  kNXSec_FAxlCCQEGamma,
  kNXSec_FAxlCCQEBeta, 
  kNXSec_FAxlCCQETheta,

  kNXSec_FAZExp_NTerms,
  kNXSec_FAZExp_TCut,  
  kNXSec_FAZExp_T0,    
  kNXSec_FAZExp_Q4Cut, 
  kNXSec_FAZExp_A0,    
  kNXSec_FAZExp_A1,    
  kNXSec_FAZExp_A2,    
  kNXSec_FAZExp_A3,    
  kNXSec_FAZExp_A4,    
  kNXSec_FAZExp_A5,    
  kNXSec_FAZExp_A6,    
  kNXSec_FAZExp_A7,
  kNXSec_FAZExp_A8,
  kNXSec_FAZExp_A9,

  //
  // NEUT Medium-effects to hadronization parameters
  //kNHadrNucl_FormZone,         ///< tweak formation zone
  //
  // NEUT Intranuclear rescattering systematics.
  // See T2K-TN-033
  kNCasc_FrAbs_pi,       ///< tweak absorption      probability for pions
  kNCasc_FrInelLow_pi,   ///< tweak inelastic (QE in NEUT) probability for low energy pions
  kNCasc_FrCExLow_pi,    ///< tweak charge exchange probability for low energy pions
  kNCasc_FrInelHigh_pi,  ///< tweak inelastic (QE in NEUT) probability for high energy pions
  kNCasc_FrCExHigh_pi,   ///< tweak charge exchange probability for high energy pions
  kNCasc_FrPiProd_pi,    ///< tweak pion (hadron) production (inelastic in NEUT) probability for pions

  // NEUT Nucleon FSI Cascade Model (intra-nuclear cascade)
  kNINuke_MFP_N,         ///< tweak mean free path for nucleons
  //kNINuke_FrCEx_N,     ///< tweak charge exchange probability for nucleons, for given total rescattering probability
  //kNINuke_FrElas_N,    ///< tweak elastic         probability for nucleons, for given total rescattering probability
  //kNINuke_FrInel_N,    ///< tweak inelastic       probability for nucleons, for given total rescattering probability
  //kNINuke_FrAbs_N,     ///< tweak absorption      probability for nucleons, for given total rescattering probability
  //kNINuke_FrPiProd_N,  ///< tweak pion production probability for nucleons, for given total rescattering probability
  //
  // NEUT Nuclear model parameters (Warning: these parameters require attention to the MC generated phase space)
  kNNucl_CCQEPauliSupViaKF,    ///<
  kNNucl_CCQEFermiSurfMom,     ///< Fermi Surface Momentum
  kNNucl_CCQEBindingEnergy,    ///< Binding Energy
  kNNucl_CCQEBindingEnergy_C12,
  kNNucl_CCQEBindingEnergy_O16,
  kNNucl_CCQEBindingEnergy_Al27,
  kNNucl_CCQEBindingEnergy_Fe56,
  kNNucl_CCQEBindingEnergy_Cu63,
  kNNucl_CCQEBindingEnergy_Zn64,
  kNNucl_CCQEBindingEnergy_Pb208,
  //kNNucl_CCQEMomDistroFGtoSF, ///<
  //
  // NEUT Resonance decay parameters
  //kNRDcy_BR1gamma,        ///< tweak Resonance -> X + gamma branching ratio, eg Delta+(1232) -> p gamma
  //kNRDcy_BR1eta,          ///< tweak Resonance -> X + eta   branching ratio, eg N+(1440) -> p eta
  //kNRDcy_Theta_Delta2Npi,  ///< distort pi angular distribution in Delta -> N + pi

  kNNucl_PilessDcyRES,
  kNNucl_PilessDcyOnlyRES,

  //
  //
  // ================== End of NEUT parameters ===================


  // ================== Start of JNuBeam parameters ===================
  //
  // JNuBeam systematic paramters available for reweighting. Defined 
  // in $JNUBEAM/JSyst.h
  //
  kJNA61pi_dedx,        ///< NA61 Particle ID for dedx analysis
  kJNA61pi_dedxtof,        ///< NA61 Particle ID for dedx-tof analysis
  kJNA61pi_feeddown,   ///< NA61 Feed down (secondary pions) and e- Contamination
  kJNA61pi_trktop,     ///< NA61 Track topology
  kJNA61pi_trkcut,     ///< NA61 Track cuts
  kJNA61pi_trkmrg,     ///< NA61 Track merging algorithm
  kJNA61pi_hcont,      ///< NA61 K- and anti proton (h- contamination)
  kJNA61pi_receff,     ///< NA61 Reconstruction efficiency
  kJNA61pi_tofeff,     ///< NA61 TOF efficiency
  kJNA61pi_piloss,     ///< NA61 Pion loss (decay and interaction)
  kJNA61pi_norm,     ///< NA61 normalization uncertainty
  
  kJkaon_bmptA,        ///< BMPT fit parameter A
  kJkaon_bmptB,        ///< BMPT fit parameter B
  kJkaon_bmptalpha,    ///< BMPT fit parameter alpha
  kJkaon_bmptbeta,     ///< BMPT fit parameter beta
  kJkaon_bmpta1,       ///< BMPT fit parameter a
  kJkaon_bmptgamma,    ///< BMPT fit parameter gamma
  kJkaon_bmptdelta,    ///< BMPT fit parameter delta

  kJTXsec_pLow,        ///< Total cross section of p-C <4 GeV/c <Chen>
  kJTXsec_ppiMid,      ///< Total cross section of p-C 4-12 GeV/c & pi-C 2-6 GeV/c <Bobchenko>
  kJTXsec_pHigh,       ///< Total cross section of p-C >12 GeV/c <NA61>
  kJTXsec_piLow,       ///< Total cross section of pi-C <2 GeV/c <Allardyce>
  kJTXsec_kLow,        ///< Total cross section of K-C <5 GeV/c <Abrams>
  kJTXsec_pikHigh,     ///< Total cross section of pi-C >6 GeV/c, (K+)-C >5 GeV/c, (K-)-C 5-17 GeV/c <Denisov>
  kJTXsec_kHigh,       ///< Total cross section of (K-)-C >17 GeV/c <Allaby>

  kJPbeam_xpos,       ///Beam x position at baffle
  kJPbeam_ypos,       ///Beam y position at baffle
  kJPbeam_xdir,       ///Beam x angle at baffle
  kJPbeam_ydir,       ///Beam y angle at baffle
  kJPbeam_xsig,       ///Beam x width at baffle
  kJPbeam_ysig,       ///Beam y width at baffle
  kJPbeam_xalph,       ///Beam x twiss alpha at baffle
  kJPbeam_yalph,       ///Beam y twiss alpha at baffle
  kJPbeam_xemit,       ///Beam x emittance 
  kJPbeam_yemit,       ///Beam y emittance

  // 2012a flux binning
  kJEnu2012a_nd5numu0,   
  kJEnu2012a_nd5numu1,   
  kJEnu2012a_nd5numu2,   
  kJEnu2012a_nd5numu3,   
  kJEnu2012a_nd5numu4,   
  kJEnu2012a_nd5numu5,   
  kJEnu2012a_nd5numu6,   
  kJEnu2012a_nd5numu7,   
  kJEnu2012a_nd5numu8,   
  kJEnu2012a_nd5numu9,   
  kJEnu2012a_nd5numu10,  
  kJEnu2012a_nd5numub0,  
  kJEnu2012a_nd5numub1,  
  kJEnu2012a_nd5numub2,  
  kJEnu2012a_nd5numub3,  
  kJEnu2012a_nd5numub4,  
  kJEnu2012a_nd5nue0,    
  kJEnu2012a_nd5nue1,    
  kJEnu2012a_nd5nue2,    
  kJEnu2012a_nd5nue3,    
  kJEnu2012a_nd5nue4,    
  kJEnu2012a_nd5nue5,    
  kJEnu2012a_nd5nue6,    
  kJEnu2012a_nd5nueb0,   
  kJEnu2012a_nd5nueb1,   
  kJEnu2012a_sknumu0,    
  kJEnu2012a_sknumu1,    
  kJEnu2012a_sknumu2,    
  kJEnu2012a_sknumu3,    
  kJEnu2012a_sknumu4,    
  kJEnu2012a_sknumu5,    
  kJEnu2012a_sknumu6,    
  kJEnu2012a_sknumu7,    
  kJEnu2012a_sknumu8,    
  kJEnu2012a_sknumu9,    
  kJEnu2012a_sknumu10,   
  kJEnu2012a_sknumub0,   
  kJEnu2012a_sknumub1,   
  kJEnu2012a_sknumub2,   
  kJEnu2012a_sknumub3,   
  kJEnu2012a_sknumub4,   
  kJEnu2012a_sknue0,     
  kJEnu2012a_sknue1,     
  kJEnu2012a_sknue2,     
  kJEnu2012a_sknue3,     
  kJEnu2012a_sknue4,     
  kJEnu2012a_sknue5,     
  kJEnu2012a_sknue6,     
  kJEnu2012a_sknueb0,    
  kJEnu2012a_sknueb1,    


  // 2013a flux binning
  kJEnu2013a_nd5numu0,   
  kJEnu2013a_nd5numu1,   
  kJEnu2013a_nd5numu2,   
  kJEnu2013a_nd5numu3,   
  kJEnu2013a_nd5numu4,   
  kJEnu2013a_nd5numu5,   
  kJEnu2013a_nd5numu6,   
  kJEnu2013a_nd5numu7,   
  kJEnu2013a_nd5numu8,   
  kJEnu2013a_nd5numu9,   
  kJEnu2013a_nd5numu10,
  kJEnu2013a_nd5numu11,   
  kJEnu2013a_nd5numu12,   
  kJEnu2013a_nd5numu13,   
  kJEnu2013a_nd5numu14,   
  kJEnu2013a_nd5numu15,   
  kJEnu2013a_nd5numu16,   
  kJEnu2013a_nd5numu17,   
  kJEnu2013a_nd5numu18,   
  kJEnu2013a_nd5numu19,   
  kJEnu2013a_nd5numu20,  
  kJEnu2013a_nd5numub0,   
  kJEnu2013a_nd5numub1,   
  kJEnu2013a_nd5numub2,   
  kJEnu2013a_nd5numub3,   
  kJEnu2013a_nd5numub4,   
  kJEnu2013a_nd5numub5,   
  kJEnu2013a_nd5numub6,   
  kJEnu2013a_nd5numub7,   
  kJEnu2013a_nd5numub8,   
  kJEnu2013a_nd5numub9,   
  kJEnu2013a_nd5numub10,
  kJEnu2013a_nd5numub11,   
  kJEnu2013a_nd5numub12,   
  kJEnu2013a_nd5numub13,   
  kJEnu2013a_nd5numub14,   
  kJEnu2013a_nd5numub15,   
  kJEnu2013a_nd5numub16,   
  kJEnu2013a_nd5numub17,   
  kJEnu2013a_nd5numub18,   
  kJEnu2013a_nd5numub19,   
  kJEnu2013a_nd5numub20,  
  kJEnu2013a_nd5nue0,   
  kJEnu2013a_nd5nue1,   
  kJEnu2013a_nd5nue2,   
  kJEnu2013a_nd5nue3,   
  kJEnu2013a_nd5nue4,   
  kJEnu2013a_nd5nue5,   
  kJEnu2013a_nd5nue6,   
  kJEnu2013a_nd5nue7,   
  kJEnu2013a_nd5nue8,   
  kJEnu2013a_nd5nue9,   
  kJEnu2013a_nd5nue10,
  kJEnu2013a_nd5nue11,   
  kJEnu2013a_nd5nue12,   
  kJEnu2013a_nd5nue13,   
  kJEnu2013a_nd5nue14,   
  kJEnu2013a_nd5nue15,   
  kJEnu2013a_nd5nue16,   
  kJEnu2013a_nd5nue17,   
  kJEnu2013a_nd5nue18,   
  kJEnu2013a_nd5nue19,   
  kJEnu2013a_nd5nue20,  
  kJEnu2013a_nd5nueb0,   
  kJEnu2013a_nd5nueb1,   
  kJEnu2013a_nd5nueb2,   
  kJEnu2013a_nd5nueb3,   
  kJEnu2013a_nd5nueb4,   
  kJEnu2013a_nd5nueb5,   
  kJEnu2013a_nd5nueb6,   
  kJEnu2013a_nd5nueb7,   
  kJEnu2013a_nd5nueb8,   
  kJEnu2013a_nd5nueb9,   
  kJEnu2013a_nd5nueb10,
  kJEnu2013a_nd5nueb11,   
  kJEnu2013a_nd5nueb12,   
  kJEnu2013a_nd5nueb13,   
  kJEnu2013a_nd5nueb14,   
  kJEnu2013a_nd5nueb15,   
  kJEnu2013a_nd5nueb16,   
  kJEnu2013a_nd5nueb17,   
  kJEnu2013a_nd5nueb18,   
  kJEnu2013a_nd5nueb19,   
  kJEnu2013a_nd5nueb20,  
  kJEnu2013a_sknumu0,   
  kJEnu2013a_sknumu1,   
  kJEnu2013a_sknumu2,   
  kJEnu2013a_sknumu3,   
  kJEnu2013a_sknumu4,   
  kJEnu2013a_sknumu5,   
  kJEnu2013a_sknumu6,   
  kJEnu2013a_sknumu7,   
  kJEnu2013a_sknumu8,   
  kJEnu2013a_sknumu9,   
  kJEnu2013a_sknumu10,
  kJEnu2013a_sknumu11,   
  kJEnu2013a_sknumu12,   
  kJEnu2013a_sknumu13,   
  kJEnu2013a_sknumu14,   
  kJEnu2013a_sknumu15,   
  kJEnu2013a_sknumu16,   
  kJEnu2013a_sknumu17,   
  kJEnu2013a_sknumu18,   
  kJEnu2013a_sknumu19,   
  kJEnu2013a_sknumu20,  
  kJEnu2013a_sknumub0,   
  kJEnu2013a_sknumub1,   
  kJEnu2013a_sknumub2,   
  kJEnu2013a_sknumub3,   
  kJEnu2013a_sknumub4,   
  kJEnu2013a_sknumub5,   
  kJEnu2013a_sknumub6,   
  kJEnu2013a_sknumub7,   
  kJEnu2013a_sknumub8,   
  kJEnu2013a_sknumub9,   
  kJEnu2013a_sknumub10,
  kJEnu2013a_sknumub11,   
  kJEnu2013a_sknumub12,   
  kJEnu2013a_sknumub13,   
  kJEnu2013a_sknumub14,   
  kJEnu2013a_sknumub15,   
  kJEnu2013a_sknumub16,   
  kJEnu2013a_sknumub17,   
  kJEnu2013a_sknumub18,   
  kJEnu2013a_sknumub19,   
  kJEnu2013a_sknumub20,  
  kJEnu2013a_sknue0,   
  kJEnu2013a_sknue1,   
  kJEnu2013a_sknue2,   
  kJEnu2013a_sknue3,   
  kJEnu2013a_sknue4,   
  kJEnu2013a_sknue5,   
  kJEnu2013a_sknue6,   
  kJEnu2013a_sknue7,   
  kJEnu2013a_sknue8,   
  kJEnu2013a_sknue9,   
  kJEnu2013a_sknue10,
  kJEnu2013a_sknue11,   
  kJEnu2013a_sknue12,   
  kJEnu2013a_sknue13,   
  kJEnu2013a_sknue14,   
  kJEnu2013a_sknue15,   
  kJEnu2013a_sknue16,   
  kJEnu2013a_sknue17,   
  kJEnu2013a_sknue18,   
  kJEnu2013a_sknue19,   
  kJEnu2013a_sknue20,  
  kJEnu2013a_sknueb0,   
  kJEnu2013a_sknueb1,   
  kJEnu2013a_sknueb2,   
  kJEnu2013a_sknueb3,   
  kJEnu2013a_sknueb4,   
  kJEnu2013a_sknueb5,   
  kJEnu2013a_sknueb6,   
  kJEnu2013a_sknueb7,   
  kJEnu2013a_sknueb8,   
  kJEnu2013a_sknueb9,   
  kJEnu2013a_sknueb10,
  kJEnu2013a_sknueb11,   
  kJEnu2013a_sknueb12,   
  kJEnu2013a_sknueb13,   
  kJEnu2013a_sknueb14,   
  kJEnu2013a_sknueb15,   
  kJEnu2013a_sknueb16,   
  kJEnu2013a_sknueb17,   
  kJEnu2013a_sknueb18,   
  kJEnu2013a_sknueb19,   
  kJEnu2013a_sknueb20,         


  //
  // ================== End of JNuBeam parameters ===================


  // ================== Start of NIWG parameters ===================
  //
  // NIWG systematic paramters available for reweighting. Defined 
  // in $NIWG/NIWGSyst.h 

  // 2010a parameters (See T2K-TN-032)
  kNIWG2010a_ccqenorm,   
  kNIWG2010a_ccqe,   
  kNIWG2010a_ccnue,   
  kNIWG2010a_cc1pi,   
  kNIWG2010a_cccoh,   
  kNIWG2010a_ccoth,   
  kNIWG2010a_nc,   
  kNIWG2010a_nc1pi0,   
  kNIWG2010a_nccoh,   
  kNIWG2010a_ncoth,   
  kNIWG2010a_fsi,  

  // 2011a parameters (under development and to be finalized)

  /*
    Dials below correspond to normalization as a function of Enu, linearly
    interpolated (NOT splined as named) between the following points:
	
    CCQE: 0, 0.3, 0.6, 0.9, 1.2, 3 GeV
    CCRES: 0, 0.6, 1.0, 1.5, 3.0, 5 GeV
    NCRES: 0, 0.6, 1.0, 2.0, 3.5, 5 GeV
	
    where each point corresponds to a tweak dial and all energies above the 
    maximum are given the same weight as at the maximum
	
  */
  kNIWG2011a_CCQESplineE0 ,
  kNIWG2011a_CCQESplineE1 ,
  kNIWG2011a_CCQESplineE2 ,
  kNIWG2011a_CCQESplineE3 ,
  kNIWG2011a_CCQESplineE4 ,
  kNIWG2011a_CCQESplineE5 ,

  kNIWG2011a_CCRESSplineE0,
  kNIWG2011a_CCRESSplineE1,
  kNIWG2011a_CCRESSplineE2,
  kNIWG2011a_CCRESSplineE3,
  kNIWG2011a_CCRESSplineE4,
  kNIWG2011a_CCRESSplineE5,

  kNIWG2011a_NCRESSplineE0,
  kNIWG2011a_NCRESSplineE1,
  kNIWG2011a_NCRESSplineE2,
  kNIWG2011a_NCRESSplineE3,
  kNIWG2011a_NCRESSplineE4,
  kNIWG2011a_NCRESSplineE5,
 
  //2012a normalization parameters
  kNIWG2012a_dismpishp,
  kNIWG2012a_ccqeE0,
  kNIWG2012a_ccqeE1,
  kNIWG2012a_ccqeE2,
  kNIWG2012a_cc1piE0,
  kNIWG2012a_cc1piE1,
  kNIWG2012a_nc1piE0,
  kNIWG2012a_nc1pi0E0,
  kNIWG2012a_1gamE0,
  kNIWG2012a_ccmultipiE0,
  kNIWG2012a_ccdisE0,
  kNIWG2012a_cccohE0,
  kNIWG2012a_nccohE0,
  kNIWG2012a_ncotherE0,
  kNIWG2012a_ccnueE0,
  kNIWG2012a_antinu,

  kNIWGDeltaMass_mean,
  kNIWGDeltaMass_width,

  // Spectral function with NuWro 2012a histograms
  kNIWG2012a_sf,

  // pF/Eb response with NEUT 5.1.4 histograms
  kNIWG2012a_eb,
  kNIWG2012a_pf,

  // MiniBooNE CC1pi Enu shape tuning
  kNIWG2012a_mbcc1pi_enushp,

  // MB Pion Outgoing angle tuning
  kNIWG2012a_1piAngle,

  // 2014 analysis dials (CW: 16/09/14)
  kNIWG2014a_SF_RFG,
  kNIWG2014a_pF_C12,
  kNIWG2014a_pF_O16,
  kNIWG2014a_pF_Al27,
  kNIWG2014a_pF_Fe56,
  kNIWG2014a_pF_Cu63,
  kNIWG2014a_pF_Zn64,
  kNIWG2014a_pF_Pb208,
  kNIWG2014a_Eb_C12,
  kNIWG2014a_Eb_O16,
  kNIWG2014a_Eb_Al27,
  kNIWG2014a_Eb_Fe56,
  kNIWG2014a_Eb_Cu63,
  kNIWG2014a_Eb_Zn64,
  kNIWG2014a_Eb_Pb208,

  // CHORUS DISMPI multiplicity tuning
  kNIWGPiMult_CorrSwitch,
  
  //SpectralFunction systematics
  kNIWGSpectralFunc_corrNormC12,
  kNIWGSpectralFunc_pFsfC12,
  kNIWGSpectralFunc_pFsfC12_smooth,
  kNIWGSpectralFunc_MFwidthC12,
  kNIWGSpectralFunc_corrNormO16,
  kNIWGSpectralFunc_pFsfO16,
  kNIWGSpectralFunc_pFsfO16_smooth,
  kNIWGSpectralFunc_MFwidthO16,
  kNIWGSpectralFunc_corrNormFe56,
  kNIWGSpectralFunc_pFsfFe56,
  kNIWGSpectralFunc_pFsfFe56_smooth,
  kNIWGSpectralFunc_MFwidthFe56,

  // MEC systematics
  kNIWGMEC_Norm_C12,
  kNIWGMEC_Norm_O16,
  kNIWGMEC_Norm_Other,
  kNIWGMEC_q3Cut,
  kNIWGMEC_Diff,

  kNIWGMEC_PDDInterp,
  kNIWGMEC_PDDWeight_C12,
  kNIWGMEC_PDDWeight_O16,
  
  // correction for the NEUT proton FSI bug (dependence on first target nucleus in each file)
  kNIWG_protonFSI_bugfix,

  //Hadron multiplicities for DIS-like NEUT modes
  kNIWGHadronMultSwitch,

  
  //
  // ================== End of NIWG parameters ===================


  // ================== Start of GEANT parameters ===================
  //
  // NIWG systematic paramters available for reweighting. Defined 
  // in $GEANTRW/GEANTSyst.h 
    
  kGEANT_PionNuclXSecABS,
  kGEANT_PionNuclXSecQEL,
  kGEANT_PionNuclXSecCXL,
  kGEANT_PionNuclXSecDCX,
//kGEANT_PionNuclXSecHAD,

  kGEANT_PionFreeXSecQEL,
  kGEANT_PionFreeXSecCXL,
//kGEANT_PionFreeXSecHAD,

  kGEANT_PionXSecTbl,

  //
  // ================== End of GEANT parameters ===================

  kNPlusOne  ///< A counter to make iterating over systematics easy. 

} T2KSyst_t;


class T2KSyst {
public:
  //......................................................................................
  static string AsString(T2KSyst_t syst) 
  {
    // a compact string code for the syst parameter
    // (can be used as part of other string codes, eg for assembling filenames, plot titles,...)
    //
    switch(syst) {
    case ( kSystNull ) : return "kSystNull"; break;
      // GENIE parameters
      // 
    case ( kGXSec_MaNCEL         ) : return "GXSec_MaNCEL";         break;
    case ( kGXSec_EtaNCEL        ) : return "GXSec_EtaNCEL";        break;
    case ( kGXSec_NormCCQE       ) : return "GXSec_NormCCQE";       break;
    case ( kGXSec_NormCCQEenu    ) : return "GXSec_NormCCQEenu";    break;
    case ( kGXSec_MaCCQEshape    ) : return "GXSec_MaCCQEshape";    break;
    case ( kGXSec_MaCCQE         ) : return "GXSec_MaCCQE";         break;
    case ( kGXSec_VecFFCCQEshape ) : return "GXSec_VecFFCCQEshape"; break;
    case ( kGXSec_NormCCRES      ) : return "GXSec_NormCCRES";      break;
    case ( kGXSec_MaCCRESshape   ) : return "GXSec_MaCCRESshape";   break;
    case ( kGXSec_MvCCRESshape   ) : return "GXSec_MvCCRESshape";   break;
    case ( kGXSec_MaCCRES        ) : return "GXSec_MaCCRES";        break;
    case ( kGXSec_MvCCRES        ) : return "GXSec_MvCCRES";        break;
    case ( kGXSec_NormNCRES      ) : return "GXSec_NormNCRES";      break;
    case ( kGXSec_MaNCRESshape   ) : return "GXSec_MaNCRESshape";   break;
    case ( kGXSec_MvNCRESshape   ) : return "GXSec_MvNCRESshape";   break;
    case ( kGXSec_MaNCRES        ) : return "GXSec_MaNCRES";        break;
    case ( kGXSec_MvNCRES        ) : return "GXSec_MvNCRES";        break;
    case ( kGXSec_MaCOHpi        ) : return "GXSec_MaCOHpi";        break;
    case ( kGXSec_R0COHpi        ) : return "GXSec_R0COHpi";        break;
    case ( kGXSec_RvpCC1pi       ) : return "GXSec_RvpCC1pi";       break;
    case ( kGXSec_RvpCC2pi       ) : return "GXSec_RvpCC2pi";       break;
    case ( kGXSec_RvpNC1pi       ) : return "GXSec_RvpNC1pi";       break;
    case ( kGXSec_RvpNC2pi       ) : return "GXSec_RvpNC2pi";       break;
    case ( kGXSec_RvnCC1pi       ) : return "GXSec_RvnCC1pi";       break;
    case ( kGXSec_RvnCC2pi       ) : return "GXSec_RvnCC2pi";       break;
    case ( kGXSec_RvnNC1pi       ) : return "GXSec_RvnNC1pi";       break;
    case ( kGXSec_RvnNC2pi       ) : return "GXSec_RvnNC2pi";       break;
    case ( kGXSec_RvbarpCC1pi    ) : return "GXSec_RvbarpCC1pi";    break;
    case ( kGXSec_RvbarpCC2pi    ) : return "GXSec_RvbarpCC2pi";    break;
    case ( kGXSec_RvbarpNC1pi    ) : return "GXSec_RvbarpNC1pi";    break;
    case ( kGXSec_RvbarpNC2pi    ) : return "GXSec_RvbarpNC2pi";    break;
    case ( kGXSec_RvbarnCC1pi    ) : return "GXSec_RvbarnCC1pi";    break;
    case ( kGXSec_RvbarnCC2pi    ) : return "GXSec_RvbarnCC2pi";    break;
    case ( kGXSec_RvbarnNC1pi    ) : return "GXSec_RvbarnNC1pi";    break;
    case ( kGXSec_RvbarnNC2pi    ) : return "GXSec_RvbarnNC2pi";    break;
    case ( kGXSec_AhtBY          ) : return "GXSec_AhtBY";          break;
    case ( kGXSec_BhtBY          ) : return "GXSec_BhtBY";          break;
    case ( kGXSec_CV1uBY         ) : return "GXSec_CV1uBY";         break;
    case ( kGXSec_CV2uBY         ) : return "GXSec_CV2uBY";         break;
    case ( kGXSec_AhtBYshape     ) : return "GXSec_AhtBYshape";     break;
    case ( kGXSec_BhtBYshape     ) : return "GXSec_BhtBYshape";     break;
    case ( kGXSec_CV1uBYshape    ) : return "GXSec_CV1uBYshape";    break;
    case ( kGXSec_CV2uBYshape    ) : return "GXSec_CV2uBYshape";    break;
    case ( kGXSec_NormDISCC      ) : return "GXSec_NormDISCC";      break;
    case ( kGXSec_RnubarnuCC     ) : return "GXSec_RnubarnuCC";     break;
    case ( kGXSec_DISNuclMod     ) : return "GXSec_DISNuclMod";     break;
    case ( kGXSec_NC             ) : return "GXSec_NC";             break;
    case ( kGHadrAGKY_xF1pi      ) : return "GHadrAGKY_xF1pi";      break;
    case ( kGHadrAGKY_pT1pi      ) : return "GHadrAGKY_pT1pi";      break;
    case ( kGHadrNucl_FormZone   ) : return "GHadrNucl_FormZone";   break;
    case ( kGINuke_MFP_pi      ) : return "GINuke_MFP_pi";      break;
    case ( kGINuke_MFP_N       ) : return "GINuke_MFP_N";       break;
    case ( kGINuke_FrCEx_pi    ) : return "GINuke_FrCEx_pi";    break;
    case ( kGINuke_FrElas_pi   ) : return "GINuke_FrElas_pi";   break;
    case ( kGINuke_FrInel_pi   ) : return "GINuke_FrInel_pi";   break;
    case ( kGINuke_FrAbs_pi    ) : return "GINuke_FrAbs_pi";    break;
    case ( kGINuke_FrPiProd_pi ) : return "GINuke_FrPiProd_pi"; break;
    case ( kGINuke_FrCEx_N     ) : return "GINuke_FrCEx_N";     break;
    case ( kGINuke_FrElas_N    ) : return "GINuke_FrElas_N";    break;
    case ( kGINuke_FrInel_N    ) : return "GINuke_FrInel_N";    break;
    case ( kGINuke_FrAbs_N     ) : return "GINuke_FrAbs_N";     break;
    case ( kGINuke_FrPiProd_N  ) : return "GINuke_FrPiProd_N";  break;
    case ( kGNucl_CCQEPauliSupViaKF   ) : return "GSystNucl_CCQEPauliSupViaKF";   break;
    case ( kGNucl_CCQEMomDistroFGtoSF ) : return "GSystNucl_CCQEMomDistroFGtoSF"; break;
    case ( kGRDcy_BR1gamma        ) : return "GRDcy_BR1gamma";        break;
    case ( kGRDcy_BR1eta          ) : return "GRDcy_BR1eta";          break;
    case ( kGRDcy_Theta_Delta2Npi ) : return "GRDcy_Theta_Delta2Npi"; break;

      // NEUT parameters
      // 

    case ( kNXSec_MaNCEL       ) : return "NXSec_MaNCEL";       break;
    case ( kNXSec_NormNCEL     ) : return "NXSec_NormNCEL";     break;
    case ( kNXSec_MaNCELshape  ) : return "NXSec_MaNCELshape";     break;
    case ( kNXSec_NormCCQE       ) : return "NXSec_NormCCQE";       break;
    case ( kNXSec_1overMaNCEL2    ) : return "NXSec_1overMaNCEL2";    break;
    case ( kNXSec_AxlFFNCEL ) : return "NXSec_AxlFFNCEL"; break;
    case ( kNXSec_VecFFNCEL ) : return "NXSec_VecFFNCEL"; break;
      //case ( kNXSec_NormCCQEenu    ) : return "NXSec_NormCCQEenu";    break;
    case ( kNXSec_MaCCQEshape    ) : return "NXSec_MaCCQEshape";    break;
    case ( kNXSec_1overMaCCQE2    ) : return "NXSec_1overMaCCQE2";    break;
    case ( kNXSec_MaCCQE         ) : return "NXSec_MaCCQE";         break;
    case ( kNXSec_AxlFFCCQE ) : return "NXSec_AxlFFCCQE"; break;	 
    case ( kNXSec_VecFFCCQE ) : return "NXSec_VecFFCCQE"; break;
    case ( kNXSec_VecFFCCQE_out ) : return "NXSec_VecFFCCQE_out"; break;
    case ( kNXSec_SCCVecQE ) : return "NXSec_SCCVecQE"; break;
    case ( kNXSec_SCCAxlQE ) : return "NXSec_SCCAxlQE"; break;
    case ( kNXSec_PsFF ) : return "NXSec_PsFF"; break;

    // P.S (26.01.17) AxialFF Patch        
    case ( kNXSec_AxlDipToAlt ) : return "NXSec_AxlDipToAlt"; break;

    case ( kNXSec_FAxlCCQEAlpha     ) : return "NXSec_FAAxlQEAlpha"; break;
    case ( kNXSec_FAxlCCQEGamma     ) : return "NXSec_FAAxlQEGamma"; break;
    case ( kNXSec_FAxlCCQEBeta      ) : return "NXSec_FAAxlQEBeta";  break;
    case ( kNXSec_FAxlCCQETheta     ) : return "NXSec_FAAxlQETheta"; break;

    case ( kNXSec_FAZExp_NTerms    ) : return "NXSec_FAZExp_NTerms"; break;
    case ( kNXSec_FAZExp_TCut      ) : return "NXSec_FAZExp_TCut";  break;
    case ( kNXSec_FAZExp_T0        ) : return "NXSec_FAZExp_T0";    break;
    case ( kNXSec_FAZExp_Q4Cut     ) : return "NXSec_FAZExp_Q4Cut"; break;
    case ( kNXSec_FAZExp_A0        ) : return "NXSec_FAZExp_A0";    break;
    case ( kNXSec_FAZExp_A1        ) : return "NXSec_FAZExp_A1";  break;
    case ( kNXSec_FAZExp_A2        ) : return "NXSec_FAZExp_A2";  break;
    case ( kNXSec_FAZExp_A3        ) : return "NXSec_FAZExp_A3";  break;
    case ( kNXSec_FAZExp_A4        ) : return "NXSec_FAZExp_A4";  break;
    case ( kNXSec_FAZExp_A5        ) : return "NXSec_FAZExp_A5";  break;
    case ( kNXSec_FAZExp_A6        ) : return "NXSec_FAZExp_A6";  break;
    case ( kNXSec_FAZExp_A7        ) : return "NXSec_FAZExp_A7";  break;
    case ( kNXSec_FAZExp_A8        ) : return "NXSec_FAZExp_A8";  break;
    case ( kNXSec_FAZExp_A9        ) : return "NXSec_FAZExp_A9";  break;

      //case ( kNXSec_VecFFCCQEshape ) : return "NXSec_VecFFCCQEshape"; break;
    case ( kNXSec_NormRES      ) : return "NXSec_NormRES";      break;
    case ( kNXSec_MaRESshape   ) : return "NXSec_MaRESshape";   break;
    case ( kNXSec_MaRES        ) : return "NXSec_MaRES";        break;
    case ( kNXSec_MvRES        ) : return "NXSec_MvRES";        break;

    case ( kNXSec_FFRES      ) : return "NXSec_FFRES";      break;
    case ( kNXSec_TypeRES    ) : return "NXSec_TypeRES";    break;
    case ( kNXSec_CA5RES     ) : return "NXSec_CA5RES";     break;
    case ( kNXSec_BgSclRES   ) : return "NXSec_BgSclRES";   break;
    case ( kNXSec_MaNFFRES   ) : return "NXSec_MaNFFRES";   break;
    case ( kNXSec_MvNFFRES   ) : return "NXSec_MvNFFRES";   break;
    case ( kNXSec_MaRSRES    ) : return "NXSec_MaRSRES";    break;
    case ( kNXSec_MvRSRES    ) : return "NXSec_MvRSRES";    break;

    case ( kNXSec_NormCCRES      ) : return "NXSec_NormCCRES";      break;
    case ( kNXSec_MaCCRESshape   ) : return "NXSec_MaCCRESshape";   break;
      //case ( kNXSec_MvCCRESshape   ) : return "NXSec_MvCCRESshape";   break;
    case ( kNXSec_MaCCRES        ) : return "NXSec_MaCCRES";        break;
    case ( kNXSec_MvCCRES        ) : return "NXSec_MvCCRES";        break;

    case ( kNXSec_FFCCRES      ) : return "NXSec_FFCCRES";      break;
    case ( kNXSec_TypeCCRES    ) : return "NXSec_TypeCCRES";    break;
    case ( kNXSec_CA5CCRES     ) : return "NXSec_CA5CCRES";     break;
    case ( kNXSec_BgSclCCRES   ) : return "NXSec_BgSclCCRES";   break;
    case ( kNXSec_MaNFFCCRES   ) : return "NXSec_MaNFFCCRES";   break;
    case ( kNXSec_MvNFFCCRES   ) : return "NXSec_MvNFFCCRES";   break;
    case ( kNXSec_MaRSCCRES    ) : return "NXSec_MaRSCCRES";    break;
    case ( kNXSec_MvRSCCRES    ) : return "NXSec_MvRSCCRES";    break;

    case ( kNXSec_NormNCRES      ) : return "NXSec_NormNCRES";      break;
    case ( kNXSec_MaNCRESshape   ) : return "NXSec_MaNCRESshape";   break;
      //case ( kNXSec_MvNCRESshape   ) : return "NXSec_MvNCRESshape";   break;
    case ( kNXSec_MaNCRES        ) : return "NXSec_MaNCRES";        break;
    case ( kNXSec_MvNCRES        ) : return "NXSec_MvNCRES";        break;

    case ( kNXSec_FFNCRES      ) : return "NXSec_FFNCRES";      break;
    case ( kNXSec_TypeNCRES    ) : return "NXSec_TypeNCRES";    break;
    case ( kNXSec_CA5NCRES     ) : return "NXSec_CA5NCRES";     break;
    case ( kNXSec_BgSclNCRES   ) : return "NXSec_BgSclNCRES";   break;
    case ( kNXSec_MaNFFNCRES   ) : return "NXSec_MaNFFNCRES";   break;
    case ( kNXSec_MvNFFNCRES   ) : return "NXSec_MvNFFNCRES";   break;
    case ( kNXSec_MaRSNCRES    ) : return "NXSec_MaRSNCRES";    break;
    case ( kNXSec_MvRSNCRES    ) : return "NXSec_MvRSNCRES";    break;
    
    case ( kNXSec_NECOHEPI       ) : return "NXSec_NECOHEPI";        break;
    case ( kNXSec_MaCOHpi        ) : return "NXSec_MaCOHpi";        break;
    case ( kNXSec_R0COHpi        ) : return "NXSec_R0COHpi";        break;
    case ( kNXSec_fA1COHpi        ) : return "NXSec_fA1COHpi";        break;
    case ( kNXSec_fb1COHpi        ) : return "NXSec_fb1COHpi";        break;

    case ( kNXSec_NormDIS          ) : return "NXSec_NormDIS";        break;
    case ( kNXSec_BYOnOffDIS          ) : return "NXSec_BYOnOffDIS";        break;
      //case ( kNXSec_RnubarnuCC     ) : return "NXSec_RnubarnuCC";     break;
      //case ( kNXSec_DISNuclMod     ) : return "NXSec_DISNuclMod";     break;
    case ( kNXSec_NC             ) : return "NXSec_NC";             break;
      //case ( kNHadrAGKY_xF1pi      ) : return "NHadrAGKY_xF1pi";      break;
      //case ( kNHadrAGKY_pT1pi      ) : return "NHadrAGKY_pT1pi";      break;
      //case ( kNHadrNucl_FormZone   ) : return "NHadrNucl_FormZone";   break;

    case ( kNCasc_FrAbs_pi    ) : return "NCasc_FrAbs_pi";    break;
    case ( kNCasc_FrInelLow_pi   ) : return "NCasc_FrInelLow_pi";   break;
    case ( kNCasc_FrCExLow_pi    ) : return "NCasc_FrCExLow_pi";    break;
    case ( kNCasc_FrInelHigh_pi   ) : return "NCasc_FrInelHigh_pi";   break;
    case ( kNCasc_FrCExHigh_pi    ) : return "NCasc_FrCExHigh_pi";    break;
    case ( kNCasc_FrPiProd_pi ) : return "NCasc_FrPiProd_pi"; break;

    case ( kNINuke_MFP_N       ) : return "NINuke_MFP_N";       break;
    //case ( kNINuke_FrCEx_N     ) : return "NINuke_FrCEx_N";     break;
    //case ( kNINuke_FrElas_N    ) : return "NINuke_FrElas_N";    break;
    //case ( kNINuke_FrInel_N    ) : return "NINuke_FrInel_N";    break;
    //case ( kNINuke_FrAbs_N     ) : return "NINuke_FrAbs_N";     break;
    //case ( kNINuke_FrPiProd_N  ) : return "NINuke_FrPiProd_N";  break;

    case ( kNNucl_CCQEPauliSupViaKF   ) : return "NSystNucl_CCQEPauliSupViaKF";   break;
    case ( kNNucl_CCQEFermiSurfMom   ) : return "NSystNucl_CCQEFermiSurfMom";   break;	 
    case ( kNNucl_CCQEBindingEnergy   ) : return "NSystNucl_CCQEBindingEnergy";   break;
      //case ( kNNucl_CCQEMomDistroFGtoSF ) : return "NSystNucl_CCQEMomDistroFGtoSF"; break;
      //case ( kNRDcy_BR1gamma        ) : return "NRDcy_BR1gamma";        break;
      //case ( kNRDcy_BR1eta          ) : return "NRDcy_BR1eta";          break;
      //case ( kNRDcy_Theta_Delta2Npi ) : return "NRDcy_Theta_Delta2Npi"; break;
    case ( kNNucl_PilessDcyRES   ) : return "NSystNucl_PilessDcyRES";   break;
    case ( kNNucl_PilessDcyOnlyRES   ) : return "NSystNucl_PilessDcyOnlyRES";   break;
    case ( kNNucl_CCQEBindingEnergy_C12   ) : return "NSystNucl_CCQEBindingEnergy_C12";   break;
    case ( kNNucl_CCQEBindingEnergy_O16   ) : return "NSystNucl_CCQEBindingEnergy_O16";   break;
    case ( kNNucl_CCQEBindingEnergy_Al27  ) : return "NSystNucl_CCQEBindingEnergy_Al27";  break;
    case ( kNNucl_CCQEBindingEnergy_Fe56  ) : return "NSystNucl_CCQEBindingEnergy_Fe56";  break;
    case ( kNNucl_CCQEBindingEnergy_Cu63  ) : return "NSystNucl_CCQEBindingEnergy_Cu63";  break;
    case ( kNNucl_CCQEBindingEnergy_Zn64  ) : return "NSystNucl_CCQEBindingEnergy_Zn64";  break;
    case ( kNNucl_CCQEBindingEnergy_Pb208 ) : return "NSystNucl_CCQEBindingEnergy_Pb208"; break;

      // JNuBeam parameters
      //
    case ( kJNA61pi_dedx      ) : return "JNA61pi_dedx"; break;       
    case ( kJNA61pi_dedxtof  ) : return "JNA61pi_dedxtof"; break;       
    case ( kJNA61pi_feeddown ) : return "JNA61pi_feeddown"; break; 
    case ( kJNA61pi_trktop   ) : return "JNA61pi_trktop"; break;    
    case ( kJNA61pi_trkcut   ) : return "JNA61pi_trkcut"; break;    
    case ( kJNA61pi_trkmrg   ) : return "JNA61pi_trkmrg"; break;    
    case ( kJNA61pi_hcont    ) : return "JNA61pi_hcont"; break;     
    case ( kJNA61pi_receff   ) : return "JNA61pi_receff"; break;    
    case ( kJNA61pi_tofeff   ) : return "JNA61pi_tofeff"; break;    
    case ( kJNA61pi_piloss   ) : return "JNA61pi_piloss"; break;    
    case ( kJNA61pi_norm   ) : return "JNA61pi_norm"; break;    
    case ( kJkaon_bmptA      ) : return "Jkaon_bmptA"; break;       
    case ( kJkaon_bmptB      ) : return "Jkaon_bmptB"; break;       
    case ( kJkaon_bmptalpha  ) : return "Jkaon_bmptalpha"; break;   
    case ( kJkaon_bmptbeta   ) : return "Jkaon_bmptbeta"; break;    
    case ( kJkaon_bmpta1     ) : return "Jkaon_bmpta1"; break;      
    case ( kJkaon_bmptgamma  ) : return "Jkaon_bmptgamma"; break;   
    case ( kJkaon_bmptdelta  ) : return "Jkaon_bmptdelta"; break;   
    case ( kJTXsec_pLow      ) : return "JTXsec_pLow"; break;       
    case ( kJTXsec_ppiMid    ) : return "JTXsec_ppiMid"; break;     
    case ( kJTXsec_pHigh     ) : return "JTXsec_pHigh"; break;      
    case ( kJTXsec_piLow     ) : return "JTXsec_piLow"; break;      
    case ( kJTXsec_kLow      ) : return "JTXsec_kLow"; break;       
    case ( kJTXsec_pikHigh   ) : return "JTXsec_pikHigh"; break;
    case ( kJTXsec_kHigh     ) : return "JTXsec_kHigh"; break;      
    case ( kJPbeam_xpos      ) : return "JPbeam_xpos"; break;      
    case ( kJPbeam_ypos      ) : return "JPbeam_ypos"; break;      
    case ( kJPbeam_xdir      ) : return "JPbeam_xdir"; break;      
    case ( kJPbeam_ydir      ) : return "JPbeam_ydir"; break;      
    case ( kJPbeam_xsig      ) : return "JPbeam_xsig"; break;      
    case ( kJPbeam_ysig      ) : return "JPbeam_ysig"; break;      
    case ( kJPbeam_xalph     ) : return "JPbeam_xalph"; break;      
    case ( kJPbeam_yalph     ) : return "JPbeam_yalph"; break;      
    case ( kJPbeam_xemit     ) : return "JPbeam_xemit"; break;      
    case ( kJPbeam_yemit     ) : return "JPbeam_yemit"; break;      

    case ( kJEnu2012a_nd5numu0         ) : return "JEnu2012a_nd5numu0";         break;
    case ( kJEnu2012a_nd5numu1         ) : return "JEnu2012a_nd5numu1";         break;
    case ( kJEnu2012a_nd5numu2         ) : return "JEnu2012a_nd5numu2";         break;
    case ( kJEnu2012a_nd5numu3         ) : return "JEnu2012a_nd5numu3";         break;
    case ( kJEnu2012a_nd5numu4         ) : return "JEnu2012a_nd5numu4";         break;
    case ( kJEnu2012a_nd5numu5         ) : return "JEnu2012a_nd5numu5";         break;
    case ( kJEnu2012a_nd5numu6         ) : return "JEnu2012a_nd5numu6";         break;
    case ( kJEnu2012a_nd5numu7         ) : return "JEnu2012a_nd5numu7";         break;
    case ( kJEnu2012a_nd5numu8         ) : return "JEnu2012a_nd5numu8";         break;
    case ( kJEnu2012a_nd5numu9         ) : return "JEnu2012a_nd5numu9";         break;
    case ( kJEnu2012a_nd5numu10        ) : return "JEnu2012a_nd5numu10";        break;
    case ( kJEnu2012a_nd5numub0        ) : return "JEnu2012a_nd5numub0";        break;
    case ( kJEnu2012a_nd5numub1        ) : return "JEnu2012a_nd5numub1";        break;
    case ( kJEnu2012a_nd5numub2        ) : return "JEnu2012a_nd5numub2";        break;
    case ( kJEnu2012a_nd5numub3        ) : return "JEnu2012a_nd5numub3";        break;
    case ( kJEnu2012a_nd5numub4        ) : return "JEnu2012a_nd5numub4";        break;
    case ( kJEnu2012a_nd5nue0          ) : return "JEnu2012a_nd5nue0";          break;
    case ( kJEnu2012a_nd5nue1          ) : return "JEnu2012a_nd5nue1";          break;
    case ( kJEnu2012a_nd5nue2          ) : return "JEnu2012a_nd5nue2";          break;
    case ( kJEnu2012a_nd5nue3          ) : return "JEnu2012a_nd5nue3";          break;
    case ( kJEnu2012a_nd5nue4          ) : return "JEnu2012a_nd5nue4";          break;
    case ( kJEnu2012a_nd5nue5          ) : return "JEnu2012a_nd5nue5";          break;
    case ( kJEnu2012a_nd5nue6          ) : return "JEnu2012a_nd5nue6";          break;
    case ( kJEnu2012a_nd5nueb0         ) : return "JEnu2012a_nd5nueb0";         break;
    case ( kJEnu2012a_nd5nueb1         ) : return "JEnu2012a_nd5nueb1";         break;
    case ( kJEnu2012a_sknumu0          ) : return "JEnu2012a_sknumu0";         break;
    case ( kJEnu2012a_sknumu1          ) : return "JEnu2012a_sknumu1";         break;
    case ( kJEnu2012a_sknumu2          ) : return "JEnu2012a_sknumu2";         break;
    case ( kJEnu2012a_sknumu3          ) : return "JEnu2012a_sknumu3";         break;
    case ( kJEnu2012a_sknumu4          ) : return "JEnu2012a_sknumu4";         break;
    case ( kJEnu2012a_sknumu5          ) : return "JEnu2012a_sknumu5";         break;
    case ( kJEnu2012a_sknumu6          ) : return "JEnu2012a_sknumu6";         break;
    case ( kJEnu2012a_sknumu7          ) : return "JEnu2012a_sknumu7";         break;
    case ( kJEnu2012a_sknumu8          ) : return "JEnu2012a_sknumu8";         break;
    case ( kJEnu2012a_sknumu9          ) : return "JEnu2012a_sknumu9";         break;
    case ( kJEnu2012a_sknumu10         ) : return "JEnu2012a_sknumu10";        break;
    case ( kJEnu2012a_sknumub0         ) : return "JEnu2012a_sknumub0";        break;
    case ( kJEnu2012a_sknumub1         ) : return "JEnu2012a_sknumub1";        break;
    case ( kJEnu2012a_sknumub2         ) : return "JEnu2012a_sknumub2";        break;
    case ( kJEnu2012a_sknumub3         ) : return "JEnu2012a_sknumub3";        break;
    case ( kJEnu2012a_sknumub4         ) : return "JEnu2012a_sknumub4";        break;
    case ( kJEnu2012a_sknue0           ) : return "JEnu2012a_sknue0";          break;
    case ( kJEnu2012a_sknue1           ) : return "JEnu2012a_sknue1";          break;
    case ( kJEnu2012a_sknue2           ) : return "JEnu2012a_sknue2";          break;
    case ( kJEnu2012a_sknue3           ) : return "JEnu2012a_sknue3";          break;
    case ( kJEnu2012a_sknue4           ) : return "JEnu2012a_sknue4";          break;
    case ( kJEnu2012a_sknue5           ) : return "JEnu2012a_sknue5";          break;
    case ( kJEnu2012a_sknue6           ) : return "JEnu2012a_sknue6";          break;
    case ( kJEnu2012a_sknueb0          ) : return "JEnu2012a_sknueb0";         break;
    case ( kJEnu2012a_sknueb1          ) : return "JEnu2012a_sknueb1";         break;

    case ( kJEnu2013a_nd5numu0          ) : return "JEnu2013a_nd5numu0";      break;          
    case ( kJEnu2013a_nd5numu1          ) : return "JEnu2013a_nd5numu1";      break;          
    case ( kJEnu2013a_nd5numu2          ) : return "JEnu2013a_nd5numu2";      break;          
    case ( kJEnu2013a_nd5numu3          ) : return "JEnu2013a_nd5numu3";      break;          
    case ( kJEnu2013a_nd5numu4          ) : return "JEnu2013a_nd5numu4";      break;          
    case ( kJEnu2013a_nd5numu5          ) : return "JEnu2013a_nd5numu5";      break;          
    case ( kJEnu2013a_nd5numu6          ) : return "JEnu2013a_nd5numu6";      break;          
    case ( kJEnu2013a_nd5numu7          ) : return "JEnu2013a_nd5numu7";      break;          
    case ( kJEnu2013a_nd5numu8          ) : return "JEnu2013a_nd5numu8";      break;          
    case ( kJEnu2013a_nd5numu9          ) : return "JEnu2013a_nd5numu9";      break;          
    case ( kJEnu2013a_nd5numu10         ) : return "JEnu2013a_nd5numu10";      break;	     
    case ( kJEnu2013a_nd5numu11         ) : return "JEnu2013a_nd5numu11";      break;         
    case ( kJEnu2013a_nd5numu12         ) : return "JEnu2013a_nd5numu12";      break;         
    case ( kJEnu2013a_nd5numu13         ) : return "JEnu2013a_nd5numu13";      break;         
    case ( kJEnu2013a_nd5numu14         ) : return "JEnu2013a_nd5numu14";      break;         
    case ( kJEnu2013a_nd5numu15         ) : return "JEnu2013a_nd5numu15";      break;         
    case ( kJEnu2013a_nd5numu16         ) : return "JEnu2013a_nd5numu16";      break;         
    case ( kJEnu2013a_nd5numu17         ) : return "JEnu2013a_nd5numu17";      break;         
    case ( kJEnu2013a_nd5numu18         ) : return "JEnu2013a_nd5numu18";      break;         
    case ( kJEnu2013a_nd5numu19         ) : return "JEnu2013a_nd5numu19";      break;         
    case ( kJEnu2013a_nd5numu20         ) : return "JEnu2013a_nd5numu20";      break;        
    case ( kJEnu2013a_nd5numub0         ) : return "JEnu2013a_nd5numub0";      break;         
    case ( kJEnu2013a_nd5numub1         ) : return "JEnu2013a_nd5numub1";      break;         
    case ( kJEnu2013a_nd5numub2         ) : return "JEnu2013a_nd5numub2";      break;         
    case ( kJEnu2013a_nd5numub3         ) : return "JEnu2013a_nd5numub3";      break;         
    case ( kJEnu2013a_nd5numub4         ) : return "JEnu2013a_nd5numub4";      break;         
    case ( kJEnu2013a_nd5numub5         ) : return "JEnu2013a_nd5numub5";      break;         
    case ( kJEnu2013a_nd5numub6         ) : return "JEnu2013a_nd5numub6";      break;         
    case ( kJEnu2013a_nd5numub7         ) : return "JEnu2013a_nd5numub7";      break;         
    case ( kJEnu2013a_nd5numub8         ) : return "JEnu2013a_nd5numub8";      break;         
    case ( kJEnu2013a_nd5numub9         ) : return "JEnu2013a_nd5numub9";      break;         
    case ( kJEnu2013a_nd5numub10        ) : return "JEnu2013a_nd5numub10";      break;     
    case ( kJEnu2013a_nd5numub11        ) : return "JEnu2013a_nd5numub11";      break;        
    case ( kJEnu2013a_nd5numub12        ) : return "JEnu2013a_nd5numub12";      break;        
    case ( kJEnu2013a_nd5numub13        ) : return "JEnu2013a_nd5numub13";      break;        
    case ( kJEnu2013a_nd5numub14        ) : return "JEnu2013a_nd5numub14";      break;        
    case ( kJEnu2013a_nd5numub15        ) : return "JEnu2013a_nd5numub15";      break;        
    case ( kJEnu2013a_nd5numub16        ) : return "JEnu2013a_nd5numub16";      break;        
    case ( kJEnu2013a_nd5numub17        ) : return "JEnu2013a_nd5numub17";      break;        
    case ( kJEnu2013a_nd5numub18        ) : return "JEnu2013a_nd5numub18";      break;        
    case ( kJEnu2013a_nd5numub19        ) : return "JEnu2013a_nd5numub19";      break;        
    case ( kJEnu2013a_nd5numub20        ) : return "JEnu2013a_nd5numub20";      break;       
    case ( kJEnu2013a_nd5nue0           ) : return "JEnu2013a_nd5nue0";      break;           
    case ( kJEnu2013a_nd5nue1           ) : return "JEnu2013a_nd5nue1";      break;           
    case ( kJEnu2013a_nd5nue2           ) : return "JEnu2013a_nd5nue2";      break;           
    case ( kJEnu2013a_nd5nue3           ) : return "JEnu2013a_nd5nue3";      break;           
    case ( kJEnu2013a_nd5nue4           ) : return "JEnu2013a_nd5nue4";      break;           
    case ( kJEnu2013a_nd5nue5           ) : return "JEnu2013a_nd5nue5";      break;           
    case ( kJEnu2013a_nd5nue6           ) : return "JEnu2013a_nd5nue6";      break;           
    case ( kJEnu2013a_nd5nue7           ) : return "JEnu2013a_nd5nue7";      break;           
    case ( kJEnu2013a_nd5nue8           ) : return "JEnu2013a_nd5nue8";      break;           
    case ( kJEnu2013a_nd5nue9           ) : return "JEnu2013a_nd5nue9";      break;           
    case ( kJEnu2013a_nd5nue10          ) : return "JEnu2013a_nd5nue10";      break;	     
    case ( kJEnu2013a_nd5nue11          ) : return "JEnu2013a_nd5nue11";      break;          
    case ( kJEnu2013a_nd5nue12          ) : return "JEnu2013a_nd5nue12";      break;          
    case ( kJEnu2013a_nd5nue13          ) : return "JEnu2013a_nd5nue13";      break;          
    case ( kJEnu2013a_nd5nue14          ) : return "JEnu2013a_nd5nue14";      break;          
    case ( kJEnu2013a_nd5nue15          ) : return "JEnu2013a_nd5nue15";      break;          
    case ( kJEnu2013a_nd5nue16          ) : return "JEnu2013a_nd5nue16";      break;          
    case ( kJEnu2013a_nd5nue17          ) : return "JEnu2013a_nd5nue17";      break;          
    case ( kJEnu2013a_nd5nue18          ) : return "JEnu2013a_nd5nue18";      break;          
    case ( kJEnu2013a_nd5nue19          ) : return "JEnu2013a_nd5nue19";      break;          
    case ( kJEnu2013a_nd5nue20          ) : return "JEnu2013a_nd5nue20";      break;         
    case ( kJEnu2013a_nd5nueb0          ) : return "JEnu2013a_nd5nueb0";      break;          
    case ( kJEnu2013a_nd5nueb1          ) : return "JEnu2013a_nd5nueb1";      break;          
    case ( kJEnu2013a_nd5nueb2          ) : return "JEnu2013a_nd5nueb2";      break;          
    case ( kJEnu2013a_nd5nueb3          ) : return "JEnu2013a_nd5nueb3";      break;          
    case ( kJEnu2013a_nd5nueb4          ) : return "JEnu2013a_nd5nueb4";      break;          
    case ( kJEnu2013a_nd5nueb5          ) : return "JEnu2013a_nd5nueb5";      break;          
    case ( kJEnu2013a_nd5nueb6          ) : return "JEnu2013a_nd5nueb6";      break;          
    case ( kJEnu2013a_nd5nueb7          ) : return "JEnu2013a_nd5nueb7";      break;          
    case ( kJEnu2013a_nd5nueb8          ) : return "JEnu2013a_nd5nueb8";      break;          
    case ( kJEnu2013a_nd5nueb9          ) : return "JEnu2013a_nd5nueb9";      break;          
    case ( kJEnu2013a_nd5nueb10         ) : return "JEnu2013a_nd5nueb10";      break;	     
    case ( kJEnu2013a_nd5nueb11         ) : return "JEnu2013a_nd5nueb11";      break;         
    case ( kJEnu2013a_nd5nueb12         ) : return "JEnu2013a_nd5nueb12";      break;         
    case ( kJEnu2013a_nd5nueb13         ) : return "JEnu2013a_nd5nueb13";      break;         
    case ( kJEnu2013a_nd5nueb14         ) : return "JEnu2013a_nd5nueb14";      break;         
    case ( kJEnu2013a_nd5nueb15         ) : return "JEnu2013a_nd5nueb15";      break;         
    case ( kJEnu2013a_nd5nueb16         ) : return "JEnu2013a_nd5nueb16";      break;         
    case ( kJEnu2013a_nd5nueb17         ) : return "JEnu2013a_nd5nueb17";      break;         
    case ( kJEnu2013a_nd5nueb18         ) : return "JEnu2013a_nd5nueb18";      break;         
    case ( kJEnu2013a_nd5nueb19         ) : return "JEnu2013a_nd5nueb19";      break;         
    case ( kJEnu2013a_nd5nueb20         ) : return "JEnu2013a_nd5nueb20";      break;        
    case ( kJEnu2013a_sknumu0           ) : return "JEnu2013a_sknumu0";      break;           
    case ( kJEnu2013a_sknumu1           ) : return "JEnu2013a_sknumu1";      break;           
    case ( kJEnu2013a_sknumu2           ) : return "JEnu2013a_sknumu2";      break;           
    case ( kJEnu2013a_sknumu3           ) : return "JEnu2013a_sknumu3";      break;           
    case ( kJEnu2013a_sknumu4           ) : return "JEnu2013a_sknumu4";      break;           
    case ( kJEnu2013a_sknumu5           ) : return "JEnu2013a_sknumu5";      break;           
    case ( kJEnu2013a_sknumu6           ) : return "JEnu2013a_sknumu6";      break;           
    case ( kJEnu2013a_sknumu7           ) : return "JEnu2013a_sknumu7";      break;           
    case ( kJEnu2013a_sknumu8           ) : return "JEnu2013a_sknumu8";      break;           
    case ( kJEnu2013a_sknumu9           ) : return "JEnu2013a_sknumu9";      break;           
    case ( kJEnu2013a_sknumu10          ) : return "JEnu2013a_sknumu10";      break;	     
    case ( kJEnu2013a_sknumu11          ) : return "JEnu2013a_sknumu11";      break;          
    case ( kJEnu2013a_sknumu12          ) : return "JEnu2013a_sknumu12";      break;          
    case ( kJEnu2013a_sknumu13          ) : return "JEnu2013a_sknumu13";      break;          
    case ( kJEnu2013a_sknumu14          ) : return "JEnu2013a_sknumu14";      break;          
    case ( kJEnu2013a_sknumu15          ) : return "JEnu2013a_sknumu15";      break;          
    case ( kJEnu2013a_sknumu16          ) : return "JEnu2013a_sknumu16";      break;          
    case ( kJEnu2013a_sknumu17          ) : return "JEnu2013a_sknumu17";      break;          
    case ( kJEnu2013a_sknumu18          ) : return "JEnu2013a_sknumu18";      break;          
    case ( kJEnu2013a_sknumu19          ) : return "JEnu2013a_sknumu19";      break;          
    case ( kJEnu2013a_sknumu20          ) : return "JEnu2013a_sknumu20";      break;         
    case ( kJEnu2013a_sknumub0          ) : return "JEnu2013a_sknumub0";      break;          
    case ( kJEnu2013a_sknumub1          ) : return "JEnu2013a_sknumub1";      break;          
    case ( kJEnu2013a_sknumub2          ) : return "JEnu2013a_sknumub2";      break;          
    case ( kJEnu2013a_sknumub3          ) : return "JEnu2013a_sknumub3";      break;          
    case ( kJEnu2013a_sknumub4          ) : return "JEnu2013a_sknumub4";      break;          
    case ( kJEnu2013a_sknumub5          ) : return "JEnu2013a_sknumub5";      break;          
    case ( kJEnu2013a_sknumub6          ) : return "JEnu2013a_sknumub6";      break;          
    case ( kJEnu2013a_sknumub7          ) : return "JEnu2013a_sknumub7";      break;          
    case ( kJEnu2013a_sknumub8          ) : return "JEnu2013a_sknumub8";      break;          
    case ( kJEnu2013a_sknumub9          ) : return "JEnu2013a_sknumub9";      break;          
    case ( kJEnu2013a_sknumub10         ) : return "JEnu2013a_sknumub10";      break;	     
    case ( kJEnu2013a_sknumub11         ) : return "JEnu2013a_sknumub11";      break;         
    case ( kJEnu2013a_sknumub12         ) : return "JEnu2013a_sknumub12";      break;         
    case ( kJEnu2013a_sknumub13         ) : return "JEnu2013a_sknumub13";      break;         
    case ( kJEnu2013a_sknumub14         ) : return "JEnu2013a_sknumub14";      break;         
    case ( kJEnu2013a_sknumub15         ) : return "JEnu2013a_sknumub15";      break;         
    case ( kJEnu2013a_sknumub16         ) : return "JEnu2013a_sknumub16";      break;         
    case ( kJEnu2013a_sknumub17         ) : return "JEnu2013a_sknumub17";      break;         
    case ( kJEnu2013a_sknumub18         ) : return "JEnu2013a_sknumub18";      break;         
    case ( kJEnu2013a_sknumub19         ) : return "JEnu2013a_sknumub19";      break;         
    case ( kJEnu2013a_sknumub20         ) : return "JEnu2013a_sknumub20";      break;        
    case ( kJEnu2013a_sknue0            ) : return "JEnu2013a_sknue0";      break;   	        
    case ( kJEnu2013a_sknue1            ) : return "JEnu2013a_sknue1";      break;   	        
    case ( kJEnu2013a_sknue2            ) : return "JEnu2013a_sknue2";      break;   	        
    case ( kJEnu2013a_sknue3            ) : return "JEnu2013a_sknue3";      break;   	        
    case ( kJEnu2013a_sknue4            ) : return "JEnu2013a_sknue4";      break;   	        
    case ( kJEnu2013a_sknue5            ) : return "JEnu2013a_sknue5";      break;   	        
    case ( kJEnu2013a_sknue6            ) : return "JEnu2013a_sknue6";      break;   	        
    case ( kJEnu2013a_sknue7            ) : return "JEnu2013a_sknue7";      break;   	        
    case ( kJEnu2013a_sknue8            ) : return "JEnu2013a_sknue8";      break;   	        
    case ( kJEnu2013a_sknue9            ) : return "JEnu2013a_sknue9";      break;   	        
    case ( kJEnu2013a_sknue10           ) : return "JEnu2013a_sknue10";      break;	     
    case ( kJEnu2013a_sknue11           ) : return "JEnu2013a_sknue11";      break;           
    case ( kJEnu2013a_sknue12           ) : return "JEnu2013a_sknue12";      break;           
    case ( kJEnu2013a_sknue13           ) : return "JEnu2013a_sknue13";      break;           
    case ( kJEnu2013a_sknue14           ) : return "JEnu2013a_sknue14";      break;           
    case ( kJEnu2013a_sknue15           ) : return "JEnu2013a_sknue15";      break;           
    case ( kJEnu2013a_sknue16           ) : return "JEnu2013a_sknue16";      break;           
    case ( kJEnu2013a_sknue17           ) : return "JEnu2013a_sknue17";      break;           
    case ( kJEnu2013a_sknue18           ) : return "JEnu2013a_sknue18";      break;           
    case ( kJEnu2013a_sknue19           ) : return "JEnu2013a_sknue19";      break;           
    case ( kJEnu2013a_sknue20           ) : return "JEnu2013a_sknue20";      break;  	       
    case ( kJEnu2013a_sknueb0           ) : return "JEnu2013a_sknueb0";      break;           
    case ( kJEnu2013a_sknueb1           ) : return "JEnu2013a_sknueb1";      break;           
    case ( kJEnu2013a_sknueb2           ) : return "JEnu2013a_sknueb2";      break;           
    case ( kJEnu2013a_sknueb3           ) : return "JEnu2013a_sknueb3";      break;           
    case ( kJEnu2013a_sknueb4           ) : return "JEnu2013a_sknueb4";      break;           
    case ( kJEnu2013a_sknueb5           ) : return "JEnu2013a_sknueb5";      break;           
    case ( kJEnu2013a_sknueb6           ) : return "JEnu2013a_sknueb6";      break;           
    case ( kJEnu2013a_sknueb7           ) : return "JEnu2013a_sknueb7";      break;           
    case ( kJEnu2013a_sknueb8           ) : return "JEnu2013a_sknueb8";      break;           
    case ( kJEnu2013a_sknueb9           ) : return "JEnu2013a_sknueb9";      break;           
    case ( kJEnu2013a_sknueb10          ) : return "JEnu2013a_sknueb10";      break;	     
    case ( kJEnu2013a_sknueb11          ) : return "JEnu2013a_sknueb11";      break;          
    case ( kJEnu2013a_sknueb12          ) : return "JEnu2013a_sknueb12";      break;          
    case ( kJEnu2013a_sknueb13          ) : return "JEnu2013a_sknueb13";      break;          
    case ( kJEnu2013a_sknueb14          ) : return "JEnu2013a_sknueb14";      break;          
    case ( kJEnu2013a_sknueb15          ) : return "JEnu2013a_sknueb15";      break;          
    case ( kJEnu2013a_sknueb16          ) : return "JEnu2013a_sknueb16";      break;          
    case ( kJEnu2013a_sknueb17          ) : return "JEnu2013a_sknueb17";      break;          
    case ( kJEnu2013a_sknueb18          ) : return "JEnu2013a_sknueb18";      break;          
    case ( kJEnu2013a_sknueb19          ) : return "JEnu2013a_sknueb19";      break;          
    case ( kJEnu2013a_sknueb20          ) : return "JEnu2013a_sknueb20";      break;        

      // NIWG parameters

    //NIWG2010a
    case ( kNIWG2010a_ccqenorm       ) : return "NIWG2010a_ccqenorm"; break;       
    case ( kNIWG2010a_ccqe       ) : return "NIWG2010a_ccqe"; break;       
    case ( kNIWG2010a_ccnue       ) : return "NIWG2010a_ccnue"; break;       
    case ( kNIWG2010a_cc1pi      ) : return "NIWG2010a_cc1pi"; break;       
    case ( kNIWG2010a_cccoh      ) : return "NIWG2010a_cccoh"; break;       
    case ( kNIWG2010a_ccoth      ) : return "NIWG2010a_ccoth"; break;       
    case ( kNIWG2010a_nc         ) : return "NIWG2010a_nc"; break;       
    case ( kNIWG2010a_nc1pi0     ) : return "NIWG2010a_nc1pi0"; break;       
    case ( kNIWG2010a_nccoh      ) : return "NIWG2010a_nccoh"; break;       
    case ( kNIWG2010a_ncoth      ) : return "NIWG2010a_ncoth"; break;       
    case ( kNIWG2010a_fsi        ) : return "NIWG2010a_fsi"; break;       

    //NIWG2011a
    case ( kNIWG2011a_CCQESplineE0  ) : return "NIWG2011a_CCQESplineE0" ; break;
    case ( kNIWG2011a_CCQESplineE1  ) : return "NIWG2011a_CCQESplineE1" ; break;
    case ( kNIWG2011a_CCQESplineE2  ) : return "NIWG2011a_CCQESplineE2" ; break;
    case ( kNIWG2011a_CCQESplineE3  ) : return "NIWG2011a_CCQESplineE3" ; break;
    case ( kNIWG2011a_CCQESplineE4  ) : return "NIWG2011a_CCQESplineE4" ; break;
    case ( kNIWG2011a_CCQESplineE5  ) : return "NIWG2011a_CCQESplineE5" ; break;

    case ( kNIWG2011a_CCRESSplineE0 ) : return "NIWG2011a_CCRESSplineE0" ; break;
    case ( kNIWG2011a_CCRESSplineE1 ) : return "NIWG2011a_CCRESSplineE1" ; break;
    case ( kNIWG2011a_CCRESSplineE2 ) : return "NIWG2011a_CCRESSplineE2" ; break;
    case ( kNIWG2011a_CCRESSplineE3 ) : return "NIWG2011a_CCRESSplineE3" ; break;
    case ( kNIWG2011a_CCRESSplineE4 ) : return "NIWG2011a_CCRESSplineE4" ; break;
    case ( kNIWG2011a_CCRESSplineE5 ) : return "NIWG2011a_CCRESSplineE5" ; break;

    case ( kNIWG2011a_NCRESSplineE0 ) : return "NIWG2011a_NCRESSplineE0" ; break;
    case ( kNIWG2011a_NCRESSplineE1 ) : return "NIWG2011a_NCRESSplineE1" ; break;
    case ( kNIWG2011a_NCRESSplineE2 ) : return "NIWG2011a_NCRESSplineE2" ; break;
    case ( kNIWG2011a_NCRESSplineE3 ) : return "NIWG2011a_NCRESSplineE3" ; break;
    case ( kNIWG2011a_NCRESSplineE4 ) : return "NIWG2011a_NCRESSplineE4" ; break;
    case ( kNIWG2011a_NCRESSplineE5 ) : return "NIWG2011a_NCRESSplineE5" ; break;

    //NIWG2012a
    case ( kNIWG2012a_sf  ) : return "NIWG2012a_sf";    break;
    case ( kNIWG2012a_eb  ) : return "NIWG2012a_eb";    break;
    case ( kNIWG2012a_pf  ) : return "NIWG2012a_pf";    break;
    case ( kNIWG2012a_dismpishp  ) : return "NIWG2012a_dismpishp";    break;
    case ( kNIWG2012a_ccqeE0  ) :    return "NIWG2012a_ccqeE0";          break;
    case ( kNIWG2012a_ccqeE1  ) :    return "NIWG2012a_ccqeE1";          break;
    case ( kNIWG2012a_ccqeE2  ) :    return "NIWG2012a_ccqeE2";          break;
    case ( kNIWG2012a_cc1piE0  ) :   return "NIWG2012a_cc1piE0";        break;
    case ( kNIWG2012a_cc1piE1  ) :   return "NIWG2012a_cc1piE1";        break;
    case ( kNIWG2012a_nc1piE0  ) :   return "NIWG2012a_nc1piE0";        break;
    case ( kNIWG2012a_nc1pi0E0  ) :   return "NIWG2012a_nc1pi0E0";        break;
    case ( kNIWG2012a_1gamE0   ) :   return "NIWG2012a_1gamE0";        break;
    case ( kNIWG2012a_ccmultipiE0  ) :   return "NIWG2012a_ccmultipiE0";        break;
    case ( kNIWG2012a_ccdisE0  ) :   return "NIWG2012a_ccdisE0";        break;
    case ( kNIWG2012a_cccohE0  ) :   return "NIWG2012a_cccohE0";        break;
    case ( kNIWG2012a_nccohE0  ) :   return "NIWG2012a_nccohE0";        break;
    case ( kNIWG2012a_ncotherE0  ) :   return "NIWG2012a_ncotherE0";        break;
    case ( kNIWG2012a_ccnueE0  ) :   return "NIWG2012a_ccnueE0";        break;
    case ( kNIWG2012a_antinu  ) :   return "NIWG2012a_antinu";        break;
      
    case ( kNIWGDeltaMass_mean  ) :   return "NIWGDeltaMass_mean";        break;
    case ( kNIWGDeltaMass_width ) :   return "NIWGDeltaMass_width";        break;
    
    case ( kNIWG2012a_1piAngle  ) :   return "NIWG2012a_1piAngle";     break;

    case ( kNIWG2014a_SF_RFG    ) :   return "NIWG2014a_SF_RFG";    break;
    case ( kNIWG2014a_pF_C12    ) :   return "NIWG2014a_pF_C12";    break;
    case ( kNIWG2014a_pF_O16    ) :   return "NIWG2014a_pF_O16";    break;
    case ( kNIWG2014a_pF_Al27   ) :   return "NIWG2014a_pF_Al27";   break;
    case ( kNIWG2014a_pF_Fe56   ) :   return "NIWG2014a_pF_Fe56";   break;
    case ( kNIWG2014a_pF_Cu63   ) :   return "NIWG2014a_pF_Cu63";   break;
    case ( kNIWG2014a_pF_Zn64   ) :   return "NIWG2014a_pF_Zn64";   break;
    case ( kNIWG2014a_pF_Pb208  ) :   return "NIWG2014a_pF_Pb208";  break;
    case ( kNIWG2014a_Eb_C12    ) :   return "NIWG2014a_Eb_C12";    break;
    case ( kNIWG2014a_Eb_O16    ) :   return "NIWG2014a_Eb_O16";    break;
    case ( kNIWG2014a_Eb_Al27   ) :   return "NIWG2014a_Eb_Al27";   break;
    case ( kNIWG2014a_Eb_Fe56   ) :   return "NIWG2014a_Eb_Fe56";   break;
    case ( kNIWG2014a_Eb_Cu63   ) :   return "NIWG2014a_Eb_Cu63";   break;
    case ( kNIWG2014a_Eb_Zn64   ) :   return "NIWG2014a_Eb_Zn64";   break;
    case ( kNIWG2014a_Eb_Pb208  ) :   return "NIWG2014a_Eb_Pb208";  break;

    case ( kNIWGPiMult_CorrSwitch ) :   return "NIWGPiMult_CorrSwitch";     break;

    case ( kNIWG2012a_mbcc1pi_enushp  ) : return "NIWG2012a_mbcc1pi_enushp";    break;

    case ( kNIWGSpectralFunc_corrNormC12) : return "NIWGSpectralFunc_corrNormC12";    break;
    case ( kNIWGSpectralFunc_pFsfC12) : return "NIWGSpectralFunc_pFsfC12";    break;
    case ( kNIWGSpectralFunc_pFsfC12_smooth) : return "NIWGSpectralFunc_pFsfC12_smooth";    break;
    case ( kNIWGSpectralFunc_MFwidthC12) : return "NIWGSpectralFunc_MFwidthC12";    break;
      
    case ( kNIWGSpectralFunc_corrNormO16) : return "NIWGSpectralFunc_corrNormO16";    break;
    case ( kNIWGSpectralFunc_pFsfO16) : return "NIWGSpectralFunc_pFsfO16";    break;
    case ( kNIWGSpectralFunc_pFsfO16_smooth) : return "NIWGSpectralFunc_pFsfO16_smooth";    break;
    case ( kNIWGSpectralFunc_MFwidthO16) : return "NIWGSpectralFunc_MFwidthO16";    break;
      
    case ( kNIWGSpectralFunc_corrNormFe56) : return "NIWGSpectralFunc_corrNormFe56";    break;
    case ( kNIWGSpectralFunc_pFsfFe56) : return "NIWGSpectralFunc_pFsfFe56";    break;
    case ( kNIWGSpectralFunc_pFsfFe56_smooth) : return "NIWGSpectralFunc_pFsfFe56_smooth";    break;
    case ( kNIWGSpectralFunc_MFwidthFe56) : return "NIWGSpectralFunc_MFwidthFe56";    break;

    case ( kNIWGMEC_Norm_C12   ) : return "NIWGMEC_Norm_C12"; break;
    case ( kNIWGMEC_Norm_O16   ) : return "NIWGMEC_Norm_O16"; break;
    case ( kNIWGMEC_Norm_Other ) : return "NIWGMEC_Norm_Other"; break;
    case ( kNIWGMEC_q3Cut      ) : return "NIWGMEC_q3Cut"; break;
    case ( kNIWGMEC_Diff       ) : return "NIWGMEC_Diff"; break;

    case ( kNIWGMEC_PDDInterp     ) : return "NIWGMEC_PDDInterp"; break;
    case ( kNIWGMEC_PDDWeight_C12 ) : return "NIWGMEC_PDDWeight_C12"; break;
    case ( kNIWGMEC_PDDWeight_O16 ) : return "NIWGMEC_PDDWeight_O16"; break;
      
    case ( kNIWG_rpaCCQE_norm ) : return "NIWG_rpaCCQE_norm"; break;
    case ( kNIWG_rpaCCQE_shape ) : return "NIWG_rpaCCQE_shape"; break;
    case ( kNIWG_rpaCCQE_norm_C12 ) : return "NIWG_rpaCCQE_norm_C12"; break;
    case ( kNIWG_rpaCCQE_shape_C12 ) : return "NIWG_rpaCCQE_shape_C12"; break;
    case ( kNIWG_rpaCCQE_norm_O16 ) : return "NIWG_rpaCCQE_norm_O16"; break;
    case ( kNIWG_rpaCCQE_shape_O16 ) : return "NIWG_rpaCCQE_shape_O16"; break;

    case ( kNIWG_Effective_rpaCCQE_Norm ) : return "NIWG_Effective_rpaCCQE_Norm"; break;
    case ( kNIWG_Effective_rpaCCQE_U ) : return "NIWG_Effective_rpaCCQE_U"; break;
    case ( kNIWG_Effective_rpaCCQE_A ) : return "NIWG_Effective_rpaCCQE_A"; break;
    case ( kNIWG_Effective_rpaCCQE_B ) : return "NIWG_Effective_rpaCCQE_B"; break;
    case ( kNIWG_Effective_rpaCCQE_C ) : return "NIWG_Effective_rpaCCQE_C"; break;
    case ( kNIWG_Effective_rpaCCQE_D ) : return "NIWG_Effective_rpaCCQE_D"; break;
    case ( kNIWG_Effective_rpaCCQE_Unom ) : return "NIWG_Effective_rpaCCQE_Unom"; break;
    case ( kNIWG_Effective_rpaCCQE_Anom ) : return "NIWG_Effective_rpaCCQE_Anom"; break;
    case ( kNIWG_Effective_rpaCCQE_Bnom ) : return "NIWG_Effective_rpaCCQE_Bnom"; break;
    case ( kNIWG_Effective_rpaCCQE_Cnom ) : return "NIWG_Effective_rpaCCQE_Cnom"; break;
    case ( kNIWG_Effective_rpaCCQE_Dnom ) : return "NIWG_Effective_rpaCCQE_Dnom"; break;

    case ( kNIWG_Effective_rpaCCQE_Norm_forweight ) : return "NIWG_Effective_rpaCCQE_Norm_forweight"; break;
    case ( kNIWG_Effective_rpaCCQE_U_forweight ) : return "NIWG_Effective_rpaCCQE_U_forweight"; break;
    case ( kNIWG_Effective_rpaCCQE_A_forweight ) : return "NIWG_Effective_rpaCCQE_A_forweight"; break;
    case ( kNIWG_Effective_rpaCCQE_B_forweight ) : return "NIWG_Effective_rpaCCQE_B_forweight"; break;
    case ( kNIWG_Effective_rpaCCQE_C_forweight ) : return "NIWG_Effective_rpaCCQE_C_forweight"; break;
    case ( kNIWG_Effective_rpaCCQE_D_forweight ) : return "NIWG_Effective_rpaCCQE_D_forweight"; break;

 
    case ( kNIWG_protonFSI_bugfix ) : return "NIWG_protonFSI_bugfix"; break;

    case ( kGEANT_PionNuclXSecABS ) : return "GEANT_PionNuclXSecABS";  break;
    case ( kGEANT_PionNuclXSecQEL ) : return "GEANT_PionNuclXSecQEL";  break;
    case ( kGEANT_PionNuclXSecCXL ) : return "GEANT_PionNuclXSecCXL";  break;
    case ( kGEANT_PionNuclXSecDCX ) : return "GEANT_PionNuclXSecDCX";  break;
  //case ( kGEANT_PionNuclXSecHAD ) : return "GEANT_PionNuclXSecHAD";  break;
					      				 
    case ( kGEANT_PionFreeXSecQEL ) : return "GEANT_PionFreeXSecQEL";  break;
    case ( kGEANT_PionFreeXSecCXL ) : return "GEANT_PionFreeXSecCXL";  break;
  //case ( kGEANT_PionFreeXSecHAD ) : return "GEANT_PionFreeXSecHAD";  break;
    
   
    case ( kGEANT_PionXSecTbl ) : return "GEANT_PionXSecTbl";  break;
	
    default: return "-";
    }
    return "";
  }
  //......................................................................................
  static T2KSyst_t FromString(string syst)
  {
    for(int isyst = 0; isyst<kNPlusOne; isyst++){
      if( T2KSyst::AsString(ET2KSyst(isyst)) == syst ) return ET2KSyst(isyst);
    }
    return kSystNull;
  }
  //......................................................................................
}; // T2KSyst class

} // t2krew namespace

#endif 

