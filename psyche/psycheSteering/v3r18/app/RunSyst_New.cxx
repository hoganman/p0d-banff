#include "AnalysisManager.hxx"
#include "ND280AnalysisUtils.hxx"
#include <sys/time.h>
#include <unistd.h>

#include "ToyMakerExample.hxx"
#include <Parameters.hxx>
#include <TCanvas.h>
#include <TH1.h>
#include <TH1F.h>
#include "TRandom3.h"
#include "MultiThread.hxx"

int main(int argc, char *argv[]){

  std::string programName = argv[0];
  std::string paramFile = "";
  int nmax = 100000000;
  Int_t ntoys=1;
  std::string inputFileName = "";
  std::string inputFileType = "kHighlandTree";
  std::string outputFileName= "";
  Int_t debug = 0;
  Int_t preload=1;
  for (;;) {
    int c = getopt(argc, argv, "n:t:d:o:p:");
    if (c < 0)
      break;
    switch (c) {
    case 'n': {
      std::istringstream tmp(optarg);
      tmp >> nmax;
      break;
    }
    case 't': {
      std::istringstream tmp(optarg);
      tmp >> ntoys;
      break;
    }
    case 'd': {
      std::istringstream tmp(optarg);
      tmp >> debug;
      break;
    }
    case 'o': {
      outputFileName= optarg;
      // TFile *test=TFile::Open(outputFileName.c_str(),"READ");
      // if(test){
      //   std::cout<<" OUTPUT ALREADY EXISTS ! please give another name..."<<std::endl;
      //   //return 0;
      // }
      // test->Close();
      // delete test;
      // break;
    }
    case 'p': {
      std::istringstream tmp(optarg);
      tmp >> preload;
      break;
    }
    default: {
    }
    }
  }
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
  
  // get the input file name
  inputFileName = argv[optind++];

  // Read the parameters files following the package hierarchy
  // first the top level package. Set the parameters as fixed
  ND::params().LoadParametersFiles(anaUtils::GetPackageHierarchy(), true);

  // Make sure no parameters have been accessed yet
  ND::params().SetReadParamOverrideFilePointPassed();

  // Parameters to control the systematics
  bool applyVariationSystematics = (bool)ND::params().GetParameterI("psycheSteering.Systematics.ApplyVariationSystematics");
  bool applyWeightSystematics    = (bool)ND::params().GetParameterI("psycheSteering.Systematics.ApplyWeightSystematics");
  bool applyFluxWeightSystematics = (bool)ND::params().GetParameterI("psycheSteering.FluxWeighting.Enable");
  bool RunOnlyAllSyst  = (bool)ND::params().GetParameterI("psycheSteering.RunSyst.RunOnlyAllSyst");
  if(!applyFluxWeightSystematics && !applyVariationSystematics && !applyWeightSystematics) { 
    std::cout<<" no systematics is set to be applied "<<std::endl;
    return false;
  }
  TFile* inputFile = new TFile(inputFileName.c_str(), "READ");
  TTree* RTV = static_cast<TTree*>(inputFile->Get("NRooTrackerVtx"));
  if(!RTV){ std::cerr << "No NRooTrackerVtx in the file, exiting." << std::cerr; throw;}
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
  std::vector<SystematicBase*> allSystWithNULL = _man.syst().GetSystematics();
  std::vector<SystematicBase*> allSyst;


  // Create a ToyMaker to configure the toy experiment. Initialize it with a random seed
  ToyMaker* toyMaker = new ToyMakerExample((UInt_t)ND::params().GetParameterI("psycheSteering.Systematics.RandomSeed"), 
                                           (bool)  ND::params().GetParameterI("psycheSteering.Systematics.ZeroVariation"));
  // Create and fill the Toy experiment with the appropriate format (number of systematics and number of parameters for each systematic)
  std::cout << "Creating " <<  ntoys << " toy experiments" << std::endl;
  toyMaker->CreateToyExperiments(ntoys, _man.syst().GetSystematics()); 

  ToyMaker* ZeroVarToyMaker = new ToyMakerExample(1, true);
  ZeroVarToyMaker->CreateToyExperiments(1, _man.syst().GetSystematics()); 
  ToyExperiment* ZeroVarToy = ZeroVarToyMaker->GetToyExperiment(0);

  std::vector<float> weights;
  // Print the steps for the different selections
  if (debug>0){
    _man.sel().GetSelection("kTrackerNumuCC")->DumpSteps();
    _man.sel().GetSelection("kTrackerNumuCCMultiPi")->DumpSteps();
    _man.syst().DumpVariationSystematics();
    _man.syst().DumpWeightSystematics();
  }

  if(preload){
    std::cout<<" preloading!!!! "<<std::endl;
    // Preload nmax events from the file
    if (!_man.ReadEvents(inputFileName, nmax)) return 0;
    if(nmax < 0) nmax = _man.GetEntries();
    _man.SetNEventsToProcess(nmax);
  }
  else{
    // Create the array of PreviousToyBox
    _man.sel().CreateToyBoxArray(nmax);
  }

  Int_t nToys = ntoys;
  if(!(nToys <= 1000)) nToys = 1000;

  Int_t Run         = -999;
  Int_t SubRun      = -999;
  Int_t EventNumber = -999;

  Int_t    TrueVertexIDNom;
  Int_t    SelectionNom   ;
  Double_t LeptonMomNom   ;
  Double_t LeptonCosNom   ;
  Double_t WeightNom      ;
  Double_t FluxWeightNom  ;
  
  Int_t    Toy            [1000];
  Int_t    TrueVertexIDToy[1000];
  Int_t    SelectionToy   [1000];
  Double_t LeptonMomToy   [1000];
  Double_t LeptonCosToy   [1000];
  Double_t WeightToy      [1000];
  Double_t FluxWeightToy  [1000];
  
  for (int iToy = 0; iToy < nToys; ++iToy) {
    Toy            [iToy] = -999;
    TrueVertexIDToy[iToy] = -999;
    SelectionToy   [iToy] = -999;
    LeptonMomToy   [iToy] = -999;
    LeptonCosToy   [iToy] = -999;
    WeightToy      [iToy] = -999;
  }

  //#endif
  TTree *tree;
  TFile *outfile = new TFile(outputFileName.c_str(),"RECREATE");
  std::vector<std::string> systnametree;
  for (std::vector<SystematicBase*>::iterator it = allSystWithNULL.begin(); it != allSystWithNULL.end(); ++it) {
    if(*it == NULL) continue;
    if(!(*it)->IsEnabled()) continue;
    systnametree.push_back(std::string((*it)->GetName())+"_syst");
    allSyst.push_back(*it);
  }
  
  systnametree.push_back("all_syst");
  
  std::vector<Int_t> npassed(allSyst.size() + 1,0);
  
  for (unsigned int isyst = 0; isyst < allSyst.size()+1; isyst++){

    if(isyst < allSyst.size() && RunOnlyAllSyst)
      continue;
    
    SystematicBase* currentSyst = NULL;

    if(isyst < allSyst.size()){
      currentSyst = allSyst[isyst];
      std::cout << "RunSyst: Process systematic " << currentSyst->GetName() << std::endl;
    }else{
      std::cout << "RunSyst: All systematics " << std::endl;
    }
    
    outfile->cd();
    tree = new TTree(systnametree[isyst].c_str(),systnametree[isyst].c_str());
    tree->Branch("Run",            &Run,           "Run/I");
    tree->Branch("SubRun",         &SubRun,        "SubRun/I");
    tree->Branch("EventNumber",    &EventNumber,   "EventNumber/I");

    tree->Branch("SelectionNom",    &SelectionNom,    "SelectionNom/I");
    tree->Branch("TrueVertexIDNom", &TrueVertexIDNom, "TrueVertexIDNom/I");
    tree->Branch("LeptonMomNom",    &LeptonMomNom,    "LeptonMomNom/D");
    tree->Branch("LeptonCosNom",    &LeptonCosNom,    "LeptonCosNom/D");
    tree->Branch("WeightNom",       &WeightNom,       "WeightNom/D");
    tree->Branch("FluxWeightNom",   &FluxWeightNom,   "FluxWeightNom/D");

    tree->Branch("nToys",           &nToys,          "nToys/I");
    tree->Branch("Toy",             Toy,             "Toy[nToys]/I");
    tree->Branch("TrueVertexIDToy", TrueVertexIDToy, "TrueVertexIDToy[nToys]/I");
    tree->Branch("SelectionToy",    SelectionToy,    "SelectionToy[nToys]/I");
    tree->Branch("LeptonMomToy",    LeptonMomToy,    "LeptonMomToy[nToys]/D");
    tree->Branch("LeptonCosToy",    LeptonCosToy,    "LeptonCosToy[nToys]/D");
    tree->Branch("WeightToy",       WeightToy,       "WeightToy[nToys]/D");
    tree->Branch("FluxWeightToy",   FluxWeightToy,   "FluxWeightToy[nToys]/D");
    
    if(ntoys>nToys){
      std::cout<<" number of toys bigger than nToys "<<std::endl;
      return 0;
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

    std::cout << "RunSyst: loop over " << nmax << " entries" << std::endl;
  
    Long64_t entry = 0;
    
    while (entry < nmax) {
      
      if(entry%1000 == 0) std::cout << "Progress " << 100.*entry/nmax << "%" << std::endl;
      // 1. ====================================================================
      // Fill the event structure
      
      AnaEventB* event;
      if(preload)
        event=  _man.GetEvent(entry);
      else{
        event = static_cast<AnaEventB*>((_man.LoadSuperEvent(entry))->Event);
      }
      
      // Fill the EventBox
      if (!preload)
        _man.sel().InitializeEvent(*event);
      
      // Run the  nominal selection
      // Create the SystBox array (only the first time it is called for each systematic)
      //_man.syst().Initialize(_man.sel(),nmax);
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
      // Initialize The SystBox for variation systematics
      //if (!preload)

      LeptonMomNom    = -999;
      LeptonCosNom    = -999;
      SelectionNom    = -999;
      FluxWeightNom   = -999;
      WeightNom       = -999;
      TrueVertexIDNom = -999;
      SelectionNom    = -999;
 
      //_man.syst().InitializeEventSystematics(_man.sel(),*event);
      bool passednom = _man.ProcessEvent(*ZeroVarToy, *event,
                                         totalweight, fluxWeightSyst);      
      if(passednom){
        npassed[isyst]++;
        AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(event->Summary);
        if(summary && summary->EventSample){
          LeptonMomNom    = (Double_t)(static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->Momentum);
          LeptonCosNom    = (Double_t)(static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->DirectionStart[2]);
          SelectionNom    = (Int_t)   summary->EventSample;
          FluxWeightNom   = (Double_t)totalweight.Correction;
          WeightNom       = (Double_t)fluxWeightSyst.Correction;
          EventNumber     = (Int_t)(*event).EventInfo.Event;
          if(summary->TrueVertex[summary->EventSample])
            TrueVertexIDNom = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->GetTrueParticle()->VertexID;
        }
      }
      
      /// 2. ====================================================================
      /// Loop over toy experiments
      for (Int_t iToy = 0; iToy < ntoys; iToy++){
        
        ToyExperiment* toy = toyMaker->GetToyExperiment(iToy);
        ToyExperiment* reducedtoy = new ToyExperiment(*toy);
        // Enable the appropriate systematics
        if(event->GetIsMC()){
  
          for (std::vector<SystematicBase*>::iterator it = allSyst.begin(); it != allSyst.end(); ++it) {
            SystematicBase* ZeroVarSyst = *it;

            // Doesnt deactivate if this is the current syst || Doesn't deactivate any if it's allsyst
            if(currentSyst == ZeroVarSyst || currentSyst == NULL)
              continue;
            for(unsigned int iparam = 0; iparam < ZeroVarSyst->GetNParameters(); iparam++)
              reducedtoy->SetToyVariation(ZeroVarSyst->GetIndex(), iparam, 0, 1);
          }
        }

        Toy            [iToy] = iToy;
        SelectionToy   [iToy] = -999;
        LeptonMomToy   [iToy] = -999;
        LeptonCosToy   [iToy] = -999;
        WeightToy      [iToy] = -999;
        FluxWeightToy  [iToy] = -999;
        TrueVertexIDToy[iToy] = -999;

        // 3. ====================================================================
        // Process the current event (bunch). That means applying the systematics, the selections and computing the weights
        bool passed = _man.ProcessEvent(*reducedtoy, *event, totalweight, fluxWeightSyst);
        
        if(passed){
          AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(event->Summary);
          if(summary && summary->EventSample){
            LeptonMomToy   [iToy] = (Double_t)static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->Momentum;
            LeptonCosToy   [iToy] = (Double_t)static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->DirectionStart[2];
            SelectionToy   [iToy] = (Int_t)  summary->EventSample;
            FluxWeightToy  [iToy] = (Double_t)totalweight.Correction;
            WeightToy      [iToy] = (Double_t)fluxWeightSyst.Correction;
            if(summary->TrueVertex[summary->EventSample])
              TrueVertexIDToy[iToy] = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->GetTrueParticle()->VertexID;
          }
        }
          
        // if(event->GetIsMC()  && _man.syst().GetNEnabledVariationSystematics()>0)
        //   _man.syst().UndoVariationSystematics(*event);
        delete reducedtoy;
      }
      // Delete the SystBox for the current event
      if (event->GetIsMC() && _man.evar().GetNEnabledEventVariations()>0)
        _man.evar().FinalizeEvent(*event);

      // Delete the SystBox for the current event
      if (event->GetIsMC() && _man.eweight().GetNEnabledEventWeights()>0)
        _man.eweight().FinalizeEvent(*event);
      
      _man.sel().FinalizeEvent(*event);

      tree->Fill();
        
      // // Delete the SystBox for the current event
      // if (!preload && _man.syst().GetNEnabledVariationSystematics()+_man.syst().GetNEnabledWeightSystematics()>0)
      //   _man.syst().FinalizeEventSystematics(*event);

      // // The PreviousToyBox must be deleted after processing one event because 
      // // it is only deleted internally when processing the next toy
      // if (!preload)
      //   _man.sel().FinalizeEvent(*event);

      // save every 5000 entries
      // if (entry%5000 == 0)
      //   tree->AutoSave("SaveSelf");
    }
    //    tree->AutoSave("SaveSelf");
    //    tree->Print();
  }
  outfile->cd();
  tree->Write();

  // std::cout << "Copying the NRooTrackerVtx Tree" << std::endl;
  // inputFile = new TFile(inputFileName.c_str(), "READ");
  // RTV = (TTree*)(inputFile->Get("NRooTrackerVtx"));
  // outfile->cd();
  // RTV->CloneTree()->Write();


  outfile->Close();
  inputFile->Close();
  gettimeofday(&tim, NULL);
  double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
  
  std::cout << "time: " << t1-t0 << std::endl;
  
  //_man.sel().PrintStatistics();
  
  //  ProfilerStop();
  for (unsigned int i = 0; i<allSyst.size()+1; i++ ){
    std::cout << "# events passing selection, syst "<< i << ": " << npassed[i] << std::endl;
  }


  
}
