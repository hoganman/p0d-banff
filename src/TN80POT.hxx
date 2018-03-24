#ifndef TN80POT_HXX
#define TN80POT_HXX

#include"Rtypes.h"
#include"TObject.h"

//The POT used in TN-80
class TN80POT: public TObject 
{

public:
    TN80POT(){SetPOT();};
    virtual ~TN80POT(){}
    Double_t GetPOTRun1WaterData() const {return _POTRun1WaterData;}
    Double_t GetPOTRun2WaterData() const {return _POTRun2WaterData;}
    Double_t GetPOTRun2AirData() const {return _POTRun2AirData;}
    Double_t GetPOTRun3AirData() const {return _POTRun3AirData;}
    Double_t GetPOTRun4WaterData() const {return _POTRun4WaterData;}
    Double_t GetPOTRun4AirData() const {return _POTRun4AirData;}
    Double_t GetPOTWaterData() const {return _POTWaterData;}
    Double_t GetPOTAirData() const {return _POTAirData;}

protected:
    void SetPOT();

protected:

    Double_t _POTRun1WaterData;
    Double_t _POTRun2WaterData;
    Double_t _POTRun2AirData;
    Double_t _POTRun3AirData;
    Double_t _POTRun4WaterData;
    Double_t _POTRun4AirData;

    Double_t _POTWaterData;
    Double_t _POTAirData;

public:
ClassDef(TN80POT,1)

};

#endif
