#include <sstream>
#include <omp.h>

#include "TXMLEngine.h"

#include "BANFFInterfaceBase.hxx"
#include "Parameters.hxx"
#include "psycheInterface/psycheBANFFEvent.hxx"

BANFFInterfaceBase::BANFFInterfaceBase(int nSamp,
                                       BANFFSampleBase** samp,
                                       FitParameters* fitParms,
                                       std::string diagnostic){

  nSamples = nSamp;
  samples = samp;
  fitParameters = fitParms;

  //Set the execution counter (for timing purposes) to 0.
  countApplyParamsAndCalcLLR = 0;

  //Set the low memory usage flag to whatever the parameters file specifies.
  useLowMem = ND::params().GetParameterI("BANFF.UseLowMemoryVersion");
  OnlyPenaltyContrib = false;
  OnlySampleContrib = false;
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
    std::cout << "Warning: multithreading disasbled in BANFFInterfacBase."
              << std::endl;
  }

}

BANFFInterfaceBase::~BANFFInterfaceBase(){

}
    
void BANFFInterfaceBase::ParseInputFilesViaXML(){

  // First create engine
  TXMLEngine* xml = new TXMLEngine;
  // Now try to parse xml file
  // Only file with restricted xml syntax are supported
  XMLDocPointer_t xmldoc = xml->ParseFile(file_list.c_str());
    
  if (xmldoc==0) {
    delete xml;
    return;
  }
    
  // take access to main node
  XMLNodePointer_t mainnode = xml->DocGetRootElement(xmldoc);
  // Now parse the heck out of this
  std::cout << "Using the XML parser!" << std::endl;
    
  XMLNodePointer_t RunPeriodNode = xml->GetChild(mainnode);

  while(RunPeriodNode != 0){ // First loop is over the run period
    std::string RunPeriod(xml->GetNodeName(RunPeriodNode));
    std::cout << "Parsing info for " << xml->GetNodeName(RunPeriodNode) << std::endl;
    XMLNodePointer_t File = xml->GetChild(RunPeriodNode);
    while(File != 0){ // Second is over the Files            
      const char* content = xml->GetNodeContent(File);
      if (content!=0){
        std::string FileType(xml->GetNodeName(File));
        std::string FileLocation(content);
        if(FileType == "Data")
          dataFiles[RunPeriod].push_back(content);
        else if(FileType == "MagnetMC")
          mcFiles  [RunPeriod].push_back(std::make_pair("magnet",content));
        else if(FileType == "SandMC")
          mcFiles  [RunPeriod].push_back(std::make_pair("sand",content));
        else{
          std::cerr << "Unrecognized file: " << std::endl;
          std::cerr << "Can only parse Data, MagnetMC, SandMC files, and not " << FileType << std::endl;
          throw;
        }
                    
      }
      File = xml->GetNext(File);
    }
    RunPeriodNode = xml->GetNext(RunPeriodNode);

  }

  // Release memory before exit
  xml->FreeDoc(xmldoc);
  delete xml;
  DumpInputFileContent();
  return;

}


void BANFFInterfaceBase::ParseInputFiles(){


  //Get the file list from the parameters file.
  file_list = ND::params().GetParameterS("BANFF.InputFileList");
    
  if(file_list.find(".xml") != std::string::npos){
    ParseInputFilesViaXML();
    return;
  }
  std::cout << "Using old method to fill the file list" << std::endl;
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
  DumpInputFileContent();
}

