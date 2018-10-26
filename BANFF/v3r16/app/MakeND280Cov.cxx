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
#include "TROOT.h"
#include "TKey.h"

#include "BinningDefinition.hxx"
#include "ND280AnalysisUtils.hxx"
#include "Parameters.hxx"

bool ApplyNIWG = false;
bool Apply1p1h = false;
const int nThrows = 1000;

void MakeCovariance(char* MCFilesListStr, char* NIWGFilesListStr,
        char* Cov1P1HStr,     char* OutFileStr){

    ND::params().SetReadParamOverrideFilePointPassed();
    BANFF::BinningDefinition& bd = BANFF::BinningDefinition::Get();

    std::vector<std::string> name_vector = bd.GetListOfBins();
    std::vector<std::string> name_vector_det = bd.GetListOfBins_Det();

    // for(int i = 0; i < (int)name_vector.size(); i++)
    //   std::cout << name_vector[i] << std::endl;

    // for(int i = 0; i < (int)name_vector_det.size(); i++)
    //   std::cout << name_vector_det[i] << std::endl;

    TFile* fake = NULL;

    TH2D* fake_cov = NULL;
    if(Apply1p1h){
        std::cout << "Fake1P1H File " << Cov1P1HStr << std::endl;
        fake = new TFile(Cov1P1HStr);
        fake_cov = (TH2D*)fake->Get("cov_1p1h");
    }

    ifstream MCFilesList;
    MCFilesList.open(MCFilesListStr);

    std::map<std::string, TChain*> MCChains;


    //  TChain* MCChain = new TChain("all_syst");

    if(MCFilesList.is_open()){
        std::string line;
        while(getline(MCFilesList, line)){
            if(MCChains.size() == 0){
                std::cout << "Reading list of systematics trees from file" << std::endl;
                TFile temp(line.c_str(), "READ");

                TIter next(temp.GetListOfKeys());
                TKey *key;
                while ((key = (TKey*)next())) {
                    TClass *cl = gROOT->GetClass(key->GetClassName());
                    if (!cl->InheritsFrom("TTree")) continue;
                    TTree *h = (TTree*)key->ReadObj();
                    std::string name = h->GetName();
                    std::cout << "Adding syst " << name << " to TChain map" << std::endl;
                    MCChains.insert(std::pair<std::string, TChain*>(name, new TChain(name.c_str())));
                } 
            }
            std::cout << "Adding the file " << line << " to the TChain" << std::endl;
            for(std::map<std::string, TChain*>::iterator it = MCChains.begin(); it != MCChains.end(); ++it){
                it->second->Add(line.c_str());
            }
        }
        MCFilesList.close();
    }else{
        std::cerr << "List of MC files not open!!" << std::endl;
        throw;
    }

    ifstream NIWGFilesList;
    NIWGFilesList.open(NIWGFilesListStr);
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
    double LeptonMomFDS   ;
    double LeptonCosNom   ;
    double WeightNom      ;
    double FluxWeightNom  ;
    double NIWGWeightNom  ;
    int    nToys          ;
    int    Toy            [1000];
    int    TrueVertexIDToy[1000];
    int    SelectionToy   [1000];
    double TrueEnuToy     [1000];
    int    TrueNuPDGToy   [1000];
    double LeptonMomToy   [1000];
    double LeptonCosToy   [1000];
    double WeightToy      [1000];
    double FluxWeightToy  [1000];
    double NIWGWeightToy  [1000];


    TChain* MCChain;

    for(std::map<std::string, TChain*>::iterator it = MCChains.begin(); it != MCChains.end(); ++it){

        MCChain = it->second;



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
        MCChain->SetBranchAddress("WeightToy"      ,  WeightToy      );
        MCChain->SetBranchAddress("FluxWeightToy"  ,  FluxWeightToy  );
        if(ApplyNIWG){
            NIWGTune->SetBranchAddress("T2KRW_NIWGWeightNom",    &NIWGWeightNom);
            NIWGTune->SetBranchAddress("T2KRW_LeptonMom1p1hFDS", &LeptonMomFDS);
            NIWGTune->SetBranchAddress("T2KRW_NIWGWeightToy",     NIWGWeightToy);
            MCChain->AddFriend(NIWGTune);
            TH2D* check1 = new TH2D("EnuCheck", "EnuCheck;Enu;Enu", 1000, 0, 5000, 1000, 0, 5000);
            MCChain->Project("EnuCheck", "TrueEnuToy:T2KRW_EnuToy");
            TH2D* check2 = new TH2D("EnuCheck2", "EnuCheck2;Enu;Enu", 1000, 0, 5000, 1000, 0, 5000);
            MCChain->Project("EnuCheck2", "TrueEnuNom:T2KRW_EnuNom");

            TCanvas* c = new TCanvas();
            check1->Draw();
            c->SaveAs("check1.png");

            TCanvas* b = new TCanvas();
            check2->Draw();
            b->SaveAs("check2.png");
        }else{
            std::cout << "WARNING!! NOT APPLYING THE NIWG WEIGHTING" << std::endl;
            for(int i = 0;i < 1000; i++) NIWGWeightToy[i] = 1;
            NIWGWeightNom = 1.;
        }

        std::cout << "Creating the file " << OutFileStr << ".root" << std::endl;
        TFile* OutputFile = new TFile(Form("%s_%s.root",OutFileStr,(it->first).c_str()), "RECREATE");

        int nBinsRedu = bd.GetNbins_Det();
        int nBinsFull = bd.GetNbins();

        std::cout << "We have " << nBinsRedu <<  " bins in the covariance matrix" << std::endl;

        TMatrixTSym<double> matrix(nBinsRedu);
        TVectorT   <double> vector(nBinsRedu);
        TH2D* corr               = new TH2D("Total_Correlation_Matrix", "Total Correlation Matrix",                      nBinsRedu, 0, nBinsRedu, nBinsRedu, 0, nBinsRedu);
        TH2D* cov                = new TH2D("Total_Covariance_Matrix",  "Total Covariance Matrix",                       nBinsRedu, 0, nBinsRedu, nBinsRedu, 0, nBinsRedu);
        TH2D* cov_var            = new TH2D("Covariance_Matrix_Var",    "Covariance Matrix for variation",               nBinsRedu, 0, nBinsRedu, nBinsRedu, 0, nBinsRedu);
        TH2D* cov_zero           = new TH2D("Covariance_Matrix_Zero",   "Covariance Matrix for zero point offset",       nBinsRedu, 0, nBinsRedu, nBinsRedu, 0, nBinsRedu);
        TH2D* cov_nofake         = new TH2D("Covariance_Matrix_NoFake", "Covariance Matrix without 1p1h error",          nBinsRedu, 0, nBinsRedu, nBinsRedu, 0, nBinsRedu);
        TH2D* fake_error_cov     = new TH2D("1p1h_fake_Error_cov",      "1p1h systematic error",                         nBinsRedu, 0, nBinsRedu, nBinsRedu, 0, nBinsRedu);
        TH1D* fake_error_redu    = new TH1D("1p1h_fake_Error_redu",     "1p1h systematic error",                         nBinsRedu, 0, nBinsRedu);
        TH1D* nominal            = new TH1D("Nominal",                  "Nominal Events",                                nBinsRedu, 0, nBinsRedu);
        TH1D* mc_sys_error       = new TH1D("MC_Sys_Error",             "MC systematic error",                           nBinsRedu, 0, nBinsRedu);
        TH1D* mc_stat_error_redu = new TH1D("MC_Stat_Error_Redu",       "MC statistical error",                          nBinsRedu, 0, nBinsRedu); 
        TH1D* var_mean           = new TH1D("Varied_Mean",              "Varied Mean",                                   nBinsRedu, 0, nBinsRedu);
        TH1D* zero_point         = new TH1D("Zero_Point",               "Zero Point",                                    nBinsRedu, 0, nBinsRedu);
        TH1D* var_mean_offset    = new TH1D("Varied_Mean_over_Nominal", "Varied Mean normalised by nominal",             nBinsRedu, 0, nBinsRedu);
        TH1D* number_events_redu = new TH1D("number_events",            "Number of Events",                              nBinsRedu, 0, nBinsRedu);
        TH1D* GOF                = new TH1D("GOF",                      "Chi-square between gaussian and bin variation", nBinsRedu, 0, nBinsRedu);
        TH1D* varied[nThrows];
        for (int i = 0; i < nThrows; ++i){
            varied[i] = new TH1D(Form("Varied_Events_%d", i), Form("Varied Events %d", i), nBinsRedu, 0, nBinsRedu);
        }

        TH1D* mc_stat_error_full = new TH1D("MC_Stat_Error_full",   "MC statistical error",   nBinsFull, 0, nBinsFull);
        TH1D* mc_stat_nom        = new TH1D("MC_Stat_Nom",          "MC statistical nominal", nBinsFull, 0, nBinsFull);
        TH1D* number_events_full = new TH1D("number_events_full",   "Number of Events",       nBinsFull, 0, nBinsFull);
        TH1D* fake_error_full    = new TH1D("1p1h_fake_Error_full", "1p1h systematic error",  nBinsFull, 0, nBinsFull);

        TH1D* Selection = new TH1D("Selection", "Selection;SampleId;N events", SampleId::kNSamples, 0, SampleId::kNSamples);


        for(int iBin = 1; iBin < nBinsRedu+1; ++iBin){
            for(int jBin = 1; jBin < nBinsRedu+1; ++jBin){
                cov       ->SetBinContent(iBin, jBin, 0);
                cov_var   ->SetBinContent(iBin, jBin, 0);
                cov_zero  ->SetBinContent(iBin, jBin, 0);
                cov_nofake->SetBinContent(iBin, jBin, 0);
            }
            nominal           ->SetBinContent(iBin, 0);
            mc_stat_error_redu->SetBinContent(iBin, 0);
            mc_sys_error      ->SetBinContent(iBin, 0);
            var_mean          ->SetBinContent(iBin, 0);
            var_mean_offset   ->SetBinContent(iBin, 0);
            zero_point        ->SetBinContent(iBin, 0);

            for(int iToy = 0; iToy < nThrows; ++iToy){
                varied[iToy]->SetBinContent(iBin, 0);
            }
        }

        double POT_Weight = 1;

        for(int i = 0; i < (int)MCChain->GetEntries(); ++i){
            MCChain->GetEntry(i);
            if(nThrows != nToys){
                std::cerr << "nThrows should be equal to nToys, change this in the beginning MakeND280Cov and recompile" << std::endl;
                throw;
            }
            if(i%10000 == 0)
                std::cout << "Progress " << 100. * ((double) i) / MCChain->GetEntries() << "%" << std::endl;
            bool isSand = 0;
            Int_t runp = anaUtils::GetRunPeriod(Run, SubRun);

            if(anaUtils::GetSandMode(Run) >= 0)
                isSand = true;
            switch (runp){
                case 0://run1 water
                    POT_Weight = 0.018057359;
                    break;
                case 1://run2 water
                    POT_Weight = 4.33765e+19/1.20341e+21;
                    break;
                case 2://run2 air
                    POT_Weight = 3.59337e+19/9.23937e+20;
                    break;       
                case 3://run3b air
                case 4://run3c air
                    POT_Weight = 1.58103e+20/3.08013e+21;
                    if(isSand) POT_Weight = 0.49537864;
                    break;
                case 5://run4 water
                    POT_Weight = 1.64277e+20/1.89224e+21;	
                    break;
                case 6://run4 air
                    POT_Weight = 1.78271e+20/3.4996e+21;
                    break;
                case 8://run5 antinu-water
                    POT_Weight = 4.3468e+19/2.29627e+21;
                    if(isSand) POT_Weight = 0.036109244;
                    break;
                case 9://run6b antinu-air
                case 10://run6c antinu-air
                case 11://run6d antinu-air
                case 12://run6e antinu-air
                    POT_Weight = 3.40756e+20/9.903e+20;
                    break;
                case 15://run7 antinu-water
                    POT_Weight = 2.83453e+20/3.37059e+21;
                    break;
                default:
                    std::cout << "NOT FOUND?!?!" << std::endl;
                    throw;
            }
            if(isSand){
                for(int i = 0;i < 1000; i++) NIWGWeightToy[i] = 1;
                NIWGWeightNom = 1.;
            }

            if(bd.IsActiveSample(SelectionNom)){
                Selection->Fill(SelectionNom);
                int BinRedu = bd.GetGlobalBinMatrixMomCos_Det((SampleId::SampleEnum)SelectionNom,
                        LeptonMomNom, LeptonCosNom);
                if(BinRedu > nBinsRedu){
                    std::cout << "BinRedu > nBinsRedu" << std::endl;
                    throw;
                }
                int BinFull = bd.GetGlobalBinMatrixMomCos((SampleId::SampleEnum)SelectionNom,
                        LeptonMomNom, LeptonCosNom);
                if(BinFull > nBinsFull){
                    std::cout << "BinFull > nBinsFull" << std::endl;
                    throw;
                }
                if(TMath::IsNaN(WeightNom    ) || !TMath::Finite(WeightNom    ) || WeightNom      == -999 || WeightNom      > 1000){ std::cerr << "WeightNom isn't finite: "     << WeightNom     << std::endl; throw;}
                if(TMath::IsNaN(NIWGWeightNom) || !TMath::Finite(NIWGWeightNom) || NIWGWeightNom  == -999 || NIWGWeightNom  > 1000){ std::cerr << "NIWGWeightNom isn't finite: " << NIWGWeightNom << std::endl; throw;}
                if(TMath::IsNaN(FluxWeightNom) || !TMath::Finite(FluxWeightNom) || FluxWeightNom  == -999 || FluxWeightNom  > 1000){ std::cerr << "FluxWeightNom isn't finite: " << FluxWeightNom << std::endl; throw;}
                if(TMath::IsNaN(POT_Weight   ) || !TMath::Finite(POT_Weight   ) || POT_Weight     == -999 || POT_Weight     > 1000){ std::cerr << "POT_Weight isn't finite: "    << POT_Weight    << std::endl; throw;}


                number_events_redu->Fill(BinRedu);
                number_events_full->Fill(BinFull);

                nominal           ->SetBinContent(BinRedu, nominal           ->GetBinContent(BinRedu) + WeightNom * NIWGWeightNom * POT_Weight);
                zero_point        ->SetBinContent(BinRedu, zero_point        ->GetBinContent(BinRedu) + WeightNom * NIWGWeightNom * FluxWeightNom * POT_Weight);
                mc_stat_nom       ->SetBinContent(BinFull, mc_stat_nom       ->GetBinContent(BinFull) + FluxWeightNom * NIWGWeightNom * POT_Weight);
                mc_stat_error_full->SetBinContent(BinFull, mc_stat_error_full->GetBinContent(BinFull) + TMath::Power(FluxWeightNom * NIWGWeightNom * POT_Weight, 2.));
                mc_stat_error_redu->SetBinContent(BinRedu, mc_stat_error_redu->GetBinContent(BinRedu) + TMath::Power(FluxWeightNom * NIWGWeightNom * POT_Weight, 2.));

            }

            for(int iToy = 0; iToy < nThrows; ++iToy){
                if(bd.IsActiveSample(SelectionToy[iToy])){
                    int BinRedu = bd.GetGlobalBinMatrixMomCos_Det((SampleId::SampleEnum)SelectionToy[iToy],
                            LeptonMomToy[iToy], LeptonCosToy[iToy]);
                    if(BinRedu > nBinsRedu){
                        std::cout << "BinRedu > nBinsRedu" << std::endl;
                        throw;
                    }
                    if(!TMath::IsNaN(WeightToy[iToy]) && TMath::Finite(WeightToy[iToy])){

                        if(WeightToy[iToy] < 100){
                            varied[iToy]->SetBinContent(BinRedu, varied[iToy]->GetBinContent(BinRedu) + POT_Weight * FluxWeightToy[iToy] * NIWGWeightToy[iToy] * WeightToy[iToy]);
                            if(TMath::IsNaN(WeightToy[iToy]    ) || !TMath::Finite(WeightToy[iToy]    ) || WeightToy[iToy]      == -999 || WeightNom      > 1000){ std::cerr << "WeightToy["     << iToy << "] isn't finite: " << WeightToy[iToy]     << std::endl; throw;}
                            if(TMath::IsNaN(NIWGWeightToy[iToy]) || !TMath::Finite(NIWGWeightToy[iToy]) || NIWGWeightToy[iToy]  == -999 || NIWGWeightNom  > 1000){ std::cerr << "NIWGWeightToy[" << iToy << "] isn't finite: " << NIWGWeightToy[iToy] << std::endl; throw;}
                            if(TMath::IsNaN(FluxWeightToy[iToy]) || !TMath::Finite(FluxWeightToy[iToy]) || FluxWeightToy[iToy]  == -999 || FluxWeightNom  > 1000){ std::cerr << "FluxWeightToy[" << iToy << "] isn't finite: " << FluxWeightToy[iToy] << std::endl; throw;}
                            if(TMath::IsNaN(POT_Weight         ) || !TMath::Finite(POT_Weight         ) || POT_Weight           == -999 || POT_Weight     > 1000){ std::cerr << "POT_Weight isn't finite: "                    << POT_Weight          << std::endl; throw;}
                        }else{
                            varied[iToy]->SetBinContent(BinRedu, varied[iToy]->GetBinContent(BinRedu) + FluxWeightToy[iToy] * NIWGWeightToy[iToy] * POT_Weight * 100.);
                        }
                    }
                }
            }
        }

        for(int iToy = 0; iToy < nThrows; ++iToy){
            for(int iBin = 1; iBin < nBinsRedu+1; ++iBin){
                var_mean->SetBinContent(iBin, var_mean->GetBinContent(iBin) + varied[iToy]->GetBinContent(iBin)/double(nThrows));
            }
        }


        for(int iToy = 0; iToy < nThrows; ++iToy){
            for(int iBin = 1; iBin < nBinsRedu+1; ++iBin){
                for(int jBin = 1; jBin < nBinsRedu+1; ++jBin){
                    cov_var->SetBinContent(iBin, jBin, cov_var->GetBinContent(iBin,jBin) + ((varied[iToy]->GetBinContent(iBin) - var_mean->GetBinContent(iBin))*(varied[iToy]->GetBinContent(jBin) - var_mean->GetBinContent(jBin))));
                }
            }
        }

        for(int iBin = 1; iBin < nBinsFull+1; ++iBin){
            if(mc_stat_nom->GetBinContent(iBin)>0){
                mc_stat_error_full->SetBinContent(iBin, mc_stat_error_full->GetBinContent(iBin) / (mc_stat_nom->GetBinContent(iBin) * mc_stat_nom->GetBinContent(iBin)));
            }
        }

        int lastk = 1;
        for(int j = 1; j < nBinsFull; ++j){
            double staterr= mc_stat_error_full->GetBinContent(j);
            double fakeerr= fake_error_full->GetBinContent(j);
            SampleId::SampleEnum sample1 = bd.GetSampleFromIndex(j);
            int localbin = j - bd.GetGlobalBinMatrix(sample1);
            double p    = bd.GetBinning_Mom(sample1)->GetBinUpEdge (int(j/bd.GetNbins_Cos(sample1)) + 1);
            double c    = bd.GetBinning_Cos(sample1)->GetBinUpEdge (int(j%bd.GetNbins_Cos(sample1)) + 1);
            double plow = bd.GetBinning_Mom(sample1)->GetBinLowEdge(int(j/bd.GetNbins_Cos(sample1)));
            double clow = bd.GetBinning_Cos(sample1)->GetBinLowEdge(int(j%bd.GetNbins_Cos(sample1)));

            for(int k = bd.GetGlobalBinMatrix_Det(sample1); k < bd.GetNbins_Det(sample1); ++k){

                if(bd.GetBinning_Mom_Det(sample1)->GetBinLowEdge(int(k/bd.GetNbins_Cos_Det(sample1))) < plow ||
                        bd.GetBinning_Cos_Det(sample1)->GetBinLowEdge(int(k%bd.GetNbins_Cos_Det(sample1))) < clow)
                    continue;

                if(staterr > mc_stat_error_redu->GetBinContent(k)){
                    mc_stat_error_redu->SetBinContent(k, staterr);
                } 
                if(Apply1p1h){
                    if(fabs(fakeerr) > fabs(fake_error_redu->GetBinContent(k))){
                        fake_error_redu->SetBinContent(k, fakeerr);
                    }
                }
                break;

            }

            if(Apply1p1h){
                fake_error_redu->Write("Fake_Error_Lin");

                TH1D* fake_lin = new TH1D("Lin_1p1h","Lin_1p1h",nBinsRedu,0,nBinsRedu); 

                for(int j = 1; j < nBinsRedu+1; ++j){
                    fake_lin->SetBinContent(j,fake_error_cov->GetBinContent(j,j));
                    for(int k = j+1; k < nBinsRedu+1; ++k){
                        fake_error_cov->SetBinContent(j,k, fake_error_redu->GetBinContent(j) * fake_error_redu->GetBinContent(k));
                    }
                    fake_error_cov->SetBinContent(j,j, fake_error_redu->GetBinContent(j) * fake_error_redu->GetBinContent(j));
                }

                fake_lin->Write();

                for(int j = 1; j < nBinsRedu+1; ++j){
                    for(int k = 1; k < j; ++k){
                        fake_error_cov->SetBinContent(j,k, fake_error_cov->GetBinContent(k,j));
                    }
                }
            }
        }


        for(int iBin = 1; iBin < nBinsRedu+1; ++iBin){
            if(nominal->GetBinContent(iBin)>0){
                vector(iBin-1) = var_mean->GetBinContent(iBin) / nominal->GetBinContent(iBin);
                var_mean_offset->SetBinContent(iBin, var_mean->GetBinContent(iBin)/nominal->GetBinContent(iBin));
            }else{
                vector(iBin-1) = 1.0;
                var_mean_offset->SetBinContent(iBin,1);
            }

            for(int jBin = 1; jBin < nBinsRedu+1; ++jBin){
                if(fabs(cov_var->GetBinContent(iBin,jBin)) > 0.00001 && nominal->GetBinContent(iBin) > 0 && nominal->GetBinContent(jBin) > 0){
                    cov_var->SetBinContent(iBin, jBin, cov_var->GetBinContent(iBin,jBin)/(double(nThrows)*nominal->GetBinContent(iBin)*nominal->GetBinContent(jBin)));
                }

                if(jBin == iBin){
                    mc_sys_error->SetBinContent(iBin, cov_var->GetBinContent(iBin,jBin));
                    cov_var->SetBinContent(iBin, jBin, cov_var->GetBinContent(iBin,jBin) + mc_stat_error_redu->GetBinContent(iBin));
                }

                cov->SetBinContent(iBin, jBin, cov_var->GetBinContent(iBin,jBin));
                if(Apply1p1h) matrix(iBin-1,jBin-1) = cov->GetBinContent(iBin,jBin) + fake_error_cov->GetBinContent(iBin,jBin);
                else          matrix(iBin-1,jBin-1) = cov->GetBinContent(iBin,jBin);
                cov_nofake->SetBinContent(iBin, jBin, cov->GetBinContent(iBin,jBin));
            }
            mc_stat_error_redu->SetBinContent(iBin, sqrt(mc_stat_error_redu->GetBinContent(iBin)));
        }

        if(Apply1p1h) cov->Add(&*fake_error_cov);

        for(int iBin = 1; iBin < nBinsRedu+1; ++iBin){
            for(int jBin = 1; jBin < nBinsRedu+1; ++jBin){
                if(cov->GetBinContent(iBin,iBin) > 0 && cov->GetBinContent(jBin,jBin) > 0)
                    corr->SetBinContent(iBin, jBin, cov->GetBinContent(iBin,jBin) / sqrt(cov->GetBinContent(iBin,iBin) * cov->GetBinContent(jBin,jBin)));
            }
        }

        OutputFile->cd();

        vector.Write("Mean_Value");
        matrix.Write("Covariance_Matrix");
        std::string CurSampleStr = "";
        for(int iBin = 1; iBin < nBinsRedu+1; ++iBin){
            std::string SampleStr = ConvertSample(bd.GetSampleFromIndex_Det(iBin));
            if(SampleStr != CurSampleStr){
                cov       ->GetXaxis()->SetBinLabel(iBin,SampleStr.c_str());
                corr      ->GetXaxis()->SetBinLabel(iBin,SampleStr.c_str());
                cov_zero  ->GetXaxis()->SetBinLabel(iBin,SampleStr.c_str());
                cov_nofake->GetXaxis()->SetBinLabel(iBin,SampleStr.c_str());
                cov       ->GetYaxis()->SetBinLabel(iBin,SampleStr.c_str());
                corr      ->GetYaxis()->SetBinLabel(iBin,SampleStr.c_str());
                cov_zero  ->GetYaxis()->SetBinLabel(iBin,SampleStr.c_str());
                cov_nofake->GetYaxis()->SetBinLabel(iBin,SampleStr.c_str());
                fake_error_redu    ->GetXaxis()->SetBinLabel(iBin,SampleStr.c_str());
                nominal            ->GetXaxis()->SetBinLabel(iBin,SampleStr.c_str());
                mc_sys_error       ->GetXaxis()->SetBinLabel(iBin,SampleStr.c_str());
                mc_stat_error_redu ->GetXaxis()->SetBinLabel(iBin,SampleStr.c_str());
                var_mean           ->GetXaxis()->SetBinLabel(iBin,SampleStr.c_str());
                zero_point         ->GetXaxis()->SetBinLabel(iBin,SampleStr.c_str());
                var_mean_offset    ->GetXaxis()->SetBinLabel(iBin,SampleStr.c_str());
                number_events_redu ->GetXaxis()->SetBinLabel(iBin,SampleStr.c_str());
                mc_stat_nom        ->GetXaxis()->SetBinLabel(iBin,SampleStr.c_str());
                CurSampleStr = SampleStr;
            }
        }

        for(int iBin = 1; iBin < Selection->GetXaxis()->GetNbins()+1; ++iBin){
            std::string SampleStr = SampleId::ConvertSample((SampleId::SampleEnum)(iBin-1));
            Selection->GetXaxis()->SetBinLabel(iBin, SampleStr.c_str());
        }

        Selection          ->Write();
        corr               ->Write();
        cov                ->Write();
        cov_var            ->Write();
        cov_zero           ->Write();
        cov_nofake         ->Write();
        nominal            ->Write();
        mc_sys_error       ->Write();
        mc_stat_error_redu ->Write(); 
        var_mean           ->Write();
        zero_point         ->Write();
        var_mean_offset    ->Write();
        number_events_redu ->Write();
        mc_stat_error_full ->Write();
        mc_stat_nom        ->Write();
        number_events_full ->Write();
        Selection          ->Write();



        const UInt_t Number_p = 5;
        Double_t Red_p[Number_p]    = { 1.00, 0.70, 0.50, 0.30, 0.100 };
        Double_t Green_p[Number_p]  = { 1.00, 0.00, 0.00, 0.00, 0.00  };
        Double_t Blue_p[Number_p]   = { 1.00, 0.70, 0.50, 0.30, 0.100 };
        Double_t Length_p[Number_p] = { 0.00, 0.10, 0.4,  0.8,  1.00  };
        Int_t nb_p = 400;
        TColor::CreateGradientColorTable(Number_p,Length_p,Red_p,Green_p,Blue_p,nb_p);  
        const Int_t NCont = 255;
        gStyle->SetNumberContours(NCont);
        gStyle->SetOptStat(0);

        TCanvas c1;
        cov->Draw("COLZ");
        c1.SetTitle("Covariance");
        c1.SetName ("canCovariance");
        c1.Write();
        cov_zero->Draw("COLZ");
        c1.SetTitle("CovarianceZero");
        c1.SetName ("canCovarianceZero");
        c1.Write();
        cov_nofake->Draw("COLZ");
        c1.SetTitle("CovarianceNoFake");
        c1.SetName ("canCovarianceNoFake");
        c1.Write();

        const Int_t NRGBs = 3;
        Double_t stops[NRGBs] = { 0.00, 0.5, 1.00 };
        Double_t red[NRGBs]   = { 0.00, 1.0, 1.00 };
        Double_t green[NRGBs] = { 0.00, 1.0, 0.00 };
        Double_t blue[NRGBs]  = { 1.00, 1.0, 0.00 };
        TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
        gStyle->SetPalette(1,0);  // use the nice blue->white->red palette

        corr->SetMaximum( 1);
        corr->SetMinimum(-1);
        corr->Draw("COLZ");
        c1.SetTitle("Correlation");
        c1.SetName ("canCorrelation");
        c1.Write();

        TDirectory* VarU = OutputFile->mkdir("VariedUniverse");
        VarU->cd();
        for (int i = 0; i < nThrows; ++i){ varied[i]->Write(); }

        TCanvas c2;
        OutputFile->cd();
        TDirectory* BinVar = OutputFile->mkdir("BinVariations");
        BinVar->cd();

        for(int iBin = 1; iBin < nBinsRedu+1; ++iBin){
            c2.SetName(Form("Bin_%d_NEvent_Spread",iBin));
            c2.SetTitle(Form("Bin_%d_NEvent_Spread",iBin));
            int nevent = int(nominal->GetBinContent(iBin));
            if(nevent == 0) nevent = 10;
            TH1F h1(Form("Bin_%d_NEvent_Spread_Hist",iBin), Form("Bin %d NEvent Spread",iBin), int(nominal->GetBinContent(iBin)*120), 0, int(nominal->GetBinContent(iBin)*3));

            TF1 f1("f1","gaus",nominal->GetBinContent(iBin)*0.5, nominal->GetBinContent(iBin)*1.5);
            TF1 f2("f2","gaus",nominal->GetBinContent(iBin)*0.5, nominal->GetBinContent(iBin)*1.5);

            f1.SetNameTitle(Form("Bin_%d_NEvent_Varying_Gaussian",iBin), "Varying Gaussian");
            f2.SetNameTitle(Form("Bin_%d_NEvent_Total_Gaussian",iBin), "Total Gaussian");

            for(int iToy = 0; iToy < nThrows; ++iToy){
                h1.Fill(varied[iToy]->GetBinContent(iBin));
            }

            h1.GetXaxis()->SetTitle("N Events selected");
            h1.SetStats(kFALSE);
            h1.Draw();

            f1.SetParameters((h1.Integral()/4.0)/(sqrt(TMath::Pi()*2)*sqrt(cov_var->GetBinContent(iBin,iBin))*nominal->GetBinContent(iBin)), var_mean->GetBinContent(iBin), sqrt(cov_var->GetBinContent(iBin,iBin))*nominal->GetBinContent(iBin));
            f2.SetParameters((h1.Integral()/4.0)/(sqrt(TMath::Pi()*2)*sqrt(cov->GetBinContent(iBin,iBin))*nominal->GetBinContent(iBin)), var_mean->GetBinContent(iBin), sqrt(cov->GetBinContent(iBin,iBin))*nominal->GetBinContent(iBin));

            h1.GetYaxis()->SetRangeUser(0,h1.GetMaximum()*1.1);
            h1.GetXaxis()->SetRange(h1.FindFirstBinAbove(0)-5,h1.FindLastBinAbove(0)+5);

            f1.FixParameter(0,(h1.Integral()/4.0)/(sqrt(TMath::Pi()*2)*sqrt(cov_var->GetBinContent(iBin,iBin))*nominal->GetBinContent(iBin)));
            f1.FixParameter(1,var_mean->GetBinContent(iBin));
            f1.FixParameter(2,sqrt(cov_var->GetBinContent(iBin,iBin))*nominal->GetBinContent(iBin));

            h1.Fit(&f1,"BNQ");
            double chi2 = f1.GetChisquare();

            GOF->SetBinContent(iBin,chi2);

            TLine l1(nominal->GetBinContent(iBin),0,nominal->GetBinContent(iBin),h1.GetMaximum());
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
        GOF->Write();

        OutputFile->cd();
        TDirectory* AxisCovDir = OutputFile->mkdir("AxisForCov");
        OutputFile->cd();
        TDirectory* AxisFitDir = OutputFile->mkdir("AxisForFit");

        for(int i = SampleId::kUnassigned; i < SampleId::kNSamples; i++){
            SampleId::SampleEnum smpl = static_cast<SampleId::SampleEnum>(i);

            if(bd.IsActiveSample(smpl)){
                std::string name = SampleId::ConvertSample(smpl);
                std::replace (name.begin(), name.end(), ' ', '_');
                AxisCovDir->cd();
                bd.GetBinning_Mom_Det(smpl)->Write((name+"_p_axis").c_str());
                bd.GetBinning_Cos_Det(smpl)->Write((name+"_th_axis").c_str());
                AxisFitDir->cd();
                bd.GetBinning_Mom(smpl)->Write((name+"_p_axis").c_str());
                bd.GetBinning_Cos(smpl)->Write((name+"_th_axis").c_str());
            }

        }

        OutputFile->Close();

    }
}


void MakeCovariance4Arg(char* MCFilesListStr, char* NIWGFilesListStr,
        char* Cov1P1HStr,     char* OutFileStr){
    MakeCovariance(MCFilesListStr, NIWGFilesListStr, Cov1P1HStr, OutFileStr);
}

void MakeCovariance3Arg(char* MCFilesListStr, char* NIWGFilesListStr, char* OutFileStr){
    MakeCovariance(MCFilesListStr, NIWGFilesListStr, "", OutFileStr);
}

void MakeCovariance2Arg(char* MCFilesListStr, char* OutFileStr){
    MakeCovariance(MCFilesListStr, "", "", OutFileStr);
}

int main(int argn, char** arg){

    switch (argn){
        case 5:{
                   ApplyNIWG = true;
                   Apply1p1h = true;
                   MakeCovariance4Arg(arg[1], arg[2], arg[3], arg[4]);
                   break;
               }
        case 4:{
                   ApplyNIWG = true;
                   Apply1p1h = false;
                   MakeCovariance3Arg(arg[1], arg[2], arg[3]);
                   break;
               }
        case 3:{
                   ApplyNIWG = false;
                   Apply1p1h = false;
                   MakeCovariance2Arg(arg[1], arg[2]);
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


