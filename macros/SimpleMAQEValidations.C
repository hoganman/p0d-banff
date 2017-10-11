//macro to plot T2KReWeight BANFF spline files
//supported SampleId from psyche (see source file)


#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <cstring>

using namespace std;

#include"TString.h"
#include"TFile.h"
#include"TTree.h"
#include"TChain.h"
#include"TH1D.h"
#include"TClonesArray.h"
#include"TGraph.h"
#include"TCanvas.h"
#include"TLegend.h"

#include "SampleId.cxx"

#include"../include/SetStyles.h"
#include"../include/MakeClassSampleSummary.C"
#include"../include/MakeClassHeader.C"
#include"../include/MakeClassHL2FlatTree.C"
#include"CheckFileROOT.C"


class parameterVariation {
  /*
   * For a given parameter, make a histogram for the parameter variations
   */

  public:
    parameterVariation(TString parameterName = "MAQE",TString variationString = "0%",Double_t variationValue = 0)
      : parameter(parameterName), variation(variationString), variationVal(variationValue)
    {
      TString name = Form("%s_var_%s",parameter.Data(),variation.Data());
      hist = new TH1D(name.Data(),"",200,-1,1); 
      hist->GetYaxis()->SetTitleOffset(1.2);
    }
    virtual ~parameterVariation(){delete hist;}

    void SetBins(Int_t nBins,Double_t low,Double_t high){ hist->SetBins(nBins,low,high); }
    //TH1D* GetClone(TString newName){return static_cast<TH1D*>(hist->Clone(newName.Data()));}
    
    TH1D* GetHistogram(){
      return hist;
    }
    void Fill(Double_t var,Double_t weight=1){hist->Fill(var,weight);}
    const char* GetVariationS() const {return variation.Data();}
    Double_t GetVariationV() const {return variationVal;}

  protected:
    TH1D* hist;
    TString parameter;
    TString variation;
    Double_t variationVal;

};

class allParameterVariations {
  /*
   * For a kinetic variable, define parameter variations and TH1 functions to change settings for all of them
   */
  public:
    virtual ~allParameterVariations(){
      delete legend; 
      for(UInt_t index = 0;index < params.size();index++) 
	delete params.at(index);
    }

    allParameterVariations(TString parameterName = "MAQE",TString xAxisTitle = "E_{#nu} (GeV)", 
      TString yAxisTitle = "Events/Bin",TString legendTitle = "") : 
	parameter(parameterName), xAxis(xAxisTitle), yAxis(yAxisTitle) 
    {
      legend = new TLegend(0.6,0.6,0.88,0.88,legendTitle.Data());
      legend->SetFillColor(0);
      legend->SetLineColor(0);
    }
    void SetLineStyles(Int_t* lineStyles){ histOperation(kLineStyles,lineStyles); }
    void SetLineColors(Int_t* lineColors){ histOperation(kLineColors,lineColors); }
    void SetLineWidths(Int_t* lineWidths){ histOperation(kLineWidths,lineWidths); }
    void DrawAll() { histOperation(kDraw); }
    void SetMaximum(Double_t max) { 
      for(UInt_t index = 0;index < params.size();index++) {
	TH1D* hist = params.at(index)->GetHistogram(); 
	hist->SetMaximum(max); 
      }
    }
    void SetMinimum(Double_t max) { 
      for(UInt_t index = 0;index < params.size();index++) {
	TH1D* hist = params.at(index)->GetHistogram(); 
	hist->SetMinimum(max); 
      }
    }

    void Scale(Double_t scale) {
      for(UInt_t index = 0;index < params.size();index++){
	params.at(index)->GetHistogram()->Scale(scale); 
      }
    }

    Double_t GetMaximum() const {
      double max = 0; 
      for(UInt_t index = 0;index < params.size();index++){
	Double_t temp = params.at(index)->GetHistogram()->GetMaximum(); 
	max = (temp > max) ? temp : max;
      } 
      return max;
    }

    Double_t GetMinimum() const {
      double min = 0; 
      for(UInt_t index = 0;index < params.size();index++){
	Double_t temp = params.at(index)->GetHistogram()->GetMinimum(); 
	min = (temp < min) ? temp : min;
      } 
      return min;
    }

