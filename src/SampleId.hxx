#ifndef SAMPLEID_HXX
#define SAMPLEID_HXX

#include "TObject.h"

/// Event sample - whether it passes the NuMu-CCQE selection, or CC1Pi selection for example
class SampleId : public TObject {
public:

    enum SampleEnum {
        kUnassigned,
        // FGD1
        kFGD1NuMuCC,
        kFGD1NuMuCCQE,
        kFGD1NuMuCC0Pi,
        kFGD1NuMuCC1Pi,
        kFGD1NuMuCCOther,
        // FGD1 CC4pi
        kFGD1NuMuCC0PiFwd,
        kFGD1NuMuCC1PiFwd,
        kFGD1NuMuCCOtherFwd,
        kFGD1NuMuCC0PiBwd,
        kFGD1NuMuCC1PiBwd,
        kFGD1NuMuCCOtherBwd,
        kFGD1NuMuCC0PiHAFwd,
        kFGD1NuMuCC1PiHAFwd,
        kFGD1NuMuCCOtherHAFwd,
        kFGD1NuMuCC0PiHABwd,
        kFGD1NuMuCC1PiHABwd,
        kFGD1NuMuCCOtherHABwd,
        // FGD2
        kFGD2NuMuCC,
        kFGD2NuMuCC0Pi,
        kFGD2NuMuCC1Pi,
        kFGD2NuMuCCOther,
        // FGD2 CC4pi
        kFGD2NuMuCC0PiFwd,
        kFGD2NuMuCC1PiFwd,
        kFGD2NuMuCCOtherFwd,
        kFGD2NuMuCC0PiBwd,
        kFGD2NuMuCC1PiBwd,
        kFGD2NuMuCCOtherBwd,
        kFGD2NuMuCC0PiHAFwd,
        kFGD2NuMuCC1PiHAFwd,
        kFGD2NuMuCCOtherHAFwd,
        kFGD2NuMuCC0PiHABwd,
        kFGD2NuMuCC1PiHABwd,
        kFGD2NuMuCCOtherHABwd,
        // FGD1
        kFGD1NuECC,
        kFGD1NuECC0Pi,
        kFGD1NuECCOther,
        // FGD2
        kFGD2NuECC,
        kFGD2NuECC0Pi,
        kFGD2NuECCOther,
        // FGD1
        kFGD1AntiNuMuCC,
        kFGD1AntiNuMuCC1Track,
        kFGD1AntiNuMuCCNTracks,
        kFGD1AntiNuMuCC0Pi,
        kFGD1AntiNuMuCC1Pi,
        kFGD1AntiNuMuCCOther,
        // FGD2
        kFGD2AntiNuMuCC,
        kFGD2AntiNuMuCC1Track,
        kFGD2AntiNuMuCCNTracks,
        kFGD2AntiNuMuCC0Pi,
        kFGD2AntiNuMuCC1Pi,
        kFGD2AntiNuMuCCOther,
        // FGD1
        kFGD1NuMuBkgInAntiNuModeCC,
        kFGD1NuMuBkgInAntiNuModeCC1Track,
        kFGD1NuMuBkgInAntiNuModeCCNTracks,
        kFGD1NuMuBkgInAntiNuModeCC0Pi,
        kFGD1NuMuBkgInAntiNuModeCC1Pi,
        kFGD1NuMuBkgInAntiNuModeCCOther,
        // FGD2
        kFGD2NuMuBkgInAntiNuModeCC,
        kFGD2NuMuBkgInAntiNuModeCC1Track,
        kFGD2NuMuBkgInAntiNuModeCCNTracks,
        kFGD2NuMuBkgInAntiNuModeCC0Pi,
        kFGD2NuMuBkgInAntiNuModeCC1Pi,
        kFGD2NuMuBkgInAntiNuModeCCOther,
        // FGD1
        kFGD1NuMuCCBwd,
        // FGD2
        kFGD2NuMuCCBwd,
        // FGD1
        kFGD1AntiNuECC,
        kFGD1AntiNuECC0Pi,
        kFGD1AntiNuECCOther,
        // FGD2
        kFGD2AntiNuECC,
        kFGD2AntiNuECC0Pi,
        kFGD2AntiNuECCOther,
        // FGD1
        kFGD1AntiNuEBkgInNuModeCC,
        kFGD1AntiNuEBkgInNuModeCC0Pi,
        kFGD1AntiNuEBkgInNuModeCCOther,
        // FGD2
        kFGD2AntiNuEBkgInNuModeCC,
        kFGD2AntiNuEBkgInNuModeCC0Pi,
        kFGD2AntiNuEBkgInNuModeCCOther,
        // FGD1
        kFGD1NuEBkgInAntiNuModeCC,
        kFGD1NuEBkgInAntiNuModeCC0Pi,
        kFGD1NuEBkgInAntiNuModeCCOther,
        // FGD2
        kFGD2NuEBkgInAntiNuModeCC,
        kFGD2NuEBkgInAntiNuModeCC0Pi,
        kFGD2NuEBkgInAntiNuModeCCOther,
        // FGD1
        kFGD1Gamma,
        // FGD2
        kFGD2Gamma,
        // FGD1
        kFGD1GammaInAntiNuMode,
        // FGD2
        kFGD2GammaInAntiNuMode,
        // P0D, water mode irrelevant
        kP0DNuMuCC,

