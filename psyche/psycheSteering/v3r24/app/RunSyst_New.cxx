#define RUNSYST_NEW_CXX

#include "AnalysisManager.hxx"
#include "ND280AnalysisUtils.hxx"
#include "ToyMakerExample.hxx"
#include "ToyMakerCorrelatedSyst.hxx"
#include "Parameters.hxx"
#include "MultiThread.hxx"
#include "EventBoxId.hxx"
#include "EventBoxTracker.hxx"

#include "TCanvas.h"
#include "TH1F.h"
#include "TRandom3.h"
#include "TDirectory.h"
#include "TGeoManager.h"

#include <sys/time.h>
#include <unistd.h>
#include <vector>

const TVector3 tNuParentDecayPointoaAnalysisCoor(3083.2, 6417 ,-245550);

#define DEBUG(X) std::cout << #X << " = " << X << std::endl;

std::string GetMCGeoPositionPath(TGeoManager* const thisGeoManger,const TLorentzVector& checkPosition);
std::vector<std::string> SplitString(const std::string &inString, const char SplitBy);
Int_t IsWaterP0Dule(TGeoManager* const tmpGeoManger, const TLorentzVector& StartPosition);
inline Bool_t IsPositionInWaterVolume(TGeoManager* const tmpGeoManger, const TLorentzVector& StartPosition);
Int_t GetSameGenerationLepton(const Int_t& nuPDG);
void FillNPrimaryParticles(const AnaTrueVertexB* const vertex, Int_t* NPrimaryParticles);
AnaTrueParticleB* GetTrueOutgoingLepton(AnaTrueVertexB* trueVertex);
inline TVector3 GetNuParentDecayPoint();
TVector3 GetNuDirection(const TVector3 &vertex);
Double_t GetAngleWRTBeam(const TVector3 &vertex, const TVector3 &trkDirection);
Double_t GetCosAngleWRTBeam(const TVector3 &vertex, const TVector3 &trkDirection);
inline void CheckIfNegative(const char* const name, const Double_t &value);

namespace Reset
{
    const Int_t kInt = -1;
    const UInt_t kUInt = 0;
    const Double_t kDouble = -999;
    const std::string kString = "";
};

