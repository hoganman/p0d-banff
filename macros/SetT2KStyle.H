#ifndef SetT2KStyle_H_
#define SetT2KStyle_H_

/*
 * This set of styles were taken from t2k.org. 
 * Added functionality to switch between the CC1PI set and T2K sets
*/
// -- WhichStyle --
// 1 = presentation large fonts
// 2 = presentation small fonts
// 3 = publication/paper
// 4 = CC1Pi SetStyle

#include<iostream>
#include"TPad.h"
#include"TString.h"
#include"TStyle.h"
#include"TColor.h"
#include"TH1.h"
#include"SetStyles.h"

//CINT does not know about nullptr
#ifndef nullptr
#define nullptr NULL
#endif

enum PresentationStyle { PresentationLargeFont = 1,PresentationSmallFont, PublicationPaper, CC1PI, Other};

//user friendly
void CenterHistoTitles(TH1 *thisHisto);
int AddGridLinesToPad(TPad *thisPad);
void SetAxesTitles(TH1 *thisHisto,const TString &xTitle,const TString &yTitle);
//use this function if you want to give the style a name you like
TStyle* SetPlotStyle(const Int_t &WhichStyle = 1, const TString &styleName = "T2K Large Font");
//use the predefined style names
TStyle* SetT2KLargeFontStyle();
TStyle* SetT2KSmallFontStyle();
TStyle* SetT2KPaperStyle();
TStyle* SetCC1PIStyle();
TStyle* SetOtherStyle();

class PlotStyleHelper {
public:
  PlotStyleHelper(){}
  virtual ~PlotStyleHelper(){}
  static PlotStyleHelper& Get(){
    static PlotStyleHelper singleton;
    return singleton;
  }
  TStyle* SetStyle(const Int_t& WhichStyle = 1, const TString& styleName = "T2K Large Font") {
    return SetPlotStyle(static_cast<const PresentationStyle>(WhichStyle),styleName); 
  }
//not intended for user
private:
  TStyle* SetT2KPlotStyle(const PresentationStyle &WhichStyle = PresentationLargeFont,const TString &styleName = "T2K");
  TStyle* SetPlotStyle(const PresentationStyle &WhichStyle = PresentationLargeFont,const TString &styleName = "T2K");
};

TStyle* PlotStyleHelper::SetPlotStyle(const PresentationStyle &WhichStyle,const TString &styleName){

  TString userNotify = "Using style: ";
  if(WhichStyle == CC1PI){
    TStyle* CC1PIStyle = SetStyles();
    userNotify = userNotify+"CC1PI";
    std::cout << userNotify.Data() << std::endl;
    return CC1PIStyle;
  }
  else if(WhichStyle == PresentationLargeFont || WhichStyle == PresentationSmallFont || WhichStyle == PublicationPaper){

    switch(WhichStyle) {
    case PresentationLargeFont:
      userNotify = userNotify+"T2K Large Font ";
      break;
    case PresentationSmallFont:
      userNotify = userNotify+"T2K Small Font ";
      break;
    case PublicationPaper:
      userNotify = userNotify+"T2K Publication ";
      break;
    default:
      userNotify = "Should never get here ";
      break;
    }
    std::cout << userNotify.Data() << std::endl;

    // T2K style
    TStyle* t2kstyle = SetT2KPlotStyle(WhichStyle, styleName);
    // -- axis --
    t2kstyle->SetStripDecimals(kFALSE); // don't do 1.0 -> 1
    gROOT->SetStyle(t2kstyle->GetName());

    return t2kstyle;
  }
  else if(WhichStyle == Other){

    userNotify = userNotify+"Custom ";
    std::cout << userNotify.Data() << std::endl;

    TStyle *OtherStyle = new TStyle("Other Style","A general all purpose set of styles that can aid T2K styles");
    // -- margin --
    OtherStyle->SetPadTopMargin(0.05);
    OtherStyle->SetPadBottomMargin(0.16);
    OtherStyle->SetPadRightMargin(0.075);//more space if no colz option 
    OtherStyle->SetPadLeftMargin(0.165); 

    // -- title/lable offset --
    OtherStyle->SetTitleOffset(1.5, "x");
    OtherStyle->SetTitleOffset(1.7, "y");
    OtherStyle->SetLabelOffset(0.02, "x");
    OtherStyle->SetLabelOffset(0.02, "y");

    // -- title/label size --
    OtherStyle->SetTitleSize(0.04, "x"); 
    OtherStyle->SetTitleSize(0.04, "y"); 
    OtherStyle->SetTitleSize(0.04, "z"); 
    OtherStyle->SetLabelSize(0.034,"x"); 
    OtherStyle->SetLabelSize(0.034,"y"); 
    OtherStyle->SetLabelSize(0.034,"z"); 

    // -- statistic and title info --
    //OtherStyle->SetOptTitle(1);
    //OtherStyle->SetOptStat(1111);

    // -- lines --
    OtherStyle->SetLineWidth(4);

    // -- fills/color --
    OtherStyle->SetFrameFillColor(0); // white color for backgroud
    OtherStyle->SetFillColor(1);

    // -- color scheme --
    // - "warm"/red-ish -
    //  const Int_t NRGBs = 3;
    //  const Int_t NCont = 500;
    //  Double_t red[]   = {1.00, 1.00, 0.25 };
    //  Double_t green[] = {1.00, 0.00, 0.00 };
    //  Double_t blue[]  = {0.00, 0.00, 0.00 };
    //  Double_t stops[] = { 0.25, 0.75, 1.00 };
    //  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    //  OtherStyle->SetNumberContours(NCont);   
    //  - rainbow -
    //  OtherStyle->SetPalette(1);  // use the rainbow color set

    // -- horizontal error bars back --
    //  OtherStyle->SetErrorX(0.5);

    // -- transparent stuff --
    OtherStyle->SetFillStyle(0);

    //TGaxis::SetMaxDigits(3); // doesn't seem to work :<
    // supressed zeroes!
    OtherStyle->SetStripDecimals(kFALSE); // don't do 1.0 -> 1
    OtherStyle->SetHistMinimumZero(kFALSE); 
    gROOT->SetStyle(OtherStyle->GetName());
    return OtherStyle;
  }
  else{
    userNotify = "Invalid choice of style! Use 1, 2, 3, or 4.\nNo style was set!";
    std::cout << userNotify.Data() << std::endl;
    return nullptr;
  }
}

