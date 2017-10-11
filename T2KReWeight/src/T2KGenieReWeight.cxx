//____________________________________________________________________________
/*

 For the class documentation see the corresponding header file.

 Authors:
   Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
   STFC, Rutherford Appleton Laboratory

   Jim Dobson <J.Dobson07 \at imperial.ac.uk>
   Imperial College London

 @ Apr 11, 2011 - JD
   Delete genie::EventRecord ptr in CalcWeight(ND::RooTrackerVtxBase*) 

*/
//____________________________________________________________________________
#include <iostream>
#include <string>

#include <TMath.h>
 
#include "T2KBuild.h"
#include "T2KGenieReWeight.h"
#include "T2KSystSet.h"
#include "T2KGenieUtils.h" 
#ifdef __T2KRW_GENIE_ENABLED__
#include "ReWeight/GSystUncertainty.h"
using namespace genie::rew;
#endif

using namespace t2krew;
using std::cout;
using std::cerr;
using std::endl;

//_______________________________________________________________________________________
T2KGenieReWeight::T2KGenieReWeight() : 
  fAtLeastOneAbsTwkDial(false) 
{
#ifdef __T2KRW_GENIE_ENABLED__
  fGenieReWeight = new GReWeight(); 

  // Adopt all weight calculators.
  fGenieReWeight->AdoptWghtCalc( "xsec_ncel",       new GReWeightNuXSecNCEL      );
  fGenieReWeight->AdoptWghtCalc( "xsec_ccqe",       new GReWeightNuXSecCCQE      );
  fGenieReWeight->AdoptWghtCalc( "xsec_coh",        new GReWeightNuXSecCOH       );
  fGenieReWeight->AdoptWghtCalc( "xsec_nonresbkg",  new GReWeightNonResonanceBkg );
  fGenieReWeight->AdoptWghtCalc( "nuclear_dis",     new GReWeightDISNuclMod      );
  fGenieReWeight->AdoptWghtCalc( "hadro_res_decay", new GReWeightResonanceDecay  );
  fGenieReWeight->AdoptWghtCalc( "hadro_fzone",     new GReWeightFZone           );
  fGenieReWeight->AdoptWghtCalc( "hadro_intranuke", new GReWeightINuke           );
  fGenieReWeight->AdoptWghtCalc( "hadro_agky",      new GReWeightAGKY            );
  fGenieReWeight->AdoptWghtCalc( "xsec_ccqe_vec",   new GReWeightNuXSecCCQEvec   );
  fGenieReWeight->AdoptWghtCalc( "xsec_dis",        new GReWeightNuXSecDIS       );
  fGenieReWeight->AdoptWghtCalc( "xsec_nc",         new GReWeightNuXSecNC        );
  fGenieReWeight->AdoptWghtCalc( "xsec_ccres",      new GReWeightNuXSecCCRES     );
  fGenieReWeight->AdoptWghtCalc( "xsec_ncres",      new GReWeightNuXSecNCRES     );
  fGenieReWeight->AdoptWghtCalc( "nuclear_qe",      new GReWeightFGM             );
  
  GReWeightNuXSecCCQE * rwccqe =
    dynamic_cast<GReWeightNuXSecCCQE *> (fGenieReWeight->WghtCalc("xsec_ccqe"));
  rwccqe->SetMode(GReWeightNuXSecCCQE::kModeMa);
  
  // Default to include shape and normalization changes for CCRES (can be changed downstream if desired)
  GReWeightNuXSecCCRES * rwccres =
    dynamic_cast<GReWeightNuXSecCCRES *> (fGenieReWeight->WghtCalc("xsec_ccres"));
  rwccres->SetMode(GReWeightNuXSecCCRES::kModeMaMv);

  // Default to include shape and normalization changes for NCRES (can be changed downstream if desired)
  GReWeightNuXSecNCRES * rwncres =
    dynamic_cast<GReWeightNuXSecNCRES *> (fGenieReWeight->WghtCalc("xsec_ncres"));
  rwncres->SetMode(GReWeightNuXSecNCRES::kModeMaMv);

  // Default to include shape and normalization changes for DIS (can be changed downstream if desired) 
  GReWeightNuXSecDIS * rwdis =
    dynamic_cast<GReWeightNuXSecDIS *> (fGenieReWeight->WghtCalc("xsec_dis"));
  rwdis->SetMode(GReWeightNuXSecDIS::kModeABCV12u);

  

#else
   std::cerr << "WARNING!! Trying to use the GENIE reweighting class without "<<
                "configuring T2KReWeight with the --enable-genie option." << std::endl;
#endif
}
//_______________________________________________________________________________________
T2KGenieReWeight::~T2KGenieReWeight()
{
#ifdef __T2KRW_GENIE_ENABLED__
   if(fGenieReWeight) delete fGenieReWeight;
#endif
}
//_______________________________________________________________________________________
void T2KGenieReWeight::SetSystTwkDial(T2KSyst_t syst, double val)
{
#ifdef __T2KRW_GENIE_ENABLED__
  GSyst_t gsyst = T2KGenieUtils::T2KtoGSyst(syst);
  //if(gsyst == kNullSystematic){ 
  //  cout << "Cannot convert to genie syst, exiting!" << endl;
  //  exit(1);
  //}
  bool can_handle = fGenieReWeight->Systematics().Added(gsyst); 
  if(!can_handle){ 
    fGenieReWeight->Systematics().Init(gsyst);
    can_handle = fGenieReWeight->Systematics().Added(gsyst);
  }
  if(can_handle) fGenieReWeight->Systematics().Set(T2KGenieUtils::T2KtoGSyst(syst), val); 
#endif
}
//_______________________________________________________________________________________
void T2KGenieReWeight::SetSystFracErr(T2KSyst_t syst, double val, bool absolute)
{
#ifdef __T2KRW_GENIE_ENABLED__
// JIMTODO - Make sure set this to true if any of the systs are set to abs tweak 
// fAtLeastOneAbsTwkDial
  GSyst_t gsyst = T2KGenieUtils::T2KtoGSyst(syst);
  bool can_handle = fGenieReWeight->Systematics().Added(gsyst); 
  if(can_handle){
    GSystUncertainty::Instance()->SetUncertainty(gsyst, val, val);
  }
#endif
  return;
}
//_______________________________________________________________________________________
#ifdef __T2KRW_GENIE_ENABLED__
genie::rew::GReWeightI * T2KGenieReWeight::GetWghtCalc(string name)
{
  return fGenieReWeight->WghtCalc(name);
}
#endif
//_______________________________________________________________________________________
#ifdef __T2KRW_GENIE_ENABLED__
genie::rew::GReWeightNuXSecCCQE * T2KGenieReWeight::GetWghtCCQECalc(string name)
{
  return (genie::rew::GReWeightNuXSecCCQE*) fGenieReWeight->WghtCalc("xsec_ccqe");
}
#endif
//_______________________________________________________________________________________
void T2KGenieReWeight::Reconfigure(void)
{
#ifdef __T2KRW_GENIE_ENABLED__
  fGenieReWeight->Reconfigure();
#endif
}
//_______________________________________________________________________________________
void T2KGenieReWeight::Reset(void)
{
}
//_______________________________________________________________________________________
#ifdef __T2KRW_OAANALYSIS_ENABLED__
double T2KGenieReWeight::CalcWeight(const ND::RooTrackerVtxBase * vtx)
{
// This method needs to cast to a GRooTrackerVtx from a RooTrackerVtxBase pointer. If we are
// actually given a RooTrackerVtxBase pointing to a NRooTrackerVtx then the method needs 
// to complain and return a weight of 1.0. After casting to the GRooTrackerVtx object we 
// then convert to the native genie event format genie::EventRecord and give this to the
// GENIE weight calculator. 
  double weight = 1.0;
#ifdef __T2KRW_GENIE_ENABLED__
  const ND::GRooTrackerVtx * grootrac_vtx = dynamic_cast<const ND::GRooTrackerVtx*>(vtx);

  if(grootrac_vtx == NULL){
    cout << "Cannot cast from ND::RooTrackerVtxBase to ND::GRooTrackerVtx object!" << endl;
    return 1.0;
  }

  genie::EventRecord * event = (genie::EventRecord *) T2KGenieUtils::GetEventRecord(grootrac_vtx);
  if(event){
    weight = fGenieReWeight->CalcWeight(*event);
    delete event; event = NULL;
  }
  else {
    cout << "Unable to convert ND::GRooTrackerVtx to genie::EventRecord. Returning weight of 1.0!" << endl;
  }

  if(event) delete event;

#endif
  return weight;
}
#endif
//_______________________________________________________________________________________
double T2KGenieReWeight::CalcWeight(const SK::SK__h1 * sktree)
{
// This method needs to cast to a GRooTrackerVtx from an SK__h1 pointer. 
  double weight = 1.0;
#ifdef __T2KRW_GENIE_ENABLED__

  // GENIETODO - Conversion from SK tree to GHep record
  cout << "Warning: Cannot reweight GENIE SK event: " << sktree->nev << ", mode = " << sktree->mode << ", Enu = " << sktree->pnu[0] << " GeV" << endl;

#endif
  return weight;
}
//_______________________________________________________________________________________
double T2KGenieReWeight::CalcWeight(TTree * intree, int entry) 
{
// Call GENIE reweighting function and reweight using entry in intree-> 
// Either convert to some native GENIE format or, if the tree already holds native genie
// event records, use the information from the input tree directly.
  double weight = 1.0;
#ifdef __T2KRW_GENIE_ENABLED__
  genie::EventRecord * event = T2KGenieUtils::GetEventRecord(intree, entry);
  if(event){
    weight = fGenieReWeight->CalcWeight(*event);
    delete event; // delete as we have ownership
  }
  else {
    cout << "Unable to convert from tree to genie EventRecord!" << endl;
  }
#endif
  return weight;
}
#ifdef __T2KRW_NEUT_ENABLED__
double T2KGenieReWeight::CalcWeight    (NeutVect *nvect)
{
  return 1.0;
}
#endif
//_______________________________________________________________________________________
double T2KGenieReWeight::CalcWeight(const JnuBeamRead * jnutree)
{
  double weight = 1.0;

  return weight;
}
//_______________________________________________________________________________________
double T2KGenieReWeight::CalcChisq()
{
  double chisq = 0.0;
#ifdef __T2KRW_GENIE_ENABLED__
  if(fAtLeastOneAbsTwkDial){
    cerr << "Warning: T2KGenieReWeight::CalcChisq is meaningless if using "<<
            " absolute tweak dial for any GENIE params, returning 0!" << endl;
  }
  else {
    chisq = fGenieReWeight->CalcChisq();
  }
#endif
  return chisq;
}
//_______________________________________________________________________________________
void T2KGenieReWeight::Print(void)
{
#ifdef __T2KRW_GENIE_ENABLED__
   fGenieReWeight->Print();
#endif
}
//_______________________________________________________________________________________
