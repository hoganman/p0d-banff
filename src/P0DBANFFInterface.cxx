#define P0DBANFFInterface_cxx

#include"P0DBANFFInterface.hxx"
ClassImp(P0DBANFFInterface)
#include<iostream>

#include"TDatime.h"
#include"TFile.h"
#include"TRandom3.h"
#include"TMath.h"
#include"TSystem.h"
#include"TCanvas.h"
#include"TImage.h"
#include"TGaxis.h"
#include"TROOT.h"
#include"TKey.h"

//**************************************************
P0DBANFFInterface::P0DBANFFInterface()
//**************************************************
{
    // CB pallate taken from Brian Connelly
    // http://bconnelly.net/2013/10/creating-colorblind-friendly-figures/
    cbBlack  = new TColor(kcbBlack , 0.00, 0.00, 0.00, "cbBlack");
    cbOrange = new TColor(kcbOrange, 0.90, 0.60, 0.00, "cbOrange");
    cbSky    = new TColor(kcbSky   , 0.35, 0.70, 0.90, "cbSky");
    cbGreen  = new TColor(kcbGreen , 0.00, 0.60, 0.50, "cbGreen");
    cbBlue   = new TColor(kcbBlue  , 0.00, 0.45, 0.70, "cbBlue");
    cbRed    = new TColor(kcbRed   , 0.80, 0.40, 0.00, "cbRed");
    cbPurple = new TColor(kcbPurple, 0.80, 0.60, 0.70, "cbPurple");
    cbYellow = new TColor(kcbYellow, 0.95, 0.90, 0.25, "cbYellow");

    P0DBANFFStyle = new TStyle("P0DBANFFStyle","Color style + colorblind frendly palletes");
    TGaxis::SetMaxDigits(3);
    P0DBANFFStyle->SetOptStat("i");
    P0DBANFFStyle->SetStatFormat("10g");
    P0DBANFFStyle->SetTitleXSize(0.055);
    P0DBANFFStyle->SetTitleYSize(0.055);
    P0DBANFFStyle->SetStripDecimals(kFALSE);
    P0DBANFFStyle->SetCanvasBorderMode(0);
    P0DBANFFStyle->SetOptTitle(kFALSE);
    P0DBANFFStyle->SetEndErrorSize(4);
    P0DBANFFStyle->SetFillColor(0);
    gROOT->SetStyle("P0DBANFFStyle");

}

//**************************************************
P0DBANFFInterface::~P0DBANFFInterface()
//**************************************************
{
    delete cbBlack ;
    delete cbOrange;
    delete cbSky   ;
    delete cbGreen ;
    delete cbBlue  ;
    delete cbRed   ;
    delete cbPurple;
    delete cbYellow;
    delete P0DBANFFStyle;
}

//**************************************************
void P0DBANFFInterface::PrettyUpTH1(TString inFileName,
        TString canvasName, TString histName,
        TString xAxisTitle, TString yAxisTitle,
        UInt_t lineColor, UInt_t fillColor,
    UInt_t lineWidth, Double_t textSizeChange) const
//**************************************************
{
  if(!CheckFile(inFileName))
  {
    std::cerr << "ERROR: Invalid input file: " << inFileName.Data()
              << std::endl;
    return;
  }
  TFile* const inputFile = TFile::Open(inFileName.Data());
  TCanvas* canvas = NULL;
  if(canvasName.Length() > 0)
      canvas = static_cast<TCanvas*>(inputFile->Get(canvasName.Data()));
  if(!canvas)
  {
    std::cout << "WARNING: Unable to find canvas with name " << canvasName.Data()
          << std::endl;
    const TList* keys = inputFile->GetListOfKeys();
    canvas = static_cast<TCanvas*>(FindObjectInFileByName(keys, "c1"));
    if(!canvas)
      canvas = static_cast<TCanvas*>(FindObjectInFileByName(keys, "canvas"));
  }
  if(!canvas)
  {
    std::cerr << "ERROR: No canvas found using standard names: \"c1\" and \
\"canvas\" and " << canvasName.Data() << std::endl;
    return;
  }

  TH1* htemp = NULL;

  if(canvas->GetListOfPrimitives()->GetSize() != 2 && histName.Length() == 0)
  {
     std::cerr << "ERROR: Canvas has more than 2 primitives with no hitogram name input"
               << std::endl;
    return;
  }
  else if(histName.Length() > 0)
  {
    htemp = static_cast<TH1*>(canvas->GetPrimitive(histName.Data()));
  }
  else
  {
    TIter iter(canvas->GetListOfPrimitives());
    TKey* key = static_cast<TKey*>(iter.Next());
    while(key){
      TString name = key->GetName();
      if(name.Contains("TFrame"))
      {
        key = static_cast<TKey*>(iter.Next());
        continue;
      }
      htemp = static_cast<TH1*>(canvas->GetPrimitive(name.Data()));
      break;
    }
  }
  if(!htemp)
  {
    std::cerr << "No valid TH1* pointer found!" << std::endl;
    return;
  }
  PrettyUpTH1(htemp, xAxisTitle, yAxisTitle, lineWidth, lineColor);
  canvas->cd();
  htemp->Draw();
  SaveCanvasAs(canvas,inFileName.Remove(inFileName.Last('.')),"root");
  inputFile->Close();


}

