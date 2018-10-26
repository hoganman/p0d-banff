#include <iostream>
#include <fstream>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TMatrixTSym.h"
#include "TVectorT.h"
#include "TLine.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TStyle.h"
#include "TColor.h"
#include "TFitResultPtr.h"
#include "TDirectory.h"

#include "BinningDefinition.hxx"
#include "ND280AnalysisUtils.hxx"
#include "Parameters.hxx"

bool ApplyNIWG = false;
bool Apply1p1h = false;
bool IgnoreWeightSyst = false;
const int nThrows = 1000;
const int nWeights = 12;

void MakeCovariance(std::string MCFilesListStr, std::string NIWGFilesListStr,
		    std::string Cov1P1HStr,     std::string OutFileStr){

  ND::params().SetReadParamOverrideFilePointPassed();
  BANFF::BinningDefinition& bd = BANFF::BinningDefinition::Get();
  int nBinsRedu = bd.GetNbins_Det();
  int nBinsFull = bd.GetNbins();

  std::map<int,int> FullToReduMap; // Call FullToReduMap[Full] -> Redu 
  for(int j = 0; j < nBinsFull; ++j){
    int binmom;
    int bincos;
    SampleId::SampleEnum sample;
    bd.GetLocalBinMatrixMomCos(j, sample, binmom, bincos);
    double ph = bd.GetBinning_Mom(sample)->GetBinUpEdge (binmom+1);
    double pl = bd.GetBinning_Mom(sample)->GetBinLowEdge(binmom+1);
    double ch = bd.GetBinning_Cos(sample)->GetBinUpEdge (bincos+1);
    double cl = bd.GetBinning_Cos(sample)->GetBinLowEdge(bincos+1);

    if(j != bd.GetGlobalBinMatrixMomCos(sample, 0.5*(ph+pl), 0.5*(ch+cl))){
      std::cout << "Closure test failed!! " << std::endl;
      std::cout << "Global Bin: " << j << std::endl;
      std::cout << "Should be identical to bd.GetGlobalBinMatrixMomCos(" << sample << ", " << 0.5*(ph+pl) << ", "<< 0.5*(ch+cl) << ") " <<  bd.GetGlobalBinMatrixMomCos(sample, 0.5*(ph+pl), 0.5*(ch+cl)) << std::endl;
      throw;
    }
    if(j != bd.GetGlobalBinMatrix(sample)+bincos*bd.GetNbins_Mom(sample)+binmom){
      std::cout << "Closure test failed!! " << std::endl;
      std::cout << "Global Bin: " << j << std::endl;
      std::cout << "Should be identical to bd.GetGlobalBinMatrix(" << sample << ")" <<  bd.GetGlobalBinMatrix(sample) << " + bincos*bd.GetNbins_Mom(sample)+binmom" << std::endl;
      throw;
    }
    double epsilon = 0.0001; // Make sure this is smaller that the cos binning!!
    int bin_det  = bd.GetGlobalBinMatrixMomCos_Det(sample, ph-epsilon, ch-epsilon);
    int bin2_det = bd.GetGlobalBinMatrixMomCos_Det(sample, pl+epsilon, cl+epsilon);
    int binmom_det, bincos_det;
    SampleId::SampleEnum sample_det;
    bd.GetLocalBinMatrixMomCos_Det(bin_det, sample_det, binmom_det, bincos_det);
    
    if(bin_det != bin2_det){
      std::cerr << "The fine binning is more coarse than the detector binning!!!" << std::endl;
      std::cerr << "Maybe epilson is too large for the Cos binning?" << std::endl;
      
      std::cerr << "First detector cov bin corresponds to (low edge of the fine binning): Global bin " << bin_det << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Mom Binning->GetBinLowEdge("<<binmom_det<<") " << bd.GetBinning_Mom_Det(sample_det)->GetBinLowEdge(binmom_det+1) << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Mom Binning->GetBinUpEdge ("<<binmom_det<<") " << bd.GetBinning_Mom_Det(sample_det)->GetBinUpEdge (binmom_det+1) << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Cos Binning->GetBinLowEdge("<<bincos_det<<") " << bd.GetBinning_Cos_Det(sample_det)->GetBinLowEdge(bincos_det+1) << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Cos Binning->GetBinUpEdge ("<<bincos_det<<") " << bd.GetBinning_Cos_Det(sample_det)->GetBinUpEdge (bincos_det+1) << std::endl;

      bd.GetLocalBinMatrixMomCos_Det(bin2_det, sample_det, binmom_det, bincos_det);
      std::cerr << "Second detector cov bin corresponds to (high edge of the fine binning): Global bin " << bin2_det << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Mom Binning->GetBinLowEdge("<<binmom_det<<") " << bd.GetBinning_Mom_Det(sample_det)->GetBinLowEdge(binmom_det+1) << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Mom Binning->GetBinUpEdge ("<<binmom_det<<") " << bd.GetBinning_Mom_Det(sample_det)->GetBinUpEdge (binmom_det+1) << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Cos Binning->GetBinLowEdge("<<bincos_det<<") " << bd.GetBinning_Cos_Det(sample_det)->GetBinLowEdge(bincos_det+1) << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Cos Binning->GetBinUpEdge ("<<bincos_det<<") " << bd.GetBinning_Cos_Det(sample_det)->GetBinUpEdge (bincos_det+1) << std::endl;

      std::cerr << "These 2 should be the same!!" << std::endl;
      throw;
    }

    double ph_det = bd.GetBinning_Mom_Det(sample_det)->GetBinUpEdge (binmom_det+1);
    double pl_det = bd.GetBinning_Mom_Det(sample_det)->GetBinLowEdge(binmom_det+1);
    double ch_det = bd.GetBinning_Cos_Det(sample_det)->GetBinUpEdge (bincos_det+1);
    double cl_det = bd.GetBinning_Cos_Det(sample_det)->GetBinLowEdge(bincos_det+1);
    if(bin_det != bd.GetGlobalBinMatrixMomCos_Det(sample_det, 0.5*(ph_det+pl_det), 0.5*(ch_det+cl_det))){
      std::cout << "Closure test failed!! " << std::endl;
      std::cout << "Global Bin: " << bin_det << std::endl;
      std::cout << "Should be identical to bd.GetGlobalBinMatrixMomCos_Det(" << sample_det << ", " << 0.5*(ph_det+pl_det) << ", "<< 0.5*(ch_det+cl_det) << ") " <<  bd.GetGlobalBinMatrixMomCos_Det(sample_det, 0.5*(ph_det+pl_det), 0.5*(ch_det+cl_det)) << std::endl;
      throw;
    }
    if(bin_det != bd.GetGlobalBinMatrix_Det(sample_det)+bincos_det*bd.GetNbins_Mom_Det(sample_det)+binmom_det){
      std::cout << "Closure test failed!! " << std::endl;
      std::cout << "Global Bin: " << bin_det << std::endl;
      std::cout << "Should be identical to bd.GetGlobalBinMatrix(" << sample_det << ", " << 0.5*(ph_det+pl_det) << ", "<< 0.5*(ch_det+cl_det) << ") " <<  bd.GetGlobalBinMatrix(sample_det) << " + bincos_det*bd.GetNbins_Mom_Det(sample_det)+binmom_det" << std::endl;
      throw;
    }
    FullToReduMap[j] = bin_det;
  } 

  std::vector<std::string> name_vector     = bd.GetListOfBins();
  std::vector<std::string> name_vector_det = bd.GetListOfBins_Det();

  // for(int i                                = 0; i < (int)name_vector.size(); i++)
  //   std::cout << name_vector[i] << std::endl;

  // for(int i                                = 0; i < (int)name_vector_det.size(); i++)
  //   std::cout << name_vector_det[i] << std::endl;
  
  TFile* fake = NULL;

  TH2D* fake_cov = NULL;
  if(Apply1p1h){
    std::cout << "Fake1P1H File " << Cov1P1HStr << std::endl;
    fake = new TFile(Cov1P1HStr.c_str());
    fake_cov = (TH2D*)fake->Get("cov_1p1h");
  }
  
 
  std::vector<std::string> SystNames;
  SystNames.push_back("BFieldDist");
  SystNames.push_back("MomScale"  );
  SystNames.push_back("MomResol"  );
  SystNames.push_back("TpcPid"    );
  SystNames.push_back("FgdPid"    );
  //SystNames.push_back("weight"    );
  //SystNames.push_back("all"       );

  for(std::vector<std::string>::iterator it = SystNames.begin(); it !=SystNames.end(); ++it){
    
    std::string TheSyst= (*it);
    bool WeightSyst = (TheSyst == "weight");
      
    TH2D* corr              [nWeights];
    TH2D* cov               [nWeights];
    TH2D* cov_var_nostat    [nWeights];
    TH2D* cov_var           [nWeights];
    TH2D* cov_zero          [nWeights];
    TH1D* var_mean          [nWeights];
    TH1D* var_mean_offset   [nWeights];
    TH1D* BruteForce        [nWeights];
    TH1D* GOF               [nWeights];
    TH1D* mc_sys_var_redu   [nWeights];  
    TH1D* mc_sys_error_redu [nWeights];
    TH1D* varied            [nWeights][nThrows];
    TH1D* nominal           ;
    TH1D* zero_point        ;
    TH1D* number_events_redu;
    TH1D* weights_histo     [nWeights];
    
    TH1D* mc_stat_var_full  ;  
    TH1D* mc_stat_var_redu  ;  
    TH1D* mc_stat_error_full;
    TH1D* mc_stat_error_redu;
    TH1D* mc_stat_nom       ;
    TH1D* number_events_full;
    TH1D* Selection         ;
    
    int nsyst = 1;
    if(WeightSyst) nsyst = nWeights;
    
    nominal            = new TH1D("Nominal",            "Nominal Events",                         nBinsRedu, 0, nBinsRedu);
    zero_point         = new TH1D("Zero_Point",         "Zero Point",                             nBinsRedu, 0, nBinsRedu);
    number_events_redu = new TH1D("number_events",      "Number of Events",                       nBinsRedu, 0, nBinsRedu);
    mc_stat_var_full   = new TH1D("mc_stat_var_full",   "MC statistical variance fine binning",   nBinsFull, 0, nBinsFull); 
    mc_stat_var_redu   = new TH1D("mc_stat_var_redu",   "MC statistical variance reduced binning",nBinsRedu, 0, nBinsRedu); 
    mc_stat_nom        = new TH1D("MC_Stat_Nom",        "MC statistical nominal",                 nBinsFull, 0, nBinsFull);
    number_events_full = new TH1D("number_events_full", "Number of Events",                       nBinsFull, 0, nBinsFull);
    Selection          = new TH1D("Selection",          "Selection;SampleId;N events",            SampleId::kNSamples, 0, SampleId::kNSamples);
    mc_stat_error_full = new TH1D("mc_stat_error_full", "MC statistical error fine binning",      nBinsFull, 0, nBinsFull); 
    mc_stat_error_redu = new TH1D("mc_stat_error_redu", "MC statistical error reduced binning",   nBinsRedu, 0, nBinsRedu); 
    
    for(int iSyst = 0; iSyst <nsyst; ++iSyst){
      
      corr           [iSyst] = new TH2D(Form("%d_Total_Correlation_Matrix",     iSyst), Form("%d Total Correlation Matrix",                      iSyst),  nBinsRedu, 0, nBinsRedu, nBinsRedu, 0, nBinsRedu);
      cov            [iSyst] = new TH2D(Form("%d_Total_Covariance_Matrix",      iSyst), Form("%d Total Covariance Matrix",                       iSyst),  nBinsRedu, 0, nBinsRedu, nBinsRedu, 0, nBinsRedu);
      cov_var_nostat [iSyst] = new TH2D(Form("%d_Covariance_Matrix_Var_nostat", iSyst), Form("%d Covariance Matrix for variation no stat",       iSyst),  nBinsRedu, 0, nBinsRedu, nBinsRedu, 0, nBinsRedu);
      cov_var        [iSyst] = new TH2D(Form("%d_Covariance_Matrix_Var",        iSyst), Form("%d Covariance Matrix for variation",               iSyst),  nBinsRedu, 0, nBinsRedu, nBinsRedu, 0, nBinsRedu);
      cov_zero       [iSyst] = new TH2D(Form("%d_Covariance_Matrix_Zero",       iSyst), Form("%d Covariance Matrix for zero point offset",       iSyst),  nBinsRedu, 0, nBinsRedu, nBinsRedu, 0, nBinsRedu);
      var_mean       [iSyst] = new TH1D(Form("%d_Varied_Mean",                  iSyst), Form("%d Varied Mean",                                   iSyst),  nBinsRedu, 0, nBinsRedu);
      var_mean_offset[iSyst] = new TH1D(Form("%d_Varied_Mean_over_Nominal",     iSyst), Form("%d Varied Mean normalised by nominal",             iSyst),  nBinsRedu, 0, nBinsRedu);
      BruteForce     [iSyst] = new TH1D(Form("%d_BruteForce",                   iSyst), Form("%d BruteForce",                                    iSyst),  nBinsRedu, 0, nBinsRedu);
      GOF            [iSyst] = new TH1D(Form("%d_GOF",                          iSyst), Form("%d Chi-square between gaussian and bin variation", iSyst),  nBinsRedu, 0, nBinsRedu);
      weights_histo  [iSyst] = new TH1D(Form("%d_weights_histo",                iSyst), Form("%d_weights_histo",                                 iSyst),  1002, -0.01, 10.01);

    
      for (int i = 0; i < nThrows; ++i){
        varied[iSyst][i] = new TH1D(Form("%d_Varied_Events_%d", iSyst, i), Form("%d Varied Events %d", iSyst, i), nBinsRedu, 0, nBinsRedu);
      }

      mc_sys_var_redu   [iSyst] = new TH1D(Form("%d_mc_sys_var_redu",     iSyst), Form("%d MC systematic variance reduced binning", iSyst), nBinsRedu, 0, nBinsRedu);
      mc_sys_error_redu [iSyst] = new TH1D(Form("%d_mc_sys_error_redu",   iSyst), Form("%d MC systematic error reduced binning",    iSyst), nBinsRedu, 0, nBinsRedu);
    }
  

    TChain* MCChain = new TChain(TheSyst.c_str());
    
    ifstream MCFilesList;
    MCFilesList.open(MCFilesListStr.c_str());
  
    if(MCFilesList.is_open()){
      std::string line;
      while(getline(MCFilesList, line)){
        std::cout << "Adding the file " << line << " to the TChain" << std::endl;
        MCChain->Add(line.c_str());
      }
      MCFilesList.close();
    }else{
      std::cerr << "List of MC files not open!!" << std::endl;
      throw;
    }

    ifstream NIWGFilesList;
    NIWGFilesList.open(NIWGFilesListStr.c_str());
    TChain* NIWGTune = NULL;

    if(ApplyNIWG){
      if(NIWGFilesList.is_open()){
        NIWGTune = new TChain("NIWGWeightTree");
        std::string line;
        while(getline(NIWGFilesList, line)){
          std::cout << "NIWG tune File " << line << std::endl;
          NIWGTune->Add(line.c_str());
        }
        NIWGFilesList.close();
      }else{
        std::cerr << "List of MC files not open!!" << std::endl;
        throw;
      }
    }
  
    int    Run            ;
    int    SubRun         ;
    int    EventNumber    ;
    int    SelectionNom   ;
    double TrueEnuNom     ;
    int    TrueNuPDGNom   ;
    int    TrueVertexIDNom;
    double LeptonMomNom   ;
    double LeptonCosNom   ;
    double WeightNom      ;
    double FluxWeightNom  ;
    double NIWGWeightNom  ;
    int    nToys          ;
    int    Toy            [nThrows];
    int    TrueVertexIDToy[nThrows];
    int    SelectionToy   [nThrows];
    double TrueEnuToy     [nThrows];
    int    TrueNuPDGToy   [nThrows];
    double LeptonMomToy   [nThrows];
    double LeptonCosToy   [nThrows];
    double WeightToy      [nThrows];
    double WeightIndToy   [20][nThrows];
    double FluxWeightToy  [nThrows];
    double NIWGWeightToy  [nThrows];
  
    MCChain->SetBranchAddress("Run"            , &Run            );
    MCChain->SetBranchAddress("SubRun"         , &SubRun         );
    MCChain->SetBranchAddress("EventNumber"    , &EventNumber    );
    MCChain->SetBranchAddress("SelectionNom"   , &SelectionNom   );
    MCChain->SetBranchAddress("TrueEnuNom"     , &TrueEnuNom     );
    MCChain->SetBranchAddress("TrueNuPDGNom"   , &TrueNuPDGNom   );
    MCChain->SetBranchAddress("TrueVertexIDNom", &TrueVertexIDNom);
    MCChain->SetBranchAddress("LeptonMomNom"   , &LeptonMomNom   );
    MCChain->SetBranchAddress("LeptonCosNom"   , &LeptonCosNom   );
    MCChain->SetBranchAddress("WeightNom"      , &WeightNom      );
    MCChain->SetBranchAddress("FluxWeightNom"  , &FluxWeightNom  );
    MCChain->SetBranchAddress("nToys"          , &nToys          );
    MCChain->SetBranchAddress("Toy"            ,  Toy            );
    MCChain->SetBranchAddress("TrueVertexIDToy",  TrueVertexIDToy);
    MCChain->SetBranchAddress("SelectionToy"   ,  SelectionToy   );
    MCChain->SetBranchAddress("TrueEnuToy"     ,  TrueEnuToy     );
    MCChain->SetBranchAddress("TrueNuPDGToy"   ,  TrueNuPDGToy   );
    MCChain->SetBranchAddress("LeptonMomToy"   ,  LeptonMomToy   );
    MCChain->SetBranchAddress("LeptonCosToy"   ,  LeptonCosToy   );
    if(!IgnoreWeightSyst){
      if(!WeightSyst)
        MCChain->SetBranchAddress("WeightToy",     WeightToy);
      else{
        MCChain->SetBranchAddress("WeightIndToy",  WeightIndToy);
      }
    }
    for(int i = 0; i < nThrows; i++){
      if(IgnoreWeightSyst)
        WeightToy[i] = 1;
      NIWGWeightToy[i] = 1;
      FluxWeightToy[i] = 1;
    }
    NIWGWeightNom = 1.;
    
    std::cout << "Creating the file " << TheSyst+"_"+OutFileStr << std::endl;
    TFile* OutputFile = new TFile((TheSyst+"_"+OutFileStr).c_str(), "RECREATE");
  
    std::cout << "We have " << nBinsRedu <<  " bins in the covariance matrix" << std::endl;

    for(int i = 0; i < (int)MCChain->GetEntries(); ++i){
      MCChain->GetEntry(i);
      if(nThrows != nToys){
        std::cerr << "nThrows should be equal to nToys, change this in the beginning MakeND280Cov and recompile" << std::endl;
        throw;
      }
      if(i%1000 == 0){
        std::cout << "Progress     " << 100. * ((double) i) / MCChain->GetEntries() << "%" << std::endl;
      }

      if(bd.IsActiveSample(SelectionNom)){
        Selection->Fill(SelectionNom);
        int BinRedu = bd.GetGlobalBinMatrixMomCos_Det((SampleId::SampleEnum)SelectionNom,
                                                      LeptonMomNom, LeptonCosNom) + 1;
        if(BinRedu > nBinsRedu){
          std::cout << "BinRedu > nBinsRedu" << std::endl;
          throw;
        }
        int BinFull = bd.GetGlobalBinMatrixMomCos((SampleId::SampleEnum)SelectionNom,
                                                  LeptonMomNom, LeptonCosNom)+1;
        if(BinFull > nBinsFull){
          std::cout << "BinFull > nBinsFull" << std::endl;
          throw;
        }
        if(TMath::IsNaN(WeightNom    ) || !TMath::Finite(WeightNom    ) || WeightNom      == -999 || WeightNom      > 100000){ std::cerr << "WeightNom isn't finite: "     << WeightNom     << std::endl; throw;}
        if(TMath::IsNaN(NIWGWeightNom) || !TMath::Finite(NIWGWeightNom) || NIWGWeightNom  == -999 || NIWGWeightNom  > 10000){ std::cerr << "NIWGWeightNom isn't finite: " << NIWGWeightNom << std::endl; throw;}
        if(TMath::IsNaN(FluxWeightNom) || !TMath::Finite(FluxWeightNom) || FluxWeightNom  == -999 || FluxWeightNom  > 10000){ std::cerr << "FluxWeightNom isn't finite: " << FluxWeightNom << std::endl; throw;}
    
        number_events_redu->Fill(BinRedu);
        number_events_full->Fill(BinFull);
     
        nominal    ->SetBinContent(BinRedu, nominal    ->GetBinContent(BinRedu) + WeightNom * NIWGWeightNom);
        zero_point ->SetBinContent(BinRedu, zero_point ->GetBinContent(BinRedu) + WeightNom * NIWGWeightNom * FluxWeightNom);
        mc_stat_nom->SetBinContent(BinFull, mc_stat_nom->GetBinContent(BinFull) + FluxWeightNom * NIWGWeightNom);
      
        mc_stat_var_full->SetBinContent(BinFull, mc_stat_var_full->GetBinContent(BinFull) + TMath::Power(FluxWeightNom * NIWGWeightNom, 2.));
        mc_stat_var_redu->SetBinContent(BinRedu, mc_stat_var_redu->GetBinContent(BinRedu) + TMath::Power(FluxWeightNom * NIWGWeightNom, 2.));
      
      }
      
      for(int iSyst = 0; iSyst <nsyst; ++iSyst){
        for(int iToy = 0; iToy < nThrows; ++iToy){
          if(bd.IsActiveSample(SelectionToy[iToy])){
            int BinRedu = bd.GetGlobalBinMatrixMomCos_Det((SampleId::SampleEnum)SelectionToy[iToy],
                                                          LeptonMomToy[iToy], LeptonCosToy[iToy])+1;
            if(BinRedu > nBinsRedu){
              std::cout << "BinRedu > nBinsRedu" << std::endl;
              throw;
            }
            if(!WeightSyst){
              if(TMath::IsNaN(WeightToy[iToy]    ) || !TMath::Finite(WeightToy[iToy]    ) || WeightToy[iToy]      == -999 || WeightToy[iToy]      > 100000){ std::cerr << "WeightToy["     << iToy << "] isn't finite: " << WeightToy[iToy]     << std::endl; throw;}
              if(TMath::IsNaN(NIWGWeightToy[iToy]) || !TMath::Finite(NIWGWeightToy[iToy]) || NIWGWeightToy[iToy]  == -999 || NIWGWeightToy[iToy]  > 10000){ std::cerr << "NIWGWeightToy[" << iToy << "] isn't finite: " << NIWGWeightToy[iToy] << std::endl; throw;}
              if(TMath::IsNaN(FluxWeightToy[iToy]) || !TMath::Finite(FluxWeightToy[iToy]) || FluxWeightToy[iToy]  == -999 || FluxWeightToy[iToy]  > 10000){ std::cerr << "FluxWeightToy[" << iToy << "] isn't finite: " << FluxWeightToy[iToy] << std::endl; throw;}

              if(WeightToy[iToy] < 10 && WeightToy[iToy] > 0){
                varied[iSyst][iToy]->SetBinContent(BinRedu, varied[iSyst][iToy]->GetBinContent(BinRedu) + FluxWeightToy[iToy] * NIWGWeightToy[iToy] * WeightToy[iToy]);
                weights_histo[iSyst]->Fill(WeightToy[iToy]);
              }else if(WeightToy[iToy] > 10){
                varied[iSyst][iToy]->SetBinContent(BinRedu, varied[iSyst][iToy]->GetBinContent(BinRedu) + FluxWeightToy[iToy] * NIWGWeightToy[iToy] * 10.);
                weights_histo[iSyst]->Fill(10.005);
              }else if(WeightToy[iToy] < 0){
                weights_histo[iSyst]->Fill(-0.005);
              }
              
            }else{
              if(TMath::IsNaN(WeightIndToy[iSyst][iToy]) || !TMath::Finite(WeightIndToy[iSyst][iToy]) || WeightIndToy[iSyst][iToy] == -999 || WeightIndToy[iSyst][iToy] > 100000){ std::cerr << "WeightIndToy[" << iSyst << "][" << iToy <<"] isn't finite: " << WeightIndToy[iSyst][iToy] << std::endl; throw;}
              if(TMath::IsNaN(NIWGWeightToy[iToy]      ) || !TMath::Finite(NIWGWeightToy[iToy]      ) || NIWGWeightToy[iToy]       == -999 || NIWGWeightToy[iToy]       > 10000){ std::cerr << "NIWGWeightToy["                 << iToy << "] isn't finite: " << NIWGWeightToy[iToy]      << std::endl; throw;}
              if(TMath::IsNaN(FluxWeightToy[iToy]      ) || !TMath::Finite(FluxWeightToy[iToy]      ) || FluxWeightToy[iToy]       == -999 || FluxWeightToy[iToy]       > 10000){ std::cerr << "FluxWeightToy["                 << iToy << "] isn't finite: " << FluxWeightToy[iToy]      << std::endl; throw;}

              varied[iSyst][iToy]->SetBinContent(BinRedu, varied[iSyst][iToy]->GetBinContent(BinRedu) + FluxWeightToy[iToy] * NIWGWeightToy[iToy]);
              weights_histo[iSyst]->Fill(WeightIndToy[iSyst][iToy]);
              // }else if(WeightIndToy[iSyst][iToy] > 10){
              //   varied[iSyst][iToy]->SetBinContent(BinRedu, varied[iSyst][iToy]->GetBinContent(BinRedu) + FluxWeightToy[iToy] * NIWGWeightToy[iToy] * 10.);
              //   weights_histo[iSyst]->Fill(10.005);
              // }else if(WeightIndToy[iSyst][iToy] < 0){
              //   weights_histo[iSyst]->Fill(-0.005);
              // }
            }
          }
          
        }
      }
    }
  
    for(int iSyst = 0; iSyst < nsyst; ++iSyst){
      std::cout << "Considering isyst " << iSyst << " / " << nsyst << std::endl;
      for(int iToy = 0; iToy < nThrows; ++iToy){
        for(int iBin = 1; iBin < nBinsRedu+1; ++iBin){
          var_mean[iSyst]->SetBinContent(iBin, var_mean[iSyst]->GetBinContent(iBin) + varied[iSyst][iToy]->GetBinContent(iBin)/double(nThrows));
        }
      }
      for(int iBin = 1; iBin < nBinsRedu+1; ++iBin){
        for(int iToy = 0; iToy < nThrows; ++iToy){
          mc_sys_var_redu[iSyst]->SetBinContent(iBin,  mc_sys_var_redu[iSyst]->GetBinContent(iBin) + ((varied[iSyst][iToy]->GetBinContent(iBin) - var_mean[iSyst]->GetBinContent(iBin)) *
                                                                                                      (varied[iSyst][iToy]->GetBinContent(iBin) - var_mean[iSyst]->GetBinContent(iBin))));
        }
        if(var_mean[iSyst]->GetBinContent(iBin) > 0) mc_sys_var_redu[iSyst]->SetBinContent(iBin, TMath::Sqrt(mc_sys_var_redu[iSyst]->GetBinContent(iBin) / (double(nThrows) * var_mean[iSyst]->GetBinContent(iBin) * var_mean[iSyst]->GetBinContent(iBin))));
      }
  
      for(int iToy = 0; iToy < nThrows; ++iToy){
        for(int iBin = 1; iBin < nBinsRedu+1; ++iBin){
          for(int jBin = 1; jBin < nBinsRedu+1; ++jBin){
            cov_var_nostat[iSyst]->SetBinContent(iBin, jBin, cov_var_nostat[iSyst]->GetBinContent(iBin,jBin) + ((varied[iSyst][iToy]->GetBinContent(iBin) - var_mean[iSyst]->GetBinContent(iBin))*
                                                                                                                (varied[iSyst][iToy]->GetBinContent(jBin) - var_mean[iSyst]->GetBinContent(jBin))));
            cov_var[iSyst]->SetBinContent(iBin,jBin, cov_var_nostat[iSyst]->GetBinContent(iBin,jBin));
          }
        }
      }

      for(int iBin = 1; iBin < nBinsFull+1; ++iBin){
        if(mc_stat_nom->GetBinContent(iBin)>0){
          mc_stat_var_full  ->SetBinContent(iBin, mc_stat_var_full->GetBinContent(iBin) / (mc_stat_nom->GetBinContent(iBin) * mc_stat_nom->GetBinContent(iBin)));
          mc_stat_error_full->SetBinContent(iBin, TMath::Sqrt(mc_stat_var_full->GetBinContent(iBin)));
        }
      }
 
      for(int iBin = 1; iBin < nBinsRedu+1; ++iBin){
        //int BinInHisto = iBin+1
        if(nominal->GetBinContent(iBin)>0){
          var_mean_offset[iSyst]->SetBinContent(iBin, var_mean[iSyst]->GetBinContent(iBin) / nominal->GetBinContent(iBin));
        }else{
          var_mean_offset[iSyst]->SetBinContent(iBin,1);
        }
    
        for(int jBin = 1; jBin < nBinsRedu+1; ++jBin){
          if(fabs(cov_var[iSyst]->GetBinContent(iBin,jBin)) > 0.00001 && var_mean[iSyst]->GetBinContent(iBin) > 0 && var_mean[iSyst]->GetBinContent(jBin) > 0){
            cov_var[iSyst]       ->SetBinContent(iBin, jBin, cov_var       [iSyst]->GetBinContent(iBin,jBin)/(double(nThrows)*var_mean[iSyst]->GetBinContent(iBin)*var_mean[iSyst]->GetBinContent(jBin)));
            cov_var_nostat[iSyst]->SetBinContent(iBin, jBin, cov_var_nostat[iSyst]->GetBinContent(iBin,jBin)/(double(nThrows)*var_mean[iSyst]->GetBinContent(iBin)*var_mean[iSyst]->GetBinContent(jBin)));
          }else{
            // std::cout << "cov_var->GetBinContent(" << iBin << "," << jBin << "): " << cov_var[iSyst]->GetBinContent(iBin,jBin) << std::endl;
            //            std::cout << "nominal->GetBinContent(" << iBin << ") " << nominal->GetBinContent(iBin) << std::endl;
            //            std::cout << "nominal->GetBinContent(" << jBin << ") " << nominal->GetBinContent(jBin) << std::endl;
            //            std::cout << "GetBin(" << iBin << ") " << bd.GetSampleFromIndex_Det(iBin)  << "  between " << bd.GetSampleFromIndex_Det(iBin-1) << " and " << bd.GetSampleFromIndex_Det(iBin+1)<< std::endl;
            //            std::cout << "GetBin(" << jBin << ") " << bd.GetSampleFromIndex_Det(iBin)  << "  between " << bd.GetSampleFromIndex_Det(iBin-1) << " and " << bd.GetSampleFromIndex_Det(iBin+1)<< std::endl;
            //throw;
          }
      
          if(jBin == iBin){
            mc_sys_var_redu[iSyst]  ->SetBinContent(iBin,             cov_var[iSyst]->GetBinContent(iBin,jBin));

            if(nominal->GetBinContent(iBin)>0) mc_sys_error_redu[iSyst]->SetBinContent(iBin, TMath::Sqrt(cov_var[iSyst]->GetBinContent(iBin,jBin)));
            else {
              // std::cerr << "nominal->GetBinContent(" << iBin << ") " << nominal->GetBinContent(iBin) << std::endl;
              // throw;
            }
          }
          cov[iSyst]->SetBinContent(iBin, jBin, cov_var[iSyst]->GetBinContent(iBin,jBin));
        }
        if(nominal->GetBinContent(iBin)>0) mc_stat_error_redu->SetBinContent(iBin, TMath::Sqrt(mc_stat_var_redu->GetBinContent(iBin)));
      }
  
      for(int iBin = 1; iBin < nBinsRedu+1; ++iBin){
        for(int jBin = 1; jBin < nBinsRedu+1; ++jBin){
          if(cov[iSyst]->GetBinContent(iBin,iBin) > 0 && cov[iSyst]->GetBinContent(jBin,jBin) > 0)
            corr[iSyst]->SetBinContent(iBin, jBin, cov[iSyst]->GetBinContent(iBin,jBin) / TMath::Sqrt(cov[iSyst]->GetBinContent(iBin,iBin) * cov[iSyst]->GetBinContent(jBin,jBin)));
        }
      }

      OutputFile->cd();
  
      std::string CurSampleStr = "";
      for(int iBin = 0; iBin < nBinsRedu; ++iBin){
        std::string SampleStr = ConvertSample(bd.GetSampleFromIndex_Det(iBin));
        if(SampleStr != CurSampleStr){
          cov           [iSyst]->GetXaxis()->SetBinLabel(iBin+1,SampleStr.c_str());
          corr          [iSyst]->GetXaxis()->SetBinLabel(iBin+1,SampleStr.c_str());
          cov_zero      [iSyst]->GetXaxis()->SetBinLabel(iBin+1,SampleStr.c_str());
          cov           [iSyst]->GetYaxis()->SetBinLabel(iBin+1,SampleStr.c_str());
          corr          [iSyst]->GetYaxis()->SetBinLabel(iBin+1,SampleStr.c_str());
          cov_zero      [iSyst]->GetYaxis()->SetBinLabel(iBin+1,SampleStr.c_str());
          cov_var_nostat[iSyst]->GetXaxis()->SetBinLabel(iBin+1,SampleStr.c_str());
          cov_var       [iSyst]->GetXaxis()->SetBinLabel(iBin+1,SampleStr.c_str());
          cov_var_nostat[iSyst]->GetYaxis()->SetBinLabel(iBin+1,SampleStr.c_str());
          cov_var       [iSyst]->GetYaxis()->SetBinLabel(iBin+1,SampleStr.c_str());
      
          nominal                  ->GetXaxis()->SetBinLabel(iBin+1,SampleStr.c_str());
          mc_sys_var_redu   [iSyst]->GetXaxis()->SetBinLabel(iBin+1,SampleStr.c_str());
          mc_sys_error_redu [iSyst]->GetXaxis()->SetBinLabel(iBin+1,SampleStr.c_str());
          mc_stat_var_redu         ->GetXaxis()->SetBinLabel(iBin+1,SampleStr.c_str());
          var_mean          [iSyst]->GetXaxis()->SetBinLabel(iBin+1,SampleStr.c_str());
          zero_point               ->GetXaxis()->SetBinLabel(iBin+1,SampleStr.c_str());
          var_mean_offset   [iSyst]->GetXaxis()->SetBinLabel(iBin+1,SampleStr.c_str());
          number_events_redu       ->GetXaxis()->SetBinLabel(iBin+1,SampleStr.c_str());
          CurSampleStr = SampleStr;
        }
      }

      for(int iBin = 1; iBin < Selection->GetXaxis()->GetNbins()+1; ++iBin){
        std::string SampleStr = SampleId::ConvertSample((SampleId::SampleEnum)(iBin-1));
        Selection->GetXaxis()->SetBinLabel(iBin, SampleStr.c_str());
      }
  
      corr             [iSyst]->Write();
      cov              [iSyst]->Write();
      cov_var          [iSyst]->Write();
      cov_var_nostat   [iSyst]->Write();
      cov_zero         [iSyst]->Write();
      var_mean         [iSyst]->Write();
      var_mean_offset  [iSyst]->Write();
      mc_sys_var_redu  [iSyst]->Write(); 
      mc_sys_error_redu[iSyst]->Write();
      weights_histo    [iSyst]->Write();
  
      TDirectory* VarU = OutputFile->mkdir(Form("VariedUniverse_%d",iSyst));
      VarU->cd();
      for (int i = 0; i < nThrows; ++i){ varied[iSyst][i]->Write(); }

      TCanvas c2;
      OutputFile->cd();
      TDirectory* BinVar = OutputFile->mkdir(Form("BinVariations_%d",iSyst));
      BinVar->cd();

      for(int iBin = 1; iBin < nBinsRedu+1; ++iBin){
        c2.SetName(Form("Bin_%d_NEvent_Spread",iBin));
        c2.SetTitle(Form("Bin_%d_NEvent_Spread",iBin));
        int nevent = int(nominal->GetBinContent(iBin));
        if(nevent == 0) nevent = 10;
        TH1F h1(Form("Bin_%d_NEvent_Spread_Hist_%d",iBin,iSyst),
                Form("Bin %d NEvent Spread",iBin), int(nominal->GetBinContent(iBin))*12, 0, int(nominal->GetBinContent(iBin))*3);

        TF1 f1("f1","gaus",nominal->GetBinContent(iBin)*0.5, nominal->GetBinContent(iBin)*1.5);
        TF1 f2("f2","gaus",nominal->GetBinContent(iBin)*0.5, nominal->GetBinContent(iBin)*1.5);

        f1.SetNameTitle(Form("Bin_%d_NEvent_Varying_Gaussian",iBin), "Varying Gaussian");
        f2.SetNameTitle(Form("Bin_%d_NEvent_Total_Gaussian",  iBin), "Total Gaussian"  );

        for(int iToy = 0; iToy < nThrows; ++iToy){
          h1.Fill(varied[iSyst][iToy]->GetBinContent(iBin));
        }
        if(h1.GetMean()>0)BruteForce[iSyst]->SetBinContent(iBin, h1.GetRMS()/h1.GetMean());
        h1.GetXaxis()->SetTitle("N Events selected");
        h1.SetStats(kFALSE);
        h1.Draw();

        f1.SetParameters(h1.GetBinContent(h1.GetMaximumBin()),
                         var_mean[iSyst]->GetBinContent(iBin),
                         sqrt(cov_var[iSyst]->GetBinContent(iBin,iBin))*nominal->GetBinContent(iBin));
        f2.SetParameters(h1.GetBinContent(h1.GetMaximumBin()),
                         var_mean[iSyst]->GetBinContent(iBin),
                         sqrt(cov[iSyst]->GetBinContent(iBin,iBin))*nominal->GetBinContent(iBin));

        // h1.GetYaxis()->SetRangeUser(0, h1.GetMaximum()*1.1);
        // h1.GetXaxis()->SetRange(h1.FindFirstBinAbove(0)-5, h1.FindLastBinAbove(0)+5);

        f1.FixParameter(0,(h1.Integral()/4.0)/(sqrt(TMath::Pi()*2)*sqrt(cov_var[iSyst]->GetBinContent(iBin,iBin))*nominal->GetBinContent(iBin)));
        f1.FixParameter(1,var_mean[iSyst]->GetBinContent(iBin));
        f1.FixParameter(2,sqrt(cov_var[iSyst]->GetBinContent(iBin,iBin)) * nominal->GetBinContent(iBin));

        h1.Fit(&f1,"BNQ");
        double chi2 = f1.GetChisquare();

        GOF[iSyst]->SetBinContent(iBin,chi2);

        TLine l1(nominal   ->GetBinContent(iBin),0,nominal   ->GetBinContent(iBin),h1.GetMaximum());
        TLine l2(zero_point->GetBinContent(iBin),0,zero_point->GetBinContent(iBin),h1.GetMaximum());

        f1.SetLineWidth(2);
        f1.SetLineColor(kRed);
        f2.SetLineWidth(2);
        f2.SetLineColor(kGreen);

        f1.Draw("SAME");
        f2.Draw("SAME");

        l1.SetLineWidth(2);
        l1.SetLineStyle(2);
        l1.SetLineColor(kBlack);
        l2.SetLineWidth(2);
        l2.SetLineStyle(2);
        l2.SetLineColor(kOrange);

        l1.Draw("SAME");
        l2.Draw("SAME");

        TLegend* leg = new TLegend(0.65,0.7,0.98,0.94);
        leg->AddEntry(&h1, "Bin content from throws","l");
        leg->AddEntry(&f1, "Gaussian from throw cov","l");
        leg->AddEntry(&f2, "Gaussian from total cov","l");
        leg->AddEntry(&l1, "Nominal bin content","l");
        leg->AddEntry(&l2, "Zero weight bin content","l");
        leg->SetFillColor(kWhite);
        leg->Draw();
        c2.Update();
        c2.Write();
      }
      BruteForce[iSyst]->Write();
      GOF[iSyst]->Write();
      std::cout << "Finished with isyst " << iSyst << " / " << nsyst << std::endl;

    }
   
    Selection      ->Write();
    nominal        ->Write();
    zero_point     ->Write();
    mc_stat_var_full->Write();
    mc_stat_var_redu->Write(); 
    mc_stat_error_full->Write();
    mc_stat_error_redu->Write(); 
    number_events_redu->Write();
    mc_stat_nom       ->Write();
    number_events_full->Write();
    Selection         ->Write();
  
  
    OutputFile->Close();
    delete    nominal           ;
    delete    zero_point        ;
    delete    number_events_redu;
    delete    mc_stat_var_full  ;  
    delete    mc_stat_var_redu  ;  
    delete    mc_stat_error_full;
    delete    mc_stat_error_redu;
    delete    mc_stat_nom       ;
    delete    number_events_full;
    delete    Selection;
    
    for(int isyst=0; isyst<nsyst;++isyst){
      delete corr           [isyst];
      delete cov            [isyst];
      delete cov_var_nostat [isyst];
      delete cov_var        [isyst];
      delete cov_zero       [isyst];
      delete var_mean       [isyst];
      delete var_mean_offset[isyst];
      delete BruteForce     [isyst];
      delete GOF            [isyst];
      delete mc_sys_var_redu   [isyst];  
      delete mc_sys_error_redu [isyst];
      for(int itoy=0;itoy<1000; ++itoy)delete varied[isyst][itoy];
    }
  
  }
  
}


