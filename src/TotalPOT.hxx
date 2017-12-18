/* A container for storing the POT from various runs for
 * MC: 6B
 * Data: 6M
 *
 * */

#ifndef TotalPOT_hxx
#define TotalPOT_hxx

#include"Rtypes.h"
#include"TString.h"
#include<iostream>

class TotalPOT {

public:
    TotalPOT();
    virtual ~TotalPOT(){}

    ///Using input string, get run POT
    Double_t GetPOT(const TString& runName) const;

    ///Get methods
    Double_t GetPOTRun2AirMC() const{ return _POTRun2AirMC;}
    Double_t GetPOTRun2WaterMC() const{ return _POTRun2WaterMC;}
    Double_t GetPOTRun3bAirMC() const{ return _POTRun3bAirMC;}
    Double_t GetPOTRun3cAirMC() const{ return _POTRun3cAirMC;}
    Double_t GetPOTRun4AirMC() const{ return _POTRun4AirMC;}
    Double_t GetPOTRun4WaterMC() const{ return _POTRun4WaterMC;}
    Double_t GetPOTRun5cWaterMC() const{ return _POTRun5cWaterMC;}
    Double_t GetPOTRun6bAirMC() const{ return _POTRun6bAirMC;}
    Double_t GetPOTRun6cAirMC() const{ return _POTRun6cAirMC;}
    Double_t GetPOTRun6dAirMC() const{ return _POTRun6dAirMC;}
    Double_t GetPOTRun6eAirMC() const{ return _POTRun6eAirMC;}
    Double_t GetPOTRun7WaterMC() const{ return _POTRun7bWaterMC;}

    Double_t GetPOTRun2AirData() const{ return _POTRun2AirData;}
    Double_t GetPOTRun2WaterData() const{ return _POTRun2WaterData;}
    Double_t GetPOTRun3bAirData() const{ return _POTRun3bAirData;}
    Double_t GetPOTRun3cAirData() const{ return _POTRun3cAirData;}
    Double_t GetPOTRun4AirData() const{ return _POTRun4AirData;}
    Double_t GetPOTRun4WaterData() const{ return _POTRun4WaterData;}
    Double_t GetPOTRun5cWaterData() const{ return _POTRun5cWaterData;}
    Double_t GetPOTRun6bAirData() const{ return _POTRun6bAirData;}
    Double_t GetPOTRun6cAirData() const{ return _POTRun6cAirData;}
    Double_t GetPOTRun6dAirData() const{ return _POTRun6dAirData;}
    Double_t GetPOTRun6eAirData() const{ return _POTRun6eAirData;}
    Double_t GetPOTRun7bWaterData() const{ return _POTRun7bWaterData;}
    ///run3b air separated based on the pot of data (run3b/(run3b+run3c)=0.13542
    Double_t GetRun3bAnd3cDataRatio() const{ return _Run3bAnd3cDataRatio; }
    Double_t GetPOTperFile() const{ return _POTperFile; }
    Double_t GetPOTperFile_Run6() const{ return _POTperFile_Run6; }

protected:
    ///set defaults
    void SetPOT();

private:

    Double_t _POTRun2AirMC;
    Double_t _POTRun2WaterMC;
    Double_t _POTRun3bAirMC;
    Double_t _POTRun3cAirMC;
    Double_t _POTRun4AirMC;
    Double_t _POTRun4WaterMC;
    Double_t _POTRun5cWaterMC;
    Double_t _POTRun6bAirMC;
    Double_t _POTRun6cAirMC;
    Double_t _POTRun6dAirMC;
    Double_t _POTRun6eAirMC;
    Double_t _POTRun7bWaterMC;

    Double_t _POTRun2AirData;
    Double_t _POTRun2WaterData;
    Double_t _POTRun3bAirData;
    Double_t _POTRun3cAirData;
    Double_t _POTRun4AirData;
    Double_t _POTRun4WaterData;
    Double_t _POTRun5cWaterData;
    Double_t _POTRun6bAirData;
    Double_t _POTRun6cAirData;
    Double_t _POTRun6dAirData;
    Double_t _POTRun6eAirData;
    Double_t _POTRun7bWaterData;
    Double_t _Run3bAnd3cDataRatio;

    ///the POTperFile for MC
    Double_t _POTperFile;
    Double_t _POTperFile_Run6;

};

#endif
