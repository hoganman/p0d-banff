#include "SampleId.hxx"


//*********************************************************
std::string SampleId::ConvertSample(SampleEnum sample){
//*********************************************************

    std::string ssample="";

    if   (sample == kUnassigned)                          ssample = "Unassigned";
    else if (sample == kFGD1NuMuCC)                       ssample = "FGD1 numuCC inclusive";
    else if (sample == kFGD1NuMuCCQE)                     ssample = "FGD1 numuCC QE";
    else if (sample == kFGD1NuMuCC0Pi)                    ssample = "FGD1 numuCC 0pi";
    else if (sample == kFGD1NuMuCC1Pi)                    ssample = "FGD1 numuCC 1pi";
    else if (sample == kFGD1NuMuCCOther)                  ssample = "FGD1 numuCC other";
    
    else if (sample == kFGD2NuMuCC)                       ssample = "FGD2 numuCC inclusive";
    else if (sample == kFGD2NuMuCC0Pi)                    ssample = "FGD2 numuCC 0pi";
    else if (sample == kFGD2NuMuCC1Pi)                    ssample = "FGD2 numuCC 1pi";
    else if (sample == kFGD2NuMuCCOther)                  ssample = "FGD2 numuCC other";
    
    else if (sample == kFGD1NuECC)                        ssample = "FGD1 nueCC inclusive";
    else if (sample == kFGD1NuECC0Pi)                     ssample = "FGD1 nueCC 0pi";
    else if (sample == kFGD1NuECCOther)                   ssample = "FGD1 nueCC other";
    
    else if (sample == kFGD2NuECC)                        ssample = "FGD2 nueCC inclusive";
    else if (sample == kFGD2NuECC0Pi)                     ssample = "FGD2 nueCC 0pi";
    else if (sample == kFGD2NuECCOther)                   ssample = "FGD2 nueCC other";
    
    else if (sample == kFGD1AntiNuMuCC)                   ssample = "FGD1 anti-numuCC inclusive";
    else if (sample == kFGD1AntiNuMuCC1Track)             ssample = "FGD1 anti-numuCC QE";
    else if (sample == kFGD1AntiNuMuCCNTracks)            ssample = "FGD1 anti-numuCC nQE";
    else if (sample == kFGD1AntiNuMuCC0Pi)                ssample = "FGD1 anti-numuCC 0pi";
    else if (sample == kFGD1AntiNuMuCC1Pi)                ssample = "FGD1 anti-numuCC 1pi";
    else if (sample == kFGD1AntiNuMuCCOther)              ssample = "FGD1 anti-numuCC other";
    
    else if (sample == kFGD2AntiNuMuCC)                   ssample = "FGD2 anti-numuCC inclusive";
    else if (sample == kFGD2AntiNuMuCC1Track)             ssample = "FGD2 anti-numuCC 1 track";
    else if (sample == kFGD2AntiNuMuCCNTracks)            ssample = "FGD2 anti-numuCC N tracks";
    else if (sample == kFGD2AntiNuMuCC0Pi)                ssample = "FGD2 anti-numuCC 0pi";
    else if (sample == kFGD2AntiNuMuCC1Pi)                ssample = "FGD2 anti-numuCC 1pi";
    else if (sample == kFGD2AntiNuMuCCOther)              ssample = "FGD2 anti-numuCC other";
    
    else if (sample == kFGD1NuMuBkgInAntiNuModeCC)        ssample = "FGD1 NuMuBkg CC inclusive in AntiNu Mode";
    else if (sample == kFGD1NuMuBkgInAntiNuModeCC1Track)  ssample = "FGD1 NuMuBkg CCQE in AntiNu Mode ";
    else if (sample == kFGD1NuMuBkgInAntiNuModeCCNTracks) ssample = "FGD1 NuMuBkg CCnQE in AntiNu Mode";
    else if (sample == kFGD1NuMuBkgInAntiNuModeCC0Pi)     ssample = "FGD1 NuMuBkg CC0pi in AntiNu Mode";
    else if (sample == kFGD1NuMuBkgInAntiNuModeCC1Pi)     ssample = "FGD1 NuMuBkg CC1pi in AntiNu Mode";
    else if (sample == kFGD1NuMuBkgInAntiNuModeCCOther)   ssample = "FGD1 NuMuBkg CCother in AntiNu Mode";
    
    else if (sample == kFGD2NuMuBkgInAntiNuModeCC)        ssample = "FGD2 NuMuBkg CC inclusive in AntiNu Mode";
    else if (sample == kFGD2NuMuBkgInAntiNuModeCC1Track)  ssample = "FGD2 NuMuBkg CCQE in AntiNu Mode ";
    else if (sample == kFGD2NuMuBkgInAntiNuModeCCNTracks) ssample = "FGD2 NuMuBkg CCnQE in AntiNu Mode";
    else if (sample == kFGD2NuMuBkgInAntiNuModeCC0Pi)     ssample = "FGD2 NuMuBkg CC0pi in AntiNu Mode";
    else if (sample == kFGD2NuMuBkgInAntiNuModeCC1Pi)     ssample = "FGD2 NuMuBkg CC1pi in AntiNu Mode";
    else if (sample == kFGD2NuMuBkgInAntiNuModeCCOther)   ssample = "FGD2 NuMuBkg CCother in AntiNu Mode";
    
    else if (sample == kFGD1AntiNuECC)                    ssample = "FGD1 anti-nueCC inclusive";
    else if (sample == kFGD1AntiNuECC0Pi)                 ssample = "FGD1 anti-nueCC 0pi";
    else if (sample == kFGD1AntiNuECCOther)               ssample = "FGD1 anti-nueCC other";
    
    else if (sample == kFGD2AntiNuECC)                    ssample = "FGD2 anti-nueCC inclusive";
    else if (sample == kFGD2AntiNuECC0Pi)                 ssample = "FGD2 anti-nueCC 0pi";
    else if (sample == kFGD2AntiNuECCOther)               ssample = "FGD2 anti-nueCC other";
    
    else if (sample == kFGD1Gamma)                        ssample = "FGD1 gamma";
    
    else if (sample == kFGD2Gamma)                        ssample = "FGD2 gamma";

    else if (sample == kP0DNuMuCC)                        ssample = "P0D+TPC1 NuMuCC inclusive";

    else                                                  ssample = "Unassigned";

    return ssample;
}


