#include "BANFFInterfaceBase.hxx"
#include "Parameters.hxx"
#include <sstream>
#include <omp.h>

BANFFInterfaceBase::BANFFInterfaceBase(int nSamp, BANFFSampleBase** samp, FitParameters* fitParms, std::string diagnostic){

    nSamples = nSamp;
    samples = samp;
    fitParameters = fitParms;

    //Set the execution counter (for timing purposes) to 0.
    countApplyParamsAndCalcLLR = 0;

    //Set the low memory usage flag to whatever the parameters file specifies.
    useLowMem = ND::params().GetParameterI("BANFF.UseLowMemoryVersion");

    //First, parse the input files.  This will store the data and MC file
    //locations, broken down by run period (and by the type for MC) in
    //convenient maps.
    ParseInputFiles();
    diagnostic_name = diagnostic;
    //Set up the summary tree using the MC files read in ParseInputFiles.
    sumTree = new WeightFile(mcFiles);

    //Set up multi-threading nCores variable
    const char* nthreads = getenv("OMP_NUM_THREADS");
    nCores = ( nthreads != NULL ? atoi( nthreads ) : 1 );
    if ( nCores < 2 ) {
	std::cout << "Warning: multithreading disabled in BANFFInterfacBase."
		  << std::endl;
    }

}

BANFFInterfaceBase::~BANFFInterfaceBase(){

}

void BANFFInterfaceBase::ParseInputFiles(){


    //Get the file list from the parameters file.
    file_list = ND::params().GetParameterS("BANFF.InputFileList");


    //The format of the file list is:
    //dataFileList.txt MCFileList.txt runPeriod
    //Key difference this year is that sand/mec are broken down by run period.
    //i.e. They would be included in the broken down lists and labelled as
    //such, while the other MC is labelled as well (e.g. magnet, basket, etc.)
    //The dataFileList would just be a list of data files for the run period,
    //as before.

    //Same parsing of the runPeriodsFile used in WeightFile.
    //Add the file and the run period to the fileName/RunPeriod pair.
    ifstream inputFile(file_list.c_str(), std::ios::in);

    std::cout << inputFile << std::endl;

    if (!inputFile) {
        std::cerr << "Cannot open input file '" << file_list << "'. Exit!" << std::endl;
        return;
    }

    std::string dataString;
    std::string mcString;
    std::string period;


    while (inputFile >> dataString >> mcString >> period ) {


        //Start by dealing with the data files.

        //Open the text file containing the data files.
        ifstream dataFile(dataString.c_str(), std::ios::in);

        //The string that the line information from the data file goes
        //into.
        std::string subFileData;

        std::cout << "Data sub file: " << dataString << std::endl;
	if ( !dataFile ) {
	    std::cout << "Warning: Could not open " << mcString << " for reading."
		      << std::endl;
	}

        //Loop through the data file list, picking out the individual file and
        //adding them to the dataFiles.
        while(dataFile >> subFileData){

            //For the given run period, add this data file to the vector.
            dataFiles[period].push_back(subFileData);
            std::cout << "Using data file: " << subFileData << std::endl;
            std::cout << dataFiles.size() << std::endl;

        }

        dataFile.close();



        //With the data files for this run period handled, deal with  MC.  The format is
        //fileType fileName
        //It is being added to a pair in that order.
        ifstream mcFile(mcString.c_str(), std::ios::in);

        std::string fileTypeMC;
        std::string subFileMC;

        std::cout << "MC sub file: " << mcString << std::endl;
	if ( !mcFile ) {
	    std::cout << "Warning: Could not open " << mcString << " for reading."
		      << std::endl;
	}

        while(mcFile >> fileTypeMC >> subFileMC){

            mcFiles[period].push_back(std::make_pair(fileTypeMC,subFileMC));
            std::cout << "Using MC file: " << subFileMC << std::endl;
            std::cout << mcFiles.size() << std::endl;

        }
        mcFile.close();
    }

    inputFile.close();


    //Debug code.
    std::cout << "Number of data run periods: " << dataFiles.size() << std::endl;
    std::cout << "Number of MC run periods: " << mcFiles.size() << std::endl;
    std::cout << "Data file information" << std::endl;

    for(std::map<std::string,std::vector<std::string> >::iterator it = dataFiles.begin(); it!= dataFiles.end(); it++){

        std::cout << "Run period: " << it->first << std::endl;
        std::cout << "Data Files:" << std::endl;
        for(unsigned int i = 0; i < it->second.size(); i++){

            std::cout << (it->second)[i] << std::endl;

        }

        std::cout << "===========================================" << std::endl;
        std::cout << "MC files:" << std::endl;
        for(unsigned int i = 0; i < mcFiles[it->first].size(); i++){
            
            std::cout << ((mcFiles[it->first])[i]).first << " " << ((mcFiles[it->first])[i]).second << std::endl;
        }

    }
    



}