int main(int argc, char **argv){

    const std::string programName = argv[0];

    int nWeights = 0;
    const Int_t debug = 0;

    Char_t usage[1024];
    sprintf(usage, "Usage: %s  -i inputfile.root -o outputfile.root (-c correlations.xml -n nevents -d isData -p parameterFileOveride -t nToys)", programName.c_str());

    if(argc < 4)
    {
        std::cerr << usage << std::endl;
        return 0;
    }

    std::string inputFileName = Reset::kString;
    std::string outputFileName = Reset::kString;
    std::string parameterFileOveride = Reset::kString;
    std::string correlationFile = Reset::kString;
    Long64_t nmax = TChain::kBigNumber;
    Int_t nToys = 0;
    Bool_t preload = kFALSE;
    Bool_t isData = kFALSE;
    TGeoManager* geoManager = NULL;

    for (;;)
    {
        Int_t c = getopt(argc, argv, "t:n:o:i:p:c:d");
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
            case 't':
            {
                std::istringstream tmp(optarg);
                tmp >> nToys;
                break;
            }
            case 'c':
            {
                correlationFile = optarg;
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
            case 'p':
            {
                parameterFileOveride = optarg;
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
                std::cerr << usage << std::endl;
                throw;
            }
        }
    }//end for

if (debug) std::cout << "is Data: " << isData << std::endl;

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
    // first the top level package
    ND::params().LoadParametersFiles(anaUtils::GetPackageHierarchy());

    if (parameterFileOveride.length() > 0 )
    {
        std::cout << "Using parameter override " << parameterFileOveride.c_str() << std::endl;
        ND::params().ReadParamOverrideFile(parameterFileOveride);
    }

    // Make sure no parameters have been accessed yet
    ND::params().SetReadParamOverrideFilePointPassed();

    // Parameters to control the systematics
    Bool_t applyVariationSystematics  = kFALSE;
    Bool_t applyWeightSystematics     = kFALSE;
    Bool_t applyFluxWeightSystematics = kFALSE;
    Bool_t RunAllSyst                 = kFALSE;
    Bool_t RunOnInidividualSyst       = kFALSE;
    Bool_t ThrowToys                  = kFALSE;

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
            std::cout << " no systematics is set to be applied, throwing..."<< std::endl;
            throw;
        }
    }

    TFile* inputFile = TFile::Open(inputFileName.c_str());
    if(!inputFile)
    {
        std::cerr << "No input file, exiting..." << std::cerr;
        throw;
    }

    TTree* RTV = static_cast<TTree*>(inputFile->Get("NRooTrackerVtx"));
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
    const std::string inputFileType = "kHighlandTree";
    if (!_man.input().Initialize(inputFileName,inputFileType, kFALSE))
        return kFALSE;

    std::vector<EventVariationBase*> allVar  = _man.evar().GetEventVariations();
    std::vector<SystematicBase*>     allSyst = _man.syst().GetSystematics();

    _man.sel().DumpSelections();
    _man.syst().DumpSystematics();

    //ToyMaker* toyMaker = NULL;
    ToyMakerCorrelatedSyst* toyMaker = NULL;
    ToyMaker* ZeroVarToyMaker = NULL;
    ToyExperiment* ZeroVarToy = NULL;

    if(!isData)
    {

        //This is what came with RunSyst_New.exe
        /*
        // Create a ToyMaker to configure the toy experiment. Initialize it
        // with a random seed
        toyMaker = new ToyMakerExample(static_cast<UInt_t>(ND::params().GetParameterI("psycheSteering.Systematics.RandomSeed")),
                                       static_cast<Bool_t>(ND::params().GetParameterI("psycheSteering.Systematics.ZeroVariation"))
                                      );
        */
        const UInt_t seed = static_cast<UInt_t>(ND::params().GetParameterI("psycheSteering.Systematics.RandomSeed"));
        toyMaker = new ToyMakerCorrelatedSyst(seed, _man.syst());
        if (correlationFile.length() < 1)
        {
            correlationFile = ND::params().GetParameterS("psycheSteering.RunSyst.CorrelationXMLFile");
        }
        toyMaker->SetXMLFile(correlationFile);
        toyMaker->ParseInputXMLFileAndCreateCorrelation();
        toyMaker->InvertMatrix();
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

    const Int_t NMAXPRIMARYPARTICLESSIZE = ParticleId::kLast + 1;

    std::cout << "The number of events = " << nmax << std::endl;
    Int_t Run         = Reset::kDouble;
    Int_t SubRun      = Reset::kDouble;
    Int_t EventNumber = Reset::kDouble;
    UInt_t RooVertexIndex = Reset::kUInt;

    Int_t    TrueVertexIDNom = Reset::kInt;
    Int_t    SelectionNom    = Reset::kInt;
    Int_t    TrueNuPDGNom    = Reset::kInt;
    Double_t TrueEnuNom      = Reset::kDouble;
    Double_t WeightNom       = Reset::kDouble;
    Double_t FluxWeightNom   = Reset::kDouble;

    //True quantities
    Int_t tLeptonPDG = Reset::kInt;
    Int_t tLeptonParentPDG = Reset::kInt;
    Int_t tLeptonGParentPDG = Reset::kInt;
    Double_t tLeptonMomentum = Reset::kDouble;
    Double_t tLeptonCosTheta = Reset::kDouble;
    Double_t tLeptonPhi = Reset::kDouble;
    Int_t tOnWaterTarget = Reset::kInt;
    Double_t tLeptonPositionX = Reset::kDouble;
    Double_t tLeptonPositionY = Reset::kDouble;
    Double_t tLeptonPositionZ = Reset::kDouble;
    Double_t tVtxX = Reset::kDouble;
    Double_t tVtxY = Reset::kDouble;
    Double_t tVtxZ = Reset::kDouble;
    Double_t EnuQE = Reset::kDouble;
    Double_t Q2QE = Reset::kDouble;

    Int_t tReactionCode = Reset::kInt;
    Double_t tNu = Reset::kDouble;
    Double_t tYbj = Reset::kDouble;
    Double_t tW2 = Reset::kDouble;
    Double_t tQ2 = Reset::kDouble;
    Double_t tEnuQE = Reset::kDouble;
    Double_t tQ2QE = Reset::kDouble;
    Double_t tXbj = Reset::kDouble;

    //reconstructed quantities
    Double_t LeptonPositionX = Reset::kDouble;
    Double_t LeptonPositionY = Reset::kDouble;
    Double_t LeptonPositionZ = Reset::kDouble;
    Double_t LeptonMomNom    = Reset::kDouble;
    Double_t LeptonCosNom    = Reset::kDouble;
    Double_t LeptonTPC1MomNom = Reset::kDouble;
    Double_t vtxX = Reset::kDouble;
    Double_t vtxY = Reset::kDouble;
    Double_t vtxZ = Reset::kDouble;
    Int_t NumberOfTracks = Reset::kInt;
    Int_t NPrimaryParticles[NMAXPRIMARYPARTICLESSIZE];
    for(Int_t index = 0; index < NMAXPRIMARYPARTICLESSIZE; ++index)
        NPrimaryParticles[index] = 0;
    Int_t inFGD1 = Reset::kInt;
    Int_t inFGD2 = Reset::kInt;

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
    else
    {
        std::cerr << "nWeights not set!" << std::endl;
        std::cerr << "Please set < psycheSteering.RunSyst.NWeights = N >" << std::endl;
        throw;
    }

    if(nWeights != nWeightSyst){
        std::cerr << "nWeights != nWeightSyst (" << nWeights << " != " << nWeightSyst << ")" << std::endl;
        std::cerr << "Change the hard coded value at the beginning of RunSyst_New.cxx" << std::endl;
        std::cerr << "or the parameter psycheSteering.RunSyst.NWeights " << std::endl;
        throw;
    }

    std::vector<EventWeightBase*>::iterator ew_iter;
    for (ew_iter = _man.eweight().GetEventWeights().begin(); ew_iter != _man.eweight().GetEventWeights().end(); ++ew_iter)
    {
        EventWeightBase* ewb = *ew_iter;
        if(!ewb)
            continue;
        if(!ewb->IsEnabled())
            continue;
        std::cout << ewb->GetName() << std::endl;
    }

    Double_t WeightIndToy[nWeights][nToys];
    Double_t FluxWeightToy[nToys];

    for (Int_t iToy = 0; iToy < nToys; ++iToy)
    {
        Toy            [iToy] = Reset::kDouble;
        TrueVertexIDToy[iToy] = Reset::kDouble;
        SelectionToy   [iToy] = Reset::kDouble;
        LeptonMomToy   [iToy] = Reset::kDouble;
        TrueEnuToy     [iToy] = Reset::kDouble;
        TrueNuPDGToy   [iToy] = Reset::kDouble;
        LeptonCosToy   [iToy] = Reset::kDouble;
        WeightToy      [iToy] = Reset::kDouble;
        for (Int_t iSyst = 0; iSyst < nWeights; ++iSyst)
            WeightIndToy[iSyst][iToy] = Reset::kDouble;
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

    const std::string weightTreeName = "weight";
    const std::string allEventsTreeName = "all";
    const std::string nominalTreeName = "nominal";
    if(RunOnInidividualSyst)
        systnametree.push_back(weightTreeName);
    if(RunAllSyst)
        systnametree.push_back(allEventsTreeName);
    if(!ThrowToys)
    {
        systnametree.clear();
        systnametree.push_back(nominalTreeName);
    }
    std::vector<Int_t> npassed(SampleId::kNSamples, 0);
    std::vector<std::string>::iterator str_iter;
    for (str_iter = systnametree.begin(); str_iter != systnametree.end(); ++str_iter)
    {
        const std::string syst_name = (*str_iter);
        std::cout << "Running over " << syst_name << std::endl;
        const Bool_t WeightSyst = syst_name == weightTreeName;
        const Bool_t AllSyst    = syst_name == allEventsTreeName;

        outfile->cd();
        tree = new TTree(syst_name.c_str(),syst_name.c_str());
        tree->Branch("Run",                           &Run, "Run/I");
        tree->Branch("SubRun",                     &SubRun, "SubRun/I");
        tree->Branch("EventNumber",           &EventNumber, "EventNumber/I");
        tree->Branch("RooVertexIndex",     &RooVertexIndex, "RooVertexIndex/i");

        tree->Branch("SelectionNom",         &SelectionNom, "SelectionNom/I");
        tree->Branch("TrueEnuNom",             &TrueEnuNom, "TrueEnuNom/D");
        tree->Branch("TrueNuPDGNom",         &TrueNuPDGNom, "TrueNuPDGNom/I");
        tree->Branch("TrueVertexIDNom",   &TrueVertexIDNom, "TrueVertexIDNom/I");
        tree->Branch("LeptonMomNom",         &LeptonMomNom, "LeptonMomNom/D");
        tree->Branch("LeptonTPC1MomNom", &LeptonTPC1MomNom, "LeptonTPC1MomNom/D");
        tree->Branch("LeptonCosNom",         &LeptonCosNom, "LeptonCosNom/D");
        tree->Branch("WeightNom",               &WeightNom, "WeightNom/D");
        tree->Branch("FluxWeightNom",       &FluxWeightNom, "FluxWeightNom/D");

        tree->Branch("tLeptonPDG", &tLeptonPDG, "tLeptonPDG/I");
        tree->Branch("tLeptonParentPDG", &tLeptonParentPDG, "tLeptonParentPDG/I");
        tree->Branch("tLeptonGParentPDG", &tLeptonGParentPDG, "tLeptonGParentPDG/I");
        tree->Branch("tLeptonMomentum", &tLeptonMomentum, "tLeptonMomentum/D");
        tree->Branch("tLeptonCosTheta", &tLeptonCosTheta, "tLeptonCosTheta/D");
        tree->Branch("tLeptonPhi", &tLeptonPhi, "tLeptonPhi/D");
        tree->Branch("vtxX" ,&vtxX ,"vtxX/D");
        tree->Branch("vtxY" ,&vtxY ,"vtxY/D");
        tree->Branch("vtxZ" ,&vtxZ ,"vtxZ/D");
        tree->Branch("EnuQE", &EnuQE, "EnuQE/D");
        tree->Branch("Q2QE", &Q2QE, "Q2QE/D");
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
        tree->Branch("tEnuQE", &tEnuQE, "tEnuQE/D");
        tree->Branch("tQ2QE", &tQ2QE, "tQ2QE/D");
        tree->Branch("tXbj", &tXbj, "tXbj/D");
        tree->Branch("tNu", &tNu, "tNu/D");
        tree->Branch("tYbj", &tYbj, "tYbj/D");
        tree->Branch("NumberOfTracks", &NumberOfTracks, "NumberOfTracks/I");
        tree->Branch("NPrimaryParticles", NPrimaryParticles, Form("NPrimaryParticles[%d]/I", NMAXPRIMARYPARTICLESSIZE));

        tree->Branch("inFGD1", &inFGD1,"inFGD1/I");
        tree->Branch("inFGD2", &inFGD2,"inFGD2/I");
        tree->Branch("tOnWaterTarget",&tOnWaterTarget,"tOnWaterTarget/I");

        Int_t* nToysPTR = const_cast<Int_t*>(&nToys);
        if(ThrowToys)
        {
            tree->Branch("nToys",            nToysPTR,        "nToys/I");
            tree->Branch("Toy",              Toy,             Form("Toy[%d]/I",              nToys));
            tree->Branch("TrueVertexIDToy",  TrueVertexIDToy, Form("TrueVertexIDToy[%d]/I",  nToys));
            tree->Branch("SelectionToy",     SelectionToy,    Form("SelectionToy[%d]/I",     nToys));
            tree->Branch("TrueEnuToy",       TrueEnuToy,      Form("TrueEnuToy[%d]/D",       nToys));
            tree->Branch("TrueNuPDGToy",     TrueNuPDGToy,    Form("TrueNuPDGToy[%d]/I",     nToys));
            tree->Branch("LeptonMomToy",     LeptonMomToy,    Form("LeptonMomToy[%d]/D",     nToys));
            tree->Branch("LeptonCosToy",     LeptonCosToy,    Form("LeptonCosToy[%d]/D",     nToys));
            tree->Branch("FluxWeightToy",    FluxWeightToy,  Form("FluxWeightToy[%d]/D",nToys));
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
        //Int_t rtvi = 0;
        //Int_t prevTruthID = -1;
        //Int_t prevRTV = -1;

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

            Bool_t FillTree = kFALSE;
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
                    _man.evar().InitializeEvent(_man.sel(), *event);
                }

                if (_man.eweight().HasEventWeights())
                {
if(debug) DEBUG(_man.eweight().HasEventWeights())
                    // Create the SystBox array (only the first time it is called for each EventWeight)
                    _man.eweight().Initialize(_man.sel(), nmax);

if(debug) std::cout << "Initialize The SystBox for variation systematics" << std::endl;
                    // Initialize The SystBox for variation systematics
                    _man.eweight().InitializeEvent(_man.sel(), *event);
                }
            }

            FluxWeightNom     = 1.;
            WeightNom         = 1.;
            TrueEnuNom        = Reset::kDouble;
            TrueNuPDGNom      = Reset::kDouble;
            LeptonMomNom      = Reset::kDouble;
            LeptonTPC1MomNom  = Reset::kDouble;
            LeptonCosNom      = Reset::kDouble;
            SelectionNom      = Reset::kDouble;
            TrueVertexIDNom   = Reset::kDouble;
            SelectionNom      = Reset::kDouble;
            tLeptonPDG        = Reset::kDouble;
            tLeptonParentPDG  = Reset::kDouble;
            tLeptonGParentPDG = Reset::kDouble;
            tLeptonMomentum   = Reset::kDouble;
            tLeptonCosTheta   = Reset::kDouble;
            tLeptonPhi        = Reset::kDouble;
            LeptonPositionX   = Reset::kDouble;
            LeptonPositionY   = Reset::kDouble;
            LeptonPositionZ   = Reset::kDouble;
            tLeptonPositionX  = Reset::kDouble;
            tLeptonPositionY  = Reset::kDouble;
            tLeptonPositionZ  = Reset::kDouble;
            tVtxX             = Reset::kDouble;
            tVtxY             = Reset::kDouble;
            tVtxZ             = Reset::kDouble;
            tW2               = Reset::kDouble;
            tQ2               = Reset::kDouble;
            tEnuQE            = Reset::kDouble;
            tQ2QE             = Reset::kDouble;
            tNu               = Reset::kDouble;
            tYbj              = Reset::kDouble;
            tXbj              = Reset::kDouble;
            vtxX              = Reset::kDouble;
            vtxY              = Reset::kDouble;
            vtxZ              = Reset::kDouble;
            EnuQE             = Reset::kDouble;
            Q2QE              = Reset::kDouble;
            tReactionCode     = Reset::kInt;
            NumberOfTracks    = Reset::kInt;
            tOnWaterTarget    = Reset::kInt;
            RooVertexIndex    = Reset::kUInt;
            for(Int_t index = 0; index < NMAXPRIMARYPARTICLESSIZE; ++index)
                NPrimaryParticles[index] = 0;

            //_man.syst().InitializeEventSystematics(_man.sel(),*event);
            Bool_t passednom = kFALSE;

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
                FillTree = true;
                //AnaVertexB** Vertices = event->Vertices;
                const Int_t &nParticles = event->nParticles;
if(debug) std::cout << "There are " << nParticles  << " particles in this event" <<std::endl;
                AnaEventInfoB* eventInfo = static_cast<AnaEventInfoB*>(&event->EventInfo);
                Run         = eventInfo->Run;
                SubRun      = eventInfo->SubRun;
                EventNumber = eventInfo->Event;

                AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(event->Summary);
                if(!summary)
                    continue;
if(debug) DEBUG(summary->EventSample)
                RooVertexIndex = summary->RooVertexIndex[summary->EventSample];
                npassed[summary->EventSample]++;
                AnaParticleMomB* lepCand = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample]);
                AnaTrueVertexB* trVtx = NULL;
                AnaTrueParticleB *trueParticle = NULL;
                AnaTrueParticleB *trueLepton = NULL;
                //fill MC info
                if(!isData)
                {
                    FluxWeightNom   = fluxWeightSyst.Correction;
                    WeightNom       = totalweight.Correction;
                    trueParticle = lepCand->GetTrueParticle();
                    if(trueParticle)
                    {
if(debug) DEBUG(trueParticle->PDG)
                        TrueVertexIDNom = trueParticle->VertexID;
                        tLeptonMomentum = trueParticle->Momentum;
                        tLeptonPDG = trueParticle->PDG;
                        tLeptonParentPDG = trueParticle->ParentPDG;
                        tLeptonGParentPDG = trueParticle->GParentPDG;
                        tLeptonPositionX = trueParticle->Position[0];
                        tLeptonPositionY = trueParticle->Position[1];
                        tLeptonPositionZ = trueParticle->Position[2];
                        tLeptonCosTheta = trueParticle->Direction[2];
                        tLeptonPhi = std::atan2(trueParticle->Direction[1], trueParticle->Direction[0]);
                    }
                    trVtx = summary->TrueVertex[summary->EventSample];
                    if(trVtx)
                    {
                        tVtxX         = trVtx->Position[0];
                        tVtxY         = trVtx->Position[1];
                        tVtxZ         = trVtx->Position[2];
                        tReactionCode = trVtx->ReacCode;
                        TrueEnuNom    = trVtx->NuEnergy;
                        TrueNuPDGNom  = trVtx->NuPDG;
                        tQ2           = trVtx->Q2;

                        FillNPrimaryParticles(trVtx, NPrimaryParticles);
                        CheckIfNegative("Enu", TrueEnuNom);
                        CheckIfNegative("4-momentum transfer Q2", tQ2);

                        trueLepton = GetTrueOutgoingLepton(trVtx);
                        if(trueLepton && tQ2 > 0)
                        {
                            const Double_t tMmu = anaUtils::GetParticleMass(ParticleId::GetParticle(trueLepton->PDG));
                            if(tMmu > 0)
                            {
                                using namespace units;
                                const Double_t &tPmu = trueLepton->Momentum;
                                const Double_t  tEmu = sqrt(tPmu * tPmu + tMmu * tMmu);
                                const Double_t &massNucleon = mass_proton;
                                //const Double_t massNucleon = (TrueNuPDGNom < 0) ? mass_neutron : mass_proton;

                                tNu  = TrueEnuNom - tEmu;
                                tYbj = tNu / TrueEnuNom;

                                const Double_t twoMnu = (2.0 * massNucleon * tNu);
                                tXbj = tQ2 / twoMnu;
                                tW2  = (massNucleon * massNucleon) + twoMnu - tQ2;

                                CheckIfNegative("Invariate mass W2", tW2 * 1e-6);

                                const TVector3 dirMu(trueLepton->Direction);
                                const TVector3 vtxPos(trVtx->Position);
                                const Double_t cosThetaMu = GetCosAngleWRTBeam(vtxPos, dirMu);
                                if(tReactionCode < 0)
                                {
                                    tEnuQE = (+(mass_neutron * mass_neutron) - (mass_proton * mass_proton)
                                             - (mass_muon * mass_muon) + (2 * mass_proton * tEmu))
                                             / (2.0 * (mass_proton - tEmu + tPmu * cosThetaMu));
                                }
                                else
                                {
                                    tEnuQE = (-(mass_neutron * mass_neutron) + (mass_proton * mass_proton)
                                             - (mass_muon * mass_muon) + (2 * mass_neutron * tEmu))
                                             / (2.0 * (mass_neutron - tEmu + tPmu * cosThetaMu));
                                }
                                tQ2QE = (tPmu * tPmu + tEnuQE * tEnuQE - 2 * tPmu * tEnuQE * cosThetaMu)
                                        + (tEmu - tEnuQE) * (tEmu - tEnuQE);
                            }
                        }

                        if(geoManager && (SampleId::IsP0DSelection(summary->EventSample)))
                        {
                            const TLorentzVector startPos(trVtx->Position);
                            tOnWaterTarget = IsPositionInWaterVolume(geoManager, startPos);
                        }
                    }
                }

                Float_t* recoVtx = summary->VertexPosition[summary->EventSample];
                Float_t* lepStart = lepCand->PositionStart;
                vtxX = recoVtx[0];
                vtxY = recoVtx[1];
                vtxZ = recoVtx[2];
                SelectionNom    = summary->EventSample;
                LeptonPositionX = lepStart[0];
                LeptonPositionY = lepStart[1];
                LeptonPositionZ = lepStart[2];
                LeptonMomNom    = lepCand->Momentum;
                LeptonCosNom    = lepCand->DirectionStart[2];
                {
                    using namespace units;
                    const TVector3 recoVtxPosition(recoVtx);
                    const TVector3 trkDirection(lepCand->DirectionStart);
                    const Double_t cosThetaMu = GetCosAngleWRTBeam(recoVtxPosition, trkDirection);
                    const Double_t Emu = sqrt(LeptonMomNom * LeptonMomNom + mass_muon * mass_muon);
                    if(static_cast<SampleId::SampleEnum>(SelectionNom) == SampleId::kP0DWaterNuMuBarInAntiNuModeCC ||
                       static_cast<SampleId::SampleEnum>(SelectionNom) == SampleId::kP0DWaterNuMuBarInAntiNuModeCC1Track ||
                       static_cast<SampleId::SampleEnum>(SelectionNom) == SampleId::kP0DWaterNuMuBarInAntiNuModeCCNTracks  ||
                       static_cast<SampleId::SampleEnum>(SelectionNom) == SampleId::kP0DAirNuMuBarInAntiNuModeCC ||
                       static_cast<SampleId::SampleEnum>(SelectionNom) == SampleId::kP0DAirNuMuBarInAntiNuModeCC1Track ||
                       static_cast<SampleId::SampleEnum>(SelectionNom) == SampleId::kP0DAirNuMuBarInAntiNuModeCCNTracks)
                    {
                        EnuQE = ((mass_neutron * mass_neutron) - (mass_proton * mass_proton) -
                                (mass_muon * mass_muon) + (2.0 * mass_neutron * Emu)) /
                                (2.0 * (mass_neutron - Emu + LeptonMomNom * cosThetaMu));
                    }
                    else
                    {
                        EnuQE = (-(mass_neutron * mass_neutron) + (mass_proton * mass_proton) -
                                (mass_muon * mass_muon) + (2.0 * mass_proton * Emu)) /
                                (2.0 * (mass_proton - Emu + LeptonMomNom * cosThetaMu));
                    }
                    Q2QE = (LeptonMomNom * LeptonMomNom + EnuQE * EnuQE - 2 * LeptonMomNom * EnuQE * cosThetaMu) + (Emu - EnuQE) * (Emu - EnuQE);
                }

                // Get all tracks
                EventBoxB* EventBox = event->EventBoxes[EventBoxId::kEventBoxTracker];
                NumberOfTracks = EventBox->nRecObjectsInGroup[EventBoxTracker::kTracksWithP0D];

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
            for (Int_t iToy = 0; iToy < nToys && ThrowToys; ++iToy)
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

                        if(WeightSyst && ZeroVarSyst->Type() == SystematicBase::kVariation)
                        {
                            for(UInt_t iparam = 0; iparam < ZeroVarSyst->GetNParameters(); ++iparam)
                                reducedtoy->SetToyVariation(ZeroVarSyst->GetIndex(), iparam, 0, 1);
                            continue;
                        }
                        // Doesnt deactivate if this is the current syst || Doesn't deactivate any if it's allsyst
                        if(ZeroVarSyst->Name() == syst_name || AllSyst || WeightSyst)
                            continue;
                        for(UInt_t iparam = 0; iparam < ZeroVarSyst->GetNParameters(); ++iparam)
                            reducedtoy->SetToyVariation(ZeroVarSyst->GetIndex(), iparam, 0, 1);
                    }
                }

                Toy[iToy]             = iToy;
                SelectionToy[iToy]    = Reset::kDouble;
                TrueEnuToy[iToy]      = Reset::kDouble;
                TrueNuPDGToy[iToy]    = Reset::kDouble;
                LeptonMomToy[iToy]    = Reset::kDouble;
                LeptonCosToy[iToy]    = Reset::kDouble;
                WeightToy[iToy]       = Reset::kDouble;
                FluxWeightToy[iToy]   = Reset::kDouble;
                TrueVertexIDToy[iToy] = Reset::kInt;
                for (Int_t iSyst = 0; iSyst < nWeights; ++iSyst)
                    WeightIndToy[iSyst][iToy] = Reset::kDouble;


                // 3. ====================================================================
                // Process the current event (bunch). That means applying the systematics, the selections and computing the weights
                Bool_t toyPassed = kFALSE;
                Weight_h* weights = NULL;
                // std::cout << "entry " << entry << std::endl;

                if(!WeightSyst)
                {
                    toyPassed = _man.ProcessEvent(*reducedtoy, *event, totalweight, fluxWeightSyst);
                }
                else
                {
                    weights = new Weight_h[NMAXSYSTEMATICS];
                    toyPassed = _man.ProcessEventWithIndividualWeightSyst(*reducedtoy, *event, weights, fluxWeightSyst);
                }
                if(toyPassed)
                {
                    FillTree = true;
                    AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(event->Summary);
                    if(summary && summary->EventSample)
                    {
                        LeptonMomToy [iToy] = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->Momentum;
                        LeptonCosToy [iToy] = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->DirectionStart[2];
                        SelectionToy [iToy] = summary->EventSample;
                        FluxWeightToy[iToy] = fluxWeightSyst.Correction;
                        if(!WeightSyst)
                            WeightToy[iToy] = totalweight.Systematic;
                        else
                        {
                            Int_t ind = 0;
                            for(UInt_t i = 0; i < NMAXSYSTEMATICS; ++i)
                            {
                                if(weights[i].Systematic != Reset::kDouble)
                                {
                                    WeightIndToy[ind][iToy] = weights[i].Systematic;
                                    //std::cout << "WeightIndToy[" << ind << "][" << iToy << "] " << WeightIndToy[ind][iToy] <<std::endl;
                                    ind++;
                                }
                            }
                            if(ind != nWeights)
                            {
                                std::cerr << "something wrong is happening" << std::endl;
                                throw;
                            }
                        }
                        if(summary->TrueVertex[summary->EventSample])
                        {
                            const AnaParticleMomB* momB = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample]);
                            TrueVertexIDToy[iToy] = momB->GetTrueParticle()->VertexID;
                            TrueEnuToy     [iToy] = summary->TrueVertex[summary->EventSample]->NuEnergy;
                            TrueNuPDGToy   [iToy] = summary->TrueVertex[summary->EventSample]->NuPDG;
                        }
                    }
                } //end if toy passed selection
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

    /*
    TTree* config = NULL;
    if(inputFile->Get("config"))
    {
        std::cout << "Copying the config Tree" << std::endl;
        config = static_cast<TTree*>(inputFile->Get("config"));
        outfile->cd();
        config->CloneTree()->Write();
    }
    */

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

  Int_t returnP0DuleNumber = atoi(tmpInSuperP0Dule[1].c_str());
  Int_t returnXorYLayer = -9999;

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
  (void)returnXorYLayer;
  return (returnP0DuleNumber+ SuperP0DuleOffset + P0DuleByP0DuleOffset)*10 + IsOnWater;
}


