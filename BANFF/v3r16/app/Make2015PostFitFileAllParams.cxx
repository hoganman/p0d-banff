#include "TFile.h"
#include "TVectorD.h"
#include "TMatrixDSym.h"
#include "TAxis.h"
#include "TObjArray.h"
#include "TObjString.h"
#include <iostream>

//Takes in a file containing pre-fit parameter information and another file
//containing post-fit information, in the BANFF format, an assembles them into
//the post-fit file format passed to oscillation analyzers.
//Usage: Make2015PostFitFile.exe preFitFile preFitParamsName preFitCovName,
//fluxFile
//postFitFile postFitParamsName postFitCov outputFileName.
int main(int argc, char** argv){

    if( argc < 9 ){
        std::cout << "Error in command line options." << std::endl;
        std::cout << "Usage: Make2015PostFitFile.exe preFitFile.root FittedParameters0 FittedCovariance0 input_flux_uncertainty_file.root postFitFile.root FittedParameters0 FittedCovariance0 output_file.root" << std::endl;
        return 1;
    }

    //Load in the pre-fit file contents.
    TFile* preFitFile = new TFile(argv[1]);
    TVectorD* preFitParams = (TVectorD*)(preFitFile->Get(argv[2]));
    TMatrixDSym* preFitCov = (TMatrixDSym*)(preFitFile->Get(argv[3]));

    //The parameter names should be the same between them, so just only load
    //one set.
    TObjArray* paramNames = (TObjArray*)(preFitFile->Get("parameterNames")); 

    //Load the flux file so can get those axes out of it.
    TFile* fluxFile = new TFile(argv[4]); 

    //Load in the post-fit file contents.
    TFile* postFitFile = new TFile(argv[5]);
    TVectorD* postFitParams = (TVectorD*)(postFitFile->Get(argv[6]));
    TMatrixDSym* postFitCov = (TMatrixDSym*)(postFitFile->Get(argv[7]));

    //Now, need to loop through and figure out which parameters to save to the
    //output file.
    bool* saveThisParam = new bool[preFitParams->GetNrows()];
    int nParamsToSave = 0;

    //Also store whether the parameter needs to have 1 added to it (since the
    //BANFF format subtracts 1 from several of them for spline use.)
    bool* addOne = new bool[preFitParams->GetNrows()];

    for(int i = 0; i < preFitParams->GetNrows(); i++){

        //Default saveThisParam to false and addOne to false.
        std::string paramName(((TObjString*)(paramNames->At(i)))->GetString().Data());
        saveThisParam[i] = false;
        addOne[i] = false;

        //If it's a nu mode parameter, include it.)
        if(paramName.find("NuMode") != std::string::npos){
            saveThisParam[i] = true;
            nParamsToSave++;
        }

        //If it's an antinu mode parameter, include it.)
        else if(paramName.find("ANuMode") != std::string::npos){
            saveThisParam[i] = true;
            nParamsToSave++;
        }

        //Save the following and add 1 to each of them.
        //MAQE
        //pF_C
        //MEC_C
        //EB_C
        //pF_O
        //MEC_O
        //EB_O
        //CA5
        //MANFFRES
        //BgRES
        //CCNUE_0
        else if(paramName == "MAQE"
                || paramName == "pF_C"
                || paramName == "MEC_C"
                || paramName == "EB_C"
                || paramName == "pF_O"
                || paramName == "MEC_O"
                || paramName == "EB_O"
                || paramName == "CA5"
                || paramName == "MANFFRES"
                || paramName == "BgRES"
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
        else if(paramName == "DISMPISHP"
                || paramName == "CCCOH_C_0"
                || paramName == "CCCOH_O_0"
                || paramName == "NCCOH_0"
                || paramName == "NCOTHER_0"){
           
            saveThisParam[i] = true;
            nParamsToSave++;
        }

        else if(paramName.find("FSI") != std::string::npos
                || paramName.find("OBS") != std::string::npos){

            saveThisParam[i] = true;
            nParamsToSave++;

        }

    }

    //Now that all the parameters have been assessed, create the output file
    //and its contents and save the results to it.
    TFile* outputFile = new TFile(argv[8],"RECREATE");
    TVectorD* outputPreFitParams = new TVectorD(nParamsToSave);
    TMatrixDSym* outputPreFitCov = new TMatrixDSym(nParamsToSave);

    TVectorD* outputPostFitParams = new TVectorD(nParamsToSave);
    TMatrixDSym* outputPostFitCov = new TMatrixDSym(nParamsToSave);

    //Also need to save a list of the parameters.
    TObjArray* param_list = new TObjArray(nParamsToSave);

    //Start by looping through the covariance matrices and putting a zero in
    //every slot.
    for(int i = 0; i < nParamsToSave; i++){

        for(int j = 0; j < nParamsToSave; j++){

            (*outputPreFitCov)(i,j) = 0.0;
            (*outputPostFitCov)(i,j) = 0.0;
        }
    }

    //Set up some saved parameter indices.
    int spi = 0; 
    int spj = 0;
    for(int i = 0; i < preFitParams->GetNrows(); i++){


        //If we're to save this parameter, do so.
        if(saveThisParam[i]){

            if(addOne[i]){
                (*outputPreFitParams)(spi) = (*preFitParams)(i) + 1.0;
                (*outputPostFitParams)(spi) = (*postFitParams)(i) + 1.0;
            }
            else{
                (*outputPreFitParams)(spi) = (*preFitParams)(i);
                (*outputPostFitParams)(spi) = (*postFitParams)(i);
            }

            //Save its name.
            param_list->AddAt(paramNames->At(i),spi);

            //Print out the parameter name.
            std::cout << spi << ": " << ((TObjString*)paramNames->At(i))->GetString() << std::endl; 

            //Now loop over all the parameters that we save to fill the
            //covariance matrix.
            spj = 0;
            for(int j = 0; j < preFitParams->GetNrows(); j++){

                if(saveThisParam[j]){

                    (*outputPreFitCov)(spi,spj) = (*preFitCov)(i,j);
                    (*outputPostFitCov)(spi,spj) = (*postFitCov)(i,j);

                    spj++;
                }
            }



            //With the covariance matrix done, increment the saved parameter
            //index.
            spi++;
        }

    }
    
    //Now that the parameter vectors are saved, save the other things that are
    //needed.  Namely:
    //SK beam binnings.
    //TAxis binnings for the CCCOH, NCCOH, and NCOTHER parameters, which are
    //each 1 bin from 0 to 30 GeV.

    //Get all the beam binnings.
    TAxis* nd_numode_numu_bins = (TAxis*)(fluxFile->Get("nd5_numode_numu_bins"));
    TAxis* nd_numode_numub_bins = (TAxis*)(fluxFile->Get("nd5_numode_numub_bins"));
    TAxis* nd_numode_nue_bins = (TAxis*)(fluxFile->Get("nd5_numode_nue_bins"));
    TAxis* nd_numode_nueb_bins = (TAxis*)(fluxFile->Get("nd5_numode_nueb_bins"));

    TAxis* nd_anumode_numu_bins = (TAxis*)(fluxFile->Get("nd5_anumode_numu_bins"));
    TAxis* nd_anumode_numub_bins = (TAxis*)(fluxFile->Get("nd5_anumode_numub_bins"));
    TAxis* nd_anumode_nue_bins = (TAxis*)(fluxFile->Get("nd5_anumode_nue_bins"));
    TAxis* nd_anumode_nueb_bins = (TAxis*)(fluxFile->Get("nd5_anumode_nueb_bins"));

    TAxis* sk_numode_numu_bins = (TAxis*)(fluxFile->Get("sk_numode_numu_bins"));
    TAxis* sk_numode_numub_bins = (TAxis*)(fluxFile->Get("sk_numode_numub_bins"));
    TAxis* sk_numode_nue_bins = (TAxis*)(fluxFile->Get("sk_numode_nue_bins"));
    TAxis* sk_numode_nueb_bins = (TAxis*)(fluxFile->Get("sk_numode_nueb_bins"));
    
    TAxis* sk_anumode_numu_bins = (TAxis*)(fluxFile->Get("sk_anumode_numu_bins"));
    TAxis* sk_anumode_numub_bins = (TAxis*)(fluxFile->Get("sk_anumode_numub_bins"));
    TAxis* sk_anumode_nue_bins = (TAxis*)(fluxFile->Get("sk_anumode_nue_bins"));
    TAxis* sk_anumode_nueb_bins = (TAxis*)(fluxFile->Get("sk_anumode_nueb_bins"));

    //Now the TAxis objects for each parameter
    TAxis* CCCOH_C_bins = new TAxis(1, 0.0, 30.0);
    TAxis* CCCOH_O_bins = new TAxis(1, 0.0, 30.0);
    TAxis* NCCOH_bins = new TAxis(1, 0.0, 30.0);
    TAxis* NCOTHER_bins = new TAxis(1, 0.0, 30.0);


    //Make sure that we're dealing with the output file.
    outputFile->cd();

    //Save these to the output file, naming all axes with the base of the
    //parameter name in the name list.
    param_list->Write("param_list",TObject::kSingleKey);

    outputPreFitParams->Write("prefit_params");
    outputPreFitCov->Write("prefit_cov");

    outputPostFitParams->Write("postfit_params");
    outputPostFitCov->Write("postfit_cov");

     nd_numode_numu_bins->Write("nd5_numode_numu_bins");
     nd_numode_numub_bins->Write("nd5_numode_numub_bins");
     nd_numode_nue_bins->Write("nd5_numode_nue_bins");
     nd_numode_nueb_bins->Write("nd5_numode_nueb_bins");

     nd_anumode_numu_bins->Write("nd5_anumode_numu_bins");
     nd_anumode_numub_bins->Write("nd5_anumode_numub_bins");
     nd_anumode_nue_bins->Write("nd5_anumode_nue_bins");
     nd_anumode_nueb_bins->Write("nd5_anumode_nueb_bins");


    sk_numode_numu_bins->Write("sk_numode_numu_bins");
    sk_numode_numub_bins->Write("sk_numode_numub_bins");
    sk_numode_nue_bins->Write("sk_numode_nue_bins");
    sk_numode_nueb_bins->Write("sk_numode_nueb_bins");
    sk_anumode_numu_bins->Write("sk_anumode_numu_bins");
    sk_anumode_numub_bins->Write("sk_anumode_numub_bins");
    sk_anumode_nue_bins->Write("sk_anumode_nue_bins");
    sk_anumode_nueb_bins->Write("sk_anumode_nueb_bins");

    CCCOH_C_bins->Write("CCCOH_C_bins");
    CCCOH_O_bins->Write("CCCOH_O_bins");
    NCCOH_bins->Write("NCCOH_bins");
    NCOTHER_bins->Write("NCOTHER_bins");


    //Close the output file.
    outputFile->Close();

}

