#include <AnalysisUtils.hxx>
#include "psycheInterface.hxx"
#include "psycheBANFFEvent.hxx"
#include "ToyMaker.hxx"

class NominalToyMaker
	: public ToyMaker
{
public:
    NominalToyMaker(void)
	: _sigma(0.0), // Fixed sigma of 0 for all systematics in the toy
	  _weight(1.0)
    {
    }
    void FillToyExperiment(ToyExperiment& toy)
    {

	for ( UInt_t isyst = 0; isyst<NMAXSYSTEMATICS; isyst++ ) {
	    SystematicBase* syst = _systematics[isyst];
	    if (!syst) continue;

	    // Create the proper structure for the ToyExperiment adding each of the systematics
	    toy.AddToyVariation(syst->GetIndex(), syst->GetNParameters());

	    // Loop over parameters for this systematic
	    for (UInt_t ipar = 0; ipar<syst->GetNParameters(); ipar++ ) {
		toy.SetToyVariation(isyst,ipar,_sigma,_weight);
	    }
	}

    }
private:
    Float_t _sigma;
    Float_t _weight;
};

psycheInterface::psycheInterface(int nSamp, BANFFSampleBase** samp,
				 FitParameters* fitParms, bool loadDetParms,
				 bool varyDetParms, std::string diagnostic_name)
    : BANFFInterfaceBase(nSamp,samp,fitParms,diagnostic_name)
{
    //Create the Analysis Manager.
    _man = new AnalysisManager();


    _man->sel().DumpSelections();

    //If loading detector parameters was requested, do so now.
    if ( loadDetParms ) {
        LoadDetectorParameters(varyDetParms);
    }

    //At this point in time we can print out the parameter information for
    //debugging, since all parameters will have been loaded by here.
    //fitParms->PrintAllParameterInfo();

    //Load the events.  Everything else takes care of the rest.
    LoadEvents();
}

psycheInterface::~psycheInterface(){

  if(_man) delete _man;
  if(_toyMaker) delete _toyMaker;

  for(int i = 0; i < nSamples; i++)
    if(samples[i])
      delete samples[i];

  for(int i = 0; i < nDataEvents; i++)
    if(dataEvents[i])
      delete dataEvents[i];


  for(int i = 0; i < nMCEvents; i++)
    if(mcEvents[i])
      delete mcEvents[i];

  delete fitParameters;
  delete sumTree;
  dataFiles.clear();
  mcFiles.clear();
}

