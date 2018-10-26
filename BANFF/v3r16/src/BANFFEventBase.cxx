#include "BANFFEventBase.hxx"
#include "FitParameters.hxx"
#include "BANFFSample/BANFFSampleBase.hxx"
#include "SampleId.hxx"

BANFFEventBase::BANFFEventBase(){

  numbXsecSysts = -1;
  evtNumb = -1;

  sample = NULL;

  nSamples = 0;

  nuEnergy = NULL;
  q2 = NULL;
  nuFlavor = NULL;
  reactionCode = NULL;
  targetMaterial = NULL;
  
  truePmu = NULL;
  trueLepPDG = NULL;

  xsecWeights = NULL;
  xsecGraphs = NULL;
  xsecSplines = NULL;
  twkDials = NULL;

  fluxParameter = -1;
  //xsecNormParameter = -1;
  xsecNormParameterNumElements = 0;
  xsecFuncParameter = -1;

  obsNormParameter = -1;
  observableBin = -1;
  sampleIndex = -1;

  //Default all weights to 1.0.
  xsecWeight = 1.0;
  xsecWeightNoDiscreteParams = 1.0;
  POTWeight = 1.0;
  fluxWeight = 1.0;
  detWeight = 1.0;
  totalWeight = 1.0;
  totalWeightNoDiscreteParams = 1.0;
  ObsNormWeight = 1.0;
}




BANFFEventBase::BANFFEventBase(int numbSysts, int number, bool conserveMemory){

  numbXsecSysts = numbSysts;

  //Default these arrays to NULL, and only allocate them if we haven't set
  //the conserveMemory flag.
  xsecWeights = NULL;
  xsecGraphs = NULL;
  xsecSplines = NULL;
  twkDials = NULL;
  if(!conserveMemory){
    xsecWeights = new double*[numbXsecSysts];
    xsecGraphs = new TGraph**[numbXsecSysts];
    xsecSplines = new TSpline3**[numbXsecSysts];
    twkDials = new double[numbXsecSysts];
    for(int i=0; i<numbXsecSysts; i++){
      twkDials[i]=0.;
      //            xsecWeights[i] = NULL;
      //            xsecGraphs[i] = NULL;
      //            xsecSplines[i] = NULL;
    }
  }

  evtNumb = number;

  nSamples = 0;

  nuEnergy = NULL;
  q2 = NULL;
  nuFlavor = NULL;
  reactionCode = NULL;
  targetMaterial = NULL;
  
  truePmu = NULL;
  trueLepPDG = NULL;
  
  ObsNormWeight = 1.0;

  fluxParameter = -1;
  //xsecNormParameter = -1;
  xsecNormParameterNumElements = 0;
  xsecFuncParameter = -1;
	
  obsNormParameter = -1;
  observableBin = -1;
  sampleIndex = -1;

  //Default all weights to 1.0.
  xsecWeight = 1.0;
  POTWeight = 1.0;
  fluxWeight = 1.0;
  detWeight = 1.0;
  totalWeight = 1.0;

  //Although the low memory spline storage options have the array initialized
  //as it is being filled, we do need to set the counters to zero here.
  nContXSRTGraphs = 0;
  nContXSRTSpline3s = 0;
  nDiscXSRTGraphs = 0;
  nDiscXSRTWeights = 0;

}

BANFFEventBase::~BANFFEventBase(){

  for(int i=0; i<numbXsecSysts; i++){
    for(int j = 0; j < nSamples; j++){

      if(xsecGraphs[i] != NULL){
        if(xsecGraphs[i][j]!=NULL){
          xsecGraphs[i][j]->Delete();
        }
      }

      if(xsecSplines[i] != NULL){
        if(xsecSplines[i][j]!=NULL){
          xsecSplines[i][j]->Delete();
        }
      }
        
    }

    if(xsecWeights[i] != NULL){
      delete[] xsecWeights[i];
    }

    if(xsecGraphs[i] != NULL){
      delete[] xsecGraphs[i];
    }

    if(xsecSplines[i] != NULL){
      delete[] xsecSplines[i];
    }
  }

  if (xsecWeights != NULL){
    delete[] xsecWeights;
  }

  if (xsecGraphs != NULL){
    delete[] xsecGraphs;
  }

  if(xsecSplines != NULL){
    delete[] xsecSplines;
  }

  if (twkDials != NULL){
    delete[]  twkDials;
  }

  if (contXSRTGraphIndices != NULL){
    delete[] contXSRTGraphIndices;
  }

  if (contXSRTGraphs != NULL){
    delete[] contXSRTGraphs;
  }

  if(contXSRTSpline3Indices != NULL){
    delete[] contXSRTSpline3Indices;
  }

  if(contXSRTSpline3s != NULL){
    delete[] contXSRTSpline3s;
  }

  if(discXSRTGraphIndices != NULL){
    delete[] discXSRTGraphIndices;
  }

  if(discXSRTGraphs != NULL){
    delete[] discXSRTGraphs;
  }

  if(discXSRTWeights != NULL){
    delete[] discXSRTWeights;
  }
  if(nuEnergy      ) delete[] nuEnergy      ;
  if(q2            ) delete[] q2            ;
  if(nuFlavor      ) delete[] nuFlavor      ;
  if(reactionCode  ) delete[] reactionCode  ;
  if(targetMaterial) delete[] targetMaterial;
  if(truePmu            ) delete[] truePmu            ;
  if(trueLepPDG      ) delete[] trueLepPDG      ;
  nuEnergy       = NULL;
  q2             = NULL;
  nuFlavor       = NULL;
  reactionCode   = NULL;
  targetMaterial = NULL;
  truePmu             = NULL;
  trueLepPDG       = NULL;
    
} 

void BANFFEventBase::AddXsecWeight(int index, double* weight_arr){

  xsecWeights[index] = new double[nSamples];
  xsecGraphs[index]  = new TGraph*[nSamples];
  xsecSplines[index] = new TSpline3*[nSamples];

  for(int i = 0; i < nSamples; ++i){
    xsecWeights[index][i] = weight_arr[i];
    xsecGraphs[index][i] = NULL;
    xsecSplines[index][i] = NULL;
  }

}

void BANFFEventBase::AddXsecResponse(int index, TClonesArray *graph, TSpline3 **spline ){

  if(index>=numbXsecSysts){
    std::cerr << "Response function index is too large " << index << std::endl;
    return;
  }

  xsecGraphs[index] = new TGraph*[nSamples];
  xsecSplines[index] = new TSpline3*[nSamples];
  xsecWeights[index] = new double[nSamples];

  for(int i = 0; i < nSamples; ++i){
    xsecWeights[index][i] = -1;
  }

  if(graph!=NULL){

    for(int i = 0; i < nSamples; i++){
         
      if(graph->At(i) == NULL ){

        xsecGraphs[index][i] = NULL;
        continue;
      }

      //Splines don't get produced if there is only 1 point in the
      //TGraph.  Need to put the 1 point exclusion here, otherwise will
      //waste a lot of memory space on completely useless TGraphs.
      if(((TGraph*)graph->At(i))->GetN() > 1){

        TGraph* tmp_gr = (TGraph*)((graph->At(i))->Clone(Form("gr_%d_%d",evtNumb,index)));
        xsecGraphs[index][i] = tmp_gr;

      }
      else{
        xsecGraphs[index][i] = NULL;
      }
    }
  } else { 

    for(int i = 0; i < nSamples; i++){   
      xsecGraphs[index][i] = NULL;
    }

  }
  if(spline!=NULL){

    for(int i = 0; i < nSamples; i++){

      if(spline[i] == NULL){
        xsecSplines[index][i] = NULL;
        continue;
      }

      TSpline3 *tmp_sp = (TSpline3*)(spline[i]->Clone(Form("sp_%d_%d",evtNumb,index)));
      xsecSplines[index][i] = tmp_sp;
    }
  } else{
    for(int i = 0; i < nSamples; i++){
      xsecSplines[index][i] = NULL;
    }
  }
}

