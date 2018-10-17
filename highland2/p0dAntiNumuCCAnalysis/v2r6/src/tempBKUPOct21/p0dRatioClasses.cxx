#define DataClasses_C

#include"p0dRatioClasses.hxx"


p0dRatioPID::p0dRatioPID(){

  ChargeRatio = 555.;
  PDG = -9999;
  nClusters = -9999;
}


p0dRatioPID::p0dRatioPID(const p0dRatioPID& p0dPID){

  ChargeRatio = p0dPID.ChargeRatio;
  PDG = p0dPID.PDG;
  nClusters = p0dPID.nClusters;
}


