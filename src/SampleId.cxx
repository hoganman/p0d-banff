#define SAMPLEID_CXX
#include "SampleId.hxx"
ClassImp(SampleId)

//**************************************************
Bool_t SampleId::IsP0DFHCSample(SampleId::SampleEnum testEnum) const
//**************************************************
{
   if(testEnum == kP0DNuMuCC) return kTRUE;
   else if(testEnum == kP0DNuMuBarBkgCC) return kTRUE;
   else return kFALSE;
}

//**************************************************
Bool_t SampleId::IsP0DRHCSample(SampleId::SampleEnum testEnum) const
//**************************************************
{
   if(testEnum == kP0DNuMuBkgInAntiNuModeCC) return kTRUE;
   else if(testEnum == kP0DNuMuBarInAntiNuModeCC) return kTRUE;
   else return kFALSE;
}