double BANFFEventBase::GetXsecWeight(){


  return xsecWeight;

}

double BANFFEventBase::GetXsecWeight(std::vector<float> &twkDialsv){

  xsecWeight = 1.0; 
  float tmp;
  for(unsigned int i=0; i<twkDialsv.size(); i++){
    if( xsecSplines[i][sample->sampleID]!=NULL ){
      tmp = xsecSplines[i][sample->sampleID]->Eval(twkDialsv[i]);
      if(tmp==tmp) xsecWeight *= tmp;
    } else if( xsecGraphs[i][sample->sampleID]!=NULL ){
      tmp = xsecGraphs[i][sample->sampleID]->Eval(twkDialsv[i]);
      if(tmp==tmp) xsecWeight *= tmp;
    } else if( xsecWeights[i]!=NULL ){
      tmp = xsecWeights[i][sample->sampleID];
      if(tmp > 0) xsecWeight *= tmp;
    }
  }
  if(xsecWeight<0.) return 0.;
  else return xsecWeight;

}

double BANFFEventBase::GetXsecWeight(FitParameters &fitParameters){

  xsecWeight = 1.0; 
  float tmp;
  for(int i=0; i<fitParameters.GetNumbSysts(); i++){
    if(fitParameters.GetParamType(i)!=XSECRESP) continue;
    if( xsecSplines[fitParameters.GetTypeIndex(i)][sample->sampleID]!=NULL ){
      /*if(reactionCode==0 && fitParameters.GetXsecID(i)==MAQE && fabs(fitParameters.GetCurrentValue(i))>1.e-5 ){
        std::cout << fitParameters.GetCurrentValue(i) << " " << 
        xsecSplines[fitParameters.GetTypeIndex(i)]->Eval(fitParameters.GetCurrentValue(i)) << std::endl;
        }*/
      tmp = xsecSplines[fitParameters.GetTypeIndex(i)][sample->sampleID]->Eval(fitParameters.GetCurrentValue(i,true));
      if(tmp==tmp) xsecWeight *= tmp;
    } else if( xsecGraphs[fitParameters.GetTypeIndex(i)][sample->sampleID]!=NULL ){
      tmp = xsecGraphs[fitParameters.GetTypeIndex(i)][sample->sampleID]->Eval(fitParameters.GetCurrentValue(i,true));
      if(tmp==tmp) xsecWeight *= tmp;
    } else if( xsecWeights[fitParameters.GetTypeIndex(i)][sample->sampleID] > 0 ){
      tmp = xsecWeights[fitParameters.GetTypeIndex(i)][sample->sampleID];
      if(tmp==tmp) xsecWeight *= tmp;
    }

  }
  if(xsecWeight<0.) return 0.;
  else return xsecWeight;

}

