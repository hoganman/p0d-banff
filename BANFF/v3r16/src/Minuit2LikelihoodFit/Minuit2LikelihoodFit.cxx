#include "Minuit2LikelihoodFit.hxx"
#include "TMath.h"
#include "TF2.h"
#include "TMatrixD.h"
#include "TFile.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/MnSimplex.h"
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnPrint.h"
#include "Minuit2/MnHesse.h"
#include "Minuit2/MnMinos.h"
#include "Minuit2/MnScan.h"
#include "TGraph.h"
#include "TGraph2DErrors.h"
#include <sys/time.h>
#include "Minuit2/MnHesse.h"

Minuit2LikelihoodFit::Minuit2LikelihoodFit(BANFFInterfaceBase* inter, std::string outputFileName){


    interface = inter;

    outputFile = new TFile(outputFileName.c_str(),"RECREATE");

    minuit2_likelihood_fit_fcn = new Minuit2LikelihoodFitFCN();
    minuit2_likelihood_fit_fcn->AssignInterface(interface);
    migrad_covariance = new TMatrixDSym(1);
    migrad_hessian = new TMatrixDSym(1);
    params = new ROOT::Minuit2::MnUserParameters();
    fminuit = new TFitterMinuit();
    fcnMin = new TVectorD(1);


    iteration = 0;

    //Pre-compute the weights for each combination of discrete
    //parameters.
    for(int i = 0; i < interface->nMCEvents; i++){

        //Note: This does not actually internally change any fitParameters
        //values, so there is no need to reset anything anywhere.
        if(interface->useLowMem){
            interface->mcEvents[i]->SaveDiscreteParameterWeightsLowMem(interface->fitParameters);
        }
        else{
            interface->mcEvents[i]->SaveDiscreteParameterWeights(interface->fitParameters);
        }

    }

    //Copy the fit parameters private vector into a public array (with a public
    //length) to speed things up for the fit.
    interface->fitParameters->FillFastVariables();

    //Set the likelihood array (for marginalizing over the discrete parameter
    //combinations) to be equal to the number of discrete parameter
    //combinations.
    interface->like_array = new double[interface->fitParameters->nDiscreteParameterCombinations];


    //Before decomposing the parameters, print their values out.
    std::cout << "======================================" << std::endl;
    std::cout << "Parameter values before decomposition:" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    for(int i=0; i < interface->fitParameters->GetNumbSysts(); i++){
        std::cout << interface->fitParameters->GetParamName(i) <<": " << interface->fitParameters->GetCurrentValue(i) << " +/- " << 
            interface->fitParameters->GetParameterError(i) << std::endl;
    }
    std::cout << "======================================" << std::endl;

    //Some parameters may require decomposition in order to perform the fit.
    //This must be done before calling "InitializeParameters()" for the fit, so do
    //it now.
    //First, compute whether decomposition is necessary.
    interface->fitParameters->ComputeEigenDecompositionRequired();

    //If decomposition is required, do so, then print out the decomposed
    //values.
    if(interface->fitParameters->eigenDecompositionRequired){

        interface->fitParameters->DecomposeParameters();

        //Print out the parameters again after decomposition.
        std::cout << "======================================" << std::endl;
        std::cout << "Parameter values after decomposition:" << std::endl;
        std::cout << "--------------------------------------" << std::endl;
        for(int i=0; i < interface->fitParameters->GetNumbSysts(); i++){
            std::cout << interface->fitParameters->GetParamName(i) <<": " << interface->fitParameters->GetCurrentValue(i) << " +/- " << 
                interface->fitParameters->GetParameterError(i) << std::endl;
        }
        std::cout << "======================================" << std::endl;
    }

}


Minuit2LikelihoodFit::~Minuit2LikelihoodFit(){}

void Minuit2LikelihoodFit::Reset(){

    interface->fitParameters->ResetValues();
    delete fminuit;
    fminuit = new TFitterMinuit();
    minuit2_likelihood_fit_fcn = new Minuit2LikelihoodFitFCN();
    minuit2_likelihood_fit_fcn->AssignInterface(interface);

}

