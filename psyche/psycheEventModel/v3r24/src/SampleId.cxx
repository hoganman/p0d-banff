#include "SampleId.hxx"
#include <cstring>

//*********************************************************
std::string SampleId::ConvertSample(const SampleEnum& sample){
//*********************************************************

    std::string ssample="";

    if      (sample == kUnassigned)                       ssample = "Unassigned";
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

    else if (sample == kFGD1Gamma)                        ssample = "FGD1 gamma FHC";
    else if (sample == kFGD2Gamma)                        ssample = "FGD2 gamma FHC";

    else if (sample == kFGD1GammaInAntiNuMode)            ssample = "FGD1 gamma RHC";
    else if (sample == kFGD2GammaInAntiNuMode)            ssample = "FGD2 gamma RHC";
    else if (sample == kFGD1AntiNuEBkgInNuModeCC)         ssample = "FGD1 Anti-NuE Bkg In NuMode CC";
    else if (sample == kFGD1AntiNuEBkgInNuModeCC0Pi)      ssample = "FGD1 Anti-NuE Bkg In NuMode CC0Pi";
    else if (sample == kFGD1AntiNuEBkgInNuModeCCOther)    ssample = "FGD1 Anti-NuE Bkg In NuMode CCOther";
    else if (sample == kFGD2AntiNuEBkgInNuModeCC)         ssample = "FGD2 Anti-NuE Bkg In NuMode CC";
    else if (sample == kFGD2AntiNuEBkgInNuModeCC0Pi)      ssample = "FGD2 Anti-NuE Bkg In NuMode CC0Pi";
    else if (sample == kFGD2AntiNuEBkgInNuModeCCOther)    ssample = "FGD2 Anti-NuE Bkg In NuMode CCOther";
    else if (sample == kFGD1NuEBkgInAntiNuModeCC)         ssample = "FGD1 NuE Bkg In Anti NuMode CC";
    else if (sample == kFGD1NuEBkgInAntiNuModeCC0Pi)      ssample = "FGD1 NuE Bkg In Anti NuMode CC0Pi";
    else if (sample == kFGD1NuEBkgInAntiNuModeCCOther)    ssample = "FGD1 NuE Bkg In Anti NuMode CCOther";
    else if (sample == kFGD2NuEBkgInAntiNuModeCC)         ssample = "FGD2 NuE Bkg In Anti NuMode CC";
    else if (sample == kFGD2NuEBkgInAntiNuModeCC0Pi)      ssample = "FGD2 NuE Bkg In Anti NuMode CC0Pi";
    else if (sample == kFGD2NuEBkgInAntiNuModeCCOther)    ssample = "FGD2 NuE Bkg In Anti NuMode CCOther";

    else if (sample == kP0DNuMuCC)                            ssample = "P0D NuMu CC Inclusive";

    else if (sample == kP0DWaterNuMuCC)                       ssample = "P0D Water NuMu CC Inclusive";
    else if (sample == kP0DWaterNuMuCC1Track)                 ssample = "P0D Water NuMu CC1Track";
    else if (sample == kP0DWaterNuMuCCNTracks)                ssample = "P0D Water NuMu CCNTracks";

    else if (sample == kP0DWaterNuMuBarBkgCC)                 ssample = "P0D Water NuMubar Bkg CC Inclusive";

    else if (sample == kP0DWaterNuMuBkgInAntiNuModeCC)        ssample = "P0D Water NuMu Bkg in Anti NuMode CC Inclusive";
    else if (sample == kP0DWaterNuMuBkgInAntiNuModeCC1Track)  ssample = "P0D Water NuMu Bkg in Anti NuMode CC1Track" ;
    else if (sample == kP0DWaterNuMuBkgInAntiNuModeCCNTracks) ssample = "P0D Water NuMu Bkg in Anti NuMode CCNTracks";

    else if (sample == kP0DWaterNuMuBarInAntiNuModeCC)        ssample = "P0D Water NuMubar in Anti NuMode CC Inclusive";
    else if (sample == kP0DWaterNuMuBarInAntiNuModeCC1Track)  ssample = "P0D Water NuMubar in Anti NuMode CC1Track" ;
    else if (sample == kP0DWaterNuMuBarInAntiNuModeCCNTracks) ssample = "P0D Water NuMubar in Anti NuMode CCNTracks";

    else if (sample == kP0DAirNuMuCC)                         ssample = "P0D Air NuMu CC Inclusive";
    else if (sample == kP0DAirNuMuCC1Track)                   ssample = "P0D Air NuMu CC CC1Track" ;
    else if (sample == kP0DAirNuMuCCNTracks)                  ssample = "P0D Air NuMu CC CCNTracks";

    else if (sample == kP0DAirNuMuBarBkgCC)                   ssample = "P0D Air NuMubar Bkg CC Inclusive";

    else if (sample == kP0DAirNuMuBkgInAntiNuModeCC)          ssample = "P0D Air NuMu Bkg in Anti NuMode CC Inclusive";
    else if (sample == kP0DAirNuMuBkgInAntiNuModeCC1Track)    ssample = "P0D Air NuMu Bkg in Anti NuMode CC CC1Track" ;
    else if (sample == kP0DAirNuMuBkgInAntiNuModeCCNTracks)   ssample = "P0D Air NuMu Bkg in Anti NuMode CC CCNTracks";

    else if (sample == kP0DAirNuMuBarInAntiNuModeCC)          ssample = "P0D Air NuMubar in Anti NuMode CC Inclusive";
    else if (sample == kP0DAirNuMuBarInAntiNuModeCC1Track)    ssample = "P0D Air NuMubar in Anti NuMode CC CC1Track" ;
    else if (sample == kP0DAirNuMuBarInAntiNuModeCCNTracks)   ssample = "P0D Air NuMubar in Anti NuMode CC CCNTracks";

    else                                                      ssample = "Unassigned";

    return ssample;
}


