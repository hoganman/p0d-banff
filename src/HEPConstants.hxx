#ifndef HEPCONSTANTS_HXX
#define HEPCONSTANTS_HXX

#include "TObject.h"
#include "TString.h"
#include "TMath.h"
#include <map>

class HEPConstants : public TObject {

public:
    HEPConstants(){Init();}
    virtual ~HEPConstants();

    typedef std::map<TString, Double_t> ConversionMap;
    ConversionMap convert;

    ///Convert different units like GeV
    ///Dimensions are preserved like energy or position
    Double_t Convert(const TString &unit) const;

    ///
    inline Double_t ConvertKineticEToMomentum(const Double_t &kinE, const Double_t &mass) const
    {
        return std::sqrt((kinE + mass) * (kinE + mass) - (mass * mass));
    }

    ///
    inline Double_t ConvertMomentumToKineticE(const Double_t &mom, const Double_t &mass) const
    {
        return sqrt(mom * mom + mass * mass) - mass;
    }

    ///
    inline Double_t ConvertMomentumToEnergy(const Double_t &mom, const Double_t &mass) const
    {
        return sqrt(mom * mom + mass * mass);
    }


    inline Bool_t IsValidParticle(const Int_t &pdg) const {return !TMath::IsNaN(pdg) && pdg != 0;}

    ///
    Double_t GetParticleMass(const Int_t &pdg) const;

    ///
    Double_t GetParticleCharge(const Int_t &pdg) const;

    ///
    Bool_t IsParticleLepton(const Int_t &pdg) const;

    ///
    Bool_t IsParticleMeson(const Int_t &pdg) const;

    ///
    Bool_t IsParticleBaryon(const Int_t &pdg) const;

    ///
    Bool_t IsParticleHadron(const Int_t &pdg) const;

    ///
    Bool_t IsParticleNeutral(const Int_t &pdg) const;

    ///
    Bool_t IsParticle(const Int_t &pdgA, Int_t pdgB) const;

    ///
    Bool_t IsParticleAbs(const Int_t &pdgA, Int_t pdgB) const;

    ///
    Bool_t IsParticleMuon(const Int_t &pdg, Double_t charge) const;

    ///
    Bool_t IsParticleChargedPion(const Int_t &pdg, Double_t charge) const;

    ///
    Bool_t IsParticlePiPlus(const Int_t &pdg) const;

    ///
    Bool_t IsParticlePiMinus(const Int_t &pdg) const;

    ///
    Bool_t IsParticlePiZero(const Int_t &pdg) const;

    ///
    Bool_t IsParticleElectron(const Int_t &pdg) const;

    ///
    Bool_t IsParticlePositron(const Int_t &pdg) const;

    ///
    Bool_t IsParticleGamma(const Int_t &pdg) const;

    ///
    Bool_t IsParticleEM(const Int_t &pdg) const;

    ///
    Bool_t IsParticleProton(const Int_t &pdg) const;

    ///
    Bool_t IsParticleNeutron(const Int_t &pdg) const;

    inline Bool_t IsValidNEUTCode(const Int_t &code) const {return !TMath::IsNaN(code) && code != 0 && abs(code) < kNEUT_Max;}

    Bool_t IsNEUTCodeNuCCQE(const Int_t &code) const;

    Bool_t IsNEUTCodeNuCC1Pi(const Int_t &code) const;

    Bool_t IsNEUTCodeNuCCNPi(const Int_t &code) const;

    Bool_t IsNEUTCodeNuCC1KaOrEta(const Int_t &code) const;

    Bool_t IsNEUTCodeNuCCDIS(const Int_t &code) const;

    Bool_t IsNEUTCodeNuNC(const Int_t &code) const;

    Bool_t IsNEUTCodeNuAny(const Int_t &code) const;

    Bool_t IsNEUTCodeAntiNuCCQE(const Int_t &code) const;

    Bool_t IsNEUTCodeAntiNuCC1Pi(const Int_t &code) const;

    Bool_t IsNEUTCodeAntiNuCCNPi(const Int_t &code) const;

    Bool_t IsNEUTCodeAntiNuCC1KaOrEta(const Int_t &code) const;