void BANFFInterfaceBase::DumpInputFileContent(){
    
  //Debug code.
  std::cout << "Number of data run periods: " << dataFiles.size() << std::endl;
  std::cout << "Number of MC run periods: " << mcFiles.size() << std::endl;
  std::cout << "Data file information" << std::endl;

  for(std::map<std::string,std::vector<std::string> >::iterator it = dataFiles.begin(); it!= dataFiles.end(); it++){
    std::cout << "===========================================" << std::endl;
    std::cout << "Run period: " << it->first << std::endl;
    std::cout << "Data Files:" << std::endl;
    for(unsigned int i = 0; i < it->second.size(); i++){

      std::cout << (it->second)[i] << std::endl;

    }

    std::cout << "MC files:" << std::endl;
    for(unsigned int i = 0; i < mcFiles[it->first].size(); i++){
            
      std::cout << ((mcFiles[it->first])[i]).first << " " << ((mcFiles[it->first])[i]).second << std::endl;
    }

  }

}

// *****************************
// Get the mass of a particle from the PDG
double BANFFInterfaceBase::GetMassFromPDG(int PDG) {
  // *****************************

  switch (abs(PDG)) {

  case 11:
    return 0.511E-3;
    break;
  case 13:
    return 105.66;
    break;
  case 15:
    return 1.77682E3;
    break;
  case 211:
    return 139.57;
    break;
  case 111:
    return 134.98;
    break;
  case 2112:
    return 939.565;
    break;
  case 2212:
    return 938.27;
    break;
  case 12:
  case 14:
  case 16:
    return 0.0;
    break;
  default:
    std::cerr << "Haven't got a saved mass for PDG " << PDG << std::endl;
    std::cerr << "Please implement me! " << __FILE__ << ":" << __LINE__ << std::endl;
    throw;
  } // End switch

  std::cerr << "Warning, didn't catch a saved mass" << std::endl;
  return 0;
}

