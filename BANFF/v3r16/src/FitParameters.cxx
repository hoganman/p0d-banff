#define FitParameters_cxx
#include "FitParameters.hxx"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TObjString.h>
#include <TObjArray.h>
#include "BANFFEventBase.hxx"
#include "BANFFSample/BANFFSampleBase.hxx"
#include "TKey.h"

void FluxParameter::MatchToEvent(BANFFEventBase* event){

  //Only perform this procedure if the flux matching has not already
  //occurred (flux matching is only performed once as the neutrino flavour
  //and energy does not change.)
  if(event->GetFluxParameter() == -1){


    double enu;
    int flav;
    int mode;
    enu = event->GetNuEnergy();
    flav = event->GetNuFlavor();
    mode = event->GetBeamMode();

    //Only do this if the neutrino flavour makes sense.
    if(flav != 0){

      //Need to convert from the PDG flavour to the flavour code used by the
      //parameters.
      if(flav == 14){
        flav = 0;
      }

      else if(flav==-14){
        flav = 1;
      }

      else if(flav==12){
        flav = 2;
      }
      else if(flav == -12){
        flav = 3;
      }

      //If the flavour matches, the neutrino energy is in the right range,
      //the event's beam mode matches this parameter's beam mode, 
      //and the detector index is correct, set this parameter as the one
      //corresponding to this event.
      if((flav == flavour) && (enu  >= emin) && (enu < emax) && (mode == beamMode) && (detector == 0) ){
        event->SetFluxParameter(index);
        nEventsAppliedTo++;
      }
    }
  }



}

void FluxParameter::PrintParameterRequirements(){

  std::cout << "Minimum Energy: " << emin << std::endl;
  std::cout << "Maximum Energy: " << emax << std::endl;

  std::string flavourString;

  if(flavour == 0){
    flavourString = "numu";
  }else if(flavour == 1){
    flavourString = "numubar";
  }else if(flavour == 2){
    flavourString = "nue";
  }
  else if(flavour == 3){
    flavourString = "nuebar";
  }

  std::cout << "Neutrino flavor: " << flavourString << std::endl;
  std::cout << "Beam Mode (1: nu, -1: antinu): " << beamMode << std::endl;
  std::cout << "Detector (0: ND, 1: SK): " << detector << std::endl; 

}

void XSecRespParameter::PrintParameterRequirements(){

  //Convert the XSecID to a string so can verify that it matches the name
  //properly.
  std::string xsecString;

  if(xsecID == NORM){
    xsecString = "NORM";
  }

  else if(xsecID == MAQE){
    xsecString = "MAQE";
  }

  else if(xsecID == MARES){
    xsecString = "MARES";
  }

  else if(xsecID == DISMPISHP){
    xsecString = "DISMPISHP";
  }

  else if(xsecID == SF){

    xsecString = "SF";
  }

  else if(xsecID == EB){

    xsecString = "EB";
  }

  else if(xsecID == PF){
    xsecString = "PF";
  }

  else if(xsecID == PDD){

    xsecString = "PDD";
  }

  else if(xsecID == FSI_INEL_LO){
    xsecString = "FSI_INEL_LO";
  }

  else if(xsecID == FSI_INEL_HI){
    xsecString = "FSI_INEL_HI";
  }

  else if(xsecID == FSI_PI_PROD){
    xsecString = "FSI_PI_PROD";
  }

  else if(xsecID == FSI_PI_ABS){
    xsecString = "FSI_PI_ABS";
  }

  else if(xsecID == FSI_CEX_LO){
    xsecString = "FSI_CEX_LO";
  }

  else if(xsecID == FSI_CEX_HI){
    xsecString = "FSI_CEX_HI";
  }

  else if(xsecID == PDD_MEC){
    xsecString = "PDD_MEC";
  }

  else if(xsecID == MEC){
    xsecString = "MEC";
  }

  else if(xsecID == MEC_C){
    xsecString = "MEC_C";
  }

  else if(xsecID == PF_C){
    xsecString = "PF_C";
  }

  else if(xsecID == EB_C){
    xsecString = "EB_C";
  }

  else if(xsecID == MEC_O){
    xsecString = "MEC_O";
  }

  else if(xsecID == PF_O){
    xsecString = "PF_O";
  }

  else if(xsecID == EB_O){
    xsecString = "EB_O";
  }

  else if(xsecID == CA5){
    xsecString = "CA5";
  }

  else if(xsecID == BgSclRes){
    xsecString = "BgSclRes";
  }

  else if(xsecID == SCCV){
    xsecString = "SCCV";
  }

  else if(xsecID == SCCA){
    xsecString = "SCCA";
  }

  else if(xsecID == RPA){
    xsecString = "RPA";
  }

  else if(xsecID == COH_BS){
    xsecString = "COH_BS";
  }

  else if(xsecID == SF_RFG){
    xsecString = "SF_RFG";
  }

  else if(xsecID == CCNuE){
    xsecString = "CCNuE";
  }
	
  else if(xsecID == MEC_shape_C){
    xsecString = "MEC_shape_C";
  }

  else if(xsecID == MEC_shape_O){
    xsecString = "MEC_shape_O";
  }

  std::cout << "xsecID will match to spline: " << xsecString << std::endl;
}

void XSecFuncParameter::MatchToEvent(BANFFEventBase* event){


  ///Only set it to something else if it hasn't already been set.
  if(event->GetXsecFuncParameter() == -1){

    double enu;
    int reactionCode;
    int flavor;
    int tgtMat;
    enu = event->GetNuEnergy();
    reactionCode = event->GetReactionCode();
    tgtMat = event->GetTargetMaterial();

    flavor = event->GetNuFlavor();

    //Only match it if the flavour makes sense.
    if(flavor!=0){


      //If the target material for this parameter is -1, don't consider
      //it.  Otherwise check it against the event's stored value.
      bool tgtMatOK = false;

      //If there is no restriction, targetMaterial will be an array with
      //the first and only entry set to -1.
      if(targetMaterial[0] == -1){

        tgtMatOK = true;
            
      }

      //Otherwise, loop through the list of target materials and if one
      //matches, set tgtMatOK to true.
      else{

        for(int tmi = 0; tmi < nTargetMaterials; tmi++){

          if(targetMaterial[tmi] == tgtMat){
            tgtMatOK = true;
          }

        }
      }


      //Check through the reaction codes for this parameter, and if one
      //matches to that of the event, set this bool to true.
      bool reacCodeMatch = false;
      for(int i = 0; i < nReactionCodes; i++){

        if(reactionCode == reactionCodes[i]){
          reacCodeMatch = true;
          break;
        }

      }
			
      //Check through the nupdg for this parameter, and if one
      //matches to that of the event, set this bool to true.
      bool nuPDGMatch = false;
      for(int i = 0; i < nNupdg; i++){

        if(flavor == Nupdg[i]){
          nuPDGMatch = true;
          break;
        }

      }

      //If the reaction code matches, and the energy is in range, set
      //this parameter as the XsecNorm parameter corresponding to this
      //event.
      if( reacCodeMatch  && enu >= 0.0 && tgtMatOK && nuPDGMatch){
        event->SetXsecFuncParameter(index);
        nEventsAppliedTo++;
      }
    }

  }
}

void XSecNormParameter::MatchToEvent(BANFFEventBase* event){


  double enu;
  int reactionCode;
  int flavor;
  int tgtMat;
  enu = event->GetNuEnergy();
  reactionCode = event->GetReactionCode();
  tgtMat = event->GetTargetMaterial();

  flavor = event->GetNuFlavor();



  //Only match it if the flavour makes sense.
  if(flavor!=0){


    //If the target material for this parameter is -1, don't consider
    //it.  Otherwise check it against the event's stored value.
    bool tgtMatOK = false;

    //If there is no restriction, targetMaterial will be an array with
    //the first and only entry set to -1.
    if(targetMaterial[0] == -1){

      tgtMatOK = true;
            
    }

    //Otherwise, loop through the list of target materials and if one
    //matches, set tgtMatOK to true.
    else{

      for(int tmi = 0; tmi < nTargetMaterials; tmi++){

        if(targetMaterial[tmi] == tgtMat){
          tgtMatOK = true;
        }

      }
    }


    //Check through the reaction codes for this parameter, and if one
    //matches to that of the event, set this bool to true.
    bool reacCodeMatch = false;
    for(int i = 0; i < nReactionCodes; i++){

      if(reactionCode == reactionCodes[i]){
        reacCodeMatch = true;
        break;
      }

    }
			
    //Check through the nupdg for this parameter, and if one
    //matches to that of the event, set this bool to true.
    bool nuPDGMatch = false;
    for(int i = 0; i < nNupdg; i++){

      if(flavor == Nupdg[i]){
        nuPDGMatch = true;
        break;
      }

    }

    //If the reaction code matches, and the energy is in range, set
    //this parameter as the XsecNorm parameter corresponding to this
    //event.
    if( reacCodeMatch  && enu >= emin && enu < emax && tgtMatOK && nuPDGMatch){
      event->SetXsecNormParameter(index);
      event->SetXsecNormParameterNumElements();
      nEventsAppliedTo++;
      //std::cout << "Event energy: " << enu << " paramIndex: " << index 
      //          << " numElements: " << event->GetXsecNormParameterSize() <<  " flavor: " << flavor 
      //          << " reactionCode: " << reactionCode << " nEventsAppliedTo: " << nEventsAppliedTo << std::endl;
    }
  }

  //}
}

void XSecNormParameter::PrintParameterRequirements(){

  std::cout << "Minimum Energy: " << emin << std::endl;
  std::cout << "Maximum Energy: " << emax << std::endl;
  std::cout << "Target Materials: " << std::endl;
  for(int tmi = 0; tmi < nTargetMaterials; tmi++){
    std::cout << targetMaterial[tmi] << std::endl;
  }

  std::cout << "Reaction codes: "; 
  for(int i = 0; i < nReactionCodes; i++){
    std::cout << reactionCodes[i] << std::endl;
  }
  std::cout << std::endl;

}


void ObsNormParameter::MatchToEvent(BANFFEventBase* event){


  //Require that the event has a pointer to a sample.
  if(event->sample != NULL){


    //This is only the right parameter if it applies to the right sample,
    //and the number of observables matches.
    if(event->sample->sampleID == sample
       && (int) obsMin.size() == event->sample->nObs
       && (int) obsMax.size() == event->sample->nObs){


      //Now, if all the observables are in the right range, assign this
      //parameter to this event.
      bool allObsInRange = true;
      for(int i = 0; i < event->sample->nObs; i++){

        float obsVal = event->sample->obs[i]->GetCurrentValue(event);
        allObsInRange = allObsInRange && (obsVal >= obsMin[i]) && (obsVal < obsMax[i]);

      }


      //Having looped through all observables, if all observables are in
      //range, set this as the parameter.
      if(allObsInRange){

        event->SetObsNormParameter(index);
        nEventsAppliedTo++;
      }
    }
  }
}

void ObsNormParameter::PrintParameterRequirements(){

  std::cout << "Sample ID: " << sample << std::endl;
  for(unsigned int i = 0; i < obsMin.size(); i++){

    std::cout << "Obs " << i << " min: " << obsMin[i] << std::endl;
    std::cout << "Obs " << i << " max: " << obsMax[i] << std::endl;

  }

}


FitParameters::FitParameters(){

  covariance = new TMatrixDSym(0);
  covarianceI = NULL;
  covarianceUD = NULL;
  covarianceUDI = NULL;
  covarianceFit = NULL;
  fluxIter = 0;
  xsecRespIter = 0;
  xsecFuncIter = 0;
  xsecNormIter = 0;
  obsNormIter = 0;
  detIter = 0;
  throwParms = NULL;
  eigenDecomp = NULL;

  //Initialize the parameters counters to 0.
  nFluxParms = 0;
  nXSecParms = 0;
  nDetParms = 0;
  nObsNormParms = 0;

  //Initialize the BeRPA parameter position.
  BeRPA_pos = -999;

  //Default to not performing eigen decomposition.
  eigenDecompositionRequired = false;

  //This should always be set before it is used, but set it here too.
  currentDiscreteParameterCombinationIndex = 0;

  //Set these to default values, so there can be a check as to whether the
  //fast version is supported.
  numbSystsFast = -1;
  fitParametersFast = NULL;

  //This needs to default to false so that we default to calculating the full
  //set of splines.
  continuousParamsComputed = false;

  //Set up multi-threading nCores variable
  const char* nthreads = getenv("OMP_NUM_THREADS");
  nCores = ( nthreads != NULL ? atoi( nthreads ) : 1 );
  if ( nCores < 2 ) {
    std::cout << "Warning: multithreading disabled in FitParameters."
              << std::endl;
  }
}

FitParameters::~FitParameters(){

  if(covariance !=NULL) covariance->Delete();
  if(covarianceI !=NULL) covarianceI->Delete();
  if(covarianceUD !=NULL) covarianceUD->Delete();
  if(covarianceUDI !=NULL) covarianceUDI->Delete();
  if(covarianceFit !=NULL) covarianceFit->Delete();
  if(throwParms !=NULL) delete throwParms;
  if(eigenDecomp != NULL) delete eigenDecomp;

}

void FitParameters::UnDecomposeParameters(bool fitted, bool skipCovariance){

  //If there's been no eigendecomposition, it nothing to un-decompose.
  if(eigenDecomp==NULL){
    return;
  }

  //Counting how many of the parameters need to be un-decomposed, and how
  //many parameters were varied in the fit.
  int decompSize=0;
  int fitSize=0;
  for(unsigned int i=0; i<fitParameters.size(); i++){
    if(fitParameters[i]->decompose>=1){
      decompSize++;
    }

    if(fitParameters[i]->vary){
      fitSize++;
    }
  }


  //Copy the parameter and covariance 
  TVectorD paramsIn(decompSize);
  TVectorD paramsOut(decompSize);
  TMatrixDSym covIn(fitSize);
  TMatrixDSym covOut(fitSize);
  int iter=0;
  int iter0=0;
  for(unsigned int i=0; i<fitParameters.size(); i++){

    //If the parameter is flagged as requiring decomposition, save its
    //currentValue to the paramsIn vector.
    if(fitParameters[i]->decompose>=1){
      paramsIn(iter) = fitParameters[i]->currentValue;
      iter++;
    }


    //Only need to deal with the matrices if not instructed to skip the
    //covariance. 
    if(!skipCovariance){
      int jter0=0;

      //If the fit parameter is varied in the fit, loop over the fit
      //parameters, and if it also varied, get the entry from the covariance
      //matrix produced in the fit (if there was one).  Otherwise, use the covariance matrix
      //stored in this FitParameters instance
      if(fitParameters[i]->vary){
        for(unsigned int j=0; j<fitParameters.size(); j++){
          if(fitParameters[j]->vary){
            if(fitted){
              covIn(iter0,jter0) = (*covarianceFit)(i,j);
            }
            else{
              covIn(iter0,jter0) = (*covariance)(i,j);
            }

            jter0++;
          }
        }
        iter0++;
      }
    }
  } 



  //Uses the properties of eigenvectors of real symmetry matrices to
  //transform the parameter vector and the fitted covariance (in this case
  //from 0 to the size of the decomposition... I guess the decomposed
  //parameters (flux and FSI) are first in the matrix?)
  eigenDecomp->GetOriginalParams(paramsIn, paramsOut, 0, paramsIn.GetNrows());
  //eigenDecomp->GetOriginalCovariance(covIn, covOut, 0, covIn.GetNrows());

  //Only need to do this if not instructed to skip the covariance.
  if(!skipCovariance){
    eigenDecomp->GetOriginalCovariance(covIn, covOut, 0, decompSize);
  }

  //Now that we've back in the original basis, copy the transformed values to
  //currentValueUD (undecomposed), and for the covariance matrix, into
  //covarianceFit if a fit had occurred, and covarianceUD if it had not.
  iter=0;
  for(unsigned int i=0; i<fitParameters.size(); i++){
    if(fitParameters[i]->decompose<1) continue;
    fitParameters[i]->currentValueUD = paramsOut(iter);
    iter++;
  }

  //Only copy the covariance information if we're not skipping it.
  if(!skipCovariance){
    iter=0;
    for(unsigned int i=0; i<fitParameters.size(); i++){
      if(!fitParameters[i]->vary){
        continue;
      }
      int jter=0;
      for(unsigned int j=0; j<fitParameters.size(); j++){
        if(!fitParameters[j]->vary){
          continue;
        }
        if(fitted){
          (*covarianceFit)(i,j) = covOut(iter,jter);
        }
        else{
          (*covarianceUD)(i,j) = covOut(iter,jter);
        }
        jter++;
      }
      iter++;
    }

  }
}

