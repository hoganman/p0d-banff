#include "psycheInterface/psycheInterface.hxx"
#include "BANFFSample/BANFFBinnedSample.hxx"
#include "psycheInterface/psycheObservables.hxx"
#include "BANFFObservableBase.hxx"
#include "FitParameters.hxx"
#include "Parameters.hxx"
#include "BANFFSample/BANFFBinnedSampleUtils.hxx"
#include <sys/time.h>
#include <TRandom3.h>

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

    ND::params().SetReadParamOverrideFilePointPassed();


    //Create the observables that will use.  Store them in an array to pass to
    //the sample.
    //TODO: Make this accessible via a card file instead of manually coding it
    //in.
    psycheLeptonCandidateMomentum* pmu = new psycheLeptonCandidateMomentum();
    psycheLeptonCandidateCosTheta* thmu = new psycheLeptonCandidateCosTheta();

    BANFFObservableBase** observables = new BANFFObservableBase*[2];
    observables[0] = pmu;
    observables[1] = thmu;

    //setting the default gRandom seed (no idea if this will work)
    gRandom->SetSeed(119911);

    //Create the samples.
    //Supply the:
    //name
    //sampleID
    //Number of observables.
    //Array of observables.
    //Axes for the histograms.
    //Whether to throw MC statistical errors.
    //Whether to throw statistical errors.
    //Whether to throw MC statistical errors with event by event Poisson throws
    //(use instead of Gaussian MC statistical errors)
    bool throwMCStat = ND::params().GetParameterI("BANFF.ThrowToys.ThrowMCStat");
    bool throwStat = ND::params().GetParameterI("BANFF.ThrowToys.ThrowStat");

    std::map<SampleId::SampleEnum,BANFFBinnedSample*> SampleMap;
    int nSample = 14;

    SampleMap[SampleId::kFGD1NuMuCC0Pi  ] = new BANFFBinnedSample(SampleId::kFGD1NuMuCC0Pi,
            2, observables, throwMCStat, throwStat);
    SampleMap[SampleId::kFGD1NuMuCC1Pi  ] = new BANFFBinnedSample(SampleId::kFGD1NuMuCC1Pi,
            2, observables, throwMCStat, throwStat);
    SampleMap[SampleId::kFGD1NuMuCCOther] = new BANFFBinnedSample(SampleId::kFGD1NuMuCCOther,
            2, observables, throwMCStat, throwStat);
    SampleMap[SampleId::kFGD2NuMuCC0Pi  ] = new BANFFBinnedSample(SampleId::kFGD2NuMuCC0Pi,
            2, observables, throwMCStat, throwStat);
    SampleMap[SampleId::kFGD2NuMuCC1Pi  ] = new BANFFBinnedSample(SampleId::kFGD2NuMuCC1Pi,
            2, observables, throwMCStat, throwStat);
    SampleMap[SampleId::kFGD2NuMuCCOther] = new BANFFBinnedSample(SampleId::kFGD2NuMuCCOther,
            2, observables, throwMCStat, throwStat);
    SampleMap[SampleId::kFGD1AntiNuMuCC1Track            ] = new BANFFBinnedSample(SampleId::kFGD1AntiNuMuCC1Track,
            2, observables, throwMCStat, throwStat);
    SampleMap[SampleId::kFGD1AntiNuMuCCNTracks           ] = new BANFFBinnedSample(SampleId::kFGD1AntiNuMuCCNTracks,
            2, observables, throwMCStat, throwStat);
    SampleMap[SampleId::kFGD2AntiNuMuCC1Track            ] = new BANFFBinnedSample(SampleId::kFGD2AntiNuMuCC1Track,
            2, observables, throwMCStat, throwStat);
    SampleMap[SampleId::kFGD2AntiNuMuCCNTracks           ] = new BANFFBinnedSample(SampleId::kFGD2AntiNuMuCCNTracks,
            2, observables, throwMCStat, throwStat);
    SampleMap[SampleId::kFGD1NuMuBkgInAntiNuModeCC1Track ] = new BANFFBinnedSample(SampleId::kFGD1NuMuBkgInAntiNuModeCC1Track,
            2, observables, throwMCStat, throwStat);
    SampleMap[SampleId::kFGD1NuMuBkgInAntiNuModeCCNTracks] = new BANFFBinnedSample(SampleId::kFGD1NuMuBkgInAntiNuModeCCNTracks,
            2, observables, throwMCStat, throwStat);
    SampleMap[SampleId::kFGD2NuMuBkgInAntiNuModeCC1Track ] = new BANFFBinnedSample(SampleId::kFGD2NuMuBkgInAntiNuModeCC1Track,
            2, observables, throwMCStat, throwStat);
    SampleMap[SampleId::kFGD2NuMuBkgInAntiNuModeCCNTracks] = new BANFFBinnedSample(SampleId::kFGD2NuMuBkgInAntiNuModeCCNTracks,
            2, observables, throwMCStat, throwStat);


    BANFFSampleBase** samples = new BANFFSampleBase*[nSample];

    int iSample = 0;
    for(int i = SampleId::kUnassigned; i < SampleId::kNSamples; ++i){
        SampleId::SampleEnum smpl = (SampleId::SampleEnum)i;
        if(SampleMap[smpl]){
            std::cout << "Loading sample " << ConvertSample(smpl)
                << " to be the " << iSample << "th selection" << std::endl;
            samples[iSample++] = SampleMap[smpl];
        }
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

    //The ability to load observable normalization parameters if so desired,
    //likely for validation purposes.
    if(ND::params().GetParameterI("BANFF.ThrowToys.LoadObsNormParams")){
        fitParameters->LoadObsNormParametersFromFile(ND::params().GetParameterS("BANFF.ObsNormInputFile"), nSample, samples,
                ND::params().GetParameterI("BANFF.ThrowToys.ThrowObsNormParams"));
    }

    //Load the XSec Parameters and specify to vary them.
    if(ND::params().GetParameterI("BANFF.ThrowToys.LoadXSecParams")){
        fitParameters->LoadXSecParametersFromFile(ND::params().GetParameterS("BANFF.XSecInputFile"),
                ND::params().GetParameterI("BANFF.ThrowToys.ThrowXSecParams"));
    }



    //With the samples, observables, and fit parameters specified, create the
    //interface, which will load in the ND280 events and store everything.
    //Usage:
    //Number of samples.
    //Array of sample pointers.
    //FitParameters pointer.
    //Whether to load the detector parameters from psyche.
    //Whether to vary the psyche detector parameters.
    //diagnostic name

    std::string outputName = ND::params().GetParameterS("BANFF.ThrowToys.OutputFile");
    std::string diagnostic_name = "diagnostic_" + outputName;

    psycheInterface* interface = new psycheInterface(nSample, samples, fitParameters, ND::params().GetParameterI("BANFF.ThrowToys.LoadDetParams"), ND::params().GetParameterI("BANFF.ThrowToys.ThrowDetParams"), diagnostic_name);


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

    std::cout << "----------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Begin toy throwing" << std::endl;
    std::cout << "----------------------------------------------------------------------------------------" << std::endl;

//    if(ND::params().GetParameterI("BANFF.ThrowToys.PullStudy")){
//        interface->ThrowToys(nToys, outputName, true);
//    }
//    else{
    interface->ThrowToys(nToys, outputName, false);
//    }

    gettimeofday(&tim, NULL);
    double t1 = tim.tv_sec + (tim.tv_usec/1000000.0);

    std::cout << "Time to throw toys: " << (t1 - t0) << std::endl;
    std::cout << "Time per toy: " << (t1 - t0)/((double)nToys) << std::endl;

    return 0;
}