    void SetBins(Int_t nBins,Double_t low,Double_t high){
      for(UInt_t index = 0;index < params.size();index++) 
	params.at(index)->SetBins(nBins,low,high);
    }

    void AddVariation(TString variationString = "0%",Double_t variationValue = -9999){
      parameterVariation* var = new parameterVariation(
	  parameter,variationString,(variationValue == -9999) ? variationString.Atof() : variationValue); 
      var->GetHistogram()->GetXaxis()->SetTitle(xAxis.Data());
      var->GetHistogram()->GetYaxis()->SetTitle(yAxis.Data());
      legend->AddEntry(var->GetHistogram(),var->GetHistogram()->GetName(),"l");
      params.push_back(var);
    }

    void Fill(UInt_t index,Double_t value,Double_t weight=1){
      params.at(index)->Fill(value,weight);
    }

    TH1D* GetHistogram(const UInt_t& index){
      return params.at(index)->GetHistogram();
    }

  protected:
    std::vector<parameterVariation*> params;
    void histOperation(Int_t option,Int_t* optionVect = 0){
      for(UInt_t index = 0;index < params.size();index++){
	if(option == kLineStyles) {
	  params.at(index)->GetHistogram()->SetLineStyle(optionVect[index]);	
	}
	if(option == kLineColors) {
	  params.at(index)->GetHistogram()->SetLineColor(optionVect[index]);	
	}
	if(option == kDraw){
	  TString drawOption = (index==0) ? "" : "SAME";
	  params.at(index)->GetHistogram()->DrawCopy(drawOption.Data());	
	}
	if(option == kLineWidths){
	  params.at(index)->GetHistogram()->SetLineWidth(optionVect[index]);	
	}
      } 
      if(option == kDraw){
	TLegend* legend_clone = static_cast<TLegend*>(legend->Clone("legend_clone"));
	legend_clone->Draw();
      }
    }

    TString parameter;
    TString xAxis;
    TString yAxis;
    const static Int_t kLineStyles = 0;
    const static Int_t kLineColors = 1;
    const static Int_t kDraw       = 2;
    const static Int_t kLineWidths = 3;
    TLegend* legend;

};


