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

        kP0DWaterNuMuCC,
        kP0DWaterNuMuCC1Track,
        kP0DWaterNuMuCCNTracks,

        kP0DWaterNuMuBarBkgCC, //DO NOT USE

        kP0DWaterNuMuBarInAntiNuModeCC,
        kP0DWaterNuMuBarInAntiNuModeCC1Track,
        kP0DWaterNuMuBarInAntiNuModeCCNTracks,

        kP0DWaterNuMuBkgInAntiNuModeCC,
        kP0DWaterNuMuBkgInAntiNuModeCC1Track,
        kP0DWaterNuMuBkgInAntiNuModeCCNTracks,

        kP0DAirNuMuCC,
        kP0DAirNuMuCC1Track,
        kP0DAirNuMuCCNTracks,

        kP0DAirNuMuBarBkgCC, //DO NOT USE

        kP0DAirNuMuBarInAntiNuModeCC,
        kP0DAirNuMuBarInAntiNuModeCC1Track,
        kP0DAirNuMuBarInAntiNuModeCCNTracks,

        kP0DAirNuMuBkgInAntiNuModeCC,
        kP0DAirNuMuBkgInAntiNuModeCC1Track,
        kP0DAirNuMuBkgInAntiNuModeCCNTracks,

        kNSamples
    };

    SampleId(){}
    virtual ~SampleId(){}

    inline SampleId::SampleEnum Convert(const Int_t& input) const {return static_cast<SampleId::SampleEnum>(input);}
    inline Int_t Convert(const SampleId::SampleEnum& input) const {return static_cast<Int_t>(input);}
    SampleId::SampleEnum GetP0DWaterNuMuCCEnum() const {return kP0DWaterNuMuCC;}
    SampleId::SampleEnum GetP0DWaterNuMuCC1TrackEnum() const {return kP0DWaterNuMuCC1Track;}
    SampleId::SampleEnum GetP0DWaterNuMuCCNTracksEnum() const {return kP0DWaterNuMuCCNTracks;}
    SampleId::SampleEnum GetP0DWaterNuMuBarBkgCCEnum() const {return kP0DWaterNuMuBarBkgCC;}
    SampleId::SampleEnum GetP0DWaterNuMuBkgInAntiNuModeCCEnum() const {return kP0DWaterNuMuBkgInAntiNuModeCC;}
    SampleId::SampleEnum GetP0DWaterNuMuBkgInAntiNuModeCC1TrackEnum() const {return kP0DWaterNuMuBkgInAntiNuModeCC1Track;}
    SampleId::SampleEnum GetP0DWaterNuMuBkgInAntiNuModeCCNTracksEnum() const {return kP0DWaterNuMuBkgInAntiNuModeCCNTracks;}
    SampleId::SampleEnum GetP0DWaterNuMuBarInAntiNuModeCCEnum() const {return kP0DWaterNuMuBarInAntiNuModeCC;}
    SampleId::SampleEnum GetP0DWaterNuMuBarInAntiNuModeCC1TrackEnum() const {return kP0DWaterNuMuBarInAntiNuModeCC1Track;}
    SampleId::SampleEnum GetP0DWaterNuMuBarInAntiNuModeCCNTracksEnum() const {return kP0DWaterNuMuBarInAntiNuModeCCNTracks;}
    SampleId::SampleEnum GetP0DAirNuMuCCEnum() const {return kP0DAirNuMuCC;}
    SampleId::SampleEnum GetP0DAirNuMuCC1TrackEnum() const {return kP0DAirNuMuCC1Track;}
    SampleId::SampleEnum GetP0DAirNuMuCCNTracksEnum() const {return kP0DAirNuMuCCNTracks;}
    SampleId::SampleEnum GetP0DAirNuMuBarBkgCCEnum() const {return kP0DAirNuMuBarBkgCC;}
    SampleId::SampleEnum GetP0DAirNuMuBkgInAntiNuModeCCEnum() const {return kP0DAirNuMuBkgInAntiNuModeCC;}
    SampleId::SampleEnum GetP0DAirNuMuBkgInAntiNuModeCC1TrackEnum() const {return kP0DAirNuMuBkgInAntiNuModeCC1Track;}
    SampleId::SampleEnum GetP0DAirNuMuBkgInAntiNuModeCCNTracksEnum() const {return kP0DAirNuMuBkgInAntiNuModeCCNTracks;}
    SampleId::SampleEnum GetP0DAirNuMuBarInAntiNuModeCCEnum() const {return kP0DAirNuMuBarInAntiNuModeCC;}
    SampleId::SampleEnum GetP0DAirNuMuBarInAntiNuModeCC1TrackEnum() const {return kP0DAirNuMuBarInAntiNuModeCC1Track;}
    SampleId::SampleEnum GetP0DAirNuMuBarInAntiNuModeCCNTracksEnum() const {return kP0DAirNuMuBarInAntiNuModeCCNTracks;}

    Int_t GetP0DWaterNuMuCC() const {return GetP0DWaterNuMuCCEnum();}
    Int_t GetP0DWaterNuMuCC1Track() const {return GetP0DWaterNuMuCC1TrackEnum();}
    Int_t GetP0DWaterNuMuCCNTracks() const {return GetP0DWaterNuMuCCNTracksEnum();}
    Int_t GetP0DWaterNuMuBarBkgCC() const {return GetP0DWaterNuMuBarBkgCCEnum();}
    Int_t GetP0DWaterNuMuBarInAntiNuModeCC() const {return GetP0DWaterNuMuBarInAntiNuModeCCEnum();}
    Int_t GetP0DWaterNuMuBarInAntiNuModeCC1Track() const {return GetP0DWaterNuMuBarInAntiNuModeCC1TrackEnum();}
    Int_t GetP0DWaterNuMuBarInAntiNuModeCCNTracks() const {return GetP0DWaterNuMuBarInAntiNuModeCCNTracksEnum();}
    Int_t GetP0DWaterNuMuBkgInAntiNuModeCC() const {return GetP0DWaterNuMuBkgInAntiNuModeCCEnum();}
    Int_t GetP0DWaterNuMuBkgInAntiNuModeCC1Track() const {return GetP0DWaterNuMuBkgInAntiNuModeCC1TrackEnum();}
    Int_t GetP0DWaterNuMuBkgInAntiNuModeCCNTracks() const {return GetP0DWaterNuMuBkgInAntiNuModeCCNTracksEnum();}
    Int_t GetP0DAirNuMuCC() const {return GetP0DAirNuMuCCEnum();}
    Int_t GetP0DAirNuMuCC1Track() const {return GetP0DAirNuMuCC1TrackEnum();}
    Int_t GetP0DAirNuMuCCNTracks() const {return GetP0DAirNuMuCCNTracksEnum();}
    Int_t GetP0DAirNuMuBarBkgCC() const {return GetP0DAirNuMuBarBkgCCEnum();}
    Int_t GetP0DAirNuMuBarInAntiNuModeCC() const {return GetP0DAirNuMuBarInAntiNuModeCCEnum();}
    Int_t GetP0DAirNuMuBarInAntiNuModeCC1Track() const {return GetP0DAirNuMuBarInAntiNuModeCC1TrackEnum();}
    Int_t GetP0DAirNuMuBarInAntiNuModeCCNTracks() const {return GetP0DAirNuMuBarInAntiNuModeCCNTracksEnum();}
    Int_t GetP0DAirNuMuBkgInAntiNuModeCC() const {return GetP0DAirNuMuBkgInAntiNuModeCCEnum();}
    Int_t GetP0DAirNuMuBkgInAntiNuModeCC1Track() const {return GetP0DAirNuMuBkgInAntiNuModeCC1TrackEnum();}
    Int_t GetP0DAirNuMuBkgInAntiNuModeCCNTracks() const {return GetP0DAirNuMuBkgInAntiNuModeCCNTracksEnum();}

    Bool_t IsP0DNuMuSample(const SampleId::SampleEnum &testEnum) const;
    Bool_t IsP0DNuMuBkgInAntiNuModeSample(const SampleId::SampleEnum &testEnum) const;
    Bool_t IsP0DNuMuBarInAntiNuModeSample(const SampleId::SampleEnum &testEnum) const;
    Bool_t IsP0DWaterSample(const SampleId::SampleEnum &testEnum) const;
    Bool_t IsP0DAirSample(const SampleId::SampleEnum &testEnum) const;
    Bool_t IsP0DFHCSample(const SampleId::SampleEnum &testEnum) const {return IsP0DNuMuSample(testEnum);}
    Bool_t IsP0DRHCSample(const SampleId::SampleEnum &testEnum) const {return IsP0DNuMuBkgInAntiNuModeSample(testEnum)||IsP0DNuMuBarInAntiNuModeSample(testEnum);}
    Bool_t IsP0DAnySample(const SampleId::SampleEnum &testEnum) const {return IsP0DFHCSample(testEnum)||IsP0DRHCSample(testEnum);}

    //Converts the Water-In to Water-Out (air) and vice versa
    SampleId::SampleEnum GetComplimentarySample(const SampleId::SampleEnum &testEnum) const;
    SampleId::SampleEnum GetComplimentarySample(const Int_t &testInt) const {return GetComplimentarySample(Convert(testInt));}

    const char* GetLabelName(const SampleId::SampleEnum &testEnum) const;
    const char* GetLabelName(const Int_t &testInt) const { return GetLabelName(Convert(testInt)); }

    ClassDef(SampleId, 1)

};

#endif
