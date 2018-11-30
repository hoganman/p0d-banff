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
    convert["cos(#theta)"] = 1.;
    convert["cos(theta)"] = 1.;
    convert["cos(q)"] = 1.;
    convert["cos #theta"] = 1.;

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

    // ***** NEU CHARGED CURRENT *****
    // -- ELASTIC --
    kNEUTNu_CCQE = 1; // NEU,N --> LEPTON-,P
    kNEUTNu_2p2h = 2; // NEU,N+X --> LEPTON-,P+X  (X=(N or P))

    // -- SINGLE PI FROM DELTA RESONANCE --
    kNEUTNuP_CC1PiPlusP = 11; // NEU,P --> LEPTON-,P,PI+
    kNEUTNuN_CC1PiZeroP = 12; // NEU,N --> LEPTON-,P,PI0
    kNEUTNuN_CC1PiPlusN = 13; // NEU,N --> LEPTON-,N,PI+

    // -- CC COHERENT --
    kNEUTNuO_CC1PiPlusO = 16; // NEU,O(16) --> LEPTON-,O(16),PI+

    // -- SINGLE GAMMA FROM DELTA RESONANCE --
    kNEUTNuN_CC1GammaP = 17; // NEU,N --> LEPTON-,P,GAMMA

    // -- MULTI PI (1.3 < W < 2.0 GeV) --
    kNEUTNu_CCMultiPi = 21; // NEU,(N OR P) --> LEPTON-,(N OR P),MULTI PI

    // -- SINGLE ETA FROM DELTA RESONANCE --
    kNEUTNuN_CC1EtaZeroP = 22; // NEU,N --> LEPTON-,P,ETA0

    // -- SINGLE K FROM DELTA RESONANCE --
    kNEUTNuN_CC1KaPlusLambda = 23; // NEU,N --> LEPTON-,LAMBDA,K+

    // -- DEEP INELASTIC (2.0 GeV < W , JET set) --
    kNEUTNu_CCDIS =  26; // NEU,(N OR P) --> LEPTON,(N OR P),MESONS

    // ***** NEU NEUTAL CURRENT *****
    // -- NC: SINGLE PI FROM DELTA RESONANCE --
    kNEUTNuN_NC1PiZeroN = 31; // NEU,N --> NEU,N,PI0
    kNEUTNuP_NC1PiZeroP = 32; // NEU,P --> NEU,P,PI0
    kNEUTNuN_NC1PiMinusP = 33; // NEU,N --> NEU,P,PI-
    kNEUTNuP_NC1PiPlusN = 34; // NEU,P --> NEU,N,PI+

    kNEUTNuO_NC1PiZeroO = 36; // NEU,O(16) --> NEU,O(16),PI0

    // -- SINGLE GAMMA FROM DELTA RESONANCE --
    kNEUTNuN_NC1GammaN = 38; // NEU,N --> NEU,N,GAMMA
    kNEUTNuP_NC1GammaP = 39; // NEU,P --> NEU,P,GAMMA

    // -- MULTI PI (1.3 GeV < W < 2.0 GeV) --
    kNEUTNu_NCMultiPi = 41; // NEU,(N OR P) --> NEU,(N OR P),MULTI PI

    // -- SINGLE ETA FROM DELTA RESONANCE --
    kNEUTNuN_NC1EtaZeroN = 42; // NEU,N --> NEU,N,ETA0
    kNEUTNuP_NC1EtaZeroP = 43; // NEU,P --> NEU,P,ETA0

    // -- SINGLE  K  FROM DELTA RESONANCE --
    kNEUTNuN_NC1KaZeroLambda = 44; // NEU,N --> NEU,LAMBDA,K0
    kNEUTNuP_NC1KaPlusLambda = 45; // NEU,P --> NEU,LAMBDA,K+

    // -- DEEP INELASTIC (2.0 GeV < W , JET set) --
    kNEUTNu_NCDIS = 46; // NEU,(N OR P) --> NEU,(N OR P),MESONS

    // -- ELASTIC --
    kNEUTNuP_NCQE = 51; // NEU,P --> NEU,P
    kNEUTNuN_NCQE = 52; // NEU,N --> NEU,N


    // ***** NEUBAR CHARGED CURRENT *****
    // -- ELASTIC --
    kNEUTAntiNu_CCQE = -1; // NEUBAR,N --> LEPTON-,P
    kNEUTAntiNu_2p2h = -2; // NEUBAR,N+X --> LEPTON-,P+X  (X=(N or P))

    // -- SINGLE PI FROM DELTA RESONANCE --
    kNEUTAntiNuP_CC1PiPlusP = -11; // NEUBAR,P --> LEPTON-,P,PI+
    kNEUTAntiNuN_CC1PiZeroP = -12; // NEUBAR,N --> LEPTON-,P,PI0
    kNEUTAntiNuN_CC1PiPlusN = -13; // NEUBAR,N --> LEPTON-,N,PI+

    // -- CC COHERENT --
    kNEUTAntiNuO_CC1PiPlusO = -16; // NEUBAR,O(16) --> LEPTON-,O(16),PI+

    // -- SINGLE GAMMA FROM DELTA RESONANCE --
    kNEUTAntiNuN_CC1GammaP = -17; // NEUBAR,N --> LEPTON-,P,GAMMA

    // -- MULTI PI (1.3 < W < 2.0 GeV) --
    kNEUTAntiNu_CCMultiPi = -21; // NEUBAR,(N OR P) --> LEPTON-,(N OR P),MULTI PI

    // -- SINGLE ETA FROM DELTA RESONANCE --
    kNEUTAntiNuN_CC1EtaZeroP = -22; // NEUBAR,N --> LEPTON-,P,ETA0

    // -- SINGLE K FROM DELTA RESONANCE --
    kNEUTAntiNuN_CC1KaPlusLambda = -23; // NEUBAR,N --> LEPTON-,LAMBDA,K+

    // -- DEEP INELASTIC (2.0 GeV < W , JET set) --
    kNEUTAntiNu_CCDIS = -26; // NEUBAR,(N OR P) --> NEUBAR,(N OR P),MESONS

    // ***** NEUBAR NEUTAL CURRENT *****
    // -- NC: SINGLE PI FROM DELTA RESONANCE --
    kNEUTAntiNuN_NC1PiZeroN = -31; // NEUBAR,N --> NEUBAR,N,PI0
    kNEUTAntiNuP_NC1PiZeroP = -32; // NEUBAR,P --> NEUBAR,P,PI0
    kNEUTAntiNuN_NC1PiMinusP = -33; // NEUBAR,N --> NEUBAR,P,PI-
    kNEUTAntiNuP_NC1PiPlusN = -34; // NEUBAR,P --> NEUBAR,N,PI+

    kNEUTAntiNuO_NC1PiZeroO = -36; // NEUBAR,O(16) --> NEUBAR,O(16),PI0

    // -- SINGLE GAMMA FROM DELTA RESONANCE --
    kNEUTAntiNuN_NC1GammaN = -38; // NEUBAR,N --> NEUBAR,N,GAMMA
    kNEUTAntiNuP_NC1GammaP = -39; // NEUBAR,P --> NEUBAR,P,GAMMA

    // -- MULTI PI (1.3 GeV < W < 2.0 GeV) --
    kNEUTAntiNu_NCMultiPi = -41; // NEUBAR,(N OR P) --> NEUBAR,(N OR P),MULTI PI

    // -- SINGLE ETA FROM DELTA RESONANCE --
    kNEUTAntiNuN_NC1EtaZeroN = -42; // NEUBAR,N --> NEUBAR,N,ETA0
    kNEUTAntiNuP_NC1EtaZeroP = -43; // NEUBAR,P --> NEUBAR,P,ETA0

    // -- SINGLE  K  FROM DELTA RESONANCE --
    kNEUTAntiNuN_NC1KaZeroLambda = -44; // NEUBAR,N --> NEUBAR,LAMBDA,K0
    kNEUTAntiNuP_NC1KaPlusLambda = -45; // NEUBAR,P --> NEUBAR,LAMBDA,K+

    // -- DEEP INELASTIC (2.0 GeV < W , JET set) --
    kNEUTAntiNu_NCDIS = -46; // NEUBAR,(N OR P) --> NEUBAR,(N OR P),MESONS

    // -- ELASTIC --
    kNEUTAntiNuP_NCQE = -51; // NEUBAR,P --> NEUBAR,P
    kNEUTAntiNuN_NCQE = -52; // NEUBAR,N --> NEUBAR,N

}