void BANFFEventBase::SaveTotalWeight(FitParameters* fitParameters, double Q2, int NuFlavor){

  //Reset xsecWeight to 1.0 (otherwise it is not reset anywhere).
  xsecWeight = 1.0;
  //The fluxWeight and the POT weight are always included.
  totalWeight = fluxWeight * POTWeight;

  //Similarly, when events are processed the detector weight is saved.  Since
  //that always applies (And is some nominal value if there were no
  //systematic variations) apply that here as well.
  totalWeight *= detWeight;

  //If the detWeight has made the total weight go negative, set it to zero
  //and return.
  if(totalWeight < 0.0){

    totalWeight = 0.0;
    return;
  }

  int debug = 1;
	
  double tmp;
  //Loop over all the parameters.
  for(int i=0; i<fitParameters->GetNumbSysts(); i++){

    //We only need to deal with XSECRESP parameters here.  If it is not
    //one, continue to the next parameter.
    if(fitParameters->GetParamType(i)!=XSECRESP){
      continue;
    }

    //If this parameter is not enabled, skip it.
    if(!(fitParameters->IsEnabled(i))){
      continue;
    }
		
    std::string paramName = fitParameters->GetParamName(i);
    //Make sure that nubar is applied to nubar only and same thing for nu
    if((paramName == "2p2h_shape_C_nu" || paramName == "2p2h_shape_O_nu") && NuFlavor < 0 ) continue;
    if((paramName == "2p2h_shape_C_nubar" || paramName == "2p2h_shape_O_nubar") && NuFlavor > 0 ) continue;
        
    //If there is a spline for this XSECRESP parameter, evaluate the spline
    //at the current parameter value.  If it gives a reasonable value (i.e.
    //a number) multiply the total weight by it.
    //std::cout << "Spline: " << fitParameters->GetParamName(i) << " "<< SampleId::ConvertSample((SampleId::SampleEnum)sample->sampleID) << std::endl;
    if(xsecSplines[fitParameters->GetTypeIndex(i)][sample->sampleID] != NULL){
      //if(debug==0)
      tmp = xsecSplines[fitParameters->GetTypeIndex(i)][sample->sampleID]->Eval(fitParameters->GetCurrentValue(i,true));

      if(tmp==tmp){
        totalWeight *= tmp;
        xsecWeight *= tmp;

        //if(fitParameters->GetParamName(i) == "SF_RFG"){
        //    std::cout << "Spline SF_RFG: " << tmp << std::endl;
        //}

      }
    } 

    //Otherwise, this XSECRESP parameter uses a Graph instead of a spline.
    //Use it to get the weight contribution for this parameter value.
    else if( xsecGraphs[fitParameters->GetTypeIndex(i)][sample->sampleID]!=NULL ){
      //if(debug==0) std::cout << "Graph: " << fitParameters->GetParamName(i) << " "<< xsecGraphs[fitParameters->GetTypeIndex(i)][sample->sampleID]->Eval(fitParameters->GetCurrentValue(i,true))<<std::endl;
      tmp = xsecGraphs[fitParameters->GetTypeIndex(i)][sample->sampleID]->Eval(fitParameters->GetCurrentValue(i,true));

      if(tmp==tmp){ 
        totalWeight *= tmp;
        xsecWeight *= tmp;
        //if(fitParameters->GetParamName(i) == "SF_RFG"){
        //    std::cout << "Graph SF_RFG: " << tmp << std::endl;
        //}
      }
    }
    else if( xsecWeights[fitParameters->GetTypeIndex(i)][sample->sampleID] > 0 ){
      //if(debug==0) std::cout << "Weight: " << fitParameters->GetParamName(i) <<" "<<xsecWeights[fitParameters->GetTypeIndex(i)][sample->sampleID]<< std::endl;
      tmp = xsecWeights[fitParameters->GetTypeIndex(i)][sample->sampleID];
      totalWeight *= tmp;
      xsecWeight *= tmp;
    }
  }

  //With the XSECRESP weights out of the way, now consider the remaining
  //contributions.

  //if(debug==0) std::cout << "Response weight " << totalWeight << std::endl;

  //If these weight applications have made the weight go negative, set it to
  //zero and stop.
  if(totalWeight<0.){
    totalWeight=0.;
    return;
  } 

  //Now multiply in the XSec Functionnal weight, if there is one (i.e. the
  //parameter index value is greater than 0, as it defaults to -1 otherwise.)
  if(xsecFuncParameter>=0){
    //Get position of the first BeRPA parameter
    int BeRPA = fitParameters->GetBeRPA();
    totalWeight *= calcRPA(Q2, fitParameters->GetCurrentValue(BeRPA,true), fitParameters->GetCurrentValue(BeRPA+1,true), fitParameters->GetCurrentValue(BeRPA+2,true), fitParameters->GetCurrentValue(BeRPA+3,true), fitParameters->GetCurrentValue(BeRPA+4,true));
    xsecWeight *= calcRPA(Q2, fitParameters->GetCurrentValue(BeRPA,true), fitParameters->GetCurrentValue(BeRPA+1,true), fitParameters->GetCurrentValue(BeRPA+2,true), fitParameters->GetCurrentValue(BeRPA+3,true), fitParameters->GetCurrentValue(BeRPA+4,true));
    if(fitParameters->GetParamName(BeRPA) != "BeRPA_A" || fitParameters->GetParamName(BeRPA+1) != "BeRPA_B" || fitParameters->GetParamName(BeRPA+2) != "BeRPA_D"
       || fitParameters->GetParamName(BeRPA+3) != "BeRPA_E"|| fitParameters->GetParamName(BeRPA+4) != "BeRPA_U"){
      std::cout<<"CAUTION !! The hardcoded parameter number for BeRPA in src/BANFEventBase.cxx are wrong, the fit is not working properly "<<std::endl;
      std::cout << "BeRPA replaced by : " << fitParameters->GetParamName(BeRPA) <<" "<< fitParameters->GetParamName(BeRPA+1) << " "<< fitParameters->GetParamName(BeRPA+3) << " "<< fitParameters->GetParamName(BeRPA+3) << " "<< fitParameters->GetParamName(BeRPA+4)<< std::endl;
    }
    //if(debug==0) std::cout <<"BeRPA weight: " << calcRPA(Q2, fitParameters->GetCurrentValue(BeRPA+4,true), fitParameters->GetCurrentValue(678,true), fitParameters->GetCurrentValue(679,true), fitParameters->GetCurrentValue(680,true), fitParameters->GetCurrentValue(681,true)) << std::endl;
		
  }
	
	
  //If these weight applications have made the weight go negative, set it to
  //zero and stop.
  if(totalWeight < 0.0){
    totalWeight = 0.0;
    return;
  } 
	
  //Multiply in the flux systematic weight, if there is one (i.e. the
  //parameter index value is greater than 0, as it defaults to -1 otherwise.)
  if(fluxParameter>=0){

    //Only apply the weight if the parameter is enabled.
    if(fitParameters->IsEnabled(fluxParameter)){
      totalWeight *= fitParameters->GetCurrentValue(fluxParameter,true);
      //if(debug==0) std::cout << "Flux weight " << fitParameters->GetCurrentValue(fluxParameter,true) << std::endl;
    }
  }


  //If the flux weight has made the total weight go negative, set it to zero
  //and return.
  if(totalWeight<0.){
    totalWeight=0.;
    return;
  } 

  //Now multiply in the XSec normalization weight, if there is one (i.e. the
  //parameter index vector is not empty, as it defaults to empty otherwise.
  if(xsecNormParameterNumElements > 0){
    for(int index = 0; index < xsecNormParameterNumElements; index++){
        totalWeight *= fitParameters->GetCurrentValue(xsecNormParameter[index], true);
        xsecWeight  *= fitParameters->GetCurrentValue(xsecNormParameter[index], true);
        //std::cout << "Norm applied: " << fitParameters->GetParamName(xsecNormParameter[index]) << " " << fitParameters->GetCurrentValue(xsecNormParameter[index], true) <<std::endl;
    }
  }
  //if(debug==0) std::cout << "Norm weight " << totalWeight << std::endl;
  ////Now multiply in the XSec normalization weight, if there is one (i.e. the
  ////parameter index value is greater than 0, as it defaults to -1 otherwise.)
  //if(xsecNormParameter>=0){
  //  if(fitParameters->GetParamName(xsecNormParameter) == "2p2h_normCtoO"){
  //    if(NuFlavor > 0){
  //      totalWeight *= fitParameters->GetCurrentValue(xsecNormParameter,true) * fitParameters->GetCurrentValue(xsecNormParameter-2,true);
  //      xsecWeight *= fitParameters->GetCurrentValue(xsecNormParameter,true) * fitParameters->GetCurrentValue(xsecNormParameter-2,true);
  //      //std::cout << "Norm applied " << fitParameters->GetParamName(xsecNormParameter)<< " "<< fitParameters->GetParamName(xsecNormParameter-2) <<" "<< fitParameters->GetCurrentValue(xsecNormParameter,true) * fitParameters->GetCurrentValue(xsecNormParameter-2,true)<< std::endl;
  //    }
  //    else if (NuFlavor < 0){
  //      totalWeight *= fitParameters->GetCurrentValue(xsecNormParameter,true) * fitParameters->GetCurrentValue(xsecNormParameter-1,true);
  //      xsecWeight *= fitParameters->GetCurrentValue(xsecNormParameter,true) * fitParameters->GetCurrentValue(xsecNormParameter-1,true);
  //      //std::cout << "Norm applied " << fitParameters->GetParamName(xsecNormParameter)<< " "<< fitParameters->GetParamName(xsecNormParameter-1) <<" "<< fitParameters->GetCurrentValue(xsecNormParameter,true) * fitParameters->GetCurrentValue(xsecNormParameter-1,true)<< std::endl;
  //    }
  //  }
  //  else {
  //    totalWeight *= fitParameters->GetCurrentValue(xsecNormParameter,true);
  //    xsecWeight *= fitParameters->GetCurrentValue(xsecNormParameter,true);
  //    //if(debug==0) std::cout << "Norm applied " << fitParameters->GetParamName(xsecNormParameter)<< " "<< fitParameters->GetCurrentValue(xsecNormParameter,true)<<std::endl;
  //  }
  //}

  //If this has made the weight negative, set to 0 and return.
  if(totalWeight<0.){
    totalWeight=0.;
    return;
  }

  //if(debug==0) std::cout <<"obsNormParameter, pre if: " << obsNormParameter << std::endl;

  //Now multiply in the observable normalization parameter weight, if there
  //is one (i.e. the parameter index value is greater than 0, since it
  //defaults to -1 otherwise.)
  if(obsNormParameter>=0) {

    //Only apply the weight if the parameter is enabled.
    if(fitParameters->IsEnabled(obsNormParameter)){
        
      //if(debug==0) std::cout << "obsNormParameter : " <<fitParameters->GetParamName(obsNormParameter)<<" "<< fitParameters->GetCurrentValue(obsNormParameter) << std::endl;
      totalWeight *= fitParameters->GetCurrentValue(obsNormParameter);
    }

  }

  //If this has made the weight negative, set to 0 and return.
  if(totalWeight<0.){
    totalWeight=0.;
    return;
  }

  if(debug==0) std::cout << "Total weight " << totalWeight << std::endl;

}


void BANFFEventBase::SetXsecTwk(XsecSysts index, float tweak ){

  twkDials[index] = tweak;  

}   


void BANFFEventBase::SetReactionCode(Int_t* x){

  if(reactionCode){
    delete[] reactionCode;
  }

  reactionCode = new Int_t[nSamples];

  for(int i = 0; i < nSamples; i++){

    reactionCode[i] = x[i];
  }

}

void BANFFEventBase::SetThrow(float weight, char sample, short bin){

  thrownWeights.push_back(weight);
  thrownSamples.push_back(sample);
  thrownBins.push_back(bin);

}

void BANFFEventBase::SetThrow(float weight, char sample, short bin, int throwIndex){

  thrownWeights[throwIndex] *= weight;
  thrownSamples[throwIndex]  = sample;
  thrownBins[throwIndex] = bin;

}


void BANFFEventBase::SetNuEnergy(Double_t* x){

  //Delete the old array if it exists;
  if(nuEnergy){
    delete[] nuEnergy;
    nuEnergy=NULL;
  }

  //Create a new array to store this information, length NSamples.
  nuEnergy = new Double_t[nSamples];

  for(int i = 0; i < nSamples; i++){
    nuEnergy[i] = x[i];
  }

}

