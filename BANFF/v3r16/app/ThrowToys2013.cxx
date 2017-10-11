#include "psycheInterface/psycheInterface.hxx"
#include "BANFFSample/BANFFBinnedSample.hxx"
#include "psycheInterface/psycheObservables.hxx"
#include "BANFFObservableBase.hxx"
#include "FitParameters.hxx"
#include "Parameters.hxx"
#include "BANFFSample/BANFFBinnedSampleUtils.hxx"
#include <sys/time.h>

//Set up the three samples with the proper binning, then pass to throw toys.
//Allow specification of a parameter override file for easy performance of
//multiple tests.
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


    //Create the observables that will use.  Store them in an array to pass to
    //the sample.
    //TODO: Make this accessible via a card file instead of manually coding it
    //in.
    psycheLeptonCandidateMomentum* pmu = new psycheLeptonCandidateMomentum();
    psycheLeptonCandidateCosTheta* thmu = new psycheLeptonCandidateCosTheta();

    BANFFObservableBase** observables = new BANFFObservableBase*[2];
    observables[0] = pmu;
    observables[1] = thmu;


    //The binning used for throwing toys.
    
    //TODO: Maybe move some of this onto the backend?  Assemble TAxis pointers
    //inside the object?
    //Alternatively, a scrip to easily assemble files with TAxis definitions in
    //them, and the card file has a path to the TAxis files?
    //Set the binning for the CC0pi sample.
    Int_t npbins_0pi = 10;
    Double_t pbins_0pi[11] = {0.,300.,500.,600.,700.,900.,1000.,1500.,3000.,5000.,30000.}; // out to 30 and combine overflow
    Int_t nctbins_0pi = 7;
    Double_t ctbins_0pi[8] = {-1.0,0.6,0.8,0.85,0.90,0.94,0.98,1.0};

    TAxis* pAxis_0pi = new TAxis(npbins_0pi,pbins_0pi);
    TAxis* thAxis_0pi = new TAxis(nctbins_0pi, ctbins_0pi);
    TAxis** cc0pi_axes = new TAxis*[2];
    cc0pi_axes[0] = pAxis_0pi;
    cc0pi_axes[1] = thAxis_0pi;


    //Set the binning for the CC1pi sample.
    Int_t npbins_1pi = 10;
    Double_t pbins_1pi[11] = {0.,300.,500.,600.,700.,900.,1000.,1500.,3000.,5000.,30000.}; // out to 30 and combine overflow
    Int_t nctbins_1pi = 7;
    Double_t ctbins_1pi[8] = {-1.0,0.6,0.8,0.85,0.90,0.94,0.98,1.0};

    TAxis* pAxis_1pi = new TAxis(npbins_1pi,pbins_1pi);
    TAxis* thAxis_1pi = new TAxis(nctbins_1pi, ctbins_1pi);
    TAxis** cc1pi_axes = new TAxis*[2];
    cc1pi_axes[0] = pAxis_1pi;
    cc1pi_axes[1] = thAxis_1pi;


    //Set the binning for the CC-Other sample.
    Int_t npbins_npi = 10;
    Double_t pbins_npi[11] = {0.,300.,500.,600.,700.,900.,1000.,1500.,3000.,5000.,30000.}; // out to 30 and combine overflow
    Int_t nctbins_npi = 7;
    Double_t ctbins_npi[8] = {-1.0,0.6,0.8,0.85,0.90,0.94,0.98,1.0};

    TAxis* pAxis_npi = new TAxis(npbins_npi,pbins_npi);
    TAxis* thAxis_npi = new TAxis(nctbins_npi, ctbins_npi);
    TAxis** ccnpi_axes = new TAxis*[2];
    ccnpi_axes[0] = pAxis_npi;
    ccnpi_axes[1] = thAxis_npi;


    //Create the samples.
    //Supply the:
    //name
    //sampleID
    //Number of observables.
    //Array of observables.
    //Axes for the histograms.
    //Whether to throw MC statistical errors.
    //Whether to throw statistical errors.
    bool throwMCStat = ND::params().GetParameterI("BANFF.ThrowToys.ThrowMCStat");
    bool throwStat = ND::params().GetParameterI("BANFF.ThrowToys.ThrowStat");
    BANFFBinnedSample* cc0pi = new BANFFBinnedSample("cc0pi", SampleId::kFGD1NuMuCC0Pi, 2, observables, cc0pi_axes, throwMCStat, throwStat);
    BANFFBinnedSample* cc1pi = new BANFFBinnedSample("cc1pi", SampleId::kFGD1NuMuCC1Pi, 2, observables, cc1pi_axes, throwMCStat, throwStat);
    BANFFBinnedSample* ccOth = new BANFFBinnedSample("ccOth", SampleId::kFGD1NuMuCCOther, 2, observables, ccnpi_axes, throwMCStat, throwStat);

    BANFFSampleBase** samples = new BANFFSampleBase*[3];
    samples[0] = cc0pi;
    samples[1] = cc1pi;
    samples[2] = ccOth;

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
        fitParameters->LoadObsNormParametersFromFile(((ND::params().GetParameterS("BANFF.ObsNormInputFile"))), 3, samples, ND::params().GetParameterI("BANFF.ThrowToys.ThrowObsNormParams"));
    }

    //With the samples, observables, and fit parameters specified, create the
    //interface, which will load in the ND280 events and store everything.
    //Usage:
    //Number of samples.
    //Array of sample pointers.
    //FitParameters pointer.
    //Whether to load the detector parameters from psyche.
    //Whether to vary the psyche detector parameters.
    psycheInterface* interface = new psycheInterface(3, samples, fitParameters, ND::params().GetParameterI("BANFF.ThrowToys.LoadDetParams"), ND::params().GetParameterI("BANFF.ThrowToys.ThrowDetParams"));


    //If requested in the parameters file, substitute in a fit result for the
    //parameter prior value.
    if(ND::params().GetParameterI("BANFF.ThrowToys.UseFitResult")){

        fitParameters->LoadFittedParameters(ND::params().GetParameterS("BANFF.ThrowToys.FitResultFile"),
                ND::params().GetParameterI("BANFF.ThrowToys.FitNumber"));
    }


    //Toy throwing functionality is built into BANFFInterfaceBase.  Call the
    //function with the number of toys desired, and the name of the output
    //file.
    int nToys = ND::params().GetParameterI("BANFF.ThrowToys.NToys");
    timeval tim;
    gettimeofday(&tim, NULL);
    double t0 = tim.tv_sec + (tim.tv_usec/1000000.0);
    
    interface->ThrowToys(nToys,ND::params().GetParameterS("BANFF.ThrowToys.OutputFile"));
    
    gettimeofday(&tim, NULL);
    double t1 = tim.tv_sec + (tim.tv_usec/1000000.0);
    
    std::cout << "Time to throw toys: " << (t1 - t0) << std::endl;
    std::cout << "Time per toy: " << (t1 - t0)/((double)nToys) << std::endl;

    return 0;
}
