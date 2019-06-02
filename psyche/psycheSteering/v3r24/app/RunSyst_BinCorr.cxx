#include "AnalysisManager.hxx"
#include "ND280AnalysisUtils.hxx"
#include <sys/time.h>
#include <unistd.h>

#include "ToyMakerCorrelatedSyst.hxx"
#include "ToyMakerExample.hxx"
#include <Parameters.hxx>
#include <TCanvas.h>
#include <TH1.h>
#include <TH1F.h>
#include "TRandom3.h"
#include "MultiThread.hxx"


int main(int argc, char *argv[]){

  int nWeights = 23;
  std::string programName = argv[0];
  std::string paramFile = "";
  int nmax = 100000000;
  int nToys = 2000;
  std::string inputFileName   = "";
  std::string inputFileType   = "kHighlandTree";
  std::string outputFileName  = "";
  std::string CorrelationFile = "";
  Int_t debug = 0;
  Int_t preload = 0;
  if(argc < 4){
    std::cerr << "You have to specify: RunSystBinCorr.exe -i inputfile.root -o outputfile.root (-n nevents -c correlations.xml -t ntoys)" << std::endl;
    throw;
  }
  for (;;) {
    int c = getopt(argc, argv, "t:n:o:i:c");
    if (c < 0)
      break;
    switch (c) {
    case 't': {
      std::istringstream tmp(optarg);
      tmp >> nToys;
      break;
    }
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
    case 'c': {
      CorrelationFile = optarg;
      break;
    }
    default: {
      std::cerr << optarg << " is an unknown option" << std::endl;
      throw;
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

  // Read the parameters files following the package hierarchy
  // first the top level package. Set the parameters as fixed
  ND::params().LoadParametersFiles(anaUtils::GetPackageHierarchy(), true);

  // Make sure no parameters have been accessed yet
  ND::params().SetReadParamOverrideFilePointPassed();

  // Parameters to control the systematics
  bool applyVariationSystematics  = (bool)ND::params().GetParameterI("psycheSteering.Systematics.ApplyVariationSystematics");
  bool applyWeightSystematics     = (bool)ND::params().GetParameterI("psycheSteering.Systematics.ApplyWeightSystematics");
  bool applyFluxWeightSystematics = (bool)ND::params().GetParameterI("psycheSteering.FluxWeighting.Enable");
  bool RunAllSyst                 = (bool)ND::params().GetParameterI("psycheSteering.RunSyst.RunAllSyst");
  bool RunOnInidividualSyst       = (bool)ND::params().GetParameterI("psycheSteering.RunSyst.RunOnInidividualSyst");
  bool ThrowToys                  = (bool)ND::params().GetParameterI("psycheSteering.RunSyst.ThrowToys");
  if(!applyFluxWeightSystematics && !applyVariationSystematics && !applyWeightSystematics) {
    std::cout<<" no systematics is set to be applied "<<std::endl;
    throw;
  }


  TFile* inputFile = new TFile(inputFileName.c_str(), "READ");
  TTree* RTV = (TTree*)inputFile->Get("NRooTrackerVtx");
  TTree *def = (TTree*)inputFile->Get("flattree");
  TTree *hdr = (TTree*)inputFile->Get("header");
  TTree *con = (TTree*)inputFile->Get("config");
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
  std::vector<EventVariationBase*> allVar  = _man.evar().GetEventVariations();
  std::vector<SystematicBase*>     allSyst = _man.syst().GetSystematics();
  _man.sel().DumpSelections();

  ToyMakerCorrelatedSyst* toyMaker = new ToyMakerCorrelatedSyst((UInt_t)ND::params().GetParameterI("psycheSteering.Systematics.RandomSeed"), _man.syst());
  if(CorrelationFile == "")
    toyMaker->SetXMLFile(ND::params().GetParameterS("psycheSteering.RunSyst.CorrelationXMLFile"));
  else
    toyMaker->SetXMLFile(CorrelationFile.c_str());

  toyMaker->ParseInputXMLFileAndCreateCorrelation();
  toyMaker->InvertMatrix();
  std::cout << "Creating " <<  nToys << " toy experiments" << std::endl;
  toyMaker->CreateToyExperiments(nToys, _man.syst().GetSystematics());

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
  }else{
    // Create the array of PreviousToyBox
    std::cout << "Creating to the box array" << std::endl;
    _man.sel().CreateToyBoxArray(nmax);
  }

  //if(!(nToys <= 1000)) nToys = 1000;

  Int_t Run         = -999;
  Int_t SubRun      = -999;
  Int_t EventNumber = -999;

  Int_t    TrueVertexIDNom;
  Int_t    SelectionNom   ;
  Double_t TrueEnuNom     ;
  Int_t    TrueNuPDGNom   ;
  Double_t LeptonMomNom   ;
  Double_t LeptonCosNom   ;
  Double_t WeightNom      ;
  Double_t FluxWeightNom  ;

  Int_t    Toy            [nToys];
  Int_t    TrueVertexIDToy[nToys];
  Int_t    SelectionToy   [nToys];
  Double_t TrueEnuToy     [nToys];
  Int_t    TrueNuPDGToy   [nToys];
  Double_t LeptonMomToy   [nToys];
  Double_t LeptonCosToy   [nToys];
  Double_t WeightToy      [nToys];
  Int_t    nWeightSyst = _man.eweight().GetNEnabledEventWeights();
  if(ND::params().HasParameter("psycheSteering.RunSyst.NWeights"))
      nWeights = ND::params().GetParameterI("psycheSteering.RunSyst.NWeights");

  if(nWeights != nWeightSyst){
    std::cerr << "nWeights != nWeightSyst (" << nWeights << " != " << nWeightSyst << ")" << std::endl;
    std::cerr << "Change the hard coded value at the beginning of RunSyst_New.cxx" << std::endl;
    std::cerr << "or the parameter psycheSteering.RunSyst.NWeights " << std::endl;
    throw;
  }
  for (std::vector<EventWeightBase*>::iterator it = _man.eweight().GetEventWeights().begin(); it != _man.eweight().GetEventWeights().end(); ++it) {
    if(*it == NULL) continue;
    if(!(*it)->IsEnabled()) continue;
    std::cout << (*it)->GetName() << std::endl;
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
  TTree *tree;
  TFile *outfile = new TFile(outputFileName.c_str(),"RECREATE");

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
    tree->Branch("TrueEnuNom",      &TrueEnuNom,      "TrueEnuNom/D"     );
    tree->Branch("TrueNuPDGNom",    &TrueNuPDGNom,    "TrueNuPDGNom/I"   );
    tree->Branch("TrueVertexIDNom", &TrueVertexIDNom, "TrueVertexIDNom/I");
    tree->Branch("LeptonMomNom",    &LeptonMomNom,    "LeptonMomNom/D"   );
    tree->Branch("LeptonCosNom",    &LeptonCosNom,    "LeptonCosNom/D"   );
    tree->Branch("WeightNom",       &WeightNom,       "WeightNom/D"      );
    tree->Branch("FluxWeightNom",   &FluxWeightNom,   "FluxWeightNom/D"  );
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

    //  ProfilerStart("prof.out");
    //--------- Loop over entries in the tree ----------------------------------
    // Get the number of entries in the tree
    nmax = std::min(nmax, (int)_man.input().GetEntries());

    std::cout << "RunSyst: loop over " << nmax << " entries" << std::endl;

    Long64_t entry = 0;
    while (entry < nmax) {

      if(entry%100 == 0)
        std::cout << "Progress " << 100.*entry/nmax << "%" << std::endl;

      // 1. ====================================================================
      // Fill the event structure

      bool FillTree = false;
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

      TrueEnuNom      = -999;
      TrueNuPDGNom    = -999;
      LeptonMomNom    = -999;
      LeptonCosNom    = -999;
      SelectionNom    = -999;
      FluxWeightNom   = -999;
      WeightNom       = -999;
      TrueVertexIDNom = -999;
      SelectionNom    = -999;

      bool passednom = _man.ProcessEvent(*ZeroVarToy, *event, totalweight, fluxWeightSyst);

      if(passednom){
        FillTree=true;
        AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(event->Summary);
        if(summary && summary->EventSample){
          npassed[summary->EventSample]++;
          LeptonMomNom    = (Double_t)(static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->Momentum);
          LeptonCosNom    = (Double_t)(static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->DirectionStart[2]);
          SelectionNom    = (Int_t)   summary->EventSample;
          WeightNom       = (Double_t)totalweight   .Correction;
          FluxWeightNom   = (Double_t)fluxWeightSyst.Correction;
          Run             = (Int_t)   (*event).EventInfo.Run;
          SubRun          = (Int_t)   (*event).EventInfo.SubRun;
          EventNumber     = (Int_t)   (*event).EventInfo.Event;
          if(summary->TrueVertex[summary->EventSample]){
            TrueEnuNom      = (Double_t)(summary->TrueVertex[summary->EventSample]->NuEnergy);
            TrueNuPDGNom    = (Int_t)   (summary->TrueVertex[summary->EventSample]->NuPDG   );
            TrueVertexIDNom = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->GetTrueParticle()->VertexID;
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
        TrueVertexIDToy[iToy] = -999;
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
        delete weights;
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
    RTV = (TTree*)inputFile->Get("NRooTrackerVtx");
    def = (TTree*)inputFile->Get("flattree");
    hdr = (TTree*)inputFile->Get("header");
    con = (TTree*)inputFile->Get("config");
    outfile->cd();
    RTV->CloneTree()->Write();
    def->CloneTree()->Write();
    hdr->CloneTree()->Write();
    con->CloneTree()->Write();
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
