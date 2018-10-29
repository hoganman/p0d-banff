#include "BANFFBinnedSample.hxx"
#include "BANFFBinnedSampleUtils.hxx"
#include "psycheInterface/psycheBANFFEvent.hxx"
#include "SampleId.hxx"
#include "BinningDefinition.hxx"
#include "TRandom.h"
#include <sstream>
#include <iomanip>

BANFFBinnedSample::BANFFBinnedSample(SampleId::SampleEnum sampleIDInput, int nObsInput,
                                     BANFFObservableBase** obsInput,
                                     bool ithrowMCStat, bool ithrowStat, bool ithrowMCStatPoisson):
  BANFFSampleBase("none", (int)sampleIDInput, nObsInput, obsInput, ithrowMCStat, ithrowStat, ithrowMCStatPoisson){

  name = SampleId::ConvertSample(sampleIDInput);
  std::replace (name.begin(), name.end(), ' ', '_');
  BANFF::TAxis2D* ax2d = BANFF::BinningDefinition::Get().GetBinningArray(sampleIDInput);
  axes = new TAxis*[2];
  axes[0] = (*ax2d)[0];
  axes[1] = (*ax2d)[1];
  
  //Set up the array to store the reaction code breakdowns.  Make it size
  //200, and fill it with NULL entries.  We will only actually create
  //histograms as we need to fill them.
  rxnPredMC = new THnD*[200];

  for(int i = 0; i < 200; i++){
    rxnPredMC[i] = NULL;
  }

  //Set up the array to store the reaction code energy breakdowns.  Make it size
  //200, and fill it with NULL entries.  We will only actually create
  //histograms as we need to fill them.
  rxnPredMCTrueE = new THnD*[200];

  for(int i = 0; i < 200; i++){
    rxnPredMCTrueE[i] = NULL;
  }

  //The observableInformationSaved variable needs to default to false.
  //If the "save observable information" method gets called, it will be
  //set to true.
  observableInformationSaved = false;

  //Initialize the histograms to have the dimension of nObs, and uniform
  //binning, since the contructor does not allow anything else.  Also,
  //changing the axes later does not affect the array that the bins are
  //stored in, so the number of bins for each dimension has to match the desired axis number of bins to start.

  int* nBinsInitial = new int[nObs];
  double *xMin = new double[nObs];
  double *xMax = new double[nObs];

  for(int i = 0; i < nObs; i++){
    nBinsInitial[i] = axes[i]->GetNbins();
    xMin[i] = 0.0;
    xMax[i] = 1.0;
  }


  data   = new THnD("data",   "data",   nObs, nBinsInitial, xMin, xMax);
  nomMC  = new THnD("nomMC",  "nomMC",  nObs, nBinsInitial, xMin, xMax);
  predMC = new THnD("predMC", "predMC", nObs, nBinsInitial, xMin, xMax);

  //Fill the diagnostic histograms.
  nomMCNoWeights      = new THnD("nomMCNoWeights",      "nomMCNoWeights",      nObs, nBinsInitial, xMin, xMax);
  nomMCPOTWeights     = new THnD("nomMCPOTWeights",     "nomMCPOTWeights",     nObs, nBinsInitial, xMin, xMax);
  nomMCNomFluxWeights = new THnD("nomMCNomFluxWeights", "nomMCNomFluxWeights", nObs, nBinsInitial, xMin, xMax);
  nomMCNomXSecWeights = new THnD("nomMCNomXSecWeights", "nomMCNomXSecWeights", nObs, nBinsInitial, xMin, xMax);
  nomMCNomDetWeights  = new THnD("nomMCNomDetWeights",  "nomMCNomDetWeights",  nObs, nBinsInitial, xMin, xMax);
  nomMCNomCovWeights  = new THnD("nomMCNomCovWeights",  "nomMCNomCovWeights",  nObs, nBinsInitial, xMin, xMax);
  nomMCNomFluxAndXSecWeights = new THnD("nomMCNomFluxAndXSecWeights","nomMCNomFluxAndXSecWeights", nObs, nBinsInitial, xMin, xMax);
  nomMCNomFluxAndDetWeights  = new THnD("nomMCNomFluxAndDetWeights", "nomMCNomFluxAndDetWeights",  nObs, nBinsInitial, xMin, xMax);

  std::string nameString = name + "_nominalFluxWeights";
  nominalFluxWeights = new TH1D(nameString.c_str(),nameString.c_str(),100,0.0,10.0);
  nameString = name + "_nominalXSecWeights";
  nominalXSecWeights = new TH1D(nameString.c_str(),nameString.c_str(),100,0.0,10.0);
  nameString = name + "_nominalDetWeights";
  nominalDetWeights = new TH1D(nameString.c_str(),nameString.c_str(),100,0.0,10.0);
  nameString = name + "_nominalCovWeights";
  nominalCovWeights = new TH1D(nameString.c_str(),nameString.c_str(),100,0.0,10.0);
    
  //Change the histogram binning from the default NDimensional binning to the
  //axes supplied by the user.
  for(int i = 0; i < nObs; i++){ 
    if(!axes[i]){
      std::cerr << "Binning for " << name << " observable " << i << " isnt defined!" << std::endl;
      throw;
    }
    data                      ->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());
    nomMC                     ->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());
    predMC                    ->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());
    nomMCNoWeights            ->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());
    nomMCPOTWeights           ->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());
    nomMCNomFluxWeights       ->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());
    nomMCNomFluxAndXSecWeights->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());
    nomMCNomFluxAndDetWeights ->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());
    nomMCNomXSecWeights       ->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());
    nomMCNomDetWeights        ->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());
    nomMCNomCovWeights        ->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());

  }


  //Now, get the information on the number of bins and the indices of the
  //non-under/overflow bins so the histogram bins can be looped through
  //easily later. (It is the same for all 3 histograms.)
  nBins = BANFFBinnedSampleUtils::GetNBins(data);
  bins  = BANFFBinnedSampleUtils::GetBins(data);

  //Delete the unneeded initialization arrays.
  delete[] nBinsInitial;
  delete[] xMin;
  delete[] xMax;


}

