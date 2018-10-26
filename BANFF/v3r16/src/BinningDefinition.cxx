#include <BinningDefinition.hxx>


// The static member pointer to the singleton.
BANFF::BinningDefinition* BANFF::BinningDefinition::fBinningDefinition = NULL;


BANFF::BinningDefinition::BinningDefinition(){
  Do4PiFHC     = (bool)ND::params().GetParameterI("BANFF.Do4PiFHC");
  DoNue        = (bool)ND::params().GetParameterI("BANFF.DoNueSelections");
  DoOnlyNue    = (bool)ND::params().GetParameterI("BANFF.DoOnlyNueSelections");
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

  //FHCNumuCC0Pi
  int    FHCNumuCC0Pi_Mom_NBin        = 29;
  double FHCNumuCC0Pi_Mom_Bin[30]     = {0.   , 200. , 300. , 400. , 450. , 500. , 550. , 600. ,
                                         650. , 700. , 750. , 800. , 850. , 900. , 950. , 1000.,
                                         1050., 1100., 1200., 1300., 1400., 1500., 1600., 1700.,
                                         1800., 2000., 2500., 3000., 5000., 30000.};
  int    FHCNumuCC0Pi_Det_Mom_NBin    = 17;
  double FHCNumuCC0Pi_Det_Mom_Bin[18] = {0.   , 200. , 300. , 400. , 450. , 550. , 600. , 650. ,
                                         700. , 750. , 800. , 850. , 900. , 950. , 1000., 1400.,
                                         5000., 30000.};
  
  int    FHCNumuCC0Pi_Cos_NBin = 29;
  double FHCNumuCC0Pi_Cos_Bin[30];
  int    FHCNumuCC0Pi_Det_Cos_NBin = 16;
  double FHCNumuCC0Pi_Det_Cos_Bin[20];
  if(!Do4PiFHC){ //FHC Multi Pi
    FHCNumuCC0Pi_Cos_NBin =  29;        FHCNumuCC0Pi_Det_Cos_NBin =  16;
    FHCNumuCC0Pi_Cos_Bin[ 0] = -1.0  ;  FHCNumuCC0Pi_Det_Cos_Bin[ 0] = -1.0  ;
    FHCNumuCC0Pi_Cos_Bin[ 1] =  0.5  ;  FHCNumuCC0Pi_Det_Cos_Bin[ 1] =  0.5  ;
    FHCNumuCC0Pi_Cos_Bin[ 2] =  0.6  ;  FHCNumuCC0Pi_Det_Cos_Bin[ 2] =  0.6  ;
    FHCNumuCC0Pi_Cos_Bin[ 3] =  0.7  ;  FHCNumuCC0Pi_Det_Cos_Bin[ 3] =  0.7  ;
    FHCNumuCC0Pi_Cos_Bin[ 4] =  0.76 ;  FHCNumuCC0Pi_Det_Cos_Bin[ 4] =  0.76 ;
    FHCNumuCC0Pi_Cos_Bin[ 5] =  0.78 ;  FHCNumuCC0Pi_Det_Cos_Bin[ 5] =  0.8  ;
    FHCNumuCC0Pi_Cos_Bin[ 6] =  0.8  ;  FHCNumuCC0Pi_Det_Cos_Bin[ 6] =  0.83 ;
    FHCNumuCC0Pi_Cos_Bin[ 7] =  0.83 ;  FHCNumuCC0Pi_Det_Cos_Bin[ 7] =  0.85 ;
    FHCNumuCC0Pi_Cos_Bin[ 8] =  0.85 ;  FHCNumuCC0Pi_Det_Cos_Bin[ 8] =  0.88 ;
    FHCNumuCC0Pi_Cos_Bin[ 9] =  0.88 ;  FHCNumuCC0Pi_Det_Cos_Bin[ 9] =  0.965;
    FHCNumuCC0Pi_Cos_Bin[10] =  0.89 ;  FHCNumuCC0Pi_Det_Cos_Bin[10] =  0.97 ;
    FHCNumuCC0Pi_Cos_Bin[11] =  0.9  ;  FHCNumuCC0Pi_Det_Cos_Bin[11] =  0.975;
    FHCNumuCC0Pi_Cos_Bin[12] =  0.91 ;  FHCNumuCC0Pi_Det_Cos_Bin[12] =  0.98 ;
    FHCNumuCC0Pi_Cos_Bin[13] =  0.92 ;  FHCNumuCC0Pi_Det_Cos_Bin[13] =  0.985;
    FHCNumuCC0Pi_Cos_Bin[14] =  0.925;  FHCNumuCC0Pi_Det_Cos_Bin[14] =  0.99 ;
    FHCNumuCC0Pi_Cos_Bin[15] =  0.93 ;  FHCNumuCC0Pi_Det_Cos_Bin[15] =  0.995;
    FHCNumuCC0Pi_Cos_Bin[16] =  0.935;  FHCNumuCC0Pi_Det_Cos_Bin[16] =  1.0  ;
    FHCNumuCC0Pi_Cos_Bin[17] =  0.94 ;
    FHCNumuCC0Pi_Cos_Bin[18] =  0.945;
    FHCNumuCC0Pi_Cos_Bin[19] =  0.95 ;
    FHCNumuCC0Pi_Cos_Bin[20] =  0.955;
    FHCNumuCC0Pi_Cos_Bin[21] =  0.96 ;
    FHCNumuCC0Pi_Cos_Bin[22] =  0.965;
    FHCNumuCC0Pi_Cos_Bin[23] =  0.97 ;
    FHCNumuCC0Pi_Cos_Bin[24] =  0.975;
    FHCNumuCC0Pi_Cos_Bin[25] =  0.98 ;
    FHCNumuCC0Pi_Cos_Bin[26] =  0.985;
    FHCNumuCC0Pi_Cos_Bin[27] =  0.99 ;
    FHCNumuCC0Pi_Cos_Bin[28] =  0.995;
    FHCNumuCC0Pi_Cos_Bin[29] =  1.0  ;
  }else{ //RHC 4pi
    FHCNumuCC0Pi_Cos_NBin = 19;        FHCNumuCC0Pi_Det_Cos_NBin = 19;	      
    FHCNumuCC0Pi_Cos_Bin[ 0] = -1.0;   FHCNumuCC0Pi_Det_Cos_Bin[ 0] = -1.0;  
    FHCNumuCC0Pi_Cos_Bin[ 1] = -0.6;   FHCNumuCC0Pi_Det_Cos_Bin[ 1] = -0.6;  
    FHCNumuCC0Pi_Cos_Bin[ 2] = -0.4;   FHCNumuCC0Pi_Det_Cos_Bin[ 2] = -0.4;  
    FHCNumuCC0Pi_Cos_Bin[ 3] = -0.2;   FHCNumuCC0Pi_Det_Cos_Bin[ 3] = -0.2;  
    FHCNumuCC0Pi_Cos_Bin[ 4] = -0.1;   FHCNumuCC0Pi_Det_Cos_Bin[ 4] = -0.1;  
    FHCNumuCC0Pi_Cos_Bin[ 5] =  0.;    FHCNumuCC0Pi_Det_Cos_Bin[ 5] =  0.;   
    FHCNumuCC0Pi_Cos_Bin[ 6] =  0.1;   FHCNumuCC0Pi_Det_Cos_Bin[ 6] =  0.1;  
    FHCNumuCC0Pi_Cos_Bin[ 7] =  0.2;   FHCNumuCC0Pi_Det_Cos_Bin[ 7] =  0.2;  
    FHCNumuCC0Pi_Cos_Bin[ 8] =  0.4;   FHCNumuCC0Pi_Det_Cos_Bin[ 8] =  0.4;  
    FHCNumuCC0Pi_Cos_Bin[ 9] =  0.6;   FHCNumuCC0Pi_Det_Cos_Bin[ 9] =  0.6;  
    FHCNumuCC0Pi_Cos_Bin[10] =  0.7;   FHCNumuCC0Pi_Det_Cos_Bin[10] =  0.7;  
    FHCNumuCC0Pi_Cos_Bin[11] =  0.8;   FHCNumuCC0Pi_Det_Cos_Bin[11] =  0.8;  
    FHCNumuCC0Pi_Cos_Bin[12] =  0.85;  FHCNumuCC0Pi_Det_Cos_Bin[12] =  0.85; 
    FHCNumuCC0Pi_Cos_Bin[13] =  0.90;  FHCNumuCC0Pi_Det_Cos_Bin[13] =  0.90; 
    FHCNumuCC0Pi_Cos_Bin[14] =  0.92;  FHCNumuCC0Pi_Det_Cos_Bin[14] =  0.92; 
    FHCNumuCC0Pi_Cos_Bin[15] =  0.94;  FHCNumuCC0Pi_Det_Cos_Bin[15] =  0.94; 
    FHCNumuCC0Pi_Cos_Bin[16] =  0.96;  FHCNumuCC0Pi_Det_Cos_Bin[16] =  0.96; 
    FHCNumuCC0Pi_Cos_Bin[17] =  0.98;  FHCNumuCC0Pi_Det_Cos_Bin[17] =  0.98; 
    FHCNumuCC0Pi_Cos_Bin[18] =  0.99;  FHCNumuCC0Pi_Det_Cos_Bin[18] =  0.99; 
    FHCNumuCC0Pi_Cos_Bin[19] =  1.0;   FHCNumuCC0Pi_Det_Cos_Bin[19] =  1.0;  
  }
  if(Do1DCheckMom){FHCNumuCC0Pi_Det_Cos_NBin=1; FHCNumuCC0Pi_Det_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){FHCNumuCC0Pi_Det_Mom_NBin=1; FHCNumuCC0Pi_Det_Mom_Bin[1] = 30000;}

  //FHCNumuCC1Pi
  int    FHCNumuCC1Pi_Mom_NBin        = 18;
  double FHCNumuCC1Pi_Mom_Bin[19]     = {0.   , 300. , 350. , 400. , 500. , 600. , 650. , 700. ,
                                         750. , 800. , 900. , 1000., 1100., 1200., 1500., 2000.,
                                         3000., 5000., 30000.};
  int    FHCNumuCC1Pi_Det_Mom_NBin    = 11;
  double FHCNumuCC1Pi_Det_Mom_Bin[12] = {0.   , 300. , 350. , 400. , 500. , 600. , 650. , 700. ,
                                         1100., 3000., 5000., 30000.};
    
  int    FHCNumuCC1Pi_Cos_NBin = 16;
  double FHCNumuCC1Pi_Cos_Bin[20];
  int    FHCNumuCC1Pi_Det_Cos_NBin = 10;
  double FHCNumuCC1Pi_Det_Cos_Bin[20];
  if(!Do4PiFHC){ //FHC Multi Pi
    FHCNumuCC1Pi_Cos_NBin = 16;         FHCNumuCC1Pi_Det_Cos_NBin = 10;
    FHCNumuCC1Pi_Cos_Bin[ 0] = -1.0  ;  FHCNumuCC1Pi_Det_Cos_Bin[ 0] = -1.0 ;
    FHCNumuCC1Pi_Cos_Bin[ 1] =  0.6  ;  FHCNumuCC1Pi_Det_Cos_Bin[ 1] =  0.6 ;
    FHCNumuCC1Pi_Cos_Bin[ 2] =  0.7  ;  FHCNumuCC1Pi_Det_Cos_Bin[ 2] =  0.7 ;
    FHCNumuCC1Pi_Cos_Bin[ 3] =  0.8  ;  FHCNumuCC1Pi_Det_Cos_Bin[ 3] =  0.8 ;
    FHCNumuCC1Pi_Cos_Bin[ 4] =  0.85 ;  FHCNumuCC1Pi_Det_Cos_Bin[ 4] =  0.85;
    FHCNumuCC1Pi_Cos_Bin[ 5] =  0.88 ;  FHCNumuCC1Pi_Det_Cos_Bin[ 5] =  0.88;
    FHCNumuCC1Pi_Cos_Bin[ 6] =  0.9  ;  FHCNumuCC1Pi_Det_Cos_Bin[ 6] =  0.9 ;
    FHCNumuCC1Pi_Cos_Bin[ 7] =  0.92 ;  FHCNumuCC1Pi_Det_Cos_Bin[ 7] =  0.92;
    FHCNumuCC1Pi_Cos_Bin[ 8] =  0.93 ;  FHCNumuCC1Pi_Det_Cos_Bin[ 8] =  0.93;
    FHCNumuCC1Pi_Cos_Bin[ 9] =  0.94 ;  FHCNumuCC1Pi_Det_Cos_Bin[ 9] =  0.94;
    FHCNumuCC1Pi_Cos_Bin[10] =  0.95 ;  FHCNumuCC1Pi_Det_Cos_Bin[10] =  1.0 ;
    FHCNumuCC1Pi_Cos_Bin[11] =  0.96 ;  
    FHCNumuCC1Pi_Cos_Bin[12] =  0.97 ;  
    FHCNumuCC1Pi_Cos_Bin[13] =  0.98 ;  
    FHCNumuCC1Pi_Cos_Bin[14] =  0.99 ;  
    FHCNumuCC1Pi_Cos_Bin[15] =  0.995;  
    FHCNumuCC1Pi_Cos_Bin[16] =  1.0  ;  
  }else{ //FHC 4pi
    FHCNumuCC1Pi_Cos_NBin = 19;        FHCNumuCC1Pi_Det_Cos_NBin = 19;	
    FHCNumuCC1Pi_Cos_Bin[ 0] = -1.0;   FHCNumuCC1Pi_Det_Cos_Bin[ 0] = -1.0;	
    FHCNumuCC1Pi_Cos_Bin[ 1] = -0.6;   FHCNumuCC1Pi_Det_Cos_Bin[ 1] = -0.6;	
    FHCNumuCC1Pi_Cos_Bin[ 2] = -0.4;   FHCNumuCC1Pi_Det_Cos_Bin[ 2] = -0.4;	
    FHCNumuCC1Pi_Cos_Bin[ 3] = -0.2;   FHCNumuCC1Pi_Det_Cos_Bin[ 3] = -0.2;	
    FHCNumuCC1Pi_Cos_Bin[ 4] = -0.1;   FHCNumuCC1Pi_Det_Cos_Bin[ 4] = -0.1;	
    FHCNumuCC1Pi_Cos_Bin[ 5] =  0.;	   FHCNumuCC1Pi_Det_Cos_Bin[ 5] =  0.;	
    FHCNumuCC1Pi_Cos_Bin[ 6] =  0.1;   FHCNumuCC1Pi_Det_Cos_Bin[ 6] =  0.1;	
    FHCNumuCC1Pi_Cos_Bin[ 7] =  0.2;   FHCNumuCC1Pi_Det_Cos_Bin[ 7] =  0.2;	
    FHCNumuCC1Pi_Cos_Bin[ 8] =  0.4;   FHCNumuCC1Pi_Det_Cos_Bin[ 8] =  0.4;	
    FHCNumuCC1Pi_Cos_Bin[ 9] =  0.6;   FHCNumuCC1Pi_Det_Cos_Bin[ 9] =  0.6;	
    FHCNumuCC1Pi_Cos_Bin[10] =  0.7;   FHCNumuCC1Pi_Det_Cos_Bin[10] =  0.7;	
    FHCNumuCC1Pi_Cos_Bin[11] =  0.8;   FHCNumuCC1Pi_Det_Cos_Bin[11] =  0.8;	
    FHCNumuCC1Pi_Cos_Bin[12] =  0.85;  FHCNumuCC1Pi_Det_Cos_Bin[12] =  0.85;	
    FHCNumuCC1Pi_Cos_Bin[13] =  0.90;  FHCNumuCC1Pi_Det_Cos_Bin[13] =  0.90;	
    FHCNumuCC1Pi_Cos_Bin[14] =  0.92;  FHCNumuCC1Pi_Det_Cos_Bin[14] =  0.92;	
    FHCNumuCC1Pi_Cos_Bin[15] =  0.94;  FHCNumuCC1Pi_Det_Cos_Bin[15] =  0.94;	
    FHCNumuCC1Pi_Cos_Bin[16] =  0.96;  FHCNumuCC1Pi_Det_Cos_Bin[16] =  0.96;	
    FHCNumuCC1Pi_Cos_Bin[17] =  0.98;  FHCNumuCC1Pi_Det_Cos_Bin[17] =  0.98;	
    FHCNumuCC1Pi_Cos_Bin[18] =  0.99;  FHCNumuCC1Pi_Det_Cos_Bin[18] =  0.99;	
    FHCNumuCC1Pi_Cos_Bin[19] =  1.0;   FHCNumuCC1Pi_Det_Cos_Bin[19] =  1.0;  
  }
  if(Do1DCheckMom){FHCNumuCC1Pi_Det_Cos_NBin=1; FHCNumuCC1Pi_Det_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){FHCNumuCC1Pi_Det_Mom_NBin=1; FHCNumuCC1Pi_Det_Mom_Bin[1] = 30000;}

  //FHCNumuCCOth
  int    FHCNumuCCOth_Mom_NBin       = 18;
  double FHCNumuCCOth_Mom_Bin[19]    = {0.   , 300. , 400. , 500. , 600. , 650. , 700. , 750. ,
                                        800. , 900. , 1000., 1100., 1250., 1500., 1750., 2000.,
                                        3000., 5000., 30000.};
  int    FHCNumuCCOth_Det_Mom_NBin   = 8;
  double FHCNumuCCOth_Det_Mom_Bin[9] = {0.   , 300. , 400. , 600. , 650. , 1750., 2000., 5000.,
                                        30000.};

  int    FHCNumuCCOth_Cos_NBin = 19;
  double FHCNumuCCOth_Cos_Bin[20];
  int    FHCNumuCCOth_Det_Cos_NBin = 9;
  double FHCNumuCCOth_Det_Cos_Bin[20];
  if(!Do4PiFHC){ //FHC Multi Pi
    FHCNumuCCOth_Cos_NBin = 19;         FHCNumuCCOth_Det_Cos_NBin = 9;
    FHCNumuCCOth_Cos_Bin[ 0] = -1.0  ;  FHCNumuCCOth_Det_Cos_Bin[ 0] = -1.0  ;
    FHCNumuCCOth_Cos_Bin[ 1] =  0.6  ;  FHCNumuCCOth_Det_Cos_Bin[ 1] =  0.6  ;
    FHCNumuCCOth_Cos_Bin[ 2] =  0.7  ;  FHCNumuCCOth_Det_Cos_Bin[ 2] =  0.93 ;
    FHCNumuCCOth_Cos_Bin[ 3] =  0.76 ;  FHCNumuCCOth_Det_Cos_Bin[ 3] =  0.94 ;
    FHCNumuCCOth_Cos_Bin[ 4] =  0.8  ;  FHCNumuCCOth_Det_Cos_Bin[ 4] =  0.95 ;
    FHCNumuCCOth_Cos_Bin[ 5] =  0.85 ;  FHCNumuCCOth_Det_Cos_Bin[ 5] =  0.96 ;
    FHCNumuCCOth_Cos_Bin[ 6] =  0.88 ;  FHCNumuCCOth_Det_Cos_Bin[ 6] =  0.98 ;
    FHCNumuCCOth_Cos_Bin[ 7] =  0.89 ;  FHCNumuCCOth_Det_Cos_Bin[ 7] =  0.99 ;
    FHCNumuCCOth_Cos_Bin[ 8] =  0.9  ;  FHCNumuCCOth_Det_Cos_Bin[ 8] =  0.995;
    FHCNumuCCOth_Cos_Bin[ 9] =  0.91 ;  FHCNumuCCOth_Det_Cos_Bin[ 9] =  1.0  ;
    FHCNumuCCOth_Cos_Bin[10] =  0.92 ;  
    FHCNumuCCOth_Cos_Bin[11] =  0.93 ;  
    FHCNumuCCOth_Cos_Bin[12] =  0.94 ;  
    FHCNumuCCOth_Cos_Bin[13] =  0.95 ;  
    FHCNumuCCOth_Cos_Bin[14] =  0.96 ;  
    FHCNumuCCOth_Cos_Bin[15] =  0.97 ;  
    FHCNumuCCOth_Cos_Bin[16] =  0.98 ;
    FHCNumuCCOth_Cos_Bin[17] =  0.99 ;
    FHCNumuCCOth_Cos_Bin[18] =  0.995;
    FHCNumuCCOth_Cos_Bin[19] =  1.0  ;
  }else{ //FHC 4pi
    FHCNumuCCOth_Cos_NBin = 19;        FHCNumuCCOth_Det_Cos_NBin = 19;	     
    FHCNumuCCOth_Cos_Bin[ 0] = -1.0;   FHCNumuCCOth_Det_Cos_Bin[ 0] = -1.0; 
    FHCNumuCCOth_Cos_Bin[ 1] = -0.6;   FHCNumuCCOth_Det_Cos_Bin[ 1] = -0.6; 
    FHCNumuCCOth_Cos_Bin[ 2] = -0.4;   FHCNumuCCOth_Det_Cos_Bin[ 2] = -0.4; 
    FHCNumuCCOth_Cos_Bin[ 3] = -0.2;   FHCNumuCCOth_Det_Cos_Bin[ 3] = -0.2; 
    FHCNumuCCOth_Cos_Bin[ 4] = -0.1;   FHCNumuCCOth_Det_Cos_Bin[ 4] = -0.1; 
    FHCNumuCCOth_Cos_Bin[ 5] =  0.;    FHCNumuCCOth_Det_Cos_Bin[ 5] =  0.;  
    FHCNumuCCOth_Cos_Bin[ 6] =  0.1;   FHCNumuCCOth_Det_Cos_Bin[ 6] =  0.1; 
    FHCNumuCCOth_Cos_Bin[ 7] =  0.2;   FHCNumuCCOth_Det_Cos_Bin[ 7] =  0.2; 
    FHCNumuCCOth_Cos_Bin[ 8] =  0.4;   FHCNumuCCOth_Det_Cos_Bin[ 8] =  0.4; 
    FHCNumuCCOth_Cos_Bin[ 9] =  0.6;   FHCNumuCCOth_Det_Cos_Bin[ 9] =  0.6; 
    FHCNumuCCOth_Cos_Bin[10] =  0.7;   FHCNumuCCOth_Det_Cos_Bin[10] =  0.7; 
    FHCNumuCCOth_Cos_Bin[11] =  0.8;   FHCNumuCCOth_Det_Cos_Bin[11] =  0.8; 
    FHCNumuCCOth_Cos_Bin[12] =  0.85;  FHCNumuCCOth_Det_Cos_Bin[12] =  0.85;
    FHCNumuCCOth_Cos_Bin[13] =  0.90;  FHCNumuCCOth_Det_Cos_Bin[13] =  0.90;
    FHCNumuCCOth_Cos_Bin[14] =  0.92;  FHCNumuCCOth_Det_Cos_Bin[14] =  0.92;
    FHCNumuCCOth_Cos_Bin[15] =  0.94;  FHCNumuCCOth_Det_Cos_Bin[15] =  0.94;
    FHCNumuCCOth_Cos_Bin[16] =  0.96;  FHCNumuCCOth_Det_Cos_Bin[16] =  0.96;
    FHCNumuCCOth_Cos_Bin[17] =  0.98;  FHCNumuCCOth_Det_Cos_Bin[17] =  0.98;
    FHCNumuCCOth_Cos_Bin[18] =  0.99;  FHCNumuCCOth_Det_Cos_Bin[18] =  0.99;
    FHCNumuCCOth_Cos_Bin[19] =  1.0;   FHCNumuCCOth_Det_Cos_Bin[19] =  1.0; 
  }
  if(Do1DCheckMom){FHCNumuCCOth_Det_Cos_NBin=1; FHCNumuCCOth_Det_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){FHCNumuCCOth_Det_Mom_NBin=1; FHCNumuCCOth_Det_Mom_Bin[1] = 30000;}

  //RHCANumuCC1Tr
  int    RHCANumuCC1Tr_Mom_NBin       = 10;
  double RHCANumuCC1Tr_Mom_Bin[11]    = {0.   , 400. , 500. , 600. , 700. , 800. , 900. , 1100.,
                                         1400., 2000., 10000.};
  int    RHCANumuCC1Tr_Det_Mom_NBin   = 5;
  double RHCANumuCC1Tr_Det_Mom_Bin[6] = {0.   , 400. , 900. , 1100., 2000., 10000.};
      
  int    RHCANumuCC1Tr_Cos_NBin       = 13;
  double RHCANumuCC1Tr_Cos_Bin[14]    = {-1.0 , 0.6 , 0.7 , 0.8 , 0.85, 0.88, 0.91, 0.93,
                                          0.95, 0.96, 0.97, 0.98, 0.99, 1.0};
  int    RHCANumuCC1Tr_Det_Cos_NBin   = 8;
  double RHCANumuCC1Tr_Det_Cos_Bin[9] = {-1.0 , 0.6 , 0.7 , 0.88, 0.95, 0.97, 0.98, 0.99,
                                          1.0};
    
  if(Do1DCheckMom){RHCANumuCC1Tr_Det_Cos_NBin=1; RHCANumuCC1Tr_Det_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCANumuCC1Tr_Det_Mom_NBin=1; RHCANumuCC1Tr_Det_Mom_Bin[1] = 30000;}

  //RHCANumuCCnTr
  int    RHCANumuCCnTr_Mom_NBin       = 7;
  double RHCANumuCCnTr_Mom_Bin[8]     = {0.   , 700. , 950. , 1200., 1500., 2000., 3000., 10000.};
  int    RHCANumuCCnTr_Det_Mom_NBin   = 6;
  double RHCANumuCCnTr_Det_Mom_Bin[7] = {0.   , 700. , 1200., 1500., 2000., 3000., 10000.};

  int    RHCANumuCCnTr_Cos_NBin       = 11;
  double RHCANumuCCnTr_Cos_Bin[12]    = {-1.0 , 0.75, 0.85, 0.88, 0.91, 0.93, 0.95, 0.96,
                                          0.97, 0.98, 0.99, 1.0};
  int    RHCANumuCCnTr_Det_Cos_NBin   = 6;
  double RHCANumuCCnTr_Det_Cos_Bin[7] = {-1.0 , 0.85, 0.88, 0.93, 0.98, 0.99, 1.0};

  if(Do1DCheckMom){RHCANumuCCnTr_Det_Cos_NBin=1; RHCANumuCCnTr_Det_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCANumuCCnTr_Det_Mom_NBin=1; RHCANumuCCnTr_Det_Mom_Bin[1] = 30000;}   

  //RHCNumuCC1Tr
  int    RHCNumuCC1Tr_Mom_NBin       = 6;
  double RHCNumuCC1Tr_Mom_Bin[7]     = {0.    , 400. , 600. , 800. , 1100., 2000., 10000.};
  int    RHCNumuCC1Tr_Det_Mom_NBin   = 5;
  double RHCNumuCC1Tr_Det_Mom_Bin[6] = {0.    , 400. , 800. , 1100., 2000., 10000.};

  int    RHCNumuCC1Tr_Cos_NBin       = 11;
  double RHCNumuCC1Tr_Cos_Bin[12]    = {-1.0 , 0.7 , 0.8 , 0.85, 0.90, 0.93, 0.95, 0.96,
                                         0.97, 0.98, 0.99, 1.0};
  int    RHCNumuCC1Tr_Det_Cos_NBin   = 8;
  double RHCNumuCC1Tr_Det_Cos_Bin[9] = {-1.0 , 0.7 , 0.85, 0.90, 0.93, 0.96, 0.98, 0.99,
                                         1.0};

  if(Do1DCheckMom){RHCNumuCC1Tr_Det_Cos_NBin=1; RHCNumuCC1Tr_Det_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCNumuCC1Tr_Det_Mom_NBin=1; RHCNumuCC1Tr_Det_Mom_Bin[1] = 30000;}   

  //RHCNumuCCnTr
  int    RHCNumuCCnTr_Mom_NBin       = 8;
  double RHCNumuCCnTr_Mom_Bin[9]     = {0.   , 500. , 700. , 1000., 1250., 1500., 2000., 3000.,
                                        10000.};
  int    RHCNumuCCnTr_Det_Mom_NBin   = 5;
  double RHCNumuCCnTr_Det_Mom_Bin[6] = {0.   , 1000., 1500., 2000., 3000., 10000.};

  int    RHCNumuCCnTr_Cos_NBin       = 11;
  double RHCNumuCCnTr_Cos_Bin[12]    = {-1.0 , 0.7 , 0.8 , 0.85, 0.90, 0.93, 0.95, 0.96,
                                         0.97, 0.98, 0.99, 1.0};
  int    RHCNumuCCnTr_Det_Cos_NBin   = 8;
  double RHCNumuCCnTr_Det_Cos_Bin[9] = {-1.0 , 0.8 , 0.9 , 0.93, 0.95, 0.96, 0.97, 0.99,
                                         1.0};
  
  if(Do1DCheckMom){RHCNumuCCnTr_Det_Cos_NBin=1; RHCNumuCCnTr_Det_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCNumuCCnTr_Det_Mom_NBin=1; RHCNumuCCnTr_Det_Mom_Bin[1] = 30000;} 

  //RHCANumuCC0Pi
  int    RHCANumuCC0Pi_Mom_NBin       = 17;
  double RHCANumuCC0Pi_Mom_Bin[18]    = {0.   , 300. , 400. , 500. , 550. , 600. , 650. , 700. ,
                                         750. , 800. , 900. , 1000., 1100., 1200., 1500., 2000.,
                                         4000., 30000.};
  int    RHCANumuCC0Pi_Det_Mom_NBin   = 7;
  double RHCANumuCC0Pi_Det_Mom_Bin[8] = {0.   , 300. , 400. , 500. , 550. , 2000., 4000., 30000.};
  
  int    RHCANumuCC0Pi_Cos_NBin       = 18;
  double RHCANumuCC0Pi_Cos_Bin[19]    = {-1.0  , 0.6  , 0.7  , 0.8  , 0.85 , 0.9  , 0.92 , 0.93 ,
                                          0.94 , 0.95 , 0.96 , 0.965, 0.97 , 0.975, 0.98 , 0.985,
                                          0.99 , 0.995, 1.0};
  int    RHCANumuCC0Pi_Det_Cos_NBin   = 7;
  double RHCANumuCC0Pi_Det_Cos_Bin[8] = {-1.0 , 0.6 , 0.7 , 0.8 , 0.85, 0.9 , 0.96, 1.0};

  if(Do1DCheckMom){RHCANumuCC0Pi_Det_Cos_NBin=1; RHCANumuCC0Pi_Det_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCANumuCC0Pi_Det_Mom_NBin=1; RHCANumuCC0Pi_Det_Mom_Bin[1] = 30000;} 

  //RHCANumuCC1Pi
  int    RHCANumuCC1Pi_Mom_NBin       = 6;
  double RHCANumuCC1Pi_Mom_Bin[7]     = {0.   , 500. , 700. , 900. , 1300., 2500., 30000.};
  int    RHCANumuCC1Pi_Det_Mom_NBin   = 2;
  double RHCANumuCC1Pi_Det_Mom_Bin[3] = {0.   , 500. , 30000.};
  
  int    RHCANumuCC1Pi_Cos_NBin       = 8;
  double RHCANumuCC1Pi_Cos_Bin[9]     = {-1.0 , 0.7 , 0.8 , 0.9 , 0.94, 0.96, 0.98,
                                          0.99, 1.0};
  int    RHCANumuCC1Pi_Det_Cos_NBin   = 2;
  double RHCANumuCC1Pi_Det_Cos_Bin[3] = {-1.0 , 0.7 , 1.0};
  
  if(Do1DCheckMom){RHCANumuCC1Pi_Det_Cos_NBin=1; RHCANumuCC1Pi_Det_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCANumuCC1Pi_Det_Mom_NBin=1; RHCANumuCC1Pi_Det_Mom_Bin[1] = 30000;} 

  //RHCANumuCCOth
  int    RHCANumuCCOth_Mom_NBin       = 8;
  double RHCANumuCCOth_Mom_Bin[9]     = {0.   , 600. , 800. , 1000., 1250., 1500., 2000., 4000.,
                                         30000.};
  int    RHCANumuCCOth_Det_Mom_NBin   = 3;
  double RHCANumuCCOth_Det_Mom_Bin[4] = {0.   , 600. , 800. , 30000.};
  
  int    RHCANumuCCOth_Cos_NBin       = 10;
  double RHCANumuCCOth_Cos_Bin[11]    = {-1.0 , 0.7 , 0.8, 0.85, 0.9, 0.93, 0.95, 0.97,
                                          0.98, 0.99, 1.0};
  int    RHCANumuCCOth_Det_Cos_NBin   = 2;
  double RHCANumuCCOth_Det_Cos_Bin[3] = {-1.0 , 0.7 , 1.0};

  if(Do1DCheckMom){RHCANumuCCOth_Det_Cos_NBin=1; RHCANumuCCOth_Det_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCANumuCCOth_Det_Mom_NBin=1; RHCANumuCCOth_Det_Mom_Bin[1] = 30000;} 

  //RHCNumuCC0Pi
  int    RHCNumuCC0Pi_Mom_NBin       = 10;
  double RHCNumuCC0Pi_Mom_Bin[11]    = {0.   , 300. , 500. , 700. , 800. , 900. , 1250., 1500.,
                                        2000., 4000., 30000.};
  int    RHCNumuCC0Pi_Det_Mom_NBin   = 5;
  double RHCNumuCC0Pi_Det_Mom_Bin[6] = {0.   , 300. , 500. , 700. , 800., 30000.};
  
  int    RHCNumuCC0Pi_Cos_NBin       = 12;
  double RHCNumuCC0Pi_Cos_Bin[13]    = {-1.0 , 0.7 , 0.8 , 0.85, 0.88, 0.9 , 0.92, 0.94,
                                        0.96, 0.97, 0.98, 0.99, 1.0};
  int    RHCNumuCC0Pi_Det_Cos_NBin   = 3;
  double RHCNumuCC0Pi_Det_Cos_Bin[4] = {-1.0 , 0.7 , 0.8 , 1.0};

  if(Do1DCheckMom){RHCNumuCC0Pi_Det_Cos_NBin=1; RHCNumuCC0Pi_Det_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCNumuCC0Pi_Det_Mom_NBin=1; RHCNumuCC0Pi_Det_Mom_Bin[1] = 30000;} 

  //RHCNumuCC1Pi
  int    RHCNumuCC1Pi_Mom_NBin       = 4;
  double RHCNumuCC1Pi_Mom_Bin[5]     = {0.   , 600. , 800. , 1500., 30000.};
  int    RHCNumuCC1Pi_Det_Mom_NBin   = 3;
  double RHCNumuCC1Pi_Det_Mom_Bin[4] = {0.   , 600. , 800. , 30000.};

  int    RHCNumuCC1Pi_Cos_NBin       = 10;
  double RHCNumuCC1Pi_Cos_Bin[11]    = {-1.0 , 0.7 , 0.8 , 0.86, 0.9 , 0.94, 0.96, 0.97,
                                        0.98, 0.99, 1.0};
  int    RHCNumuCC1Pi_Det_Cos_NBin   = 2;
  double RHCNumuCC1Pi_Det_Cos_Bin[3] = {-1.0 , 0.7 , 1.0};

  if(Do1DCheckMom){RHCNumuCC1Pi_Det_Cos_NBin=1; RHCNumuCC1Pi_Det_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCNumuCC1Pi_Det_Mom_NBin=1; RHCNumuCC1Pi_Det_Mom_Bin[1] = 30000;} 

  //RHCNumuCCOth
  int    RHCNumuCCOth_Mom_NBin       = 6;
  double RHCNumuCCOth_Mom_Bin[7]     = {0.   , 600. , 1000., 1250., 2000., 4000., 30000.};
  int    RHCNumuCCOth_Det_Mom_NBin   = 2;
  double RHCNumuCCOth_Det_Mom_Bin[3] = {0.   , 600. , 30000.};

  int    RHCNumuCCOth_Cos_NBin       = 9;
  double RHCNumuCCOth_Cos_Bin[10]    = {-1.0, 0.7, 0.8, 0.86, 0.9 , 0.93, 0.95, 0.97,
                                         0.99, 1.0};
  int    RHCNumuCCOth_Det_Cos_NBin   = 2;
  double RHCNumuCCOth_Det_Cos_Bin[3] = {-1.0 , 0.7 , 1.0};

  if(Do1DCheckMom){RHCNumuCCOth_Det_Cos_NBin=1; RHCNumuCCOth_Det_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCNumuCCOth_Det_Mom_NBin=1; RHCNumuCCOth_Det_Mom_Bin[1] = 30000;} 

  //FHC nue
  int    FHCNueCC_Mom_NBin   = 6;
  double FHCNueCC_Mom_Bin[7] = {200. , 400. , 600. , 800. , 1000., 1500., 30000.};
  
  int    FHCNueCC_Cos_NBin   = 4;
  double FHCNueCC_Cos_Bin[5] = {-1.0 , 0.8 , 0.9 , 0.95, 1.0};

  if(Do1DCheckMom){FHCNueCC_Cos_NBin=1; FHCNueCC_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){FHCNueCC_Mom_NBin=1; FHCNueCC_Mom_Bin[1] = 30000;} 

  //FHC anue
  int    FHCANueCC_Mom_NBin   = 4;
  double FHCANueCC_Mom_Bin[5] = {200., 300., 400., 1000., 30000.};
  
  int    FHCANueCC_Cos_NBin   = 1;
  double FHCANueCC_Cos_Bin[2] = {-1.0 , 1.0};

  if(Do1DCheckMom){FHCANueCC_Cos_NBin=1; FHCANueCC_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){FHCANueCC_Mom_NBin=1; FHCANueCC_Mom_Bin[1] = 30000;} 

  //FHC gamma
  int    FHCGamma_Mom_NBin   = 5;
  double FHCGamma_Mom_Bin[6] = {200. , 300. , 400. , 600. , 1000., 30000.};
  
  int    FHCGamma_Cos_NBin   = 1;
  double FHCGamma_Cos_Bin[2] = {-1.0 , 1.0};

  if(Do1DCheckMom){FHCGamma_Cos_NBin=1; FHCGamma_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){FHCGamma_Mom_NBin=1; FHCGamma_Mom_Bin[1] = 30000;} 

  //RHC nue
  int    RHCNueCC_Mom_NBin   = 6;
  double RHCNueCC_Mom_Bin[7] = {200. , 400. , 600. , 800. , 1000., 2000., 30000.};
  
  int    RHCNueCC_Cos_NBin   = 2;
  double RHCNueCC_Cos_Bin[3] = {-1.0 , 0.9 , 1.0};

  if(Do1DCheckMom){RHCNueCC_Cos_NBin=1; RHCNueCC_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCNueCC_Mom_NBin=1; RHCNueCC_Mom_Bin[1] = 30000;} 

  //RHC anue
  int    RHCANueCC_Mom_NBin   = 3;
  double RHCANueCC_Mom_Bin[4] = {200. , 500. , 1000., 30000.};
  
  int    RHCANueCC_Cos_NBin   = 2;
  double RHCANueCC_Cos_Bin[3] = {-1.0 , 0.9 , 1.0};

  if(Do1DCheckMom){RHCANueCC_Cos_NBin=1; RHCANueCC_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCANueCC_Mom_NBin=1; RHCANueCC_Mom_Bin[1] = 30000;} 

  //RHC gamma
  int    RHCGamma_Mom_NBin   = 5;
  double RHCGamma_Mom_Bin[6] = {200. , 400. , 600. , 800. , 1000., 30000.};
  
  int    RHCGamma_Cos_NBin   = 1;
  double RHCGamma_Cos_Bin[2] = {-1.0 , 1.0};

  if(Do1DCheckMom){RHCGamma_Cos_NBin=1; RHCGamma_Cos_Bin[1] = 1;}
  if(Do1DCheckCos){RHCGamma_Mom_NBin=1; RHCGamma_Mom_Bin[1] = 30000;} 

  
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
      bothAxis_Det[SampleId::kFGD1AntiNuMuCC0Pi] = new TAxis2D(RHCANumuCC0Pi_Det_Mom_NBin, RHCANumuCC0Pi_Det_Mom_Bin,
                                                               RHCANumuCC0Pi_Det_Cos_NBin, RHCANumuCC0Pi_Det_Cos_Bin);

      bothAxis    [SampleId::kFGD1AntiNuMuCC1Pi] = new TAxis2D(RHCANumuCC1Pi_Mom_NBin    , RHCANumuCC1Pi_Mom_Bin,
                                                               RHCANumuCC1Pi_Cos_NBin    , RHCANumuCC1Pi_Cos_Bin);
      bothAxis_Det[SampleId::kFGD1AntiNuMuCC1Pi] = new TAxis2D(RHCANumuCC1Pi_Det_Mom_NBin, RHCANumuCC1Pi_Det_Mom_Bin,
                                                               RHCANumuCC1Pi_Det_Cos_NBin, RHCANumuCC1Pi_Det_Cos_Bin);

      bothAxis    [SampleId::kFGD1AntiNuMuCCOther] = new TAxis2D(RHCANumuCCOth_Mom_NBin    , RHCANumuCCOth_Mom_Bin,
                                                                 RHCANumuCCOth_Cos_NBin    , RHCANumuCCOth_Cos_Bin);
      bothAxis_Det[SampleId::kFGD1AntiNuMuCCOther] = new TAxis2D(RHCANumuCCOth_Det_Mom_NBin, RHCANumuCCOth_Det_Mom_Bin,
                                                                 RHCANumuCCOth_Det_Cos_NBin, RHCANumuCCOth_Det_Cos_Bin);

  

      bothAxis    [SampleId::kFGD1NuMuBkgInAntiNuModeCC0Pi] = new TAxis2D(RHCNumuCC0Pi_Mom_NBin    , RHCNumuCC0Pi_Mom_Bin,
                                                                          RHCNumuCC0Pi_Cos_NBin    , RHCNumuCC0Pi_Cos_Bin);
      bothAxis_Det[SampleId::kFGD1NuMuBkgInAntiNuModeCC0Pi] = new TAxis2D(RHCNumuCC0Pi_Det_Mom_NBin, RHCNumuCC0Pi_Det_Mom_Bin,
                                                                          RHCNumuCC0Pi_Det_Cos_NBin, RHCNumuCC0Pi_Det_Cos_Bin);

      bothAxis    [SampleId::kFGD1NuMuBkgInAntiNuModeCC1Pi] = new TAxis2D(RHCNumuCC1Pi_Mom_NBin    , RHCNumuCC1Pi_Mom_Bin,
                                                                          RHCNumuCC1Pi_Cos_NBin    , RHCNumuCC1Pi_Cos_Bin);
      bothAxis_Det[SampleId::kFGD1NuMuBkgInAntiNuModeCC1Pi] = new TAxis2D(RHCNumuCC1Pi_Det_Mom_NBin, RHCNumuCC1Pi_Det_Mom_Bin,
                                                                          RHCNumuCC1Pi_Det_Cos_NBin, RHCNumuCC1Pi_Det_Cos_Bin);

      bothAxis    [SampleId::kFGD1NuMuBkgInAntiNuModeCCOther] = new TAxis2D(RHCNumuCCOth_Mom_NBin    , RHCNumuCCOth_Mom_Bin,
                                                                            RHCNumuCCOth_Cos_NBin    , RHCNumuCCOth_Cos_Bin);
      bothAxis_Det[SampleId::kFGD1NuMuBkgInAntiNuModeCCOther] = new TAxis2D(RHCNumuCCOth_Det_Mom_NBin, RHCNumuCCOth_Det_Mom_Bin,
                                                                            RHCNumuCCOth_Det_Cos_NBin, RHCNumuCCOth_Det_Cos_Bin);
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
      bothAxis_Det[SampleId::kFGD2AntiNuMuCC0Pi] = new TAxis2D(RHCANumuCC0Pi_Det_Mom_NBin, RHCANumuCC0Pi_Det_Mom_Bin,
                                                               RHCANumuCC0Pi_Det_Cos_NBin, RHCANumuCC0Pi_Det_Cos_Bin);

      bothAxis    [SampleId::kFGD2AntiNuMuCC1Pi] = new TAxis2D(RHCANumuCC1Pi_Mom_NBin    , RHCANumuCC1Pi_Mom_Bin,
                                                               RHCANumuCC1Pi_Cos_NBin    , RHCANumuCC1Pi_Cos_Bin);
      bothAxis_Det[SampleId::kFGD2AntiNuMuCC1Pi] = new TAxis2D(RHCANumuCC1Pi_Det_Mom_NBin, RHCANumuCC1Pi_Det_Mom_Bin,
                                                               RHCANumuCC1Pi_Det_Cos_NBin, RHCANumuCC1Pi_Det_Cos_Bin);

      bothAxis    [SampleId::kFGD2AntiNuMuCCOther] = new TAxis2D(RHCANumuCCOth_Mom_NBin    , RHCANumuCCOth_Mom_Bin,
                                                                 RHCANumuCCOth_Cos_NBin    , RHCANumuCCOth_Cos_Bin);
      bothAxis_Det[SampleId::kFGD2AntiNuMuCCOther] = new TAxis2D(RHCANumuCCOth_Det_Mom_NBin, RHCANumuCCOth_Det_Mom_Bin,
                                                                 RHCANumuCCOth_Det_Cos_NBin, RHCANumuCCOth_Det_Cos_Bin);

  

      bothAxis    [SampleId::kFGD2NuMuBkgInAntiNuModeCC0Pi] = new TAxis2D(RHCNumuCC0Pi_Mom_NBin    , RHCNumuCC0Pi_Mom_Bin,
                                                                          RHCNumuCC0Pi_Cos_NBin    , RHCNumuCC0Pi_Cos_Bin);
      bothAxis_Det[SampleId::kFGD2NuMuBkgInAntiNuModeCC0Pi] = new TAxis2D(RHCNumuCC0Pi_Det_Mom_NBin, RHCNumuCC0Pi_Det_Mom_Bin,
                                                                          RHCNumuCC0Pi_Det_Cos_NBin, RHCNumuCC0Pi_Det_Cos_Bin);

      bothAxis    [SampleId::kFGD2NuMuBkgInAntiNuModeCC1Pi] = new TAxis2D(RHCNumuCC1Pi_Mom_NBin    , RHCNumuCC1Pi_Mom_Bin,
                                                                          RHCNumuCC1Pi_Cos_NBin    , RHCNumuCC1Pi_Cos_Bin);
      bothAxis_Det[SampleId::kFGD2NuMuBkgInAntiNuModeCC1Pi] = new TAxis2D(RHCNumuCC1Pi_Det_Mom_NBin, RHCNumuCC1Pi_Det_Mom_Bin,
                                                                          RHCNumuCC1Pi_Det_Cos_NBin, RHCNumuCC1Pi_Det_Cos_Bin);

      bothAxis    [SampleId::kFGD2NuMuBkgInAntiNuModeCCOther] = new TAxis2D(RHCNumuCCOth_Mom_NBin    , RHCNumuCCOth_Mom_Bin,
                                                                            RHCNumuCCOth_Cos_NBin    , RHCNumuCCOth_Cos_Bin);
      bothAxis_Det[SampleId::kFGD2NuMuBkgInAntiNuModeCCOther] = new TAxis2D(RHCNumuCCOth_Det_Mom_NBin, RHCNumuCCOth_Det_Mom_Bin,
                                                                            RHCNumuCCOth_Det_Cos_NBin, RHCNumuCCOth_Det_Cos_Bin);
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
        //bothAxis.erase(sample);
        bothAxis[sample] = new TAxis2D(nBinsFlatMomBinningForNuMuComp,FlatMomBinningForNuMuComp,
                                       nBinsFlatCosBinningForNuMuComp,FlatCosBinningForNuMuComp);
      }
      if(bothAxis_Det.find(sample) != bothAxis_Det.end()){
        //bothAxis_Det.erase(sample);
        bothAxis_Det[sample] = new TAxis2D(nBinsFlatMomBinningForNuMuComp,FlatMomBinningForNuMuComp,
                                           nBinsFlatCosBinningForNuMuComp,FlatCosBinningForNuMuComp);
      }
    }
  }

  for(int i = SampleId::kUnassigned; i < SampleId::kNSamples; i++){
    SampleId::SampleEnum sample = static_cast<SampleId::SampleEnum>(i);
    

    std::map<SampleId::SampleEnum, TAxis2D*>::iterator it0 = bothAxis    .find(sample);
    std::map<SampleId::SampleEnum, TAxis2D*>::iterator it1 = bothAxis_Det.find(sample);

    ActiveSample[sample] = (it0 != bothAxis.end() && it1 != bothAxis_Det.end());

    if(ActiveSample[sample]){
      TAxis2D* ax1 = bothAxis    [sample];
      TAxis2D* ax2 = bothAxis_Det[sample];
      Axis_Mom    [sample] = (*ax1)[0];
      Axis_Cos    [sample] = (*ax1)[1];
      Axis_Mom_Det[sample] = (*ax2)[0];
      Axis_Cos_Det[sample] = (*ax2)[1];
    }
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


int BANFF::BinningDefinition::GetGlobalBinMatrixMomCos_Det(SampleId::SampleEnum sample, double Mom, double Cos){
  /// This gets the global bin number in the detector matrix
  int row = 0;
  if(Mom < 0     || Cos < -1) return -1;
  if(Mom > 30000 || Cos >  1) return -1;

  for (std::map<SampleId::SampleEnum, TAxis*>::const_iterator it=Axis_Mom_Det.begin(); it!=Axis_Mom_Det.end(); ++it){
    if(sample == (*it).first){
      TAxis* MomAxis = Axis_Mom_Det[(*it).first]; //Makes the code faster by not calling endless time the map?
      TAxis* CosAxis = Axis_Cos_Det[(*it).first];
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


int BANFF::BinningDefinition::GetGlobalBinMatrixMomCos(const SampleId::SampleEnum sample, const double Mom, const double Cos){
  /// This gets the global bin number in the matrix
  int row = 0;
  if(Mom < 0     || Cos < -1) return -1;
  if(Mom > 30000 || Cos >  1) return -1;

  for (std::map<SampleId::SampleEnum, TAxis*>::const_iterator it=Axis_Mom.begin(); it!=Axis_Mom.end(); ++it){
    if(sample == (*it).first){
      TAxis* MomAxis = Axis_Mom[(*it).first]; //Makes the code faster by not calling endless time the map?
      TAxis* CosAxis = Axis_Cos[(*it).first];
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

void BANFF::BinningDefinition::GetLocalBinMatrixMomCos(const int GlobalBin, SampleId::SampleEnum& Sample, int& MomBin, int& CosBin){
  
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
  return;

}
void BANFF::BinningDefinition::GetLocalBinMatrixMomCos_Det(const int GlobalBin, SampleId::SampleEnum& Sample, int& MomBin, int& CosBin){
  
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
  return;

}


int BANFF::BinningDefinition::GetGlobalBinMatrix_Det(const SampleId::SampleEnum sample){
  /// This gets the global bin number in the detector matrix
  int row = 0;

  for (std::map<SampleId::SampleEnum, TAxis*>::iterator it=Axis_Mom_Det.begin(); it!=Axis_Mom_Det.end(); ++it){
    if(sample == it->first)
      return row;
    row = row + GetNbins_Det(it->first);
  }
  return -1; 
}


int BANFF::BinningDefinition::GetGlobalBinMatrix(const SampleId::SampleEnum sample){
  /// This gets the global bin number in the matrix
  int row = 0;

  for (std::map<SampleId::SampleEnum, TAxis*>::iterator it=Axis_Mom.begin(); it!=Axis_Mom.end(); ++it){
    if(sample == it->first)
      return row;
    row = row + GetNbins(it->first);
  }
  return -1; 

}


int BANFF::BinningDefinition::GetNbins_Det(){
  /// This gets the total number of bins in the detector matrix
  int row = 0;

  for (std::map<SampleId::SampleEnum, TAxis*>::iterator it=Axis_Mom_Det.begin(); it!=Axis_Mom_Det.end(); ++it){
    row = row + GetNbins_Det(it->first);
  }
  return row; 

}


int BANFF::BinningDefinition::GetNbins(){
  /// This gets the total number of bins in the matrix
  int row = 0;

  for (std::map<SampleId::SampleEnum, TAxis*>::iterator it=Axis_Mom.begin(); it!=Axis_Mom.end(); ++it){
    row = row + GetNbins(it->first);
  }
  return row; 

}

SampleId::SampleEnum BANFF::BinningDefinition::GetSampleFromIndex(const int index){

  int row = 0;

  for (std::map<SampleId::SampleEnum, TAxis*>::iterator it=Axis_Mom.begin(); it!=Axis_Mom.end(); ++it){
    row = row + GetNbins(it->first);
    if(index < row)
      return it->first;
  }
  return SampleId::kUnassigned; 
}

SampleId::SampleEnum BANFF::BinningDefinition::GetSampleFromIndex_Det(const int index){

  int row = 0;

  for (std::map<SampleId::SampleEnum, TAxis*>::iterator it=Axis_Mom_Det.begin(); it!=Axis_Mom_Det.end(); ++it){
    row = row + GetNbins_Det(it->first);
    if(index < row)
      return it->first;
  }
  return SampleId::kUnassigned; 
  
}

std::vector<std::string> BANFF::BinningDefinition::GetListOfBins(){

  std::vector<std::string> BinList;
  
  for(int i = SampleId::kUnassigned; i < SampleId::kNSamples; i++){
    SampleId::SampleEnum sample = static_cast<SampleId::SampleEnum>(i);
    if(ActiveSample[sample]){
      std::string samplename = ConvertSample(sample);
      for(int ibin = 0; ibin < GetNbins(sample); ibin++)
	BinList.push_back(std::string(Form("%s_%d", samplename.c_str(), ibin)));
    }
  }
  
  return BinList;
  
}

std::vector<std::string> BANFF::BinningDefinition::GetListOfBins_Det(){

  std::vector<std::string> BinList;
  
  for(int i = SampleId::kUnassigned; i < SampleId::kNSamples; i++){
    SampleId::SampleEnum sample = static_cast<SampleId::SampleEnum>(i);
    if(ActiveSample[sample]){
      std::string samplename = ConvertSample(sample);
      for(int ibin = 0; ibin < GetNbins_Det(sample); ibin++)
	BinList.push_back(std::string(Form("%s_%d", samplename.c_str(), ibin)));
    }
  }
  
  return BinList;
  
}

// A map to be able to go from full -> reduced binning
std::map<int,int> BANFF::BinningDefinition::GetFullToReduMap(){

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




