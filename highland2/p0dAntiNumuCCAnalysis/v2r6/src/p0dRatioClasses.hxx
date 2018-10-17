#ifndef p0dRatioClasses_hxx
#define p0dRatioClasses_hxx

class p0dRatioPID{
public:
  
  p0dRatioPID();
  //virtual ~p0dRatioPID();

  //virtual p0dRatioPID* Clone() {
    //return new p0dRatioPID(*this);
  //}

protected:

  p0dRatioPID(const p0dRatioPID& PID);

public:  

  float ChargeRatio;
  float CloseChargeRatio;
  float ChargeMin;
  float nMuLike;
  float ChargeRatioErr;
  float PosX;
  float PosY;
  float PosZ;
  int PDG;
  int nClusters;
  int nShowers;
  int eCalVeto;

};



#endif