BANFFBinnedSample::BANFFBinnedSample(std::string nameInput, int sampleIDInput, int nObsInput, BANFFObservableBase** obsInput, TAxis** axesInput, bool ithrowMCStat, bool ithrowStat, bool ithrowMCStatPoisson)
  : BANFFSampleBase(nameInput, sampleIDInput, nObsInput, obsInput, ithrowMCStat, ithrowStat, ithrowMCStatPoisson){

  axes = axesInput;

  //Set up the array to store the reaction code breakdowns.  Make it size
  //200, and fill it with NULL entries.  We will only actually create
  //histograms as we need to fill them.
  rxnPredMC = new THnD*[200];

  for(int i = 0; i < 200; i++){
    rxnPredMC[i] = NULL;
  }

  //Set up the array to store the reaction code energy breakdowns.  Make it size
  //200, and fill it with NULL entries.  We will only actually create
  //histograms as we need to fill them.
  rxnPredMCTrueE = new THnD*[200];

  for(int i = 0; i < 200; i++){
    rxnPredMCTrueE[i] = NULL;
  }

  //The observableInformationSaved variable needs to default to false.
  //If the "save observable information" method gets called, it will be
  //set to true.
  observableInformationSaved = false;

  //Initialize the histograms to have the dimension of nObs, and uniform
  //binning, since the contructor does not allow anything else.  Also,
  //changing the axes later does not affect the array that the bins are
  //stored in, so the number of bins for each dimension has to match the desired axis number of bins to start.

  int* nBinsInitial = new int[nObs];
  double *xMin = new double[nObs];
  double *xMax = new double[nObs];

  for(int i = 0; i < nObs; i++){

    nBinsInitial[i] = axes[i]->GetNbins();
    xMin[i] = 0.0;
    xMax[i] = 1.0;
  }


  data   = new THnD("data",  "data",   nObs, nBinsInitial, xMin, xMax);
  nomMC  = new THnD("nomMC", "nomMC",  nObs, nBinsInitial, xMin, xMax);
  predMC = new THnD("predMC","predMC", nObs, nBinsInitial, xMin, xMax);

  //Fill the diagnostic histograms.
  nomMCNoWeights             = new THnD("nomMCNoWeights",             "nomMCNoWeights",             nObs, nBinsInitial, xMin, xMax);
  nomMCPOTWeights            = new THnD("nomMCPOTWeights",            "nomMCPOTWeights",            nObs, nBinsInitial, xMin, xMax);
  nomMCNomFluxWeights        = new THnD("nomMCNomFluxWeights",        "nomMCNomFluxWeights",        nObs, nBinsInitial, xMin, xMax);
  nomMCNomXSecWeights        = new THnD("nomMCNomXSecWeights",        "nomMCNomXSecWeights",        nObs, nBinsInitial, xMin, xMax);
  nomMCNomDetWeights         = new THnD("nomMCNomDetWeights",         "nomMCNomDetWeights",         nObs, nBinsInitial, xMin, xMax);
  nomMCNomCovWeights         = new THnD("nomMCNomCovWeights",         "nomMCNomCovWeights",         nObs, nBinsInitial, xMin, xMax);
  nomMCNomFluxAndXSecWeights = new THnD("nomMCNomFluxAndXSecWeights", "nomMCNomFluxAndXSecWeights", nObs, nBinsInitial, xMin, xMax);
  nomMCNomFluxAndDetWeights  = new THnD("nomMCNomFluxAndDetWeights",  "nomMCNomFluxAndDetWeights",  nObs, nBinsInitial, xMin, xMax);
  std::string nameString = nameInput + "_nominalFluxWeights";
  nominalFluxWeights = new TH1D(nameString.c_str(),nameString.c_str(),100,0.0,10.0);
  nameString = nameInput + "_nominalXSecWeights";
  nominalXSecWeights = new TH1D(nameString.c_str(),nameString.c_str(),100,0.0,10.0);
  nameString = nameInput + "_nominalDetWeights";
  nominalDetWeights  = new TH1D(nameString.c_str(),nameString.c_str(),100,0.0,10.0);
  nameString = name + "_nominalCovWeights";
  nominalCovWeights = new TH1D(nameString.c_str(),nameString.c_str(),100,0.0,10.0);
  
  //Change the histogram binning from the default NDimensional binning to the
  //axes supplied by the user.
  for(int i = 0; i < nObs; i++){
    data  ->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());
    nomMC ->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());
    predMC->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());

    nomMCNoWeights            ->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());
    nomMCPOTWeights           ->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());
    nomMCNomFluxWeights       ->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());
    nomMCNomFluxAndXSecWeights->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());
    nomMCNomFluxAndDetWeights ->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());
    nomMCNomXSecWeights       ->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());
    nomMCNomDetWeights        ->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());
    nomMCNomCovWeights        ->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());
    std::cout << "Observable " << i << std::endl;
    axes[i]->Dump();

  }


  //Now, get the information on the number of bins and the indices of the
  //non-under/overflow bins so the histogram bins can be looped through
  //easily later. (It is the same for all 3 histograms.)
  nBins = BANFFBinnedSampleUtils::GetNBins(data);
  bins = BANFFBinnedSampleUtils::GetBins(data);

  //Delete the unneeded initialization arrays.
  delete[] nBinsInitial;
  delete[] xMin;
  delete[] xMax;


}


