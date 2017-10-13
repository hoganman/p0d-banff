#define ColorBlindColors_cxx
#include"ColorBlindFriendly.hxx"
#include "TStyle.h"

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
