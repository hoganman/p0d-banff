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
        std::cout << "Usage: Make2018PostFitFile.exe preFitFile.root "
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
        //CA5, MARES, ISO_BKG
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
        //CC_DIS
        //BeRPA_A, BeRPA_B, BeRPA_D, BeRPA_E, BeRPA_U
        //CC_norm_nu, CC_norm_nubar
        //nue_numu, nuebar_numubar
        //CC_Coh_O
        //NC_Coh, NC_1gamma
        //NC_other_far
        else if(paramName == "CC_DIS"
            	|| paramName.find("2p2h_norm") != std::string::npos
	            || paramName.find("BeRPA") != std::string::npos
                || paramName.find("CC_norm") != std::string::npos
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

        //Include FSI too
        else if(paramName.find("FEF") != std::string::npos
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

    TVectorD*    outputPreFitParams_temp = new TVectorD(nParamsToSave);
    TMatrixDSym* outputPreFitCov_temp    = new TMatrixDSym(nParamsToSave);

    TVectorD*    outputPostFitParams_temp = new TVectorD(nParamsToSave);
    TMatrixDSym* outputPostFitCov_temp    = new TMatrixDSym(nParamsToSave);
	
    //Also need to save a list of the parameters.
    TObjArray* param_list      = new TObjArray(nParamsToSave);
    TObjArray* param_list_temp = new TObjArray(nParamsToSave);
	
    //Start by looping through the covariance matrices and putting a zero in
    //every slot.
    for(int i = 0; i < nParamsToSave; i++){
        for(int j = 0; j < nParamsToSave; j++){
            (*outputPreFitCov)(i,j)       = 0.0;
            (*outputPostFitCov)(i,j)      = 0.0;
            (*outputPreFitCov_temp)(i,j)  = 0.0;
            (*outputPostFitCov_temp)(i,j) = 0.0;
        }
    }

    //Find starting location of SK flux params, FSI and XSec params
    int iSKNuMode = -1;
    int iBeRPA    = -1;
    int iFEFQE    = -1; 
    int iMAQE     = -1;
    int iCA5      = -1;
    int iNCOther  = -1;
    int iCCNorm   = -1;
    int iNuENuMu  = -1;

    //Set up some saved parameter indices.
    int spi = 0; 
    int spj = 0;
    
    for(int i = 0; i < preFitParams->GetNrows(); i++){

        //If we're to save this parameter, do so.
        if(saveThisParam[i]){
            //std::cout << "This is a test" <<std::endl;
            if(addOne[i]){
                (*outputPreFitParams_temp)(spi)  = (*preFitParams)(i) + 1.0;
                (*outputPostFitParams_temp)(spi) = (*postFitParams)(i) + 1.0;
            }
            else{
                (*outputPreFitParams_temp)(spi)  = (*preFitParams)(i);
                (*outputPostFitParams_temp)(spi) = (*postFitParams)(i);
            }

            // Find locations of certain parameters to remove hardcoding
            std::string paramName(((TObjString*)(paramNames->At(i)))->GetString().Data());
            if(paramName == "SKNuModeNumu0") { iSKNuMode = spi; }
            if(paramName == "FEFQE")         { iFEFQE = spi; }
            if(paramName == "MAQE")          { iMAQE = spi; }
            if(paramName == "BeRPA_A")       { iBeRPA = spi; }
            if(paramName == "CA5")           { iCA5 = spi; }
            if(paramName == "CC_norm_nu")    { iCCNorm = spi; }
            if(paramName == "nue_numu")      { iNuENuMu = spi; }
            if(paramName == "NC_other_far")  { iNCOther = spi; }
            //Save its name.
            param_list_temp->AddAt(paramNames->At(i),spi);

            //Print out the parameter name.
            //std::cout << spi << ": " << ((TObjString*)paramNames->At(i))->GetString() << std::endl; 

            //Now loop over all the parameters that we save to fill the covariance matrix.
            spj = 0;
            for(int j = 0; j < preFitParams->GetNrows(); j++){
                if(saveThisParam[j]){
                    (*outputPreFitCov_temp)(spi,spj)  = (*preFitCov)(i,j);
                    (*outputPostFitCov_temp)(spi,spj) = (*postFitCov)(i,j);

                    spj++;
                }
            }

            //With the covariance matrix done, increment the saved parameter index.
            spi++;
        }
    }
    
    std::cout << "SK flux: "  << iSKNuMode
              << " FEFQE: "   << iFEFQE
              << " MAQE: "    << iMAQE
              << " BeRPA: "   << iBeRPA
              << " CA5: "     << iCA5
              << " CCNorm: "  << iCCNorm
              << " NuENuMu: " << iNuENuMu
              << " NCOther: " << iNCOther <<std::endl;

    // Number of BeRPA, FSI, CC0pi parameters
    // nRest_Params includes DIS, COH, and NC parameters
    int nFSI_Params    =  5;
    int nCC0pi_Params  =  6;
    int nBeRPA_Params  =  5;
    int nRES_Params    =  3;
    int nCCNorm_Params =  2;
    int nRest_Params   =  7;

    for(int oldi = 0; oldi < nParamsToSave; oldi++){

        int newi;

        if(oldi >= iFEFQE && oldi < iFEFQE+nFSI_Params){
            newi = oldi + nCC0pi_Params + nBeRPA_Params
                        + nRES_Params + nRest_Params;
        }
        else if(oldi >= iMAQE && oldi < iMAQE+nCC0pi_Params){
            newi = oldi - nFSI_Params;
        }
        else if(oldi >= iBeRPA && oldi < iBeRPA+nBeRPA_Params){
            newi = oldi + nRES_Params + nRest_Params - nFSI_Params;
        }
        else if(oldi >= iCA5 && oldi < iCA5+nRES_Params){
            newi = oldi - nBeRPA_Params - nFSI_Params;
        }
        else if(oldi >= iCCNorm && oldi < iCCNorm+nCCNorm_Params){
            newi = oldi + nRest_Params + nBeRPA_Params + nFSI_Params
                        - nFSI_Params - nBeRPA_Params;
        }
        else if(oldi >= iNuENuMu){
            newi = oldi - nFSI_Params - nBeRPA_Params - nCCNorm_Params;
        }
        else{
            newi = oldi;
        }
		param_list->AddAt(param_list_temp->At(oldi), newi);
        (*outputPreFitParams)(newi)  = (*outputPreFitParams_temp)(oldi);
        (*outputPostFitParams)(newi) = (*outputPostFitParams_temp)(oldi);
		
        //With the parameters taken care of, now take care of the covariance
        //matrix.
        for(int oldj = 0; oldj < nParamsToSave; oldj++){

			int newj;
            if(oldj >= iFEFQE && oldj < iFEFQE+nFSI_Params){
                newj = oldj + nCC0pi_Params + nBeRPA_Params
                    + nRES_Params + nRest_Params;
            }
            else if(oldj >= iMAQE && oldj < iMAQE+nCC0pi_Params){
                newj = oldj - nFSI_Params;
            }
            else if(oldj >= iBeRPA && oldj < iBeRPA+nBeRPA_Params){
                newj = oldj + nRES_Params + nRest_Params - nFSI_Params;
            }
            else if(oldj >= iCA5 && oldj < iCA5+nRES_Params){
                newj = oldj - nBeRPA_Params - nFSI_Params;
            }
            else if(oldj >= iCCNorm && oldj < iCCNorm+nCCNorm_Params){
                newj = oldj + nRest_Params + nBeRPA_Params + nFSI_Params
                    - nFSI_Params - nBeRPA_Params;
            }
            else if(oldj >= iNuENuMu){
                newj = oldj - nFSI_Params - nBeRPA_Params - nCCNorm_Params;
            }
	        else{
	            newj = oldj;
	        }

            (*outputPreFitCov)(newi,newj)  = (*outputPreFitCov_temp)(oldi,oldj);
            (*outputPostFitCov)(newi,newj) = (*outputPostFitCov_temp)(oldi,oldj);

        }
    }
	
    //Now that the parameter vectors are saved, save the other things that are
    //needed.  Namely:
    //SK beam binnings.
    //TAxis binnings for the CCCOH, NCCOH, and NCOTHER parameters, which are
    //each 1 bin from 0 to 30 GeV.

    //Get all the beam binnings.
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