void BANFFBinnedSample::AddDataEvent(BANFFEventBase* event){

  //If the observable bin information is saved, use that.
  //Also need to increment the histogram entries counter manually when do
  //this.
  if(event->observableBin != -1){

    data->SetBinContent(event->observableBin, data->GetBinContent(event->observableBin) + 1.0);
    data->SetEntries(data->GetEntries() + 1);

  }

  //Otherwise, make use of the variable information that is stored.
  else{
    //Create the array to feed to THnBase Fill method.
    double* obsVals = new double[nObs];

    //Fill this array for each observable.
    for(int i = 0; i < nObs; i++){

      obsVals[i] = obs[i]->GetCurrentValue(event);

    }

    //Feed the array of observables into the data histogram. No weight is
    //applied to data.
    data->Fill(obsVals);

    //Delete the observable array now that we're done with it.
    delete[] obsVals;
  }
}

void BANFFBinnedSample::LoadDataFromFile(TFile* dataFile, std::string dataName){

  THnD* dataFromFile = (THnD*)(dataFile->Get(dataName.c_str()));

  //Check that the number of bins for this sample matches that of the
  //dataFromFile histogram.
  if(BANFFBinnedSampleUtils::GetNBins(dataFromFile) != nBins){

    std::cout << "Data in file does not match binning of sample.  Abort." << std::endl;
    exit(1);
  }

  else{
    //Otherwise, clone the data from the file into the data histogram for
    //this sample.
    data = (THnD*)(dataFromFile->Clone());
  }
}

