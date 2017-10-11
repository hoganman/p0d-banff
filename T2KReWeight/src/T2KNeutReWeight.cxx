//____________________________________________________________________________
/*

 For the class documentation see the corresponding header file.

*/
//____________________________________________________________________________
#include <cstdlib>
#include <iostream>

#include "T2KBuild.h"
#include "T2KNeutReWeight.h"
#include "T2KSystSet.h"
#include "T2KNeutUtils.h" 

using namespace t2krew;
#ifdef __T2KRW_NEUT_ENABLED__
#include "NSystUncertainty.h"
#include "NReWeightBindingEnergy.h"
using namespace neut::rew;
#endif
using std::cout;
using std::endl;
using std::cerr;

//_______________________________________________________________________________________
T2KNeutReWeight::T2KNeutReWeight() : 
  fAtLeastOneAbsTwkDial(false) 
{
#ifdef __T2KRW_NEUT_ENABLED__
  
  fNeutReWeight = new NReWeight(); 
  // Adopt all weight calculators.
  fNeutReWeight->AdoptWghtCalc( "nucl_eb_C12",    new NReWeightBindingEnergy(neut::rew::binding_energy::CarbonTarget  ((double) 25., (double) 9.)) );
  fNeutReWeight->AdoptWghtCalc( "nucl_eb_O16",    new NReWeightBindingEnergy(neut::rew::binding_energy::OxygenTarget  ((double) 27., (double) 9.)) );
  fNeutReWeight->AdoptWghtCalc( "nucl_eb_Al27",   new NReWeightBindingEnergy(neut::rew::binding_energy::AluminumTarget((double) 28., (double) 9.)) );
  fNeutReWeight->AdoptWghtCalc( "nucl_eb_Fe56",   new NReWeightBindingEnergy(neut::rew::binding_energy::IronTarget    ((double) 33., (double) 9.)) );
  fNeutReWeight->AdoptWghtCalc( "nucl_eb_Cu63",   new NReWeightBindingEnergy(neut::rew::binding_energy::CopperTarget  ((double) 35., (double) 9.)) );
  fNeutReWeight->AdoptWghtCalc( "nucl_eb_Zn64",   new NReWeightBindingEnergy(neut::rew::binding_energy::ZincTarget    ((double) 35., (double) 9.)) );
  fNeutReWeight->AdoptWghtCalc( "nucl_eb_Pb208",  new NReWeightBindingEnergy(neut::rew::binding_energy::LeadTarget    ((double) 44., (double) 9.)) );

  fNeutReWeight->AdoptWghtCalc( "xsec_ccqe",      new NReWeightNuXSecCCQE    );
  fNeutReWeight->AdoptWghtCalc( "xsec_res",       new NReWeightNuXSecRES     );
  fNeutReWeight->AdoptWghtCalc( "xsec_ccres",     new NReWeightNuXSecCCRES   );
  fNeutReWeight->AdoptWghtCalc( "xsec_coh",       new NReWeightNuXSecCOH     );
  fNeutReWeight->AdoptWghtCalc( "xsec_dis",       new NReWeightNuXSecDIS     );
  fNeutReWeight->AdoptWghtCalc( "xsec_ncel",      new NReWeightNuXSecNCEL    );
  fNeutReWeight->AdoptWghtCalc( "xsec_nc",        new NReWeightNuXSecNC      );
  fNeutReWeight->AdoptWghtCalc( "xsec_ncres",     new NReWeightNuXSecNCRES   );
  fNeutReWeight->AdoptWghtCalc( "nucl_casc",      new NReWeightCasc          );
  //Used for Proton FSI pass through not present in current NEUT version
  //fNeutReWeight->AdoptWghtCalc( "nucl_nuclcasc",  new NReWeightINuke       );
  fNeutReWeight->AdoptWghtCalc( "nucl_piless",    new NReWeightNuclPiless    );

  // Following is done by default in NReWeight
  //NReWeightNuXSecCCQE * rwccqe =
  //  dynamic_cast<NReWeightNuXSecCCQE *> (fNeutReWeight->WghtCalc("xsec_ccqe"));
  //rwccqe->SetMode(NReWeightNuXSecCCQE::kModeMa);
  
#else
   std::cerr << "WARNING!! Trying to use the NEUT reweighting class without "<<
                "configuring T2KReWeight with the --enable-neut option." << std::endl;
#endif
}
//_______________________________________________________________________________________
T2KNeutReWeight::~T2KNeutReWeight()
{
#ifdef __T2KRW_NEUT_ENABLED__
  if(fNeutReWeight) delete fNeutReWeight;
#endif
}
//_______________________________________________________________________________________
void T2KNeutReWeight::SetSystTwkDial(T2KSyst_t syst, double val)
{
#ifdef __T2KRW_NEUT_ENABLED__
  // JIMTODO - Add functionality to complain or exit if try to add non existent nsyst etc...

   NSyst_t nsyst = T2KNeutUtils::T2KtoNSyst(syst);
   //if(nsyst == kNullSystematic){ 
   //cout << "Cannot convert to neut syst, exiting!" << endl;
   //exit(1);
   //}

   bool can_handle = fNeutReWeight->Systematics().Added(nsyst); 
  
   if(!can_handle){ 
     fNeutReWeight->Systematics().Init(nsyst);
     can_handle = fNeutReWeight->Systematics().Added(nsyst);
   }

   if(can_handle) fNeutReWeight->Systematics().Set(T2KNeutUtils::T2KtoNSyst(syst), val); 
#endif
}
//_______________________________________________________________________________________
#ifdef __T2KRW_NEUT_ENABLED__
neut::rew::NReWeightI * T2KNeutReWeight::GetWghtCalc(string name)
{
  return fNeutReWeight->WghtCalc(name);
}
double T2KNeutReWeight::GetWeightXsec()
{
  return fNeutReWeight->GetWeightXsec();
}
#endif
//_______________________________________________________________________________________
//#ifdef __T2KRW_NEUT_ENABLED__
//neut::rew::NReWeightNuXSecCCQE * T2KNeutReWeight::GetWghtCCQECalc(string name)
//{
//  return (neut::rew::NReWeightNuXSecCCQE*) fNeutReWeight->WghtCalc("xsec_ccqe");
//}
////_______________________________________________________________________________________
//neut::rew::NReWeightCasc * T2KNeutReWeight::GetWghtCascCalc(string name)
//{
//  return (neut::rew::NReWeightCasc*) fNeutReWeight->WghtCalc("nucl_casc");
//}
//#endif
//_______________________________________________________________________________________
void T2KNeutReWeight::SetSystFracErr(T2KSyst_t syst, double val, bool absolute)
{

#ifdef __T2KRW_NEUT_ENABLED__
  if(absolute){
    
    fAtLeastOneAbsTwkDial = true; // CalcChisq function will be meaningless if absolute tweak dial is used for any systematics
    
    NSyst_t nsyst = T2KNeutUtils::T2KtoNSyst(syst);
    
    bool can_handle = fNeutReWeight->Systematics().Added(nsyst); 
    // Override the default values fractional error for the uncertainty with 'val'
    // val is set to 1.0 when SetAbsTweak is called
    // Variation in T2KWghtEngineI is: tweakedvalue = nominal*(1.0+fractional_error * newvalue)
    // becomes an absolute (relative) variation: tweakedvalue = nominal*(1.0+ newvalue)
  
    if(can_handle){    
      NSystUncertainty::Instance()->SetUncertainty(nsyst, val, val);
    }
  }
#endif

}
//_______________________________________________________________________________________
void T2KNeutReWeight::Reconfigure(void)
{
#ifdef __T2KRW_NEUT_ENABLED__
  fNeutReWeight->Reconfigure();
#endif
}
//_______________________________________________________________________________________
void T2KNeutReWeight::Reset(void)
{
#ifdef __T2KRW_NEUT_ENABLED__
#endif
}
//_______________________________________________________________________________________
#ifdef __T2KRW_OAANALYSIS_ENABLED__
double T2KNeutReWeight::CalcWeight(const ND::RooTrackerVtxBase * vtx)
{
  double weight = 1.0;
#ifdef __T2KRW_NEUT_ENABLED__
 
// Code to convert from NRooTracker to NEUT native format and call neut weight 
// calculator method.
  const ND::NRooTrackerVtx * nrootrac_vtx = dynamic_cast<const ND::NRooTrackerVtx*>(vtx);

  if(nrootrac_vtx == NULL){
    cout << "Cannot cast from ND::RooTrackerVtxBase to ND::NRooTrackerVtx object!" << endl;
    return 1.0;
  }

  T2KNeutUtils::fill_neut_commons(nrootrac_vtx);

  weight = fNeutReWeight->CalcWeight();

#endif
  return weight;
}
#endif
//_______________________________________________________________________________________
double T2KNeutReWeight::CalcWeight(const SK::SK__h1 * sktree)
{
  double weight = 1.0;
#ifdef __T2KRW_NEUT_ENABLED__

  T2KNeutUtils::fill_neut_commons(sktree);

  weight = fNeutReWeight->CalcWeight();

#endif
  return weight;
}