void FitParameters::UnDecomposeLoadedParameters(bool fitted, bool skipCovariance, bool replaceOriginal){

    //If there's been no eigendecomposition, it nothing to un-decompose.
    if(eigenDecomp==NULL){
        return;
    }

    //Counting how many of the parameters need to be un-decomposed, and how
    //many parameters were varied in the fit.
    int decompSize=0;
    int fitSize=0;
    for(unsigned int i=0; i<fitParameters.size(); i++){
        if(fitParameters[i]->decompose>=1){
            decompSize++;
        }

        if(fitParameters[i]->vary){
            fitSize++;
        }
    }


    //Copy the parameter and covariance 
    TVectorD paramsIn(decompSize);
    TVectorD paramsOut(decompSize);
    TMatrixDSym covIn(fitSize);
    TMatrixDSym covOut(fitSize);
    int iter=0;
    int iter0=0;
    for(unsigned int i=0; i<fitParameters.size(); i++){

        //If the parameter is flagged as requiring decomposition, save its
        //currentValue to the paramsIn vector.
        if(fitParameters[i]->decompose>=1){
            paramsIn(iter) = fitParameters[i]->currentValue;
            iter++;
        }

        //Only need to deal with the matrices if not instructed to skip the
        //covariance. 
        if(!skipCovariance){
            int jter0=0;

            //If the fit parameter is varied in the fit, loop over the fit
            //parameters, and if it also varied, get the entry from the covariance
            //matrix produced in the fit (if there was one).  Otherwise, use the covariance matrix
            //stored in this FitParameters instance
            if(fitParameters[i]->vary){
                for(unsigned int j=0; j<fitParameters.size(); j++){
                    if(fitParameters[j]->vary){
                        if(fitted){
                            covIn(iter0,jter0) = (*covarianceFit)(i,j);
                        }
                        else{
                            covIn(iter0,jter0) = (*covariance)(i,j);
                        }

                        jter0++;
                    }
                }
                iter0++;
            }
        }
    }



    //Uses the properties of eigenvectors of real symmetry matrices to
    //transform the parameter vector and the fitted covariance (in this case
    //from 0 to the size of the decomposition... I guess the decomposed
    //parameters (flux and FSI) are first in the matrix?)
    eigenDecomp->GetOriginalParams(paramsIn, paramsOut, 0, paramsIn.GetNrows());
    //eigenDecomp->GetOriginalCovariance(covIn, covOut, 0, covIn.GetNrows());

    //Only need to do this if not instructed to skip the covariance.
    if(!skipCovariance){
        eigenDecomp->GetOriginalCovariance(covIn, covOut, 0, decompSize);
    }

    //Now that we've back in the original basis, copy the transformed values to
    //currentValueUD (undecomposed), and for the covariance matrix, into
    //covarianceFit if a fit had occurred, and covarianceUD if it had not.
    iter=0;
    for(unsigned int i=0; i<fitParameters.size(); i++){
        if(fitParameters[i]->decompose<1) continue;
        if(replaceOriginal){
            fitParameters[i]->currentValueUD = paramsOut(iter);
            fitParameters[i]->currentValue = paramsOut(iter);
        }
        else{
            fitParameters[i]->currentValueUD = paramsOut(iter);
        }
        iter++;
    }

    //Only copy the covariance information if we're not skipping it.
    if(!skipCovariance){
        iter=0;
        for(unsigned int i=0; i<fitParameters.size(); i++){
            if(!fitParameters[i]->vary){
                continue;
            }
            int jter=0;
            for(unsigned int j=0; j<fitParameters.size(); j++){
                if(!fitParameters[j]->vary){
                    continue;
                }
                if(fitted){
                    (*covarianceFit)(i,j) = covOut(iter,jter);
                }
                else if(replaceOriginal){
                    (*covarianceUD)(i,j) = covOut(iter,jter);
                    (*covariance)(i,j) = covOut(iter,jter);
                }
                else{
                    (*covarianceUD)(i,j) = covOut(iter,jter);
                }
                jter++;
            }
            iter++;
        }
    }
}

void FitParameters::DecomposeParameters(){


  //Find the number of parameters that need to be decomposed.
  //Save their priorValue and currentValue prior to decomposition.
  //decompSize is equal to the total numnber of parameters that need to be
  //decomposed (i.e. have correlations with other parameters). 
  int decompSize=0;
  for(unsigned int i=0; i<fitParameters.size(); i++){

    //If a parameter does not need to be decomposed (i.e. Is not correlated
    //with other parameters) its decompose flag is set to 0.
    if(fitParameters[i]->decompose>=1){

      fitParameters[i]->currentValueUD = fitParameters[i]->currentValue;
      fitParameters[i]->priorValueUD = fitParameters[i]->priorValue;

      //A decompose value of 2 indicates a parameter that needs to be
      //decomposed but is not varied in the fit.
      if(fitParameters[i]->decompose==2){
        fitParameters[i]->vary = false;
      }

      decompSize++;
    }
  }

  //Copy the priorValue of the parameters that require decomposition.
  TVectorD params(decompSize);
  int iter=0;
  for(unsigned int i=0; i<fitParameters.size(); i++){
    if(fitParameters[i]->decompose>=1){
      params(iter) = fitParameters[i]->priorValue;
      iter++;
    }
  }

  //Store the undecomposed covariance matrix and its inverse (clear any
  //previous versions from memory first.)
  if(covarianceUD!=NULL) covarianceUD->Delete(); 
  if(covarianceUDI!=NULL) covarianceUDI->Delete(); 

  covarianceUD = new TMatrixDSym(*covariance);
  covarianceUDI = new TMatrixDSym(*covariance);
  covarianceUDI->Invert();


  //Make a covariance matrix for only the parameters that have correlations
  //with each other (and therefore require decomposition.)
  iter=0;
  int jter=0;
  TMatrixDSym cov(decompSize);
  for(unsigned int i=0; i<fitParameters.size(); i++){
    jter=0;

    //If a parameter's decompose flag is not set, it does not have any
    //correlations with other parameters, so we don't need to loop over the
    //other parameters to look for correlations with it.
    if( !(fitParameters[i]->decompose>=1) ){
      continue;
    }

    //Since this parameter has correlations with other parameters, loop
    //through the other parameters to pick out the covariances with other
    //parameters with correlations.
    for(unsigned int j=0; j<fitParameters.size(); j++){

      //If a parameter does not have its decompose flag set, the
      //covariance matrix entry (should be zero) does not need to be
      //added into this covariance submatrix.
      if( !(fitParameters[j]->decompose>=1) ){
        continue;
      }

      //If we get to here, we should be copying a non-zero value from the
      //original covariance matrix into the submatrix with correlated
      //parameters.
      cov(iter,jter) = (*covariance)(i,j);
      jter++;
    }
    iter++;
  }

  //The EigenDecomp code takes in the parameters and the covariance submatrix
  //and performs the Eigen Decomposition.
  if(eigenDecomp!=NULL){
    delete eigenDecomp;
  }

  eigenDecomp = new EigenDecomp(params,cov);

  eigenDecomp->GetDecomposition(params, cov);

  //Now that the array of the prior value of the correlated parameters and
  //the covariance submatrix have undergone Eigen  Decomposition, copy the
  //new parameter values back to the parameter current and prior value. (This
  //code is only called once before the fit begins, so current = prior)
  iter=0;
  for(unsigned int i=0; i<fitParameters.size(); i++){
    if(fitParameters[i]->decompose>=1){
      fitParameters[i]->currentValue = params(iter);
      fitParameters[i]->priorValue = params(iter);
      iter++;
    }
  }

  //Copy the entries from the eigenvalue decomposed submatrix to the full
  //covariance matrix.
  iter=0;
  jter=0;
  for(unsigned int i=0; i<fitParameters.size(); i++){
    jter=0;

    if( !(fitParameters[i]->decompose>=1) ){
      continue;
    }

    for(unsigned int j=0; j<fitParameters.size(); j++){

      if( !(fitParameters[j]->decompose>=1) ){
        continue;
      }

      (*covariance)(i,j) = cov(iter,jter);
      jter++;
    }
    iter++;
  }


  //Create a copy of the inverse of the (now diagonal) covariance matrix.
  if(covarianceI !=NULL){
    covarianceI->Delete();
  }
  covarianceI = new TMatrixDSym(*covariance);
  covarianceI->Invert();  

}

void FitParameters::MatchToEvent(BANFFEventBase* event){

  for(unsigned int i = 0; i < fitParameters.size(); i++){

    fitParameters[i]->MatchToEvent(event);

  }

}




///Calculates the Chi^2 comparing the prior and current values of all
///parameters that do not have a flat prior, making use of their covariance.
double FitParameters::CalcChi2(){

  int nrows = covarianceI->GetNrows();
  double chi2 = 0.;
  //for(unsigned int i=0; i<fitParameters.size(); i++)
  //  for(unsigned int j=0; j<fitParameters.size(); j++)
#pragma omp parallel for num_threads(nCores) schedule(dynamic,(nrows*nrows)/nCores) reduction(+:chi2)
  for(int i=0; i<nrows*nrows; i++){
    //        for(int j=0; j<nrows; j++){
    int remainder = i%nrows;
    int row = i/nrows;
    //Don't calculate for the fit parameters with a Flat prior.
    if(fitParameters[row]->priorType==PDFFlat || fitParameters[remainder]->priorType==PDFFlat ){
      continue;
    }

    //Don't calculate for fit parameters that are discrete.
    if(fitParameters[row]->discrete || fitParameters[remainder]->discrete){
      continue;
    }

    chi2+= (fitParameters[row]->currentValue-fitParameters[row]->priorValue)*
      (fitParameters[remainder]->currentValue-fitParameters[remainder]->priorValue)*
      (*covarianceI)(row,remainder);
  }
  return chi2;

}

double FitParameters::CalcChi2OfType(ParamType parameterType){

  double chi2 = 0.;

  for(int i=0; i<covarianceI->GetNrows(); i++){
    for(int j=0; j<covarianceI->GetNrows(); j++){

      //Don't calculate for the fit parameters with a Flat prior.
      if(fitParameters[i]->priorType==PDFFlat || fitParameters[j]->priorType==PDFFlat ){
        continue;
      }

      //Don't calculate for fit parameters that are discrete.
      if(fitParameters[i]->discrete || fitParameters[j]->discrete){
        continue;
      }

      bool bothParametersOfRightType = false;

      if(parameterType == XSECRESP){

        bothParametersOfRightType = (fitParameters[i]->paramType == XSECRESP
                                     || fitParameters[i]->paramType == XSECNORM) 
          && (fitParameters[j]->paramType == XSECRESP
              || fitParameters[j]->paramType == XSECNORM);

      }
      else{

        bothParametersOfRightType = (fitParameters[i]->paramType == parameterType)
          && (fitParameters[j]->paramType == parameterType);

      }


      if(bothParametersOfRightType){

        chi2+= (fitParameters[i]->currentValue-fitParameters[i]->priorValue)*
          (fitParameters[j]->currentValue-fitParameters[j]->priorValue)*
          (*covarianceI)(i,j);
      }
    }
  }
  return chi2;

}


void FitParameters::LoadFluxParametersFromFile(std::string fileName, bool loadNuMode, bool loadAntiNuMode, bool vary){

  //Load the flux file.
  fluxFile = new TFile(fileName.c_str());

  //Loop through the file, looking at the keys.  For each key, check for the
  //string "numode".  If we finish looping through the keys and don't find
  //it, we're looking at a 2013 style file.  If we do find it, it's 2014
  //style.

  TList* keys = fluxFile->GetListOfKeys();
  Int_t nKeys = fluxFile->GetNkeys();
  bool is2014StyleFile = false;

  for(int i = 0; i < nKeys; i++){

    TKey* tmpKey = (TKey*)(keys->At(i));
    std::string keyName(tmpKey->GetName());

    //If "numode" is in the key name, we have a 2014 style file, so set the
    //bool and break.
    if(keyName.find("numode") != std::string::npos){

      is2014StyleFile = true;
      break;
    }
  }


  //If LoadFluxParametersFromFile is called with both loadNuMode and
  //loadAntiNuMode false, print out a warning but continue.
  if((!loadNuMode) && (!loadAntiNuMode)){

    std::cout << "LoadFluxParametersFromFile called, but no flux parameters loaded." << std::endl;
  }


  //If it's not a 2014 style file and loadAntiNuMode is requested, print an
  //error message and exit.
  if(loadAntiNuMode && !is2014StyleFile){

    std::cout << "ERROR: AntiNuMode flux parameters requested from 2013 style file.  Exiting." << std::endl;
    exit(1);
  }

  //If asked to load nu mode parameters, and have been given a 2013 style
  //file, call the same code as 2013 (with beam mode parameter added and set
  //to neutrino.)
  if(loadNuMode && !is2014StyleFile){

    //Take the information we need from the flux file.
    flux_cov = (TMatrixDSym*)(fluxFile->Get("total_flux_cov"));
    nd_numode_numu_bins = (TAxis*)(fluxFile->Get("nd_numu_bins"));
    nd_numode_numub_bins = (TAxis*)(fluxFile->Get("nd_numub_bins"));
    nd_numode_nue_bins = (TAxis*)(fluxFile->Get("nd_nue_bins"));
    nd_numode_nueb_bins = (TAxis*)(fluxFile->Get("nd_nueb_bins"));
    sk_numode_numu_bins = (TAxis*)(fluxFile->Get("sk_numu_bins"));
    sk_numode_numub_bins = (TAxis*)(fluxFile->Get("sk_numub_bins"));
    sk_numode_nue_bins = (TAxis*)(fluxFile->Get("sk_nue_bins"));
    sk_numode_nueb_bins = (TAxis*)(fluxFile->Get("sk_nueb_bins"));

    //ND280 numu flux parameters.
    AddFluxParameters(nd_numode_numu_bins, 0, 0, 1, vary);
    nFluxParms += nd_numode_numu_bins->GetNbins();

    //ND280 antinumu flux parameters.
    AddFluxParameters(nd_numode_numub_bins, 0, 1, 1, vary);
    nFluxParms += nd_numode_numub_bins->GetNbins();

    //ND280 nue flux parameters.
    AddFluxParameters(nd_numode_nue_bins, 0, 2, 1, vary);
    nFluxParms += nd_numode_nue_bins->GetNbins();

    //ND280 antinue flux parameters.
    AddFluxParameters(nd_numode_nueb_bins, 0, 3, 1, vary);
    nFluxParms += nd_numode_nueb_bins->GetNbins();

    //SK numu flux parameters.
    AddFluxParameters(sk_numode_numu_bins, 1, 0, 1, vary);
    nFluxParms += sk_numode_numu_bins->GetNbins();

    //SK antinumu flux parameters.
    AddFluxParameters(sk_numode_numub_bins, 1, 1, 1, vary);
    nFluxParms += sk_numode_numub_bins->GetNbins();

    //SK nue flux parameters.
    AddFluxParameters(sk_numode_nue_bins, 1, 2, 1, vary);
    nFluxParms += sk_numode_nue_bins->GetNbins();

    //SK antinue flux parameters.
    AddFluxParameters(sk_numode_nueb_bins, 1, 3, 1, vary);
    nFluxParms += sk_numode_nueb_bins->GetNbins();

    //Add in the flux covariance matrix for these parameters.
    AppendCovariance(flux_cov);

  }

  //If it is a 2014 style file, load those parameters as requested.
  if(is2014StyleFile){

    //Always grab the covariance.
    flux_cov = (TMatrixDSym*)(fluxFile->Get("total_flux_cov"));


    //Parameter ordering is as follows (see:
    //http://www.t2k.org/asg/oagroup/meeting/2014/banffoa20141016/beaminputs/at_download/file )
    //0-24: ND280 neutrino mode
    //25-49: ND280 antineutrino mode
    //50-74: SK neutrino mode
    //75-99: SK antineutrino mode

    //If instructed to load the neutrino mode parameters, start by loading
    //the ND280 neutrino mode parameters.
    if(loadNuMode){

      nd_numode_numu_bins = (TAxis*)(fluxFile->Get("nd5_numode_numu_bins"));
      nd_numode_numub_bins = (TAxis*)(fluxFile->Get("nd5_numode_numub_bins"));
      nd_numode_nue_bins = (TAxis*)(fluxFile->Get("nd5_numode_nue_bins"));
      nd_numode_nueb_bins = (TAxis*)(fluxFile->Get("nd5_numode_nueb_bins"));

      //ND280 numu flux parameters.
      AddFluxParameters(nd_numode_numu_bins, 0, 0, 1, vary);
      nFluxParms += nd_numode_numu_bins->GetNbins();

      //ND280 antinumu flux parameters.
      AddFluxParameters(nd_numode_numub_bins, 0, 1, 1, vary);
      nFluxParms += nd_numode_numub_bins->GetNbins();

      //ND280 nue flux parameters.
      AddFluxParameters(nd_numode_nue_bins, 0, 2, 1, vary);
      nFluxParms += nd_numode_nue_bins->GetNbins();

      //ND280 antinue flux parameters.
      AddFluxParameters(nd_numode_nueb_bins, 0, 3, 1, vary);
      nFluxParms += nd_numode_nueb_bins->GetNbins();

    }


    //If instructed to load the anti-neutrino mode parameters, now load in
    //the ND280 antineutrino mode parameters.
    if(loadAntiNuMode){

      nd_anumode_numu_bins = (TAxis*)(fluxFile->Get("nd5_anumode_numu_bins"));
      nd_anumode_numub_bins = (TAxis*)(fluxFile->Get("nd5_anumode_numub_bins"));
      nd_anumode_nue_bins = (TAxis*)(fluxFile->Get("nd5_anumode_nue_bins"));
      nd_anumode_nueb_bins = (TAxis*)(fluxFile->Get("nd5_anumode_nueb_bins"));

      //ND280 numu flux parameters.
      AddFluxParameters(nd_anumode_numu_bins, 0, 0, -1, vary);
      nFluxParms += nd_anumode_numu_bins->GetNbins();

      //ND280 antinumu flux parameters.
      AddFluxParameters(nd_anumode_numub_bins, 0, 1, -1, vary);
      nFluxParms += nd_anumode_numub_bins->GetNbins();

      //ND280 nue flux parameters.
      AddFluxParameters(nd_anumode_nue_bins, 0, 2, -1, vary);
      nFluxParms += nd_anumode_nue_bins->GetNbins();

      //ND280 antinue flux parameters.
      AddFluxParameters(nd_anumode_nueb_bins, 0, 3, -1, vary);
      nFluxParms += nd_anumode_nueb_bins->GetNbins();

    }

    //If instructed to load the neutrino mode parameters, now load
    //the SK neutrino mode parameters.
    if(loadNuMode){

      sk_numode_numu_bins = (TAxis*)(fluxFile->Get("sk_numode_numu_bins"));
      sk_numode_numub_bins = (TAxis*)(fluxFile->Get("sk_numode_numub_bins"));
      sk_numode_nue_bins = (TAxis*)(fluxFile->Get("sk_numode_nue_bins"));
      sk_numode_nueb_bins = (TAxis*)(fluxFile->Get("sk_numode_nueb_bins"));

      //SK numu flux parameters.
      AddFluxParameters(sk_numode_numu_bins, 1, 0, 1, vary);
      nFluxParms += sk_numode_numu_bins->GetNbins();

      //SK antinumu flux parameters.
      AddFluxParameters(sk_numode_numub_bins, 1, 1, 1, vary);
      nFluxParms += sk_numode_numub_bins->GetNbins();

      //SK nue flux parameters.
      AddFluxParameters(sk_numode_nue_bins, 1, 2, 1, vary);
      nFluxParms += sk_numode_nue_bins->GetNbins();

      //SK antinue flux parameters.
      AddFluxParameters(sk_numode_nueb_bins, 1, 3, 1, vary);
      nFluxParms += sk_numode_nueb_bins->GetNbins();

    }

    //If instructed to load the anti-neutrino mode parameters, now load in
    //the SK antineutrino mode parameters.
    if(loadAntiNuMode){

      sk_anumode_numu_bins = (TAxis*)(fluxFile->Get("sk_anumode_numu_bins"));
      sk_anumode_numub_bins = (TAxis*)(fluxFile->Get("sk_anumode_numub_bins"));
      sk_anumode_nue_bins = (TAxis*)(fluxFile->Get("sk_anumode_nue_bins"));
      sk_anumode_nueb_bins = (TAxis*)(fluxFile->Get("sk_anumode_nueb_bins"));

      //SK numu flux parameters.
      AddFluxParameters(sk_anumode_numu_bins, 1, 0, -1, vary);
      nFluxParms += sk_anumode_numu_bins->GetNbins();

      //SK antinumu flux parameters.
      AddFluxParameters(sk_anumode_numub_bins, 1, 1, -1, vary);
      nFluxParms += sk_anumode_numub_bins->GetNbins();

      //SK nue flux parameters.
      AddFluxParameters(sk_anumode_nue_bins, 1, 2, -1, vary);
      nFluxParms += sk_anumode_nue_bins->GetNbins();

      //SK antinue flux parameters.
      AddFluxParameters(sk_anumode_nueb_bins, 1, 3, -1, vary);
      nFluxParms += sk_anumode_nueb_bins->GetNbins();
    }


    //Now to handle the covariance.  How much of it gets loaded in depends
    //on which parameters have loaded.

    //If we load both sets of parameters, just copy the covariance.
    if(loadNuMode && loadAntiNuMode){

      AppendCovariance(flux_cov);

    }

    //If we only load NuMode, create a new 50 row covariance which only
    //loads the relevant submatrices.
    //ND neutrino mode: 0-24
    //SK neutrino mode: 50-74.
    else if(loadNuMode){

      TMatrixDSym* subMatrix = new TMatrixDSym(50);

      //Loop over all matrix entries.  If the row or column number is
      //in the right range, add it in (but if the row or column number is
      //greater than or equal to 50, subtract 25 from it to get the new
      //matrix value for that index.)
      for(int i = 0; i < flux_cov->GetNrows(); i++){

        for(int j = 0; j < flux_cov->GetNrows(); j++){

          //If i or j  isn't right continue to the next parameter.
          if(24 < i && i < 50) continue;
          if(24 < j && j < 50) continue;
          if(74 < i || 74 < j) continue;

          //The new matrix indices.
          int k;
          int l;

          if( i < 25){

            k = i;

          }

          else{
            k = i - 25;
          }

          if(j < 25){

            l = j;

          }else{

            l = j -25;

          }

          //So now we know we have a good flux_cov index and a good
          //subMatrix index.  Populate the submatrix with the
          //corresponding full matrix entry.
          (*subMatrix)(k,l) = (*flux_cov)(i,j);

        }



      }

      //The sub matrix is what is appended to the FitParameters for the
      //flux parameters.
      AppendCovariance(subMatrix);

    }

    //Otherwise, if we're to load the anti-neutrino mode parameters, excise
    //the correct submatrix and append it.
    //ND anti-neutrino mode: 25-49
    //SK anti-neutrino mode: 75-99
    else if(loadAntiNuMode){

      TMatrixDSym* subMatrix = new TMatrixDSym(50);

      //Loop over all matrix entries.  If the row or column number is
      //in the right range, add it in (but if the row or column number is
      //greater than or equal to 25 but less than 50, subtract 25 from it to get the new
      //matrix value for that index.)
      //If it's greater than or equal to 75, subtract 50.
      for(int i = 0; i < flux_cov->GetNrows(); i++){

        for(int j = 0; j < flux_cov->GetNrows(); j++){

          //If i or j  isn't right continue to the next parameter.
          if(50 < i && i < 75) continue;
          if(50 < j && j < 75) continue;
          if(i < 25 || j < 25) continue;

          //The new matrix indices.
          int k;
          int l;

          if( i < 75){

            k = i -25;

          }

          else{
            k = i - 50;
          }

          if(j < 75){

            l = j - 25;

          }else{

            l = j -50;

          }

          //So now we know we have a good flux_cov index and a good
          //subMatrix index.  Populate the submatrix with the
          //corresponding full matrix entry.
          (*subMatrix)(k,l) = (*flux_cov)(i,j);

        }



      }

      //The sub matrix is what is appended to the FitParameters for the
      //flux parameters.
      AppendCovariance(subMatrix);



    }













  }


  //TODO: Close the flux file, as it is no longer needed. Or is it?
  //fluxFile.Close();
}




