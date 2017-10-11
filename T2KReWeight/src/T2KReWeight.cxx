//____________________________________________________________________________
/*

 For the class documentation see the corresponding header file.

 Authors:
   Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
   STFC, Rutherford Appleton Laboratory

   Jim Dobson <J.Dobson07 \at imperial.ac.uk>
   Imperial College London

*/
//____________________________________________________________________________

#include <iostream>
#include <vector>
#include <TMath.h>
#include <TString.h>

#include "T2KBuild.h"
#include "T2KReWeight.h"

//!!#define _T2K_REWEIGHT_DEBUG_ //!!

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

using namespace t2krew;

//_______________________________________________________________________________________
T2KReWeight::T2KReWeight()
{
}
//_______________________________________________________________________________________
T2KReWeight::~T2KReWeight()
{
  this->CleanUp();
}
//____________________________________________________________________________
void T2KReWeight::AdoptWghtEngine(string name, T2KWghtEngineI* wcalc)
{
  if(!wcalc) return;
  fWghtCalc.insert(map<string, T2KWghtEngineI*>::value_type(name,wcalc));
}
//____________________________________________________________________________
T2KWghtEngineI* T2KReWeight::WghtEngine(string name)
{
  map<string, T2KWghtEngineI*>::iterator iter = fWghtCalc.find(name);
  if(iter != fWghtCalc.end()) return iter->second;

  return 0;
}
//_______________________________________________________________________________________
T2KSystSet & T2KReWeight::Systematics(void)
{ 
  return fSystSet; 
}
//_______________________________________________________________________________________
void T2KReWeight::Reconfigure(void)
{
// Loop over all added weighting engines and try to update all systematics for each.
#ifdef _T2K_REWEIGHT_DEBUG_
  cout << endl << "------------------------------" << endl;
  cout << endl << "T2KReWeight: Reconfiguring ..." << endl;
#endif

  vector<T2KSyst_t> svec = fSystSet.AllIncluded();

  map<string, T2KWghtEngineI *>::iterator it = fWghtCalc.begin();
  for( ; it != fWghtCalc.end(); ++it) {

      T2KWghtEngineI * wcalc = it->second;

      vector<T2KSyst_t>::const_iterator parm_iter = svec.begin();
      for( ; parm_iter != svec.end(); ++parm_iter) {
          T2KSyst_t syst = *parm_iter;

          // Update the tweaking dials 
          wcalc->SetSystTwkDial(syst, fSystSet.CurTwkDialVal(syst));
 
          // If requesed modify fractional errors
          if(fSystSet.UsingDefFracErr(syst) == false){
            bool abs_tweak = fSystSet.AbsTweak(syst);  
            double frac_err = fSystSet.CurFracErrVal(syst);
            wcalc->SetSystFracErr(syst, frac_err, abs_tweak);
          }

      }//params

      wcalc->Reconfigure();

  }//weight calculators

#ifdef _T2K_REWEIGHT_DEBUG_
  this->Print();

  fSystSet.PrintSummary();
  cout << "T2KReWeight: Done reconfiguring" << endl << endl;
  cout << endl << "------------------------------" << endl;
#endif
}
//_______________________________________________________________________________________
#ifdef __T2KRW_OAANALYSIS_ENABLED__
double T2KReWeight::CalcWeight(const ND::RooTrackerVtxBase * event)
{
// Calculate weight for all enabled weighting engines for a given entry in input tree.
//
  double weight = 1.0;
  map<string, T2KWghtEngineI *>::iterator it = fWghtCalc.begin();
  for( ; it != fWghtCalc.end(); ++it) {
    T2KWghtEngineI * wcalc = it->second;
    double w = wcalc->CalcWeight(event); 
#ifdef _T2K_REWEIGHT_DEBUG_
    cout << "Calculator: " << it->first << " => wght = " << w << endl;
#endif
    weight *= w;
  }
  return weight;
}
#endif
//_______________________________________________________________________________________
double T2KReWeight::CalcWeight(const SK::SK__h1 * sktree)
{
// Calculate weight for all enabled weighting engines for a given entry in SK input tree.
//
  double weight = 1.0;
  map<string, T2KWghtEngineI *>::iterator it = fWghtCalc.begin();
  for( ; it != fWghtCalc.end(); ++it) {
    T2KWghtEngineI * wcalc = it->second;
    double w = wcalc->CalcWeight(sktree); 
#ifdef _T2K_REWEIGHT_DEBUG_
    cout << "Calculator: " << it->first << " => wght = " << w << endl;
#endif
    weight *= w;
  }
  return weight;
}
//_______________________________________________________________________________________
double T2KReWeight::CalcWeight(const JnuBeamRead * jnubeam)
{
// Calculate weight for all enabled weighting engines for a given entry in SK input tree.
//
  double weight = 1.0;
  map<string, T2KWghtEngineI *>::iterator it = fWghtCalc.begin();
  for( ; it != fWghtCalc.end(); ++it) {
    T2KWghtEngineI * wcalc = it->second;
    double w = wcalc->CalcWeight(jnubeam); 
#ifdef _T2K_REWEIGHT_DEBUG_
    cout << "Calculator: " << it->first << " => wght = " << w << endl;
#endif
    weight *= w;
  }
  return weight;
}
//_______________________________________________________________________________________
double T2KReWeight::CalcWeight(TTree * intree, int entry)
{
// Calculate weight for all enabled weighting engines for a given entry in input tree.
//
  double weight = 1.0;
  map<string, T2KWghtEngineI *>::iterator it = fWghtCalc.begin();
  for( ; it != fWghtCalc.end(); ++it) {
    T2KWghtEngineI * wcalc = it->second;
    double w = wcalc->CalcWeight(intree, entry); 
#ifdef _T2K_REWEIGHT_DEBUG_
    cout << "Calculator: " << it->first << " => wght = " << w << endl;
#endif
    weight *= w;
  }
  return weight;
}
#ifdef __T2KRW_NEUT_ENABLED__
//_______________________________________________________________________________________
double T2KReWeight::CalcWeight(NeutVect* nvect)
{

  double weight = 1.0;
  map<string, T2KWghtEngineI *>::iterator it = fWghtCalc.begin();
  for( ; it != fWghtCalc.end(); ++it) {
    T2KWghtEngineI * wcalc = it->second;
    double w = wcalc->CalcWeight(nvect);
#ifdef _T2K_REWEIGHT_DEBUG_
    cout << "Calculator: " << it->first << " => wght = " << w << endl;
#endif
    weight *= w;
  }
  return weight;
}
#endif

