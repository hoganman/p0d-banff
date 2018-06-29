#ifndef DEFINECUTS_HXX
#define DEFINECUTS_HXX

/* A set of TCuts to be used with the RunSyst_New TTrees
 */

#include "TObject.h"
#include "TCut.h"
#include "TVector3.h"

class DefineCuts : public TObject {

public:
    DefineCuts(){SetCuts();}
    virtual ~DefineCuts(){}

    Bool_t IsSandCut(TCut cut);

    TCut AndTCuts(TCut cutA, TCut cutB) const;
    TCut AndTCuts(TCut cutA, TString cutB) const {return AndTCuts(cutA, TCut(cutB.Data()));}

    ///Passes the mu- selection
    TCut muMinusSelection;
    ///Passes the mu+ selection
    TCut muPlusSelection;
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
    /// The 3 dim coordinates of P0D FV
    TVector3 minFidVolCoords;
    TVector3 maxFidVolCoords;
    TVector3 minFidVolTN208Coords;
    TVector3 maxFidVolTN208Coords;
    TVector3 minSandCoords;
    TVector3 maxSandCoords;



protected:
    void SetCuts();

public:
    ClassDef(DefineCuts, 1)
};

#endif