void BANFFBinnedSample::UseNominalMCAsData(){


  data = (THnD*)(nomMC->Clone());


}

void BANFFBinnedSample::SaveData(std::string dataName){

  data->Write(dataName.c_str());

}


void BANFFBinnedSample::AddNominalMCEvent(BANFFEventBase* event){

  //If we have pre-saved information for the observable bin to use, use that
  //to fill the histogram.
  //Also, need to increment the entries counter for the histogram for each
  //event we add.
  if(event->observableBin != -1){

    nomMC->SetBinContent(event->observableBin, nomMC->GetBinContent(event->observableBin) + event->GetTotalWeight());
    nomMC->SetEntries(nomMC->GetEntries() + 1);

    //Fill the diagnostic histograms.
    nomMCNoWeights->SetBinContent(event->observableBin, nomMCNoWeights->GetBinContent(event->observableBin) + 1.0);
    nomMCNoWeights->SetEntries(nomMCNoWeights->GetEntries() + 1);

    nomMCPOTWeights->SetBinContent(event->observableBin, nomMCPOTWeights->GetBinContent(event->observableBin) + event->GetPOTWeight());
    nomMCPOTWeights->SetEntries(nomMCPOTWeights->GetEntries() + 1);

    nomMCNomFluxWeights->SetBinContent(event->observableBin, nomMCNomFluxWeights->GetBinContent(event->observableBin) + (event->GetPOTWeight())*(event->GetFluxWeight()));
    nomMCNomFluxWeights->SetEntries(nomMCNomFluxWeights->GetEntries() + 1);

    nomMCNomXSecWeights->SetBinContent(event->observableBin, nomMCNomXSecWeights->GetBinContent(event->observableBin) + (event->GetPOTWeight())*(event->GetXsecWeight()));
    nomMCNomXSecWeights->SetEntries(nomMCNomXSecWeights->GetEntries() + 1);

    nomMCNomDetWeights->SetBinContent(event->observableBin, nomMCNomDetWeights->GetBinContent(event->observableBin) + (event->GetPOTWeight())*(event->GetDetWeight()));
    nomMCNomDetWeights->SetEntries(nomMCNomDetWeights->GetEntries() + 1);

    nomMCNomCovWeights->SetBinContent(event->observableBin, nomMCNomCovWeights->GetBinContent(event->observableBin) + (event->GetPOTWeight())*(event->GetObservableWeight()));
    nomMCNomCovWeights->SetEntries(nomMCNomDetWeights->GetEntries() + 1);
    
    //The combination plots.
    nomMCNomFluxAndXSecWeights->SetBinContent(event->observableBin, nomMCNomFluxAndXSecWeights->GetBinContent(event->observableBin) + (event->GetPOTWeight())*(event->GetFluxWeight())*(event->GetXsecWeight()));
    nomMCNomFluxAndXSecWeights->SetEntries(nomMCNomFluxAndXSecWeights->GetEntries() + 1);

    nomMCNomFluxAndDetWeights->SetBinContent(event->observableBin, nomMCNomFluxAndDetWeights->GetBinContent(event->observableBin) + (event->GetPOTWeight())*(event->GetFluxWeight())*(event->GetDetWeight()));
    nomMCNomFluxAndDetWeights->SetEntries(nomMCNomFluxAndDetWeights->GetEntries() + 1);
#ifdef DEBUGWEIGHTS
#pragma message "DEBUGGING WEIGHT, couting all the weights"
    std::cout << "POTWeight     " << event->GetPOTWeight()     << std::endl;
    std::cout << "FluxWeight    " << event->GetFluxWeight()    << std::endl;
    std::cout << "XsecWeight    " << event->GetXsecWeight()    << std::endl;
    std::cout << "DetWeight     " << event->GetDetWeight()     << std::endl;
    std::cout << "NuEnergy      " << event->GetNuEnergy()      << std::endl;
    std::cout << "ReactionCode  " << event->GetReactionCode()  << std::endl;
    std::cout << "ObservableBin " << event->GetObservableBin() << std::endl;
    std::cout << "ObservableWeight " << event->GetObservableWeight() << std::endl;
#endif 
  }else{//Otherwise, use the full treatment.
    
    //Create the array to feed to THnBase Fill method.
    double* obsVals = new double[nObs];

    //Fill this array for each observable.
    for(int i = 0; i < nObs; i++){

      obsVals[i] = obs[i]->GetCurrentValue(event);

    }

    //Feed the array of observables into the data histogram.  Allow for a
    //weight.
    nomMC->Fill(obsVals, event->GetTotalWeight());

    //Fill the diagnostic histograms.
    nomMCNoWeights->Fill(obsVals,1.0);
    nomMCPOTWeights->Fill(obsVals,event->GetPOTWeight());
    nomMCNomFluxWeights->Fill(obsVals,(event->GetPOTWeight())*(event->GetFluxWeight()));
    nomMCNomXSecWeights->Fill(obsVals,(event->GetPOTWeight())*(event->GetXsecWeight()));
    nomMCNomDetWeights ->Fill(obsVals,(event->GetPOTWeight())*(event->GetDetWeight ()));

    //The combination plots.
    nomMCNomFluxAndXSecWeights->Fill(obsVals,(event->GetPOTWeight())*(event->GetFluxWeight())*(event->GetXsecWeight()));
    nomMCNomFluxAndDetWeights ->Fill(obsVals,(event->GetPOTWeight())*(event->GetFluxWeight())*(event->GetDetWeight ()));

    //Delete the observable array now that we're done with it.
    delete[] obsVals;
  }

  nominalFluxWeights->Fill(event->GetFluxWeight      ());
  nominalXSecWeights->Fill(event->GetXsecWeight      ());
  nominalDetWeights ->Fill(event->GetDetWeight       ());
  nominalCovWeights ->Fill(event->GetObservableWeight());


}

