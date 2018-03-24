#define RUNSYST_NEW_CXX

#include "AnalysisManager.hxx"
#include "ND280AnalysisUtils.hxx"
#include "ToyMakerExample.hxx"
#include "Parameters.hxx"
#include "MultiThread.hxx"
//#include "CategoriesUtils.hxx"
//#include "DataClasses.hxx"

#include "TCanvas.h"
#include "TH1F.h"
#include "TRandom3.h"
#include "TDirectory.h"
#include "TGeoManager.h"

#include <vector>
#include <sys/time.h>
#include <unistd.h>

const Int_t nWeights = 21;
const Int_t nToys = 0;//1000;
const Int_t debug = 0;

std::string GetMCGeoPositionPath(TGeoManager* const thisGeoManger,const TLorentzVector& checkPosition);
std::vector<std::string> SplitString(const std::string &inString, char SplitBy);
Int_t IsWaterP0Dule(TGeoManager* const tmpGeoManger,const TLorentzVector& StartPosition);
Bool_t IsAntiNu(const TString& fileName);
//determines if NC, CC0Pi, CC1Pi, CCOther

int main(int argc, char *argv[]){

  std::string programName = argv[0];
  std::string paramFile = "";
  std::string inputFileName = "";
  std::string inputFileType = "kHighlandTree";
  std::string outputFileName= "";

  Int_t nmax = 100000000;
  Int_t ntoys;
  Int_t preload=1;

  Bool_t isAntiNu = false;
  Bool_t isData = false;

  TGeoManager* geoManager = NULL;

  if(argc < 4){
    std::cerr << "You have to specify: RunSyst_New.exe -i inputfile.root -o outputfile.root (-n nevents)" << std::endl;
    throw;
  }
  for (;;) {
    int c = getopt(argc, argv, "n:o:i:d");
    if (c < 0)
      break;
    switch (c) {
      case 'n': {
        std::istringstream tmp(optarg);
        tmp >> nmax;
        break;
      }
      case 'o': {
        outputFileName = optarg;
        break;
      }
      case 'i': {
        inputFileName = optarg;
        break;
      }
      case 'd': 
      {
	isData = true;		
	break;
      }
      default: {
        std::cerr << optarg << " is an unknown option" << std::endl;
        throw;
      }
    }
  }

  std::cout << "is Data: " << isData << std::endl;
    
  preload=0;
#ifndef MULTITHREAD
  if (preload==1){
    std::cout << "For preloading mode you must uncomment the line #define MULTITHREAD in psycheCore/vXrY/srx/MultiThread.hxx and recompile everything !!!" << std::endl;
    return 0;
  }
#else
  if (preload==0){
    std::cout << "For interactive mode you must comment out the line #define MULTITHREAD psycheCore/vXrY/srx/MultiThread.hxx and recompile everything !!!" << std::endl;
    return 0;
  }
#endif
  
  // Read the parameters files following the package hierarchy
  // first the top level package. Set the parameters as fixed
  ND::params().LoadParametersFiles(anaUtils::GetPackageHierarchy(), true);

  // Make sure no parameters have been accessed yet
  ND::params().SetReadParamOverrideFilePointPassed();

  // Parameters to control the systematics

  bool applyVariationSystematics  = false;
  bool applyWeightSystematics     = false;
  bool applyFluxWeightSystematics = false;
  bool RunAllSyst                 = false;
  bool RunOnInidividualSyst       = false;
  bool ThrowToys                  = false;

  if(!isData)
  {
    applyVariationSystematics  = (bool)ND::params().GetParameterI("psycheSteering.Systematics.ApplyVariationSystematics");
    applyWeightSystematics     = (bool)ND::params().GetParameterI("psycheSteering.Systematics.ApplyWeightSystematics");
    applyFluxWeightSystematics = (bool)ND::params().GetParameterI("psycheSteering.FluxWeighting.Enable");
    RunAllSyst                 = (bool)ND::params().GetParameterI("psycheSteering.RunSyst.RunAllSyst");
    RunOnInidividualSyst       = (bool)ND::params().GetParameterI("psycheSteering.RunSyst.RunOnInidividualSyst");
    ThrowToys                  = (bool)ND::params().GetParameterI("psycheSteering.RunSyst.ThrowToys");
    if(!applyFluxWeightSystematics && !applyVariationSystematics && !applyWeightSystematics) { 
      std::cout<<" no systematics is set to be applied "<<std::endl;
      throw;
    }
  }

  
  TFile* inputFile = TFile::Open(inputFileName.c_str());
  if(!inputFile)
  {
      std::cerr << "No input file, exiting..." << std::cerr; 
      throw;
  }
  TTree* RTV       = static_cast<TTree*>(inputFile->Get("NRooTrackerVtx"));
  if(!RTV && !isData)
  { 
      std::cerr << "No NRooTrackerVtx in the file, exiting..." << std::cerr; 
      throw;
  }
  inputFile->Close();

  // Initialize clock
  timeval tim;
  gettimeofday(&tim, NULL);
  double t0=tim.tv_sec+(tim.tv_usec/1000000.0);
  std::cout<<" DEFINING MANAGER "<<std::endl;
  AnalysisManager _man;

  // Make and fill the EventSummary even when the selection is not passed.
  if(ND::params().GetParameterI("psycheSteering.Selections.ForceFillEventSummary"))
    _man.sel().SetForceFillEventSummary(true);

  // Initialize the InputManager by specifying the input type and the input file
  if (!_man.input().Initialize(inputFileName,inputFileType, false)) return false;
  std::vector<EventVariationBase*> allVar  = _man.evar().GetEventVariations();
  std::vector<SystematicBase*>     allSyst = _man.syst().GetSystematics();
  isAntiNu = IsAntiNu(inputFileName);
  _man.sel().DumpSelections();
  
  ToyMaker* toyMaker = NULL;
  ToyMaker* ZeroVarToyMaker = NULL;
  ToyExperiment* ZeroVarToy = NULL;
  std::vector<float> weights;
  if(!isData)
  {
    // Create a ToyMaker to configure the toy experiment. Initialize it with a random seed
    toyMaker = new ToyMakerExample((UInt_t)ND::params().GetParameterI("psycheSteering.Systematics.RandomSeed"), 
    (bool)  ND::params().GetParameterI("psycheSteering.Systematics.ZeroVariation"));
    // Create and fill the Toy experiment with the appropriate format (number of systematics and number of parameters for each systematic)
    std::cout << "Creating " <<  nToys << " toy experiments" << std::endl;
    toyMaker->CreateToyExperiments(nToys, _man.syst().GetSystematics()); 

    ZeroVarToyMaker = new ToyMakerExample(1, true);
    ZeroVarToyMaker->CreateToyExperiments(1, _man.syst().GetSystematics()); 
    ZeroVarToy = ZeroVarToyMaker->GetToyExperiment(0);

    // Print the steps for the different selections
    if (debug>0){
      //_man.sel().GetSelection("kTrackerNumuCC")->DumpSteps();
      //_man.sel().GetSelection("kTrackerNumuCCMultiPi")->DumpSteps();
      _man.syst().DumpVariationSystematics();
      _man.syst().DumpWeightSystematics();
    }

  }
  if(!isData)
  {
    std::cout << "Preloading data events!" << std::endl;
    if (!_man.ReadEvents(inputFileName, nmax)) return 0;
    if(nmax < 0) nmax = _man.GetEntries();
    _man.SetNEventsToProcess(nmax);
  }
  else {
    if(preload){
      std::cout <<" preloading!!!! "<<std::endl;
      // Preload nmax events from the file
      if (!_man.ReadEvents(inputFileName, nmax)) return 0;
      if(nmax < 0) nmax = _man.GetEntries();
      _man.SetNEventsToProcess(nmax);
    }
    else{
      // Create the array of PreviousToyBox
      std::cout << "Creating to the box array" << std::endl;
      _man.sel().CreateToyBoxArray(nmax);
    }
  }

  //std::cout << "The number of events = " << nmax << std::endl;

  //if(!(nToys <= 1000)) nToys = 1000;

  Int_t Run         = -999;
  Int_t SubRun      = -999;
  Int_t EventNumber = -999;
  Int_t isBKG        = -999;
  Int_t isOOF        = -999;
  Int_t isNoTrueVtx = -999;
  Int_t isCCzeroPi  = -999;
  Int_t isCConePi   = -999;
  Int_t isCCOther   = -999;
  Int_t isOther = -999;

  Int_t    TrueVertexIDNom = -999;
  Int_t    SelectionNom    = -999;
  Double_t TrueEnuNom      = -999;
  Int_t    TrueNuPDGNom    = -999;
  Double_t LeptonMomNom    = -999;
  Double_t LeptonCosNom    = -999;
  Double_t WeightNom       = -999;
  Double_t FluxWeightNom   = -999;
  Double_t tVtxX = -999;
  Double_t tVtxY = -999;
  Double_t tVtxZ = -999;
  Double_t vtxX = -999;
  Double_t vtxY = -999;
  Double_t vtxZ = -999;
  Int_t onWaterTarget = -999;

  Int_t    Toy            [nToys];
  Int_t    TrueVertexIDToy[nToys];
  Int_t    SelectionToy   [nToys];
  Double_t TrueEnuToy     [nToys];
  Int_t    TrueNuPDGToy   [nToys];
  Double_t LeptonMomToy   [nToys];
  Double_t LeptonCosToy   [nToys];
  Double_t WeightToy      [nToys];
  Int_t    nWeightSyst = _man.eweight().GetNEnabledEventWeights();

  if(nWeights != nWeightSyst){
    std::cerr << "nWeights != nWeightSyst (" << nWeights << " != " << nWeightSyst << ")" << std::endl;
    std::cerr << "Change the hard coded value at the beginning of RunSyst_New.cxx" << std::endl;
    throw;
  }

  std::vector<EventWeightBase*>::iterator EW_iterator;
  for (EW_iterator = _man.eweight().GetEventWeights().begin(); EW_iterator != _man.eweight().GetEventWeights().end(); ++EW_iterator) {
    EventWeightBase* ewb = *EW_iterator;
    if(!ewb) continue;
    if(!(ewb->IsEnabled())) continue;
    std::cout << ewb->GetName() << std::endl;
  }
  
  
  Double_t WeightIndToy [nWeights][nToys];
  Double_t FluxWeightToy[nToys];
  
  for (int iToy = 0; iToy < nToys; ++iToy) {
    Toy            [iToy] = -999;
    TrueVertexIDToy[iToy] = -999;
    SelectionToy   [iToy] = -999;
    LeptonMomToy   [iToy] = -999;
    TrueEnuToy     [iToy] = -999;
    TrueNuPDGToy   [iToy] = -999;
    LeptonCosToy   [iToy] = -999;
    WeightToy      [iToy] = -999;
    for (int iSyst = 0; iSyst < nWeights; ++iSyst) {
      WeightIndToy[iSyst][iToy] = -999;
    }    
  }

  TTree *tree = NULL;
  TFile *outfile = new TFile(outputFileName.c_str(), "RECREATE");
  std::vector<std::string> systnametree;
  for (std::vector<EventVariationBase*>::iterator it = allVar.begin(); it != allVar.end(); ++it) {
    if(*it == NULL) continue;
    if(!(*it)->IsEnabled()) continue;
    if(RunOnInidividualSyst)
      systnametree.push_back(std::string((*it)->GetName()));
  }
 
  if(RunOnInidividualSyst)
    systnametree.push_back("weight");
  if(RunAllSyst)
    systnametree.push_back("all");
  if(!ThrowToys){
    systnametree.clear();
    systnametree.push_back("nominal");
  }
  std::vector<int> npassed(SampleId::kNSamples, 0);
    
  for (std::vector<std::string>::iterator it = systnametree.begin(); it != systnametree.end(); ++it){
    std::string syst_name = (*it);
    std::cout << "Running over " << syst_name << std::endl;
    bool WeightSyst = (syst_name == "weight");
    bool AllSyst    = (syst_name == "all");
    
    outfile->cd();
    tree = new TTree(syst_name.c_str(),syst_name.c_str());
    tree->Branch("Run",             &Run,             "Run/I"        );
    tree->Branch("SubRun",          &SubRun,          "SubRun/I"     );
    tree->Branch("EventNumber",     &EventNumber,     "EventNumber/I");

    tree->Branch("SelectionNom",    &SelectionNom,    "SelectionNom/I"   );
    tree->Branch("isBKG",       &isBKG,      "isBKG/I");
    tree->Branch("isOOF",       &isOOF,      "isOOF/I");
    tree->Branch("isNoTrueVtx",       &isNoTrueVtx,      "isNoTrueVtx/I");
    tree->Branch("isOther",       &isOther,      "isOther/I");
    tree->Branch("isCCzeroPi", &isCCzeroPi,"isCCzeroPi/I");
    tree->Branch("isCConePi",  &isCConePi, "isCConePi/I");
    tree->Branch("isCCOther",  &isCCOther, "isCCOther/I");

    tree->Branch("TrueEnuNom",      &TrueEnuNom,      "TrueEnuNom/D"     );
    tree->Branch("TrueNuPDGNom",    &TrueNuPDGNom,    "TrueNuPDGNom/I"   );
    tree->Branch("TrueVertexIDNom", &TrueVertexIDNom, "TrueVertexIDNom/I");
    tree->Branch("LeptonMomNom",    &LeptonMomNom,    "LeptonMomNom/D"   );
    tree->Branch("LeptonCosNom",    &LeptonCosNom,    "LeptonCosNom/D"   );
    tree->Branch("WeightNom",       &WeightNom,       "WeightNom/D"      );
    tree->Branch("FluxWeightNom",   &FluxWeightNom,   "FluxWeightNom/D"  );

    tree->Branch("vtxX" ,&vtxX ,"vtxX/D");
    tree->Branch("vtxY" ,&vtxY ,"vtxY/D");
    tree->Branch("vtxZ" ,&vtxZ ,"vtxZ/D");
    tree->Branch("tVtxX",&tVtxX,"tVtxX/D");
    tree->Branch("tVtxY",&tVtxY,"tVtxY/D");
    tree->Branch("tVtxZ",&tVtxZ,"tVtxZ/D");
    tree->Branch("onWaterTarget",&onWaterTarget,"onWaterTarget/I");
    
    int nt = nToys;
    if(ThrowToys){
      tree->Branch("nToys",          &nt,              "nToys/I");
      tree->Branch("Toy",             Toy,              Form("Toy[%d]/I"            , nToys));
      tree->Branch("TrueVertexIDToy", TrueVertexIDToy,  Form("TrueVertexIDToy[%d]/I", nToys));
      tree->Branch("SelectionToy",    SelectionToy,     Form("SelectionToy[%d]/I"   , nToys));
      tree->Branch("TrueEnuToy",      TrueEnuToy,       Form("TrueEnuToy[%d]/D"     , nToys));
      tree->Branch("TrueNuPDGToy",    TrueNuPDGToy,     Form("TrueNuPDGToy[%d]/I"   , nToys));
      tree->Branch("LeptonMomToy",    LeptonMomToy,     Form("LeptonMomToy[%d]/D"   , nToys));
      tree->Branch("LeptonCosToy",    LeptonCosToy,     Form("LeptonCosToy[%d]/D"   , nToys));
      if(!WeightSyst)
        tree->Branch("WeightToy",     WeightToy,        Form("WeightToy[%d]/D", nToys));
      else{
        tree->Branch("nSyst",        &nWeightSyst,      "nSyst/I");
        tree->Branch("WeightIndToy",  WeightIndToy,     Form("WeightIndToy[%d][%d]/D", nWeightSyst, nToys));
      }
    
      tree->Branch("FluxWeightToy",   FluxWeightToy,    Form("FluxWeightToy[%d]/D",nToys));
    }
    std::vector<float> weights;
    Weight_h totalweight;
    Weight_h fluxWeightSyst;
    int rtvi = 0;
    int prevTruthID = -1;
    int prevRTV = -1;
    
    //  ProfilerStart("prof.out");
    //--------- Loop over entries in the tree ----------------------------------
    // Get the number of entries in the tree
    nmax = std::min(nmax, (int)_man.input().GetEntries());

    if(gDirectory->FindObjectAny("ND280Geometry"))
	std::cout << "Found ND280Geometry" << std::endl;

    std::cout << "RunSyst: loop over " << nmax << " entries" << std::endl;
  
    Long64_t entry = 0;
    while (entry < nmax) {
      
      if(!geoManager && !isData)
	  geoManager = static_cast<TGeoManager*>(gDirectory->FindObjectAny("ND280Geometry"));

      if(entry % 100 == 0)
        std::cout << "Progress " << 100.*entry/nmax << "%" << std::endl;

      // 1. ====================================================================
      // Fill the event structure

      bool FillTree = false;
      AnaEventB* event = NULL;

      if(preload)
        event=  _man.GetEvent(entry);
      else{
        event = static_cast<AnaEventB*>((_man.LoadSuperEvent(entry))->Event);
      }

std::cout << "got event" << std::endl;
      
      // Fill the EventBox
      if (!preload)
      {
        _man.sel().InitializeEvent(*event);
        std::cout << "initialized" << std::endl;
      }
      
      if(!isData)
      {
      
        // Run the  nominal selection
        // Create the SystBox array (only the first time it is called for each systematic)
        if (_man.evar().HasEventVariations()){
          // Create the SystBox array (only the first time it is called for each EventVariation)
          _man.evar().Initialize(nmax);
    
          // Initialize The SystBox for EventVariations
          _man.evar().InitializeEvent(_man.sel(),*event);
        }    

        if (_man.eweight().HasEventWeights()){
          // Create the SystBox array (only the first time it is called for each EventWeight)
          _man.eweight().Initialize(_man.sel(),nmax);
    
          // Initialize The SystBox for variation systematics
          _man.eweight().InitializeEvent(_man.sel(),*event);
        }    
      }
      // Initialize The SystBox for variation systematics
      //if (!preload)

      TrueEnuNom      = -999;
      TrueNuPDGNom    = -999;
      LeptonMomNom    = -999;
      LeptonCosNom    = -999;
      SelectionNom    = -999;
      FluxWeightNom   = -999;
      WeightNom       = -999;
      TrueVertexIDNom = -999;
      SelectionNom    = -999;
      tVtxX = -999; 
      tVtxY = -999;
      tVtxZ = -999;
      vtxX =  -999;
      vtxY =  -999;
      vtxZ =  -999;
      onWaterTarget = -1;
      isBKG = -999;
      isCCzeroPi = -999;
      isCConePi = -999;
      isCCOther = -999;
      isOOF = -999;
      isNoTrueVtx = -999;
      isOther = -999;
 
      //_man.syst().InitializeEventSystematics(_man.sel(),*event);
      bool passednom = false;

      if(!isData)
	passednom = _man.ProcessEvent(*ZeroVarToy, *event, totalweight, fluxWeightSyst);
      else
        passednom = _man.ProcessEvent(*event);
       
      if(passednom){
        FillTree=true;
	//AnaVertexB** Vertices = event->Vertices;
	const Int_t nParticles = event->nParticles;
	std::cout << "There are " << nParticles  << " particles in this event" <<std::endl;
        AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(event->Summary);
        if(!summary || !summary->EventSample)
	    continue;

        npassed[summary->EventSample]++;
	AnaParticleMomB* lepCand = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample]);
	AnaTrueVertexB* trVtx = static_cast<AnaTrueVertexB*>(summary->TrueVertex[summary->EventSample]);

	vtxX = summary->VertexPosition[summary->EventSample][0];
	vtxY = summary->VertexPosition[summary->EventSample][1];
	vtxZ = summary->VertexPosition[summary->EventSample][2];

        LeptonMomNom    = lepCand->Momentum;
        LeptonCosNom    = lepCand->DirectionStart[2];
        SelectionNom    = (Int_t)   summary->EventSample;
	if(!isData)
	{
          FluxWeightNom   = (Double_t)fluxWeightSyst.Correction;
          WeightNom       = (Double_t)totalweight.Correction;
          EventNumber     = (Int_t)   (*event).EventInfo.Event;
	}
        if(trVtx)
	{
	  tVtxX = trVtx->Position[0];
	  tVtxY = trVtx->Position[1];
	  tVtxZ = trVtx->Position[2];
          TrueVertexIDNom = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->GetTrueParticle()->VertexID; 
          TrueEnuNom      = (Double_t)(summary->TrueVertex[summary->EventSample]->NuEnergy);
          TrueNuPDGNom    = (Int_t)   (summary->TrueVertex[summary->EventSample]->NuPDG   );
          if(geoManager && (summary->EventSample == SampleId::kP0DNuMuCC || summary->EventSample == SampleId::kP0DNuMuBarCC))
          {
	      TLorentzVector start = trVtx->Position;
              Int_t tmp = IsWaterP0Dule(geoManager,start);
	      //onWaterTarget = (tmp - (tmp % 10)) / 10;
	      onWaterTarget = (tmp % 10);
          }
        }
      }
            
      /// 2. ====================================================================
      /// Loop over toy experiments
      for (Int_t iToy = 0; iToy < nToys && ThrowToys; iToy++){
        ToyExperiment* toy = toyMaker->GetToyExperiment(iToy);
        ToyExperiment* reducedtoy = new ToyExperiment(*toy);
        // Enable the appropriate systematics
        if(event->GetIsMC()){
  
          for (std::vector<SystematicBase*>::iterator it = allSyst.begin(); it != allSyst.end(); ++it) {
            SystematicBase* ZeroVarSyst = *it;

            if(!ZeroVarSyst)
              continue;

            if(WeightSyst && ZeroVarSyst->Type() == SystematicBase::kVariation){
              for(unsigned int iparam = 0; iparam < ZeroVarSyst->GetNParameters(); iparam++)
                reducedtoy->SetToyVariation(ZeroVarSyst->GetIndex(), iparam, 0, 1);
              continue;
            }
            // Doesnt deactivate if this is the current syst || Doesn't deactivate any if it's allsyst
            if(ZeroVarSyst->Name() == syst_name || AllSyst || WeightSyst)
              continue;
            for(unsigned int iparam = 0; iparam < ZeroVarSyst->GetNParameters(); iparam++)
              reducedtoy->SetToyVariation(ZeroVarSyst->GetIndex(), iparam, 0, 1);
          }
        }

        Toy            [iToy] = iToy;
        SelectionToy   [iToy] = -999;
        TrueEnuToy     [iToy] = -999;
        TrueNuPDGToy   [iToy] = -999;
        LeptonMomToy   [iToy] = -999;
        LeptonCosToy   [iToy] = -999;
        WeightToy      [iToy] = -999;
        
        for (int iSyst = 0; iSyst < nWeights; ++iSyst) {
          WeightIndToy[iSyst][iToy] = -999;
        }
          
        FluxWeightToy  [iToy] = -999;
        TrueVertexIDToy[iToy] = -999;

        // 3. ====================================================================
        // Process the current event (bunch). That means applying the systematics, the selections and computing the weights
        bool passed = false;
        Weight_h* weights = new Weight_h[NMAXSYSTEMATICS];
        // std::cout << "entry " << entry << std::endl;
        
        if(!WeightSyst){
          passed = _man.ProcessEvent(*reducedtoy, *event, totalweight, fluxWeightSyst);
        }else{
          passed = _man.ProcessEventWithIndividualWeightSyst(*reducedtoy, *event, weights, fluxWeightSyst);
        }
        if(passed){
          FillTree=true;
          AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(event->Summary);
          if(summary && summary->EventSample){
            LeptonMomToy [iToy] = (Double_t)(static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->Momentum);
            LeptonCosToy [iToy] = (Double_t)(static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->DirectionStart[2]);
            SelectionToy [iToy] = (Int_t)   (summary->EventSample);
            FluxWeightToy[iToy] = (Double_t)(fluxWeightSyst.Correction);
            if(!WeightSyst)
              WeightToy[iToy]   = (Double_t)(totalweight.Systematic);
            else{
              
              int ind=0;
              for(UInt_t i = 0; i < NMAXSYSTEMATICS; ++i){
                if(weights[i].Systematic != -999){
                  WeightIndToy[ind][iToy] = weights[i].Systematic;
                  //std::cout << "WeightIndToy[" << ind << "][" << iToy << "] " << WeightIndToy[ind][iToy] <<std::endl;
                  ind++;
                }
              }
              if(ind!=nWeights){
                std::cerr << "something wrong is happening" << std::endl;
                throw; 
              }
            }
            if(summary->TrueVertex[summary->EventSample]){
              TrueVertexIDToy[iToy] = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->GetTrueParticle()->VertexID;
              TrueEnuToy     [iToy] = (Double_t)(summary->TrueVertex[summary->EventSample]->NuEnergy);
              TrueNuPDGToy   [iToy] = (Int_t)   (summary->TrueVertex[summary->EventSample]->NuPDG   );
            }
          }
        }
        delete reducedtoy;
      } 
      
      
      // Delete the SystBox for the current event
      if (event->GetIsMC() && _man.evar().GetNEnabledEventVariations()>0)
        _man.evar().FinalizeEvent(*event);

      // Delete the SystBox for the current event
      if (event->GetIsMC() && _man.eweight().GetNEnabledEventWeights()>0)
        _man.eweight().FinalizeEvent(*event);
      _man.sel().FinalizeEvent(*event);

      if(FillTree)
        tree->Fill();
      
    }
    outfile->cd();
    tree->Write();
  }

  if((bool)ND::params().GetParameterI("psycheSteering.RunSyst.SaveAllTheNRooVtx")){
    std::cout << "Copying the NRooTrackerVtx Tree" << std::endl;
    inputFile = new TFile(inputFileName.c_str(), "READ");
    RTV = (TTree*)(inputFile->Get("NRooTrackerVtx"));
    outfile->cd();
    RTV->CloneTree()->Write();
  }

  outfile->Close();
  inputFile->Close();
  gettimeofday(&tim, NULL);
  double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
  
  std::cout << "time: " << t1-t0 << std::endl;
  
  //_man.sel().PrintStatistics();
  
  //  ProfilerStop();
  for (unsigned int i = 0; i < SampleId::kNSamples; ++i){
    std::cout << "# events passing selection, Selection "<< SampleId::ConvertSample((SampleId::SampleEnum)i) << ": " << npassed[i] << std::endl;
  }
}

