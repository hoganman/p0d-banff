#ifndef DEFINECUTS_HXX
#define DEFINECUTS_HXX

/* A set of TCuts to be used with the RunSyst_New TTrees
 */

#include "TVector3.h"
#include "SampleId.hxx"
#include "PlottingSelectionInfo.hxx"
#include <vector>
#if defined(__CINT__) || defined(__MAKECINT__)
#pragma link C++ nestedclasses;
#pragma link C++ class PlottingSelectionInfo+;
#pragma link C++ class std::vector<PlottingSelectionInfo*>+;
#endif

class DefineCuts : public TObject {

public:
    DefineCuts(){SetCuts();}
    virtual ~DefineCuts();

    ///Passes the mu- CC-Inc selection for P0D+TPC
    TCut muMinusCCSelection;
    ///Passes the mu+ in anti-nu mode CC-Inc selection for P0D+TPC
    TCut muPlusInRHCCCSelection;
    /// Passes the mu- in anti-nu mode CC-Inc selection for P0D+TPC
    TCut muMinusBkgInRHCCCSelection;

    ///Passes the mu- CC1Track selection for P0D+TPC
    TCut muMinusCC1TrackSelection;
    ///Passes the mu+ in anti-nu mode CC1Track selection for P0D+TPC
    TCut muPlusInRHCCC1TrackSelection;
    /// Passes the mu- in anti-nu mode CC1Track selection for P0D+TPC
    TCut muMinusBkgInRHCCC1TrackSelection;

    ///Passes the mu- CCNTracks selection for P0D+TPC
    TCut muMinusCCNTracksSelection;
    ///Passes the mu+ in anti-nu mode CCNTracks selection for P0D+TPC
    TCut muPlusInRHCCCNTracksSelection;
    /// Passes the mu- in anti-nu mode CCNTracks selection for P0D+TPC
    TCut muMinusBkgInRHCCCNTracksSelection;

    TCut anyP0DSelection;

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
    TCut tSand;
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

    ///The number of outgoing muons <= 0
    TCut tBKGTopology;
    ///The number of outgoing pions == 0, no other mesons and any number of baryons
    TCut tCC0PiTopology;
    ///The number of outgoing pions == 1, no other mesons and any number of baryons
    TCut tCC1PiTopology;
    ///Topology does not fit into any above category
    TCut tCCOtherTopology;

    ///Is the parent interaction NC?
    /// The 3 dim coordinates of P0D FV
    TVector3 minFidVolCoords;
    TVector3 maxFidVolCoords;
    TVector3 minFidVolTN208Coords;
    TVector3 maxFidVolTN208Coords;
    TVector3 minSandCoords;
    TVector3 maxSandCoords;

    static const UInt_t NMAXNEUTSELECTIONS = 11;
    static const UInt_t NMAXPARTICLESELECTIONS = 10;
    static const UInt_t NMAXNEUTRINOSELECTIONS = 7;
    static const UInt_t NMAXTOPOLOGYSELECTIONS = 7;

    std::vector<PlottingSelectionInfo*> ParticleSelections;
    std::vector<PlottingSelectionInfo*> NeutrinoSelections;
    std::vector<PlottingSelectionInfo*> NEUTNuSelections;
    std::vector<PlottingSelectionInfo*> NEUTAntiNuSelections;
    std::vector<PlottingSelectionInfo*> TopologySelections;

    void FillParticleSelections(const TString &name, const TString &title,
            const SampleId::SampleEnum &sampleID, const TCut &additionalCuts = "");

    void FillNeutrinoSelections(const TString &name, const TString &title,
            const SampleId::SampleEnum &sampleID, const TCut &additionalCuts = "");

    void FillNEUTNuSelections(const TString &name, const TString &title,
            const SampleId::SampleEnum &sampleID, const TCut &additionalCuts = "");

    void FillNEUTAntiNuSelections(const TString &name, const TString &title,
            const SampleId::SampleEnum &sampleID, const TCut &additionalCuts = "");

    void FillTopologySelections(const TString &name, const TString &title,
            const SampleId::SampleEnum &sampleID, const TCut &additionalCuts = "");

    ///These methods are PyROOT friendly
    PlottingSelectionInfo* GetParticleSelection(const UInt_t &index) const {return ParticleSelections[index];}
    PlottingSelectionInfo* GetNeutrinoSelection(const UInt_t &index) const {return NeutrinoSelections[index];}
    PlottingSelectionInfo* GetNEUTNuSelection(const UInt_t &index) const {return NEUTNuSelections[index];}
    PlottingSelectionInfo* GetNEUTAntiNuSelection(const UInt_t &index) const {return NEUTAntiNuSelections[index];}
    PlottingSelectionInfo* GetTopologySelection(const UInt_t &index) const {return TopologySelections[index];}

    ///These methods are NOT PyROOT friendly (CRASHES!)
    PlottingSelectionInfo** FillAndGetParticleSelections(const TString &name, const TString &title,
            const SampleId::SampleEnum &sampleID, const TCut &additionalCuts = "")
    {
        FillParticleSelections(name, title, sampleID, additionalCuts);
        return &ParticleSelections[0];
    }

    PlottingSelectionInfo** FillAndGetNeutrinoSelections(const TString &name, const TString &title,
            const SampleId::SampleEnum &sampleID, const TCut &additionalCuts = "")
    {
        FillNeutrinoSelections(name, title, sampleID, additionalCuts);
        return &NeutrinoSelections[0];
    }

    PlottingSelectionInfo** FillAndGetNEUTNuSelections(const TString &name, const TString &title,
            const SampleId::SampleEnum &sampleID, const TCut &additionalCuts = "")
    {
        FillNEUTNuSelections(name, title, sampleID, additionalCuts);
        return &NEUTNuSelections[0];
    }

    PlottingSelectionInfo** FillAndGetNEUTAntiNuSelections(const TString &name, const TString &title,
            const SampleId::SampleEnum &sampleID, const TCut &additionalCuts = "")
    {
        FillNEUTAntiNuSelections(name, title, sampleID, additionalCuts);
        return &NEUTAntiNuSelections[0];
    }

    PlottingSelectionInfo** FillAndGetTopologySelections(const TString &name, const TString &title,
            const SampleId::SampleEnum &sampleID, const TCut &additionalCuts = "")
    {
        FillTopologySelections(name, title, sampleID, additionalCuts);
        return &TopologySelections[0];
    }

    TCut GetNominalSelectionCut(const SampleId::SampleEnum &sampleID) const;


protected:
    void SetCuts();

public:
    ClassDef(DefineCuts, 1)
};

#endif
