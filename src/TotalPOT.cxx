#define TotalPOT_cxx
#include"TotalPOT.hxx"
#include<iostream>
ClassImp(TotalPOT)

//**************************************************
void TotalPOT::SetPOT(){
//**************************************************
    //taken from https://www.t2k.org/nd280/datacomp/production006/mcp/mcProdSummary
    _POTperFile = 5E17;
    _POTperFile_Run6 = 1E17;
    _POTRun2AirMC = 1848 * _POTperFile; // 9.24E20
    _POTRun2WaterMC = 2407 * _POTperFile; // 1.2E21
    _POTRun3bAirMC = 896 * _POTperFile; // Run 3b+c TOTAL = 3.1E21
    _POTRun3cAirMC = 5264 * _POTperFile;
    _POTRun4AirMC = 6999 * _POTperFile; // 3.5E21
    _POTRun4WaterMC = 6995 * _POTperFile; //3.5E21

    //while https://www.t2k.org/nd280/datacomp/production006/mcp/mcProdSummary
    // says that there are 4197N files, only 4164 are non-zero in size
    _POTRun5cWaterMC = 4164 * _POTperFile; //2.082E21;
    _POTRun6bAirMC = 14309 * _POTperFile_Run6;
    _POTRun6cAirMC =  5325 * _POTperFile_Run6;
    _POTRun6dAirMC =  6948 * _POTperFile_Run6;
    _POTRun6eAirMC =  8680 * _POTperFile_Run6; // Run 6 Air TOTAL = 3.5E21
    _POTRun7bWaterMC = 6742 * _POTperFile; // 3.36E21

    _POTRun8WaterMC = 5292 * _POTperFile; // 2.6466E21

    _POTRun8AirMC = 7211 * _POTperFile; // 3.606E21

    _POTFHCWaterMC = _POTRun2WaterMC + _POTRun4WaterMC + _POTRun8WaterMC;
    _POTFHCAirMC = _POTRun2AirMC + _POTRun3bAirMC + _POTRun3cAirMC + _POTRun4AirMC + _POTRun8AirMC;
    _POTRHCWaterMC = _POTRun5cWaterMC + _POTRun7bWaterMC;
    _POTRHCAirMC = _POTRun6bAirMC + _POTRun6cAirMC + _POTRun6dAirMC + _POTRun6eAirMC;

    _POTFHCAirSandMC = 1.119879e+21;
    _POTRHCAirSandMC = 1.408190e+21;

    //taken from https://www.t2k.org/nd280/physics/pot_for_analyses
    _POTRun1WaterData = 1.6494E+19;
    const Double_t data_POT = 1.00E+20;
    _POTRun2AirData   = 0.35934 * data_POT;
    _POTRun2WaterData = 0.43329 * data_POT;
    _POTRun3bAirData  = 0.21727 * data_POT;
    _POTRun3cAirData  = 1.36447 * data_POT;
    _POTRun4AirData   = 1.78271 * data_POT;
    _POTRun4WaterData = 1.64277 * data_POT;

    //taken from https://www.t2k.org/nd280/runco/data/quality/inforuns
    _POTRun5cWaterData = 0.44013 * data_POT;
    _POTRun6bAirData   = 1.27412 * data_POT;
    _POTRun6cAirData   = 0.50203 * data_POT;
    _POTRun6dAirData   = 0.76662 * data_POT;
    _POTRun6eAirData   = 0.83972 * data_POT; // Run 6 Air TOTAL = 3.38E20
    _POTRun7bWaterData = 2.41221 * data_POT;

    _POTRun8WaterData = 1.580529 * data_POT;
    _POTRun8AirData   = 4.148967 * data_POT;

    //Special
    _POTFHCWaterDataWithRun1 = _POTRun1WaterData + _POTRun2WaterData + _POTRun4WaterData + _POTRun8WaterData; // 3.8215E+20;

    //Regular
    _POTFHCWaterData = _POTRun2WaterData + _POTRun4WaterData + _POTRun8WaterData; // 3.6566E+20
    _POTFHCAirData = _POTRun2AirData + _POTRun3bAirData + _POTRun3cAirData + _POTRun4AirData + _POTRun8AirData; // 7.872E+20;
    _POTRHCWaterData = _POTRun5cWaterData + _POTRun7bWaterData; // 2.852E+20;
    _POTRHCAirData = _POTRun6bAirData + _POTRun6cAirData + _POTRun6dAirData + _POTRun6eAirData; // 3.382E+20;

    //taken from ND280AnalysisUtils.cxx v3r22
    _Run3bAnd3cDataRatio = 0.13542;

}

