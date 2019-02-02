/*
 * Make 1D TGraph to get binning recommendations for momentum and angle
 *
 * Procedure is described in TN-333
 *
*/

#include"T2KDataMC.hxx"
#include"DefineCuts.hxx"
#include"SampleId.hxx"
#include"TotalPOT.hxx"
#include"P0DBANFFInterface.hxx"

#include"TCanvas.h"
#include"TChain.h"
#include"TCut.h"
#include"TDirectory.h"
#include"TEventList.h"
#include"TF1.h"
#include"TGaxis.h"
#include"TGraph.h"
#include"TGraphErrors.h"
#include"TH1D.h"
#include"TLegend.h"
#include"TMath.h"
#include"TMultiGraph.h"
#include"TROOT.h"
#include"TString.h"
#include"TSystem.h"

#include"Math/DistFunc.h"
#include"Math/ProbFunc.h"

#include<algorithm>
#include<cmath>
#include<iostream>
#include<sstream>
#include<utility>
#include<vector>

namespace Reset
{
    const Int_t kInt = -1;
    const UInt_t kUInt = 0;
    const Double_t kDouble = -999;
    const std::string kString = "";
};

typedef std::pair<Int_t, Int_t> BinPair;

//Two percent
const Double_t MinFracDiff = 2.0e-02;
Double_t POTweight = 1;
Bool_t residualTruePlotVar = kFALSE;
TChain* eventTree = NULL;

class Edge
{
public:
    Edge() :
        hRef(NULL), hFineBinning(NULL), isUpEdge(kFALSE)
    {}
    Edge(TH1D* hReference,TH1D* hEqualStatistics,Bool_t UpEdge);

    virtual ~Edge(){}

    Bool_t MoveUp();

    Bool_t MoveLow();

    void ShiftToBin(Int_t newBin);

    TH1D* hRef;
    TH1D* hFineBinning;
    Bool_t isUpEdge;
    Int_t bin;
    Double_t edge;
    Int_t firstNonZeroBin;
    Int_t lastNonZeroBin;

};

//Really a struct
class OneDimParameters
{
public:
    Double_t low;
    Double_t up;
    Double_t nEntriesMC;
    Double_t nEntriesData;
    Double_t statFracErr;;
    Double_t sysFracErr;
    Double_t sigma;
    Double_t trueSigma;
    Bool_t rangeUsed;
};


const Bool_t SortParametersByEdges(const OneDimParameters &p1,
        const OneDimParameters &p2);

//return the one-sided (RHS) probability
inline const Double_t ProbNormal_c(const Double_t& x,
        const Double_t& sigma=1, const Double_t& x0=0);

inline const Double_t alphaParameter(const Double_t& highRange,
        const Double_t& lowRange, const Double_t& fitSigma);

BinPair GetLowHighBinsFromLowEnd(TH1D* const hFineBinning,
        const Double_t& lowEdge, const Double_t& nMCStatsMin);

BinPair GetLowHighBinsFromHighEnd(TH1D* hFineBinning, const Double_t& highEdge,
        const Double_t& nMCStatsMin);

Double_t GetSigma(const char* const trueVar, const char* const recoVar,
        Double_t recoLowValue, Double_t recoHighValue, TCut cuts,
        Double_t minBinWidth, Int_t limitEntries);

inline const Double_t CalculateSystematicError(const Double_t& alpha);

void CalculateErrors(const TH1D* hist, const Edge& lowEdge, const Edge& upEdge,
        const Double_t& sigma, Double_t& errorStat, Double_t& errorSystematic,
        Double_t& alpha);

