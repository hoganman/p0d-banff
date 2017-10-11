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
        fitParameters->LoadObsNormParametersFromFile(((ND::params().GetParameterS("BANFF.ObsNormInputFile"))), 3, samples, ND::params().GetParameterI("BANFF.RunFit.FitObsNormParams"));
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
    psycheInterface* interface = new psycheInterface(3, samples, fitParameters, false, false);


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
