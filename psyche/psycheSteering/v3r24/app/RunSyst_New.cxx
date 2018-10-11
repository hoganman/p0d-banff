#define RUNSYST_NEW_CXX

#include "AnalysisManager.hxx"
#include "ND280AnalysisUtils.hxx"
#include "ToyMakerExample.hxx"
#include "Parameters.hxx"
#include "MultiThread.hxx"
//#include "CategoriesUtils.hxx"
//#include "DataClasses.hxx"
#include <sys/time.h>
#include <unistd.h>

#include "TCanvas.h"
#include "TH1F.h"
#include "TRandom3.h"
#include "TDirectory.h"
#include "TGeoManager.h"

#include <vector>

#define DEBUG(X) std::cout << #X << " = " << X << std::endl;
std::string GetMCGeoPositionPath(TGeoManager* const thisGeoManger,const TLorentzVector& checkPosition);
std::vector<std::string> SplitString(const std::string &inString, char SplitBy);
Int_t IsWaterP0Dule(TGeoManager* const tmpGeoManger, const TLorentzVector& StartPosition);
inline Bool_t IsPositionInWaterVolume(TGeoManager* const tmpGeoManger, const TLorentzVector& StartPosition);
AnaTrueParticleB* GetTrueVtxLepton(AnaTrueVertexB* trueVtx, Bool_t absPDG = kTRUE);
Int_t GetSameGenerationLepton(const Int_t& nuPDG);