double BANFFInterfaceBase::ApplyCoulombShift(BANFFEventBase* event, double enu, double pmu, int target, int reaction, int kinType, double InitialValue) {
  // ******************************************
  
  // The amount we shift by
  double shift = 0.0;

  // Only want CC events
  if (TMath::Abs(reaction) >= 30) return shift;

  // The event summary
  psycheBANFFEvent* pEvent = static_cast<psycheBANFFEvent*>(event);
  AnaEventSummaryB* Summary = dynamic_cast<AnaEventSummaryB*>(pEvent->detEvent->Event->Summary);
  // Get the selection this event falls into
  SampleId::SampleEnum index = Summary->EventSample;

  // Make sure the lepton candidate has true particle ID of lepton (so we only shift true leptons that are reconstructed as leptons, not shift true pions that get reconstructed as lepton)
  // Get the lepton candidate
  AnaParticleB* LeptonCandidate = dynamic_cast<AnaParticleB*>(Summary->LeptonCandidate[index]);
  
  AnaParticleMomB* LeptonCandidateMom = dynamic_cast<AnaParticleMomB*>(Summary->LeptonCandidate[index]);

  // If not a true particle return zero shift
  if(!LeptonCandidateMom->GetTrueParticle())
    return shift;
  
  // Get the true particle giving rise to our lepton candidate
  AnaTrueParticleB* TrueParticle = dynamic_cast<AnaTrueParticleB*>(LeptonCandidate->GetTrueParticle());
  // Get the true particle's PDG
  int TruePDG = TrueParticle->PDG;

  // Get the neutrino PDG
  AnaTrueVertexB *vtx = Summary->TrueVertex[index];
  int NuPDG = vtx->NuPDG;

  // Check the lepton candidate matches the neutrino PDG
  // If anti-neutrino interaction, check that NuPDG 
  if (NuPDG < 0) {
    if (NuPDG != TruePDG-1) return shift;
  }
  else {
    if (NuPDG != TruePDG+1) return shift;
  }
  // Check PID of true particle making this lepton candidate
  // This is now done above as before. The following line is only left for historical comparison
  //if (abs(TruePDG) != 11 && abs(TruePDG) != 13 && abs(TruePDG) != 15) return 0.0;
  
  bool isCarbon = (target == 12);
  bool isOxygen = (target == 16);

  // Shift the lepton momentum
  if (kinType == 0) {
    // Neutrino
    if (TruePDG > 0) {
      // If target was C
      if (isCarbon) {
        shift = -3.6;
        // If target was O
      } else if (isOxygen) {
        shift = -4.3;
      }
      // Anti-neutrino
    } else if (TruePDG < 0) {
      // If target was C
      if (isCarbon) {
        shift = 2.6;
      } else if (isOxygen) {
        shift = 3.3;
      }
    }
    // Return the shifted lepton momentum
    return shift;
  // Shift the Q2 if requested
  }
  else if (kinType == 1) {
    // Get the true vertex
    AnaTrueVertexB *vtx = Summary->TrueVertex[index];
    if (vtx == NULL) {
      std::cerr << "Could not find TrueVertex object, sorry" << std::endl;
      std::cerr << __FILE__ << ":" << __LINE__ << std::endl;
      return 0.0;
    }
    //double EnuTrue = vtx->NuEnergy/1.E3;
    double EnuTrue = enu;

    // Then we get the TLorentzVector of the highest momentum muon
    //double PLepTrue = LeptonCandidateMom->Momentum/1.E3;
    double PLepTrue = pmu;
    double MLep = GetMassFromPDG(TruePDG)/1.E3;
    double ELepTrue = sqrt(PLepTrue*PLepTrue + MLep*MLep);

    // Now we can get the ThNuMu true
    double CosThNuMu = (2.0*EnuTrue*ELepTrue - MLep*MLep - InitialValue)/(2.0*EnuTrue*PLepTrue);

    // Now shift the muon momentum (returned shift is in MeV)
    PLepTrue += ApplyCoulombShift(event, enu, pmu, target, reaction, 0)/1.E3;
    // And update the lepton momentum
    ELepTrue = sqrt(PLepTrue*PLepTrue + MLep*MLep);

    // Calculate the new Q2
    double Q2Upd = -(MLep*MLep) + 2.0*EnuTrue*(ELepTrue - PLepTrue*CosThNuMu);

    // And finally return the shift
    shift = Q2Upd - InitialValue;

    if (std::isnan(shift)) {
      std::cerr << "Found nan shift" << std::endl;
      std::cerr << "TruePDG:  " << TruePDG << std::endl;
      std::cerr << "NuPDG:  " << TruePDG << std::endl;
      std::cerr << "PLepTrue: " << PLepTrue << std::endl;
      std::cerr << "ELepTrue: " << ELepTrue << std::endl;
      std::cerr << "MLep:     " << MLep << std::endl;
      std::cerr << "CosThNuMu: " << CosThNuMu << std::endl;
      std::cerr << "EnuTrue:  " << EnuTrue << std::endl;
      std::cerr << "Q2Upd:    " << Q2Upd << std::endl;
      throw;
    }
    return shift;
  } else {
    std::cerr << "Can't Coulomb shift the requested kinematic variable (" << kinType << ")" << std::endl;
    std::cerr << "Will throw error" << std::endl;
    std::cerr << __FILE__ << ":" << __LINE__ << std::endl;
    throw;
  }

  return shift;
}

