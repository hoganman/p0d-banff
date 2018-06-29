#define HEPCONSTANTS_CXX
#include "HEPConstants.hxx"
#include <algorithm>
#include <cmath>
#include <iostream>
ClassImp(HEPConstants)

//**************************************************
void HEPConstants::Init()
//**************************************************
{

    MeV = 1.0;
    convert["MeV"   ] = MeV;
    convert["MeV/c" ] = MeV;
    convert["MeV/c2"] = MeV;
    GeV = 1.0e+3 * MeV;
    convert["GeV"   ] = GeV;
    convert["GeV/c" ] = GeV;
    convert["GeV/c2"] = GeV;
    MeV2 = MeV*MeV;
    convert["MeV2"] = MeV2;
    GeV2 = GeV*GeV;
    convert["GeV2"] = GeV2;

    mm = 1.0;
    meter = 1.0e+3 * mm;
    convert["mm"] = mm;
    convert["m"] = meter;
    convert["meter"] = meter;

    PDGINDEX    = 0;
    MASSINDEX   = 1;
    CHARGEINDEX = 2;
    kElectronPDG  = +11;
    kNuEPDG       = +12;
    kMuMinusPDG   = +13;
    kNuMuPDG      = +14;
    kTauMinusPDG  = +15;
    kNuTauPDG     = +16;
    kPositronPDG  = -11;
    kNuEBarPDG    = -12;
    kMuPlusPDG    = -13;
    kNuMuBarPDG   = -14;
    kTauPlusPDG   = -15;
    kNuTauBarPDG  = -16;
    kAntiNuEPDG   = -12;
    kAntiNuMuPDG  = -14;
    kAntiNuTauPDG = -16;

    kElectronMass = 0.510998928 * MeV;
    kMuMinusMass  = 105.6583745 * MeV;
    kTauMinusMass = 1776.82 * MeV;
    kMuPlusMass   = 105.6583745 * MeV;

    kGammaPDG  = 22;
    kGammaMass = 0.0;

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

    kKaMinusMass     = 493.667 * MeV;
    kRho770MinusMass = 775.26 * MeV;
    kPiMinusMass     = 139.570 * MeV;

    kPiZeroMass      = 134.9766 * MeV;
    kRho770ZeroMass  = 775.26 * MeV;
    kKaZeroLongMass  = 497.614 * MeV;
    kPiPlusMass      = 139.570 * MeV;
    kRho770PlusMass  = 775.26 * MeV;
    kEtaMass         = 547.862 * MeV;
    komega782Mass    = 782.65 * MeV;
    kZeroShortMass   = 497.614 * MeV;
    kKaZeroMass      = 497.648 * MeV;
    kKaPlusMass      = 493.667 * MeV;
    kEtaPrimeMass    = 547.862 * MeV;

    kNeutronPDG       = 2112;
    kProtonPDG        = 2212;
    kDeltaMinusPDG    = 1114;
    kDeltaZeroPDG     = 2114;
    kDeltaPlusPDG     = 2214;
    kDeltaPlusPlusPDG = 2224;
    kLambdaZeroPDG    = 3122;
    kNeutronMass       = 939.565379 * MeV;
    kProtonMass        = 938.272046 * MeV;
    kLambdaZeroMass    = 1115.683 * MeV;
    kDeltaMinusMass    = 1232 * MeV;
    kDeltaZeroMass     = 1232 * MeV;
    kDeltaPlusMass     = 1232 * MeV;
    kDeltaPlusPlusMass = 1232 * MeV;

}

//**************************************************
Double_t HEPConstants::Convert(TString unit) const {
//**************************************************
    ConversionMap::const_iterator iter = convert.find(unit);
    if(iter != convert.end())
    {
        return 1.0/iter->second;
    }
    return 1.0;
}

//**************************************************
HEPConstants::~HEPConstants()
//**************************************************
{
}

//**************************************************
inline Double_t HEPConstants::ConvertKineticEToMomentum(Double_t kinE, Double_t mass) const
//**************************************************
{
    return std::sqrt((kinE + mass) * (kinE + mass) - (mass * mass));
}

//**************************************************
inline Double_t HEPConstants::ConvertMomentumToKineticE(Double_t mom, Double_t mass) const
//**************************************************
{
    return sqrt(mom * mom + mass * mass) - mass;
}

//**************************************************
inline Double_t HEPConstants::ConvertMomentumToEnergy(Double_t mom, Double_t mass) const
//**************************************************
{
    return sqrt(mom * mom + mass * mass);
}

