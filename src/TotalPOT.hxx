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
    Double_t GetPOTRun5WaterMC() const{ return _POTRun5WaterMC;}

    Double_t GetPOTRun2AirData() const{ return _POTRun2AirData;}
    Double_t GetPOTRun2WaterData() const{ return _POTRun2WaterData;}
    Double_t GetPOTRun3bAirData() const{ return _POTRun3bAirData;}
    Double_t GetPOTRun3cAirData() const{ return _POTRun3cAirData;}
    Double_t GetPOTRun4AirData() const{ return _POTRun4AirData;}
    Double_t GetPOTRun4WaterData() const{ return _POTRun4WaterData;}
    Double_t GetPOTRun5WaterData() const{ return _POTRun5WaterData;}
    ///run3b air separated based on the pot of data (run3b/(run3b+run3c)=0.13542
    Double_t GetRun3bAnd3cMCRatio() const{ return _Run3bAnd3cMCRatio; }
    Double_t GetPOTperFile() const{ return _POTperFile; }

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
    Double_t _POTRun5WaterMC;

    Double_t _POTRun2AirData;
    Double_t _POTRun2WaterData;
    Double_t _POTRun3bAirData;
    Double_t _POTRun3cAirData;
    Double_t _POTRun4AirData;
    Double_t _POTRun4WaterData;
    Double_t _POTRun5WaterData;
    Double_t _Run3bAnd3cMCRatio;

    ///the POTperFile for MC
    Double_t _POTperFile;

};

#endif