//**************************************************
Bool_t HEPConstants::IsNEUTCodeNuCCQE(const Int_t &code) const
//**************************************************
{
    if(!IsValidNEUTCode(code))
        return kFALSE;
    return code == kNEUTNu_CCQE || code == kNEUTNu_2p2h;
}

//**************************************************
Bool_t HEPConstants::IsNEUTCodeNuCC1Pi(const Int_t &code) const
//**************************************************
{
    if(!IsValidNEUTCode(code))
        return kFALSE;
    if(code == kNEUTNuP_CC1PiPlusP) return kTRUE; //11 : NEU,P --> LEPTON-,P,PI+
    if(code == kNEUTNuN_CC1PiZeroP) return kTRUE; //12 : NEU,N --> LEPTON-,P,PI0
    if(code == kNEUTNuN_CC1PiPlusN) return kTRUE; //13 : NEU,N --> LEPTON-,N,PI+
    if(code == kNEUTNuO_CC1PiPlusO) return kTRUE; //16 : NEU,O(16) --> LEPTON-,O(16),PI+
    return kFALSE;
}

//**************************************************
Bool_t HEPConstants::IsNEUTCodeNuCCNPi(const Int_t &code) const
//**************************************************
{
    if(!IsValidNEUTCode(code))
        return kFALSE;
    return code == kNEUTNu_CCMultiPi;
}