//**************************************************
Double_t HEPConstants::LoopOnParticles(Int_t pdg, const Int_t& returnIndex) const
//**************************************************
{
    //----------------------------------------------
    //first the most frequent particles
    if (pdg == kMuMinusPDG      ) return (returnIndex == MASSINDEX) ? kMuMinusMass      : -1.0;
    if (pdg == kProtonPDG       ) return (returnIndex == MASSINDEX) ? kProtonMass       : +1.0;
    if (pdg == kPiPlusPDG       ) return (returnIndex == MASSINDEX) ? kPiPlusMass       : +1.0;
    if (pdg == kPiZeroPDG       ) return (returnIndex == MASSINDEX) ? kPiZeroMass       :  0.0;
    if (pdg == kPiMinusPDG      ) return (returnIndex == MASSINDEX) ? kPiMinusMass      : -1.0;
    if (pdg == kNeutronPDG      ) return (returnIndex == MASSINDEX) ? kNeutronMass      :  0.0;
    if (pdg == kMuPlusPDG       ) return (returnIndex == MASSINDEX) ? kMuPlusMass       : +1.0;
    if (pdg == kElectronPDG     ) return (returnIndex == MASSINDEX) ? kElectronMass     : -1.0;
    if (pdg == kPositronPDG     ) return (returnIndex == MASSINDEX) ? kElectronMass     : -1.0;
    if (pdg == kGammaPDG        ) return (returnIndex == MASSINDEX) ? kGammaMass        :  0.0;
    if (pdg == kLambdaZeroPDG   ) return (returnIndex == MASSINDEX) ? kLambdaZeroMass   :  0.0;
    if (pdg == kKaPlusPDG       ) return (returnIndex == MASSINDEX) ? kKaPlusMass       : +1.0;
    if (pdg == kKaMinusPDG      ) return (returnIndex == MASSINDEX) ? kKaMinusMass      : -1.0;
    if (pdg == kKaZeroPDG       ) return (returnIndex == MASSINDEX) ? kKaZeroMass       :  0.0;
    //less or unlikely particles
    if (pdg == kRho770ZeroPDG   ) return (returnIndex == MASSINDEX) ? kRho770ZeroMass   :  0.0;
    if (pdg == kRho770PlusPDG   ) return (returnIndex == MASSINDEX) ? kRho770PlusMass   : +1.0;
    if (pdg == kRho770MinusPDG  ) return (returnIndex == MASSINDEX) ? kRho770MinusMass  : -1.0;
    if (pdg == kEtaPDG          ) return (returnIndex == MASSINDEX) ? kEtaMass          :  0.0;
    if (pdg == kEtaPrimePDG     ) return (returnIndex == MASSINDEX) ? kEtaPrimeMass     :  0.0;
    if (pdg == kDeltaMinusPDG   ) return (returnIndex == MASSINDEX) ? kDeltaMinusMass   : -1.0;
    if (pdg == kDeltaZeroPDG    ) return (returnIndex == MASSINDEX) ? kDeltaZeroMass    :  0.0;
    if (pdg == kDeltaPlusPDG    ) return (returnIndex == MASSINDEX) ? kDeltaPlusMass    : +1.0;
    if (pdg == kDeltaPlusPlusPDG) return (returnIndex == MASSINDEX) ? kDeltaPlusPlusMass: +2.0;
    if (pdg == kTauMinusPDG     ) return (returnIndex == MASSINDEX) ? kTauMinusMass     : -1.0;
    if (pdg == kTauPlusPDG      ) return (returnIndex == MASSINDEX) ? kTauMinusMass     : +1.0;
    if (pdg == komega782PDG     ) return (returnIndex == MASSINDEX) ? komega782Mass     :  0.0;
    std::cout << "ERROR: Particle with PDG " << pdg
              << " does NOT exist in HEPConstants::LoopOnParticles(Int_t, const Int_t)"
              << std::endl;
    //end
    return -9999;
}

//**************************************************
Double_t HEPConstants::GetParticleMass(Int_t pdg) const
//**************************************************
{
    return LoopOnParticles(pdg, MASSINDEX);
}

//**************************************************
Double_t HEPConstants::GetParticleCharge(Int_t pdg) const
//**************************************************
{
    return LoopOnParticles(pdg, CHARGEINDEX);
}

//**************************************************
Bool_t HEPConstants::IsParticleLepton(Int_t pdg) const
//**************************************************
{
    const Int_t abspdg = abs(pdg);
    //if not any of these leptons, then NOT a lepton
    if(abspdg != kElectronPDG &&
       abspdg != kMuMinusPDG &&
       abspdg != kTauMinusPDG &&
       abspdg != kNuEPDG &&
       abspdg != kNuMuPDG &&
       abspdg != kNuTauPDG
      )
        return kFALSE;
    return kTRUE;
}