void Minuit2LikelihoodFit::InitializeParameters(){


  //If we're not starting with an empty parameter list, means that we've
  //looped around, and therefore need to delete and start fresh.
  std::vector<double> pars = params->Params();
  if(pars.size()!=0){
    delete params;
    params = new ROOT::Minuit2::MnUserParameters();
  }

  //Make sure the fitParameters are reset as well.
  interface->fitParameters->ResetValues();

  std::cout << "Initialize parameters" << std::endl;
  for(int i=0; i < interface->fitParameters->GetNumbSysts(); i++){

    std::cout << interface->fitParameters->GetParamName(i) << " ("
	      << interface->fitParameters->GetShortName(i) << ") : "
	      << interface->fitParameters->GetCurrentValue(i,true) << " +/- "
	      << interface->fitParameters->GetParameterError(i,true) << std::endl;
    
    //If the parameter has scan bounds, use them to set up the parameters
    //for the scan.
    if(interface->fitParameters->HasScanBounds(i)){
      params->Add(interface->fitParameters->GetShortName(i), interface->fitParameters->GetPriorValue(i),
		  interface->fitParameters->GetScanStepSize(i), interface->fitParameters->GetScanLowerBound(i),interface->fitParameters->GetScanUpperBound(i));
    }
    
    //Otherwise don't assign bounds.
    else{
      params->Add(interface->fitParameters->GetShortName(i), interface->fitParameters->GetPriorValue(i),
		  interface->fitParameters->GetScanStepSize(i));
    }
    
    
    //The parameters always have some kind of bound for the fit.
    fminuit->SetParameter(i,(interface->fitParameters->GetShortName(i)).c_str(), interface->fitParameters->GetPriorValue(i),
			  interface->fitParameters->GetParameterError(i)/10.0, interface->fitParameters->GetLowerBound(i),interface->fitParameters->GetUpperBound(i));
    
    
    //If the parameter is not to be varied, indicate to minuit that it is
    //fixed.
    //We also don't want to vary discrete parameters in the fit, so fix
    //them as well.
    if( (!(interface->fitParameters->GetDoVary(i))) || (interface->fitParameters->GetIsDiscrete(i)) ){
      std::cout << "Fixing parameter: " << interface->fitParameters->GetParamName(i) << "... Change the following to fit them:" << std::endl;
      std::cout << "     - interface->fitParameters->GetDoVary    ("<< i << "): " << interface->fitParameters->GetDoVary    (i) << "-> 1" << std::endl;
      std::cout << "     - Otherwise it's not a fittable parameter (interface->fitParameters->GetIsDiscrete("<< i << "): " << interface->fitParameters->GetIsDiscrete(i) << ")" << std::endl;
      fminuit->FixParameter(i);
    }else{
      std::cout << "Fitting parameter: " << interface->fitParameters->GetParamName(i) << std::endl;
    }
  }
  
}


void Minuit2LikelihoodFit::ScanParameters(){

    std::vector<double> pars = params->Params();
    TGraph** par_scans = new TGraph*[pars.size()];

    int ii=0;
    for(int i=0; i < interface->fitParameters->GetNumbSysts(); i++){
        if( (interface->fitParameters->GetDoVary(i)) || (!(interface->fitParameters->GetIsDiscrete(i)))){
            InitializeParameters(); 
            ROOT::Minuit2::MnScan mnscan_nd280(*minuit2_likelihood_fit_fcn, *params);
            par_scans[ii] = new TGraph();
            par_scans[ii]->SetName(Form("%s_scan",interface->fitParameters->GetParamName(i).c_str()));
            std::vector< std::pair<double,double> > pscan = mnscan_nd280.Scan(i,50,pars[ii]-5.0*10*3*params->Error(ii),pars[ii]+5.0*10*3*params->Error(ii) );

            for(unsigned int j=1; j<pscan.size(); j++){

                par_scans[i]->SetPoint(j-1,pscan[j].first,pscan[j].second);
            }

            ii++;
        }
    }

    outputFile->cd();

    for(unsigned int i=0; i<pars.size(); i++){
        par_scans[i]->Write();
    }

    outputFile->Close();


    for(unsigned int i=0; i<pars.size(); i++){ 
        par_scans[i]->Delete();
    }


}



