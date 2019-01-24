#define BINNINGDEFINITION_CXX

#include <BinningDefinition.hxx>
#include "sys/types.h"

//*****************************************************************************
BANFF::BinningDefinition::BinningDefinition(){
//*****************************************************************************
  Do4PiFHC     = (bool)ND::params().GetParameterI("BANFF.Do4PiFHC");
  DoNue        = (bool)ND::params().GetParameterI("BANFF.DoNueSelections");
  DoOnlyNue    = (bool)ND::params().GetParameterI("BANFF.DoOnlyNueSelections");
  DoOnlyP0D    = (bool)ND::params().GetParameterI("BANFF.DoOnlyP0DSelections");
  DoMultiPiRHC = (bool)ND::params().GetParameterI("BANFF.DoMultiPiRHC");

  Do1DCheckMom       = (bool)ND::params().GetParameterI("BANFF.Do1DCheckMom");
  Do1DCheckCos       = (bool)ND::params().GetParameterI("BANFF.Do1DCheckCos");
  UseFlatNuMuBinning = (bool)ND::params().GetParameterI("BANFF.UseFlatNuMuBinning");

  if(Do1DCheckMom && Do1DCheckCos){
    std::cerr << "You cannot check 1D Cov Mat for cos and mom in the same time." << std::endl;
    std::cerr << "Change the parameter file (Do1DCheck)" << std::endl;
    throw;
  }
  FullToReduMap.clear();

  //P0D FHC numuCC-Inclusive
  int    P0DFHCNumuCCInclusive_Mom_NBin = 8;
  double P0DFHCNumuCCInclusive_Mom_Bin[9] = {0, 450, 700, 1100, 1600, 2200, 3000, 5000, 30000};
  int    P0DFHCNumuCCInclusive_Det_Mom_NBin = 6;
  double P0DFHCNumuCCInclusive_Det_Mom_Bin[7] = {0, 450, 1100, 2200, 3000, 5000, 30000};

  int    P0DFHCNumuCCInclusive_Cos_NBin = 5;
  double P0DFHCNumuCCInclusive_Cos_Bin[6] = {-1.0, +0.5, +0.82, +0.94, +0.986, +1.0};
  int    P0DFHCNumuCCInclusive_Det_Cos_NBin = 4;
  double P0DFHCNumuCCInclusive_Det_Cos_Bin[5] = {-1.0, +0.5, +0.82, +0.94, +1.0};

  //P0D RHC numu BKG CC-Inclusive
  int    P0DRHCNumuBkgCCInclusive_Mom_NBin = 8;
  double P0DRHCNumuBkgCCInclusive_Mom_Bin[9] = {0, 450, 700, 1100, 1600, 2200, 3000, 5000, 30000};
  int    P0DRHCNumuBkgCCInclusive_Det_Mom_NBin = 6;
  double P0DRHCNumuBkgCCInclusive_Det_Mom_Bin[7] = {0, 450, 1100, 2200, 3000, 5000, 30000};

  int    P0DRHCNumuBkgCCInclusive_Cos_NBin = 5;
  double P0DRHCNumuBkgCCInclusive_Cos_Bin[6] = {-1.0, +0.5, +0.82, +0.94, +0.986, +1.0};
  int    P0DRHCNumuBkgCCInclusive_Det_Cos_NBin = 4;
  double P0DRHCNumuBkgCCInclusive_Det_Cos_Bin[5] = {-1.0, +0.5, +0.82, +0.94, +1.0};

  //P0D RHC numubar CC-Inclusive
  int    P0DRHCNumubarCCInclusive_Mom_NBin = 8;
  double P0DRHCNumubarCCInclusive_Mom_Bin[9] = {0, 450, 700, 1100, 1600, 2200, 3000, 5000, 30000};
  int    P0DRHCNumubarCCInclusive_Det_Mom_NBin = 6;
  double P0DRHCNumubarCCInclusive_Det_Mom_Bin[7] = {0, 450, 1100, 2200, 3000, 5000, 30000};

  int    P0DRHCNumubarCCInclusive_Cos_NBin = 5;
  double P0DRHCNumubarCCInclusive_Cos_Bin[6] = {-1.0, +0.5, +0.82, +0.94, +0.986, +1.0};
  int    P0DRHCNumubarCCInclusive_Det_Cos_NBin = 4;
  double P0DRHCNumubarCCInclusive_Det_Cos_Bin[5] = {-1.0, +0.5, +0.82, +0.94, +1.0};

  //P0D FHC numu CC 1-track
  int    P0DFHCNumuCC1Tr_Mom_NBin = 7;
  double P0DFHCNumuCC1Tr_Mom_Bin[8] = {0, 400, 600, 800, 1000, 2500, 5000, 30000};
  int    P0DFHCNumuCC1Tr_Det_Mom_NBin = 6;
  int    P0DFHCNumuCC1Tr_Det_Mom_Bin[5] = {0, 600, 1000, 5000, 30000};

  int    P0DFHCNumuCC1Tr_Cos_NBin = 8;
  double P0DFHCNumuCC1Tr_Cos_Bin[9] = {-1.0, 0.7, 0.85, 0.92, 0.96, 0.98, 0.99, 0.995, 1.0};
  int    P0DFHCNumuCC1Tr_Det_Cos_NBin = 4;
  int    P0DFHCNumuCC1Tr_Det_Cos_Bin[5] = {-1.0, 0.85, 0.92, 0.96, 1.0};


  //FHCNumuCC0Pi
  int    FHCNumuCC0Pi_Mom_NBin = 14;
  double FHCNumuCC0Pi_Mom_Bin[15] = {0, 300, 400, 500, 600, 700, 800, 900, 1000, 1250, 1500, 2000, 3000, 5000, 30000};
  int    FHCNumuCC0Pi_Det_Mom_NBin = 6;
  double FHCNumuCC0Pi_Det_Mom_Bin[7] = {0, 1000, 1250, 2000, 3000, 5000, 30000};

  int    FHCNumuCC0Pi_Cos_NBin = 11;
  double FHCNumuCC0Pi_Cos_Bin[25];
  int    FHCNumuCC0Pi_Det_Cos_NBin = 7;
  double FHCNumuCC0Pi_Det_Cos_Bin[25];

  if(!Do4PiFHC){
    FHCNumuCC0Pi_Cos_NBin = 11;
    FHCNumuCC0Pi_Cos_Bin[ 0] = -1.0;
    FHCNumuCC0Pi_Cos_Bin[ 1] =  0.6;
    FHCNumuCC0Pi_Cos_Bin[ 2] =  0.7;
    FHCNumuCC0Pi_Cos_Bin[ 3] =  0.8;
    FHCNumuCC0Pi_Cos_Bin[ 4] =  0.85;
    FHCNumuCC0Pi_Cos_Bin[ 5] =  0.90;
    FHCNumuCC0Pi_Cos_Bin[ 6] =  0.92;
    FHCNumuCC0Pi_Cos_Bin[ 7] =  0.94;
    FHCNumuCC0Pi_Cos_Bin[ 8] =  0.96;
    FHCNumuCC0Pi_Cos_Bin[ 9] =  0.98;
    FHCNumuCC0Pi_Cos_Bin[10] =  0.99;
    FHCNumuCC0Pi_Cos_Bin[11] =  1.0;
    FHCNumuCC0Pi_Det_Cos_NBin = 7;
    FHCNumuCC0Pi_Det_Cos_Bin[ 0] = -1.0;
    FHCNumuCC0Pi_Det_Cos_Bin[ 1] =  0.6;
    FHCNumuCC0Pi_Det_Cos_Bin[ 2] =  0.7;
    FHCNumuCC0Pi_Det_Cos_Bin[ 3] =  0.8;
    FHCNumuCC0Pi_Det_Cos_Bin[ 4] =  0.85;
    FHCNumuCC0Pi_Det_Cos_Bin[ 5] =  0.94;
    FHCNumuCC0Pi_Det_Cos_Bin[ 6] =  0.96;
    FHCNumuCC0Pi_Det_Cos_Bin[ 7] =  1.;
  }else{
    FHCNumuCC0Pi_Cos_NBin = 19;         FHCNumuCC0Pi_Det_Cos_NBin = 19;
    FHCNumuCC0Pi_Cos_Bin[ 0] = -1.0;    FHCNumuCC0Pi_Det_Cos_Bin[ 0] = -1.0;
    FHCNumuCC0Pi_Cos_Bin[ 1] = -0.6;    FHCNumuCC0Pi_Det_Cos_Bin[ 1] = -0.6;
    FHCNumuCC0Pi_Cos_Bin[ 2] = -0.4;    FHCNumuCC0Pi_Det_Cos_Bin[ 2] = -0.4;
    FHCNumuCC0Pi_Cos_Bin[ 3] = -0.2;    FHCNumuCC0Pi_Det_Cos_Bin[ 3] = -0.2;
    FHCNumuCC0Pi_Cos_Bin[ 4] = -0.1;    FHCNumuCC0Pi_Det_Cos_Bin[ 4] = -0.1;
    FHCNumuCC0Pi_Cos_Bin[ 5] =  0.;     FHCNumuCC0Pi_Det_Cos_Bin[ 5] =  0.;
    FHCNumuCC0Pi_Cos_Bin[ 6] =  0.1;    FHCNumuCC0Pi_Det_Cos_Bin[ 6] =  0.1;
    FHCNumuCC0Pi_Cos_Bin[ 7] =  0.2;    FHCNumuCC0Pi_Det_Cos_Bin[ 7] =  0.2;
    FHCNumuCC0Pi_Cos_Bin[ 8] =  0.4;    FHCNumuCC0Pi_Det_Cos_Bin[ 8] =  0.4;
    FHCNumuCC0Pi_Cos_Bin[ 9] =  0.6;    FHCNumuCC0Pi_Det_Cos_Bin[ 9] =  0.6;
    FHCNumuCC0Pi_Cos_Bin[10] =  0.7;    FHCNumuCC0Pi_Det_Cos_Bin[10] =  0.7;
    FHCNumuCC0Pi_Cos_Bin[11] =  0.8;    FHCNumuCC0Pi_Det_Cos_Bin[11] =  0.8;
    FHCNumuCC0Pi_Cos_Bin[12] =  0.85;   FHCNumuCC0Pi_Det_Cos_Bin[12] =  0.85;
    FHCNumuCC0Pi_Cos_Bin[13] =  0.90;   FHCNumuCC0Pi_Det_Cos_Bin[13] =  0.90;
    FHCNumuCC0Pi_Cos_Bin[14] =  0.92;   FHCNumuCC0Pi_Det_Cos_Bin[14] =  0.92;
    FHCNumuCC0Pi_Cos_Bin[15] =  0.94;   FHCNumuCC0Pi_Det_Cos_Bin[15] =  0.94;
    FHCNumuCC0Pi_Cos_Bin[16] =  0.96;   FHCNumuCC0Pi_Det_Cos_Bin[16] =  0.96;
    FHCNumuCC0Pi_Cos_Bin[17] =  0.98;   FHCNumuCC0Pi_Det_Cos_Bin[17] =  0.98;
    FHCNumuCC0Pi_Cos_Bin[18] =  0.99;   FHCNumuCC0Pi_Det_Cos_Bin[18] =  0.99;
    FHCNumuCC0Pi_Cos_Bin[19] =  1.0;    FHCNumuCC0Pi_Det_Cos_Bin[19] =  1.0;
  }

  if(Do1DCheckMom){FHCNumuCC0Pi_Det_Cos_NBin=1; FHCNumuCC0Pi_Det_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){FHCNumuCC0Pi_Det_Mom_NBin=1; FHCNumuCC0Pi_Det_Mom_Bin[1] = 30000;}

  //FHCNumuCC1Pi
  int    FHCNumuCC1Pi_Mom_NBin = 13;
  double FHCNumuCC1Pi_Mom_Bin[14] = {0, 300, 400, 500, 600, 700, 800, 900, 1000, 1250, 1500, 2000, 5000, 30000};
  int    FHCNumuCC1Pi_Det_Mom_NBin = 5;
  double FHCNumuCC1Pi_Det_Mom_Bin[6] = {0, 300, 1250, 1500, 5000, 30000};

  int    FHCNumuCC1Pi_Cos_NBin = 11;
  double FHCNumuCC1Pi_Cos_Bin[25];
  int    FHCNumuCC1Pi_Det_Cos_NBin = 8;
  double FHCNumuCC1Pi_Det_Cos_Bin[25];

  if(!Do4PiFHC){
    FHCNumuCC1Pi_Cos_NBin = 11;
    FHCNumuCC1Pi_Cos_Bin[ 0] = -1.0;
    FHCNumuCC1Pi_Cos_Bin[ 1] =  0.6;
    FHCNumuCC1Pi_Cos_Bin[ 2] =  0.7;
    FHCNumuCC1Pi_Cos_Bin[ 3] =  0.8;
    FHCNumuCC1Pi_Cos_Bin[ 4] =  0.85;
    FHCNumuCC1Pi_Cos_Bin[ 5] =  0.90;
    FHCNumuCC1Pi_Cos_Bin[ 6] =  0.92;
    FHCNumuCC1Pi_Cos_Bin[ 7] =  0.94;
    FHCNumuCC1Pi_Cos_Bin[ 8] =  0.96;
    FHCNumuCC1Pi_Cos_Bin[ 9] =  0.98;
    FHCNumuCC1Pi_Cos_Bin[10] =  0.99;
    FHCNumuCC1Pi_Cos_Bin[11] =  1.0;
    FHCNumuCC1Pi_Det_Cos_NBin = 8;
    FHCNumuCC1Pi_Det_Cos_Bin[ 0] = -1.0;
    FHCNumuCC1Pi_Det_Cos_Bin[ 1] =  0.7;
    FHCNumuCC1Pi_Det_Cos_Bin[ 2] =  0.85;
    FHCNumuCC1Pi_Det_Cos_Bin[ 3] =  0.9;
    FHCNumuCC1Pi_Det_Cos_Bin[ 4] =  0.92;
    FHCNumuCC1Pi_Det_Cos_Bin[ 5] =  0.96;
    FHCNumuCC1Pi_Det_Cos_Bin[ 6] =  0.98;
    FHCNumuCC1Pi_Det_Cos_Bin[ 7] =  0.99;
    FHCNumuCC1Pi_Det_Cos_Bin[ 8] =  1.0;
  }else{
    FHCNumuCC1Pi_Cos_NBin = 19;         FHCNumuCC1Pi_Det_Cos_NBin = 19;
    FHCNumuCC1Pi_Cos_Bin[ 0] = -1.0;    FHCNumuCC1Pi_Det_Cos_Bin[ 0] = -1.0;
    FHCNumuCC1Pi_Cos_Bin[ 1] = -0.6;    FHCNumuCC1Pi_Det_Cos_Bin[ 1] = -0.6;
    FHCNumuCC1Pi_Cos_Bin[ 2] = -0.4;    FHCNumuCC1Pi_Det_Cos_Bin[ 2] = -0.4;
    FHCNumuCC1Pi_Cos_Bin[ 3] = -0.2;    FHCNumuCC1Pi_Det_Cos_Bin[ 3] = -0.2;
    FHCNumuCC1Pi_Cos_Bin[ 4] = -0.1;    FHCNumuCC1Pi_Det_Cos_Bin[ 4] = -0.1;
    FHCNumuCC1Pi_Cos_Bin[ 5] =  0.;      FHCNumuCC1Pi_Det_Cos_Bin[ 5] =  0.;
    FHCNumuCC1Pi_Cos_Bin[ 6] =  0.1;    FHCNumuCC1Pi_Det_Cos_Bin[ 6] =  0.1;
    FHCNumuCC1Pi_Cos_Bin[ 7] =  0.2;    FHCNumuCC1Pi_Det_Cos_Bin[ 7] =  0.2;
    FHCNumuCC1Pi_Cos_Bin[ 8] =  0.4;    FHCNumuCC1Pi_Det_Cos_Bin[ 8] =  0.4;
    FHCNumuCC1Pi_Cos_Bin[ 9] =  0.6;    FHCNumuCC1Pi_Det_Cos_Bin[ 9] =  0.6;
    FHCNumuCC1Pi_Cos_Bin[10] =  0.7;    FHCNumuCC1Pi_Det_Cos_Bin[10] =  0.7;
    FHCNumuCC1Pi_Cos_Bin[11] =  0.8;    FHCNumuCC1Pi_Det_Cos_Bin[11] =  0.8;
    FHCNumuCC1Pi_Cos_Bin[12] =  0.85;    FHCNumuCC1Pi_Det_Cos_Bin[12] =  0.85;
    FHCNumuCC1Pi_Cos_Bin[13] =  0.90;    FHCNumuCC1Pi_Det_Cos_Bin[13] =  0.90;
    FHCNumuCC1Pi_Cos_Bin[14] =  0.92;    FHCNumuCC1Pi_Det_Cos_Bin[14] =  0.92;
    FHCNumuCC1Pi_Cos_Bin[15] =  0.94;    FHCNumuCC1Pi_Det_Cos_Bin[15] =  0.94;
    FHCNumuCC1Pi_Cos_Bin[16] =  0.96;    FHCNumuCC1Pi_Det_Cos_Bin[16] =  0.96;
    FHCNumuCC1Pi_Cos_Bin[17] =  0.98;    FHCNumuCC1Pi_Det_Cos_Bin[17] =  0.98;
    FHCNumuCC1Pi_Cos_Bin[18] =  0.99;    FHCNumuCC1Pi_Det_Cos_Bin[18] =  0.99;
    FHCNumuCC1Pi_Cos_Bin[19] =  1.0;  FHCNumuCC1Pi_Det_Cos_Bin[19] =  1.0;
  }
  if(Do1DCheckMom){FHCNumuCC1Pi_Det_Cos_NBin=1; FHCNumuCC1Pi_Det_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){FHCNumuCC1Pi_Det_Mom_NBin=1; FHCNumuCC1Pi_Det_Mom_Bin[1] = 30000;}

  //FHCNumuCCOth
  int    FHCNumuCCOth_Mom_NBin = 14;
  double FHCNumuCCOth_Mom_Bin[15] = {0, 300, 400, 500, 600, 700, 800, 900, 1000, 1250, 1500, 2000, 3000, 5000, 30000};
  int    FHCNumuCCOth_Det_Mom_NBin = 5;
  double FHCNumuCCOth_Det_Mom_Bin[6] = {0, 1500, 2000, 3000, 5000, 30000};

  int    FHCNumuCCOth_Cos_NBin = 11;
  double FHCNumuCCOth_Cos_Bin[25];
  int    FHCNumuCCOth_Det_Cos_NBin = 8;
  double FHCNumuCCOth_Det_Cos_Bin[25];

  if(!Do4PiFHC){
    FHCNumuCCOth_Cos_NBin = 11;
    FHCNumuCCOth_Cos_Bin[ 0] = -1.0;
    FHCNumuCCOth_Cos_Bin[ 1] =  0.6;
    FHCNumuCCOth_Cos_Bin[ 2] =  0.7;
    FHCNumuCCOth_Cos_Bin[ 3] =  0.8;
    FHCNumuCCOth_Cos_Bin[ 4] =  0.85;
    FHCNumuCCOth_Cos_Bin[ 5] =  0.90;
    FHCNumuCCOth_Cos_Bin[ 6] =  0.92;
    FHCNumuCCOth_Cos_Bin[ 7] =  0.94;
    FHCNumuCCOth_Cos_Bin[ 8] =  0.96;
    FHCNumuCCOth_Cos_Bin[ 9] =  0.98;
    FHCNumuCCOth_Cos_Bin[10] =  0.99;
    FHCNumuCCOth_Cos_Bin[11] =  1.0;

    FHCNumuCCOth_Det_Cos_NBin = 8;
    FHCNumuCCOth_Det_Cos_Bin[ 0] = -1.0;
    FHCNumuCCOth_Det_Cos_Bin[ 1] =  0.8;
    FHCNumuCCOth_Det_Cos_Bin[ 2] =  0.85;
    FHCNumuCCOth_Det_Cos_Bin[ 3] =  0.9;
    FHCNumuCCOth_Det_Cos_Bin[ 4] =  0.92;
    FHCNumuCCOth_Det_Cos_Bin[ 5] =  0.96;
    FHCNumuCCOth_Det_Cos_Bin[ 6] =  0.98;
    FHCNumuCCOth_Det_Cos_Bin[ 7] =  0.99;
    FHCNumuCCOth_Det_Cos_Bin[ 8] =  1.0;
  }else{
    FHCNumuCCOth_Cos_NBin = 19;       FHCNumuCCOth_Det_Cos_NBin = 19;
    FHCNumuCCOth_Cos_Bin[ 0] = -1.0;  FHCNumuCCOth_Det_Cos_Bin[ 0] = -1.0;
    FHCNumuCCOth_Cos_Bin[ 1] = -0.6;  FHCNumuCCOth_Det_Cos_Bin[ 1] = -0.6;
    FHCNumuCCOth_Cos_Bin[ 2] = -0.4;  FHCNumuCCOth_Det_Cos_Bin[ 2] = -0.4;
    FHCNumuCCOth_Cos_Bin[ 3] = -0.2;  FHCNumuCCOth_Det_Cos_Bin[ 3] = -0.2;
    FHCNumuCCOth_Cos_Bin[ 4] = -0.1;  FHCNumuCCOth_Det_Cos_Bin[ 4] = -0.1;
    FHCNumuCCOth_Cos_Bin[ 5] =  0.;   FHCNumuCCOth_Det_Cos_Bin[ 5] =  0.;
    FHCNumuCCOth_Cos_Bin[ 6] =  0.1;  FHCNumuCCOth_Det_Cos_Bin[ 6] =  0.1;
    FHCNumuCCOth_Cos_Bin[ 7] =  0.2;  FHCNumuCCOth_Det_Cos_Bin[ 7] =  0.2;
    FHCNumuCCOth_Cos_Bin[ 8] =  0.4;  FHCNumuCCOth_Det_Cos_Bin[ 8] =  0.4;
    FHCNumuCCOth_Cos_Bin[ 9] =  0.6;  FHCNumuCCOth_Det_Cos_Bin[ 9] =  0.6;
    FHCNumuCCOth_Cos_Bin[10] =  0.7;  FHCNumuCCOth_Det_Cos_Bin[10] =  0.7;
    FHCNumuCCOth_Cos_Bin[11] =  0.8;  FHCNumuCCOth_Det_Cos_Bin[11] =  0.8;
    FHCNumuCCOth_Cos_Bin[12] =  0.85; FHCNumuCCOth_Det_Cos_Bin[12] =  0.85;
    FHCNumuCCOth_Cos_Bin[13] =  0.90; FHCNumuCCOth_Det_Cos_Bin[13] =  0.90;
    FHCNumuCCOth_Cos_Bin[14] =  0.92; FHCNumuCCOth_Det_Cos_Bin[14] =  0.92;
    FHCNumuCCOth_Cos_Bin[15] =  0.94; FHCNumuCCOth_Det_Cos_Bin[15] =  0.94;
    FHCNumuCCOth_Cos_Bin[16] =  0.96; FHCNumuCCOth_Det_Cos_Bin[16] =  0.96;
    FHCNumuCCOth_Cos_Bin[17] =  0.98; FHCNumuCCOth_Det_Cos_Bin[17] =  0.98;
    FHCNumuCCOth_Cos_Bin[18] =  0.99; FHCNumuCCOth_Det_Cos_Bin[18] =  0.99;
    FHCNumuCCOth_Cos_Bin[19] =  1.0;  FHCNumuCCOth_Det_Cos_Bin[19] =  1.0;
  }
  if(Do1DCheckMom){FHCNumuCCOth_Det_Cos_NBin=1; FHCNumuCCOth_Det_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){FHCNumuCCOth_Det_Mom_NBin=1; FHCNumuCCOth_Det_Mom_Bin[1] = 30000;}

  //RHCANumuCC1Tr
  int    RHCANumuCC1Tr_Mom_NBin = 10;
  double RHCANumuCC1Tr_Mom_Bin[11] = {0., 400., 500., 600., 700., 800., 900., 1100., 1400., 2000., 10000.};
  int    RHCANumuCC1Tr_Det_Mom_NBin = 5;
  double RHCANumuCC1Tr_Det_Mom_Bin[6] = { 0., 400., 900., 1100., 2000., 10000.};

  int    RHCANumuCC1Tr_Cos_NBin = 13;
  double RHCANumuCC1Tr_Cos_Bin[14] = {-1., 0.6, 0.7, 0.8, 0.85, 0.88, 0.91, 0.93, 0.95, 0.96, 0.97, 0.98, 0.99, 1.00};
  int    RHCANumuCC1Tr_Det_Cos_NBin = 8;
  double RHCANumuCC1Tr_Det_Cos_Bin[9] = { -1., 0.6, 0.7, 0.88, 0.95, 0.97, 0.98, 0.99, 1.00};

  if(Do1DCheckMom){RHCANumuCC1Tr_Det_Cos_NBin=1; RHCANumuCC1Tr_Det_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCANumuCC1Tr_Det_Mom_NBin=1; RHCANumuCC1Tr_Det_Mom_Bin[1] = 30000;}

  //RHCANumuCCnTr
  int    RHCANumuCCnTr_Mom_NBin = 7;
  double RHCANumuCCnTr_Mom_Bin[8] = {0., 700., 950., 1200., 1500., 2000., 3000., 10000.};
  int    RHCANumuCCnTr_Det_Mom_NBin = 6;
  double RHCANumuCCnTr_Det_Mom_Bin[7] = {0., 700., 1200., 1500., 2000., 3000., 10000.};

  int    RHCANumuCCnTr_Cos_NBin = 11;
  double RHCANumuCCnTr_Cos_Bin[12] = {-1., 0.75, 0.85, 0.88, 0.91, 0.93, 0.95, 0.96, 0.97, 0.98, 0.99, 1.00};
  int    RHCANumuCCnTr_Det_Cos_NBin = 6;
  double RHCANumuCCnTr_Det_Cos_Bin[7] = {-1., 0.85, 0.88, 0.93, 0.98, 0.99, 1.00};

  if(Do1DCheckMom){RHCANumuCCnTr_Det_Cos_NBin=1; RHCANumuCCnTr_Det_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCANumuCCnTr_Det_Mom_NBin=1; RHCANumuCCnTr_Det_Mom_Bin[1] = 30000;}

  //RHCNumuCC1Tr
  int    RHCNumuCC1Tr_Mom_NBin = 6;
  double RHCNumuCC1Tr_Mom_Bin[7] = {0., 400., 600., 800., 1100., 2000., 10000.};
  int    RHCNumuCC1Tr_Det_Mom_NBin = 5;
  double RHCNumuCC1Tr_Det_Mom_Bin[6] = {0., 400., 800., 1100., 2000., 10000.};

  int    RHCNumuCC1Tr_Cos_NBin = 11;
  double RHCNumuCC1Tr_Cos_Bin[12] = {-1., 0.7, 0.8, 0.85, 0.90, 0.93, 0.95, 0.96, 0.97, 0.98, 0.99, 1.00};
  int    RHCNumuCC1Tr_Det_Cos_NBin = 8;
  double RHCNumuCC1Tr_Det_Cos_Bin[9] = {-1., 0.7, 0.85, 0.90, 0.93, 0.96, 0.98, 0.99, 1.00};

  if(Do1DCheckMom){RHCNumuCC1Tr_Det_Cos_NBin=1; RHCNumuCC1Tr_Det_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCNumuCC1Tr_Det_Mom_NBin=1; RHCNumuCC1Tr_Det_Mom_Bin[1] = 30000;}


  //RHCNumuCCnTr
  int    RHCNumuCCnTr_Mom_NBin = 8;
  double RHCNumuCCnTr_Mom_Bin[9] = {0., 500., 700., 1000., 1250., 1500., 2000., 3000., 10000.};
  int    RHCNumuCCnTr_Det_Mom_NBin = 5;
  double RHCNumuCCnTr_Det_Mom_Bin[6] = {0., 1000., 1500., 2000., 3000., 10000.};

  int    RHCNumuCCnTr_Cos_NBin = 11;
  double RHCNumuCCnTr_Cos_Bin[12] = {-1., 0.7, 0.8, 0.85, 0.90, 0.93, 0.95, 0.96, 0.97, 0.98, 0.99, 1.00};
  int    RHCNumuCCnTr_Det_Cos_NBin = 8;
  double RHCNumuCCnTr_Det_Cos_Bin[9] = {-1., 0.8, 0.90, 0.93, 0.95, 0.96, 0.97, 0.99, 1.00};

  if(Do1DCheckMom){RHCNumuCCnTr_Det_Cos_NBin=1; RHCNumuCCnTr_Det_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCNumuCCnTr_Det_Mom_NBin=1; RHCNumuCCnTr_Det_Mom_Bin[1] = 30000;}

  //RHCANumuCC0Pi
  int    RHCANumuCC0Pi_Mom_NBin = 13;
  double RHCANumuCC0Pi_Mom_Bin[14] = {0., 300., 400., 500., 600., 700., 800., 900., 1000., 1250., 1500., 2000., 4000., 30000.};

  int    RHCANumuCC0Pi_Cos_NBin = 10;
  double RHCANumuCC0Pi_Cos_Bin[11] = {-1., 0.7, 0.8, 0.85, 0.9, 0.94, 0.96, 0.98, 0.99, 0.995, 1.};

  if(Do1DCheckMom){RHCANumuCC0Pi_Cos_NBin=1; RHCANumuCC0Pi_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCANumuCC0Pi_Mom_NBin=1; RHCANumuCC0Pi_Mom_Bin[1] = 30000;}

  //RHCANumuCC1Pi
  int    RHCANumuCC1Pi_Mom_NBin = 8;
  double RHCANumuCC1Pi_Mom_Bin[9] = {0., 400., 600., 800., 1000., 1250., 1500., 2500., 30000.};

  int    RHCANumuCC1Pi_Cos_NBin = 4;
  double RHCANumuCC1Pi_Cos_Bin[5] = {-1, 0.8, 0.9, 0.96, 1};

  if(Do1DCheckMom){RHCANumuCC1Pi_Cos_NBin=1; RHCANumuCC1Pi_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCANumuCC1Pi_Mom_NBin=1; RHCANumuCC1Pi_Mom_Bin[1] = 30000;}

  //RHCANumuCCOth
  int    RHCANumuCCOth_Mom_NBin = 12;
  double RHCANumuCCOth_Mom_Bin[13] = {0., 350., 500., 600., 700., 800., 900., 1000., 1250., 1500., 2000., 4000., 30000.};

  int    RHCANumuCCOth_Cos_NBin = 5;
  double RHCANumuCCOth_Cos_Bin[6] = {-1., 0.8, 0.9, 0.95, 0.98, 1.};

  if(Do1DCheckMom){RHCANumuCCOth_Cos_NBin=1; RHCANumuCCOth_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCANumuCCOth_Mom_NBin=1; RHCANumuCCOth_Mom_Bin[1] = 30000;}

  //RHCNumuCC0Pi
  int    RHCNumuCC0Pi_Mom_NBin = 11;
  double RHCNumuCC0Pi_Mom_Bin[12] = {0., 350., 500., 650., 800., 900., 1000., 1250., 1500., 2000., 4000., 30000.};

  int    RHCNumuCC0Pi_Cos_NBin = 8;
  double RHCNumuCC0Pi_Cos_Bin[9] = {-1, 0.75, 0.85, 0.9, 0.92, 0.94, 0.96, 0.98, 1};

  if(Do1DCheckMom){RHCNumuCC0Pi_Cos_NBin=1; RHCNumuCC0Pi_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCNumuCC0Pi_Mom_NBin=1; RHCNumuCC0Pi_Mom_Bin[1] = 30000;}

  //RHCNumuCC1Pi
  int    RHCNumuCC1Pi_Mom_NBin =  8;
  double RHCNumuCC1Pi_Mom_Bin[9] = {0., 350., 500., 650., 800., 1000., 1250., 2000., 30000.};

  int    RHCNumuCC1Pi_Cos_NBin = 4;
  double RHCNumuCC1Pi_Cos_Bin[5] = {-1, 0.8, 0.9, 0.95, 1.};

  if(Do1DCheckMom){RHCNumuCC1Pi_Cos_NBin=1; RHCNumuCC1Pi_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCNumuCC1Pi_Mom_NBin=1; RHCNumuCC1Pi_Mom_Bin[1] = 30000;}

  //RHCNumuCCOth
  int    RHCNumuCCOth_Mom_NBin =  10;
  double RHCNumuCCOth_Mom_Bin[11] = {0., 300., 450., 600., 800., 1000., 1250., 1500., 2000., 5000., 30000.};

  int    RHCNumuCCOth_Cos_NBin = 8;
  double RHCNumuCCOth_Cos_Bin[9] = {-1., 0.7, 0.8, 0.86, 0.92, 0.94, 0.96, 0.98, 1.};

  if(Do1DCheckMom){RHCNumuCCOth_Cos_NBin=1; RHCNumuCCOth_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCNumuCCOth_Mom_NBin=1; RHCNumuCCOth_Mom_Bin[1] = 30000;}

  //FHC nue
  int    FHCNueCC_Mom_NBin = 6;
  double FHCNueCC_Mom_Bin[7] = {200., 400., 600., 800., 1000., 1500., 30000.};

  int    FHCNueCC_Cos_NBin = 4;
  double FHCNueCC_Cos_Bin[5] = {-1., 0.8, 0.9, 0.95, 1.};

  if(Do1DCheckMom){FHCNueCC_Cos_NBin=1; FHCNueCC_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){FHCNueCC_Mom_NBin=1; FHCNueCC_Mom_Bin[1] = 30000;}

  //FHC anue
  int    FHCANueCC_Mom_NBin = 4;
  double FHCANueCC_Mom_Bin[5] = {200., 300., 400., 1000., 30000.};

  int    FHCANueCC_Cos_NBin = 1;
  double FHCANueCC_Cos_Bin[2] = {-1., 1.};

  if(Do1DCheckMom){FHCANueCC_Cos_NBin=1; FHCANueCC_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){FHCANueCC_Mom_NBin=1; FHCANueCC_Mom_Bin[1] = 30000;}

  //FHC gamma
  int    FHCGamma_Mom_NBin = 5;
  double FHCGamma_Mom_Bin[6] = {200., 300., 400., 600., 1000., 30000.};

  int    FHCGamma_Cos_NBin = 1;
  double FHCGamma_Cos_Bin[2] = {-1., 1.};

  if(Do1DCheckMom){FHCGamma_Cos_NBin=1; FHCGamma_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){FHCGamma_Mom_NBin=1; FHCGamma_Mom_Bin[1] = 30000;}

  //RHC nue
  int    RHCNueCC_Mom_NBin = 6;
  double RHCNueCC_Mom_Bin[7] = {200., 400., 600., 800., 1000., 2000., 30000.};

  int    RHCNueCC_Cos_NBin = 2;
  double RHCNueCC_Cos_Bin[3] = {-1., 0.9, 1.};

  if(Do1DCheckMom){RHCNueCC_Cos_NBin=1; RHCNueCC_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCNueCC_Mom_NBin=1; RHCNueCC_Mom_Bin[1] = 30000;}

  //RHC anue
  int    RHCANueCC_Mom_NBin = 3;
  double RHCANueCC_Mom_Bin[4] = {200., 500., 1000., 30000.};

  int    RHCANueCC_Cos_NBin = 2;
  double RHCANueCC_Cos_Bin[3] = {-1., 0.9, 1.};

  if(Do1DCheckMom){RHCANueCC_Cos_NBin=1; RHCANueCC_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCANueCC_Mom_NBin=1; RHCANueCC_Mom_Bin[1] = 30000;}

  //RHC gamma
  int    RHCGamma_Mom_NBin = 5;
  double RHCGamma_Mom_Bin[6] = {200., 400., 600., 800., 1000., 30000.};

  int    RHCGamma_Cos_NBin = 1;
  double RHCGamma_Cos_Bin[2] = {-1., 1.};

  if(Do1DCheckMom){RHCGamma_Cos_NBin=1; RHCGamma_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCGamma_Mom_NBin=1; RHCGamma_Mom_Bin[1] = 30000;}

  //Added by MH
  if(DoOnlyP0D)
  {

    // P0D numu FHC
    bothAxis    [SampleId::kP0DWaterNuMuCC] = new TAxis2D(P0DFHCNumuCCInclusive_Mom_NBin,     P0DFHCNumuCCInclusive_Mom_Bin,
                                                          P0DFHCNumuCCInclusive_Cos_NBin,     P0DFHCNumuCCInclusive_Cos_Bin);
    bothAxis_Det[SampleId::kP0DWaterNuMuCC] = new TAxis2D(P0DFHCNumuCCInclusive_Det_Mom_NBin, P0DFHCNumuCCInclusive_Det_Mom_Bin,
                                                          P0DFHCNumuCCInclusive_Det_Cos_NBin, P0DFHCNumuCCInclusive_Det_Cos_Bin);

    bothAxis    [SampleId::kP0DAirNuMuCC] =   new TAxis2D(P0DFHCNumuCCInclusive_Mom_NBin,     P0DFHCNumuCCInclusive_Mom_Bin,
                                                          P0DFHCNumuCCInclusive_Cos_NBin,     P0DFHCNumuCCInclusive_Cos_Bin);
    bothAxis_Det[SampleId::kP0DAirNuMuCC] =   new TAxis2D(P0DFHCNumuCCInclusive_Det_Mom_NBin, P0DFHCNumuCCInclusive_Det_Mom_Bin,
                                                          P0DFHCNumuCCInclusive_Det_Cos_NBin, P0DFHCNumuCCInclusive_Det_Cos_Bin);

    // P0D numu BKG in RHC
    bothAxis    [SampleId::kP0DWaterNuMuBkgInAntiNuModeCC] = new TAxis2D(P0DRHCNumuBkgCCInclusive_Mom_NBin,
                                                                         P0DRHCNumuBkgCCInclusive_Mom_Bin,
                                                                         P0DRHCNumuBkgCCInclusive_Cos_NBin,
                                                                         P0DRHCNumuBkgCCInclusive_Cos_Bin);
    bothAxis_Det[SampleId::kP0DWaterNuMuBkgInAntiNuModeCC] = new TAxis2D(P0DRHCNumuBkgCCInclusive_Det_Mom_NBin,
                                                                         P0DRHCNumuBkgCCInclusive_Det_Mom_Bin,
                                                                         P0DRHCNumuBkgCCInclusive_Det_Cos_NBin,
                                                                         P0DRHCNumuBkgCCInclusive_Det_Cos_Bin);
    bothAxis    [SampleId::kP0DAirNuMuBkgInAntiNuModeCC] =   new TAxis2D(P0DRHCNumuBkgCCInclusive_Mom_NBin,
                                                                         P0DRHCNumuBkgCCInclusive_Mom_Bin,
                                                                         P0DRHCNumuBkgCCInclusive_Cos_NBin,
                                                                         P0DRHCNumuBkgCCInclusive_Cos_Bin);
    bothAxis_Det[SampleId::kP0DAirNuMuBkgInAntiNuModeCC] =   new TAxis2D(P0DRHCNumuBkgCCInclusive_Det_Mom_NBin,
                                                                         P0DRHCNumuBkgCCInclusive_Det_Mom_Bin,
                                                                         P0DRHCNumuBkgCCInclusive_Det_Cos_NBin,
                                                                         P0DRHCNumuBkgCCInclusive_Det_Cos_Bin);

    // P0D numubar in RHC
    bothAxis    [SampleId::kP0DWaterNuMuBarInAntiNuModeCC] = new TAxis2D(P0DRHCNumubarCCInclusive_Mom_NBin,
                                                                         P0DRHCNumubarCCInclusive_Mom_Bin,
                                                                         P0DRHCNumubarCCInclusive_Cos_NBin,
                                                                         P0DRHCNumubarCCInclusive_Cos_Bin);
    bothAxis_Det[SampleId::kP0DWaterNuMuBarInAntiNuModeCC] = new TAxis2D(P0DRHCNumubarCCInclusive_Det_Mom_NBin,
                                                                         P0DRHCNumubarCCInclusive_Det_Mom_Bin,
                                                                         P0DRHCNumubarCCInclusive_Det_Cos_NBin,
                                                                         P0DRHCNumubarCCInclusive_Det_Cos_Bin);
    bothAxis    [SampleId::kP0DAirNuMuBarInAntiNuModeCC] =   new TAxis2D(P0DRHCNumubarCCInclusive_Mom_NBin,
                                                                         P0DRHCNumubarCCInclusive_Mom_Bin,
                                                                         P0DRHCNumubarCCInclusive_Cos_NBin,
                                                                         P0DRHCNumubarCCInclusive_Cos_Bin);
    bothAxis_Det[SampleId::kP0DAirNuMuBarInAntiNuModeCC] =   new TAxis2D(P0DRHCNumubarCCInclusive_Det_Mom_NBin,
                                                                         P0DRHCNumubarCCInclusive_Det_Mom_Bin,
                                                                         P0DRHCNumubarCCInclusive_Det_Cos_NBin,
                                                                         P0DRHCNumubarCCInclusive_Det_Cos_Bin);
  }

  if(!DoOnlyNue){
    bothAxis    [SampleId::kFGD1NuMuCC0Pi] = new TAxis2D(FHCNumuCC0Pi_Mom_NBin    , FHCNumuCC0Pi_Mom_Bin,
                                                         FHCNumuCC0Pi_Cos_NBin    , FHCNumuCC0Pi_Cos_Bin);
    bothAxis_Det[SampleId::kFGD1NuMuCC0Pi] = new TAxis2D(FHCNumuCC0Pi_Det_Mom_NBin, FHCNumuCC0Pi_Det_Mom_Bin,
                                                         FHCNumuCC0Pi_Det_Cos_NBin, FHCNumuCC0Pi_Det_Cos_Bin);

    bothAxis    [SampleId::kFGD1NuMuCC1Pi] = new TAxis2D(FHCNumuCC1Pi_Mom_NBin    , FHCNumuCC1Pi_Mom_Bin,
                                                         FHCNumuCC1Pi_Cos_NBin    , FHCNumuCC1Pi_Cos_Bin);
    bothAxis_Det[SampleId::kFGD1NuMuCC1Pi] = new TAxis2D(FHCNumuCC1Pi_Det_Mom_NBin, FHCNumuCC1Pi_Det_Mom_Bin,
                                                         FHCNumuCC1Pi_Det_Cos_NBin, FHCNumuCC1Pi_Det_Cos_Bin);

    bothAxis    [SampleId::kFGD1NuMuCCOther] = new TAxis2D(FHCNumuCCOth_Mom_NBin    , FHCNumuCCOth_Mom_Bin,
                                                           FHCNumuCCOth_Cos_NBin    , FHCNumuCCOth_Cos_Bin);
    bothAxis_Det[SampleId::kFGD1NuMuCCOther] = new TAxis2D(FHCNumuCCOth_Det_Mom_NBin, FHCNumuCCOth_Det_Mom_Bin,
                                                           FHCNumuCCOth_Det_Cos_NBin, FHCNumuCCOth_Det_Cos_Bin);


    if(!DoMultiPiRHC){
      bothAxis    [SampleId::kFGD1AntiNuMuCC1Track] = new TAxis2D(RHCANumuCC1Tr_Mom_NBin    , RHCANumuCC1Tr_Mom_Bin,
                                                                  RHCANumuCC1Tr_Cos_NBin    , RHCANumuCC1Tr_Cos_Bin);
      bothAxis_Det[SampleId::kFGD1AntiNuMuCC1Track] = new TAxis2D(RHCANumuCC1Tr_Det_Mom_NBin, RHCANumuCC1Tr_Det_Mom_Bin,
                                                                  RHCANumuCC1Tr_Det_Cos_NBin, RHCANumuCC1Tr_Det_Cos_Bin);

      bothAxis    [SampleId::kFGD1AntiNuMuCCNTracks] = new TAxis2D(RHCANumuCCnTr_Mom_NBin    , RHCANumuCCnTr_Mom_Bin,
                                                                   RHCANumuCCnTr_Cos_NBin    , RHCANumuCCnTr_Cos_Bin);
      bothAxis_Det[SampleId::kFGD1AntiNuMuCCNTracks] = new TAxis2D(RHCANumuCCnTr_Det_Mom_NBin, RHCANumuCCnTr_Det_Mom_Bin,
                                                                   RHCANumuCCnTr_Det_Cos_NBin, RHCANumuCCnTr_Det_Cos_Bin);



      bothAxis    [SampleId::kFGD1NuMuBkgInAntiNuModeCC1Track] = new TAxis2D(RHCNumuCC1Tr_Mom_NBin     , RHCNumuCC1Tr_Mom_Bin,
                                                                             RHCNumuCC1Tr_Cos_NBin     , RHCNumuCC1Tr_Cos_Bin);
      bothAxis_Det[SampleId::kFGD1NuMuBkgInAntiNuModeCC1Track] = new TAxis2D(RHCNumuCC1Tr_Det_Mom_NBin , RHCNumuCC1Tr_Det_Mom_Bin,
                                                                             RHCNumuCC1Tr_Det_Cos_NBin , RHCNumuCC1Tr_Det_Cos_Bin);

      bothAxis    [SampleId::kFGD1NuMuBkgInAntiNuModeCCNTracks] = new TAxis2D(RHCNumuCCnTr_Mom_NBin     , RHCNumuCCnTr_Mom_Bin,
                                                                              RHCNumuCCnTr_Cos_NBin     , RHCNumuCCnTr_Cos_Bin);
      bothAxis_Det[SampleId::kFGD1NuMuBkgInAntiNuModeCCNTracks] = new TAxis2D(RHCNumuCCnTr_Det_Mom_NBin , RHCNumuCCnTr_Det_Mom_Bin,
                                                                              RHCNumuCCnTr_Det_Cos_NBin , RHCNumuCCnTr_Det_Cos_Bin);

    }else{

      bothAxis    [SampleId::kFGD1AntiNuMuCC0Pi] = new TAxis2D(RHCANumuCC0Pi_Mom_NBin    , RHCANumuCC0Pi_Mom_Bin,
                                                               RHCANumuCC0Pi_Cos_NBin    , RHCANumuCC0Pi_Cos_Bin);
      bothAxis_Det[SampleId::kFGD1AntiNuMuCC0Pi] = new TAxis2D(RHCANumuCC0Pi_Mom_NBin, RHCANumuCC0Pi_Mom_Bin,
                                                               RHCANumuCC0Pi_Cos_NBin, RHCANumuCC0Pi_Cos_Bin);

      bothAxis    [SampleId::kFGD1AntiNuMuCC1Pi] = new TAxis2D(RHCANumuCC1Pi_Mom_NBin    , RHCANumuCC1Pi_Mom_Bin,
                                                               RHCANumuCC1Pi_Cos_NBin    , RHCANumuCC1Pi_Cos_Bin);
      bothAxis_Det[SampleId::kFGD1AntiNuMuCC1Pi] = new TAxis2D(RHCANumuCC1Pi_Mom_NBin, RHCANumuCC1Pi_Mom_Bin,
                                                               RHCANumuCC1Pi_Cos_NBin, RHCANumuCC1Pi_Cos_Bin);

      bothAxis    [SampleId::kFGD1AntiNuMuCCOther] = new TAxis2D(RHCANumuCCOth_Mom_NBin    , RHCANumuCCOth_Mom_Bin,
                                                                 RHCANumuCCOth_Cos_NBin    , RHCANumuCCOth_Cos_Bin);
      bothAxis_Det[SampleId::kFGD1AntiNuMuCCOther] = new TAxis2D(RHCANumuCCOth_Mom_NBin, RHCANumuCCOth_Mom_Bin,
                                                                 RHCANumuCCOth_Cos_NBin, RHCANumuCCOth_Cos_Bin);



      bothAxis    [SampleId::kFGD1NuMuBkgInAntiNuModeCC0Pi] = new TAxis2D(RHCNumuCC0Pi_Mom_NBin    , RHCNumuCC0Pi_Mom_Bin,
                                                                          RHCNumuCC0Pi_Cos_NBin    , RHCNumuCC0Pi_Cos_Bin);
      bothAxis_Det[SampleId::kFGD1NuMuBkgInAntiNuModeCC0Pi] = new TAxis2D(RHCNumuCC0Pi_Mom_NBin, RHCNumuCC0Pi_Mom_Bin,
                                                                          RHCNumuCC0Pi_Cos_NBin, RHCNumuCC0Pi_Cos_Bin);

      bothAxis    [SampleId::kFGD1NuMuBkgInAntiNuModeCC1Pi] = new TAxis2D(RHCNumuCC1Pi_Mom_NBin    , RHCNumuCC1Pi_Mom_Bin,
                                                                          RHCNumuCC1Pi_Cos_NBin    , RHCNumuCC1Pi_Cos_Bin);
      bothAxis_Det[SampleId::kFGD1NuMuBkgInAntiNuModeCC1Pi] = new TAxis2D(RHCNumuCC1Pi_Mom_NBin, RHCNumuCC1Pi_Mom_Bin,
                                                                          RHCNumuCC1Pi_Cos_NBin, RHCNumuCC1Pi_Cos_Bin);

      bothAxis    [SampleId::kFGD1NuMuBkgInAntiNuModeCCOther] = new TAxis2D(RHCNumuCCOth_Mom_NBin    , RHCNumuCCOth_Mom_Bin,
                                                                            RHCNumuCCOth_Cos_NBin    , RHCNumuCCOth_Cos_Bin);
      bothAxis_Det[SampleId::kFGD1NuMuBkgInAntiNuModeCCOther] = new TAxis2D(RHCNumuCCOth_Mom_NBin, RHCNumuCCOth_Mom_Bin,
                                                                            RHCNumuCCOth_Cos_NBin, RHCNumuCCOth_Cos_Bin);
    }


    bothAxis    [SampleId::kFGD2NuMuCC0Pi] = new TAxis2D(FHCNumuCC0Pi_Mom_NBin    , FHCNumuCC0Pi_Mom_Bin,
                                                         FHCNumuCC0Pi_Cos_NBin    , FHCNumuCC0Pi_Cos_Bin);
    bothAxis_Det[SampleId::kFGD2NuMuCC0Pi] = new TAxis2D(FHCNumuCC0Pi_Det_Mom_NBin, FHCNumuCC0Pi_Det_Mom_Bin,
                                                         FHCNumuCC0Pi_Det_Cos_NBin, FHCNumuCC0Pi_Det_Cos_Bin);

    bothAxis    [SampleId::kFGD2NuMuCC1Pi] = new TAxis2D(FHCNumuCC1Pi_Mom_NBin    , FHCNumuCC1Pi_Mom_Bin,
                                                         FHCNumuCC1Pi_Cos_NBin    , FHCNumuCC1Pi_Cos_Bin);
    bothAxis_Det[SampleId::kFGD2NuMuCC1Pi] = new TAxis2D(FHCNumuCC1Pi_Det_Mom_NBin, FHCNumuCC1Pi_Det_Mom_Bin,
                                                         FHCNumuCC1Pi_Det_Cos_NBin, FHCNumuCC1Pi_Det_Cos_Bin);

    bothAxis    [SampleId::kFGD2NuMuCCOther] = new TAxis2D(FHCNumuCCOth_Mom_NBin    , FHCNumuCCOth_Mom_Bin,
                                                           FHCNumuCCOth_Cos_NBin    , FHCNumuCCOth_Cos_Bin);
    bothAxis_Det[SampleId::kFGD2NuMuCCOther] = new TAxis2D(FHCNumuCCOth_Det_Mom_NBin, FHCNumuCCOth_Det_Mom_Bin,
                                                           FHCNumuCCOth_Det_Cos_NBin, FHCNumuCCOth_Det_Cos_Bin);


    if(!DoMultiPiRHC){

      bothAxis    [SampleId::kFGD2AntiNuMuCC1Track] = new TAxis2D(RHCANumuCC1Tr_Mom_NBin    , RHCANumuCC1Tr_Mom_Bin,
                                                                  RHCANumuCC1Tr_Cos_NBin    , RHCANumuCC1Tr_Cos_Bin);
      bothAxis_Det[SampleId::kFGD2AntiNuMuCC1Track] = new TAxis2D(RHCANumuCC1Tr_Det_Mom_NBin, RHCANumuCC1Tr_Det_Mom_Bin,
                                                                  RHCANumuCC1Tr_Det_Cos_NBin, RHCANumuCC1Tr_Det_Cos_Bin);

      bothAxis    [SampleId::kFGD2AntiNuMuCCNTracks] = new TAxis2D(RHCANumuCCnTr_Mom_NBin    , RHCANumuCCnTr_Mom_Bin,
                                                                   RHCANumuCCnTr_Cos_NBin    , RHCANumuCCnTr_Cos_Bin);
      bothAxis_Det[SampleId::kFGD2AntiNuMuCCNTracks] = new TAxis2D(RHCANumuCCnTr_Det_Mom_NBin, RHCANumuCCnTr_Det_Mom_Bin,
                                                                   RHCANumuCCnTr_Det_Cos_NBin, RHCANumuCCnTr_Det_Cos_Bin);



      bothAxis    [SampleId::kFGD2NuMuBkgInAntiNuModeCC1Track] = new TAxis2D(RHCNumuCC1Tr_Mom_NBin     , RHCNumuCC1Tr_Mom_Bin,
                                                                             RHCNumuCC1Tr_Cos_NBin     , RHCNumuCC1Tr_Cos_Bin);
      bothAxis_Det[SampleId::kFGD2NuMuBkgInAntiNuModeCC1Track] = new TAxis2D(RHCNumuCC1Tr_Det_Mom_NBin , RHCNumuCC1Tr_Det_Mom_Bin,
                                                                             RHCNumuCC1Tr_Det_Cos_NBin , RHCNumuCC1Tr_Det_Cos_Bin);

      bothAxis    [SampleId::kFGD2NuMuBkgInAntiNuModeCCNTracks] = new TAxis2D(RHCNumuCCnTr_Mom_NBin     , RHCNumuCCnTr_Mom_Bin,
                                                                              RHCNumuCCnTr_Cos_NBin     , RHCNumuCCnTr_Cos_Bin);
      bothAxis_Det[SampleId::kFGD2NuMuBkgInAntiNuModeCCNTracks] = new TAxis2D(RHCNumuCCnTr_Det_Mom_NBin , RHCNumuCCnTr_Det_Mom_Bin,
                                                                              RHCNumuCCnTr_Det_Cos_NBin , RHCNumuCCnTr_Det_Cos_Bin);
    }else{


      bothAxis    [SampleId::kFGD2AntiNuMuCC0Pi] = new TAxis2D(RHCANumuCC0Pi_Mom_NBin    , RHCANumuCC0Pi_Mom_Bin,
                                                               RHCANumuCC0Pi_Cos_NBin    , RHCANumuCC0Pi_Cos_Bin);
      bothAxis_Det[SampleId::kFGD2AntiNuMuCC0Pi] = new TAxis2D(RHCANumuCC0Pi_Mom_NBin, RHCANumuCC0Pi_Mom_Bin,
                                                               RHCANumuCC0Pi_Cos_NBin, RHCANumuCC0Pi_Cos_Bin);

      bothAxis    [SampleId::kFGD2AntiNuMuCC1Pi] = new TAxis2D(RHCANumuCC1Pi_Mom_NBin    , RHCANumuCC1Pi_Mom_Bin,
                                                               RHCANumuCC1Pi_Cos_NBin    , RHCANumuCC1Pi_Cos_Bin);
      bothAxis_Det[SampleId::kFGD2AntiNuMuCC1Pi] = new TAxis2D(RHCANumuCC1Pi_Mom_NBin, RHCANumuCC1Pi_Mom_Bin,
                                                               RHCANumuCC1Pi_Cos_NBin, RHCANumuCC1Pi_Cos_Bin);

      bothAxis    [SampleId::kFGD2AntiNuMuCCOther] = new TAxis2D(RHCANumuCCOth_Mom_NBin    , RHCANumuCCOth_Mom_Bin,
                                                                 RHCANumuCCOth_Cos_NBin    , RHCANumuCCOth_Cos_Bin);
      bothAxis_Det[SampleId::kFGD2AntiNuMuCCOther] = new TAxis2D(RHCANumuCCOth_Mom_NBin, RHCANumuCCOth_Mom_Bin,
                                                                 RHCANumuCCOth_Cos_NBin, RHCANumuCCOth_Cos_Bin);



      bothAxis    [SampleId::kFGD2NuMuBkgInAntiNuModeCC0Pi] = new TAxis2D(RHCNumuCC0Pi_Mom_NBin    , RHCNumuCC0Pi_Mom_Bin,
                                                                          RHCNumuCC0Pi_Cos_NBin    , RHCNumuCC0Pi_Cos_Bin);
      bothAxis_Det[SampleId::kFGD2NuMuBkgInAntiNuModeCC0Pi] = new TAxis2D(RHCNumuCC0Pi_Mom_NBin, RHCNumuCC0Pi_Mom_Bin,
                                                                          RHCNumuCC0Pi_Cos_NBin, RHCNumuCC0Pi_Cos_Bin);

      bothAxis    [SampleId::kFGD2NuMuBkgInAntiNuModeCC1Pi] = new TAxis2D(RHCNumuCC1Pi_Mom_NBin    , RHCNumuCC1Pi_Mom_Bin,
                                                                          RHCNumuCC1Pi_Cos_NBin    , RHCNumuCC1Pi_Cos_Bin);
      bothAxis_Det[SampleId::kFGD2NuMuBkgInAntiNuModeCC1Pi] = new TAxis2D(RHCNumuCC1Pi_Mom_NBin, RHCNumuCC1Pi_Mom_Bin,
                                                                          RHCNumuCC1Pi_Cos_NBin, RHCNumuCC1Pi_Cos_Bin);

      bothAxis    [SampleId::kFGD2NuMuBkgInAntiNuModeCCOther] = new TAxis2D(RHCNumuCCOth_Mom_NBin    , RHCNumuCCOth_Mom_Bin,
                                                                            RHCNumuCCOth_Cos_NBin    , RHCNumuCCOth_Cos_Bin);
      bothAxis_Det[SampleId::kFGD2NuMuBkgInAntiNuModeCCOther] = new TAxis2D(RHCNumuCCOth_Mom_NBin, RHCNumuCCOth_Mom_Bin,
                                                                            RHCNumuCCOth_Cos_NBin, RHCNumuCCOth_Cos_Bin);
    }
  }

  if(DoNue){
    bothAxis    [SampleId::kFGD1NuECC] = new TAxis2D(FHCNueCC_Mom_NBin      , FHCNueCC_Mom_Bin,
                                                     FHCNueCC_Cos_NBin      , FHCNueCC_Cos_Bin);
    bothAxis_Det[SampleId::kFGD1NuECC] = new TAxis2D(FHCNueCC_Mom_NBin  , FHCNueCC_Mom_Bin,
                                                     FHCNueCC_Cos_NBin  , FHCNueCC_Cos_Bin);

    bothAxis    [SampleId::kFGD1AntiNuEBkgInNuModeCC] = new TAxis2D(FHCANueCC_Mom_NBin     , FHCANueCC_Mom_Bin,
                                                                    FHCANueCC_Cos_NBin     , FHCANueCC_Cos_Bin);
    bothAxis_Det[SampleId::kFGD1AntiNuEBkgInNuModeCC] = new TAxis2D(FHCANueCC_Mom_NBin , FHCANueCC_Mom_Bin,
                                                                    FHCANueCC_Cos_NBin , FHCANueCC_Cos_Bin);

    bothAxis    [SampleId::kFGD1Gamma] = new TAxis2D(FHCGamma_Mom_NBin      , FHCGamma_Mom_Bin,
                                                     FHCGamma_Cos_NBin      , FHCGamma_Cos_Bin);
    bothAxis_Det[SampleId::kFGD1Gamma] = new TAxis2D(FHCGamma_Mom_NBin  , FHCGamma_Mom_Bin,
                                                     FHCGamma_Cos_NBin  , FHCGamma_Cos_Bin);



    bothAxis    [SampleId::kFGD1NuEBkgInAntiNuModeCC] = new TAxis2D(RHCNueCC_Mom_NBin      , RHCNueCC_Mom_Bin,
                                                                    RHCNueCC_Cos_NBin      , RHCNueCC_Cos_Bin);
    bothAxis_Det[SampleId::kFGD1NuEBkgInAntiNuModeCC] = new TAxis2D(RHCNueCC_Mom_NBin  , RHCNueCC_Mom_Bin,
                                                                    RHCNueCC_Cos_NBin  , RHCNueCC_Cos_Bin);

    bothAxis    [SampleId::kFGD1AntiNuECC] = new TAxis2D(RHCANueCC_Mom_NBin, RHCANueCC_Mom_Bin,
                                                         RHCANueCC_Cos_NBin, RHCANueCC_Cos_Bin);
    bothAxis_Det[SampleId::kFGD1AntiNuECC] = new TAxis2D(RHCANueCC_Mom_NBin, RHCANueCC_Mom_Bin,
                                                         RHCANueCC_Cos_NBin, RHCANueCC_Cos_Bin);

    bothAxis    [SampleId::kFGD1GammaInAntiNuMode] = new TAxis2D(RHCGamma_Mom_NBin, RHCGamma_Mom_Bin,
                                                                 RHCGamma_Cos_NBin, RHCGamma_Cos_Bin);
    bothAxis_Det[SampleId::kFGD1GammaInAntiNuMode] = new TAxis2D(RHCGamma_Mom_NBin, RHCGamma_Mom_Bin,
                                                                 RHCGamma_Cos_NBin, RHCGamma_Cos_Bin);



    bothAxis    [SampleId::kFGD2NuECC] = new TAxis2D(FHCNueCC_Mom_NBin, FHCNueCC_Mom_Bin,
                                                     FHCNueCC_Cos_NBin, FHCNueCC_Cos_Bin);
    bothAxis_Det[SampleId::kFGD2NuECC] = new TAxis2D(FHCNueCC_Mom_NBin, FHCNueCC_Mom_Bin,
                                                     FHCNueCC_Cos_NBin, FHCNueCC_Cos_Bin);

    bothAxis    [SampleId::kFGD2AntiNuEBkgInNuModeCC] = new TAxis2D(FHCANueCC_Mom_NBin, FHCANueCC_Mom_Bin,
                                                                    FHCANueCC_Cos_NBin, FHCANueCC_Cos_Bin);
    bothAxis_Det[SampleId::kFGD2AntiNuEBkgInNuModeCC] = new TAxis2D(FHCANueCC_Mom_NBin, FHCANueCC_Mom_Bin,
                                                                    FHCANueCC_Cos_NBin, FHCANueCC_Cos_Bin);

    bothAxis    [SampleId::kFGD2Gamma] = new TAxis2D(FHCGamma_Mom_NBin, FHCGamma_Mom_Bin,
                                                     FHCGamma_Cos_NBin, FHCGamma_Cos_Bin);
    bothAxis_Det[SampleId::kFGD2Gamma] = new TAxis2D(FHCGamma_Mom_NBin, FHCGamma_Mom_Bin,
                                                     FHCGamma_Cos_NBin, FHCGamma_Cos_Bin);



    bothAxis    [SampleId::kFGD2NuEBkgInAntiNuModeCC] = new TAxis2D(RHCNueCC_Mom_NBin, RHCNueCC_Mom_Bin,
                                                                    RHCNueCC_Cos_NBin, RHCNueCC_Cos_Bin);
    bothAxis_Det[SampleId::kFGD2NuEBkgInAntiNuModeCC] = new TAxis2D(RHCNueCC_Mom_NBin, RHCNueCC_Mom_Bin,
                                                                    RHCNueCC_Cos_NBin, RHCNueCC_Cos_Bin);

    bothAxis    [SampleId::kFGD2AntiNuECC] = new TAxis2D(RHCANueCC_Mom_NBin, RHCANueCC_Mom_Bin,
                                                         RHCANueCC_Cos_NBin, RHCANueCC_Cos_Bin);
    bothAxis_Det[SampleId::kFGD2AntiNuECC] = new TAxis2D(RHCANueCC_Mom_NBin, RHCANueCC_Mom_Bin,
                                                         RHCANueCC_Cos_NBin, RHCANueCC_Cos_Bin);

    bothAxis    [SampleId::kFGD2GammaInAntiNuMode] = new TAxis2D(RHCGamma_Mom_NBin, RHCGamma_Mom_Bin,
                                                                 RHCGamma_Cos_NBin, RHCGamma_Cos_Bin);
    bothAxis_Det[SampleId::kFGD2GammaInAntiNuMode] = new TAxis2D(RHCGamma_Mom_NBin, RHCGamma_Mom_Bin,
                                                                 RHCGamma_Cos_NBin, RHCGamma_Cos_Bin);
  }

  int nBinsFlatMomBinningForNuMuComp = 20;
  double FlatMomBinningForNuMuComp[21];
  for(int i = 0; i < nBinsFlatMomBinningForNuMuComp+1; ++i){
    FlatMomBinningForNuMuComp[i] = 250*i;
  }

  if(UseFlatNuMuBinning){
    int nBinsFlatCosBinningForNuMuComp = 1;
    double FlatCosBinningForNuMuComp[2]= {-1,1};

    for(int i = SampleId::kUnassigned; i < SampleId::kNSamples; ++i){
      SampleId::SampleEnum sample = static_cast<SampleId::SampleEnum>(i);

      if(bothAxis.find(sample) != bothAxis.end()){
//        bothAxis.erase(sample);
        bothAxis[sample] = new TAxis2D(nBinsFlatMomBinningForNuMuComp,FlatMomBinningForNuMuComp,
                                       nBinsFlatCosBinningForNuMuComp,FlatCosBinningForNuMuComp);
      }
      if(bothAxis_Det.find(sample) != bothAxis_Det.end()){
  //      bothAxis_Det.erase(sample);
        bothAxis_Det[sample] = new TAxis2D(nBinsFlatMomBinningForNuMuComp,FlatMomBinningForNuMuComp,
                                           nBinsFlatCosBinningForNuMuComp,FlatCosBinningForNuMuComp);
      }
    }
  }

  for(int i = SampleId::kUnassigned; i < SampleId::kNSamples; i++){
    SampleId::SampleEnum sample = static_cast<SampleId::SampleEnum>(i);

    std::map<SampleId::SampleEnum, TAxis2D*>::const_iterator it0 = bothAxis    .find(sample);
    std::map<SampleId::SampleEnum, TAxis2D*>::const_iterator it1 = bothAxis_Det.find(sample);

    const bool isActiveSample = it0 != bothAxis.end() && it1 != bothAxis_Det.end();
    ActiveSample[sample] = isActiveSample;

    if(isActiveSample)
    {
      TAxis2D* ax1 = bothAxis    [sample];
      TAxis2D* ax2 = bothAxis_Det[sample];
      Axis_Mom    [sample] = (*ax1)[0];
      Axis_Cos    [sample] = (*ax1)[1];
      Axis_Mom_Det[sample] = (*ax2)[0];
      Axis_Cos_Det[sample] = (*ax2)[1];
    }
  }

  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableNuMuCCP0DWater")))
  {
      if(DoOnlyP0D)
      {
          std::cout << "ERROR: P0D water sample disabled but < BANFF.DoOnlyP0DSelections = 1 >" << std::endl;
          throw;
      }
      ActiveSample[SampleId::kP0DWaterNuMuCC] = false;
  }

  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableNuMuCCP0DAir")))
  {
      if(DoOnlyP0D)
      {
          std::cout << "ERROR: P0D air sample disabled but < BANFF.DoOnlyP0DSelections = 1 >" << std::endl;
          throw;
      }
      ActiveSample[SampleId::kP0DAirNuMuCC  ] = false;
  }

  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableNuMuBkgInAntiNuModeCCP0DWater")))
  {
      if(DoOnlyP0D)
      {
          std::cout << "ERROR: P0D water sample disabled but < BANFF.DoOnlyP0DSelections = 1 >" << std::endl;
          throw;
      }
      ActiveSample[SampleId::kP0DWaterNuMuBkgInAntiNuModeCC] = false;
  }

  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableNuMuBkgInAntiNuModeCCP0DAir")))
  {
      if(DoOnlyP0D)
      {
          std::cout << "ERROR: P0D air sample disabled but < BANFF.DoOnlyP0DSelections = 1 >" << std::endl;
          throw;
      }
      ActiveSample[SampleId::kP0DAirNuMuBkgInAntiNuModeCC] = false;
  }

  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableNuMubarInAntiNuModeCCP0DWater")))
  {
      if(DoOnlyP0D)
      {
          std::cout << "ERROR: P0D water sample disabled but < BANFF.DoOnlyP0DSelections = 1 >" << std::endl;
          throw;
      }
      ActiveSample[SampleId::kP0DWaterNuMuBarInAntiNuModeCC] = false;
  }

  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableNuMubarInAntiNuModeCCP0DAir")))
  {
      if(DoOnlyP0D)
      {
          std::cout << "ERROR: P0D air sample disabled but < BANFF.DoOnlyP0DSelections = 1 >" << std::endl;
          throw;
      }
      ActiveSample[SampleId::kP0DAirNuMuBarInAntiNuModeCC] = false;
  }

  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCCMultiPi"))){
    ActiveSample[SampleId::kFGD1NuMuCC0Pi  ] = false;
    ActiveSample[SampleId::kFGD1NuMuCC1Pi  ] = false;
    ActiveSample[SampleId::kFGD1NuMuCCOther] = false;
  }
  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCCMultiPiFGD2"))){
    ActiveSample[SampleId::kFGD2NuMuCC0Pi  ] = false;
    ActiveSample[SampleId::kFGD2NuMuCC1Pi  ] = false;
    ActiveSample[SampleId::kFGD2NuMuCCOther] = false;
  }
  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCC4piMultiPi"))){
    ActiveSample[SampleId::kFGD1NuMuCC0PiFwd    ] = false;
    ActiveSample[SampleId::kFGD1NuMuCC1PiFwd    ] = false;
    ActiveSample[SampleId::kFGD1NuMuCCOtherFwd  ] = false;
    ActiveSample[SampleId::kFGD1NuMuCC0PiBwd    ] = false;
    ActiveSample[SampleId::kFGD1NuMuCC1PiBwd    ] = false;
    ActiveSample[SampleId::kFGD1NuMuCCOtherBwd  ] = false;
    ActiveSample[SampleId::kFGD1NuMuCC0PiHAFwd  ] = false;
    ActiveSample[SampleId::kFGD1NuMuCC1PiHAFwd  ] = false;
    ActiveSample[SampleId::kFGD1NuMuCCOtherHAFwd] = false;
    ActiveSample[SampleId::kFGD1NuMuCC0PiHABwd  ] = false;
    ActiveSample[SampleId::kFGD1NuMuCC1PiHABwd  ] = false;
    ActiveSample[SampleId::kFGD1NuMuCCOtherHABwd] = false;
  }
  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCC4piMultiPiFGD2"))){
    ActiveSample[SampleId::kFGD2NuMuCC0PiFwd    ] = false;
    ActiveSample[SampleId::kFGD2NuMuCC1PiFwd    ] = false;
    ActiveSample[SampleId::kFGD2NuMuCCOtherFwd  ] = false;
    ActiveSample[SampleId::kFGD2NuMuCC0PiBwd    ] = false;
    ActiveSample[SampleId::kFGD2NuMuCC1PiBwd    ] = false;
    ActiveSample[SampleId::kFGD2NuMuCCOtherBwd  ] = false;
    ActiveSample[SampleId::kFGD2NuMuCC0PiHAFwd  ] = false;
    ActiveSample[SampleId::kFGD2NuMuCC1PiHAFwd  ] = false;
    ActiveSample[SampleId::kFGD2NuMuCCOtherHAFwd] = false;
    ActiveSample[SampleId::kFGD2NuMuCC0PiHABwd  ] = false;
    ActiveSample[SampleId::kFGD2NuMuCC1PiHABwd  ] = false;
    ActiveSample[SampleId::kFGD2NuMuCCOtherHABwd] = false;
  }
  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiPi"))){
    ActiveSample[SampleId::kFGD1AntiNuMuCC0Pi  ] = false;
    ActiveSample[SampleId::kFGD1AntiNuMuCC1Pi  ] = false;
    ActiveSample[SampleId::kFGD1AntiNuMuCCOther] = false;
  }
  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiPiFGD2"))){
    ActiveSample[SampleId::kFGD2AntiNuMuCC0Pi  ] = false;
    ActiveSample[SampleId::kFGD2AntiNuMuCC1Pi  ] = false;
    ActiveSample[SampleId::kFGD2AntiNuMuCCOther] = false;
  }
  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiPi"))){
    ActiveSample[SampleId::kFGD1NuMuBkgInAntiNuModeCC0Pi  ] = false;
    ActiveSample[SampleId::kFGD1NuMuBkgInAntiNuModeCC1Pi  ] = false;
    ActiveSample[SampleId::kFGD1NuMuBkgInAntiNuModeCCOther] = false;
  }
  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiPiFGD2"))){
    ActiveSample[SampleId::kFGD2NuMuBkgInAntiNuModeCC0Pi  ] = false;
    ActiveSample[SampleId::kFGD2NuMuBkgInAntiNuModeCC1Pi  ] = false;
    ActiveSample[SampleId::kFGD2NuMuBkgInAntiNuModeCCOther] = false;
  }
  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiTrack"))){
    ActiveSample[SampleId::kFGD1AntiNuMuCC1Track ] = false;
    ActiveSample[SampleId::kFGD1AntiNuMuCCNTracks] = false;
  }
  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiTrackFGD2"))){
    ActiveSample[SampleId::kFGD2AntiNuMuCC1Track ] = false;
    ActiveSample[SampleId::kFGD2AntiNuMuCCNTracks] = false;
  }
  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiTrack"))){
    ActiveSample[SampleId::kFGD1NuMuBkgInAntiNuModeCC1Track ] = false;
    ActiveSample[SampleId::kFGD1NuMuBkgInAntiNuModeCCNTracks] = false;
  }
  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiTrackFGD2"))){
    ActiveSample[SampleId::kFGD2NuMuBkgInAntiNuModeCC1Track ] = false;
    ActiveSample[SampleId::kFGD2NuMuBkgInAntiNuModeCCNTracks] = false;
  }
  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNuECC"))){
    ActiveSample[SampleId::kFGD1NuECC               ] = false;
    ActiveSample[SampleId::kFGD1NuEBkgInAntiNuModeCC] = false;
  }
  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNuECCFGD2"))){
    ActiveSample[SampleId::kFGD2NuECC               ] = false;
    ActiveSample[SampleId::kFGD2NuEBkgInAntiNuModeCC] = false;
  }
  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNuECC"))){
    ActiveSample[SampleId::kFGD1AntiNuECC           ] = false;
    ActiveSample[SampleId::kFGD1AntiNuEBkgInNuModeCC] = false;
  }
  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNuECCFGD2"))){
    ActiveSample[SampleId::kFGD2AntiNuECC           ] = false;
    ActiveSample[SampleId::kFGD2AntiNuEBkgInNuModeCC] = false;
  }
  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerGamma"))){
    ActiveSample[SampleId::kFGD1Gamma            ] = false;
    ActiveSample[SampleId::kFGD1GammaInAntiNuMode] = false;
  }
  if(!(bool)(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerGammaFGD2"))){
    ActiveSample[SampleId::kFGD2Gamma            ] = false;
    ActiveSample[SampleId::kFGD2GammaInAntiNuMode] = false;
  }

}

