#ifndef ROOTUtils_hxx
#define ROOTUtils_hxx

#include<iostream>

#include"Rtypes.h"

#include"TDatime.h"
#include"TSystem.h"
#include"TRandom3.h"
#include"TMath.h"

///Have ROOT sleep for time in seconds
///The time is between zero (0) and "scale" seconds.
///The default is NO sleep time is initiated
///For scale == 10, then ROOT will sleep between 0 to 10 seconds
///The seed is from the current Unix time unless otherwise stated
void ROOTRandomSleep(Double_t scale = 0, Int_t seed=0);

#endif