void FitParameters::AddFluxParameters(TAxis *bins, int det, int flav, int mode, bool vary){

  if(bins==NULL) return;

  //2013 method of doing this.
  //    std::string base("NDNumu");
  //    if(det==0 && flav==1) base = "NDNumub";
  //    if(det==0 && flav==2) base = "NDNue";
  //    if(det==0 && flav==3) base = "NDNueb";
  //    if(det==1 && flav==0) base = "SKNumu";
  //    if(det==1 && flav==1) base = "SKNumub";
  //    if(det==1 && flav==2) base = "SKNue";
  //    if(det==1 && flav==3) base = "SKNueb";

  //Declare the base name for the flux parameter.
  std::string base("");

  //Specify the detector component.
  if(det == 0){

    base = "ND";

  }

  if(det == 1){

    base = "SK";
  }

  //Specify the beam mode.
  if( mode == 1 ){

    base.append("NuMode");

  }

  if( mode == -1 ){

    base.append("ANuMode");
  }

  //Specify the flavour.
  if(flav == 0){

    base.append("Numu");

  }
  if(flav == 1){

    base.append("Numub");

  }
  if(flav == 2){

    base.append("Nue");

  }
  if(flav == 3){

    base.append("Nueb");

  }

  for(int i=0; i<bins->GetNbins(); i++){
    FluxParameter* tmp = new FluxParameter();
    tmp->priorValue = 1.0;
    tmp->currentValue = 1.0;
    tmp->currentValueUD = 1.0;
    tmp->paramType = FLUX;
    tmp->emax = bins->GetBinUpEdge(i+1);
    tmp->emin = bins->GetBinLowEdge(i+1);

    //tmp.lowBound = 0.;
    //tmp.upBound = 999.;

    //Set to the bounds passed to the fitter.
    tmp->lowBound = -30.0;
    tmp->upBound = 30.0;

    tmp->hasScanBounds = false;

    //Set as default to same value as bounds used in fit.
    tmp->lowBoundScan = -30.0;
    tmp->upBoundScan = 30.0;

    tmp->flavour = flav;
    //tmp.xsecID = NORM;
    tmp->detector = det;
    //tmp.spline = false;
    tmp->index = fitParameters.size();
    tmp->typeIndex = fluxIter;
    tmp->vary = vary;
    tmp->priorType = PDFGaus;
    tmp->decompose = 0;
    tmp->throwParm = vary;
    std::string name(Form("%s%d",base.c_str(),i));
    tmp->paramName = name;

    std::string sname(Form("b%d",fluxIter));
    tmp->shortName = sname;

    tmp->beamMode = mode;

    tmp->discrete = false;

    fluxIter++;
    fitParameters.push_back(tmp);
  }

}

