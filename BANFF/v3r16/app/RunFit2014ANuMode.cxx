#include "psycheInterface/psycheInterface.hxx"
#include "BANFFSample/BANFFBinnedSample.hxx"
#include "psycheInterface/psycheObservables.hxx"
#include "BANFFObservableBase.hxx"
#include "FitParameters.hxx"
#include "Parameters.hxx"
#include "BANFFSample/BANFFBinnedSampleUtils.hxx"
#include "Minuit2LikelihoodFit/Minuit2LikelihoodFit.cxx"
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

    //Create the observables we will use.
    psycheLeptonCandidateMomentum* pmu = new psycheLeptonCandidateMomentum();
    psycheLeptonCandidateCosTheta* thmu = new psycheLeptonCandidateCosTheta();

    BANFFObservableBase** observables = new BANFFObservableBase*[2];
    observables[0] = pmu;
    observables[1] = thmu;


    //For each observable and sample set up the binning used in the fit.

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
    //Whether to throw statistical errors
    bool throwMCStat = ND::params().GetParameterI("BANFF.RunFit.ThrowMCStat");
    bool throwStat = ND::params().GetParameterI("BANFF.RunFit.ThrowStat");   
    BANFFBinnedSample* anuccqe = new BANFFBinnedSample("anuccqe", SampleId::kFGD1AntiNuMuCCQE, 2, observables, anuccqe_axes, throwMCStat, throwStat);
    BANFFBinnedSample* anuccnqe = new BANFFBinnedSample("anuccnqe", SampleId::kFGD1AntiNuMuCCnQE, 2, observables, anuccnqe_axes, throwMCStat, throwStat);
    BANFFBinnedSample* nuccqe = new BANFFBinnedSample("nuccqe", SampleId::kFGD1NuMuBkgInAntiNuModeCCQE, 2, observables, nuccqe_axes, throwMCStat, throwStat);
    BANFFBinnedSample* nuccnqe = new BANFFBinnedSample("nuccnqe", SampleId::kFGD1NuMuBkgInAntiNuModeCCnQE, 2, observables, nuccnqe_axes, throwMCStat, throwStat);


    BANFFSampleBase** samples = new BANFFSampleBase*[7];
    samples[0] = anuccqe;
    samples[1] = anuccnqe;
    samples[2] = nuccqe;
    samples[3] = nuccnqe;

    //With the samples ready, now define the parameters that are going to be
    //considered.
    //TODO: Again, maybe some of this can be moved to a backend?
    FitParameters* fitParameters = new FitParameters();

    //Load the Flux Parameters.
    //Specify to fit them.
    if(ND::params().GetParameterI("BANFF.RunFit.LoadFluxParams")){
        fitParameters->LoadFluxParametersFromFile(ND::params().GetParameterS("BANFF.FluxInputFile"), 
                ND::params().GetParameterI("BANFF.RunFit.LoadNuModeFluxParams"),  ND::params().GetParameterI("BANFF.RunFit.LoadAntiNuModeFluxParams"),
                ND::params().GetParameterI("BANFF.RunFit.FitFluxParams"));
    }

    //Load the XSec Parameters and specify to fit them
    if(ND::params().GetParameterI("BANFF.RunFit.LoadXSecParams")){
        fitParameters->LoadXSecParametersFromFile(((ND::params().GetParameterS("BANFF.XSecInputFile"))), ND::params().GetParameterI("BANFF.RunFit.FitXSecParams"));
    }

    //Load the ObsNorm parameters, and specify to fit them. 
    if(ND::params().GetParameterI("BANFF.RunFit.LoadObsNormParams")){
        fitParameters->LoadObsNormParametersFromFile(((ND::params().GetParameterS("BANFF.ObsNormInputFile"))), 4, samples, ND::params().GetParameterI("BANFF.RunFit.FitObsNormParams"));
    }

    //We can save pre-fit information to a file here, since everything that has
    //been loaded will be.
    //Just make it iteration 0, save the names, and don't access the
    //undecomposed variable.
    //TFile* preFitFile = new TFile("preFitParameters.root","RECREATE");
    //preFitFile->cd();
    //fitParameters->WriteResults(0, true, false);
    //preFitFile->Close();

    //Now that all parameters are added, flag the flux and FSI parameters for
    //decomposition.
    for(int i = 0; i < fitParameters->GetNumbSysts(); i++){

        if(fitParameters->GetParamType(i)==FLUX){
            fitParameters->SetDecompose(i,1);
        }

        if(fitParameters->GetParamName(i).find("FSI")!=std::string::npos   ){
            std::cout << "Decompose " << fitParameters->GetParamName(i) << std::endl;
            fitParameters->SetDecompose(i,1);
        }

    }

    //Create the psyche interface.  Do not load in or vary the detector
    //parameters from psyche.
    //NB: Although there are program parameters for this, it is something that
    //would never be done.
    psycheInterface* interface = new psycheInterface(4, samples, fitParameters, false, false);


    //Create the fitter that we want, and pass it the interface and the name of
    //the output file.
    Minuit2LikelihoodFit* fitter = new Minuit2LikelihoodFit(interface, ND::params().GetParameterS("BANFF.RunFit.OutputFile"));

    //If requested in the program parameters file, fit to the data loaded in by
    //psyche.
    if(ND::params().GetParameterI("BANFF.RunFit.FitData")){

        fitter->outputFile->cd();
        interface->BuildData();
        interface->BuildNominalMC();

        for(int si = 0; si < interface->nSamples; si++){

            std::string dataName = interface->samples[si]->name + "_data";
            std::string mcName = interface->samples[si]->name + "_prefit";
            samples[si]->SaveData(dataName);
            samples[si]->SaveNominalMC(mcName);

        }

        fitter->InitializeParameters();
        fitter->DoFit();
    }

    //Otherwise, if the nominal MC flag is set, build the nominal MC then use
    //it as the data to fit.
    else if(ND::params().GetParameterI("BANFF.RunFit.FitNominalMC")){

        fitter->outputFile->cd();
        interface->BuildNominalMC();

        for(int si = 0; si < interface->nSamples; si++){

            samples[si]->UseNominalMCAsData();
            std::string dataName = interface->samples[si]->name + "_data";
            std::string mcName = interface->samples[si]->name + "_prefit";
            samples[si]->SaveData(dataName);
            samples[si]->SaveNominalMC(mcName);
        }

        fitter->InitializeParameters();
        fitter->DoFit();


    }

    //Otherwise, fit to a number of toy MC histgorams in a file specified in
    //the program parameters file.
    //Taking advantage of this allows performing multiple validation fits
    //without having to spend time reloading the MC.
    else{

        //Set up the file containing the data.
        TFile* toyFile = new TFile((ND::params().GetParameterS("BANFF.RunFit.ToyFile")).c_str());

        //Loop over the toys.
        for(int i = ND::params().GetParameterI("BANFF.RunFit.FirstToyToFit"); i < ND::params().GetParameterI("BANFF.RunFit.FirstToyToFit") + ND::params().GetParameterI("BANFF.RunFit.NToysToFit"); i++){

            
            fitter->outputFile->cd();
            interface->BuildNominalMC();


            //Loop over the samples.
            for(int si = 0; si < interface->nSamples; si++){

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

            fitter->InitializeParameters();

            //DoFit has an internal counter that will automatically iterate the
            //names of the fit output to fit multiple toys and save the output
            //to the same file..
            fitter->DoFit();

        }



        toyFile->Close();
    }
}