void BANFFInterfaceBase::LoadSummaryTreeInfo(BANFFEventBase* event){


    //Load the content of the summary tree entry corresponding to this event.
    sumTree->GetEntry(event->GetSumTreeIndex());

    //The number of samples (i.e. the size of the arrays storing the summary
    //tree information.)
    event->SetNSamples(sumTree->NSamples);

    //The neutrino energy from the summary tree, in GeV.
    event->SetNuEnergy(sumTree->Enu);

    //The Q2 from the summary tree.
    event->SetQ2(sumTree->Q2);
	
    //The neutrino code from the summary tree.  Corresponds to the PDG code for
    //the neutrino.
    event->SetNuFlavor(sumTree->NeutrinoCode);

    //The flux correction for the event, put into the tree from the beam flux
    //tuning files.
    event->SetFluxWeight(sumTree->FluxWeight);
    
    //The reaction code for the event as read from the RooTrackerVertex into
    //the summary tree.
    event->SetReactionCode(sumTree->ReactionCode);

    //The target material that the neutrino interaction occurred on, as read in
    //from the RooTrackerVertex information in the summary tree.
    event->SetTargetMaterial(sumTree->TgtMat);

    //Load the relevant XSec response functions into the event.
    //If a program parameter has requested the low memory version, use that.
    if(useLowMem){
        fitParameters->LoadXSecResponsesLowMem(event,sumTree);
    }
    else{
        fitParameters->LoadXSecResponses(event,sumTree);

        //The command that copies the content from the one interaction code it
        //is stored for into every interaction code slot in the array.
        //Comment this out when not using it for a specific purpose.
        //event->CopySummaryTreeInfo();
    }

    //Store for checking the matching.
    if(event->sample != NULL){
        sumTreeEnu = sumTree->Enu[event->sample->sampleID];
        sumTreeTruthVtx = sumTree->TruthVtx[event->sample->sampleID];
        sumTreeNuPDG = sumTree->NeutrinoCode[event->sample->sampleID];
    }
}


void BANFFInterfaceBase::ComputeEventWeights(){

//These weight calculations can be done independently for each event, therefore
//multithreading can be used here to speed things up.
#pragma omp parallel for num_threads(nCores) schedule(dynamic,nMCEvents/nCores)
    for(int i = 0; i < nMCEvents; i++){

        if(mcEvents[i]->sample != NULL){
            //If we have the information saved to use it, use
            //SaveTotalWeightFast.
            if(fitParameters->numbSystsFast != -1){
                
                if(useLowMem){
					/*std::cout<<"=========================================================="<<std::endl;
					std::cout<<"Event number : "<<mcEvents[i]->GetSumTreeIndex()<< std::endl;
					std::cout<<"Event reaction code : "<<mcEvents[i]->GetReactionCode()<< std::endl;
					std::cout<<"Event Q2 : "<<mcEvents[i]->GetQ2()<< std::endl;
					std::cout<<"Event Target : "<<mcEvents[i]->GetTargetMaterial()<< std::endl;
					std::cout<<"Fit parameter size"<<fitParameters->GetNumbSysts()<<std::endl;*/
					if(mcEvents[i]->GetQ2() > 0) mcEvents[i]->SaveTotalWeightFastLowMem(fitParameters, mcEvents[i]->GetQ2(), mcEvents[i]->GetNuFlavor());
					else mcEvents[i]->SaveTotalWeightFastLowMem(fitParameters, 0, mcEvents[i]->GetNuFlavor());
					//std::cout<<"=========================================================="<<std::endl;
				}
                else{
					if(mcEvents[i]->GetQ2() > 0) mcEvents[i]->SaveTotalWeightFast(fitParameters, mcEvents[i]->GetQ2(), mcEvents[i]->GetNuFlavor());
					else mcEvents[i]->SaveTotalWeightFast(fitParameters, 0, mcEvents[i]->GetNuFlavor());
                }
            }
            //Otherwise, use the original version.
		    else{
				/*std::cout<<"=========================================================="<<std::endl;
				std::cout<<"Event number : "<<mcEvents[i]->GetSumTreeIndex()<< std::endl;
				std::cout<<"Event reaction code : "<<mcEvents[i]->GetReactionCode()<< std::endl;
				std::cout<<"Event Q2 : "<<mcEvents[i]->GetQ2()<< std::endl;
				std::cout<<"Event Target : "<<mcEvents[i]->GetTargetMaterial()<< std::endl;
				std::cout<<"Fit parameter size"<<fitParameters->GetNumbSysts()<<std::endl;*/
				if(mcEvents[i]->GetQ2() > 0)mcEvents[i]->SaveTotalWeight(fitParameters, mcEvents[i]->GetQ2(), mcEvents[i]->GetNuFlavor());
				else mcEvents[i]->SaveTotalWeight(fitParameters, 0, mcEvents[i]->GetNuFlavor());
				//std::cout<<"=========================================================="<<std::endl;
			 }
        }
    }

}