//*********************************************************
std::string SampleId::ConvertSampleToSelection(const SampleEnum& sample){
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

    else if (sample == kP0DWaterNuMuCC)                       ssample = "kNuMuCCP0DWater";
    else if (sample == kP0DWaterNuMuCC1Track)                 ssample = "kNuMuCCMultiTrackP0DWater";
    else if (sample == kP0DWaterNuMuCCNTracks)                ssample = "kNuMuCCMultiTrackP0DWater";

    else if (sample == kP0DWaterNuMuBarBkgCC)                 ssample = "kNuMubarBkgCCP0DWater";

    else if (sample == kP0DWaterNuMuBkgInAntiNuModeCC)        ssample = "kNuMuBkgInAntiNuModeCCP0DWater";
    else if (sample == kP0DWaterNuMuBkgInAntiNuModeCC1Track)  ssample = "kNuMuBkgInAntiNuModeCCMultiTrackP0DWater";
    else if (sample == kP0DWaterNuMuBkgInAntiNuModeCCNTracks) ssample = "kNuMuBkgInAntiNuModeCCMultiTrackP0DWater";

    else if (sample == kP0DWaterNuMuBarInAntiNuModeCC)        ssample = "kNuMubarInAntiNuModeCCP0DWater";
    else if (sample == kP0DWaterNuMuBarInAntiNuModeCC1Track)  ssample = "kNuMubarInAntiNuModeCCMultiTrackP0DWater";
    else if (sample == kP0DWaterNuMuBarInAntiNuModeCCNTracks) ssample = "kNuMubarInAntiNuModeCCMultiTrackP0DWater";

    else if (sample == kP0DAirNuMuCC)                         ssample = "kNuMuCCP0DAir";
    else if (sample == kP0DAirNuMuCC1Track)                   ssample = "kNuMuCCP0DMultiTrackAir";
    else if (sample == kP0DAirNuMuCCNTracks)                  ssample = "kNuMuCCP0DMultiTrackAir";

    else if (sample == kP0DAirNuMuBarBkgCC)                   ssample = "kNuMubarBkgCCP0DAir";

    else if (sample == kP0DAirNuMuBkgInAntiNuModeCC)          ssample = "kNuMuBkgInAntiNuModeCCP0DAir";
    else if (sample == kP0DAirNuMuBkgInAntiNuModeCC1Track)    ssample = "kNuMuBkgInAntiNuModeCCMultiTrackP0DAir";
    else if (sample == kP0DAirNuMuBkgInAntiNuModeCCNTracks)   ssample = "kNuMuBkgInAntiNuModeCCMultiTrackP0DAir";

    else if (sample == kP0DAirNuMuBarInAntiNuModeCC)          ssample = "kNuMubarInAntiNuModeCCP0DAir";
    else if (sample == kP0DAirNuMuBarInAntiNuModeCC1Track)    ssample = "kNuMubarInAntiNuModeCCMultiTrackP0DAir";
    else if (sample == kP0DAirNuMuBarInAntiNuModeCCNTracks)   ssample = "kNuMubarInAntiNuModeCCMultiTrackP0DAir";

    else                                                  ssample = "Unassigned";

    return ssample;
}