void BANFFInterfaceBase::LoadSummaryTreeInfo(BANFFEventBase* event){

  //Content of the summary tree entry corresponding to this event already loaded by GetEntry in WeightFile::MakeSplines
    
  // Get the detector weight from the splines (and only accept detector weight bigger than 0)
  if(sumTree->DetNomWeight > 0 && sumTree->DetNomWeight < 10){
    event->SetDetWeight(sumTree->DetNomWeight);
  }else if(sumTree->DetNomWeight == -999){
    std::cerr << "ERROR: Detector weight is -999 there is something wrong in the spline linking." << std::endl;
    throw;
  }else if(sumTree->DetNomWeight < 0){
    event->SetDetWeight(0);
  }else if(sumTree->DetNomWeight > 10){
    event->SetDetWeight(10);
  }

  //The number of samples (i.e. the size of the arrays storing the summary
  //tree information.)
  event->SetNSamples(sumTree->NSamples);

  //The neutrino energy from the summary tree, in GeV.
  event->SetNuEnergy(sumTree->Enu);
  
    
  //The neutrino code from the summary tree.  Corresponds to the PDG code for
  //the neutrino.
  event->SetNuFlavor(sumTree->NeutrinoCode);

  //The flux correction for the event, put into the tree from the beam flux
  //tuning files.
  if(event->sample != NULL){
    event->SetFluxWeight(sumTree->FluxWeight[event->sample->sampleID]);
  }
  else{
    event->SetFluxWeight(-999.);
  }
    
  //The reaction code for the event as read from the RooTrackerVertex into
  //the summary tree.
  event->SetReactionCode(sumTree->ReactionCode);

  //The target material that the neutrino interaction occurred on, as read in
  //from the RooTrackerVertex information in the summary tree.
  event->SetTargetMaterial(sumTree->TgtMat);

  //The true lepton momentum from the summary tree, in GeV.
  event->SetTruePmu(sumTree->TruePmu);

  //The true lepton PDG code from the summary tree.
  event->SetTrueLepPDG(sumTree->TrueLepPDG);

  //The Q2 from the summary tree.
  //event->SetQ2(sumTree->Q2);

  //Calculate Q2 here using shifted lepton momentum
  Double_t* q2_old = new Double_t[sumTree->NSamples];
  Double_t* q2_new = new Double_t[sumTree->NSamples];
  for(int i = 0; i < sumTree->NSamples; i++){
    q2_old[i] = sumTree->Q2[i];
    q2_new[i] = sumTree->Q2[i];
    if( q2_new[i] < -998 || event->sample == NULL )
      continue;
    q2_new[i] += ApplyCoulombShift(event, sumTree->Enu[i], sumTree->Pmu[i]/1.E3, sumTree->TgtMat[i], sumTree->ReactionCode[i], 1, q2_new[i]);
  }
  event->SetQ2(q2_new);

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
    sumTreeEnu     = sumTree->Enu[event->sample->sampleID];
    sumTreeTruthVtx = sumTree->TruthVtx[event->sample->sampleID];
    sumTreeNuPDG = sumTree->NeutrinoCode[event->sample->sampleID];
    if(sumTreeTruthVtx < 0){
      std::cout <<" SAMPLE ID " << event->sample->sampleID << std::endl;
      for(int iX = 0; iX < SampleId::kNSamples; iX++){
        std::cout << "SAMPLE[" << iX << "] " << sumTree->TruthVtx[iX] << std::endl;
      }
    }
  }/*else{
    std::cout <<" SAMPLE ID " << event->sample->sampleID << std::endl;
    for(int iX = 0; iX < SampleId::kNSamples; iX++){
      std::cout << "SAMPLE[" << iX << "] " << sumTree->TruthVtx[iX] << std::endl;
    }
    }*/
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
          //std::cout << "Event energy: " << mcEvents[i]->GetNuEnergy() <<std::endl;
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
#ifdef DEBUGWEIGHTS
#pragma message "DEBUGING THE WEIGHTS"
    std::cout << "Event #" << i << std::endl;
#endif      
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

      //           //TODO: For debugging. 
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
                   //std::cout << "XSec norm parameter index: " << mcEvents[i]->GetXsecNormParameter() << std::endl; 
                   for (int idx = 0; idx < mcEvents[i]->GetXsecNormParameterSize(); idx++) {
                     std::cout << "XSec norm parameter index: " << mvEvents[i]->xsecNormParameter[idx] <<std::endl;
                   }
                   debugPrinted = true;
                   lastDebug = i;
                   }*/
			

    }
  }

}