void BANFFEventBase::SetQ2(Double_t* x){
  //Delete the old array if it exists;
  if(q2){
    delete[] q2;
    q2=NULL;
  }

  //Create a new array to store this information, length NSamples.
  q2 = new Double_t[nSamples];

  for(int i = 0; i < nSamples; i++){
    q2[i] = x[i];
  }

}

void BANFFEventBase::SetNuFlavor(Int_t* x){

  //Delete the old array if it exists;
  if(nuFlavor){
    delete[] nuFlavor;
    nuFlavor=NULL;
  }

  //Create a new array to store this information, length NSamples.
  nuFlavor = new Int_t[nSamples];

  for(int i = 0; i < nSamples; i++){
    nuFlavor[i] = x[i];
  }

}

void BANFFEventBase::SetTargetMaterial(Int_t* x){

  //Delete the old array if it exists;
  if(targetMaterial){
    delete[] targetMaterial;
    targetMaterial=NULL;
  }

  //Create a new array to store this information, length NSamples.
  targetMaterial = new Int_t[nSamples];

  for(int i = 0; i < nSamples; i++){
    targetMaterial[i] = x[i];
  }

}

void BANFFEventBase::SetTruePmu(Double_t* x){
  //Delete the old array if it exists;
  if(truePmu){
    delete[] truePmu;
    truePmu=NULL;
  }

  //Create a new array to store this information, length NSamples.
  truePmu = new Double_t[nSamples];

  for(int i = 0; i < nSamples; i++){
    truePmu[i] = x[i];
  }

}

void BANFFEventBase::SetTrueLepPDG(Int_t* x){

  //Delete the old array if it exists;
  if(trueLepPDG){
    delete[] trueLepPDG;
    trueLepPDG=NULL;
  }

  //Create a new array to store this information, length NSamples.
  trueLepPDG = new Int_t[nSamples];

  for(int i = 0; i < nSamples; i++){
    trueLepPDG[i] = x[i];
  }

}


int BANFFEventBase::GetTargetMaterial(){

  return targetMaterial[sample->sampleID];

}


double BANFFEventBase::GetNuEnergy(){

  return nuEnergy[sample->sampleID];
}

double BANFFEventBase::GetQ2(){

  return q2[sample->sampleID];

}

int BANFFEventBase::GetNuFlavor(){

  return nuFlavor[sample->sampleID];
}

int BANFFEventBase::GetReactionCode(){

  return reactionCode[sample->sampleID];

}

double BANFFEventBase::GetTruePmu(){
  return truePmu[sample->sampleID];
}

int BANFFEventBase::GetTrueLepPDG(){
  return trueLepPDG[sample->sampleID];
}