void psycheInterface::LoadEvents()
{
    //Set up the Experiment.
    Experiment* exp = new Experiment("nd280");

    //Double check that the same number of data and mc run periods exist.
    if( dataFiles.size() != mcFiles.size() ) {
      for(std::map<std::string,std::vector<std::string> >::iterator it = dataFiles.begin(); it != dataFiles.end(); it++)
	std::cout << "dataFiles[" << "] " << it->first << std::endl;
      for(std::map<std::string, std::vector<std::pair<std::string, std::string> > >::iterator it = mcFiles.begin(); it != mcFiles.end(); it++)
	std::cout << "mcFiles  [" << "] " << it->first << std::endl;

      std::cout << "dataFiles.size() " << dataFiles.size() << std::endl;
      std::cout << "mcFiles.size()   " << mcFiles.size()   << std::endl;
      std::cout << "Error: Data and MC run periods do not match." << std::endl;
        exit(1);
    }

    //So, we have the same number of run periods between data and MC.  Create
    //an array of SampleGroups, one for each from the input files.
    SampleGroup** sampleGroups = new SampleGroup*[dataFiles.size()];

    //Iterate through the maps, adding the data samples and the MC samples.
    //sgi is an iterator over the sample groups.
    int sgi = 0;
    for ( std::map<std::string,std::vector<std::string> >::iterator it = dataFiles.begin();
	  it != dataFiles.end(); it++) {
      std::cout << "Filling the experiment" << std::endl;

        //Set up the new SampleGroup corresponding to this run period.
        sampleGroups[sgi] = new SampleGroup(it->first);

        //Add all data files.
        for( unsigned int i = 0; i < (it->second).size(); i++ ) {
          sampleGroups[sgi]->AddDataSample(new DataSample((it->second)[i]));
        }

        //Now, for the same run period, add all the MC files.
        for ( unsigned int i = 0; i < mcFiles[it->first].size(); i++ ) {
          sampleGroups[sgi]->AddMCSample(((mcFiles[it->first])[i]).first,
                                         new DataSample(((mcFiles[it->first])[i]).second));
        }

        //Add this sample group to the experiment.
        exp->AddSampleGroup(it->first, *sampleGroups[sgi]);

        Float_t POTdata, POTmc;
        sampleGroups[sgi]->GetPOT(POTdata, POTmc);
        std::cout << "Data POT: " << POTdata << std::endl
		  << "MC POT: " << POTmc << std::endl;

        //Add the data and MC POT for this sample to the variables that count
        //the total data and MC POT.
        totalDataPOT += POTdata;
        totalMCPOT += POTmc;

        //Since we're done with this sample group, increment the counter.
        sgi++;
    }

    //With all files loaded into the experiment, set the experiment into the
    //Analysis Manager.
    _man->SetExperiment(exp);

    //Set up a bool to indicate whether we want to preload events or not.  This
    //is only true if there are detector parameters to vary.
    const bool preloadEvents = (fitParameters->nDetParms > 0) && fitParameters->varyDetParms;

    //Now preload the events into the arrays in the AnalysisManager.
    //Only do this if there are detector parameters to vary.  Otherwise we
    //consider events one by one and don't store the AnaSuperEvent.
    if ( preloadEvents ) {
        _man->PreloadEvents();
    }
    else {
	//If we're not preloading events, this function still needs to be called
	//but with 2 false arguments in order to pick up the POT weights.
        _man->PreloadEvents(false,false);
    }


    // TODO: Double check that this is the right way to get nominal MC.
    // TODO continued: Especially with flux systematics, if we implement the
    // psyche implementation.
    // Set up a nominal toy.
    NominalToyMaker nominalToyMaker;
    const UInt_t nExperiments = 1;
    nominalToyMaker.CreateToyExperiments( nExperiments, _man->syst().GetSystematics());
    ToyExperiment* nominalToy = nominalToyMaker.GetToyExperiment(0);

    Weight_h nominalWeight;
    Weight_h fluxWeightSyst;

    //Now, loop over the data events, processing them and saving pointers to
    //them in psycheDataEvents if they pass selection.
    //TODO: Need to allow for skipping this if just want to preload existing
    //data histograms.
    //Or do we need to load them to get the POT weights for MC, at least as
    //currently coded?
    //Double check, but the weight value may be computed prior to actually
    //loading the events, so could have a flag of some sort.
    //TODO: Get to the above when get fitter working.
    //If we're varying detector parameters, the data events are already in
    //memory, so use that.  Otherwise, load and deal with individually.
    if ( !preloadEvents ) {
        _man->InitialiseDataTChain();
    }

    //Need to initialize the data counter and array here, so GetNDataEvents()
    //returns the right value whether or not preloading has occurred.
    const UInt_t nDE = _man->GetNDataEvents();

    nDataEvents = 0; // Incremented for events that pass the cuts
    dataEvents = new BANFFEventBase*[nDE];

    // Create the array of PreviousToyBox
    _man->sel().CreateToyBoxArray(nDE);

    //Initialized the systematics
    _man->syst().Initialize(_man->sel(), nDE);

    //for(Long64_t i = 0; i < _man->GetNDataEvents(); ++i){
    UInt_t evt = 0;
    Long64_t entry = 0; // Incremented by the event loading method
    UInt_t nentries_input_tree = _man->GetEntries();
    while ( evt < nDE && entry < nentries_input_tree ) {
	AnaSuperEventB* dataSuperEvent;

	//What we put into the data super event depends on whether we have
        //preloaded events or not.
        if ( preloadEvents ) {
            dataSuperEvent = _man->GetPreloadedDataSuperEvent(entry);
        }
        else {
            dataSuperEvent = _man->LoadSuperEvent(entry);
        }

        AnaEventB* dataEvent = dynamic_cast<AnaEventB*>(dataSuperEvent->Event);

	// Fill the EventBox using the current selection
	_man->sel().InitializeEvent(*dataEvent);
	_man->syst().InitializeEventSystematics(_man->sel(), *dataEvent);

        bool passed =  _man->ProcessEvent((*nominalToy), *(dataEvent), nominalWeight, fluxWeightSyst);
        //If it passes selection, add it to the dataEvents array and increment
        //the counter.
        if( passed ) {
            psycheBANFFEvent* pEvent = new psycheBANFFEvent(dataSuperEvent);

            //Assign a pointer the sample to the event.
            bool sampleMatchFound = false;
	    AnaEventSummaryB* pEventSummary = dynamic_cast<AnaEventSummaryB*>(pEvent->detEvent->Event->Summary);
            for ( int j = 0; j < nSamples; j++ ) {
              if ( pEventSummary->EventSample == samples[j]->sampleID ) {
                pEvent->sample = samples[j];
                sampleMatchFound = true;
                break;
              }
            }

            if ( !sampleMatchFound ) {
                pEvent->sample = NULL;
            }

            if( !preloadEvents ) {
                // Since the AnaSuperEventB is going to be deleted, save
                // observable information here.
                // NB: We don't run into a problem later if it is null, because
                // there are sample != NULL checks whenever an observable access
                // attempt is made, since observables are a property of the
                // sample.
                if ( pEvent->sample != NULL ) {
                    pEvent->sample->SaveObservableInformation(pEvent);
                }
            }
            dataEvents[nDataEvents] = pEvent;

            //Now, if we're not preloading events, psyche is going to delete
            //the pointer to the AnaSuperEventB internally.  So for good
            //measure, set the soon to be dangling pointer to NULL.
            if ( !preloadEvents ) {
                pEvent->detEvent = NULL;
            }
            nDataEvents++;
        }
	evt++;
        _man->syst().FinalizeEventSystematics(*dataEvent);
        _man->sel() .FinalizeEvent(*dataEvent);
    }

    //Read from the parameters file whether we want to override the default POT
    //weighting for the MC with a user supplied value.
    //The same scale factor is supplied to all events.  In this way, the
    //proportion of any different run periods will be preserved, just the
    //total POT can be changed as desired.
    bool scaleMCPOT = ND::params().GetParameterI("BANFF.ScaleMCPOT");
    double scaleFactor = 1.0;

    if ( scaleMCPOT ) {
        double scaledMCPOT = ND::params().GetParameterD("BANFF.ScaledMCPOT");
        scaleFactor = scaledMCPOT/totalDataPOT;

        std::cout << "==================================================================================================================" << std::endl;
        std::cout << "MC POT will be scaled to " << scaledMCPOT << " POT by applying a scale factor of " << scaleFactor << " to all MC events." << std::endl;
        std::cout << "==================================================================================================================" << std::endl;
    }



    //Before we start loading events, make sure that useLowMem is set to false
    //if there are detector parameters being varied, because the low memory
    //version does not work for varying detector parameters.
    if((fitParameters->nDetParms > 0) && (fitParameters->varyDetParms == true)){
        useLowMem = false;
    }

    //With the dataEvents taken care of, process the MC.
    if ( !preloadEvents ) {
        _man->InitialiseMCTChain();
    }

    //Initialize MC event counter and mcEvents array here, so GetNMCEvents()
    //returns the correct value whether or not the events are preloaded.
    const UInt_t nMC = _man->GetNMCEvents();
    nMCEvents = 0; // Incremented for good MC events
    mcEvents = new BANFFEventBase*[nMC];

    //A temporary sand counter.
    int nSandEvents = 0;

    //Add in a counter for raw events passed for each selection, prior to any
    //binning.  Useful for cross comparisons.
    int* rawEventCounter      = new int[nSamples];

    for ( int si = 0; si < nSamples; si++ ) {
        rawEventCounter[si] = 0;
    }
    // Create the array of PreviousToyBox
    _man->sel().CreateToyBoxArray(nMC);
    // Create the array of SystBox
    _man->syst().Initialize(_man->sel(),nMC);

    evt = 0;
    entry = 0; // Incremented by the LoadEvent method


    //Set up a tree here to make a friend tree to store some information for
    //diagnostic purposes.
    TFile* diagFile = new TFile(diagnostic_name.c_str(),"RECREATE");
    std::cout<< "creating diagnostic file: " << diagnostic_name << std::endl;
    diagFile->cd();
    TTree* diagTree = new TTree("diagnosticTree", "A diagnostic friend to the flat tree.");
    Double_t POTWgt;
    Double_t FluxWgt;
    Double_t DetWgt;
    Int_t SampleID;
    Double_t MuMom;
    Double_t MuCosTheta;
    Bool_t Passed;

    diagTree->Branch("POTWgt",     &POTWgt,     "POTWgt/D");
    diagTree->Branch("FluxWgt",    &FluxWgt,    "FluxWgt/D");
    diagTree->Branch("DetWgt",     &DetWgt,     "DetWgt/D");
    diagTree->Branch("SampleID",   &SampleID,   "SampleID/I");
    diagTree->Branch("MuMom",      &MuMom,      "MuMom/D");
    diagTree->Branch("MuCosTheta", &MuCosTheta, "MuCosTheta/D");
    diagTree->Branch("evt",        &evt,        "evt/i" );
    diagTree->Branch("entry",      &entry,      "entry/L" );
    diagTree->Branch("nMCEvents",  &nMCEvents,  "nMCEvents/I" );
    diagTree->Branch("Passed",     &Passed,     "Passed/O" );

    nentries_input_tree = _man->GetEntries();
    while ( evt < nMC && entry< nentries_input_tree ){

        //Set all of the diagnostic tree entries to -1 for now.  If the event
        //isn't selected, they'll be written as that.
	POTWgt = -1.0;
	FluxWgt = -1.0;
	DetWgt = -1.0;
	SampleID = -1;
	MuMom = -1.0;
	MuCosTheta = -1.0;

        if ( entry%10000 == 0 ) {
	  std::cout << "Considering AnalysisManager event " << entry/1000 << "k of " << (double)nMC/1000. <<"k"<< std::endl;
	  std::cout << "Number of events loaded in BANFF Interface: " << (double)nMCEvents/1000. << "k" << std::endl;
	  std::cout << "entry = " << entry/1000 << "k, evt = " << (double)evt/1000. << "k, nMCEvents = " << (double)nMCEvents/1000. << "k" << std::endl;
        }


        //Process the Event with nominal toys.
        AnaSuperEventB* mcSuperEvent;
        if ( preloadEvents ) {
            mcSuperEvent = _man->GetPreloadedMCSuperEvent(entry);
        }
        else {
            mcSuperEvent = _man->LoadSuperEvent(entry);
        }
        AnaEventB* event = dynamic_cast<AnaEventB*>(mcSuperEvent->Event);
        // Fill the EventBox
        _man->sel().InitializeEvent(*event);
        // Initialize The SystBox for variation systematics
        _man->syst().InitializeEventSystematics(_man->sel(),*event);


        bool passed = _man->ProcessEvent((*nominalToy), *(event), nominalWeight, fluxWeightSyst);

	// Set some of the diagnositics tree variables.
	Passed = passed;
	AnaEventSummaryB* eventSummary = dynamic_cast<AnaEventSummaryB*>(event->Summary);
	SampleID = eventSummary->EventSample;

        // The only way events that previously did not pass selection can
        // migrate into selection is if there are detector parameters that are
        // being varied.  If there are not, there is no need to consider events
        // that did not pass at this step.  So, in that case, skip to the next
        // event.
        // Detector parameters are loaded before the events are, so if there are
        // any, nDetParms should be non-zero.
        if( !passed && ((fitParameters->nDetParms == 0) || !fitParameters->varyDetParms)  ){
            // Write the diagnostic tree with default values then go to the next
            // event.
            diagTree->Fill();
	    evt++;
            // Reset The SystBox for variation systematics
            _man->syst().FinalizeEventSystematics(*event);
            // Reset the EventBox
            _man->sel().FinalizeEvent(*event);
            continue;
        }

        //If the selection was passed, verify that there was a true vertex
        //associated with it.  If not, print a warning message and skip this
        //event.
        if ( passed ) {
            if ( eventSummary->TrueVertex[eventSummary->EventSample] == NULL ) {
                std::cout << "WARNING1: No true vertex associated with event " << entry
			  << ".  Skipping event." << std::endl;
                diagTree->Fill();
		evt++;
                // Reset The SystBox for variation systematics
                _man->syst().FinalizeEventSystematics(*event);
                // Reset the EventBox
                _man->sel().FinalizeEvent(*event);
                continue;
            }

            //Count the raw events for output later.
            for ( int si = 0; si < nSamples; si++ ) {
	      // std::cout << "samples " << samples[si]->sampleID.name << std::endl;
              if ( eventSummary->EventSample == samples[si]->sampleID ) {
                rawEventCounter[si]++;
              }
            }
        }

        //The summary tree information at the same entry as this event now
        //contains information for each selection.  We just load all of it into
        //the stored event information.  The correct sample's information will
        //be used when dealing with this event in toy throwing or fitting.

        //Make the splines we'll be assigning to the event.
        sumTree->MakeSplines(evt);

        psycheBANFFEvent* pEvent = new psycheBANFFEvent(mcSuperEvent, fitParameters->GetNumbXsecRespSysts(),
                                                        evt, useLowMem);

	AnaEventSummaryB* pEventSummary = dynamic_cast<AnaEventSummaryB*>(pEvent->detEvent->Event->Summary);
        //Having found the weight provided by psyche, store it in
        //the event structure.  "Nominal" includes a nominal weight from
        //psyche.
        //Cap the nominal weight at 100.0 to be consistent with
        //MaCh3 and in accordance with discusssions as to what is
        //reasonable.
        //The pile-up weight is contained in event->Weight, and we need to
        //include it here.
        //TODO: If this changes in the future, may need to change this.
        //Also, not sure what the correct thing to do is when throwing detector
        //parameters, so don't change that for now.
        nominalWeight = nominalWeight*(event->Weight);
        if ( nominalWeight.Correction > 100.0 ) {
            nominalWeight.Correction = 100.0;
        }

        pEvent->SetDetWeight(nominalWeight.Correction);

        //If the user has requested a POT weight scaling, perform
        //this now.
        if ( scaleMCPOT ) {
            double oldWeight = pEvent->GetPOTWeight();
            pEvent->SetPOTWeight(oldWeight*scaleFactor);
        }

        //Assign the sample to the event (both integer code and a
        //pointer.)
        bool sampleMatchFound = false;
        for ( int j = 0; j < nSamples; j++ ) {

          if ( pEventSummary->EventSample == samples[j]->sampleID ) {
            pEvent->SetSampleIndex( samples[j]->sampleID );
            pEvent->sample = samples[j];

            //If we're not varying detector parameters, pre-save
            //information to speed up making the MC prediction.
            if( (fitParameters->nDetParms == 0) || !(fitParameters->varyDetParms) ) {
              pEvent->sample->SaveObservableInformation(pEvent);
            }
            sampleMatchFound = true;
            break;
          }
        }

        //If a match is not found, set the sample to NULL and the
        //ID to -1
        if ( !sampleMatchFound ) {
            pEvent->SetSampleIndex(-1);
            pEvent->sample = NULL;
        }
        mcEvents[nMCEvents] = pEvent;

        //Load all the information from the summary tree,
        //including any XSec response splines, into the event.
        LoadSummaryTreeInfo( mcEvents[nMCEvents] );

        //If the selection was passed, (sample != NULL), LoadSummaryTreeInfo
        //has saved info for the passed selection in sumTreeTruthVertex,
        //sumTreeEnu, and sumTreeNuPDG.  Compare this information to the
        //corresponding field in psyche.  For neutrino energy, require that the
        //difference between the spline and the flat tree is less than 2e-3 or
        //spit out a mismatch warning.
        bool eventIsGood = true;

        //Only execute this code block if we're not dealing with sand MC.
        if ( !( dynamic_cast<AnaEventB*>(pEvent->detEvent->Event)->GetIsSandMC()  ) ) {

            if ( mcEvents[nMCEvents]->sample != NULL) {

                // if ( mcEvents[nMCEvents]->GetNuEnergy() < 0 ) {
                //     std::cout << "EVEN AT NOMINAL NEGATIVE ENERGIES ARE ALLOWED." << std::endl;
                // }

                //int truthVertexID = eventSummary->TrueVertex[eventSummary->EventSample]->TruthVertexID;
		// FIXME: Is this correct? The TruthVertexID field does not
		// exist in pysche v3 AnaTrueVertexB class. Instead there is an
		// ID parameter in the AnaTrueObjectC base class.
		int truthVertexID = eventSummary->TrueVertex[eventSummary->EventSample]->ID;


                bool match_TruthVertexID = truthVertexID == sumTreeTruthVtx;

                Float_t nuE = eventSummary->TrueVertex[eventSummary->EventSample]->NuEnergy;
                bool match_Enu = fabs(1000.0*(sumTreeEnu) - nuE) < 2.e-3;

                Int_t nuPDG = eventSummary->TrueVertex[eventSummary->EventSample]->NuPDG;
                bool match_NuPDG = sumTreeNuPDG == nuPDG;

                if (!(match_TruthVertexID && match_Enu && match_NuPDG)){
                    std::cout << "==========================================================="
			      << std::endl
			      << "WARNING2: Event " << entry << " has a discrepancy with the summary tree."
			      << std::endl
			      << "Summary tree TruthVertexID: " << sumTreeTruthVtx
			      << " psyche TruthVertexID: " << truthVertexID << std::endl
			      << "Summary tree Enu: " << 1000.0*sumTreeEnu
			      << " psyche Enu: " << nuE << std::endl
			      << "Summary tree nuPDG: " << sumTreeNuPDG
			      << " psyche nuPDG: " << nuPDG << std::endl;

		    std::cout << "evt = " << evt
			      << ", entry = " << entry
			      << ", nMCEvents = " << nMCEvents
			      << ", event->Summary->EventSample = " << eventSummary->EventSample
			      << "(" << static_cast<int>(eventSummary->EventSample) << ")"
			      << std::endl
			      << "==========================================================="
			      << std::endl;

                    eventIsGood = false;
                }
            }
        }
        else {
	    // Increment a separate (and temporary) sand counter so we can keep track.
            nSandEvents++;
        }

        //Increase the MC Events counter.

        //TODO: Only do this if we don't have a mismatch.  Temporary diagnostic
        //measure.
        //Actually keep it in for now, as we don't want mismatched events
        //included, though this isn't a problem at the moment.
        if ( eventIsGood ) {
            //Since this always needs to be done for MC events, and all parameters
            //are loaded at this time, perform the initial fit parameter matching
            //to events here.  Other calls to match will only occur if detector
            //parameters are varied (thus ensuring that the entire AnaSuperEventB
            //is in memory to be accessed.)
            if ( mcEvents[nMCEvents]->sample != NULL ) {
                fitParameters->MatchToEvent( mcEvents[nMCEvents] );
            }

            //Double check that sand MC doesn't get matched to flux or xsec
            //parameters.
            if( dynamic_cast<AnaEventB*>(pEvent->detEvent->Event)->GetIsSandMC()){

                if( pEvent->GetFluxParameter() >= 0 ) {
                    std::cout << "=======================================================" << std::endl
			      << "WARNING3: Sand MC Event matched to flux parameter."      << std::endl
			      << "=======================================================" << std::endl;
                }

                if( pEvent->GetXsecNormParameter() >= 0 ) {
                    std::cout << "=======================================================" << std::endl
			      << "WARNING4: Sand MC Event matched to xsec norm parameter." << std::endl
			      << "=======================================================" << std::endl;
                }
            }

            // Fill diagnostic tree info here.
            POTWgt = pEvent->GetPOTWeight();
            FluxWgt = pEvent->GetFluxWeight();
            DetWgt = pEvent->GetDetWeight();

	    AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(pEvent->detEvent->Event->Summary);
	    AnaParticleMomB* leptonCandidate = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample]);
	    MuMom = leptonCandidate->Momentum;
	    MuCosTheta = leptonCandidate->DirectionStart[2];

            //Now, if we're not preloading events, psyche is going to delete
            //the pointer to the AnaSuperEventB internally.  So for good
            //measure, set the soon to be dangling pointer to NULL.
            if ( !preloadEvents ) {
                pEvent->detEvent = NULL;
            }
            nMCEvents++;


        }

	// This fills if there was a mismatch or not (default values with
        // mismatch.)
	diagTree->Fill();
	evt++;
        // Reset The SystBox for variation systematics
        _man->syst().FinalizeEventSystematics(*event);
        // Reset the EventBox
        _man->sel().FinalizeEvent(*event);
    }//End loop over MC events.

    std::cout << "=======================================================" << std::endl
	      << "Total Data Events loaded: " << nDataEvents << std::endl
	      << "Total MC Events loaded: " << nMCEvents << std::endl
	      << "Number of MC Events that are Sand Events: " << nSandEvents << std::endl
	      << "=======================================================" << std::endl;

    //Print out the sample specific raw event counts with no binning.
    std::cout << "Raw event counts in each sample: " << std::endl;
    for( int si = 0; si < nSamples; si++ ) {
        std::cout << samples[si]->name << ": " << rawEventCounter[si]
		  << std::endl;
    }
    std::cout << "=======================================================" << std::endl;

    diagFile->cd();
    diagTree->Write();
    diagFile->Close();
}