void FitParameters::LoadXSecParametersFromFile(std::string fileName, bool vary){

  fxsec = new TFile(fileName.c_str());

  //One of the differences of the new file type is a "name" field.  For
  //compatibility purposes, check for xsec_param_name, and if it doesn't
  //exist, treat this as a 2013 style file.  Otherwise, use the general
  //format meant for 2014 and beyond.
  if((fxsec->Get("xsec_param_names") == NULL)){

    //Get the information we need from the file.
    xsec_cov = (TMatrixDSym*)(fxsec->Get("xsec_cov")); 
    xsec_param_nom = (TVectorD*)(fxsec->Get("xsec_param_nom"));
    xsec_param_prior = (TVectorD*)(fxsec->Get("xsec_param_prior"));
    xsec_param_lb = (TVectorD*)(fxsec->Get("xsec_param_lb"));
    xsec_param_ub = (TVectorD*)(fxsec->Get("xsec_param_ub"));
    xsec_param_id = (TVectorD*)(fxsec->Get("xsec_param_id"));
    xsec_param_throw = (TVectorD*)(fxsec->Get("xsec_param_throw"));
    xsec_param_fit = (TVectorD*)(fxsec->Get("xsec_param_fit"));
    xsec_param_constrain = (TVectorD*)(fxsec->Get("xsec_param_constrain"));
    xsec_param_mode = (TVectorD*)(fxsec->Get("xsec_int_mode"));
    xsec_param_spline = (TVectorD*)(fxsec->Get("xsec_param_spline"));
    xsec_param_weight = (TVectorD*)(fxsec->Get("xsec_param_weight"));
    ccqe_bins = (TAxis*)fxsec->Get("ccqe_ebins");
    cc1pi_bins = (TAxis*)fxsec->Get("cc1pi_ebins");
    nc1pi0_bins = (TAxis*)fxsec->Get("nc1pi0_ebins");
    ncother_bins = (TAxis*)fxsec->Get("ncother_ebins");
    cccoh_bins = (TAxis*)fxsec->Get("cccoh_ebins");

    // If we can't find a weight vector (an older file type) create one with everything set to 0;
    if(xsec_param_weight == NULL){
      xsec_param_weight = new TVectorD(xsec_param_spline->GetNrows());
      for(int i = 0; i < xsec_param_spline->GetNrows(); ++i){
        (*xsec_param_weight)(i) = 0;
      }
    }

    //Turn this information into XSec Norm and XSec Resp parameters.

    //Loop over the parameters.
    int prev_norm = -1;
    int norm_iter = 0;

    for(int i=0; i<xsec_param_nom->GetNrows(); i++){
      bool throw_param = false;

      //If throwing is allowed, whether this parameter is to be varied is
      //dictated entirely by the value of vary passed to this method.
      if( (*xsec_param_throw)(i)>0. ){
        throw_param = (true && vary);
      }

      int* tgtMatArray = new int[1];
      tgtMatArray[0] = -1;
      if( (*xsec_param_id)(i)>=0 ){
        AddXsecRespParameter("", (*xsec_param_prior)(i), (XsecSysts)((int)((*xsec_param_id)(i)+0.00001)), 
                             (*xsec_param_mode)(i), 1, tgtMatArray, (*xsec_param_lb)(i), (*xsec_param_ub)(i), ((*xsec_param_spline)(i)>0.99 ? true : false), ((*xsec_param_weight)(i)>0.99 ? true : false),
                             throw_param, ((*xsec_param_constrain)(i)>0.5) );
        nXSecParms++;
      }

      else {
        TAxis *tmp = NULL;
        int curr_norm = (int)((*xsec_param_mode)(i)+0.01);

        if(curr_norm != prev_norm){
          norm_iter = 0;
        }

        if(curr_norm == 0){
          tmp=ccqe_bins;
        }

        else if(curr_norm == 1){
          tmp=cc1pi_bins;
        }

        else if(curr_norm == 2){
          tmp=cccoh_bins;
        }

        else if(curr_norm == 5){
          tmp=nc1pi0_bins;
        }

        else if(curr_norm == 6){
          tmp=ncother_bins;
        }

        if(tmp != NULL){

          //TODO: Fix this so can still do 2013.
          //AddXsecNormParameter( "", (*xsec_param_prior)(i), curr_norm, -1, tmp, norm_iter, throw_param);
        }

        prev_norm = curr_norm;
        norm_iter++;
        nXSecParms++;
      }
    }    
  }

  //Otherwise, if no xsec_norm_modes assume the 2014 format here.
  else if((fxsec->Get("xsec_norm_modes") == NULL)){

    xsec_cov = (TMatrixDSym*)(fxsec->Get("xsec_cov")); 
    xsec_param_nom = (TVectorD*)(fxsec->Get("xsec_param_nom"));
    xsec_param_prior = (TVectorD*)(fxsec->Get("xsec_param_prior"));
    xsec_param_lb = (TVectorD*)(fxsec->Get("xsec_param_lb"));
    xsec_param_ub = (TVectorD*)(fxsec->Get("xsec_param_ub"));
    xsec_param_throw = (TVectorD*)(fxsec->Get("xsec_param_throw"));
    xsec_param_fit = (TVectorD*)(fxsec->Get("xsec_param_fit"));
    xsec_param_constrain = (TVectorD*)(fxsec->Get("xsec_param_constrain"));
    xsec_param_spline = (TVectorD*)(fxsec->Get("xsec_param_spline"));
    xsec_param_weight = (TVectorD*)(fxsec->Get("xsec_param_weight"));
    xsec_param_name = (TObjArray*)(fxsec->Get("xsec_param_names"));
    xsec_param_binsname = (TObjArray*)(fxsec->Get("xsec_param_binsname"));
    xsec_param_target = (TVectorD*)(fxsec->Get("xsec_param_target"));
    xsec_param_bin = (TVectorD*)(fxsec->Get("xsec_param_bin"));
    xsec_param_discrete = (TVectorD*)(fxsec->Get("xsec_param_discrete"));

    // If we can't find a weight vector (an older file type) create one with everything set to 0;
    if(xsec_param_weight == NULL){
      xsec_param_weight = new TVectorD(xsec_param_spline->GetNrows());
      for(int i = 0; i < xsec_param_spline->GetNrows(); ++i){
        (*xsec_param_weight)(i) = 0;
      }
    }

    for(int i = 0; i < xsec_param_nom->GetNrows(); i++){

      bool throw_param = false;

      //If throwing is allowed, whether this parameter is to be varied is
      //dictated entirely by the value of vary passed to this method.
      if( (*xsec_param_throw)(i)>0. ){
        throw_param = (true && vary);
      }

      //Pull out the parameter name.
      std::string paramName((((TObjString*)(xsec_param_name->At(i)))->GetString()).Data());

      //Check for a target material TVector belonging to this parameter.
      //if it exists, use it to set the target material.
      std::string tgtMatVectorName = paramName + "_targets";

      //Set up the variables for the target materials.
      int nTgtMat;
      int* tgtMatArray;

      //If we don't see the corresponding TVectorD in the file, use the
      //old variable to create and fill the array (with one entry).
      if(fxsec->Get(tgtMatVectorName.c_str()) == NULL){

        nTgtMat = 1;
        tgtMatArray = new int[nTgtMat];

        //Default the target material to -1 (no
        //target specificity).  If xsec_param_target > 0, add a small amount then case to
        //int to ensure it is rounded properly.)
        int tgtMat = -1;
        if((*xsec_param_target)(i) > 0){

          tgtMat = (int)((*xsec_param_target)(i) + 0.01);
        }

        tgtMatArray[0] = tgtMat;

      }

      //Otherwise, use the information in the vector.
      else{

        TVectorD* tgtMatVector = (TVectorD*)(fxsec->Get(tgtMatVectorName.c_str()));

        //The number of target materials is equal to the length of this
        //vector.
        nTgtMat = tgtMatVector->GetNrows();

        //Make the array that size.
        tgtMatArray = new int[nTgtMat];

        //Just use the same method as we did when it was just one
        //number.
        for(int tmvi = 0; tmvi < nTgtMat; tmvi++){

          int tgtMat = -1;
          if((*tgtMatVector)(tmvi) > 0){
            tgtMat = (int)((*tgtMatVector)(tmvi) + 0.01);
          }
          tgtMatArray[tmvi] = tgtMat;   
        }
      }

      //If the id  number of this parameters is greater than or equal to zero, it corresponds to an
      //XSEC response parameter, so add it as one.
      if(ParamNameToXsecSysts(paramName) >=0 ){

        AddXsecRespParameter(paramName, (*xsec_param_prior)(i), ParamNameToXsecSysts(paramName), 
                             -1, nTgtMat, tgtMatArray, (*xsec_param_lb)(i), (*xsec_param_ub)(i), ((*xsec_param_spline)(i)>0.99 ? true : false), ((*xsec_param_weight)(i)>0.99 ? true : false),
                             throw_param, ((*xsec_param_constrain)(i)>0.5), ((*xsec_param_discrete)(i) > 0.5) );
        nXSecParms++;
      }

      //Otherwise it's an XSec Norm parameter with a binning definned by
      //a TAxis in named after the parameter.
      else{
        std::string binsName((((TObjString*)(xsec_param_binsname->At(i)))->GetString()).Data());
        TAxis* param_bins = NULL;
        param_bins = (TAxis*)(fxsec->Get(binsName.c_str()));

        //We also need the TVectorD for this parameter containing the
        //list of reaction codes it applies for.
        std::string reacCodesVecName = paramName + "_reactionCodes";

        TVectorD* reacCodesVec = (TVectorD*)(fxsec->Get(reacCodesVecName.c_str()));

        if (param_bins != NULL){
          AddXsecNormParameter(paramName, (*xsec_param_prior)(i), reacCodesVec, nTgtMat, tgtMatArray, param_bins, (int)((*xsec_param_bin)(i)+0.01), throw_param);

          nXSecParms++;
        }

        //Delete the param_bins pointer now that we're done with it, at
        //least for now.
        delete param_bins;

      }


    }//End loop over parameters to add.


    //If we're not varying the xsec parameters, create one discrete
    //parameter combination that contains the prior values for each, and
    //set that as the only option.
    //Just copy the prior of everything into this array.  The discrete
    //parameters will be picked up as needed, and only them by the code
    //elsewhere.
    if(!vary){

      nDiscreteParameterCombinations = 1;
      discreteParameterCombinations = new double*[1];
      discreteParameterCombinations[0] = new double[fitParameters.size()];

      for(unsigned int fpi = 0; fpi < fitParameters.size(); fpi++){
        discreteParameterCombinations[0][fpi] = fitParameters[fpi]->priorValue;
      }
    }
    else{

      //Otherwise, set up the full array, loading multiple allowed
      //combinations from a file.
      //Now that all the parameters are added, the fitParameters internal
      //array is as large as it needs to be to consider up to and including
      //the discrete parameters.  So, create the array that stores the
      //allowed discrete parameter combinations, and then fill it with
      //information from the file at the corresponding parameter index by
      //matching up the name.
      xsec_param_discreteNames = (TObjArray*)(fxsec->Get("xsec_param_discreteNames"));

      //The number of entries in this TObjArray is the number of discrete
      //parameters.
      nDiscreteParameters = xsec_param_discreteNames->GetEntries();


      xsec_param_discreteCombinations = (TObjArray*)(fxsec->Get("xsec_param_discreteCombinations"));

      //The number of discrete parameter combinations is the number of
      //entries in this TObjArray.
      nDiscreteParameterCombinations = xsec_param_discreteCombinations->GetEntries();

      //We can now create the array to store these discrete parameter
      //combinations in.
      discreteParameterCombinations = new double*[nDiscreteParameterCombinations];

      for(int dpi = 0; dpi < nDiscreteParameterCombinations; dpi++){

        discreteParameterCombinations[dpi] = new double[fitParameters.size()];

        //Copy in the prior value for everything to initialize.
        for(unsigned int fpi = 0; fpi < fitParameters.size(); fpi++){

          discreteParameterCombinations[dpi][fpi] = fitParameters[fpi]->priorValue;

        }

      }

      //With the array made, now we fill it.
      //Loop over the discrete parameter combinations
      for(int dpi = 0; dpi < nDiscreteParameterCombinations; dpi++){

        //Grab the TVectorD of discrete parameter values allowed for
        //this combinations.
        TVectorD* currentCombination = (TVectorD*)(xsec_param_discreteCombinations->At(dpi));

        //Loop through the names of the discrete parameters.
        for(int dpn = 0; dpn < nDiscreteParameters; dpn++){

          //Grab the name as a string.
          std::string currentDiscreteName((((TObjString*)(xsec_param_discreteNames->At(dpn)))->GetString()).Data());

          //Now, loop through the fit parameters.  If the name
          //matches, copy the value from the TVectorD for this
          //discrete parameter index into the fitParameter index of
          //the discreteParameterCombinations array.
          for(unsigned int fpi = 0; fpi < fitParameters.size(); fpi++){

            if(currentDiscreteName == fitParameters[fpi]->paramName){

              discreteParameterCombinations[dpi][fpi] = (*currentCombination)(dpn);

            }

          } //End loop over fit parameters.

        } //End loop over discrete parameters.
      } //End loop over discrete parameter combinations.

    } //End addition of discrete parameter combinations.

    //Having set up the discrete parameters, print out the combinations
    //that the BANFF code will consider.
    std::cout << "=================================================" << std::endl;
    std::cout << "Total allowed discrete parameter combinations: " << nDiscreteParameterCombinations << std::endl;
    std::cout << "=================================================" << std::endl;
    for(int dpi = 0; dpi < nDiscreteParameterCombinations; dpi++){

      std::cout << "--------------------------------------------" << std::endl;
      std::cout << "Combination: " << dpi << std::endl;
      std::cout << "--------------------------------------------" << std::endl;

      for(unsigned int fpi = 0 ; fpi < fitParameters.size(); fpi++){

        if(fitParameters[fpi]->discrete){

          std::cout << fitParameters[fpi]->paramName << ": " << discreteParameterCombinations[dpi][fpi] << std::endl;
        }
      }
    }

    std::cout << "=================================================" << std::endl;
  }
  //Otherwise, assume the 2016 format here.
  else{

    xsec_cov             = (TMatrixDSym*)(fxsec->Get("xsec_cov")); 
    xsec_param_nom       = (TVectorD*)   (fxsec->Get("xsec_param_nom"));
    xsec_param_prior     = (TVectorD*)   (fxsec->Get("xsec_param_prior"));
    xsec_param_lb        = (TVectorD*)   (fxsec->Get("xsec_param_lb"));
    xsec_param_ub        = (TVectorD*)   (fxsec->Get("xsec_param_ub"));
    xsec_param_throw     = (TVectorD*)   (fxsec->Get("xsec_param_throw"));
    xsec_param_fit       = (TVectorD*)   (fxsec->Get("xsec_param_fit"));
    xsec_param_constrain = (TVectorD*)   (fxsec->Get("xsec_param_constrain"));
    xsec_param_spline    = (TVectorD*)   (fxsec->Get("xsec_param_spline"));
    xsec_param_weight    = (TVectorD*)   (fxsec->Get("xsec_param_weight"));
    xsec_param_name      = (TObjArray*)  (fxsec->Get("xsec_param_names"));
    xsec_param_binsname  = (TObjArray*)  (fxsec->Get("xsec_param_binsname"));
    xsec_param_target    = (TVectorD*)   (fxsec->Get("xsec_param_target"));
    xsec_param_bin       = (TVectorD*)   (fxsec->Get("xsec_param_bin"));
    xsec_param_discrete  = (TVectorD*)   (fxsec->Get("xsec_param_discrete"));
    xsec_norm_modes      = (TObjArray*)  (fxsec->Get("xsec_norm_modes"));
    xsec_norm_elements   = (TObjArray*)  (fxsec->Get("xsec_norm_elements"));
    xsec_norm_nupdg      = (TObjArray*)  (fxsec->Get("xsec_norm_nupdg"));
    xsec_param_id        = (TVectorD*)   (fxsec->Get("xsec_param_id"));

    // If we can't find a weight vector (an older file type) create one with everything set to 0;
    if(xsec_param_weight == NULL){
      xsec_param_weight = new TVectorD(xsec_param_spline->GetNrows());
      for(int i = 0; i < xsec_param_spline->GetNrows(); ++i){
        (*xsec_param_weight)(i) = 0;
      }
    }

    for(int i = 0; i < xsec_param_nom->GetNrows(); i++){

      bool throw_param = false;

      //If throwing is allowed, whether this parameter is to be varied is
      //dictated entirely by the value of vary passed to this method.
      if( (*xsec_param_throw)(i)>0. ){
        throw_param = (true && vary);
      }

      //Pull out the parameter name.
      std::string paramName((((TObjString*)(xsec_param_name->At(i)))->GetString()).Data());

      //Set up the variables for the target materials.
      int nTgtMat;
      int* tgtMatArray;

      //Check for a target material TVector belonging to this parameter.
      //if it exists, use it to set the target material.
      if(xsec_norm_elements->At(i) != NULL){
        TVectorD* tgtMatVector = (TVectorD*)(xsec_norm_elements->At(i));

        //The number of target materials is equal to the length of this
        //vector.
        nTgtMat = tgtMatVector->GetNrows();

        //Make the array that size.
        tgtMatArray = new int[nTgtMat];

        //Just use the same method as we did when it was just one
        //number.
        for(int tmvi = 0; tmvi < nTgtMat; tmvi++){

          int tgtMat = -1;
          if((*tgtMatVector)(tmvi) > 0){
            tgtMat = (int)((*tgtMatVector)(tmvi) + 0.01);
          }
          else if((*tgtMatVector)(tmvi) < -900){
            tgtMat = -999;
          }
          tgtMatArray[tmvi] = tgtMat;   
        }
      }
      //If we don't see the corresponding TVectorD in the norm TObjArray, use the
      //old variable to create and fill the array (with one entry).
      else{

        nTgtMat = 1;
        tgtMatArray = new int[nTgtMat];

        //Default the target material to -1 (no
        //target specificity).  If xsec_param_target > 0, add a small amount then case to
        //int to ensure it is rounded properly.)
        int tgtMat = -1;
        if((*xsec_param_target)(i) > 0){

          tgtMat = (int)((*xsec_param_target)(i) + 0.01);
        }

        tgtMatArray[0] = tgtMat;

      }

      //If the id  number of this parameters is greater than or equal to zero, it corresponds to an
      //XSEC response parameter, so add it as one.
      if(ParamNameToXsecSysts(paramName) >=0 ){

        std::cout<<"Response xsec "<<paramName<<std::endl;
        AddXsecRespParameter(paramName, (*xsec_param_prior)(i), ParamNameToXsecSysts(paramName), 
                             -1, nTgtMat, tgtMatArray, (*xsec_param_lb)(i), (*xsec_param_ub)(i), ((*xsec_param_spline)(i)>0.99 ? true : false), ((*xsec_param_weight)(i)>0.99 ? true : false),
                             throw_param, ((*xsec_param_constrain)(i)>0.5), ((*xsec_param_discrete)(i) > 0.5) );
        nXSecParms++;
      }

      //If not XsecResp and id == -1 it's an XSec Norm parameter with a binning defined by
      //a TAxis in named after the parameter.
      else if((*xsec_param_id)(i)>-1.5){
        std::string binsName((((TObjString*)(xsec_param_binsname->At(i)))->GetString()).Data());
        TAxis* param_bins = NULL;
        param_bins = (TAxis*)(fxsec->Get(binsName.c_str()));

        //We also need the TVectorD for this parameter containing the
        //list of reaction codes it applies for.

        TVectorD* reacCodesVec = NULL;
        TVectorD* nupdgVec = NULL;
        if(xsec_norm_nupdg->At(i)!=NULL) nupdgVec = (TVectorD*)(xsec_norm_nupdg->At(i));
        if(xsec_norm_modes->At(i)!=NULL && xsec_norm_nupdg->At(i)!=NULL) reacCodesVec = FitParameters::MACH3toNEUTReacCode((TVectorD*)(xsec_norm_modes->At(i)), FitParameters::IsNeutrinoorAntiNeutrino(nupdgVec));	
				
        if (param_bins != NULL){
          std::cout<<"Adding the parameter "<< paramName << " to the list" << std::endl;
					
          AddXsecNormParameter2016(paramName, (*xsec_param_prior)(i), reacCodesVec,nupdgVec, nTgtMat, tgtMatArray, param_bins, (int)((*xsec_param_bin)(i)+0.01), throw_param, ((*xsec_param_constrain)(i)>0.5));

          nXSecParms++;
        }

        //Delete the param_bins pointer now that we're done with it, at
        //least for now.
        delete param_bins;
        delete reacCodesVec;
        delete nupdgVec;
      }
      //Functionnal parameters (BeRPA)
      else if((*xsec_param_id)(i)> -2.5){

        std::cout<<"Functionnal xsec "<<paramName<<std::endl;
        nTgtMat = 2;
        tgtMatArray = new int[nTgtMat];
        tgtMatArray[0] = 12;tgtMatArray[1] = 16;
				
        TVectorD* reacCodesVec = new TVectorD(2);
        TVectorD* nupdgVec  = new TVectorD(4);
        (*reacCodesVec)(0)= -1; (*reacCodesVec)(1)= 1;
        (*nupdgVec)(0)= -14; (*nupdgVec)(1)= -12; (*nupdgVec)(2)= 12;(*nupdgVec)(3)= 14;

				
        AddXsecFuncParameter(paramName, (*xsec_param_prior)(i),
                             reacCodesVec,nupdgVec, nTgtMat, tgtMatArray, (*xsec_param_lb)(i), (*xsec_param_ub)(i),
                             throw_param, ((*xsec_param_constrain)(i)>0.5), ((*xsec_param_discrete)(i) > 0.5) );
        nXSecParms++;
        delete reacCodesVec;
        delete nupdgVec;
      }


    }//End loop over parameters to add.


    //If we're not varying the xsec parameters, create one discrete
    //parameter combination that contains the prior values for each, and
    //set that as the only option.
    //Just copy the prior of everything into this array.  The discrete
    //parameters will be picked up as needed, and only them by the code
    //elsewhere.
    if(!vary){

      nDiscreteParameterCombinations = 1;
      discreteParameterCombinations = new double*[1];
      discreteParameterCombinations[0] = new double[fitParameters.size()];

      for(unsigned int fpi = 0; fpi < fitParameters.size(); fpi++){
        discreteParameterCombinations[0][fpi] = fitParameters[fpi]->priorValue;
      }
    }
    else{

      //Otherwise, set up the full array, loading multiple allowed
      //combinations from a file.
      //Now that all the parameters are added, the fitParameters internal
      //array is as large as it needs to be to consider up to and including
      //the discrete parameters.  So, create the array that stores the
      //allowed discrete parameter combinations, and then fill it with
      //information from the file at the corresponding parameter index by
      //matching up the name.
      xsec_param_discreteNames = (TObjArray*)(fxsec->Get("xsec_param_discreteNames"));

      //The number of entries in this TObjArray is the number of discrete
      //parameters.
      nDiscreteParameters = xsec_param_discreteNames->GetEntries();


      xsec_param_discreteCombinations = (TObjArray*)(fxsec->Get("xsec_param_discreteCombinations"));

      //The number of discrete parameter combinations is the number of
      //entries in this TObjArray.
      nDiscreteParameterCombinations = xsec_param_discreteCombinations->GetEntries();

      //We can now create the array to store these discrete parameter
      //combinations in.
      discreteParameterCombinations = new double*[nDiscreteParameterCombinations];

      for(int dpi = 0; dpi < nDiscreteParameterCombinations; dpi++){

        discreteParameterCombinations[dpi] = new double[fitParameters.size()];

        //Copy in the prior value for everything to initialize.
        for(unsigned int fpi = 0; fpi < fitParameters.size(); fpi++){

          discreteParameterCombinations[dpi][fpi] = fitParameters[fpi]->priorValue;

        }

      }

      //With the array made, now we fill it.
      //Loop over the discrete parameter combinations
      for(int dpi = 0; dpi < nDiscreteParameterCombinations; dpi++){

        //Grab the TVectorD of discrete parameter values allowed for
        //this combinations.
        TVectorD* currentCombination = (TVectorD*)(xsec_param_discreteCombinations->At(dpi));

        //Loop through the names of the discrete parameters.
        for(int dpn = 0; dpn < nDiscreteParameters; dpn++){

          //Grab the name as a string.
          std::string currentDiscreteName((((TObjString*)(xsec_param_discreteNames->At(dpn)))->GetString()).Data());

          //Now, loop through the fit parameters.  If the name
          //matches, copy the value from the TVectorD for this
          //discrete parameter index into the fitParameter index of
          //the discreteParameterCombinations array.
          for(unsigned int fpi = 0; fpi < fitParameters.size(); fpi++){

            if(currentDiscreteName == fitParameters[fpi]->paramName){

              discreteParameterCombinations[dpi][fpi] = (*currentCombination)(dpn);

            }

          } //End loop over fit parameters.

        } //End loop over discrete parameters.
      } //End loop over discrete parameter combinations.

    } //End addition of discrete parameter combinations.

    //Having set up the discrete parameters, print out the combinations
    //that the BANFF code will consider.
    std::cout << "=================================================" << std::endl;
    std::cout << "Total allowed discrete parameter combinations: " << nDiscreteParameterCombinations << std::endl;
    std::cout << "=================================================" << std::endl;
    for(int dpi = 0; dpi < nDiscreteParameterCombinations; dpi++){

      std::cout << "--------------------------------------------" << std::endl;
      std::cout << "Combination: " << dpi << std::endl;
      std::cout << "--------------------------------------------" << std::endl;

      for(unsigned int fpi = 0 ; fpi < fitParameters.size(); fpi++){

        if(fitParameters[fpi]->discrete){

          std::cout << fitParameters[fpi]->paramName << ": " << discreteParameterCombinations[dpi][fpi] << std::endl;
        }
      }
    }

    std::cout << "=================================================" << std::endl;
  }
  //Once all the parameters have been added, add their covariance as well.
  AppendCovariance(xsec_cov);


  //TODO: Close the XSec file here, or somewhere else?
}

