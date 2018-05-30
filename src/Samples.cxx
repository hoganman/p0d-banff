#define SAMPLES_CXX

#include "Samples.hxx"
ClassImp(Samples)

//**************************************************
Samples::Samples()
//**************************************************
{
    nMomentumBinEdges = 0;
    nCosThetaBinEdges = 0;
    momentumBins = NULL;
    cosThetaBins = NULL;
}


//**************************************************
Samples::~Samples()
//**************************************************
{
    if(momentumBins) delete momentumBins;
    if(cosThetaBins) delete cosThetaBins;
}