//*****************************************************************************
BANFF::TAxis2D::TAxis2D(int nbin1, double* bins1,int nbin2, double* bins2)
//*****************************************************************************
{
    FirstDim  = new TAxis(nbin1, bins1);
    SecondDim = new TAxis(nbin2, bins2);
    Array = new TAxis*[ArraySize];
    Array[0] = FirstDim;
    Array[1] = SecondDim;
}

//*****************************************************************************
BANFF::TAxis2D::~TAxis2D()
//*****************************************************************************
{
    delete FirstDim;
    delete SecondDim;
    delete Array;
};

//*****************************************************************************
TAxis* BANFF::TAxis2D::operator[] (const unsigned int &d)
//*****************************************************************************
{
    if(d >= ArraySize)
    {
        std::cerr << "Can't acces more than 2D" << std::endl;
        throw;
    }
    return Array[d];
}

//*****************************************************************************
BANFF::BinningDefinition::~BinningDefinition()
//*****************************************************************************
{
    SampleEnumToTAxisMap_t::iterator it;
    for (it = Axis_Mom.begin(); it!=Axis_Mom.end(); ++it){
      if(it->second) delete it->second;
      Axis_Mom.erase(it);
    }

    for (it = Axis_Mom_Det.begin(); it!=Axis_Mom_Det.end(); ++it){
      if(it->second) delete it->second;
      Axis_Mom_Det.erase(it);
    }
    for (it = Axis_Cos.begin(); it!=Axis_Cos.end(); ++it){
      if(it->second) delete it->second;
      Axis_Cos.erase(it);
    }

    for (it = Axis_Cos_Det.begin(); it!=Axis_Cos_Det.end(); ++it){
      if(it->second) delete it->second;
      Axis_Cos_Det.erase(it);
    }
}

