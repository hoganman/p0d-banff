#define P0DBANFFInterface_cxx

#include"P0DBANFFInterface.hxx"
ClassImp(P0DBANFFInterface)
#include<iostream>

#include"TH2.h"
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
void P0DBANFFInterface::PrettyUpTH1(TString inFileName, TString outputName,
        TString canvasName, TString histName, TString xAxisTitle,
	TString yAxisTitle, UInt_t lineColor, UInt_t fillColor,
    UInt_t lineWidth, Double_t textSizeChange) const
//**************************************************
{
    TCanvas* canvas = NULL;
    TH1* htemp = NULL;
    TH1* htemp_1Dclone = NULL;
    if(!CheckFile(inFileName))
    {
      std::cerr << "ERROR: Invalid input file: " << inFileName.Data()
                << std::endl;
      return;
    }
    TFile* const inputFile = TFile::Open(inFileName.Data());
    canvas = GetTCanvasFromFile(inputFile, canvasName);
    if(canvas)
    {
        htemp = GetTH1FromCanvas(canvas, histName);
    }
    else
    {
        std::cout << " Trying to find \"" << histName.Data() << "\" in TFile..." << std::endl;
	htemp = static_cast<TH1*>(FindObjectInFileByName(inputFile, histName));
	if(htemp)
	{
	    canvas = new TCanvas("canvas", "", 800, 800);
	    htemp_1Dclone = static_cast<TH1*>(htemp->Clone(Form("%s_clone", htemp->GetName())));
	    htemp = htemp_1Dclone;
	}
    }

    if(!htemp)
    {
        std::cerr << "No valid TH1* pointer found!" << std::endl;
        return;
    }

    PrettyUpTH1(htemp, xAxisTitle, yAxisTitle, lineColor, fillColor, lineWidth, textSizeChange);
    canvas->cd();
    htemp->Draw();

    if(outputName.Contains("."))
    {
        SaveCanvasAs(canvas,outputName.Remove(outputName.Last('.')),"png");
    }
    else
    {
        SaveCanvasAs(canvas,outputName,"png");
    }

    inputFile->Close();
    if (canvas) delete canvas;
    if (htemp_1Dclone) delete htemp_1Dclone;

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
    inHist->GetXaxis()->SetTitleOffset(0.85);
    inHist->GetYaxis()->SetTitleOffset(0.85);
    if(!xAxisTitle.Contains("none"))
    {
        inHist->GetXaxis()->SetTitle(xAxisTitle.Data());
    }
    if(!yAxisTitle.Contains("none"))
    {
        inHist->GetYaxis()->SetTitle(yAxisTitle.Data());
    }
    if(textSizeChange > 0)
    {
        inHist->GetXaxis()->SetTitleSize(inHist->GetXaxis()->GetTitleSize()*(1+textSizeChange));
        inHist->GetYaxis()->SetTitleSize(inHist->GetYaxis()->GetTitleSize()*(1+textSizeChange));
    }
    if(inHist->GetDimension() == 1)
    {
	if(lineWidth > 0)
    	    inHist->SetLineWidth(lineWidth);
    	if(lineColor > 0)
    	    inHist->SetLineColor(lineColor);
    	if(fillColor > 0)
    	    inHist->SetFillColor(fillColor);
    }

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
    Bool_t status = kFALSE;
    TString copy(fileName);
    if(fileName.Contains('/'))
    {
	const char* search = Form("%s",copy(copy.Last('/'), copy.Length()-copy.Last('/')).Data());
	status = static_cast<Bool_t>(gSystem->FindFile(search,copy));
    }
    else
    {
	const char* search = gSystem->Getenv("PWD");
	status = static_cast<Bool_t>(gSystem->FindFile(search,copy));
    }

    if(!status)
    {
	std::cout << "ERROR: Could NOT find file \"" << fileName.Data() << "\"" << std::endl;
	return status;
    }
    else
    {
        TFile* const f = TFile::Open(copy);
        if(!f)
            status = kFALSE;
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
            status = kTRUE;
        f->Close();
    }
    return status;
}

