#include "TAxis.h"


namespace BinningDefinition{

  //namespace numuCC0PiFHC{
    int npbins_0pi1 = 14;
    double pbins_0pi1[15] = {0, 300, 400, 500, 600, 700, 800, 900, 1000, 1250, 1500, 2000, 3000, 5000, 30000};
    int npbins_0pi = 6;
    double pbins_0pi[7] = {0, 1000, 1250, 2000, 3000, 5000, 30000};
    //cos(theta) bins
    //CC0pi
    int nctbins_0pi1 = 11;
    double ctbins_0pi1[12] = {-1, 0.6, 0.7, 0.8, 0.85, 0.9, 0.92, 0.94, 0.96, 0.98, 0.99, 1};
    int nctbins_0pi = 7;
    double ctbins_0pi[8] = { -1, 0.6, 0.7, 0.8, 0.85,0.94, 0.96, 1};
    //}

  //namespace numuCC1PiFHC{
    //CC1pi
    int npbins_1pi1 = 13;
    double pbins_1pi1[14] = {0, 300, 400, 500, 600, 700, 800, 900, 1000, 1250, 1500, 2000, 5000, 30000};
    int npbins_1pi = 5;
    double pbins_1pi[6] = {0, 300, 1250, 1500, 5000, 30000};
    //CC1pi
    int nctbins_1pi1 = 11;
    double ctbins_1pi1[12] = {-1, 0.6, 0.7, 0.8, 0.85, 0.9, 0.92, 0.94, 0.96, 0.98, 0.99, 1};
    int nctbins_1pi = 8;
    double ctbins_1pi[9] = {-1, 0.7, 0.85, 0.9, 0.92, 0.96, 0.98, 0.99, 1};
    //}
  //namespace numuCCOthFHC{
    //CCOther
    int npbins_npi1 = 14;
    double pbins_npi1[15] = {0, 300, 400, 500, 600, 700, 800, 900, 1000, 1250, 1500, 2000, 3000, 5000, 30000};
    int npbins_npi = 5;
    double pbins_npi[6] = {0, 1500, 2000, 3000, 5000, 30000};
    //CCOther
    int nctbins_npi1 = 11;
    double ctbins_npi1[12] = {-1, 0.6, 0.7, 0.8, 0.85, 0.9, 0.92, 0.94, 0.96, 0.98, 0.99, 1};
    int nctbins_npi = 8;
    double ctbins_npi[9] = {-1, 0.8, 0.85, 0.9, 0.92, 0.96, 0.98, 0.99, 1};
  //}
  
  //namespace anumuCC1trRHC{
    
    //Anti-numu QE selection in anti-neutrino beam mode.
    Int_t npbins_anuqe1 = 10;
    Double_t pbins_anuqe1[11] = {0., 400., 500., 600., 700., 800., 900., 1100., 1400., 2000., 10000.};
    Int_t npbins_anuqe = 5;
    Double_t pbins_anuqe[6] = { 0., 400., 900., 1100., 2000., 10000.};

    Int_t nctbins_anuqe1 = 13;
    Double_t ctbins_anuqe1[14] = {-1., 0.6, 0.7, 0.8, 0.85, 0.88, 0.91, 0.93, 0.95, 0.96, 0.97, 0.98, 0.99, 1.00};
    Int_t nctbins_anuqe = 8;
    Double_t ctbins_anuqe[9] = { -1., 0.6, 0.7, 0.88, 0.95, 0.97, 0.98, 0.99, 1.00};
    //}
  
  //Anti-numu nQE selection in anti-neutrino beam mode.
  Int_t npbins_anunqe1 = 7;
  Double_t pbins_anunqe1[8] = {0., 700., 950., 1200., 1500., 2000., 3000., 10000.};
  Int_t npbins_anunqe = 6;
  Double_t pbins_anunqe[7] = {0., 700., 1200., 1500., 2000., 3000., 10000.};
  Int_t nctbins_anunqe1 = 11;
  Double_t ctbins_anunqe1[12] = {-1., 0.75, 0.85, 0.88, 0.91, 0.93, 0.95, 0.96, 0.97, 0.98, 0.99, 1.00};
  Int_t nctbins_anunqe = 6;
  Double_t ctbins_anunqe[7] = {-1., 0.85, 0.88, 0.93, 0.98, 0.99, 1.00};

  //Numu QE selection in anti-neutrino beam mode.
  Int_t npbins_nuqe1 = 6;
  Double_t pbins_nuqe1[7] = {0., 400., 600., 800., 1100., 2000., 10000.};
  Int_t npbins_nuqe = 5;
  Double_t pbins_nuqe[6] = {0., 400., 800., 1100., 2000., 10000.};
  Int_t nctbins_nuqe1 = 11;
  Double_t ctbins_nuqe1[12] = {-1., 0.7, 0.8, 0.85, 0.90, 0.93, 0.95, 0.96, 0.97, 0.98, 0.99, 1.00};
  Int_t nctbins_nuqe = 8;
  Double_t ctbins_nuqe[9] = {-1., 0.7, 0.85, 0.90, 0.93, 0.96, 0.98, 0.99, 1.00};


  //Numu CCnQE selection in anti-neutrino beam mode.
  Int_t npbins_nunqe1 = 8;
  Double_t pbins_nunqe1[9] = {0., 500., 700., 1000., 1250., 1500., 2000., 3000., 10000.};
  Int_t npbins_nunqe = 5;
  Double_t pbins_nunqe[6] = {0., 1000., 1500., 2000., 3000., 10000.};
  Int_t nctbins_nunqe1 = 11;
  Double_t ctbins_nunqe1[12] = {-1., 0.7, 0.8, 0.85, 0.90, 0.93, 0.95, 0.96, 0.97, 0.98, 0.99, 1.00};
  Int_t nctbins_nunqe = 8;
  Double_t ctbins_nunqe[9] = {-1., 0.8, 0.90, 0.93, 0.95, 0.96, 0.97, 0.99, 1.00};

};
