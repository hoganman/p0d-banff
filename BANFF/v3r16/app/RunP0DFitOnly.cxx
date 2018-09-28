#define RUNP0DFITONLY_CXX

#include "psycheInterface/psycheInterface.hxx"
#include "BANFFSample/BANFFBinnedSample.hxx"
#include "psycheInterface/psycheObservables.hxx"
#include "BANFFObservableBase.hxx"
#include "FitParameters.hxx"
#include "Parameters.hxx"
#include "BANFFSample/BANFFBinnedSampleUtils.hxx"
#include "Minuit2LikelihoodFit/Minuit2LikelihoodFit.cxx"
#include "XMLTools.hxx"
#include <sstream>

inline void Declare(const std::string& str)
{
    std::cout << str.c_str() << std::endl;
}

void Usage(const char* programName)
{
    using namespace std;
    cout << programName << " [INPUTS]" << endl;
    cout << endl;
    cout << "INPUTS" << endl;
    cout << "    -h this message" << endl;
    cout << "    -i path/to/Samples.xml: The path to the XML file containing";
    cout << " sample information" << endl;
    cout << "    -o outputFile.root: The name of the output ROOT file" << endl;
    exit(0);
}

//Set up the samples and observables with proper binning, then load the
//parameters and perform the fit.
int main(int argc, char** argv){

    std::string outputFileName = "";
    std::string inputFileName = "";

    if(argc > 2){

        std::cout << "Too many command line arguments!" << std::endl;
    }

    //If there is 1 command line argument, it is specifying a parameter
    //override file.  Read this in now.
    //According to the documentation for ReadParamOverrideFile, the code will
    //definitely use these values of the parameters, ignoring the contents of
    //any other parameters file.
    else if(argc == 2){

        std::string paramFile(argv[1]);
        ND::params().ReadParamOverrideFile(paramFile);
    }

    ND::params().SetReadParamOverrideFilePointPassed();

    //Create the observables we will use.
    psycheLeptonCandidateMomentum* pmu  = new psycheLeptonCandidateMomentum();
    psycheLeptonCandidateCosTheta* thmu = new psycheLeptonCandidateCosTheta();

    BANFFObservableBase** observables = new BANFFObservableBase*[2];
    observables[0] = pmu;
    observables[1] = thmu;

    //For each observable and sample set up the binning used in the fit.

    //Set the binning for the CCp0d_water_inc sample.
    Int_t npbins_p0d_water_inc = 6;
    Double_t pbins_p0d_water_inc[7] = {0.,450.,700.,1100.,1600.,2600.,5000.};
    Int_t nctbins_p0d_water_inc = 6;
    Double_t ctbins_p0d_water_inc[7] = {0.0,+0.6,+0.8,+0.90,+0.95,+0.975,+1.0};

    TAxis* pAxis_p0d_water_inc = new TAxis(npbins_p0d_water_inc, pbins_p0d_water_inc);
    TAxis* thAxis_p0d_water_inc = new TAxis(nctbins_p0d_water_inc, ctbins_p0d_water_inc);
    TAxis** ccp0d_water_inc_axes = new TAxis*[2];
    ccp0d_water_inc_axes[0] = pAxis_p0d_water_inc;
    ccp0d_water_inc_axes[1] = thAxis_p0d_water_inc;

    //Set the binning for the CCp0d_air_inc sample.
    Int_t npbins_p0d_air_inc = 6;
    Double_t pbins_p0d_air_inc[7] = {0.,450.,700.,1100.,1600.,2600.,5000.};
    Int_t nctbins_p0d_air_inc = 6;
    Double_t ctbins_p0d_air_inc[7] = {0.0,+0.6,+0.8,+0.90,+0.95,+0.975,+1.0};

    TAxis* pAxis_p0d_air_inc = new TAxis(npbins_p0d_air_inc, pbins_p0d_air_inc);
    TAxis* thAxis_p0d_air_inc = new TAxis(nctbins_p0d_air_inc, ctbins_p0d_air_inc);
    TAxis** ccp0d_air_inc_axes = new TAxis*[2];
    ccp0d_air_inc_axes[0] = pAxis_p0d_air_inc;
    ccp0d_air_inc_axes[1] = thAxis_p0d_air_inc;

    //Create the samples.
    //Supply the:
    //name
    //sampleID
    //Number of observables.
    //Array of observables.
    //Axes for the histograms.
    //Whether to throw MC statistical errors.
    //Whether to throw statistical errors
    bool throwMCStat = ND::params().GetParameterI("BANFF.RunFit.ThrowMCStat");
    bool throwStat = ND::params().GetParameterI("BANFF.RunFit.ThrowStat");

    int nSamples = 2;
    // // FHC Numu selections
    BANFFBinnedSample* fhc_p0d_water_numuccinc = new BANFFBinnedSample("fhc_p0d_water_numuccinc", SampleId::kP0DWaterNuMuCC, 2, observables, ccp0d_water_inc_axes, throwMCStat, throwStat);
    BANFFBinnedSample* fhc_p0d_air_numuccinc = new BANFFBinnedSample("fhc_p0d_air_numuccinc", SampleId::kP0DAirNuMuCC, 2, observables, ccp0d_air_inc_axes, throwMCStat, throwStat);

    BANFFSampleBase** samples = new BANFFSampleBase*[nSamples];
    samples[0] = fhc_p0d_water_numuccinc;
    samples[1] = fhc_p0d_air_numuccinc;

    //With the samples ready, now define the parameters that are going to be
    //considered.
    //TODO: Again, maybe some of this can be moved to a backend?
    FitParameters* fitParameters = new FitParameters();

    //Load the Flux Parameters.
    //Specify to fit them.
    if(ND::params().GetParameterI("BANFF.RunFit.LoadFluxParams"))
    {
        std::cout << "Load flux parameters" << std::endl;
        fitParameters->LoadFluxParametersFromFile(
                ND::params().GetParameterS("BANFF.FluxInputFile"),
                ND::params().GetParameterI("BANFF.RunFit.LoadNuModeFluxParams"),
                ND::params().GetParameterI("BANFF.RunFit.LoadAntiNuModeFluxParams"),
                ND::params().GetParameterI("BANFF.RunFit.FitFluxParams"));
        std::cout << "DONE loading flux parameters" << std::endl;
    }

    //Load the XSec Parameters and specify to fit them
    if(ND::params().GetParameterI("BANFF.RunFit.LoadXSecParams"))
    {
        std::cout << "Load XSec parameters" << std::endl;
        fitParameters->LoadXSecParametersFromFile(
                ND::params().GetParameterS("BANFF.XSecInputFile"),
                ND::params().GetParameterI("BANFF.RunFit.FitXSecParams"));
        std::cout << "Done loading XSec parameters" << std::endl;
    }

    //Load the ObsNorm parameters, and specify to fit them.
    if(ND::params().GetParameterI("BANFF.RunFit.LoadObsNormParams"))
    {
        std::cout << "Load ObsNorm parameters" << std::endl;
        fitParameters->LoadObsNormParametersFromFile(
                ND::params().GetParameterS("BANFF.ObsNormInputFile"),
                nSamples, samples,
                ND::params().GetParameterI("BANFF.RunFit.FitObsNormParams"));
        std::cout << "DONE loading ObsNorm parameters" << std::endl;
    }

    //We can save pre-fit information to a file here, since everything that has
    //been loaded will be.
    //Just make it iteration 0, save the names, and don't access the
    //undecomposed variable.

    std::string outputName = ND::params().GetParameterS("BANFF.RunFit.OutputFile");
    //std::string PsycheVersion = anaUtils::GetSoftwareVersionFromPath((std::string)getenv("ND280PSYCHEROOT"));
    //outputName = PsycheVersion+"_"+outputName;

    if(ND::params().GetParameterI("BANFF.SavePrefitParamsAndExit"))
    {
        TFile* preFitFile = new TFile(("prefit_"+outputName).c_str(),"RECREATE");
        preFitFile->cd();
        fitParameters->WriteResults(0, true, false);
        preFitFile->Close();
        exit(0);
    }

    //Now that all parameters are added, flag the flux and FSI parameters for
    //decomposition.
    for(int i = 0; i < fitParameters->GetNumbSysts(); i++)
    {

        if(fitParameters->GetParamType(i)==FLUX)
            fitParameters->SetDecompose(i,1);

        if(fitParameters->GetParamName(i).find("FSI")!=std::string::npos)
        {
            std::cout << "Decompose " << fitParameters->GetParamName(i) << std::endl;
            fitParameters->SetDecompose(i,1);
        }
    }

    //Create the psyche interface.  Do not load in or vary the detector
    //parameters from psyche.
    //NB: Although there are program parameters for this, it is something that
    //would never be done.

    std::string diagnostic_name = "diagnostic_" + outputName;

    psycheInterface* interface = new psycheInterface(nSamples, samples, fitParameters, false, false, diagnostic_name);

    //Create the fitter that we want, and pass it the interface and the name of
    //the output file.
    Minuit2LikelihoodFit* fitter = new Minuit2LikelihoodFit(interface, outputName);

    //If requested in the program parameters file, fit to the data loaded in by
    //psyche.
    if(ND::params().GetParameterI("BANFF.RunFit.FitData"))
    {

        fitter->outputFile->cd();
        interface->BuildData();
        interface->BuildNominalMC();

        for(int si = 0; si < interface->nSamples; si++)
        {
            std::string dataName = interface->samples[si]->name + "_data";
            std::string mcName   = interface->samples[si]->name + "_prefit";
            samples[si]->SaveData(dataName);
            samples[si]->SaveNominalMC(mcName);
        }

        if(ND::params().GetParameterI("BANFF.SavePrefitAndExit"))
            exit(0);

        fitter->InitializeParameters();
        fitter->DoFit();
    }

    //Otherwise, if the nominal MC flag is set, build the nominal MC then use
    //it as the data to fit.
    else if(ND::params().GetParameterI("BANFF.RunFit.FitNominalMC"))
    {

        fitter->outputFile->cd();
        interface->BuildNominalMC();

        for(int si = 0; si < interface->nSamples; si++)
        {
            samples[si]->UseNominalMCAsData();
            std::string dataName = interface->samples[si]->name + "_data";
            std::string mcName   = interface->samples[si]->name + "_prefit";
            samples[si]->SaveData(dataName);
            samples[si]->SaveNominalMC(mcName);
        }

        if(ND::params().GetParameterI("BANFF.SavePrefitAndExit"))
            exit(0);

        fitter->InitializeParameters();
        fitter->DoFit();
    }

    //Otherwise, fit to a number of toy MC histgorams in a file specified in
    //the program parameters file.
    //Taking advantage of this allows performing multiple validation fits
    //without having to spend time reloading the MC.
    else
    {
        //Set up the file containing the data.
        TFile* toyFile = new TFile((ND::params().GetParameterS("BANFF.RunFit.ToyFile")).c_str());

        //Loop over the toys.
        for(int i = ND::params().GetParameterI("BANFF.RunFit.FirstToyToFit");
                i < ND::params().GetParameterI("BANFF.RunFit.FirstToyToFit") + ND::params().GetParameterI("BANFF.RunFit.NToysToFit");
                i++)
        {

            fitter->outputFile->cd();
            interface->BuildNominalMC();

            //Loop over the samples.
            for(int si = 0; si < interface->nSamples; si++)
            {
                std::stringstream histoStream;
                histoStream << samples[si]->name << "_predMC_" << i;

                //Load the predicted MC histogram in for this sample's data
                //histogram.
                samples[si]->LoadDataFromFile(toyFile,histoStream.str());

                std::string dataName = interface->samples[si]->name + "_data";
                std::string mcName = interface->samples[si]->name + "_prefit";
                samples[si]->SaveData(dataName);
                samples[si]->SaveNominalMC(mcName);
            }
            if(ND::params().GetParameterI("BANFF.SavePrefitAndExit"))
                exit(0);
            fitter->InitializeParameters();

            //DoFit has an internal counter that will automatically iterate the
            //names of the fit output to fit multiple toys and save the output
            //to the same file..
            fitter->DoFit();
        }

        toyFile->Close();
    }
    /*
    for (;;)
    {
        Int_t c = getopt(argc, argv, "i:o:h");
        if (c < 0)
            break;
        switch (c)
        {
            case 'i':
            {
                inputFileName = optarg;
                break;
            }
            case 'h':
            {
                Usage(argv[0]);
            }
            case 'o':
            {
                outputFileName = optarg;
                break;
            }
            default:
            {
               Usage(argv[0]);
            }
        }
    }//end get options

    if(outputFileName.length() < 1)
        Usage(argv[0]);

    if(inputFileName.length() < 1)
    {
        std::cout << "WARNING: Using "
                  << inputFileName.c_str()
                  << " as input file since none was provided!"
                  << std::endl;
        inputFileName = Form("%s/inputs/Binning.xml", getenv("BANFFROOT"));
    }

    if(outputFileName.length() < 1)
    {
        std::cout << "ERROR: No output name given" << std::endl;
        Usage(argv[0]);
    }

    //If there is 1 command line argument, it is specifying a parameter
    //override file.  Read this in now.
    //According to the documentation for ReadParamOverrideFile, the code will
    //definitely use these values of the parameters, ignoring the contents of
    //any other parameters file.
    if(argc == 4){

        std::string paramFile(argv[3]);
        ND::params().ReadParamOverrideFile(paramFile);
    }

    ND::params().SetReadParamOverrideFilePointPassed();
    Declare("Loading parameters!");
    const Bool_t throwMCStat              = ND::params().GetParameterI("BANFF.RunFit.ThrowMCStat");
    const Bool_t throwStat                = ND::params().GetParameterI("BANFF.RunFit.ThrowStat");
    const Bool_t loadXSecParams           = ND::params().GetParameterI("BANFF.RunFit.LoadXSecParams");
    const Bool_t loadObsNormParams        = ND::params().GetParameterI("BANFF.RunFit.LoadObsNormParams");
    const Bool_t runDataFit               = ND::params().GetParameterI("BANFF.RunFit.FitData");
    const Bool_t runNominalMCFit          = ND::params().GetParameterI("BANFF.RunFit.FitNominalMC");
    const Bool_t loadFluxParams           = ND::params().GetParameterI("BANFF.ThrowToys.LoadFluxParams");
    const Bool_t throwXSecParams          = ND::params().GetParameterI("BANFF.ThrowToys.ThrowXSecParams");
    const Bool_t loadNuModeFluxParams     = ND::params().GetParameterI("BANFF.ThrowToys.LoadNuModeFluxParams");
    const Bool_t loadAntiNuModeFluxParams = ND::params().GetParameterI("BANFF.ThrowToys.LoadAntiNuModeFluxParams");
    const Bool_t throwFluxParams          = ND::params().GetParameterI("BANFF.ThrowToys.ThrowFluxParams");
    const Bool_t throwObsNormParams       = ND::params().GetParameterI("BANFF.ThrowToys.ThrowObsNormParams");
    const Bool_t loadDetParams            = ND::params().GetParameterI("BANFF.ThrowToys.LoadDetParams");
    const Bool_t throwDetParams           = ND::params().GetParameterI("BANFF.ThrowToys.ThrowDetParams");
    const Bool_t savePrefitAndExit        = ND::params().GetParameterI("BANFF.SavePrefitParamsAndExit");
    const std::string fluxInputFile     = ND::params().GetParameterS("BANFF.FluxInputFile");
    const std::string xSecInputFile     = ND::params().GetParameterS("BANFF.XSecInputFile");
    const std::string obsNormInputFile  = ND::params().GetParameterS("BANFF.ObsNormInputFile");
    const std::string outputName        = ND::params().GetParameterS("BANFF.RunFit.OutputFile");

    const std::string pBinsEdges = "pBinsEdges";
    const std::string ctBinsEdges = "ctBinsEdges";
    const std::string pBinsEdgesN = "pBinsEdgesN";
    const std::string ctBinsEdgesN = "ctBinsEdgesN";
    const std::string SampleId = "SampleId";

    //Create the observables we will use.
    psycheLeptonCandidateMomentum* pmu  = new psycheLeptonCandidateMomentum();
    psycheLeptonCandidateCosTheta* thmu = new psycheLeptonCandidateCosTheta();

    Int_t nObs = 2;
    BANFFObservableBase** observables = new BANFFObservableBase*[2];
    observables[0] = pmu;
    observables[1] = thmu;

    //For each observable and sample set up the binning used in the fit.

    //Set the samples and binning using a custom XML parsing class
    XMLTools xml(inputFileName.c_str());
    std::map<TString, XMLTools::AttributeMap> samples_attributes = xml.GetAllNodes();
    std::vector<BANFFBinnedSample> banffsamplesVect;
    std::map<TString, XMLTools::AttributeMap>::iterator map_iter;
    for(map_iter  = samples_attributes.begin();
        map_iter != samples_attributes.end();
        ++map_iter
       )
    {
        const char* sample_name = map_iter->first.Data();
        Declare(Form("Loading sample %s...", sample_name));
        XMLTools::AttributeMap attributes = map_iter->second;
        SampleId::SampleEnum sampleId = SampleId::ConvertSample(attributes[SampleId].Data());
        TAxis* axes[2];
        std::vector<TString> pBinsEdgesStr = XMLTools::SplitString(attributes[pBinsEdges]);
        std::vector<TString> ctBinsEdgesStr = XMLTools::SplitString(attributes[ctBinsEdges]);
        std::vector<Double_t> pBinsEdges(pBinsEdgesStr.size());
        std::vector<Double_t> ctBinsEdges(ctBinsEdgesStr.size());
        for(UInt_t index = 0; index < pBinsEdges.size(); ++index)
        {
            pBinsEdges.at(index) = pBinsEdgesStr.at(index).Atof();
        }
        UInt_t attr_pBinsEdgesN = attributes[pBinsEdgesN.c_str()].Atoi();
        UInt_t pBinsEdges_size = pBinsEdges.size();
        if(attr_pBinsEdgesN != pBinsEdges_size)
        {
            std::cout << "ERROR: the number of stated pBins for "
                      << sample_name
                      << " does not match in the file: "
                      << xml.GetXMLFile().Data() << std::endl;
            exit(1);
        }
        for(UInt_t index = 0; index < ctBinsEdges.size(); ++index)
        {
            ctBinsEdges.at(index) = ctBinsEdgesStr.at(index).Atof();
        }
        UInt_t attr_ctBinsEdgesN = attributes[ctBinsEdgesN.c_str()].Atoi();
        UInt_t ctBinsEdges_size = ctBinsEdges.size();
        if(attr_ctBinsEdgesN != ctBinsEdges_size)
        {
            std::cout << "ERROR: the number of stated ctBins for "
                      << sample_name
                      << "does not match in the file: "
                      << xml.GetXMLFile().Data() << std::endl;
            exit(1);
        }
        TAxis* pAxis = new TAxis(pBinsEdges_size-1, &pBinsEdges[0]);
        TAxis* ctAxis = new TAxis(ctBinsEdges_size-1, &ctBinsEdges[0]);
        axes[0] = pAxis;
        axes[1] = ctAxis;
        //Create the samples. Supply the:
        //name
        //sampleID
        //Number of observables.
        //Array of observables.
        //Axes for the histograms.
        //Whether to throw MC statistical errors.
        //Whether to throw statistical errors
        BANFFBinnedSample binnedSample(sample_name, sampleId, nObs,
                                       observables, axes, throwMCStat,
                                       throwStat);
        banffsamplesVect.push_back(binnedSample);
    } //end loop over samples

    Int_t nSamples = banffsamplesVect.size();
    Declare(Form("There are %d samples loaded!", nSamples));
    std::vector<BANFFSampleBase*> samples(nSamples);
    std::vector<BANFFBinnedSample>::iterator binnedSampl_iter;
    //fill a vector of pointers to the samples
    for(binnedSampl_iter  = banffsamplesVect.begin();
        binnedSampl_iter != banffsamplesVect.end();
        ++binnedSampl_iter
       )
    {
        const UInt_t index = binnedSampl_iter - banffsamplesVect.begin();
        samples.at(index) = &(*binnedSampl_iter);
    }


    //With the samples ready, now define the parameters that are going to be
    //considered.
    //TODO: Again, maybe some of this can be moved to a backend?
    FitParameters* fitParameters = new FitParameters();

    //Load the Flux Parameters.
    //Specify not to vary them.
    if(loadFluxParams)
    {
        fitParameters->LoadFluxParametersFromFile(fluxInputFile,
                                                  loadNuModeFluxParams,
                                                  loadAntiNuModeFluxParams,
                                                  throwFluxParams
                                                 );
    }

    //Load the XSec Parameters and specify not to vary them.
    if(loadXSecParams)
    {
        fitParameters->LoadXSecParametersFromFile(xSecInputFile,
                                                  throwXSecParams
                                                 );
    }

    //The ability to load observable normalization parameters if so desired,
    //likely for validation purposes.
    if(loadObsNormParams)
    {
        fitParameters->LoadObsNormParametersFromFile(obsNormInputFile,
                                                     nSamples,
                                                     &samples[0],
                                                     throwObsNormParams
                                                    );
    }

    //We can save pre-fit information to a file here, since everything that has
    //been loaded will be.
    //Just make it iteration 0, save the names, and don't access the
    //undecomposed variable.

    if(savePrefitAndExit)
    {
        TFile* preFitFile = new TFile(("prefit_"+outputName).c_str(),"RECREATE");
        preFitFile->cd();
        fitParameters->WriteResults(0, true, false);
        preFitFile->Close();
        exit(0);
    }

    //Now that all parameters are added, flag the flux and FSI parameters for
    //decomposition.
    for(int i = 0; i < fitParameters->GetNumbSysts(); i++)
    {

        if(fitParameters->GetParamType(i)==FLUX)
            fitParameters->SetDecompose(i,1);

        if(fitParameters->GetParamName(i).find("FSI")!=std::string::npos)
        {
            std::cout << "Decompose " << fitParameters->GetParamName(i) << std::endl;
            fitParameters->SetDecompose(i,1);
        }
    }

    //With the samples, observables, and fit parameters specified, create the
    //interface, which will load in the ND280 events and store everything.
    //Usage:
    //Number of samples.
    //Array of sample pointers.
    //FitParameters pointer.
    //Whether to load the detector parameters from psyche.
    //Whether to vary the psyche detector parameters.
    //BANFFInterfaceBase* interface = 0;
    //interface = new BANFFInterfaceBase(nSamples, &samples[0], fitParameters, "diagnostic_ignore_me.root");

    //Create the psyche interface.  Do not load in or vary the detector
    //parameters from psyche.
    //NB: Although there are program parameters for this, it is something that
    //would never be done.
    psycheInterface* interface = 0;
    interface = new psycheInterface(nSamples, &samples[0], fitParameters,
                                    loadDetParams, throwDetParams, "diagnostic_ignore_me.root");

    //Create the fitter that we want, and pass it the interface and the name of
    //the output file.
    Minuit2LikelihoodFit* fitter = new Minuit2LikelihoodFit(interface, outputName);

    //If requested in the program parameters file, fit to the data loaded in by
    //psyche.
    if(runDataFit)
    {

        fitter->outputFile->cd();
        interface->BuildData();
        interface->BuildNominalMC();

        for(int si = 0; si < interface->nSamples; si++)
        {
            std::string dataName = interface->samples[si]->name + "_data";
            std::string mcName   = interface->samples[si]->name + "_prefit";
            samples[si]->SaveData(dataName);
            samples[si]->SaveNominalMC(mcName);
        }

        if(ND::params().GetParameterI("BANFF.SavePrefitAndExit"))
            exit(0);

        fitter->InitializeParameters();
        fitter->DoFit();
    }

    //Otherwise, if the nominal MC flag is set, build the nominal MC then use
    //it as the data to fit.
    else if(runNominalMCFit)
    {

        fitter->outputFile->cd();
        interface->BuildNominalMC();

        for(int si = 0; si < interface->nSamples; si++)
        {
            samples[si]->UseNominalMCAsData();
            std::string dataName = interface->samples[si]->name + "_data";
            std::string mcName   = interface->samples[si]->name + "_prefit";
            samples[si]->SaveData(dataName);
            samples[si]->SaveNominalMC(mcName);
        }

        if(ND::params().GetParameterI("BANFF.SavePrefitAndExit"))
            exit(0);

        fitter->InitializeParameters();
        fitter->DoFit();
    }

    //Otherwise, fit to a number of toy MC histgorams in a file specified in
    //the program parameters file.
    //Taking advantage of this allows performing multiple validation fits
    //without having to spend time reloading the MC.
    else
    {
        //Set up the file containing the data.
        TFile* toyFile = new TFile((ND::params().GetParameterS("BANFF.RunFit.ToyFile")).c_str());

        //Loop over the toys.
        for(int i = ND::params().GetParameterI("BANFF.RunFit.FirstToyToFit");
                i < ND::params().GetParameterI("BANFF.RunFit.FirstToyToFit") + ND::params().GetParameterI("BANFF.RunFit.NToysToFit");
                i++)
        {

            fitter->outputFile->cd();
            interface->BuildNominalMC();

            //Loop over the samples.
            for(int si = 0; si < interface->nSamples; si++)
            {
                std::stringstream histoStream;
                histoStream << samples[si]->name << "_predMC_" << i;

                //Load the predicted MC histogram in for this sample's data
                //histogram.
                samples[si]->LoadDataFromFile(toyFile,histoStream.str());

                std::string dataName = interface->samples[si]->name + "_data";
                std::string mcName = interface->samples[si]->name + "_prefit";
                samples[si]->SaveData(dataName);
                samples[si]->SaveNominalMC(mcName);
            }
            if(ND::params().GetParameterI("BANFF.SavePrefitAndExit"))
                exit(0);
            fitter->InitializeParameters();

            //DoFit has an internal counter that will automatically iterate the
            //names of the fit output to fit multiple toys and save the output
            //to the same file..
            fitter->DoFit();
        }

        toyFile->Close();
    }

    //Create the output file that the plots will be saved into
    TFile* outFile = new TFile(outputFileName.c_str(),"RECREATE");
    outFile->cd();

    Declare("BuildSaveAndResetMCPrediction(\"all\")");

    //Building and save the MC prediction with everything enabled.
    interface->BuildSaveAndResetMCPrediction("all");

    outFile->Close();
    return 0;
    */

    //Garbage collection causes crashes!?
    //delete interface;
    //delete fitParameters;
    //delete observables;
    //delete pmu;
    //delete thmu;

}
