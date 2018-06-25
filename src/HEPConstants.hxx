#ifndef HEPCONSTANTS_HXX
#define HEPCONSTANTS_HXX

#include "TObject.h"
#include "TString.h"
#include <map>

class HEPConstants : public TObject {

public:
    HEPConstants()
    {
        kElectronPDG   = +11;
        kNuEPDG        = +12;
        kMuMinusPDG    = +13;
        kNuMuPDG       = +14;
        kTauMinusPDG   = +15;
        kNuTauPDG      = +16;
        kPositronPDG   = -11;
        kNuEBarPDG     = -12;
        kMuPlusPDG     = -13;
        kNuMuBarPDG    = -14;
        kTauPlusPDG    = -15;
        kNuTauBarPDG   = -16;
        kAntiNuEPDG    = -12;
        kAntiNuMuPDG   = -14;
        kAntiNuTauPDG  = -16;

        kElectronMass = 0.510998928;
        kMuMinusMass  = 105.6583745;
        kTauMinusMass = 1776.82;
        kMuPlusMass  = 105.6583745;

        kGammaPDG = 22;
        kGammaMass = 0.0;

        //***********
        // Mesons
        //***********
        kKaMinusPDG     = -321;
        kRho770MinusPDG = -213;
        kPiMinusPDG     = -211;

        kPiZeroPDG      = +111;
        kRho770ZeroPDG  = +113;
        kKaZeroLongPDG  = +130;
        kPiPlusPDG      = +211;
        kRho770PlusPDG  = +213;
        kEtaPDG         = +221;
        komega782PDG    = +223;
        kKaZeroShortPDG = +310;
        kKaZeroPDG      = +311;
        kKaPlusPDG      = +321;
        kEtaPrimePDG    = +331;

        kKaMinusMass     = 493.667;
        kRho770MinusMass = 775.26;
        kPiMinusMass     = 139.570;

        kPiZeroMass      = 134.9766;
        kRho770ZeroMass  = 775.26;
        kKaZeroLongMass  = 497.614;
        kPiPlusMass      = 139.570;
        kRho770PlusMass  = 775.26;
        kEtaMass         = 547.862;
        komega782Mass    = 782.65;
        kZeroShortMass   = 497.614;
        kKaZeroMass      = 497.648;
        kKaPlusMass      = 493.667;
        kEtaPrimeMass    = 547.862;

        //***********
        // Baryons
        //***********
        kNeutronPDG       = 2112;
        kProtonPDG        = 2212;
        kDeltaMinusPDG    = 1114;
        kDeltaZeroPDG     = 2114;
        kDeltaPlusPDG     = 2214;
        kDeltaPlusPlusPDG = 2224;
        kLambdaZeroPDG    = 3122;
        kNeutronMass       = 939.565379;
        kProtonMass        = 938.272046;
        kLambdaZeroMass    = 1115.683;
        kDeltaMinusMass    = 1232;
        kDeltaZeroMass     = 1232;
        kDeltaPlusMass     = 1232;
        kDeltaPlusPlusMass = 1232;

        MeV = 1.0;
        conversionMap["MeV"] = MeV;
        conversionMap["MeV/c"] = MeV;
        conversionMap["MeV/c2"] = MeV;
        GeV = 1e-3 * MeV;
        conversionMap["GeV"] = GeV;
        conversionMap["GeV/c"] = GeV;
        conversionMap["GeV/c2"] = GeV;

        mm = 1.0;
        meter = 1e-3 * mm;
        conversionMap["mm"] = mm;
        conversionMap["m"] = meter;
        conversionMap["meter"] = meter;

    }

    virtual ~HEPConstants(){}
    //***********
    // Lepton
    //***********
    Int_t kElectronPDG;
    Int_t kNuEPDG;
    Int_t kMuMinusPDG;
    Int_t kNuMuPDG;
    Int_t kTauMinusPDG;
    Int_t kNuTauPDG;
    Int_t kPositronPDG;
    Int_t kNuEBarPDG;
    Int_t kMuPlusPDG;
    Int_t kNuMuBarPDG;
    Int_t kTauPlusPDG;
    Int_t kNuTauBarPDG;
    Int_t kAntiNuEPDG;
    Int_t kAntiNuMuPDG;
    Int_t kAntiNuTauPDG;

    Double_t kElectronMass;
    Double_t kMuMinusMass;
    Double_t kTauMinusMass;
    Double_t kMuPlusMass;

    Int_t kGammaPDG;
    Double_t kGammaMass;

    //***********
    // Mesons
    //***********
    Int_t kKaMinusPDG;
    Int_t kRho770MinusPDG;
    Int_t kPiMinusPDG;

    Int_t kPiZeroPDG;
    Int_t kRho770ZeroPDG;
    Int_t kKaZeroLongPDG;
    Int_t kPiPlusPDG;
    Int_t kRho770PlusPDG;
    Int_t kEtaPDG;
    Int_t komega782PDG;
    Int_t kKaZeroShortPDG;
    Int_t kKaZeroPDG;
    Int_t kKaPlusPDG;
    Int_t kEtaPrimePDG;

    Double_t kKaMinusMass;
    Double_t kRho770MinusMass;
    Double_t kPiMinusMass;

    Double_t kPiZeroMass;
    Double_t kRho770ZeroMass;
    Double_t kKaZeroLongMass;
    Double_t kPiPlusMass;
    Double_t kRho770PlusMass;
    Double_t kEtaMass;
    Double_t komega782Mass;
    Double_t kZeroShortMass;
    Double_t kKaZeroMass;
    Double_t kKaPlusMass;
    Double_t kEtaPrimeMass;

    //***********
    // Baryons
    //***********
    Int_t kNeutronPDG;
    Int_t kProtonPDG;
    Int_t kDeltaMinusPDG;
    Int_t kDeltaZeroPDG;
    Int_t kDeltaPlusPDG;
    Int_t kDeltaPlusPlusPDG;
    Int_t kLambdaZeroPDG;
    Double_t kNeutronMass;
    Double_t kProtonMass;
    Double_t kDeltaMinusMass;
    Double_t kDeltaZeroMass;
    Double_t kDeltaPlusMass;
    Double_t kDeltaPlusPlusMass;
    Double_t kLambdaZeroMass;

    Double_t MeV;
    Double_t GeV;
    Double_t mm;
    Double_t meter;

    std::map<TString, Double_t> conversionMap;
    Double_t Convert(TString unit) const;
    ClassDef(HEPConstants, 1)
};
#endif







