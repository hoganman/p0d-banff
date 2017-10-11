//____________________________________________________________________________
/*
 For the class documentation see the corresponding header file.
*/
//____________________________________________________________________________
#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include <cstdlib>

#include <TFile.h>
#include <TTree.h>
#include <TArrayF.h>

#include "T2KWeightsStorer.h"

#include "T2KSystSet.h"

using std::cout;
using std::endl;
using std::vector;
using std::map;

using namespace t2krew;

//____________________________________________________________________________
T2KWeightsStorer::T2KWeightsStorer(const char * outfilename) 
{
  this->Init(); 
  std::cout << "New WeightsStorer outputfile: " << outfilename << std::endl << std::endl;
  fOutputFile = new TFile(outfilename, "RECREATE");
  bool success = fOutputFile && fOutputFile->IsZombie() == false;
  if(success == false){
    // JIMTODO - consider changing to setting an ignore flag which disables
    // class rather than exiting
    std::cerr << "Cannot open output file - exiting!" << std::endl;
    exit(1);
  }

}
//____________________________________________________________________________
T2KWeightsStorer::~T2KWeightsStorer()
{
  this->Cleanup();
}
//____________________________________________________________________________
void T2KWeightsStorer::NewSystSet(T2KSystSet & systset){

  if(fNSystSetsFilled == 1 && fNEntriesExpected < 0){ 
    cout << "Setting number of expected weights as the number for the first set of systs!" << endl;
    fNEntriesExpected = fNEntriesSoFar;
  }

  // Check that have filled a complete set
  bool fCannotContinue = false;
  if(fNEntriesSoFar != fNEntriesExpected){
    // Only enforce if not in first set
    if(fNEntriesExpected >= 0) fCannotContinue = true; 
  } 

  if(fCannotContinue){
    // JIMTODO - consider changing to setting an ignore flag which disables
    // class rather than exiting
    std::cerr << "Severe mismatch when filling weights - cannot continue!" << std::endl;
    exit(1);
  }

  // Loop over the current set of systematics and store values for use later
  vector<t2krew::T2KSyst_t> systs = systset.AllIncluded(); 
  for(unsigned int isyst = 0; isyst < systs.size(); isyst++){ 
    
    string name = T2KSyst::AsString(systs[isyst]);   
    // If this is the first time then make new entries
    if(fTwkDialValues.find(name) == fTwkDialValues.end()){
      fTwkDialValues[name] = new vector<float>;
    }
    fTwkDialValues[name]->push_back(systset.CurTwkDialVal(systs[isyst]));
  } 

  // Reset counter if ok to continue 
  fNEntriesSoFar = 0;  
  fCompleteSetFilled = true;
  fNSystSetsFilled++;
}
//____________________________________________________________________________
void T2KWeightsStorer::AddWeight(float weight){

  if(this->FilledCompleteSet()){
    cout << "Weights filled - start a new set before adding more! " << endl; 
    return;
  }  
  // Add new vector of weights if first time for this event 
  if(fWeightsMap.find(fNEntriesSoFar) == fWeightsMap.end()){
    fWeightsMap[fNEntriesSoFar] = new vector<float>;
  }

  // Fill the weight for the current set of systematics as the next entry in 
  // the vector of weights for this event
  fWeightsMap.find(fNEntriesSoFar)->second->push_back(weight);
  fNEntriesSoFar++;
}
//____________________________________________________________________________
void T2KWeightsStorer::SaveToFile(void){
  // if only one set of systsets, then FilledCompleteSet is a given
  if(fNSystSetsFilled == 1) {fNEntriesExpected = fNEntriesSoFar;}
  // Check that have filled a complete set of weights
  if(this->FilledCompleteSet()){ 
    // Save current set of weights if an output file exists 
    if(fOutputFile && fOutputFile->IsZombie() == false){

      cout << "Generated weights for "<< fNSystSetsFilled << 
	" different syst sets for " << fWeightsMap.size() << 
	" events - saving to output file" << endl;

      int nsystsets = fNSystSetsFilled; 
      
      fOutputFile->cd();      
      fOutputTree = new TTree("weightstree", "Tree storing TArray of weights and twkdials per event");
      fWeightsArray = new TArrayF(nsystsets);
      
      fOutputTree->Branch("nweights", &nsystsets);
      fOutputTree->Branch("weights", &fWeightsArray);
      
      TArrayF * fCurrArray[fTwkDialValues.size()];
      int idial=0;
      
      std::map<string, std::vector<float> *>::iterator systvals_it = fTwkDialValues.begin();
      for(; systvals_it != fTwkDialValues.end(); systvals_it++){
      	std::vector<float> * values = systvals_it->second;
        fCurrArray[idial] = new TArrayF(values->size());
        for(unsigned int iarray = 0; iarray < values->size(); iarray++){
          fCurrArray[idial]->SetAt(values->at(iarray), iarray);
        }
        fTwkDialArrays.push_back(fCurrArray[idial]); 
        string branch_name = "br"+ systvals_it->first;
        fOutputTree->Branch(branch_name.c_str(), &fCurrArray[idial]);
      	idial++;
      }
      
      // Loop over events
      for(unsigned int ievt=0; ievt<fWeightsMap.size(); ievt++){
        assert((int) fWeightsMap[ievt]->size() == nsystsets);
        for(int iset = 0; iset < nsystsets; iset++){
          float weight = fWeightsMap[ievt]->at(iset);
          fWeightsArray->SetAt(weight, iset);
        }
        fOutputTree->Fill();
      }
      fOutputTree->Write();
    }
    else std::cout << "Cannot save weights - no output file!" << std::endl;
  }
  else {
    std::cerr << 
      "Cannot save weights as have not filled a complete set!" << std::endl;  
  }
}
//____________________________________________________________________________
void T2KWeightsStorer::Init(void)
{
  //std::cout << "Initializing T2KWeightsStorer" << std::endl;
  fNEntriesExpected = -1;
  fNEntriesSoFar = 0;
  fCompleteSetFilled = false;
  fOutputFile = NULL;
  fNSystSetsFilled = 0;
  fOutputTree = NULL;
  fWeightsArray = NULL;
}
//____________________________________________________________________________
void T2KWeightsStorer::Cleanup(void)
{
  for (unsigned int i=0; i<fWeightsMap.size(); i++) 
    if (fWeightsMap[i]) delete fWeightsMap[i];
  
  std::map<string, std::vector<float> *>::iterator systvals_it = fTwkDialValues.begin();
  for(; systvals_it != fTwkDialValues.end(); systvals_it++)
    delete systvals_it->second;
  
  for (unsigned int i=0; i<fTwkDialArrays.size(); i++) 
    if (fTwkDialArrays[i]) delete fTwkDialArrays[i];
  
  if (fWeightsArray) delete fWeightsArray;
  if (fOutputTree) delete fOutputTree;
  if (fOutputFile) {fOutputFile->Close(); delete fOutputFile;}
}
//____________________________________________________________________________

