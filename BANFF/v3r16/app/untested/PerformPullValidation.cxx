#include "TFile.h"
#include "TMatrixDSym.h"
#include "TVector.h"
#include "TGraphErrors.h"
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include "TKey.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "THStack.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TLine.h"
#include "TStyle.h"
#include "Parameters.hxx"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

//Function to determine if a fit has a minimization error
//To use this you must have a folder containing the log files of each fit
//with a file number corresponding to one of the fit output root files.
bool bad_fit( int file_num );

//When given which parameters to consider (foxd), a toy throw file, and a file with
//a list of fit results to those toys, calculates the pulls for each parameter,
//adds them plots for each, fits gaussians, and plots the results for each
//parameter.  Maybe calculates the chi squared too.
//3rd argument should be the number of toys to consult (not necessarily the
//full number in the file).
//4th argument should be an ordered list of fit files in a txt file.
//5th argument should be an optional parameter override file
//
//Usage:
//
//PerformPullValidation.exe params toyFile nFits fitOutputList paramFile
//
//e.g.
//PerformPullValidation.exe fox toys_to_fit.root 300 fit_output_list.txt param_file.dat
//Meaning of the options:
//f: flux parameters included in fit and toys files.
//g: Exclude flux parameters from final plots.
//o: obsNorm parameters included in fit and toys files.
//p: Exclude obsNorm parameters from final plots.
//x: xsec parameters included in fit and toys files.
//y: Exclude xsec parameters from final plots.
//Note: If the parameters you wish to exclude are later in the list, just don't
//mention them.  (i.e. if you only want flux, just pass the "f" option.  If you
//want flux and xsec but not obsNorm, don't need to mention obsNorm at all
//(i.e. use fx). Only obsNorm is fgo. Only xsec is fgopx.
int main(int argc, char** argv){

    if(argc > 6){
        std::cout << "Too many command line arguments!" << std::endl;
    }

    //If there are 5 command line arguments, it is specifying a parameter
    //override file.  Read this in now.
    //According to the documentation for ReadParamOverrideFile, the code will
    //definitely use these values of the parameters, ignoring the contents of
    //any other parameters file.
    else if(argc == 6){
        std::string paramFile(argv[5]);
        ND::params().ReadParamOverrideFile(paramFile);
    }
    ND::params().SetReadParamOverrideFilePointPassed();

    std::string options(argv[1]);
    bool checkFlux = options.find("f") != std::string::npos;
    bool excludeFlux = options.find("g") != std::string::npos;
    
    bool checkObsNorm = options.find("o") != std::string::npos;
    bool excludeObsNorm = options.find("p") != std::string::npos;

    bool checkXSec = options.find("x") != std::string::npos;
    bool excludeXSec = options.find("y") != std::string::npos;
    
    bool checkDet = options.find("d") != std::string::npos;

    TFile* toyFile = new TFile(argv[2]);

    TObjArray *parameterNames;
    //Set up the default of 0 of each set of parameters.
    int nFluxParms = 0;
    TVectorD* flux_parms;

    int nXSecParms = 0;
    TVectorD* xsec_parms;

    int nDetParms = 0;
    TVectorD* det_parms;

    int nObsNormParms = 0;
    TVectorD* obsNorm_parms;

    std::string outputFileName = "validation_pulls_";

    //There will be the same number of parameters for each fit, so we just
    //start by looking at the first one to get the number of parameters.
    if(checkFlux){
        flux_parms = (TVectorD*)(toyFile->Get("flux_parms_0"));
        nFluxParms = flux_parms->GetNrows();
    }

    if(checkFlux & (!excludeFlux)){
        outputFileName += "f";
    }

    if(checkXSec){
        xsec_parms = (TVectorD*)(toyFile->Get("xsec_parms_0"));
        nXSecParms = xsec_parms->GetNrows();
    }

    if(checkXSec && (!excludeXSec)){
        outputFileName += "x";
    }

    if(checkDet){
        det_parms = (TVectorD*)toyFile->Get("det_parms_0");
        nDetParms = det_parms->GetNrows();
        outputFileName += "d";
    }

    if(checkObsNorm){
        obsNorm_parms = (TVectorD*)toyFile->Get("obsNorm_parms_0");
        nObsNormParms = obsNorm_parms->GetNrows();
    }

    if(checkObsNorm && (!excludeObsNorm)){
        outputFileName += "o";
    }

    int nParms = nFluxParms + nXSecParms + nDetParms + nObsNormParms;

    //Create one TVectorD to store all the toy parameters for convenience.
    std::vector<TVectorD*>  allToyParms;
    std::vector<TVectorD*>  allFittedParms;
    std::vector<TVectorD*>  allFittedSigmaSquared;
    char paramType[1000][20];

    //Store the prior in priorSigma.
    TVectorD* priorSigmaSquared = new TVectorD(nParms);

    //Store the prior value of the parameter.  This is what "nominal" is in the
    //pull calculation.
    TVectorD* priorValue = new TVectorD(nParms);

    if(checkFlux){
	std::cout << ND::params().GetParameterS("BANFF.FluxInputFile").c_str() << std::endl;
        TFile* fluxInput = new TFile(ND::params().GetParameterS("BANFF.FluxInputFile").c_str());
        TMatrixDSym* flux_cov = (TMatrixDSym*)fluxInput->Get("total_flux_cov");

        for(int fi = 0; fi < flux_cov->GetNrows(); fi++){
            (*priorSigmaSquared)(fi) = (*flux_cov)(fi,fi);
            //All flux parameter prior values are 1.
            (*priorValue)(fi) = 1.0;
            //Label this as a flux parameter.
            sprintf(paramType[fi],"f");
        }
    }

    if(checkObsNorm){
        std::cout << ND::params().GetParameterS("BANFF.ObsNormInputFile").c_str() << std::endl;
        TFile* obsNormInput = new TFile(ND::params().GetParameterS("BANFF.ObsNormInputFile").c_str());
	TMatrixDSym* obsNorm_cov = (TMatrixDSym*)(obsNormInput->Get("obsNorm_cov"));
        TVectorD* obsNorm_weights = (TVectorD*)(obsNormInput->Get("obsNorm_weights"));

        for(int oi = 0; oi < nObsNormParms; oi++){
            (*priorSigmaSquared)(nFluxParms + oi) = (*obsNorm_cov)(oi,oi);
            (*priorValue)(nFluxParms + oi) = (*obsNorm_weights)(oi);
            sprintf(paramType[nFluxParms + oi],"o");
        }
    }

    if(checkXSec){
	std::cout << ND::params().GetParameterS("BANFF.XSecInputFile").c_str() << std::endl;
        TFile* xsecInput = new TFile(ND::params().GetParameterS("BANFF.XSecInputFile").c_str());
        TMatrixDSym* xsec_cov = (TMatrixDSym*)(xsecInput->Get("xsec_cov"));
        TVectorD* xsec_param_prior = (TVectorD*)(xsecInput->Get("xsec_param_prior"));
        
        for(int xi = 0; xi < xsec_cov->GetNrows(); xi++){
            (*priorSigmaSquared)(nFluxParms + nObsNormParms + xi) = (*xsec_cov)(xi,xi);
            (*priorValue)(nFluxParms + nObsNormParms + xi) = (*xsec_param_prior)(xi);
            sprintf(paramType[nFluxParms + nObsNormParms + xi],"x");
        }
    }

    if(checkDet){
        //TODO: For now, assume prior value of 0 and covariance matrix entry of
        //1 for detector parameters.  This information could be read from
        //psyche though.
        for(int di = 0; di < nDetParms; di++){
            (*priorSigmaSquared)(nFluxParms + nXSecParms + nObsNormParms + di) = 1.0;
            (*priorValue)(nFluxParms + nXSecParms + nObsNormParms + di) = 0.0;
            sprintf(paramType[nFluxParms + nXSecParms + nObsNormParms + di],"d");
        }
    }

    //Now load in all the toy parameters for each toy.
    //Assumes they are in the order: flux, obsNorm, xsec, det
    int nToys = atoi(argv[3]);
    for(int i = 0; i < nToys; i++){

        TVectorD* thisToyParms = new TVectorD(nParms);

        if(checkFlux){
            flux_parms = (TVectorD*)(toyFile->Get(Form("flux_parms_%d",i)));
            for(int j = 0; j < nFluxParms; j++){
                (*thisToyParms)(j) = (*flux_parms)(j);
            }
        }

        if(checkObsNorm){
            obsNorm_parms = (TVectorD*)(toyFile->Get(Form("obsNorm_parms_%d",i)));
            for(int j = 0; j < nObsNormParms; j++){
                (*thisToyParms)(j + nFluxParms) = (*obsNorm_parms)(j);
            }
        }

        if(checkXSec){
            xsec_parms = (TVectorD*)(toyFile->Get(Form("xsec_parms_%d",i)));
            for(int j = 0; j < nXSecParms; j++){
                (*thisToyParms)(j + nFluxParms + nObsNormParms) = (*xsec_parms)(j);
            }
        }

        if(checkDet){
            det_parms = (TVectorD*)(toyFile->Get(Form("det_parms_%d",i)));
            for(int j = 0; j < nDetParms; j++){
                (*thisToyParms)(j + nFluxParms + nXSecParms + nObsNormParms) = (*det_parms)(j);
            }
        }

        //With all the parameters added, push back the parameter vector.
        allToyParms.push_back(thisToyParms);
    }

    //Now, loop through the fit result files and grab all of their fit results.
    //Assume that the files have been passed in in the correct order if there
    //are multiple fit output files.
    ifstream fitFiles(argv[4]);
    std::string line;
    int line_num = 0;

    if (fitFiles.is_open()) {
	while ( getline(fitFiles,line) ) {

	    if( line_num > nToys -1 )
		break;

	    //If any root files are broken for whatever reason you can choose to skip over them here
	    bool skip_noThrowStat = line.find("_109.") != std::string::npos
		|| line.find("_113.") != std::string::npos
		|| line.find("_278.") != std::string::npos
		|| line.find("_415.") != std::string::npos;
	    bool skip_throwStat = line.find("_38.") != std::string::npos
		|| line.find("_150.") != std::string::npos;
	    bool skip_0131 = false;
	    if( skip_0131 || bad_fit(line_num) ){
		std::cout << "Skipped file : "<< line << std::endl;
		TVectorD* fittedParameters =  new TVectorD(1);
		TVectorD* thisFittedSigmaSquared = new TVectorD(1);
		allFittedParms.push_back(fittedParameters);
		allFittedSigmaSquared.push_back(thisFittedSigmaSquared);
		line_num++;
		continue;
	    }

	    TFile* fitFile = new TFile(line.c_str());

	    if(fitFile->IsZombie()){
		std::cout << "Skipped file : "<< line << std::endl;
		TVectorD* fittedParameters =  new TVectorD(1);
		TVectorD* thisFittedSigmaSquared = new TVectorD(1);
		allFittedParms.push_back(fittedParameters);
		allFittedSigmaSquared.push_back(thisFittedSigmaSquared);
		line_num++;
		continue;
	    }

	    //Count the number of fits performed in each file.
	    TList* keys = fitFile->GetListOfKeys();
	    Int_t nKeys = fitFile->GetNkeys();

	    int nFitsThisFile = 0;

	    for(int j = 0; j < nKeys; j++){
		TKey* tmpKey = (TKey*)(keys->At(j));
		std::string keyName(tmpKey->GetName());

		if(keyName.find("FittedParameters") != std::string::npos){
		    nFitsThisFile++;
		}
	    }
	    if(nFitsThisFile > 1)
		std::cout << line << " has more than one fit!" << std::endl;

	    parameterNames = (TObjArray*)fitFile->Get("parameterNames");
	    //Now, for each fit in the file (starting at 0, going
	    //out to nFitsThisFile, load in the fitted
	    //parameters and covariance.
	    //Fits in the file are numbered from 0 to nFitsThisFile.
	    for(int j = 0; j < nFitsThisFile; j++){

		TVectorD* fittedParameters = (TVectorD*)fitFile->Get(Form("FittedParameters%d",j));
		TMatrixDSym* fittedCovariance = (TMatrixDSym*)fitFile->Get(Form("FittedCovariance%d",j));
		TVectorD* thisFittedSigmaSquared = new TVectorD(fittedCovariance->GetNrows());

		allFittedParms.push_back(fittedParameters);

		for(int k = 0; k < fittedCovariance->GetNrows(); k++){
		    (*thisFittedSigmaSquared)(k) = (*fittedCovariance)(k,k);
		}
		allFittedSigmaSquared.push_back(thisFittedSigmaSquared);
	    }
	    line_num++;
	}
	fitFiles.close();
    } // if (fitFiles.is_open())

    std::cout<<"Finished loading all the toys"<<std::endl;

    //Alright, so now all the toy throw values and their priors should be in memory, as should
    //the fitted parameter values and their fitted errors.  Now we compute the
    //pulls and add them to a histogram, for each parameter.  Put them in an
    //output file too.
    TFile* outputFile = new TFile((outputFileName + ".root").c_str(),"RECREATE");
    TH1D** pulls = new TH1D*[nParms];
    TH1D** params = new TH1D*[nParms];
    TH1D** params_error = new TH1D*[nParms];
    TH1D** thrown_param = new TH1D*[nParms];
    TH1D** diff_param = new TH1D*[nParms];
    TH1D** diff_error = new TH1D*[nParms];
    TH1D** diff_thrown = new TH1D*[nParms];
    TH2D** berpa_a = new TH2D*[nParms];
    TH2D** berpa_d = new TH2D*[nParms];
    TH2D** berpa_e = new TH2D*[nParms];

    for(int i = 0; i < nParms; i++){
	//Be sure to optimise pulls binning for fitting
        pulls[i] = new TH1D(Form("1%s%d",paramType[i],i),Form("%s%d;Parameter pull",paramType[i],i),16,-4.0,4.0);
        params[i] = new TH1D(Form("2%s%d",paramType[i],i),Form("%s%d;Parameter values",paramType[i],i),1000,-3.0,3.0);
        params_error[i] = new TH1D(Form("3%s%d",paramType[i],i),Form("%s%d;Parameter errors",paramType[i],i),2500,0.0,1.0);
        thrown_param[i] = new TH1D(Form("4%s%d",paramType[i],i),Form("%s%d;Thrown parameter values",paramType[i],i),1000,-3.0,3.0);
        diff_param[i] = new TH1D(Form("5%s%d",paramType[i],i),Form("%s%d;Differences of parameter fitted - prefit",paramType[i],i),1000,-2.0,2.0);
        diff_error[i] = new TH1D(Form("6%s%d",paramType[i],i),Form("%s%d;Differences of error prefit - fitted",paramType[i],i),1000,-2.0,2.0);
        diff_thrown[i] = new TH1D(Form("7%s%d",paramType[i],i),Form("%s%d;Differences of parameter fitted - thrown",paramType[i],i),1000,-2.0,2.0);
        berpa_a[i] = new TH2D(Form("8%s%d",paramType[i],i),Form("%s%d;BeRPA_A correlation",paramType[i],i),16,-4.0,4.0,16,-4.0,4.0);
        berpa_d[i] = new TH2D(Form("9%s%d",paramType[i],i),Form("%s%d;BeRPA_D correlation",paramType[i],i),16,-4.0,4.0,16,-4.0,4.0);
        berpa_e[i] = new TH2D(Form("10%s%d",paramType[i],i),Form("%s%d;BeRPA_E correlation",paramType[i],i),16,-4.0,4.0,16,-4.0,4.0);
    }

    TH2D* correlation = new TH2D("h_correlation","BeRPA correlations",35,0,35,3,0,3);
    TH2D* correlation_flux = new TH2D("h_correlation_flux","BeRPA correlations",100,0,100,3,0,3);
    TH2D* correlation_obs = new TH2D("h_correlation_obs","BeRPA correlations",556,0,556,3,0,3);

    //Loop over the number of toys that are considering.
    for(int i = 0; i < nToys; i++){

	//Skip file if a zombie
	if(allFittedParms[i]->GetNrows() == 1)
	    continue;

        //Loop over each parameter, and calculate the pull and put it into the
        //histogram for that parameter.
        //Added the correct calculations for unconstrained parameters
	for(int j = 0; j < nParms; j++){

	    if(true){
		double pull = ((*(allFittedParms[i]))(j) - (*(priorValue))(j))
		    /TMath::Sqrt(((*(allFittedSigmaSquared[i]))(j)));
		double pull_berpa_a = ((*(allFittedParms[i]))(669) - (*(priorValue))(669))
		    /TMath::Sqrt(((*(allFittedSigmaSquared[i]))(669)));
		double pull_berpa_d = ((*(allFittedParms[i]))(671) - (*(priorValue))(671))
		    /TMath::Sqrt(((*(allFittedSigmaSquared[i]))(671)));
		double pull_berpa_e = ((*(allFittedParms[i]))(672) - (*(priorValue))(672))
		    /TMath::Sqrt(((*(allFittedSigmaSquared[i]))(672)));
		pulls[j]->Fill(pull);
		berpa_a[j]->Fill(pull_berpa_a,pull);
		berpa_d[j]->Fill(pull_berpa_d,pull);
		berpa_e[j]->Fill(pull_berpa_e,pull);
		//std::cout<<"Unconstrained : thrown = "<<(*(allToyParms[i]))(j)<<" Fitted : "<<(*(allFittedParms[i]))(j)<<" Prior = "<<(*(priorValue))(j)<<" Error = "<<(*(allFittedSigmaSquared[i]))(j)<<" Pull = "<<pull<<std::endl;
            }
            else{
		double pull = ((*(allFittedParms[i]))(j) - (*(priorValue))(j))
		    /TMath::Sqrt((*priorSigmaSquared)(j)-(*(allFittedSigmaSquared[i]))(j));
		pulls[j]->Fill(pull);
		//std::cout<<"Constrained : thrown = "<<(*(allToyParms[i]))(j)<<" Fitted : "<<(*(allFittedParms[i]))(j)<<" Prior = "<<(*(priorValue))(j)<<" Prior Error = "<<(*priorSigmaSquared)(j)<<" Error = "<<(*(allFittedSigmaSquared[i]))(j)<<" Pull = "<<pull<<std::endl;
            }
            params[j]->Fill((*(allFittedParms[i]))(j));
            params_error[j]->Fill(sqrt((*(allFittedSigmaSquared[i]))(j)));
            thrown_param[j]->Fill((*(allToyParms[i]))(j));
            diff_param[j]->Fill((*(allFittedParms[i]))(j) - (*(priorValue))(j));
            diff_error[j]->Fill((*priorSigmaSquared)(j)-(*(allFittedSigmaSquared[i]))(j));
            diff_thrown[j]->Fill((*(allFittedParms[i]))(j) - (*(allToyParms[i]))(j));

        }

    }

    for(int i=0; i<35; i++){
      correlation->SetBinContent(i+1, 1, berpa_a[i+656]->GetCorrelationFactor());
      correlation->SetBinContent(i+1, 2, berpa_d[i+656]->GetCorrelationFactor());
      correlation->SetBinContent(i+1, 3, berpa_e[i+656]->GetCorrelationFactor());
    }

    for(int i=0; i<100; i++){
      correlation_flux->SetBinContent(i+1, 1, berpa_a[i]->GetCorrelationFactor());
      correlation_flux->SetBinContent(i+1, 2, berpa_d[i]->GetCorrelationFactor());
      correlation_flux->SetBinContent(i+1, 3, berpa_e[i]->GetCorrelationFactor());
    }

    for(int i=0; i<556; i++){
      correlation_obs->SetBinContent(i+1, 1, berpa_a[i+100]->GetCorrelationFactor());
      correlation_obs->SetBinContent(i+1, 2, berpa_d[i+100]->GetCorrelationFactor());
      correlation_obs->SetBinContent(i+1, 3, berpa_e[i+100]->GetCorrelationFactor());
    }

    //Output parameter plots for debugging
    bool debug_plots = true;
    if(debug_plots){
	TCanvas *c2 = new TCanvas("c2","sub data",200,10,700,500);
	c2->Print("pulls.pdf[","pdf");
	for(int i = 0; i < nParms; i++){
	    pulls[i]->Draw();
	    c2->Print("pulls.pdf","pdf");
	}
	c2->Print("pulls.pdf]","pdf");

	TCanvas *c3 = new TCanvas("c3","sub data",200,10,700,500);
	c3->Print("params.pdf[","pdf");
	for(int i = 0; i < nParms; i++){
	    params[i]->Draw();
	    c3->Print("params.pdf","pdf");
	}
	c3->Print("params.pdf]","pdf");

	TCanvas *c4 = new TCanvas("c4","sub data",200,10,700,500);
	c4->Print("params_error.pdf[","pdf");
	for(int i = 0; i < nParms; i++){
	    params_error[i]->Draw();
	    c4->Print("params_error.pdf","pdf");
	}
	c4->Print("params_error.pdf]","pdf");

	TCanvas *c5 = new TCanvas("c5","sub data",200,10,700,500);
	c5->Print("thrown_param.pdf[","pdf");
	for(int i = 0; i < nParms; i++){
	    thrown_param[i]->Draw();
	    c5->Print("thrown_param.pdf","pdf");
	}
	c5->Print("thrown_param.pdf]","pdf");

	TCanvas *c6 = new TCanvas("c6","sub data",200,10,700,500);
	c6->Print("diff_param.pdf[","pdf");
	for(int i = 0; i < nParms; i++){
	    diff_param[i]->Draw();
	    c6->Print("diff_param.pdf","pdf");
	}
	c6->Print("diff_param.pdf]","pdf");

	TCanvas *c7 = new TCanvas("c7","sub data",200,10,700,500);
	c7->Print("diff_thrown.pdf[","pdf");
	for(int i = 0; i < nParms; i++){
	    diff_thrown[i]->Draw();
	    c7->Print("diff_thrown.pdf","pdf");
	}
	c7->Print("diff_thrown.pdf]","pdf");

	TCanvas *c8 = new TCanvas("c8","sub data",200,10,700,500);
	c8->Print("diff_error.pdf[","pdf");
	for(int i = 0; i < nParms; i++){
	    diff_error[i]->Draw();
	    c8->Print("diff_error.pdf","pdf");
	}
	c8->Print("diff_error.pdf]","pdf");

	TCanvas *c9 = new TCanvas("c9","sub data",200,10,700,500);
	c9->Print("berpa_a.pdf[","pdf");
	for(int i = 0; i < nParms; i++){
	    berpa_a[i]->Draw("COLZ");
	    c9->Print("berpa_a.pdf","pdf");
	}
	c9->Print("berpa_a.pdf]","pdf");

	TCanvas *c10 = new TCanvas("c10","sub data",200,10,700,500);
	c10->Print("berpa_d.pdf[","pdf");
	for(int i = 0; i < nParms; i++){
	    berpa_d[i]->Draw("COLZ");
	    c10->Print("berpa_d.pdf","pdf");
	}
	c10->Print("berpa_d.pdf]","pdf");

	TCanvas *c11 = new TCanvas("c11","sub data",200,10,700,500);
	c11->Print("berpa_e.pdf[","pdf");
	for(int i = 0; i < nParms; i++){
	    berpa_e[i]->Draw("COLZ");
	    c11->Print("berpa_e.pdf","pdf");
	}
	c11->Print("berpa_e.pdf]","pdf");
    }

    //Now that all the pull histograms are filled, fit a gaussian to each of
    //them.  Store the fitted parameter, the fitted sigma, and their errors
    //from the fit.
    //Parameter 1 is the mean.
    //Parameter 2 is the sigma.
    Double_t* mean = new Double_t[nParms];
    Double_t* meanError = new Double_t[nParms];
    Double_t* sigma = new Double_t[nParms];
    Double_t* sigmaError = new Double_t[nParms];
    Double_t* paramNumber = new Double_t[nParms];
    outputFile->cd();
    for(int i = 0; i < nParms; i++){
        params[i]->Write();
        params_error[i]->Write();
        thrown_param[i]->Write();
        diff_param[i]->Write();
        diff_thrown[i]->Write();
        //std::cout << "Now fitting parameter: " << i << std::endl;
        pulls[i]->Fit("gaus","Q");
        pulls[i]->Write();

        //If the fit was bad (discrete parameter, for example) outut that and
        //just output defaults.
        if(pulls[i]->GetFunction("gaus") == NULL){

            std::cout << "Bad fit for parameter: " << i << std::endl;
            mean[i] = 0.0;
            meanError[i] = 0.0;
            sigma[i] = 1.0;
            sigmaError[i] = 0.0;
            paramNumber[i] = (Double_t)i;
            continue;
        }

        mean[i] = pulls[i]->GetFunction("gaus")->GetParameter(1);
        meanError[i] = pulls[i]->GetFunction("gaus")->GetParError(1);
        sigma[i] = pulls[i]->GetFunction("gaus")->GetParameter(2);
        sigmaError[i] = pulls[i]->GetFunction("gaus")->GetParError(2);

        paramNumber[i] = (Double_t)i;
        //std::cout << "Mean: " <<  mean[i] << std::endl;
        //std::cout << "MeanErr: " <<  meanError[i] << std::endl;
        //std::cout << "Sigma: " << sigma[i] << std::endl;
        //std::cout << "SigmaErr: " <<  sigmaError[i] << std::endl;

    }

    //With all fits performed and the quantities of interest pulled from them,
    //make a TGraphErrors with the mean and a TGraphErrors with the sigma.

    std::cout << "nParms: " << nParms << std::endl;

    int startingParam = 0;

    if(excludeFlux){
        startingParam += nFluxParms;
    }

    if(excludeObsNorm){
        startingParam += nObsNormParms;
    }

    Double_t* meanFinal = new Double_t[nParms-startingParam];
    Double_t* meanErrorFinal = new Double_t[nParms-startingParam];
    Double_t* sigmaFinal = new Double_t[nParms-startingParam];
    Double_t* sigmaErrorFinal = new Double_t[nParms-startingParam];

    for(int ip = 0; ip < nParms-startingParam; ip++){
        meanFinal[ip] = mean[ip + startingParam];
        meanErrorFinal[ip] = meanError[ip + startingParam];
        sigmaFinal[ip] = sigma[ip + startingParam];
        sigmaErrorFinal[ip] = sigmaError[ip + startingParam];
    }

    TH1D* meanHist = new TH1D("meanFinal","meanFinal",nParms-startingParam,0,nParms-startingParam);
    TH1D* sigmaHist = new TH1D("sigmaFinal","sigmaFinal",nParms-startingParam,0,nParms-startingParam);

    for(int i=1; i<=nParms-startingParam; i++){
	meanHist->SetBinContent(i, meanFinal[i-1]);
	sigmaHist->SetBinContent(i, sigmaFinal[i-1]);
	meanHist->SetBinError(i, meanErrorFinal[i-1]);
	sigmaHist->SetBinError(i, sigmaErrorFinal[i-1]);
    }

    //Save the individual Gaussian distributions too.
    //outputFile->Write();

    //Now we're done.  Save to file.
    meanHist->Write("mean");
    sigmaHist->Write("sigma");

    const char *binLabels[900];
    for (int i = 0; i <nParms;i++){
        binLabels[i]="";
    }
    int nbinlabel = 0;
    if(outputFileName == "validation_pulls_f"){
	binLabels[0] = "ND280 #nu_{#mu}, FHC";
	binLabels[11] = "ND280 #bar{#nu}_{#mu}, FHC";
	binLabels[16] = "ND280 #nu_{e}, FHC";
	binLabels[23] = "ND280 #bar{#nu}_{e}, FHC";
	binLabels[25] = "ND280 #nu_{#mu}, RHC";
	binLabels[30] = "ND280 #bar{#nu}_{#mu}, RHC";
	binLabels[41] = "ND280 #nu_{e}, RHC";
	binLabels[43] = "ND280 #bar{#nu}_{e}, RHC";
	binLabels[50] = "SK #nu_{#mu}, FHC";
	binLabels[61] = "SK #bar{#nu}_{#mu}, FHC";
	binLabels[66] = "SK #nu_{e}, FHC";
	binLabels[73] = "SK #bar{#nu}_{e}, FHC";
	binLabels[75] = "SK #nu_{#mu}, RHC";
	binLabels[80] = "SK #bar{#nu}_{#mu}, RHC";
	binLabels[91] = "SK #nu_{e}, RHC";
	binLabels[93] = "SK #bar{#nu}_{e}, RHC";
	//nbinlabel  = 100;
	nbinlabel = nFluxParms;
    }
    else if(outputFileName == "validation_pulls_x"){
        /*//Don't know why this doesn't work so setting manually below
	//Fix if you can
	TFile* xsecInput = new TFile(ND::params().GetParameterS("BANFF.XSecInputFile").c_str());
        TObjArray* xsec_param_names = (TObjArray*)(xsecInput->Get("xsec_param_names"));
	for(int i=0; i<nXSecParms; i++){
	    std::string paramName((((TObjString*)(xsec_param_names->At(i)))->GetString()).Data());
	    binLabels[i] = paramName.c_str();
	    }*/
	binLabels[0] = "FEFQE";
	binLabels[1] = "FEFQEH";
	binLabels[2] = "FEFINEL" ;
	binLabels[3] =  "FEFABS";
	binLabels[4] = "FEFCX";
	binLabels[5] = "MAQE";
	binLabels[6] =  "pF_C";
	binLabels[7] = "pF_O";
	binLabels[8] = "2p2h_norm_nu";
	binLabels[9] = "2p2h_norm_nubar";
	binLabels[10] = "2p2h_normCtoO";
	binLabels[11] = "2p2h_shape_C";
	binLabels[12] = "2p2h_shape_O";
	binLabels[13] =  "BeRPA_A";
	binLabels[14] = "BeRPA_B";
	binLabels[15] = "BeRPA_D";
	binLabels[16] = "BeRPA_E";
	binLabels[17] = "BeRPA_U";
	binLabels[18] = "CA5";
	binLabels[19] = "MARES";
	binLabels[20] = "ISO_BKG";
	binLabels[21] = "CC_norm_nu";
	binLabels[22] = "CC_norm_nubar";
	binLabels[23] = "nue_numu";
	binLabels[24] = "nuebar_numubar";
	binLabels[25] = "CC_DIS";
	binLabels[26] = "CC_Coh_C";
	binLabels[27] = "CC_Coh_O";
	binLabels[28] = "NC_Coh";
	binLabels[29] = "NC_1gamma";
	binLabels[30] = "NC_other_near";
	binLabels[31] = "NC_other_far";
	binLabels[32] = "SF_RFG";
	binLabels[33] = "RPA_C";
	binLabels[34] = "COH_BS";
	nbinlabel  = nXSecParms;
    }
    else if(outputFileName == "validation_pulls_o"){
	binLabels[0] = "FGD 1 CC-0#pi, FHC";
	binLabels[43] = "FGD 1 CC-1#pi, FHC";
	binLabels[84] = "FGD 1 CC-Other, FHC";
	binLabels[125] = "FGD 1 #bar{#nu}_{#mu} CC-1Track, RHC";
	binLabels[166] = "FGD 1 #bar{#nu}_{#mu} CC-NTracks, RHC";
	binLabels[202] = "FGD 1 #nu_{#mu} CC-1Track, RHC";
	binLabels[233] = "FGD 1 #nu_{#mu} CC-NTracks, RHC";
	binLabels[274] = "FGD 2 CC-0#pi, FHC";
	binLabels[315] = "FGD 2 CC-1#pi, FHC";
	binLabels[356] = "FGD 2 CC-Other, FHC";
	binLabels[397] = "FGD 2 #bar{#nu}_{#mu} CC-1Track, RHC";
	binLabels[438] = "FGD 2 #bar{#nu}_{#mu} CC-NTracks, RHC";
	binLabels[475] = "FGD 2 #nu_{#mu} CC-1Track, RHC";
	binLabels[516] = "FGD 2 #nu_{#mu} CC-NTracks, RHC";
	nbinlabel = 555;
    }
    else{
	nbinlabel = nParms;
    }

    //Might as well assemble a publication quality plot while we're at it.
    //Start by setting up the proper colour scheme.
    meanHist->SetMarkerStyle(20);
    meanHist->SetMarkerColor(kBlack);
    meanHist->SetLineColor(kBlack);
    sigmaHist->SetMarkerStyle(20);
    sigmaHist->SetMarkerColor(kRed);
    sigmaHist->SetLineColor(kRed);
    std::cout << "nbinlabel: " << nbinlabel << std::endl;
    for (int j=1;j<=nbinlabel;j++){
	meanHist->GetXaxis()->SetBinLabel(j,binLabels[j-1]);
	correlation->GetXaxis()->SetBinLabel(j,binLabels[j-1]);
    }
    TCanvas* c1 = new TCanvas("mean_and_sigma");
    c1->cd();
    c1->SetBottomMargin(0.35);
    c1->SetLeftMargin(0.06);
    c1->SetRightMargin(0.06);
    THStack *stack = new THStack("pStack","Parameter pull means and widths");
    stack->Add(meanHist,"EP");
    stack->Add(sigmaHist,"EP");
    stack->SetMaximum(1.4);
    stack->SetMinimum(-0.4);
    stack->Draw("NOSTACK");
    stack->GetXaxis()->LabelsOption("v");
    stack->GetXaxis()->SetLabelSize(0.04);

    c1->Modified();
    c1->Update();

    TLegend *leg = new TLegend(0.80,0.905,0.99,0.995);
    leg->SetFillColor(kWhite);
    leg->AddEntry(meanHist, "Pull mean", "EP");
    leg->AddEntry(sigmaHist, "Pull width", "EP");
    leg->Draw();

    c1->Write();
    c1->Print((outputFileName + ".pdf").c_str());
    c1->Print((outputFileName + ".png").c_str());
    c1->Print((outputFileName + ".eps").c_str());

    gStyle->SetOptStat(0);

    TCanvas *c12 = new TCanvas("c12","sub data",200,10,700,500);
    c12->SetBottomMargin(0.25);
    c12->Print("correlation.pdf[","pdf");
    correlation->Draw("COLZ");
    correlation->GetXaxis()->LabelsOption("v");
    correlation->GetXaxis()->SetLabelSize(0.04);
    c12->Modified();
    c12->Update();
    c12->Print("correlation.pdf","pdf");
    c12->Print("correlation.pdf]","pdf");

    TCanvas *c13 = new TCanvas("c13","sub data",200,10,700,500);
    c13->Print("correlation_flux.pdf[","pdf");
    correlation_flux->Draw("COLZ");
    c13->Print("correlation_flux.pdf","pdf");
    c13->Print("correlation_flux.pdf]","pdf");

    TCanvas *c14 = new TCanvas("c14","sub data",200,10,700,500);
    c14->Print("correlation_obsnorm.pdf[","pdf");
    correlation_obs->Draw("COLZ");
    c14->Print("correlation_obsnorm.pdf","pdf");
    c14->Print("correlation_obsnorm.pdf]","pdf");

    outputFile->Close();
	
}

bool bad_fit( int file_num ){

    stringstream ss;
    ss << file_num;
    //string fit_output_name = "../fit_output_throwStat/0124_asimov_fit_file_0_toy_" + ss.str() + ".out";
    string fit_output_name = "../fit_output_0131/0131_asimov_fit_file_0_toy_" + ss.str() + ".out";

    ifstream fit_output(fit_output_name.c_str());
    std::string line;

    bool error_found = false;

    if (fit_output.is_open()) {
	while ( getline(fit_output,line) ) {

	    //Look for a matrix forced positive definite error message here
	    error_found = error_found
		|| line.find("matrix forced pos-def") != std::string::npos
		|| line.find("negative or zero diagonal element") != std::string::npos;
	    if( error_found ) break;

	}
	fit_output.close();
    }

    if( error_found ){
	std::cout << "Bad fit found for " << fit_output_name << std::endl;
    }

    return error_found;
}
