#ifndef ColorBlindColors_hxx
#define ColorBlindColors_hxx

#include "TColor.h"

/// CB pallate taken from Brian Connelly
/// http://bconnelly.net/2013/10/creating-colorblind-friendly-figures/

namespace ColorBlindColors {

    const Int_t kcbBlack  = 2002;
    const Int_t kcbOrange = 2003;
    const Int_t kcbSky    = 2004;
    const Int_t kcbGreen  = 2005;
    const Int_t kcbBlue   = 2006;
    const Int_t kcbRed    = 2007;
    const Int_t kcbPurple = 2008;
    const Int_t kcbYellow = 2009;

    const TColor cbBlack =  TColor(kcbBlack ,0,0,0,"cbBlack");
    const TColor cbOrange = TColor(kcbOrange,.9,.6,0,"cbOrange");
    const TColor cbSky =    TColor(kcbSky   ,.35,.7,.9,"cbSky");
    const TColor cbGreen =  TColor(kcbGreen ,0,.6,.5,"cbGreen");
    const TColor cbBlue =   TColor(kcbBlue  ,0,.45,.7,"cbBlue");
    const TColor cbRed =    TColor(kcbRed   ,.8,.4,0,"cbRed");
    const TColor cbPurple = TColor(kcbPurple,.8,.6,.7,"cbPurple");
    const TColor cbYellow = TColor(kcbYellow,.95,.90,.25,"cbYellow");

    TColor* const pcbBlack =  const_cast<TColor*>(&cbBlack);
    TColor* const pcbOrange = const_cast<TColor*>(&cbOrange);
    TColor* const pcbSky =    const_cast<TColor*>(&cbSky);
    TColor* const pcbGreen =  const_cast<TColor*>(&cbGreen);
    TColor* const pcbBlue =   const_cast<TColor*>(&cbBlue);
    TColor* const pcbRed =    const_cast<TColor*>(&cbRed);
    TColor* const pcbPurple = const_cast<TColor*>(&cbPurple);
    TColor* const pcbYellow = const_cast<TColor*>(&cbYellow);

    const Int_t nColorBins = 999;
    const Double_t length4[] = {0.00,0.3333,0.6667,1.00} ;
    const Double_t fourColorR[] = {0.00,0.80,0.80,0.95};
    const Double_t fourColorG[] = {0.45,0.60,0.40,0.90};
    const Double_t fourColorB[] = {0.70,0.70,0.00,0.25};

    const Double_t length5[] = {0.00,0.25,0.50,0.75,1.00} ;
    const Double_t fiveColorR[] = {0.00,0.35,0.80,0.80,0.95};
    const Double_t fiveColorG[] = {0.45,0.70,0.60,0.40,0.90};
    const Double_t fiveColorB[] = {0.70,0.90,0.70,0.00,0.25};

    const Int_t ncb = 8;
    const TString cbColorNames[ncb] = {"cbBlack","cbOrange","cbSky","cbGreen","cbBlue","cbRed","cbPurple","cbYellow"};

    void SetCBFColorBar(int nColors = 5);

};


#endif
