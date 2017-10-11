#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TMatrixTSym.h"
#include "TVectorT.h"
#include <iostream>
#include "TLine.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TFitResultPtr.h"
#include "BinningDefinition.hxx"
#include "ND280AnalysisUtils.hxx"

using namespace BinningDefinition;

void MakeCovariance(char* infile, char* outfile, char* fake_data_file){


  //gROOT->SetBatch(kTRUE);

    TFile *fake = new TFile(fake_data_file);
    TH2D* fake_cov = NULL;
    if(fake)
      fake_cov = (TH2D*) fake->Get("cov_1p1h");

    TChain inChain("SelectedEventTruth");
    inChain.Add(infile);

    //TODO: Maybe move some of this onto the backend? Assemble TAxis pointers
    //inside the object?
    //Alternatively, a scrip to easily assemble files with TAxis definitions in
    //them, and the card file has a path to the TAxis files?
    
    //The binning is with similar systematic merging (2017 summer analysis)
    //The "1" binning is the fit binning (2017 summer analysis)

    int nThrows = 502;
    float* PMu        = new float[nThrows];
    float* ThetaMu    = new float[nThrows];
    float* DetWeight  = new float[nThrows];
    float* FluxWeight = new float[nThrows];
    float* NIWGWeight = new float[nThrows];
    int* Selection    = new int  [nThrows];
    int EventNumber, Run, SubRun;
    for( int i = 0; i < nThrows; ++i){
        FluxWeight[i] = 1;
        DetWeight[i]  = 1;
        NIWGWeight[i] = 1;
        PMu[i]       = -999;
        ThetaMu[i]   = -999;
        Selection[i] = 0;
    }

    inChain.SetBranchAddress("nThrows",     &nThrows);
    inChain.SetBranchAddress("PMu",          PMu);
    inChain.SetBranchAddress("ThetaMu",      ThetaMu);
    inChain.SetBranchAddress("DetWeight",    DetWeight);
    inChain.SetBranchAddress("FluxWeight",   FluxWeight);
    inChain.SetBranchAddress("NIWGWeight",   NIWGWeight);
    inChain.SetBranchAddress("Selection",    Selection);
    inChain.SetBranchAddress("EventNumber", &EventNumber);
    inChain.SetBranchAddress("Run",         &Run);
    inChain.SetBranchAddress("SubRun",      &SubRun);
  
    TFile output(outfile,"RECREATE");
    int nbins_0pi = nctbins_0pi*npbins_0pi;
    int nbins_1pi = nctbins_1pi*npbins_1pi;
    int nbins_npi = nctbins_npi*npbins_npi;
    int nbins_0pi1 = nctbins_0pi1*npbins_0pi1;
    int nbins_1pi1 = nctbins_1pi1*npbins_1pi1;
    int nbins_npi1 = nctbins_npi1*npbins_npi1;
    int nbins_anuqe = nctbins_anuqe*npbins_anuqe;
    int nbins_anunqe = nctbins_anunqe*npbins_anunqe;
    int nbins_nuqe = nctbins_nuqe*npbins_nuqe;
    int nbins_nunqe = nctbins_nunqe*npbins_nunqe;
    int nbins_anuqe1 = nctbins_anuqe1*npbins_anuqe1;
    int nbins_anunqe1 = nctbins_anunqe1*npbins_anunqe1;
    int nbins_nuqe1 = nctbins_nuqe1*npbins_nuqe1;
    int nbins_nunqe1 = nctbins_nunqe1*npbins_nunqe1;
    int nbins = nbins_0pi+nbins_1pi+nbins_npi+nbins_anuqe+nbins_anunqe+nbins_nuqe+nbins_nunqe;
    int nbins1 = nbins_0pi1+nbins_1pi1+nbins_npi1+nbins_anuqe1+nbins_anunqe1+nbins_nuqe1+nbins_nunqe1;

    nbins = nbins*2;
    nbins1 = nbins1*2;

    std::cout << "We have " << nbins <<  " bins in the covariance matrix" << std::endl;

    TMatrixTSym<double> matrix(nbins);
    TVectorT<double> vector(nbins);
    TH2D corr("Total_Correlation_Matrix","Total Correlation Matrix",nbins, 0, nbins, nbins, 0, nbins);
    TH2D cov("Total_Covariance_Matrix","Total Covariance Matrix",nbins, 0, nbins, nbins, 0, nbins);
    TH2D cov_var("Covariance_Matrix_Var","Covariance Matrix for variation",nbins, 0, nbins, nbins, 0, nbins);
    TH2D cov_zero("Covariance_Matrix_Zero","Covariance Matrix for zero point offset",nbins, 0, nbins, nbins, 0, nbins);
    TH2D cov_nofake("Covariance_Matrix_NoFake","Covariance Matrix without 1p1h error",nbins, 0, nbins, nbins, 0, nbins);
    TH2D fake_error("1p1h_fake_Error","1p1h systematic error",nbins, 0, nbins, nbins, 0, nbins);
    TH1D nominal("Nominal","Nominal Events",nbins, 0, nbins);
    TH1D mc_sys_error("MC_Sys_Error","MC systematic error",nbins, 0, nbins);
    TH1D mc_stat_error1("MC_Stat_Error1","MC statistical error",nbins1, 0, nbins1);
    TH1D mc_stat_error("MC_Stat_Error","MC statistical error",nbins, 0, nbins);
    TH1D mc_stat_nom("MC_Stat_Nom","MC statistical nominal",nbins1, 0, nbins1);
    TH1D var_mean("Varied_Mean","Varied Mean",nbins, 0, nbins);
    TH1D zero_point("Zero_Point","Zero Point",nbins, 0, nbins);
    TH1D var_mean_offset("Varied_Mean_over_Nominal","Varied Mean normalised by nominal",nbins, 0, nbins);
    TH1D* varied[502];
    TH1D number_events("number_events","Number of Events",nbins, 0, nbins);

    TH1D GOF("GOF","Chi-square between gaussian and bin variation",nbins, 0, nbins);

    for (int i = 0; i < nThrows; ++i){
        varied[i] = new TH1D(Form("Varied_Events_%d",i),Form("Varied Events %d",i),nbins, 0, nbins);
    }

    for(int j = 1; j < nbins+1; ++j){
        for(int k = 1; k < nbins+1; ++k){
            cov.SetBinContent(j,k, 0);
            cov_var.SetBinContent(j,k, 0);
            cov_zero.SetBinContent(j,k, 0);
        	cov_nofake.SetBinContent(j,k, 0);
		}
        nominal.SetBinContent(j,0);
        mc_stat_error.SetBinContent(j,0);
        mc_sys_error.SetBinContent(j,0);
        var_mean.SetBinContent(j,0);
        var_mean_offset.SetBinContent(j,0);
        zero_point.SetBinContent(j,0);
        for(int i = 0; i < nThrows; ++i){
            varied[i]->SetBinContent(j,0);
        }
    }

    double POT_Weight = 1;
    TRandom3 rand(0);

    for(int i = 0; i < (int)inChain.GetEntries(); ++i){
        inChain.GetEntry(i);
	
        if(i%100000 == 0)
	  std::cout << "Progress " << 100. * ((double) i) / inChain.GetEntries() << "%" << std::endl;
        bool isSand = 0;
	Int_t runp = anaUtils::GetRunPeriod(Run, SubRun);

	if(anaUtils::GetSandMode(Run) >= 0)
	  isSand = true;
        switch (runp){
	  // run2a:  25.9438         (3.56131e+19 / 9.23937e+20)
	  // run2w:  27.744          (4.33756e+19 / 1.20341e+21)
	  // run3b:  20.613          (2.17273e+19 / 4.47864e+20)
	  // run3c:  16.9685         (1.36447e+20 / 2.3153e+21 )
	  // run4a:  18.62           (1.78203e+20 / 3.31813e+21)
	  // run4w:  21.2819         (1.64277e+20 / 3.49613e+21)
	  // run5w:  52.8266         (4.3468e+19  / 2.29627e+21)
	  // run6b:  9.87713         (1.28838e+20 / 1.27255e+21)
	  // run6c:  10.3869         (5.07819e+19 / 5.27465e+20)
	  // run6d:  8.88964         (7.74385e+19 / 6.884e+20  )
	  // run6e:  10.0878         (8.51668e+19 / 8.59145e+20)
	case 0://run1 water
	   POT_Weight = 0.018057359;
	  break;
	case 1://run2 water
	  POT_Weight = 0.3604390856;
	  break;
	case 2://run2 air
	  POT_Weight = 0.03854494408;
	  break;
	case 3://run3b air separated based on the pot of data (run3b/(run3b+run3c)=0.13542
	  POT_Weight = 0.04851316471;
	  break;
	case 4://run3c air separated based on the pot of data
	  POT_Weight = 0.0589327517;
	  if(isSand) POT_Weight = 0.49537864;
	  break;
	case 5://run4 water
	   POT_Weight = 0.04698824128;	
	  break;
	case 6://run4 air
	  POT_Weight = 0.05370585239;
	  break;
	case 8://run5 antinu-water
	  POT_Weight = 0.01892982968;
	  if(isSand) POT_Weight = 0.036109244;
	  break;
	case 9://run6b antinu-air
	  POT_Weight = 0.101243959;
	  break;
	case 10://run6c antinu-air - have to split Run 6 due to different flux tunings for the different parts
	  POT_Weight = 0.09627539268;
	  break;
	case 11://run6d antinu-air
	  POT_Weight = 0.1124905578;
	  break;
	case 12://run6e antinu-air
	  POT_Weight = 0.09912971617;
	  break;
	default:
	  std::cout << "NOT FOUND?!?!" << std::endl;
	  exit(0);
	}

	int test = 0;

        for(int j = 0; j < nThrows; ++j){
            int bin = 0;
            double* pbins = pbins_0pi;
            double* tbins = ctbins_0pi;
            int npbins = npbins_0pi;
            int ntbins = nctbins_0pi;        

            int bin1 = 0;
            double* pbins1 = pbins_0pi1;
            double* tbins1 = ctbins_0pi1;
            int npbins1 = npbins_0pi1;
            int ntbins1 = nctbins_0pi1;        

            switch(Selection[j]){
                // Find the correct bin in the massive 2D histogram/matrix
	    case  SampleId::kFGD1NuMuCC0Pi:
                    break;   
                    // FGD1 CC1Pi
	    case SampleId::kFGD1NuMuCC1Pi :
                    bin = nbins_0pi;
                    pbins = pbins_1pi;
                    tbins = ctbins_1pi;
                    npbins = npbins_1pi;
                    ntbins = nctbins_1pi;
                    bin1 = nbins_0pi1;
                    pbins1 = pbins_1pi1;
                    tbins1 = ctbins_1pi1;
                    npbins1 = npbins_1pi1;
                    ntbins1 = nctbins_1pi1;
                    break;
                    // FGD 1 CCNPi
                case SampleId::kFGD1NuMuCCOther :
                    bin = nbins_0pi + nbins_1pi;
                    pbins = pbins_npi;
                    tbins = ctbins_npi;
                    npbins = npbins_npi;
                    ntbins = nctbins_npi;
                    bin1 = nbins_0pi1 + nbins_1pi1;
                    pbins1 = pbins_npi1;
                    tbins1 = ctbins_npi1;
                    npbins1 = npbins_npi1;
                    ntbins1 = nctbins_npi1;
                    break;
                    // FGD 1 anu qe
                case SampleId::kFGD1AntiNuMuCC1Track :
                    bin = nbins_0pi + nbins_1pi + nbins_npi;
                    pbins = pbins_anuqe;
                    tbins = ctbins_anuqe;
                    npbins = npbins_anuqe;
                    ntbins = nctbins_anuqe;
                    bin1 = nbins_0pi1 + nbins_1pi1 + nbins_npi1;
                    pbins1 = pbins_anuqe1;
                    tbins1 = ctbins_anuqe1;
                    npbins1 = npbins_anuqe1;
                    ntbins1 = nctbins_anuqe1;
                    break;
                    // FGD 1 anu nqe
                case SampleId::kFGD1AntiNuMuCCNTracks :
                    bin = nbins_0pi + nbins_1pi + nbins_npi + nbins_anuqe;
                    pbins = pbins_anunqe;
                    tbins = ctbins_anunqe;
                    npbins = npbins_anunqe;
                    ntbins = nctbins_anunqe;
                    bin1 = nbins_0pi1 + nbins_1pi1 + nbins_npi1 + nbins_anuqe1;
                    pbins1 = pbins_anunqe1;
                    tbins1 = ctbins_anunqe1;
                    npbins1 = npbins_anunqe1;
                    ntbins1 = nctbins_anunqe1;
                    break;
                    // Not anu selection
                case SampleId::kFGD1NuMuBkgInAntiNuModeCC1Track :
                    bin = nbins_0pi + nbins_1pi + nbins_npi + nbins_anuqe + nbins_anunqe;
                    pbins = pbins_nuqe;
                    tbins = ctbins_nuqe;
                    npbins = npbins_nuqe;
                    ntbins = nctbins_nuqe;
                    bin1 = nbins_0pi1 + nbins_1pi1 + nbins_npi1 + nbins_anuqe1 + nbins_anunqe1;
                    pbins1 = pbins_nuqe1;
                    tbins1 = ctbins_nuqe1;
                    npbins1 = npbins_nuqe1;
                    ntbins1 = nctbins_nuqe1;
                    break;
                    // Not nu_in_anu_qe
                case SampleId::kFGD1NuMuBkgInAntiNuModeCCNTracks :
                    bin = nbins_0pi + nbins_1pi + nbins_npi + nbins_anuqe + nbins_anunqe + nbins_nuqe;
                    pbins = pbins_nunqe;
                    tbins = ctbins_nunqe;
                    npbins = npbins_nunqe;
                    ntbins = nctbins_nunqe;
                    bin1 = nbins_0pi1 + nbins_1pi1 + nbins_npi1 + nbins_anuqe1 + nbins_anunqe1 + nbins_nuqe1;
                    pbins1 = pbins_nunqe1;
                    tbins1 = ctbins_nunqe1;
                    npbins1 = npbins_nunqe1;
                    ntbins1 = nctbins_nunqe1;
                    break;
                    //now repeat for fgd2 - bin should be set fine
                    //int bin = 0;
                case SampleId::kFGD2NuMuCC0Pi :
                    bin = nbins_0pi + nbins_1pi + nbins_npi + nbins_anuqe + nbins_anunqe + nbins_nuqe + nbins_nunqe;
                    pbins = pbins_0pi;
                    tbins = ctbins_0pi;
                    npbins = npbins_0pi;
                    ntbins = nctbins_0pi;
                    bin1 = nbins_0pi1 + nbins_1pi1 + nbins_npi1 + nbins_anuqe1 + nbins_anunqe1 + nbins_nuqe1 + nbins_nunqe1;
                    pbins1 = pbins_0pi1;
                    tbins1 = ctbins_0pi1;
                    npbins1 = npbins_0pi1;
                    ntbins1 = nctbins_0pi1;
                    break;
                    // FGD 2 1PI
	    case SampleId::kFGD2NuMuCC1Pi :
                    bin = 2*nbins_0pi + nbins_1pi + nbins_npi + nbins_anuqe + nbins_anunqe + nbins_nuqe + nbins_nunqe;
                    pbins = pbins_1pi;
                    tbins = ctbins_1pi;
                    npbins = npbins_1pi;
                    ntbins = nctbins_1pi;
                    bin1 = 2*nbins_0pi1 + nbins_1pi1 + nbins_npi1 + nbins_anuqe1 + nbins_anunqe1 + nbins_nuqe1 + nbins_nunqe1;
                    pbins1 = pbins_1pi1;
                    tbins1 = ctbins_1pi1;
                    npbins1 = npbins_1pi1;
                    ntbins1 = nctbins_1pi1;
                    break;
                case SampleId::kFGD2NuMuCCOther :
                    //FGD2 NPi
                    bin = 2*nbins_0pi + 2*nbins_1pi + nbins_npi + nbins_anuqe + nbins_anunqe + nbins_nuqe + nbins_nunqe;
                    pbins = pbins_npi;
                    tbins = ctbins_npi;
                    npbins = npbins_npi;
                    ntbins = nctbins_npi;
                    bin1 = 2*nbins_0pi1 + 2*nbins_1pi1 + nbins_npi1 + nbins_anuqe1 + nbins_anunqe1 + nbins_nuqe1 + nbins_nunqe1;
                    pbins1 = pbins_npi1;
                    tbins1 = ctbins_npi1;
                    npbins1 = npbins_npi1;
                    ntbins1 = nctbins_npi1;
                    break;
                case SampleId::kFGD2AntiNuMuCC1Track :
                    //FGD 2 anu 1tr
                    bin = 2*nbins_0pi + 2*nbins_1pi + 2*nbins_npi + nbins_anuqe + nbins_anunqe + nbins_nuqe + nbins_nunqe;
                    pbins = pbins_anuqe;
                    tbins = ctbins_anuqe;
                    npbins = npbins_anuqe;
                    ntbins = nctbins_anuqe;
                    bin1 = 2*nbins_0pi1 + 2*nbins_1pi1 + 2*nbins_npi1 + nbins_anuqe1 + nbins_anunqe1 + nbins_nuqe1 + nbins_nunqe1;
                    pbins1 = pbins_anuqe1;
                    tbins1 = ctbins_anuqe1;
                    npbins1 = npbins_anuqe1;
                    ntbins1 = nctbins_anuqe1;
                    break;
                case SampleId::kFGD2AntiNuMuCCNTracks :
                    //fgd2 anu ntr
                    bin = 2*nbins_0pi + 2*nbins_1pi + 2*nbins_npi + 2*nbins_anuqe + nbins_anunqe + nbins_nuqe + nbins_nunqe;
                    pbins = pbins_anunqe;
                    tbins = ctbins_anunqe;
                    npbins = npbins_anunqe;
                    ntbins = nctbins_anunqe;
                    bin1 = 2*nbins_0pi1 + 2*nbins_1pi1 + 2*nbins_npi1 + 2*nbins_anuqe1 + nbins_anunqe1 + nbins_nuqe1 + nbins_nunqe1;
                    pbins1 = pbins_anunqe1;
                    tbins1 = ctbins_anunqe1;
                    npbins1 = npbins_anunqe1;
                    ntbins1 = nctbins_anunqe1;
                    break;
                case SampleId::kFGD2NuMuBkgInAntiNuModeCC1Track :
                    //fgd2 nu in anu 1tr
                    bin = 2*nbins_0pi + 2*nbins_1pi + 2*nbins_npi + 2*nbins_anuqe + 2*nbins_anunqe + nbins_nuqe + nbins_nunqe;
                    pbins = pbins_nuqe;
                    tbins = ctbins_nuqe;
                    npbins = npbins_nuqe;
                    ntbins = nctbins_nuqe;
                    bin1 = 2*nbins_0pi1 + 2*nbins_1pi1 + 2*nbins_npi1 + 2*nbins_anuqe1 + 2*nbins_anunqe1 + nbins_nuqe1 + nbins_nunqe1;
                    pbins1 = pbins_nuqe1;
                    tbins1 = ctbins_nuqe1;
                    npbins1 = npbins_nuqe1;
                    ntbins1 = nctbins_nuqe1;
                    break;
                case SampleId::kFGD2NuMuBkgInAntiNuModeCCNTracks :
                    //fgd2 nu in anu ntr
                    bin = 2*nbins_0pi + 2*nbins_1pi + 2*nbins_npi + 2*nbins_anuqe + 2*nbins_anunqe + 2*nbins_nuqe + nbins_nunqe;
                    pbins = pbins_nunqe;
                    tbins = ctbins_nunqe;
                    npbins = npbins_nunqe;
                    ntbins = nctbins_nunqe;
                    bin1 = 2*nbins_0pi1 + 2*nbins_1pi1 + 2*nbins_npi1 + 2*nbins_anuqe1 + 2*nbins_anunqe1 + 2*nbins_nuqe1 + nbins_nunqe1;
                    pbins1 = pbins_nunqe1;
                    tbins1 = ctbins_nunqe1;
                    npbins1 = npbins_nunqe1;
                    ntbins1 = nctbins_nunqe1;
                    break;
                default :
                    continue;

            }

            int pbin = 0;
            int tbin = 0;

            for(int k = 1; k < npbins+1; ++k){
                if(PMu[j] <= pbins[k]){
                    pbin = k;
                    break;
                }
            }
            for(int k = 1; k < ntbins+1; ++k){
                if(ThetaMu[j] <= tbins[k]){
                    tbin = k;
                    break;
                }
            }
            if(pbin==0 || tbin == 0) continue;
            if(TMath::IsNaN(DetWeight[j]) || !TMath::Finite(DetWeight[j])) continue;

            bin += (pbin - 1)*ntbins + tbin;

			if(test==0) number_events.Fill(bin);
			test = 1;
			
            int pbin1 = 0;
            int tbin1 = 0;

            for(int k = 1; k < npbins1+1; ++k){
                if(PMu[j] <= pbins1[k]){
                    pbin1 = k;
                    break;
                }
            }
            for(int k = 1; k < ntbins1+1; ++k){
                if(ThetaMu[j] <= tbins1[k]){
                    tbin1 = k;
                    break;
                }
            }
            bin1 += (pbin1 - 1)*ntbins1 + tbin1;

            if(j==0){
                nominal.SetBinContent(bin, nominal.GetBinContent(bin) + FluxWeight[j]*NIWGWeight[j]*POT_Weight);
                mc_stat_error1.SetBinContent(bin1, mc_stat_error1.GetBinContent(bin1) + (FluxWeight[j]*NIWGWeight[j]*FluxWeight[j]*NIWGWeight[j]*POT_Weight*POT_Weight));
                mc_stat_nom.SetBinContent(bin1, mc_stat_nom.GetBinContent(bin1) + (FluxWeight[j]*NIWGWeight[j]*POT_Weight));
            }
            else if(j==1){
                zero_point.SetBinContent(bin, zero_point.GetBinContent(bin) + FluxWeight[j]*NIWGWeight[j]*DetWeight[j]*POT_Weight);
            }
            else{
                if(DetWeight[j] < 100){
                    varied[j-2]->SetBinContent(bin, varied[j-2]->GetBinContent(bin) + POT_Weight*FluxWeight[j]*NIWGWeight[j]*(DetWeight[j]));
                    if(TMath::IsNaN(varied[j-2]->GetBinContent(bin)) || !TMath::Finite(varied[j-2]->GetBinContent(bin))){
                        std::cout << "Not a number in bin " << bin << "!" << std::endl;
                    }
                }
                else{
                    varied[j-2]->SetBinContent(bin, varied[j-2]->GetBinContent(bin) + FluxWeight[j]*NIWGWeight[j]*POT_Weight*100);
                }
            }
        }
    }
    for(int i = 0; i < nThrows-2; ++i){
        for(int j = 1; j < nbins+1; ++j){
            var_mean.SetBinContent(j, var_mean.GetBinContent(j) + varied[i]->GetBinContent(j)/double(nThrows-2));
        }
    }


    for(int i = 0; i < nThrows-2; ++i){
        for(int j = 1; j < nbins+1; ++j){
            for(int k = 1; k < nbins+1; ++k){
                cov_var.SetBinContent(j, k, cov_var.GetBinContent(j,k) + ((varied[i]->GetBinContent(j) - var_mean.GetBinContent(j))*(varied[i]->GetBinContent(k) - var_mean.GetBinContent(k))));
            }
        }
    }

    for(int j = 1; j < nbins1+1; ++j){
        if(mc_stat_nom.GetBinContent(j)>0){
            mc_stat_error1.SetBinContent(j, mc_stat_error1.GetBinContent(j)/(mc_stat_nom.GetBinContent(j)*mc_stat_nom.GetBinContent(j)));
        }
    }

    int lastk = 1;
    for(int j = 1; j < nbins1+1; ++j){
        int sample = 0;
        double p = 0;
        double c = 0;
        double plow = 0;
        double clow = 0;
        double staterr= mc_stat_error1.GetBinContent(j);
        double fakeerr=0;// fake_cov->GetBinContent(j,j);
        //FGD1 CC0pi
		if(j < nbins_0pi1 + 1){
            p = pbins_0pi1[int((j-1)/nctbins_0pi1) + 1];
            c = ctbins_0pi1[int((j-1)%nctbins_0pi1) + 1];
            plow = pbins_0pi1[int((j-1)/nctbins_0pi1)];
            clow = ctbins_0pi1[int((j-1)%nctbins_0pi1)];
            for(int k = 1; k < nbins_0pi+1; ++k){
                if( pbins_0pi[int((k-1)/nctbins_0pi)] < plow || ctbins_0pi[int((k-1)%nctbins_0pi)] < clow) continue;
                if(staterr > mc_stat_error.GetBinContent(k)){
					mc_stat_error.SetBinContent(k, staterr);
                } 
                if(fabs(fakeerr) > fabs(fake_error.GetBinContent(k,k))){
					 fake_error.SetBinContent(k,k, fakeerr);
				 }
				break;
            }
        }
        //FGD1 CC1pi
		else if(j < nbins_1pi1+nbins_0pi1+1){
            p = pbins_1pi1[int((j-nbins_0pi1-1)/nctbins_1pi1) + 1];
            c = ctbins_1pi1[int((j-nbins_0pi1-1)%nctbins_1pi1) + 1];
            plow = pbins_1pi1[int((j-nbins_0pi1-1)/nctbins_1pi1)];
            clow = ctbins_1pi1[int((j-nbins_0pi1-1)%nctbins_1pi1)];
            for(int k = nbins_0pi+1; k < nbins_0pi+nbins_1pi+1; ++k){
                if( pbins_1pi[int((k - nbins_0pi - 1)/nctbins_1pi)] < plow || ctbins_1pi[int((k - nbins_0pi - 1)%nctbins_1pi)] < clow) continue;
                if(staterr > mc_stat_error.GetBinContent(k)){
					mc_stat_error.SetBinContent(k, staterr);
                } 
                if(fabs(fakeerr) > fabs(fake_error.GetBinContent(k,k))){
					 fake_error.SetBinContent(k,k, fakeerr);
				 }
				break;
            }
        }
        //FGD1 CCOther
		else if(j < nbins_npi1+nbins_1pi1+nbins_0pi1+1){
            p = pbins_npi1[int((j - nbins_1pi1-nbins_0pi1-1)/nctbins_npi1) + 1];
            c = ctbins_npi1[int((j - nbins_1pi1-nbins_0pi1-1)%nctbins_npi1) + 1];
            plow = pbins_npi1[int((j - nbins_1pi1-nbins_0pi1-1)/nctbins_npi1)];
            clow = ctbins_npi1[int((j - nbins_1pi1-nbins_0pi1-1)%nctbins_npi1)];
            for(int k = nbins_0pi+nbins_1pi+1; k < nbins_0pi+nbins_1pi+nbins_npi+1; ++k){
                if( pbins_npi[int((k - nbins_0pi - nbins_1pi - 1)/nctbins_npi)] < plow || ctbins_npi[int((k - nbins_0pi - nbins_1pi - 1)%nctbins_npi)] < clow) continue;
                if(staterr > mc_stat_error.GetBinContent(k)){
					mc_stat_error.SetBinContent(k, staterr);
                } 
                if(fabs(fakeerr) > fabs(fake_error.GetBinContent(k,k))){
					 fake_error.SetBinContent(k,k, fakeerr);
				 }
				break;
            }
        }
		//FGD1 Anuccqe
		else if(j < nbins_anuqe1 + nbins_npi1+nbins_1pi1+nbins_0pi1+1){
            p = pbins_anuqe1[int((j - nbins_npi1-nbins_1pi1-nbins_0pi1-1)/nctbins_anuqe1) + 1];
            c = ctbins_anuqe1[int((j - nbins_npi1-nbins_1pi1-nbins_0pi1-1)%nctbins_anuqe1) + 1];
            plow = pbins_anuqe1[int((j - nbins_npi1-nbins_1pi1-nbins_0pi1-1)/nctbins_anuqe1)];
            clow = ctbins_anuqe1[int((j - nbins_npi1-nbins_1pi1-nbins_0pi1-1)%nctbins_anuqe1)];
            for(int k = nbins_0pi+nbins_1pi+nbins_npi+1; k < nbins_0pi+nbins_1pi+nbins_npi+nbins_anuqe+1; ++k){
                if( pbins_anuqe[int((k - nbins_0pi - nbins_1pi - nbins_npi - 1)/nctbins_anuqe)] < plow || ctbins_anuqe[int((k - nbins_0pi - nbins_1pi - nbins_npi - 1)%nctbins_anuqe)] < clow) continue;
                if(staterr > mc_stat_error.GetBinContent(k)){
					mc_stat_error.SetBinContent(k, staterr);
                } 
                if(fabs(fakeerr) > fabs(fake_error.GetBinContent(k,k))){
					 fake_error.SetBinContent(k,k, fakeerr);
				 }
				break;
            }
		}
		//FGD1 Anuccnqe
		else if(j < nbins_anunqe1 + nbins_anuqe1 + nbins_npi1+nbins_1pi1+nbins_0pi1+1){
            p = pbins_anunqe1[int((j - nbins_anuqe1- nbins_npi1-nbins_1pi1-nbins_0pi1-1)/nctbins_anunqe1) + 1];
            c = ctbins_anunqe1[int((j -nbins_anuqe1- nbins_npi1-nbins_1pi1-nbins_0pi1-1)%nctbins_anunqe1) + 1];
            plow = pbins_anunqe1[int((j -nbins_anuqe1- nbins_npi1-nbins_1pi1-nbins_0pi1-1)/nctbins_anunqe1)];
            clow = ctbins_anunqe1[int((j -nbins_anuqe1- nbins_npi1-nbins_1pi1-nbins_0pi1-1)%nctbins_anunqe1)];
            for(int k = nbins_0pi+nbins_1pi+nbins_npi+nbins_anuqe+1; k < nbins_0pi+nbins_1pi+nbins_npi+nbins_anuqe+nbins_anunqe+1; ++k){
                if( pbins_anunqe[int((k - nbins_0pi - nbins_1pi - nbins_npi - nbins_anuqe-1)/nctbins_anunqe)] < plow || ctbins_anunqe[int((k - nbins_0pi - nbins_1pi - nbins_npi - nbins_anuqe-1)%nctbins_anunqe)] < clow) continue;
                if(staterr > mc_stat_error.GetBinContent(k)){
					mc_stat_error.SetBinContent(k, staterr);
                } 
                if(fabs(fakeerr) > fabs(fake_error.GetBinContent(k,k))){
					 fake_error.SetBinContent(k,k, fakeerr);
				 }
				break;
            }
		}
		//FGD1 Nuccqe
		else if(j < nbins_nuqe1+ nbins_anunqe1 + nbins_anuqe1 + nbins_npi1+nbins_1pi1+nbins_0pi1+1){
            p = pbins_nuqe1[int((j - nbins_anunqe1-nbins_anuqe1- nbins_npi1-nbins_1pi1-nbins_0pi1-1)/nctbins_nuqe1) + 1];
            c = ctbins_nuqe1[int((j -nbins_anunqe1-nbins_anuqe1- nbins_npi1-nbins_1pi1-nbins_0pi1-1)%nctbins_nuqe1) + 1];
            plow = pbins_nuqe1[int((j -nbins_anunqe1-nbins_anuqe1- nbins_npi1-nbins_1pi1-nbins_0pi1-1)/nctbins_nuqe1)];
            clow = ctbins_nuqe1[int((j -nbins_anunqe1-nbins_anuqe1- nbins_npi1-nbins_1pi1-nbins_0pi1-1)%nctbins_nuqe1)];
            for(int k = nbins_0pi+nbins_1pi+nbins_npi+nbins_anuqe+nbins_anunqe+1; k < nbins_0pi+nbins_1pi+nbins_npi+nbins_anuqe+nbins_anunqe+nbins_nuqe+1; ++k){
                if( pbins_nuqe[int((k - nbins_0pi - nbins_1pi - nbins_npi - nbins_anuqe-nbins_anunqe-1)/nctbins_nuqe)] < plow || ctbins_nuqe[int((k - nbins_0pi - nbins_1pi - nbins_npi - nbins_anuqe-nbins_anunqe-1)%nctbins_nuqe)] < clow) continue;
                if(staterr > mc_stat_error.GetBinContent(k)){
					mc_stat_error.SetBinContent(k, staterr);
                } 
                if(fabs(fakeerr) > fabs(fake_error.GetBinContent(k,k))){
					 fake_error.SetBinContent(k,k, fakeerr);
				 }
				break;
            }
		}
		//FGD1 Nuccnqe
		else if(j < nbins_nunqe1+nbins_nuqe1+ nbins_anunqe1 + nbins_anuqe1 + nbins_npi1+nbins_1pi1+nbins_0pi1+1){
            p = pbins_nunqe1[int((j - nbins_nuqe1-nbins_anunqe1-nbins_anuqe1- nbins_npi1-nbins_1pi1-nbins_0pi1-1)/nctbins_nunqe1) + 1];
            c = ctbins_nunqe1[int((j -nbins_nuqe1-nbins_anunqe1-nbins_anuqe1- nbins_npi1-nbins_1pi1-nbins_0pi1-1)%nctbins_nunqe1) + 1];
            plow = pbins_nunqe1[int((j -nbins_nuqe1-nbins_anunqe1-nbins_anuqe1- nbins_npi1-nbins_1pi1-nbins_0pi1-1)/nctbins_nunqe1)];
            clow = ctbins_nunqe1[int((j -nbins_nuqe1-nbins_anunqe1-nbins_anuqe1- nbins_npi1-nbins_1pi1-nbins_0pi1-1)%nctbins_nunqe1)];
            for(int k = nbins_0pi+nbins_1pi+nbins_npi+nbins_anuqe+nbins_anunqe+nbins_nuqe+1; k < nbins_0pi+nbins_1pi+nbins_npi+nbins_anuqe+nbins_anunqe+nbins_nuqe+nbins_nunqe+1; ++k){
                if( pbins_nunqe[int((k - nbins_0pi - nbins_1pi - nbins_npi - nbins_anuqe-nbins_anunqe-nbins_nuqe-1)/nctbins_nunqe)] < plow || ctbins_nunqe[int((k - nbins_0pi - nbins_1pi - nbins_npi - nbins_anuqe-nbins_anunqe-nbins_nuqe-1)%nctbins_nunqe)] < clow) continue;
                if(staterr > mc_stat_error.GetBinContent(k)){
					mc_stat_error.SetBinContent(k, staterr);
                } 
                if(fabs(fakeerr) > fabs(fake_error.GetBinContent(k,k))){
					 fake_error.SetBinContent(k,k, fakeerr);
				 }
				break;
            }
		}
		//FGD2 CC0pi
        else if(j < nbins_nunqe1+nbins_nuqe1+nbins_anunqe1+nbins_anuqe1+nbins_npi1+nbins_1pi1+(2*nbins_0pi1) + 1){
            p = pbins_0pi1[int((j-nbins_nunqe1-nbins_nuqe1-nbins_anunqe1-nbins_anuqe1-nbins_npi1-nbins_1pi1-nbins_0pi1 - 1)/nctbins_0pi1) + 1];
            c = ctbins_0pi1[int((j-nbins_nunqe1-nbins_nuqe1-nbins_anunqe1-nbins_anuqe1-nbins_npi1-nbins_1pi1-nbins_0pi1 - 1)%nctbins_0pi1) + 1];
            plow = pbins_0pi1[int((j-nbins_nunqe1-nbins_nuqe1-nbins_anunqe1-nbins_anuqe1-nbins_npi1-nbins_1pi1-nbins_0pi1 - 1)/nctbins_0pi1)];
            clow = ctbins_0pi1[int((j-nbins_nunqe1-nbins_nuqe1-nbins_anunqe1-nbins_anuqe1-nbins_npi1-nbins_1pi1-nbins_0pi1 - 1)%nctbins_0pi1)];
            for(int k = nbins_nunqe+nbins_nuqe+nbins_anunqe+nbins_anuqe+nbins_npi+nbins_1pi+nbins_0pi + 1; k < nbins_nunqe+nbins_nuqe+nbins_anunqe+nbins_anuqe+nbins_npi+nbins_1pi+2*nbins_0pi + 1; ++k){
                if( pbins_0pi[int((k-nbins_nunqe-nbins_nuqe-nbins_anunqe-nbins_anuqe-nbins_npi-nbins_1pi-nbins_0pi - 1)/nctbins_0pi)] < plow || ctbins_0pi[int((k-nbins_nunqe-nbins_nuqe-nbins_anunqe-nbins_anuqe-nbins_npi-nbins_1pi-nbins_0pi - 1)%nctbins_0pi)] < clow) continue;
                if(staterr > mc_stat_error.GetBinContent(k)){
					mc_stat_error.SetBinContent(k, staterr);
                } 
                if(fabs(fakeerr) > fabs(fake_error.GetBinContent(k,k))){
					 fake_error.SetBinContent(k,k, fakeerr);
				 }
                break;
            }
        }
		//FGD2 CC1pi
        else if(j < nbins_nunqe1+nbins_nuqe1+nbins_anunqe1+nbins_anuqe1+nbins_npi1+2*nbins_1pi1+2*nbins_0pi1 + 1){
            p = pbins_1pi1[int((j-nbins_nunqe1-nbins_nuqe1-nbins_anunqe1-nbins_anuqe1-nbins_npi1-nbins_1pi1-2*nbins_0pi1 - 1)/nctbins_1pi1) + 1];
            c = ctbins_1pi1[int((j-nbins_nunqe1-nbins_nuqe1-nbins_anunqe1-nbins_anuqe1-nbins_npi1-nbins_1pi1-2*nbins_0pi1 - 1)%nctbins_1pi1) + 1];
            plow = pbins_1pi1[int((j-nbins_nunqe1-nbins_nuqe1-nbins_anunqe1-nbins_anuqe1-nbins_npi1-nbins_1pi1-2*nbins_0pi1 - 1)/nctbins_1pi1)];
            clow = ctbins_1pi1[int((j-nbins_nunqe1-nbins_nuqe1-nbins_anunqe1-nbins_anuqe1-nbins_npi1-nbins_1pi1-2*nbins_0pi1 - 1)%nctbins_1pi1)];
            for(int k = nbins_nunqe+nbins_nuqe+nbins_anunqe+nbins_anuqe+nbins_npi+nbins_1pi+2*nbins_0pi + 1; k < nbins_nunqe+nbins_nuqe+nbins_anunqe+nbins_anuqe+nbins_npi+2*nbins_1pi+2*nbins_0pi + 1; ++k){
                if( pbins_1pi[int((k-nbins_nunqe-nbins_nuqe-nbins_anunqe-nbins_anuqe-nbins_npi-nbins_1pi-2*nbins_0pi - 1)/nctbins_1pi)] < plow || ctbins_1pi[int((k-nbins_nunqe-nbins_nuqe-nbins_anunqe-nbins_anuqe-nbins_npi-nbins_1pi-2*nbins_0pi - 1)%nctbins_1pi)] < clow) continue;
                if(staterr > mc_stat_error.GetBinContent(k)){
					mc_stat_error.SetBinContent(k, staterr);
                } 
                if(fabs(fakeerr) > fabs(fake_error.GetBinContent(k,k))){
					 fake_error.SetBinContent(k,k, fakeerr);
				 }
                break;
            }
        }
		//FGD2 CCOther
        else if(j < nbins_nunqe1+nbins_nuqe1+nbins_anunqe1+nbins_anuqe1+2*nbins_npi1+2*nbins_1pi1+2*nbins_0pi1 + 1){
            p = pbins_npi1[int((j-nbins_nunqe1-nbins_nuqe1-nbins_anunqe1-nbins_anuqe1-nbins_npi1-2*nbins_1pi1-2*nbins_0pi1 - 1)/nctbins_npi1) + 1];
            c = ctbins_npi1[int((j-nbins_nunqe1-nbins_nuqe1-nbins_anunqe1-nbins_anuqe1-nbins_npi1-2*nbins_1pi1-2*nbins_0pi1 - 1)%nctbins_npi1) + 1];
            plow = pbins_npi1[int((j-nbins_nunqe1-nbins_nuqe1-nbins_anunqe1-nbins_anuqe1-nbins_npi1-2*nbins_1pi1-2*nbins_0pi1 - 1)/nctbins_npi1)];
            clow = ctbins_npi1[int((j-nbins_nunqe1-nbins_nuqe1-nbins_anunqe1-nbins_anuqe1-nbins_npi1-2*nbins_1pi1-2*nbins_0pi1 - 1)%nctbins_npi1)];
            for(int k = nbins_nunqe+nbins_nuqe+nbins_anunqe+nbins_anuqe+nbins_npi+2*nbins_1pi+2*nbins_0pi + 1; k < nbins_nunqe+nbins_nuqe+nbins_anunqe+nbins_anuqe+2*nbins_npi+2*nbins_1pi+2*nbins_0pi + 1; ++k){
                if( pbins_npi[int((k-nbins_nunqe-nbins_nuqe-nbins_anunqe-nbins_anuqe-nbins_npi-2*nbins_1pi-2*nbins_0pi - 1)/nctbins_npi)] < plow || ctbins_npi[int((k-nbins_nunqe-nbins_nuqe-nbins_anunqe-nbins_anuqe-nbins_npi-2*nbins_1pi-2*nbins_0pi - 1)%nctbins_npi)] < clow) continue;
                if(staterr > mc_stat_error.GetBinContent(k)){
					mc_stat_error.SetBinContent(k, staterr);
                } 
                if(fabs(fakeerr) > fabs(fake_error.GetBinContent(k,k))){
					 fake_error.SetBinContent(k,k, fakeerr);
				 }
                break;
            }
        }
		//FGD2 AnuCCQE
        else if(j < nbins_nunqe1+nbins_nuqe1+nbins_anunqe1+2*nbins_anuqe1+2*nbins_npi1+2*nbins_1pi1+2*nbins_0pi1 + 1){
            p = pbins_anuqe1[int((j-nbins_nunqe1-nbins_nuqe1-nbins_anunqe1-nbins_anuqe1-2*nbins_npi1-2*nbins_1pi1-2*nbins_0pi1 - 1)/nctbins_anuqe1) + 1];
            c = ctbins_anuqe1[int((j-nbins_nunqe1-nbins_nuqe1-nbins_anunqe1-nbins_anuqe1-2*nbins_npi1-2*nbins_1pi1-2*nbins_0pi1 - 1)%nctbins_anuqe1) + 1];
            plow = pbins_anuqe1[int((j-nbins_nunqe1-nbins_nuqe1-nbins_anunqe1-nbins_anuqe1-2*nbins_npi1-2*nbins_1pi1-2*nbins_0pi1 - 1)/nctbins_anuqe1)];
            clow = ctbins_anuqe1[int((j-nbins_nunqe1-nbins_nuqe1-nbins_anunqe1-nbins_anuqe1-2*nbins_npi1-2*nbins_1pi1-2*nbins_0pi1 - 1)%nctbins_anuqe1)];
            for(int k = nbins_nunqe+nbins_nuqe+nbins_anunqe+nbins_anuqe+2*nbins_npi+2*nbins_1pi+2*nbins_0pi + 1; k < nbins_nunqe+nbins_nuqe+nbins_anunqe+2*nbins_anuqe+2*nbins_npi+2*nbins_1pi+2*nbins_0pi + 1; ++k){
                if( pbins_anuqe[int((k-nbins_nunqe-nbins_nuqe-nbins_anunqe-nbins_anuqe-2*nbins_npi-2*nbins_1pi-2*nbins_0pi - 1)/nctbins_anuqe)] < plow || ctbins_anuqe[int((k-nbins_nunqe-nbins_nuqe-nbins_anunqe-nbins_anuqe-2*nbins_npi-2*nbins_1pi-2*nbins_0pi - 1)%nctbins_anuqe)] < clow) continue;
                if(staterr > mc_stat_error.GetBinContent(k)){
					mc_stat_error.SetBinContent(k, staterr);
                } 
                if(fabs(fakeerr) > fabs(fake_error.GetBinContent(k,k))){
					 fake_error.SetBinContent(k,k, fakeerr);
				 }
                break;
            }
        }
		//FGD2 AnuCCNQE
        else if(j < nbins_nunqe1+nbins_nuqe1+2*nbins_anunqe1+2*nbins_anuqe1+2*nbins_npi1+2*nbins_1pi1+2*nbins_0pi1 + 1){
            p = pbins_anunqe1[int((j-nbins_nunqe1-nbins_nuqe1-nbins_anunqe1-2*nbins_anuqe1-2*nbins_npi1-2*nbins_1pi1-2*nbins_0pi1 - 1)/nctbins_anunqe1) + 1];
            c = ctbins_anunqe1[int((j-nbins_nunqe1-nbins_nuqe1-nbins_anunqe1-2*nbins_anuqe1-2*nbins_npi1-2*nbins_1pi1-2*nbins_0pi1 - 1)%nctbins_anunqe1) + 1];
            plow = pbins_anunqe1[int((j-nbins_nunqe1-nbins_nuqe1-nbins_anunqe1-2*nbins_anuqe1-2*nbins_npi1-2*nbins_1pi1-2*nbins_0pi1 - 1)/nctbins_anunqe1)];
            clow = ctbins_anunqe1[int((j-nbins_nunqe1-nbins_nuqe1-nbins_anunqe1-2*nbins_anuqe1-2*nbins_npi1-2*nbins_1pi1-2*nbins_0pi1 - 1)%nctbins_anunqe1)];
            for(int k = nbins_nunqe+nbins_nuqe+nbins_anunqe+2*nbins_anuqe+2*nbins_npi+2*nbins_1pi+2*nbins_0pi + 1; k < nbins_nunqe+nbins_nuqe+2*nbins_anunqe+2*nbins_anuqe+2*nbins_npi+2*nbins_1pi+2*nbins_0pi + 1; ++k){
                if( pbins_anunqe[int((k-nbins_nunqe-nbins_nuqe-nbins_anunqe-2*nbins_anuqe-2*nbins_npi-2*nbins_1pi-2*nbins_0pi - 1)/nctbins_anunqe)] < plow || ctbins_anunqe[int((k-nbins_nunqe-nbins_nuqe-nbins_anunqe-2*nbins_anuqe-2*nbins_npi-2*nbins_1pi-2*nbins_0pi - 1)%nctbins_anunqe)] < clow) continue;
                if(staterr > mc_stat_error.GetBinContent(k)){
					mc_stat_error.SetBinContent(k, staterr);
                } 
                if(fabs(fakeerr) > fabs(fake_error.GetBinContent(k,k))){
					 fake_error.SetBinContent(k,k, fakeerr);
				 }
                break;
            }
        }
		//FGD2 NuCCQE
        else if(j < nbins_nunqe1+2*nbins_nuqe1+2*nbins_anunqe1+2*nbins_anuqe1+2*nbins_npi1+2*nbins_1pi1+2*nbins_0pi1 + 1){
            p = pbins_nuqe1[int((j-nbins_nunqe1-nbins_nuqe1-2*nbins_anunqe1-2*nbins_anuqe1-2*nbins_npi1-2*nbins_1pi1-2*nbins_0pi1 - 1)/nctbins_nuqe1) + 1];
            c = ctbins_nuqe1[int((j-nbins_nunqe1-nbins_nuqe1-2*nbins_anunqe1-2*nbins_anuqe1-2*nbins_npi1-2*nbins_1pi1-2*nbins_0pi1 - 1)%nctbins_nuqe1) + 1];
            plow = pbins_nuqe1[int((j-nbins_nunqe1-nbins_nuqe1-2*nbins_anunqe1-2*nbins_anuqe1-2*nbins_npi1-2*nbins_1pi1-2*nbins_0pi1 - 1)/nctbins_nuqe1)];
            clow = ctbins_nuqe1[int((j-nbins_nunqe1-nbins_nuqe1-2*nbins_anunqe1-2*nbins_anuqe1-2*nbins_npi1-2*nbins_1pi1-2*nbins_0pi1 - 1)%nctbins_nuqe1)];
            for(int k = nbins_nunqe+nbins_nuqe+2*nbins_anunqe+2*nbins_anuqe+2*nbins_npi+2*nbins_1pi+2*nbins_0pi + 1; k < nbins_nunqe+2*nbins_nuqe+2*nbins_anunqe+2*nbins_anuqe+2*nbins_npi+2*nbins_1pi+2*nbins_0pi + 1; ++k){
                if( pbins_nuqe[int((k-nbins_nunqe-nbins_nuqe-2*nbins_anunqe-2*nbins_anuqe-2*nbins_npi-2*nbins_1pi-2*nbins_0pi - 1)/nctbins_nuqe)] < plow || ctbins_nuqe[int((k-nbins_nunqe-nbins_nuqe-2*nbins_anunqe-2*nbins_anuqe-2*nbins_npi-2*nbins_1pi-2*nbins_0pi - 1)%nctbins_nuqe)] < clow) continue;
                if(staterr > mc_stat_error.GetBinContent(k)){
					mc_stat_error.SetBinContent(k, staterr);
                } 
                if(fabs(fakeerr) > fabs(fake_error.GetBinContent(k,k))){
					 fake_error.SetBinContent(k,k, fakeerr);
				 }
                break;
            }
        }
		//FGD2 NuCCQE
        else if(j < 2*nbins_nunqe1+2*nbins_nuqe1+2*nbins_anunqe1+2*nbins_anuqe1+2*nbins_npi1+2*nbins_1pi1+2*nbins_0pi1 + 1){
			p = pbins_nunqe1[int((j-nbins_nunqe1-2*nbins_nuqe1-2*nbins_anunqe1-2*nbins_anuqe1-2*nbins_npi1-2*nbins_1pi1-2*nbins_0pi1 - 1)/nctbins_nunqe1) + 1];
            c = ctbins_nunqe1[int((j-nbins_nunqe1-2*nbins_nuqe1-2*nbins_anunqe1-2*nbins_anuqe1-2*nbins_npi1-2*nbins_1pi1-2*nbins_0pi1 - 1)%nctbins_nunqe1) + 1];
            plow = pbins_nunqe1[int((j-nbins_nunqe1-2*nbins_nuqe1-2*nbins_anunqe1-2*nbins_anuqe1-2*nbins_npi1-2*nbins_1pi1-2*nbins_0pi1 - 1)/nctbins_nunqe1)];
            clow = ctbins_nunqe1[int((j-nbins_nunqe1-2*nbins_nuqe1-2*nbins_anunqe1-2*nbins_anuqe1-2*nbins_npi1-2*nbins_1pi1-2*nbins_0pi1 - 1)%nctbins_nunqe1)];
            for(int k = nbins_nunqe+2*nbins_nuqe+2*nbins_anunqe+2*nbins_anuqe+2*nbins_npi+2*nbins_1pi+2*nbins_0pi + 1; k < 2*nbins_nunqe+2*nbins_nuqe+2*nbins_anunqe+2*nbins_anuqe+2*nbins_npi+2*nbins_1pi+2*nbins_0pi + 1; ++k){
                if( pbins_nunqe[int((k-nbins_nunqe-2*nbins_nuqe-2*nbins_anunqe-2*nbins_anuqe-2*nbins_npi-2*nbins_1pi-2*nbins_0pi - 1)/nctbins_nunqe)] < plow || ctbins_nunqe[int((k-nbins_nunqe-2*nbins_nuqe-2*nbins_anunqe-2*nbins_anuqe-2*nbins_npi-2*nbins_1pi-2*nbins_0pi - 1)%nctbins_nunqe)] < clow) continue;
                if(staterr > mc_stat_error.GetBinContent(k)){
					mc_stat_error.SetBinContent(k, staterr);
                } 
                if(fabs(fakeerr) > fabs(fake_error.GetBinContent(k,k))){
					 fake_error.SetBinContent(k,k, fakeerr);
				 }
                break;
            }
        }
    }
    // End of stat error stuff

    fake_error.Write("Fake_Error_Lin");
	
	TH1D* fake_lin = new TH1D("Lin_1p1h","Lin_1p1h",nbins,0,nbins); 
	
    for(int j = 1; j < nbins+1; ++j){
		fake_lin->SetBinContent(j,fake_error.GetBinContent(j,j));
        for(int k = j+1; k < nbins+1; ++k){
            fake_error.SetBinContent(j,k, fake_error.GetBinContent(j,j) * fake_error.GetBinContent(k,k));
        }
        fake_error.SetBinContent(j,j, fake_error.GetBinContent(j,j) * fake_error.GetBinContent(j,j));
    }
	
	fake_lin->Write();

    for(int j = 1; j < nbins+1; ++j){
        for(int k = 1; k < j; ++k){
            fake_error.SetBinContent(j,k, fake_error.GetBinContent(k,j));
        }
    }
		
    for(int j = 1; j < nbins+1; ++j){
        if(nominal.GetBinContent(j)>0){
            vector(j-1) = var_mean.GetBinContent(j)/nominal.GetBinContent(j);
            var_mean_offset.SetBinContent(j, var_mean.GetBinContent(j)/nominal.GetBinContent(j));
        }
        else{
            vector(j-1) = 1.0;
            var_mean_offset.SetBinContent(j,1);
        }
        for(int k = 1; k < nbins+1; ++k){

            if(fabs(cov_var.GetBinContent(j,k)) > 0.00001 && nominal.GetBinContent(j) > 0 && nominal.GetBinContent(k) > 0){
                cov_var.SetBinContent(j, k, cov_var.GetBinContent(j,k)/(double(nThrows-2)*nominal.GetBinContent(j)*nominal.GetBinContent(k)));
            }
            if(k==j){
                mc_sys_error.SetBinContent(j, cov_var.GetBinContent(j,k));
                cov_var.SetBinContent(j, k, cov_var.GetBinContent(j,k) + mc_stat_error.GetBinContent(j));
            }
            cov.SetBinContent(j, k, cov_var.GetBinContent(j,k));
            matrix(j-1,k-1) = cov.GetBinContent(j,k) + fake_error.GetBinContent(j,k);
        	cov_nofake.SetBinContent(j, k, cov.GetBinContent(j,k));
		}
        mc_stat_error.SetBinContent(j, sqrt(mc_stat_error.GetBinContent(j)));
    }
	cov.Add(&fake_error);
	
    vector.Write("Mean_Value");
    matrix.Write("Covariance_Matrix");

    for(int i = 1; i < nbins+1; ++i){
        for(int j = 1; j < nbins+1; ++j){
            corr.SetBinContent(i,j,cov.GetBinContent(i,j)/sqrt(cov.GetBinContent(i,i)*cov.GetBinContent(j,j)));
        }
    }

    corr.Write();
    cov.Write();
    mc_sys_error.Write();
    cov_var.Write();
    cov_zero.Write();
    cov_nofake.Write();
    nominal.Write();
    mc_stat_error.Write();
    fake_error.Write();
    zero_point.Write();
    var_mean.Write();
    var_mean_offset.Write();

    for(int i = 0; i < nThrows-2; ++i){
        varied[i]->Write();
    }

    number_events.Write();

    TCanvas c1;

    for(int i = 1; i < nbins+1; ++i){
        c1.SetName(Form("Bin_%d_NEvent_Spread",i));
        c1.SetTitle(Form("Bin_%d_NEvent_Spread",i));
        TH1F h1(Form("Bin_%d_NEvent_Spread_Hist",i), Form("Bin %d NEvent Spread",i), int(nominal.GetBinContent(i))*12, 0, int(nominal.GetBinContent(i))*3);

        TF1 f1("f1","gaus",nominal.GetBinContent(i)*0.5, nominal.GetBinContent(i)*1.5);
        TF1 f2("f2","gaus",nominal.GetBinContent(i)*0.5, nominal.GetBinContent(i)*1.5);

        f1.SetNameTitle(Form("Bin_%d_NEvent_Varying_Gaussian",i), "Varying Gaussian");
        f2.SetNameTitle(Form("Bin_%d_NEvent_Total_Gaussian",i), "Total Gaussian");

        for(int j = 0; j <nThrows-2; ++j){
            h1.Fill(varied[j]->GetBinContent(i));
        }

        h1.GetXaxis()->SetTitle("N Events selected");
        h1.SetStats(kFALSE);
        h1.Draw();

        f1.SetParameters((h1.Integral()/4.0)/(sqrt(TMath::Pi()*2)*sqrt(cov_var.GetBinContent(i,i))*nominal.GetBinContent(i)), var_mean.GetBinContent(i), sqrt(cov_var.GetBinContent(i,i))*nominal.GetBinContent(i));
        f2.SetParameters((h1.Integral()/4.0)/(sqrt(TMath::Pi()*2)*sqrt(cov.GetBinContent(i,i))*nominal.GetBinContent(i)), var_mean.GetBinContent(i), sqrt(cov.GetBinContent(i,i))*nominal.GetBinContent(i));

        h1.GetYaxis()->SetRangeUser(0,h1.GetMaximum()*1.1);
        h1.GetXaxis()->SetRange(h1.FindFirstBinAbove(0)-5,h1.FindLastBinAbove(0)+5);

        f1.FixParameter(0,(h1.Integral()/4.0)/(sqrt(TMath::Pi()*2)*sqrt(cov_var.GetBinContent(i,i))*nominal.GetBinContent(i)));
        f1.FixParameter(1,var_mean.GetBinContent(i));
        f1.FixParameter(2,sqrt(cov_var.GetBinContent(i,i))*nominal.GetBinContent(i));

        h1.Fit(&f1,"BNQ");
        double chi2 = f1.GetChisquare();

        GOF.SetBinContent(i,chi2);

        TLine l1(nominal.GetBinContent(i),0,nominal.GetBinContent(i),h1.GetMaximum());
        TLine l2(zero_point.GetBinContent(i),0,zero_point.GetBinContent(i),h1.GetMaximum());

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
        c1.Update();
        c1.Write();

    }
    GOF.Write();
    output.Close();
}




int main(int argn, char** arg){

  if(argn != 3){
    std::cout << "Specify MakeND280DetectorCov.exe infile outfile.root" << std::endl;
    return 1;
  }
  MakeCovariance(arg[1], arg[2], NULL);
  return 0;

}