    Bool_t IsNEUTCodeAntiNuCCDIS(const Int_t &code) const;

    Bool_t IsNEUTCodeAntiNuNC(const Int_t &code) const;

    Bool_t IsNEUTCodeAntiNuAny(const Int_t &code) const;

    Bool_t IsNEUTCodeNuCC1Gamma(const Int_t &code) const;

    Bool_t IsNEUTCodeAntiNuCC1Gamma(const Int_t &code) const;

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

    // A number larger than anything NEUT uses
    static const Int_t kNEUT_Max = 101;
    // ***** NEU CHARGED CURRENT *****
    // -- ELASTIC --
    Int_t kNEUTNu_CCQE; //1 : NEU,N --> LEPTON-,P
    Int_t kNEUTNu_2p2h; //2 : NEU,N+X --> LEPTON-,P+X  (X=(N or P))

    // -- SINGLE PI FROM DELTA RESONANCE --
    Int_t kNEUTNuP_CC1PiPlusP; //11 : NEU,P --> LEPTON-,P,PI+
    Int_t kNEUTNuN_CC1PiZeroP; //12 : NEU,N --> LEPTON-,P,PI0
    Int_t kNEUTNuN_CC1PiPlusN; //13 : NEU,N --> LEPTON-,N,PI+
    Int_t kNEUTNuO_CC1PiPlusO; //16 : NEU,O(16) --> LEPTON-,O(16),PI+

    // -- SINGLE GAMMA FROM DELTA RESONANCE --
    Int_t kNEUTNuN_CC1GammaP; //17 : NEU,N --> LEPTON-,P,GAMMA

    // -- MULTI PI (1.3 < W < 2.0 GeV) --
    Int_t kNEUTNu_CCMultiPi; //21 : NEU,(N OR P) --> LEPTON-,(N OR P),MULTI PI

    // -- SINGLE ETA FROM DELTA RESONANCE --
    Int_t kNEUTNuN_CC1EtaZeroP; //22 : NEU,N --> LEPTON-,P,ETA0

    // -- SINGLE K FROM DELTA RESONANCE --
    Int_t kNEUTNuN_CC1KaPlusLambda; //23 : NEU,N --> LEPTON-,LAMBDA,K+

    // -- DEEP INELASTIC (2.0 GeV < W , JET set) --
    Int_t kNEUTNu_CCDIS; // 46 : NEU,(N OR P) --> NEU,(N OR P),MESONS

    // ***** NEU NEUTAL CURRENT *****
    // -- NC: SINGLE PI FROM DELTA RESONANCE --
    Int_t kNEUTNuN_NC1PiZeroN; //31 : NEU,N --> NEU,N,PI0
    Int_t kNEUTNuP_NC1PiZeroP; //32 : NEU,P --> NEU,P,PI0
    Int_t kNEUTNuN_NC1PiMinusP; //33 : NEU,N --> NEU,P,PI-
    Int_t kNEUTNuP_NC1PiPlusN; //34 : NEU,P --> NEU,N,PI+

    Int_t kNEUTNuO_NC1PiZeroO; //36 : NEU,O(16) --> NEU,O(16),PI0

    // -- SINGLE GAMMA FROM DELTA RESONANCE --
    Int_t kNEUTNuN_NC1GammaN; //38 : NEU,N --> NEU,N,GAMMA
    Int_t kNEUTNuP_NC1GammaP; //39 : NEU,P --> NEU,P,GAMMA

    // -- MULTI PI (1.3 GeV < W < 2.0 GeV) --
    Int_t kNEUTNu_NCMultiPi; //41 : NEU,(N OR P) --> NEU,(N OR P),MULTI PI

    // -- SINGLE ETA FROM DELTA RESONANCE --
    Int_t kNEUTNuN_NC1EtaZeroN; //42 : NEU,N --> NEU,N,ETA0
    Int_t kNEUTNuP_NC1EtaZeroP; //43 : NEU,P --> NEU,P,ETA0

    // -- SINGLE  K  FROM DELTA RESONANCE --
    Int_t kNEUTNuN_NC1KaZeroLambda; //44 : NEU,N --> NEU,LAMBDA,K0
    Int_t kNEUTNuP_NC1KaPlusLambda; //45 : NEU,P --> NEU,LAMBDA,K+