//**************************************************
void P0DBANFFInterface::PrettyUpTHStack(THStack* stack,
        TString xAxisTitle, TString yAxisTitle,
    Double_t textSizeChange) const
//**************************************************
{
  if(!stack)
  {
    std::cerr << "ERROR: Invalid input THStack*"
              << std::endl;
    return;
  }
  stack->Draw("goff");
  if(!xAxisTitle.Contains("none"))
  {
      stack->GetXaxis()->SetTitle(xAxisTitle.Data());
      stack->GetXaxis()->SetTitleOffset(0.85);
  }
  if(!yAxisTitle.Contains("none"))
  {
      stack->GetYaxis()->SetTitle(yAxisTitle.Data());
      stack->GetYaxis()->SetTitleOffset(0.85);
  }
  if(textSizeChange > 0)
  {
      stack->GetXaxis()->SetTitleSize(stack->GetXaxis()->GetTitleSize()*(1+textSizeChange));
      stack->GetYaxis()->SetTitleSize(stack->GetYaxis()->GetTitleSize()*(1+textSizeChange));
  }
}

//**************************************************
void P0DBANFFInterface::PrettyUpTH1(TH1* inHist,
        TString xAxisTitle, TString yAxisTitle,
        UInt_t lineColor, UInt_t fillColor,
    UInt_t lineWidth, Double_t textSizeChange) const
//**************************************************
{
  if(!inHist)
  {
    std::cerr << "ERROR: Invalid input TH1*"
              << std::endl;
    return;
  }
  inHist->SetTitle("");
  if(!xAxisTitle.Contains("none"))
  {
      inHist->GetXaxis()->SetTitle(xAxisTitle.Data());
      inHist->GetXaxis()->SetTitleOffset(0.85);
  }
  if(!yAxisTitle.Contains("none"))
  {
      inHist->GetYaxis()->SetTitle(yAxisTitle.Data());
      inHist->GetYaxis()->SetTitleOffset(0.85);
  }
  if(textSizeChange > 0)
  {
      inHist->GetXaxis()->SetTitleSize(inHist->GetXaxis()->GetTitleSize()*(1+textSizeChange));
      inHist->GetYaxis()->SetTitleSize(inHist->GetYaxis()->GetTitleSize()*(1+textSizeChange));
  }
  if(lineWidth > 0)
      inHist->SetLineWidth(lineWidth);
  if(lineColor > 0)
      inHist->SetLineColor(lineColor);
  if(fillColor > 0)
      inHist->SetFillColor(fillColor);

}

//**************************************************
void P0DBANFFInterface::RandomSleep(Int_t nSeconds, Int_t seed) const
//**************************************************
{
    if(nSeconds <= 0){
      return;
    }
    if(seed==0){
      const TDatime time;
      seed = time.Get();
    }
    TRandom3 ran(seed);
    const UInt_t sleepTime = TMath::Nint(ran.Integer(nSeconds+1)*1e3);//seconds
    std::cout << "Sleeping for " << (sleepTime*1e-3) << " seconds " << std::endl;
    gSystem->Sleep(sleepTime);
}

//**************************************************
Bool_t P0DBANFFInterface::CheckFile(TString fileName) const
//**************************************************
{
    Bool_t status = false;
    const char* search = fileName.Data();
    if(!gSystem->FindFile(search,fileName))
      status = false;
    else{
      TFile* const f = TFile::Open(search);
      if(!f)
        status = false;
      else if(!f->IsOpen())
        status = false;
      else if(f->IsRaw())
        status  = false;
      else if(!f->IsBinary())
        status =  false;
      else if(!f->GetListOfKeys()->GetSize())
        status =  false;
      else if(f->IsZombie())
        status =  false;
      else if(f->TestBit(TFile::kRecovered))
        status =  false;
      else
        status = true;
      f->Close();
    }
    return status;
}

