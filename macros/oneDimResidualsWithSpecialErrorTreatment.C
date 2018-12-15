/*
 * Make 1D TGraph to get binning recommendations for momentum and angle
 *
 * Procedure is described in TN-333
 *
 * Example: Kalman contained muons
 *    root -b -q -l 'oneDimResidualsWithSpecialErrorTreatment.C(1041003,"momentumMuonHypothesis_Vect","htemp(50,0,10000)","htemp","isContained_Vect==1&&algorithm_Vect==1","tMomentum_Vect","Reco Momentum [MeV/c]",false,"muKalContainedPBins.root")'
 *
 * Example: Kalman P0D->TPC muons
 *    root -b -q -l 'oneDimResidualsWithSpecialErrorTreatment.C(1041003,"theta_Vect*TMath::RadtoDeg()","htemp(50,0.0,90.0)","htemp","isEnteringTPC_Vect==1&&T2PMatchFound_Vect==1&&algorithm_Vect==1","tTheta_Vect","Reco #theta",true,"muKalTPCThetaBins.root")'
*/

#include<utility>
#include<vector>
#include<iostream>
#include<cmath>
#include<algorithm>
#include"TH1D.h"
#include"TCut.h"
#include"TF1.h"
#include"TCanvas.h"
#include"TDirectory.h"
#include"TROOT.h"
#include"TEventList.h"
#include"TGraphErrors.h"
#include"TGraph.h"
#include"TMultiGraph.h"
#include"TLegend.h"
#include"TMath.h"
#include"TGaxis.h"
#include"TSystem.h"
#include"TChain.h"

#include"Math/ProbFunc.h"
#include"Math/DistFunc.h"

typedef std::pair<Int_t, Int_t> BinPair;

//void oneDimResidualsWithSpecialErrorTreatment(const Int_t files, const string recoPlotVar, const string histRefNameAndBins, const string histRefName, const TCut cuts, const string truePlotVar, const string xAxisTitle = "X var [Units]", const Bool_t truePlotVarIsAResidual = false, const string outputName = "", const Double_t lowestBinEdge = -1, const Double_t highestBinEdge = -1, const Int_t limitEntries = -1);

const Double_t MinFracDiff = 2.0e-02; //
bool residualTruePlotVar;
TChain* FT = NULL;
Double_t POTweight = 1;

TMultiGraph* mg;
TGraphErrors* gStatErrors;// = new TGraphErrors(nPoints,graphMiddleX_augmented,graphY_augmented,graphErrX_augmented,graphErr1Y_augmented);
TGraphErrors* gSysErrors ;//= new TGraphErrors(nPoints,graphMiddleX_augmented,graphY_augmented,graphErrX_augmented,graphErr2Y_augmented);
TH1D* herrorStat;
TH1D* herrorSystematic;
using namespace std;

class Edge
{
public:
    Edge() :
        hRef(NULL), hFineBinning(NULL), isUpEdge(false)
    {}
    Edge(TH1D* hReference,TH1D* hEqualStatistics,bool UpEdge);

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


inline const Bool_t SortParametersByEdges(OneDimParameters p1,
        OneDimParameters p2);

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
        Int_t limitEntries);

void GetAllMC(TChain* const chain, const Int_t sampleID);

inline const Double_t CalculateSystematicError(const Double_t& alpha);

void CalculateErrors(const TH1D* hist, const Edge& lowEdge, const Edge& upEdge,
        const Double_t& sigma, Double_t& errorStat, Double_t& errorSystematic,
        Double_t& alpha);