void oneDimResidualsWithSpecialErrorTreatment(const SampleId::SampleEnum &sampleID,
        const std::string recoPlotVar, const std::string histParametersTuple,
        const TCut cuts, const std::string truePlotVar,
        const std::string xAxisTitle = "X var [Units]",
        const Bool_t truePlotVarIsAResidual = kFALSE,
        const std::string outputName = "", const Double_t lowestBinEdge = -1,
        const Double_t highestBinEdge = -1, const Double_t minBinWidth = -1,
        const Long64_t limitEntries = -1)
{

    P0DBANFFInterface interface;
    interface.GetThisStyle()->SetOptStat(0000);
    interface.LoadColorBlindPalette();

    const std::string histRefName = "histReference";
    const DefineCuts defCuts;
    const T2KDataMC t2kDataMC;
    const SampleId samples;
    const TotalPOT T2KPOT;
    const TString RunSyst_New_TTree = "all";
    const TString file_path = getenv("SYSTEMATICSROOT");
    const Double_t statsLimit = 150;
    const Int_t nBinsFine = 20000;
    eventTree = new TChain("all", "");

    const TCut sampleCut = defCuts.GetNominalSelectionCut(sampleID);
    TCut allCuts = sampleCut && defCuts.tFV;
    if(((TString)cuts.GetTitle()).Length() > 0)
    {
        allCuts = allCuts&&cuts;
    }

    if(((TString)sampleCut.GetTitle()).Length() < 1)
    {
        P0DBANFFInterface::Error("oneDimResidualsWithSpecialErrorTreatment", TString::Format("Unable to determine the sample cut using SampleEnum=%d", samples.Convert(sampleID)));
    }

    if(samples.IsP0DAirSample(sampleID) && samples.IsP0DNuMuSample(sampleID))
    {
        //TChain* chn_NEUTRun2Air = t2kDataMC.RUN2A.GetAllChainsFrom(RunSyst_New_TTree, file_path);
        //TChain* chn_NEUTRun3bAir = t2kDataMC.RUN3B.GetAllChainsFrom(RunSyst_New_TTree, file_path);
        //TChain* chn_NEUTRun3cAir = t2kDataMC.RUN3C.GetAllChainsFrom(RunSyst_New_TTree, file_path);
        TChain* chn_NEUTRun4Air = t2kDataMC.RUN4A.GetAllChainsFrom(RunSyst_New_TTree, file_path);
        //TChain* chn_NEUTRun8Air = t2kDataMC.RUN8A.GetAllChainsFrom(RunSyst_New_TTree, file_path);
        //eventTree->Add(chn_NEUTRun2Air);
        //eventTree->Add(chn_NEUTRun3bAir);
        //eventTree->Add(chn_NEUTRun3cAir);
        eventTree->Add(chn_NEUTRun4Air);
        //eventTree->Add(chn_NEUTRun8Air);
        POTweight = T2KPOT.GetPOTFHCAirData()/T2KPOT.GetPOTRun4AirMC();
        allCuts = allCuts && defCuts.tLepMuMinus;
    }
    else if(samples.IsP0DWaterSample(sampleID) && samples.IsP0DNuMuSample(sampleID))
    {
        //TChain* chn_NEUTRun2Wtr = t2kDataMC.RUN2W.GetAllChainsFrom(RunSyst_New_TTree, file_path);
        TChain* chn_NEUTRun4Wtr = t2kDataMC.RUN4W.GetAllChainsFrom(RunSyst_New_TTree, file_path);
        //TChain* chn_NEUTRun8Wtr = t2kDataMC.RUN8W.GetAllChainsFrom(RunSyst_New_TTree, file_path);
        //eventTree->Add(chn_NEUTRun2Wtr);
        eventTree->Add(chn_NEUTRun4Wtr);
        //eventTree->Add(chn_NEUTRun8Wtr);
        POTweight = T2KPOT.GetPOTFHCWaterData()/T2KPOT.GetPOTRun4WaterMC();
        allCuts = allCuts && defCuts.tLepMuMinus;
    }
    else if(samples.IsP0DWaterSample(sampleID) && samples.IsP0DRHCSample(sampleID))
    {
        TChain* chn_NEUTRun5cWtr = t2kDataMC.RUN5C.GetAllChainsFrom(RunSyst_New_TTree, file_path);
        //TChain* chn_NEUTRun7bWtr = t2kDataMC.RUN7B.GetAllChainsFrom(RunSyst_New_TTree, file_path);
        eventTree->Add(chn_NEUTRun5cWtr);
        //eventTree->Add(chn_NEUTRun7bWtr);
        if(samples.IsP0DNuMuBkgInAntiNuModeSample(sampleID))
            allCuts = allCuts && defCuts.tLepMuMinus;
        else
            allCuts = allCuts && defCuts.tLepMuPlus;
        POTweight = T2KPOT.GetPOTRHCWaterData()/T2KPOT.GetPOTRun5cWaterMC();
    }
    else if(samples.IsP0DAirSample(sampleID) && samples.IsP0DRHCSample(sampleID))
    {
        TChain* chn_NEUTRun6bAir = t2kDataMC.RUN6B.GetAllChainsFrom(RunSyst_New_TTree, file_path);
        //TChain* chn_NEUTRun6cAir = t2kDataMC.RUN6C.GetAllChainsFrom(RunSyst_New_TTree, file_path);
        //TChain* chn_NEUTRun6dAir = t2kDataMC.RUN6D.GetAllChainsFrom(RunSyst_New_TTree, file_path);
        //TChain* chn_NEUTRun6eAir = t2kDataMC.RUN6E.GetAllChainsFrom(RunSyst_New_TTree, file_path);
        eventTree->Add(chn_NEUTRun6bAir);
        //eventTree->Add(chn_NEUTRun6cAir);
        //eventTree->Add(chn_NEUTRun6dAir);
        //eventTree->Add(chn_NEUTRun6eAir);
        if(samples.IsP0DNuMuBkgInAntiNuModeSample(sampleID))
            allCuts = allCuts && defCuts.tLepMuMinus;
        else
            allCuts = allCuts && defCuts.tLepMuPlus;
        POTweight = T2KPOT.GetPOTRHCAirData()/T2KPOT.GetPOTRun6bAirMC();
    }
    else
    {
        P0DBANFFInterface::Error("oneDimResidualsWithSpecialErrorTreatment", TString::Format("Unable to determine the sample TChain using SampleEnum=%d", samples.Convert(sampleID)));
        throw;
    }

    //gROOT->SetBatch(1);
    const TCut allCutsCopy(allCuts);
    const Long64_t nEntries = limitEntries <= 0 ? eventTree->GetEntries() : limitEntries;

    P0DBANFFInterface::Announce("oneDimResidualsWithSpecialErrorTreatment", "Getting event list");
    eventTree->SetEventList(0);
    eventTree->Draw(">>eList",allCutsCopy);
    TEventList* eList = static_cast<TEventList*>(gDirectory->Get("eList"));
    eventTree->SetEventList(eList);
    P0DBANFFInterface::Announce("oneDimResidualsWithSpecialErrorTreatment", "...Done!");

    const TString weights = "WeightNom*FluxWeightNom";
    P0DBANFFInterface::Announce("oneDimResidualsWithSpecialErrorTreatment", "Creating regular histogram using input specifications");
    //TCanvas* canvas = new TCanvas("canvas", "", 800, 600);
    //canvas->cd();
    eventTree->Draw(Form("%s>>%s%s",recoPlotVar.c_str(),histRefName.c_str(), histParametersTuple.c_str()),
                    Form("(%s)*(%s)",weights.Data(),allCutsCopy.GetTitle()),
                    "goff",
                    nEntries);
    //P0DBANFFInterface::SaveCanvasAs(canvas, histRefName.c_str());
    P0DBANFFInterface::Announce("oneDimResidualsWithSpecialErrorTreatment", "...Done!");

    TH1D* histRef = static_cast<TH1D*>( gDirectory->Get(histRefName.c_str()) );
    if(!histRef){
        std::cout << "ERROR! No histogram with name " << histRefName.c_str() << std::endl;
        return;
    }

    residualTruePlotVar = truePlotVarIsAResidual;
    const Double_t highRef = histRef->GetXaxis()->GetBinUpEdge(histRef->GetNbinsX());
    const Double_t lowRef = histRef->GetBinLowEdge(1);
    //const Double_t uniformBinWidth = (highRef-lowRef)/histRef->GetNbinsX();
    //a sansity check that steps in the finest binning is not too infinitessimal
    //const Double_t limitStepSize = 0.1*uniformBinWidth;
    TH1D* histFineBinning = new TH1D("histFineBinning","",nBinsFine,lowRef,highRef);

    P0DBANFFInterface::Announce("oneDimResidualsWithSpecialErrorTreatment", "Getting finely binned histogram");
    //canvas->cd();
    eventTree->Draw(Form("%s>>histFineBinning",recoPlotVar.c_str()),Form("(%s)*(%s)",weights.Data(),allCutsCopy.GetTitle()),"goff",nEntries);
    //P0DBANFFInterface::SaveCanvasAs(canvas, "histFineBinning");
    P0DBANFFInterface::Announce("oneDimResidualsWithSpecialErrorTreatment", "...Done!");

    std::vector<OneDimParameters> allParams;
    Int_t UpEdge = 1;
    Int_t LowEdge = 0;
    Double_t alpha = 0;
    Double_t sigma = 0;
    Double_t errorStat = -1;
    Double_t errorSystematic = -2;

    const Int_t nShifts = 1;
    Edge *middleEdge = new Edge(histRef,histFineBinning,LowEdge);
    //use slightly wider reco bins to get accurate residual fit
    Edge *tempLowEdge = new Edge(histRef,histFineBinning,LowEdge);
    Edge *tempUpEdge  = new Edge(histRef,histFineBinning,UpEdge);

    P0DBANFFInterface::Announce("oneDimResidualsWithSpecialErrorTreatment", "Determining first bin edges to analyze");
    do{
        if(histFineBinning->Integral(tempLowEdge->bin,middleEdge->bin) > 0.5*statsLimit
            && histFineBinning->GetXaxis()->GetBinLowEdge(middleEdge->bin) - histFineBinning->GetXaxis()->GetBinLowEdge(tempLowEdge->bin) > 0.5*minBinWidth)
        {
            break;
        }
        if(!tempLowEdge->MoveLow())
        {
            break;
        }
    }while(1);
    P0DBANFFInterface::Announce("oneDimResidualsWithSpecialErrorTreatment", TString::Format("Got low edge = %.3f", histFineBinning->GetXaxis()->GetBinLowEdge(tempLowEdge->bin)));
    delete middleEdge;
    do{
        if(histFineBinning->Integral(tempLowEdge->bin,tempUpEdge->bin) > statsLimit
               && histFineBinning->GetXaxis()->GetBinUpEdge(tempUpEdge->bin) - histFineBinning->GetXaxis()->GetBinLowEdge(tempLowEdge->bin) > minBinWidth)
        {
            break;
        }
        if(!tempUpEdge->MoveUp())
        {
            break;
        }
    }while(1);
    P0DBANFFInterface::Announce("oneDimResidualsWithSpecialErrorTreatment", TString::Format("Got up edge = %.3f", histFineBinning->GetXaxis()->GetBinLowEdge(tempUpEdge->bin)));

    P0DBANFFInterface::Announce("oneDimResidualsWithSpecialErrorTreatment", "Determining first bin edges simga");
    sigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),tempLowEdge->edge,tempUpEdge->edge,allCutsCopy,minBinWidth, nEntries);
    delete tempLowEdge;
    delete tempUpEdge;
    P0DBANFFInterface::Announce("oneDimResidualsWithSpecialErrorTreatment", "...Done!");

    Edge lowEdge(histRef, histFineBinning, LowEdge);
    Edge upEdge(histRef, histFineBinning, UpEdge);
    CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);

    std::cout << lowEdge.bin << std::endl;
    std::cout << upEdge.bin <<  std::endl;

    Edge previousLowEdge = lowEdge;
    Edge previousUpEdge = upEdge;
    //now that we have a sigma, need to find how many events fall outside reco range for max bin plus extra needed width
    do {
        CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
        //std::cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << std::endl;
        //std::cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << std::endl;
        //if(errorStat >= errorSystematic && pow(errorStat,-2.)/POTweight >= 500){
        if(errorStat >= errorSystematic && 1./(errorStat*errorStat) > statsLimit){
            break;
        }
        Edge previousLowEdge = lowEdge;
        //do{
            lowEdge.MoveLow();
            if(lowEdge.bin == lowEdge.firstNonZeroBin)
                break;
        //}while(histFineBinning->Integral(lowEdge.bin,previousLowEdge.bin) <= 1);
        //std::cout << lowEdge.bin << std::endl;
        //std::cout << upEdge.bin << std::endl;
        CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
        //std::cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << std::endl;
        //std::cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << std::endl;
        //if(errorStat >= errorSystematic && pow(errorStat,-2.)/POTweight >= 500){
        if(errorStat >= errorSystematic && 1./(errorStat*errorStat) > statsLimit){
            break;
        }
        Edge previousUpEdge = upEdge;
        //do{
            upEdge.MoveUp();
            if(upEdge.bin == upEdge.lastNonZeroBin)
                break;
        //}while(histFineBinning->Integral(previousUpEdge.bin,upEdge.bin) <= 1);

        //std::cout << lowEdge.bin << std::endl;
        //std::cout << upEdge.bin << std::endl;
        //std::cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << std::endl;
        //std::cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << std::endl;
    }while(1);

    //remember the edges we already compared
    Edge peakBinLowEdge = lowEdge;
    Edge peakBinUpEdge = upEdge;
    Double_t nMCStats = histFineBinning->Integral(peakBinLowEdge.bin,peakBinUpEdge.bin);
    Double_t nMCStatsMin = nMCStats; //histFineBinning->Integral(peakBinLowEdge.bin,peakBinUpEdge.bin);
    Double_t previousBinMCStats = nMCStats;
    std::cout << "nMCStatsMin = " << nMCStatsMin << std::endl;

    OneDimParameters peakBin;
    peakBin.low = lowEdge.edge;
    peakBin.up    = upEdge.edge;
    peakBin.sigma = sigma;
    peakBin.trueSigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),lowEdge.edge,upEdge.edge,allCutsCopy,minBinWidth, nEntries);
    peakBin.nEntriesMC = nMCStats;
    peakBin.nEntriesData = nMCStats*POTweight;
    peakBin.sysFracErr = errorSystematic;
    peakBin.statFracErr = errorStat;

    allParams.push_back(peakBin);
    std::cout << "saved a parameter for bins " << lowEdge.bin << " to " << upEdge.bin    << std::endl;
    std::cout << "                                            edge " << lowEdge.edge << " to " << upEdge.edge    << std::endl;
    std::cout << "N = " << peakBin.nEntriesData << " and errorStat = " << errorStat << std::endl;
    std::cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << std::endl;

    std::cout << "process higher bins" << std::endl;
    lowEdge.ShiftToBin(peakBinUpEdge.bin+1);
    //upEdge.ShiftToBin(upEdge.lastNonZeroBin);

    //now process higher bins
    while(lowEdge.bin < lowEdge.lastNonZeroBin){
        //nMCStatsMin = max(nMCStatsMin,1.1*previousBinMCStats);
        BinPair lowHighBins = GetLowHighBinsFromLowEnd(histFineBinning,lowEdge.edge,nMCStatsMin);
        upEdge.ShiftToBin(lowHighBins.second);

        //std::cout << lowEdge.bin << std::endl;
        //std::cout << upEdge.bin << std::endl;
        std::cout << "Number of MC entries is " << histFineBinning->Integral(lowEdge.bin,upEdge.bin) << std::endl;

        //if we cannot satisfy the stats condition with the widest bin possible
        //if(histFineBinning->Integral(lowEdge.bin,upEdge.bin) <= 0.25*previousBinMCStats && abs(upEdge.bin-upEdge.lastNonZeroBin)<=1){
        if(histFineBinning->Integral(lowEdge.bin,upEdge.bin) <= 0.25*nMCStatsMin && abs(upEdge.bin-upEdge.lastNonZeroBin)<=1){
            std::cout << "Merging previous high bin with current range" << std::endl;
            std::cout << lowEdge.bin << std::endl;
            std::cout << upEdge.bin << std::endl;
            lowEdge = previousLowEdge;
            upEdge.ShiftToBin(upEdge.lastNonZeroBin);

            sigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),lowEdge.edge,upEdge.edge,allCutsCopy,minBinWidth, nEntries);
            CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
            Double_t nStatsMC = histFineBinning->Integral(lowEdge.bin,upEdge.bin);
            previousBinMCStats = nStatsMC;
            OneDimParameters params;
            params.low = lowEdge.edge;
            params.up    = upEdge.edge;
            params.sigma = sigma;
            params.trueSigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),lowEdge.edge,upEdge.edge,allCutsCopy, minBinWidth, nEntries);
            params.nEntriesMC = nStatsMC;
            peakBin.nEntriesData = nStatsMC*POTweight;
            params.sysFracErr = errorSystematic;
            params.statFracErr = errorStat;
            if(fabs(lowEdge.edge-allParams.back().low) <= 0.01 && (upEdge.edge-lowEdge.edge) > (allParams.back().up-allParams.back().low)){
                allParams.pop_back();
                std::cout << "Previous entry removed" << std::endl;
            }
            allParams.push_back(params);

            std::cout << "saved a parameter for bins " << lowEdge.bin << " to " << upEdge.bin    << std::endl;
            std::cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << std::endl;
            std::cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << std::endl;
            //now that we did this, we should shift the bins to be at the far end
            lowEdge.ShiftToBin(lowEdge.lastNonZeroBin);
            upEdge.ShiftToBin(upEdge.lastNonZeroBin);
            break;
        }

        //std::cout << lowEdge.bin << std::endl;
        //std::cout << upEdge.bin << std::endl;
        sigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),lowEdge.edge,upEdge.edge,allCutsCopy,minBinWidth, nEntries);


        //if(histFineBinning->FindBin(upEdge.edge*0.9) >= lowEdge.bin){
        //    upEdge.ShiftToBin(histFineBinning->FindBin(upEdge.edge*0.9));
        //}
        upEdge.ShiftToBin(lowEdge.bin);
        CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
        //std::cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << std::endl;
        while(errorStat > 0.5 || errorStat == TMath::Infinity() || 1./(errorStat*errorStat) <= 0 || TMath::IsNaN(errorStat)){
            upEdge.MoveUp();
            CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
            //std::cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << std::endl;
        }

        CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
        Int_t nIterations = 0;
        while(errorStat <= errorSystematic || 1./(errorStat*errorStat) < statsLimit){
            for(Int_t i = 0;i<nShifts;i++){
                upEdge.MoveUp();
            }
            if(upEdge.bin >= upEdge.lastNonZeroBin){
                upEdge.ShiftToBin(upEdge.lastNonZeroBin);
                sigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),lowEdge.edge,upEdge.edge,allCutsCopy,minBinWidth, nEntries);
                CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
                break;
            }

            //std::cout << lowEdge.bin << std::endl;
            //std::cout << upEdge.bin << std::endl;
            CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
            //std::cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << std::endl;
            //std::cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << std::endl;
            nIterations++;

        }

        if(nIterations == 0){
            std::cout << "first attempt to find appropriate binning by increasing the size failed. Attemping to move downwards" << std::endl;

            std::cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << std::endl;
            std::cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << std::endl;
            //upEdge.ShiftToBin(lowHighBins.second);
            upEdge.ShiftToBin(GetLowHighBinsFromLowEnd(histFineBinning,lowEdge.edge,3.*nMCStatsMin).second);
            while(errorSystematic <= errorStat || 1./(errorStat*errorStat) < statsLimit){
            //while(errorSystematic <= errorStat){
                for(Int_t i = 0;i<nShifts;i++){
                    upEdge.MoveLow();
                }

                //std::cout << lowEdge.bin << std::endl;
                //std::cout << upEdge.bin << std::endl;
                CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
                std::cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << std::endl;
                std::cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << std::endl;
                nIterations++;
            }
        }
        Double_t nStatsMC = histFineBinning->Integral(lowEdge.bin,upEdge.bin);
        OneDimParameters params;
        params.low = lowEdge.edge;
        params.up    = upEdge.edge;
        params.sigma = sigma;
        params.trueSigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),lowEdge.edge,upEdge.edge,allCutsCopy,minBinWidth, nEntries);
        params.nEntriesMC = nStatsMC;
        params.nEntriesData = nStatsMC*POTweight;
        params.sysFracErr = errorSystematic;
        params.statFracErr = errorStat;
        allParams.push_back(params);
        //nMCStatsMin = nStatsMC;
        previousBinMCStats = nStatsMC;

        std::cout << "saved a parameter for bins " << lowEdge.bin << " to " << upEdge.bin    << std::endl;
        std::cout << "                                            edge " << lowEdge.edge << " to " << upEdge.edge    << std::endl;
        std::cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << std::endl;
        std::cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << std::endl;

        previousUpEdge = upEdge;
        previousLowEdge = lowEdge;

        lowEdge.ShiftToBin(upEdge.bin+1);
        upEdge.ShiftToBin(upEdge.lastNonZeroBin);
        std::cout << lowEdge.bin << std::endl;
        std::cout << upEdge.bin << std::endl;

    }

    std::cout << lowEdge.bin << std::endl;
    std::cout << upEdge.bin << std::endl;

    std::cout << "done with upper bins" << std::endl;

    //sort(allParams.begin(),allParams.end(),SortParametersByEdges);
    //for(UInt_t i = 0;i<allParams.size();i++){
    //    std::cout << "low Edge = " << allParams[i].low << std::endl;
    //    std::cout << "N = " << allParams[i].nEntriesData << std::endl;
    //    std::cout << "errorStat = " << allParams[i].statFracErr << std::endl;
    //    std::cout << "errorSystematic = " << allParams[i].sysFracErr << std::endl;
    //    std::cout << "si = " << allParams[i].sigma << std::endl;
    //    std::cout << "up Edge = " << allParams[i].up << std::endl;
    //}

    upEdge.ShiftToBin(peakBinLowEdge.bin-1);
    lowEdge.ShiftToBin(lowEdge.firstNonZeroBin);

    previousUpEdge = upEdge;
    previousLowEdge = lowEdge;

    std::cout << lowEdge.bin << std::endl;
    std::cout << upEdge.bin << std::endl;

    //work on lower bins
    std::cout << "lower bins" << std::endl;
    while(upEdge.bin > upEdge.firstNonZeroBin){

        //nMCStatsMin = max(nMCStatsMin,1.1*previousBinMCStats);
        BinPair lowHighBins = GetLowHighBinsFromHighEnd(histFineBinning,upEdge.edge,nMCStatsMin);
        lowEdge.ShiftToBin(lowHighBins.first);
        //if we cannot satisfy the stats condition with the widest bin possible
        if(histFineBinning->Integral(lowEdge.bin,upEdge.bin) <= 0.25*nMCStatsMin && abs(lowEdge.bin-lowEdge.firstNonZeroBin)<=1){

            lowEdge.ShiftToBin(lowEdge.firstNonZeroBin);
            upEdge = previousUpEdge;
            sigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),lowEdge.edge,upEdge.edge,allCutsCopy,minBinWidth, nEntries);
            CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
            Double_t nStatsMC = histFineBinning->Integral(lowEdge.bin,upEdge.bin);
            previousBinMCStats = nStatsMC;
            OneDimParameters params;
            params.low = lowEdge.edge;
            params.up    = upEdge.edge;
            params.sigma = sigma;
            params.trueSigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),lowEdge.edge,upEdge.edge,allCutsCopy,minBinWidth, nEntries);
            params.nEntriesMC = nStatsMC;
            params.nEntriesData = nStatsMC*POTweight;
            params.sysFracErr = errorSystematic;
            params.statFracErr = errorStat;
            if(fabs(upEdge.edge-allParams.back().up) <= 0.01 && (upEdge.edge-lowEdge.edge) > (allParams.back().up-allParams.back().low)){
                allParams.pop_back();
                std::cout << "Previous entry removed" << std::endl;
            }
            allParams.push_back(params);

            std::cout << "saved a parameter in special check for bins " << lowEdge.bin << " to " << upEdge.bin    << std::endl;
            std::cout << "                                                                             edges" << lowEdge.edge << " to " << upEdge.edge    << std::endl;
            std::cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << std::endl;
            std::cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << std::endl;
            lowEdge.ShiftToBin(lowEdge.firstNonZeroBin);
            upEdge.ShiftToBin(upEdge.firstNonZeroBin);
            break;
        }

        //std::cout << lowEdge.bin << std::endl;
        //std::cout << upEdge.bin << std::endl;
        sigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),lowEdge.edge,upEdge.edge,allCutsCopy,minBinWidth, nEntries);

        lowEdge.ShiftToBin(upEdge.bin);
        CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
        //std::cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << std::endl;
        while(errorStat > 0.5 || errorStat == TMath::Infinity() || 1./(errorStat*errorStat) <= 0 || TMath::IsNaN(errorStat)){
        //while(errorStat > 0.5 || errorStat == TMath::Infinity()){
            lowEdge.MoveLow();
            CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
            //std::cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << std::endl;
        }

        //std::cout << "lowEdge bin = " << lowEdge.bin << std::endl;
        //std::cout << "upEdge bin = " << upEdge.bin << std::endl;
        CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
        std::cout << "upEdge bin = " << upEdge.bin << std::endl;

        Int_t nIterations = 0;
        while(errorStat <= errorSystematic || 1./(errorStat*errorStat) < statsLimit){
            for(Int_t i = 0;i<nShifts;i++){
                lowEdge.MoveLow();
            }
            if(lowEdge.bin <= lowEdge.firstNonZeroBin){
                lowEdge.ShiftToBin(lowEdge.firstNonZeroBin);
                sigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),lowEdge.edge,upEdge.edge,allCutsCopy,minBinWidth, nEntries);
                CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
                break;
            }
            std::cout << "lowEdge bin = " << lowEdge.bin << std::endl;
            CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
            std::cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << std::endl;
            std::cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << std::endl;
            nIterations++;

        }
        if(nIterations == 0){
            std::cout << "first attempt to find appropriate binning by increasing the size failed. Attemping to move downwards" << std::endl;
            lowEdge.ShiftToBin(lowHighBins.first);
            while(errorSystematic <= errorStat || 1./(errorStat*errorStat) < statsLimit){
                for(Int_t i = 0;i<nShifts;i++){
                    lowEdge.MoveUp();
                }

                //std::cout << lowEdge.bin << std::endl;
                //std::cout << upEdge.bin << std::endl;
                CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
                //std::cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << std::endl;
                //std::cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << std::endl;
                nIterations++;
            }
        }
        Double_t nStatsMC = histFineBinning->Integral(lowEdge.bin,upEdge.bin);
        OneDimParameters params;
        params.low = lowEdge.edge;
        params.up    = upEdge.edge;
        params.sigma = sigma;
        params.trueSigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),lowEdge.edge,upEdge.edge,allCutsCopy,minBinWidth, nEntries);
        params.nEntriesMC = nStatsMC;
        params.nEntriesData = nStatsMC*POTweight;
        params.sysFracErr = errorSystematic;
        params.statFracErr = errorStat;
        allParams.push_back(params);
        //nMCStatsMin = nStatsMC;
        previousBinMCStats = nStatsMC;

        std::cout << "saved a parameter for bins " << lowEdge.bin << " to " << upEdge.bin    << std::endl;
        std::cout << "                                            edges" << lowEdge.edge << " to " << upEdge.edge    << std::endl;
        std::cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << std::endl;
        std::cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << std::endl;

        previousUpEdge = upEdge;
        previousLowEdge = lowEdge;

        upEdge.ShiftToBin(lowEdge.bin-1);
        lowEdge.ShiftToBin(lowEdge.firstNonZeroBin);
    }
    std::cout << "done with lower bins" << std::endl;

    std::cout << "number of parameter sets saved = " << allParams.size() << std::endl;
    sort(allParams.begin(),allParams.end(),SortParametersByEdges);

    OneDimParameters lowestPossibleEdge;
    lowestPossibleEdge.low = lowestBinEdge;
    lowestPossibleEdge.sigma = 0;
    lowestPossibleEdge.trueSigma = 0;
    lowestPossibleEdge.nEntriesMC = 0;
    lowestPossibleEdge.nEntriesData = 0;
    lowestPossibleEdge.sysFracErr = 0;
    lowestPossibleEdge.statFracErr = 0;

    OneDimParameters highestPossibleEdge;
    highestPossibleEdge.up = highestBinEdge;
    highestPossibleEdge.sigma = 0;
    highestPossibleEdge.trueSigma = 0;
    highestPossibleEdge.nEntriesMC = 0;
    highestPossibleEdge.nEntriesData = 0;
    highestPossibleEdge.sysFracErr = 0;
    highestPossibleEdge.statFracErr = 0;

    if(allParams.back().up < highestPossibleEdge.up && highestBinEdge != -1){
        highestPossibleEdge.low = allParams.back().up;
        allParams.push_back(highestPossibleEdge);
    }

    if(allParams.front().low > lowestPossibleEdge.low && lowestBinEdge != -1){
        lowestPossibleEdge.up = allParams.front().low;
        allParams.push_back(lowestPossibleEdge);
    }

    sort(allParams.begin(),allParams.end(),SortParametersByEdges);

    std::vector<double> binEdgesXVect_augmented;
    std::vector<double> graphMiddleXVect_augmented;
    std::vector<double> graphErrXVect_augmented;
    std::vector<double> graphYVectVect_augmented;
    std::vector<double> graphErr1YVect_augmented;
    std::vector<double> graphErr2YVect_augmented;

    //binEdgesXVect_augmented.push_back(lowestBinEdge);
    for(UInt_t i = 0 ; i<allParams.size() ; i++){
        std::cout << "low Edge = " << allParams[i].low << std::endl;
        //std::cout << "errorStat = " << pow(allParams[i].nEntriesData,-0.5) << std::endl;
        //std::cout << "errorStat = " << allParams[i].statFracErr << std::endl;
        //std::cout << "errorSystematic = " << allParams[i].sysFracErr << std::endl;
        //std::cout << "si = " << allParams[i].sigma << std::endl;
        //std::cout << "up Edge = " << allParams[i].up << std::endl;
        //const Double_t N = allParams[i].nEntriesData;
        const Double_t N = allParams[i].statFracErr == 0 ? 0 : pow(allParams[i].statFracErr,-2.);
        binEdgesXVect_augmented.push_back(allParams[i].low);
        graphMiddleXVect_augmented.push_back(0.5*(allParams[i].up+allParams[i].low));
        graphErrXVect_augmented.push_back(0.5*(allParams[i].up-allParams[i].low));
        graphYVectVect_augmented.push_back(N);
        //graphErr1YVect_augmented.push_back(N*errorStatprime);
        //graphErr2YVect_augmented.push_back(N*sqrt(pow(errorStatprime,2)+pow(allParams[i].sysFracErr,2)));

        graphErr1YVect_augmented.push_back(sqrt(N));
        graphErr2YVect_augmented.push_back(N*sqrt( 1./N + pow(allParams[i].sysFracErr,2)));
    }
    binEdgesXVect_augmented.push_back(allParams.back().up);
    //binEdgesXVect_augmented.push_back(highestBinEdge);
    const Int_t nBins_augmented = binEdgesXVect_augmented.size()-1;
    double* binEdgesX_augmented = &binEdgesXVect_augmented[0];


    const Int_t nPoints = graphMiddleXVect_augmented.size();
    double* graphMiddleX_augmented = &graphMiddleXVect_augmented[0];
    double* graphErrX_augmented    = &graphErrXVect_augmented[0];
    double* graphY_augmented       = &graphYVectVect_augmented[0];
    double* graphErr1Y_augmented   = &graphErr1YVect_augmented[0];
    double* graphErr2Y_augmented   = &graphErr2YVect_augmented[0];


    TCanvas* gCanvas = new TCanvas("gCanvas","",800,800);
    gCanvas->cd();
    gCanvas->SetLeftMargin(.135);
    gCanvas->SetBottomMargin(.125);
    gCanvas->SetGridx();
    gCanvas->SetGridy();

    TMultiGraph* mg = new TMultiGraph();
    TGraphErrors* gStatErrors = new TGraphErrors(nPoints,graphMiddleX_augmented,graphY_augmented,graphErrX_augmented,graphErr1Y_augmented);
    TGraphErrors* gSysErrors  = new TGraphErrors(nPoints,graphMiddleX_augmented,graphY_augmented,graphErrX_augmented,graphErr2Y_augmented);
    TH1D* hNoErrors = new TH1D("hNoErrors","",nBins_augmented,binEdgesX_augmented);
    for(UInt_t i = 0;i<allParams.size();i++){
        //const Double_t N = pow(allParams[i].statFracErr,-2.);
        const Double_t N = allParams[i].statFracErr == 0 ? 0 : pow(allParams[i].statFracErr,-2.);
        hNoErrors->SetBinContent(hNoErrors->FindBin(allParams[i].low),N);
        hNoErrors->SetBinError(hNoErrors->FindBin(allParams[i].low),0);

    }

    P0DBANFFInterface::PrettyUpTH1(hNoErrors, xAxisTitle.c_str(), "Entries per bin", kBlack);
    gStatErrors->SetFillColor(P0DBANFFInterface::kcbBlue);
    gSysErrors->SetFillColor(P0DBANFFInterface::kcbRed);

    mg->Add(gSysErrors);
    mg->Add(gStatErrors);
    mg->Draw("AP5");
    mg->SetMinimum(0);
    mg->GetXaxis()->SetTitle(xAxisTitle.c_str());
    mg->GetYaxis()->SetTitle("Entries per bin");
    mg->GetYaxis()->SetTitleOffset(mg->GetYaxis()->GetTitleOffset()*1.25);
    hNoErrors->SetMinimum(0);
    hNoErrors->GetXaxis()->SetTitle(xAxisTitle.c_str());
    hNoErrors->GetYaxis()->SetTitle("Entries per bin");
    hNoErrors->GetYaxis()->SetTitleOffset(mg->GetYaxis()->GetTitleOffset()*1.25);
    mg->Draw("AP5");
    hNoErrors->Draw("same");
    histRef->Scale(hNoErrors->GetMaximum()/histRef->GetMaximum());
    histRef->SetFillStyle(3001);
    histRef->SetFillColor(kBlack);
    histRef->SetLineWidth(0);
    histRef->Draw("same");

    TLegend* gLegend = new TLegend(0.187,0.679,0.366,0.86,"");
    gLegend->AddEntry(gStatErrors,"Stat. Error","F");
    gLegend->AddEntry(gSysErrors,"Total Error","F");

    gLegend->SetLineStyle(0);
    gLegend->SetBorderSize(0);
    gLegend->SetFillColor(kWhite);
    gLegend->Draw();
    gCanvas->SetLogx();
    P0DBANFFInterface::SaveCanvasAs(gCanvas, TString::Format("plots/%serrors", outputName.c_str()));

    TCanvas* hCanvas = new TCanvas("hCanvas","",800,800);
    hCanvas->cd();

    TH1D* hSigmas = new TH1D("hSigmas","",nBins_augmented,binEdgesX_augmented);
    P0DBANFFInterface::PrettyUpTH1(hSigmas, xAxisTitle.c_str(), "1 #sigma range", P0DBANFFInterface::kcbBlack);
    TH1D* htrueSigmas = new TH1D("htrueSigmas","",nBins_augmented,binEdgesX_augmented);
    P0DBANFFInterface::PrettyUpTH1(htrueSigmas, xAxisTitle.c_str(), "1 #sigma range", P0DBANFFInterface::kcbBlue);

    TH1D* hSigmaOverBinCenter = new TH1D("hSigmaOverBinCenter","",nBins_augmented,binEdgesX_augmented);
    TH1D* htrueSigmaSigmaRatio = new TH1D("htrueSigmaSigmaRatio","",nBins_augmented,binEdgesX_augmented);
    P0DBANFFInterface::PrettyUpTH1(hSigmaOverBinCenter, xAxisTitle.c_str(), "1 #sigma range", P0DBANFFInterface::kcbRed);
    hSigmaOverBinCenter->SetLineStyle(2);
    for(UInt_t i = 0;i<allParams.size();i++){
        hSigmas->SetBinContent(i+1,allParams[i].sigma);
        htrueSigmas->SetBinContent(i+1,allParams[i].trueSigma);
        hSigmaOverBinCenter->SetBinContent(i+1,allParams[i].sigma/hSigmaOverBinCenter->GetBinCenter(i+1));
        htrueSigmaSigmaRatio->SetBinContent(i+1,allParams[i].sigma/allParams[i].trueSigma);
    }

    hSigmas->SetMaximum(hSigmas->GetMaximum()*1.1);
    hSigmas->SetMinimum(0);

    hSigmas->Draw();
    htrueSigmas->Draw("same");

    TLegend* hLegend = new TLegend(0.2,0.7,0.45,0.88,"");
    hLegend->AddEntry(hSigmas,"Initial Estimate","L");
    hLegend->AddEntry(htrueSigmas,"After Bin Formed","L");
    hLegend->SetLineStyle(0);
    hLegend->SetLineColor(0);
    hLegend->SetFillStyle(0);
    hLegend->SetBorderSize(0);
    hLegend->Draw();

    hCanvas->Update();
    Double_t rightmax = 1.1*htrueSigmaSigmaRatio->GetMaximum();
    Double_t scale = gPad->GetUymax()/rightmax;
    htrueSigmaSigmaRatio->Scale(scale);
    htrueSigmaSigmaRatio->Draw("same");

    TGaxis* axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),
         gPad->GetUxmax(), gPad->GetUymax(),0,rightmax,510,"+L");
    axis-> SetLineColor(P0DBANFFInterface::kcbRed);
    axis->SetLabelColor(P0DBANFFInterface::kcbRed);
    axis->SetTitleColor(P0DBANFFInterface::kcbRed);
    //axis->SetTitleSize(axis->GetTitleSize()*0.5);
    //axis->SetLabelSize(axis->GetTitleSize());
    axis->SetTitle("Ratio");
    axis->SetTitleOffset(axis->GetTitleOffset()*1.25);
    axis->Draw();

    P0DBANFFInterface::SaveCanvasAs(hCanvas, TString::Format("plots/%ssigmas", outputName.c_str()));

    delete histFineBinning;

    delete axis;
    delete hSigmas;
    delete hSigmaOverBinCenter;
    delete htrueSigmas;
    delete htrueSigmaSigmaRatio;
    delete hCanvas;

    delete gStatErrors;
    delete gSysErrors;
    delete gLegend;
    delete mg;
    delete hNoErrors;
    delete gCanvas;


}

