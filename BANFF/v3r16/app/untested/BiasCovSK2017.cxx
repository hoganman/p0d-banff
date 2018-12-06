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
#include "TObjString.h"

int main(int argc, char** argv){

    TFile* toyFile = new TFile(argv[1]);
    //Code to compute the bias covariance and bias correction for
    // SK relevant parameters
    // Usage: BiasCovSK.exe  ThrownToyFile.root nToys preFitParameterFile.root ToyFits.root
    //Set up the default of 0 of each set of parameters.
    int nFluxParms = 0;
    TVectorD* flux_parms;

    int nXSecParms = 0;
    TVectorD* xsec_parms;
    int nObsParms = 555;    
    
    std::string outputFileName = "biascov_sk";   

    flux_parms = (TVectorD*)(toyFile->Get("flux_parms_0"));
    nFluxParms = flux_parms->GetNrows();    
    xsec_parms = (TVectorD*)(toyFile->Get("xsec_parms_0"));
    nXSecParms = xsec_parms->GetNrows();    

    
    int nParms = nFluxParms + nXSecParms + nObsParms;    
    std::vector<TVectorD*>  allFittedParms;
    std::vector<TVectorD*>  allFittedSigmaSquared;
    std::vector<TVectorD*>  allFittedSigma;
    char paramType[1000][20];   

    //TVectorD* priorObs = new TVectorD(555); 
    TVectorD* priorValue = new TVectorD(nParms);    

    TFile* fluxInput = new TFile(ND::params().GetParameterS("BANFF.FluxInputFile").c_str());
    //TMatrixDSym* flux_cov = (TMatrixDSym*)fluxInput->Get("total_flux_cov");

    for(int fi = 0; fi < nFluxParms; fi++){

        //(*priorSigmaSquared)(fi) = (*flux_cov)(fi,fi);

        //All flux parameter prior values are 1.
        (*priorValue)(fi) = 1.0;

        //Label this as a flux parameter.
        //sprintf(paramType[fi],"f");

    }    

    TFile* xsecInput = new TFile(ND::params().GetParameterS("BANFF.XSecInputFile").c_str());
    TVectorD* xsec_param_prior = (TVectorD*)(xsecInput->Get("xsec_param_prior"));

   for(int xi = 0; xi < nXSecParms; xi++){
        //(*priorValue)(nFluxParms + xi) = (*xsec_param_prior)(xi);
        (*priorValue)(nFluxParms + 555 + xi) = (*xsec_param_prior)(xi);

   }

    TFile* obsNormInput = new TFile(ND::params().GetParameterS("BANFF.ObsNormInputFile").c_str());
    TVectorD* obsNorm_weights = (TVectorD*)(obsNormInput->Get("obsNorm_weights"));
    for(int oi = 0; oi < 555; oi++){
    //(*priorValue)(nFluxParms + nXSecParms + oi) = (*obsNorm_weights)(oi);
    (*priorValue)(nFluxParms + oi) = (*obsNorm_weights)(oi);
    }    

     int nToys = atoi(argv[2]); 
    TFile* prefitFile = new TFile(argv[3]);
    bool* saveThisParam = new bool[nParms];
    int nParamsToSave = 0;    
    TObjArray* paramNames = (TObjArray*)(prefitFile->Get("parameterNames"));
    bool* addOne = new bool[nParms]; 

    for(int i = 0; i < nParms; i++){

        //Default saveThisParam to false and addOne to false.
        std::string paramName(((TObjString*)(paramNames->At(i)))->GetString().Data());
        saveThisParam[i] = false;
        addOne[i] = false;

        //If it's an SK nu mode parameter, include it.)
        if(paramName.find("SKNuMode") != std::string::npos){
            saveThisParam[i] = true;
            nParamsToSave++;
        }

        //If it's an SK antinu mode parameter, include it.)
        else if(paramName.find("SKANuMode") != std::string::npos){
            saveThisParam[i] = true;
            nParamsToSave++;
        }

        //Save the following and add 1 to each of them.
        //MAQE
        //pF_O
        //MEC_O
        //EB_O
        //CA5
        //MANFFRES
        //BgRES
        //CCNUE_0
        else if(paramName == "MAQE"
                || paramName == "pF_O"
                || paramName == "CA5"
                || paramName == "MARES"
                || paramName == "ISO_BKG"
                || paramName == "CCNUE_0"){
            
            saveThisParam[i] = true;
            addOne[i] = true;
            nParamsToSave++;
        }

        //The following xsec parameters we also include, but don't add anything
        //for.
        //DISMPISHP
        //CCCOH_C_0
        //CCCOH_O_0
        //NCCOH_0
        //NCOTHER_0
        //Might as well include FSI too.
        else if(paramName == "CC_DIS"
            	|| paramName.find("2p2h_norm") != std::string::npos
	            || paramName.find("BeRPA") != std::string::npos
	            || paramName == "2p2h_shape_O"
	            || paramName == "nue_numu"
		        || paramName == "nuebar_numubar"
                || paramName == "CC_Coh_O"
                || paramName == "NC_Coh"
                || paramName == "NC_1gamma"                
				|| paramName == "NC_other_far"){
           
            saveThisParam[i] = true;
            nParamsToSave++;
        }
    
  
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
            TVectorD* thisFittedSigma = new TVectorD(fittedCovariance->GetNrows());

            allFittedParms.push_back(fittedParameters);

            for(int k = 0; k < fittedCovariance->GetNrows(); k++){

                (*thisFittedSigmaSquared)(k) = (*fittedCovariance)(k,k);
                (*thisFittedSigma)(k) = pow((*fittedCovariance)(k,k),0.5);
            }
            allFittedSigmaSquared.push_back(thisFittedSigmaSquared);
            allFittedSigma.push_back(thisFittedSigma);

        }

    }        
    
    TFile* outputFile = new TFile((outputFileName + ".root").c_str(),"RECREATE");
    TH1D** values = new TH1D*[nParms];
    TH1D** errors = new TH1D*[nParms];
    TMatrixDSym* bias = new TMatrixDSym(nParamsToSave);
    TVectorD *biasScale = new TVectorD(nParamsToSave);   
    TObjArray* param_list = new TObjArray(nParamsToSave);
    for(int i = 0; i < nParamsToSave; i++){
	for(int j = 0; j < nParamsToSave; j++){
		(*bias)(i,j) = 0.0;
	}
    } 

    for(int i = 0; i < nParms; i++){
    
        //Halve the number of bins from 32 to 16.
        if(i < 100){
        values[i] = new TH1D(Form("ff%d",i),Form("ff%d;Parameter fit",i),150,.5,1.5);
	errors[i] = new TH1D(Form("fe%d",i),Form("fe%d;Parameter error",i),64,0,0.15);}
        //else if(i >= 100 && i < nParms - 555){
        else if( i >=680){
        values[i] = new TH1D(Form("xf%d",i),Form("xf%d;Parameter fit",i),300,-3,3);
	errors[i] = new TH1D(Form("xe%d",i),Form("xe%d;Parameter error",i),100,0,1);}        
	else{
	values[i] = new TH1D(Form("df%d",i),Form("df%d;Parameter fit",i),160,0,2);
	errors[i] = new TH1D(Form("de%d",i),Form("de%d;Parameter fit",i),300,0,0.3);
	}
        

    }    
   for(int i = 0; i < nToys; i++){
        //if((*(allFittedParms[i]))(111) <= -0.990464){continue;}
        //Loop over each parameter, and calculate the pull and put it into the
        //histogram for that parameter.
        //Added the correct calculations for unconstrained parameters
        for(int j = 0; j < nParms; j++){
		//if(j < nParms ){
                values[j]->Fill((*(allFittedParms[i]))(j));
		errors[j]->Fill((*(allFittedSigma[i]))(j));//}
		//else{
		//values[j]->Fill((*(allFittedParms[i]))(j)/(*(priorObs))(j - nFluxParms - nXSecParms));
		//errors[j]->Fill((*(allFittedSigma[i]))(j)/(*(priorObs))(j -nFluxParms - nXSecParms));
		//}
                }
        }
        
    Double_t* mean = new Double_t[nParms];
    Double_t* nom = new Double_t[nParms];
    //Double_t* meanError = new Double_t[nParms];
    //Double_t* sigma = new Double_t[nParms];
    //Double_t* sigmaError = new Double_t[nParms];
    //Double_t* paramNumber = new Double_t[nParms];
    for(int i = 0; i < nParms; i++){       
        std::cout << "Now fitting parameter: " << i << std::endl;
        //values[i]->Fit("gaus");
        //errors[i]->Fit("gaus");
        
        //If the fit was bad (discrete parameter, for example) outut that and
        //just output defaults.
        //if(values[i]->GetFunction("gaus") == NULL
                /*|| i == 108
                || i == 390
                || i == 107
                || i == 106
                || i == 110
                || i == 111*///){

            //std::cout << "Bad fit for parameter: " << i << std::endl;
            //mean[i] = 0.0;
            //meanError[i] = 0.0;
            //sigma[i] = 1.0;
            //sigmaError[i] = 0.0;
            //paramNumber[i] = (Double_t)i;
            //continue;
        //}*/
        //mean[i] = values[i]->GetMean();
        //nom[i] = (Double_t)(*(priorValue))(i);
	if(addOne[i]){
		mean[i] = values[i]->GetMean() + 1.0;
		nom[i] = (Double_t)(*(priorValue))(i) + 1.0;
		std::cout << " mean " << i << " is " << mean[i] << " and nom is " << nom[i] << std::endl;
	}
        else{
		mean[i] = values[i]->GetMean();
		nom[i] = (Double_t)(*(priorValue))(i);
	}
        //meanError[i] = errors[i]->GetMean();
      //  sigma[i] = pulls[i]->GetFunction("gaus")->GetParameter(2);
      //  sigmaError[i] = pulls[i]->GetFunction("gaus")->GetParError(2);

        //paramNumber[i] = (Double_t)i;
        std::cout << "Mean: " <<  mean[i] << std::endl;
        //std::cout << "MeanErr: " <<  meanError[i] << std::endl;
        //std::cout << "Sigma: " << sigma[i] << std::endl;
        //std::cout << "SigmaErr: " <<  sigmaError[i] << std::endl;
        }
 
    int spi = 0; 
    int spj = 0; 
    for(int i = 0; i < nParms; i++){
        if(saveThisParam[i]){
            spj = 0;
	    std::cout << "Saving param # " << i << std::endl;
	    param_list->AddAt(paramNames->At(i),spi);
            (*biasScale)(spi) = nom[i]/mean[i]; 
            for(int j = 0; j < nParms; j++){
                if(saveThisParam[j]){
                    (*bias)(spi,spj) = (mean[i]/nom[i] - 1.0)*(mean[j]/nom[j] - 1.0);
                    spj++;
                }

            }
            spi++;
        }
    }
    //TGraphErrors* meanGraph = new TGraphErrors(nParms,paramNumber,  mean, NULL, meanError);
    //TGraphErrors* sigmaGraph = new TGraphErrors(nParms,paramNumber, sigma, NULL, sigmaError);

    //Save the individual Gaussian distributions too.
    //outputFile->Write();

    //Now we're done.  Save to file.
    //meanGraph->Write("mean");
    //sigmaGraph->Write("sigma");

    //Might as well assemble a publication quality plot while we're at it.
    //Start by setting up the proper colour scheme.
    //meanGraph->SetMarkerStyle(20);
    //sigmaGraph->SetMarkerStyle(20);
    //sigmaGraph->SetMarkerColor(kRed);
    //sigmaGraph->SetLineColor(kRed);
    //TCanvas* c1 = new TCanvas("mean_and_sigma");
    //c1->cd();
    //TMultiGraph *mg = new TMultiGraph();
    //mg->SetTitle("Parameter fit means;Parameter number;Fit value mean");
    //mg->Add(meanGraph,"P");
    //mg->Add(sigmaGraph,"P");
    //mg->Draw("A");
    //c1->Modified();
    //c1->Update();

    //TLegend *leg = new TLegend(0.80,0.905,0.99,0.995);
    //leg->SetFillColor(kWhite);
    //leg->AddEntry(meanGraph, "Fit value mean", "P");
    //leg->AddEntry(sigmaGraph, "Fit value width", "P");
    //leg->Draw();

    //c1->Write();
    //c1->Print((outputFileName + ".pdf").c_str());
    bias->Write("bias_cov");
    biasScale->Write("bias_frac");
    param_list->Write("param_list",TObject::kSingleKey);

    outputFile->Close();

}        
}