void BANFFInterfaceBase::BuildData(){

    //Loop over the stored data events, and use the stored sample pointer to
    //add them to the sample.
    for(int i = 0; i < nDataEvents; i++){

        if(dataEvents[i]->sample != NULL){
            dataEvents[i]->sample->AddDataEvent(dataEvents[i]);
        }
    }

}

void BANFFInterfaceBase::BuildNominalMC(){


    //First, loop through the events to apply any weights.  This is done before
    //any toys are thrown, so this represents any corrections a nominal set of
    //toys would give, plus flux and POT weights.
    ComputeEventWeights();

    //Loop over the stored mc events, and use the stored sample pointer to
    //add that event to the sample.
    for(int i = 0; i < nMCEvents; i++){

        if(mcEvents[i]->sample != NULL){
            mcEvents[i]->sample->AddNominalMCEvent(mcEvents[i]);
        }
    }


}


void BANFFInterfaceBase::BuildReactionCodeBreakdowns(){

    //We do not need to compute event weights again, because this should only be
    //called after another method has already computed the weights.
    for(int i = 0; i < nMCEvents; i++){

        if(mcEvents[i]->sample != NULL){
            mcEvents[i]->sample->AddMCEventToReactionCodeBreakdown(mcEvents[i]);
        }
    }
}


void BANFFInterfaceBase::BuildPredictedMC(){

    //First, loop through the events to apply any weights.  These are
    //calculated based on the current value of the fit parameters.
    ComputeEventWeights();

    //Loop over the stored mc events, and use the stored sample pointer to
    //add that event to the sample.
    //    bool debugPrinted = false;
    //    int lastDebug = 0;
#pragma omp parallel for num_threads(nCores) schedule(dynamic)//,nSamples/nCores)
    for(int j = 0; j < nSamples; ++j){
        for(int i = 0; i < nMCEvents; i++){
            if(mcEvents[i]->sample != NULL){
                if(mcEvents[i]->sample->sampleID != samples[j]->sampleID) continue;
                mcEvents[i]->sample->AddPredictedMCEvent(mcEvents[i]);
            }

            //            //TODO: For debugging.
             /*           if((i - lastDebug) > 10000){
			                debugPrinted = false;
			            }
						
						            
						if(!debugPrinted){

                         	std::cout << "=========================================================================" << std::endl;
						  	 std::cout << "Total Weight for event " << i << " is "  << mcEvents[i]->GetTotalWeight() << std::endl;
						  	 std::cout << "Flux weight for event " << i << " is " << mcEvents[i]->GetFluxWeight() << std::endl;
						  	 std::cout << "POT weight for event " << i << " is " << mcEvents[i]->GetPOTWeight() << std::endl;
						   	std::cout << "Detector weight for event " << i << " is " << mcEvents[i]->GetDetWeight() << std::endl;
						   	std::cout << "XSec weight for event " << i << " is " << mcEvents[i]->GetXsecWeight() << std::endl;
						  	 std::cout << "Flux parameter index: " << mcEvents[i]->GetFluxParameter() << std::endl;
						  	 std::cout << "XSec norm parameter index: " << mcEvents[i]->GetXsecNormParameter() << std::endl; 
			                debugPrinted = true;
		              	  	lastDebug = i;
				         }*/
			

        }
    }

}

