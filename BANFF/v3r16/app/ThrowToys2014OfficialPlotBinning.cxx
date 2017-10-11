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
   
    //If you initialize the TAxis with constant binning, it doesn't save the
    //bin axis, breaking stuff.
    //For p add overflow at the end, and for ct, at it at the beginning.

    //Set the binning for the CC0pi sample.
    Int_t npbins_0pi = 51;
    Double_t pbins_0pi[52];
    for(int i = 0; i < 51; i++){

        pbins_0pi[i] = ((Double_t)i)*100.0;
    }
    pbins_0pi[51] = 30000.0;
    
    Int_t nctbins_0pi = 26;
    Double_t ctbins_0pi[27];

    for(int i = 0; i < 26; i++){

        ctbins_0pi[i+1] = ((Double_t)i)*(1.0/25.0);
    }
    ctbins_0pi[0] = -1.0;


    TAxis* pAxis_0pi = new TAxis(npbins_0pi,pbins_0pi);
    TAxis* thAxis_0pi = new TAxis(nctbins_0pi, ctbins_0pi);
    TAxis** cc0pi_axes = new TAxis*[2];
    cc0pi_axes[0] = pAxis_0pi;
    cc0pi_axes[1] = thAxis_0pi;


    //Set the binning for the CC1pi sample.
    Int_t npbins_1pi = 51;
    Double_t pbins_1pi[52];
    for(int i = 0; i < 51; i++){

        pbins_1pi[i] = ((Double_t)i)*100.0;
    }
    pbins_1pi[51] = 30000.0;
    
    Int_t nctbins_1pi = 26;
    Double_t ctbins_1pi[27];

    for(int i = 0; i < 26; i++){

        ctbins_1pi[i+1] = ((Double_t)i)*(1.0/25.0);
    }
    ctbins_1pi[0] = -1.0;

    TAxis* pAxis_1pi = new TAxis(npbins_1pi,pbins_1pi);
    TAxis* thAxis_1pi = new TAxis(nctbins_1pi, ctbins_1pi);
    TAxis** cc1pi_axes = new TAxis*[2];
    cc1pi_axes[0] = pAxis_1pi;
    cc1pi_axes[1] = thAxis_1pi;


    //Set the binning for the CC-Other sample.
    Int_t npbins_npi = 51;
    Double_t pbins_npi[52];
    for(int i = 0; i < 51; i++){

        pbins_npi[i] = ((Double_t)i)*100.0;
    }
    pbins_npi[51] = 30000.0;

    Int_t nctbins_npi = 26;
    Double_t ctbins_npi[27];

    for(int i = 0; i < 26; i++){

        ctbins_npi[i+1] = ((Double_t)i)*(1.0/25.0);
    }
    ctbins_npi[0] = -1.0;
    
    TAxis* pAxis_npi = new TAxis(npbins_npi,pbins_npi);
    TAxis* thAxis_npi = new TAxis(nctbins_npi, ctbins_npi);
    TAxis** ccnpi_axes = new TAxis*[2];
    ccnpi_axes[0] = pAxis_npi;
    ccnpi_axes[1] = thAxis_npi;

    //Anti-numu CCQE selection in anti-neutrino beam mode.
    //Make this 100 MeV bins from 0-800 MeV, then 200 MeV bins out to 10000.
    //So a total of 8 + 46 = 54 bins
    Int_t npbins_anuccqe = 54;
    Double_t pbins_anuccqe[55];
    for(int i = 0; i < 55; i++){

        if(i < 8){
            pbins_anuccqe[i] = ((Double_t)i)*100.0;
        }
        else{
            pbins_anuccqe[i] = 800 + ((Double_t)(i-8))*200.0;
        }
    }

    Int_t nctbins_anuccqe = 17;
    Double_t ctbins_anuccqe[18];

    for(int i = 0; i < 17; i++){

        ctbins_anuccqe[i+1] = ((Double_t)i)*(1.0/16.0);
    }
    ctbins_anuccqe[0] = -1.0;

    TAxis* pAxis_anuccqe = new TAxis(npbins_anuccqe,pbins_anuccqe);
    TAxis* thAxis_anuccqe = new TAxis(nctbins_anuccqe, ctbins_anuccqe);
    TAxis** anuccqe_axes = new TAxis*[2];
    anuccqe_axes[0] = pAxis_anuccqe;
    anuccqe_axes[1] = thAxis_anuccqe;

    //Anti-numu CCnQE selection in anti-neutrino beam mode.
    Int_t npbins_anuccnqe = 34;
    Double_t pbins_anuccnqe[35];
    for(int i = 0; i < 35; i++){

        pbins_anuccnqe[i] = ((Double_t)i)*300.0;

        if(i == 34){
            
            pbins_anuccnqe[i] = 10000.0;

        }
    }

    Int_t nctbins_anuccnqe = 17;
    Double_t ctbins_anuccnqe[18];

    for(int i = 0; i < 17; i++){

        ctbins_anuccnqe[i+1] = ((Double_t)i)*(1.0/16.0);
    }
    ctbins_anuccnqe[0] = -1.0;

    TAxis* pAxis_anuccnqe = new TAxis(npbins_anuccnqe,pbins_anuccnqe);
    TAxis* thAxis_anuccnqe = new TAxis(nctbins_anuccnqe, ctbins_anuccnqe);
    TAxis** anuccnqe_axes = new TAxis*[2];
    anuccnqe_axes[0] = pAxis_anuccnqe;
    anuccnqe_axes[1] = thAxis_anuccnqe;

    //Numu CCQE selection in anti-neutrino beam mode.
    Int_t npbins_nuccqe = 26;
    Double_t pbins_nuccqe[27];
    for(int i = 0; i < 26; i++){

        pbins_nuccqe[i] = ((Double_t)i)*100.0;
    }
    pbins_nuccqe[26] = 10000.0;

    Int_t nctbins_nuccqe = 26;
    Double_t ctbins_nuccqe[27];

    for(int i = 0; i < 26; i++){

        ctbins_nuccqe[i+1] = ((Double_t)i)*(1.0/25.0);
    }
    ctbins_nuccqe[0] = -1.0;

    TAxis* pAxis_nuccqe = new TAxis(npbins_nuccqe,pbins_nuccqe);
    TAxis* thAxis_nuccqe = new TAxis(nctbins_nuccqe, ctbins_nuccqe);
    TAxis** nuccqe_axes = new TAxis*[2];
    nuccqe_axes[0] = pAxis_nuccqe;
    nuccqe_axes[1] = thAxis_nuccqe;

    //Numu CCnQE selection in anti-neutrino beam mode.
    Int_t npbins_nuccnqe = 26;
    Double_t pbins_nuccnqe[27];
    for(int i = 0; i < 26; i++){

        pbins_nuccnqe[i] = ((Double_t)i)*100.0;
    }
    pbins_nuccnqe[26] = 10000.0;

    Int_t nctbins_nuccnqe = 26;
    Double_t ctbins_nuccnqe[27];

    for(int i = 0; i < 26; i++){

        ctbins_nuccnqe[i+1] = ((Double_t)i)*(1.0/25.0);
    }
    ctbins_nuccnqe[0] = -1.0;

    TAxis* pAxis_nuccnqe = new TAxis(npbins_nuccnqe,pbins_nuccnqe);
    TAxis* thAxis_nuccnqe = new TAxis(nctbins_nuccnqe, ctbins_nuccnqe);
    TAxis** nuccnqe_axes = new TAxis*[2];
    nuccnqe_axes[0] = pAxis_nuccnqe;
    nuccnqe_axes[1] = thAxis_nuccnqe;


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
    bool throwMCStatPoisson = ND::params().GetParameterI("BANFF.ThrowToys.ThrowMCStatPoisson");

    BANFFBinnedSample* cc0pi = new BANFFBinnedSample("cc0pi", SampleId::kFGD1NuMuCC0Pi, 2, observables, cc0pi_axes, throwMCStat, throwStat, throwMCStatPoisson);
    BANFFBinnedSample* cc1pi = new BANFFBinnedSample("cc1pi", SampleId::kFGD1NuMuCC1Pi, 2, observables, cc1pi_axes, throwMCStat, throwStat, throwMCStatPoisson);
    BANFFBinnedSample* ccOth = new BANFFBinnedSample("ccOth", SampleId::kFGD1NuMuCCOther, 2, observables, ccnpi_axes, throwMCStat, throwStat, throwMCStatPoisson);
    BANFFBinnedSample* anuccqe = new BANFFBinnedSample("anuccqe", SampleId::kFGD1AntiNuMuCCQE, 2, observables, anuccqe_axes, throwMCStat, throwStat, throwMCStatPoisson);
    BANFFBinnedSample* anuccnqe = new BANFFBinnedSample("anuccnqe", SampleId::kFGD1AntiNuMuCCnQE, 2, observables, anuccnqe_axes, throwMCStat, throwStat, throwMCStatPoisson);
    BANFFBinnedSample* nuccqe = new BANFFBinnedSample("nuccqe", SampleId::kFGD1NuMuBkgInAntiNuModeCCQE, 2, observables, nuccqe_axes, throwMCStat, throwStat, throwMCStatPoisson);
    BANFFBinnedSample* nuccnqe = new BANFFBinnedSample("nuccnqe", SampleId::kFGD1NuMuBkgInAntiNuModeCCnQE, 2, observables, nuccnqe_axes, throwMCStat, throwStat, throwMCStatPoisson);

    BANFFSampleBase** samples = new BANFFSampleBase*[7];
    samples[0] = cc0pi;
    samples[1] = cc1pi;
    samples[2] = ccOth;
    samples[3] = anuccqe;
    samples[4] = anuccnqe;
    samples[5] = nuccqe;
    samples[6] = nuccnqe;

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
        fitParameters->LoadObsNormParametersFromFile(((ND::params().GetParameterS("BANFF.ObsNormInputFile"))), 7, samples, ND::params().GetParameterI("BANFF.ThrowToys.ThrowObsNormParams"));
    }

    //With the samples, observables, and fit parameters specified, create the
    //interface, which will load in the ND280 events and store everything.
    //Usage:
    //Number of samples.
    //Array of sample pointers.
    //FitParameters pointer.
    //Whether to load the detector parameters from psyche.
    //Whether to vary the psyche detector parameters.
    psycheInterface* interface = new psycheInterface(7, samples, fitParameters, ND::params().GetParameterI("BANFF.ThrowToys.LoadDetParams"), ND::params().GetParameterI("BANFF.ThrowToys.ThrowDetParams"));


    //If requested in the parameters file, substitute in a fit result for the
    //parameter prior value.
    if(ND::params().GetParameterI("BANFF.ThrowToys.UseFitResult")){

        fitParameters->LoadFittedParameters(ND::params().GetParameterS("BANFF.ThrowToys.FitResultFile"),
                ND::params().GetParameterI("BANFF.ThrowToys.FitNumber"));
    }

    //Print out the parameter information so we can be sure that we loaded in
    //properly.
    fitParameters->PrintCurrentValues();

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
