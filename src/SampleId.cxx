#define SAMPLEID_CXX
#include "SampleId.hxx"
ClassImp(SampleId)

//**************************************************
Bool_t SampleId::IsP0DNuMuSample(const SampleId::SampleEnum &testEnum) const
//**************************************************
{
    if(testEnum == kP0DWaterNuMuCC) return kTRUE;
    if(testEnum == kP0DWaterNuMuCC1Track) return kTRUE;
    if(testEnum == kP0DWaterNuMuCCNTracks) return kTRUE;
    else if(testEnum == kP0DAirNuMuCC) return kTRUE;
    else if(testEnum == kP0DAirNuMuCC1Track) return kTRUE;
    else if(testEnum == kP0DAirNuMuCCNTracks) return kTRUE;
    else return kFALSE;
}

//**************************************************
Bool_t SampleId::IsP0DNuMuBkgSample(const SampleId::SampleEnum &testEnum) const
//**************************************************
{
    if(testEnum == kP0DWaterNuMuBarBkgCC) return kTRUE;
    else if(testEnum == kP0DAirNuMuBarBkgCC) return kTRUE;
    else return kFALSE;
}

//**************************************************
Bool_t SampleId::IsP0DNuMuBkgInAntiNuModeSample(const SampleId::SampleEnum &testEnum) const
//**************************************************
{
    if(testEnum == kP0DWaterNuMuBkgInAntiNuModeCC) return kTRUE;
    if(testEnum == kP0DWaterNuMuBkgInAntiNuModeCC1Track) return kTRUE;
    if(testEnum == kP0DWaterNuMuBkgInAntiNuModeCCNTracks) return kTRUE;
    else if(testEnum == kP0DAirNuMuBkgInAntiNuModeCC) return kTRUE;
    else if(testEnum == kP0DAirNuMuBkgInAntiNuModeCC1Track) return kTRUE;
    else if(testEnum == kP0DAirNuMuBkgInAntiNuModeCCNTracks) return kTRUE;
    else return kFALSE;
}

//**************************************************
Bool_t SampleId::IsP0DNuMuBarInAntiNuModeSample(const SampleId::SampleEnum &testEnum) const
//**************************************************
{
    if(testEnum == kP0DWaterNuMuBarInAntiNuModeCC) return kTRUE;
    if(testEnum == kP0DWaterNuMuBarInAntiNuModeCC1Track) return kTRUE;
    if(testEnum == kP0DWaterNuMuBarInAntiNuModeCCNTracks) return kTRUE;
    else if(testEnum == kP0DAirNuMuBarInAntiNuModeCC) return kTRUE;
    else if(testEnum == kP0DAirNuMuBarInAntiNuModeCC1Track) return kTRUE;
    else if(testEnum == kP0DAirNuMuBarInAntiNuModeCCNTracks) return kTRUE;
    else return kFALSE;
}

//**************************************************
Bool_t SampleId::IsP0DWaterSample(const SampleId::SampleEnum &testEnum) const
//**************************************************
{
    if(testEnum == kP0DWaterNuMuCC) return kTRUE;
    if(testEnum == kP0DWaterNuMuCC1Track) return kTRUE;
    if(testEnum == kP0DWaterNuMuCCNTracks) return kTRUE;
    else if(testEnum == kP0DWaterNuMuBarBkgCC) return kTRUE;
    else if(testEnum == kP0DWaterNuMuBarInAntiNuModeCC) return kTRUE;
    else if(testEnum == kP0DWaterNuMuBarInAntiNuModeCC1Track) return kTRUE;
    else if(testEnum == kP0DWaterNuMuBarInAntiNuModeCCNTracks) return kTRUE;
    else if(testEnum == kP0DWaterNuMuBkgInAntiNuModeCC) return kTRUE;
    else if(testEnum == kP0DWaterNuMuBkgInAntiNuModeCC1Track) return kTRUE;
    else if(testEnum == kP0DWaterNuMuBkgInAntiNuModeCCNTracks) return kTRUE;
    else return kFALSE;
}
//**************************************************
Bool_t SampleId::IsP0DAirSample(const SampleId::SampleEnum &testEnum) const
//**************************************************
{
    if(testEnum == kP0DAirNuMuCC) return kTRUE;
    if(testEnum == kP0DAirNuMuCC1Track) return kTRUE;
    if(testEnum == kP0DAirNuMuCCNTracks) return kTRUE;
    else if(testEnum == kP0DAirNuMuBarBkgCC) return kTRUE;
    else if(testEnum == kP0DAirNuMuBarInAntiNuModeCC) return kTRUE;
    else if(testEnum == kP0DAirNuMuBarInAntiNuModeCC1Track) return kTRUE;
    else if(testEnum == kP0DAirNuMuBarInAntiNuModeCCNTracks) return kTRUE;
    else if(testEnum == kP0DAirNuMuBkgInAntiNuModeCC) return kTRUE;
    else if(testEnum == kP0DAirNuMuBkgInAntiNuModeCC1Track) return kTRUE;
    else if(testEnum == kP0DAirNuMuBkgInAntiNuModeCCNTracks) return kTRUE;
    else return kFALSE;
}