void FitParameters::AddXsecRespParameter(std::string paramName, double value, XsecSysts parID, int reactioncode, int nTgtMat, int* tgtMatArray, double lowerbound, double upperbound, bool doSpline, bool doWeight, bool vary, bool constrain, bool isDiscrete){

  std::string name("OTHER");
  //To maintain compatibility with the 2013 style XSec file, check if
  //paramName is empty here, and default to the 2013 treatment if it is.
  if(paramName == ""){

    if(parID == MAQE) name = "MAQE";
    else if(parID == MARES) name = "MARES";
    else if(parID == DISMPISHP) name = "DISMPISHP";
    else if(parID == SF ) name = "SF";
    else if(parID == EB ) name = "EB";
    else if(parID == PF ) name = "PF";
    else if(parID == PDD) name = "PDD";
    else if(parID == FSI_PI_ABS) name = "FSI_PI_ABS";
    else if(parID == FSI_PI_PROD) name = "FSI_PI_PROD";
    else if(parID == FSI_INEL_LO) name = "FSI_INEL_LO";
    else if(parID == FSI_INEL_HI) name = "FSI_INEL_HI";
    else if(parID == FSI_CEX_LO) name = "FSI_CEX_LO";
    else if(parID == FSI_CEX_HI) name = "FSI_CEX_HI";
    else if(parID == PDD_MEC) name = "PDD_MEC";
    else if(parID == MEC) name = "MEC";
  }

  //Otherwise, use the supplied name.
  else{
    name = paramName;
  }

  XSecRespParameter* tmp = new XSecRespParameter();
  tmp->priorValue = value;
  tmp->currentValue = value;
  tmp->currentValueUD = value;
  tmp->paramType = XSECRESP;
  tmp->emax = 0.;
  tmp->emin = 0.;
  tmp->lowBound = lowerbound;
  tmp->upBound = upperbound;

  tmp->hasScanBounds = true;
  tmp->lowBoundScan = lowerbound;
  tmp->upBoundScan = upperbound;

  tmp->reactionCode = reactioncode;
  tmp->nTargetMaterials = nTgtMat;
  tmp->targetMaterial = tgtMatArray;
  tmp->xsecID = parID;
  tmp->detector = -1;
  tmp->spline = doSpline;
  tmp->weight = doWeight;
  tmp->index = fitParameters.size();
  tmp->typeIndex = xsecRespIter;
  tmp->vary = vary;
  tmp->throwParm = vary;
  if(constrain){
    tmp->priorType = PDFGaus;
  }
  else{
    tmp->priorType = PDFFlat;
  }
  tmp->paramName = name;

  std::string sname(Form("x%d",xsecRespIter));
  tmp->shortName = sname;



  tmp->decompose = 0;
  tmp->discrete = isDiscrete;
  xsecRespIter++;
  fitParameters.push_back(tmp);

}

void FitParameters::AddXsecFuncParameter(std::string paramName, double value, TVectorD* reacCodes, TVectorD* nuPDG, int nTgtMat, int* tgtMatArray, double lowerbound, double upperbound, bool vary, bool constrain, bool isDiscrete){

  XSecFuncParameter* tmp = new XSecFuncParameter();
  tmp->priorValue = value;
  tmp->currentValue = value;
  tmp->currentValueUD = value;
  tmp->paramType = XSECFUNC;
  tmp->emax = 0.;
  tmp->emin = 0.;
  tmp->lowBound = lowerbound;
  tmp->upBound = upperbound;

  tmp->hasScanBounds = true;
  tmp->lowBoundScan = lowerbound;
  tmp->upBoundScan = upperbound;
	
  std::cout << "===============================================" << std::endl;
  if(reacCodes == NULL){
    tmp->nReactionCodes = 0;
    tmp->reactionCodes = new int[1];
    tmp->reactionCodes[0] = -999;
  }
  else{ 
    tmp->nReactionCodes = reacCodes->GetNrows();
    tmp->reactionCodes = new int[tmp->nReactionCodes];
    std::cout << "XSecFuncParameter " << paramName << " applies for the following reaction codes: " << std::endl;
    for(int j = 0; j < tmp->nReactionCodes; j++){

      //If it's positive, add 0.01 to ensure that binary vs. decimal precison
      //issues don't cause the cast to go to the incorrect int value.
      if((*reacCodes)(j) > 0.0){
        tmp->reactionCodes[j] = (int)((*reacCodes)(j) + 0.01);
      }

      //Otherwise, it's a negative reaction code, so we need to subtract 0.01
      //instead (for the same reason).
      else{
        tmp->reactionCodes[j] = (int)((*reacCodes)(j) - 0.01);
      }

      //Print out the reaction code that we just added so that we can cross
      //check on loading.
      std::cout << tmp->reactionCodes[j] << std::endl;
    }
  }
  if(nuPDG == NULL){
    tmp->nNupdg = 0;
    tmp->Nupdg = new int[1];
    tmp->Nupdg[0] = -999;
  }
  else{
    tmp->nNupdg = nuPDG->GetNrows();
    tmp->Nupdg = new int[tmp->nNupdg];
    std::cout << "With neutrino PDG:" << std::endl;
    for(int j = 0; j < tmp->nNupdg; j++){

      //If it's positive, add 0.01 to ensure that binary vs. decimal precison
      //issues don't cause the cast to go to the incorrect int value.
      if((*nuPDG)(j) > 0.0){
        tmp->Nupdg[j] = (int)((*nuPDG)(j) + 0.01);
      }

      //Otherwise, it's a negative nuPDG, so we need to subtract 0.01
      //instead (for the same reason).
      else{
        tmp->Nupdg[j] = (int)((*nuPDG)(j) - 0.01);
      }

      //Print out nuPDG that we just added so that we can cross
      //check on loading.
      std::cout << tmp->Nupdg[j] << std::endl;
    }
  }
  std::cout << "On target materials:" << std::endl;
  for(int tmi = 0; tmi < nTgtMat; tmi++){
    std::cout << tgtMatArray[tmi] << std::endl;
  }
  std::cout << "===============================================" << std::endl;
	
  tmp->nTargetMaterials = nTgtMat;
  tmp->targetMaterial = tgtMatArray;
  tmp->detector = -1;
  tmp->index = fitParameters.size();
  tmp->typeIndex = xsecFuncIter;
  tmp->vary = vary;
  tmp->throwParm = vary;
  if(constrain){
    tmp->priorType = PDFGaus;
  }
  else{
    tmp->priorType = PDFFlat;
  }
  tmp->paramName = paramName;

  std::string sname(Form("xf%d",xsecFuncIter));
  tmp->shortName = sname;

  tmp->decompose = 0;
  tmp->discrete = isDiscrete;
  xsecFuncIter++;
  fitParameters.push_back(tmp);

  //If first BeRPA parameter set position to fitParameters index
  if(BeRPA_pos < 0){
    if(paramName.find("BeRPA")!=std::string::npos){
      BeRPA_pos = fitParameters.size()-1;
    }
  }

}

void FitParameters::LoadXSecResponses(BANFFEventBase* event, WeightFile* sumTree){

  const int nSamples = sumTree->NSamples;

  //Loop through the fit parameters.
  for(unsigned int i = 0; i < fitParameters.size(); i++){

    //We're only interested in cross section response parameters at this
    //point, so if this is not one, continue to the next parameter.
    if(fitParameters[i]->paramType != XSECRESP){

      continue;
    }

    //Now that we know it's an XSecRespParameter, can static cast directly
    //to it and deal with the XSecRespParameter specific variables needed
    //to add splines to the event.
    XSecRespParameter* param = static_cast<XSecRespParameter*>(fitParameters[i]);

    XsecSysts systID = param->xsecID;
    int index = param->typeIndex;
		
    //Make sure that nubar is applied to nubar only and same thing for nu
    //if((systID == MEC_shape_C_nu || systID == MEC_shape_O_nu ) && event->GetNuFlavor() < 0 ) continue;
    //if((systID == MEC_shape_C_nubar || systID == MEC_shape_O_nubar) && event->GetNuFlavor() > 0 ) continue;
    std::string param_Name = fitParameters[i]->paramName ;
    std::string nubar ("nubar");
    std::string nu ("nu");
    std::size_t found_nu = param_Name.find(nu);
    std::size_t found_nubar = param_Name.find(nubar);
    if(found_nu!=std::string::npos && found_nubar==std::string::npos && event->GetNuFlavor() < 0 ) {
      //std::cout<<param_Name<<" Found nu with xsexid "<< systID<< " "<<event->GetNuFlavor()<<std::endl;
      continue;
    }
    if(found_nubar!=std::string::npos && event->GetNuFlavor() > 0 ){
      //std::cout<<param_Name<<" Found nubar with xsexid "<< systID<< " "<<event->GetNuFlavor()<<std::endl;
      continue;
    }
		
    // Splines and graphs have same number of points regardless of which sample the event falls in
    // but any empty sample has only one point, so always search through to find the filled sample
    // Spline changes are currently broken and cause a crash if using the  non-low memory functions
    // Therefore these are commented out for now
    if(param->spline){
      event->AddXsecResponse(index, NULL, sumTree->GetResponseSpline(systID));
      int sampleNum = int(SampleId::kFGD1NuMuCC0Pi);
      TSpline3* spline = event->GetXSecSpline(index, sampleNum);
      sampleNum++;
      while(spline == NULL && sampleNum < nSamples){
        spline = event->GetXSecSpline(index, sampleNum);
        sampleNum++;
      }
      if(spline == NULL || spline->GetNp() < 2) continue;
      param->stepSize = double(spline->GetXmax() - spline->GetXmin())/double(spline->GetNp()-1);
      param->xMax = spline->GetXmax()-param->stepSize;
      param->xMin = spline->GetXmin()+param->stepSize;
      param->nStep = spline->GetNp()-1;
			
      //Check that the knots are equidistants if not set nstep = -1
      // Use 15 size arrays for now, should never have more than 15 knots in a spline
      Double_t x[15];
      Double_t y[15];
      Double_t diff = 0;
      for(int j=0;j<spline->GetNp();j++){
        spline->GetKnot(j,x[j],y[j]);
        if(j>1){
          diff = fabs(x[j]-x[j-1])-fabs(x[j-1]-x[j-2]);
          if(diff>1e-5){
            param->nStep = -1;
            break;
          }
        }
      }
    }
    else if(param->weight){
      event->AddXsecWeight(index, sumTree->GetXSecWeightArray(systID));
    }
    else{
      event->AddXsecResponse(index, sumTree->GetResponseGraph(systID), NULL);
      int sampleNum = int(SampleId::kFGD1NuMuCC0Pi);
      TGraph* graph = event->GetXSecGraph(index, sampleNum);
      sampleNum++;
      while(graph == NULL && sampleNum < nSamples){
        graph = event->GetXSecGraph(index, sampleNum);
        sampleNum++;
      }
      if(graph == NULL || graph->GetN() < 2) continue;
      double* arr = new double[graph->GetN()];
      arr = graph->GetX();
      param->stepSize = double((arr[graph->GetN()-1] - arr[0]))/double(graph->GetN()-1);
      param->xMax = arr[graph->GetN()-1] - param->stepSize;
      param->xMin = arr[0] + param->stepSize;
      param->nStep = graph->GetN()-1;
    }
  }
}

void FitParameters::LoadXSecResponsesLowMem(BANFFEventBase* event, WeightFile* sumTree){

  //Loop through the fit parameters, getting response splines and graphs for
  //this event's sample only, and counting them.
  for(unsigned int i = 0; i < fitParameters.size(); i++){

    //We're only interested in cross section response parameters at this
    //point, so if this is not one, continue to the next parameter.
    if(fitParameters[i]->paramType != XSECRESP){

      continue;
    }

    //Now that we know it's an XSecRespParameter, can static cast directly
    //to it and deal with the XSecRespParameter specific variables needed
    //to add splines to the event.
    XSecRespParameter* param = static_cast<XSecRespParameter*>(fitParameters[i]);

    XsecSysts systID = param->xsecID;

    //Make sure that nubar is applied to nubar only and same thing for nu
    //if((systID == MEC_shape_C_nu || systID == MEC_shape_O_nu ) && event->GetNuFlavor() < 0 ) continue;
    //if((systID == MEC_shape_C_nubar || systID == MEC_shape_O_nubar) && event->GetNuFlavor() > 0 ) continue;
    std::string param_Name = fitParameters[i]->paramName ;
    std::string nubar ("nubar");
    std::string nu ("nu");
    std::size_t found_nu = param_Name.find(nu);
    std::size_t found_nubar = param_Name.find(nubar);
    if(found_nu!=std::string::npos && found_nubar==std::string::npos && event->GetNuFlavor() < 0 ) {
      //std::cout<<param_Name<<" Found nu with xsexid "<< systID<< " "<<event->GetNuFlavor()<<std::endl;
      continue;
    }
    if(found_nubar!=std::string::npos && event->GetNuFlavor() > 0 ){
      //std::cout<<param_Name<<" Found nubar with xsexid "<< systID<< " "<<event->GetNuFlavor()<<std::endl;
      continue;
    }			
    if(param->spline){

      //Make sure we aren't getting NULL for the spline.
      TSpline3** tmpSpline = sumTree->GetResponseSpline(systID);

      if(tmpSpline == NULL){
        continue;
      }

      //So if we get to here we can access the spline array contents.
      //If it's NULL for this event's sample, we're not interested.
      if(tmpSpline[event->sample->sampleID] == NULL){
        continue;
      }

      //If we get here, it is not NULL.  Increment the spline counter.
      event->nContXSRTSpline3s++;

    }
    else if (param->weight){
      event->nDiscXSRTWeights++;
    }
    //If we're not instructed to use a spline, check the TGraphs instead,
    //separating the discrete parameters from the non-discrete ones.
    else{

      //Make sure we aren't getting NULL for the TGraph TClonesArray.
      TClonesArray* tmpGraph = sumTree->GetResponseGraph(systID);

      if(tmpGraph == NULL){
        continue;
      }
      //So if we get to here we can access the TGraph TClonesArray contents.
      //If it's NULL for this event's sample, we're not interested.
      if(tmpGraph->At(event->sample->sampleID) == NULL){
        continue;
      }
      //We're only interested in this TGraph if it has more than 1 point.
      if(((TGraph*)(tmpGraph->At(event->sample->sampleID)))->GetN() > 1){
        //If this is a discrete parameter, count it as one.
        if(param->discrete){
          event->nDiscXSRTGraphs++;
        }
        else{
          event->nContXSRTGraphs++;
        }

      }

    }


  } //End first loop over the fit parameters.

    //Now that we know exactly how big the arrays need to be, we can construct
    //them.
  event->contXSRTGraphIndices = new int[event->nContXSRTGraphs];
  event->contXSRTGraphs = new TGraph*[event->nContXSRTGraphs];

  event->contXSRTSpline3Indices = new int[event->nContXSRTSpline3s];
  event->contXSRTSpline3s = new TSpline3*[event->nContXSRTSpline3s];

  event->discXSRTGraphIndices = new int[event->nDiscXSRTGraphs];
  event->discXSRTGraphs = new TGraph*[event->nDiscXSRTGraphs];

  event->discXSRTWeightIndices = new int[event->nDiscXSRTWeights];
  event->discXSRTWeights = new double[event->nDiscXSRTWeights];

  //Now go through the parameters as above, loading the correct responses
  //into the correct locations.
  //Loop through the fit parameters, getting response splines and graphs for
  //this event's sample only, and counting them.
  int currentContXSRTGraph = 0;
  int currentContXSRTSpline3 = 0;
  int currentDiscXSRTGraph = 0;
  int currentDiscXSRTWeight = 0;
  for(unsigned int i = 0; i < fitParameters.size(); i++){

    //We're only interested in cross section response parameters at this
    //point, so if this is not one, continue to the next parameter.
    if(fitParameters[i]->paramType != XSECRESP){

      continue;
    }

    //Now that we know it's an XSecRespParameter, can static cast directly
    //to it and deal with the XSecRespParameter specific variables needed
    //to add splines to the event.
    XSecRespParameter* param = static_cast<XSecRespParameter*>(fitParameters[i]);

    XsecSysts systID = param->xsecID;

    //Make sure that nubar is applied to nubar only and same thing for nu
    //if((systID == MEC_shape_C_nu || systID == MEC_shape_O_nu ) && event->GetNuFlavor() < 0 ) continue;
    //if((systID == MEC_shape_C_nubar || systID == MEC_shape_O_nubar) && event->GetNuFlavor() > 0 ) continue;
    std::string param_Name = fitParameters[i]->paramName ;
    std::string nubar ("nubar");
    std::string nu ("nu");
    std::size_t found_nu = param_Name.find(nu);
    std::size_t found_nubar = param_Name.find(nubar);
    if(found_nu!=std::string::npos && found_nubar==std::string::npos && event->GetNuFlavor() < 0 ) {
      //std::cout<<param_Name<<" 2 Found nu with xsexid "<< systID<< " "<<event->GetNuFlavor()<<std::endl;
      continue;
    }
    if(found_nubar!=std::string::npos && event->GetNuFlavor() > 0 ){
      //std::cout<<param_Name<<" 2 Found nubar with xsexid "<< systID<< " "<<event->GetNuFlavor()<<std::endl;
      continue;
    }	
		
    if(param->spline){

      //Make sure we aren't getting NULL for the spline.
      TSpline3** tmpSpline = sumTree->GetResponseSpline(systID);

      if(tmpSpline == NULL){
        continue;
      }

      //So if we get to here we can access the spline array contents.
      //If it's NULL for this event's sample, we're not interested.
      if(tmpSpline[event->sample->sampleID] == NULL){
        continue;
      }

      //If we get here, it's a spline we want to add.  Add it into the
      //spline array at the current index, the increment the index.  Also
      //store the fit parameter index at the current spline index so we
      //know which fit parameter it is associated with.

      //Fit parameter index related to this spline.
      event->contXSRTSpline3Indices[currentContXSRTSpline3] = i;

      //The actual spline.
      TSpline3 *tmp_sp = (TSpline3*)(tmpSpline[event->sample->sampleID]->Clone(Form("sp_%d_%d",event->GetSumTreeIndex(),i)));
      event->contXSRTSpline3s[currentContXSRTSpline3] = tmp_sp;
      param->stepSize = double(tmp_sp->GetXmax() - tmp_sp->GetXmin())/double(tmp_sp->GetNp()-1);
      param->xMax = tmp_sp->GetXmax()-param->stepSize;
      param->xMin = tmp_sp->GetXmin()+param->stepSize;
      param->nStep = tmp_sp->GetNp()-1;
			

      //Check that the knots are equidistants if not set nstep = -1
      // Use 15 size arrays for now, should never have more than 15 knots in a spline
      Double_t x[15];
      Double_t y[15];
      Double_t diff = 0;
      for(int j=0;j<tmp_sp->GetNp();j++){
        tmp_sp->GetKnot(j,x[j],y[j]);
        if(j>1){
          diff = fabs(x[j]-x[j-1])-fabs(x[j-1]-x[j-2]);
          if(diff>1e-5){
            param->nStep = -1;
            break;
          }
        }
      }
			
      //Increment the current continuous TSpline counter.
      /*std::cout<<"================================================"<<std::endl;
        std::cout<<"Added Spline "<<fitParameters[i]->paramName<<std::endl;
        std::cout<<fitParameters[i]->paramName<<" "<<param->stepSize<<" "<<param->xMax<<" "<<param->xMin<<" "<<param->nStep<<std::endl;
        std::cout<<"================================================"<<std::endl;*/
			
      currentContXSRTSpline3++;

    }
    else if(param->weight){
      double* weight_arr = sumTree->GetXSecWeightArray(systID);
      event->discXSRTWeightIndices[currentDiscXSRTWeight] = i;
      event->discXSRTWeights[currentDiscXSRTWeight] = weight_arr[event->sample->sampleID];
      currentDiscXSRTWeight++;
    }
    //If we're not instructed to use a spline, check the TGraphs instead,
    //separating the discrete parameters from the non-discrete ones.
    else{

      //Make sure we aren't getting NULL for the TGraph TClonesArray.
      TClonesArray* tmpGraph = sumTree->GetResponseGraph(systID);

      if(tmpGraph == NULL){
        continue;
      }

      //So if we get to here we can access the TGraph TClonesArray contents.
      //If it's NULL for this event's sample, we're not interested.
      if(tmpGraph->At(event->sample->sampleID) == NULL){
        continue;
      }

      //We're only interested in this TGraph if it has more than 1 point.
      if(((TGraph*)(tmpGraph->At(event->sample->sampleID)))->GetN() > 1){

        //If this is a discrete parameter, deal with it as one.
        if(param->discrete){
          event->discXSRTGraphIndices[currentDiscXSRTGraph] = i;
          TGraph* tmp_gr = (TGraph*)((tmpGraph->At(event->sample->sampleID))->Clone(Form("gr_%d_%d",event->GetSumTreeIndex(),i)));

          event->discXSRTGraphs[currentDiscXSRTGraph] = tmp_gr;
          currentDiscXSRTGraph++; 
        }

        //Otherwise, include it as a continuous parameter.
        else{
          event->contXSRTGraphIndices[currentContXSRTGraph] = i;
          TGraph* tmp_gr = (TGraph*)((tmpGraph->At(event->sample->sampleID))->Clone(Form("gr_%d_%d",event->GetSumTreeIndex(),i)));
          double* arr = tmp_gr->GetX();
          param->stepSize = double((arr[tmp_gr->GetN()-1] - arr[0]))/double(tmp_gr->GetN()-1);
          param->xMax = arr[tmp_gr->GetN()-1] - param->stepSize;
          param->xMin = arr[0] + param->stepSize;
          param->nStep = tmp_gr->GetN()-1;

          event->contXSRTGraphs[currentContXSRTGraph] = tmp_gr;
          currentContXSRTGraph++;
        }
      }
    }
  } //End second loop over the fit parameters.

    //At this point, all the TGraphs and splines required for this event should
    //be stored.

}

