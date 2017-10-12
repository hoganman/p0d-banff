#ifndef ROOTUtils_hxx
#define ROOTUtils_hxx

#include<iostream>
#include<vector>
#include<time.h>

#include"Rtypes.h"

#include"TDatime.h"
#include"TSystem.h"
#include"TRandom3.h"
#include"TMath.h"
#include"TFile.h"
#include"TCanvas.h"

///Have ROOT sleep for time in seconds
///The time is between zero (0) and "scale" seconds.
///For scale == 10, then ROOT will sleep between 0 to 10 seconds
///The seed is from the current Unix time unless otherwise stated
void ROOTRandomSleep(Double_t scale, Int_t seed=0);

///Read in a TFile and check the status of the file contents
///If the file is good, return true;
Bool_t ROOTCheckFile(TString fileName);

/// Split a TString into its components by a separator
std::vector<TString> ROOTSplitString(const TString& theOpt,
    const Char_t separator);

/// Convert a ROOT file with a canvas to various output formats
void ROOTSaveCanvasAs(const TString& inputFileName, const TString& canvasName,
    const TString& outputNamePrefix, const TString& formats = "eps,png,pdf",
    const Char_t delimiter = ',');


#endif
