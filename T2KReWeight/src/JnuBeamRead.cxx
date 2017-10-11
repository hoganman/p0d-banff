#define JnuBeamRead_cxx
#include <iostream>
#include <math.h>
#include "JnuBeamRead.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void JnuBeamRead::Loop()
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   }
}

int JnuBeamRead::INGRIDModule(int det, double xpos, double ypos){

  if(det==3){
    if(fabs(xpos)<50. && fabs(ypos)<50.) return 3;
    else if(fabs(xpos+150.)<50. && fabs(ypos)<50.) return 2;
    else if(fabs(xpos+300.)<50. && fabs(ypos)<50.) return 1;
    else if(fabs(xpos+450.)<50. && fabs(ypos)<50.) return 0;
    else if(fabs(xpos-150.)<50. && fabs(ypos)<50.) return 4;
    else if(fabs(xpos-300.)<50. && fabs(ypos)<50.) return 5;
    else if(fabs(xpos-450.)<50. && fabs(ypos)<50.) return 6;
    else return -1;
  } else if(det==4){
    if(fabs(ypos)<50. && fabs(xpos)<50.) return 10;
    else if(fabs(ypos+150.)<50. && fabs(xpos)<50.) return 9;
    else if(fabs(ypos+300.)<50. && fabs(xpos)<50.) return 8;
    else if(fabs(ypos+450.)<50. && fabs(xpos)<50.) return 7;
    else if(fabs(ypos-150.)<50. && fabs(xpos)<50.) return 11;
    else if(fabs(ypos-300.)<50. && fabs(xpos)<50.) return 12;
    else if(fabs(ypos-450.)<50. && fabs(xpos)<50.) return 13;
    else return -1;
  } else return -1;


}

