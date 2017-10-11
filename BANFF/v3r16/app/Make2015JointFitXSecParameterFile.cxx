#include "TFile.h"
#include "TMatrixDSym.h"
#include "TVectorD.h"
#include <iostream>
#include "TObjArray.h"
#include "TObjString.h"
#include "TAxis.h"
#include <stdlib.h>

///Read in an input file provided containing the XSec parameter information for
///2014, and use this (making changes as needed) to assemble the XSec input for
///2014 used by the BANFF fit.
///Usage: Make2014XSecParameterFile inputFile outputFile forToyThrows
int main(int argc, char** argv){

    if(argc != 4){
        std::cout << "Warning: Unexpected number of arguments." << std::endl;
    }


    TFile* inputFile = new TFile(argv[1]);
    TFile* outputFile = new TFile(argv[2], "RECREATE");
    bool forToyThrows = atoi(argv[3]);

    TMatrixDSym* xsec_cov_in = (TMatrixDSym*)(inputFile->Get("xsec_cov"));
    TVectorD* xsec_param_prior_in = (TVectorD*)(inputFile->Get("xsec_param_prior"));
    TVectorD* xsec_param_nom_in = (TVectorD*)(inputFile->Get("xsec_param_nom"));
    TVectorD* xsec_param_lb_in = (TVectorD*)(inputFile->Get("xsec_param_lb"));
    TVectorD* xsec_param_ub_in = (TVectorD*)(inputFile->Get("xsec_param_ub"));
    TObjArray* xsec_param_names_in = (TObjArray*)(inputFile->Get("xsec_param_names"));

    //Set the number of parameters.
    //For now, we're putting SF_RFG in the place of RPA_O.
    //Need to add RPA_C and SF_RFG  + CCCoh_BS at the end, so put 3 more on.
    // Subtract one for the CCoh on carbon
    int nParams = xsec_param_prior_in->GetNrows() + 3 - 1;

    
    //Make newer, bigger versions of each of these arrays that can include the
    //discrete parameters.
    TMatrixDSym* xsec_cov = new TMatrixDSym(nParams);
    TVectorD* xsec_param_prior = new TVectorD(nParams);
    TVectorD* xsec_param_nom = new TVectorD(nParams);
    TVectorD* xsec_param_lb = new TVectorD(nParams);
    TVectorD* xsec_param_ub = new TVectorD(nParams);
    TObjArray* xsec_param_names = new TObjArray(nParams);

    //Fill the parameters with their old values.  Fill 1s on the diagonal for
    //RPA_C and SF_RFG.  The parameters themselves have their prior, lb, ub,
    //and names filled elsewhere.  Just put in 0s for nom, since it is never
    //used but should still be valid.
    for(int i = 0; i < nParams; i++){

        if(i < nParams-3){
            if(i < 12){
                (*xsec_param_prior)(i) = (*xsec_param_prior_in)(i);
                (*xsec_param_nom)(i) = (*xsec_param_nom_in)(i);
                (*xsec_param_lb)(i) = (*xsec_param_lb_in)(i);
                (*xsec_param_ub)(i) = (*xsec_param_ub_in)(i);
                xsec_param_names->AddAt(xsec_param_names_in->At(i),i);
            }
            else{
                (*xsec_param_prior)(i) = (*xsec_param_prior_in)(i+1);
                (*xsec_param_nom)(i) = (*xsec_param_nom_in)(i+1);
                (*xsec_param_lb)(i) = (*xsec_param_lb_in)(i+1);
                (*xsec_param_ub)(i) = (*xsec_param_ub_in)(i+1);
                xsec_param_names->AddAt(xsec_param_names_in->At(i+1),i);
            }
        }
        else{
            (*xsec_param_nom)(i) = 0.0;
        }

        for(int j = 0; j < nParams; j++){

            //If we're in the old covariance matrix, just copy it over.
            if((i < nParams-3) && (j < nParams-3)){
                if(i < 12 && j < 12){
                    (*xsec_cov)(i,j) = (*xsec_cov_in)(i,j);
                }
                else if(i >= 12 && j < 12){
                    (*xsec_cov)(i,j) = (*xsec_cov_in)(i+1,j);
                }
                else if(i < 12 && j >= 12){
                    (*xsec_cov)(i,j) = (*xsec_cov_in)(i,j+1);
                }
                else{
                    (*xsec_cov)(i,j) = (*xsec_cov_in)(i+1,j+1);
                }

            }
            //Otherwise, we're in the RPA_C and SF_RFG rows and columns.
            else{

                //If we're on the diagonal, put in 1.0.
                if( i == j){

                    (*xsec_cov)(i,j) = 1.0;
                }

                //Otherwise, put in 0.0;
                else{

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

    //Whether the parameter has a Gaussian constraint or whether to use just a
    //flat prior.
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
    //the bins it uses.  This year, all xsec norm parameters only have one bin,
    //the same for all, so we're just copy the same name throughout.
    TObjArray* xsec_param_binsname = new TObjArray(nParams);

    //Indicates whether the parameter is discrete or not.
    TVectorD* xsec_param_discrete = new TVectorD(nParams);

    //Indicates whether the parameter is weight or not.
    TVectorD* xsec_param_weight = new TVectorD(nParams);


    //TODO: Fill the new variables properly, as well as add axes for the xsec
    //norm parameters. Rearrange/delete entries from the covariance matrix as
    //needed.
    //Start by replacing the RPA_O slot entry with SF_RFG.
    //Its prior value needs to be 0, which weights SF to RFG.
    //Set the lower and upper bounds to something acceptable.
    xsec_param_names->AddAt(new TObjString("SF_RFG"),nParams-1);
    (*xsec_param_prior)(nParams-1) = 0.0;
    (*xsec_param_lb)(nParams-1) = -1.0;
    (*xsec_param_ub)(nParams-1) = 2.0;

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
    (*xsec_param_prior)(nParams-2) = 1.0;
    (*xsec_param_lb)(nParams-2) = -2.0;
    (*xsec_param_ub)(nParams-2) = 2.0;

    // New for 2015 analysis - CCCoh reweight to BS model
    xsec_param_names->AddAt(new TObjString("COH_BS"),nParams-3);
    (*xsec_param_prior)(nParams-3) = 1.0;
    (*xsec_param_lb)(nParams-3) = -1.0;
    (*xsec_param_ub)(nParams-3) = 2.0;


    //Now loop over all parameters, and set various variables to appropriate
    //defaults.
    for(int i = 0; i < nParams; i++){

        //throw, fit, and constain should all be 1.0
        (*xsec_param_throw)(i) = 1.0;
        (*xsec_param_fit)(i) = 1.0;
        (*xsec_param_constrain)(i) = 1.0;

        //default spline to 0.  Will change later for those that are non-zero.
        (*xsec_param_spline)(i) = 0.0;

        //default target to -1.  Will change later for those with a target
        //specification required.
        (*xsec_param_target)(i) = -1.0;

        //default bin to 0.  XSecResp parameters don't use the bin, and all
        //XSecNorm parameters only have the 1 bin.
        (*xsec_param_bin)(i) = 0.0;

        //Since this only gets checked for xsec_norm parameters, all of which
        //use the same binning, might as well
        //set it to the same value for all entries here. 
        xsec_param_binsname->AddAt(new TObjString("xsec_param_bins"),i);

        //Set the discrete flag to 0.0 for all.  Will set discrete parameters
        //to 1.0 later.
        (*xsec_param_discrete)(i) = 0.0;

        //Set the weight flag to 0.0 for all.  Will set weight parameters
        //to 1.0 later.
        (*xsec_param_weight)(i) = 0.0;


    }

    //Now set the discrete flag for CCCoh_BS, RPA_C and SF_RFG to true.
    //They are at nParams-2 and nParams-1
    (*xsec_param_discrete)(nParams-3) = 1.0;
    (*xsec_param_discrete)(nParams-2) = 1.0;
    (*xsec_param_discrete)(nParams-1) = 1.0;

    //Now set the weight flag for CCCoh_BS, RPA_C and SF_RFG to true.
    //They are at nParams-2 and nParams-1
    (*xsec_param_weight)(nParams-3) = 1.0;

    //The prior value for MAQE (index 0) through BgSclRes (index 9) needs to
    //have 1.0 subtracted from it in order to be used in spline calculations.
    //Do the same for the LB and UB.
    //The value of nominal is not used, so don't worry about it.
    //These also all have splines associated with them, so set
    //xsec_param_spline to true for them.
    //Same holds true for CCNuE at index 10.
    for(int i = 0; i < 11; i++){

        (*xsec_param_prior)(i) = (*xsec_param_prior)(i) - 1.0;
        (*xsec_param_lb)(i) = (*xsec_param_lb)(i) - 1.0;
        (*xsec_param_ub)(i) = (*xsec_param_ub)(i) - 1.0;
        (*xsec_param_spline)(i) = 1.0;
    }


    //DISMPISHP has a spline and is at index 11.  Doesn't need any subtractions
    //to be performed though.
    (*xsec_param_spline)(11) = 1.0;


    //Parameters 16 through 22 also have splines associated with them.
    //Don't require any modification though.
    for(int i = 17; i < 23; i++){

        (*xsec_param_spline)(i) = 1.0;

    }

    //The last three parameters, CCCoh_BS, SF_RFG and RPA_C should have TGraphs associated
    //with them instead of TSpline3.  So, set xsec_param_spline to 0 for them.
    (*xsec_param_spline)(23) = 0.0;
    (*xsec_param_spline)(24) = 0.0;
    (*xsec_param_spline)(25) = 0.0;

    //The MEC_C (index 2) and MEC_O (index 5) splines are linear responses, and are thus best
    //represented by TGraphs instead of splines.
    (*xsec_param_spline)(2) = 0.0;
    (*xsec_param_spline)(5) = 0.0;


    if(forToyThrows){

        std::cout << "Applying FSI bounds for toy throws." << std::endl;

        //The FSI parameters have splines that have limited ranges of validity.
        //Although we do not want to restrict the fit, it would be prudent to only
        //throw toys within that range of validity.  We add these lower and upper
        //bounds now.
        //16: FSI_INEL_LO 
        //LB: -0.8
        //UB: 1.2
        (*xsec_param_lb)(17) = -0.8;
        (*xsec_param_ub)(17) = 1.2;

        //17: FSI_INEL_HI
        //LB: -0.8
        //UB: 1.2
        (*xsec_param_lb)(18) = -0.8;
        (*xsec_param_ub)(18) = 1.2;


        //18: FSI_PI_PROD
        //LB: -0.8
        //UB: 1.2
        (*xsec_param_lb)(19) = -0.8;
        (*xsec_param_ub)(19) = 1.2;



        //19: FSI_PI_ABS
        //LB: -0.6
        //UB: 0.9
        (*xsec_param_lb)(20) = -0.6;
        (*xsec_param_ub)(20) = 0.9;


        //20:FSI_CEX_LO
        //LB: -0.8
        //UB: 1.2
        (*xsec_param_lb)(21) = -0.8;
        (*xsec_param_ub)(21) = 1.2;


        //21:FSI_CEX_HI
        //LB: -0.8
        //UB: 1.2
        (*xsec_param_lb)(22) = -0.8;
        (*xsec_param_ub)(22) = 1.2;

    }

    //Set CCQE parameters to unconstrained
    for(int i = 0; i < 3; i++){
       (*xsec_param_constrain)(i) = 0.0;
    }
    (*xsec_param_constrain)(4) = 0.0;
    (*xsec_param_constrain)(5) = 0.0;
    
    //Temp fix since CC Coh O and CC Coh C are degenerate in 2015b
//    (*xsec_cov)(12,12) = (*xsec_cov)(12,12) + 0.00000000000001;
//    (*xsec_cov)(13,13) = (*xsec_cov)(13,13) + 0.00000000000001;
    //(*xsec_param_fit)(15) = 0.0;
    //(*xsec_param_fit)(17) = 0.0;
    //(*xsec_param_fit)(13) = 0.0;
    //Use more information from this web page to make sure everything else is
    //correct.
    //http://www.t2k.org/asg/oagroup/inputs/201415/niwg2015

    //Comparing to 2013, the normalization parameters, which only have 1 energy
    //bin, should have an energy bin stretching from 0 to 30 GeV.  So a TAxis
    //for each, with 1 bin.
    //Since we specify the name of the TAxis, just need one, call it
    //xsec_param_bins, filled above.
    //Fill the relevant information for them here too.

    //Set up the TAxis with the binning here.
    //1 bin going from 0.0 to 30.0
    TAxis* xsec_param_bins = new TAxis(1, 0.0, 30.0);

    //CCCOH_C_0
    //It's at index 12.
    //Target material is Carbon (12)
    //Mode is 16
    //Must also include -16 for anti-neutrinos.
//    (*xsec_param_target)(12) = 12.0;
//    TVectorD* CCCOH_C_0_targets = new TVectorD(2);
//    (*CCCOH_C_0_targets)(0) = 12.0;
//    (*CCCOH_C_0_targets)(1) = 16.0;
//    TVectorD* CCCOH_C_0_reactionCodes = new TVectorD(2);
//    (*CCCOH_C_0_reactionCodes)(0) = 16.0;
//    (*CCCOH_C_0_reactionCodes)(1) = -16.0;

    //CCCOH_O_0
    //It's at index 13
    //Target material is Oxygen (16)
    //Mode is 16.
    //Must also include -16 for anti-neutrinos.
//    (*xsec_param_target)(13) = 16.0;
//    (*xsec_param_target)(13) = 999.0;
    TVectorD* CCCOH_O_0_reactionCodes = new TVectorD(2);
    (*CCCOH_O_0_reactionCodes)(0) = 16.0;
    (*CCCOH_O_0_reactionCodes)(1) = -16.0;
    TVectorD* CCCOH_O_0_targets = new TVectorD(2);
    (*CCCOH_O_0_targets)(0) = 12.0;
    (*CCCOH_O_0_targets)(1) = 16.0;

    //NCCOH_0
    //It's at index 14.
    //Target material is all, the default (-1)
    //Mode is 36.
    TVectorD* NCCOH_0_reactionCodes = new TVectorD(2);
    (*NCCOH_0_reactionCodes)(0) = 36.0;
    (*NCCOH_0_reactionCodes)(1) = -36.0;


    //NCOTHER_0 is now NCOTHER_NEAR_0
    //It's at index 15.
    //Target material is all, the default (-1)
    //Modes are 38, 39, 41, 42, 43, 44, 45, 46, 51, 52
    //Inclue the negative modes for antineutrinos.
    TVectorD* NCOTHER_NEAR_0_reactionCodes = new TVectorD(20);
    (*NCOTHER_NEAR_0_reactionCodes)(0) = 38.0;
    (*NCOTHER_NEAR_0_reactionCodes)(1) = 39.0;
    (*NCOTHER_NEAR_0_reactionCodes)(2) = 41.0;
    (*NCOTHER_NEAR_0_reactionCodes)(3) = 42.0;
    (*NCOTHER_NEAR_0_reactionCodes)(4) = 43.0;
    (*NCOTHER_NEAR_0_reactionCodes)(5) = 44.0;
    (*NCOTHER_NEAR_0_reactionCodes)(6) = 45.0;
    (*NCOTHER_NEAR_0_reactionCodes)(7) = 46.0;
    (*NCOTHER_NEAR_0_reactionCodes)(8) = 51.0;
    (*NCOTHER_NEAR_0_reactionCodes)(9) = 52.0;
    (*NCOTHER_NEAR_0_reactionCodes)(10) = -38.0;
    (*NCOTHER_NEAR_0_reactionCodes)(11) = -39.0;
    (*NCOTHER_NEAR_0_reactionCodes)(12) = -41.0;
    (*NCOTHER_NEAR_0_reactionCodes)(13) = -42.0;
    (*NCOTHER_NEAR_0_reactionCodes)(14) = -43.0;
    (*NCOTHER_NEAR_0_reactionCodes)(15) = -44.0;
    (*NCOTHER_NEAR_0_reactionCodes)(16) = -45.0;
    (*NCOTHER_NEAR_0_reactionCodes)(17) = -46.0;
    (*NCOTHER_NEAR_0_reactionCodes)(18) = -51.0;
    (*NCOTHER_NEAR_0_reactionCodes)(19) = -52.0;

    //Reaction codes for NC1GAMMA and ncother far
    //We don't actually fit these
    TVectorD* NC1GAMMA_0_reactionCodes = new TVectorD(4);
    (*NC1GAMMA_0_reactionCodes)(0) = -38;
    (*NC1GAMMA_0_reactionCodes)(1) = -39;
    (*NC1GAMMA_0_reactionCodes)(2) = 39;
    (*NC1GAMMA_0_reactionCodes)(3) = 38;

    TVectorD* NCOTHER_FAR_0_reactionCodes = new TVectorD(2);
    (*NCOTHER_FAR_0_reactionCodes)(0) = 38;
    (*NCOTHER_FAR_0_reactionCodes)(1) = 39;

    (*xsec_param_target)(14) = 999.0;
    (*xsec_param_target)(16) = 999.0;
    //Disable any parameters we don't actually want to fit
    //These are NCOTHER_FAR, NC1GAMMA, and CCCOH_O (functionality given to CCCOH C to help with fit issues
//    (*xsec_param_fit)(14) = 0.0;
//    (*xsec_param_fit)(16) = 0.0;
//    (*xsec_param_fit)(12) = 0.0;
    //The easiest way to implement putting FSI first is to just prepare the
    //file as did before, relative to Asher's numbering, then rearrange it
    //write before writing the new versions to file.  So, create FSIFirst
    //versions of everything indexed by xsec parameter index that we are writing to
    //file.
    TMatrixDSym* xsec_covFSIFirst = new TMatrixDSym(nParams);
    TVectorD* xsec_param_priorFSIFirst = new TVectorD(nParams);
    TVectorD* xsec_param_nomFSIFirst = new TVectorD(nParams);
    TVectorD* xsec_param_lbFSIFirst = new TVectorD(nParams);
    TVectorD* xsec_param_ubFSIFirst = new TVectorD(nParams);
    TObjArray* xsec_param_namesFSIFirst = new TObjArray(nParams);
    TVectorD* xsec_param_throwFSIFirst = new TVectorD(nParams);
    TVectorD* xsec_param_fitFSIFirst = new TVectorD(nParams);
    TVectorD* xsec_param_constrainFSIFirst = new TVectorD(nParams);
    TVectorD* xsec_param_splineFSIFirst = new TVectorD(nParams);
    TVectorD* xsec_param_targetFSIFirst = new TVectorD(nParams);
    TVectorD* xsec_param_binFSIFirst =  new TVectorD(nParams);
    TObjArray* xsec_param_binsnameFSIFirst = new TObjArray(nParams);
    TVectorD* xsec_param_discreteFSIFirst = new TVectorD(nParams);
    TVectorD* xsec_param_weightFSIFirst = new TVectorD(nParams);

    //Loop over the full matrix.
    //Move indices 16 to 21 to 0-5, move 0-15 to 6-21, and keep 22-23 where
    //they are.
    for(int oldi = 0; oldi < nParams; oldi++){

        int newi;

        if(oldi < 17){

            newi = oldi + 6;
        }
        else if(17 <= oldi && oldi <= 22){

            newi = oldi - 17;

        }
        else{

            newi = oldi;
        }

        (*xsec_param_priorFSIFirst)(newi) = (*xsec_param_prior)(oldi);
        (*xsec_param_nomFSIFirst)(newi) = (*xsec_param_nom)(oldi);
        (*xsec_param_lbFSIFirst)(newi) = (*xsec_param_lb)(oldi);
        (*xsec_param_ubFSIFirst)(newi) = (*xsec_param_ub)(oldi);
        xsec_param_namesFSIFirst->AddAt(xsec_param_names->At(oldi),newi);
        (*xsec_param_throwFSIFirst)(newi) = (*xsec_param_throw)(oldi);
        (*xsec_param_fitFSIFirst)(newi) = (*xsec_param_fit)(oldi);
        (*xsec_param_constrainFSIFirst)(newi) = (*xsec_param_constrain)(oldi);
        (*xsec_param_splineFSIFirst)(newi) = (*xsec_param_spline)(oldi);
        (*xsec_param_targetFSIFirst)(newi) = (*xsec_param_target)(oldi);
        (*xsec_param_binFSIFirst)(newi) = (*xsec_param_bin)(oldi);
        xsec_param_binsnameFSIFirst->AddAt(xsec_param_binsname->At(oldi),newi);
        (*xsec_param_discreteFSIFirst)(newi) = (*xsec_param_discrete)(oldi);
        (*xsec_param_weightFSIFirst)(newi) = (*xsec_param_weight)(oldi);

        //With the parameters taken care of, now take care of the covariance
        //matrix.
        for(int oldj = 0; oldj < nParams; oldj++){

            int newj;

            if(oldj < 17){

                newj = oldj + 6;
            }
            else if((17 <= oldj) && (oldj <= 22)){

                newj = oldj - 17;

            }
            else{

                newj = oldj;
            }


            (*xsec_covFSIFirst)(newi,newj) = (*xsec_cov)(oldi,oldj);

        }
    }

    //Discrete parameters have acceptable combinations that must be defined
    //here.
    //The discrete parameter names are stored in "xsec_param_discreteNames", in
    //the order their values will take in a TVectorD of each allowed
    //combination.
    //There are only three (at least for now): CCCoh_BS, SF_RFG and RPA_C.
    TObjArray* xsec_param_discreteNames = new TObjArray(3);
    xsec_param_discreteNames->AddAt(new TObjString("SF_RFG"), 0);
    xsec_param_discreteNames->AddAt(new TObjString("RPA_C"), 1);
    xsec_param_discreteNames->AddAt(new TObjString("COH_BS"), 2);

    //There are two allowed configurations here.  In both cases, SF_RFG is 0,
    //which is SF->RFG tuning applied in the spline.
    //RPA_C is either 0 (prior, non-relativistic RPA correction)
    //of 1 (relativistic RPA correction.)
    //NB: The first discrete combination should be the same as the prior (i.e.
    //the nominal value for the toys or fit.)
    //    TVectorD* discreteCombo1 = new TVectorD(2);
    //    (*discreteCombo1)(0) = 0.0; //SF_RFG
    //    (*discreteCombo1)(1) = 0.0; //RPA_C
    //
    //    TVectorD* discreteCombo2 = new TVectorD(2);
    //    (*discreteCombo2)(0) = 0.0; //SF_RFG
    //    (*discreteCombo2)(1) = 1.0; //RPA_C

    //As of January 22, 2015 there is only 1 allowed discrete parameter
    //combination, SF->RFG applied, and relativistic RPA correction.  So set
    //that here.
    TVectorD* discreteCombo = new TVectorD(3);
    (*discreteCombo)(0) = 0.0; //SF_RFG
    (*discreteCombo)(1) = 1.0; //RPA_C
    (*discreteCombo)(2) = 1.0; //CCCoh_BS

    //Now put these TVectorD into a TObjArray for convenient storage.
    //TObjArray* xsec_param_discreteCombinations = new TObjArray(2);
    //xsec_param_discreteCombinations->AddAt(discreteCombo1,0);
    //xsec_param_discreteCombinations->AddAt(discreteCombo2,1);
    //The only discrete combination still needs to go into a TObjArray for the
    //loading to work properly.
    TObjArray* xsec_param_discreteCombinations = new TObjArray(1);
    xsec_param_discreteCombinations->AddAt(discreteCombo,0);

    //Save everything to file.
    outputFile->cd();

    //Original order
    //    xsec_cov->Write("xsec_cov");
    //    xsec_param_prior->Write("xsec_param_prior");
    //    xsec_param_nom->Write("xsec_param_nom");
    //    xsec_param_lb->Write("xsec_param_lb");
    //    xsec_param_ub->Write("xsec_param_ub");
    //    xsec_param_names->Write("xsec_param_names",TObject::kSingleKey);
    //    xsec_param_throw->Write("xsec_param_throw");
    //    xsec_param_fit->Write("xsec_param_fit");
    //    xsec_param_constrain->Write("xsec_param_constrain");
    //    xsec_param_spline->Write("xsec_param_spline");
    //    xsec_param_target->Write("xsec_param_target");
    //    xsec_param_bin->Write("xsec_param_bin");
    //    xsec_param_binsname->Write("xsec_param_binsname", TObject::kSingleKey);

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

    //Common to both orderings.
    xsec_param_bins->Write("xsec_param_bins");
//    CCCOH_C_0_targets->Write("CCCOH_C_0_targets");
//    CCCOH_C_0_reactionCodes->Write("CCCOH_C_0_reactionCodes");
    CCCOH_O_0_targets->Write("CCCOH_O_0_targets");
    CCCOH_O_0_reactionCodes->Write("CCCOH_O_0_reactionCodes");
    NCCOH_0_reactionCodes->Write("NCCOH_0_reactionCodes");
    NCOTHER_NEAR_0_reactionCodes->Write("NCOTHER_NEAR_0_reactionCodes");
    NC1GAMMA_0_reactionCodes->Write("NC1GAMMA_0_reactionCodes");
    NCOTHER_FAR_0_reactionCodes->Write("NCOTHER_FAR_0_reactionCodes");
    //Discrete parameter combination information.
    xsec_param_discreteNames->Write("xsec_param_discreteNames",TObject::kSingleKey);
    xsec_param_discreteCombinations->Write("xsec_param_discreteCombinations",TObject::kSingleKey);

    //With everything written, close the file.
    outputFile->Close();


}
