#define DataClasses_C

#include"p0dRatioClasses.hxx"


p0dRatioPID::p0dRatioPID(){

  ChargeRatio = 555.;
  CloseChargeRatio = 555.;
  ChargeMin = -555;
  ChargeRatioErr = 0.0;
  nMuLike = 1;
  PosX = -9999.0;
  PosY = -9999.0;
  PosZ = -9999.0;
  PDG = -9999;
  nClusters = -9999;
  nShowers = 0;
  eCalVeto = -1;
}


p0dRatioPID::p0dRatioPID(const p0dRatioPID& p0dPID){

  ChargeRatio = p0dPID.ChargeRatio;
  CloseChargeRatio = p0dPID.CloseChargeRatio;
  ChargeMin = p0dPID.ChargeMin;
  ChargeRatioErr = p0dPID.ChargeRatioErr;
  nMuLike = p0dPID.nMuLike;
  PosX = p0dPID.PosX;
  PosY = p0dPID.PosY;
  PosZ = p0dPID.PosZ;
  PDG = p0dPID.PDG;
  nClusters = p0dPID.nClusters;
  nShowers = p0dPID.nShowers;
  eCalVeto = p0dPID.eCalVeto;
}