//**************************************************
Double_t TotalPOT::GetPOT(const TString& runName) const {
//**************************************************

    if(runName.Contains("MC")){
        if(runName.Contains("Run2Air"))   return _POTRun2AirMC;
        if(runName.Contains("Run2Water")) return _POTRun2WaterMC;
        if(runName.Contains("Run3bAir"))  return _POTRun3bAirMC;
        if(runName.Contains("Run3cAir"))  return _POTRun3cAirMC;
        if(runName.Contains("Run4Air"))   return _POTRun4AirMC;
        if(runName.Contains("Run4Water")) return _POTRun4WaterMC;
        if(runName.Contains("Run5cWater")) return _POTRun5cWaterMC;
        if(runName.Contains("Run6bAir"))   return _POTRun6bAirMC;
        if(runName.Contains("Run6cAir"))   return _POTRun6cAirMC;
        if(runName.Contains("Run6dAir"))   return _POTRun6dAirMC;
        if(runName.Contains("Run6eAir"))   return _POTRun6eAirMC;
        if(runName.Contains("Run7bWater")) return _POTRun7bWaterMC;
        if(runName.Contains("Run8Water")) return _POTRun8WaterMC;
        if(runName.Contains("Run8Air"))   return _POTRun8AirMC;

	    if(runName.Contains("Run2_Air"))   return _POTRun2AirMC;
        if(runName.Contains("Run2_Water")) return _POTRun2WaterMC;
        if(runName.Contains("Run3b_Air"))  return _POTRun3bAirMC;
        if(runName.Contains("Run3c_Air"))  return _POTRun3cAirMC;
        if(runName.Contains("Run4_Air"))   return _POTRun4AirMC;
        if(runName.Contains("Run4_Water")) return _POTRun4WaterMC;
        if(runName.Contains("Run5c_Water")) return _POTRun5cWaterMC;
        if(runName.Contains("Run6b_Air"))   return _POTRun6bAirMC;
        if(runName.Contains("Run6c_Air"))   return _POTRun6cAirMC;
        if(runName.Contains("Run6d_Air"))   return _POTRun6dAirMC;
        if(runName.Contains("Run6e_Air"))   return _POTRun6eAirMC;
        if(runName.Contains("Run7b_Water")) return _POTRun7bWaterMC;
        if(runName.Contains("Run8_Water")) return _POTRun8WaterMC;
        if(runName.Contains("Run8_Air")) return _POTRun8AirMC;

        if(runName.Contains("Run3_Air_FHC")) return _POTFHCAirSandMC;
        if(runName.Contains("Run3_Air_RHC")) return _POTRHCAirSandMC;

    }
    else if(runName.Contains("Data") || runName.Contains("data")){
        if(runName.Contains("Run2Air"))   return _POTRun2AirData;
        if(runName.Contains("Run2Water")) return _POTRun2WaterData;
        if(runName.Contains("Run3bAir"))  return _POTRun3bAirData;
        if(runName.Contains("Run3cAir"))  return _POTRun3cAirData;
        if(runName.Contains("Run4Air"))   return _POTRun4AirData;
        if(runName.Contains("Run4Water")) return _POTRun4WaterData;
        if(runName.Contains("Run5cWater")) return _POTRun5cWaterData;
        if(runName.Contains("Run6bAir"))   return _POTRun6bAirData;
        if(runName.Contains("Run6cAir"))   return _POTRun6cAirData;
        if(runName.Contains("Run6dAir"))   return _POTRun6dAirData;
        if(runName.Contains("Run6eAir"))   return _POTRun6eAirData;
        if(runName.Contains("Run7bWater")) return _POTRun7bWaterData;
        if(runName.Contains("Run8Water")) return _POTRun8WaterData;
        if(runName.Contains("Run8Air")) return _POTRun8AirData;

        if(runName.Contains("Run2_Air"))   return _POTRun2AirData;
        if(runName.Contains("Run2_Water")) return _POTRun2WaterData;
        if(runName.Contains("Run3b_Air"))  return _POTRun3bAirData;
        if(runName.Contains("Run3c_Air"))  return _POTRun3cAirData;
        if(runName.Contains("Run4_Air"))   return _POTRun4AirData;
        if(runName.Contains("Run4_Water")) return _POTRun4WaterData;
        if(runName.Contains("Run5c_Water")) return _POTRun5cWaterData;
        if(runName.Contains("Run6b_Air"))   return _POTRun6bAirData;
        if(runName.Contains("Run6c_Air"))   return _POTRun6cAirData;
        if(runName.Contains("Run6d_Air"))   return _POTRun6dAirData;
        if(runName.Contains("Run6e_Air"))   return _POTRun6eAirData;
        if(runName.Contains("Run7b_Water")) return _POTRun7bWaterData;
        if(runName.Contains("Run8_Water")) return _POTRun8WaterData;
        if(runName.Contains("Run8_Air")) return _POTRun8AirData;

    }
    else{
        std::cout  << "Could not determine if data or MC" << std::endl;
    }
    return 0;

}
