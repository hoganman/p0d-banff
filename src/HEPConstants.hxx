#ifndef HEPCONSTANTS_HXX
#define HEPCONSTANTS_HXX

#include "TObject.h"
#include "TString.h"
#include <map>

class HEPConstants : public TObject {

public:
    HEPConstants(){Init();}
    virtual ~HEPConstants();

    typedef std::map<TString, Double_t> ConversionMap;
    ConversionMap convert;

    ///Convert different units like GeV
    ///Dimensions are preserved like energy or position
    Double_t Convert(TString unit) const;

    ///
    Double_t ConvertKineticEToMomentum(Double_t kinE, Double_t mass) const;

    ///
    Double_t ConvertMomentumToKineticE(Double_t mom, Double_t mass) const;

    ///
    Double_t ConvertMomentumToEnergy(Double_t mom, Double_t mass) const;

    ///
    Double_t GetParticleMass(Int_t pdg) const;

    ///
    Double_t GetParticleCharge(Int_t pdg) const;

    ///
    Bool_t IsParticleLepton(Int_t pdg) const;

    ///
    Bool_t IsParticleMeson(Int_t pdg) const;

    ///
    Bool_t IsParticleBaryon(Int_t pdg) const;

    ///
    Bool_t IsParticleHadron(Int_t pdg) const;

    ///
    Bool_t IsParticleNeutral(Int_t pdg) const;

    ///
    Bool_t IsParticle(Int_t pdgA, Int_t pdgB) const;

    ///
    Bool_t IsParticleAbs(Int_t pdgA, Int_t pdgB) const;

    ///
    Bool_t IsParticleMuon(Int_t pdg, Double_t charge) const;

    ///
    Bool_t IsParticleChargedPion(Int_t pdg, Double_t charge) const;

    ///
    Bool_t IsParticlePiPlus(Int_t pdg) const;

    ///
    Bool_t IsParticlePiMinus(Int_t pdg) const;

    ///
    Bool_t IsParticlePiZero(Int_t pdg) const;

    ///
    Bool_t IsParticleElectron(Int_t pdg) const;

    ///
    Bool_t IsParticlePositron(Int_t pdg) const;

    ///
    Bool_t IsParticleGamma(Int_t pdg) const;

    ///
    Bool_t IsParticleEM(Int_t pdg) const;

    ///
    Bool_t IsParticleProton(Int_t pdg) const;

    ///
    Bool_t IsParticleNeutron(Int_t pdg) const;

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

    Int_t    kGammaPDG;
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

    //************
    //Units
    //************
    Double_t MeV;
    Double_t MeV2;
    Double_t GeV;
    Double_t GeV2;
    Double_t mm;
    Double_t meter;

private:

    Int_t PDGINDEX;
    Int_t MASSINDEX;
    Int_t CHARGEINDEX;
    Double_t LoopOnParticles(Int_t pdg, const Int_t& returnIndex) const;
    void Init();

public:
    ClassDef(HEPConstants, 1)
};
#endif







