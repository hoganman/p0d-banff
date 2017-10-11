#include "TFile.h"
#include "TMatrixDSym.h"
#include "TVector.h"
#include "TGraphErrors.h"
#include <stdlib.h>
#include <sstream>
#include "TKey.h"
#include "TH1D.h"
#include "TF1.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "Parameters.hxx"

//When given which parameters to consider (fxod), a toy throw file, and a file with
//a list of fit results to those toys, calculates the pulls for each parameter,
//adds them plots for each, fits gaussians, and plots the results for each
//parameter.  Maybe calculates the chi squared too.
//3rd argument should be the number of toys to consult (not necessarily the
//full number in the file).
//The last arguments should be the fit files.  Just iterate out to argc.
//
//Usage:
//
//PerformPullValidation.exe params toyFile nFits fitoutput1 fitoutput2 ... 
//
//e.g.
//PerformPullValidation.exe fxo toys_to_fit.root 300 fit_output.root
//Meaning of the options:
//f: flux parameters included in fit and toys files.
//g: Exclude flux parameters from final plots.
//x: xsec parameters included in fit and toys files.
//y: Exclude xsec parameters from final plots.
//o: obsNorm parameters included in fit and toys files.
//p: Exclude obsNorm parameters from final plots.
//Note: If the parameters you wish to exclude are later in the list, just don't
//mention them.  (i.e. if you only want flux, just pass the "f" option.  If you
//want flux and xsec but not obsNorm, don't need to mention obsNorm at all
//(i.e. use fx.)  Only xsec is fgx.
int main(int argc, char** argv){
    
    std::string options(argv[1]);
    bool checkFlux = options.find("f") != std::string::npos;
    bool excludeFlux = options.find("g") != std::string::npos;

    bool checkXSec = options.find("x") != std::string::npos;
    bool excludeXSec = options.find("y") != std::string::npos;
    
    bool checkDet = options.find("d") != std::string::npos;
    
    bool checkObsNorm = options.find("o") != std::string::npos;
    // Not implemented yet, so commented out for now
    // bool excludeObsNorm = options.find("p") != std::string::npos;

    TFile* toyFile = new TFile(argv[2]);


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
        outputFileName += "o";

    }


    int nParms = nFluxParms + nXSecParms + nDetParms + nObsNormParms;
    

    //Create one TVectorD to store all the toy parameters for convenience.
    std::vector<TVectorD*>  allToyParms;
    std::vector<TVectorD*>  allFittedParms;
    std::vector<TVectorD*>  allFittedSigmaSquared;
    char paramType[1000][20];

    //Store the prior in priorSigma.
    TVectorD*  priorSigmaSquared = new TVectorD(nParms);
    
    //Store the prior value of the parameter.  This is what "nominal" is in the
    //pull calculation.
    TVectorD* priorValue = new TVectorD(nParms);

    if(checkFlux){

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

    if(checkXSec){

        TFile* xsecInput = new TFile(ND::params().GetParameterS("BANFF.XSecInputFile").c_str());
        TMatrixDSym* xsec_cov = (TMatrixDSym*)(xsecInput->Get("xsec_cov"));
        TVectorD* xsec_param_prior = (TVectorD*)(xsecInput->Get("xsec_param_prior"));
        
        for(int xi = 0; xi < xsec_cov->GetNrows(); xi++){

            (*priorSigmaSquared)(nFluxParms + xi) = (*xsec_cov)(xi,xi);
            (*priorValue)(nFluxParms + xi) = (*xsec_param_prior)(xi);
            sprintf(paramType[nFluxParms + xi],"x");

        }

    }

    if(checkObsNorm){

        TFile* obsNormInput = new TFile(ND::params().GetParameterS("BANFF.ObsNormInputFile").c_str());
        TMatrixDSym* obsNorm_cov = (TMatrixDSym*)(obsNormInput->Get("obsNorm_cov"));
        TVectorD* obsNorm_weights = (TVectorD*)(obsNormInput->Get("obsNorm_weights"));

        for(int oi = 0; oi < nObsNormParms; oi++){

            (*priorSigmaSquared)(nFluxParms + nXSecParms + oi) = (*obsNorm_cov)(oi,oi);
            (*priorValue)(nFluxParms + nXSecParms + oi) = (*obsNorm_weights)(oi);
            sprintf(paramType[nFluxParms + nXSecParms + oi],"o");

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
    int nToys = atoi(argv[3]);

    for(int i = 0; i < nToys; i++){


        TVectorD* thisToyParms = new TVectorD(nParms);

        if(checkFlux){

            flux_parms = (TVectorD*)(toyFile->Get(Form("flux_parms_%d",i)));

            for(int j = 0; j < nFluxParms; j++){

                (*thisToyParms)(j) = (*flux_parms)(j);

            }

        }

        if(checkXSec){

            xsec_parms = (TVectorD*)(toyFile->Get(Form("xsec_parms_%d",i)));

            for(int j = 0; j < nXSecParms; j++){


                (*thisToyParms)(j + nFluxParms) = (*xsec_parms)(j);

            }
        }

        if(checkObsNorm){

            obsNorm_parms = (TVectorD*)(toyFile->Get(Form("obsNorm_parms_%d",i)));

            for(int j = 0; j < nObsNormParms; j++){


                (*thisToyParms)(j + nFluxParms + nXSecParms) = (*obsNorm_parms)(j);

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
    for(int i = 4; i < argc; i++){

        TFile* fitFile = new TFile(argv[i]);

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

    }

    //Alright, so now all the toy throw values and their priors should be in memory, as should
    //the fitted parameter values and their fitted errors.  Now we compute the
    //pulls and add them to a histogram, for each parameter.  Put them in an
    //output file too.
    TFile* outputFile = new TFile((outputFileName + ".root").c_str(),"RECREATE");
    TH1D** pulls = new TH1D*[nParms];

    for(int i = 0; i < nParms; i++){
    
        //Halve the number of bins from 32 to 16.
        pulls[i] = new TH1D(Form("%s%d",paramType[i],i),Form("%s%d;Parameter pull",paramType[i],i),16,-4.0,4.0);

    }

    //Loop over the number of toys that are considering.
    for(int i = 0; i < nToys; i++){

        //Loop over each parameter, and calculate the pull and put it into the
        //histogram for that parameter.
        for(int j = 0; j < nParms; j++){

            double pull = ((*(allFittedParms[i]))(j) - (*(priorValue))(j))
                /TMath::Sqrt((*priorSigmaSquared)(j)-(*(allFittedSigmaSquared[i]))(j));
            pulls[j]->Fill(pull);
        }

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
    for(int i = 0; i < nParms; i++){

        std::cout << "Now fitting parameter: " << i << std::endl;
        pulls[i]->Fit("gaus");
        
        //If the fit was bad (discrete parameter, for example) outut that and
        //just output defaults.
        if(pulls[i]->GetFunction("gaus") == NULL
                || i == 108
                || i == 390
                || i == 107){

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
        std::cout << "Mean: " <<  mean[i] << std::endl;
        std::cout << "MeanErr: " <<  meanError[i] << std::endl;
        std::cout << "Sigma: " << sigma[i] << std::endl;
        std::cout << "SigmaErr: " <<  sigmaError[i] << std::endl;

    }

    //With all fits performed and the quantities of interest pulled from them,
    //make a TGraphErrors with the mean and a TGraphErrors with the sigma.

    std::cout << "nParms: " << nParms << std::endl;

    int startingParam = 0;

    if(excludeFlux){
        startingParam += nFluxParms;
    }

    if(excludeXSec){
        startingParam += nXSecParms;
    }

    Double_t* meanFinal = new Double_t[nParms-startingParam];
    Double_t* meanErrorFinal = new Double_t[nParms-startingParam];
    Double_t* sigmaFinal = new Double_t[nParms-startingParam];
    Double_t* sigmaErrorFinal = new Double_t[nParms-startingParam];
    Double_t* paramNumberFinal = new Double_t[nParms-startingParam];

    for(int ip = 0; ip < nParms-startingParam; ip++){
        meanFinal[ip] = mean[ip + startingParam]; 
        meanErrorFinal[ip] = meanError[ip + startingParam];
        sigmaFinal[ip] = sigma[ip + startingParam];
        sigmaErrorFinal[ip] = sigmaError[ip + startingParam];
        paramNumberFinal[ip] = paramNumber[ip + startingParam];
    }

    TGraphErrors* meanGraph = new TGraphErrors(nParms-startingParam,paramNumberFinal,  meanFinal, NULL, meanErrorFinal);
    TGraphErrors* sigmaGraph = new TGraphErrors(nParms-startingParam,paramNumberFinal, sigmaFinal, NULL, sigmaErrorFinal);

    //Save the individual Gaussian distributions too.
    outputFile->Write();

    //Now we're done.  Save to file.
    meanGraph->Write("mean");
    sigmaGraph->Write("sigma");

    //Might as well assemble a publication quality plot while we're at it.
    //Start by setting up the proper colour scheme.
    meanGraph->SetMarkerStyle(20);
    sigmaGraph->SetMarkerStyle(20);
    sigmaGraph->SetMarkerColor(kRed);
    sigmaGraph->SetLineColor(kRed);
    TCanvas* c1 = new TCanvas("mean_and_sigma");
    c1->cd();
    TMultiGraph *mg = new TMultiGraph();
    mg->SetTitle("Parameter pull means and widths;Parameter number;Pull mean or width");
    mg->Add(meanGraph,"P");
    mg->Add(sigmaGraph,"P");
    mg->Draw("A");
    c1->Modified();
    c1->Update();

    TLegend *leg = new TLegend(0.80,0.905,0.99,0.995);
    leg->SetFillColor(kWhite);
    leg->AddEntry(meanGraph, "Pull mean", "P");
    leg->AddEntry(sigmaGraph, "Pull width", "P");
    leg->Draw();

    c1->Write();
    c1->Print((outputFileName + ".pdf").c_str());

    outputFile->Close();

}