        // P0D, water in
        kP0DWaterNuMuCC,
        kP0DWaterNuMuBarBkgCC,
        kP0DWaterNuMuBarInAntiNuModeCC,
        kP0DWaterNuMuBkgInAntiNuModeCC,

        // P0D, water-out
        kP0DAirNuMuCC,
        kP0DAirNuMuBarBkgCC,
        kP0DAirNuMuBarInAntiNuModeCC,
        kP0DAirNuMuBkgInAntiNuModeCC,

        kNSamples
    };

    SampleId(){}
    virtual ~SampleId(){}

    SampleId::SampleEnum GetP0DWaterNuMuCCEnum() const {return kP0DWaterNuMuCC;}
    SampleId::SampleEnum GetP0DWaterNuMuBarBkgCCEnum() const {return kP0DWaterNuMuBarBkgCC;}
    SampleId::SampleEnum GetP0DWaterNuMuBkgInAntiNuModeCCEnum() const {return kP0DWaterNuMuBkgInAntiNuModeCC;}
    SampleId::SampleEnum GetP0DWaterNuMuBarInAntiNuModeCCEnum() const {return kP0DWaterNuMuBarInAntiNuModeCC;}
    SampleId::SampleEnum GetP0DAirNuMuCCEnum() const {return kP0DAirNuMuCC;}
    SampleId::SampleEnum GetP0DAirNuMuBarBkgCCEnum() const {return kP0DAirNuMuBarBkgCC;}
    SampleId::SampleEnum GetP0DAirNuMuBkgInAntiNuModeCCEnum() const {return kP0DAirNuMuBkgInAntiNuModeCC;}
    SampleId::SampleEnum GetP0DAirNuMuBarInAntiNuModeCCEnum() const {return kP0DAirNuMuBarInAntiNuModeCC;}
    inline const SampleId::SampleEnum Convert(const Int_t& input) const {return static_cast<SampleId::SampleEnum>(input);}

    Int_t GetP0DWaterNuMuCC() const {return GetP0DWaterNuMuCCEnum();}
    Int_t GetP0DWaterNuMuBarBkgCC() const {return GetP0DWaterNuMuBarBkgCCEnum();}
    Int_t GetP0DWaterNuMuBarInAntiNuModeCC() const {return GetP0DWaterNuMuBarInAntiNuModeCCEnum();}
    Int_t GetP0DWaterNuMuBkgInAntiNuModeCC() const {return GetP0DWaterNuMuBkgInAntiNuModeCCEnum();}
    Int_t GetP0DAirNuMuCC() const {return GetP0DAirNuMuCCEnum();}
    Int_t GetP0DAirNuMuBarBkgCC() const {return GetP0DAirNuMuBarBkgCCEnum();}
    Int_t GetP0DAirNuMuBarInAntiNuModeCC() const {return GetP0DAirNuMuBarInAntiNuModeCCEnum();}
    Int_t GetP0DAirNuMuBkgInAntiNuModeCC() const {return GetP0DAirNuMuBkgInAntiNuModeCCEnum();}

    Bool_t IsP0DNuMuSample(const SampleId::SampleEnum &testEnum) const;
    Bool_t IsP0DNuMuBkgSample(const SampleId::SampleEnum &testEnum) const;
    Bool_t IsP0DNuMuBkgInAntiNuModeSample(const SampleId::SampleEnum &testEnum) const;
    Bool_t IsP0DNuMuBarInAntiNuModeSample(const SampleId::SampleEnum &testEnum) const;
    Bool_t IsP0DWaterSample(const SampleId::SampleEnum &testEnum) const;
    Bool_t IsP0DAirSample(const SampleId::SampleEnum &testEnum) const;
    inline Bool_t IsP0DFHCSample(const SampleId::SampleEnum &testEnum) const {return IsP0DNuMuSample(testEnum)||IsP0DNuMuBkgSample(testEnum);}
    inline Bool_t IsP0DRHCSample(const SampleId::SampleEnum &testEnum) const {return IsP0DNuMuBkgInAntiNuModeSample(testEnum)||IsP0DNuMuBarInAntiNuModeSample(testEnum);}

    Bool_t IsP0DNuMuSample(const Int_t &testInt) const {return IsP0DNuMuSample(Convert(testInt));}
    Bool_t IsP0DNuMuBkgSample(const Int_t &testInt) const {return IsP0DNuMuBkgSample(Convert(testInt));}
    Bool_t IsP0DNuMuBkgInAntiNuModeSample(const Int_t &testInt) const {return IsP0DNuMuBkgInAntiNuModeSample(Convert(testInt));}
    Bool_t IsP0DNuMuBarInAntiNuModeSample(const Int_t &testInt) const {return IsP0DNuMuBarInAntiNuModeSample(Convert(testInt));}
    Bool_t IsP0DWaterSample(const Int_t &testInt) const {return IsP0DWaterSample(Convert(testInt));}
    Bool_t IsP0DAirSample(const Int_t &testInt) const {return IsP0DAirSample(Convert(testInt));}

    const char* GetLabelName(const SampleId::SampleEnum &testEnum) const;
    const char* GetLabelName(const Int_t &testInt) const { return GetLabelName(Convert(testInt)); }

    ClassDef(SampleId, 1)

};

#endif