//*********************************************************
SampleId::SampleEnum SampleId::ConvertSample(const std::string& sample)
//*********************************************************
{
    SampleEnum enum_sample = kUnassigned;
    if( strcmp("kFGD1NuMuCC",sample.c_str()) == 0) enum_sample = kFGD1NuMuCC;
    else if( strcmp("kFGD1NuMuCCQE",sample.c_str()) == 0) enum_sample = kFGD1NuMuCCQE;
    else if( strcmp("kFGD1NuMuCC0Pi",sample.c_str()) == 0) enum_sample = kFGD1NuMuCC0Pi;
    else if( strcmp("kFGD1NuMuCC1Pi",sample.c_str()) == 0) enum_sample = kFGD1NuMuCC1Pi;
    else if( strcmp("kFGD1NuMuCCOther",sample.c_str()) == 0) enum_sample = kFGD1NuMuCCOther;
    else if( strcmp("kFGD1NuMuCC0PiFwd",sample.c_str()) == 0) enum_sample = kFGD1NuMuCC0PiFwd;
    else if( strcmp("kFGD1NuMuCC1PiFwd",sample.c_str()) == 0) enum_sample = kFGD1NuMuCC1PiFwd;
    else if( strcmp("kFGD1NuMuCCOtherFwd",sample.c_str()) == 0) enum_sample = kFGD1NuMuCCOtherFwd;
    else if( strcmp("kFGD1NuMuCC0PiBwd",sample.c_str()) == 0) enum_sample = kFGD1NuMuCC0PiBwd;
    else if( strcmp("kFGD1NuMuCC1PiBwd",sample.c_str()) == 0) enum_sample = kFGD1NuMuCC1PiBwd;
    else if( strcmp("kFGD1NuMuCCOtherBwd",sample.c_str()) == 0) enum_sample = kFGD1NuMuCCOtherBwd;
    else if( strcmp("kFGD1NuMuCC0PiHAFwd",sample.c_str()) == 0) enum_sample = kFGD1NuMuCC0PiHAFwd;
    else if( strcmp("kFGD1NuMuCC1PiHAFwd",sample.c_str()) == 0) enum_sample = kFGD1NuMuCC1PiHAFwd;
    else if( strcmp("kFGD1NuMuCCOtherHAFwd",sample.c_str()) == 0) enum_sample = kFGD1NuMuCCOtherHAFwd;
    else if( strcmp("kFGD1NuMuCC0PiHABwd",sample.c_str()) == 0) enum_sample = kFGD1NuMuCC0PiHABwd;
    else if( strcmp("kFGD1NuMuCC1PiHABwd",sample.c_str()) == 0) enum_sample = kFGD1NuMuCC1PiHABwd;
    else if( strcmp("kFGD1NuMuCCOtherHABwd",sample.c_str()) == 0) enum_sample = kFGD1NuMuCCOtherHABwd;
    else if( strcmp("kFGD2NuMuCC",sample.c_str()) == 0) enum_sample = kFGD2NuMuCC;
    else if( strcmp("kFGD2NuMuCC0Pi",sample.c_str()) == 0) enum_sample = kFGD2NuMuCC0Pi;
    else if( strcmp("kFGD2NuMuCC1Pi",sample.c_str()) == 0) enum_sample = kFGD2NuMuCC1Pi;
    else if( strcmp("kFGD2NuMuCCOther",sample.c_str()) == 0) enum_sample = kFGD2NuMuCCOther;
    else if( strcmp("kFGD2NuMuCC0PiFwd",sample.c_str()) == 0) enum_sample = kFGD2NuMuCC0PiFwd;
    else if( strcmp("kFGD2NuMuCC1PiFwd",sample.c_str()) == 0) enum_sample = kFGD2NuMuCC1PiFwd;
    else if( strcmp("kFGD2NuMuCCOtherFwd",sample.c_str()) == 0) enum_sample = kFGD2NuMuCCOtherFwd;
    else if( strcmp("kFGD2NuMuCC0PiBwd",sample.c_str()) == 0) enum_sample = kFGD2NuMuCC0PiBwd;
    else if( strcmp("kFGD2NuMuCC1PiBwd",sample.c_str()) == 0) enum_sample = kFGD2NuMuCC1PiBwd;
    else if( strcmp("kFGD2NuMuCCOtherBwd",sample.c_str()) == 0) enum_sample = kFGD2NuMuCCOtherBwd;
    else if( strcmp("kFGD2NuMuCC0PiHAFwd",sample.c_str()) == 0) enum_sample = kFGD2NuMuCC0PiHAFwd;
    else if( strcmp("kFGD2NuMuCC1PiHAFwd",sample.c_str()) == 0) enum_sample = kFGD2NuMuCC1PiHAFwd;
    else if( strcmp("kFGD2NuMuCCOtherHAFwd",sample.c_str()) == 0) enum_sample = kFGD2NuMuCCOtherHAFwd;
    else if( strcmp("kFGD2NuMuCC0PiHABwd",sample.c_str()) == 0) enum_sample = kFGD2NuMuCC0PiHABwd;
    else if( strcmp("kFGD2NuMuCC1PiHABwd",sample.c_str()) == 0) enum_sample = kFGD2NuMuCC1PiHABwd;
    else if( strcmp("kFGD2NuMuCCOtherHABwd",sample.c_str()) == 0) enum_sample = kFGD2NuMuCCOtherHABwd;
    else if( strcmp("kFGD1NuECC",sample.c_str()) == 0) enum_sample = kFGD1NuECC;
    else if( strcmp("kFGD1NuECC0Pi",sample.c_str()) == 0) enum_sample = kFGD1NuECC0Pi;
    else if( strcmp("kFGD1NuECCOther",sample.c_str()) == 0) enum_sample = kFGD1NuECCOther;
    else if( strcmp("kFGD2NuECC",sample.c_str()) == 0) enum_sample = kFGD2NuECC;
    else if( strcmp("kFGD2NuECC0Pi",sample.c_str()) == 0) enum_sample = kFGD2NuECC0Pi;
    else if( strcmp("kFGD2NuECCOther",sample.c_str()) == 0) enum_sample = kFGD2NuECCOther;
    else if( strcmp("kFGD1AntiNuMuCC",sample.c_str()) == 0) enum_sample = kFGD1AntiNuMuCC;
    else if( strcmp("kFGD1AntiNuMuCC1Track",sample.c_str()) == 0) enum_sample = kFGD1AntiNuMuCC1Track;
    else if( strcmp("kFGD1AntiNuMuCCNTracks",sample.c_str()) == 0) enum_sample = kFGD1AntiNuMuCCNTracks;
    else if( strcmp("kFGD1AntiNuMuCC0Pi",sample.c_str()) == 0) enum_sample = kFGD1AntiNuMuCC0Pi;
    else if( strcmp("kFGD1AntiNuMuCC1Pi",sample.c_str()) == 0) enum_sample = kFGD1AntiNuMuCC1Pi;
    else if( strcmp("kFGD1AntiNuMuCCOther",sample.c_str()) == 0) enum_sample = kFGD1AntiNuMuCCOther;
    else if( strcmp("kFGD2AntiNuMuCC",sample.c_str()) == 0) enum_sample = kFGD2AntiNuMuCC;
    else if( strcmp("kFGD2AntiNuMuCC1Track",sample.c_str()) == 0) enum_sample = kFGD2AntiNuMuCC1Track;
    else if( strcmp("kFGD2AntiNuMuCCNTracks",sample.c_str()) == 0) enum_sample = kFGD2AntiNuMuCCNTracks;
    else if( strcmp("kFGD2AntiNuMuCC0Pi",sample.c_str()) == 0) enum_sample = kFGD2AntiNuMuCC0Pi;
    else if( strcmp("kFGD2AntiNuMuCC1Pi",sample.c_str()) == 0) enum_sample = kFGD2AntiNuMuCC1Pi;
    else if( strcmp("kFGD2AntiNuMuCCOther",sample.c_str()) == 0) enum_sample = kFGD2AntiNuMuCCOther;
    else if( strcmp("kFGD1NuMuBkgInAntiNuModeCC",sample.c_str()) == 0) enum_sample = kFGD1NuMuBkgInAntiNuModeCC;
    else if( strcmp("kFGD1NuMuBkgInAntiNuModeCC1Track",sample.c_str()) == 0) enum_sample = kFGD1NuMuBkgInAntiNuModeCC1Track;
    else if( strcmp("kFGD1NuMuBkgInAntiNuModeCCNTracks",sample.c_str()) == 0) enum_sample = kFGD1NuMuBkgInAntiNuModeCCNTracks;
    else if( strcmp("kFGD1NuMuBkgInAntiNuModeCC0Pi",sample.c_str()) == 0) enum_sample = kFGD1NuMuBkgInAntiNuModeCC0Pi;
    else if( strcmp("kFGD1NuMuBkgInAntiNuModeCC1Pi",sample.c_str()) == 0) enum_sample = kFGD1NuMuBkgInAntiNuModeCC1Pi;
    else if( strcmp("kFGD1NuMuBkgInAntiNuModeCCOther",sample.c_str()) == 0) enum_sample = kFGD1NuMuBkgInAntiNuModeCCOther;
    else if( strcmp("kFGD2NuMuBkgInAntiNuModeCC",sample.c_str()) == 0) enum_sample = kFGD2NuMuBkgInAntiNuModeCC;
    else if( strcmp("kFGD2NuMuBkgInAntiNuModeCC1Track",sample.c_str()) == 0) enum_sample = kFGD2NuMuBkgInAntiNuModeCC1Track;
    else if( strcmp("kFGD2NuMuBkgInAntiNuModeCCNTracks",sample.c_str()) == 0) enum_sample = kFGD2NuMuBkgInAntiNuModeCCNTracks;
    else if( strcmp("kFGD2NuMuBkgInAntiNuModeCC0Pi",sample.c_str()) == 0) enum_sample = kFGD2NuMuBkgInAntiNuModeCC0Pi;
    else if( strcmp("kFGD2NuMuBkgInAntiNuModeCC1Pi",sample.c_str()) == 0) enum_sample = kFGD2NuMuBkgInAntiNuModeCC1Pi;
    else if( strcmp("kFGD2NuMuBkgInAntiNuModeCCOther",sample.c_str()) == 0) enum_sample = kFGD2NuMuBkgInAntiNuModeCCOther;
    else if( strcmp("kFGD1NuMuCCBwd",sample.c_str()) == 0) enum_sample = kFGD1NuMuCCBwd;
    else if( strcmp("kFGD2NuMuCCBwd",sample.c_str()) == 0) enum_sample = kFGD2NuMuCCBwd;
    else if( strcmp("kFGD1AntiNuECC",sample.c_str()) == 0) enum_sample = kFGD1AntiNuECC;
    else if( strcmp("kFGD1AntiNuECC0Pi",sample.c_str()) == 0) enum_sample = kFGD1AntiNuECC0Pi;
    else if( strcmp("kFGD1AntiNuECCOther",sample.c_str()) == 0) enum_sample = kFGD1AntiNuECCOther;
    else if( strcmp("kFGD2AntiNuECC",sample.c_str()) == 0) enum_sample = kFGD2AntiNuECC;
    else if( strcmp("kFGD2AntiNuECC0Pi",sample.c_str()) == 0) enum_sample = kFGD2AntiNuECC0Pi;
    else if( strcmp("kFGD2AntiNuECCOther",sample.c_str()) == 0) enum_sample = kFGD2AntiNuECCOther;
    else if( strcmp("kFGD1AntiNuEBkgInNuModeCC",sample.c_str()) == 0) enum_sample = kFGD1AntiNuEBkgInNuModeCC;
    else if( strcmp("kFGD1AntiNuEBkgInNuModeCC0Pi",sample.c_str()) == 0) enum_sample = kFGD1AntiNuEBkgInNuModeCC0Pi;
    else if( strcmp("kFGD1AntiNuEBkgInNuModeCCOther",sample.c_str()) == 0) enum_sample = kFGD1AntiNuEBkgInNuModeCCOther;
    else if( strcmp("kFGD2AntiNuEBkgInNuModeCC",sample.c_str()) == 0) enum_sample = kFGD2AntiNuEBkgInNuModeCC;
    else if( strcmp("kFGD2AntiNuEBkgInNuModeCC0Pi",sample.c_str()) == 0) enum_sample = kFGD2AntiNuEBkgInNuModeCC0Pi;
    else if( strcmp("kFGD2AntiNuEBkgInNuModeCCOther",sample.c_str()) == 0) enum_sample = kFGD2AntiNuEBkgInNuModeCCOther;
    else if( strcmp("kFGD1NuEBkgInAntiNuModeCC",sample.c_str()) == 0) enum_sample = kFGD1NuEBkgInAntiNuModeCC;
    else if( strcmp("kFGD1NuEBkgInAntiNuModeCC0Pi",sample.c_str()) == 0) enum_sample = kFGD1NuEBkgInAntiNuModeCC0Pi;
    else if( strcmp("kFGD1NuEBkgInAntiNuModeCCOther",sample.c_str()) == 0) enum_sample = kFGD1NuEBkgInAntiNuModeCCOther;
    else if( strcmp("kFGD2NuEBkgInAntiNuModeCC",sample.c_str()) == 0) enum_sample = kFGD2NuEBkgInAntiNuModeCC;
    else if( strcmp("kFGD2NuEBkgInAntiNuModeCC0Pi",sample.c_str()) == 0) enum_sample = kFGD2NuEBkgInAntiNuModeCC0Pi;
    else if( strcmp("kFGD2NuEBkgInAntiNuModeCCOther",sample.c_str()) == 0) enum_sample = kFGD2NuEBkgInAntiNuModeCCOther;
    else if( strcmp("kFGD1Gamma",sample.c_str()) == 0) enum_sample = kFGD1Gamma;
    else if( strcmp("kFGD2Gamma",sample.c_str()) == 0) enum_sample = kFGD2Gamma;
    else if( strcmp("kFGD1GammaInAntiNuMode",sample.c_str()) == 0) enum_sample = kFGD1GammaInAntiNuMode;
    else if( strcmp("kFGD2GammaInAntiNuMode",sample.c_str()) == 0) enum_sample = kFGD2GammaInAntiNuMode;

    else if( strcmp("kP0DWaterNuMuCC",sample.c_str()) == 0)                       enum_sample = kP0DWaterNuMuCC;
    else if( strcmp("kP0DWaterNuMuCC1Track",sample.c_str()) == 0)                 enum_sample = kP0DWaterNuMuCC1Track;
    else if( strcmp("kP0DWaterNuMuCCNTracks",sample.c_str()) == 0)                enum_sample = kP0DWaterNuMuCCNTracks;

    else if( strcmp("kP0DWaterNuMuBarBkgCC",sample.c_str()) == 0)                 enum_sample = kP0DWaterNuMuBarBkgCC;

    else if( strcmp("kP0DWaterNuMuBkgInAntiNuModeCC",sample.c_str()) == 0)        enum_sample = kP0DWaterNuMuBkgInAntiNuModeCC;
    else if( strcmp("kP0DWaterNuMuBkgInAntiNuModeCC1Track",sample.c_str()) == 0)  enum_sample = kP0DWaterNuMuBkgInAntiNuModeCC1Track;
    else if( strcmp("kP0DWaterNuMuBkgInAntiNuModeCCNTracks",sample.c_str()) == 0) enum_sample = kP0DWaterNuMuBkgInAntiNuModeCCNTracks;

    else if( strcmp("kP0DWaterNuMuBarInAntiNuModeCC",sample.c_str()) == 0)        enum_sample = kP0DWaterNuMuBarInAntiNuModeCC;
    else if( strcmp("kP0DWaterNuMuBarInAntiNuModeCC1Track",sample.c_str()) == 0)  enum_sample = kP0DWaterNuMuBarInAntiNuModeCC1Track;
    else if( strcmp("kP0DWaterNuMuBarInAntiNuModeCCNTracks",sample.c_str()) == 0) enum_sample = kP0DWaterNuMuBarInAntiNuModeCCNTracks;

    else if( strcmp("kP0DAirNuMuCC",sample.c_str()) == 0)                         enum_sample = kP0DAirNuMuCC;
    else if( strcmp("kP0DAirNuMuCC1Track",sample.c_str()) == 0)                   enum_sample = kP0DAirNuMuCC1Track;
    else if( strcmp("kP0DAirNuMuCCNTracks",sample.c_str()) == 0)                  enum_sample = kP0DAirNuMuCCNTracks;

    else if( strcmp("kP0DAirNuMuBarBkgCC",sample.c_str()) == 0)                   enum_sample = kP0DAirNuMuBarBkgCC;

    else if( strcmp("kP0DAirNuMuBkgInAntiNuModeCC",sample.c_str()) == 0)          enum_sample = kP0DAirNuMuBkgInAntiNuModeCC;
    else if( strcmp("kP0DAirNuMuBkgInAntiNuModeCC1Track",sample.c_str()) == 0)    enum_sample = kP0DAirNuMuBkgInAntiNuModeCC1Track;
    else if( strcmp("kP0DAirNuMuBkgInAntiNuModeCCNTracks",sample.c_str()) == 0)   enum_sample = kP0DAirNuMuBkgInAntiNuModeCCNTracks;

    else if( strcmp("kP0DAirNuMuBarInAntiNuModeCC",sample.c_str()) == 0)          enum_sample = kP0DAirNuMuBarInAntiNuModeCC;
    else if( strcmp("kP0DAirNuMuBarInAntiNuModeCC1Track",sample.c_str()) == 0)    enum_sample = kP0DAirNuMuBarInAntiNuModeCC1Track;
    else if( strcmp("kP0DAirNuMuBarInAntiNuModeCCNTracks",sample.c_str()) == 0)   enum_sample = kP0DAirNuMuBarInAntiNuModeCCNTracks;

    else enum_sample = kUnassigned;
    return enum_sample;
}


//*********************************************************
bool SampleId::IsP0DSelection(const SampleEnum& sample)
//*********************************************************
{
    if(sample == kP0DAirNuMuCC || sample == kP0DWaterNuMuCC)
        return true;
    else if(sample == kP0DAirNuMuBarBkgCC || sample == kP0DWaterNuMuBarBkgCC)
        return true;
    else if(sample == kP0DAirNuMuBarInAntiNuModeCC || sample == kP0DWaterNuMuBarInAntiNuModeCC)
        return true;
    else if(sample == kP0DAirNuMuBkgInAntiNuModeCC || sample == kP0DWaterNuMuBkgInAntiNuModeCC)
        return true;
    else
        return false;
}