void BANFFInterfaceBase::ThrowToys(int nToys, std::string outputFileName){


    //Start by creating the output file.
    TFile* outFile = new TFile(outputFileName.c_str(),"RECREATE");

    //Set the low memory flag to false, because it is unsupported for toy
    //throwing because it requires fast variables which are not implemented for
    //toys at present.
    useLowMem = false;

    //Create a TObject to save the number of toys thrown, so it can be easily
    //accessed by any code dealing with the toys.
    TObject* numberOfToys = new TObject();
    numberOfToys->SetUniqueID(nToys);
    numberOfToys->Write("nToys");

    //Create a TObjArray storing the sample names in the proper order as
    //TObjString objects, so that the names of the samples can easily be retrieved
    //from the output toy file.
    TObjArray* sampleNames = new TObjArray(nSamples,0);
    for(int si = 0; si < nSamples; si++){

        sampleNames->AddAt(new TObjString((samples[si]->name).c_str()),si);

    }

    //Need to specify to write the TObjArray as 1 objects instead of breaking
    //out into its individual pieces for the save.
    sampleNames->Write("sampleNames",TObject::kSingleKey);

    //Build and save the data and mc nominal histograms for each sample.
    //This makes use of the original processing of the events on loading, and
    //in the case of NominalMC, uses a nominal set of weights.
    BuildData();
    BuildNominalMC();
    for(int si = 0; si < nSamples; si++){

        std::string dataName = samples[si]->name + "_data";
        std::string mcName = samples[si]->name + "_nomMC";
        samples[si]->SaveData(dataName);
        samples[si]->SaveNominalMC(mcName);

    }

    //Create some arrays to save the thrown parameters.
    TVectorD **thrown_flux_parms = new TVectorD*[nToys];
    TVectorD **thrown_xsec_parms = new TVectorD*[nToys];
    TVectorD **thrown_det_parms = new TVectorD*[nToys];
    TVectorD **thrown_obsNorm_parms = new TVectorD*[nToys];

    //Initialize the parameter throws
    fitParameters->InitializeThrows();

    //Now, throw the toys.
    for(int toy_iter=0; toy_iter<nToys; toy_iter++){

        //For every 100 toys, print out which toy we're on.
       // if(toy_iter%100==0){

            std::cout << "Toy " << toy_iter << std::endl;
			//}

        //Make the TVector big enough to store each individual type of
        //parameter value present.
        thrown_flux_parms[toy_iter] = new TVectorD(fitParameters->nFluxParms);
        thrown_xsec_parms[toy_iter] = new TVectorD(fitParameters->nXSecParms);
        thrown_det_parms[toy_iter] = new TVectorD(fitParameters->nDetParms);
        thrown_obsNorm_parms[toy_iter] = new TVectorD(fitParameters->nObsNormParms);

        //Throw the parameters.
        //fitParameters->DoThrow();
        fitParameters->DoThrowRTOOB();

        int flux_iter = 0;
        int xsec_iter = 0;
        int det_iter = 0;
        int obsNorm_iter = 0;

        for(int i=0; i<fitParameters->GetNumbSysts(); i++){

            //Save the flux parameter values to the flux parameter TVectorD.
            if(fitParameters->GetParamType(i)==FLUX){
                (*(thrown_flux_parms[toy_iter]))(flux_iter) = fitParameters->GetCurrentValue(i);
                //std::cout<<"FLUX "<< fitParameters->GetParamName(i)<<std::endl;
				flux_iter++;
            } 

            //Save the XSec parameter values (both Response and Normalization)
            //to the xsec parameter TVectorD.
            else if(fitParameters->GetParamType(i)==XSECRESP || fitParameters->GetParamType(i)==XSECNORM || fitParameters->GetParamType(i)==XSECFUNC){
                (*(thrown_xsec_parms[toy_iter]))(xsec_iter) = fitParameters->GetCurrentValue(i);
                //std::cout<<"XSEC "<< fitParameters->GetParamName(i)<<std::endl;
                xsec_iter++;
            } 

            //Save the detector parameter values to the detector parameter
            //TVectorD.
            else if(fitParameters->GetParamType(i)==DET){
                (*(thrown_det_parms[toy_iter]))(det_iter) = fitParameters->GetCurrentValue(i);
                //std::cout<<"DET "<< fitParameters->GetParamName(i)<<std::endl;
                det_iter++;
            }

            //Save the observable normalization parameters to the observable
            //normalization parameter TVectorD.
            else if(fitParameters->GetParamType(i)==OBSNORM){
                (*(thrown_obsNorm_parms[toy_iter]))(obsNorm_iter) = fitParameters->GetCurrentValue(i);
                //std::cout<<"OBSNORM "<< fitParameters->GetParamName(i)<<std::endl;
                obsNorm_iter++;
            }
        }



        //Print the current values of the parameters for this toy.
        //fitParameters->PrintCurrentValues();


        //If there are any parameters that are detector parameters, and those
        //detector parameters are varied, need to reprocess the MC and then do the matching again.
        if(fitParameters->nDetParms > 0 && fitParameters->varyDetParms){

            ReProcessMC();

            for(int i = 0; i < nMCEvents; i++){

                if(mcEvents[i]->sample != NULL){
                    fitParameters->MatchToEvent(mcEvents[i]);
                }

            }
        }


        //TODO: Code this later, as it is currently not needed..
        //On the first toy we want to have a set of plots broken down by
        //reaction code.
        //        if(toy_iter == 0){
        //
        //            //Preparing reaction code breakdown.
        //            for(int riter = 0; riter < 9; riter++){
        //
        //                //Loop over the samples.
        //                for(int si = 0; si < nSamples; si++){
        //
        //
        //
        //                }
        //
        //            }
        //
        //
        //        }


        //Now for this toy, make the MC predictions.
        //This method calls the final total weight calculation.
        BuildPredictedMC();

        //Loop over the samples and save the MC predictions.
        for(int si = 0; si < nSamples; si++){

            //Finalize the prediction.
            samples[si]->FinalizeMCPrediction();

            std::stringstream outName;
            outName << samples[si]->name << "_predMC_" << toy_iter;
            //std::cout << outName.str() << std::endl;

            samples[si]->SaveMCPrediction(outName.str());

            //Once saved, reset it for next toy.
            samples[si]->ResetMCPrediction();

        }//End loop over samples.

    } //End loop over toys.

    //The histograms have all been written out.  Now write out the parameter
    //arrays.
    for(int i = 0; i < nToys; i++){
        thrown_flux_parms[i]->Write(Form("flux_parms_%d",i));
        thrown_xsec_parms[i]->Write(Form("xsec_parms_%d",i));
        thrown_det_parms[i]->Write(Form("det_parms_%d",i));
        thrown_obsNorm_parms[i]->Write(Form("obsNorm_parms_%d",i));
    }

    //Having looped over all toys and saved everything we wanted, close the
    //output file.
    outFile->Close();

}