void psycheInterface::ReProcessMC()
{
    //Set up a toy experiment.
    NominalToyMaker nominalToyMaker;
    const UInt_t nExperiments = 1;
    nominalToyMaker.CreateToyExperiments( nExperiments, _man->syst().GetSystematics());
    ToyExperiment* toy = nominalToyMaker.GetToyExperiment(0);

    //Also grab out the Variation and Weight Systematics.
    //They are stored in the toy in the following order:
    //1. variation systematic parameters.
    //2. weight systematic parameters.
    //3. flux systematic parameters.
    //The detThrows vector is only as big as the number of detector parameters
    //corresponding to enabled systematics, but these will be in the same order
    //as in the toy.  So, create the toy, and slot in the value from detThrows
    //when the toy index corresponds to an enabled systematic, the disabled systematics
    //should not have their toy index be accessed, so don't waste time filling those indices.
    int nVariationSyst;
    int nWeightSyst;
    EventVariationBase** varSyst = _man->syst().GetVariationSystematics(nVariationSyst);
    EventWeightBase** weightSyst = _man->syst().GetWeightSystematics(nWeightSyst);

    //Get the vector of throws.
    std::vector<double> detThrows = fitParameters->GetDetectorVector();

    //Now, go through the systematics and set the variations.
    unsigned int throw_iter = 0;
    for ( unsigned int i = 0; i < toy->GetNToyVariations(); i++ ) {

        // std::cout << "-------------------------------------------" << std::endl;
        // If this index corresponds to an enabled systematic, set the variation
        // to the corresponding entry from detThrows.
        bool enabled = false;
        if( (int)i < nVariationSyst ) {
            enabled = varSyst[i]->IsEnabled();
        }
        else if ( (int)i >=  nVariationSyst && (int)i < nVariationSyst+nWeightSyst ) {
            int weightSystIdx = (int) i - nVariationSyst;
            enabled = weightSyst[weightSystIdx]->IsEnabled();
        }

        // If we've gone outside the supplied vector of throws, and we have an
        // enabled systematic to pass a throw value to, break out and print an
        // error message.
        if ( (throw_iter >= detThrows.size()) && enabled ) {
            std::cout << "throw_iter " << throw_iter << std::endl
		      << "detThrows.size() " << detThrows.size() << std::endl
		      << "NSystematics " << toy->GetNToyVariations() << std::endl
		      << "NParams this syst " << toy->GetNParameters(i) << std::endl
		      << "ERROR: Not enough DET throws!" << std::endl;
            break;
        }

        for ( unsigned int j = 0; j < toy->GetNParameters(i); j++ ) {
            if ( enabled ) {
                toy->SetToyVariation(i,j, (Float_t) detThrows[throw_iter]);
                throw_iter++;
            }
            else {
                toy->SetToyVariation(i,j,0.0);
            }
        }
    }


    //    std::cout << "================================================" << std::endl;
    //    for(unsigned int i = 0; i < toy.GetNSystematics(); i++){
    //
    //        for(unsigned int j = 0; j < toy.GetNParameters(i); j++){
    //
    //            std::cout << toy._systematics[i]->Variations[j]  << std::endl;
    //
    //        }
    //
    //    }
    //    std::cout << "================================================" << std::endl;

    //Loop over the MC events, process them, save the weight to the event's
    //detector weight field, and change the sampleID and sample pointer as
    //needed.
    //By setting the proper flag in the requirements file for BANFF and in the
    //right places for psyche, can enable multithreaded psyche processing.
#ifdef MULTITHREADPSYCHE
#pragma omp parallel for schedule(dynamic,20)
#endif
    for( int i = 0; i < nMCEvents; i++ ) {

        //Cast the mcEvent back to a psycheBANFFEvent.
        psycheBANFFEvent* pEvent = static_cast<psycheBANFFEvent*>(mcEvents[i]);

        //Extract the super event.
        AnaSuperEventB* sevent = pEvent->detEvent;

        //Get the event to be modified.  This is the only one used in the analysis.
        AnaEventB* event = static_cast<AnaEventB*>(sevent->Event);

        //Vector of weights corresponding to individual selections.  Gets filled by
        //ProcessEvent.  If the selections are mutually exclusive, only one should
        //be non-zero.
        Weight_h systWeight = pEvent->GetDetWeight();
        Weight_h fluxWeightSyst;

        _man->ProcessEvent( *toy, *event, systWeight, fluxWeightSyst );

        //Multiply in the pileup weight here too.
        systWeight = systWeight*(event->Weight);

        //Having found the weight provided by psyche, store it in
        //the event structure.
        //Cap this weight at 100.0, as was done for the nominal weights above.
        if ( systWeight.Correction > 100.0 ) {
            systWeight.Correction = 100.0;
        }
        pEvent->SetDetWeight(systWeight.Correction);

        //Assign the sample to the event (both integer code and a
        //pointer.)
        bool sampleMatchFound = false;
        for ( int j = 0; j < nSamples; j++ ) {
            //We force summary creation, and the event sample is created in the
            //constructor, so this should never be a problem.
	    if ( static_cast<AnaEventSummaryB*>(event->Summary)->EventSample == samples[j]->sampleID ) {
                pEvent->SetSampleIndex( samples[j]->sampleID );
                pEvent->sample = samples[j];
                sampleMatchFound = true;
                break;
            }
        }

        //If a match is not found, set the sample to NULL and the
        //ID to -1
        if ( !sampleMatchFound ) {
            pEvent->SetSampleIndex(-1);
            pEvent->sample = NULL;
        }
    }

    //With the event reprocessed, weights copied over, and sample re-specified,
    //the event has enough information for other methods to do the rest of the
    //work.
}