void BANFFBinnedSample::AddPredictedMCEvent(BANFFEventBase* event){

  //If we have pre-saved information for the observable bin to use, use that
  //to fill the histogram.
  //NB: Although we should also increment the entries counter, we only really
  //care about it if saving the histogram, so don't want to waste time on
  //doing it here.  So, the number of entries just gets set to that of the
  //nominal MC histogram, right before the predicted MC is saved.
  if(event->observableBin != -1){

    if(throwMCStatPoisson){
      double thisEventContrib = ((double)(gRandom->Poisson(1)))*(event->GetTotalWeight());
      predMC->SetBinContent(event->observableBin, predMC->GetBinContent(event->observableBin) + thisEventContrib);
    }
    else{
      double weight = event->GetTotalWeight();
      //The bin error is only relevant if we're throwing MC stat using
      //the Gaussian instead of individual Poisson throws.
      predMC->SetBinContent(event->observableBin, predMC->GetBinContent(event->observableBin) + weight);
      predMC->SetBinError(event->observableBin, predMC->GetBinError(event->observableBin) + weight*weight);
    }
  }

  //Otherwise, go through the procedure of getting observable values and
  //using them.
  else{
    //Create the array to feed to THnBase Fill method.
    double* obsVals = new double[nObs];

    //Fill this array for each observable.
    for(int i = 0; i < nObs; i++){

      obsVals[i] = obs[i]->GetCurrentValue(event);

    }

    //Feed the array of observables into the data histogram.  Allow for a
    //weight.
    //NB: We now increment the content manually below for manual error
    //calculation purposes.
    //This way the incrementing of the 
    //predMC->Fill(obsVals, event->GetTotalWeight());

    //Get the bin that this set of observables corresponds to.
    int currentPredMCBin = predMC->GetBin(obsVals);

    if(throwMCStatPoisson){
      double thisEventContrib = ((double)(gRandom->Poisson(1)))*(event->GetTotalWeight());
      predMC->SetBinContent(currentPredMCBin, predMC->GetBinContent(currentPredMCBin) + thisEventContrib);
            
      //NB: Only need to set the bin error if we're using the other
      //method of throwing MC stat, which we will not be doing if we are
      //throwing them this way.
        
    }


    else{
      //Manually increment the bin content, as if we use "Fill" it will do
      //something automatically and mess up the calculation.
      predMC->SetBinContent(currentPredMCBin, predMC->GetBinContent(currentPredMCBin) + event->GetTotalWeight());

      //Manually set the error in this bin to the sum of the previous error
      //plus the recent addition squared.
      predMC->SetBinError(currentPredMCBin, predMC->GetBinError(currentPredMCBin) + pow(event->GetTotalWeight(),2));
    }

    //If we're using this method of filling the histogram, it means we can
    //afford to be a bit slower, and the number of entries may not be the
    //same as for nominal MC.  So increment the bin counter here.
    predMC->SetEntries(predMC->GetEntries() + 1);

    //Delete the observable array now that we're done with it.
    delete[] obsVals;
  }

}

