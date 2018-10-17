#ifndef p0dRatioNumuUtils_h
#define p0dRatioNumuUtils_h

#include "AnalysisUtils.hxx"
#include "P0DAnalysisUtils.hxx"


namespace p0dRatioUtils{

  int TagMichel(AnaP0DVertex** p0dVertices, int nVerts, int* nMichelOut);
  int MipPID(AnaP0DVertex** p0dVertices, int nVerts, float* TrkChargeRatioOut, float* TrkChargeRatioPDGsOut, int nTracks);
  
}

#endif
