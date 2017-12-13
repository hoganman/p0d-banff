#ifndef ColorBlindColors_hxx
#define ColorBlindColors_hxx

#include "TColor.h"

/// CB pallate taken from Brian Connelly
/// http://bconnelly.net/2013/10/creating-colorblind-friendly-figures/

namespace ColorBlindColors {

    //not used color IDs
    const Int_t kcbBlack  = 101;
    const Int_t kcbOrange = 102;
    const Int_t kcbSky    = 103;
    const Int_t kcbGreen  = 104;
    const Int_t kcbBlue   = 105;
    const Int_t kcbRed    = 106;
    const Int_t kcbPurple = 107;
    const Int_t kcbYellow = 108;

    const Int_t ncb = 8;
    const TString cbColorNames[ncb] = {"cbBlack","cbOrange","cbSky","cbGreen","cbBlue","cbRed","cbPurple","cbYellow"};

    extern const TColor cbBlack ;// = TColor(kcbBlack ,0,0,0,"cbBlack");
    extern const TColor cbOrange;// = TColor(kcbOrange,.9,.6,0,"cbOrange");
    extern const TColor cbSky   ;// = TColor(kcbSky   ,.35,.7,.9,"cbSky");
    extern const TColor cbGreen ;// = TColor(kcbGreen ,0,.6,.5,"cbGreen");
    extern const TColor cbBlue  ;// = TColor(kcbBlue  ,0,.45,.7,"cbBlue");
    extern const TColor cbRed   ;// = TColor(kcbRed   ,.8,.4,0,"cbRed");
    extern const TColor cbPurple;// = TColor(kcbPurple,.8,.6,.7,"cbPurple");
    extern const TColor cbYellow;// = TColor(kcbYellow,.95,.90,.25,"cbYellow");

    extern const TColor* const pcbBlack ;// =  nullptr;//const_cast<TColor*>(&cbBlack);
    extern const TColor* const pcbOrange;// = nullptr;//const_cast<TColor*>(&cbOrange);
    extern const TColor* const pcbSky   ;// =    nullptr;//const_cast<TColor*>(&cbSky);
    extern const TColor* const pcbGreen ;// =  nullptr;//const_cast<TColor*>(&cbGreen);
    extern const TColor* const pcbBlue  ;// =   nullptr;//const_cast<TColor*>(&cbBlue);
    extern const TColor* const pcbRed   ;// =    nullptr;//const_cast<TColor*>(&cbRed);
    extern const TColor* const pcbPurple;// = nullptr;//const_cast<TColor*>(&cbPurple);
    extern const TColor* const pcbYellow;// = nullptr;//const_cast<TColor*>(&cbYellow);

    const Int_t nColorBins = 999;
    const Double_t length4[]    = {0.00,0.3333,0.6667,1.00} ;
    const Double_t fourColorR[] = {0.00,0.80,0.80,0.95};
    const Double_t fourColorG[] = {0.45,0.60,0.40,0.90};
    const Double_t fourColorB[] = {0.70,0.70,0.00,0.25};

    const Double_t length5[]    = {0.00,0.25,0.50,0.75,1.00} ;
    const Double_t fiveColorR[] = {0.00,0.35,0.80,0.80,0.95};
    const Double_t fiveColorG[] = {0.45,0.70,0.60,0.40,0.90};
    const Double_t fiveColorB[] = {0.70,0.90,0.70,0.00,0.25};


    void SetCBFColorBar(int nColors = 5);

};


#endif