//**************************************************
Bool_t HEPConstants::IsParticleMeson(Int_t pdg) const
//**************************************************
{
    if(pdg != kKaMinusPDG &&
       pdg != kRho770MinusPDG &&
       pdg != kPiMinusPDG &&
       pdg != kPiZeroPDG &&
       pdg != kRho770ZeroPDG &&
       pdg != kPiPlusPDG &&
       pdg != kRho770PlusPDG &&
       pdg != kEtaPDG &&
       pdg != kEtaPrimePDG &&
       pdg != kKaZeroPDG &&
       pdg != kKaZeroShortPDG &&
       pdg != kKaZeroLongPDG &&
       pdg != kKaPlusPDG
      )
        return kFALSE;
    return kTRUE;
}

//**************************************************
Bool_t HEPConstants::IsParticleBaryon(Int_t pdg) const
//**************************************************
{
    if(pdg != kNeutronPDG &&
       pdg != kProtonPDG &&
       pdg != kLambdaZeroPDG
      )
        return kFALSE;
    return kTRUE;
}

//**************************************************
Bool_t HEPConstants::IsParticleHadron(Int_t pdg) const
//**************************************************
{
    if(IsParticleBaryon(pdg))
        return kTRUE;
    if(IsParticleMeson(pdg))
        return kTRUE;
    return kFALSE;
}

//**************************************************
Bool_t HEPConstants::IsParticleNeutral(Int_t pdg) const
//**************************************************
{
    if(GetParticleCharge(pdg) == 0.)
        return kTRUE;
    return kFALSE;
}

//**************************************************
inline Bool_t HEPConstants::IsParticle(Int_t pdgA, Int_t pdgB) const
//**************************************************
{
    return pdgA == pdgB ? kTRUE : kFALSE;
}

//**************************************************
inline Bool_t HEPConstants::IsParticleAbs(Int_t pdgA, Int_t pdgB) const
//**************************************************
{
    return std::abs(pdgA)==std::abs(pdgB) ? kTRUE : kFALSE;
}

//**************************************************
Bool_t HEPConstants::IsParticleMuon(Int_t pdg, Double_t charge) const
//**************************************************
{
    if(charge == 0)
        return IsParticleAbs(pdg, kMuMinusPDG);
    else if(charge == 1)
        return IsParticle(pdg, kMuMinusPDG);
    else if(charge == -1)
        return IsParticle(pdg, kMuPlusPDG);
    else
        return kFALSE;
}

//**************************************************
Bool_t HEPConstants::IsParticleChargedPion(Int_t pdg, Double_t charge) const
//**************************************************
{
    if(charge == 0)
        return IsParticleAbs(pdg, kPiPlusPDG);
    else if(charge == 1)
        return IsParticlePiPlus(pdg);
    else if(charge == -1)
        return IsParticlePiMinus(pdg);
    else
        return kTRUE;
}

//**************************************************
inline Bool_t HEPConstants::IsParticlePiPlus(Int_t pdg) const
//**************************************************
{
    return IsParticle(pdg, kPiPlusPDG);
}

//**************************************************
inline Bool_t HEPConstants::IsParticlePiMinus(Int_t pdg) const
//**************************************************
{
    return IsParticle(pdg, kPiMinusPDG);
}

//**************************************************
inline Bool_t HEPConstants::IsParticlePiZero(Int_t pdg) const
//**************************************************
{
    return IsParticleAbs(pdg, kPiZeroPDG);
}

//**************************************************
inline Bool_t HEPConstants::IsParticleElectron(Int_t pdg) const
//**************************************************
{
    return IsParticle(pdg, kElectronPDG);
}

//**************************************************
inline Bool_t HEPConstants::IsParticlePositron(Int_t pdg) const
//**************************************************
{
    return IsParticle(pdg, kPositronPDG);
}

//**************************************************
inline Bool_t HEPConstants::IsParticleGamma(Int_t pdg) const
//**************************************************
{
    return IsParticleAbs(pdg, kGammaPDG);
}

//**************************************************
Bool_t HEPConstants::IsParticleEM(Int_t pdg) const
//**************************************************
{
    return IsParticleElectron(pdg) ||
           IsParticleGamma(pdg) ||
           IsParticlePositron(pdg);
}

//**************************************************
inline Bool_t HEPConstants::IsParticleProton(Int_t pdg) const
//**************************************************
{
    return IsParticle(pdg, kProtonPDG);
}

//**************************************************
inline Bool_t HEPConstants::IsParticleNeutron(Int_t pdg) const
//**************************************************
{
    return IsParticle(pdg, kNeutronPDG);
}