void BANFFEventBase::SaveTotalWeightFast(FitParameters* fitParameters, double Q2, int NuFlavor){

  //If the continuous parameters have not been computed for this set of fit
  //parameter values yet, do the full total weight with no discrete
  //parameters.
  //NB: The value of continuousParamsComputed should be set and reset outside
  //of this method, only after all events have been manipulated properly.
  if(!(fitParameters->continuousParamsComputed)){

    //Reset xsecWeightNoDiscreteParams to 1.0 (otherwise it is not reset anywhere).
    xsecWeightNoDiscreteParams = 1.0;
    //The fluxWeight and the POT weight are always included.
    totalWeightNoDiscreteParams = fluxWeight * POTWeight;

    //Similarly, when events are processed the detector weight is saved.  Since
    //that always applies (And is some nominal value if there were no
    //systematic variations) apply that here as well.
    totalWeightNoDiscreteParams *= detWeight;

    //If the detWeight has made the total weight go negative, set it to zero
    //and return.
    if(totalWeightNoDiscreteParams < 0.0){

      totalWeightNoDiscreteParams = 0.0;
      totalWeight = 0.0;
      return;
    }

    double tmp;
    double x, y, b, c, d, num, pos;
    int l;

    //Loop over all the parameters.
    for(int i=0; i<fitParameters->numbSystsFast; i++){
      //We only need to deal with XSECRESP parameters here.  If it is not
      //one, continue to the next parameter.
      //We also want to skip discrete parameters, since their pre-stored
      //values will be multiplied on later to save processing time.
      if(((fitParameters->fitParametersFast[i]->paramType) != XSECRESP)
         || (fitParameters->fitParametersFast[i]->discrete)){
        continue;
      }

      //If there is a spline for this XSECRESP parameter, evaluate the spline
      //at the current parameter value.  If it gives a reasonable value (i.e.
      //a number) multiply the total weight by it.
      if( xsecSplines[fitParameters->fitParametersFast[i]->typeIndex][sample->sampleID]!=NULL ){
        if(fitParameters->fitParametersFast[i]->nStep < 0){
          if(fitParameters->fitParametersFast[i]->decompose != 0) tmp = xsecSplines[fitParameters->fitParametersFast[i]->typeIndex][sample->sampleID]->Eval(fitParameters->fitParametersFast[i]->currentValueUD);
          else tmp = xsecSplines[fitParameters->fitParametersFast[i]->typeIndex][sample->sampleID]->Eval(fitParameters->fitParametersFast[i]->currentValue);
          //std::cout << "Non equidistant spline : " << fitParameters->fitParametersFast[i]->paramName<<" "<<tmp << std::endl;
          if(tmp==tmp){
            totalWeight *= tmp;
            xsecWeight *= tmp;
          }
        }
        else{
          //We already know that this TSpline3 is not NULL, otherwise it
          //wouldn't have been saved.  So proceed straight to using it.
          if(fitParameters->fitParametersFast[i]->decompose != 0){
            pos = fitParameters->fitParametersFast[i]->currentValueUD;
            if(pos < fitParameters->fitParametersFast[i]->xMin){
              xsecSplines[fitParameters->fitParametersFast[i]->typeIndex][sample->sampleID]->GetCoeff(0, x, y, b, c, d);
              num = pos-x;
              tmp = (y + num*b + num*num*c + num*num*num*d);
            }
            else if(pos > fitParameters->fitParametersFast[i]->xMax){
              xsecSplines[fitParameters->fitParametersFast[i]->typeIndex][sample->sampleID]->GetCoeff(fitParameters->fitParametersFast[i]->nStep - 1, x, y, b, c, d);
              num = pos-x;
              tmp = (y + num*b + num*num*c + num*num*num*d);
            }
            else{
              l = ((pos-fitParameters->fitParametersFast[i]->xMin)/fitParameters->fitParametersFast[i]->stepSize)+1;
              xsecSplines[fitParameters->fitParametersFast[i]->typeIndex][sample->sampleID]->GetCoeff(l, x, y, b, c, d);
              num = pos-x;
              tmp = (y + num*b + num*num*c + num*num*num*d);
            }
          }
          else{
            pos = fitParameters->fitParametersFast[i]->currentValue;
            if(pos < fitParameters->fitParametersFast[i]->xMin){
              xsecSplines[fitParameters->fitParametersFast[i]->typeIndex][sample->sampleID]->GetCoeff(0, x, y, b, c, d);
              num = pos-x;
              tmp = (y + num*b + num*num*c + num*num*num*d);
            }
            else if(pos > fitParameters->fitParametersFast[i]->xMax){
              xsecSplines[fitParameters->fitParametersFast[i]->typeIndex][sample->sampleID]->GetCoeff(fitParameters->fitParametersFast[i]->nStep-1, x, y, b, c, d);
              num = pos-x;
              tmp = (y + num*b + num*num*c + num*num*num*d);
            }
            else{
              l = ((pos-fitParameters->fitParametersFast[i]->xMin)/fitParameters->fitParametersFast[i]->stepSize)+1;
              xsecSplines[fitParameters->fitParametersFast[i]->typeIndex][sample->sampleID]->GetCoeff(l, x, y, b, c, d);
              num = pos-x;
              tmp = (y + num*b + num*num*c + num*num*num*d);
            }
          }
          if(tmp==tmp){
            totalWeightNoDiscreteParams *= tmp;
            xsecWeightNoDiscreteParams *= tmp;


          }
        }
      } 

      //Otherwise, this XSECRESP parameter uses a Graph instead of a spline.
      //Use it to get the weight contribution for this parameter value.
      else if( xsecGraphs[fitParameters->fitParametersFast[i]->typeIndex][sample->sampleID]!=NULL ){
        if(fitParameters->fitParametersFast[i]->decompose != 0){
          tmp = xsecGraphs[fitParameters->fitParametersFast[i]->typeIndex][sample->sampleID]->Eval(fitParameters->fitParametersFast[i]->currentValueUD);
        }

        else{
          tmp = xsecGraphs[fitParameters->fitParametersFast[i]->typeIndex][sample->sampleID]->Eval(fitParameters->fitParametersFast[i]->currentValue);

        }
        if(tmp==tmp){ 
          totalWeightNoDiscreteParams *= tmp;
          xsecWeightNoDiscreteParams *= tmp;
        }
      }
    }

    //With the XSECRESP weights out of the way, now consider the remaining
    //contributions.

    //std::cout << "Response weight " << totalWeight << std::endl;

    //If these weight applications have made the weight go negative, set it to
    //zero and stop.
    if(totalWeightNoDiscreteParams < 0.0){
      totalWeightNoDiscreteParams = 0.0;
      totalWeight = 0.0;
      return;
    } 

    //Now multiply in the XSec Functionnal weight, if there is one (i.e. the
    //parameter index value is greater than 0, as it defaults to -1 otherwise.)
    if(xsecFuncParameter>=0){
      //Get position of the first BeRPA parameter
      int BeRPA = fitParameters->GetBeRPA();
      totalWeightNoDiscreteParams *= calcRPA(Q2, fitParameters->fitParametersFast[BeRPA]->currentValue, fitParameters->fitParametersFast[BeRPA+1]->currentValue, fitParameters->fitParametersFast[BeRPA+2]->currentValue, fitParameters->fitParametersFast[BeRPA+3]->currentValue, fitParameters->fitParametersFast[BeRPA+4]->currentValue);
      xsecWeightNoDiscreteParams *= calcRPA(Q2, fitParameters->fitParametersFast[BeRPA]->currentValue, fitParameters->fitParametersFast[BeRPA+1]->currentValue, fitParameters->fitParametersFast[BeRPA+2]->currentValue, fitParameters->fitParametersFast[BeRPA+3]->currentValue, fitParameters->fitParametersFast[BeRPA+4]->currentValue);
      if(fitParameters->fitParametersFast[BeRPA]->paramName != "BeRPA_A" || fitParameters->fitParametersFast[BeRPA+1]->paramName != "BeRPA_B" || fitParameters->fitParametersFast[BeRPA+2]->paramName != "BeRPA_D" || fitParameters->fitParametersFast[BeRPA+3]->paramName != "BeRPA_E"|| fitParameters->fitParametersFast[BeRPA+4]->paramName != "BeRPA_U"){
        std::cout<<"CAUTION !! The hardcoded parameter number for BeRPA in src/BANFEventBase.cxx are wrong, the fit is not working properly "<<std::endl;
        std::cout << "BeRPA replaced by : " << fitParameters->fitParametersFast[BeRPA]->paramName <<" "<< fitParameters->fitParametersFast[BeRPA+1]->paramName << " "<< fitParameters->fitParametersFast[BeRPA+2]->paramName << " "<< fitParameters->fitParametersFast[BeRPA+3]->paramName << " "<< fitParameters->fitParametersFast[BeRPA+4]->paramName<< std::endl;
      }
      //std::cout <<"XsecFuncParameter weight: " << calcRPA(Q2, fitParameters->fitParametersFast[677]->currentValue, fitParameters->fitParametersFast[678]->currentValue, fitParameters->fitParametersFast[679]->currentValue, fitParameters->fitParametersFast[680]->currentValue, fitParameters->fitParametersFast[681]->currentValue) << std::endl;
    }
		
    //If these weight applications have made the weight go negative, set it to
    //zero and stop.
    if(totalWeightNoDiscreteParams < 0.0){
      totalWeightNoDiscreteParams = 0.0;
      totalWeight = 0.0;
      return;
    } 
				
    //Multiply in the flux systematic weight, if there is one (i.e. the
    //parameter index value is greater than 0, as it defaults to -1 otherwise.)
    if(fluxParameter>=0){

      if(fitParameters->fitParametersFast[fluxParameter]->decompose != 0){
        totalWeightNoDiscreteParams *= fitParameters->fitParametersFast[fluxParameter]->currentValueUD;
      }
      else{
        totalWeightNoDiscreteParams *= fitParameters->fitParametersFast[fluxParameter]->currentValue;
      }

    }

    //std::cout << "Flux weight " << totalWeight << std::endl;

    //If the flux weight has made the total weight go negative, set it to zero
    //and return.
    if(totalWeightNoDiscreteParams < 0.0){
      totalWeight = 0.0;
      totalWeightNoDiscreteParams = 0.0;
      return;
    } 

    //Now multiply in the XSec normalization weight, if there is one (i.e. the
    //parameter index vector is not empty, as it defaults to empty otherwise.
    if(xsecNormParameterNumElements > 0){
        for(int index = 0; index < xsecNormParameterNumElements; index++){
            totalWeightNoDiscreteParams *= fitParameters->fitParametersFast[xsecNormParameter[index]]->currentValue;
            xsecWeightNoDiscreteParams  *= fitParameters->fitParametersFast[xsecNormParameter[index]]->currentValue;
            //std::cout << "Norm applied: " << fitParameters->fitParametersFast[xsecNormParameter[index]]->paramName << " " <<fitParameters->fitParametersFast[xsecNormParameter[index]]->currentValue <<std::endl;
        }
    }
    //if(debug==0) std::cout << "Norm weight " << totalWeightNoDiscreteParams << std::endl;
    ////Now multiply in the XSec normalization weight, if there is one (i.e. the
    ////parameter index value is greater than 0, as it defaults to -1 otherwise.)
    //if(xsecNormParameter>=0){
    //  if(fitParameters->fitParametersFast[xsecNormParameter]->paramName ==  "nue_numu")
    //    std::cout << "Xsec Norm: " << fitParameters->fitParametersFast[xsecNormParameter]->paramName << ": "
    //              << fitParameters->fitParametersFast[xsecNormParameter]->currentValue << std::endl;
    //  if(fitParameters->fitParametersFast[xsecNormParameter]->paramName == "2p2h_normCtoO"){
    //    if(NuFlavor > 0){
    //      totalWeightNoDiscreteParams *= fitParameters->fitParametersFast[xsecNormParameter]->currentValue * fitParameters->fitParametersFast[xsecNormParameter -2 ]->currentValue;
    //      xsecWeightNoDiscreteParams *= fitParameters->fitParametersFast[xsecNormParameter]->currentValue * fitParameters->fitParametersFast[xsecNormParameter -2 ]->currentValue;
    //    }
    //    else if(NuFlavor < 0){
    //      totalWeightNoDiscreteParams *= fitParameters->fitParametersFast[xsecNormParameter]->currentValue * fitParameters->fitParametersFast[xsecNormParameter -1 ]->currentValue;
    //      xsecWeightNoDiscreteParams *= fitParameters->fitParametersFast[xsecNormParameter]->currentValue * fitParameters->fitParametersFast[xsecNormParameter -1 ]->currentValue;
    //    }
    //  }
    //  else {
    //    totalWeightNoDiscreteParams *= fitParameters->fitParametersFast[xsecNormParameter]->currentValue;
    //    xsecWeightNoDiscreteParams *= fitParameters->fitParametersFast[xsecNormParameter]->currentValue;
    //  }
    //}

    //If this has made the weight negative, set to 0 and return.
    if(totalWeightNoDiscreteParams < 0.0){
      totalWeight = 0.0;
      totalWeightNoDiscreteParams = 0.0;
      return;
    }

    //std::cout <<"obsNormParameter, pre if: " << obsNormParameter << std::endl;

    //Now multiply in the observable normalization parameter weight, if there
    //is one (i.e. the parameter index value is greater than 0, since it
    //defaults to -1 otherwise.)
    if(obsNormParameter>=0) {
      //std::cout << "obsNormParameter, post if: " << obsNormParameter << std::endl;
      totalWeightNoDiscreteParams *= fitParameters->fitParametersFast[obsNormParameter]->currentValue;
    }

    //If this has made the weight negative, set to 0 and return.
    if(totalWeightNoDiscreteParams < 0.0){
      totalWeight = 0.0;
      totalWeightNoDiscreteParams = 0.0;
      return;
    }
  }

  //At this point the total weight with no discrete parameters has been
  //calculated above or is pre-stored from a previous discrete parameter
  //value.  Apply the weight for the appropriate discrete parameter
  //combination to this event.
  totalWeight = totalWeightNoDiscreteParams*(discreteParameterWeights[fitParameters->currentDiscreteParameterCombinationIndex]);
  xsecWeight = xsecWeightNoDiscreteParams*(discreteParameterWeights[fitParameters->currentDiscreteParameterCombinationIndex]);

  if(totalWeight < 0.0){

    totalWeight = 0.0;
    return;
  }


}