double BANFFInterfaceBase::ApplyParamsAndCalcLLR(const std::vector<double>& x){

    //Increment the counter for this method, which is used for fit timing
    //purposes.
    countApplyParamsAndCalcLLR++;
//    if(countApplyParamsAndCalcLLR==1){
//        gettimeofday(&tim, NULL);
//        t0 = tim.tv_sec+(tim.tv_usec/1000000.0);
//    }

    //Read the parameters back into the FitParameters.  The parameter vector
    //contains both parameters that were varied in the fit and parameters that
    //were not (this has been checked), so just copy the whole thing into the
    //fitParameters array one by one.
    for(int i = 0; i < fitParameters->GetNumbSysts(); i++){


        fitParameters->SetParameterValue(i,x[i]);

        //If a NAN parameter was passed, we want to pass NAN to the fitter.
        if(x[i] != x[i]){

            std::cout << "Parameter NAN" << i << std::endl;
            return NAN;
        }
    }

    //The fitter operates in a parameter space where any parameters marked for
    //decomposition have been replaced by parameters corresponding to the
    //eigenbasis of the covariance matrix (or some submatrix thereof).  These values must be transformed
    //back into values that make sense to apply to the events.  So, undecompose
    //the parameters here.  Note: It is not necessary to perform another
    //decomposition operation after the undecomposed parameters have been used, 
    //because the fitter will continue to explore its
    //parameter space, overwriting the content of fitParameters as it does so.
    //It is thus necessary to UnDecomposeParameters for each set of parameters
    //the fitter supplies, as we do here.
    //Only need to undecompose if decomposition was originally required.
    if(fitParameters->eigenDecompositionRequired){

        //The first argument ("fitted") always defaulted to true.  However,
        //this matrix value is not used in this calculation, so specify to skip
        //the covariance calculation in the 2nd "true" argument, in order to
        //save time.
        fitParameters->UnDecomposeParameters(true,true);
    }


    //If there are any parameters that are detector parameters, and those
    //detector parameters are varied, need to reprocess the MC and then do the matching again.
    if(fitParameters->nDetParms > 0 && fitParameters->varyDetParms){

        ReProcessMC();
        for(int i = 0; i < nMCEvents; i++){

            if(mcEvents[i]->sample != NULL){
                fitParameters->MatchToEvent(mcEvents[i]);
            }

        }
    }


    //Before start looping over the discrete parameter combinations, set
    //continuousParamsComputed to false.
    fitParameters->continuousParamsComputed = false;

    //The discrete parameters will not affect the matching, so start the loop
    //over them here.
    for(int dpi = 0; dpi < fitParameters->nDiscreteParameterCombinations; dpi++){

        //Loop over the parameters, and apply the relevant value for the
        //discrete parameters.
        for(int ip = 0; ip < fitParameters->GetNumbSysts(); ip++){

            if(fitParameters->GetIsDiscrete(ip)){

                fitParameters->SetParameterValue(ip, fitParameters->discreteParameterCombinations[dpi][ip]);
            }
        }

        //Change the fitParameters currentDiscreteParameterCombinationIndex to
        //the value of dpi, so that the weight calculation takes advantage of
        //time saving pre-calculations.
        fitParameters->currentDiscreteParameterCombinationIndex = dpi;


        //This method calls the final total weight calculation, and builds the MC
        //predicted histogram for each sample.
        BuildPredictedMC();

        //Since BuildPredictedMC() computes the event weights for all events,
        //we know that the continuous parameters have now been computed since
        //the fit parameter values were changed above.  Set
        //continuousParamsComputed to true to reflect this, allowing time
        //saving for any additional discrete parameter combinations.
        fitParameters->continuousParamsComputed = true;

        //With the predicted MC histograms now made, loop through the samples and
        //calculate their contributions to the overall -2*log likelihood ratio.
        double sampleLLRContribs = 0.0;

        for(int i = 0; i < nSamples; i++){

            //Finalize the MC prediction before calculating the log likelihood
            //ratio contribution.
            samples[i]->FinalizeMCPrediction();

            sampleLLRContribs += samples[i]->CalcLLRContrib();

            //Once the contribution is calculated and added to the total, reset the MC prediction for the next
            //iteration of the fit.
            samples[i]->ResetMCPrediction();
        }

        //With the -2*log likelihood ratio determined for this discrete
        //parameter combination, put it into this combination's slot in the
        //likelihood array, but as a likelihood (i.e. -2*(what is coming from
        //the sample.)
        like_array[dpi] = -0.5*sampleLLRContribs;

    }//End looping over discrete parameters.

    //Since we are done looping over the discrete parameters, and we always
    //want the event weight computation to default to discrete parameter
    //nominal values (the zeroth index) unless otherwise requested, reset the
    //currentDiscreteParameterCombinationIndex to 0 here.
    fitParameters->currentDiscreteParameterCombinationIndex = 0;


    //At this point, have a log likelihood for each discrete parameter
    //combination.  Now go through and find the maximum one.
    int ml_index = -1;
    double ml = -1.0e20;
    for(int i = 0; i < fitParameters->nDiscreteParameterCombinations; i++){

        if(like_array[i] > ml){

            ml_index = i;
            ml = like_array[i];
        }
    }


    //Now we need the sum of the exponential of the difference from the maximum
    //for each entry that is not the maximum likelihood entry.
    double sumOfNonMaxDiff = 0.0;
    for(int i = 0; i < fitParameters->nDiscreteParameterCombinations; i++){

        if(i == ml_index){
            continue;
        }

        sumOfNonMaxDiff += TMath::Exp(like_array[i] - like_array[ml_index]);

    }

    //Now, the total sample coontribution is -2.0*ln(L_avg), which is equal to:
    //-2.0*(ln(L_max) + ln(1 + sumOfNonMax) - ln(nDiscreteParameterCombinations))
    double totalSampleContrib = -2.0*(like_array[ml_index] + TMath::Log(1.0 + sumOfNonMaxDiff)
            - TMath::Log((double)(fitParameters->nDiscreteParameterCombinations)));



    //If this iteration is a multiple of 1000, print it out to give the idea of
    //progress.
    if(countApplyParamsAndCalcLLR % 1000 == 0){
//        gettimeofday(&tim, NULL);
//        t1 = tim.tv_sec+(tim.tv_usec/1000000.0);
        std::cout << "Iteration " << countApplyParamsAndCalcLLR << " complete." << std::endl;
//        std::cout << "Time for " << countApplyParamsAndCalcLLR << " iterations = " << t1 - t0 << std::endl;
//        std::cout << "Time per iteration = " << (t1 - t0)/countApplyParamsAndCalcLLR << std::endl;
    }


    //The quantity minimized in the BANFF fit is made up of the contribution
    //from each of the samples plus a prior term supplied by calculating the
    //chi squared of the current value of the fit parameters in comparison to
    //their old values, making use of the covariance matrix.  Calculate this
    //quantity and add it to the  contribution from the samples, then that is
    //what is returned.
    return (totalSampleContrib + fitParameters->CalcChi2());
}


