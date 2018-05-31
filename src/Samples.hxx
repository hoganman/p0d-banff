#ifndef SAMPLES_HXX
#define SAMPLES_HXX

#include "AnalysisBins.hxx"
#include "HEPConstants.hxx"

class Samples : public TObject {

public:
    Samples();
    ///The name of the sample must be the same as in the config XML file
    Samples(TString name, TString configFile);
    virtual ~Samples();

    ///Get the AnalysisBins associated with momentum binning
    AnalysisBins* GetAnalysisMomentumBins() {return momentumBins;}

    ///Get the AnalysisBins associated with cosTheta binning
    AnalysisBins* GetAnalysisCosThetaBins() {return cosThetaBins;}

protected:

    Int_t nMomentumBinEdges;
    Int_t nCosThetaBinEdges;
    Double_t momentumBinsEdges[20];
    Double_t cosThetaBinsEdges[20];
    AnalysisBins* momentumBins;
    AnalysisBins* cosThetaBins;

    ClassDef(Samples, 1)
};

#endif
