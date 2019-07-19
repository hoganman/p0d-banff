#ifndef P0DBANFFInterface_hxx
#define P0DBANFFInterface_hxx

#include <vector>
#include <map>

#include "TString.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TH1.h"
#include "TH2.h"
#include "THStack.h"
#include "TROOT.h"
#include "TGaxis.h"

#include "RunName.hxx"

///The primary way that the user interacts within ROOT and pyROOT
class P0DBANFFInterface : public TObject {

public:
    P0DBANFFInterface();

    virtual ~P0DBANFFInterface();

    ///Loads all the custom colors
    void DefineColors();

    ///Define my custom style
    void DefineP0DBANFFStyle();

    ///Define the T2K style
    void DefineT2KStyle(Int_t WhichStyle=1);

    ///Same as calling gROOT->SetTyle(style)
    void SetStyle(const char* style) const {gROOT->SetStyle(style);}

    ///Read in a TFile and check the status of the file contents
    ///If the file is good, return GoodFile == kTRUE;
    static Bool_t CheckFile(TString fileName);
    static inline Bool_t IsGoodFile(TString fileName)
    {
        return CheckFile(fileName) == GoodFile;
    }
    ///Have ROOT sleep for time in seconds
    ///The time is between zero (0) and "scale" seconds.
    ///For scale == 10, then ROOT will sleep between 0 to 10 seconds
    ///The seed is from the current Unix time unless otherwise stated
    static void RandomSleep(Int_t nSeconds, Int_t seed=0);

    ///Split a TString into its components by a separator
    static std::vector<TString> SplitString(TString theOpt, Char_t separator);

    ///For a given TCanvas, get the TH1
    static TH1* GetTH1FromCanvas(TCanvas* inCanvas, TString histName);

    ///For a given TCanvas, get the TH2
    static TH2* GetTH2FromCanvas(TCanvas* inCanvas, TString histName);

    ///For a given TFile, get the TCanvas
    static TCanvas* GetTCanvasFromFile(TFile* inFile, TString canvasName);

    ///Convert a ROOT file with a canvas to various other file formats
    ///To specify different file formats, separate them by a char delimiter
    static void SaveCanvasAs(TString inputFileName, TString canvasName = "c1",
        TString outputNamePrefix = "", TString formats = "root,png",
        Char_t delimiter = ',');

    ///Save a canvas to various file formats. Default is eps, png, pdf, and root
    ///To specify different file formats, separate them by a char delimiter
    static void SaveCanvasAs(TCanvas* const canvas, TString outputNamePrefix,
        TString formats = "root,png", Char_t delimiter = ',');

    ///Applies a set of styles and font size to make
    ///canvases easier to see in presentations
    static void PrettyUpTH1(TString inFileName, TString outputName, TString canvasName = "c1",
        TString histName = "", TString xAxisTitle = "none",
        TString yAxisTitle = "none", TString saveAsFormats = "root,png",
        Char_t delimiter = ',', Int_t lineColor = P0DBANFFInterface::kcbBlue,
        Int_t fillColor = 0, Int_t lineWidth = 3, Double_t textSizeChange = 0.0);

    ///Applies a set of styles and font size to make
    ///canvases easier to see in presentations
    static void PrettyUpTH1(TH1* inHist, TString xAxisTitle = "none",
        TString yAxisTitle = "none",
        Int_t lineColor = P0DBANFFInterface::kcbBlue, Int_t fillColor = 0,
        Int_t lineWidth = 3, Double_t textSizeChange = 0.0);

    ///Applies a set of styles and font size to make
    ///canvases easier to see in presentations
    static void PrettyUpTHStack(THStack* stack, TString xAxisTitle = "none",
        TString yAxisTitle = "none", Double_t textSizeChange = 0.0);

    ///Applies a set of styles and font size to make
    ///canvases easier to see in presentations
    static void PrettyUpTH2(TString inFileName, TString outputName, TString canvasName = "c1",
        TString histName = "", TString xAxisTitle = "none",
        TString yAxisTitle = "none", TString saveAsFormats = "root,png",
        Char_t delimiter = ',', Double_t textSizeChange = 0.0);

    ///Applies a set of styles and font size to make
    ///canvases easier to see in presentations
    static void PrettyUpTH2(TH2* inHist, TString xAxisTitle = "none",
        TString yAxisTitle = "none", Double_t textSizeChange = 0.0);

    ///Get the TStyle pointer
    TStyle* GetThisStyle() const {TGaxis::SetMaxDigits(3); return P0DBANFFStyle;}

    ///Get the TStyle pointer
    TStyle* GetT2KStyle();

    ///Set Batch mode
    void SetBatch(Bool_t batch=kTRUE) const {gROOT->SetBatch(batch);}

    ///To convert sci notation for stringification
    static Int_t GetExponentBase10(Double_t arg);
    static Double_t GetMantissaBase10(Double_t arg, Int_t exp=9999);

    static TObject* FindObjectInFileByName(TFile* inFile, const TString& name_search);

    inline static void Warning(const TObject* obj, const TString &msg)
    {
        P0DBANFFInterface::Message(obj->IsA()->GetName(), "WARNING", msg);
    }

    inline static void Warning(const TString &className, const TString &msg)
    {
        P0DBANFFInterface::Message(className, "WARNING", msg);
    }