void BANFFInterfaceBase::VaryParametersByNSigma(int nSigmaValues, double* sigmaValues, std::string fileName){


    //Start by creating the output file.
    TFile* outFile = new TFile(fileName.c_str(),"RECREATE");

    //Create a TObjArray storing the sample names in the proper order as
    //TObjString objects, so that the names of the samples can easily be retrieved
    //from the output toy file.
    TObjArray* sampleNames = new TObjArray(nSamples,0);
    for(int si = 0; si < nSamples; si++){

        sampleNames->AddAt(new TObjString((samples[si]->name).c_str()),si);

    }

    //Need to specify to write the TObjArray as 1 objects instead of breaking
    //out into its individual pieces for the save.
    sampleNames->Write("sampleNames",TObject::kSingleKey);

    //Create a TVectorD containing the sigma values that we are varying each
    //parameter by.
    TVectorD* sigmaValuesVector = new TVectorD(nSigmaValues, sigmaValues);
    sigmaValuesVector->Write("sigmaValues");

    //Build and save the data and mc nominal histograms for each sample.
    //This makes use of the original processing of the events on loading, and
    //in the case of NominalMC, uses a nominal set of weights.
    BuildData();
    BuildNominalMC();
    for(int si = 0; si < nSamples; si++){

        std::string dataName = samples[si]->name + "_data";
        std::string mcName = samples[si]->name + "_nomMC";
        samples[si]->SaveData(dataName);
        samples[si]->SaveNominalMC(mcName);

    }

    //Create a TObjArray of TObjString to store the parameter names.
    //Note: If a parameter (like SF_RFG or RPA) does not end up getting used,
    //the corresponding entries would be unfilled, but GetEntries() only
    //includes the number of non-empty spots, so can still be used.
    TObjArray* parameterNames = new TObjArray(fitParameters->GetNumbSysts(),0);

    //Now, all parameters should be at their nominal value.  Loop through all
    //the parameters, then all of the sigma values, building a thrown MC
    //histogram from each, naming it with the parameter name and the number of
    //sigma, then switch it to nominal before moving to the next parameter.
    //Skip RPA_C and SF_RFG.  Also, save the names of the parameters to a
    //TObjArray of TObjStrings, for use in however want to deal with the
    //output.
    for(int i = 0; i < fitParameters->GetNumbSysts(); i++){

		if(fitParameters->GetParamType(i) == DET || fitParameters->GetParamType(i) == OBSNORM || fitParameters->GetParamType(i) == FLUX ) continue;
        //Get this parameter's name, nominal value, i.e. its current value,
        //and what constitutes 1 sigma.
        std::string paramName = fitParameters->GetParamName(i);

        //If it's SF_RFG or RPA don't do anything.
        if((paramName == "SF_RFG") || (paramName == "RPA_C") || (paramName == "COH_BS")){
            continue;
        }

        double paramNominal = fitParameters->GetCurrentValue(i);
        double paramSigma = fitParameters->GetParameterError(i); 
		std::cout<<"========================================================="<<std::endl;
		std::cout<<paramName<<" "<<paramNominal<<" "<<paramSigma<<std::endl;
        for(int j = 0; j < nSigmaValues; j++){

            //Set the parameter value.
            fitParameters->SetParameterValue(i, paramNominal + sigmaValues[j]*paramSigma);
			if(fitParameters->GetCurrentValue(i) < fitParameters->GetLowerBound(i)) fitParameters->SetParameterValue(i, fitParameters->GetLowerBound(i));
			if(fitParameters->GetCurrentValue(i) > fitParameters->GetUpperBound(i)) fitParameters->SetParameterValue(i, fitParameters->GetUpperBound(i));
			std::cout<<sigmaValues[j]<<" "<<paramNominal + sigmaValues[j]*paramSigma<<" "<<fitParameters->GetCurrentValue(i)<<std::endl;

            //If this was a detector parameter, reprocess the MC and re-match parameters.
            if(fitParameters->GetParamType(i) == DET){

                ReProcessMC();

                for(int i = 0; i < nMCEvents; i++){

                    if(mcEvents[i]->sample != NULL){
                        fitParameters->MatchToEvent(mcEvents[i]);
                    }
                }
            }

            //Now with this new parameter value, make the MC predictions.
            //This method calls the final total weight calculation.
            BuildPredictedMC();

            //Loop over the samples and save the MC predictions.
            for(int si = 0; si < nSamples; si++){

                //Finalize the prediction.
                samples[si]->FinalizeMCPrediction();

                //Name it with the parameter name that was varied and how many
                //sigma it was varied by.
                std::stringstream outName;
                if(fabs(sigmaValues[j])*sigmaValues[j]>0) outName << samples[si]->name << "_predMC_" << paramName << "_" << sigmaValues[j] << "sigma";
                else outName << samples[si]->name << "_predMC_" << paramName << "_m" << fabs(sigmaValues[j]) << "sigma";
                //std::cout << outName.str() << std::endl;

                samples[si]->SaveMCPrediction(outName.str());

                //Once saved, reset it for next toy.
                samples[si]->ResetMCPrediction();

            }
        }
		std::cout<<"========================================================="<<std::endl;

        //Having now finished with this parameter, set its value back to
        //nominal.
        fitParameters->SetParameterValue(i, paramNominal);

        //Also, save its name to the parameter name array.
        parameterNames->AddAt(new TObjString(paramName.c_str()),i);

    }

    //Having looped over all the parameters, save the parameterNames array.
    parameterNames->Write("parameterNames",TObject::kSingleKey);

    //Having looped over all toys and saved everything we wanted, close the
    //output file.
    outFile->Close();

}


void BANFFInterfaceBase::BuildSaveAndResetMCPrediction(std::string nameSuffix){

    //Build the MC prediction and save it out.
    BuildPredictedMC();
    
    //Loop over the samples and save the MC predictions.
    for(int si = 0; si < nSamples; si++){

        //Finalize the prediction.
        samples[si]->FinalizeMCPrediction();

        //Name it with the parameter name that was varied and how many
        //sigma it was varied by.
        std::stringstream outName;
        outName << samples[si]->name << "_predMC_" << nameSuffix;

        samples[si]->SaveMCPrediction(outName.str());

        //Once saved, reset it for next toy.
        samples[si]->ResetMCPrediction();

    }

}