//**************************************************
const char* SampleId::GetLabelName(const SampleId::SampleEnum &testEnum) const
//**************************************************
{
    if(testEnum == GetP0DWaterNuMuCCEnum())                return "#nu_{#mu} CCInc in FHC"     ;
    if(testEnum == GetP0DWaterNuMuBarInAntiNuModeCCEnum()) return "#bar{#nu}_{#mu} CCInc in RHC";
    if(testEnum == GetP0DWaterNuMuBkgInAntiNuModeCCEnum()) return "#nu_{#mu} Bkg CCInc in RHC"  ;
    if(testEnum == GetP0DAirNuMuCCEnum())                  return "#nu_{#mu} CCInc in FHC"     ;
    if(testEnum == GetP0DAirNuMuBarInAntiNuModeCCEnum())   return "#bar{#nu}_{#mu} CCInc in RHC";
    if(testEnum == GetP0DAirNuMuBkgInAntiNuModeCCEnum())   return "#nu_{#mu} Bkg CCInc in RHC"  ;
    if(testEnum == GetP0DWaterNuMuBarBkgCCEnum())          return "#bar{#nu}_{#mu} CCInc in FHC";
    if(testEnum == GetP0DAirNuMuBarBkgCCEnum())            return "#bar{#nu}_{#mu} CCInc in FHC";

    if(testEnum == GetP0DWaterNuMuCC1TrackEnum())                 return "#nu_{#mu} CC1Track in FHC"     ;
    if(testEnum == GetP0DWaterNuMuCCNTracksEnum())                return "#nu_{#mu} CCNTracks in FHC"     ;
    if(testEnum == GetP0DWaterNuMuBarInAntiNuModeCC1TrackEnum())  return "#bar{#nu}_{#mu} CC1Track in RHC";
    if(testEnum == GetP0DWaterNuMuBarInAntiNuModeCCNTracksEnum()) return "#bar{#nu}_{#mu} CCNTracks in RHC";
    if(testEnum == GetP0DWaterNuMuBkgInAntiNuModeCC1TrackEnum())  return "#nu_{#mu} Bkg CC1Track in RHC"  ;
    if(testEnum == GetP0DWaterNuMuBkgInAntiNuModeCCNTracksEnum()) return "#nu_{#mu} Bkg CCNTracks in RHC"  ;
    if(testEnum == GetP0DAirNuMuCC1TrackEnum())                   return "#nu_{#mu} CC1Track in FHC"     ;
    if(testEnum == GetP0DAirNuMuCCNTracksEnum())                  return "#nu_{#mu} CCNTracks in FHC"     ;
    if(testEnum == GetP0DAirNuMuBarInAntiNuModeCC1TrackEnum())    return "#bar{#nu}_{#mu} CC1Track in RHC";
    if(testEnum == GetP0DAirNuMuBarInAntiNuModeCCNTracksEnum())   return "#bar{#nu}_{#mu} CCNTracks in RHC";
    if(testEnum == GetP0DAirNuMuBkgInAntiNuModeCC1TrackEnum())    return "#nu_{#mu} Bkg CC1Track in RHC"  ;
    if(testEnum == GetP0DAirNuMuBkgInAntiNuModeCCNTracksEnum())   return "#nu_{#mu} Bkg CCNTracks in RHC"  ;

    return "";
}
