#define RUNP0DFITONLY_WITHRHC_CXX

#include "psycheInterface/psycheInterface.hxx"
#include "BANFFSample/BANFFBinnedSample.hxx"
#include "psycheInterface/psycheObservables.hxx"
#include "BANFFObservableBase.hxx"
#include "FitParameters.hxx"
#include "Parameters.hxx"
#include "BANFFSample/BANFFBinnedSampleUtils.hxx"
#include "Minuit2LikelihoodFit/Minuit2LikelihoodFit.cxx"
//#include "XMLTools.hxx"
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
    TAxis* pAxis_p0d_water_inc  = new TAxis(npbins_p0d_water_inc, pbins_p0d_water_inc);
    TAxis* thAxis_p0d_water_inc = new TAxis(nctbins_p0d_water_inc, ctbins_p0d_water_inc);
    TAxis** p0d_water_numuccinc_axes = new TAxis*[2];
    p0d_water_numuccinc_axes[0] = pAxis_p0d_water_inc;
    p0d_water_numuccinc_axes[1] = thAxis_p0d_water_inc;

    Int_t npbins_p0d_air_inc = 6;
    Double_t pbins_p0d_air_inc[7] = {0.,450.,700.,1100.,1600.,2600.,5000.};
    Int_t nctbins_p0d_air_inc = 6;
    Double_t ctbins_p0d_air_inc[7] = {0.0,+0.6,+0.8,+0.90,+0.95,+0.975,+1.0};
    TAxis* pAxis_p0d_air_inc  = new TAxis(npbins_p0d_air_inc, pbins_p0d_air_inc);
    TAxis* thAxis_p0d_air_inc = new TAxis(nctbins_p0d_air_inc, ctbins_p0d_air_inc);
    TAxis** p0d_air_numuccinc_axes = new TAxis*[2];
    p0d_air_numuccinc_axes[0] = pAxis_p0d_air_inc;
    p0d_air_numuccinc_axes[1] = thAxis_p0d_air_inc;

    Int_t npbins_p0d_water_numubar_inc = 6;
    Double_t pbins_p0d_water_numubar_inc[7] = {0.,450.,700.,1100.,1600.,2600.,5000.};
    Int_t nctbins_p0d_water_numubar_inc = 6;
    Double_t ctbins_p0d_water_numubar_inc[7] = {0.0,+0.6,+0.8,+0.90,+0.95,+0.975,+1.0};
    TAxis* pAxis_p0d_water_numubar_inc  = new TAxis(npbins_p0d_water_numubar_inc, pbins_p0d_water_numubar_inc);
    TAxis* thAxis_p0d_water_numubar_inc = new TAxis(nctbins_p0d_water_numubar_inc, ctbins_p0d_water_numubar_inc);
    TAxis** p0d_water_numubarRHCccinc_axes = new TAxis*[2];
    p0d_water_numubarRHCccinc_axes[0] = pAxis_p0d_water_numubar_inc;
    p0d_water_numubarRHCccinc_axes[1] = thAxis_p0d_water_numubar_inc;

    Int_t npbins_p0d_air_numubar_inc = 6;
    Double_t pbins_p0d_air_numubar_inc[7] = {0.,450.,700.,1100.,1600.,2600.,5000.};
    Int_t nctbins_p0d_air_numubar_inc = 6;
    Double_t ctbins_p0d_air_numubar_inc[7] = {0.0,+0.6,+0.8,+0.90,+0.95,+0.975,+1.0};
    TAxis* pAxis_p0d_air_numubar_inc  = new TAxis(npbins_p0d_air_numubar_inc, pbins_p0d_air_numubar_inc);
    TAxis* thAxis_p0d_air_numubar_inc = new TAxis(nctbins_p0d_air_numubar_inc, ctbins_p0d_air_numubar_inc);
    TAxis** p0d_air_numubarRHCccinc_axes = new TAxis*[2];
    p0d_air_numubarRHCccinc_axes[0] = pAxis_p0d_air_numubar_inc;
    p0d_air_numubarRHCccinc_axes[1] = thAxis_p0d_air_numubar_inc;

    Int_t npbins_p0d_water_numuRHC_inc = 6;
    Double_t pbins_p0d_water_numuRHC_inc[7] = {0.,450.,700.,1100.,1600.,2600.,5000.};
    Int_t nctbins_p0d_water_numuRHC_inc = 6;
    Double_t ctbins_p0d_water_numuRHC_inc[7] = {0.0,+0.6,+0.8,+0.90,+0.95,+0.975,+1.0};
    TAxis* pAxis_p0d_water_numuRHC_inc  = new TAxis(npbins_p0d_water_numuRHC_inc, pbins_p0d_water_numuRHC_inc);
    TAxis* thAxis_p0d_water_numuRHC_inc = new TAxis(nctbins_p0d_water_numuRHC_inc, ctbins_p0d_water_numuRHC_inc);
    TAxis** p0d_water_numuRHCccinc_axes = new TAxis*[2];
    p0d_water_numuRHCccinc_axes[0] = pAxis_p0d_water_numuRHC_inc;
    p0d_water_numuRHCccinc_axes[1] = thAxis_p0d_water_numuRHC_inc;

    Int_t npbins_p0d_air_numuRHC_inc = 6;
    Double_t pbins_p0d_air_numuRHC_inc[7] = {0.,450.,700.,1100.,1600.,2600.,5000.};
    Int_t nctbins_p0d_air_numuRHC_inc = 6;
    Double_t ctbins_p0d_air_numuRHC_inc[7] = {0.0,+0.6,+0.8,+0.90,+0.95,+0.975,+1.0};
    TAxis* pAxis_p0d_air_numuRHC_inc  = new TAxis(npbins_p0d_air_numuRHC_inc, pbins_p0d_air_numuRHC_inc);
    TAxis* thAxis_p0d_air_numuRHC_inc = new TAxis(nctbins_p0d_air_numuRHC_inc, ctbins_p0d_air_numuRHC_inc);
    TAxis** p0d_air_numuRHCccinc_axes = new TAxis*[2];
    p0d_air_numuRHCccinc_axes[0] = pAxis_p0d_air_numuRHC_inc;
    p0d_air_numuRHCccinc_axes[1] = thAxis_p0d_air_numuRHC_inc;

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

    // FHC Numu selections
    BANFFBinnedSample* fhc_p0d_water_numuccinc = new BANFFBinnedSample("fhc_p0d_water_numuccinc", SampleId::kP0DWaterNuMuCC, 2, observables, p0d_water_numuccinc_axes, throwMCStat, throwStat);

    BANFFBinnedSample* rhc_p0d_water_numubarccinc = new BANFFBinnedSample("rhc_p0d_water_numubarccinc", SampleId::kP0DWaterNuMuBarInAntiNuModeCC, 2, observables, p0d_water_numubarRHCccinc_axes, throwMCStat, throwStat);

    BANFFBinnedSample* rhc_p0d_water_numubkgccinc = new BANFFBinnedSample("rhc_p0d_water_numubkgccinc", SampleId::kP0DWaterNuMuBkgInAntiNuModeCC, 2, observables, p0d_water_numuRHCccinc_axes, throwMCStat, throwStat);

    BANFFBinnedSample* fhc_p0d_air_numuccinc = new BANFFBinnedSample("fhc_p0d_air_numuccinc", SampleId::kP0DAirNuMuCC, 2, observables, p0d_air_numuccinc_axes, throwMCStat, throwStat);

    BANFFBinnedSample* rhc_p0d_air_numubarccinc = new BANFFBinnedSample("rhc_p0d_air_numubarccinc", SampleId::kP0DAirNuMuBarInAntiNuModeCC, 2, observables, p0d_air_numubarRHCccinc_axes, throwMCStat, throwStat);

    BANFFBinnedSample* rhc_p0d_air_numubkgccinc = new BANFFBinnedSample("rhc_p0d_air_numubkgccinc", SampleId::kP0DAirNuMuBkgInAntiNuModeCC, 2, observables, p0d_air_numuRHCccinc_axes, throwMCStat, throwStat);

    int nSamples = 0;
    BANFFSampleBase** samples = new BANFFSampleBase*[nSamples];
    samples[nSamples++] = fhc_p0d_water_numuccinc;
    samples[nSamples++] = rhc_p0d_water_numubarccinc;
    samples[nSamples++] = rhc_p0d_water_numubkgccinc;
    samples[nSamples++] = fhc_p0d_air_numuccinc;
    samples[nSamples++] = rhc_p0d_air_numubarccinc;
    samples[nSamples++] = rhc_p0d_air_numubkgccinc;
;
    //With the samples ready, now define the parameters that are going to be
    //considered.
    //TODO: Again, maybe some of this can be moved to a backend?
    FitParameters* fitParameters = new FitParameters();

    //Load the Flux Parameters.
    //Specify to fit them.
    if(ND::params().GetParameterI("BANFF.RunFit.LoadFluxParams")){
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

    //Garbage collection causes crashes!?
    //delete interface;
    //delete fitParameters;
    //delete observables;
    //delete pmu;
    //delete thmu;

}