const Bool_t SortParametersByEdges(const OneDimParameters& p1,
        const OneDimParameters& p2)
{
  return p1.low < p2.low;
}

const Double_t ProbNormal_c(const Double_t& x, const Double_t& sigma,
        const Double_t& x0)
{
  return ROOT::Math::normal_cdf_c(x, sigma, x0);
}

const Double_t alphaParameter(const Double_t& highRange,
        const Double_t& lowRange, const Double_t& fitSigma)
{
  return fabs(highRange - lowRange)/(2. * fitSigma);
}


BinPair GetLowHighBinsFromLowEnd(TH1D* const hFineBinning,
        const Double_t& lowEdge, const Double_t& nMCStatsMin)
{
    Int_t retLowBin = hFineBinning->FindBin(lowEdge);
    const Int_t startBin = retLowBin;
    const Int_t endBin = hFineBinning->FindLastBinAbove(0);
    Int_t retHighBin = endBin;
    Double_t integral = 0;
    for(Int_t bin = startBin;bin <= endBin; bin++){
        integral += hFineBinning->Integral(bin,bin);
        if(integral >= nMCStatsMin){
            retHighBin = bin;
            break;
        }
    }
    return BinPair(retLowBin,retHighBin);

}


BinPair GetLowHighBinsFromHighEnd(TH1D* hFineBinning, const Double_t& highEdge,
        const Double_t& nMCStatsMin)
{
  Int_t retHighBin = hFineBinning->FindBin(highEdge);
  const Int_t startBin = retHighBin;
  const Int_t endBin = hFineBinning->FindFirstBinAbove(0);
  Int_t retLowBin = endBin;
  Double_t integral = 0;
  for(Int_t bin = startBin;bin >= endBin; bin--){
    integral += hFineBinning->Integral(bin,bin);
    if(integral >= nMCStatsMin){
      retLowBin = bin;
      break;
    }
  }
  return BinPair(retLowBin,retHighBin);
}