    inline static void Error(const TObject* obj, const TString &msg)
    {
        P0DBANFFInterface::Message("ERROR", obj->IsA()->GetName(), msg);
    }

    inline static void Error(const TString &className, const TString &msg)
    {
        P0DBANFFInterface::Message(className, "ERROR", msg);
    }

    inline static void Announce(const TObject* obj, const TString &msg)
    {
        P0DBANFFInterface::Message(obj->IsA()->GetName(), "ANNOUCEMENT", msg);
    }

    inline static void Announce(const TString &className, const TString &msg)
    {
        P0DBANFFInterface::Message(className, "ANNOUCEMENT", msg);
    }

    inline static void Message(const TString &className, const TString &type, const TString &msg)
    {
        printf("%s::%s %s\n", className.Data(), type.Data(), msg.Data());
    }

    std::vector<Int_t> GetStackColors() const {return std::vector<Int_t>(StackColors);}

protected:

    std::vector<Int_t> StackColors;

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

    //taken from https://personal.sron.nl/~pault/
    static const Int_t kcbBrightBlue = 109;
    static const Int_t kcbBrightCyan = 2110;
    static const Int_t kcbBrightGreen = 111;
    static const Int_t kcbBrightYellow = 112;
    static const Int_t kcbBrightPink = 113; //called red
    static const Int_t kcbBrightPurple = 114;
    static const Int_t kcbBrightGrey = 115;

    //taken from https://personal.sron.nl/~pault/
    static const Int_t kcbVibrantBlue = 116;
    static const Int_t kcbVibrantCyan = 117;
    static const Int_t kcbVibrantGreen = 118; //called teal
    static const Int_t kcbVibrantOrange = 119;
    static const Int_t kcbVibrantRed = 120;
    static const Int_t kcbVibrantMagenta = 121;
    //static const Int_t kcbVibrantGrey = 122;

    //taken from https://personal.sron.nl/~pault/
    static const Int_t kcbMutedBlue = 123;
    static const Int_t kcbMutedCyan = 124;
    static const Int_t kcbMutedTeal = 125;
    static const Int_t kcbMutedGreen = 126;
    static const Int_t kcbMutedOlive = 127;
    static const Int_t kcbMutedYellow = 128; //called sand
    static const Int_t kcbMutedPink = 129; //called rose
    static const Int_t kcbMutedWine = 130;
    static const Int_t kcbMutedPurple = 131;
    static const Int_t kcbMutedGrey = 132;

    static const Int_t kDataMarkerStyle = 21;
    static const Int_t kThickDashedLineStyle = 9;
    static const Int_t kDashedLineStyle = 7;
    static const Int_t kDottedLineStyle = 2;
    static const Int_t kSolidLineStyle = 1;

protected:

    //taken from http://bconnelly.net/2013/10/creating-colorblind-friendly-figures/
    TColor* cbBlack;   //!
    TColor* cbOrange;   //!
    TColor* cbSky;   //!
    TColor* cbGreen;   //!
    TColor* cbBlue;   //!
    TColor* cbRed;   //!
    TColor* cbPurple;   //!
    TColor* cbYellow;   //!

    //taken from https://personal.sron.nl/~pault/
    TColor* cbBrightBlue;   //!
    TColor* cbBrightCyan;   //!
    TColor* cbBrightGreen;   //!
    TColor* cbBrightYellow;   //!
    TColor* cbBrightPink;   //!
    TColor* cbBrightPurple;   //!
    TColor* cbBrightGrey;   //!

    //taken from https://personal.sron.nl/~pault/
    TColor* cbVibrantBlue;   //!
    TColor* cbVibrantCyan;   //!
    TColor* cbVibrantGreen;   //!
    TColor* cbVibrantOrange;   //!
    TColor* cbVibrantRed;   //!
    TColor* cbVibrantMagenta;   //!

    //taken from https://personal.sron.nl/~pault/
    TColor* cbMutedBlue;   //!
    TColor* cbMutedCyan;   //!
    TColor* cbMutedTeal;   //!
    TColor* cbMutedGreen;   //!
    TColor* cbMutedOlive;   //!
    TColor* cbMutedYellow;   //!
    TColor* cbMutedPink;   //!
    TColor* cbMutedWine;   //!
    TColor* cbMutedPurple;   //!
    TColor* cbMutedGrey;   //!

    TStyle* P0DBANFFStyle;   //!
    TStyle* T2KStyle;
    std::map<Int_t, Int_t> pdgColorCodes;

public:

    //Copied from ROOT6, all but kP0DBANFF and kInvertedDarkBodyRadiator are supported
    enum EColorPalette {kP0DBANFF=1, kInvertedDarkBodyRadiator=56, kRedWhiteBlue};

    EColorPalette GetP0DBANFFColorPalette() const {return kP0DBANFF;}
    EColorPalette GetInvertedDarkBodyRadiatorColorPalette() const {return kInvertedDarkBodyRadiator;}
    EColorPalette GetRedWhiteBlueColorPalette() const {return kRedWhiteBlue;}
    ///Sets a nice color scale
    void LoadColorBlindPalette(EColorPalette palette = kP0DBANFF) const;

public:

    ClassDef(P0DBANFFInterface,1)
};


#endif