void BANFFEventBase::SaveDiscreteParameterWeights(FitParameters* fitParameters){

  //Start by setting up the array to store the weights in.
  discreteParameterWeights = new double[fitParameters->nDiscreteParameterCombinations];

  //Set up a temporary variable to store the weight value in.
  double tmp = 1.0;

  //Loop over all the discrete parameter combinations in fitParameters.
  for(int dpi = 0; dpi < fitParameters->nDiscreteParameterCombinations; dpi++){

    //Initialize the discrete parameter weight for this event and
    //combination to 1.0;
    discreteParameterWeights[dpi] = 1.0;

    //Loop over the fit parameters.
    for(int ip = 0; ip < fitParameters->GetNumbSysts(); ip++){

      //If the parameter is not discrete, continue to the next one.
      if(!(fitParameters->GetIsDiscrete(ip))){
        continue;
      }

      //If we make it here, we have a discrete parameter.  Discrete
      //parameters use TGraphs instead of TSpline3.  Verify that this
      //parameter has a non-NULL TGraph associated with this event before
      //proceeding.
      if(xsecGraphs[fitParameters->GetTypeIndex(ip)][sample->sampleID] != NULL){

        //Fill tmp with the value from this xsecGraphs evaluated at
        //this discrete parameter combination's value for this
        //parameter.
        tmp = xsecGraphs[fitParameters->GetTypeIndex(ip)][sample->sampleID]->Eval(fitParameters->discreteParameterCombinations[dpi][ip]);

        //If there's not a problem with tmp, multiply it by this
        //discreteParameterWeights value.
        if(tmp==tmp){
          discreteParameterWeights[dpi] *= tmp;
        }
      }
      else if( xsecWeights[fitParameters->GetTypeIndex(ip)][sample->sampleID] > 0  ){
        tmp = xsecWeights[fitParameters->GetTypeIndex(ip)][sample->sampleID];
        discreteParameterWeights[dpi] *= tmp;
      }
    }
  } 
}