Double_t GetSigma(const char* const trueVar, const char* const recoVar,
        Double_t recoLowValue, Double_t recoHighValue, TCut cuts,
        Double_t minBinWidth, Int_t limitEntries)
{
    Double_t fracChange = 0.;
    const Double_t deltaFracChange = 0.0005;
    Double_t lowCut = recoLowValue;
    Double_t highCut = recoHighValue;
    Double_t sigmas[2] = {2, 1.};
    TH1D* htemp = 0;//static_cast<TH1D*>(gDirectory->Get("htemp"));
    TCanvas* temp = new TCanvas("temp","",800,800);
    temp->cd();
    TF1* normFit = new TF1("normFit","gaus(0)",0.1*recoLowValue,2*recoHighValue);
    P0DBANFFInterface::Announce("GetSigma", "Attempting to find bin edges and sigmas");
    do
    {
        TCut rangeCut = Form("%f<=%s&&%s<%f",lowCut,recoVar,recoVar,highCut);
        std::string DrawCommand = "";//Form("%s>>htemp(32,%f,%f)",trueVar,0.1*lowCut,2*highCut);
        if(residualTruePlotVar)
        {
            Double_t avg = 0.5*(lowCut+highCut);
            DrawCommand = Form("%s>>htemp(32,%f,%f)",trueVar,-1.25*avg,1.25*avg);
        }
        else
            DrawCommand = Form("%s>>htemp(32,%f,%f)",trueVar,0.1*lowCut,2*highCut);

        P0DBANFFInterface::Announce("GetSigma", TString::Format("Range cut \"%s\"",rangeCut.GetTitle()));
        eventTree->Draw(DrawCommand.c_str(),cuts&&rangeCut,"goff",limitEntries);
        htemp = static_cast<TH1D*>(gDirectory->Get("htemp"));
        normFit->SetParameters(htemp->GetMaximum(),htemp->GetXaxis()->GetBinLowEdge(htemp->GetMaximumBin()),0.5*fabs(highCut-lowCut));
        htemp->Fit(normFit);

        while((normFit->GetParameter(0) < 0 || normFit->GetParameter(2) < 0)
                && (htemp->GetNbinsX() >= 3))
        {
            htemp->Rebin(2);
            normFit->SetParameters(htemp->GetMaximum(),
                                   htemp->GetXaxis()->GetBinLowEdge(htemp->GetMaximumBin()),
                                   0.5*fabs(highCut-lowCut));
            htemp->Fit(normFit);
        }
        //std::cout << sigmas[0] << std::endl;
        //std::cout << sigmas[1] << std::endl;
        if(sigmas[1] > 0 && sigmas[0] > 0)
        {
            sigmas[0] = sigmas[1];
            sigmas[1] = normFit->GetParameter(2);
            if(highCut - lowCut > minBinWidth)
            {
                if(sigmas[1] > sigmas[0] && fabs(sigmas[1]/sigmas[0]-1.)<MinFracDiff)
                    break;

                if(sigmas[1] < sigmas[0] && fabs(sigmas[0]/sigmas[1]-1.)<MinFracDiff)
                    break;
            }
        }
        fracChange += deltaFracChange;
        lowCut    *= (1 - fracChange);
        highCut *= (1 + fracChange);
    } while(1);

    //htemp->Draw();
    //temp->SaveAs(Form("plots/range_%6f_%6f.root",recoLowValue,recoHighValue));
    //temp->SaveAs(Form("plots/range_%6f_%6f.png",recoLowValue,recoHighValue));
    //temp->SaveAs(Form("plots/range_%6f_%6f.pdf",recoLowValue,recoHighValue));
    //temp->SaveAs(Form("plots/range_%6f_%6f.eps",recoLowValue,recoHighValue));
    P0DBANFFInterface::Announce("GetSigma", TString::Format("Sigma found! sigma[1]=%f", sigmas[1]));
    if(normFit) delete normFit;
    if(temp) delete temp;

    return sigmas[1];
}


