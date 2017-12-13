#define TotalPOT_cxx

#include"TotalPOT.hxx"

//**************************************************
TotalPOT::TotalPOT(){
//**************************************************
    SetPOT();
}

//**************************************************
TotalPOT::~TotalPOT(){
//**************************************************
}

//**************************************************
void TotalPOT::SetPOT(){
//**************************************************
    //taken from https://www.t2k.org/nd280/datacomp/production006/mcp/mcProdSummary
    _POTperFile = 5E17;
    _POTRun2AirMC = 1848 * _POTperFile;
    _POTRun2WaterMC = 2407 * _POTperFile;
    _POTRun3bAirMC = 896 * _POTperFile;
    _POTRun3cAirMC = 5264 * _POTperFile;
    _POTRun4AirMC = 6999 * _POTperFile;
    _POTRun4WaterMC = 6995 * _POTperFile;
    _POTRun5WaterMC = 4164 * _POTperFile;
    
    const Double_t data_POT = 1E20;
    _POTRun2AirData =  0.35934 * data_POT;
    _POTRun2WaterData = 0.43329 * data_POT;
    _POTRun3bAirData = 0.21727 * data_POT;
    _POTRun3cAirData = 1.36447 * data_POT;
    _POTRun4AirData = 1.78271 * data_POT;
    _POTRun4WaterData = 1.64277 * data_POT;
    //taken from https://www.t2k.org/nd280/runco/data/quality/inforuns
    _POTRun5WaterData = 1.9876e+19 + 4.4013e+19;

    //taken from ND280AnalysisUtils.cxx v3r22
    _Run3bAnd3cMCRatio = 0.13542;

}