//*****************************************************************************
bool BANFF::BinningDefinition::IsActiveSample(const SampleId::SampleEnum &sample) const
//*****************************************************************************
{
    std::map<SampleId::SampleEnum, bool>::const_iterator it;
    it = ActiveSample.find(sample);
    if(it != ActiveSample.end())
        return it->second;
    return false;
}

//*****************************************************************************
bool BANFF::BinningDefinition::IsActiveSample(const int &sample) const
//*****************************************************************************
{
    if(sample >= SampleId::kUnassigned && sample < SampleId::kNSamples)
    {
        const SampleId::SampleEnum s = static_cast<SampleId::SampleEnum>(sample);
        return IsActiveSample(s);
    }
    return false;
};


//*****************************************************************************
int BANFF::BinningDefinition::GetNbins_Mom (const SampleId::SampleEnum &sample) const
//*****************************************************************************
{
    if(IsActiveSample(sample))
        return Axis_Mom.at(sample)->GetNbins();
    return 0;
}

//*****************************************************************************
int BANFF::BinningDefinition::GetNbins_Mom_Det(const SampleId::SampleEnum &sample) const
//*****************************************************************************
{
  if(IsActiveSample(sample))
    return Axis_Mom_Det.at(sample)->GetNbins();
  return 0;
}

