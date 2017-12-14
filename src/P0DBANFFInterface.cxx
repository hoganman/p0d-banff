#define P0DBANFFInterface_cxx

#include"P0DBANFFInterface.hxx"
ClassImp(P0DBANFFInterface)

#include"TDatime.h"
#include"TFile.h"
#include"TRandom3.h"
#include"TMath.h"
#include"TSystem.h"
#include"TStyle.h"
#include"TCanvas.h"
#include"TString.h"
#include<iostream>
#include<vector>

//**************************************************
P0DBANFFInterface::P0DBANFFInterface()
//**************************************************
{
    // CB pallate taken from Brian Connelly
    // http://bconnelly.net/2013/10/creating-colorblind-friendly-figures/
    cbBlack  = new TColor(kcbBlack ,0,0,0,"cbBlack");
    cbOrange = new TColor(kcbOrange,.9,.6,0,"cbOrange");
    cbSky    = new TColor(kcbSky   ,.35,.7,.9,"cbSky");
    cbGreen  = new TColor(kcbGreen ,0,.6,.5,"cbGreen");
    cbBlue   = new TColor(kcbBlue  ,0,.45,.7,"cbBlue");
    cbRed    = new TColor(kcbRed   ,.8,.4,0,"cbRed");
    cbPurple = new TColor(kcbPurple,.8,.6,.7,"cbPurple");
    cbYellow = new TColor(kcbYellow,.95,.90,.25,"cbYellow");
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
}


//**************************************************
void P0DBANFFInterface::RandomSleep(const Int_t& nSeconds, Int_t seed) const
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
Bool_t P0DBANFFInterface::CheckFile(TString& fileName) const
//**************************************************
{
    Bool_t status = false;
    const char* search = fileName.Data();
    if(!gSystem->FindFile(search,fileName)){
      status = false;
    }
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
void P0DBANFFInterface::SaveCanvasAs( TString& inputFileName, const TString& canvasName,
    const TString& outputNamePrefix, const TString& formats,
    const Char_t delimiter)
//**************************************************
{
  if(!CheckFile(inputFileName)){
    std::cerr << "ERROR: Invalid input file: " << inputFileName.Data()
      << std::endl;
    return;
  }

  TFile* const inputFile = TFile::Open(inputFileName.Data());
  TCanvas* const canvas =
    static_cast<TCanvas*>(inputFile->Get(canvasName.Data()));
  if(!canvas){
    std::cerr << "ERROR: No TCanvas with name \""<< canvasName.Data() <<
      "\" exists in " << inputFileName.Data() << std::endl;
    return;
  }
  std::vector<TString> fileOutputs = SplitString(formats,delimiter);
  for(UInt_t outputI = 0;outputI<fileOutputs.size();++outputI){
    Char_t buffer[1000];
    const TString outputFormat = fileOutputs.at(outputI);
    sprintf(buffer,"%s.%s",outputNamePrefix.Data(),outputFormat.Data());
    canvas->SaveAs(buffer);
  }
  inputFile->Close();

}

//**************************************************
std::vector<TString> P0DBANFFInterface::SplitString(const TString& theOpt,
    const Char_t separator)
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
