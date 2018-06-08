#define TN80POT_CXX
#include"TN80POT.hxx"
ClassImp(TN80POT)

//**************************************************
void TN80POT::SetPOT()
//**************************************************
{
    _POTRun1WaterData = 2.946e+19;
    _POTRun2WaterData = 4.286e+19;
    _POTRun2AirData = 3.552e+19;
    _POTRun3AirData = 13.48e+19;
    _POTRun4WaterData = 16.24e+19;
    _POTRun4AirData = 15.86e+19;
    _POTWaterData = 23.47e+19;
    _POTWaterDataWithoutRun1 = _POTWaterData - _POTRun1WaterData;
    _POTAirData = 32.89e+19;
}