//*****************************************************************************
int BANFF::BinningDefinition::GetNbins_Cos(const SampleId::SampleEnum &sample) const
//*****************************************************************************
{
  if(IsActiveSample(sample))
    return Axis_Cos.at(sample)->GetNbins();
  return 0;
}

//*****************************************************************************
int BANFF::BinningDefinition::GetNbins_Cos_Det(const SampleId::SampleEnum &sample) const
//*****************************************************************************
{
  if(IsActiveSample(sample))
    return Axis_Cos_Det.at(sample)->GetNbins();
  return 0;
}

//*****************************************************************************
int BANFF::BinningDefinition::GetNbins_Det(const SampleId::SampleEnum &sample) const
//*****************************************************************************
{
  if(IsActiveSample(sample))
    return GetNbins_Mom_Det(sample) * GetNbins_Cos_Det(sample);
  return 0;
}

//*****************************************************************************
int BANFF::BinningDefinition::GetNbins(const SampleId::SampleEnum &sample) const
//*****************************************************************************
{
  if(IsActiveSample(sample))
    return GetNbins_Mom(sample) * GetNbins_Cos(sample);
  return 0;
};

//*****************************************************************************
TAxis* BANFF::BinningDefinition::GetBinning_Mom(const SampleId::SampleEnum &sample) const
//*****************************************************************************
{
  if(!IsActiveSample(sample))
  {
      std::cerr << "No momentum binning for the sample " << ConvertSample(sample) << std::endl;
      throw;
  }
  return Axis_Mom.at(sample);
}

