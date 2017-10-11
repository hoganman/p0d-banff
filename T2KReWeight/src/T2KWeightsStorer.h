//____________________________________________________________________________
/*!

\class    t2krew::T2KWeightsStorer

\brief    Utility to help store generated weights. Outputs a tree which 
          contains a many arrays of weights with dimension equal to the number
          of events. Each array corresponds to a different tweak dial setting.

\author   Jim Dobson <j.dobson07 \at imperial.ac.uk>
          Imperial College London
        
\created  June 6, 2011
*/
//____________________________________________________________________________

#ifndef _T2KRW_WEIGHT_STORER_H_
#define _T2KRW_WEIGHT_STORER_H_

#include <string>

#include "T2KSystSet.h"

class TFile;
class TTree;
class TArrayF;
class TArrayI;

using std::string;

namespace t2krew {

 class T2KWeightsStorer
 {
 public:
   T2KWeightsStorer(const char * outfile_name = "weights.root");
  ~T2KWeightsStorer(); 

  // Notify weights storer that starting to fill new set of weights 
  void NewSystSet(t2krew::T2KSystSet & systset); 

  // Add the next weight for the current set of systs
  void AddWeight(float weight); 

  // Save set of weights to output file
  void SaveToFile (void);
 
 private:

   void Init  (void);
   void Cleanup (void);

   bool FilledCompleteSet (void) { return fNEntriesExpected == fNEntriesSoFar;}

   int fNEntriesExpected;   // The expected number of events to be reweighted 
   int fNEntriesSoFar;      // The number of weights generated for current set
   bool fCompleteSetFilled; // True if have filled a complete set of weights
   int fNSystSetsFilled;    // Number of syst sets for which weights have been generated

   TFile * fOutputFile;
   TTree * fOutputTree;
   TArrayF * fWeightsArray; 
   std::map<int, std::vector<float> *> fWeightsMap;
   std::map<string, std::vector<float> *> fTwkDialValues;
   std::vector<TArrayF*> fTwkDialArrays;

 };
}

#endif

