#define SAMPLEID_CXX
#include "SampleId.hxx"
ClassImp(SampleId)

//**************************************************
Bool_t SampleId::IsP0DNuMuSample(const SampleId::SampleEnum &testEnum) const
//**************************************************
{
    if(testEnum == kP0DWaterNuMuCC) return kTRUE;
    else if(testEnum == kP0DAirNuMuCC) return kTRUE;
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
    else if(testEnum == kP0DAirNuMuBkgInAntiNuModeCC) return kTRUE;
    else return kFALSE;
}

//**************************************************
Bool_t SampleId::IsP0DNuMuBarInAntiNuModeSample(const SampleId::SampleEnum &testEnum) const
//**************************************************
{
    if(testEnum == kP0DWaterNuMuBarInAntiNuModeCC) return kTRUE;
    else if(testEnum == kP0DAirNuMuBarInAntiNuModeCC) return kTRUE;
    else return kFALSE;
}

//**************************************************
Bool_t SampleId::IsP0DWaterSample(const SampleId::SampleEnum &testEnum) const
//**************************************************
{
    if(testEnum == kP0DWaterNuMuCC) return kTRUE;
    else if(testEnum == kP0DWaterNuMuBarBkgCC) return kTRUE;
    else if(testEnum == kP0DWaterNuMuBarInAntiNuModeCC) return kTRUE;
    else if(testEnum == kP0DWaterNuMuBkgInAntiNuModeCC) return kTRUE;
    else return kFALSE;
}
//**************************************************
Bool_t SampleId::IsP0DAirSample(const SampleId::SampleEnum &testEnum) const
//**************************************************
{
    if(testEnum == kP0DAirNuMuCC) return kTRUE;
    else if(testEnum == kP0DAirNuMuBarBkgCC) return kTRUE;
    else if(testEnum == kP0DAirNuMuBarInAntiNuModeCC) return kTRUE;
    else if(testEnum == kP0DAirNuMuBkgInAntiNuModeCC) return kTRUE;
    else return kFALSE;
}


//**************************************************
const char* SampleId::GetLabelName(const SampleId::SampleEnum &testEnum) const
//**************************************************
{
    if(testEnum == GetP0DWaterNuMuCCEnum())                return "#nu_{#mu} in FHC"     ;
    if(testEnum == GetP0DWaterNuMuBarInAntiNuModeCCEnum()) return "#bar{#nu}_{#mu} in RHC";
    if(testEnum == GetP0DWaterNuMuBkgInAntiNuModeCCEnum()) return "#nu_{#mu} Bkg in RHC"  ;
    if(testEnum == GetP0DAirNuMuCCEnum())                  return "#nu_{#mu} in FHC"     ;
    if(testEnum == GetP0DAirNuMuBarInAntiNuModeCCEnum())   return "#bar{#nu}_{#mu} in RHC";
    if(testEnum == GetP0DAirNuMuBkgInAntiNuModeCCEnum())   return "#nu_{#mu} Bkg in RHC"  ;
    if(testEnum == GetP0DWaterNuMuBarBkgCCEnum())          return "#bar{#nu}_{#mu} in FHC";
    if(testEnum == GetP0DAirNuMuBarBkgCCEnum())            return "#bar{#nu}_{#mu} in FHC";
    return "";
}
