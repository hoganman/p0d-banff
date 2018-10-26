#include "psycheInterface/psycheInterface.hxx"
#include "BANFFSample/BANFFBinnedSample.hxx"
#include "psycheInterface/psycheObservables.hxx"
#include "BANFFObservableBase.hxx"
#include "FitParameters.hxx"
#include "Parameters.hxx"
#include "BANFFSample/BANFFBinnedSampleUtils.hxx"
#include "Minuit2LikelihoodFit/Minuit2LikelihoodFit.cxx"
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
    bool Do4PiFHC = (bool)ND::params().GetParameterI("BANFF.Do4PiFHC");

    //Create the samples.
    //Supply the:
    //OPTIONAL name
    //sampleID
    //Number of observables.
    //Array of observables.
    //OPTIONAL Axes for the histograms.
    //Whether to throw MC statistical errors.
    //Whether to throw statistical errors
    bool throwMCStat = ND::params().GetParameterI("BANFF.RunFit.ThrowMCStat");
    bool throwStat   = ND::params().GetParameterI("BANFF.RunFit.ThrowStat");

    // // FHC Numu selections 
    std::map<SampleId::SampleEnum,BANFFBinnedSample*> SampleMap;
    
    int nSample = 6;
    bool DoNue        = (bool)ND::params().GetParameterI("BANFF.DoNueSelections");
    bool DoOnlyNue    = (bool)ND::params().GetParameterI("BANFF.DoOnlyNueSelections");
    bool DoMultiPiRHC = (bool)ND::params().GetParameterI("BANFF.DoMultiPiRHC");

    if(!Do4PiFHC && !DoOnlyNue){
      std::cout << "---------------------------------" << std::endl;
      std::cout << "Applying FHC CCMultiPI selections" << std::endl;
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

    }else if(!DoOnlyNue){
      std::cout << "-----------------------------" << std::endl;
      std::cout << "Applying CC4Pi FHC selections" << std::endl;
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
      
    }
										 
    if(DoNue){
      std::cout << "-----------------------" << std::endl;
      std::cout << "Applying nue selections" << std::endl;
      std::cout << "-----------------------" << std::endl;

      SampleMap[SampleId::kFGD1NuECC               ] = new BANFFBinnedSample(SampleId::kFGD1NuECC,
                                                                             2, observables, throwMCStat, throwStat);
      SampleMap[SampleId::kFGD1AntiNuEBkgInNuModeCC] = new BANFFBinnedSample(SampleId::kFGD1AntiNuEBkgInNuModeCC,
                                                                             2, observables, throwMCStat, throwStat);
      SampleMap[SampleId::kFGD1Gamma               ] = new BANFFBinnedSample(SampleId::kFGD1Gamma,
                                                                             2, observables, throwMCStat, throwStat);
      SampleMap[SampleId::kFGD2NuECC               ] = new BANFFBinnedSample(SampleId::kFGD2NuECC,
                                                                             2, observables, throwMCStat, throwStat);
      SampleMap[SampleId::kFGD2AntiNuEBkgInNuModeCC] = new BANFFBinnedSample(SampleId::kFGD2AntiNuEBkgInNuModeCC,
                                                                             2, observables, throwMCStat, throwStat);
      SampleMap[SampleId::kFGD2Gamma               ] = new BANFFBinnedSample(SampleId::kFGD2Gamma,
                                                                             2, observables, throwMCStat, throwStat);
      SampleMap[SampleId::kFGD1NuEBkgInAntiNuModeCC] = new BANFFBinnedSample(SampleId::kFGD1NuEBkgInAntiNuModeCC,
                                                                             2, observables, throwMCStat, throwStat);
      SampleMap[SampleId::kFGD1AntiNuECC           ] = new BANFFBinnedSample(SampleId::kFGD1AntiNuECC,
                                                                             2, observables, throwMCStat, throwStat);
      SampleMap[SampleId::kFGD1GammaInAntiNuMode   ] = new BANFFBinnedSample(SampleId::kFGD1GammaInAntiNuMode,
                                                                             2, observables, throwMCStat, throwStat);
      SampleMap[SampleId::kFGD2NuEBkgInAntiNuModeCC] = new BANFFBinnedSample(SampleId::kFGD2NuEBkgInAntiNuModeCC,
                                                                             2, observables, throwMCStat, throwStat);
      SampleMap[SampleId::kFGD2AntiNuECC           ] = new BANFFBinnedSample(SampleId::kFGD2AntiNuECC,
                                                                             2, observables, throwMCStat, throwStat);
      SampleMap[SampleId::kFGD2GammaInAntiNuMode   ] = new BANFFBinnedSample(SampleId::kFGD2GammaInAntiNuMode,
                                                                             2, observables, throwMCStat, throwStat);

      nSample=nSample+12;
      
      // Some idiot proof check...
      if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNuECC")         == 0 ||
	 ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNuECC")     == 0 ||
	 ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerGamma")         == 0 ||
	 ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNuECCFGD2")     == 0 ||
	 ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNuECCFGD2") == 0 || 
 	 ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerGammaFGD2")     == 0
	 ){
	std::cout << "Nue not activated in psycheSteering!" << std::endl;
	throw;
      }
    }else{
      // Some idiot proof check...
      if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNuECC")         == 1 ||
	 ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNuECC")     == 1 ||
	 ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerGamma")         == 1 ||
	 ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNuECCFGD2")     == 1 ||
	 ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNuECCFGD2") == 1 ||
	 ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerGammaFGD2")     == 1
	 ){
	std::cout << "Nue is activated in psycheSteering!" << std::endl;
	throw;
      }
    }
    if(!DoOnlyNue){
      if(DoMultiPiRHC){
	std::cout << "------------------------------" << std::endl;
	std::cout << "Applying RHCMultiPi selections" << std::endl;
	std::cout << "------------------------------" << std::endl;
	
	SampleMap[SampleId::kFGD1NuMuBkgInAntiNuModeCC0Pi   ] = new BANFFBinnedSample(SampleId::kFGD1NuMuBkgInAntiNuModeCC0Pi,
                                                                                      2, observables, throwMCStat, throwStat);
	SampleMap[SampleId::kFGD1NuMuBkgInAntiNuModeCC1Pi   ] = new BANFFBinnedSample(SampleId::kFGD1NuMuBkgInAntiNuModeCC1Pi,
                                                                                      2, observables, throwMCStat, throwStat);
	SampleMap[SampleId::kFGD1NuMuBkgInAntiNuModeCCOther ] = new BANFFBinnedSample(SampleId::kFGD1NuMuBkgInAntiNuModeCCOther,
                                                                                      2, observables, throwMCStat, throwStat);
        SampleMap[SampleId::kFGD1AntiNuMuCC0Pi              ] = new BANFFBinnedSample(SampleId::kFGD1AntiNuMuCC0Pi,
                                                                                      2, observables, throwMCStat, throwStat);
	SampleMap[SampleId::kFGD1AntiNuMuCC1Pi              ] = new BANFFBinnedSample(SampleId::kFGD1AntiNuMuCC1Pi,
                                                                                      2, observables, throwMCStat, throwStat);
	SampleMap[SampleId::kFGD1AntiNuMuCCOther            ] = new BANFFBinnedSample(SampleId::kFGD1AntiNuMuCCOther,
                                                                                      2, observables, throwMCStat, throwStat);
        SampleMap[SampleId::kFGD2NuMuBkgInAntiNuModeCC0Pi   ] = new BANFFBinnedSample(SampleId::kFGD2NuMuBkgInAntiNuModeCC0Pi,
                                                                                      2, observables, throwMCStat, throwStat);
	SampleMap[SampleId::kFGD2NuMuBkgInAntiNuModeCC1Pi   ] = new BANFFBinnedSample(SampleId::kFGD2NuMuBkgInAntiNuModeCC1Pi,
                                                                                      2, observables, throwMCStat, throwStat);
	SampleMap[SampleId::kFGD2NuMuBkgInAntiNuModeCCOther ] = new BANFFBinnedSample(SampleId::kFGD2NuMuBkgInAntiNuModeCCOther,
                                                                                      2, observables, throwMCStat, throwStat);
        SampleMap[SampleId::kFGD2AntiNuMuCC0Pi              ] = new BANFFBinnedSample(SampleId::kFGD2AntiNuMuCC0Pi,
                                                                                      2, observables, throwMCStat, throwStat);
	SampleMap[SampleId::kFGD2AntiNuMuCC1Pi              ] = new BANFFBinnedSample(SampleId::kFGD2AntiNuMuCC1Pi,
                                                                                      2, observables, throwMCStat, throwStat);
	SampleMap[SampleId::kFGD2AntiNuMuCCOther            ] = new BANFFBinnedSample(SampleId::kFGD2AntiNuMuCCOther,
                                                                                      2, observables, throwMCStat, throwStat);
	
	nSample=nSample+12;
	
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
      }else{
	std::cout << "--------------------------------" << std::endl;
	std::cout << "Applying RHCMultiTrack selection" << std::endl;
	std::cout << "--------------------------------" << std::endl;
	
	SampleMap[SampleId::kFGD1AntiNuMuCC1Track            ] = new BANFFBinnedSample(SampleId::kFGD1AntiNuMuCC1Track,
                                                                                       2, observables, throwMCStat, throwStat);
	SampleMap[SampleId::kFGD1AntiNuMuCCNTracks           ] = new BANFFBinnedSample(SampleId::kFGD1AntiNuMuCCNTracks,
                                                                                       2, observables, throwMCStat, throwStat);
	SampleMap[SampleId::kFGD1NuMuBkgInAntiNuModeCC1Track ] = new BANFFBinnedSample(SampleId::kFGD1NuMuBkgInAntiNuModeCC1Track,
                                                                                       2, observables, throwMCStat, throwStat);
	SampleMap[SampleId::kFGD1NuMuBkgInAntiNuModeCCNTracks] = new BANFFBinnedSample(SampleId::kFGD1NuMuBkgInAntiNuModeCCNTracks,
                                                                                       2, observables, throwMCStat, throwStat);
	SampleMap[SampleId::kFGD2AntiNuMuCC1Track            ] = new BANFFBinnedSample(SampleId::kFGD2AntiNuMuCC1Track,
                                                                                       2, observables, throwMCStat, throwStat);
	SampleMap[SampleId::kFGD2AntiNuMuCCNTracks           ] = new BANFFBinnedSample(SampleId::kFGD2AntiNuMuCCNTracks,
                                                                                       2, observables, throwMCStat, throwStat);
	SampleMap[SampleId::kFGD2NuMuBkgInAntiNuModeCC1Track ] = new BANFFBinnedSample(SampleId::kFGD2NuMuBkgInAntiNuModeCC1Track,
                                                                                       2, observables, throwMCStat, throwStat);
	SampleMap[SampleId::kFGD2NuMuBkgInAntiNuModeCCNTracks] = new BANFFBinnedSample(SampleId::kFGD2NuMuBkgInAntiNuModeCCNTracks,
                                                                                       2, observables, throwMCStat, throwStat);
	
	nSample=nSample+8;
	
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
      }
    }else{ // ONLY NUE SELECTIONS
      nSample=12;
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
    }

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

    //Load the XSec Parameters and specify not to vary them.
    if(ND::params().GetParameterI("BANFF.ThrowToys.LoadXSecParams")){
        fitParameters->LoadXSecParametersFromFile(ND::params().GetParameterS("BANFF.XSecInputFile"),
                                                  ND::params().GetParameterI("BANFF.ThrowToys.ThrowXSecParams"));
    }

    //The ability to load observable normalization parameters if so desired,
    //likely for validation purposes.
    if(ND::params().GetParameterI("BANFF.ThrowToys.LoadObsNormParams")){
        fitParameters->LoadObsNormParametersFromFile(ND::params().GetParameterS("BANFF.ObsNormInputFile"), nSample, samples,
                                                     ND::params().GetParameterI("BANFF.ThrowToys.ThrowObsNormParams"));
    }

    //With the samples, observables, and fit parameters specified, create the
    //interface, which will load in the ND280 events and store everything.
    //Usage:
    //Number of samples.
    //Array of sample pointers.
    //FitParameters pointer.
    //Whether to load the detector parameters from psyche.
    //Whether to vary the psyche detector parameters.
    psycheInterface* interface = new psycheInterface(nSample, samples, fitParameters,
                                                     ND::params().GetParameterI("BANFF.ThrowToys.LoadDetParams"),
                                                     ND::params().GetParameterI("BANFF.ThrowToys.ThrowDetParams"));


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

    timeval tim;
    gettimeofday(&tim, NULL);
    double t0 = tim.tv_sec + (tim.tv_usec/1000000.0);
    Minuit2LikelihoodFit* fitter = new Minuit2LikelihoodFit(interface, ND::params().GetParameterS("BANFF.ThrowToys.OutputFile"));

    double* sigmaVariations = new double[5];
    sigmaVariations[0] = -3.0;
    sigmaVariations[1] = -1.0;
    sigmaVariations[2] =  0.0;
    sigmaVariations[3] =  1.0;
    sigmaVariations[4] =  3.0;
    interface->VaryParametersByNSigma(5,sigmaVariations,
                                      ND::params().GetParameterS("BANFF.ThrowToys.OutputFile"));

    gettimeofday(&tim, NULL);
    double t1 = tim.tv_sec + (tim.tv_usec/1000000.0);
    int nToys = 5*(fitParameters->GetNumbSysts());
    std::cout << "Time to throw toys: " << (t1 - t0) << std::endl;
    std::cout << "Time per toy: " << (t1 - t0)/((double)nToys) << std::endl;

    return 0;
}