void BANFFEventBase::SaveTotalWeightFastLowMem(FitParameters* fitParameters, double Q2, int NuFlavor){
	
  //If the continuous parameters have not been computed for this set of fit
  //parameter values yet, do the full total weight with no discrete
  //parameters.
  //NB: The value of continuousParamsComputed should be set and reset outside
  //of this method, only after all events have been manipulated properly.
  if(!(fitParameters->continuousParamsComputed)){

    //Reset xsecWeightNoDiscreteParams to 1.0 (otherwise it is not reset anywhere).
    xsecWeightNoDiscreteParams = 1.0;
    //The fluxWeight and the POT weight are always included.
    totalWeightNoDiscreteParams = fluxWeight * POTWeight;

    //Similarly, when events are processed the detector weight is saved.  Since
    //that always applies (And is some nominal value if there were no
    //systematic variations) apply that here as well.
    totalWeightNoDiscreteParams *= detWeight;

    //If the detWeight has made the total weight go negative, set it to zero
    //and return.
    if(totalWeightNoDiscreteParams < 0.0){
      totalWeightNoDiscreteParams = 0.0;
      totalWeight = 0.0;
      return;
    }

    double tmp;
    double x, y, b, c, d, num, pos;
    int l;
    //Start by looping through the stored TSpline3s for continuous
    //parameters.
    for(int i = 0; i < nContXSRTSpline3s; i++){			
      //We already know that this TSpline3 is not NULL, otherwise it
      //wouldn't have been saved.  So proceed straight to using it.

      if(fitParameters->fitParametersFast[contXSRTSpline3Indices[i]]->nStep < 0){
        if(fitParameters->fitParametersFast[contXSRTSpline3Indices[i]]->decompose != 0){
          tmp = contXSRTSpline3s[i]->Eval(fitParameters->fitParametersFast[contXSRTSpline3Indices[i]]->currentValueUD);
          //std::cout << "Non equidistant spline decomposed : " <<fitParameters->fitParametersFast[contXSRTSpline3Indices[i]]->paramName<<" "<<fitParameters->fitParametersFast[contXSRTSpline3Indices[i]]->currentValueUD<<" "<<tmp << std::endl;
					 
        }
        else{
          tmp = contXSRTSpline3s[i]->Eval(fitParameters->fitParametersFast[contXSRTSpline3Indices[i]]->currentValue);

        }
        if(tmp==tmp){
          totalWeightNoDiscreteParams *= tmp;
          xsecWeightNoDiscreteParams *= tmp;
        }
      }
      else{
        //If found a parameter with nubar in the name, then it's a nubar event
        if(fitParameters->fitParametersFast[contXSRTSpline3Indices[i]]->decompose != 0){
          pos = fitParameters->fitParametersFast[contXSRTSpline3Indices[i]]->currentValueUD;
          if(pos < fitParameters->fitParametersFast[contXSRTSpline3Indices[i]]->xMin){
            contXSRTSpline3s[i]->GetCoeff(0, x, y, b, c, d);
            num = pos-x;
            tmp = (y + num*b + num*num*c + num*num*num*d);

          }
          else if(pos > fitParameters->fitParametersFast[contXSRTSpline3Indices[i]]->xMax){
            contXSRTSpline3s[i]->GetCoeff(fitParameters->fitParametersFast[contXSRTSpline3Indices[i]]->nStep - 1, x, y, b, c, d);
            num = pos-x;
            tmp = (y + num*b + num*num*c + num*num*num*d);

          }
          else{
            l = ((pos-fitParameters->fitParametersFast[contXSRTSpline3Indices[i]]->xMin)/fitParameters->fitParametersFast[contXSRTSpline3Indices[i]]->stepSize)+1;
            contXSRTSpline3s[i]->GetCoeff(l, x, y, b, c, d);
            num = pos-x;
            tmp = (y + num*b + num*num*c + num*num*num*d);
						    
          }
        }
        else{
          pos = fitParameters->fitParametersFast[contXSRTSpline3Indices[i]]->currentValue;
          if(pos < fitParameters->fitParametersFast[contXSRTSpline3Indices[i]]->xMin){
            contXSRTSpline3s[i]->GetCoeff(0, x, y, b, c, d);
            num = pos-x;
            tmp = (y + num*b + num*num*c + num*num*num*d);

          }
          else if(pos > fitParameters->fitParametersFast[contXSRTSpline3Indices[i]]->xMax){
            contXSRTSpline3s[i]->GetCoeff(fitParameters->fitParametersFast[contXSRTSpline3Indices[i]]->nStep-1, x, y, b, c, d);
            num = pos-x;
            tmp = (y + num*b + num*num*c + num*num*num*d);

          }
          else{
            l = ((pos-fitParameters->fitParametersFast[contXSRTSpline3Indices[i]]->xMin)/fitParameters->fitParametersFast[contXSRTSpline3Indices[i]]->stepSize)+1;
            contXSRTSpline3s[i]->GetCoeff(l, x, y, b, c, d);
            num = pos-x;
            tmp = (y + num*b + num*num*c + num*num*num*d);

          }
        }
        if(tmp==tmp){
          totalWeightNoDiscreteParams *= tmp;
          xsecWeightNoDiscreteParams *= tmp;
          //std::cout << "Equidistant spline : " <<fitParameters->fitParametersFast[contXSRTSpline3Indices[i]]->paramName<<" "<<fitParameters->fitParametersFast[contXSRTSpline3Indices[i]]->currentValue<<" "<<tmp << std::endl;
        }
      }
    }

    //With all continuous TSpline3s relevant to this event considered, move
    //on to the TGraphs for continuous parameters.
    for(int i = 0; i < nContXSRTGraphs; i++){
      //We already know that this TGraph is not NULL, otherwise it
      //wouldn't have been saved.  So proceed straight to using it.
      if(fitParameters->fitParametersFast[contXSRTGraphIndices[i]]->decompose != 0){
        pos = fitParameters->fitParametersFast[contXSRTGraphIndices[i]]->currentValueUD;
        tmp = contXSRTGraphs[i]->Eval(pos);
      }
      else{
        pos = fitParameters->fitParametersFast[contXSRTGraphIndices[i]]->currentValue;
        tmp = contXSRTGraphs[i]->Eval(pos);
      }
      if(tmp==tmp){
        totalWeightNoDiscreteParams *= tmp;
        xsecWeightNoDiscreteParams *= tmp;
      }
    }
        
    //With the XSECRESP weights out of the way, now consider the remaining
    //contributions.

    //std::cout << "Response weight " << totalWeight << std::endl;

    //Now multiply in the XSec Functionnal weight, if there is one (i.e. the
    //parameter index value is greater than 0, as it defaults to -1 otherwise.)
    if(xsecFuncParameter>=0){
      //Get position of the first BeRPA parameter
      int BeRPA = fitParameters->GetBeRPA();
      totalWeightNoDiscreteParams *= calcRPA(Q2, fitParameters->fitParametersFast[BeRPA]->currentValue, fitParameters->fitParametersFast[BeRPA+1]->currentValue, fitParameters->fitParametersFast[BeRPA+2]->currentValue, fitParameters->fitParametersFast[BeRPA+3]->currentValue, fitParameters->fitParametersFast[BeRPA+4]->currentValue);
      xsecWeightNoDiscreteParams *= calcRPA(Q2, fitParameters->fitParametersFast[BeRPA]->currentValue, fitParameters->fitParametersFast[BeRPA+1]->currentValue, fitParameters->fitParametersFast[BeRPA+2]->currentValue, fitParameters->fitParametersFast[BeRPA+3]->currentValue, fitParameters->fitParametersFast[BeRPA+4]->currentValue);
      if(fitParameters->fitParametersFast[BeRPA]->paramName != "BeRPA_A" || fitParameters->fitParametersFast[BeRPA+1]->paramName != "BeRPA_B" || fitParameters->fitParametersFast[BeRPA+2]->paramName != "BeRPA_D" || fitParameters->fitParametersFast[BeRPA+3]->paramName != "BeRPA_E"|| fitParameters->fitParametersFast[BeRPA+4]->paramName != "BeRPA_U"){
        std::cout<<"CAUTION !! The hardcoded parameter number for BeRPA in src/BANFEventBase.cxx are wrong, the fit is not working properly "<<std::endl;
        std::cout << "BeRPA replaced by : " << fitParameters->fitParametersFast[BeRPA]->paramName <<" "<< fitParameters->fitParametersFast[BeRPA+1]->paramName << " "<< fitParameters->fitParametersFast[BeRPA+2]->paramName << " "<< fitParameters->fitParametersFast[BeRPA+3]->paramName << " "<< fitParameters->fitParametersFast[BeRPA+4]->paramName<< std::endl;
      }
      //std::cout <<"XsecFuncParameter weight: " << calcRPA(Q2, fitParameters->fitParametersFast[117]->currentValue, fitParameters->fitParametersFast[122]->currentValue, fitParameters->fitParametersFast[123]->currentValue, fitParameters->fitParametersFast[124]->currentValue, fitParameters->fitParametersFast[125]->currentValue) << std::endl;
    }
		
    //If these weight applications have made the weight go negative, set it to
    //zero and stop.
    if(totalWeightNoDiscreteParams < 0.0){
      totalWeightNoDiscreteParams = 0.0;
      totalWeight = 0.0;
      return;
    } 

    //Multiply in the flux systematic weight, if there is one (i.e. the
    //parameter index value is greater than 0, as it defaults to -1 otherwise.)
    if(fluxParameter>=0){
      if(fitParameters->fitParametersFast[fluxParameter]->decompose != 0){
        totalWeightNoDiscreteParams *= fitParameters->fitParametersFast[fluxParameter]->currentValueUD;
      }
      else{
        totalWeightNoDiscreteParams *= fitParameters->fitParametersFast[fluxParameter]->currentValue;
      }
    }

    //std::cout << "Flux weight " << totalWeight << std::endl;

    //Now multiply in the XSec normalization weight, if there is one (i.e. the
    //parameter index vector is not empty, as it defaults to empty otherwise.
    if(xsecNormParameterNumElements > 0){
        //std::cout << "Number of Normalization parameters to apply: " << xsecNormParameterNumElements <<std::endl;
        for(int index = 0; index < xsecNormParameterNumElements; index++){
            totalWeightNoDiscreteParams *= fitParameters->fitParametersFast[xsecNormParameter[index]]->currentValue;
            xsecWeightNoDiscreteParams  *= fitParameters->fitParametersFast[xsecNormParameter[index]]->currentValue;
	    //std::cout << "Norm applied: " << fitParameters->fitParametersFast[xsecNormParameter[index]]->paramName << " " <<fitParameters->fitParametersFast[xsecNormParameter[index]]->currentValue <<std::endl;
        }
    }
    
    //if(debug==0) std::cout << "Norm weight " << totalWeightNoDiscreteParams << std::endl;
    ////Now multiply in the XSec normalization weight, if there is one (i.e. the
    ////parameter index value is greater than 0, as it defaults to -1 otherwise.)
    //if(xsecNormParameter>=0){
    //  //std::cout << "Xsec Norm: " << fitParameters->fitParametersFast[xsecNormParameter]->paramName <<std::endl;
    //  if(fitParameters->fitParametersFast[xsecNormParameter]->paramName == "2p2h_normCtoO"  && NuFlavor > 0 ){
    //    totalWeightNoDiscreteParams *= fitParameters->fitParametersFast[xsecNormParameter]->currentValue * fitParameters->fitParametersFast[xsecNormParameter -2 ]->currentValue;
    //    xsecWeightNoDiscreteParams *= fitParameters->fitParametersFast[xsecNormParameter]->currentValue * fitParameters->fitParametersFast[xsecNormParameter -2 ]->currentValue;
    //    //std::cout << "2p2h_normCtoO: " << fitParameters->fitParametersFast[xsecNormParameter]->paramName <<" "<<fitParameters->fitParametersFast[xsecNormParameter-2]->paramName<<std::endl;
	//			
    //  }
    //  else if(fitParameters->fitParametersFast[xsecNormParameter]->paramName == "2p2h_normCtoO"  && NuFlavor < 0 ){
    //    totalWeightNoDiscreteParams *= fitParameters->fitParametersFast[xsecNormParameter]->currentValue * fitParameters->fitParametersFast[xsecNormParameter -1 ]->currentValue;
    //    xsecWeightNoDiscreteParams *= fitParameters->fitParametersFast[xsecNormParameter]->currentValue * fitParameters->fitParametersFast[xsecNormParameter -1 ]->currentValue;
    //    //std::cout << "2p2h_normCtoO: " << fitParameters->fitParametersFast[xsecNormParameter]->paramName <<" "<<fitParameters->fitParametersFast[xsecNormParameter-1]->paramName<<std::endl;
    //  }
    //  else {
    //    totalWeightNoDiscreteParams *= fitParameters->fitParametersFast[xsecNormParameter]->currentValue;
    //    xsecWeightNoDiscreteParams *= fitParameters->fitParametersFast[xsecNormParameter]->currentValue;
    //    //std::cout << "else: " << fitParameters->fitParametersFast[xsecNormParameter]->paramName <<std::endl;
    //  }
    //}

    //std::cout <<"obsNormParameter, pre if: " << obsNormParameter << std::endl;

    //Now multiply in the observable normalization parameter weight, if there
    //is one (i.e. the parameter index value is greater than 0, since it
    //defaults to -1 otherwise.)
    if(obsNormParameter>=0) {
      if(fitParameters->fitParametersFast[obsNormParameter]->decompose != 0){
        ObsNormWeight = fitParameters->fitParametersFast[obsNormParameter]->currentValueUD;
        totalWeightNoDiscreteParams *= fitParameters->fitParametersFast[obsNormParameter]->currentValueUD;
      }
      else{
        //std::cout << "obsNormParameter, post if: " << obsNormParameter << std::endl;
        ObsNormWeight = fitParameters->fitParametersFast[obsNormParameter]->currentValue;
        totalWeightNoDiscreteParams *= fitParameters->fitParametersFast[obsNormParameter]->currentValue;
      }
    }
  }

  //At this point the total weight with no discrete parameters has been
  //calculated above or is pre-stored from a previous discrete parameter
  //value.  Apply the weight for the appropriate discrete parameter
  //combination to this event.
  totalWeight = totalWeightNoDiscreteParams*(discreteParameterWeights[fitParameters->currentDiscreteParameterCombinationIndex]);
  xsecWeight = xsecWeightNoDiscreteParams*(discreteParameterWeights[fitParameters->currentDiscreteParameterCombinationIndex]);

  if(totalWeight < 0.0){
    totalWeight = 0.0;
    return;
  }
}

