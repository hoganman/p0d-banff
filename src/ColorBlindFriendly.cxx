#define ColorBlindColors_cxx
#include"ColorBlindFriendly.hxx"
#include "TStyle.h"

namespace ColorBlindColors
{
    const TColor cbBlack  = TColor(kcbBlack ,0,0,0,"cbBlack");
    const TColor cbOrange = TColor(kcbOrange,.9,.6,0,"cbOrange");
    const TColor cbSky    = TColor(kcbSky   ,.35,.7,.9,"cbSky");
    const TColor cbGreen  = TColor(kcbGreen ,0,.6,.5,"cbGreen");
    const TColor cbBlue   = TColor(kcbBlue  ,0,.45,.7,"cbBlue");
    const TColor cbRed    = TColor(kcbRed   ,.8,.4,0,"cbRed");
    const TColor cbPurple = TColor(kcbPurple,.8,.6,.7,"cbPurple");
    const TColor cbYellow = TColor(kcbYellow,.95,.90,.25,"cbYellow");

    const TColor* const pcbBlack  = const_cast<TColor*>(&cbBlack);
    const TColor* const pcbOrange = const_cast<TColor*>(&cbOrange);
    const TColor* const pcbSky    = const_cast<TColor*>(&cbSky);
    const TColor* const pcbGreen  = const_cast<TColor*>(&cbGreen);
    const TColor* const pcbBlue   = const_cast<TColor*>(&cbBlue);
    const TColor* const pcbRed    = const_cast<TColor*>(&cbRed);
    const TColor* const pcbPurple = const_cast<TColor*>(&cbPurple);
    const TColor* const pcbYellow = const_cast<TColor*>(&cbYellow);
}

void ColorBlindColors::SetCBFColorBar(int nColors)
{
    if(nColors==5)
    {
	Int_t fiveColorPalette[nColorBins];
        static Int_t FI2 = TColor::CreateGradientColorTable(nColors,const_cast<Double_t*>(length5),const_cast<Double_t*>(fiveColorR),const_cast<Double_t*>(fiveColorG),const_cast<Double_t*>(fiveColorB),nColorBins);
        for (int i = 0; i < nColorBins; i++) fiveColorPalette[i]=FI2+i;
        gStyle->SetPalette(nColorBins,fiveColorPalette);
        gStyle->SetNumberContours(nColorBins);
    }
    if(nColors==4)
    {
	Int_t fourColorPalette[nColorBins];
        static Int_t FI2 = TColor::CreateGradientColorTable(nColors,const_cast<Double_t*>(length4),const_cast<Double_t*>(fourColorR),const_cast<Double_t*>(fourColorG),const_cast<Double_t*>(fourColorB),nColorBins);
        for (int i = 0; i < nColorBins; i++) fourColorPalette[i]=FI2+i;
        gStyle->SetPalette(nColorBins,fourColorPalette);
        gStyle->SetNumberContours(nColorBins);
    }
}