void BANFFBinnedSample::AddMCEventToReactionCodeBreakdown(BANFFEventBase* event){


  //First, check the event's reaction code and see whether a histogram exists
  //to put the event into.  If the histogram does not exist, make it.
  int evtRxnCode = event->GetReactionCode();

  //The sand MC has a reaction code of -999, which breaks this.  So, just
  //ignore it and any other non-sensical reaction code.
  if((evtRxnCode < -99)
     || (evtRxnCode > 99)){
    return;
  }

  //Set up a variable to store the reaction code bin, which is 100 + the
  //reaction code.
  int rcb = evtRxnCode + 100;

  //Check whether a plot exists at that index.  If it does not, create one.
  if(rxnPredMC[rcb] == NULL){

    int* nBinsRxn = new int[nObs];
    double *xMin = new double[nObs];
    double *xMax = new double[nObs];

    for(int i = 0; i < nObs; i++){

      nBinsRxn[i] = axes[i]->GetNbins();
      xMin[i] = 0.0;
      xMax[i] = 1.0;
    }

    rxnPredMC[rcb] = new THnD(Form("rxnPredMC_%d",rcb),Form("rxnPredMC_%d",rcb), nObs, nBinsRxn, xMin, xMax);

    //Change the histogram binning from the default NDimensional binning to the
    //axes supplied by the user.
    for(int i = 0; i < nObs; i++){
      rxnPredMC[rcb]->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());

    }

    //Delete the unneeded initialization arrays.
    delete[] nBinsRxn;
    delete[] xMin;
    delete[] xMax;

  }

  //By the time we get here, the histogram exists.  So fill it appropriately.

  //If we have pre-saved information for the observable bin to use, use that
  //to fill the histogram.
  //NB: Although we should also increment the entries counter, we only really
  //care about it if saving the histogram, so don't want to waste time on
  //doing it here.  So, the number of entries just gets set to that of the
  //nominal MC histogram, right before the predicted MC is saved.
  if(event->observableBin != -1){

    rxnPredMC[rcb]->SetBinContent(event->observableBin, rxnPredMC[rcb]->GetBinContent(event->observableBin) + event->GetTotalWeight());
    rxnPredMC[rcb]->SetEntries(rxnPredMC[rcb]->GetEntries() + 1);

  }

  //Otherwise, go through the procedure of getting observable values and
  //using them.
  else{
    //Create the array to feed to THnBase Fill method.
    double* obsVals = new double[nObs];

    //Fill this array for each observable.
    for(int i = 0; i < nObs; i++){

      obsVals[i] = obs[i]->GetCurrentValue(event);

    }

    //Feed the array of observables into the data histogram.  Allow for a
    //weight.
    rxnPredMC[rcb]->Fill(obsVals, event->GetTotalWeight());

    //Delete the observable array now that we're done with it.
    delete[] obsVals;
  }

}

