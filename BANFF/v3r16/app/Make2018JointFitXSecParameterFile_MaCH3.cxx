#include "TFile.h"
#include "TMatrixDSym.h"
#include "TVectorD.h"
#include <iostream>
#include "TObjArray.h"
#include "TObjString.h"
#include "TAxis.h"
#include <stdlib.h>
#include <iomanip>
#include <math.h>

///Read in an input file provided containing the XSec parameter information for
///2016, and use this (making changes as needed) to assemble the XSec input for
///2016 used by the BANFF fit.
///Usage: Make2016XSecParameterFile inputFile outputFile forToyThrows onesigma mirror
int main(int argc, char** argv){

    if(argc != 6){
        std::cout << "Warning: Unexpected number of arguments." << std::endl;
        exit(1);
    }


    TFile* inputFile  = new TFile(argv[1]);
    TFile* outputFile = new TFile(argv[2], "RECREATE");
    bool forToyThrows = atoi(argv[3]);
    bool onesigma     = atoi(argv[4]);
    bool mirror       = atoi(argv[5]);

    TMatrixDSym* xsec_cov_in           = (TMatrixDSym*) (inputFile->Get("xsec_cov"));
    TVectorD*    xsec_param_prior_in   = (TVectorD*)    (inputFile->Get("xsec_param_prior"));
    TVectorD*    xsec_param_nom_in     = (TVectorD*)    (inputFile->Get("xsec_param_nom"));
    TVectorD*    xsec_param_lb_in      = (TVectorD*)    (inputFile->Get("xsec_param_lb"));
    TVectorD*    xsec_param_ub_in      = (TVectorD*)    (inputFile->Get("xsec_param_ub"));
    TObjArray*   xsec_param_names_in   = (TObjArray*)   (inputFile->Get("xsec_param_names"));
    TMatrixD*    xsec_param_id_in      = (TMatrixD*)    (inputFile->Get("xsec_param_id"));
    TObjArray*   xsec_norm_modes_in    = (TObjArray*)   (inputFile->Get("xsec_norm_modes"));
    TObjArray*   xsec_norm_elements_in = (TObjArray*)   (inputFile->Get("xsec_norm_elements"));
    TObjArray*   xsec_norm_nupdg_in    = (TObjArray*)   (inputFile->Get("xsec_norm_nupdg"));

    //Skip Eb_dial at end of covariance file
    //This dial is only used for SK and is not needed for BANFF
    int nXsecPriorRows = xsec_param_prior_in->GetNrows() - 1;
	//Add discrete parameters at the end
    int nParams = nXsecPriorRows + 3;

    
    //Make newer, bigger versions of each of these arrays that can include the
    //discrete parameters.
    TMatrixDSym* xsec_cov           = new TMatrixDSym(nParams);
    TVectorD*    xsec_param_prior   = new TVectorD(nParams);
    TVectorD*    xsec_param_nom     = new TVectorD(nParams);
    TVectorD*    xsec_param_lb      = new TVectorD(nParams);
    TVectorD*    xsec_param_ub      = new TVectorD(nParams);
    TObjArray*   xsec_param_names   = new TObjArray(nParams);
    TObjArray*   xsec_norm_modes    = new TObjArray(nParams);
    TObjArray*   xsec_norm_elements = new TObjArray(nParams);
    TObjArray*   xsec_norm_nupdg    = new TObjArray(nParams);
    TVectorD*    xsec_param_id      = new TVectorD(nParams);
	
    //Fill the parameters with their old values.  
    for(int i = 0; i < nParams; i++) {
        if(i < nParams-3) {
            (*xsec_param_prior)(i) = (*xsec_param_prior_in)(i);
            (*xsec_param_nom)(i)   = (*xsec_param_nom_in)(i);
            (*xsec_param_lb)(i)    = (*xsec_param_lb_in)(i);
            (*xsec_param_ub)(i)    = (*xsec_param_ub_in)(i);
            (*xsec_param_id)(i)    = (*xsec_param_id_in)(i,0);
            xsec_param_names->AddAt(xsec_param_names_in->At(i),i);

            if(i < nParams-8) {
                xsec_norm_modes->AddAt(xsec_norm_modes_in->At(i),i);
                //Don't copy target elements for NC_Coh or NC_other_near
                if(i != 23 && i != 25){
                    xsec_norm_elements->AddAt(xsec_norm_elements_in->At(i),i);
                }
                xsec_norm_nupdg->AddAt(xsec_norm_nupdg_in->At(i),i);
            }
        }
        else {
            (*xsec_param_nom)(i) = 0.0;
            (*xsec_param_id)(i)  = -1;
        }
        for(int j = 0; j < nParams; j++) {
            if((i < nParams-3) && (j < nParams-3)) {
                //Just copy it over.
                (*xsec_cov)(i,j) = (*xsec_cov_in)(i,j);
                if( i >=16 && i<=17 && j >=16 && j<=17 ){
                    (*xsec_cov)(i,j) = (*xsec_cov_in)(i,j);
                    if( i==j ){
                        (*xsec_cov)(i,j) += 1.0e-6;
                    }
                }
            }
            else {
                //If we're on the diagonal, put in 1.0.
                if( i == j) {
                    (*xsec_cov)(i,j) = 1.0;
                }
                //Otherwise, put in 0.0;
                else {
                    (*xsec_cov)(i,j) = 0.0;
                }
            }
        }
    }

    //Now add the additional TVectors required.
    //Whether to throw the parameter.
    TVectorD* xsec_param_throw = new TVectorD(nParams);

    //Whether to fit the parameter.
    TVectorD* xsec_param_fit = new TVectorD(nParams);

    //Whether the parameter has a Gaussian constraint
    //or whether to use just a flat prior.
    TVectorD* xsec_param_constrain = new TVectorD(nParams);

    //Whether this parameter has a spline associated with it or not.
    TVectorD* xsec_param_spline = new TVectorD(nParams);

    //Which nuclear target this parameter applies to.  Only relevant for
    //XSecNormParameters.  -1 for any, 12 for Carbon, 16 for Oxygen.
    TVectorD* xsec_param_target = new TVectorD(nParams);

    //Which bin this parameter corresponds to in the accompanying axes.  This
    //is only relevant for XSecNorm parameters.
    TVectorD* xsec_param_bin =  new TVectorD(nParams);

    //Only relevant for xsec norm parameters, the name of the TAxis containing
    //the bins it uses. In general, only one bin is used.
    //However, the CC_norm_nu and CC_norm_nubar require a different binning
    //and must be set accordingly.
    TObjArray* xsec_param_binsname = new TObjArray(nParams);

    //Indicates whether the parameter is discrete or not. 
    //This year no discrete parameter.
    TVectorD* xsec_param_discrete = new TVectorD(nParams);

    //Indicates whether the parameter is weight or not.
    TVectorD* xsec_param_weight = new TVectorD(nParams);


    // New for 2015 analysis - CCCoh reweight to BS model
    xsec_param_names->AddAt(new TObjString("COH_BS"),nParams-1);
    (*xsec_param_prior)(nParams-1) =  1.0;
    (*xsec_param_lb)(nParams-1)    = -1.0;
    (*xsec_param_ub)(nParams-1)    =  2.0;
    //(*xsec_param_throw)(nParams-1) = 0;

    //SF_RFG.
    //Its prior value needs to be 0, which weights SF to RFG.
    //Set the lower and upper bounds to something acceptable.
    xsec_param_names->AddAt(new TObjString("SF_RFG"),nParams-3);
    (*xsec_param_prior)(nParams-3) =  0.0;
    (*xsec_param_lb)(nParams-3)    = -1.0;
    (*xsec_param_ub)(nParams-3)    =  2.0;
    //(*xsec_param_throw)(nParams-3) = 0;

    //SF_RFGGraph and RPAGraph are set up as follows:
    /**
     *• SF_RFGGraph
     • 1 is SF (should have weights of 1 all the time.)
     • 0 is RFG. (this is what we want to the prior to be)
     • RPAGraph
     • -1: neut nominal.  (No RPA correction)
     • 0: non-relativistic RPA correction (value from the CCQE fit tuning)  
     This is what we want the prior to be.  (i.e. zero is the prior in both cases.)
     • 1: relativistic RPA correction
     **/
    //So set the prior for RPA_C to be 0.
    //New, January 22, 2015: The new prior value for this parameter should now
    //be 1, for relativistic RPA.  So set it to 1 here.
    //New, March 18, 2015.  Since we add the RPA_C and SF_RFG parameters in
    //now, have to add the RPA_C name in.
    xsec_param_names->AddAt(new TObjString("RPA_C"),nParams-2);
    (*xsec_param_prior)(nParams-2) = -1.0;
    (*xsec_param_lb)(nParams-2)    = -2.0;
    (*xsec_param_ub)(nParams-2)    =  2.0;
    //(*xsec_param_throw)(nParams-2) = 0;

    //Set CC_norm_nu and CC_norm_nubar modes to be a value that isn't
    //used in NEUT (currently)
    //NOTE: Indices are for 2018 analysis
    TVectorD *modeCC_norm_nu    = new TVectorD(1);
    TVectorD *modeCC_norm_nubar = new TVectorD(1);
    (*modeCC_norm_nu)(0) = 99;
    (*modeCC_norm_nubar)(0) = 99;
    xsec_norm_modes->AddAt(modeCC_norm_nu, 16);
    xsec_norm_modes->AddAt(modeCC_norm_nubar, 17);

    //Do the same for the nue_numu and nuebar_numubar parameters
    //NOTE: Indices are for 2018 analysis
    TVectorD *modenue_numu       = new TVectorD(1);
    TVectorD *modenuebar_numubar = new TVectorD(1);
    (*modenue_numu)(0) = 99;
    (*modenuebar_numubar)(0) = 99;
    xsec_norm_modes->AddAt(modenue_numu, 18);
    xsec_norm_modes->AddAt(modenuebar_numubar, 19);

    //Now loop over all parameters, and set various variables to appropriate defaults.
    for(int i = 0; i < nParams; i++) {
        //throw, fit, and constrain should all be 1.0
        (*xsec_param_throw)(i) = 1.0;
        (*xsec_param_fit)(i) = 1.0;
        (*xsec_param_constrain)(i) = 1.0;

        //Directly reading from the input, >=0 is spline, -1 is norm and -2 is functional
        if(i < nParams-3) {  
            if ((*xsec_param_id_in)(i,0) == -1 || (*xsec_param_id_in)(i,0) == -2) {
                (*xsec_param_spline)(i) = 0.0;
                //std::cout << "Normalization or functional parameter" <<std::endl;
                //std::cout << std::left << std::setw(23) 
                //          << (((TObjString*)(xsec_param_names->At(i)))->GetString()).Data() <<std::setw(23) 
                //          << (*xsec_param_prior_in)(i) << " " << (*xsec_param_prior)(i) <<std::endl;
            }
            else if ((*xsec_param_id_in)(i,0) >= 0) {
                //Spline parameter
                (*xsec_param_spline)(i) = 1.0;
                //For spline parameters that aren't FSI:
                //Input spline parameter priors from MaCH3 are centered around 1,
                //however BANFF spline parameters should be centered around 0.
                //Check if prior is "close" to 1, if it is, then subtract 1 from
                //the prior, lower bound, and upper bound.
                //0.5 is an arbitrary value and may need to be adjusted in the future
                if (fabs((*xsec_param_prior)(i)) > 0.5) {
                    (*xsec_param_prior)(i) = (*xsec_param_prior)(i) - 1.0;
                    (*xsec_param_lb)(i)    = (*xsec_param_lb)(i) - 1.0;
                    (*xsec_param_ub)(i)    = (*xsec_param_ub)(i) - 1.0;
                }
                //If not "close" to 1, then prior and bounds don't need to be changed.
                //This is the case for the 2018 FSI parameters.
                //std::cout << "Spline Parameter parameter" <<std::endl;
                //std::cout << std::left << std::setw(23) 
                //          << (((TObjString*)(xsec_param_names->At(i)))->GetString()).Data() <<std::setw(23) 
                //          << (*xsec_param_prior_in)(i) << " " << (*xsec_param_prior)(i) <<std::endl;
            }
        }


        //default target to -1.  Will change later for those with a target
        //specification required.
        (*xsec_param_target)(i) = -1.0;

        //default bin to 0.  XSecResp parameters don't use the bin, and all
        //XSecNorm parameters only have the 1 bin.
        (*xsec_param_bin)(i) = 0.0;

        //Since this only gets checked for xsec_norm parameters, all of which
        //use the same binning, might as well
        //set it to the same value for all entries here. 
        //EXCEPT for the CC_norm_nu and CC_norm_nubar, which use a smaller axis
        //NOTE: indices based on 2018 analysis
        if (i == 16 || i == 17) { xsec_param_binsname->AddAt(new TObjString("xsec_coulomb_bins"),i); }
        else { xsec_param_binsname->AddAt(new TObjString("xsec_param_bins"),i); }

        //Set the discrete flag to 0.0 for all.  Will set discrete parameters
        //to 1.0 later.
        (*xsec_param_discrete)(i) = 0.0;

        //Set the weight flag to 0.0 for all.  Will set weight parameters
        //to 1.0 later.
        (*xsec_param_weight)(i) = 0.0;

        std::cout << std::left << std::setw(23) 
            << (((TObjString*)(xsec_param_names->At(i)))->GetString()).Data() <<std::setw(23) 
            << (*xsec_param_prior)(i) <<std::endl;
        if(xsec_norm_elements->At(i) != NULL) {
            std::cout << (((TObjString*)(xsec_param_names->At(i)))->GetString()).Data() 
                <<" has target material specified" <<std::endl;
        }
        if(xsec_norm_modes->At(i) != NULL) {
            std::cout << (((TObjString*)(xsec_param_names->At(i)))->GetString()).Data() 
                << " is mode " << std::endl;
            ((TVectorD*)(xsec_norm_modes->At(i)))->Print();
        }
    }

    //Set bounds of 2p2h_shape parameters
    /*(*xsec_param_lb)(6) =  -2;
    (*xsec_param_ub)(6) = 2;
    (*xsec_param_lb)(7) =  -2;
    (*xsec_param_ub)(7) = 2;*/
		
    //Set prior of CC1pi parameters
    //CA5, MARES, ISO_BKG
    //(*xsec_param_prior)(13) =  -0.049505;
    //(*xsec_param_prior)(14) = 0.12632;
    //(*xsec_param_prior)(15) = -0.261538;
	
    //Now set the BeRPA U parameter to not vary in the fit
    (*xsec_param_throw)(12) = 0.0;

    // Don't fit the nue parameters
    // nue_numu, nuebar_numubar
    (*xsec_param_throw)(18) = 0.0;
    (*xsec_param_throw)(19) = 0.0;

    // Nor the far detector parameters
    // NC_1gamma, NC_other_far
    (*xsec_param_throw)(24) = 0.0;
    (*xsec_param_throw)(26) = 0.0;

    //Now set the discrete flag for CCCoh_BS, RPA_C and SF_RFG to true. And weight to true for CCCoh_BS
    //They are at nParams-3, nParams-2 and nParams-1 
    (*xsec_param_discrete)(nParams-3) = 1.0;
    (*xsec_param_spline)(nParams-3)   = 1.0;
    (*xsec_param_discrete)(nParams-2) = 1.0;
    (*xsec_param_spline)(nParams-2)   = 1.0;
    (*xsec_param_discrete)(nParams-1) = 1.0;
    (*xsec_param_spline)(nParams-1)   = 0.0;

    //And weight to true for CCCoh_BS
    (*xsec_param_weight)(nParams-1) = 1.0;

    (*xsec_param_spline)(nParams-2) = 0.0;    
    (*xsec_param_spline)(nParams-3) = 0.0;

    if(mirror) {
        (*xsec_param_lb)(1) = 2.0*(*xsec_param_lb)(1); //pF_C
        (*xsec_param_ub)(1) = 2.0*(*xsec_param_ub)(1);
        (*xsec_param_lb)(2) = 2.0*(*xsec_param_lb)(2); //pF_O
        (*xsec_param_ub)(2) = 2.0*(*xsec_param_ub)(2);
        (*xsec_param_lb)(6) = 2.0*(*xsec_param_lb)(6); //2p2h_shape_C
        (*xsec_param_ub)(6) = 2.0*(*xsec_param_ub)(6);
        (*xsec_param_lb)(7) = 2.0*(*xsec_param_lb)(7); //2p2h_shape_O
        (*xsec_param_ub)(7) = 2.0*(*xsec_param_ub)(7);
    }
	
    if(forToyThrows) {

        std::cout << "Applying FSI bounds for toy throws." << std::endl;

        //The FSI parameters have splines that have limited ranges of validity.
        //Although we do not want to restrict the fit, it would be prudent to only
        //throw toys within that range of validity.  We add these lower and upper
        //bounds now.
        //FSI_INEL_LO or FEFQE
        //LB: -0.8
        //UB: 1.2
        (*xsec_param_lb)(27) = -0.8;
        (*xsec_param_ub)(27) = 1.2;

        //FSI_INEL_HI or FEFQEH
        //LB: -0.8
        //UB: 1.2
        (*xsec_param_lb)(28) = -0.8;
        (*xsec_param_ub)(28) = 1.2;

        //FSI_PI_PROD or FEFINEL
        //LB: -0.8
        //UB: 1.2
        (*xsec_param_lb)(29) = -0.8;
        (*xsec_param_ub)(29) = 1.2;

        //FSI_PI_ABS or FEFABS
        //LB: -0.6
        //UB: 0.9
        (*xsec_param_lb)(30) = -0.6;
        (*xsec_param_ub)(30) = 0.9;

        //FSI_CEX_LO or FEFCX
        //LB: -0.8
        //UB: 1.2
        (*xsec_param_lb)(31) = -0.8;
        (*xsec_param_ub)(31) = 1.2;

        //FSI_CEX_HI or FEFCXH
        //Not used in the 2018 analysis
        //LB: -0.8
        //UB: 1.2
        //(*xsec_param_lb)(31) = -0.8;
        //(*xsec_param_ub)(31) = 1.2;

        /*// Don't throw pF and 2p2h_shape
          (*xsec_param_throw)(1) = 0.0;
          (*xsec_param_throw)(2) = 0.0;
          (*xsec_param_throw)(6) = 0.0;
          (*xsec_param_throw)(7) = 0.0;

        // Don't throw BeRPA
        (*xsec_param_throw)(8) = 0.0;
        (*xsec_param_throw)(9) = 0.0;
        (*xsec_param_throw)(10) = 0.0;
        (*xsec_param_throw)(11) = 0.0;
        (*xsec_param_throw)(12) = 0.0;*/

        //Set CCQE parameters (only MAQE and pF) and all the 2p2h parameters to no throw 
        /* for(int i = 0; i < 3; i++){
           (*xsec_param_throw)(i) = 0.0;
           }
           for(int i = 3; i < 8; i++){
           (*xsec_param_throw)(i) = 0.0;
           }*/
		
    }

    //Set CCQE parameters (only MAQE and pF) and all the 2p2h parameters to unconstrained 
    for(int i = 0; i < 5; i++) {
      (*xsec_param_constrain)(i) = 0.0;
    }

    //Comparing to 2013, the normalization parameters, which only have 1 energy
    //bin, should have an energy bin stretching from 0 to 30 GeV.  So a TAxis
    //for each, with 1 bin.
    //Since we specify the name of the TAxis, just need one, call it
    //xsec_param_bins, filled above.
    //Fill the relevant information for them here too.

    //Set up the TAxis with the binning here.
    //1 bin going from 0.0 to 30.0
    TAxis* xsec_param_bins   = new TAxis(1, 0.0, 30.0);
    TAxis* xsec_coulomb_bins = new TAxis(1, 0.4, 0.6);

    //The easiest way to implement putting FSI first is to just prepare the
    //file as did before, relative to Asher's numbering, then rearrange it
    //write before writing the new versions to file.  So, create FSIFirst
    //versions of everything indexed by xsec parameter index that we are writing to
    //file.
    TMatrixDSym* xsec_covFSIFirst             = new TMatrixDSym(nParams);
    TVectorD*    xsec_param_priorFSIFirst     = new TVectorD(nParams);
    TVectorD*    xsec_param_nomFSIFirst       = new TVectorD(nParams);
    TVectorD*    xsec_param_lbFSIFirst        = new TVectorD(nParams);
    TVectorD*    xsec_param_ubFSIFirst        = new TVectorD(nParams);
    TObjArray*   xsec_param_namesFSIFirst     = new TObjArray(nParams);
    TVectorD*    xsec_param_throwFSIFirst     = new TVectorD(nParams);
    TVectorD*    xsec_param_fitFSIFirst       = new TVectorD(nParams);
    TVectorD*    xsec_param_constrainFSIFirst = new TVectorD(nParams);
    TVectorD*    xsec_param_splineFSIFirst    = new TVectorD(nParams);
    TVectorD*    xsec_param_targetFSIFirst    = new TVectorD(nParams);
    TVectorD*    xsec_param_binFSIFirst       = new TVectorD(nParams);
    TObjArray*   xsec_param_binsnameFSIFirst  = new TObjArray(nParams);
    TVectorD*    xsec_param_discreteFSIFirst  = new TVectorD(nParams);
    TVectorD*    xsec_param_weightFSIFirst    = new TVectorD(nParams);
    TObjArray*   xsec_norm_modesFSIFirst      = new TObjArray(nParams);
    TObjArray*   xsec_norm_elementsFSIFirst   = new TObjArray(nParams);
    TObjArray*   xsec_norm_nupdgFSIFirst      = new TObjArray(nParams);
    TVectorD*    xsec_param_idFSIFirst        = new TVectorD(nParams);
    TVectorD*    xsec_param_beginFSIFirst     = new TVectorD(nParams);
	
    //Loop over the full matrix.
    //Move indices 28 to 33 to 0-5, move 0-27 to 6-32  and leave the 34 as it is.
    //they are.
    for(int oldi = 0; oldi < nParams; oldi++) {

        int newi = 0;

        if(oldi < 27) {
            newi = oldi + 5;
        }
        else if(27 <= oldi && oldi <= 31) {
            newi = oldi - 27;
        }
        else {
            newi = oldi;
        }

        (*xsec_param_priorFSIFirst)(newi)     = (*xsec_param_prior)(oldi);
        (*xsec_param_nomFSIFirst)(newi)       = (*xsec_param_nom)(oldi);
        (*xsec_param_lbFSIFirst)(newi)        = (*xsec_param_lb)(oldi);
        (*xsec_param_ubFSIFirst)(newi)        = (*xsec_param_ub)(oldi);
        xsec_param_namesFSIFirst->AddAt(xsec_param_names->At(oldi),newi);
        (*xsec_param_throwFSIFirst)(newi)     = (*xsec_param_throw)(oldi);
        (*xsec_param_fitFSIFirst)(newi)       = (*xsec_param_fit)(oldi);
        (*xsec_param_constrainFSIFirst)(newi) = (*xsec_param_constrain)(oldi);
        (*xsec_param_splineFSIFirst)(newi)    = (*xsec_param_spline)(oldi);
        (*xsec_param_targetFSIFirst)(newi)    = (*xsec_param_target)(oldi);
        (*xsec_param_binFSIFirst)(newi)       = (*xsec_param_bin)(oldi);
        xsec_param_binsnameFSIFirst->AddAt(xsec_param_binsname->At(oldi),newi);
        (*xsec_param_discreteFSIFirst)(newi)  = (*xsec_param_discrete)(oldi);
        (*xsec_param_weightFSIFirst)(newi)    = (*xsec_param_weight)(oldi);
        xsec_norm_modesFSIFirst->AddAt(xsec_norm_modes->At(oldi),newi);
        xsec_norm_elementsFSIFirst->AddAt(xsec_norm_elements->At(oldi),newi);
        xsec_norm_nupdgFSIFirst->AddAt(xsec_norm_nupdg->At(oldi),newi);
        (*xsec_param_idFSIFirst)(newi)        = (*xsec_param_id)(oldi);

        //With the parameters taken care of, now take care of the covariance
        //matrix.
        for(int oldj = 0; oldj < nParams; oldj++) {

            int newj = 0;
	    
            if(oldj < 27) {
                newj = oldj + 5;
            }
            else if((27 <= oldj) && (oldj <= 31)) {
                newj = oldj - 27;
            }
            else {
                newj = oldj;
            }

            (*xsec_covFSIFirst)(newi,newj) = (*xsec_cov)(oldi,oldj);

        }
    }

    TObjArray* xsec_param_discreteNames = new TObjArray(3);
    xsec_param_discreteNames->AddAt(new TObjString("SF_RFG"), 0);
    xsec_param_discreteNames->AddAt(new TObjString("RPA_C"), 1);
    xsec_param_discreteNames->AddAt(new TObjString("COH_BS"), 2);
	
    //As of October, 2016 there is only 1 allowed discrete parameter
    //SF applied as RFG, no RPA (since BeRPA) and CCCoh_BS
    //So set that here.
    TVectorD* discreteCombo = new TVectorD(3);
    (*discreteCombo)(0)     = 0.0; //SF_RFG
    (*discreteCombo)(1)     = -1.0; //RPA_C
    (*discreteCombo)(2)     = 1.0; //CCCoh_BS
	
    //Now put these TVectorD into a TObjArray for convenient storage.
    //TObjArray* xsec_param_discreteCombinations = new TObjArray(2);
    //xsec_param_discreteCombinations->AddAt(discreteCombo1,0);
    //xsec_param_discreteCombinations->AddAt(discreteCombo2,1);
    //The only discrete combination still needs to go into a TObjArray for the
    //loading to work properly.
    TObjArray* xsec_param_discreteCombinations = new TObjArray(1);
    xsec_param_discreteCombinations->AddAt(discreteCombo,0);


    if(onesigma) {
		for (int i = 0; i < nParams-3; i++) {
        	if ((*xsec_param_throwFSIFirst)(i) > 0) {
                (*xsec_param_beginFSIFirst)(i) = (*xsec_param_priorFSIFirst)(i) + sqrt((*xsec_covFSIFirst)(i,i));
            }
		}
    }	
	
    //Save everything to file.
    outputFile->cd();

    //Original order
    //xsec_cov->Write("xsec_cov");
 	//xsec_param_prior->Write("xsec_param_prior");
 	//xsec_param_nom->Write("xsec_param_nom");
 	//xsec_param_lb->Write("xsec_param_lb");
 	//xsec_param_ub->Write("xsec_param_ub");
 	//xsec_param_names->Write("xsec_param_names",TObject::kSingleKey);
 	//xsec_param_throw->Write("xsec_param_throw");
 	//xsec_param_fit->Write("xsec_param_fit");
 	//xsec_param_constrain->Write("xsec_param_constrain");
 	//xsec_param_spline->Write("xsec_param_spline");
 	//xsec_param_target->Write("xsec_param_target");
 	//xsec_param_bin->Write("xsec_param_bin");
 	//xsec_param_binsname->Write("xsec_param_binsname", TObject::kSingleKey);

    //With FSI first.
    xsec_covFSIFirst->Write("xsec_cov");
    xsec_param_priorFSIFirst->Write("xsec_param_prior");
    xsec_param_nomFSIFirst->Write("xsec_param_nom");
    xsec_param_lbFSIFirst->Write("xsec_param_lb");
    xsec_param_ubFSIFirst->Write("xsec_param_ub");
    xsec_param_namesFSIFirst->Write("xsec_param_names",TObject::kSingleKey);
    xsec_param_throwFSIFirst->Write("xsec_param_throw");
    xsec_param_fitFSIFirst->Write("xsec_param_fit");
    xsec_param_constrainFSIFirst->Write("xsec_param_constrain");
    xsec_param_splineFSIFirst->Write("xsec_param_spline");
    xsec_param_targetFSIFirst->Write("xsec_param_target");
    xsec_param_binFSIFirst->Write("xsec_param_bin");
    xsec_param_binsnameFSIFirst->Write("xsec_param_binsname", TObject::kSingleKey);
    xsec_param_discreteFSIFirst->Write("xsec_param_discrete");
    xsec_param_weightFSIFirst->Write("xsec_param_weight");
    xsec_norm_modesFSIFirst->Write("xsec_norm_modes",TObject::kSingleKey);
    xsec_norm_elementsFSIFirst->Write("xsec_norm_elements",TObject::kSingleKey);
    xsec_norm_nupdgFSIFirst->Write("xsec_norm_nupdg",TObject::kSingleKey);
    xsec_param_idFSIFirst->Write("xsec_param_id");
    xsec_param_discreteNames->Write("xsec_param_discreteNames",TObject::kSingleKey);
    xsec_param_discreteCombinations->Write("xsec_param_discreteCombinations",TObject::kSingleKey);
	
    if(onesigma) {
        xsec_param_beginFSIFirst->Write("xsec_param_beginpoint",TObject::kSingleKey);
    }	
	
    //Common to both orderings.
    xsec_param_bins->Write("xsec_param_bins");
    xsec_coulomb_bins->Write("xsec_coulomb_bins");
		
    //With everything written, close the file.
    outputFile->Close();


}
