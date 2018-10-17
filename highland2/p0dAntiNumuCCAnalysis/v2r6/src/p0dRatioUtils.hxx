#ifndef p0dRatioNumuUtils_h
#define p0dRatioNumuUtils_h

#include "AnalysisUtils.hxx"
#include "P0DAnalysisUtils.hxx"
#include "p0dRatioClasses.hxx"


namespace p0dRatioUtils{

  int TagMichel(AnaP0DVertex** p0dVertices, int nVerts, int* nMichelOut);
  int MipPID(AnaP0DVertex** p0dVertices, int nVerts, p0dRatioPID* p0dRatioPIDOut, int nTracks, float* temp_selmu_pos);
  
}

#endif