const Double_t CalculateSystematicError(const Double_t& alpha)
{
  return 2.*ProbNormal_c(alpha);
}

void CalculateErrors(const TH1D* hist, const Edge& lowEdge, const Edge& upEdge,
        const Double_t& sigma, Double_t& errorStat, Double_t& errorSystematic,
        Double_t& alpha)
{
  const Double_t nEntries = hist->Integral(lowEdge.bin,upEdge.bin);
  errorStat = 1. / sqrt(POTweight * nEntries);
  alpha = alphaParameter(lowEdge.edge, upEdge.edge, sigma);
  errorSystematic = CalculateSystematicError(alpha);
}

Edge::Edge(TH1D* hReference,TH1D* hEqualStatistics,Bool_t UpEdge)
{
    hRef = hReference;
    hFineBinning = hEqualStatistics;
    bin = hFineBinning->FindBin(hRef->GetXaxis()->GetBinLowEdge(hRef->GetMaximumBin()));
    isUpEdge = UpEdge;
    if(isUpEdge)
        edge = hFineBinning->GetXaxis()->GetBinUpEdge(bin);
    else
        edge = hFineBinning->GetXaxis()->GetBinLowEdge(bin);
    firstNonZeroBin = hFineBinning->FindFirstBinAbove(0);
    lastNonZeroBin = hFineBinning->FindLastBinAbove(0);
}