//**************************************************
Bool_t HEPConstants::IsNEUTCodeNuCC1KaOrEta(const Int_t &code) const
//**************************************************
{
    if(!IsValidNEUTCode(code))
        return kFALSE;
    return code == kNEUTNuN_CC1KaPlusLambda || kNEUTNuN_CC1EtaZeroP;
}

//**************************************************
Bool_t HEPConstants::IsNEUTCodeNuCCDIS(const Int_t &code) const
//**************************************************
{
    if(!IsValidNEUTCode(code))
        return kFALSE;
    return code == kNEUTNu_CCDIS;
}

//**************************************************
Bool_t HEPConstants::IsNEUTCodeNuNC(const Int_t &code) const
//**************************************************
{
    if(!IsValidNEUTCode(code))
        return kFALSE;
    return code >= kNEUTNuN_NC1PiZeroN;
}

//**************************************************
Bool_t HEPConstants::IsNEUTCodeAntiNuCCQE(const Int_t &code) const
//**************************************************
{
    if(!IsValidNEUTCode(code))
        return kFALSE;
    return code == kNEUTAntiNu_CCQE || code == kNEUTAntiNu_2p2h;
}

//**************************************************
Bool_t HEPConstants::IsNEUTCodeAntiNuCC1Pi(const Int_t &code) const
//**************************************************
{
    if(!IsValidNEUTCode(code))
        return kFALSE;
    if(code == kNEUTAntiNuP_CC1PiPlusP) return kTRUE;
    if(code == kNEUTAntiNuN_CC1PiZeroP) return kTRUE;
    if(code == kNEUTAntiNuN_CC1PiPlusN) return kTRUE;
    if(code == kNEUTAntiNuO_CC1PiPlusO) return kTRUE;
    return kFALSE;
}

//**************************************************
Bool_t HEPConstants::IsNEUTCodeAntiNuCCNPi(const Int_t &code) const
//**************************************************
{
    if(!IsValidNEUTCode(code))
        return kFALSE;
    return code == kNEUTAntiNu_CCMultiPi;
}

//**************************************************
Bool_t HEPConstants::IsNEUTCodeAntiNuCC1KaOrEta(const Int_t &code) const
//**************************************************
{
    if(!IsValidNEUTCode(code))
        return kFALSE;
    return code == kNEUTAntiNuN_CC1KaPlusLambda || kNEUTAntiNuN_CC1EtaZeroP;
}

//**************************************************
Bool_t HEPConstants::IsNEUTCodeAntiNuCCDIS(const Int_t &code) const
//**************************************************
{
    if(!IsValidNEUTCode(code))
        return kFALSE;
    return code == kNEUTAntiNu_CCDIS;
}

//**************************************************
Bool_t HEPConstants::IsNEUTCodeAntiNuNC(const Int_t &code) const
//**************************************************
{
    if(!IsValidNEUTCode(code))
        return kFALSE;
    return code <= kNEUTAntiNuN_NC1PiZeroN;
}


//**************************************************
Bool_t HEPConstants::IsNEUTCodeNuAny(const Int_t &code) const
//**************************************************
{
    if(!IsValidNEUTCode(code))
        return kFALSE;
    return code >= kNEUTNu_CCQE;
}

//**************************************************
Bool_t HEPConstants::IsNEUTCodeAntiNuAny(const Int_t &code) const
//**************************************************
{
    if(!IsValidNEUTCode(code))
        return kFALSE;
    return code <= kNEUTAntiNu_CCQE;
}

