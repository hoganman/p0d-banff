#ifndef DEFINECUTS_HXX
#define DEFINECUTS_HXX

/* A set of TCuts to be used with the RunSyst_New TTrees
 */

#include "TObject.h"
#include "TCut.h"
#include "TVector3.h"

class DefineCuts : public TObject {

public:
    DefineCuts();
    virtual ~DefineCuts(){}

    ///Passes the mu- selection
    TCut muMinusSelection;
    ///Passes the mu+ selection
    TCut muPlusSelection;
    ///In the reconstructed P0D fiducial volume
    TCut FV;
    ///In the true P0D fiducial volume
    TCut tFV;
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
    ///Is lepton candidate true ka+ or ka-
    TCut tLepKaon;
    ///Any lepton candidate species not listed above
    TCut tLepOther;
    ///Is parent neutrino a numu
    TCut tParNuMu;
    ///Is parent neutrino a numubar
    TCut tParNuMubar;
    ///Is parent neutrino a nue and nuebar
    TCut tParNuEs;
    /// The 3 dim coordinates of P0D FV
    TVector3 minFVcoords;
    TVector3 maxFVcoords;

    ClassDef(DefineCuts, 1)
};

#endif
