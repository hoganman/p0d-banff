#ifndef ANALYSISBINS_HXX
#define ANALYSISBINS_HXX
///This class allows the user to declare a set of bins
///and vary the content (or bins themselves if you get
///around to it)
///Also the last bin can be used as overflow using the last
///bool parameter in the constructor. true means use last bin
///as overflow

#include "TH1D.h"
#include "TString.h"

class AnalysisBins : public TObject {

public:

    ///The default constructor
    AnalysisBins(){}

    ///The main constructor if using Double_t array of bin edges
    AnalysisBins(TString name, Double_t* edges, Int_t nEntries, Bool_t setShowOverflow = kFALSE);

    ///The main constructor if using Float_t array of bin edges
    AnalysisBins(TString name, Float_t* edges, Int_t nEntries,  Bool_t setShowOverflow = kFALSE);

    ///The main constructor if using a histogram already constructed
    AnalysisBins(TString name, TH1D* template_hist, Bool_t setShowOverflow = kFALSE);

    ///The destructor
    ~AnalysisBins();

    ///Returns the name of this object assigned
    TString GetBinningName() const {return binningName;}

    ///Get the bin edges array
    Double_t* GetBinEdges() const {return binEdges;}

    ///Returns the number of bins
    Int_t GetNbins() const {return nBins;}

    ///Indicates to use the last bin as overflow
    void SetShowOverflow(Bool_t setShowOverflow = kTRUE) {showOverflow = setShowOverflow;}

    ///Fills the histogram taking into account overflow
    Int_t Fill(Double_t val, Double_t weight=1.0);

    ///Identical to the TH1::DrawCopy() method 
    void DrawCopy(TString options) {hist->DrawCopy(options.Data());}

    ///Clone the AnalysisBins histogram
    TH1D* GetTH1DClone(TString histName) {return static_cast<TH1D*>(hist->Clone(histName));}

    ///Vary the bin contents of the AnalysisBins histogram according to
    ///Poisson statistics. The mean of the Poisson PDF is the bin content
    TH1D* VaryPoisson();

protected:

    TString binningName;
    Int_t nBins;
    Double_t* binEdges; //[nBins]
    Bool_t showOverflow;
    TH1D* hist;


ClassDef(AnalysisBins,1)

};

#endif
