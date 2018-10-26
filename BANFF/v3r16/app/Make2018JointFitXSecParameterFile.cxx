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
///Usage: Make2016XSecParameterFile inputFile outputFile forToyThrows onesigma
int main(int argc, char** argv){

    if(argc != 5){
        std::cout << "Warning: Unexpected number of arguments." << std::endl;
        exit(1);
    }


    TFile* inputFile = new TFile(argv[1]);
    TFile* outputFile = new TFile(argv[2], "RECREATE");
    bool forToyThrows = atoi(argv[3]);
    bool onesigma = atoi(argv[4]);

    TMatrixDSym* xsec_cov_in = (TMatrixDSym*)(inputFile->Get("xsec_cov"));
    TVectorD* xsec_param_prior_in = (TVectorD*)(inputFile->Get("xsec_param_prior"));
    TVectorD* xsec_param_nom_in = (TVectorD*)(inputFile->Get("xsec_param_nom"));
    TVectorD* xsec_param_lb_in = (TVectorD*)(inputFile->Get("xsec_param_lb"));
    TVectorD* xsec_param_ub_in = (TVectorD*)(inputFile->Get("xsec_param_ub"));
    TObjArray* xsec_param_names_in = (TObjArray*)(inputFile->Get("xsec_param_names"));
    TMatrixD* xsec_param_id_in = (TMatrixD*)(inputFile->Get("xsec_param_id"));
    TObjArray* xsec_norm_modes_in = (TObjArray*)(inputFile->Get("xsec_norm_modes"));
    TObjArray* xsec_norm_elements_in = (TObjArray*)(inputFile->Get("xsec_norm_elements"));
    TObjArray* xsec_norm_nupdg_in = (TObjArray*)(inputFile->Get("xsec_norm_nupdg"));

	//Add CCCoh_BS at the end 
    int nParams = xsec_param_prior_in->GetNrows() + 5;
    int inputParams = xsec_param_prior_in->GetNrows();

    
    //Make newer, bigger versions of each of these arrays that can include the
    //discrete parameters.
    TMatrixDSym* xsec_cov = new TMatrixDSym(nParams);
    TVectorD* xsec_param_prior = new TVectorD(nParams);
    TVectorD* xsec_param_nom = new TVectorD(nParams);
    TVectorD* xsec_param_lb = new TVectorD(nParams);
    TVectorD* xsec_param_ub = new TVectorD(nParams);
    TObjArray* xsec_param_names = new TObjArray(nParams);
    TObjArray* xsec_norm_modes = new TObjArray(nParams);
    TObjArray* xsec_norm_elements = new TObjArray(nParams);
    TObjArray* xsec_norm_nupdg = new TObjArray(nParams);
    TVectorD* xsec_param_id = new TVectorD(nParams);
	
    //Fill the parameters with their old values.  
    for(int i = 0; i < nParams; i++){
        if(i < inputParams){
        	(*xsec_param_prior)(i) = (*xsec_param_prior_in)(i);
        	(*xsec_param_nom)(i) = (*xsec_param_nom_in)(i);
        	(*xsec_param_lb)(i) = (*xsec_param_lb_in)(i);
			(*xsec_param_ub)(i) = (*xsec_param_ub_in)(i);
			(*xsec_param_id)(i) = (*xsec_param_id_in)(i,0);
        	xsec_param_names->AddAt(xsec_param_names_in->At(i),i);
            if(i < inputParams-6){
                xsec_norm_modes->AddAt(xsec_norm_modes_in->At(i),i);
            	xsec_norm_elements->AddAt(xsec_norm_elements_in->At(i),i);
            	xsec_norm_nupdg->AddAt(xsec_norm_nupdg_in->At(i),i);
            }
		}
        else{
            (*xsec_param_nom)(i) = 0.0;
			(*xsec_param_id)(i) = -1;
        }
        for(int j = 0; j < nParams; j++){
            if((i < inputParams) && (j < inputParams)){
          	  	//Just copy it over.
           	 	(*xsec_cov)(i,j) = (*xsec_cov_in)(i,j);
			}
            else if (i == inputParams+3) {
                if (j == i) { (*xsec_cov)(i,j) = 0.0004000001; }
                else if (j == inputParams+4) { (*xsec_cov)(i,j) = -0.0002; }
                else {(*xsec_cov)(i,j) = 0.0; }
            }
            else if (i == inputParams+4) {
                if (j == i) { (*xsec_cov)(i,j) = 0.0001000001; }
                else if (j == inputParams+3) { (*xsec_cov)(i,j) = -0.0002; }
                else {(*xsec_cov)(i,j) = 0.0; }
            }
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

    //Indicates whether the parameter is discrete or not. This year no discrete parameter.
    TVectorD* xsec_param_discrete = new TVectorD(nParams);

    //Indicates whether the parameter is weight or not.
    TVectorD* xsec_param_weight = new TVectorD(nParams);


    // New for 2015 analysis - CCCoh reweight to BS model
    xsec_param_names->AddAt(new TObjString("COH_BS"),inputParams+2);
    (*xsec_param_prior)(inputParams+2) = 1.0;
    (*xsec_param_lb)(inputParams+2) = -1.0;
    (*xsec_param_ub)(inputParams+2) = 2.0;
//  (*xsec_param_throw)(inputParams+2) = 0;

    //SF_RFG.
    //Its prior value needs to be 0, which weights SF to RFG.
    //Set the lower and upper bounds to something acceptable.
    xsec_param_names->AddAt(new TObjString("SF_RFG"),inputParams);
    (*xsec_param_prior)(inputParams) = 0.0;
    (*xsec_param_lb)(inputParams) = -1.0;
    (*xsec_param_ub)(inputParams) = 2.0;
	//  (*xsec_param_throw)(inputParams) = 0;

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
    xsec_param_names->AddAt(new TObjString("RPA_C"),inputParams+1);
    (*xsec_param_prior)(inputParams+1) = -1.0;
    (*xsec_param_lb)(inputParams+1) = -2.0;
    (*xsec_param_ub)(inputParams+1) = 2.0;
	//  (*xsec_param_throw)(inputParams+1) = 0;

    // New for 2018 analysis - nu/nubar Coulomb Uncertainty
    TVectorD *mode = new TVectorD(1);
    (*mode)(0) = -1;
    TVectorD *elements = new TVectorD(1);
    (*elements)(0) = -1;
    TVectorD *nuPDG = new TVectorD(2);
    (*nuPDG)(0) = 14;
    (*nuPDG)(1) = 12;
    TVectorD *nubarPDG = new TVectorD(2);
    (*nubarPDG)(0) = -14;
    (*nubarPDG)(1) = -12;

    xsec_param_names->AddAt(new TObjString("NU_COULOMB"),inputParams+3);
    (*xsec_param_prior)(inputParams+3) = 1.0;
    (*xsec_param_lb)(inputParams+3) = 0.0;
    //(*xsec_param_ub)(inputParams+3) = 2.0;
    //(*xsec_param_throw)(inputParams+3) = 1;
    xsec_norm_modes->AddAt(mode,inputParams+3);
    xsec_norm_elements->AddAt(elements,inputParams+3);
    xsec_norm_nupdg->AddAt(nuPDG,inputParams+3);
    (*xsec_param_discrete)(inputParams+3) = 0.0;
    (*xsec_param_spline)(inputParams+3) = 0.0;

    xsec_param_names->AddAt(new TObjString("NUBAR_COULOMB"),inputParams+4);
    (*xsec_param_prior)(inputParams+4) = 1.0;
    (*xsec_param_lb)(inputParams+4) = 0.0;
    //(*xsec_param_ub)(inputParams+4) = 2.0;
    //(*xsec_param_throw)(inputParams+4) = 1;
    xsec_norm_modes->AddAt(mode,inputParams+4);
    xsec_norm_elements->AddAt(elements,inputParams+4);
    xsec_norm_nupdg->AddAt(nubarPDG,inputParams+4);
    (*xsec_param_discrete)(inputParams+4) = 0.0;
    (*xsec_param_spline)(inputParams+4) = 0.0;

    //Now loop over all parameters, and set various variables to appropriate
    //defaults.
    for(int i = 0; i < nParams; i++){

        //throw, fit, and constain should all be 1.0
        (*xsec_param_throw)(i) = 1.0;
        (*xsec_param_fit)(i) = 1.0;
        (*xsec_param_constrain)(i) = 1.0;

        //default spline to 0.  Will change later for those that are non-zero.
        //(*xsec_param_spline)(i) = 0.0;
        //Directly reading from the input, >=0 is spline, -1 is norm and -2 is functionnal
	    if(i<inputParams){  
		    if ((*xsec_param_id_in)(i,0)== -1 || (*xsec_param_id_in)(i,0)== -2) (*xsec_param_spline)(i) = 0.0;
			else if ((*xsec_param_id_in)(i,0) >= 0) {
				(*xsec_param_spline)(i) = 1.0;
	    		//The prior value for all resp parameters with prior different than 0 needs to
	    		//have 1.0 subtracted from it in order to be used in spline calculations.
	    		//Do the same for the LB and UB.
	    		//The value of nominal is not used, so don't worry about it.
				if(!((*xsec_param_prior)(i)- 0.1 < 0.0)){
			        (*xsec_param_prior)(i) = 0.0;
			        (*xsec_param_lb)(i) = (*xsec_param_lb)(i) - 1.0;
			        (*xsec_param_ub)(i) = (*xsec_param_ub)(i) - 1.0;
				}
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
        if (i == inputParams+3 || i == inputParams+4) { xsec_param_binsname->AddAt(new TObjString("xsec_coulomb_bins"),i); }
        else { xsec_param_binsname->AddAt(new TObjString("xsec_param_bins"),i); }

        //Set the discrete flag to 0.0 for all.  Will set discrete parameters
        //to 1.0 later.
        (*xsec_param_discrete)(i) = 0.0;

        //Set the weight flag to 0.0 for all.  Will set weight parameters
        //to 1.0 later.
        (*xsec_param_weight)(i) = 0.0;

        std::cout << std::left << std::setw(23) << (((TObjString*)(xsec_param_names->At(i)))->GetString()).Data()<<std::setw(23) <<(*xsec_param_prior)(i)<<std::endl;
        if(xsec_norm_elements->At(i) != NULL) std::cout << (((TObjString*)(xsec_param_names->At(i)))->GetString()).Data()<<" has target material specified"<<std::endl;
        if(xsec_norm_modes->At(i) != NULL) {
			std::cout << (((TObjString*)(xsec_param_names->At(i)))->GetString()).Data() << " is mode " << std::endl;
			((TVectorD*)(xsec_norm_modes->At(i)))->Print();
		}

    }

	//Set bounds of 2p2h_shape parameters
    /*(*xsec_param_lb)(6) =  -2;
    (*xsec_param_ub)(6) = 2;
    (*xsec_param_lb)(7) =  -2;
    (*xsec_param_ub)(7) = 2;*/
		
	//Set prior of CC1pi parameters
    (*xsec_param_prior)(13) =  -0.049505;
    (*xsec_param_prior)(14) = 0.12632;
    (*xsec_param_prior)(15) = -0.261538;
	
	//Now set the BeRPA U parameter to not vary in the fit
    (*xsec_param_throw)(12) = 0.0;

    // Don't fit the nue parameters
    (*xsec_param_throw)(16) = 0.0;
    (*xsec_param_throw)(17) = 0.0;

    // Nor the far detector parameters
    (*xsec_param_throw)(22) = 0.0;
    (*xsec_param_throw)(24) = 0.0;

    //Now set the discrete flag for CCCoh_BS, RPA_C and SF_RFG to true. And weight to true for CCCoh_BS
    //They are at inputParams, inputParams+1 and inputParams+2 
    (*xsec_param_discrete)(inputParams) = 1.0;
    (*xsec_param_spline)(inputParams) = 1.0;
    (*xsec_param_discrete)(inputParams+1) = 1.0;
    (*xsec_param_spline)(inputParams+1) = 1.0;
    (*xsec_param_discrete)(inputParams+2) = 1.0;
    (*xsec_param_spline)(inputParams+2) = 0.0;

    //And weight to true for CCCoh_BS
    (*xsec_param_weight)(inputParams+2) = 1.0;

	(*xsec_param_spline)(inputParams+1) = 0.0;    
	(*xsec_param_spline)(inputParams) = 0.0;
	
    if(forToyThrows){

        std::cout << "Applying FSI bounds for toy throws." << std::endl;

        //The FSI parameters have splines that have limited ranges of validity.
        //Although we do not want to restrict the fit, it would be prudent to only
        //throw toys within that range of validity.  We add these lower and upper
        //bounds now.
        //16: FSI_INEL_LO 
        //LB: -0.8
        //UB: 1.2
        (*xsec_param_lb)(25) = -0.8;
        (*xsec_param_ub)(25) = 1.2;

        //17: FSI_INEL_HI
        //LB: -0.8
        //UB: 1.2
        (*xsec_param_lb)(26) = -0.8;
        (*xsec_param_ub)(26) = 1.2;


        //18: FSI_PI_PROD
        //LB: -0.8
        //UB: 1.2
        (*xsec_param_lb)(27) = -0.8;
        (*xsec_param_ub)(27) = 1.2;



        //19: FSI_PI_ABS
        //LB: -0.6
        //UB: 0.9
        (*xsec_param_lb)(28) = -0.6;
        (*xsec_param_ub)(28) = 0.9;


        //20:FSI_CEX_LO
        //LB: -0.8
        //UB: 1.2
        (*xsec_param_lb)(29) = -0.8;
        (*xsec_param_ub)(29) = 1.2;


        //21:FSI_CEX_HI
        //LB: -0.8
        //UB: 1.2
        (*xsec_param_lb)(30) = -0.8;
        (*xsec_param_ub)(30) = 1.2;

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
    
    if(!forToyThrows){
        //Set CCQE parameters (only MAQE and pF) and all the 2p2h parameters to unconstrained 
        for(int i = 0; i < 5; i++){
           (*xsec_param_constrain)(i) = 0.0;
        }
    }
	
    //Comparing to 2013, the normalization parameters, which only have 1 energy
    //bin, should have an energy bin stretching from 0 to 30 GeV.  So a TAxis
    //for each, with 1 bin.
    //Since we specify the name of the TAxis, just need one, call it
    //xsec_param_bins, filled above.
    //Fill the relevant information for them here too.

    //Set up the TAxis with the binning here.
    //1 bin going from 0.0 to 30.0
    TAxis* xsec_param_bins = new TAxis(1, 0.0, 30.0);
    TAxis* xsec_coulomb_bins = new TAxis(1, 0.4, 0.6);

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
    TObjArray* xsec_norm_modesFSIFirst = new TObjArray(nParams);
    TObjArray* xsec_norm_elementsFSIFirst = new TObjArray(nParams);
    TObjArray* xsec_norm_nupdgFSIFirst = new TObjArray(nParams);
    TVectorD* xsec_param_idFSIFirst = new TVectorD(nParams);
	TVectorD* xsec_param_beginFSIFirst = new TVectorD(nParams);
	
    //Loop over the full matrix.
    //Move indices 28 to 33 to 0-5, move 0-27 to 6-32  and leave the 34 as it is.
    //they are.
    for(int oldi = 0; oldi < nParams; oldi++){

        int newi = 0;

        if(oldi < 25){
            newi = oldi + 6;
        }
        else if(25 <= oldi && oldi <= 30){
            newi = oldi - 25;
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
        xsec_norm_modesFSIFirst->AddAt(xsec_norm_modes->At(oldi),newi);
        xsec_norm_elementsFSIFirst->AddAt(xsec_norm_elements->At(oldi),newi);
        xsec_norm_nupdgFSIFirst->AddAt(xsec_norm_nupdg->At(oldi),newi);
        (*xsec_param_idFSIFirst)(newi) = (*xsec_param_id)(oldi);

        //With the parameters taken care of, now take care of the covariance
        //matrix.
        for(int oldj = 0; oldj < nParams; oldj++){

            int newj = 0;
            if(oldj < 25){
                newj = oldj + 6;
            }
            else if((25 <= oldj) && (oldj <= 30)){
                newj = oldj - 25;
            }
            else{
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
    (*discreteCombo)(0) = 0.0; //SF_RFG
    (*discreteCombo)(1) = -1.0; //RPA_C
    (*discreteCombo)(2) = 1.0; //CCCoh_BS
	
    //Now put these TVectorD into a TObjArray for convenient storage.
    //TObjArray* xsec_param_discreteCombinations = new TObjArray(2);
    //xsec_param_discreteCombinations->AddAt(discreteCombo1,0);
    //xsec_param_discreteCombinations->AddAt(discreteCombo2,1);
    //The only discrete combination still needs to go into a TObjArray for the
    //loading to work properly.
    TObjArray* xsec_param_discreteCombinations = new TObjArray(1);
    xsec_param_discreteCombinations->AddAt(discreteCombo,0);


    if(onesigma){
		for (int i = 0; i < inputParams;i++){
        	if ((*xsec_param_throwFSIFirst)(i)> 0) (*xsec_param_beginFSIFirst)(i) = (*xsec_param_priorFSIFirst)(i)+ sqrt((*xsec_covFSIFirst)(i,i));
		}
    }	
	
    //Save everything to file.
    outputFile->cd();

//    //Original order
//    xsec_cov->Write("xsec_cov");
//	xsec_param_prior->Write("xsec_param_prior");
//	xsec_param_nom->Write("xsec_param_nom");
//	xsec_param_lb->Write("xsec_param_lb");
//	xsec_param_ub->Write("xsec_param_ub");
//	xsec_param_names->Write("xsec_param_names",TObject::kSingleKey);
//	xsec_param_throw->Write("xsec_param_throw");
//	xsec_param_fit->Write("xsec_param_fit");
//	xsec_param_constrain->Write("xsec_param_constrain");
//	xsec_param_spline->Write("xsec_param_spline");
//	xsec_param_target->Write("xsec_param_target");
//	xsec_param_bin->Write("xsec_param_bin");
//	xsec_param_binsname->Write("xsec_param_binsname", TObject::kSingleKey);

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
	
    if(onesigma){
	    xsec_param_beginFSIFirst->Write("xsec_param_beginpoint",TObject::kSingleKey);
    }	
	
    //Common to both orderings.
    xsec_param_bins->Write("xsec_param_bins");
    xsec_coulomb_bins->Write("xsec_coulomb_bins");
		
    //With everything written, close the file.
    outputFile->Close();


}