void BANFFInterfaceBase::ThrowToys(int nToys, std::string outputFileName, bool resetPrior){


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
    if(toy_iter%100==0){
      std::cout << "Toy " << toy_iter << std::endl;
    }

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
//        std::cout<<"FLUX "<< fitParameters->GetParamName(i)<<std::endl;
//        std::cout << "value = " << fitParameters->GetCurrentValue(i) << std::endl;
        flux_iter++;
      } 

      //Save the XSec parameter values (both Response and Normalization)
      //to the xsec parameter TVectorD.
      else if(fitParameters->GetParamType(i)==XSECRESP || fitParameters->GetParamType(i)==XSECNORM || fitParameters->GetParamType(i)==XSECFUNC){
        (*(thrown_xsec_parms[toy_iter]))(xsec_iter) = fitParameters->GetCurrentValue(i);
//        std::cout<<"XSEC "<< fitParameters->GetParamName(i)<<std::endl;
//        std::cout << "value = " << fitParameters->GetCurrentValue(i) << std::endl;
        xsec_iter++;
      } 

      //Save the detector parameter values to the detector parameter
      //TVectorD.
      else if(fitParameters->GetParamType(i)==DET){
        (*(thrown_det_parms[toy_iter]))(det_iter) = fitParameters->GetCurrentValue(i);
//        std::cout<<"DET "<< fitParameters->GetParamName(i)<<std::endl;
//        std::cout << "value = " << fitParameters->GetCurrentValue(i) << std::endl;
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

    //Reset the parameter values to the priors, so that the MC fake data is generated at the nominal values
    if( resetPrior ){
      fitParameters->ResetValues();
    }
    //        fitParameters->PrintCurrentValues();


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
  if     ( OnlySampleContrib && !OnlyPenaltyContrib) return (totalSampleContrib);
  else if(!OnlySampleContrib &&  OnlyPenaltyContrib) return (fitParameters->CalcChi2());
  else                                               return (totalSampleContrib + fitParameters->CalcChi2());
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
    std::string mcName   = samples[si]->name + "_nomMC";
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

    if(fitParameters->GetParamType(i) == DET) continue;
    //Get this parameter's name, nominal value, i.e. its current value,
    //and what constitutes 1 sigma.
    std::string paramName = fitParameters->GetParamName(i);

    //If it's SF_RFG or RPA don't do anything.
    if((paramName == "SF_RFG") || (paramName == "RPA_C") || (paramName == "COH_BS")){
      continue;
    }

    double paramNominal = fitParameters->GetCurrentValue(i);
    double paramSigma = fitParameters->GetParameterError(i); 
    std::cout << "=======================================================" << std::endl;
    std::cout << "Name\tNominal\tSigma" << std::endl;
    std::cout << paramName << "\t" << paramNominal << "\t" << paramSigma << std::endl;
    for(int j = 0; j < nSigmaValues; j++){

      //Set the parameter value.
      fitParameters->SetParameterValue(i, paramNominal + sigmaValues[j]*paramSigma);
      if(fitParameters->GetCurrentValue(i) < fitParameters->GetLowerBound(i)){
        std::cout << "The current value is smaller than the lower bound:"
                  << fitParameters->GetCurrentValue(i) << " < "
                  << fitParameters->GetLowerBound(i) << std::endl;
        std::cout << "Putting the parameter in the limit." << std::endl;
        fitParameters->SetParameterValue(i, fitParameters->GetLowerBound(i));
      }
      if(fitParameters->GetCurrentValue(i) > fitParameters->GetUpperBound(i)){
        std::cout << "The current value is bigger than the lower bound:"
                  << fitParameters->GetCurrentValue(i) << " > "
                  << fitParameters->GetUpperBound(i) << std::endl;
        std::cout << "Putting the parameter in the limit." << std::endl;
        fitParameters->SetParameterValue(i, fitParameters->GetUpperBound(i));
      }
      std::cout << "Calculating the prediction for " << paramName << " at "
                << fitParameters->GetCurrentValue(i) << std::endl;

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