void oneDimResidualsWithSpecialErrorTreatment(const Int_t sampleID,
        const string recoPlotVar, const string histRefNameAndBins,
        const string histRefName, const TCut cuts, const string truePlotVar,
        const string xAxisTitle = "X var [Units]",
        const Bool_t truePlotVarIsAResidual = false,
        const string outputName = "", const Double_t lowestBinEdge = -1,
        const Double_t highestBinEdge = -1, const Long64_t limitEntries = -1)
{

  TChain* const FT = new TChain("all");
  GetAllMC(FT, sampleID);
  gROOT->SetBatch(1);
  const Long64_t nEntries = limitEntries <= 0 ? FT->GetEntries() : limitEntries;

  TCut allCuts;
  if(cuts.GetTitle()){
    allCuts =  allCuts&&cuts;
  }
  const TCut allCutsCopy = allCuts;
  const Double_t POTweight = 1;

  FT->SetEventList(0);
  FT->Draw(">>eList",allCutsCopy);
  TEventList* eList = static_cast<TEventList*>(gDirectory->Get("eList"));
  FT->SetEventList(eList);

  Double_t weight = 1;
  FT->Draw(Form("%s>>%s",recoPlotVar.c_str(),histRefNameAndBins.c_str()),Form("(%f)*(%s)",weight,allCutsCopy.GetTitle()),"goff",nEntries);

  TH1D* histRef = static_cast<TH1D*>( gDirectory->Get(histRefName.c_str()) );
  if(!histRef){
    cout << "ERROR! No histogram with name " << histRefName.c_str() << endl;
    return;
  }

  residualTruePlotVar = truePlotVarIsAResidual;
  const Double_t highRef = histRef->GetXaxis()->GetBinUpEdge(histRef->GetNbinsX());
  const Double_t lowRef = histRef->GetBinLowEdge(1);
  const Double_t uniformBinWidth = (highRef-lowRef)/histRef->GetNbinsX();
  //a sansity check that steps in the finest binning is not too infinitessimal
  const Double_t limitStepSize = 0.1*uniformBinWidth;

  const Double_t statsLimit = 150;
  const Int_t nBinsFine = 5000;
  TH1D* histFineBinning = new TH1D("histFineBinning","",nBinsFine,lowRef,highRef);

  FT->Draw(Form("%s>>histFineBinning",recoPlotVar.c_str()),Form("(%f)*(%s)",weight,allCutsCopy.GetTitle()),"goff",nEntries);

  std::vector<OneDimParameters> allParams;
  Int_t UpEdge = 1;
  Int_t LowEdge = 0;
  Double_t alpha = 0;
  Double_t sigma = 0;
  Double_t errorStat = -1;
  Double_t errorSystematic = -2;

  const Int_t nShifts = 1;
  const Edge middleEdge = Edge(histRef,histFineBinning,LowEdge);
  //use slightly wider reco bins to get accurate residual fit
  Edge tempLowEdge(histRef,histFineBinning,LowEdge);
  Edge tempUpEdge (histRef,histFineBinning,UpEdge);
  do{
    if(!tempLowEdge.MoveLow()){
      break;
    }
  }while(histFineBinning->Integral(tempLowEdge.bin,middleEdge.bin) <= statsLimit);

  do{
    if(!tempUpEdge.MoveUp()){
      break;
    }
  }while(histFineBinning->Integral(middleEdge.bin,tempUpEdge.bin) <= statsLimit);

  sigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),tempLowEdge.edge,tempUpEdge.edge,allCutsCopy,nEntries);

  Edge lowEdge(histRef,histFineBinning,LowEdge);
  Edge upEdge(histRef,histFineBinning,UpEdge);
  CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);

  cout << lowEdge.bin << endl;
  cout << upEdge.bin << endl;

  Edge previousLowEdge = lowEdge;
  Edge previousUpEdge = upEdge;
  //now that we have a sigma, need to find how many events fall outside reco range for max bin plus extra needed width
  do {
    CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
    //cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << endl;
    //cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << endl;
    //if(errorStat >= errorSystematic && pow(errorStat,-2.)/POTweight >= 500){
    if(errorStat >= errorSystematic && errorStat <= 0.5){
      break;
    }
    Edge previousLowEdge = lowEdge;
    //do{
      lowEdge.MoveLow();
      if(lowEdge.bin == lowEdge.firstNonZeroBin)
        break;
    //}while(histFineBinning->Integral(lowEdge.bin,previousLowEdge.bin) <= 1);
    //cout << lowEdge.bin << endl;
    //cout << upEdge.bin << endl;
    CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
    //cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << endl;
    //cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << endl;
    //if(errorStat >= errorSystematic && pow(errorStat,-2.)/POTweight >= 500){
    if(errorStat >= errorSystematic && errorStat <= 0.5){
      break;
    }
    Edge previousUpEdge = upEdge;
    //do{
      upEdge.MoveUp();
      if(upEdge.bin == upEdge.lastNonZeroBin)
        break;
    //}while(histFineBinning->Integral(previousUpEdge.bin,upEdge.bin) <= 1);

    //cout << lowEdge.bin << endl;
    //cout << upEdge.bin << endl;
    //cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << endl;
    //cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << endl;
  }while(1);

  //remember the edges we already compared
  Edge peakBinLowEdge = lowEdge;
  Edge peakBinUpEdge = upEdge;
  Double_t nMCStats = histFineBinning->Integral(peakBinLowEdge.bin,peakBinUpEdge.bin);
  Double_t nMCStatsMin = nMCStats; //histFineBinning->Integral(peakBinLowEdge.bin,peakBinUpEdge.bin);
  Double_t previousBinMCStats = nMCStats;
  cout << "nMCStatsMin = " << nMCStatsMin << endl;

  OneDimParameters peakBin;
  peakBin.low = lowEdge.edge;
  peakBin.up  = upEdge.edge;
  peakBin.sigma = sigma;
  peakBin.trueSigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),lowEdge.edge,upEdge.edge,allCutsCopy,nEntries);
  peakBin.nEntriesMC = nMCStats;
  peakBin.nEntriesData = nMCStats*POTweight;
  peakBin.sysFracErr = errorSystematic;
  peakBin.statFracErr = errorStat;

  allParams.push_back(peakBin);
  cout << "saved a parameter for bins " << lowEdge.bin << " to " << upEdge.bin  << endl;
  cout << "                      edge " << lowEdge.edge << " to " << upEdge.edge  << endl;
  cout << "N = " << peakBin.nEntriesData << " and errorStat = " << errorStat << endl;
  cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << endl;

  cout << "process higher bins" << endl;
  lowEdge.ShiftToBin(peakBinUpEdge.bin+1);
  //upEdge.ShiftToBin(upEdge.lastNonZeroBin);

  //now process higher bins
  while(lowEdge.bin < lowEdge.lastNonZeroBin){
    //nMCStatsMin = max(nMCStatsMin,1.1*previousBinMCStats);
    BinPair lowHighBins = GetLowHighBinsFromLowEnd(histFineBinning,lowEdge.edge,nMCStatsMin);
    upEdge.ShiftToBin(lowHighBins.second);

    //cout << lowEdge.bin << endl;
    //cout << upEdge.bin << endl;
    cout << "Number of MC entries is " << histFineBinning->Integral(lowEdge.bin,upEdge.bin) << endl;

    //if we cannot satisfy the stats condition with the widest bin possible
    //if(histFineBinning->Integral(lowEdge.bin,upEdge.bin) <= 0.25*previousBinMCStats && abs(upEdge.bin-upEdge.lastNonZeroBin)<=1){
    if(histFineBinning->Integral(lowEdge.bin,upEdge.bin) <= 0.25*nMCStatsMin && abs(upEdge.bin-upEdge.lastNonZeroBin)<=1){
      cout << "Merging previous high bin with current range" << endl;
      cout << lowEdge.bin << endl;
      cout << upEdge.bin << endl;
      lowEdge = previousLowEdge;
      upEdge.ShiftToBin(upEdge.lastNonZeroBin);

      sigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),lowEdge.edge,upEdge.edge,allCutsCopy,nEntries);
      CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
      Double_t nStatsMC = histFineBinning->Integral(lowEdge.bin,upEdge.bin);
      previousBinMCStats = nStatsMC;
      OneDimParameters params;
      params.low = lowEdge.edge;
      params.up  = upEdge.edge;
      params.sigma = sigma;
      params.trueSigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),lowEdge.edge,upEdge.edge,allCutsCopy,nEntries);
      params.nEntriesMC = nStatsMC;
      peakBin.nEntriesData = nStatsMC*POTweight;
      params.sysFracErr = errorSystematic;
      params.statFracErr = errorStat;
      if(fabs(lowEdge.edge-allParams.back().low) <= 0.01 && (upEdge.edge-lowEdge.edge) > (allParams.back().up-allParams.back().low)){
        allParams.pop_back();
        cout << "Previous entry removed" << endl;
      }
      allParams.push_back(params);

      cout << "saved a parameter for bins " << lowEdge.bin << " to " << upEdge.bin  << endl;
      cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << endl;
      cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << endl;
      //now that we did this, we should shift the bins to be at the far end
      lowEdge.ShiftToBin(lowEdge.lastNonZeroBin);
      upEdge.ShiftToBin(upEdge.lastNonZeroBin);
      break;
    }

    //cout << lowEdge.bin << endl;
    //cout << upEdge.bin << endl;
    sigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),lowEdge.edge,upEdge.edge,allCutsCopy,nEntries);


    //if(histFineBinning->FindBin(upEdge.edge*0.9) >= lowEdge.bin){
    //  upEdge.ShiftToBin(histFineBinning->FindBin(upEdge.edge*0.9));
    //}
    upEdge.ShiftToBin(lowEdge.bin);
    CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
    //cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << endl;
    while(errorStat > 0.5 || errorStat == TMath::Infinity() || pow(errorStat,-2.) <= 0 || TMath::IsNaN(errorStat)){
      upEdge.MoveUp();
      CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
      //cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << endl;
    }

    CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
    Int_t nIterations = 0;
    while(errorStat <= errorSystematic){
      for(Int_t i = 0;i<nShifts;i++){
        upEdge.MoveUp();
      }
      if(upEdge.bin >= upEdge.lastNonZeroBin){
        upEdge.ShiftToBin(upEdge.lastNonZeroBin);
        sigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),lowEdge.edge,upEdge.edge,allCutsCopy,nEntries);
        CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
        break;
      }

      //cout << lowEdge.bin << endl;
      //cout << upEdge.bin << endl;
      CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
      //cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << endl;
      //cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << endl;
      nIterations++;

    }

    if(nIterations == 0){
      cout << "first attempt to find appropriate binning by increasing the size failed. Attemping to move downwards" << endl;

      cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << endl;
      cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << endl;
      //upEdge.ShiftToBin(lowHighBins.second);
      upEdge.ShiftToBin(GetLowHighBinsFromLowEnd(histFineBinning,lowEdge.edge,3.*nMCStatsMin).second);
      while(errorSystematic <= errorStat){
      //while(errorSystematic <= errorStat){
        for(Int_t i = 0;i<nShifts;i++){
          upEdge.MoveLow();
        }

        //cout << lowEdge.bin << endl;
        //cout << upEdge.bin << endl;
        CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
        cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << endl;
        cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << endl;
        nIterations++;
      }
    }
    Double_t nStatsMC = histFineBinning->Integral(lowEdge.bin,upEdge.bin);
    OneDimParameters params;
    params.low = lowEdge.edge;
    params.up  = upEdge.edge;
    params.sigma = sigma;
    params.trueSigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),lowEdge.edge,upEdge.edge,allCutsCopy,nEntries);
    params.nEntriesMC = nStatsMC;
    params.nEntriesData = nStatsMC*POTweight;
    params.sysFracErr = errorSystematic;
    params.statFracErr = errorStat;
    allParams.push_back(params);
    //nMCStatsMin = nStatsMC;
    previousBinMCStats = nStatsMC;

    cout << "saved a parameter for bins " << lowEdge.bin << " to " << upEdge.bin  << endl;
    cout << "                      edge " << lowEdge.edge << " to " << upEdge.edge  << endl;
    cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << endl;
    cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << endl;

    previousUpEdge = upEdge;
    previousLowEdge = lowEdge;

    lowEdge.ShiftToBin(upEdge.bin+1);
    upEdge.ShiftToBin(upEdge.lastNonZeroBin);
    cout << lowEdge.bin << endl;
    cout << upEdge.bin << endl;

  }

  cout << lowEdge.bin << endl;
  cout << upEdge.bin << endl;

  cout << "done with upper bins" << endl;

  //sort(allParams.begin(),allParams.end(),SortParametersByEdges);
  //for(UInt_t i = 0;i<allParams.size();i++){
  //  cout << "low Edge = " << allParams[i].low << endl;
  //  cout << "N = " << allParams[i].nEntriesData << endl;
  //  cout << "errorStat = " << allParams[i].statFracErr << endl;
  //  cout << "errorSystematic = " << allParams[i].sysFracErr << endl;
  //  cout << "si = " << allParams[i].sigma << endl;
  //  cout << "up Edge = " << allParams[i].up << endl;
  //}

  upEdge.ShiftToBin(peakBinLowEdge.bin-1);
  lowEdge.ShiftToBin(lowEdge.firstNonZeroBin);

  previousUpEdge = upEdge;
  previousLowEdge = lowEdge;

  cout << lowEdge.bin << endl;
  cout << upEdge.bin << endl;

  //work on lower bins
  cout << "lower bins" << endl;
  while(upEdge.bin > upEdge.firstNonZeroBin){

    //nMCStatsMin = max(nMCStatsMin,1.1*previousBinMCStats);
    BinPair lowHighBins = GetLowHighBinsFromHighEnd(histFineBinning,upEdge.edge,nMCStatsMin);
    lowEdge.ShiftToBin(lowHighBins.first);
    //if we cannot satisfy the stats condition with the widest bin possible
    if(histFineBinning->Integral(lowEdge.bin,upEdge.bin) <= 0.25*nMCStatsMin && abs(lowEdge.bin-lowEdge.firstNonZeroBin)<=1){

      lowEdge.ShiftToBin(lowEdge.firstNonZeroBin);
      upEdge = previousUpEdge;
      sigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),lowEdge.edge,upEdge.edge,allCutsCopy,nEntries);
      CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
      Double_t nStatsMC = histFineBinning->Integral(lowEdge.bin,upEdge.bin);
      previousBinMCStats = nStatsMC;
      OneDimParameters params;
      params.low = lowEdge.edge;
      params.up  = upEdge.edge;
      params.sigma = sigma;
      params.trueSigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),lowEdge.edge,upEdge.edge,allCutsCopy,nEntries);
      params.nEntriesMC = nStatsMC;
      params.nEntriesData = nStatsMC*POTweight;
      params.sysFracErr = errorSystematic;
      params.statFracErr = errorStat;
      if(fabs(upEdge.edge-allParams.back().up) <= 0.01 && (upEdge.edge-lowEdge.edge) > (allParams.back().up-allParams.back().low)){
        allParams.pop_back();
        cout << "Previous entry removed" << endl;
      }
      allParams.push_back(params);

      cout << "saved a parameter in special check for bins " << lowEdge.bin << " to " << upEdge.bin  << endl;
      cout << "                                       edges" << lowEdge.edge << " to " << upEdge.edge  << endl;
      cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << endl;
      cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << endl;
      lowEdge.ShiftToBin(lowEdge.firstNonZeroBin);
      upEdge.ShiftToBin(upEdge.firstNonZeroBin);
      break;
    }

    //cout << lowEdge.bin << endl;
    //cout << upEdge.bin << endl;
    sigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),lowEdge.edge,upEdge.edge,allCutsCopy,nEntries);

    lowEdge.ShiftToBin(upEdge.bin);
    CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
    //cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << endl;
    while(errorStat > 0.5 || errorStat == TMath::Infinity() || pow(errorStat,-2.) <= 0 || TMath::IsNaN(errorStat)){
    //while(errorStat > 0.5 || errorStat == TMath::Infinity()){
      lowEdge.MoveLow();
      CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
      //cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << endl;
    }

    //cout << "lowEdge bin = " << lowEdge.bin << endl;
    //cout << "upEdge bin = " << upEdge.bin << endl;
    CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
    cout << "upEdge bin = " << upEdge.bin << endl;

    Int_t nIterations = 0;
    while(errorStat <= errorSystematic){
      for(Int_t i = 0;i<nShifts;i++){
        lowEdge.MoveLow();
      }
      if(lowEdge.bin <= lowEdge.firstNonZeroBin){
        lowEdge.ShiftToBin(lowEdge.firstNonZeroBin);
        sigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),lowEdge.edge,upEdge.edge,allCutsCopy,nEntries);
        CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
        break;
      }
      cout << "lowEdge bin = " << lowEdge.bin << endl;
      CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
      cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << endl;
      cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << endl;
      nIterations++;

    }
    if(nIterations == 0){
      cout << "first attempt to find appropriate binning by increasing the size failed. Attemping to move downwards" << endl;
      lowEdge.ShiftToBin(lowHighBins.first);
      while(errorSystematic <= errorStat){
        for(Int_t i = 0;i<nShifts;i++){
          lowEdge.MoveUp();
        }

        //cout << lowEdge.bin << endl;
        //cout << upEdge.bin << endl;
        CalculateErrors(histFineBinning,lowEdge,upEdge,sigma,errorStat,errorSystematic,alpha);
        //cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << endl;
        //cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << endl;
        nIterations++;
      }
    }
    Double_t nStatsMC = histFineBinning->Integral(lowEdge.bin,upEdge.bin);
    OneDimParameters params;
    params.low = lowEdge.edge;
    params.up  = upEdge.edge;
    params.sigma = sigma;
    params.trueSigma = GetSigma(truePlotVar.c_str(),recoPlotVar.c_str(),lowEdge.edge,upEdge.edge,allCutsCopy,nEntries);
    params.nEntriesMC = nStatsMC;
    params.nEntriesData = nStatsMC*POTweight;
    params.sysFracErr = errorSystematic;
    params.statFracErr = errorStat;
    allParams.push_back(params);
    //nMCStatsMin = nStatsMC;
    previousBinMCStats = nStatsMC;

    cout << "saved a parameter for bins " << lowEdge.bin << " to " << upEdge.bin  << endl;
    cout << "                      edges" << lowEdge.edge << " to " << upEdge.edge  << endl;
    cout << "N = " << pow(errorStat,-2.) << " and errorStat = " << errorStat << endl;
    cout << "alpha = " << alpha << " and errorSystematic = " << errorSystematic << endl;

    previousUpEdge = upEdge;
    previousLowEdge = lowEdge;

    upEdge.ShiftToBin(lowEdge.bin-1);
    lowEdge.ShiftToBin(lowEdge.firstNonZeroBin);
  }
  cout << "done with lower bins" << endl;

  cout << "number of parameter sets saved = " << allParams.size() << endl;
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
    cout << "low Edge = " << allParams[i].low << endl;
    //cout << "errorStat = " << pow(allParams[i].nEntriesData,-0.5) << endl;
    cout << "errorStat = " << allParams[i].statFracErr << endl;
    cout << "errorSystematic = " << allParams[i].sysFracErr << endl;
    cout << "si = " << allParams[i].sigma << endl;
    cout << "up Edge = " << allParams[i].up << endl;
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

  mg          = new TMultiGraph();
  gStatErrors = new TGraphErrors(nPoints,graphMiddleX_augmented,graphY_augmented,graphErrX_augmented,graphErr1Y_augmented);
  gSysErrors  = new TGraphErrors(nPoints,graphMiddleX_augmented,graphY_augmented,graphErrX_augmented,graphErr2Y_augmented);
  TH1D* hNoErrors = new TH1D("hNoErrors","",nBins_augmented,binEdgesX_augmented);
  for(UInt_t i = 0;i<allParams.size();i++){
    //const Double_t N = pow(allParams[i].statFracErr,-2.);
    const Double_t N = allParams[i].statFracErr == 0 ? 0 : pow(allParams[i].statFracErr,-2.);
    hNoErrors->SetBinContent(hNoErrors->FindBin(allParams[i].low),N);
    hNoErrors->SetBinError(hNoErrors->FindBin(allParams[i].low),0);

  }

  gStatErrors->SetFillColor(kBlue);
  gSysErrors->SetFillColor(kRed);
  hNoErrors->SetLineWidth(3);

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

  TLegend* gLegend = new TLegend(0.7,0.7,0.88,0.88,"");
  gLegend->AddEntry(gStatErrors,"Stat. Error","F");
  gLegend->AddEntry(gSysErrors,"Total Error","F");

  gLegend->SetLineStyle(0);
  gLegend->SetLineColor(0);
  gLegend->SetFillStyle(0);
  gLegend->Draw();
  gCanvas->SaveAs(Form("plots/%serrors.root",outputName.c_str()));
  gCanvas->SaveAs(Form("plots/%serrors.png",outputName.c_str()));
  gCanvas->SaveAs(Form("plots/%serrors.eps",outputName.c_str()));
  gCanvas->SaveAs(Form("plots/%serrors.pdf",outputName.c_str()));

  TCanvas* hCanvas = new TCanvas("hCanvas","",800,800);
  hCanvas->cd();

  TH1D* hSigmas = new TH1D("hSigmas","",nBins_augmented,binEdgesX_augmented);
  TH1D* htrueSigmas = new TH1D("htrueSigmas","",nBins_augmented,binEdgesX_augmented);
  htrueSigmas->SetLineColor(kBlue);
  TH1D* hSigmaOverBinCenter = new TH1D("hSigmaOverBinCenter","",nBins_augmented,binEdgesX_augmented);
  TH1D* htrueSigmaSigmaRatio = new TH1D("htrueSigmaSigmaRatio","",nBins_augmented,binEdgesX_augmented);
  hSigmas->GetXaxis()->SetTitle(xAxisTitle.c_str());
  hSigmas->GetYaxis()->SetTitle("1 #sigma range");
  hSigmas->GetYaxis()->SetTitleOffset(hSigmas->GetYaxis()->GetTitleOffset()*1.25);
  hSigmas->SetLineWidth(3);
  hSigmaOverBinCenter->SetLineColor(kRed);
  hSigmaOverBinCenter->SetLineWidth(3);
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

  TLegend* hLegend = new TLegend(0.7,0.7,0.88,0.88,"");
  hLegend->AddEntry(hSigmas,"Initial Estimate","L");
  hLegend->AddEntry(htrueSigmas,"After Bin Formed","L");
  hLegend->SetLineStyle(0);
  hLegend->SetLineColor(0);
  hLegend->SetFillStyle(0);
  hLegend->Draw();


  hCanvas->Update();
  Double_t rightmax = 1.1*htrueSigmaSigmaRatio->GetMaximum();
  Double_t scale = gPad->GetUymax()/rightmax;
  htrueSigmaSigmaRatio->Scale(scale);
  htrueSigmaSigmaRatio->Draw("same");

  TGaxis* axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),
     gPad->GetUxmax(), gPad->GetUymax(),0,rightmax,510,"+L");
  axis->SetLineColor(kRed);
  axis->SetLabelColor(kRed);
  axis->SetTitleColor(kRed);
  //axis->SetTitleSize(axis->GetTitleSize()*0.5);
  //axis->SetLabelSize(axis->GetTitleSize());
  axis->SetTitle("Ratio");
  axis->SetTitleOffset(axis->GetTitleOffset()*1.5);
  axis->Draw();

  hCanvas->SaveAs(Form("plots/%ssigmas.root",outputName.c_str()));
  hCanvas->SaveAs(Form("plots/%ssigmas.png",outputName.c_str()));
  hCanvas->SaveAs(Form("plots/%ssigmas.eps",outputName.c_str()));
  hCanvas->SaveAs(Form("plots/%ssigmas.pdf",outputName.c_str()));

  delete histFineBinning;

  delete axis;
  delete hSigmas;
  delete hSigmaOverBinCenter;
  delete htrueSigmas;
  delete htrueSigmaSigmaRatio;
  delete hCanvas;

  delete gStatErrors;
  delete gSysErrors ;
  delete gLegend;
  delete mg         ;
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
        Int_t limitEntries)
{
    Double_t fracChange = 0.;
    Double_t lowCut = recoLowValue;
    Double_t highCut = recoHighValue;
    Double_t sigmas[2] = {2, 1.};
    TH1D* htemp = 0;//static_cast<TH1D*>(gDirectory->Get("htemp"));
    TCanvas* temp = new TCanvas("temp","",800,800);
    temp->cd();
    TF1* normFit = new TF1("normFit","gaus(0)",0.1*recoLowValue,2*recoHighValue);
    do
    {
        TCut rangeCut = Form("%f<=%s&&%s<%f",lowCut,recoVar,recoVar,highCut);
        string DrawCommand = "";//Form("%s>>htemp(32,%f,%f)",trueVar,0.1*lowCut,2*highCut);
        if(residualTruePlotVar)
        {
            Double_t avg = 0.5*(lowCut+highCut);
            DrawCommand = Form("%s>>htemp(32,%f,%f)",trueVar,-1.25*avg,1.25*avg);
        }
        else
            DrawCommand = Form("%s>>htemp(32,%f,%f)",trueVar,0.1*lowCut,2*highCut);

        cout << rangeCut.GetTitle() << endl;
        FT->Draw(DrawCommand.c_str(),cuts&&rangeCut,"goff",limitEntries);
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
        //cout << sigmas[0] << endl;
        //cout << sigmas[1] << endl;
        if(sigmas[1] > 0 && sigmas[0] > 0)
        {
            sigmas[0] = sigmas[1];
            sigmas[1] = normFit->GetParameter(2);
            if(sigmas[1] > sigmas[0] && fabs(sigmas[1]/sigmas[0]-1.)<MinFracDiff)
                break;

            if(sigmas[1] < sigmas[0] && fabs(sigmas[0]/sigmas[1]-1.)<MinFracDiff)
                break;

        }
        fracChange += 0.0005;
        lowCut    *= (1 - fracChange);
        highCut *= (1 + fracChange);
    //}while(fabs(sigmas[1]/sigmas[0]-1.)>MinFracDiff || TMath::IsNaN(sigmas[1]/sigmas[0]) || sigmas[1]/sigmas[0] == TMath::Infinity() || sigmas[1] == 0);
    } while(1);

    htemp->Draw();
    //temp->SaveAs(Form("plots/range_%6f_%6f.root",recoLowValue,recoHighValue));
    //temp->SaveAs(Form("plots/range_%6f_%6f.png",recoLowValue,recoHighValue));
    //temp->SaveAs(Form("plots/range_%6f_%6f.pdf",recoLowValue,recoHighValue));
    //temp->SaveAs(Form("plots/range_%6f_%6f.eps",recoLowValue,recoHighValue));
    delete normFit;
    delete temp;

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

Edge::Edge(TH1D* hReference,TH1D* hEqualStatistics,bool UpEdge) :
        hRef(NULL), hFineBinning(NULL), isUpEdge(false)
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
        return false;
    }
    return true;
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
        return false;
    }
    return true;
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