void BANFFBinnedSample::AddMCEventToReactionCodeTrueEnergyBreakdown(BANFFEventBase* event){


  //First, check the event's reaction code and see whether a histogram exists
  //to put the event into.  If the histogram does not exist, make it.
  int evtRxnCode = event->GetReactionCode();

  //The sand MC has a reaction code of -999, which breaks this.  So, just
  //ignore it and any other non-sensical reaction code.
  if((evtRxnCode < -99) || (evtRxnCode > 99)){
    return;
  }

  //Set up a variable to store the reaction code bin, which is 100 + the
  //reaction code.
  int rcb = evtRxnCode + 100;

  //Check whether a plot exists at that index.  If it does not, create one.
  if(rxnPredMCTrueE[rcb] == NULL){

    int     nEnergy  = 1;
    int    *nBinsRxn = new int[nEnergy];
    double *xMin     = new double[nEnergy];
    double *xMax     = new double[nEnergy];

    for(int i = 0; i < nEnergy; i++){

      nBinsRxn[i] = 100;
      xMin[i] = 0.0;
      xMax[i] = 10.0;
    }

    rxnPredMCTrueE[rcb] = new THnD(Form("rxnPredMCTrueE_%d",rcb),Form("rxnPredMCTrueE_%d",rcb), nEnergy, nBinsRxn, xMin, xMax);

    //Delete the unneeded initialization arrays.
    delete[] nBinsRxn;
    delete[] xMin;
    delete[] xMax;

  }

  //In order to find the correct bin, set up the x axis from rxnPredMCTrueE
  TAxis *rxnAxis = rxnPredMCTrueE[rcb]->GetAxis(0);
  //std::cout << "Axis is: " << rxnAxis <<std::endl;

  //Find bin that the neutrino energy falls in
  int trueEBin = rxnAxis->FindBin(event->GetNuEnergy());

  //By the time we get here, the histogram exists.  So fill it appropriately.
  //If we have pre-saved information for the observable bin to use, use that
  //to fill the histogram.
  //NB: Although we should also increment the entries counter, we only really
  //care about it if saving the histogram, so don't want to waste time on
  //doing it here.  So, the number of entries just gets set to that of the
  //nominal MC histogram, right before the predicted MC is saved.
  if(event->observableBin != -1){
    rxnPredMCTrueE[rcb]->SetBinContent(trueEBin, rxnPredMCTrueE[rcb]->GetBinContent(trueEBin) + event->GetTotalWeight());
    rxnPredMCTrueE[rcb]->SetEntries(rxnPredMCTrueE[rcb]->GetEntries() + 1);
  }

}


void BANFFBinnedSample::SaveNominalMC(std::string nomMCName){

  //Write out the histogram.
  nomMC->Write(nomMCName.c_str());

  nomMCNoWeights     ->Write((nomMCName + "_withNoWeights"     ).c_str());
  nomMCPOTWeights    ->Write((nomMCName + "_withPOTWeights"    ).c_str());
  nomMCNomFluxWeights->Write((nomMCName + "_withNomFluxWeights").c_str());
  nomMCNomXSecWeights->Write((nomMCName + "_withNomXSecWeights").c_str());
  nomMCNomDetWeights ->Write((nomMCName + "_withNomDetWeights" ).c_str());
  nomMCNomCovWeights ->Write((nomMCName + "_withNomCovWeights" ).c_str());
  nomMCNomFluxAndXSecWeights->Write((nomMCName + "_withNomFluxAndXSecWeights").c_str());
  nomMCNomFluxAndDetWeights ->Write((nomMCName + "_withNomFluxAndDetWeights" ).c_str());

  nominalFluxWeights->Write((nomMCName + "_nominalFluxWeights").c_str());
  nominalXSecWeights->Write((nomMCName + "_nominalXSecWeights").c_str());
  nominalDetWeights ->Write((nomMCName + "_nominalDetWeights" ).c_str());
  nominalCovWeights ->Write((nomMCName + "_nominalCovWeights" ).c_str());
  //Take this opportunity to also write out the axes for this sample.
  for(int i = 0; i < nObs; i++){

    //Format expected when dealing with ObsNorm parameters is
    //sampleName_obsName_axis.
    std::string axisName = name + "_" + obs[i]->name + "_axis";
    axes[i]->Write(axisName.c_str());

  }

}


void BANFFBinnedSample::FinalizeMCPrediction(){

  double bin_content = 0.0;
  double bin_error = 0.0;

  for(int i = 0; i < nBins; i++){

    predMC->SetBinError(bins[i],sqrt(predMC->GetBinError(bins[i])));

    //If we're throwing statistical or MC statistical errors,
    //alter the bin content here now that the error has been
    //determined.
    if(throwStat || throwMCStat){

      if(throwMCStat){
        bin_content = predMC->GetBinContent(bins[i]);
        bin_error = predMC->GetBinError(bins[i]);

        predMC->SetBinContent(bins[i], gRandom->Gaus(bin_content,bin_error));
      }
      if(throwStat){

        bin_content = predMC->GetBinContent(bins[i]);

        predMC->SetBinContent(bins[i], gRandom->Poisson( bin_content ));
      }
    }
  }
}