std::vector<std::string> SplitString(const std::string &inString, const char SplitBy)
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

void FillNPrimaryParticles(const AnaTrueVertexB* const vertex, Int_t* NPrimaryParticles){

  for (Int_t i = 0; i < vertex->nTrueParticles; i++)
  {
      AnaTrueParticleB* trueTrack = vertex->TrueParticles[i];

      if(!trueTrack)
          continue;

      if(trueTrack->ParentPDG != 0)
          continue; //should correspond to the primary vertex

      if (abs(trueTrack->PDG) > 1000 && abs(trueTrack->PDG) < 10000) NPrimaryParticles[ParticleId::kBaryons]++;
      if (abs(trueTrack->PDG) > 100 && abs(trueTrack->PDG) < 1000) NPrimaryParticles[ParticleId::kMesons]++;
      if (abs(trueTrack->PDG) > 10 && abs(trueTrack->PDG) < 19) NPrimaryParticles[ParticleId::kLeptons]++;
      if (trueTrack->PDG == +12 || trueTrack->PDG == +14 || trueTrack->PDG == +16) NPrimaryParticles[ParticleId::kNeutrinos]++;
      if (trueTrack->PDG == -12 || trueTrack->PDG == -14 || trueTrack->PDG == -16) NPrimaryParticles[ParticleId::kAntiNeutrinos]++;
      if (trueTrack->PDG == ParticleId::kMuonPDG) NPrimaryParticles[ParticleId::kMuon]++;
      if (trueTrack->PDG == ParticleId::kAntiMuonPDG) NPrimaryParticles[ParticleId::kAntiMuon]++;
      if (trueTrack->PDG == ParticleId::kPiPlusPDG) NPrimaryParticles[ParticleId::kPiPos]++;
      if (trueTrack->PDG == ParticleId::kPiMinusPDG) NPrimaryParticles[ParticleId::kPiNeg]++;
      if (trueTrack->PDG == ParticleId::kPiZeroPDG) NPrimaryParticles[ParticleId::kPi0]++;
      //if (trueTrack->PDG == ParticleId::kK0PDG) NPrimaryParticles[ParticleId::kK0]++;
      //if (trueTrack->PDG == ParticleId::kAntiK0PDG) NPrimaryParticles[ParticleId::kAntiK0]++;

  } // end loop over vertex->TrueParticles

  // Fill NPrimaryParticles for kPions and Kaons
  NPrimaryParticles[ParticleId::kPions] = NPrimaryParticles[ParticleId::kPi0]   +
                                                  NPrimaryParticles[ParticleId::kPiPos] +
                                                  NPrimaryParticles[ParticleId::kPiNeg] ;
  NPrimaryParticles[ParticleId::kKaons] = NPrimaryParticles[ParticleId::kK0]     +
                                                  NPrimaryParticles[ParticleId::kAntiK0] +
                                                  NPrimaryParticles[ParticleId::kK0L]    +
                                                  NPrimaryParticles[ParticleId::kK0S]    +
                                                  NPrimaryParticles[ParticleId::kKPos]   +
                                                  NPrimaryParticles[ParticleId::kKNeg]   ;

}