void FitParameters::AddXsecNormParameter(std::string nameBase, double value, TVectorD* reacCodes, int nTgtMat, int* tgtMatArray, TAxis *norm_bins, int bin, bool vary){

  if(norm_bins==NULL) return;

  std::string mode("OTHER");

  //2013 file format.
  //TODO: Fix the 2013 file format to go with the new reaction code encoding
  //then fix this.
  if(nameBase == ""){
    //        if(reactioncode == 0) mode = "CCQE";
    //        else if(reactioncode == 1) mode = "CCRES";
    //        else if(reactioncode == 2) mode = "CCCOH";
    //        else if(reactioncode == 6) mode = "NCOTHER";
    //        else if(reactioncode == 5) mode = "NC1PI0";

    mode = "FIXMODENAME";
  }

  //2014 format and beyond.
  else{
    mode = nameBase;
  }

  XSecNormParameter* tmp = new XSecNormParameter();
  tmp->priorValue = value;
  tmp->currentValue = value;
  tmp->currentValueUD = value;
  tmp->paramType = XSECNORM;
  tmp->emax = norm_bins->GetBinUpEdge(bin+1);
  tmp->emin = norm_bins->GetBinLowEdge(bin+1);
  tmp->lowBound = 0.;
  tmp->upBound = 999.;
  std::cout << "===============================================" << std::endl;
  std::cout << mode.c_str() << " " << tmp->emin << " " << tmp->emax << std::endl;

  tmp->nReactionCodes = reacCodes->GetNrows();
  tmp->reactionCodes = new int[tmp->nReactionCodes];
  std::cout << "XSecNormParameter " << mode.c_str() << " applies for the following reaction codes: " << std::endl;
  for(int j = 0; j < tmp->nReactionCodes; j++){

    //If it's positive, add 0.01 to ensure that binary vs. decimal precison
    //issues don't cause the cast to go to the incorrect int value.
    if((*reacCodes)(j) > 0.0){
      tmp->reactionCodes[j] = (int)((*reacCodes)(j) + 0.01);
    }

    //Otherwise, it's a negative reaction code, so we need to subtract 0.01
    //instead (for the same reason).
    else{
      tmp->reactionCodes[j] = (int)((*reacCodes)(j) - 0.01);
    }

    //Print out the reaction code that we just added so that we can cross
    //check on loading.
    std::cout << tmp->reactionCodes[j] << std::endl;
  }

  std::cout << "On target materials:" << std::endl;
  for(int tmi = 0; tmi < nTgtMat; tmi++){
    std::cout << tgtMatArray[tmi] << std::endl;
  }
  std::cout << "===============================================" << std::endl;
  tmp->nTargetMaterials = nTgtMat; 
  tmp->targetMaterial = tgtMatArray;
  tmp->xsecID = NORM;
  tmp->detector = -1;
  //tmp.spline = false;
  tmp->index = fitParameters.size();
  tmp->typeIndex = xsecNormIter;
  tmp->vary = vary;
  tmp->throwParm = vary;
  tmp->priorType = PDFGaus;
  tmp->nNupdg = 0;

  //The 2013 way.  Now the full name is stored.
  //std::string name(Form("%s_%d",mode.c_str(),bin));
  //tmp->paramName = name;
  tmp->paramName = nameBase;
  std::string sname(Form("xn%d",xsecNormIter));
  tmp->shortName = sname;
  tmp->decompose = 0;
  tmp->discrete = false;
  xsecNormIter++;
  fitParameters.push_back(tmp);
}

void FitParameters::AddXsecNormParameter2016(std::string nameBase, double value, TVectorD* reacCodes, TVectorD* nuPDG, int nTgtMat, int* tgtMatArray, TAxis *norm_bins, int bin, bool vary, bool constrain){

  if(norm_bins==NULL) return;

  std::string mode("OTHER");

  mode = nameBase;

  XSecNormParameter* tmp = new XSecNormParameter();
  tmp->priorValue = value;
  tmp->currentValue = value;
  tmp->currentValueUD = value;
  tmp->paramType = XSECNORM;
  tmp->emax = norm_bins->GetBinUpEdge(bin+1);
  tmp->emin = norm_bins->GetBinLowEdge(bin+1);
  tmp->lowBound = 0.;
  tmp->upBound = 9999.;
  tmp->hasScanBounds = true;
  tmp->lowBoundScan = 0.;
  tmp->upBoundScan = 2.;
  std::cout << "===============================================" << std::endl;
  std::cout << mode.c_str() << " " << tmp->emin << " " << tmp->emax << std::endl;

  if(reacCodes == NULL){
    tmp->nReactionCodes = 0;
    tmp->reactionCodes = new int[1];
    tmp->reactionCodes[0] = -999;
    std::cout << "XSecNormParameter " << mode.c_str() << " has a NULL reacCodes. " << std::endl;
  }
  else{ 
    tmp->nReactionCodes = reacCodes->GetNrows();
    tmp->reactionCodes = new int[tmp->nReactionCodes];
    std::cout << "XSecNormParameter " << mode.c_str() << " applies for the following reaction codes: " << std::endl;
    for(int j = 0; j < tmp->nReactionCodes; j++){

      //If it's positive, add 0.01 to ensure that binary vs. decimal precison
      //issues don't cause the cast to go to the incorrect int value.
      if((*reacCodes)(j) > 0.0){
        tmp->reactionCodes[j] = (int)((*reacCodes)(j) + 0.01);
      }

      //Otherwise, it's a negative reaction code, so we need to subtract 0.01
      //instead (for the same reason).
      else{
        tmp->reactionCodes[j] = (int)((*reacCodes)(j) - 0.01);
      }

      //Print out the reaction code that we just added so that we can cross
      //check on loading.
      std::cout << tmp->reactionCodes[j] << std::endl;
    }
  }
  if(nuPDG == NULL){
    tmp->nNupdg = 0;
    tmp->Nupdg = new int[1];
    tmp->Nupdg[0] = -999;
  }
  else{
    tmp->nNupdg = nuPDG->GetNrows();
    tmp->Nupdg = new int[tmp->nNupdg];
    std::cout << "With neutrino PDG:" << std::endl;
    for(int j = 0; j < tmp->nNupdg; j++){

      //If it's positive, add 0.01 to ensure that binary vs. decimal precison
      //issues don't cause the cast to go to the incorrect int value.
      if((*nuPDG)(j) > 0.0){
        tmp->Nupdg[j] = (int)((*nuPDG)(j) + 0.01);
      }

      //Otherwise, it's a negative nuPDG, so we need to subtract 0.01
      //instead (for the same reason).
      else{
        tmp->Nupdg[j] = (int)((*nuPDG)(j) - 0.01);
      }

      //Print out nuPDG that we just added so that we can cross
      //check on loading.
      std::cout << tmp->Nupdg[j] << std::endl;
    }
  }
  std::cout << "On target materials:" << std::endl;
  for(int tmi = 0; tmi < nTgtMat; tmi++){
    std::cout << tgtMatArray[tmi] << std::endl;
  }
  std::cout << "===============================================" << std::endl;
  tmp->nTargetMaterials = nTgtMat; 
  tmp->targetMaterial = tgtMatArray;
  tmp->xsecID = NORM;
  tmp->detector = -1;
  //tmp.spline = false;
  tmp->index = fitParameters.size();
  tmp->typeIndex = xsecNormIter;
  tmp->vary = vary;
  tmp->throwParm = vary;
  if(constrain){
    tmp->priorType = PDFGaus;
  }
  else{
    tmp->priorType = PDFFlat;
  }

  tmp->paramName = nameBase;
  std::string sname(Form("xn%d",xsecNormIter));
  tmp->shortName = sname;
  tmp->decompose = 0;
  tmp->discrete = false;
  xsecNormIter++;
  fitParameters.push_back(tmp);
}

void FitParameters::LoadObsNormParametersFromFile(std::string fileName, int nSamples, BANFFSampleBase** samples, bool vary){



  obsNormFile = new TFile(fileName.c_str());


  //The nominal values for the observable normalization parameters are
  //stored in a TVectorD for all of them that make up the covariance matrix.
  //Extract that now.
  obsNorm_weights = (TVectorD*)obsNormFile->Get("obsNorm_weights");

  //Load the observable normalization parameter covariance matrix from the
  //file.
  obsNorm_cov = (TMatrixDSym*)obsNormFile->Get("obsNorm_cov");

  //Set up a counter for iterating over the ObsNorm parameters in this file
  //(it is conceivable that some might be added manually or loaded from a
  //different file, so cannot count on nObsNormParms to iterate over this
  //file's contents.
  int obsNormIter = 0;

  //Loop over the samples to pull out all the observable axes we need from
  //the files, then create the parameters that want from that information.
  //TODO: This needs to be very well documented...
  for(int i = 0; i < nSamples; i++){

    //The observable axes of this sample specifically.
    std::vector<TAxis*> sampleAxes;

    //The bin number being considered (NB: Bin numbering starts at 1)
    //for each observable.
    std::vector<int> obsBin;

    //Also store the total number of bins for the observable.
    std::vector<int> obsNBins;

    //Now loop over each observable for this sample, getting its axis and
    //adding it to the vector of axes.
    for(int j = 0; j < samples[i]->nObs; j++){

      std::string axisName = samples[i]->name + "_"+ samples[i]->obs[j]->name + "_axis";
      obs_axes.push_back(((TAxis*)obsNormFile->Get(axisName.c_str())));
      sampleAxes.push_back(obs_axes.back());
      obsBin.push_back(1);
      obsNBins.push_back(sampleAxes.back()->GetNbins());

    }


    //Now, loop backwards from the last bin.  If a bin value is equal to
    //the total number of bins for that observable, reset it to 1, and
    //continue to the next observable.  Continue until find an observable
    //whose bin can be incremented.  Once the first observable has no bins
    //left, and has been paired with all bins from all the other observables, 
    //all parameters for this sample have been added.

    //Do not try and add any more parameters after we have reached the last
    //bin of the first observable.
    bool firstParam = true;
    while(obsBin[0] <= obsNBins[0]){


      //Have a vector of minimum and maximum values for the observables.
      //(A new vector for each set of parameters).
      std::vector<double> obsMin;
      std::vector<double> obsMax;

      //Loop backwards through all the observables.
      for(int k = samples[i]->nObs - 1; k >= 0; k--){

        //If it's the very first parameter we're adding, don't change
        //anything yet, just break out and skip straight to adding it.
        if(firstParam){

          firstParam = false;
          break;
        }


        //If the bin we were previously considering for this observable
        //was the last one, reset it to the first bin, then consider
        //the next observable.
        if(obsBin[k] == obsNBins[k]){

          obsBin[k] = 1;
        }

        //Otherwise, consider the next bin for this observable, and
        //break.
        else{

          obsBin[k]++;
          break;

        }

      }

      //With all the bins selected, setup the obsMin and obsMax vectors,
      //then create an observable normalization parameter from this
      //combination of observable bins.
      //Loop over the samples in the correct order to set up ObsMin and
      //ObsMax vectors.
      for(int k = 0; k < samples[i]->nObs; k++){

        obsMin.push_back(sampleAxes[k]->GetBinLowEdge(obsBin[k]));
        obsMax.push_back(sampleAxes[k]->GetBinUpEdge(obsBin[k]));

      }

      //With the bin edges now set, add the parameter.
      AddObsNormParameter((*obsNorm_weights)(obsNormIter), obsMin, obsMax, 0, samples[i]->sampleID, vary);

      //Increment the counter of observable normalization parameters, and
      //the counter for this specific file.
      nObsNormParms++;
      obsNormIter++;

      //If the observable bin is the last observable bin to consider,
      //break out of this loop.
      bool lastBin = true;
      for(int lbi = 0; lbi < samples[i]->nObs; lbi++){

        lastBin = lastBin && (obsBin[lbi] == obsNBins[lbi]);

      }

      if(lastBin){

        break;

      }

    } //End while loop ensuring don't try to add more ObsNorm parameters than there are.

  } //End loop over samples.

    //With all of the parameters added, append the covariance matrix.
  AppendCovariance(obsNorm_cov);
}


void FitParameters::AddObsNormParameter(double value, std::vector<double> obsMin, std::vector<double> obsMax, int det, int sample, bool vary ){

  ObsNormParameter* tmp = new ObsNormParameter();
  tmp->priorValue = value;
  tmp->currentValue = value;
  tmp->currentValueUD = value;
  tmp->paramType = OBSNORM;
  tmp->emax = 0.;
  tmp->emin = 0.;
  tmp->hasScanBounds = true;
  tmp->lowBoundScan = 0.0;
  tmp->upBoundScan = 100.0;

  //lower bound set to -30.0 for allowing decomposition.
  tmp->lowBound = -30.0;
  tmp->upBound = 30.0;
  //tmp.physicsCode = -999.;
  //tmp.xsecID = NORM;
  tmp->detector = det;
  //tmp.spline = false;
  tmp->index = fitParameters.size();
  tmp->typeIndex = obsNormIter;
  tmp->vary = vary;
  tmp->throwParm = vary;
  tmp->priorType = PDFGaus;
  std::string name(Form("OBSNORM_%d",obsNormIter));
  tmp->paramName = name;

  std::string sname(Form("o%d",obsNormIter));
  tmp->shortName = sname;

  //TODO: Most general format would allow a choice on decomposition.
  tmp->decompose = 0;
  tmp->obsMin = obsMin;
  tmp->obsMax = obsMax;
  tmp->sample = sample;
  tmp->discrete = false;
  obsNormIter++;
  fitParameters.push_back(tmp);


}