void BANFFBinnedSample::SaveMCPrediction(std::string predMCName){

  //For some reason THn objects don't have their number of entries counter
  //incremented when SetBinContent() is used to fill them, as is done if
  //observable information has been pre-saved.  Although this doesn't affect
  //the operation of the fit, if you try to draw the histogram it will appear
  //to be empty.  So, in this case set the number of entries in the predMC histogram to
  //equal the number of entries in the nomMC histogram (since they should be
  //equal.)
  if(observableInformationSaved){

    predMC->SetEntries(nomMC->GetEntries());
  }

  predMC->Write(predMCName.c_str());

}


void BANFFBinnedSample::ResetMCPrediction(){


  if(predMC != NULL){


    //Need to delete the histogram completely and then start from scratch,
    //otherwise the Entries counter keeps going up.
    predMC->Delete();

    int* nBinsReset = new int[nObs];
    double *xMin = new double[nObs];
    double *xMax = new double[nObs];

    for(int i = 0; i < nObs; i++){

      nBinsReset[i] = axes[i]->GetNbins();
      xMin[i] = 0.0;
      xMax[i] = 1.0;
    }

    predMC = new THnD("predMC","predMC", nObs, nBinsReset, xMin, xMax);

    //Change the histogram binning from the default NDimensional binning to the
    //axes supplied by the user.
    for(int i = 0; i < nObs; i++){
      predMC->GetAxis(i)->Set(axes[i]->GetNbins(),axes[i]->GetXbins()->GetArray());
    }

    //Delete the unneeded initialization arrays.
    delete[] nBinsReset;
    delete[] xMin;
    delete[] xMax;

  }

}



double BANFFBinnedSample::CalcLLRContrib(){

  double chisq = 0.0;
  double dataVal, predVal;

  //Loop over all the bins one by one using their unique bin index.
  //Use the stored nBins value and bins array so avoid trying to calculate
  //over underflow or overflow bins.
  for(int i = 0; i < nBins; i++){

    dataVal = data->GetBinContent(bins[i]);
    predVal = predMC->GetBinContent(bins[i]);

    if((predVal > 0.0) && (dataVal > 0.0)){

      chisq += 2.0*(predVal - dataVal +dataVal*TMath::Log( dataVal/predVal) );

    }

    else if(predVal > 0.0){

      chisq += 2.0*predVal;

    }

    if(isinf(chisq)){
      std::cout << "Infinite chi2 " << predVal << " " << dataVal
                << " " << nomMC->GetBinContent(bins[i]) << " "
                << predMC->GetBinError(bins[i]) << " "
                << predMC->GetBinContent(bins[i]) << std::endl;
    }
  }

  return chisq;

}

void BANFFBinnedSample::SaveObservableInformation(BANFFEventBase* event){

  //Create the array to feed to THnBase GetBin method.
  double* obsVals = new double[nObs];

  //Fill this array for each observable.
  for(int i = 0; i < nObs; i++){

    obsVals[i] = obs[i]->GetCurrentValue(event);

  }

  int bin = predMC->GetBin(obsVals);

  event->observableBin = bin;

  //Now delete the obsVals array since it is no longer needed.
  delete[] obsVals;

  //Set the bool that keeps track of this function having been called to
  //true.
  observableInformationSaved = true;

}

void BANFFBinnedSample::SaveReactionCodeBreakdown(std::string nameBase){


  //Loop through the array of reaction code breakdowns and save the non-NULL
  //ones to file.
  for(int i = 0; i < 200; i++){

    if(rxnPredMC[i] != NULL){

      std::stringstream outName;
      outName << nameBase << "_rxnPredMC_" << i;

      std::string outNameString(outName.str());

      //Write out the histogram.
      rxnPredMC[i]->Write(outNameString.c_str());
    }

    if(rxnPredMCTrueE[i] != NULL){

      std::stringstream outNameTrueE;
      outNameTrueE << nameBase << "_rxnPredMCTrueE_" << i;

      std::string outNameTrueEString(outNameTrueE.str());

      //Write out the histogram.
      rxnPredMCTrueE[i]->Write(outNameTrueEString.c_str());
    }
  }
}