//*****************************************************************************
TAxis* BANFF::BinningDefinition::GetBinning_Mom_Det(const SampleId::SampleEnum &sample) const
//*****************************************************************************
{
  if(!IsActiveSample(sample))
  {
    std::cerr << "No momentum (det cov) binning for the sample " << ConvertSample(sample) << std::endl;
    throw;
  }
  return Axis_Mom_Det.at(sample);
}

//*****************************************************************************
TAxis* BANFF::BinningDefinition::GetBinning_Cos(const SampleId::SampleEnum &sample) const
//*****************************************************************************
{
  if(!IsActiveSample(sample))
  {
    std::cerr << "No costheta binning for the sample " << ConvertSample(sample) << std::endl;
    throw;
  }
  return Axis_Cos.at(sample);
}

//*****************************************************************************
TAxis* BANFF::BinningDefinition::GetBinning_Cos_Det(const SampleId::SampleEnum &sample) const
//*****************************************************************************
{
  if(!IsActiveSample(sample))
  {
    std::cerr << "No costheta (det cov) binning for the sample " << ConvertSample(sample) << std::endl;
    throw;
  }
  return Axis_Cos_Det.at(sample);
}

//*****************************************************************************
BANFF::TAxis2D* BANFF::BinningDefinition::GetBinningArray(const SampleId::SampleEnum &sample) const
//*****************************************************************************
{
  if(!IsActiveSample(sample))
  {
    std::cerr << "No costheta (det cov) binning for the sample " << ConvertSample(sample) << std::endl;
    throw;
  }
  return bothAxis.at(sample);
}