// --- T2K style ---

TStyle* PlotStyleHelper::SetT2KPlotStyle(const PresentationStyle &WhichStyle, const TString &styleName) {
  TStyle *t2kStyle= new TStyle(styleName, "T2K approved plots style");

  Int_t FontStyle = 22;
  Float_t FontSizeLabel = 0.035;
  Float_t FontSizeTitle = 0.05;
  Float_t YOffsetTitle = 1.3;
 
  switch(WhichStyle) {
  case PresentationLargeFont:
    FontStyle = 42;
    FontSizeLabel = 0.05;
    FontSizeTitle = 0.065;
    YOffsetTitle = 1.19;
    break;
  case PresentationSmallFont:
    FontStyle = 42;
    FontSizeLabel = 0.035;
    FontSizeTitle = 0.05;
    YOffsetTitle = 1.6;
    break;
  case PublicationPaper:
    FontStyle = 132;
    FontSizeLabel = 0.035;
    FontSizeTitle = 0.05;
    YOffsetTitle = 1.6;
    break;
  default:
    break;
  }

  // use plain black on white colors
  t2kStyle->SetFrameBorderMode(0);
  t2kStyle->SetCanvasBorderMode(0);
  t2kStyle->SetPadBorderMode(0);
  t2kStyle->SetCanvasBorderSize(0);
  t2kStyle->SetFrameBorderSize(0);
  t2kStyle->SetDrawBorder(0);
  t2kStyle->SetTitleBorderSize(0);

  t2kStyle->SetPadColor(0);
  t2kStyle->SetCanvasColor(0);
  t2kStyle->SetStatColor(0);
  t2kStyle->SetFillColor(0);

  t2kStyle->SetEndErrorSize(4);
  t2kStyle->SetStripDecimals(kFALSE);

  t2kStyle->SetLegendBorderSize(0);
  t2kStyle->SetLegendFont(FontStyle);

  // set the paper & margin sizes
  t2kStyle->SetPaperSize(20, 26);
  t2kStyle->SetPadTopMargin(0.1);
  t2kStyle->SetPadBottomMargin(0.15);
  t2kStyle->SetPadRightMargin(0.13); // 0.075 -> 0.13 for colz option
  t2kStyle->SetPadLeftMargin(0.16);//to include both large/small font options
  
  // Fonts, sizes, offsets
  t2kStyle->SetTextFont(FontStyle);
  t2kStyle->SetTextSize(0.08);

  t2kStyle->SetLabelFont(FontStyle, "x");
  t2kStyle->SetLabelFont(FontStyle, "y");
  t2kStyle->SetLabelFont(FontStyle, "z");
  t2kStyle->SetLabelFont(FontStyle, "t");
  t2kStyle->SetLabelSize(FontSizeLabel, "x");
  t2kStyle->SetLabelSize(FontSizeLabel, "y");
  t2kStyle->SetLabelSize(FontSizeLabel, "z");
  t2kStyle->SetLabelOffset(0.015, "x");
  t2kStyle->SetLabelOffset(0.015, "y");
  t2kStyle->SetLabelOffset(0.015, "z");

  t2kStyle->SetTitleFont(FontStyle, "x");
  t2kStyle->SetTitleFont(FontStyle, "y");
  t2kStyle->SetTitleFont(FontStyle, "z");
  t2kStyle->SetTitleFont(FontStyle, "t");
  t2kStyle->SetTitleSize(FontSizeTitle, "y");
  t2kStyle->SetTitleSize(FontSizeTitle, "x");
  t2kStyle->SetTitleSize(FontSizeTitle, "z");
  t2kStyle->SetTitleOffset(1.14, "x");
  t2kStyle->SetTitleOffset(YOffsetTitle, "y");
  t2kStyle->SetTitleOffset(1.2, "z");

  t2kStyle->SetTitleStyle(0);
  t2kStyle->SetTitleFontSize(0.06);//0.08
  t2kStyle->SetTitleFont(FontStyle, "pad");
  t2kStyle->SetTitleBorderSize(0);
  t2kStyle->SetTitleX(0.1f);
  t2kStyle->SetTitleW(0.8f);

  // use bold lines and markers
  t2kStyle->SetMarkerStyle(20);
  t2kStyle->SetHistLineWidth( Width_t(2.5) );
  t2kStyle->SetLineStyleString(2, "[12 12]"); // postscript dashes
  
  // get rid of X error bars and y error bar caps
  t2kStyle->SetErrorX(0.001);
  
  // do not display any of the standard histogram decorations
  t2kStyle->SetOptTitle(0);
  t2kStyle->SetOptStat(0);
  t2kStyle->SetOptFit(0);
  
  // put tick marks on top and RHS of plots
  t2kStyle->SetPadTickX(1);
  t2kStyle->SetPadTickY(1);
  
  // -- color --
  // functions blue
  t2kStyle->SetFuncColor(600-4);

  t2kStyle->SetFillColor(1); // make color fillings (not white)
  // - color setup for 2D -
  // - "cold"/ blue-ish -
  Double_t red[]   = { 0.00, 0.00, 0.00 };
  Double_t green[] = { 1.00, 0.00, 0.00 };
  Double_t blue[]  = { 1.00, 1.00, 0.25 };
  // - "warm" red-ish colors -
  //  Double_t red[]   = {1.00, 1.00, 0.25 };
  //  Double_t green[] = {1.00, 0.00, 0.00 };
  //  Double_t blue[]  = {0.00, 0.00, 0.00 };

  Double_t stops[] = { 0.25, 0.75, 1.00 };
  const Int_t NRGBs = 3;
  const Int_t NCont = 500;

  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  t2kStyle->SetNumberContours(NCont);

  // - Rainbow -
  //  t2kStyle->SetPalette(1);  // use the rainbow color set

  // -- axis --
  t2kStyle->SetStripDecimals(kFALSE); // don't do 1.0 -> 1
  //  TGaxis::SetMaxDigits(3); // doesn't have an effect 
  // no supressed zeroes!
  t2kStyle->SetHistMinimumZero(kTRUE);


 return(t2kStyle);
}