    // -- DEEP INELASTIC (2.0 GeV < W , JET set) --
    Int_t kNEUTNu_NCDIS; //46 : NEU,(N OR P) --> NEU,(N OR P),MESONS

    // -- ELASTIC --
    Int_t kNEUTNuP_NCQE; //51 : NEU,P --> NEU,P
    Int_t kNEUTNuN_NCQE; //52 : NEU,N --> NEU,N

    // ***** NEUBAR CHARGED CURRENT *****
    // -- ELASTIC --
    Int_t kNEUTAntiNu_CCQE; //-1 : NEUBAR,N --> LEPTON-,P
    Int_t kNEUTAntiNu_2p2h; //-2 : NEUBAR,N+X --> LEPTON-,P+X  (X=(N or P))

    // -- SINGLE PI FROM DELTA RESONANCE --
    Int_t kNEUTAntiNuP_CC1PiPlusP; //-11 : NEUBAR,P --> LEPTON-,P,PI+
    Int_t kNEUTAntiNuN_CC1PiZeroP; //-12 : NEUBAR,N --> LEPTON-,P,PI0
    Int_t kNEUTAntiNuN_CC1PiPlusN; //-13 : NEUBAR,N --> LEPTON-,N,PI+
    Int_t kNEUTAntiNuO_CC1PiPlusO; //-16 : NEUBAR,O(16) --> LEPTON-,O(16),PI+

    // -- SINGLE GAMMA FROM DELTA RESONANCE --
    Int_t kNEUTAntiNuN_CC1GammaP; //-17 : NEUBAR,N --> LEPTON-,P,GAMMA

    // -- MULTI PI (1.3 < W < 2.0 GeV) --
    Int_t kNEUTAntiNu_CCMultiPi; //-21 : NEUBAR,(N OR P) --> LEPTON-,(N OR P),MULTI PI

    // -- SINGLE ETA FROM DELTA RESONANCE --
    Int_t kNEUTAntiNuN_CC1EtaZeroP; //-22 : NEUBAR,N --> LEPTON-,P,ETA0

    // -- SINGLE K FROM DELTA RESONANCE --
    Int_t kNEUTAntiNuN_CC1KaPlusLambda; //-23 : NEUBAR,N --> LEPTON-,LAMBDA,K+

    // -- DEEP INELASTIC (2.0 GeV < W , JET set) --
    Int_t kNEUTAntiNu_CCDIS; //- 46 : NEUBAR,(N OR P) --> NEUBAR,(N OR P),MESONS

    // ***** NEUBAR NEUTAL CURRENT *****
    // -- NC: SINGLE PI FROM DELTA RESONANCE --
    Int_t kNEUTAntiNuN_NC1PiZeroN; //-31 : NEUBAR,N --> NEUBAR,N,PI0
    Int_t kNEUTAntiNuP_NC1PiZeroP; //-32 : NEUBAR,P --> NEUBAR,P,PI0
    Int_t kNEUTAntiNuN_NC1PiMinusP; //-33 : NEUBAR,N --> NEUBAR,P,PI-
    Int_t kNEUTAntiNuP_NC1PiPlusN; //-34 : NEUBAR,P --> NEUBAR,N,PI+

    Int_t kNEUTAntiNuO_NC1PiZeroO; //-36 : NEUBAR,O(16) --> NEUBAR,O(16),PI0

    // -- SINGLE GAMMA FROM DELTA RESONANCE --
    Int_t kNEUTAntiNuN_NC1GammaN; //-38 : NEUBAR,N --> NEUBAR,N,GAMMA
    Int_t kNEUTAntiNuP_NC1GammaP; //-39 : NEUBAR,P --> NEUBAR,P,GAMMA

    // -- MULTI PI (1.3 GeV < W < 2.0 GeV) --
    Int_t kNEUTAntiNu_NCMultiPi; //-41 : NEUBAR,(N OR P) --> NEUBAR,(N OR P),MULTI PI

    // -- SINGLE ETA FROM DELTA RESONANCE --
    Int_t kNEUTAntiNuN_NC1EtaZeroN; //-42 : NEUBAR,N --> NEUBAR,N,ETA0
    Int_t kNEUTAntiNuP_NC1EtaZeroP; //-43 : NEUBAR,P --> NEUBAR,P,ETA0