int main(int argc, char **argv){

    const Int_t nWeights = 21;//20;
    const UInt_t nToys = 0;//1000;
    const Int_t debug = 0;
    const Double_t kDoubleInit = -999;
    const Int_t kIntInit = -1;
    const UInt_t kUIntInit = 0;

    std::string programName = argv[0];
    std::string paramFile = "";
    std::string inputFileName = "";
    std::string inputFileType = "kHighlandTree";
    std::string outputFileName= "";
    Int_t ntoys;
    Bool_t preload = kFALSE;
    Long64_t nmax = 100000000;
    Bool_t isData = false;
    TGeoManager* geoManager = NULL;

    if(argc < 4)
    {
        std::cerr << "You have to specify: RunSyst_New.exe -i inputfile.root -o outputfile.root (-n nevents) (-d isData)" << std::endl;
        throw;
    }

    for (;;)
    {
        Int_t c = getopt(argc, argv, "n:o:i:d");
        if (c < 0)
            break;
        switch (c)
        {
            case 'n':
            {
                std::istringstream tmp(optarg);
                tmp >> nmax;
                break;
            }
            case 'o':
            {
                outputFileName = optarg;
                break;
            }
            case 'i':
            {
                inputFileName = optarg;
                break;
            }
            case 'd':
            {
                isData = true;
                break;
            }
            default:
            {
                std::cerr << optarg << " is an unknown option" << std::endl;
                throw;
            }
        }
    }//end for

    std::cout << "is Data: " << isData << std::endl;

#ifndef MULTITHREAD
    if (preload)
    {
        std::cout << "For preloading mode you must uncomment the line #define MULTITHREAD in psycheCore/vXrY/srx/MultiThread.hxx and recompile everything !!!" << std::endl;
        return 0;
    }
#else
    if (!preload){
        std::cout << "For Int_teractive mode you must comment out the line #define MULTITHREAD psycheCore/vXrY/srx/MultiThread.hxx and recompile everything !!!" << std::endl;
        return 0;
    }
#endif

    // Read the parameters files following the package hierarchy
    // first the top level package. Set the parameters as fixed
    ND::params().LoadParametersFiles(anaUtils::GetPackageHierarchy(), true);

    // Make sure no parameters have been accessed yet
    ND::params().SetReadParamOverrideFilePointPassed();

    // Parameters to control the systematics
    Bool_t applyVariationSystematics  = false;
    Bool_t applyWeightSystematics     = false;
    Bool_t applyFluxWeightSystematics = false;
    Bool_t RunAllSyst                 = false;
    Bool_t RunOnInidividualSyst       = false;
    Bool_t ThrowToys                  = false;

    if(!isData)
    {
        applyVariationSystematics  = static_cast<Bool_t>(ND::params().GetParameterI("psycheSteering.Systematics.ApplyVariationSystematics"));
        applyWeightSystematics     = static_cast<Bool_t>(ND::params().GetParameterI("psycheSteering.Systematics.ApplyWeightSystematics"));
        applyFluxWeightSystematics = static_cast<Bool_t>(ND::params().GetParameterI("psycheSteering.FluxWeighting.Enable"));
        RunAllSyst                 = static_cast<Bool_t>(ND::params().GetParameterI("psycheSteering.RunSyst.RunAllSyst"));
        RunOnInidividualSyst       = static_cast<Bool_t>(ND::params().GetParameterI("psycheSteering.RunSyst.RunOnInidividualSyst"));
        ThrowToys                  = static_cast<Bool_t>(ND::params().GetParameterI("psycheSteering.RunSyst.ThrowToys"));
        if(!applyFluxWeightSystematics && !applyVariationSystematics && !applyWeightSystematics)
        {
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

    // Initialize the InputManager by specifying the input type and the input
    // file
    if (!_man.input().Initialize(inputFileName,inputFileType, false)) return false;
    std::vector<EventVariationBase*> allVar  = _man.evar().GetEventVariations();
    std::vector<SystematicBase*>     allSyst = _man.syst().GetSystematics();
    _man.sel().DumpSelections();

    ToyMaker* toyMaker = NULL;
    ToyMaker* ZeroVarToyMaker = NULL;
    ToyExperiment* ZeroVarToy = NULL;
    std::vector<float> weights;
    if(!isData)
    {
        // Create a ToyMaker to configure the toy experiment. Initialize it
        // with a random seed
        toyMaker = new ToyMakerExample(static_cast<UInt_t>(ND::params().GetParameterI("psycheSteering.Systematics.RandomSeed")),
                                       static_cast<Bool_t>(ND::params().GetParameterI("psycheSteering.Systematics.ZeroVariation"))
                                      );
        // Create and fill the Toy experiment with the appropriate format
        // (number of systematics and number of parameters for each systematic)
        std::cout << "Creating " <<  nToys << " toy experiments" << std::endl;
        toyMaker->CreateToyExperiments(nToys, _man.syst().GetSystematics());

        ZeroVarToyMaker = new ToyMakerExample(1, true);
        ZeroVarToyMaker->CreateToyExperiments(1, _man.syst().GetSystematics());
        ZeroVarToy = ZeroVarToyMaker->GetToyExperiment(0);

        // Print the steps for the different selections
        if (debug>0)
        {
            _man.sel().GetSelection("kNuMuCCP0DWater")->DumpSteps();
            _man.sel().GetSelection("kNuMuCCP0DAir")->DumpSteps();
            _man.syst().DumpVariationSystematics();
            _man.syst().DumpWeightSystematics();
        }
    }


    if(!isData)
    {
        if(preload)
        {
            std::cout <<" preloading!!!! "<<std::endl;

            // Preload nmax events from the file
            if (!_man.ReadEvents(inputFileName, nmax))
                return 0;

            if(nmax < 0)
                nmax = _man.GetEntries();

            _man.SetNEventsToProcess(nmax);
        }
        else
        {
            // Create the array of PreviousToyBox
            std::cout << "Creating to the box array" << std::endl;
            _man.sel().CreateToyBoxArray(nmax);
        }
    }
    else
    {
        std::cout << "Preloading data events!" << std::endl;
        if (!_man.ReadEvents(inputFileName, nmax))
            return 0;
        if(nmax < 0)
            nmax = _man.GetEntries();
        _man.SetNEventsToProcess(nmax);

    }

    std::cout << "The number of events = " << nmax << std::endl;
    Int_t Run         = kDoubleInit;
    Int_t SubRun      = kDoubleInit;
    Int_t EventNumber = kDoubleInit;
    UInt_t RooVertexIndex = kUIntInit;

    Int_t    TrueVertexIDNom = kIntInit;
    Int_t    SelectionNom    = kIntInit;
    Int_t    TrueNuPDGNom    = kIntInit;
    Double_t TrueEnuNom      = kDoubleInit;
    Double_t WeightNom       = kDoubleInit;
    Double_t FluxWeightNom   = kDoubleInit;

    //True quantities
    Int_t tLeptonPDG = kIntInit;
    Int_t tLeptonParentPDG = kIntInit;
    Int_t tLeptonGParentPDG = kIntInit;
    Double_t tLeptonMomentum = kDoubleInit;
    Double_t tLeptonCosTheta = kDoubleInit;
    Double_t tLeptonPhi = kDoubleInit;
    Int_t tOnWaterTarget = kIntInit;
    Double_t tLeptonPositionX = kDoubleInit;
    Double_t tLeptonPositionY = kDoubleInit;
    Double_t tLeptonPositionZ = kDoubleInit;
    Double_t tVtxX = kDoubleInit;
    Double_t tVtxY = kDoubleInit;
    Double_t tVtxZ = kDoubleInit;

    Int_t tReactionCode = kIntInit;
    Double_t tNu = kDoubleInit;
    Double_t tYbj = kDoubleInit;
    Double_t tW2 = kDoubleInit;
    Double_t tQ2 = kDoubleInit;
    Double_t tXbj = kDoubleInit;

    //reconstructed quantities
    Double_t LeptonPositionX = kDoubleInit;
    Double_t LeptonPositionY = kDoubleInit;
    Double_t LeptonPositionZ = kDoubleInit;
    Double_t LeptonMomNom    = kDoubleInit;
    Double_t LeptonCosNom    = kDoubleInit;
    Double_t LeptonTPC1MomNom = kDoubleInit;
    Double_t vtxX = kDoubleInit;
    Double_t vtxY = kDoubleInit;
    Double_t vtxZ = kDoubleInit;
    Int_t inFGD1 = kIntInit;
    Int_t inFGD2 = kIntInit;

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

    std::vector<EventWeightBase*>::iterator ew_iter;
    for (ew_iter = _man.eweight().GetEventWeights().begin(); ew_iter != _man.eweight().GetEventWeights().end(); ++ew_iter)
    {
        EventWeightBase* ewb = *ew_iter;
        if(!ewb) continue;
        if(!(ewb->IsEnabled())) continue;
        std::cout << ewb->GetName() << std::endl;
    }

    Double_t WeightIndToy[nWeights][nToys];
    Double_t FluxWeightToy[nToys];

    for (Int_t iToy = 0; iToy < nToys; ++iToy)
    {
        Toy                        [iToy] = kDoubleInit;
        TrueVertexIDToy[iToy] = kDoubleInit;
        SelectionToy     [iToy] = kDoubleInit;
        LeptonMomToy     [iToy] = kDoubleInit;
        TrueEnuToy         [iToy] = kDoubleInit;
        TrueNuPDGToy     [iToy] = kDoubleInit;
        LeptonCosToy     [iToy] = kDoubleInit;
        WeightToy            [iToy] = kDoubleInit;
        for (Int_t iSyst = 0; iSyst < nWeights; ++iSyst) {
            WeightIndToy[iSyst][iToy] = kDoubleInit;
        }
    }

    TTree *tree = NULL;
    TFile *outfile = new TFile(outputFileName.c_str(), "RECREATE");
    std::vector<std::string> systnametree;
    std::vector<EventVariationBase*>::iterator evb_iter;
    for (evb_iter = allVar.begin(); evb_iter != allVar.end(); ++evb_iter)
    {
        if(!*evb_iter)
            continue;
        if(!(*evb_iter)->IsEnabled())
            continue;
        if(RunOnInidividualSyst)
            systnametree.push_back(std::string((*evb_iter)->GetName()));
    }

    if(RunOnInidividualSyst)
        systnametree.push_back("weight");
    if(RunAllSyst)
        systnametree.push_back("all");
    if(!ThrowToys)
    {
        systnametree.clear();
        systnametree.push_back("nominal");
    }
    std::vector<Int_t> npassed(SampleId::kNSamples, 0);
    std::vector<std::string>::iterator str_iter;
    for (str_iter = systnametree.begin(); str_iter != systnametree.end(); ++str_iter)
    {
        std::string syst_name = (*str_iter);
        std::cout << "Running over " << syst_name << std::endl;
        Bool_t WeightSyst = (syst_name == "weight");
        Bool_t AllSyst        = (syst_name == "all");

        outfile->cd();
        tree = new TTree(syst_name.c_str(),syst_name.c_str());
        tree->Branch("Run",                         &Run,                         "Run/I"                );
        tree->Branch("SubRun",                    &SubRun,                    "SubRun/I"         );
        tree->Branch("EventNumber",         &EventNumber,         "EventNumber/I");
        tree->Branch("RooVertexIndex",    &RooVertexIndex,    "RooVertexIndex/i");

        tree->Branch("SelectionNom",        &SelectionNom,        "SelectionNom/I"     );
        tree->Branch("TrueEnuNom",            &TrueEnuNom,            "TrueEnuNom/D"         );
        tree->Branch("TrueNuPDGNom",        &TrueNuPDGNom,        "TrueNuPDGNom/I"     );
        tree->Branch("TrueVertexIDNom", &TrueVertexIDNom, "TrueVertexIDNom/I");
        tree->Branch("LeptonMomNom",        &LeptonMomNom,        "LeptonMomNom/D"     );
        tree->Branch("LeptonTPC1MomNom",        &LeptonTPC1MomNom,        "LeptonTPC1MomNom/D"     );
        tree->Branch("LeptonCosNom",        &LeptonCosNom,        "LeptonCosNom/D"     );
        tree->Branch("WeightNom",             &WeightNom,             "WeightNom/D"            );
        tree->Branch("FluxWeightNom",     &FluxWeightNom,     "FluxWeightNom/D"    );

        tree->Branch("tLeptonPDG", &tLeptonPDG, "tLeptonPDG/I");
        tree->Branch("tLeptonParentPDG", &tLeptonParentPDG, "tLeptonParentPDG/I");
        tree->Branch("tLeptonGParentPDG", &tLeptonGParentPDG, "tLeptonGParentPDG/I");
        tree->Branch("tLeptonMomentum", &tLeptonMomentum, "tLeptonMomentum/D");
        tree->Branch("tLeptonCosTheta", &tLeptonCosTheta, "tLeptonCosTheta/D");
        tree->Branch("tLeptonPhi", &tLeptonPhi, "tLeptonPhi/D");
        tree->Branch("vtxX" ,&vtxX ,"vtxX/D");
        tree->Branch("vtxY" ,&vtxY ,"vtxY/D");
        tree->Branch("vtxZ" ,&vtxZ ,"vtxZ/D");
        tree->Branch("LeptonPositionX", &LeptonPositionX, "LeptonPositionX/D");
        tree->Branch("LeptonPositionY", &LeptonPositionY, "LeptonPositionY/D");
        tree->Branch("LeptonPositionZ", &LeptonPositionZ, "LeptonPositionZ/D");
        tree->Branch("tLeptonPositionX", &tLeptonPositionX, "tLeptonPositionX/D");
        tree->Branch("tLeptonPositionY", &tLeptonPositionY, "tLeptonPositionY/D");
        tree->Branch("tLeptonPositionZ", &tLeptonPositionZ, "tLeptonPositionZ/D");
        tree->Branch("tVtxX",&tVtxX,"tVtxX/D");
        tree->Branch("tVtxY",&tVtxY,"tVtxY/D");
        tree->Branch("tVtxZ",&tVtxZ,"tVtxZ/D");
        tree->Branch("tReactionCode",&tReactionCode,"tReactionCode/I");
        tree->Branch("tW2", &tW2, "tW2/D");
        tree->Branch("tQ2", &tQ2, "tQ2/D");
        tree->Branch("tXbj", &tXbj, "tXbj/D");
        tree->Branch("tNu", &tNu, "tNu/D");
        tree->Branch("tYbj", &tYbj, "tYbj/D");

        tree->Branch("inFGD1", &inFGD1,"inFGD1/I");
        tree->Branch("inFGD2", &inFGD2,"inFGD2/I");
        tree->Branch("tOnWaterTarget",&tOnWaterTarget,"tOnWaterTarget/I");

        if(ThrowToys)
        {
            tree->Branch("nToys",            (Int_t*)&nToys,  "nToys/i");
            tree->Branch("Toy",              Toy,             Form("Toy[%d]/I",              nToys));
            tree->Branch("TrueVertexIDToy",  TrueVertexIDToy, Form("TrueVertexIDToy[%d]/I",  nToys));
            tree->Branch("SelectionToy",     SelectionToy,    Form("SelectionToy[%d]/I",     nToys));
            tree->Branch("TrueEnuToy",       TrueEnuToy,      Form("TrueEnuToy[%d]/D",       nToys));
            tree->Branch("TrueNuPDGToy",     TrueNuPDGToy,    Form("TrueNuPDGToy[%d]/I",     nToys));
            tree->Branch("LeptonMomToy",     LeptonMomToy,    Form("LeptonMomToy[%d]/D",     nToys));
            tree->Branch("LeptonCosToy",     LeptonCosToy,    Form("LeptonCosToy[%d]/D",     nToys));
            tree->Branch("FluxWeightToy",     FluxWeightToy,  Form("FluxWeightToy[%d]/D",nToys));
            if(!WeightSyst)
                tree->Branch("WeightToy",    WeightToy,       Form("WeightToy[%d]/D",        nToys));
            else
            {
                tree->Branch("nSyst",        &nWeightSyst,    "nSyst/I");
                tree->Branch("WeightIndToy", WeightIndToy,    Form("WeightIndToy[%d][%d]/D", nWeightSyst, nToys));
            }

        }
        std::vector<Float_t> weights;
        Weight_h totalweight;
        Weight_h fluxWeightSyst;
        Int_t rtvi = 0;
        Int_t prevTruthID = -1;
        Int_t prevRTV = -1;

        //  ProfilerStart("prof.out");
        //--------- Loop over entries in the tree ----------------------------------
        // Get the number of entries in the tree
        nmax = std::min(nmax, static_cast<Long64_t>(_man.input().GetEntries()));

        if(gDirectory->FindObjectAny("ND280Geometry"))
            std::cout << "Found ND280Geometry" << std::endl;

        std::cout << "RunSyst: loop over " << nmax << " entries" << std::endl;

        Long64_t entry = 0;
        while (entry < nmax)
        {
            if(!geoManager && !isData)
            geoManager = static_cast<TGeoManager*>(gDirectory->FindObjectAny("ND280Geometry"));

            if(entry % 100 == 0)
                std::cout << "Progress " << 100.*entry/nmax << "%" << std::endl;

            // 1. ====================================================================
            // Fill the event structure

            Bool_t FillTree = false;
            AnaEventB* event = NULL;

            if(preload)
                event = _man.GetEvent(entry);
            else
                event = static_cast<AnaEventB*>((_man.LoadSuperEvent(entry))->Event);

if(debug) std::cout << "got event" << std::endl;

            // Fill the EventBox
            if (!preload)
            {
                _man.sel().InitializeEvent(*event);
if(debug) std::cout << "initialized" << std::endl;
            }

            // this is MC
            if(!isData)
            {
if(debug) DEBUG(!isData)

                // Run the  nominal selection
                // Create the SystBox array (only the first time it is called for each systematic)
                if (_man.evar().HasEventVariations())
                {
if(debug) DEBUG(nmax)
                    // Create the SystBox array (only the first time it is called for each EventVariation)
                    _man.evar().Initialize(nmax);
if(debug) std::cout << "Initialize The SystBox for EventVariations" << std::endl;
                    // Initialize The SystBox for EventVariations
                    _man.evar().InitializeEvent(_man.sel(),*event);
                }

                if (_man.eweight().HasEventWeights())
                {
if(debug) DEBUG(_man.eweight().HasEventWeights())
                    // Create the SystBox array (only the first time it is called for each EventWeight)
                    _man.eweight().Initialize(_man.sel(),nmax);

if(debug) std::cout << "Initialize The SystBox for variation systematics" << std::endl;
                    // Initialize The SystBox for variation systematics
                    _man.eweight().InitializeEvent(_man.sel(),*event);
                }
            }
            TrueEnuNom        = kDoubleInit;
            TrueNuPDGNom      = kDoubleInit;
            LeptonMomNom      = kDoubleInit;
            LeptonTPC1MomNom  = kDoubleInit;
            LeptonCosNom      = kDoubleInit;
            SelectionNom      = kDoubleInit;
            FluxWeightNom     = kDoubleInit;
            WeightNom         = kDoubleInit;
            TrueVertexIDNom   = kDoubleInit;
            SelectionNom      = kDoubleInit;
            tLeptonPDG        = kDoubleInit;
            tLeptonParentPDG  = kDoubleInit;
            tLeptonGParentPDG = kDoubleInit;
            tLeptonMomentum   = kDoubleInit;
            tLeptonCosTheta   = kDoubleInit;
            tLeptonPhi        = kDoubleInit;
            LeptonPositionX = kDoubleInit;
            LeptonPositionY = kDoubleInit;
            LeptonPositionZ = kDoubleInit;
            tLeptonPositionX = kDoubleInit;
            tLeptonPositionY = kDoubleInit;
            tLeptonPositionZ = kDoubleInit;
            tVtxX = kDoubleInit;
            tVtxY = kDoubleInit;
            tVtxZ = kDoubleInit;
            tReactionCode = kIntInit;
            tW2   = kDoubleInit;
            tQ2   = kDoubleInit;
            tNu   = kDoubleInit;
            tYbj  = kDoubleInit;
            tXbj  = kDoubleInit;
            vtxX  =  kDoubleInit;
            vtxY  =  kDoubleInit;
            vtxZ  =  kDoubleInit;
            tOnWaterTarget    = kIntInit;
            RooVertexIndex    = kUIntInit;

            //_man.syst().InitializeEventSystematics(_man.sel(),*event);
            Bool_t passednom = false;

            if(!isData)
            {
                passednom = _man.ProcessEvent(*ZeroVarToy, *event, totalweight, fluxWeightSyst);
            }
            else
            {
                passednom = _man.ProcessEvent(*event);
            }

if(debug) std::cout << "passednom = " << passednom << std::endl;
            if(passednom)
            {
if(debug) std::cout << "passednom = " << passednom << std::endl;
                FillTree=true;
                //AnaVertexB** Vertices = event->Vertices;
                const Int_t nParticles = event->nParticles;
if(debug) std::cout << "There are " << nParticles  << " particles in this event" <<std::endl;
                AnaEventInfoB* eventInfo = static_cast<AnaEventInfoB*>(&event->EventInfo);
                Run = eventInfo->Run;
                SubRun = eventInfo->SubRun;
                EventNumber = eventInfo->Event;

                AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(event->Summary);
                if(!summary || !summary->EventSample)
                    continue;
if(debug) DEBUG(summary->EventSample)
                RooVertexIndex = summary->RooVertexIndex[summary->EventSample];
                npassed[summary->EventSample]++;
                AnaParticleMomB* lepCand = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample]);
                AnaTrueVertexB* trVtx = NULL;
                AnaTrueParticleB *trueParticle = NULL;
                //fill MC info
                if(!isData)
                {

                    FluxWeightNom   = (Double_t)fluxWeightSyst.Correction;
                    WeightNom       = (Double_t)totalweight.Correction;
                    EventNumber     = (Int_t)   (*event).EventInfo.Event;

                    trueParticle = lepCand->GetTrueParticle();
                    if(trueParticle)
                    {
if(debug) DEBUG(trueParticle->PDG)
                        tLeptonPDG = trueParticle->PDG;
                        tLeptonParentPDG = trueParticle->ParentPDG;
                        tLeptonGParentPDG = trueParticle->GParentPDG;
                        TLorentzVector trStart = trueParticle->Position;
                        tLeptonPositionX = trStart.X();
                        tLeptonPositionY = trStart.Y();
                        tLeptonPositionZ = trStart.Z();
                    }
                    trVtx = static_cast<AnaTrueVertexB*>(summary->TrueVertex[summary->EventSample]);
                    if(trVtx)
                    {
                        tVtxX = trVtx->Position[0];
                        tVtxY = trVtx->Position[1];
                        tVtxZ = trVtx->Position[2];
                        tReactionCode = trVtx->ReactionCode;
                        TrueVertexIDNom = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->GetTrueParticle()->VertexID;
                        TrueEnuNom      = (Double_t)(summary->TrueVertex[summary->EventSample]->NuEnergy);
                        TrueNuPDGNom    = (Int_t)   (summary->TrueVertex[summary->EventSample]->NuPDG   );
                        tQ2 = summary->TrueVertex[summary->EventSample]->Q2;

                        TVector3 nu3Mom(trVtx->NuDir[0], trVtx->NuDir[1], trVtx->NuDir[2]);
                        nu3Mom.SetMag(TrueEnuNom);

                        AnaTrueParticleB* trLepton = GetTrueVtxLepton(trVtx);
                        if(trLepton)
                        {
                            tLeptonMomentum = trLepton->Momentum;
                            const TVector3 tDirection(trLepton->Direction);
                            tLeptonCosTheta = tDirection.CosTheta();
                            tLeptonPhi = tDirection.Phi();
                            tLeptonMomentum = trLepton->Momentum;
                            const Double_t Mmu = anaUtils::GetParticleMass(ParticleId::GetParticle(trLepton->PDG));
                            if(Mmu > 0)
                            {
                                TLorentzVector nu4Mom;
                                nu4Mom.SetXYZT(nu3Mom.X(), nu3Mom.Y(), nu3Mom.Z(), TrueEnuNom);
                                const Double_t Emu = std::sqrt(tLeptonMomentum*tLeptonMomentum+Mmu*Mmu);
                                const Double_t M_N = (TrueNuPDGNom < 0) ?
                                                     anaUtils::GetParticleMass(ParticleId::kNeutron) :
                                                     anaUtils::GetParticleMass(ParticleId::kProton);  // neutron vs proton mass
                                tNu  = nu4Mom.E() - Emu;
                                tYbj = tNu/nu4Mom[3];
                                tXbj = tQ2/(2.0*M_N*tNu);
                                tW2  = M_N*M_N + 2.0*M_N*tNu - tQ2;
                            }
                        }

                        if(geoManager && (SampleId::IsP0DSelection(summary->EventSample)))
                        {
                            TLorentzVector start = trVtx->Position;
                            Int_t tmp = IsWaterP0Dule(geoManager,start);
                            //tOnWaterTarget = (tmp - (tmp % 10)) / 10;
                            tOnWaterTarget = (tmp % 10);
                        }
                    }
                }

                TLorentzVector recoVtx = summary->VertexPosition[summary->EventSample];
                TLorentzVector lepStart = lepCand->PositionStart;
                vtxX = recoVtx.X();
                vtxY = recoVtx.Y();
                vtxZ = recoVtx.Z();
                LeptonPositionX = lepStart.X();
                LeptonPositionY = lepStart.Y();
                LeptonPositionZ = lepStart.Z();
                LeptonMomNom    = lepCand->Momentum;
                LeptonCosNom    = lepCand->DirectionStart[2];
                SelectionNom    = (Int_t)summary->EventSample;

                inFGD1 = 0;
                inFGD2 = 0;
                if(static_cast<AnaTrackB*>(lepCand)->FGDSegments)
                {
                    if(static_cast<AnaTrackB*>(lepCand)->FGDSegments[0])
                        inFGD1 = 1;
                    if(static_cast<AnaTrackB*>(lepCand)->FGDSegments[1])
                        inFGD2 = 1;
                }
                if(static_cast<AnaTrackB*>(lepCand)->TPCSegments)
                {
                    AnaTPCParticleB* tpc1_Track = static_cast<AnaTrackB*>(lepCand)->TPCSegments[0];
                    LeptonTPC1MomNom = tpc1_Track->Momentum;
                }


            }

            /// 2. ====================================================================
            /// Loop over toy experiments
            for (Int_t iToy = 0; iToy < nToys && ThrowToys; iToy++)
            {
                ToyExperiment* toy = toyMaker->GetToyExperiment(iToy);
                ToyExperiment* reducedtoy = new ToyExperiment(*toy);
                // Enable the appropriate systematics
                if(event->GetIsMC())
                {
                    std::vector<SystematicBase*>::iterator sb_iter;
                    for (sb_iter = allSyst.begin(); sb_iter != allSyst.end(); ++sb_iter)
                    {
                        SystematicBase* ZeroVarSyst = *sb_iter;

                        if(!ZeroVarSyst)
                            continue;

                        if(WeightSyst && ZeroVarSyst->Type() == SystematicBase::kVariation){
                            for(UInt_t iparam = 0; iparam < ZeroVarSyst->GetNParameters(); iparam++)
                                reducedtoy->SetToyVariation(ZeroVarSyst->GetIndex(), iparam, 0, 1);
                            continue;
                        }
                        // Doesnt deactivate if this is the current syst || Doesn't deactivate any if it's allsyst
                        if(ZeroVarSyst->Name() == syst_name || AllSyst || WeightSyst)
                            continue;
                        for(UInt_t iparam = 0; iparam < ZeroVarSyst->GetNParameters(); iparam++)
                            reducedtoy->SetToyVariation(ZeroVarSyst->GetIndex(), iparam, 0, 1);
                    }
                }

                Toy[iToy] = iToy;
                SelectionToy[iToy] = kDoubleInit;
                TrueEnuToy[iToy] = kDoubleInit;
                TrueNuPDGToy[iToy] = kDoubleInit;
                LeptonMomToy[iToy] = kDoubleInit;
                LeptonCosToy[iToy] = kDoubleInit;
                WeightToy[iToy] = kDoubleInit;

                for (Int_t iSyst = 0; iSyst < nWeights; ++iSyst)
                {
                    WeightIndToy[iSyst][iToy] = kDoubleInit;
                }

                FluxWeightToy[iToy] = kDoubleInit;
                TrueVertexIDToy[iToy] = kDoubleInit;

                // 3. ====================================================================
                // Process the current event (bunch). That means applying the systematics, the selections and computing the weights
                Bool_t passed = false;
                Weight_h* weights = new Weight_h[NMAXSYSTEMATICS];
                // std::cout << "entry " << entry << std::endl;

                if(!WeightSyst)
                {
                    passed = _man.ProcessEvent(*reducedtoy, *event, totalweight, fluxWeightSyst);
                }
                else
                {
                    passed = _man.ProcessEventWithIndividualWeightSyst(*reducedtoy, *event, weights, fluxWeightSyst);
                }
                if(passed)
                {
                    FillTree=true;
                    AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(event->Summary);
                    if(summary && summary->EventSample){
                        LeptonMomToy [iToy] = (Double_t)(static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->Momentum);
                        LeptonCosToy [iToy] = (Double_t)(static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->DirectionStart[2]);
                        SelectionToy [iToy] = (Int_t)     (summary->EventSample);
                        FluxWeightToy[iToy] = (Double_t)(fluxWeightSyst.Correction);
                        if(!WeightSyst)
                            WeightToy[iToy]     = (Double_t)(totalweight.Systematic);
                        else
                        {
                            Int_t ind=0;
                            for(UInt_t i = 0; i < NMAXSYSTEMATICS; ++i)
                            {
                                if(weights[i].Systematic != kDoubleInit)
                                {
                                    WeightIndToy[ind][iToy] = weights[i].Systematic;
                                    //std::cout << "WeightIndToy[" << ind << "][" << iToy << "] " << WeightIndToy[ind][iToy] <<std::endl;
                                    ind++;
                                }
                            }
                            if(ind!=nWeights)
                            {
                                std::cerr << "something wrong is happening" << std::endl;
                                throw;
                            }
                        }
                        if(summary->TrueVertex[summary->EventSample])
                        {
                            TrueVertexIDToy[iToy] = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->GetTrueParticle()->VertexID;
                            TrueEnuToy     [iToy] = (Double_t)(summary->TrueVertex[summary->EventSample]->NuEnergy);
                            TrueNuPDGToy   [iToy] = (Int_t)   (summary->TrueVertex[summary->EventSample]->NuPDG);
                        }
                    }
                }
                if(weights)
                    delete weights;
                if(reducedtoy)
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

    inputFile = new TFile(inputFileName.c_str(), "READ");
    //if((Bool_t)ND::params().GetParameterI("psycheSteering.RunSyst.SaveAllTheNRooVtx") && inputFile->Get("NRooTrackerVtx"))
    //{
    //    std::cout << "Copying the NRooTrackerVtx Tree" << std::endl;
    //    RTV = (TTree*)(inputFile->Get("NRooTrackerVtx"));
    //    outfile->cd();
    //    RTV->CloneTree()->Write();
    //}

    TTree* header = NULL;
    if(inputFile->Get("header"))
    {
        std::cout << "Copying the header Tree" << std::endl;
        header = static_cast<TTree*>(inputFile->Get("header"));
        outfile->cd();
        header->CloneTree()->Write();
    }

    TTree* config = NULL;
    if(inputFile->Get("config"))
    {
        std::cout << "Copying the config Tree" << std::endl;
        config = static_cast<TTree*>(inputFile->Get("config"));
        outfile->cd();
        config->CloneTree()->Write();
    }

    outfile->Close();
    inputFile->Close();
    gettimeofday(&tim, NULL);
    double t1=tim.tv_sec+(tim.tv_usec/1000000.0);

    std::cout << "time: " << t1-t0 << std::endl;

    //_man.sel().PrInt_tStatistics();

    //  ProfilerStop();
    for (UInt_t i = 0; i < SampleId::kNSamples; ++i)
    {
        std::cout << "# events passing selection, Selection "<< SampleId::ConvertSample((SampleId::SampleEnum)i) << ": " << npassed[i] << std::endl;
    }
}

Bool_t IsPositionInWaterVolume(TGeoManager* const tmpGeoManger, const TLorentzVector& StartPosition)
{
    return (IsWaterP0Dule(tmpGeoManger, StartPosition) % 10) == 1;
}

Int_t IsWaterP0Dule(TGeoManager* const tmpGeoManger, const TLorentzVector& StartPosition)
{

  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  // * NOTICE : There is a special case where the P0Dule number will return 40 *
  // *          this means that the position                                   *
  // *          is in the 39 CECal_0/P0Dule_6/Epoxy/DownstreamCover_0          *
  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  //
  // this funtion will return an Int_t such that (return value)%10 will be 1 if the position is in water and 0 if not
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

  IsOnWater = 0;

  size_t LeftOnWater = PositionPath.find(LeftWater);
  //if(LeftOnWater!=std::string::npos) cout << "on Left Water Target" << endl;
  //if(LeftOnWater!=std::string::npos) return 1;
  if(LeftOnWater!=std::string::npos) IsOnWater=1;
  size_t RightOnWater = PositionPath.find(RightWater);
  //if(RightOnWater!=std::string::npos) cout << "on Right Water Target" << endl;
  //if(RightOnWater!=std::string::npos) return 1;
  if(RightOnWater!=std::string::npos) IsOnWater=1;

  //const char* PosPathStr = PositionPath.c_str();
  //const Int_t LeftOnWater = strstr(PosPathStr, "LeftTarget");
  //const Int_t RightOnWater = strstr(PosPathStr, "RightTarget");
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
    thisGeoManger->InitTrack(checkPosition.X(), checkPosition.Y(), checkPosition.Z(), 0, 0, 1); // 0, 0, 1 = the direction vector
    std::string tmpMCPath = thisGeoManger->GetPath();
    return tmpMCPath;
}

AnaTrueParticleB* GetTrueVtxLepton(AnaTrueVertexB* trueVtx, Bool_t absPDG)
{
    const Int_t vtxID = trueVtx->ID;
    const Int_t nuPDG = trueVtx->NuPDG;
    Int_t pdg = GetSameGenerationLepton(nuPDG);
    const Int_t checkPDG = (absPDG) ? std::abs(pdg) : pdg;
    for (Int_t part_i = 0; part_i < trueVtx->nTrueParticles; ++part_i)
    {
        AnaTrueParticleB* truePart = trueVtx->TrueParticles[part_i];
        if(truePart->VertexID != vtxID)
            continue;
        const Int_t particle_PDG = (absPDG) ? std::abs(truePart->PDG) : truePart->PDG;
        //if(checkPDG == particle_PDG && nuPDG == truePart->ParentPDG){
        if(checkPDG == particle_PDG){// && nuPDG == truePart->ParentPDG){
            return truePart;
        }
    }
    return NULL;
}

Int_t GetSameGenerationLepton(const Int_t& nuPDG)
{
    if(nuPDG == 14) return 13;
    if(nuPDG == -14) return -13;
    if(nuPDG == 12) return 11;
    if(nuPDG == -12) return -11;
    return 0;
}