AnaTrueParticleB* GetTrueOutgoingLepton(AnaTrueVertexB* trueVertex)
{
    if(!trueVertex)
        return NULL;
    //NC events have no true observable lepton
    if(abs(trueVertex->ReacCode) >= 31)
        return NULL;
    AnaTrueParticleB** particles = trueVertex->TrueParticles;
    if(!particles)
        return NULL;
    const Int_t nTrueParticles = trueVertex->nTrueParticles;
    for(Int_t particleIndex = 0; particleIndex < nTrueParticles; ++particleIndex)
    {
        AnaTrueParticleB* trueParticle = particles[particleIndex];
        if(!trueParticle)
            continue;
        const Int_t absParentPDG = abs(trueParticle->ParentPDG);
        const Int_t absPDG = abs(trueParticle->PDG);
        const Int_t absNuPDG = abs(trueVertex->NuPDG);
        //first try to get muon for CC
        if(absPDG == ParticleId::kMuonPDG && absNuPDG == 14 && absParentPDG == 0)
            return trueParticle;
        //try to get electron for CC
        if(absPDG == ParticleId::kElectronPDG && absNuPDG == 12 && absParentPDG == 0)
            return trueParticle;
    }
    return NULL;
}


TVector3 GetNuParentDecayPoint()
{
    return tNuParentDecayPointoaAnalysisCoor;
}

TVector3 GetNuDirection(const TVector3 &vertex)
{
    return (vertex - GetNuParentDecayPoint()).Unit();
}

Double_t GetAngleWRTBeam(const TVector3 &vertex, const TVector3 &trkDirection)
{
    return GetNuDirection(vertex).Angle(trkDirection);
}

Double_t GetCosAngleWRTBeam(const TVector3 &vertex, const TVector3 &trkDirection)
{
    return TMath::Cos(GetAngleWRTBeam(vertex,trkDirection));
}

void CheckIfNegative(const char* const name, const Double_t &value)
{
    if(value < 0) printf("WARNING: %s (%f) is negative! \n", name, value);
}
