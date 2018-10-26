#include "Minuit2LikelihoodFit/Minuit2LikelihoodFit.cxx"
#include "BANFFSample/BANFFBinnedSample.hxx"
#include "BANFFSample/BANFFBinnedSampleUtils.hxx"
#include "psycheInterface/psycheInterface.hxx"
#include "psycheInterface/psycheObservables.hxx"
#include "BANFFObservableBase.hxx"
#include "FitParameters.hxx"
#include "Parameters.hxx"
#include <sstream>

//Set up the samples and observables with proper binning, 
//  then load the parameters and perform the fit.
int main(int argc, char** argv){

    if(argc > 2){
        std::cout << "Too many command line arguments!" << std::endl;
    }

    //If there is 1 command line argument, it is specifying a parameter override file.
    //Read this in now.
    //According to the documentation for ReadParamOverrideFile, the code will
    //  definitely use these values of the parameters, ignoring the contents of
    //  any other parameters file.
    else if(argc == 2){
        std::string paramFile(argv[1]);
        ND::params().ReadParamOverrideFile(paramFile);
    }

    ND::params().SetReadParamOverrideFilePointPassed();

    //Create the observables we will use.
    psycheLeptonCandidateMomentum* pmu  = new psycheLeptonCandidateMomentum();
    pmu->SetApplyCoulombShift();
    pmu->SetShiftMuCarbon ();
    pmu->SetShiftAMuCarbon();
    pmu->SetShiftMuOxygen ();
    pmu->SetShiftAMuOxygen();

    psycheLeptonCandidateCosTheta* thmu = new psycheLeptonCandidateCosTheta();

    BANFFObservableBase** observables = new BANFFObservableBase*[2];
    observables[0] = pmu;
    observables[1] = thmu;

    //Create the samples.
    //Supply the:
    //  Name                                    (Optional)
    //  sampleID                                (Required)
    //  Number of observables                   (Required)
    //  Array of observables                    (Required)
    //  Axes for the histograms                 (Optional - required if name is given)
    //  Whether to throw MC statistical errors  (Default: false)
    //  Whether to throw statistical errors     (Default: false)

    //Define selections to use
    bool DoNue        = (bool)ND::params().GetParameterI("BANFF.DoNueSelections");
    bool DoOnlyNue    = (bool)ND::params().GetParameterI("BANFF.DoOnlyNueSelections");
    bool DoMultiPiRHC = (bool)ND::params().GetParameterI("BANFF.DoMultiPiRHC");
    bool Do4PiFHC     = (bool)ND::params().GetParameterI("BANFF.Do4PiFHC");

    //Define whether to throw MC statistical errors and/or statistical errors
    bool throwMCStat  = ND::params().GetParameterI("BANFF.RunFit.ThrowMCStat");
    bool throwStat    = ND::params().GetParameterI("BANFF.RunFit.ThrowStat");

    //Define whether to do likelihood scan or not
    bool likelihoodscan = (bool)ND::params().GetParameterI("BANFF.DoLikelihoodScan");

    //Map for storing the sample IDs
    std::map<SampleId::SampleEnum,BANFFBinnedSample*> SampleMap;

    //Keep track of the number of samples
    int nSample = 6;

    //FHC NuMu selections 
    if(!Do4PiFHC && !DoOnlyNue){ //FHC Multi Pi selections
        std::cout << "---------------------------------" << std::endl;
        std::cout << "Applying FHC CCMultiPi selections" << std::endl;
        std::cout << "---------------------------------" << std::endl;

        // Some idiot proof check...
        if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCCMultiPi")     == 0 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCCMultiPiFGD2") == 0){
            std::cout << "FHC MultiPi is not activated in psycheSteering!" << std::endl;
            throw;
        }
        if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCC4piMultiPi")     == 1 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCC4piMultiPiFGD2") == 1){
            std::cout << "FHC 4Pi is activated in psycheSteering!" << std::endl;
            throw;
        }

        SampleMap[SampleId::kFGD1NuMuCC0Pi  ] = new BANFFBinnedSample(SampleId::kFGD1NuMuCC0Pi  , 2,
                                                                      observables, throwMCStat, throwStat);
        SampleMap[SampleId::kFGD1NuMuCC1Pi  ] = new BANFFBinnedSample(SampleId::kFGD1NuMuCC1Pi  , 2,
                                                                      observables, throwMCStat, throwStat);
        SampleMap[SampleId::kFGD1NuMuCCOther] = new BANFFBinnedSample(SampleId::kFGD1NuMuCCOther, 2,
                                                                      observables, throwMCStat, throwStat);

        SampleMap[SampleId::kFGD2NuMuCC0Pi  ] = new BANFFBinnedSample(SampleId::kFGD2NuMuCC0Pi  , 2,
                                                                      observables, throwMCStat, throwStat);
        SampleMap[SampleId::kFGD2NuMuCC1Pi  ] = new BANFFBinnedSample(SampleId::kFGD2NuMuCC1Pi  , 2,
                                                                      observables, throwMCStat, throwStat);
        SampleMap[SampleId::kFGD2NuMuCCOther] = new BANFFBinnedSample(SampleId::kFGD2NuMuCCOther, 2,
                                                                      observables, throwMCStat, throwStat);
    }
    else if(!DoOnlyNue){ //FHC 4pi selections
        std::cout << "-----------------------------" << std::endl;
        std::cout << "Applying FHC CC4Pi selections" << std::endl;
        std::cout << "-----------------------------" << std::endl;

        // Some idiot proof check...
        if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCCMultiPi")     == 1 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCCMultiPiFGD2") == 1){
            std::cout << "FHC MultiPi is activated in psycheSteering!" << std::endl;
            throw;
        }
        if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCC4piMultiPi")     == 0 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCC4piMultiPiFGD2") == 0){
            std::cout << "FHC 4Pi is not activated in psycheSteering!" << std::endl;
            throw;
        }

        SampleMap[SampleId::kFGD1NuMuCC0Pi  ] = new BANFFBinnedSample(SampleId::kFGD1NuMuCC0Pi  , 2,
                                                                      observables, throwMCStat, throwStat);
        SampleMap[SampleId::kFGD1NuMuCC1Pi  ] = new BANFFBinnedSample(SampleId::kFGD1NuMuCC1Pi  , 2,
                                                                      observables, throwMCStat, throwStat);
        SampleMap[SampleId::kFGD1NuMuCCOther] = new BANFFBinnedSample(SampleId::kFGD1NuMuCCOther, 2,
                                                                      observables, throwMCStat, throwStat);

        SampleMap[SampleId::kFGD2NuMuCC0Pi  ] = new BANFFBinnedSample(SampleId::kFGD2NuMuCC0Pi  , 2,
                                                                      observables, throwMCStat, throwStat);
        SampleMap[SampleId::kFGD2NuMuCC1Pi  ] = new BANFFBinnedSample(SampleId::kFGD2NuMuCC1Pi  , 2,
                                                                      observables, throwMCStat, throwStat);
        SampleMap[SampleId::kFGD2NuMuCCOther] = new BANFFBinnedSample(SampleId::kFGD2NuMuCCOther, 2,
                                                                      observables, throwMCStat, throwStat);

    }

    if(DoNue){ //NuE selections
        std::cout << "-----------------------" << std::endl;
        std::cout << "Applying NuE selections" << std::endl;
        std::cout << "-----------------------" << std::endl;

        // Some idiot proof check...
        if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNuECC")         == 0 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNuECC")     == 0 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerGamma")         == 0 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNuECCFGD2")     == 0 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNuECCFGD2") == 0 || 
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerGammaFGD2")     == 0){
            std::cout << "Nue not activated in psycheSteering!" << std::endl;
            throw;
        }

        //Add the NuE samples to the counter
        nSample = nSample + 12;

        SampleMap[SampleId::kFGD1NuECC               ] = new BANFFBinnedSample(SampleId::kFGD1NuECC               , 2,
                                                                               observables, throwMCStat, throwStat);
        SampleMap[SampleId::kFGD1AntiNuEBkgInNuModeCC] = new BANFFBinnedSample(SampleId::kFGD1AntiNuEBkgInNuModeCC, 2,
                                                                               observables, throwMCStat, throwStat);
        SampleMap[SampleId::kFGD1Gamma               ] = new BANFFBinnedSample(SampleId::kFGD1Gamma               , 2,
                                                                               observables, throwMCStat, throwStat);
        SampleMap[SampleId::kFGD2NuECC               ] = new BANFFBinnedSample(SampleId::kFGD2NuECC               , 2,
                                                                               observables, throwMCStat, throwStat);
        SampleMap[SampleId::kFGD2AntiNuEBkgInNuModeCC] = new BANFFBinnedSample(SampleId::kFGD2AntiNuEBkgInNuModeCC, 2,
                                                                               observables, throwMCStat, throwStat);
        SampleMap[SampleId::kFGD2Gamma               ] = new BANFFBinnedSample(SampleId::kFGD2Gamma               , 2,
                                                                               observables, throwMCStat, throwStat);
        SampleMap[SampleId::kFGD1NuEBkgInAntiNuModeCC] = new BANFFBinnedSample(SampleId::kFGD1NuEBkgInAntiNuModeCC, 2,
                                                                               observables, throwMCStat, throwStat);
        SampleMap[SampleId::kFGD1AntiNuECC           ] = new BANFFBinnedSample(SampleId::kFGD1AntiNuECC           , 2,
                                                                               observables, throwMCStat, throwStat);
        SampleMap[SampleId::kFGD1GammaInAntiNuMode   ] = new BANFFBinnedSample(SampleId::kFGD1GammaInAntiNuMode   , 2,
                                                                               observables, throwMCStat, throwStat);
        SampleMap[SampleId::kFGD2NuEBkgInAntiNuModeCC] = new BANFFBinnedSample(SampleId::kFGD2NuEBkgInAntiNuModeCC, 2,
                                                                               observables, throwMCStat, throwStat);
        SampleMap[SampleId::kFGD2AntiNuECC           ] = new BANFFBinnedSample(SampleId::kFGD2AntiNuECC           , 2,
                                                                               observables, throwMCStat, throwStat);
        SampleMap[SampleId::kFGD2GammaInAntiNuMode   ] = new BANFFBinnedSample(SampleId::kFGD2GammaInAntiNuMode   , 2,
                                                                               observables, throwMCStat, throwStat);
    }
    else{ //Make sure NuE selections are turned off
        // Some idiot proof check...
        if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNuECC")         == 1 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNuECC")     == 1 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerGamma")         == 1 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNuECCFGD2")     == 1 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNuECCFGD2") == 1 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerGammaFGD2")     == 1){
            std::cout << "Nue is activated in psycheSteering!" << std::endl;
            throw;
        }
    }

    // RHC NuMu selections
    if(!DoOnlyNue){
        if(DoMultiPiRHC){ //RHC MultiPi Selections
            std::cout << "---------------------------------" << std::endl;
            std::cout << "Applying RHC CCMultiPi selections" << std::endl;
            std::cout << "---------------------------------" << std::endl;

            // Some idiot proof check...
            if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiPi")             == 0 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiPi")     == 0 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiPiFGD2")         == 0 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiPiFGD2") == 0){
                std::cout << "MultiPi RHC not activated in psycheSteering!" << std::endl;
                throw;
            }
            // Some idiot proof check...
            if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiTrack")             == 1 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiTrack")     == 1 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiTrackFGD2")         == 1 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiTrackFGD2") == 1){
                std::cout << "MultiTrack RHC is activated in psycheSteering!" << std::endl;
                throw;
            }

            //Add RHC MultiPi samples to the counter
            nSample = nSample + 12;

            SampleMap[SampleId::kFGD1NuMuBkgInAntiNuModeCC0Pi   ] = new BANFFBinnedSample(SampleId::kFGD1NuMuBkgInAntiNuModeCC0Pi  , 2,
                                                                                          observables, throwMCStat, throwStat);
            SampleMap[SampleId::kFGD1NuMuBkgInAntiNuModeCC1Pi   ] = new BANFFBinnedSample(SampleId::kFGD1NuMuBkgInAntiNuModeCC1Pi  , 2,
                                                                                          observables, throwMCStat, throwStat);
            SampleMap[SampleId::kFGD1NuMuBkgInAntiNuModeCCOther ] = new BANFFBinnedSample(SampleId::kFGD1NuMuBkgInAntiNuModeCCOther, 2,
                                                                                          observables, throwMCStat, throwStat);
            SampleMap[SampleId::kFGD1AntiNuMuCC0Pi              ] = new BANFFBinnedSample(SampleId::kFGD1AntiNuMuCC0Pi             , 2,
                                                                                          observables, throwMCStat, throwStat);
            SampleMap[SampleId::kFGD1AntiNuMuCC1Pi              ] = new BANFFBinnedSample(SampleId::kFGD1AntiNuMuCC1Pi             , 2,
                                                                                          observables, throwMCStat, throwStat);
            SampleMap[SampleId::kFGD1AntiNuMuCCOther            ] = new BANFFBinnedSample(SampleId::kFGD1AntiNuMuCCOther           , 2,
                                                                                          observables, throwMCStat, throwStat);
            SampleMap[SampleId::kFGD2NuMuBkgInAntiNuModeCC0Pi   ] = new BANFFBinnedSample(SampleId::kFGD2NuMuBkgInAntiNuModeCC0Pi  , 2,
                                                                                          observables, throwMCStat, throwStat);
            SampleMap[SampleId::kFGD2NuMuBkgInAntiNuModeCC1Pi   ] = new BANFFBinnedSample(SampleId::kFGD2NuMuBkgInAntiNuModeCC1Pi  , 2,
                                                                                          observables, throwMCStat, throwStat);
            SampleMap[SampleId::kFGD2NuMuBkgInAntiNuModeCCOther ] = new BANFFBinnedSample(SampleId::kFGD2NuMuBkgInAntiNuModeCCOther, 2,
                                                                                          observables, throwMCStat, throwStat);
            SampleMap[SampleId::kFGD2AntiNuMuCC0Pi              ] = new BANFFBinnedSample(SampleId::kFGD2AntiNuMuCC0Pi             , 2,
                                                                                          observables, throwMCStat, throwStat);
            SampleMap[SampleId::kFGD2AntiNuMuCC1Pi              ] = new BANFFBinnedSample(SampleId::kFGD2AntiNuMuCC1Pi             , 2,
                                                                                          observables, throwMCStat, throwStat);
            SampleMap[SampleId::kFGD2AntiNuMuCCOther            ] = new BANFFBinnedSample(SampleId::kFGD2AntiNuMuCCOther           , 2,
                                                                                          observables, throwMCStat, throwStat);
        }
        else{ //RHC MultiTrack Selections
            std::cout << "---------------------------------" << std::endl;
            std::cout << "Applying RHC MultiTrack selection" << std::endl;
            std::cout << "---------------------------------" << std::endl;

            // Some idiot proof check...
            if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiPi")             == 1 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiPi")     == 1 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiPiFGD2")         == 1 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiPiFGD2") == 1){
                std::cout << "MultiPi RHC is activated in psycheSteering!" << std::endl;
                throw;
            }
            // Some idiot proof check...
            if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiTrack")             == 0 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiTrack")     == 0 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiTrackFGD2")         == 0 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiTrackFGD2") == 0){
                std::cout << "MultiTrack RHC not activated in psycheSteering!" << std::endl;
                throw;
            }

            //Add RHC MultiTrack samples to the counter
            nSample=nSample+8;

            SampleMap[SampleId::kFGD1AntiNuMuCC1Track            ] = new BANFFBinnedSample(SampleId::kFGD1AntiNuMuCC1Track            , 2,
                                                                                           observables, throwMCStat, throwStat);
            SampleMap[SampleId::kFGD1AntiNuMuCCNTracks           ] = new BANFFBinnedSample(SampleId::kFGD1AntiNuMuCCNTracks           , 2,
                                                                                           observables, throwMCStat, throwStat);
            SampleMap[SampleId::kFGD1NuMuBkgInAntiNuModeCC1Track ] = new BANFFBinnedSample(SampleId::kFGD1NuMuBkgInAntiNuModeCC1Track , 2,
                                                                                           observables, throwMCStat, throwStat);
            SampleMap[SampleId::kFGD1NuMuBkgInAntiNuModeCCNTracks] = new BANFFBinnedSample(SampleId::kFGD1NuMuBkgInAntiNuModeCCNTracks, 2,
                                                                                           observables, throwMCStat, throwStat);
            SampleMap[SampleId::kFGD2AntiNuMuCC1Track            ] = new BANFFBinnedSample(SampleId::kFGD2AntiNuMuCC1Track            , 2,
                                                                                           observables, throwMCStat, throwStat);
            SampleMap[SampleId::kFGD2AntiNuMuCCNTracks           ] = new BANFFBinnedSample(SampleId::kFGD2AntiNuMuCCNTracks           , 2,
                                                                                           observables, throwMCStat, throwStat);
            SampleMap[SampleId::kFGD2NuMuBkgInAntiNuModeCC1Track ] = new BANFFBinnedSample(SampleId::kFGD2NuMuBkgInAntiNuModeCC1Track , 2,
                                                                                           observables, throwMCStat, throwStat);
            SampleMap[SampleId::kFGD2NuMuBkgInAntiNuModeCCNTracks] = new BANFFBinnedSample(SampleId::kFGD2NuMuBkgInAntiNuModeCCNTracks, 2,
                                                                                           observables, throwMCStat, throwStat);
        }
    }
    else{ // ONLY NUE SELECTIONS
        // Some idiot proof check...
        if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiPi")             == 1 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiPi")     == 1 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiPiFGD2")         == 1 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiPiFGD2") == 1){
            std::cout << "MultiPi RHC is activated in psycheSteering!" << std::endl;
            throw;
        }
        if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiTrack")             == 1 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiTrack")     == 1 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiTrackFGD2")         == 1 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiTrackFGD2") == 1){
            std::cout << "MultiTrack RHC is activated in psycheSteering!" << std::endl;
            throw;
        }
        if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCCMultiPi")     == 1 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCCMultiPiFGD2") == 1){
            std::cout << "FHC MultiPi is activated in psycheSteering!" << std::endl;
            throw;
        }
        if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCC4piMultiPi")     == 1 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCC4piMultiPiFGD2") == 1){
            std::cout << "FHC CC4pi multiPi is activated in psycheSteering!" << std::endl;
            throw;
        }

        //Sample counter only needs the number of samples in the NuE selection
        nSample = 12;
    }

    BANFFSampleBase** samples = new BANFFSampleBase*[nSample];

    int iSample = 0;
    for(int i = SampleId::kUnassigned; i < SampleId::kNSamples; ++i){
        SampleId::SampleEnum smpl = (SampleId::SampleEnum)i;
        if(SampleMap[smpl]){
            std::cout << "Loading sample " << ConvertSample(smpl) << " to be the number " << iSample << " selection" << std::endl;
            samples[iSample++] = SampleMap[smpl];
        }
    }

    //With the samples ready, now define the parameters that are going to be considered.
    //TODO: Again, maybe some of this can be moved to a backend?
    FitParameters* fitParameters = new FitParameters();

    //Load the Flux Parameters and specify whether to fit them.
    if(ND::params().GetParameterI("BANFF.RunFit.LoadFluxParams")){
        fitParameters->LoadFluxParametersFromFile(ND::params().GetParameterS("BANFF.FluxInputFile"),
                                                  ND::params().GetParameterI("BANFF.RunFit.LoadNuModeFluxParams"),
                                                  ND::params().GetParameterI("BANFF.RunFit.LoadAntiNuModeFluxParams"),
                                                  ND::params().GetParameterI("BANFF.RunFit.FitFluxParams"));
    }

    //Load the ObsNorm parameters and specify whether to fit them. 
    if(ND::params().GetParameterI("BANFF.RunFit.LoadObsNormParams")){
        fitParameters->LoadObsNormParametersFromFile(ND::params().GetParameterS("BANFF.ObsNormInputFile"),
                                                     nSample, samples,
                                                     ND::params().GetParameterI("BANFF.RunFit.FitObsNormParams"));
    }

    //Load the XSec Parameters and specify whether to fit them
    if(ND::params().GetParameterI("BANFF.RunFit.LoadXSecParams")){
        fitParameters->LoadXSecParametersFromFile(ND::params().GetParameterS("BANFF.XSecInputFile"),
                                                  ND::params().GetParameterI("BANFF.RunFit.FitXSecParams"));
    }

    //Format output file name
    std::string PsycheVersion = anaUtils::GetSoftwareVersionFromPath((std::string)getenv("ND280PSYCHEROOT"));
    std::string outputName    = ND::params().GetParameterS("BANFF.RunFit.OutputFile");

    outputName = PsycheVersion + "_" + outputName;

    if(Do4PiFHC) { outputName = "CC4PiFHC_" + outputName; }
    if(DoNue)    { outputName = "NuE_" + outputName;      }

    if(!DoOnlyNue){
        if(DoMultiPiRHC) { outputName = "MultiPiRHC_"+outputName;    } 
        else             { outputName = "MultiTrackRHC_"+outputName; }
    } 
    else{ outputName = "ONLY"+outputName; }

    if(likelihoodscan) { outputName = "SCAN_"+outputName; }

    //Since everything that needs to be loaded will be,
    //  we can save the prefit information to a file here.
    //Make it iteration 0, save the names, and don't access the undecomposed variable.
    if(ND::params().GetParameterI("BANFF.SavePrefitParamsAndExit")){
        //For toy fitting only
        if(!ND::params().GetParameterI("BANFF.RunFit.FitData") &&
           !ND::params().GetParameterI("BANFF.RunFit.FitNominalMC")){
            //Set up the file containing the data.
            TFile* toyFile = new TFile((ND::params().GetParameterS("BANFF.RunFit.ToyFile")).c_str());

            //Now apply new prior from throw file
            fitParameters->SetPriorFromToyFile(toyFile, ND::params().GetParameterI("BANFF.RunFit.FirstToyToFit"));

            //Copy prior values across to current value to be written out
            for(int i = 0; i < fitParameters->GetNumbSysts(); i++){
                fitParameters->SetParameterValue(i, fitParameters->GetPriorValue(i));
            }
        }

        TFile* preFitFile = new TFile(("prefit_"+outputName).c_str(),"RECREATE");
        preFitFile->cd();
        fitParameters->WriteResults(0, true, false);
        preFitFile->Close();
        exit(1);
    }

    //Now that all parameters are added, flag the flux, detector (obsnorm) and FSI parameters for decomposition.
    std::cout << "======================" << std::endl;
    std::cout << "Decomposed parameters:" << std::endl;
    std::cout << "----------------------" << std::endl;
    if (!likelihoodscan) { //asimov fit or prefit
        for(int i = 0; i < fitParameters->GetNumbSysts(); i++){

            if(fitParameters->GetParamType(i)==FLUX){
                fitParameters->SetDecompose(i,1);
                std::cout << "    " << fitParameters->GetParamName(i) << ": YES" << std::endl;
            }
            else if(fitParameters->GetParamType(i)==OBSNORM){
                fitParameters->SetDecompose(i,1);
                std::cout << "    " << fitParameters->GetParamName(i) << ": YES" << std::endl;
            }
            else if(fitParameters->GetParamName(i).find("FEF")!=std::string::npos){
                fitParameters->SetDecompose(i,1);
                std::cout << "    " << fitParameters->GetParamName(i) << ": YES" << std::endl;
            }
            else{
                std::cout << "    " << fitParameters->GetParamName(i) << ": NO" << std::endl;
            }

        }
    }
    else { //likelihood scan
        //If DoLikelihoodScan is set, don't decompose any parameters
        for(int i = 0; i < fitParameters->GetNumbSysts(); i++){

            if(fitParameters->GetParamType(i)==FLUX){
                fitParameters->SetDecompose(i,0);
                std::cout << "    " << fitParameters->GetParamName(i) << ": NO" << std::endl;
            }
            else if(fitParameters->GetParamType(i)==OBSNORM){
                fitParameters->SetDecompose(i,0);
                std::cout << "    " << fitParameters->GetParamName(i) << ": NO" << std::endl;
            }
            else if(fitParameters->GetParamName(i).find("FEF")!=std::string::npos){
                fitParameters->SetDecompose(i,0);
                std::cout << "    " << fitParameters->GetParamName(i) << ": NO" << std::endl;
            }
            else{
                std::cout << "    " << fitParameters->GetParamName(i) << ": NO" << std::endl;
            }

        }
    }
    std::cout << "======================" << std::endl;

    //Create the psyche interface.
    //Do not load in or vary the detector parameters from psyche.
    //NB: Although there are program parameters for this, 
    //    it is something that would never be done.

    std::string diagnostic_name = "diagnostic_" + outputName;

    psycheInterface* interface = new psycheInterface(nSample, samples, fitParameters, false, false, diagnostic_name);

    //Create the fitter that we want, and pass it the interface and the name of the output file.
    Minuit2LikelihoodFit* fitter = new Minuit2LikelihoodFit(interface, outputName);

    //If requested in the program parameters file, fit to the data loaded in by psyche.
    if(ND::params().GetParameterI("BANFF.RunFit.FitData")){

        fitter->outputFile->cd();
        interface->BuildData();
        interface->BuildNominalMC();

        for(int si = 0; si < interface->nSamples; si++){
            std::string dataName = interface->samples[si]->name + "_data";
            std::string mcName   = interface->samples[si]->name + "_prefit";
            samples[si]->SaveData(dataName);
            samples[si]->SaveNominalMC(mcName);
        }

        if(ND::params().GetParameterI("BANFF.SavePrefitAndExit")){
            fitter->outputFile->Close();
            exit(1);
        }

        fitter->InitializeParameters();

        if(likelihoodscan) {
            fitter->ScanParameters();
            exit(1);
        }

        fitter->DoFit();
        fitter->outputFile->Close();
        exit(1);
    }

    //Otherwise, if the nominal MC flag is set, build the nominal MC then use it as the data to fit.
    else if(ND::params().GetParameterI("BANFF.RunFit.FitNominalMC")){

        fitter->outputFile->cd();
        interface->BuildNominalMC();

        for(int si = 0; si < interface->nSamples; si++){
            samples[si]->UseNominalMCAsData();
            std::string dataName = interface->samples[si]->name + "_data";
            std::string mcName   = interface->samples[si]->name + "_prefit";
            samples[si]->SaveData(dataName);
            samples[si]->SaveNominalMC(mcName);
        }

        if(ND::params().GetParameterI("BANFF.SavePrefitAndExit")){
            fitter->outputFile->Close();
            exit(1);
        }

        fitter->InitializeParameters();

        if(likelihoodscan) {
            fitter->ScanParameters();
            exit(1);
        }

        fitter->DoFit();
        fitter->outputFile->Close();
        exit(1);
    }

    //Otherwise, fit to a number of toy MC histograms in a file specified in the program parameters file.
    //Using this allows you to perform multiple validation fits without having to spend time reloading the MC.
    else{

        //Set up the file containing the data.
        TFile* toyFile = new TFile((ND::params().GetParameterS("BANFF.RunFit.ToyFile")).c_str());

        //If running a pull study, undecompose the parameters before moving on
        if(ND::params().GetParameterI("BANFF.RunFit.PullStudy")){
            interface->fitParameters->UnDecomposeLoadedParameters(0,0,true);
        }

        //Loop over the toys.
        for(int i = ND::params().GetParameterI("BANFF.RunFit.FirstToyToFit");
                i < ND::params().GetParameterI("BANFF.RunFit.FirstToyToFit") + ND::params().GetParameterI("BANFF.RunFit.NToysToFit");
                i++){

            if(ND::params().GetParameterI("BANFF.RunFit.PullStudy")){
                // Now apply new prior from throw file
                interface->fitParameters->SetPriorFromToyFile(toyFile, i);
                // Now decompose again, so the fit works correctly
                interface->fitParameters->DecomposeParameters();
            }

            fitter->outputFile->cd();
            interface->BuildNominalMC();

            //Loop over the samples.
            for(int si = 0; si < interface->nSamples; si++){
                std::stringstream histoStream;
                histoStream << samples[si]->name << "_predMC_" << i;

                //Load the predicted MC histogram in for this sample's data histogram.
                samples[si]->LoadDataFromFile(toyFile,histoStream.str());

                std::string dataName = interface->samples[si]->name + "_data";
                std::string mcName   = interface->samples[si]->name + "_prefit";
                samples[si]->SaveData(dataName);
                samples[si]->SaveNominalMC(mcName);
            }

            if(ND::params().GetParameterI("BANFF.SavePrefitAndExit")) { throw; }
            fitter->InitializeParameters();

            //DoFit has an internal counter that will automatically iterate the
            //names of the fit output to fit multiple toys and save the output
            //to the same file.
            fitter->DoFit();
        }

        toyFile->Close();
    }

}
