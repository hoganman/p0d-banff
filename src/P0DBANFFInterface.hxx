#ifndef P0DBANFFInterface_hxx
#define P0DBANFFInterface_hxx

#include "TColor.h"

class P0DBANFFInterface : public TObject {
public:
    P0DBANFFInterface();
    virtual ~P0DBANFFInterface();

    ///Sets a nice color scale
    void LoadColorBlindPalette(Int_t nColors=5) const;

    ///Read in a TFile and check the status of the file contents
    ///If the file is good, return true;
    Bool_t CheckFile(TString& fileName) const;

    ///Have ROOT sleep for time in seconds
    ///The time is between zero (0) and "scale" seconds.
    ///For scale == 10, then ROOT will sleep between 0 to 10 seconds
    ///The seed is from the current Unix time unless otherwise stated
    void RandomSleep(const Int_t& nSeconds, Int_t seed=0) const;

    /// Split a TString into its components by a separator
    std::vector<TString> SplitString(const TString& theOpt,
        const Char_t separator);
    
    /// Convert a ROOT file with a canvas to various output formats
    void SaveCanvasAs(TString& inputFileName, const TString& canvasName,
        const TString& outputNamePrefix, const TString& formats = "eps,png,pdf",
        const Char_t delimiter = ',');

    static const Bool_t GoodFile = true;
    static const Bool_t BadFile  = false;

    static const Bool_t GoodStatus = true;
    static const Bool_t BadStatus = false;

    /// color IDs
    static const Int_t kcbBlack  = 101;
    static const Int_t kcbOrange = 102;
    static const Int_t kcbSky    = 103;
    static const Int_t kcbGreen  = 104;
    static const Int_t kcbBlue   = 105;
    static const Int_t kcbRed    = 106;
    static const Int_t kcbPurple = 107;
    static const Int_t kcbYellow = 108;

    TColor* cbBlack ;
    TColor* cbOrange;
    TColor* cbSky   ;
    TColor* cbGreen ;
    TColor* cbBlue  ;
    TColor* cbRed   ;
    TColor* cbPurple;
    TColor* cbYellow;
    ClassDef(P0DBANFFInterface,1)
};

#endif