Int_t IsWaterP0Dule(TGeoManager* const tmpGeoManger,const TLorentzVector& StartPosition)
{

  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  // * NOTICE : There is a special case where the P0Dule number will return 40 *
  // *          this means that the position                                   *
  // *          is in the 39 CECal_0/P0Dule_6/Epoxy/DownstreamCover_0          *
  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  //
  // this funtion will return an int such that (return value)%10 will be 1 if the position is in water and 0 if not
  // and ((return value)-(return value)%10)/10 will be the P0Dule index
  // P0Dule index starts at 0 (most upstream P0Dule of the upstream P0D ecal) and ends at 39 (most downstream P0Dule
  // of the downstream P0D ecal)
  // The central water target consists of P0Dules 8-32 inclusive

  //P0DuleOrLayer: 1= p0dule number, 2=layer X(1)/Y(2), 3=LayerIndex (1 -> 80)

  Int_t IsOnWater = -1;
  // -1 = not in the P0D, -2 = in the P0D not in a SuperP0Dule
  // -3 = in SuperP0Dule but not in a P0Dule or WT or Radiator
  // -4 = in a P0Dule but not in Epoxy (where the bars are)

  std::string PositionPath = GetMCGeoPositionPath( tmpGeoManger, StartPosition );
  //cout<<PositionPath<<endl;
  std::string LeftWater = "LeftTarget";
  std::string RightWater = "RightTarget";

  IsOnWater=0;

  size_t LeftOnWater = PositionPath.find(LeftWater);
  //if(LeftOnWater!=std::string::npos) cout << "on Left Water Target" << endl;
  //if(LeftOnWater!=std::string::npos) return 1;
  if(LeftOnWater!=std::string::npos) IsOnWater=1;
  size_t RightOnWater = PositionPath.find(RightWater);
  //if(RightOnWater!=std::string::npos) cout << "on Right Water Target" << endl;
  //if(RightOnWater!=std::string::npos) return 1;
  if(RightOnWater!=std::string::npos) IsOnWater=1;

  //const char* PosPathStr = PositionPath.c_str();
  //const int LeftOnWater = strstr(PosPathStr, "LeftTarget");
  //const int RightOnWater = strstr(PosPathStr, "RightTarget");
  //cout << LeftOnWater << " " << RightOnWater << endl;

  std::vector<std::string> DetectorVolumes
    = SplitString( PositionPath.c_str(), '/');

  //Element 5 should be /P0D/
  //Element 6 should be /USECal_0/ or /USTarget_0/ or /CTarget_0/ or /CECal_0/
  //Element 7 should be /P0Dule_#/ or /Radiator_#/ or /Target_#/ where # gives the p0dule number with in the superp0dule (0-6 for ecals and 0-12 for targets)

  Int_t returnP0DuleNumber = -9999;
  Int_t returnXorYLayer = -9999;

  if (DetectorVolumes.size() < 6 )
    return (-1);

  if (DetectorVolumes[5] != "P0D_0")
    return (-1);

  if (DetectorVolumes.size() == 6)
    return (-2);

  if (DetectorVolumes[6]!="USECal_0" && DetectorVolumes[6]!="CECal_0" && DetectorVolumes[6]!="USTarget_0" && DetectorVolumes[6]!="CTarget_0")
    return (-2);

  if (DetectorVolumes.size() == 7)
    return (-3);

  std::vector<std::string> tmpInSuperP0Dule
    = SplitString(DetectorVolumes[7].c_str(),'_');

  if (tmpInSuperP0Dule[0]!="P0Dule" && tmpInSuperP0Dule[0]!="Radiator" && tmpInSuperP0Dule[0]!="Target")
    return (-3);

  Int_t SuperP0DuleOffset=0;
  if(DetectorVolumes[6] == "USTarget_0")
    SuperP0DuleOffset = 7;
  if(DetectorVolumes[6] == "CTarget_0")
    SuperP0DuleOffset = 20;
  if(DetectorVolumes[6] == "CECal_0")
    SuperP0DuleOffset = 33;

  returnP0DuleNumber = atoi(tmpInSuperP0Dule[1].c_str());

  Int_t P0DuleByP0DuleOffset = 0;
  if(tmpInSuperP0Dule[0]=="Radiator" || tmpInSuperP0Dule[0]=="Target") {
    returnXorYLayer = 1;
    if (DetectorVolumes[6] != "CECal_0")
      P0DuleByP0DuleOffset = 1;
  }

  if(tmpInSuperP0Dule[0] == "P0Dule") {

    if(DetectorVolumes.size() == 8)
      returnXorYLayer = -4;
      //      return ( -4 );

    if(DetectorVolumes.size() > 8) {

      if(DetectorVolumes[8] != "Epoxy_0") {

        if(DetectorVolumes[8]=="BottomLIFrame_0" || DetectorVolumes[8]=="BottomLICover_0" || DetectorVolumes[8]=="TopMPPCFrame_0")
          returnXorYLayer = 1;

        if(DetectorVolumes[8]=="SideLIFrame_0" || DetectorVolumes[8]=="SideLICover_0" || DetectorVolumes[8]=="SideMPPCFrame_0")
          returnXorYLayer = 2;
      }

      if(DetectorVolumes[8] == "Epoxy_0") {

        if(DetectorVolumes.size() == 9)
          returnXorYLayer = 2;

        if(DetectorVolumes.size() > 9){

          if(DetectorVolumes[9]=="X_0" ||DetectorVolumes[9]=="UpstreamCover_0")
            returnXorYLayer = 1;

          if(DetectorVolumes[9] == "Y_0")
            returnXorYLayer = 2;

          if(DetectorVolumes[9] == "DownstreamCover_0") {
            returnXorYLayer = 1;
            P0DuleByP0DuleOffset = 1;
          }

        } // if > 9
      } // Epoxy_0
    } // if > 8
  } // if P0Dule

  return (returnP0DuleNumber+ SuperP0DuleOffset + P0DuleByP0DuleOffset)*10 + IsOnWater;
}


std::vector<std::string> SplitString(const std::string &inString, char SplitBy)
{
  std::vector<std::string> outStringVec;

  std::stringstream WorkOnThisString( inString );
  std::string tmpStringPart;

  while ( std::getline( WorkOnThisString, tmpStringPart, SplitBy) ) {
    outStringVec.push_back( tmpStringPart );
  }

  return (outStringVec );
}


std::string GetMCGeoPositionPath(TGeoManager* const thisGeoManger,const TLorentzVector& checkPosition)
{
  thisGeoManger->InitTrack( checkPosition.X(), checkPosition.Y(), checkPosition.Z(), 0, 0, 1); // 0, 0, 1 = the direction vector
  std::string tmpMCPath = thisGeoManger->GetPath();
  return ( tmpMCPath );
}


Bool_t IsAntiNu(const TString& fileName){
    Bool_t isantinu = false;
    if(fileName.Contains("5c") || fileName.Contains("7b")
	    || fileName.Contains("6b") || fileName.Contains("6c")
	    || fileName.Contains("6c") || fileName.Contains("6d")){
	isantinu = true; 
    }
    return isantinu;
}