///The static member pointer to the singleton.
//*****************************************************************************
BANFF::BinningDefinition* BANFF::BinningDefinition::fBinningDefinition = NULL;
//*****************************************************************************

//*****************************************************************************
BANFF::BinningDefinition& BANFF::BinningDefinition::Get() {
//*****************************************************************************
  if (!fBinningDefinition)
    fBinningDefinition = new BANFF::BinningDefinition();
  return *(fBinningDefinition);
}



//*****************************************************************************
int BANFF::BinningDefinition::GetGlobalBinMatrixMomCos_Det(const SampleId::SampleEnum &sample,
        const double &Mom, const double &Cos) const
//*****************************************************************************
{
  /// This gets the global bin number in the detector matrix
  int row = 0;
  if(Mom < 0     || Cos < -1) return -1;
  if(Mom > 30000 || Cos >  1) return -1;
  SampleEnumToTAxisMap_t::const_iterator it;
  for (it=Axis_Mom_Det.begin(); it!=Axis_Mom_Det.end(); ++it){
    if(sample == (*it).first){
      TAxis* MomAxis = Axis_Mom_Det.at((*it).first); //Makes the code faster by not calling endless time the map?
      TAxis* CosAxis = Axis_Cos_Det.at((*it).first);
      int binmom = MomAxis->FindBin(Mom); int nbinmom = MomAxis->GetNbins();
      int bincos = CosAxis->FindBin(Cos); int nbincos = CosAxis->GetNbins();
      if(bincos==0 || bincos>nbincos || binmom==0 || binmom>nbinmom){ return -1; }
      return row + (bincos-1) * nbinmom + binmom-1;
    }else{
      row = row + GetNbins_Det((*it).first);
    }
  }
  return -1;
}