void SimpleMAQEValidations(const TString& inputFileName, const TString& selection = "kTrackerNumuCCMultiPi", Long64_t limitEntries = -1){
  /*
   * Make the variation
   *
   */


  std::cout << "The selection for this macro is " << selection << std::endl;
  //std::cout << "The sample for this macro is " << SampleId::ConvertSample(sampleNumber).c_str() << std::endl;
  SetStyles();

  Long64_t nEntries = -1;
  const UInt_t nVariations = 7;
  const Double_t variationValues[nVariations] = {0.75,0.5,0.25,0,-0.25,-0.5,-0.75};
  const UInt_t nominal = 3;
  const Int_t NEUTCCQE = 1;
  Int_t lineColors[nVariations] = {cbBlack,cbOrange,cbSky,cbGreen,cbBlue,cbRed,cbPurple};
  Int_t lineWidths[nVariations]= {3,3,3,3,3,3,3};
  Int_t lineStyles[nVariations]= {1,1,1,1,2,2,2};

  std::vector<Int_t> SampleNumbers;
  TString legendHeader = "True CCQE";

  if(!selection.Contains("k")){
    SampleId::SampleEnum selectionEnum = (SampleId::SampleEnum)selection.Atoi();
    if(selectionEnum == 0){
      cout << "invalid sample option" << selection.Data() << endl;
      exit(1);
    }
    SampleNumbers.push_back((Int_t)selectionEnum); 
    //legendHeader = SampleId::ConvertSample(selectionEnum);
  }
  else{
    //legendHeader = selection;
    for(Int_t sampleID = 0;sampleID < SampleId::kNSamples;++sampleID){
      TString testSelection = SampleId::ConvertSampleToSelection((SampleId::SampleEnum)sampleID); 
      if(testSelection.EqualTo(selection)){
	SampleNumbers.push_back(sampleID); 
      }
    }
    if(SampleNumbers.size() == 0){
      cout << "Invalid selection: " <<  selection.Data() << endl;
      exit(1);
    }
  }

  TCanvas* const canvas = new TCanvas("canvas","",800,800);
  TFile* const inputFile = TFile::Open(inputFileName.Data());
  TChain* const sample_sum = new TChain("sample_sum");
  TChain* const header = new TChain("header");
  sample_sum->Add(inputFileName.Data()); 
  header->Add(inputFileName.Data());

  MakeClassHeader* mcHeader = new MakeClassHeader(header);
  MakeClassSampleSummary* mcSample_Sum = new MakeClassSampleSummary(sample_sum);

  mcHeader->GetEntry(0);
  mcSample_Sum->GetEntry(0);

  Double_t goodPOT = 0;
  if(limitEntries <= 0){
    nEntries = sample_sum->GetEntries(); 
  }
  else{
    nEntries = std::min(sample_sum->GetEntries(),limitEntries);
  }

  for(Long64_t entry = 0;entry<header->GetEntries();entry++){
    mcHeader->GetEntry(entry);
    goodPOT += mcHeader->POTInfo->_POT_GoodBeamGoodND280;
  }

  const Double_t POTused = (goodPOT / sample_sum->GetEntries()) * nEntries;
  const Double_t eventsPerPOTscale = (POTused == 0) ? 1 : 1.6e20 / POTused;
  std::cout << "events per POT scale = " << eventsPerPOTscale << std::endl;

  const Double_t nBins_Enu = 30;
  const Double_t Enu_max = 3; // GeV
  const Double_t Enu_min = 0;
  const string Enu_binWidth = "100 MeV";

  allParameterVariations* MAQE_Enu_FractionalChange = new allParameterVariations("MAQE","E_{#nu} (GeV)",Form("Fractional Change/%s/1.6x10^{20} POT",Enu_binWidth.c_str()),legendHeader.Data());
  //allParameterVariations* MAQE_Enu_FractionalChange = new allParameterVariations("MAQE","E_{#nu} (GeV)",Form("Events/%s/1.6x10^{20} POT",Enu_binWidth.c_str()),legendHeader.Data());

  for(UInt_t variationIndex = 0;variationIndex < nVariations;variationIndex++){
     MAQE_Enu_FractionalChange->AddVariation(Form("%3.2f",variationValues[variationIndex]));
  }

  MAQE_Enu_FractionalChange->SetBins(nBins_Enu,Enu_min,Enu_max);
  MAQE_Enu_FractionalChange->SetLineColors(lineColors);
  MAQE_Enu_FractionalChange->SetLineWidths(lineWidths);
  MAQE_Enu_FractionalChange->SetLineStyles(lineStyles);

  for(Long64_t entry = 0;entry < nEntries;entry++){
    sample_sum->GetEntry(entry);
    //print only every 1000 entries
    if ((entry % 1000) == 0) std::cout << "Entry = " << entry*1e-3 << "k" << std::endl;
    for(UInt_t variationIndex = 0;variationIndex < nVariations;variationIndex++){
      for(std::vector<Int_t>::const_iterator it = SampleNumbers.begin(); it!=SampleNumbers.end();++it){
	const Int_t sampleNumber = *it;
	if(mcSample_Sum->ReactionCode[sampleNumber] != NEUTCCQE){
	  continue;
	}
	TGraph* graph = static_cast<TGraph*>(mcSample_Sum->MAQEGraph->At(sampleNumber));
	const Double_t tempWeight = graph->Eval(variationValues[variationIndex],0,"S");
	const Double_t tempEnu = mcSample_Sum->Enu[sampleNumber];
	MAQE_Enu_FractionalChange->Fill(variationIndex,tempEnu,tempWeight);
      }
    }

  }


  canvas->cd();

  MAQE_Enu_FractionalChange->Scale(eventsPerPOTscale);
  
  TH1D* base = MAQE_Enu_FractionalChange->GetHistogram(nominal);
  for(UInt_t ind = 0;ind<nVariations;++ind){
    cout << ind << endl;
    if(ind == nominal)
      continue;
    TH1D* currentHist = MAQE_Enu_FractionalChange->GetHistogram(ind);
    currentHist->Divide(base);
  } 
  base->Divide(base);
  
  MAQE_Enu_FractionalChange->SetMaximum(MAQE_Enu_FractionalChange->GetMaximum()*1.1);
  MAQE_Enu_FractionalChange->SetMinimum(MAQE_Enu_FractionalChange->GetMinimum()*0.9);

  MAQE_Enu_FractionalChange->DrawAll();

  inputFile->Close();
  return;

}