//**************************************************
void P0DBANFFInterface::LoadColorBlindPalette(Int_t nColors) const
//**************************************************
{
    const Int_t nColorBins = 999;
    if(nColors==5)
    {
      const Double_t length5[]    = {0.00,0.25,0.50,0.75,1.00} ;
      const Double_t fiveColorR[] = {0.00,0.35,0.80,0.80,0.95};
      const Double_t fiveColorG[] = {0.45,0.70,0.60,0.40,0.90};
      const Double_t fiveColorB[] = {0.70,0.90,0.70,0.00,0.25};
      Int_t fiveColorPalette[nColorBins];
      static Int_t FI2 = TColor::CreateGradientColorTable(nColors,const_cast<Double_t*>(length5),const_cast<Double_t*>(fiveColorR),const_cast<Double_t*>(fiveColorG),const_cast<Double_t*>(fiveColorB),nColorBins);
      for (int i = 0; i < nColorBins; i++) fiveColorPalette[i]=FI2+i;
      gStyle->SetPalette(nColorBins,fiveColorPalette);
      gStyle->SetNumberContours(nColorBins);
    }
    if(nColors==4)
    {
      const Double_t length4[]    = {0.00,0.3333,0.6667,1.00} ;
      const Double_t fourColorR[] = {0.00,0.80,0.80,0.95};
      const Double_t fourColorG[] = {0.45,0.60,0.40,0.90};
      const Double_t fourColorB[] = {0.70,0.70,0.00,0.25};
      Int_t fourColorPalette[nColorBins];
      static Int_t FI2 = TColor::CreateGradientColorTable(nColors,const_cast<Double_t*>(length4),const_cast<Double_t*>(fourColorR),const_cast<Double_t*>(fourColorG),const_cast<Double_t*>(fourColorB),nColorBins);
      for (int i = 0; i < nColorBins; i++) fourColorPalette[i]=FI2+i;
      gStyle->SetPalette(nColorBins,fourColorPalette);
      gStyle->SetNumberContours(nColorBins);
    }
}

//**************************************************
void P0DBANFFInterface::SaveCanvasAs(TCanvas* const canvas,
    TString outputNamePrefix, TString formats,
    Char_t delimiter) const
//**************************************************
{
  if(!canvas)
    return;
  std::vector<TString> fileOutputs = SplitString(formats,delimiter);
  for(UInt_t outputI = 0;outputI<fileOutputs.size();++outputI)
  {
    Char_t buffer[1000];
    const TString outputFormat = fileOutputs.at(outputI);
    sprintf(buffer,"%s.%s",outputNamePrefix.Data(),outputFormat.Data());
    if(outputFormat.Contains("png") || outputFormat.Contains("jpg"))
    {
        TImage* img =  TImage::Create();
        canvas->Draw();
        canvas->cd();
        img->FromPad(canvas);
        img->WriteImage(buffer);
        delete img;
    }
    //else if(outputFormat.Contains("root"))
    //{
    //    if(!CheckFile(buffer))
    //        canvas->SaveAs(buffer);
    //}
    else{
        canvas->SaveAs(buffer);
    }
  }
}


//**************************************************
void P0DBANFFInterface::SaveCanvasAs(TString inputFileName,
        TString canvasName, TString outputNamePrefix,
        TString formats, Char_t delimiter) const
//**************************************************
{
  if(!CheckFile(inputFileName)){
    std::cerr << "ERROR: Invalid input file: " << inputFileName.Data()
              << std::endl;
    return;
  }

  TFile* const inputFile = TFile::Open(inputFileName.Data());
  TCanvas* canvas = NULL;
  if(canvasName.Length() > 0)
      canvas = static_cast<TCanvas*>(inputFile->Get(canvasName.Data()));
  else
  {
    TIter iter(inputFile->GetListOfKeys());
    TKey* key = static_cast<TKey*>(iter.Next());
    while(key){
      TString name = key->GetName();
      if(!name.Contains(canvasName) && !name.Contains("c1") && !name.Contains("canvas"))
        continue;
      canvas = static_cast<TCanvas*>(key->ReadObj());
      break;
    }
  }
  if(!canvas)
  {
    std::cerr << "ERROR: No canvas found using standard names: \"c1\" and \"canvas\" and"
              << canvasName.Data() << std::endl;
    return;
  }
  if(outputNamePrefix.Length() == 0)
  {
    outputNamePrefix = inputFileName;
    outputNamePrefix.Remove(outputNamePrefix.Last('.'));
  }
  SaveCanvasAs(canvas, outputNamePrefix, formats, delimiter);
  inputFile->Close();

}

//**************************************************
std::vector<TString> P0DBANFFInterface::SplitString(TString theOpt,
    Char_t separator) const
//**************************************************
{
   // splits the option string at 'separator' and fills the list
   // 'splitV' with the primitive strings
   std::vector<TString> splitV;
   TString splitOpt(theOpt);
   splitOpt.ReplaceAll("\n"," ");
   splitOpt = splitOpt.Strip(TString::kBoth,separator);
   while (splitOpt.Length()>0) {
      if ( !splitOpt.Contains(separator) ) {
         splitV.push_back(splitOpt);
         break;
      }
      else {
         TString toSave = splitOpt(0,splitOpt.First(separator));
         splitV.push_back(toSave);
         splitOpt = splitOpt(splitOpt.First(separator),splitOpt.Length());
      }
      splitOpt = splitOpt.Strip(TString::kLeading,separator);
   }
   return splitV;
}


//**************************************************
TObject* P0DBANFFInterface::FindObjectInFileByName(const TList* inList,
    const TString name_search) const
//**************************************************
{
    TObject* result = NULL;
    TIter iter(inList);
    TKey* key = static_cast<TKey*>(iter.Next());
    while(key){
      TString name = key->GetName();
      if(!name.Contains(name_search))
      {
        key = static_cast<TKey*>(iter.Next());
        continue;
      }
      result = key->ReadObj();
      break;
    }
    return result;
}
