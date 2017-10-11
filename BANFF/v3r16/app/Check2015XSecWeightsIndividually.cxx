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


    //The binning used for performing the fit.
    
    //TODO: Maybe move some of this onto the backend?  Assemble TAxis pointers
    //inside the object?
    //Alternatively, a scrip to easily assemble files with TAxis definitions in
    //them, and the card file has a path to the TAxis files?
    //Set the binning for the CC0pi sample.
    Int_t npbins_0pi = 14;
    Double_t pbins_0pi[15] = {0.,300.,400.,500.,600.,700.,800.,900.,1000.,1250.,1500.,2000.,3000.,5000.,30000.};
    Int_t nctbins_0pi = 11;
    Double_t ctbins_0pi[12] = {-1.0,0.6,0.7,0.8,0.85,0.90,0.92,0.94,0.96,0.98,0.99,1.0};
    
    TAxis* pAxis_0pi = new TAxis(npbins_0pi,pbins_0pi);
    TAxis* thAxis_0pi = new TAxis(nctbins_0pi, ctbins_0pi);
    TAxis** cc0pi_axes = new TAxis*[2];
    cc0pi_axes[0] = pAxis_0pi;
    cc0pi_axes[1] = thAxis_0pi;


    //Set the binning for the CC1pi sample.
    Int_t npbins_1pi = 13;
    Double_t pbins_1pi[14] = {0.,300.,400.,500.,600.,700.,800.,900.,1000.,1250.,1500.,2000.,5000.,30000.};
    Int_t nctbins_1pi = 11;
    Double_t ctbins_1pi[12] = {-1.0,0.6,0.7,0.8,0.85,0.90,0.92,0.94,0.96,0.98,0.99,1.0}; 
    
    TAxis* pAxis_1pi = new TAxis(npbins_1pi,pbins_1pi);
    TAxis* thAxis_1pi = new TAxis(nctbins_1pi, ctbins_1pi);
    TAxis** cc1pi_axes = new TAxis*[2];
    cc1pi_axes[0] = pAxis_1pi;
    cc1pi_axes[1] = thAxis_1pi;


    //Set the binning for the CC-Other sample.
    Int_t npbins_npi = 14;
    Double_t pbins_npi[15] = {0.,300.,400.,500.,600.,700.,800.,900.,1000.,1250.,1500.,2000.,3000.,5000.,30000.};
    Int_t nctbins_npi = 11;
    Double_t ctbins_npi[12] = {-1.0,0.6,0.7,0.8,0.85,0.90,0.92,0.94,0.96,0.98,0.99,1.0};
    
    TAxis* pAxis_npi = new TAxis(npbins_npi,pbins_npi);
    TAxis* thAxis_npi = new TAxis(nctbins_npi, ctbins_npi);
    TAxis** ccnpi_axes = new TAxis*[2];
    ccnpi_axes[0] = pAxis_npi;
    ccnpi_axes[1] = thAxis_npi;

    //Anti-numu CCQE selection in anti-neutrino beam mode.
    Int_t npbins_anuccqe = 5;
    Double_t pbins_anuccqe[6] = {0., 500., 900., 1200., 2000., 10000.};

    Int_t nctbins_anuccqe = 4;
    Double_t ctbins_anuccqe[5] = {-1.0, 0.8, 0.92, 0.98, 1.0};

    TAxis* pAxis_anuccqe = new TAxis(npbins_anuccqe,pbins_anuccqe);
    TAxis* thAxis_anuccqe = new TAxis(nctbins_anuccqe, ctbins_anuccqe);
    TAxis** anuccqe_axes = new TAxis*[2];
    anuccqe_axes[0] = pAxis_anuccqe;
    anuccqe_axes[1] = thAxis_anuccqe;

    //Anti-numu CCnQE selection in anti-neutrino beam mode.
    Int_t npbins_anuccnqe = 5;
    Double_t pbins_anuccnqe[6] = {0., 600., 1000., 1500., 2200., 10000.};

    Int_t nctbins_anuccnqe = 4;
    Double_t ctbins_anuccnqe[5] = {-1.0, 0.8, 0.9, 0.97, 1.0};

    TAxis* pAxis_anuccnqe = new TAxis(npbins_anuccnqe,pbins_anuccnqe);
    TAxis* thAxis_anuccnqe = new TAxis(nctbins_anuccnqe, ctbins_anuccnqe);
    TAxis** anuccnqe_axes = new TAxis*[2];
    anuccnqe_axes[0] = pAxis_anuccnqe;
    anuccnqe_axes[1] = thAxis_anuccnqe;

    //Numu CCQE selection in anti-neutrino beam mode.
    Int_t npbins_nuccqe = 5;
    Double_t pbins_nuccqe[6] = {0., 500., 900., 1200., 2000., 10000.};

    Int_t nctbins_nuccqe = 4;
    Double_t ctbins_nuccqe[5] = {-1.0, 0.8, 0.92, 0.98, 1.0};

    TAxis* pAxis_nuccqe = new TAxis(npbins_nuccqe,pbins_nuccqe);
    TAxis* thAxis_nuccqe = new TAxis(nctbins_nuccqe, ctbins_nuccqe);
    TAxis** nuccqe_axes = new TAxis*[2];
    nuccqe_axes[0] = pAxis_nuccqe;
    nuccqe_axes[1] = thAxis_nuccqe;

    //Numu CCnQE selection in anti-neutrino beam mode.
    Int_t npbins_nuccnqe = 5;
    Double_t pbins_nuccnqe[6] = {0., 600., 1000., 1500., 2200., 10000.};

    Int_t nctbins_nuccnqe = 4;
    Double_t ctbins_nuccnqe[5] = {-1.0, 0.8, 0.9, 0.97, 1.0};

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
    BANFFBinnedSample* cc0pi = new BANFFBinnedSample("cc0pi", SampleId::kFGD1NuMuCC0Pi, 2, observables, cc0pi_axes, throwMCStat, throwStat);
    BANFFBinnedSample* cc1pi = new BANFFBinnedSample("cc1pi", SampleId::kFGD1NuMuCC1Pi, 2, observables, cc1pi_axes, throwMCStat, throwStat);
    BANFFBinnedSample* ccOth = new BANFFBinnedSample("ccOth", SampleId::kFGD1NuMuCCOther, 2, observables, ccnpi_axes, throwMCStat, throwStat);
    BANFFBinnedSample* anuccqe = new BANFFBinnedSample("anuccqe", SampleId::kFGD1AntiNuMuCC1Track, 2, observables, anuccqe_axes, throwMCStat, throwStat);
    BANFFBinnedSample* anuccnqe = new BANFFBinnedSample("anuccnqe", SampleId::kFGD1AntiNuMuCCNTracks, 2, observables, anuccnqe_axes, throwMCStat, throwStat);
    BANFFBinnedSample* nuccqe = new BANFFBinnedSample("nuccqe", SampleId::kFGD1NuMuBkgInAntiNuModeCC1Track, 2, observables, nuccqe_axes, throwMCStat, throwStat);
    BANFFBinnedSample* nuccnqe = new BANFFBinnedSample("nuccnqe", SampleId::kFGD1NuMuBkgInAntiNuModeCCNTracks, 2, observables, nuccnqe_axes, throwMCStat, throwStat);

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


    //Create the output file that the plots will be saved into
    TFile* outFile = new TFile((ND::params().GetParameterS("BANFF.ThrowToys.OutputFile")).c_str(),"RECREATE");
    outFile->cd();

    //Start by building and saving an MC prediction with everything enabled.
    interface->BuildSaveAndResetMCPrediction("all");

    //Now, start by disabling all XSec parameters, then saving predMC.
    fitParameters->DisableParameterByName("FSI_INEL_LO_E");
    fitParameters->DisableParameterByName("FSI_INEL_HI_E");
    fitParameters->DisableParameterByName("FSI_PI_PROD");
    fitParameters->DisableParameterByName("FSI_PI_ABS");
    fitParameters->DisableParameterByName("FSI_CEX_LO_E");
    fitParameters->DisableParameterByName("FSI_CEX_HI_E");
    fitParameters->DisableParameterByName("MAQE");
    fitParameters->DisableParameterByName("pF_C");
    fitParameters->DisableParameterByName("MEC_ND280");
    fitParameters->DisableParameterByName("EB_C");
    fitParameters->DisableParameterByName("pF_O");
    fitParameters->DisableParameterByName("MEC_SK");
    fitParameters->DisableParameterByName("EB_O");
    fitParameters->DisableParameterByName("CA5");
    fitParameters->DisableParameterByName("MANFFRES");
    fitParameters->DisableParameterByName("BgRES");
    fitParameters->DisableParameterByName("CCNUE_0");
    fitParameters->DisableParameterByName("DISMPISHP");
    fitParameters->DisableParameterByName("CCCOH_C_0");
    fitParameters->DisableParameterByName("CCCOH_O_0");
    fitParameters->DisableParameterByName("NCCOH_0");
    fitParameters->DisableParameterByName("NCOTHER_0");
    fitParameters->DisableParameterByName("RPA_C");
    fitParameters->DisableParameterByName("SF_RFG");


    interface->BuildSaveAndResetMCPrediction("noXSec");

    //Now add in SF to RFG weighting.
    fitParameters->EnableParameterByName("SF_RFG");
    interface->BuildSaveAndResetMCPrediction("RFG");

    //Now add in RPA. 
    fitParameters->EnableParameterByName("RPA_C");
    interface->BuildSaveAndResetMCPrediction("RFGwRPA");

    //Now add in the norms.
    fitParameters->EnableParameterByName("MEC_ND280");
    fitParameters->EnableParameterByName("MEC_SK");
    fitParameters->EnableParameterByName("CCCOH_C_0");
    fitParameters->EnableParameterByName("CCCOH_O_0");
    fitParameters->EnableParameterByName("NCCOH_0");
    fitParameters->EnableParameterByName("NCOTHER_0");
    interface->BuildSaveAndResetMCPrediction("RFGwRPAwNorms");

    //Now add in MAQE.
    fitParameters->EnableParameterByName("MAQE");
    interface->BuildSaveAndResetMCPrediction("RFGwRPAwNormswMAQE");

    //Now add in pF_C
    fitParameters->EnableParameterByName("pF_C");
    interface->BuildSaveAndResetMCPrediction("RFGwRPAwNormswMAQEwpFC");

    outFile->Close();
    
    return 0;
}