void FitParameters::AddDetParameter(double value, int det, PDFcode prior, double lBound, double uBound, std::string &name, bool vary){

  DetParameter* tmp = new DetParameter();
  tmp->priorValue = value;
  tmp->currentValue = value;
  tmp->currentValueUD = value;
  tmp->paramType = DET;
  tmp->emax = 0.;
  tmp->emin = 0.;
  tmp->lowBound = lBound;
  tmp->upBound = uBound;
  //tmp.xsecID = NORM;
  tmp->detector = det;
  //tmp.spline = false;
  tmp->index = fitParameters.size();
  tmp->typeIndex = detIter;
  tmp->vary = vary;
  tmp->throwParm = vary;
  tmp->priorType = prior;
  tmp->paramName = name;
  tmp->discrete  = false;
  tmp->decompose = 0;
  detIter++;
  fitParameters.push_back(tmp);
}


void FitParameters::AppendCovariance(TMatrixDSym* cov){

  std::cout << covariance->GetNrows() << " " << cov->GetNrows() << std::endl;

  if(covarianceI!=NULL){
    std::cout << covarianceI->GetNrows() << std::endl;
  }

  //Get the number of rows from the current covariance matrix and the covariance 
  //matrix to add, and declare a new covariance matrix with a number of rows equal to their sum.
  int numbRows = covariance->GetNrows()+cov->GetNrows();
  TMatrixDSym *newCov = new TMatrixDSym(numbRows);

  //Set the entries in the new covariance matrix to zero.
  for(int i=0; i<numbRows; i++){
    for(int j=0; j<numbRows; j++){
      (*newCov)(i,j) = 0.;
    }
  }

  //Add in the old covariance so that entry (i,j) in it is
  //ewntry (i,j) in the new covariance matrix.
  for(int i=0; i<covariance->GetNrows(); i++){
    for(int j=0; j<covariance->GetNrows(); j++){
      (*newCov)(i,j) = (*covariance)(i,j);
    }
  }

  //Add the new covariance matrix after the old covariance matrix. 
  for(int i=0; i<cov->GetNrows(); i++){
    for(int j=0; j<cov->GetNrows(); j++){
      (*newCov)(i+covariance->GetNrows(),j+covariance->GetNrows()) = (*cov)(i,j);
    }
  }


  //Delete the old covariances if needed, and replace them using the new
  //covariance accordingly.
  if(covariance!=NULL){

    covariance->Delete();
  }

  covariance = new TMatrixDSym(*newCov);

  if(covarianceI!=NULL){
    covarianceI->Delete();
  }

  covarianceI = new TMatrixDSym(*newCov);
  covarianceI->Invert();

  if(covarianceFit!=NULL){
    covarianceFit->Delete();
  }

  covarianceFit = new TMatrixDSym(*newCov);

  newCov->Delete();
  std::cout << "Number of parameters " << covariance->GetNrows() << std::endl;

}

void FitParameters::LoadFittedParameters(std::string fileName, int fitNumber){

  TFile* fitFile = new TFile(fileName.c_str());

  TVectorD* fittedParms = (TVectorD*)(fitFile->Get(Form("FittedParameters%d",fitNumber)));

  TMatrixDSym* fittedCov = (TMatrixDSym*)(fitFile->Get(Form("FittedCovariance%d",fitNumber)));

  //Loop through the parameters and replace their prior value with the value
  //from the fit.  Must also set the current value and current value
  //undecomposed, just as if were adding the parameter again. 
  for(int i = 0; i < (int)fitParameters.size(); i++){

    fitParameters[i]->priorValue = (*fittedParms)(i);
    fitParameters[i]->currentValue = (*fittedParms)(i);
    fitParameters[i]->currentValueUD = (*fittedParms)(i);

  }

  //With the prior values now changed, substitute the fitted covariance in
  //for the existing one.
  SubstituteCovariance(fittedCov);

}

void FitParameters::SubstituteCovariance(TMatrixDSym* newCov){

  //Replace the covariance.
  if(covariance != NULL){

    covariance->Delete();
  }

  covariance = new TMatrixDSym(*newCov);

  //Replace the inverted covariance.
  if(covarianceI != NULL){

    covarianceI->Delete();
  }

  covarianceI = new TMatrixDSym(*newCov);
  covarianceI->Invert();

  //Replace any stored fitted covariance.
  if(covarianceFit != NULL){

    covarianceFit->Delete();
  }

  covarianceFit = new TMatrixDSym(*newCov);

}

std::vector<double> FitParameters::GetDetectorVector(){

  std::vector<double> detValues;
  for(unsigned int i=0; i<fitParameters.size(); i++){

    if(fitParameters[i]->paramType == DET){
      detValues.push_back(fitParameters[i]->currentValue);
    }

  }

  return detValues;

}

void FitParameters::MatchParameters(BANFFEventBase* event){

  for(unsigned int i = 0; i < fitParameters.size(); i++){

    fitParameters[i]->MatchToEvent(event);
  }
}

void FitParameters::InitializeThrows(int seed){

  if(throwParms!=NULL){
    delete throwParms; 
  }

  //std::cout << "Number of parameters " << covariance->GetNrows() << std::endl;
  TVectorD priorVec(covariance->GetNrows());
  for(int i=0; i<covariance->GetNrows(); i++){
    priorVec(i) = fitParameters[i]->priorValue;
  }

  throwParms = new ThrowParms(priorVec,(*covariance));
  throwParms->SetSeed(seed);

  random.SetSeed(seed*1987);

}

void FitParameters::DoThrow(bool saveChi2){

  //Get a full set of thrown parameters from ThrowParms.
  std::vector<double> thrownValues;
  throwParms->ThrowSet(thrownValues);

  for(unsigned int i=0; i<thrownValues.size(); i++){

    //If the parameter is being thrown, change the current value of the
    //parameter accordingly.
    if(fitParameters[i]->throwParm){

      //If it's discrete, continue, as we will deal with this elsewhere.
      if(fitParameters[i]->discrete){
        continue;
      }

      //Set the current value of this fit parameter to the thrown value
      //from ThrowParms.
      fitParameters[i]->currentValue = thrownValues[i];

      //If this parameter's has a flat prior, instead of using the value
      //from ThrowParms, replace it with a thrown value between the lower
      //and upper bounds of the parameter.
      if(fitParameters[i]->priorType==PDFFlat){ 
        fitParameters[i]->currentValue = fitParameters[i]->lowBound +random.Rndm()*(fitParameters[i]->upBound-fitParameters[i]->lowBound);
      }

      //If the current value is below the lower bound, set it to the
      //lower bound.
      if(fitParameters[i]->currentValue < fitParameters[i]->lowBound){
        fitParameters[i]->currentValue = fitParameters[i]->lowBound;
      }

      //If the current value is above the upper bound, set it to the
      //upper bound.
      if(fitParameters[i]->currentValue > fitParameters[i]->upBound){
        fitParameters[i]->currentValue = fitParameters[i]->upBound;
      }

      //std::cout << "Fit Parameter error " << i << ": " << fitParameters[i].currentValue << std::endl;
    }

    //If the parameter is not to be thrown, set its value to the prior
    //value.
    else{
      fitParameters[i]->currentValue = fitParameters[i]->priorValue;
      //std::cout << "Fit Parameter " << i << ": " << fitParameters[i].currentValue << std::endl;
    }
  }

  //Need to handle discrete parameters separately.
  //The code currently either supports throwing all of them or throwing none
  //of them.  Need to check for the right conditions first.
  int nDiscParamsThrown = 0;
  for(unsigned int i = 0; i < fitParameters.size(); i++){

    if(fitParameters[i]->discrete && fitParameters[i]->throwParm){
      nDiscParamsThrown++;
    }

  }

  //If it's a non-zero number of discrete parameters thrown, but not all of
  //them, print an error message.
  if((nDiscParamsThrown != 0) && (nDiscParamsThrown != nDiscreteParameters)){

    std::cout << "ERROR: Unsupported configuration of discrete parameters.  Nothing no discrete parameters have been thrown." << std::endl;
  }

  //Otherwise, the prior value is unchanged if they are not thrown, but if a
  //non-zero number is thrown, choose one of the acceptable situations from a
  //random throw and set those values.
  else if(nDiscParamsThrown != 0){

    //Make a random throw from 0 to nDiscreteParameterCombinations, cast to
    //int, then use that discrete parameter combination set of values.
    //RnDm produces uniformly distributed floating points in (0,1), i.e.
    //0 < return value < 1.
    int comboIndex = (int)(random.Rndm()*nDiscreteParameterCombinations);

    for(unsigned int i = 0; i < fitParameters.size(); i++){

      if(fitParameters[i]->discrete){

        fitParameters[i]->currentValue = discreteParameterCombinations[comboIndex][i];
      }
    }
  }


  if(saveChi2) thrownChi2.push_back(CalcChi2());
  //PrintCurrentValues();
}

void FitParameters::DoThrowRTOOB(bool saveChi2){

  //Get a full set of thrown parameters from ThrowParms.
  std::vector<double> thrownValues;


  bool gausParamsInBounds = false;

  //Keep throwing until we get a set of parameters where all the Gaussian
  //parameters are in the correct bounds.
  while(!gausParamsInBounds){

    throwParms->ThrowSet(thrownValues);

    bool tmpInBounds = true;
    for(unsigned int i = 0; i < thrownValues.size(); i++){
      //Only care about parameters we are throwing that are not discrete
      //and are not flat.
      if((fitParameters[i]->discrete)
         || (!(fitParameters[i]->throwParm))
         || (fitParameters[i]->priorType==PDFFlat)){

        continue;

      }

      //Otherwise we're dealing with a parameter whose bounds we care
      //about.
      tmpInBounds = tmpInBounds && thrownValues[i] >= fitParameters[i]->lowBound;
      tmpInBounds = tmpInBounds && thrownValues[i] <= fitParameters[i]->upBound;

      //To save us some cycles if we're out of bounds, break if
      //tmpInBounds is now false, to induce a new throw.
      if(!tmpInBounds){
        //std::cout<<thrownValues[i]<< " "<<fitParameters[i]->paramName<<std::endl;
        break;

      }
    }

    //Set the value of gausParamsInBounds to equal that of tmpInBounds.
    //If all parameters were in bounds, the loop will exit and follow a
    //copy of the original DoThrows() code.  If not, the loop will continue
    //with a new throw until we have a good set of bounds.
    gausParamsInBounds = tmpInBounds;

  }

  for(unsigned int i=0; i<thrownValues.size(); i++){

    //If the parameter is being thrown, change the current value of the
    //parameter accordingly.
    if(fitParameters[i]->throwParm && fitParameters[i]->priorType!=PDFFlat){

      //If it's discrete, continue, as we will deal with this elsewhere.
      if(fitParameters[i]->discrete){
        continue;
      }

      //Set the current value of this fit parameter to the thrown value
      //from ThrowParms.
      fitParameters[i]->currentValue = thrownValues[i];

      //If the current value is below the lower bound, set it to the
      //lower bound.
      if(fitParameters[i]->currentValue < fitParameters[i]->lowBound){
        fitParameters[i]->currentValue = fitParameters[i]->lowBound;
      }

      //If the current value is above the upper bound, set it to the
      //upper bound.
      if(fitParameters[i]->currentValue > fitParameters[i]->upBound){
        fitParameters[i]->currentValue = fitParameters[i]->upBound;
      }

      //std::cout << "Fit Parameter error " << i << ": " << fitParameters[i].currentValue << std::endl;
    }

    //If the parameter is not to be thrown, set its value to the prior
    //value.
    else{
      fitParameters[i]->currentValue = fitParameters[i]->priorValue;
      //std::cout << "Fit Parameter " << i << ": " << fitParameters[i].currentValue << std::endl;
    }
  }

  //Need to handle discrete parameters separately.
  //The code currently either supports throwing all of them or throwing none
  //of them.  Need to check for the right conditions first.
  int nDiscParamsThrown = 0;
  for(unsigned int i = 0; i < fitParameters.size(); i++){

    if(fitParameters[i]->discrete && fitParameters[i]->throwParm){
      nDiscParamsThrown++;
    }

  }

  //If it's a non-zero number of discrete parameters thrown, but not all of
  //them, print an error message.
  if((nDiscParamsThrown != 0) && (nDiscParamsThrown != nDiscreteParameters)){

    std::cout << "ERROR: Unsupported configuration of discrete parameters.  Nothing no discrete parameters have been thrown." << std::endl;
  }

  //Otherwise, the prior value is unchanged if they are not thrown, but if a
  //non-zero number is thrown, choose one of the acceptable situations from a
  //random throw and set those values.
  else if(nDiscParamsThrown != 0){

    //Make a random throw from 0 to nDiscreteParameterCombinations, cast to
    //int, then use that discrete parameter combination set of values.
    //RnDm produces uniformly distributed floating points in (0,1), i.e.
    //0 < return value < 1.
    int comboIndex = (int)(random.Rndm()*nDiscreteParameterCombinations);

    for(unsigned int i = 0; i < fitParameters.size(); i++){

      if(fitParameters[i]->discrete){

        fitParameters[i]->currentValue = discreteParameterCombinations[comboIndex][i];
      }
    }
  }


  if(saveChi2) thrownChi2.push_back(CalcChi2());
  //PrintCurrentValues();
}



double FitParameters::GetParameterError(int i, bool fitted){

  if( !fitted && (i < covariance->GetNrows())){
    return sqrt( (*covariance)(i,i) );
  }

  else if( fitted && eigenDecompositionRequired){

    if(i < covarianceUD->GetNrows()){
      return sqrt( (*covarianceFit)(i,i) );
    }
    else{
      return 0.0;
    }
  }
  else if(fitted && !eigenDecompositionRequired){

    if(i < covariance->GetNrows()){
      return sqrt( (*covarianceFit)(i,i) );
    }
    else{
      return 0.0;
    }
  }

  else{
    return 0.;
  }

}


double FitParameters::GetScanStepSize(int i){

  //TODO: This method of assigning special step sizes to the first 4 flux
  //parameters should probably be more obvious to the user than buried away
  //here.
  double stepsize[4] = {4.0e-3, 4.0e-3, 2.5e-3, 5.0e-3};
  if(fitParameters[i]->paramType == FLUX){


    if(i < 4){


      return stepsize[i];


    }

    else{

      return GetParameterError(i)/20.0;

    }

  }

  else{

    return GetParameterError(i)/50.0;

  }


}

void FitParameters::PrintCurrentValues(){

  for(unsigned int i=0; i<fitParameters.size(); i++){

    std::cout << fitParameters[i]->paramName << ", type=" << fitParameters[i]->paramType << ", current value=" <<
      fitParameters[i]->currentValue << ", pdf=" << fitParameters[i]->priorType 
              << ", do vary=" << fitParameters[i]->vary << std::endl;
  }

}

void FitParameters::PrintPriorValues(){

  for(unsigned int i=0; i<fitParameters.size(); i++){

    std::cout << fitParameters[i]->paramName << ", type=" << fitParameters[i]->paramType << ", prior value=" <<
      fitParameters[i]->priorValue << ", pdf=" << fitParameters[i]->priorType 
              << ", do vary=" << fitParameters[i]->vary << std::endl;
  }

}

void FitParameters::ResetValues(){

  for(unsigned int i=0; i<fitParameters.size(); i++)
    fitParameters[i]->currentValue = fitParameters[i]->priorValue;

  if(covarianceFit!=NULL) covarianceFit->Delete();
  covarianceFit = new TMatrixDSym(*covariance);

  fitIter = 0;
}

double FitParameters::GetCurrentValue(int i, bool UD){



  if(!UD || eigenDecomp==NULL || fitParameters[i]->decompose==0 ){
    //std::cout << (bool) (&fitParameters[i])->currentValue << std::endl;
    //std::cout << "i = " << i << std::endl;
    //std::cout << "About to access value" << std::endl;
    //std::cout << fitParameters[i].currentValue << std::endl;
    //std::cout << "We get here!" << std::endl;

    return fitParameters[i]->currentValue; 
  }else{ 
    return fitParameters[i]->currentValueUD;
  }
}

double FitParameters::GetPriorValue(int i, bool UD){

  if(!UD || eigenDecomp==NULL || fitParameters[i]->decompose==0 )
    return fitParameters[i]->priorValue; 
  else 
    return fitParameters[i]->priorValueUD;
}


