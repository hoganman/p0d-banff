/*
 * This app is designed to take the post fit result on the flattree+T2KReWeight splines
 *
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

void Usage(const char* programName)
{
    using namespace std;
    cout << programName << " [INPUTS]" << endl;
    cout << endl;
    cout << "INPUTS" << endl;
    cout << "    -i path/to/Samples.xml: The path to the XML file containing sample information" << endl;
    cout << "    -h this message" << endl;
    cout << "    -o outputFile.root: The name of the output ROOT file" << endl;
    exit(0);
}

//Set up the samples and observables with proper binning, then load the
//parameters and perform the fit.
int main(int argc, char** argv){

    std::string outputFileName = "";
    std::string inputFileName = "";
    if(argc > 4){
        std::cout << "Too many command line arguments!" << std::endl;
	Usage(argv[0]);
    }

    for (;;)
    {
        Int_t c = getopt(argc, argv, "i:o:h");
        if (c < 0)
            break;
        switch (c)
        {
            case 'i':
            {
                outputFileName = optarg;
                break;
            }
            case 'h':
            {
		Usage(argv[0]);
            }
            case 'o':
            {
                inputFileName = optarg;
                break;
            }
            default:
            {
		Usage(argv[0]);
            }
        }
    }//end for

    if(outputFileName.length() < 1)
    {
	Usage(argv[0]);
    }

    if(inputFileName.length() < 1)
    {
	char* buffer[1000];
	inputFileName = sprintf(buffer,"%s/inputs/Binning.xml", getenv("BANFFROOT"));
	std::cout << "WARNING: Using " << buffer << as input file since none was provided!" << endl;
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
    const Bool_t throwMCStat  = static_cast<Bool_t>(ND::params().GetParameterI("BANFF.RunFit.ThrowMCStat"));
    const Bool_t throwStat    = static_cast<Bool_t>(ND::params().GetParameterI("BANFF.RunFit.ThrowStat"));

    //Create the observables we will use.
    psycheLeptonCandidateMomentum* pmu  = new psycheLeptonCandidateMomentum();
    psycheLeptonCandidateCosTheta* thmu = new psycheLeptonCandidateCosTheta();

    BANFFObservableBase** observables = new BANFFObservableBase*[2];
    observables[0] = pmu;
    observables[1] = thmu;
    Int_t nObs = 2;

    //For each observable and sample set up the binning used in the fit.

    //Set the binning
    XMLTools xml(inputFileName.c_str());
    std::map<TString, XMLTools::AttributeMap> samples_attributes = xml.GetAllNodes();
    std::vector<BANFFBinnedSample> banffsamplesVect;
    for(std::map<TString, XMLTools::AttributeMap>::iterator map_iter = samples_attributes.begin();
        map_iter != samples_attributes.end();
        ++map_iter
       )
    {
        const char* sample_name = map_iter->first.Data();
        XMLTools::AttributeMap attributes = map_iter->second;
        SampleId::SampleEnum sampleId = SampleId::ConvertSample(attributes["SampleId"].Data());
        TAxis* axes[2];
        std::vector<TString> pBinsEdgesStr = XMLTools::SplitString(attributes["pBinsEdges"]);
        std::vector<TString> ctBinsEdgesStr = XMLTools::SplitString(attributes["ctBinsEdges"]);
        std::vector<Double_t> pBinsEdges(pBinsEdgesStr.size());
        std::vector<Double_t> ctBinsEdges(ctBinsEdgesStr.size());
        for(UInt_t index = 0; index < pBinsEdges.size(); ++index)
        {
            pBinsEdges.at(index) = pBinsEdgesStr.at(index).Atof();
        }
        if(attributes["pBinsEdgesN"].Atoi() != pBinsEdges.size())
        {
            std::cout << "ERROR: the number of stated pBins for "
                      << sample_name
                      << "does not match in the file: "
                      << xml.GetXMLFile().Data() << std::endl;
	    exit(1);
        }
        for(UInt_t index = 0; index < ctBinsEdges.size(); ++index)
        {
            ctBinsEdges.at(index) = ctBinsEdgesStr.at(index).Atof();
        }
        if(attributes["ctBinsEdgesN"].Atoi() != ctBinsEdges.size())
        {
            std::cout << "ERROR: the number of stated ctBins for "
                      << sample_name
                      << "does not match in the file: "
                      << xml.GetXMLFile().Data() << std::endl;
	    exit(1);
        }
        TAxis* pAxis = new TAxis(pBinsEdges.size()-1, &pBinsEdges[0]);
        TAxis* ctAxis = new TAxis(ctBinsEdges.size()-1, &ctBinsEdges[0]);
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
        BANFFBinnedSample binnedSample(sample_name, sampleId, nObs, observables, axes, throwMCStat, throwStat);
        banffsamplesVect.push_back(binnedSample);
    }

    Int_t nSamples = banffsamplesVect.size();
    std::vector<BANFFSampleBase*> samples(nSamples);
    for(std::vector<BANFFBinnedSample>::iterator binnedSampl_iter = banffsamplesVect.begin();
        binnedSampl_iter != banffsamplesVect.end();
        ++binnedSampl_iter
       )
    {
        const UInt_t index = banffsamplesVect.end() - binnedSampl_iter;
        samples.at(index) = &(*binnedSampl_iter);
    }


    //With the samples ready, now define the parameters that are going to be
    //considered.
    //TODO: Again, maybe some of this can be moved to a backend?
    FitParameters* fitParameters = new FitParameters();

    //Load the Flux Parameters.
    //Specify not to vary them.
    if(ND::params().GetParameterI("BANFF.ThrowToys.LoadFluxParams")){
        fitParameters->LoadFluxParametersFromFile(ND::params().GetParameterS("BANFF.FluxInputFile"),
                ND::params().GetParameterI("BANFF.ThrowToys.LoadNuModeFluxParams"),
                ND::params().GetParameterI("BANFF.ThrowToys.LoadAntiNuModeFluxParams"),
                ND::params().GetParameterI("BANFF.ThrowToys.ThrowFluxParams"));
    }

    //Load the XSec Parameters and specify not to vary them.
    if(ND::params().GetParameterI("BANFF.ThrowToys.LoadXSecParams")){
        fitParameters->LoadXSecParametersFromFile(((ND::params().GetParameterS("BANFF.XSecInputFile"))), ND::params().GetParameterI("BANFF.ThrowToys.ThrowXSecParams"));
    }

    //The ability to load observable normalization parameters if so desired,
    //likely for validation purposes.
    if(ND::params().GetParameterI("BANFF.ThrowToys.LoadObsNormParams")){
        fitParameters->LoadObsNormParametersFromFile(((ND::params().GetParameterS("BANFF.ObsNormInputFile"))), nSamples, samples, ND::params().GetParameterI("BANFF.ThrowToys.ThrowObsNormParams"));
    }

    //With the samples, observables, and fit parameters specified, create the
    //interface, which will load in the ND280 events and store everything.
    //Usage:
    //Number of samples.
    //Array of sample pointers.
    //FitParameters pointer.
    //Whether to load the detector parameters from psyche.
    //Whether to vary the psyche detector parameters.
    psycheInterface* interface = new psycheInterface(nSamples, samples, fitParameters, ND::params().GetParameterI("BANFF.ThrowToys.LoadDetParams"), ND::params().GetParameterI("BANFF.ThrowToys.ThrowDetParams"));


    //Create the output file that the plots will be saved into
    TFile* outFile = new TFile(outputFileName.c_str(),"RECREATE");
    outFile->cd();

    //Building and save the MC prediction with everything enabled.
    interface->BuildSaveAndResetMCPrediction("all");

    //Garbage collection
    outFile->Close();

    delete interface;
    delete fitParameters;
    delete observables;
    delete pmu;
    delete thmu;

}
