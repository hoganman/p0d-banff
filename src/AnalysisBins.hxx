#ifndef ANALYSISBINS_HXX
#define ANALYSISBINS_HXX
///This class allows the user to declare a set of bins
///and vary the content (or bins themselves if you get
///around to it)
///Also the last bin can be used as overflow using the last
///bool parameter in the constructor. true means use last bin
///as overflow
#include <vector>
#include "TH1D.h"
#include "TString.h"
#include "XMLTools.hxx"

class AnalysisBins : public TObject {

public:

    typedef std::vector<Double_t> BinEdges;

    ///The default constructor
    AnalysisBins(){Init();}

    ///Copy constructor
    AnalysisBins(const AnalysisBins&);

    ///The main constructor if using Double_t array of bin edges
    AnalysisBins(TString name, Double_t* edges, Int_t nEntries, Bool_t setShowOverflow = kFALSE);

    ///The main constructor if using Float_t array of bin edges
    AnalysisBins(TString name, Float_t* edges, Int_t nEntries,  Bool_t setShowOverflow = kFALSE);

    ///The main constructor if using a histogram already constructed
    AnalysisBins(TString name, TH1D* template_hist, Bool_t setShowOverflow = kFALSE);

    ///Get the 1D binning from a config XML file
    AnalysisBins(TString name, TString configFile, XMLTools* xml=NULL);

    ///The destructor
    virtual ~AnalysisBins();

    ///Returns the name of this object assigned
    TString GetBinningName() const {return binningName;}

    ///Get the bin edges array
    const Double_t* GetBinEdges() const {return &binEdges[0];}

    ///Get the bin edges vector
    std::vector<Double_t> GetBinEdgesVect() const {return binEdges;}

    ///Returns the number of bins
    Int_t GetNbins() const {return nBins;}

    ///Indicates to use the last bin as overflow
    void SetShowOverflow(Bool_t setShowOverflow = kTRUE) {showOverflow = setShowOverflow;}

    ///Gets status flag to use the last bin as overflow
    Bool_t GetShowOverflow() const {return showOverflow;}

    ///Gets status flag to divide bins by bin width
    Bool_t GetDivideByBinWidth() const {return divideByBinWidth;}

    ///Indicates to divide by bin widths
    void SetDivideByBinWidth(Bool_t setDivideByBinWidth = kTRUE) {divideByBinWidth = setDivideByBinWidth;}

    ///Is the bin widths uniform aka the same width
    Bool_t GetIsUniform() const {return isUniform;}

    ///Return the units of this histogram
    TString GetUnits() const {return units;}

    ///Return the units of this histogram
    void SetUnits(TString setUnits){units = setUnits;}

    ///Fills the histogram taking into account overflow
    Int_t Fill(Double_t val, Double_t weight=1.0);

    ///Reset TH1D
    void Reset() {if(hist) hist->Reset();}

    ///Set to show errors
    void Sumw2(Bool_t flag = kTRUE) {if(hist) hist->Sumw2(flag);}

    ///Set to show errors
    Bool_t GetSumw2() const {return hist->GetSumw2() != NULL;}

    ///Identical to the TH1::DrawCopy() method
    void DrawCopy(TString options="") {if(hist) hist->DrawCopy(options.Data());}

    ///Clone the AnalysisBins histogram
    TH1D* GetTH1DClone(TString histName) {return (hist) ? static_cast<TH1D*>(hist->Clone(histName)) : NULL;}

    ///Vary the bin contents of the AnalysisBins histogram according to
    ///Poisson statistics. The mean of the Poisson PDF is the bin content
    TH1D* VaryPoisson();

    ///Divide each bin by its bin width. If hist->Sumw2(kTRUE), then errors are calculated
    void DivideByBinWidth(Bool_t Sumw2=kTRUE);

    Double_t GetMinimum() {return hist->GetMinimum();}

    Double_t GetMaximum() {return hist->GetMaximum();}

    void Scale(Double_t scale) {hist->Scale(scale);}

protected:

    ///Initializes the members
    void Init();

    TString binningName;
    TString units;
    Int_t nBins;
    BinEdges binEdges;
    Bool_t showOverflow;
    Bool_t divideByBinWidth;
    Bool_t isUniform;
    TH1D* hist;


ClassDef(AnalysisBins,1)

};

#endif
