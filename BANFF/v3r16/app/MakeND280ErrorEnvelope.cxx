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
#include "THn.h"

#include "BinningDefinition.hxx"
#include "ND280AnalysisUtils.hxx"
#include "Parameters.hxx"

bool ApplyNIWG    = false;
const int nThrows = 2000;

void MakeErrorEnvelope(char* MCFilesListStr, char* NIWGFilesListStr, char* OutFileStr) {

    ND::params().SetReadParamOverrideFilePointPassed();
    BANFF::BinningDefinition& bd = BANFF::BinningDefinition::Get();

    std::vector<std::string> name_vector     = bd.GetListOfBins();
    std::vector<std::string> name_vector_det = bd.GetListOfBins_Det();

    //for(int i = 0; i < (int)name_vector.size(); i++)
    //  std::cout << name_vector[i] << std::endl;

    //for(int i = 0; i < (int)name_vector_det.size(); i++)
    //  std::cout << name_vector_det[i] << std::endl;

    ifstream MCFilesList;
    MCFilesList.open(MCFilesListStr);

    std::map<std::string, TChain*> MCChains;

    if (MCFilesList.is_open()) {
        std::string line;
        while (getline(MCFilesList, line)) {
            if (MCChains.size() == 0) {
                std::cout << "Reading list of systematics trees from file" << std::endl;
                TFile temp(line.c_str(), "READ");

                TIter next(temp.GetListOfKeys());
                TKey *key;
                while ((key = (TKey*)next())) {
                    TClass *cl = gROOT->GetClass(key->GetClassName());
                    if (!cl->InheritsFrom("TTree")) continue;
                    TTree *h = (TTree*)key->ReadObj();
                    std::string name = h->GetName();
                    // Skip the NRooTrackerVtx, flattree, header, and config chains
                    if (name.find("RooTrackerVtx")!=std::string::npos) { continue; }
                    if (name.find("flat")!=std::string::npos) { continue; }
                    if (name.find("head")!=std::string::npos) { continue; }
                    if (name.find("conf")!=std::string::npos) { continue; }
                    // Now that we are only using the all tree
                    std::cout << "Adding syst " << name << " to TChain map" << std::endl;
                    MCChains.insert(std::pair<std::string, TChain*>(name, new TChain(name.c_str())));
                } 
            }
            //std::cout << "Adding the file " << line << " to the TChain" << std::endl;
            for (std::map<std::string, TChain*>::iterator it = MCChains.begin(); it != MCChains.end(); ++it) {
                it->second->Add(line.c_str());
            }
        }
        MCFilesList.close();
    }
    else {
        std::cerr << "List of MC files not open!!" << std::endl;
        throw;
    }

    ifstream NIWGFilesList;
    NIWGFilesList.open(NIWGFilesListStr);
    TChain* NIWGTune = NULL;

    if (ApplyNIWG) {
        if (NIWGFilesList.is_open()) {
            NIWGTune = new TChain("NIWGWeightTree");
            std::string line;
            while (getline(NIWGFilesList, line)) {
                //std::cout << "NIWG tune File " << line << std::endl;
                NIWGTune->Add(line.c_str());
            }
            NIWGFilesList.close();
        }
        else {
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
    double FluxWeightToy  [nThrows];
    double NIWGWeightToy  [nThrows];

    TChain* MCChain;

    for (std::map<std::string, TChain*>::iterator it = MCChains.begin(); it != MCChains.end(); ++it) {

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
        if (ApplyNIWG) {
            std::cout << "Saving NIWG Check histograms" <<std::endl;
            NIWGTune->SetBranchAddress("T2KRW_NIWGWeightNom", &NIWGWeightNom);
            NIWGTune->SetBranchAddress("T2KRW_NIWGWeightToy",  NIWGWeightToy);
            MCChain->AddFriend(NIWGTune);

            //TH2D* check1 = new TH2D("EnuCheck", "EnuCheck;Enu;Enu", 1000, 0, 5000, 1000, 0, 5000);
            //MCChain->Project("EnuCheck", "TrueEnuToy:T2KRW_EnuToy");

            //TH2D* check2 = new TH2D("EnuCheck2", "EnuCheck2;Enu;Enu", 1000, 0, 5000, 1000, 0, 5000);
            //MCChain->Project("EnuCheck2", "TrueEnuNom:T2KRW_EnuNom");

            //TCanvas* c = new TCanvas();
            //check1->Draw();
            //c->SaveAs("check1.png");

            //TCanvas* b = new TCanvas();
            //check2->Draw();
            //b->SaveAs("check2.png");
        }
        else{
            std::cout << "WARNING!! NOT APPLYING THE NIWG WEIGHTING" << std::endl;
            for(int i = 0;i < nThrows; i++) NIWGWeightToy[i] = 1;
            NIWGWeightNom = 1.;
        }

        std::cout << "Creating the file " << OutFileStr << "_" << (it->first).c_str() << ".root" << std::endl;
        TFile* OutputFile = new TFile(Form("%s_%s.root",OutFileStr,(it->first).c_str()), "RECREATE");

        int nBinsRedu = bd.GetNbins_Det();
        int nBinsFull = bd.GetNbins();

        std::cout << "We have " << nBinsRedu <<  " bins in the detector covariance matrix" << std::endl;

        TMatrixTSym<double> matrix(nBinsFull);
        TVectorT   <double> vector(nBinsFull);
        TH2D* var_cov            = new TH2D("Varied_Covariance_Matrix",  "Varied Covariance Matrix",                              nBinsFull, 0, nBinsFull, nBinsFull, 0, nBinsFull);
        TH2D* var_corr           = new TH2D("Varied_Correlation_Matrix", "Varied Correlation Matrix",                             nBinsFull, 0, nBinsFull, nBinsFull, 0, nBinsFull);
        TH1D* var_mean           = new TH1D("Varied_Mean",               "Varied Mean",                                           nBinsFull, 0, nBinsFull);
        TH1D* nominal            = new TH1D("Nominal",                   "Nominal Events",                                        nBinsFull, 0, nBinsFull);
        TH1D* number_events_full = new TH1D("number_events",             "Number of Events",                                      nBinsFull, 0, nBinsFull);
        TH1D* var_mean_offset    = new TH1D("Varied_Mean_over_Nominal",  "Varied Mean normalised by nominal",                     nBinsFull, 0, nBinsFull);
        TH1D* mc_sys_error       = new TH1D("MC_Sys_Error",              "MC systematic error",                                   nBinsFull, 0, nBinsFull);
        TH1D* mc_stat_error_full = new TH1D("MC_Stat_Error_full",        "MC statistical error",                                  nBinsFull, 0, nBinsFull);
        TH1D* mc_stat_nom        = new TH1D("MC_Stat_Nom",               "MC statistical nominal",                                nBinsFull, 0, nBinsFull);
        TH1D* error_band         = new TH1D("Error_Band",                "Detector Error Band Around Varied Events",              nBinsFull, 0, nBinsFull);
        TH1D* bigger_error_band  = new TH1D("Bigger_Error_Band",         "Detector plus MC StatsError Band Around Varied Events", nBinsFull, 0, nBinsFull);
        TH1D* varied[nThrows];
        for (int i = 0; i < nThrows; ++i){
            varied[i] = new TH1D(Form("Varied_Events_%d", i), Form("Varied Events %d", i), nBinsFull, 0, nBinsFull);
        }

        std::cout << "Number of samples: " << SampleId::kNSamples <<std::endl;
        TH1D* Selection = new TH1D("Selection", "Selection;SampleId;N events", SampleId::kNSamples, 0, SampleId::kNSamples);

        std::cout << "Setting values to 0 for initializing" <<std::endl;
        for (int iBin = 1; iBin < nBinsFull+1; ++iBin) {
            for (int jBin = 1; jBin < nBinsFull+1; ++jBin) {
                var_cov   ->SetBinContent(iBin, jBin, 0);
            }
            nominal           ->SetBinContent(iBin, 0);
            var_mean          ->SetBinContent(iBin, 0);
            var_mean_offset   ->SetBinContent(iBin, 0);
            mc_stat_error_full->SetBinContent(iBin, 0);
            mc_sys_error      ->SetBinContent(iBin, 0);

            for (int iToy = 0; iToy < nThrows; ++iToy) {
                varied[iToy]->SetBinContent(iBin, 0);
            }
        }
        std::cout << "Entries from TChain: " << MCChain->GetEntries() <<std::endl;

        double POT_Weight = 1;

        std::cout << "Looping through entries" <<std::endl;
        for (int i = 0; i < (int)MCChain->GetEntries(); ++i) {
            MCChain->GetEntry(i);
            if (nThrows != nToys) {
                std::cerr << "nThrows should be equal to nToys, change this in the beginning MakeND280Cov and recompile" << std::endl;
                throw;
            }
            if (i%10000 == 0) {
                std::cout << "Progress " << 100. * ((double) i) / MCChain->GetEntries() << "%" << std::endl;
            }

            Int_t runp = anaUtils::GetRunPeriod(Run, SubRun);

            bool isSand = false;
            if(anaUtils::GetSandMode(Run) >= 0) { isSand = true; }

            switch (runp){
                case 0://run1 water
                    POT_Weight = 0.018057359;
                    break;
                case 1://run2 water
                    POT_Weight = 4.33765e+19/1.20341e+21;
                    if(isSand) POT_Weight = 4.33765e+19/4.00035e+19;
                    break;
                case 2://run2 air
                    POT_Weight = 3.59337e+19/9.23937e+20;
                    if(isSand) POT_Weight = 3.59337e+19/3.7132e+19;
                    break;       
                case 3://run3b air
                    POT_Weight = 2.1705e+19/4.44873e+20;
                    if(isSand) POT_Weight = 2.1705e+19/2.35053e+19;
                    break;       
                case 4://run3c air
                    POT_Weight = 1.36398e+20/2.63027e+21;
                    if(isSand) POT_Weight = 1.36398e+20/1.31337e+20;
                    break;
                case 5://run4 water
                    POT_Weight = 1.64277e+20/2.26216e+21;	
                    if(isSand) POT_Weight = 1.64277e+20/1.59801e+20;
                    break;
                case 6://run4 air
                    POT_Weight = 1.78271e+20/3.4996e+21;
                    if(isSand) POT_Weight = 1.78271e+20/1.74125e+20;
                    break;
                case 8://run5 antinu-water
                    POT_Weight = 4.3468e+19/2.29627e+21;
                    if(isSand) POT_Weight = 4.3468e+19/9.07403e+19;
                    break;
                case 9://run6b antinu-air
                     POT_Weight = 1.27301e+20/1.4174e+21;
                    if(isSand) POT_Weight = 1.27301e+20/3.41655e+20;
                    break;
                case 10://run6c antinu-air
                     POT_Weight = 5.07819e+19/5.27562e+20;
                    if(isSand) POT_Weight = 5.07819e+19/1.04626e+20;
                    break;
                case 11://run6d antinu-air
                     POT_Weight = 7.75302e+19/6.883e+20;
                    if(isSand) POT_Weight = 7.75302e+19/1.58059e+20;
                    break;
                case 12://run6e antinu-air
                    POT_Weight = 8.51429e+19/8.59439e+20;
                    if(isSand) POT_Weight = 8.51429e+19/1.75435e+20;
                    break;
                case 15://run7b antinu-water
                    POT_Weight = 2.43683e+20/3.37059e+21;
                    if(isSand) POT_Weight = 2.43683e+20/5.03961e+20;
                    break;
                case 17://run8 nu-water
                    POT_Weight = 1.58053e+20/2.64115e+21;
                    if(isSand) POT_Weight = 1.58053e+20/1.61263e+20;
                    break;
                case 18://run8 nu-air
                    POT_Weight = 4.14909e+20/3.63054e+21;
                    if(isSand) POT_Weight = 4.14909e+20/4.04241e+20;
                    break;
                default:
                    std::cout << "NOT FOUND?!?!" << std::endl;
                    throw;
            }
            if (isSand) {
                for (int i = 0;i < nThrows; i++) {
                    NIWGWeightToy[i] = 1;
                }
                NIWGWeightNom = 1.;
            }

            if (POT_Weight > 2) {
                std::cout << "BAD POT WEIGHT" << std::endl;
                exit(1);
            }

            if (bd.IsActiveSample(SelectionNom)) {
                Selection->Fill(SelectionNom);
                int BinFull = bd.GetGlobalBinMatrixMomCos((SampleId::SampleEnum)SelectionNom, LeptonMomNom, LeptonCosNom) + 1;
                if(BinFull > nBinsFull){
                    std::cout << "BinFull > nBinsFull" << std::endl;
                    throw;
                }
                if(TMath::IsNaN(WeightNom    ) || !TMath::Finite(WeightNom    ) || WeightNom      == -999 || WeightNom      > 1000) {
                    std::cerr << "WeightNom isn't finite: " << WeightNom << std::endl;
                    throw;
                }
                if(TMath::IsNaN(NIWGWeightNom) || !TMath::Finite(NIWGWeightNom) || NIWGWeightNom  == -999 || NIWGWeightNom  > 1000) {
                    std::cerr << "NIWGWeightNom isn't finite: " << NIWGWeightNom << std::endl;
                    throw;
                }
                if(TMath::IsNaN(FluxWeightNom) || !TMath::Finite(FluxWeightNom) || FluxWeightNom  == -999 || FluxWeightNom  > 1000) {
                    std::cerr << "FluxWeightNom isn't finite: " << FluxWeightNom << std::endl;
                    throw;
                }
                if(TMath::IsNaN(POT_Weight   ) || !TMath::Finite(POT_Weight   ) || POT_Weight     == -999 || POT_Weight     > 1000) {
                    std::cerr << "POT_Weight isn't finite: " << POT_Weight << std::endl;
                    throw;
                }

                number_events_full->Fill(BinFull);

                nominal           ->SetBinContent(BinFull, nominal           ->GetBinContent(BinFull) + WeightNom * FluxWeightNom * NIWGWeightNom * POT_Weight);
                mc_stat_nom       ->SetBinContent(BinFull, mc_stat_nom       ->GetBinContent(BinFull) + WeightNom * FluxWeightNom * NIWGWeightNom * POT_Weight);
                mc_stat_error_full->SetBinContent(BinFull, mc_stat_error_full->GetBinContent(BinFull) + TMath::Power(WeightNom * FluxWeightNom * NIWGWeightNom * POT_Weight, 2.));
            }

            for (int iToy = 0; iToy < nThrows; ++iToy) {
                if (bd.IsActiveSample(SelectionToy[iToy])) {
                    int BinFull = bd.GetGlobalBinMatrixMomCos((SampleId::SampleEnum)SelectionToy[iToy], LeptonMomToy[iToy], LeptonCosToy[iToy]) + 1;
                    if(BinFull > nBinsFull){
                        std::cout << "BinFull > nBinsFull" << std::endl;
                        throw;
                    }
                    if(!TMath::IsNaN(WeightToy[iToy]) && TMath::Finite(WeightToy[iToy])){

                        std::cout << "Bin: " << BinFull << " POT_Weight: " << POT_Weight << " FluxWeightToy[" << iToy << "]: " << FluxWeightToy[iToy] <<
                                     " NIWGWeightToy[" << iToy << "]: " << NIWGWeightToy[iToy] << 
                                     " WeightToy[" << iToy << "]: " << WeightToy[iToy] << std::endl;

                        if(WeightToy[iToy] < 100){
                            varied[iToy]->SetBinContent(BinFull, varied[iToy]->GetBinContent(BinFull) + POT_Weight * FluxWeightToy[iToy] * NIWGWeightToy[iToy] * WeightToy[iToy]);
                            if(TMath::IsNaN(WeightToy[iToy]    ) || !TMath::Finite(WeightToy[iToy]    ) || WeightToy[iToy]      == -999 || WeightNom      > 1000) {
                                std::cerr << "WeightToy["     << iToy << "] isn't finite: " << WeightToy[iToy]     << std::endl;
                                throw;
                            }
                            if(TMath::IsNaN(NIWGWeightToy[iToy]) || !TMath::Finite(NIWGWeightToy[iToy]) || NIWGWeightToy[iToy]  == -999 || NIWGWeightNom  > 1000) {
                                std::cerr << "NIWGWeightToy[" << iToy << "] isn't finite: " << NIWGWeightToy[iToy] << std::endl;
                                throw;
                            }
                            if(TMath::IsNaN(FluxWeightToy[iToy]) || !TMath::Finite(FluxWeightToy[iToy]) || FluxWeightToy[iToy]  == -999 || FluxWeightNom  > 1000) {
                                std::cerr << "FluxWeightToy[" << iToy << "] isn't finite: " << FluxWeightToy[iToy] << std::endl;
                                throw;
                            }
                            if(TMath::IsNaN(POT_Weight         ) || !TMath::Finite(POT_Weight         ) || POT_Weight           == -999 || POT_Weight     > 1000) {
                                std::cerr << "POT_Weight isn't finite: "                    << POT_Weight          << std::endl;
                                throw;
                            }
                        }
                        else{
                            std::cout << "Bin: " << BinFull << " POT_Weight: " << POT_Weight << " FluxWeightToy[" << iToy << "]: " << FluxWeightToy[iToy] <<
                                     " NIWGWeightToy[" << iToy << "]: " << NIWGWeightToy[iToy] << std::endl;

                            varied[iToy]->SetBinContent(BinFull, varied[iToy]->GetBinContent(BinFull) + FluxWeightToy[iToy] * NIWGWeightToy[iToy] * POT_Weight);
                        }
                    }
                }
            }
        }

        for (int iToy = 0; iToy < nThrows; ++iToy) {
            for (int iBin = 1; iBin < nBinsFull+1; ++iBin) {
                var_mean->SetBinContent(iBin, var_mean->GetBinContent(iBin) + varied[iToy]->GetBinContent(iBin) / double(nThrows));
            }
        }

        for (int iToy = 0; iToy < nThrows; ++iToy) {
            for (int iBin = 1; iBin < nBinsFull+1; ++iBin) {
                for (int jBin = 1; jBin < nBinsFull+1; ++jBin) {
                    var_cov->SetBinContent(iBin, jBin, var_cov->GetBinContent(iBin,jBin) + ((varied[iToy]->GetBinContent(iBin) - var_mean->GetBinContent(iBin)) *
                                                                                            (varied[iToy]->GetBinContent(jBin) - var_mean->GetBinContent(jBin))));
                }
            }
        }

        for (int iBin = 1; iBin < nBinsFull+1; ++iBin) {
            if (mc_stat_nom->GetBinContent(iBin)>0) {
                mc_stat_error_full->SetBinContent(iBin, mc_stat_error_full->GetBinContent(iBin) / (mc_stat_nom->GetBinContent(iBin) * mc_stat_nom->GetBinContent(iBin)));
            }
        }

        for (int j = 0; j < nBinsFull; ++j) {
            SampleId::SampleEnum sample1 = bd.GetSampleFromIndex(j);
            int localbin = j - bd.GetGlobalBinMatrix(sample1);
            double p    = bd.GetBinning_Mom(sample1)->GetBinUpEdge(int(localbin % bd.GetNbins_Mom(sample1))+1);
            double c    = bd.GetBinning_Cos(sample1)->GetBinUpEdge(int(localbin / bd.GetNbins_Mom(sample1))+1);
            double plow = bd.GetBinning_Mom(sample1)->GetBinLowEdge(int(localbin % bd.GetNbins_Mom(sample1))+1);
            double clow = bd.GetBinning_Cos(sample1)->GetBinLowEdge(int(localbin / bd.GetNbins_Mom(sample1))+1);

            //if(sample1 == 3){
            //    std::cout << "p = " << p << " plow = " << plow << " c = " << c << " clow = " << clow << " sample = " << sample1 << std::endl;
            //}
            for (int k = bd.GetGlobalBinMatrix(sample1); k < bd.GetGlobalBinMatrix(sample1) + bd.GetNbins(sample1); ++k) {

                localbin = k - bd.GetGlobalBinMatrix(sample1);

                // If the fit bin does not overlap at all with the systematics obsnorm bin then skip it.
                if (bd.GetBinning_Mom(sample1)->GetBinUpEdge(int(localbin%bd.GetNbins_Mom(sample1))+1) <= plow ||
                    bd.GetBinning_Cos(sample1)->GetBinUpEdge(int(localbin/bd.GetNbins_Mom(sample1))+1) <= clow     ||
                    bd.GetBinning_Mom(sample1)->GetBinLowEdge(int(localbin%bd.GetNbins_Mom(sample1))+1) >= p       ||
                    bd.GetBinning_Cos(sample1)->GetBinLowEdge(int(localbin/bd.GetNbins_Mom(sample1))+1) >= c) {
                    continue;
                }

                //if(sample1==3){
                //    std::cout << " plow+det = "  << bd.GetBinning_Mom(sample1)->GetBinLowEdge(int(localbin%bd.GetNbins_Mom(sample1))+1) << std::endl;
                //    std::cout << " p+det = "     << bd.GetBinning_Mom(sample1)->GetBinUpEdge(int(localbin%bd.GetNbins_Mom(sample1))+1)  << std::endl;
                //    std::cout << " clow+det = "  << bd.GetBinning_Cos(sample1)->GetBinLowEdge(int(localbin/bd.GetNbins_Mom(sample1))+1) << std::endl;
                //    std::cout << " c+det = "     << bd.GetBinning_Cos(sample1)->GetBinUpEdge(int(localbin/bd.GetNbins_Mom(sample1))+1)  << std::endl;
                //    std::cout << " local bin = " << localbin << std::endl;
                //    std::cout << " Fit Bin = "   << k << std::endl;
                //}

                break;
            }
        }

        for (int iBin = 1; iBin <= nBinsFull; ++iBin) {
            if (nominal->GetBinContent(iBin) > 0) {
                vector(iBin-1) = var_mean->GetBinContent(iBin) / nominal->GetBinContent(iBin);
                var_mean_offset->SetBinContent(iBin, var_mean->GetBinContent(iBin) / nominal->GetBinContent(iBin));
            }
            else {
                vector(iBin-1) = 1.0;
                var_mean_offset->SetBinContent(iBin, 1);
            }

            for (int jBin = 1; jBin <= nBinsFull; ++jBin) {
                if (fabs(var_cov->GetBinContent(iBin,jBin)) > 0 && var_mean->GetBinContent(iBin) > 0 && var_mean->GetBinContent(jBin) > 0) {
                    var_cov->SetBinContent(iBin, jBin, var_cov->GetBinContent(iBin,jBin) / (double(nThrows) * var_mean->GetBinContent(iBin) * var_mean->GetBinContent(jBin)));
                }

                if(jBin == iBin){
                    mc_sys_error->SetBinContent(iBin, var_cov->GetBinContent(iBin,jBin));
                }

                matrix(iBin-1,jBin-1) = var_cov->GetBinContent(iBin,jBin);
            }
        }

        for (int iBin = 1; iBin <= nBinsFull; ++iBin) {
            for (int jBin = 1; jBin <= nBinsFull; ++jBin) {
                if (var_cov->GetBinContent(iBin,iBin) > 0 && var_cov->GetBinContent(jBin,jBin) > 0) {
                    var_corr->SetBinContent(iBin, jBin, var_cov->GetBinContent(iBin,jBin) / sqrt(var_cov->GetBinContent(iBin,iBin) * var_cov->GetBinContent(jBin,jBin)));
                }
            }
        }

        OutputFile->cd();

        vector.Write("Mean_Value");
        matrix.Write("Covariance_Matrix");
        std::string CurSampleStr = "";
        for (int iBin = 1; iBin < nBinsFull+1; ++iBin) {
            std::string SampleStr = ConvertSample(bd.GetSampleFromIndex(iBin));
            if (SampleStr != CurSampleStr) {
                nominal           ->GetXaxis()->SetBinLabel(iBin, SampleStr.c_str());
                mc_sys_error      ->GetXaxis()->SetBinLabel(iBin, SampleStr.c_str());
                mc_stat_error_full->GetXaxis()->SetBinLabel(iBin, SampleStr.c_str());
                var_mean          ->GetXaxis()->SetBinLabel(iBin, SampleStr.c_str());
                var_mean_offset   ->GetXaxis()->SetBinLabel(iBin, SampleStr.c_str());
                number_events_full->GetXaxis()->SetBinLabel(iBin, SampleStr.c_str());
                mc_stat_nom       ->GetXaxis()->SetBinLabel(iBin, SampleStr.c_str());
                CurSampleStr = SampleStr;
            }
        }

        for (int iBin = 1; iBin < Selection->GetXaxis()->GetNbins()+1; ++iBin) {
            std::string SampleStr = SampleId::ConvertSample((SampleId::SampleEnum)(iBin-1));
            Selection->GetXaxis()->SetBinLabel(iBin, SampleStr.c_str());
        }

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

        const Int_t NRGBs = 3;
        Double_t stops[NRGBs] = { 0.00, 0.5, 1.00 };
        Double_t red[NRGBs]   = { 0.00, 1.0, 1.00 };
        Double_t green[NRGBs] = { 0.00, 1.0, 0.00 };
        Double_t blue[NRGBs]  = { 1.00, 1.0, 0.00 };
        TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
        gStyle->SetPalette(1,0);  // use the nice blue->white->red palette

        TDirectory* VarU = OutputFile->mkdir("VariedUniverse");
        VarU->cd();
        for (int i = 0; i < nThrows; ++i) { varied[i]->Write(); }

        TCanvas c2;
        OutputFile->cd();
        TDirectory* BinVar = OutputFile->mkdir("BinVariations");
        BinVar->cd();

        for (int iBin = 1; iBin < nBinsFull+1; ++iBin) {
            c2.SetName(Form("Bin_%d_NEvent_Spread", iBin));
            c2.SetTitle(Form("Bin_%d_NEvent_Spread", iBin));
            int nevent = int(nominal->GetBinContent(iBin));
            if (nevent == 0) { nevent = 10; }
            TH1F h1(Form("Bin_%d_NEvent_Spread_Hist", iBin), Form("Bin %d NEvent Spread", iBin), nevent*120, 0, nevent*3);

            TF1 f1("f1", "gaus", nominal->GetBinContent(iBin)*0.5, nominal->GetBinContent(iBin)*1.5);

            f1.SetNameTitle(Form("Bin_%d_NEvent_Varying_Gaussian", iBin), "Varying Gaussian");

            for(int iToy = 0; iToy < nThrows; ++iToy){
                h1.Fill(varied[iToy]->GetBinContent(iBin));
            }

            h1.GetXaxis()->SetTitle("N Events selected");
            h1.SetStats(kFALSE);
            h1.Draw();

            f1.SetParameters((h1.Integral()/4.0) / (sqrt(TMath::Pi()*2)*sqrt(var_cov->GetBinContent(iBin,iBin)) * nominal->GetBinContent(iBin)),
                             var_mean->GetBinContent(iBin), sqrt(var_cov->GetBinContent(iBin,iBin)) * nominal->GetBinContent(iBin));

            h1.GetYaxis()->SetRangeUser(0,h1.GetMaximum()*1.1);
            h1.GetXaxis()->SetRange(h1.FindFirstBinAbove(0)-5, h1.FindLastBinAbove(0)+5);

            f1.FixParameter(0, (h1.Integral()/4.0) / (sqrt(TMath::Pi()*2) * sqrt(var_cov->GetBinContent(iBin,iBin)) * nominal->GetBinContent(iBin)));
            f1.FixParameter(1, var_mean->GetBinContent(iBin));
            f1.FixParameter(2, sqrt(var_cov->GetBinContent(iBin,iBin)) * nominal->GetBinContent(iBin));

            //h1.Fit(&f1, "BNQ");
            h1.Fit("gaus");
            TF1 *fit_results = h1.GetFunction("gaus");

            double mean  = fit_results->GetParameter(1);
            double width = fit_results->GetParameter(2);
            std::cout << "Mean: " << mean << " Width: " << width <<std::endl;
            error_band->SetBinContent(iBin, mean);
            error_band->SetBinError(iBin, width);
            // MC Stats error values are currently variances, so does not need to be squared
            bigger_error_band->SetBinContent(iBin, mean);
            bigger_error_band->SetBinError(iBin, sqrt(width*width + mc_stat_error_full->GetBinContent(iBin)));

            TLine l1(nominal->GetBinContent(iBin), 0, nominal->GetBinContent(iBin) , h1.GetMaximum());

            f1.SetLineWidth(2);
            f1.SetLineColor(kRed);

            //f1.Draw("SAME");

            l1.SetLineWidth(2);
            l1.SetLineStyle(2);
            l1.SetLineColor(kBlack);

            l1.Draw("SAME");

            TLegend* leg = new TLegend(0.65,0.7,0.98,0.94);
            leg->AddEntry(&h1, "Bin content from throws", "l");
            //leg->AddEntry(&f1, "Gaussian from throw cov", "l");
            leg->AddEntry(&l1, "Nominal bin content",     "l");
            leg->SetFillColor(kWhite);
            leg->Draw();
            c2.Update();
            c2.Write();
        }

        //OutputFile->cd();
        //TDirectory* AxisCovDir = OutputFile->mkdir("AxisForCov");
        OutputFile->cd();
        TDirectory* AxisFitDir = OutputFile->mkdir("AxisForFit");

        for (int i = SampleId::kUnassigned; i < SampleId::kNSamples; i++) {
            SampleId::SampleEnum smpl = static_cast<SampleId::SampleEnum>(i);

            if(bd.IsActiveSample(smpl)){
                std::string name = SampleId::ConvertSample(smpl);
                std::replace (name.begin(), name.end(), ' ', '_');
                //AxisCovDir->cd();
                //bd.GetBinning_Mom(smpl)->Write((name+"_p_axis").c_str());
                //bd.GetBinning_Cos(smpl)->Write((name+"_th_axis").c_str());
                AxisFitDir->cd();
                bd.GetBinning_Mom(smpl)->Write((name+"_p_axis").c_str());
                bd.GetBinning_Cos(smpl)->Write((name+"_th_axis").c_str());
            }

        }

        OutputFile->cd();
        Selection         ->Write();
        var_cov           ->Write();
        nominal           ->Write();
        mc_sys_error      ->Write();
        mc_stat_error_full->Write(); 
        var_mean          ->Write();
        var_mean_offset   ->Write();
        number_events_full->Write();
        // Take root of MC stats plot to convert it from variance into an error
        for(int i = 1; i <= mc_stat_error_full->GetNbinsX(); ++i){
            mc_stat_error_full->SetBinContent(i, sqrt(mc_stat_error_full->GetBinContent(i)));
        }
        mc_stat_error_full->Write();
        mc_stat_nom       ->Write();
        Selection         ->Write();
        var_corr          ->Write();
        error_band        ->Write();
        bigger_error_band ->Write();

        OutputFile->Close();
    }
}


void MakeErrorEnvelope3Arg(char* MCFilesListStr, char* NIWGFilesListStr, char* OutFileStr){
    MakeErrorEnvelope(MCFilesListStr, NIWGFilesListStr, OutFileStr);
}

void MakeErrorEnvelope2Arg(char* MCFilesListStr, char* OutFileStr){
    MakeErrorEnvelope(MCFilesListStr, "", OutFileStr);
}

int main(int argn, char** arg){

    switch (argn){
        case 4:{
                   ApplyNIWG = true;
                   MakeErrorEnvelope3Arg(arg[1], arg[2], arg[3]);
                   break;
               }
        case 3:{
                   ApplyNIWG = false;
                   MakeErrorEnvelope2Arg(arg[1], arg[2]);
                   break;
               }
        default:{
                    std::cout << "\n\n\nUsage:\nMakeND280ErrorEnvelope.exe ListOfFilesWithNDThrows ListOfFileWithNIWGWeight outfile.root" << std::endl;
                    std::cout << "or:\nMakeND280DetectorCov.exe ListOfFilesWithNDThrows outfile.root" << std::endl;
                    throw;
                }
    }

    return 0;

}