void MakeCovariance4Arg(std::string MCFilesListStr, std::string NIWGFilesListStr,
std::string Cov1P1HStr,     std::string OutFileStr){
  MakeCovariance(MCFilesListStr, NIWGFilesListStr, Cov1P1HStr, OutFileStr);
}

void MakeCovariance3Arg(std::string MCFilesListStr, std::string NIWGFilesListStr, std::string OutFileStr){
  MakeCovariance(MCFilesListStr, NIWGFilesListStr, "", OutFileStr);
}

void MakeCovariance2Arg(std::string MCFilesListStr, std::string OutFileStr){
  MakeCovariance(MCFilesListStr, "", "", OutFileStr);
}

int main(int argn, char** arg){
  
  switch (argn){
    case 5:{
      ApplyNIWG = true;
      Apply1p1h = true;
      MakeCovariance4Arg(std::string(arg[1]), std::string(arg[2]), std::string(arg[3]), std::string(arg[4]));
      break;
    }
    case 4:{
      ApplyNIWG = true;
      Apply1p1h = false;
      MakeCovariance3Arg(std::string(arg[1]), std::string(arg[2]), std::string(arg[3]));
      break;
    }
    case 3:{
      ApplyNIWG = false;
      Apply1p1h = false;
      MakeCovariance2Arg(std::string(arg[1]), std::string(arg[2]));
      break;
    }
    default:{
      std::cout << "\n\n\nUsage:\nMakeND280DetectorCov.exe ListOfFilesWithNDThrows ListOfFileWithNIWGWeight 1p1hCov.root outfile.root" << std::endl;
      std::cout << "or:\nMakeND280DetectorCov.exe ListOfFilesWithNDThrows ListOfFileWithNIWGWeight outfile.root" << std::endl;
      std::cout << "or:\nMakeND280DetectorCov.exe ListOfFilesWithNDThrows outfile.root" << std::endl;
      throw;
    }
  }

  return 0;
  
}