//*****************************************************************************
int BANFF::BinningDefinition::GetGlobalBinMatrixMomCos(const SampleId::SampleEnum &sample,
        const double &Mom, const double &Cos) const
//*****************************************************************************
{
  /// This gets the global bin number in the matrix
  int row = 0;
  if(Mom < 0     || Cos < -1) return -1;
  if(Mom > 30000 || Cos >  1) return -1;

  for (SampleEnumToTAxisMap_t::const_iterator it=Axis_Mom.begin(); it!=Axis_Mom.end(); ++it){
    if(sample == (*it).first){
      TAxis* MomAxis = Axis_Mom.at((*it).first); //Makes the code faster by not calling endless time the map?
      TAxis* CosAxis = Axis_Cos.at((*it).first);
      int binmom = MomAxis->FindBin(Mom); int nbinmom = MomAxis->GetNbins();
      int bincos = CosAxis->FindBin(Cos); int nbincos = CosAxis->GetNbins();
      if(bincos==0 || bincos>nbincos || binmom==0 || binmom>nbinmom){ return -1; }
      return row + (bincos-1) * nbinmom + binmom-1;
    }else{
      row = row + GetNbins((*it).first);
    }
  }
  return -1;

}

//*****************************************************************************
void BANFF::BinningDefinition::GetLocalBinMatrixMomCos(const int &GlobalBin, SampleId::SampleEnum& Sample, int& MomBin, int& CosBin){
//*****************************************************************************

  Sample = SampleId::kUnassigned;
  MomBin = -1;
  CosBin = -1;

  if(GlobalBin > GetNbins()){
    return;
  }
  Sample = GetSampleFromIndex(GlobalBin);
  // Gets the first index in the matrix with this sample
  int Ind = GetGlobalBinMatrix(Sample);
  int LocalIndex = GlobalBin - Ind;

  int nMomBin = GetNbins_Mom((SampleId::SampleEnum)Sample);
  int nCosBin = GetNbins_Cos((SampleId::SampleEnum)Sample);

  for(int i = 0; i < nMomBin; ++i){
    for(int j = 0; j < nCosBin; ++j){
      if(j * nMomBin + i == LocalIndex){
        MomBin = i;
        CosBin = j;
        return;
      }
    }
  }

}
//*****************************************************************************
void BANFF::BinningDefinition::GetLocalBinMatrixMomCos_Det(const int &GlobalBin, SampleId::SampleEnum& Sample, int& MomBin, int& CosBin){
//*****************************************************************************

  Sample = SampleId::kUnassigned;
  MomBin = -1;
  CosBin = -1;

  if(GlobalBin > GetNbins_Det()){
    return;
  }
  Sample = GetSampleFromIndex_Det(GlobalBin);
  // Gets the first index in the matrix with this sample
  int Ind = GetGlobalBinMatrix_Det(Sample);
  int LocalIndex = GlobalBin - Ind;
  int nMomBin = GetNbins_Mom_Det((SampleId::SampleEnum)Sample);
  int nCosBin = GetNbins_Cos_Det((SampleId::SampleEnum)Sample);

  for(int i = 0; i < nMomBin; ++i){
    for(int j = 0; j < nCosBin; ++j){
      if(j * nMomBin + i == LocalIndex){
        MomBin = i;
        CosBin = j;
        return;
      }
    }
  }

}