void BANFFEventBase::SaveDiscreteParameterWeightsLowMem(FitParameters* fitParameters){

  //Start by setting up the array to store the weights in.
  discreteParameterWeights = new double[fitParameters->nDiscreteParameterCombinations];

  //Set up a temporary variable to store the weight value in.
  double tmp = 1.0;

  //Loop over all the discrete parameter combinations in fitParameters.
  for(int dpi = 0; dpi < fitParameters->nDiscreteParameterCombinations; dpi++){

    //Initialize the discrete parameter weight for this event and
    //combination to 1.0;
    discreteParameterWeights[dpi] = 1.0;

    //Loop over the saved discrete TGraphs relevant to this event.
    for(int i = 0; i < nDiscXSRTGraphs; i++){

      tmp = discXSRTGraphs[i]->Eval(fitParameters->discreteParameterCombinations[dpi][discXSRTGraphIndices[i]]);
            
      if(tmp==tmp){
        discreteParameterWeights[dpi] *= tmp;
      }
    }
    for(int i = 0; i < nDiscXSRTWeights; i++){
      tmp = 1.0;
      if(fitParameters->discreteParameterCombinations[dpi][discXSRTWeightIndices[i]]){
        tmp = discXSRTWeights[i];
      }
      if(tmp==tmp){
        discreteParameterWeights[dpi] *= tmp;
      }
    }
  }
}

void BANFFEventBase::CopySummaryTreeInfo(){

  //The following information needs to be copied across:
  //nuEnergy
  //nuFlavor
  //fluxWeight
  //reactionCode
  //targetMaterial
  //xsecSplines
  //xsecGraphs
  //
  //In each case, if there is a selection result stored, copy that result
  //over.  Otherwise, look for the first sensical nuFlavor entry, then use
  //the corresponding index to copy that over.

  //If there is somehow no truth information, just default to the zeroth
  //entry.
  int sourceIndex = 0;

  //If a sample is matched, use that.
  if(sample != NULL){

    sourceIndex = sample->sampleID;

  }

  //Otherwise, check that the neutrino flavor makes sense (that it is 12, 14,
  //-12, or -14 for an entry, and use the index of the first entry it does.
  else{

    for(int i = 0; i < nSamples; i++){

      bool isNuE = (nuFlavor[i] == 12);
      bool isNuMu = (nuFlavor[i] == 14);
      bool isNuEBar = (nuFlavor[i] == -12);
      bool isNuMuBar = (nuFlavor[i] == -14);

      if (isNuE || isNuMu || isNuEBar || isNuMuBar){

        sourceIndex = i;
        break;
      }
    }


  }

  //Now, for each of the variables listed above, copy the content from
  //sourceIndex to every other index.
  for(int i = 0; i < nSamples; i++){

    //nuEnergy
    //nuFlavor
    //reactionCode
    //targetMaterial
    //xsecSplines (2D in [numbXsecSysts][nSamples])
    //xsecGraphs (ditto)

    nuEnergy[i] = nuEnergy[sourceIndex];
    nuFlavor[i] = nuFlavor[sourceIndex];
    reactionCode[i] = reactionCode[sourceIndex];
    targetMaterial[i] = targetMaterial[sourceIndex];

    //Loop over numbXsecSysts.  Copy the content from [index][sourceIndex]
    //into [index][i]
    for(int j = 0; j < numbXsecSysts; j++){

      xsecSplines[j][i] = xsecSplines[j][sourceIndex];
      xsecGraphs[j][i] = xsecGraphs[j][sourceIndex];

    }
  }
}

double const BANFFEventBase::calcRPA(double Q2, double A, double B, double D, double E, double U) {
  // Callum's eye-balled nominals
  // A = 0.6
  // B = 1.0
  // D = 1.2
  // E = 1.0
  // U = 1.2
  // 
  // Callum's fitted nominals
  // A = 0.59 +/- 20%
  // B = 1.05 +/- 20%
  // D = 1.13 +/- 15%
  // E = 0.88 +/- 40%
  // U = 1.2

  // Kept for convenience
  double eRPA = 1.;
 
  // Q2 transition; less than U -> polynominal
  if (Q2 < U) {
    // xprime as prescribed by Callum
    double xprime = Q2/U;
    double C      = D + U*E*(D-1)/3.;
    eRPA          = A*(1-xprime)*(1-xprime)*(1-xprime) + 3*B*(1-xprime)*(1-xprime)*xprime + 3*C*(1-xprime)*xprime*xprime + D*xprime*xprime*xprime;
  } else {
    eRPA = 1 + (D-1)*exp(-E*(Q2-U));
  }

  return eRPA;
}