//**************************************************
Double_t HEPConstants::Convert(const TString &unit) const {
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
Double_t HEPConstants::LoopOnParticles(const Int_t &pdg, const Int_t& returnIndex) const
//**************************************************
{
    if(!IsValidParticle(pdg))
        return kFALSE;
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
Double_t HEPConstants::GetParticleMass(const Int_t &pdg) const
//**************************************************
{
    if(!IsValidParticle(pdg))
        return -9999;
    return LoopOnParticles(pdg, MASSINDEX);
}

//**************************************************
Double_t HEPConstants::GetParticleCharge(const Int_t &pdg) const
//**************************************************
{
    if(!IsValidParticle(pdg))
        return -9999;
    return LoopOnParticles(pdg, CHARGEINDEX);
}

//**************************************************
Bool_t HEPConstants::IsParticleLepton(const Int_t &pdg) const
//**************************************************
{
    if(!IsValidParticle(pdg))
        return kFALSE;
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
Bool_t HEPConstants::IsParticleMeson(const Int_t &pdg) const
//**************************************************
{
    if(!IsValidParticle(pdg))
        return kFALSE;
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
Bool_t HEPConstants::IsParticleBaryon(const Int_t &pdg) const
//**************************************************
{
    if(!IsValidParticle(pdg))
        return kFALSE;
    if(pdg != kNeutronPDG &&
       pdg != kProtonPDG &&
       pdg != kLambdaZeroPDG
      )
        return kFALSE;
    return kTRUE;
}

//**************************************************
Bool_t HEPConstants::IsParticleHadron(const Int_t &pdg) const
//**************************************************
{
    if(!IsValidParticle(pdg))
        return kFALSE;
    if(IsParticleBaryon(pdg))
        return kTRUE;
    if(IsParticleMeson(pdg))
        return kTRUE;
    return kFALSE;
}

//**************************************************
Bool_t HEPConstants::IsParticleNeutral(const Int_t &pdg) const
//**************************************************
{
    if(!IsValidParticle(pdg))
        return kFALSE;
    if(GetParticleCharge(pdg) == 0.)
        return kTRUE;
    return kFALSE;
}

//**************************************************
inline Bool_t HEPConstants::IsParticle(const Int_t &pdgA, Int_t pdgB) const
//**************************************************
{
    return pdgA == pdgB ? kTRUE : kFALSE;
}

//**************************************************
inline Bool_t HEPConstants::IsParticleAbs(const Int_t &pdgA, Int_t pdgB) const
//**************************************************
{
    return std::abs(pdgA)==std::abs(pdgB) ? kTRUE : kFALSE;
}

//**************************************************
Bool_t HEPConstants::IsParticleMuon(const Int_t &pdg, Double_t charge) const
//**************************************************
{
    if(!IsValidParticle(pdg))
        return kFALSE;
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
Bool_t HEPConstants::IsParticleChargedPion(const Int_t &pdg, Double_t charge) const
//**************************************************
{
    if(!IsValidParticle(pdg))
        return kFALSE;
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
inline Bool_t HEPConstants::IsParticlePiPlus(const Int_t &pdg) const
//**************************************************
{
    if(!IsValidParticle(pdg))
        return kFALSE;
    return IsParticle(pdg, kPiPlusPDG);
}

//**************************************************
inline Bool_t HEPConstants::IsParticlePiMinus(const Int_t &pdg) const
//**************************************************
{
    if(!IsValidParticle(pdg))
        return kFALSE;
    return IsParticle(pdg, kPiMinusPDG);
}

//**************************************************
inline Bool_t HEPConstants::IsParticlePiZero(const Int_t &pdg) const
//**************************************************
{
    if(!IsValidParticle(pdg))
        return kFALSE;
    return IsParticleAbs(pdg, kPiZeroPDG);
}

//**************************************************
inline Bool_t HEPConstants::IsParticleElectron(const Int_t &pdg) const
//**************************************************
{
    if(!IsValidParticle(pdg))
        return kFALSE;
    return IsParticle(pdg, kElectronPDG);
}

//**************************************************
inline Bool_t HEPConstants::IsParticlePositron(const Int_t &pdg) const
//**************************************************
{
    if(!IsValidParticle(pdg))
        return kFALSE;
    return IsParticle(pdg, kPositronPDG);
}

//**************************************************
inline Bool_t HEPConstants::IsParticleGamma(const Int_t &pdg) const
//**************************************************
{
    if(!IsValidParticle(pdg))
        return kFALSE;
    return IsParticleAbs(pdg, kGammaPDG);
}

//**************************************************
Bool_t HEPConstants::IsParticleEM(const Int_t &pdg) const
//**************************************************
{
    if(!IsValidParticle(pdg))
        return kFALSE;
    return IsParticleElectron(pdg) ||
           IsParticleGamma(pdg) ||
           IsParticlePositron(pdg);
}

//**************************************************
inline Bool_t HEPConstants::IsParticleProton(const Int_t &pdg) const
//**************************************************
{
    if(!IsValidParticle(pdg))
        return kFALSE;
    return IsParticle(pdg, kProtonPDG);
}

//**************************************************
inline Bool_t HEPConstants::IsParticleNeutron(const Int_t &pdg) const
//**************************************************
{
    if(!IsValidParticle(pdg))
        return kFALSE;
    return IsParticle(pdg, kNeutronPDG);
}