//*****************************************************************************
int BANFF::BinningDefinition::GetGlobalBinMatrix_Det(const SampleId::SampleEnum &sample) const {
//*****************************************************************************
  /// This gets the global bin number in the detector matrix
  int row = 0;

  for (SampleEnumToTAxisMap_t::const_iterator it=Axis_Mom_Det.begin(); it!=Axis_Mom_Det.end(); ++it){
    if(sample == it->first)
      return row;
    row = row + GetNbins_Det(it->first);
  }
  return -1;
}


//*****************************************************************************
int BANFF::BinningDefinition::GetGlobalBinMatrix(const SampleId::SampleEnum &sample) const {
//*****************************************************************************
  /// This gets the global bin number in the matrix
  int row = 0;

  for (SampleEnumToTAxisMap_t::const_iterator it=Axis_Mom.begin(); it!=Axis_Mom.end(); ++it){
    if(sample == it->first)
      return row;
    row = row + GetNbins(it->first);
  }
  return -1;

}


//*****************************************************************************
int BANFF::BinningDefinition::GetNbins_Det() const{
//*****************************************************************************
  /// This gets the total number of bins in the detector matrix
  int row = 0;

  for (SampleEnumToTAxisMap_t::const_iterator it=Axis_Mom_Det.begin(); it!=Axis_Mom_Det.end(); ++it){
    row = row + GetNbins_Det(it->first);
  }
  return row;

}


//*****************************************************************************
int BANFF::BinningDefinition::GetNbins() const{
//*****************************************************************************
  /// This gets the total number of bins in the matrix
  int row = 0;

  for (SampleEnumToTAxisMap_t::const_iterator it=Axis_Mom.begin(); it!=Axis_Mom.end(); ++it){
    row = row + GetNbins(it->first);
  }
  return row;

}

//*****************************************************************************
SampleId::SampleEnum BANFF::BinningDefinition::GetSampleFromIndex(const int &index) const
//*****************************************************************************
{

  int row = 0;

  for (SampleEnumToTAxisMap_t::const_iterator it=Axis_Mom.begin(); it!=Axis_Mom.end(); ++it)
  {
    row = row + GetNbins(it->first);
    if(index < row)
      return it->first;
  }
  return SampleId::kUnassigned;
}

//*****************************************************************************
SampleId::SampleEnum BANFF::BinningDefinition::GetSampleFromIndex_Det(const int &index) const
//*****************************************************************************
{

  int row = 0;

  for (SampleEnumToTAxisMap_t::const_iterator it = Axis_Mom_Det.begin(); it != Axis_Mom_Det.end(); ++it){
    row = row + GetNbins_Det(it->first);
    if(index < row)
      return it->first;
  }
  return SampleId::kUnassigned;

}

//*****************************************************************************
std::vector<std::string> BANFF::BinningDefinition::GetListOfBins() const
//*****************************************************************************
{

  std::vector<std::string> BinList;

  for(int i = SampleId::kUnassigned; i < SampleId::kNSamples; i++){
    const SampleId::SampleEnum sample = static_cast<SampleId::SampleEnum>(i);
    if(!IsActiveSample(sample))
      continue;
    const std::string samplename = ConvertSample(sample);
    for(int ibin = 0; ibin < GetNbins(sample); ibin++)
      BinList.push_back(std::string(Form("%s_%d", samplename.c_str(), ibin)));
  }

  return BinList;

}

//*****************************************************************************
std::vector<std::string> BANFF::BinningDefinition::GetListOfBins_Det() const
//*****************************************************************************
{
  std::vector<std::string> BinList;

  for(int i = SampleId::kUnassigned; i < SampleId::kNSamples; i++){
    const SampleId::SampleEnum sample = static_cast<SampleId::SampleEnum>(i);
    if(!IsActiveSample(sample))
      continue;
    const std::string samplename = ConvertSample(sample);
    for(int ibin = 0; ibin < GetNbins_Det(sample); ibin++)
      BinList.push_back(std::string(Form("%s_%d", samplename.c_str(), ibin)));
  }

  return BinList;
}

//*****************************************************************************
std::map<int,int> BANFF::BinningDefinition::GetFullToReduMap()
//*****************************************************************************
{
  /// A map to be able to go from full -> reduced binning
  if(FullToReduMap.size() > 0)
    return FullToReduMap;

  //int nBinsRedu = GetNbins_Det();
  int nBinsFull = GetNbins();

  //Start with some check that the actual binning is ok.
  for(int j = 0; j < nBinsFull; ++j){
    int binmom;
    int bincos;
    SampleId::SampleEnum sample;
    // First get the local bin (in the block of the sample)
    GetLocalBinMatrixMomCos(j, sample, binmom, bincos);
    double ph = GetBinning_Mom(sample)->GetBinUpEdge (binmom+1);
    double pl = GetBinning_Mom(sample)->GetBinLowEdge(binmom+1);
    double ch = GetBinning_Cos(sample)->GetBinUpEdge (bincos+1);
    double cl = GetBinning_Cos(sample)->GetBinLowEdge(bincos+1);

    // Check that these match up they should be the same
    if(j != GetGlobalBinMatrixMomCos(sample, 0.5*(ph+pl), 0.5*(ch+cl))){
      std::cout << "Closure test failed!! " << std::endl;
      std::cout << "Global Bin: " << j << std::endl;
      std::cout << "Should be identical to GetGlobalBinMatrixMomCos(" << sample << ", " << 0.5*(ph+pl) << ", "<< 0.5*(ch+cl) << ") " <<  GetGlobalBinMatrixMomCos(sample, 0.5*(ph+pl), 0.5*(ch+cl)) << std::endl;
      // If they dont exit
      throw;
    }
    if(j != GetGlobalBinMatrix(sample)+bincos*GetNbins_Mom(sample)+binmom){
      std::cout << "Closure test failed!! " << std::endl;
      std::cout << "Global Bin: " << j << std::endl;
      std::cout << "Should be identical to GetGlobalBinMatrix(" << sample << ") + bincos*GetNbins_Mom(sample) + binmom : " <<  GetGlobalBinMatrix(sample) << " + " << bincos << " * " << GetNbins_Mom(sample) << " + " << binmom << std::endl;
      // If they dont exit
      throw;
    }

    double epsilon = 0.0001; // Make sure this is smaller that the cos binning!!
    int bin_det  = GetGlobalBinMatrixMomCos_Det(sample, ph-epsilon, ch-epsilon);
    int bin2_det = GetGlobalBinMatrixMomCos_Det(sample, pl+epsilon, cl+epsilon);
    int binmom_det, bincos_det;
    SampleId::SampleEnum sample_det;
    GetLocalBinMatrixMomCos_Det(bin_det, sample_det, binmom_det, bincos_det);

    if(bin_det != bin2_det){
      std::cerr << "The fine binning is more coarse than the detector binning!!!" << std::endl;
      std::cerr << "Maybe epilson is too large for the Cos binning?" << std::endl;

      std::cerr << "First detector cov bin corresponds to (low edge of the fine binning): Global bin " << bin_det << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Mom Binning->GetBinLowEdge("<<binmom_det<<") " << GetBinning_Mom_Det(sample_det)->GetBinLowEdge(binmom_det+1) << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Mom Binning->GetBinUpEdge ("<<binmom_det<<") " << GetBinning_Mom_Det(sample_det)->GetBinUpEdge (binmom_det+1) << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Cos Binning->GetBinLowEdge("<<bincos_det<<") " << GetBinning_Cos_Det(sample_det)->GetBinLowEdge(bincos_det+1) << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Cos Binning->GetBinUpEdge ("<<bincos_det<<") " << GetBinning_Cos_Det(sample_det)->GetBinUpEdge (bincos_det+1) << std::endl;

      GetLocalBinMatrixMomCos_Det(bin2_det, sample_det, binmom_det, bincos_det);
      std::cerr << "Second detector cov bin corresponds to (high edge of the fine binning): Global bin " << bin2_det << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Mom Binning->GetBinLowEdge("<<binmom_det<<") " << GetBinning_Mom_Det(sample_det)->GetBinLowEdge(binmom_det+1) << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Mom Binning->GetBinUpEdge ("<<binmom_det<<") " << GetBinning_Mom_Det(sample_det)->GetBinUpEdge (binmom_det+1) << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Cos Binning->GetBinLowEdge("<<bincos_det<<") " << GetBinning_Cos_Det(sample_det)->GetBinLowEdge(bincos_det+1) << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Cos Binning->GetBinUpEdge ("<<bincos_det<<") " << GetBinning_Cos_Det(sample_det)->GetBinUpEdge (bincos_det+1) << std::endl;

      std::cerr << "These 2 should be the same!!" << std::endl;
      throw;
    }

    double ph_det = GetBinning_Mom_Det(sample_det)->GetBinUpEdge (binmom_det+1);
    double pl_det = GetBinning_Mom_Det(sample_det)->GetBinLowEdge(binmom_det+1);
    double ch_det = GetBinning_Cos_Det(sample_det)->GetBinUpEdge (bincos_det+1);
    double cl_det = GetBinning_Cos_Det(sample_det)->GetBinLowEdge(bincos_det+1);
    if(bin_det != GetGlobalBinMatrixMomCos_Det(sample_det, 0.5*(ph_det+pl_det), 0.5*(ch_det+cl_det))){
      std::cout << "Closure test failed!! " << std::endl;
      std::cout << "Global Bin: " << bin_det << std::endl;
      std::cout << "Should be identical to GetGlobalBinMatrixMomCos_Det(" << sample_det << ", " << 0.5*(ph_det+pl_det) << ", "<< 0.5*(ch_det+cl_det) << ") " <<  GetGlobalBinMatrixMomCos_Det(sample_det, 0.5*(ph_det+pl_det), 0.5*(ch_det+cl_det)) << std::endl;
      throw;
    }
    if(bin_det != GetGlobalBinMatrix_Det(sample_det)+bincos_det*GetNbins_Mom_Det(sample_det)+binmom_det){
      std::cout << "Closure test failed!! " << std::endl;
      std::cout << "Global Bin: " << bin_det << std::endl;
      std::cout << "Should be identical to GetGlobalBinMatrix(" << sample_det << ", " << 0.5*(ph_det+pl_det) << ", "<< 0.5*(ch_det+cl_det) << ") " <<  GetGlobalBinMatrix(sample_det) << " + bincos_det*GetNbins_Mom_Det(sample_det)+binmom_det" << std::endl;
      throw;
    }
    FullToReduMap[j] = bin_det;
  }
  return FullToReduMap;
}

void BANFF::BinningDefinition::DumpActiveSamples() const
{
    std::map<SampleId::SampleEnum, bool>::const_iterator it;
    for(it = ActiveSample.begin(); it != ActiveSample.end(); ++it)
    {
        if(it->second)
        {
            const SampleId::SampleEnum sample = it->first;
            std::cout << "Active sample " << sample << " of type " << SampleId::ConvertSample(sample).c_str() << std::endl;
        }
    }
}