//**************************************************
void P0DBANFFInterface::LoadColorBlindPalette(Int_t nColors) const
//**************************************************
{
    std::cout << "...Loading color palette with nColors = " << nColors << "...";
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
    else if(nColors==4)
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
    else {
        std::cout << "...Cannot load nColors = " << nColors << "...FAILED!" << std::endl;
        return;
    }
    std::cout << "DONE!" << std::endl;
}

//**************************************************
void P0DBANFFInterface::SaveCanvasAs(TCanvas* const canvas,
    TString outputNamePrefix, TString formats,
    Char_t delimiter) const
//**************************************************
{
    std::cout << "...Saving canvas as " << std::endl;
    if(!canvas)
        return;
    std::vector<TString> fileOutputs = SplitString(formats,delimiter);
    for(UInt_t outputI = 0;outputI<fileOutputs.size();++outputI)
    {
        Char_t buffer[1000];
        const TString outputFormat = fileOutputs.at(outputI);
        sprintf(buffer,"%s.%s",outputNamePrefix.Data(),outputFormat.Data());
        std::cout << "                    " << buffer << std::endl;
        if(outputFormat.Contains("png") || outputFormat.Contains("jpg"))
        {
            TImage* img =  TImage::Create();
            canvas->Draw();
            canvas->cd();
            img->FromPad(canvas);
            img->WriteImage(buffer);
            delete img;
        }
        else{
            canvas->SaveAs(buffer);
        }
    }
    std::cout << "...DONE!" << std::endl;
}


//**************************************************
void P0DBANFFInterface::SaveCanvasAs(TString inputFileName,
        TString canvasName, TString outputNamePrefix,
        TString formats, Char_t delimiter) const
//**************************************************
{
  TCanvas* canvas = NULL;
  if(!CheckFile(inputFileName)){
      std::cerr << "ERROR: Invalid input file: " << inputFileName.Data()
                << std::endl;
      return;
  }

  TFile* const inputFile = TFile::Open(inputFileName.Data());
  canvas = GetTCanvasFromFile(inputFile, canvasName);
  if(!canvas)
  {
      std::cerr << "ERROR: No canvas found with name \"" << canvasName.Data() << "\"" << std::endl;
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
TObject* P0DBANFFInterface::FindObjectInFileByName(TFile* inFile,
    const TString& name_search) const
//**************************************************
{
    TObject* result = inFile->Get(name_search.Data());
    if(result)
	return result;

    const TList* keys = inFile->GetListOfKeys();
    TIter iter(keys);
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


//**************************************************
TCanvas* P0DBANFFInterface::GetTCanvasFromFile(TFile* inFile,
	TString canvasName) const
//**************************************************
{
    TCanvas* canvas = NULL;
    canvas = static_cast<TCanvas*>(inFile->Get(canvasName.Data()));

    //try to find canvas using non-standard methods
    if(!canvas)
    {
        std::cout << "WARNING: Unable to find canvas with name " << canvasName.Data()
            << std::endl;
        const Int_t nNames = 3;
        const TString canvasNames[nNames] = {"c1", "canvas", "canvas1"};
        for(Int_t iName = 0; iName < nNames; ++iName)
        {
	  canvas = static_cast<TCanvas*>(FindObjectInFileByName(inFile, canvasNames[iName]));
	  if(canvas)
	    break;
        }
    }
    return canvas;

}

//**************************************************
TH1* P0DBANFFInterface::GetTH1FromCanvas(TCanvas* inCanvas,
	TString histName) const
//**************************************************
{
    TH1* htemp = NULL;
    if(histName.Length() > 0)
    {
        htemp = static_cast<TH1*>(inCanvas->GetPrimitive(histName.Data()));
    }
    else
    {
        TIter iter(inCanvas->GetListOfPrimitives());
        TKey* key = static_cast<TKey*>(iter.Next());
        while(key){
          TString name = key->GetName();
          if(name.Contains("TFrame"))
          {
            key = static_cast<TKey*>(iter.Next());
            continue;
          }
          htemp = static_cast<TH1*>(inCanvas->GetPrimitive(name.Data()));
          break;
	}
    }
    return htemp;
}
