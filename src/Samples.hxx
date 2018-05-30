#ifndef SAMPLES_HXX
#define SAMPLES_HXX

#include "AnalysisBins.hxx"
#include "Units.hxx"

class Samples : public TObject {

public:
    Samples();
    virtual ~Samples();

protected:

    Int_t nMomentumBinEdges;
    Int_t nCosThetaBinEdges;
    Double_t momentumBinsEdges[20]; //[nMomentumBinEdges]
    Double_t cosThetaBinsEdges[20]; //[nCosThetaBinEdges]
    AnalysisBins* momentumBins;
    AnalysisBins* cosThetaBins;

    ClassDef(Samples, 1)
};

#endif
