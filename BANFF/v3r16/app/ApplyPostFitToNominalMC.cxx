/*
 * This app is designed to take the post fit result on the flattree+T2KReWeight splines
 * See Usage function below for how to use the app
*/

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

inline void Declare(const char* str)
{
    std::cout << str << std::endl;
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
        inputFileName = Form("%s/inputs/Binning.xml", getenv("BANFFROOT"));
        std::cout << "WARNING: Using "
                  << inputFileName.c_str()
                  << " as input file since none was provided!"
                  << std::endl;
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
    bool throwMCStat              = ND::params().GetParameterI("BANFF.RunFit.ThrowMCStat");
    bool throwStat                = ND::params().GetParameterI("BANFF.RunFit.ThrowStat");
    bool loadXSecParams           = ND::params().GetParameterI("BANFF.RunFit.LoadXSecParams");
    bool loadObsNormParams        = ND::params().GetParameterI("BANFF.RunFit.LoadObsNormParams");
    bool loadFluxParams           = ND::params().GetParameterI("BANFF.ThrowToys.LoadFluxParams");
    bool throwXSecParams          = ND::params().GetParameterI("BANFF.ThrowToys.ThrowXSecParams");
    bool loadNuModeFluxParams     = ND::params().GetParameterI("BANFF.ThrowToys.LoadNuModeFluxParams");
    bool loadAntiNuModeFluxParams = ND::params().GetParameterI("BANFF.ThrowToys.LoadAntiNuModeFluxParams");
    bool throwFluxParams          = ND::params().GetParameterI("BANFF.ThrowToys.ThrowFluxParams");
    bool throwObsNormParams       = ND::params().GetParameterI("BANFF.ThrowToys.ThrowObsNormParams");
    bool loadDetParams            = ND::params().GetParameterI("BANFF.ThrowToys.LoadDetParams");
    bool throwDetParams           = ND::params().GetParameterI("BANFF.ThrowToys.ThrowDetParams");
    std::string fluxInputFile     = ND::params().GetParameterS("BANFF.FluxInputFile");
    std::string xSecInputFile     = ND::params().GetParameterS("BANFF.XSecInputFile");
    std::string obsNormInputFile  = ND::params().GetParameterS("BANFF.ObsNormInputFile");

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
    psycheInterface* interface = 0;
    interface = new psycheInterface(nSamples, &samples[0], fitParameters,
                                    loadDetParams, throwDetParams, "diagnostic_ignore_me.root");


    //Create the output file that the plots will be saved into
    TFile* outFile = new TFile(outputFileName.c_str(),"RECREATE");
    outFile->cd();

    std::cout << "BuildSaveAndResetMCPrediction(\"all\")" << std::endl;
    //Building and save the MC prediction with everything enabled.
    interface->BuildSaveAndResetMCPrediction("all");

    outFile->Close();
    return 0;

    //Garbage collection causes crashes!?
    //delete interface;
    //delete fitParameters;
    //delete observables;
    //delete pmu;
    //delete thmu;

}