    // -- SINGLE  K  FROM DELTA RESONANCE --
    Int_t kNEUTAntiNuN_NC1KaZeroLambda; //-44 : NEUBAR,N --> NEUBAR,LAMBDA,K0
    Int_t kNEUTAntiNuP_NC1KaPlusLambda; //-45 : NEUBAR,P --> NEUBAR,LAMBDA,K+

    // -- DEEP INELASTIC (2.0 GeV < W , JET set) --
    Int_t kNEUTAntiNu_NCDIS; //-46 : NEUBAR,(N OR P) --> NEUBAR,(N OR P),MESONS

    // -- ELASTIC --
    Int_t kNEUTAntiNuP_NCQE; //-51 : NEUBAR,P --> NEUBAR,P
    Int_t kNEUTAntiNuN_NCQE; //-52 : NEUBAR,N --> NEUBAR,N


    /// Enumeration of all particles
    enum ParticleEnum {
         kBaryons,
         kMesons,
         kLeptons,
         kNeutrinos,
         kAntiNeutrinos,
         kPions,
         kKaons,
         kNuclei,          // Genie lists also the remnant nuclei in StdHep# variables
         kPseudoParticles, // Genie *pseudo-particles", such as 'hadron blob' etc, see $GENIE/data/evgen/catalogues/pdg/genie_pdg_table.txt
         kOthers,

         kProton,
         kAntiProton,
         kNeutron,
         kAntiNeutron,
         kElectron,
         kPositron, kAntiElectron = kPositron,
         kMuon, kMuNeg = kMuon, kMuMinus = kMuon, kMuonNeg = kMuon, kMuonMinus = kMuon,
         kAntiMuon, kMuPos = kAntiMuon, kMuPlus = kAntiMuon, kMuonPos = kAntiMuon, kMuonPlus = kAntiMuon,
         kTau, kTauNeg = kTau, kTauMinus = kTau,
         kAntiTau, kTauPos = kAntiTau, kTauPlus = kAntiTau,
         kPi0, kPion0 = kPi0, kPiZero = kPi0, kPionZero = kPi0,
         kPiPos, kPiPlus = kPiPos, kPionPos = kPiPos, kPionPlus = kPiPos,
         kPiNeg, kPiMinus = kPiNeg, kPionNeg = kPiNeg, kPionMinus = kPiNeg,
         kK0, kKaon0 = kK0, kKZero = kK0, kKaonZero = kK0,
         kAntiK0, kAntiKaon0 = kAntiK0, kAntiKZero = kAntiK0, kAntiKaonZero = kAntiK0,
         kK0L, kKaonZeroLong = kK0L, kK0Long = kK0L, kKaon0Long = kK0L, kKZeroLong = kK0L, kKaonZeroL = kK0L, kKaon0L = kK0L, kKZeroL = kK0L,
         kK0S, kKaonZeroShort = kK0S, kK0Short = kK0S, kKaon0Short = kK0S, kKZeroShort = kK0S, kKaonZeroS = kK0S, kKaon0S = kK0S, kKZeroS = kK0S,
         kKPos, kKPlus = kKPos, kKaonPos = kKPos, kKaonPlus = kKPos,
         kKNeg, kKMinus = kKNeg, kKaonNeg = kKNeg, kKaonMinus = kKNeg,
         kEta, kEtaZero = kEta, kEta0 = kEta,
         kRho0, kRhoZero = kRho0,
         kPhoton,
         kSigmaPos, kSigmaPlus=kSigmaPos,
         kSigmaNeg, kSigmaMinus=kSigmaNeg,
         kSigma0, kSigmaZero=kSigma0,
         kLast // used to give the size to AnaTrueVertex::NPrimaryParticle array and to loop over this enum
    };

private:

    Int_t PDGINDEX;
    Int_t MASSINDEX;
    Int_t CHARGEINDEX;
    Double_t LoopOnParticles(const Int_t &pdg, const Int_t& returnIndex) const;
    void Init();

public:
    ClassDef(HEPConstants, 1)
};
#endif







