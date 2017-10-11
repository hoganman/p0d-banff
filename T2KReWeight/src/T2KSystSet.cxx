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
#include <cstdlib>

#include "T2KSystSet.h"

using std::cerr;
using std::cout;
using std::endl;

using namespace t2krew;

//_______________________________________________________________________________________
T2KSystSet::T2KSystSet()
{
}
T2KSystSet::T2KSystSet(const T2KSystSet & syst)
{
  this->CopyFrom(syst);
}
//_______________________________________________________________________________________
T2KSystSet::~T2KSystSet()
{
  for(map<T2KSyst_t, T2KSystInfo *>::iterator it = fSystematics.begin(); it != fSystematics.end(); ++it) {
    delete it->second;
  }
  fSystematics.clear();
}
//_______________________________________________________________________________________
void T2KSystSet::Include(T2KSyst_t syst)
{
  if(syst == kSystNull) {
    cout << "Error in T2KSystSet::Include(T2KSyst_t syst): Attempt to add null systematic" << endl;
    exit (1);
  }

  T2KSystInfo * syst_info = new T2KSystInfo;
  fSystematics.insert( map<T2KSyst_t, T2KSystInfo*>::value_type(syst, syst_info) );     
}
//_______________________________________________________________________________________
int T2KSystSet::NIncluded(void) const
{
  return fSystematics.size();
}
//_______________________________________________________________________________________
bool T2KSystSet::IsIncluded(T2KSyst_t syst) const
{
  return (fSystematics.find(syst) != fSystematics.end());
}
//_______________________________________________________________________________________
vector<t2krew::T2KSyst_t> T2KSystSet::AllIncluded(void)
{
  vector<T2KSyst_t> svec;

  map<T2KSyst_t, T2KSystInfo*>::const_iterator it = fSystematics.begin();
  for( ; it != fSystematics.end(); ++it) {
    T2KSyst_t syst = it->first;
    svec.push_back(syst);
  }
  return svec;
}
//_______________________________________________________________________________________
void T2KSystSet::Remove(T2KSyst_t syst)
{
  fSystematics.erase(syst);
}
//_______________________________________________________________________________________
void T2KSystSet::RemoveAll(void)
{
  fSystematics.clear();
}
//_______________________________________________________________________________________
void T2KSystSet::SetTwkDial(T2KSyst_t syst, double val)
{
  if ( this->IsIncluded(syst) ) {
    fSystematics[syst]->CurValue = val;
  }
  else{
      std::cout << "ERROR - T2KSystSet::SetTwkDial - Trying to set tweak dial that has not been Included." << std::endl;
  }
}
//_______________________________________________________________________________________
void T2KSystSet::ResetTwkDial(T2KSyst_t syst)
{ 
  if ( this->IsIncluded(syst) ) {
    this->SetTwkDial(syst, fSystematics[syst]->InitValue);
  }
}
//_______________________________________________________________________________________
void T2KSystSet::ResetAllTwkDials(void)
{ 
  vector<t2krew::T2KSyst_t> svec = this->AllIncluded();
  vector<t2krew::T2KSyst_t>::const_iterator it = svec.begin();
  for( ; it != svec.end(); ++it) {
    this->ResetTwkDial(*it);
  }
}
//_______________________________________________________________________________________
void T2KSystSet::SetFracErr(T2KSyst_t syst, double val)
{
  if ( this->IsIncluded(syst) ) {
    fSystematics[syst]->OverrideDefSigma = true;
    fSystematics[syst]->SigmaOverrideValue = val;
    fSystematics[syst]->AbsTweakDial = false;
  } 
}
//_______________________________________________________________________________________
void T2KSystSet::SetAbsTwk(T2KSyst_t syst)
{
  if ( this->IsIncluded(syst) ) {
    this->SetFracErr(syst, 1.0);
    fSystematics[syst]->AbsTweakDial = true;
  } 
}
//_______________________________________________________________________________________
double T2KSystSet::CurTwkDialVal(T2KSyst_t syst) const
{
  if ( this->IsIncluded(syst) ) return fSystematics.find(syst)->second->CurValue;
  else return 0.;
}
//_______________________________________________________________________________________
double T2KSystSet::CurFracErrVal(T2KSyst_t syst) const
{
  if ( this->IsIncluded(syst) ) {
    if( this->UsingDefFracErr(syst) == false ){
      if( this->AbsTweak(syst) ){
	// KM: Patrick added a warning which better notes this.
	//        cerr << "Warning: meaningless frac error as using abs tweak!" << endl; 
      }
    }
    else {
      // Ideally would like to retrieve from the generator library in this
      // case but is not possible at present as the instance of T2KSystSet in 
      // t2krew::T2KReWeight does not know about the generator libraries  
      cerr << "Warning: meaningless frac error as using generator defaults!" << endl;
    }
    return fSystematics.find(syst)->second->SigmaOverrideValue;
  }
  else cerr << "Warning: meaningless fractional error as could not find systematic!" << endl;
  return 1.0;
}
//_______________________________________________________________________________________
bool T2KSystSet::UsingDefFracErr(T2KSyst_t syst) const
{
  bool using_def = true;
  if ( this->IsIncluded(syst) ){
    if(fSystematics.find(syst)->second->OverrideDefSigma) using_def = false;
  }
  return using_def;
}
//_______________________________________________________________________________________
bool T2KSystSet::AbsTweak(T2KSyst_t syst) const
{
  if ( this->IsIncluded(syst) ){
    return fSystematics.find(syst)->second->AbsTweakDial;
  }
  return false;
}
//_______________________________________________________________________________________
void T2KSystSet::PrintSummary(void)
{
  cout << "Have included " << this->NIncluded() << " systematics:" << endl;  
				    
  vector<t2krew::T2KSyst_t> svec = this->AllIncluded();
  T2KSyst syst_obj;

  vector<t2krew::T2KSyst_t>::const_iterator it = svec.begin();
  for( ; it != svec.end(); ++it) {
    T2KSyst_t syst = *it;
    
    cout << " |--> " << syst_obj.AsString(syst) << " = " << this->CurTwkDialVal(syst);
    if(this->UsingDefFracErr(syst))
      cout << " (using generator def for 1sigma frac error)" << endl;
    else if(this->AbsTweak(syst))
      cout << " (using absolute twk dial: frac error -> "<< this->CurFracErrVal(syst) << ")" << endl;
    else
      cout << " (using user def for 1sigma frac error = "<< this->CurFracErrVal(syst) << ")" << endl;
  }
}
//_______________________________________________________________________________________
void T2KSystSet::CopyFrom(const T2KSystSet & syst_set)
{
  // Clear out any existing ones 
  this->RemoveAll();

  // Loop over all systs in syst_set and ad ad copy to this set
  map<T2KSyst_t, T2KSystInfo*>::const_iterator it = syst_set.fSystematics.begin();
  for( ; it != syst_set.fSystematics.end(); ++it) {
    T2KSyst_t syst = it->first;
    this->Include(syst);
    fSystematics.find(syst)->second->CopyFrom(*it->second); 
  }
}
//_______________________________________________________________________________________
