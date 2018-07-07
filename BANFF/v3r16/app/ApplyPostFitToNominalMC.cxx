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

//Set up the samples and observables with proper binning, then load the
//parameters and perform the fit.
int main(int argc, char** argv){

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
    const Bool_t throwMCStat  = static_cast<Bool_t>(ND::params().GetParameterI("BANFF.RunFit.ThrowMCStat"));
    const Bool_t throwStat    = static_cast<Bool_t>(ND::params().GetParameterI("BANFF.RunFit.ThrowStat"));
    //const Bool_t DoMultiPiRHC = static_cast<Bool_t>(ND::params().GetParameterI("BANFF.DoMultiPiRHC"));
    //const Bool_t Do4PiFHC     = static_cast<Bool_t>(ND::params().GetParameterI("BANFF.Do4PiFHC"));
    //const Bool_t DoNue        = static_cast<Bool_t>(ND::params().GetParameterI("BANFF.DoNueSelections"));
    //const Bool_t DoOnlyNue    = static_cast<Bool_t>(ND::params().GetParameterI("BANFF.DoOnlyNueSelections"));


    //Create the observables we will use.
    psycheLeptonCandidateMomentum* pmu  = new psycheLeptonCandidateMomentum();
    psycheLeptonCandidateCosTheta* thmu = new psycheLeptonCandidateCosTheta();

    BANFFObservableBase** observables = new BANFFObservableBase*[2];
    observables[0] = pmu;
    observables[1] = thmu;
    Int_t nObs = 2;

    //For each observable and sample set up the binning used in the fit.

    //Set the binning for the CC0pi sample.
    XMLTools xml("/physics/home/mhogan/software/p0d-banff/p0d-banff/BANFF/v3r16/inputs/Binning.xml");
    std::map<TString, XMLTools::AttributeMap> samples_attributes = xml.GetAllNodes();
    std::vector<BANFFBinnedSample> banffsamplesVect;
    for(std::map<TString, XMLTools::AttributeMap>::iterator map_iter = samples_attributes.begin();
        map_iter != samples_attributes.end();
        ++map_iter
       )
    {
        const char* sample_name = map_iter->first.Data();
        SampleId::SampleEnum sampleId = SampleId::ConvertSample(sample_name);
        XMLTools::AttributeMap attributes = map_iter->second;
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
        }
        TAxis* pAxis = new TAxis(pBinsEdges.size()-1, &pBinsEdges[0]);
        TAxis* ctAxis = new TAxis(ctBinsEdges.size()-1, &ctBinsEdges[0]);
        axes[0] = pAxis;
        axes[1] = ctAxis;
        BANFFBinnedSample binnedSample(sample_name, sampleId, nObs, observables, axes, throwMCStat, throwStat);
        banffsamplesVect.push_back(binnedSample);
    }
    //Create the samples.
    //Supply the:
    //name
    //sampleID
    //Number of observables.
    //Array of observables.
    //Axes for the histograms.
    //Whether to throw MC statistical errors.
    //Whether to throw statistical errors

    const UInt_t nSamples = banffsamplesVect.size();
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
    //Specify to fit them.
    if(ND::params().GetParameterI("BANFF.RunFit.LoadFluxParams"))
    {
        fitParameters->LoadFluxParametersFromFile(
                ND::params().GetParameterS("BANFF.FluxInputFile"),
                ND::params().GetParameterI("BANFF.RunFit.LoadNuModeFluxParams"),
                ND::params().GetParameterI("BANFF.RunFit.LoadAntiNuModeFluxParams"),
                ND::params().GetParameterI("BANFF.RunFit.FitFluxParams"));
    }

    //Load the XSec Parameters and specify to fit them
    if(ND::params().GetParameterI("BANFF.RunFit.LoadXSecParams"))
    {
        fitParameters->LoadXSecParametersFromFile(
                ND::params().GetParameterS("BANFF.XSecInputFile"),
                ND::params().GetParameterI("BANFF.RunFit.FitXSecParams"));
    }

    //Load the ObsNorm parameters, and specify to fit them.
    if(ND::params().GetParameterI("BANFF.RunFit.LoadObsNormParams"))
    {
        fitParameters->LoadObsNormParametersFromFile(
                ND::params().GetParameterS("BANFF.ObsNormInputFile"),
                static_cast<int>(nSamples), &samples[0],
                ND::params().GetParameterI("BANFF.RunFit.FitObsNormParams"));
    }

    //We can save pre-fit information to a file here, since everything that has
    //been loaded will be.
    //Just make it iteration 0, save the names, and don't access the
    //undecomposed variable.

    std::string outputName = ND::params().GetParameterS("BANFF.RunFit.OutputFile");
    std::string PsycheVersion = anaUtils::GetSoftwareVersionFromPath((std::string)getenv("ND280PSYCHEROOT"));
    outputName = PsycheVersion+"_"+outputName;

    if(ND::params().GetParameterI("BANFF.SavePrefitParamsAndExit"))
    {
        TFile* preFitFile = new TFile(("prefit_"+outputName).c_str(),"RECREATE");
        preFitFile->cd();
        fitParameters->WriteResults(0, true, false);
        preFitFile->Close();
        exit(0);
    }

}
