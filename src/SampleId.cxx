#define SAMPLEID_CXX
#include "SampleId.hxx"
ClassImp(SampleId)

//**************************************************
Bool_t SampleId::IsP0DFHCSample(const SampleId::SampleEnum &testEnum) const
//**************************************************
{
    if(testEnum == kP0DWaterNuMuCC) return kTRUE;
    else if(testEnum == kP0DWaterNuMuBarBkgCC) return kTRUE;
    else if(testEnum == kP0DAirNuMuCC) return kTRUE;
    else if(testEnum == kP0DAirNuMuBarBkgCC) return kTRUE;
    else return kFALSE;
}

//**************************************************
Bool_t SampleId::IsP0DRHCSample(const SampleId::SampleEnum &testEnum) const
//**************************************************
{
    if(testEnum == kP0DWaterNuMuBarInAntiNuModeCC) return kTRUE;
    else if(testEnum == kP0DWaterNuMuBkgInAntiNuModeCC) return kTRUE;
    else if(testEnum == kP0DAirNuMuBarInAntiNuModeCC) return kTRUE;
    else if(testEnum == kP0DAirNuMuBkgInAntiNuModeCC) return kTRUE;
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