void Minuit2LikelihoodFit::DoFit(){

    //Make sure we're pointed to the output file to write to.
    outputFile->cd();

    std::cout << "Set the FCN" << std::endl;

    //TODO: Replace?
    //if(simplex){
    //    minuit2_likelihood_fit_fcn->MatchBins(true);
    //}

    fminuit->SetMinuitFCN(minuit2_likelihood_fit_fcn);

    std::cout << "Create the minimizer" << std::endl;

    //This defaults to Migrad unless a different minimizer is supplied.
    fminuit->CreateMinimizer();

    std::cout << "Set the print level" << std::endl;
    fminuit->SetPrintLevel(2);

    std::cout << "Start the minimization!!" << std::endl;

    //TODO: Replace or remove?
    //if(simplex){
    //    fminuit->SetFitMethod("SIMPLEX");
    //}

    timeval tim;
    gettimeofday(&tim, NULL);
    double t0 = tim.tv_sec+(tim.tv_usec/1000000.0);

    fminuit->Minimize(0,0.01);
    std::cout << "Finished with the minimization!!" << std::endl;

    gettimeofday(&tim, NULL);
    double t1 = tim.tv_sec+(tim.tv_usec/1000000.0);

    std::cout << "Total time to minimize (s): " << (t1 - t0) << std::endl;
    std::cout << "Number of iterations to minimize: " << (interface->countApplyParamsAndCalcLLR) << std::endl;
    std::cout << "Time per fit function call (s): " << ((t1 - t0)/(double)(interface->countApplyParamsAndCalcLLR)) << std::endl;

    //Store the number of calls before proceeding to the Hessian calculation.
    int minimizationCalls = interface->countApplyParamsAndCalcLLR;


    //Get the fit status and chi2
    Double_t amin, edm, errdef;
    Int_t nvpar, nparx;
    fminuit->GetStats(amin, edm, errdef, nvpar, nparx); 
    std::cout << amin << " " << edm << " " << errdef << " " << nvpar << " " << nparx << std::endl;
    (*fcnMin)(0) = amin;
    fcnMin->Write(Form("fcnMin%d",iteration));

    //TODO: Replace or remove?
    //if(!simplex){
    if(true){
        
        //Variable required for "ExecuteCommand()" which no longer is used.
        //double args[1] = {0.};

        gettimeofday(&tim, NULL);
        double t2 = tim.tv_sec+(tim.tv_usec/1000000.0);

        //Try doing HESSE manually so as not to be saddled with the
        //sometimes insufficient nmaxcalls value hard coded into this function.
        //fminuit->ExecuteCommand("HESS",args, 0);
        ROOT::Minuit2::MnHesse hesse(fminuit->GetStrategy());
        const ROOT::Minuit2::FCNBase *fcn = fminuit->GetMinuitFCN();
        assert(fcn != 0);
      
        //Pull out the state here in const form.
        //Have to do some magic with the const casting in order to pull out the
        //state with overloaded public and protected methods.
        //See
        //http://stackoverflow.com/questions/1361618/const-overloading-public-private-lookup-in-c-class/1361664#1361664
        const TFitterMinuit& const_fminuit = *fminuit;
        const ROOT::Minuit2::MnUserParameterState& fitState = const_fminuit.State(); 

        //Unfortunately it looks like we can't modify the internal state of
        //TFitterMinuit
        ROOT::Minuit2::MnUserParameterState hesseOutput = hesse(*fcn,fitState,500000);

        gettimeofday(&tim, NULL);
        double t3 = tim.tv_sec+(tim.tv_usec/1000000.0);

        std::cout << "Finished HESS" << std::endl;

        std::cout << "Total time to finish HESS (s): " << (t3 - t2) << std::endl;
        std::cout << "Number of iterations for HESS: " << (interface->countApplyParamsAndCalcLLR - minimizationCalls) << std::endl;
        std::cout << "Time per fit function call (s): " << ((t3 - t2)/(double)((interface->countApplyParamsAndCalcLLR - minimizationCalls))) << std::endl;

        //How this was done when using TFitterMinuit's "ExecuteCommand"
        //function.  We access the state contents directly below.
//        int fi=0;
//        for(int i=0; i < interface->fitParameters->GetNumbSysts(); i++){
//            interface->fitParameters->SetParameterValue(i,fminuit->GetParameter(i));
//
//            if( (!(interface->fitParameters->GetDoVary(i))) || (interface->fitParameters->GetIsDiscrete(i)) ){
//                continue;
//            }
//
//            int fj=0;
//            for(int j=0; j < interface->fitParameters->GetNumbSysts(); j++){
//
//                if( (!(interface->fitParameters->GetDoVary(j))) || (interface->fitParameters->GetIsDiscrete(j))){
//                    continue;
//                }
//
//                if((fi >= fminuit->GetNumberFreeParameters()) || (fj >= fminuit->GetNumberFreeParameters()) ){
//                    std::cout << "Indices too large " << fi << " " << fj << std::endl;
//                }
//
//                interface->fitParameters->SetFitCovarianceMatrixElement(i,j,fminuit->GetCovarianceMatrixElement(fi,fj));
//                fj++;
//            }
//            fi++;
//
//
//
//        }
        
        int fi=0;
        for(int i=0; i < interface->fitParameters->GetNumbSysts(); i++){
            interface->fitParameters->SetParameterValue(i,hesseOutput.Value(i));

            if( (!(interface->fitParameters->GetDoVary(i))) || (interface->fitParameters->GetIsDiscrete(i)) ){
                continue;
            }

            int fj=0;
            for(int j=0; j < interface->fitParameters->GetNumbSysts(); j++){

                if( (!(interface->fitParameters->GetDoVary(j))) || (interface->fitParameters->GetIsDiscrete(j))){
                    continue;
                }

                //Can still use the fminuit references here for the number of
                //free parameters, because this will not have changed in the
                //Hessian.
                if((fi >= fminuit->GetNumberFreeParameters()) || (fj >= fminuit->GetNumberFreeParameters()) ){
                    std::cout << "Indices too large " << fi << " " << fj << std::endl;
                }

                interface->fitParameters->SetFitCovarianceMatrixElement(i,j,hesseOutput.Covariance()(fi,fj));
                fj++;
            }
            fi++;
        }
    }

    //Only need to undecompose if decomposed to begin with.
    if(interface->fitParameters->eigenDecompositionRequired){
      interface->fitParameters->UnDecomposeParameters(true);

      //Once the fit is complete and the parameters un-decomposed, print their
      //values to the terminal for easy readback.
      for(int i=0; i < interface->fitParameters->GetNumbSysts(); i++){
	std::cout << interface->fitParameters->GetParamName(i) << " ("
		  << interface->fitParameters->GetShortName(i) << ") : "
		  << interface->fitParameters->GetCurrentValue(i,true) << " +/- "
		  << interface->fitParameters->GetParameterError(i,true) << std::endl;
      }

    }

    //Otherwise, print off the parameters instead of trying to access the
    //undecomposed copy.
    else{

      for(int i=0; i < interface->fitParameters->GetNumbSysts(); i++){
	std::cout << interface->fitParameters->GetParamName(i) << " ("
		  << interface->fitParameters->GetShortName(i) << ") : "
		  << interface->fitParameters->GetCurrentValue(i,false) << " +/- "
		  << interface->fitParameters->GetParameterError(i,true) << std::endl;
      }
      
    }



    //Make sure that the following will write to the output file. 
    outputFile->cd();

    //Write the results of the fit to the output file.
    //We pass which fit we are on (iteration) whether to save the parameter
    //names (only need to once, when iteration == 0) and whether to save the
    //undecomposed parameter values (true).
    interface->fitParameters->WriteResults(iteration, (iteration==0), true);



    //TODO: This saving section needs a rework in concert with interface changes
    //and sample changes.
    //As with ThrowToys, this section is about saving a reaction code
    //breakdown, and as with ThrowToys, it can wait until a later release.

    //    if(iteration == 0){
    //
    //        //Loop over PDFs.
    //        for(unsigned int piter = 0; piter < fitPDFs.size(); piter++){
    //
    //
    //            //Loop over reaction modes.
    //            for(int riter = 0; riter < 9; riter++){
    //
    //                //Fill the bins for this reaction mode and
    //                //this throw.
    //                fitPDFs[piter]->FillBins(fitPDFs[piter]->lminThrow,fitPDFs[piter]->lminThrow,riter);
    //
    //                //Loop over samples
    //                for(int siter = 0; siter < fitPDFs[piter]->GetNSamples(); siter++){
    //
    //                    ND280SampleBase* nd280Sample = fitPDFs[piter]->GetND280SampleVector()[siter];
    //
    //
    //                    //If riter is zero, save the data histogram.
    //                    if(riter == 0){
    //
    //                        nd280Sample->SaveEngineOutput("data",piter,-1,0,true,false);
    //                    }
    //
    //                    //Otherwise, save thrownPredMC histograms.
    //
    //                    nd280Sample->SaveEngineOutput("pred",piter,fitPDFs[piter]->lminThrow,riter);
    //                }
    //            }
    //        }
    //    }


    //Save the post-fit MC prediction.  The parameters have already been
    //decomposed (if necessary) by this point.
    //We also want to save the contributions from each sample and each part of
    //the prior constraint at the best fit point to the output file, so we will do that here too.
    //Do all of this for each discrete parameter
    //combination.
    //Essentially reproduce the discrete parameter handling used in
    //BANFFInteferfaceBase.  This is possibly best moved there at some point.
    //---------------------------------------------------------------------------

    //The prior contraints don't depend on the discrete parameter combination.  This vector will
    //take the form flux, xsec, obsNorm, total.  (Where the individual terms
    //are calculated with individual functions, and the total using the same
    //function used in the fit.)  The individual ones will not include cross
    //correlations with parameters of different types in the prior.
    //Also note that this function is operating in the decomposed space still,
    //so if there were correlations between these types in the prior (there
    //usually are not) then this "Type" distinction does not apply at all.
    TVectorD* priorConstraint = new TVectorD(4);
    (*priorConstraint)(0) = interface->fitParameters->CalcChi2OfType(FLUX);

    //In this case, XSECRESP acts as a proxy for XSECRESP and XSECNORM.
    (*priorConstraint)(1) = interface->fitParameters->CalcChi2OfType(XSECRESP); 
    (*priorConstraint)(2) = interface->fitParameters->CalcChi2OfType(OBSNORM);
    (*priorConstraint)(3) = interface->fitParameters->CalcChi2();

    priorConstraint->Write(Form("FittedPriorTerm%d",iteration));


    //Before start looping over the discrete parameter combinations, set
    //continuousParamsComputed to false.
    interface->fitParameters->continuousParamsComputed = false;

    //The discrete parameters will not affect the matching, so start the loop
    //over them here.
    //We also want to save the totalsample contribution for each discrete parameter combination,
    //so set up the TVectorD objects to do that here.
    TVectorD* totalSampleContribs = new TVectorD(interface->fitParameters->nDiscreteParameterCombinations);

    //We'll also compute the complete minimized quantity, with the breakdown:
    TVectorD* minimizedQuantity = new TVectorD(1);

    for(int dpi = 0; dpi < interface->fitParameters->nDiscreteParameterCombinations; dpi++){

        //Loop over the parameters, and apply the relevant value for the
        //discrete parameters.
        for(int ip = 0; ip < interface->fitParameters->GetNumbSysts(); ip++){

            if(interface->fitParameters->GetIsDiscrete(ip)){

                interface->fitParameters->SetParameterValue(ip, interface->fitParameters->discreteParameterCombinations[dpi][ip]);
            }
        }

        //Change the fitParameters currentDiscreteParameterCombinationIndex to
        //the value of dpi, so that the weight calculation takes advantage of
        //time saving pre-calculations.
        interface->fitParameters->currentDiscreteParameterCombinationIndex = dpi;

        //This method calls the final total weight calculation, and builds the MC
        //predicted histogram for each sample.
        interface->BuildPredictedMC();

        //Let's only fill the reaction code plots for discrete parameter index
        //0.
        if(dpi == 0){
            interface->BuildReactionCodeBreakdowns();
        }

        //Since BuildPredictedMC() computes the event weights for all events,
        //we know that the continuous parameters have now been computed since
        //the fit parameter values were changed above.  Set
        //continuousParamsComputed to true to reflect this, allowing time
        //saving for any additional discrete parameter combinations.
        interface->fitParameters->continuousParamsComputed = true;

        //We'll store the total of all samples here.
        double sampleLLRContribs = 0.0;

        //A TVectorD to store all samples individually.
        TVectorD* individualSampleContribs = new TVectorD(interface->nSamples);

        for(int si = 0; si < interface->nSamples; si++){

            //Finalize the MC prediction before calculating the log likelihood
            //ratio contribution.
            interface->samples[si]->FinalizeMCPrediction();

            std::stringstream outName;
            outName << interface->samples[si]->name << "_postfit_" << dpi << "_" << iteration;

            //If we're on the zeroth dpi, save the reaction code breakdown.
            if(dpi == 0){
               
                interface->samples[si]->SaveReactionCodeBreakdown(interface->samples[si]->name);
                
            }
            
            interface->samples[si]->SaveMCPrediction(outName.str());

            double thisContrib = interface->samples[si]->CalcLLRContrib();

            sampleLLRContribs += thisContrib;

            (*individualSampleContribs)(si) = thisContrib;

            //Once the contribution is calculated and added to the total, reset the MC prediction for the next
            //iteration.
            interface->samples[si]->ResetMCPrediction();
        }


        //With the total computed, put it in the TVectorD for saving.
        (*totalSampleContribs)(dpi) = sampleLLRContribs;

        //With the -2*log likelihood ratio determined for this discrete
        //parameter combination, put it into this combination's slot in the
        //likelihood array, but as a likelihood (i.e. -2*(what is coming from
        //the sample.)
        interface->like_array[dpi] = -0.5*sampleLLRContribs;

        //Save the individual sample contribution vector to the file before
        //looping around to the next discrete parameter combination.
        individualSampleContribs->Write(Form("FittedIndividualSampleContribs_%d_%d",dpi,iteration));

    }

    //Now that we're done with the discrete parameter combinations, save the
    //fitted total sample contribs.
    totalSampleContribs->Write(Form("FittedTotalSampleContribs%d",iteration));

    //Since we are done looping over the discrete parameters, and we always
    //want the event weight computation to default to discrete parameter
    //nominal values (the zeroth index) unless otherwise requested, reset the
    //currentDiscreteParameterCombinationIndex to 0 here.
    interface->fitParameters->currentDiscreteParameterCombinationIndex = 0;


    //At this point, have a log likelihood for each discrete parameter
    //combination.  Now go through and find the maximum one.
    int ml_index = -1;
    double ml = -1.0e20;
    for(int i = 0; i < interface->fitParameters->nDiscreteParameterCombinations; i++){

        if(interface->like_array[i] > ml){

            ml_index = i;
            ml = interface->like_array[i];
        }
    }


    //Now we need the sum of the exponential of the difference from the maximum
    //for each entry that is not the maximum likelihood entry.
    double sumOfNonMaxDiff = 0.0;
    for(int i = 0; i < interface->fitParameters->nDiscreteParameterCombinations; i++){

        if(i == ml_index){
            continue;
        }

        sumOfNonMaxDiff += TMath::Exp(interface->like_array[i] - interface->like_array[ml_index]);

    }

    //Now, the total sample coontribution is -2.0*ln(L_avg), which is equal to:
    //-2.0*(ln(L_max) + ln(1 + sumOfNonMax) - ln(nDiscreteParameterCombinations))
    double totalSampleContrib = -2.0*(interface->like_array[ml_index] + TMath::Log(1.0 + sumOfNonMaxDiff)
            - TMath::Log((double)(interface->fitParameters->nDiscreteParameterCombinations)));

    (*minimizedQuantity)(0) = totalSampleContrib + interface->fitParameters->CalcChi2();
    minimizedQuantity->Write(Form("FittedMinimizedQuantity%d",iteration));

    //-----------------------------------------------------------------------------

    //Increase the value of the iteration variable.  Only becomes important if
    //doing more than 1 fit while MC is loaded into memory (typically for
    //validation purposes).
    iteration++;

    //Now that the fit is complete, Reset it in case we're looping back around.
    Reset();

}
