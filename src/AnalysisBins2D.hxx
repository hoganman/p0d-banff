#ifndef ANALYSISBINS2D_HXX
#define ANALYSISBINS2D_HXX
///This class is a 2D implementation of AnalysisBins
#include <vector>
#include "TH2D.h"
#include "TCanvas.h"
#include "TString.h"
#include "XMLTools.hxx"
#include "AnalysisBins.hxx"

class AnalysisBins2D : public TObject
{

public:

    ///The default constructor
    AnalysisBins2D(){Init();}

    ///The main constructor
    AnalysisBins2D(TString xBinsName, TString yBinsName, TString configFile, XMLTools* xml=NULL);

    ///Destructor
    virtual ~AnalysisBins2D();

    ///Returns the name of this object assigned
    TString GetBinningName() const {return binningName;}

    ///Get the bin edges array
    const Double_t* GetBinEdgesX() const {return &binEdgesX[0];}

    ///Get the bin edges array
    const Double_t* GetBinEdgesY() const {return &binEdgesY[0];}

    ///Returns the number of bins
    Int_t GetNbins() const {return nBins;}

    ///Return the units of this histogram X dim
    TString GetUnitsX() const {return unitsX;}

    ///Return the units of this histogram Y dim
    TString GetUnitsY() const {return unitsY;}

    ///Set the units of this histogram X dim
    void SetXunits(TString setUnits) {unitsX = setUnits;}

    ///Set the units of this histogram Y dim
    void SetYunits(TString setUnits) {unitsY = setUnits;}

    ///Fills the histogram taking into account overflow
    Int_t Fill(Double_t x_val, Double_t y_val, Double_t weight=1.0);

    ///Reset all histograms
    void Reset();

    ///Identical to the TH1::DrawCopy() method
    void DrawCopy(TString options="") {if(hist) hist->DrawCopy(options.Data());}

    ///Draws a 3x1 TCanvas with each histogram
    TCanvas* DrawAll(TString twoDimOptions, TString oneDimOptions);

    ///Clone the AnalysisBins histogram
    TH2D* GetTH2DClone(TString histName) {return (hist) ? static_cast<TH2D*>(hist->Clone(histName)) : NULL;}

    AnalysisBins* anaBinsX;
    AnalysisBins* anaBinsY;

protected:
    void Init();
    TH2D* hist;
    TString binningName;
    Int_t nBins;
    Int_t nBinsX;
    Int_t nBinsY;
    TString unitsX;
    TString unitsY;
    AnalysisBins::BinEdges binEdgesX;
    AnalysisBins::BinEdges binEdgesY;

public:
    ClassDef(AnalysisBins2D, 1)
};

#endif
