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
//Usage: Make2018PostFitFile.exe 
//          preFitFile          -- ROOT file from SavePrefitParamsAndExit
//          preFitParamsName    -- FittedParameters0
//          preFitCovName       -- FittedCovariance0
//          fluxFile            -- input flux file in parameter override file
//          postFitFile         -- ROOT file from postfit results
//          postFitParamsName   -- FittedParameter0
//          postFitCov          -- FittedCovariance0
//          outputFileName      -- Name of output ROOT file
int main(int argc, char** argv){

    if( argc < 9 ){
        std::cout << "Error in command line options." << std::endl;
        std::cout << "Usage: Make2018PostFitFileAllParams.exe preFitFile.root "
                  << "FittedParameters0 FittedCovariance0 "
                  << "input_flux_uncertainty_file.root postFitFile.root "
                  << "FittedParameters0 FittedCovariance0 output_file.root" << std::endl;
        return 1;
    }

    //Load in the pre-fit file contents.
    TFile*       preFitFile   = new TFile(argv[1]);
    TVectorD*    preFitParams = (TVectorD*)(preFitFile->Get(argv[2]));
    TMatrixDSym* preFitCov    = (TMatrixDSym*)(preFitFile->Get(argv[3]));

    //The parameter names should be the same between them, so just only load one set.
    TObjArray* paramNames = (TObjArray*)(preFitFile->Get("parameterNames")); 

    //Load the flux file so can get those axes out of it.
    TFile* fluxFile = new TFile(argv[4]); 

    //Load in the post-fit file contents.
    TFile*       postFitFile   = new TFile(argv[5]);
    TVectorD*    postFitParams = (TVectorD*)(postFitFile->Get(argv[6]));
    TMatrixDSym* postFitCov    = (TMatrixDSym*)(postFitFile->Get(argv[7]));

    //Now, need to loop through and figure out which parameters to save to the output file.
    bool* saveThisParam = new bool[preFitParams->GetNrows()];
    int   nParamsToSave = 0;

    //Also store whether the parameter needs to have 1 added to it (since the
    //BANFF format subtracts 1 from several of them for spline use.)
    bool* addOne = new bool[preFitParams->GetNrows()];

    for(int i = 0; i < preFitParams->GetNrows(); i++){

        //Default saveThisParam to false and addOne to false.
        std::string paramName(((TObjString*)(paramNames->At(i)))->GetString().Data());
        saveThisParam[i] = false;
        addOne[i]        = false;

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
        //pF_C, pF_O
        //CA5, MARES, ISO_BKG
        //2p2h_shape_C, 2p2h_shape_O
        //CCNUE_0
        else if(paramName == "MAQE"
                || paramName == "pF_C"
                || paramName == "pF_O"
                || paramName.find("2p2h_shape") != std::string::npos
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
        //CC_DIS
        //2p2h_norm_C, 2p2h_norm_O
        //BeRPA_A, BeRPA_B, BeRPA_D, BeRPA_E, BeRPA_U
        //CC_norm_nu, CC_norm_nubar
        //nue_numu, nuebar_numubar
        //CC_Coh_C, CC_Coh_O
        //NC_Coh, NC_1gamma
        //NC_other_near, NC_other_far
        else if(paramName == "CC_DIS"
            	|| paramName.find("2p2h_norm") != std::string::npos
	            || paramName.find("BeRPA") != std::string::npos
                || paramName.find("CC_norm") != std::string::npos
	            || paramName == "nue_numu"
		        || paramName == "nuebar_numubar"
                || paramName == "CC_Coh_C"
                || paramName == "CC_Coh_O"
                || paramName == "NC_Coh"
                || paramName == "NC_1gamma"                
				|| paramName == "NC_other_near"
				|| paramName == "NC_other_far"){
           
            saveThisParam[i] = true;
            nParamsToSave++;
        }

        //Might as well include FSI and obsNorm too.
        else if(paramName.find("FEF") != std::string::npos
                || paramName.find("OBS") != std::string::npos
                || paramName.find("FSI") != std::string::npos){

            saveThisParam[i] = true;
            nParamsToSave++;
        }
    } //End loop looking for parameters to save

    //Now that all the parameters have been assessed, create the output file
    //and its contents and save the results to it.
    TFile*       outputFile         = new TFile(argv[8],"RECREATE");
    TVectorD*    outputPreFitParams = new TVectorD(nParamsToSave);
    TMatrixDSym* outputPreFitCov    = new TMatrixDSym(nParamsToSave);

    TVectorD*    outputPostFitParams = new TVectorD(nParamsToSave);
    TMatrixDSym* outputPostFitCov    = new TMatrixDSym(nParamsToSave);

    //Also need to save a list of the parameters.
    TObjArray* param_list = new TObjArray(nParamsToSave);

    //Start by looping through the covariance matrices and putting a zero in every slot.
    for(int i = 0; i < nParamsToSave; i++){
        for(int j = 0; j < nParamsToSave; j++){

            (*outputPreFitCov)(i,j)  = 0.0;
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

            //Now loop over all the parameters that we save to fill the covariance matrix.
            spj = 0;
            for(int j = 0; j < preFitParams->GetNrows(); j++){
                if(saveThisParam[j]){
                    (*outputPreFitCov)(spi,spj)  = (*preFitCov)(i,j);
                    (*outputPostFitCov)(spi,spj) = (*postFitCov)(i,j);

                    spj++;
                }
            }

            //With the covariance matrix done, increment the saved parameter index.
            spi++;
        }
    }
    
    //Now that the parameter vectors are saved, save the other things that are needed.  Namely:
    //SK beam binnings.
    //TAxis binnings for the CCCOH, NCCOH, and NCOTHER parameters, which are
    //each 1 bin from 0 to 30 GeV.

    //Get all the beam binnings.
    TAxis* nd_numode_numu_bins  = (TAxis*)(fluxFile->Get("nd5_numode_numu_bins"));
    TAxis* nd_numode_numub_bins = (TAxis*)(fluxFile->Get("nd5_numode_numub_bins"));
    TAxis* nd_numode_nue_bins   = (TAxis*)(fluxFile->Get("nd5_numode_nue_bins"));
    TAxis* nd_numode_nueb_bins  = (TAxis*)(fluxFile->Get("nd5_numode_nueb_bins"));

    TAxis* nd_anumode_numu_bins  = (TAxis*)(fluxFile->Get("nd5_anumode_numu_bins"));
    TAxis* nd_anumode_numub_bins = (TAxis*)(fluxFile->Get("nd5_anumode_numub_bins"));
    TAxis* nd_anumode_nue_bins   = (TAxis*)(fluxFile->Get("nd5_anumode_nue_bins"));
    TAxis* nd_anumode_nueb_bins  = (TAxis*)(fluxFile->Get("nd5_anumode_nueb_bins"));

    TAxis* sk_numode_numu_bins  = (TAxis*)(fluxFile->Get("sk_numode_numu_bins"));
    TAxis* sk_numode_numub_bins = (TAxis*)(fluxFile->Get("sk_numode_numub_bins"));
    TAxis* sk_numode_nue_bins   = (TAxis*)(fluxFile->Get("sk_numode_nue_bins"));
    TAxis* sk_numode_nueb_bins  = (TAxis*)(fluxFile->Get("sk_numode_nueb_bins"));
    
    TAxis* sk_anumode_numu_bins  = (TAxis*)(fluxFile->Get("sk_anumode_numu_bins"));
    TAxis* sk_anumode_numub_bins = (TAxis*)(fluxFile->Get("sk_anumode_numub_bins"));
    TAxis* sk_anumode_nue_bins   = (TAxis*)(fluxFile->Get("sk_anumode_nue_bins"));
    TAxis* sk_anumode_nueb_bins  = (TAxis*)(fluxFile->Get("sk_anumode_nueb_bins"));

    //Now the TAxis objects for each parameter
    //Not actually used in 2018 analysis
    //Left here for historical sake
    TAxis* CCCOH_C_bins = new TAxis(1, 0.0, 30.0);
    TAxis* CCCOH_O_bins = new TAxis(1, 0.0, 30.0);
    TAxis* NCCOH_bins   = new TAxis(1, 0.0, 30.0);
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

    //Close the output file.
    outputFile->Close();

}