void FitParameters::WriteResults(int iteration, bool names, bool UD ){

  //A TVectorD to store the current values of the fit parameters.
  TVectorD pars(fitParameters.size());

  //A TObjArray to store the parameter names to write out.
  TObjArray* parameterNames = new TObjArray(fitParameters.size(),0);

  //Loop over the fit parameters, storing either the current value or the
  //current value undecomposed, depending on 
  for(unsigned int i=0; i<fitParameters.size(); i++){

    //If this is a parameter that does not undergo decomposition, or we do
    //not desire the undecomposed value to be saved, save the current value
    //of the parameter.
    if(fitParameters[i]->decompose==0 || !UD){
      pars(i) = fitParameters[i]->currentValue;
    }

    //Otherwise, if it is a parameter that is decomposed, save the
    //undecomposed value.
    else if(fitParameters[i]->decompose>=1){
      pars(i) = fitParameters[i]->currentValueUD;
    }

    //The value of the string being saved is the parameter name.
    TObjString name(fitParameters[i]->paramName.c_str());

    //The "names" variable specifies whether to save the parameter names.
    //These are TObjString objects, whose name creates a correspondance
    //between the stored parameter name and the parameter number i.  They
    //only need to be saved once, so WriteResults() only saves the
    //parameter values if specifically requested to by this bool.
    if(names){
      name.Write(Form("ParamName%d",i));
      parameterNames->AddAt(new TObjString((fitParameters[i]->paramName).c_str()),i);
    }
  }  

  //Having looped over all the parameters, save the parameterNames array, if
  //we're supposed to save names.
  if(names){
    parameterNames->Write("parameterNames",TObject::kSingleKey);
  }


  //Save the value of the parameters for this specific fit.  (When one fit is
  //being performed, the iteration is 0.  If multiple fits are being
  //performed, iteration will take different values.)
  pars.Write(Form("FittedParameters%d",iteration));

  //Similarly, save the fitted covariance.
  covarianceFit->Write(Form("FittedCovariance%d",iteration));

}

void FitParameters::ComputeEigenDecompositionRequired(){

  //Loop through the fit parameters.  eigenDecompositionRequired is set to
  //false.  If at least one parameter requires eigendecomposition,
  //eigenDecompositionRequired will be set to true.
  for(unsigned int i = 0; i < fitParameters.size(); i++){

    eigenDecompositionRequired = eigenDecompositionRequired || (fitParameters[i]->decompose >= 1);

  }

}

void FitParameters::FillFastVariables(){

  numbSystsFast = (int)fitParameters.size();

  fitParametersFast = new FitParameter*[numbSystsFast];
  for(unsigned int i = 0; i < fitParameters.size(); i++){
    fitParametersFast[i] = fitParameters[i];
    std::cout<<i<<" "<<fitParametersFast[i]->paramName<<std::endl;
  }

}

void FitParameters::PrintAllParameterInfo(){

  int detiter  = 0;
  int obsnormiter  = 0;
  int xsecrespiter = 0;
  int xsecnormiter = 0;
  int xsecfunciter = 0;
  int fluxiter = 0;
  int notvaried =0;
  for(unsigned int i = 0; i < fitParameters.size(); i++){

    std::cout << "====================================================" << std::endl;
    std::cout << "Name: " << fitParameters[i]->paramName << std::endl;
    std::cout << "Short Name: " << fitParameters[i]->shortName << std::endl; 
    std::string paramTypeString;
    if(fitParameters[i]->paramType == FLUX){
      paramTypeString = "FLUX";
      fluxiter++;
    }

    else if(fitParameters[i]->paramType == XSECRESP){
      paramTypeString = "XSECRESP";
      xsecrespiter++;
    }
		
    else if(fitParameters[i]->paramType == XSECFUNC){
      paramTypeString = "XSECFUNC";
      xsecfunciter++;
    }

    else if(fitParameters[i]->paramType == XSECNORM){
      paramTypeString = "XSECNORM";
      xsecnormiter++;
    }

    else if(fitParameters[i]->paramType == DET){
      paramTypeString = "DET";
      detiter++;
    }

    else if (fitParameters[i]->paramType == OBSNORM){
      paramTypeString = "OBSNORM";
      obsnormiter++;
    }

    std::cout << "Param Type: " << paramTypeString << std::endl;


    std::cout << "Current Value: " << fitParameters[i]->currentValue << std::endl;
    std::cout << "Current Error: " << GetParameterError(i) << std::endl;
    std::cout << "Lower Bound: " << fitParameters[i]->lowBound << std::endl;
    std::cout << "Upper Bound: " << fitParameters[i]->upBound << std::endl;

    std::string pdfTypeString = "";
    if(fitParameters[i]->priorType == PDFGaus){
      pdfTypeString = "Gaussian";
    }
    else if(fitParameters[i]->priorType == PDFFlat){

      pdfTypeString = "Flat";
    }
    else if(fitParameters[i]->priorType == PDFBino){

      pdfTypeString = "Binomial";

    }

    std::cout << "PDF Type: " << pdfTypeString << std::endl;
    std::cout << "Vary: " << fitParameters[i]->vary << std::endl;
    std::cout << "Decompose: " << fitParameters[i]->decompose << std::endl;
    std::cout << "Discrete: " << fitParameters[i]->discrete << std::endl;

    if(fitParameters[i]->vary == 0){
      notvaried++;
    }
    //Print each individual parameter type's extra information.
    fitParameters[i]->PrintParameterRequirements();

  }
  std::cout << "====================================================" << std::endl;
  std::cout << "Total Number of Parameters: " << fitParameters.size()<<std::endl;
  std::cout << "Number of Varying Parameters : " << fitParameters.size() - notvaried<<std::endl;
  std::cout << "Number of Flux Parameters: " << fluxiter<<std::endl;
  std::cout << "Number of XSec Norm Parameters: " << xsecnormiter<<std::endl;
  std::cout << "Number of XSec Resp Parameters: " << xsecrespiter<<std::endl;
  std::cout << "Number of XSec Func Parameters: " << xsecfunciter<<std::endl;
  std::cout << "Number of Detector Parameters: " << detiter<<std::endl;
  std::cout << "Number of Obs Norm Parameters: " << obsnormiter<<std::endl;


}

int FitParameters::GetParameterIndexFromName(std::string thisParamName){

  for(unsigned int i = 0; i < fitParameters.size(); i++){

    if(thisParamName == fitParameters[i]->paramName){

      return (int)i;

    }
  }

  return -1;

}

void FitParameters::EnableParameterByName(std::string paramToEnable){

  int paramIndex = GetParameterIndexFromName(paramToEnable);

  if(paramIndex < 0){

    std::cout << "ERROR: Trying to enable non-existent parameter " << paramToEnable << std::endl;

    std::cout << "Doing nothing instead." << std::endl;
  }

  else{
    EnableParameter(paramIndex);
  }
}

void FitParameters::DisableParameterByName(std::string paramToDisable){

  int paramIndex = GetParameterIndexFromName(paramToDisable);

  if(paramIndex < 0){

    std::cout << "ERROR: Trying to disable non-existent parameter " << paramToDisable << std::endl;

    std::cout << "Doing nothing instead." << std::endl;
  }

  else{
    DisableParameter(paramIndex);
  }
}

//Convert MACH3 mode to neut code
//MACH3 is a vector of mode from MACH3 and isneut is true for neutrino, false for anti-neutrino
TVectorD* FitParameters::MACH3toNEUTReacCode(TVectorD* MACH3, int isneut){
  if((*MACH3)(0)==0){//CCQE
    if(isneut == 1 || isneut == -1)(*MACH3)(0) = isneut*1;
    else if(isneut == 0){
      MACH3->ResizeTo(2);(*MACH3)(0) = -1;(*MACH3)(1) = 1;
    }
    return MACH3;
  }
  else if((*MACH3)(0)==1){//CC1pi
    if(isneut == 1 || isneut == -1){
      MACH3->ResizeTo(3); (*MACH3)(0) = isneut*11;(*MACH3)(1) = isneut*12;(*MACH3)(2) = isneut*13;
    }
    else if(isneut == 0){
      MACH3->ResizeTo(6); (*MACH3)(0) = -11;(*MACH3)(1) = -12;(*MACH3)(2) = -13;
      (*MACH3)(3) = 11;(*MACH3)(4) = 12;(*MACH3)(5) = 13;
    }
    return MACH3;
  }
  else if((*MACH3)(0)==2){//CCCoh
    if(isneut == 1 || isneut == -1){
      (*MACH3)(0) = isneut*16;
    }
    else if(isneut == 0){
      MACH3->ResizeTo(2); (*MACH3)(0) = -16;(*MACH3)(1) = 16;
    }
    return MACH3;
  }
  else if((*MACH3)(0)==3){//CCMultiPi
    if(isneut == 1 || isneut == -1){
      (*MACH3)(0) = isneut*21;
    }
    else if(isneut == 0){
      MACH3->ResizeTo(2); (*MACH3)(0) = -21;(*MACH3)(1) = 21;
    }
    return MACH3;
  }
  else if((*MACH3)(0)==4){//CCDis
    if(isneut == 1 || isneut == -1){
      MACH3->ResizeTo(4); (*MACH3)(0) = isneut*17;(*MACH3)(1) = isneut*22;
      (*MACH3)(2) = isneut*23;(*MACH3)(3) = isneut*26;
    }
    else if(isneut == 0){
      MACH3->ResizeTo(8); (*MACH3)(0) = -17;(*MACH3)(1) = -22;(*MACH3)(2) = -23;(*MACH3)(3) = -26;
      (*MACH3)(4) = 17;(*MACH3)(5) = 22;(*MACH3)(6) = 23;(*MACH3)(7) = 26;
    }
    return MACH3;
  }
  else if((*MACH3)(0)==5){//NC1pi0
    if(isneut == 1 || isneut == -1){
      MACH3->ResizeTo(2); (*MACH3)(0) = isneut*31;(*MACH3)(1) = isneut*32;
    }
    else if(isneut == 0){
      MACH3->ResizeTo(4); (*MACH3)(0) = -31;(*MACH3)(1) = -32;(*MACH3)(2) = 31;(*MACH3)(3) = 32;
    }
    return MACH3;
  }
  else if((*MACH3)(0)==6){// NC1pi+ or NC1pi-
    if(isneut == 1 || isneut == -1){
      MACH3->ResizeTo(2); (*MACH3)(0) = isneut*33;(*MACH3)(1) = isneut*34;
    }
    else if(isneut == 0){
      MACH3->ResizeTo(4); (*MACH3)(0) = -33;(*MACH3)(1) = -34;(*MACH3)(2) = 33;(*MACH3)(3) = 34;
    }
    return MACH3;
  }
  else if((*MACH3)(0)==7){// NC coh
    if(isneut == 1 || isneut == -1){
      (*MACH3)(0) = isneut*36;
    }
    else if(isneut == 0){
      MACH3->ResizeTo(2); (*MACH3)(0) = -36;(*MACH3)(1) = 36;
    }
    return MACH3;
  }
  else if((*MACH3)(0)==8){// NC other
    if(isneut == 1 || isneut == -1){
      MACH3->ResizeTo(8); (*MACH3)(0) = isneut*41;(*MACH3)(1) = isneut*42;(*MACH3)(2) = isneut*43;(*MACH3)(3) = isneut*44;
      (*MACH3)(4) = isneut*45;(*MACH3)(5) = isneut*46;(*MACH3)(6) = isneut*51;(*MACH3)(7) = isneut*52;
    }
    else if(isneut == 0){
      MACH3->ResizeTo(16); (*MACH3)(0) = -41;(*MACH3)(1) = -42;(*MACH3)(2) = -43;(*MACH3)(3) = -44;
      (*MACH3)(4) = -45;(*MACH3)(5) = -46;(*MACH3)(6) = -51;(*MACH3)(7) = -52;(*MACH3)(8) = 41;
      (*MACH3)(9) = 42;(*MACH3)(10) = 43;(*MACH3)(11) = 44;
      (*MACH3)(12) = 45;(*MACH3)(13) = 46;(*MACH3)(14) = 51;(*MACH3)(15) = 52;
    }
    return MACH3;
  }
  else if((*MACH3)(0)==9){//2p2h
    if(isneut == 1 || isneut == -1){
      (*MACH3)(0) = isneut*2;
    }
    else if(isneut == 0){
      MACH3->ResizeTo(2); (*MACH3)(0) = -2;(*MACH3)(1) = 2;
    }
    return MACH3;
  }
  else if((*MACH3)(0)==10){// NC 1gamma
    MACH3->ResizeTo(2); (*MACH3)(0) = 38; (*MACH3)(1) = 39;
    if(isneut == 1 || isneut == -1){
      MACH3->ResizeTo(2); (*MACH3)(0) = isneut*38;(*MACH3)(1) = isneut*39;
    }
    else if(isneut == 0){
      MACH3->ResizeTo(4); (*MACH3)(0) = -38;(*MACH3)(1) = -39;(*MACH3)(2) = 38;(*MACH3)(3) = 39;
    }
    return MACH3;
  }
  else if((*MACH3)(0)==99){//All CC interactions
    if(isneut == 1 || isneut == -1){
      //std::cout << "Setting reaction codes to all CC interactions" <<std::endl;
      MACH3->ResizeTo(11);
      (*MACH3)(0) = isneut*1; (*MACH3)(1) = isneut*2; //CCQE, 2p2h
      (*MACH3)(2) = isneut*11; (*MACH3)(3) = isneut*12; (*MACH3)(4) = isneut*13; //CC1pi
      (*MACH3)(5) = isneut*16; (*MACH3)(6) = isneut*21; //CCCoh, CCMultiPi
      (*MACH3)(7) = isneut*17; (*MACH3)(8) = isneut*22; (*MACH3)(9) = isneut*23; //CCDIS
      (*MACH3)(10) = isneut*26; //CCDIS
      //MACH3->Print();
    }
    else if(isneut == 0){
      std::cout << "Setting reaction codes to all CC interactions" <<std::endl;
      MACH3->ResizeTo(22);
      (*MACH3)(0) = -1; (*MACH3)(1) = -2; //CCQE, 2p2h
      (*MACH3)(2) = -11; (*MACH3)(3) = -12; (*MACH3)(4) = -13; //CC1pi
      (*MACH3)(5) = -16; (*MACH3)(6) = -21; //CCCoh, CCMultiPi
      (*MACH3)(7) = -17; (*MACH3)(8) = -22; (*MACH3)(9) = -23; //CCDIS
      (*MACH3)(10) = -26; //CCDIS
      (*MACH3)(11) = 1; (*MACH3)(12) = 2; //CCQE, 2p2h
      (*MACH3)(13) = 11; (*MACH3)(14) = 12; (*MACH3)(15) = 13; //CC1pi
      (*MACH3)(16) = 16; (*MACH3)(17) = 21; //CCCoh, CCMultiPi
      (*MACH3)(18) = 17; (*MACH3)(19) = 22; (*MACH3)(20) = 23; //CCDIS
      (*MACH3)(21) = 26; //CCDIS
    }
    return MACH3;
  }
  else if((*MACH3)(0)==-1){
    MACH3 = NULL;
    return MACH3;
  }
  return NULL;
}

int FitParameters::IsNeutrinoorAntiNeutrino(TVectorD* nuPDG){
  bool neutrino = false;
  bool antineutrino = false;
	
  for(int i = 0; i< nuPDG->GetNrows();i++){
    if((*nuPDG)(i)<0) antineutrino = true;
    if((*nuPDG)(i)>0) neutrino = true;
  }
	
  if(neutrino && antineutrino) return 0;
  else if(neutrino) return 1;
  else if(antineutrino) return -1;

  return 2;
}

bool FitParameters::SetPriorFromToyFile(TFile* toyFile, int toy){

    int nparams = covarianceI->GetNrows();
    int counter = 0;
    int ff = 0;
    int xf = 0;
    int df = 0;
    int fl = 0;
    int xl = 0;
    int dl = 0;

    for(int i = 0; i < nparams; ++i){
        counter++;
        if(GetShortName(i)[0] == 'b' && ff == 0){
            ff = counter;
            if( df != 0 && dl == 0){
                dl = counter;
            }
            if( xf != 0 && xl == 0){
                xl = counter;
            }
        }
        if(GetShortName(i)[0] == 'x' && xf == 0){
            xf = counter;
            if( df != 0 && dl == 0){
                dl = counter;
            }
            if( ff != 0 && fl == 0){
                fl = counter;
            }
        }
        if(GetShortName(i)[0] == 'o' && df == 0){
            df = counter;
            if( ff != 0 && fl == 0){
                fl = counter;
            }
            if( xf != 0 && xl == 0){
                xl = counter;
            }
        }
    }

    if(dl == 0) dl = nparams -1;
    if(fl == 0) fl = nparams -1;
    if(xl == 0) xl = nparams -1;
    df -= 1;
    dl -= 1;
    xf -= 1;
    xl -= 1;
    ff -= 1;
    fl -= 1;

    TVectorT<double>* xsec_parms = (TVectorT<double>*) toyFile->Get(Form("xsec_parms_%d",toy));
    TVectorT<double>* flux_parms = (TVectorT<double>*) toyFile->Get(Form("flux_parms_%d",toy));
    TVectorT<double>* obs_parms = (TVectorT<double>*) toyFile->Get(Form("obsNorm_parms_%d",toy));

    for(int i = 0; i < nparams; ++i){
        if(fitParameters[i]->priorType==PDFFlat) continue;
        if(fitParameters[i]->discrete) continue;
        if( i < fl && i >= ff ){
            SetPriorParameterValue(i,(*flux_parms)[i - ff]);
        }
        else if( i < xl && i >= xf ){
            SetPriorParameterValue(i,(*xsec_parms)[i - xf]);
        }
        else if( i < dl && i >= df ){
            SetPriorParameterValue(i,(*obs_parms)[i - df]);
        }
    }
    this->PrintPriorValues();
    this->PrintCurrentValues();
    return 1;
}