void psycheInterface::LoadDetectorParameters( bool vary ) {

    //Save this so know whether need to re-process MC.
    fitParameters->varyDetParms = vary;

    //Also grab out the Variation and Weight Systematics.
    int nVariationSyst;
    int nWeightSyst;
    EventVariationBase** varSyst = _man->syst().GetVariationSystematics( nVariationSyst );
    EventWeightBase** weightSyst = _man->syst().GetWeightSystematics( nWeightSyst );

    std::cout << "Loading detector parameters from psyche." << std::endl;

    //TODO: This should nominally be done in psyche inside the systematic code,
    //but needs to be done here for now.
    //Set the PDF type for each of the systematics.  Everything is Gaussian
    //except for kBFieldDist.
    for( int i = 0; i < nWeightSyst; i++ ) {
        weightSyst[i]->SetPDF(SystematicBase::kGaussian);
    }

    for ( int i = 0; i < nVariationSyst; i++ ) {
        //If it's kBFieldDist, it's a uniform distribution between 0 and 1,
        //which is how uniform PDFs are treated with the upper and lower bound
        //below.
        if( varSyst[i]->Name() == "kBFieldDist" ) {
            varSyst[i]->SetPDF( SystematicBase::kUniform );
        }
        else {
            varSyst[i]->SetPDF( SystematicBase::kGaussian );
        }
    }


    //Add detector parameters for the variation systematics.
    for ( int i = 0; i < nVariationSyst; i++ ) {
        EventVariationBase* tmp = varSyst[i];

        //We only want to add a detector parameter for it is enabled.  So, if
        //it is not enabled, continue to the next systematic.
        if ( !(tmp->IsEnabled()) ) {
            continue;
        }

        std::string name = tmp->Name();
        PDFcode pdfCode = GetPDFcode(tmp->PDF());

        for ( unsigned int j = 0; j < tmp->GetNParameters(); j++ ) {

            //TODO: Get the initial value from psyche.
            double value = 0.0;

            //TODO: Get the bounds from psyche.
            double lBound = -999.0;
            double uBound = 999.0;
            if ( pdfCode == PDFFlat ) {
                lBound = 0.0;
                uBound = 1.0;
            }

            std::string detsyst(Form("%s_%d",name.c_str(),j));
            fitParameters->AddDetParameter(value, 0, pdfCode, lBound, uBound, detsyst,vary);
            fitParameters->nDetParms++;
        }
    }

    //Add detector parameters for the weight systematics.
    for ( int i = 0; i < nWeightSyst; i++ ) {
        EventWeightBase* tmp = weightSyst[i];

        //We only want to add a detector parameter for it is enabled.  So, if
        //it is not enabled, continue to the next systematic.
        if(!(tmp->IsEnabled())){
            continue;
        }

        std::string name = tmp->Name();
        PDFcode pdfCode = GetPDFcode(tmp->PDF());

        for ( unsigned int j = 0; j < tmp->GetNParameters(); j++ ) {

            //TODO: Get the initial value from psyche.
            double value = 0.0;

            //TODO: Get the bounds from psyche.
            double lBound = -999.0;
            double uBound = 999.0;
            if ( pdfCode == PDFFlat ) {
                lBound = 0.0;
                uBound = 1.0;
            }

            std::string detsyst( Form("%s_%d",name.c_str(),j) );
            fitParameters->AddDetParameter(value, 0, pdfCode, lBound, uBound ,detsyst,vary);
            fitParameters->nDetParms++;
        }
    }

    //Make the covariance matrix for detector systematics
    //TODO: Add in ability to pull covariance from psyche, once psyche has that
    //ability.
    TMatrixDSym* det_cov = new TMatrixDSym(fitParameters->nDetParms);
    for ( int i=0; i<det_cov->GetNrows(); i++ ) {
        for(int j=0; j<det_cov->GetNrows(); j++ ) {
            if ( i==j ) {
                (*det_cov)(i,j) = 1.0;
            }
            else {
                (*det_cov)(i,j) = 0.0;
            }
        }
    }
    fitParameters->AppendCovariance(det_cov);
}

PDFcode psycheInterface::GetPDFcode(SystematicBase::PDFEnum pdfEnum){

    if ( pdfEnum == SystematicBase::kGaussian ) {
        return PDFGaus;
    }

    if ( pdfEnum == SystematicBase::kUniform ) {
        return PDFFlat;
    }

    if ( pdfEnum == SystematicBase::kBinomial ) {
        return PDFBino;
    }

    std::cout << "ERROR: Unknown PDF supplied from psyche.  Using Gaussian."
	      << std::endl;
    return PDFGaus;
}
