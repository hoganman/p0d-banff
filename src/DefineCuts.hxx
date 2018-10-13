#ifndef DEFINECUTS_HXX
#define DEFINECUTS_HXX

/* A set of TCuts to be used with the RunSyst_New TTrees
 */

#include "TObject.h"
#include "TCut.h"
#include "TVector3.h"
#include <vector>
#include "SampleId.hxx"
#include "PlottingSelectionInfo.hxx"
#if defined(__CINT__) || defined(__MAKECINT__)
#pragma link C++ nestedclasses;
#pragma link C++ class PlottingSelectionInfo+;
#pragma link C++ class std::vector<PlottingSelectionInfo*>+;
#endif

class DefineCuts : public TObject {

public:
    DefineCuts(){SetCuts();}
    virtual ~DefineCuts();

    //Bool_t IsSandCut(TCut cut);

    ///Passes the mu- selection
    TCut muMinusSelection;
    ///Passes the mu+ selection
    TCut muPlusInRHCSelection;
    ///
    TCut muMinusBkgInRHCSelection;
    ///In the reconstructed P0D fiducial volume
    TCut FV;
    ///In the true P0D fiducial volume
    TCut tFV;
    ///True out of FV
    TCut tOOFV;
    /// Reduce the FV to the TN208 value instead of TN73
    TCut FVTN208;
    /// Reduce the true FV to the TN208 value instead of TN73
    TCut tFVTN208;
    /// True out of FV for TN208
    TCut tOOFVTN208;
    ///Is lepton candidate true mu-
    TCut tLepMuMinus;
    ///Is lepton candidate true mu+
    TCut tLepMuPlus;
    ///Is lepton candidate true pi+
    TCut tLepPiPlus;
    ///Is lepton candidate true pi-
    TCut tLepPiMinus;
    ///Is lepton candidate true proton
    TCut tLepProton;
    ///Is lepton candidate true gamma
    TCut tLepGamma;
    ///Is lepton candidate true electron
    TCut tLepElectron;
    ///Is lepton candidate true positron
    TCut tLepPositron;
    ///Is lepton candidate true gamma, e-, or e+
    TCut tLepEMParticle;
    ///Any lepton candidate species not listed above
    TCut tLepOther;
    ///Is the particle coming from the sand?
    TCut tLepSand;
    ///Is parent neutrino a numu
    TCut tParNuMu;
    ///Is parent neutrino a numubar
    TCut tParNuMubar;
    ///Is parent neutrino a nue and nuebar
    TCut tParNuEs;
    ///Is parent neutrino not a numu, numubar, nue, nuebar
    TCut tParOther;
    ///Is parent neutrino in the sand?
    TCut tParNuSand;
    ///NEUT code is nu CC
    TCut tNEUTNuCCAny;
    ///NEUT code is Nu CCQE (not anti-nu CCQE)
    TCut tNEUTNuCCQE;
    ///NEUT code is Nu CC-2p2h (not anti-nu CCQE)
    TCut tNEUTNuCC2p2h;
    ///NEUT code is Nu CC-1pi (not anti-nu CC-1pi)
    TCut tNEUTNuCC1pi;
    ///NEUT code is Nu CC-Npi (not anti-nu CC-Npi)
    TCut tNEUTNuCCNpi;
    ///NEUT code is Nu CC-1K+ or Nu CC-1eta0 (not anti-nu CC-1K+ or anti-nu CC-1eto0)
    TCut tNEUTNuCCOther;
    ///NEUT code is Nu CC-DIS (not anti-nu CC-DIS)
    TCut tNEUTNuCCDIS;
    ///NEUT code is NC
    TCut tNEUTNC;
    ///NEUT code is any anti-nu CC
    TCut tNEUTAntiNuCCAny;
    ///NEUT code is Anti-nu CCQE (not nu CCQE)
    TCut tNEUTAntiNuCCQE;
    ///NEUT code is Anti-nu CC-2p2h (not nu CCQE)
    TCut tNEUTAntiNuCC2p2h;
    ///NEUT code is Anti-nu CC-1pi (not nu CC-1pi)
    TCut tNEUTAntiNuCC1pi;
    ///NEUT code is Anti-nu CC-Npi (not nu CC-Npi)
    TCut tNEUTAntiNuCCNpi;
    ///NEUT code is Anti-nu CC-1K+ or Anti-nu CC-1eta0 (not nu CC-1K+ or nu CC-1eto0)
    TCut tNEUTAntiNuCCOther;
    ///NEUT code is Anti-nu CC-DIS (not nu CC-DIS)
    TCut tNEUTAntiNuCCDIS;

    ///Is the parent interaction NC?
    /// The 3 dim coordinates of P0D FV
    TVector3 minFidVolCoords;
    TVector3 maxFidVolCoords;
    TVector3 minFidVolTN208Coords;
    TVector3 maxFidVolTN208Coords;
    TVector3 minSandCoords;
    TVector3 maxSandCoords;

    static const UInt_t NMAXPARTICLESELECTIONS = 10;
    static const UInt_t NMAXNEUTRINOSELECTIONS = 7;
    static const UInt_t NMAXNEUTSELECTIONS = 9;

    PlottingSelectionInfo* GetParticleSelection(const Int_t &index) const {return ParticleSelections[index];}
    PlottingSelectionInfo* GetNeutrinoSelection(const Int_t &index) const {return NeutrinoSelections[index];}
    PlottingSelectionInfo* GetNEUTSelection(const Int_t &index) const {return NEUTSelections[index];}

    std::vector<PlottingSelectionInfo*> ParticleSelections;
    std::vector<PlottingSelectionInfo*> NeutrinoSelections;
    std::vector<PlottingSelectionInfo*> NEUTSelections;

    void FillParticleSelections(const TString &name, const TString &title,
            const SampleId::SampleEnum &sampleID, const TCut &additionalCuts = "");

    void FillNeutrinoSelections(const TString &name, const TString &title,
            const SampleId::SampleEnum &sampleID, const TCut &additionalCuts = "");

    void FillNEUTNuSelections(const TString &name, const TString &title,
            const SampleId::SampleEnum &sampleID, const TCut &additionalCuts = "");

protected:
    void SetCuts();

public:
    ClassDef(DefineCuts, 1)
};

#endif