//_______________________________________________________________________________________
double T2KNeutReWeight::CalcWeight(const JnuBeamRead * jnutree)
{
  double weight = 1.0;

  return weight;
}
//_______________________________________________________________________________________
double T2KNeutReWeight::CalcWeight(TTree * intree, int entry) 
{
  double weight = 1.0;
#ifdef __T2KRW_NEUT_ENABLED__
  
  T2KNeutUtils::fill_neut_commons(intree,entry);

  weight = fNeutReWeight->CalcWeight();

#endif
  return weight;
}

#ifdef __T2KRW_NEUT_ENABLED__
double T2KNeutReWeight::CalcWeight(NeutVect *nvect)
{
  double weight = 1.0;

  T2KNeutUtils::fill_neut_commons(nvect);

  weight = fNeutReWeight->CalcWeight();

  return weight;
}
#endif

//_______________________________________________________________________________________
double T2KNeutReWeight::CalcChisq()
{
  // This function needs to retrieve the chisq from the external reweighing 
  // library. If fAtLeastOneAbsTwkDial is set to true then it makes no sense to be calculating
  // the chisq so this function needs to print a warning and return 0.0  
  double chisq = 0.0;
#ifdef __T2KRW_NEUT_ENABLED__
  if(fAtLeastOneAbsTwkDial){
    cerr << "Warning: T2KNeutReWeight::CalcChisq is meaningless if using "<<
            " absolute tweak dial for any Neut params, returning 0!" << endl;
  }
  else {
    chisq = fNeutReWeight->CalcChisq();
  }

#endif
  return chisq;
}
//_______________________________________________________________________________________
void T2KNeutReWeight::Print(void)
{
#ifdef __T2KRW_NEUT_ENABLED__
  fNeutReWeight->Print();
#endif
}
//_______________________________________________________________________________________