Bool_t Edge::MoveUp()
{
    Int_t rightBin = bin+1;
    Double_t tempEdge = 0;
    if(isUpEdge)
        tempEdge = hFineBinning->GetXaxis()->GetBinUpEdge(rightBin);
    else
        tempEdge = hFineBinning->GetXaxis()->GetBinLowEdge(rightBin);
    bin = rightBin;
    edge = tempEdge;
    if(rightBin > lastNonZeroBin)
    {
        ShiftToBin(lastNonZeroBin+1);
        return kFALSE;
    }
    return kTRUE;
}

Bool_t Edge::MoveLow()
{
    Int_t leftBin = bin-1;
    Double_t tempEdge = 0;
    if(isUpEdge)
        tempEdge = hFineBinning->GetXaxis()->GetBinUpEdge(leftBin);
    else
        tempEdge = hFineBinning->GetXaxis()->GetBinLowEdge(leftBin);

    bin = leftBin;
    edge = tempEdge;
    if(leftBin < firstNonZeroBin)
    {
        ShiftToBin(firstNonZeroBin);
        return kFALSE;
    }
    return kTRUE;
}

void Edge::ShiftToBin(Int_t newBin)
{
    if(newBin == bin)
        return;
    if(newBin < bin)
    {
        bin = newBin+1;
        MoveLow();
        return;
    }
    if(newBin > bin)
    {
        bin = newBin-1;
        MoveUp();
        return;
    }
}


