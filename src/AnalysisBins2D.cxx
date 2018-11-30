#define ANALYSISBINS2D_CXX

#include "AnalysisBins2D.hxx"
ClassImp(AnalysisBins2D)
#include "AnalysisBins.hxx"
#include "P0DBANFFInterface.hxx"
#include "TMath.h"
#include <iostream>

//**************************************************
AnalysisBins2D::AnalysisBins2D(TString xBinsName, TString yBinsName, TString configFile, XMLTools* xml)
//**************************************************
{
    Init();
    XMLTools* new_xml_inst = NULL;
    if(!xml)
    {
        P0DBANFFInterface::Warning(this, "Creating new XMLTools instance");
        xml = new XMLTools();
        new_xml_inst = xml;
    }
    xml->SetFile(configFile);
    binningName = TString::Format("%s_and_%s", xBinsName.Data(), yBinsName.Data());

    anaBinsX = new AnalysisBins(xBinsName, configFile, xml);
    anaBinsY = new AnalysisBins(yBinsName, configFile, xml);

    binEdgesX = anaBinsX->GetBinEdgesVect();
    binEdgesY = anaBinsY->GetBinEdgesVect();

    nBinsX = anaBinsX->GetNbins();
    nBinsY = anaBinsY->GetNbins();

    unitsX = anaBinsX->GetUnits();
    unitsY = anaBinsY->GetUnits();

    hist = new TH2D(binningName.Data(), "", nBinsX, GetBinEdgesX(), nBinsY, GetBinEdgesY());
    nBins = nBinsX * nBinsY;

    if(new_xml_inst)
    {
        P0DBANFFInterface::Warning(this, "Cleaning new XMLTools instance");
        delete new_xml_inst;
    }
}

//**************************************************
AnalysisBins2D::AnalysisBins2D(AnalysisBins* inputAnaBinsX, AnalysisBins* inputAnaBinsY)
//**************************************************
{
    Init();
    binningName = TString::Format("%s_and_%s", inputAnaBinsX->GetBinningName().Data(), inputAnaBinsY->GetBinningName().Data());

    anaBinsX = new AnalysisBins(*inputAnaBinsX);
    anaBinsY = new AnalysisBins(*inputAnaBinsY);

    binEdgesX = anaBinsX->GetBinEdgesVect();
    binEdgesY = anaBinsY->GetBinEdgesVect();

    nBinsX = anaBinsX->GetNbins();
    nBinsY = anaBinsY->GetNbins();

    unitsX = anaBinsX->GetUnits();
    unitsY = anaBinsY->GetUnits();

    hist = new TH2D(binningName.Data(), "", nBinsX, GetBinEdgesX(), nBinsY, GetBinEdgesY());
    nBins = nBinsX * nBinsY;

    divideByBinWidth = anaBinsX->GetDivideByBinWidth() && anaBinsY->GetDivideByBinWidth();
    Sumw2 = anaBinsX->GetSumw2() && anaBinsY->GetSumw2();
    if(Sumw2)
        hist->Sumw2();

}

//**************************************************
AnalysisBins2D::~AnalysisBins2D()
//**************************************************
{
    if (hist) hist->Delete();
    if (anaBinsX) delete anaBinsX;
    if (anaBinsY) delete anaBinsY;
}

//**************************************************
void AnalysisBins2D::Init()
//**************************************************
{
    binningName ="";
    nBins = 0;
    unitsX = "";
    unitsY = "";
    hist = NULL;
    anaBinsX = NULL;
    anaBinsY = NULL;
    divideByBinWidth = kFALSE;
}

//**************************************************
Int_t AnalysisBins2D::Fill(Double_t x_val, Double_t y_val, Double_t weight)
//**************************************************
{
    anaBinsX->Fill(x_val, weight);
    anaBinsY->Fill(y_val, weight);
    return hist->Fill(x_val, y_val, weight);
}

//**************************************************
void AnalysisBins2D::Reset()
//**************************************************
{
    anaBinsX->Reset();
    anaBinsY->Reset();
    hist->Reset();
}

//**************************************************
TCanvas* AnalysisBins2D::Draw2D(TString twoDimOptions)
//**************************************************
{
    TCanvas* canvas = new TCanvas(TString::Format("%s_canvas", binningName.Data()), "", 800, 600);
    hist->Draw(twoDimOptions.Data());
    return canvas;
}

//**************************************************
TCanvas* AnalysisBins2D::DrawAll(TString twoDimOptions, TString oneDimOptions)
//**************************************************
{
    TCanvas* canvas = new TCanvas(TString::Format("%s_canvas", binningName.Data()), "", 900, 300);
    canvas->Divide(3,1);
    canvas->cd(1);
    anaBinsX->DrawCopy(oneDimOptions);
    canvas->cd(2);
    hist->Draw(twoDimOptions.Data());
    canvas->cd(3);
    anaBinsY->DrawCopy(oneDimOptions);
    return canvas;
}

//**************************************************
void AnalysisBins2D::DivideByBinWidth(Bool_t sumw2)
//**************************************************
{
    for(Int_t xBin = 1; xBin <= nBinsX; ++xBin)
    {
        for(Int_t yBin = 1; yBin <= nBinsY; ++yBin)
        {
            const Double_t old_content = hist->GetBinContent(xBin, yBin);
            const Double_t xHeight = hist->GetXaxis()->GetBinWidth(xBin);
            const Double_t yHeight = hist->GetYaxis()->GetBinWidth(yBin);
            const Double_t binArea = xHeight * yHeight;
            hist->SetBinContent(xBin, yBin, old_content/binArea);
            if(Sumw2)
            {
                const Double_t new_error = std::sqrt(old_content)/std::fabs(binArea);
                hist->SetBinError(xBin, yBin, new_error);
            }
        }
    }

}
