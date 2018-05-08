#ifndef P0DBANFFInterface_hxx
#define P0DBANFFInterface_hxx

#include <vector>

#include "TString.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TH1.h"
#include "THStack.h"
#include "TROOT.h"

///The primary way that the user interacts within ROOT and pyROOT
class P0DBANFFInterface : public TObject {

public:
    P0DBANFFInterface();
    virtual ~P0DBANFFInterface();

    ///Sets a nice color scale
    void LoadColorBlindPalette(Int_t nColors=5) const;

    ///Read in a TFile and check the status of the file contents
    ///If the file is good, return kTRUE;
    Bool_t CheckFile(TString fileName) const;

    ///Have ROOT sleep for time in seconds
    ///The time is between zero (0) and "scale" seconds.
    ///For scale == 10, then ROOT will sleep between 0 to 10 seconds
    ///The seed is from the current Unix time unless otherwise stated
    void RandomSleep(Int_t nSeconds, Int_t seed=0) const;

    ///Split a TString into its components by a separator
    std::vector<TString> SplitString(TString theOpt, Char_t separator) const;

    ///Convert a ROOT file with a canvas to various other file formats
    ///To specify different file formats, separate them by a char delimiter
    void SaveCanvasAs(TString inputFileName, TString canvasName = "c1",
        TString outputNamePrefix = "", TString formats = "eps,png,pdf,root",
        Char_t delimiter = ',') const;

    ///Save a canvas to various file formats. Default is eps, png, pdf, and root
    ///To specify different file formats, separate them by a char delimiter
    void SaveCanvasAs(TCanvas* const canvas, TString outputNamePrefix,
        TString formats = "eps,png,pdf,root", Char_t delimiter = ',') const;

    ///Applies a set of styles and font size to make
    ///canvases easier to see in presentations
    void PrettyUpTH1(TString inFileName, TString outputName, TString canvasName = "c1",
        TString histName = "", TString xAxisTitle = "none",
        TString yAxisTitle = "none",
        UInt_t lineColor = P0DBANFFInterface::kcbBlue, UInt_t fillColor = 0,
        UInt_t lineWidth = 3, Double_t textSizeChange = 0.0) const;

    ///Applies a set of styles and font size to make
    ///canvases easier to see in presentations
    void PrettyUpTH1(TH1* inHist, TString xAxisTitle = "none",
        TString yAxisTitle = "none",
        UInt_t lineColor = P0DBANFFInterface::kcbBlue, UInt_t fillColor = 0,
        UInt_t lineWidth = 3, Double_t textSizeChange = 0.0) const;

    ///Applies a set of styles and font size to make
    ///canvases easier to see in presentations
    void PrettyUpTHStack(THStack* stack, TString xAxisTitle = "none",
        TString yAxisTitle = "none", Double_t textSizeChange = 0.0) const;


protected:

    TObject* FindObjectInFileByName(const TList* inList, const TString name_search) const;

public:

    static const Bool_t GoodFile = kTRUE;
    static const Bool_t BadFile  = kFALSE;

    static const Bool_t GoodStatus = kTRUE;
    static const Bool_t BadStatus = kFALSE;

    /// color IDs
    static const Int_t kcbBlack  = 101;
    static const Int_t kcbOrange = 102;
    static const Int_t kcbSky    = 103;
    static const Int_t kcbGreen  = 104;
    static const Int_t kcbBlue   = 105;
    static const Int_t kcbRed    = 106;
    static const Int_t kcbPurple = 107;
    static const Int_t kcbYellow = 108;

protected:

    TColor* cbBlack ;
    TColor* cbOrange;
    TColor* cbSky   ;
    TColor* cbGreen ;
    TColor* cbBlue  ;
    TColor* cbRed   ;
    TColor* cbPurple;
    TColor* cbYellow;
    TStyle* P0DBANFFStyle;

public:

    ///Get the TStyle pointer
    TStyle* GetThisStyle() const {return P0DBANFFStyle;}

    ///Set Batch mode
    void SetBatch(Bool_t batch=kTRUE) const {gROOT->SetBatch(batch);}

    ClassDef(P0DBANFFInterface,1)

};

#endif