//_______________________________________________________________________________________
double T2KReWeight::CalcChisq(void) 
{
// calculate the sum of penalty terms for all generators tweaked physics params 
  double tot_chisq = 0.0;

  map<string, T2KWghtEngineI *>::iterator it = fWghtCalc.begin();
  for( ; it != fWghtCalc.end(); ++it) {
    T2KWghtEngineI * wcalc = it->second;
    double chisq = wcalc->CalcChisq(); 
    //#ifdef _T2K_REWEIGHT_DEBUG_
    //cout << "Calculator: " << it->first << " => chisq = " << chisq << endl;       
    //#endif
    tot_chisq += chisq;
  }//weight calculators 

  return tot_chisq;
}
//_______________________________________________________________________________________
void T2KReWeight::CleanUp(void)
{
  map<string, T2KWghtEngineI *>::iterator it = fWghtCalc.begin();
  for( ; it != fWghtCalc.end(); ++it) {
    T2KWghtEngineI * rw = it->second;
    if(rw) {
      delete rw;
      rw=0;
    }
  }
  fWghtCalc.clear();
}
//____________________________________________________________________________
void T2KReWeight::Print()
{
  map<string, T2KWghtEngineI *>::iterator it = fWghtCalc.begin();
  for( ; it != fWghtCalc.end(); ++it) {
    T2KWghtEngineI * rw = it->second;
    if(rw) {
      cout << "\nPrinting info for calculator: "<< it->first << endl; 
      rw->Print();
      cout << "-------" << endl;
    }
  }
  cout << endl;
 
//  vector<T2KSyst_t> syst_vec = this->Systematics().AllIncluded();
//  int vec_size = syst_vec.size();

//  cout << "Current set of systematic params is:" << endl;   
//  for(int i = 0 ; i < vec_size ; i ++){
//     cout << " --o " << T2KSyst::AsString(syst_vec[i]) << 
//             " is set at " << this->Systematics().CurValue(syst_vec[i]) << endl;
//  }                             
//  double chi2val = this->CalcChisq();
//  cout << "Chisq_{penalty} = " << chi2val << endl;
}
//____________________________________________________________________________