void CenterHistoTitles(TH1 *thisHisto){
  thisHisto->GetXaxis()->CenterTitle();
  thisHisto->GetYaxis()->CenterTitle();
  thisHisto->GetZaxis()->CenterTitle();
}


int AddGridLinesToPad(TPad *thisPad) {
  thisPad->SetGridx();
  thisPad->SetGridy();
  return(0);
}

TStyle* SetT2KLargeFontStyle(){
  const Int_t WhichStyle  = static_cast<const Int_t>(PresentationLargeFont);
  const TString styleName = "T2K Presentation Large Font";
  return SetPlotStyle(WhichStyle,styleName);
}

TStyle* SetT2KSmallFontStyle(){
  const Int_t WhichStyle = static_cast<const Int_t>(PresentationSmallFont);
  const TString styleName = "T2K Presentation Small Font";
  return SetPlotStyle(WhichStyle,styleName);
}

TStyle* SetT2KPaperStyle(){
  const Int_t WhichStyle = static_cast<const Int_t>(PublicationPaper);
  const TString styleName = "T2K Publication/Paper";
  return SetPlotStyle(WhichStyle,styleName);
}

TStyle* SetCC1PIStyle(){
  const Int_t WhichStyle = static_cast<const Int_t>(CC1PI);
  const TString styleName = "CC1PI";
  return SetPlotStyle(WhichStyle,styleName);
}

TStyle* SetOtherStyle(){
  const Int_t WhichStyle = static_cast<const Int_t>(Other);
  const TString styleName = "Other";
  return SetPlotStyle(WhichStyle,styleName);
}

TStyle* SetPlotStyle(const Int_t &WhichStyle,const TString &styleName) {
  return PlotStyleHelper::Get().SetStyle(WhichStyle,styleName);
}

#endif