//*********************************************************
std::string SampleId::ConvertSampleToSelection(SampleEnum sample){
//*********************************************************

    //These should be the same as what is used to initialize selection names in 
    //AnalysisManager. Some of them are made up right now. -ACK

    std::string ssample="";

    if      (sample == kUnassigned)                       ssample = "Unassigned";
    else if (sample == kFGD1NuMuCC)                       ssample = "kTrackerNumuCC";
    else if (sample == kFGD1NuMuCCQE)                     ssample = "kTrackerNumuCCQE";
    else if (sample == kFGD1NuMuCC0Pi)                    ssample = "kTrackerNumuCCMultiPi";
    else if (sample == kFGD1NuMuCC1Pi)                    ssample = "kTrackerNumuCCMultiPi";
    else if (sample == kFGD1NuMuCCOther)                  ssample = "kTrackerNumuCCMultiPi";
    else if (sample == kFGD2NuMuCC)                       ssample = "kTrackerNumuCCFGD2";
    else if (sample == kFGD2NuMuCC0Pi)                    ssample = "kTrackerNumuCCMultiPiFGD2";
    else if (sample == kFGD2NuMuCC1Pi)                    ssample = "kTrackerNumuCCMultiPiFGD2";
    else if (sample == kFGD2NuMuCCOther)                  ssample = "kTrackerNumuCCMultiPiFGD2";
    else if (sample == kFGD1NuECC)                        ssample = "kTrackerNueCC";
    else if (sample == kFGD1NuECC0Pi)                     ssample = "kTrackerNueCCMultiPi";
    else if (sample == kFGD1NuECCOther)                   ssample = "kTrackerNueCCMultiPi";
    else if (sample == kFGD2NuECC)                        ssample = "kTrackerNueCCFGD2";
    else if (sample == kFGD2NuECC0Pi)                     ssample = "kTrackerNueCCMultiPiFGD2";
    else if (sample == kFGD2NuECCOther)                   ssample = "kTrackerNueCCMultiPiFGD2";
    
    else if (sample == kFGD1AntiNuMuCC)                   ssample = "kTrackerAntiNumuCC";
    
    // To be renamed
    else if (sample == kFGD1AntiNuMuCC1Track)             ssample = "kTrackerAntiNumuCCMultiTrack";
    else if (sample == kFGD1AntiNuMuCCNTracks)            ssample = "kTrackerAntiNumuCCMultiTrack";
    
    else if (sample == kFGD1AntiNuMuCC0Pi)                ssample = "kTrackerAntiNumuCCMultiPi";
    else if (sample == kFGD1AntiNuMuCC1Pi)                ssample = "kTrackerAntiNumuCCMultiPi";
    else if (sample == kFGD1AntiNuMuCCOther)              ssample = "kTrackerAntiNumuCCMultiPi";
    
    
    // To be renamed
    else if (sample == kFGD2AntiNuMuCC)                   ssample = "kTrackerAntiNumuCCFGD2";
    else if (sample == kFGD2AntiNuMuCC1Track)             ssample = "kTrackerAntiNumuCCMultiTrackFGD2";
    else if (sample == kFGD2AntiNuMuCCNTracks)            ssample = "kTrackerAntiNumuCCMultiTrackFGD2";
    
    else if (sample == kFGD2AntiNuMuCC0Pi)                ssample = "kTrackerAntiNumuCCMultiPiFGD2"; 
    else if (sample == kFGD2AntiNuMuCC1Pi)                ssample = "kTrackerAntiNumuCCMultiPiFGD2";
    else if (sample == kFGD2AntiNuMuCCOther)              ssample = "kTrackerAntiNumuCCMultiPiFGD2";
    
    else if (sample == kFGD1NuMuBkgInAntiNuModeCC)        ssample = "kTrackerNumuInAntiNuModeCC";
    else if (sample == kFGD1NuMuBkgInAntiNuModeCC1Track)  ssample = "kTrackerNumuInAntiNuModeCCMultiTrack";
    else if (sample == kFGD1NuMuBkgInAntiNuModeCCNTracks) ssample = "kTrackerNumuInAntiNuModeCCMultiTrack";
    
    else if (sample == kFGD1NuMuBkgInAntiNuModeCC0Pi)     ssample = "kTrackerNumuInAntiNuModeCCMultiPi"; 
    else if (sample == kFGD1NuMuBkgInAntiNuModeCC1Pi)     ssample = "kTrackerNumuInAntiNuModeCCMultiPi";
    else if (sample == kFGD1NuMuBkgInAntiNuModeCCOther)   ssample = "kTrackerNumuInAntiNuModeCCMultiPi";
    
    else if (sample == kFGD2NuMuBkgInAntiNuModeCC)        ssample = "kTrackerNumuInAntiNuModeCCFGD2";
    else if (sample == kFGD2NuMuBkgInAntiNuModeCC1Track)  ssample = "kTrackerNumuInAntiNuModeCCMultiTrackFGD2";
    else if (sample == kFGD2NuMuBkgInAntiNuModeCCNTracks) ssample = "kTrackerNumuInAntiNuModeCCMultiTrackFGD2";
  
    else if (sample == kFGD2NuMuBkgInAntiNuModeCC0Pi)     ssample = "kTrackerNumuInAntiNuModeCCMultiPiFGD2"; 
    else if (sample == kFGD2NuMuBkgInAntiNuModeCC1Pi)     ssample = "kTrackerNumuInAntiNuModeCCMultiPiFGD2";
    else if (sample == kFGD2NuMuBkgInAntiNuModeCCOther)   ssample = "kTrackerNumuInAntiNuModeCCMultiPiFGD2";
    
    
    else if (sample == kFGD1AntiNuECC)                    ssample = "kTrackerAntiNueCC";
    else if (sample == kFGD1AntiNuECC0Pi)                 ssample = "kTrackerAntiNueCCMultiPi";
    else if (sample == kFGD1AntiNuECCOther)               ssample = "kTrackerAntiNueCCMultiPi";
    
    else if (sample == kFGD2AntiNuECC)                    ssample = "kTrackerAntiNueCCFGD2";
    else if (sample == kFGD2AntiNuECC0Pi)                 ssample = "kTrackerAntiNueCCMultiPiFGD2";
    else if (sample == kFGD2AntiNuECCOther)               ssample = "kTrackerAntiNueCCMultiPiFGD2";
    
    else if (sample == kFGD1Gamma)                        ssample = "kTrackerGamma";
    else if (sample == kFGD2Gamma)                        ssample = "kTrackerGammaFGD2";

    else if (sample == kP0DNuMuCC)                        ssample = "kP0DNuMuCCInclusive";
    else                                                  ssample = "Unassigned";

    return ssample;
}
