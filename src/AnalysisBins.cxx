#define ANALYSISBINS_CXX

#include"AnalysisBins.hxx"
#include"TMath.h"
#include"TF1.h"
#include<iostream>
ClassImp(AnalysisBins)

//**************************************************
void AnalysisBins::Init()
//**************************************************
{
    binningName ="";
    nBins = 0;
    units = "";
    showOverflow = kFALSE;
    divideByBinWidth = kFALSE;
    isUniform = kFALSE;
    hist = NULL;
}

//**************************************************
AnalysisBins::AnalysisBins(TString name, Double_t* edges, Int_t nEntries, Bool_t setShowOverflow)
//**************************************************
{
    Init();
    if(!edges)
    {
        printf("ERROR: Unable to create AnalysisBins");
        printf("       NULL Double_t ptr in constructor");
        return;
    }
    if(nEntries <= 0)
    {
        printf("ERROR: Unable to create AnalysisBins");
        printf("       nEntries = 0!");
	return;
    }
    binningName = name;
    binEdges.resize(nEntries);
    nBins = nEntries - 1;
    for( Int_t index = 0; index < nEntries; ++index )
    {
        //std::cout << edges[index] << std::endl;
        binEdges[index] = edges[index];
    }
    Char_t buffer[500];
    sprintf(buffer, "AnalysisBins_%s", binningName.Data());
    hist = new TH1D(buffer, "", nBins, GetBinEdges());
    showOverflow = setShowOverflow;
}

//**************************************************
AnalysisBins::AnalysisBins(TString name, Float_t* edges, Int_t nEntries, Bool_t setShowOverflow)
//**************************************************
{
    Init();
    if(!edges)
    {
        printf("ERROR: Unable to create AnalysisBins");
        printf("       NULL Float_t ptr in constructor");
        return;
    }
    if(nEntries <= 0)
    {
        printf("ERROR: Unable to create AnalysisBins");
        printf("       nEntries = 0!");
	return;
    }
    binningName = name;
    binEdges.resize(nEntries);
    nBins = nEntries - 1;
    for(Int_t index = 0; index < nEntries; ++index )
    {
        binEdges[index] = static_cast<Double_t>(edges[index]);
    }
    Char_t buffer[500];
    sprintf(buffer, "AnalysisBins__%s", binningName.Data());
    hist = new TH1D(buffer, "", nBins, GetBinEdges());
    showOverflow = setShowOverflow;
}

//**************************************************
AnalysisBins::AnalysisBins(TString name, TH1D* template_hist, Bool_t setShowOverflow)
//**************************************************
{
    Init();
    if(!template_hist)
    {
        printf("ERROR: Unable to create AnalysisBins");
        printf("       NULL TH1D ptr in constructor");
	return;
    }
    hist = new TH1D(*template_hist);
    nBins = hist->GetNbinsX();
    for( Int_t bin = 1; bin<=hist->GetNbinsX(); ++bin)
    {
        binEdges.push_back(hist->GetXaxis()->GetBinLowEdge(bin));
    }
    binningName = name;
    showOverflow = setShowOverflow;
}

//**************************************************
AnalysisBins::AnalysisBins(TString name, TString configFile, XMLTools* xml)
//**************************************************
{
    Init();
    XMLTools* new_inst = NULL;
    if(!xml)
    {
        std::cout << "WARNING: Creating new XMLTools instance" << std::endl;
        xml = new XMLTools();
        new_inst = xml;
    }
    xml->SetFile(configFile);
    hist = xml->GetTH1DWithBinning(name);
    nBins = hist->GetNbinsX();
    for( Int_t bin = 1; bin<=hist->GetNbinsX(); ++bin)
    {
        binEdges.push_back(hist->GetXaxis()->GetBinLowEdge(bin));
    }
    XMLTools::AttributeMap attribs = xml->GetAllChildAttributesFromNode(name);
    if (attribs["isUniform"].Atoi() == 1)
        isUniform = kTRUE;
    if (attribs["units"].Length() > 0)
        units = attribs["units"];
    if (attribs["lastBinOverflow"].Atoi() == 1)
        showOverflow = kTRUE;
    if (attribs["divideByBinWidth"].Atoi() == 1)
    {
        divideByBinWidth = kTRUE;
    }
    if(new_inst)
    {
        std::cout << "Cleaning new XMLTools instance" << std::endl;
        new_inst->Delete();
    }
}

//**************************************************
AnalysisBins::~AnalysisBins()
//**************************************************
{
    if(hist) hist->Delete();
}

//**************************************************
TH1D* AnalysisBins::VaryPoisson()
//**************************************************
{
    TH1D* toyExperiment = new TH1D(*hist);
    // for each bin, throw a random number determined by bin content
    for(Int_t bin = 1; bin <= nBins; ++bin)
    {
	// this is the mean for a poisson throw
        const Double_t mean = hist->GetBinContent(bin);
        Char_t buffer[100];
        sprintf(buffer, "TMath::Poisson(x, %f)", mean);
        TF1 pois("pois", buffer, 0, 100. * mean);
        const Double_t a_throw = pois.GetRandom();
        toyExperiment->SetBinContent(bin, a_throw);
    }
    return toyExperiment;
}

//**************************************************
Int_t AnalysisBins::Fill(Double_t val, Double_t weight)
//**************************************************
{
    if(showOverflow)
    {
        if(val >= hist->GetXaxis()->GetBinLowEdge(nBins))
        {
            const Double_t fillValue = hist->GetXaxis()->GetBinLowEdge(nBins);
            hist->Fill(fillValue, weight);
            return nBins;
        }
        else
        {
            return hist->Fill(val, weight);
        }
    }
    else
    {
        return hist->Fill(val, weight);
    }
    std::cout << "NEVER GET HERE!" << std::endl;
    return 0;
}

//**************************************************
void AnalysisBins::DivideByBinWidth(Bool_t Sumw2)
//**************************************************
{
    if(!divideByBinWidth)
        return;
    for(Int_t bin = 1; bin <= hist->GetNbinsX(); ++bin)
    {
        const Double_t old_content = hist->GetBinContent(bin);
        const Double_t old_error = hist->GetBinError(bin);
        const Double_t binLowEdge = hist->GetXaxis()->GetBinLowEdge(bin);
        const Double_t binUpEdge = hist->GetXaxis()->GetBinUpEdge(bin);
        const Double_t binWidth = (binUpEdge-binLowEdge);
        hist->SetBinContent(bin, old_content/binWidth);
        if(Sumw2 && hist->GetSumw2())
            hist->SetBinError(bin, old_error/std::fabs(binWidth));
    }
}