int main(int argc, char** argv)
{

    const std::string programName = argv[0];
    Char_t usage[2048];
    sprintf(usage, "Usage: %s -i sampleID -o outputfile.root -x recoPlotVariable -t truePlotVariable -h (NbinsX,xLow,xHigh) -m minimumBinWidth \
\n       Additional Parameters: -c additionalCuts=\"\" -T xAxisTitle=\"recoPlotVariable\" \
-r isTrueVariableResidual=kFalse -L lowestBinEdgeValue=-1 -H highestBinEdgeValue=-1 -n limitEntries=-1]", programName.c_str());

    if(argc < 5)
    {
        std::cerr << usage << std::endl;
        return 0;
    }

    using namespace Reset;
    Int_t sampleID = kInt;
    TString outputFileName = kString;
    TString recoPlotVariable = kString;
    TString truePlotVariable = kString;
    TString histogramParameters = kString;
    TString additionalCuts = kString;
    TString xAxisTitle = kString;
    Int_t isTrueVariableResidual = kFALSE;
    TString lowestBinEdgeValue = "-1";
    TString highestBinEdgeValue = "-1";
    TString minBinWidth = "-1";
    Long64_t nmax = TChain::kBigNumber;

    for (;;)
    {
        Int_t c = getopt(argc, argv, "i:o:x:t:h:c:m:T:r:L:H:n:");
        if (c < 0)
            break;
        switch (c)
        {
            case 'n':
            {
                std::istringstream tmp(optarg);
                tmp >> nmax;
                break;
            }
            case 'i':
            {
                std::istringstream tmp(optarg);
                tmp >> sampleID;
                break;
            }
            case 'o':
            {
                outputFileName = optarg;
                break;
            }
            case 'x':
            {
                recoPlotVariable = optarg;
                break;
            }
            case 't':
            {
                truePlotVariable = optarg;
                break;
            }
            case 'h':
            {
                histogramParameters = optarg;
                break;
            }
            case 'c':
            {
                additionalCuts = optarg;
                break;
            }
            case 'm':
            {
                minBinWidth = optarg;
                break;
            }
            case 'T':
            {
                xAxisTitle = optarg;
                break;
            }
            case 'r':
            {
                std::istringstream tmp(optarg);
                tmp >> isTrueVariableResidual;
                break;
            }
            case 'L':
            {
                lowestBinEdgeValue = optarg;
                break;
            }
            case 'H':
            {
                highestBinEdgeValue = optarg;
                break;
            }
            default:
            {
                std::cout << optarg << " is an unknown option" << std::endl;
                std::cout << usage << std::endl;
                throw;
            }
        }
    }//end for

    SampleId sampleIDs;
    if(sampleID <= 0 || !sampleIDs.IsP0DAnySample(sampleIDs.Convert(sampleID)))
    {
        printf("Invalid sampleId = %d\nMust be a P0D sampleId defined in SampleId.hxx", sampleID);
        std::cout << usage << std::endl;
        return 0;
    }
    if(outputFileName.Length() < 1)
    {
        printf("Invalid output name %s\n", outputFileName.Data());
        std::cout << usage << std::endl;
        return 0;
    }
    if(recoPlotVariable.Length() < 1)
    {
        printf("Invalid reco plot variable %s\n", recoPlotVariable.Data());
        std::cout << usage << std::endl;
        return 0;
    }
    if(truePlotVariable.Length() < 1)
    {
        printf("Invalid true plot variable %s\n", truePlotVariable.Data());
        std::cout << usage << std::endl;
        return 0;
    }
    if(!(isTrueVariableResidual == kFALSE || isTrueVariableResidual == kTRUE))
    {
        printf("Invalid parameter isTrueVariableResidual %d\nMust be 0 or 1", isTrueVariableResidual);
        std::cout << usage << std::endl;
        return 0;
    }
    if(histogramParameters.First('(') == -1 || histogramParameters.First(')') == -1 || histogramParameters.CountChar(',') != 2 || histogramParameters.First(',') < histogramParameters.First('(') || histogramParameters.Last(',') > histogramParameters.Last(')'))
    {
        printf("Invalid parameter histogramParameters %s\nMust be (nBinsX,lowX,highX)", histogramParameters.Data());
        std::cout << usage << std::endl;
        return 0;
    }
    if(minBinWidth.Atof() < 0)
    {
        printf("Invalid minimum bin width variabe %s", minBinWidth.Data());
        std::cout << usage << std::endl;
        return 0;
    }
    printf("sampleID %d, recoPlotVariable.Data() %s\n, histogramParameters.Data() %s,\n additionalCuts.Data() %s, truePlotVariable.Data() %s\n, xAxisTitle.Data() %s, isTrueVariableResidual %d\n, outputFileName.Data() %s, lowestBinEdgeValue.Atof() %f, highestBinEdgeValue.Atof() %f, nmax %lld\n",
            sampleID, recoPlotVariable.Data(),
            histogramParameters.Data(),
            additionalCuts.Data(), truePlotVariable.Data(),
            xAxisTitle.Data(), isTrueVariableResidual,
            outputFileName.Data(), lowestBinEdgeValue.Atof(),
            highestBinEdgeValue.Atof(), nmax);

    oneDimResidualsWithSpecialErrorTreatment(sampleIDs.Convert(sampleID), recoPlotVariable.Data(),
            histogramParameters.Data(),
            additionalCuts.Data(), truePlotVariable.Data(),
            xAxisTitle.Data(), isTrueVariableResidual,
            outputFileName.Data(), lowestBinEdgeValue.Atof(),
            highestBinEdgeValue.Atof(), minBinWidth.Atof(), nmax);
    return 0;
}
