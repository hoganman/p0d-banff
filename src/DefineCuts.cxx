#define DEFINECUTS_CXX

#include "DefineCuts.hxx"
#include "TString.h"
#include "SampleId.hxx"
#include "HEPConstants.hxx"
ClassImp(DefineCuts)

//**************************************************
void DefineCuts::SetCuts()
//**************************************************
{

    const SampleId samples;
    const HEPConstants pdg;
    minFidVolCoords.SetXYZ(-836., -871., -2969.);  // in mm
    maxFidVolCoords.SetXYZ(+765., +869., -1264.);  // in mm

    minFidVolTN208Coords.SetXYZ(-836., -871., -2962.);
    maxFidVolTN208Coords.SetXYZ(+765., +869., -1274.);

    minSandCoords.SetXYZ(-10000., -10000., -280.e+3);  // in mm
    maxSandCoords.SetXYZ(+10000., +10000., -3500.);  // in mm

    muMinusSelection = TCut(TString::Format("SelectionNom==%d||SelectionNom==%d",
                samples.GetP0DWaterNuMuCC(), samples.GetP0DAirNuMuCC()));
    muMinusSelection.SetName("#mu^{-} Selection Cut");

    muPlusInRHCSelection = TCut(TString::Format("SelectionNom==%d||SelectionNom==%d",
                samples.GetP0DWaterNuMuBarInAntiNuModeCC(), samples.GetP0DAirNuMuBarInAntiNuModeCC()));
    muPlusInRHCSelection.SetName("#mu^{+} in RHC Selection Cut");

    muMinusBkgInRHCSelection = TCut(TString::Format("SelectionNom==%d||SelectionNom==%d",
                samples.GetP0DWaterNuMuBkgInAntiNuModeCC(), samples.GetP0DAirNuMuBkgInAntiNuModeCC()));
    muMinusBkgInRHCSelection.SetName("#mu^{-} Bkg in RHC Selection Cut");

    FV = TCut(TString::Format("(%f<=%s&&%s<=%f)&&(%f<=%s&&%s<=%f)&&(%f<=%s&&%s<=%f)",
                                minFidVolCoords.Z(), "vtxZ", "vtxZ", maxFidVolCoords.Z(),
                                minFidVolCoords.X(), "vtxX", "vtxX", maxFidVolCoords.X(),
                                minFidVolCoords.Y(), "vtxY", "vtxY", maxFidVolCoords.Y()
                             )
             );
    FV.SetName("Fiducial Volume Cut");

    tFV = TCut(TString::Format("(%f<=%s&&%s<=%f)&&(%f<=%s&&%s<=%f)&&(%f<=%s&&%s<=%f)&&!(%f<=%s&&%s<=%f)",
                                 minFidVolCoords.Z(), "tVtxZ", "tVtxZ", maxFidVolCoords.Z(),
                                 minFidVolCoords.X(), "tVtxX", "tVtxX", maxFidVolCoords.X(),
                                 minFidVolCoords.Y(), "tVtxY", "tVtxY", maxFidVolCoords.Y(),
                                 minSandCoords.Z(),   "tVtxZ", "tVtxZ", maxSandCoords.Z()
                              )
              );
    tFV.SetName("True Fiducial Volume Cut");

    tOOFV = !tFV;
    tOOFV.SetName("True OOFV Cut");

    FVTN208 = TCut(TString::Format("(%f<=%s&&%s<=%f)&&(%f<=%s&&%s<=%f)&&(%f<=%s&&%s<=%f)",
                                minFidVolTN208Coords.Z(), "vtxZ", "vtxZ", maxFidVolTN208Coords.Z(),
                                minFidVolTN208Coords.X(), "vtxX", "vtxX", maxFidVolTN208Coords.X(),
                                minFidVolTN208Coords.Y(), "vtxY", "vtxY", maxFidVolTN208Coords.Y()
                             )
             );
    FVTN208.SetName("Fiducial Volume TN208 Cut");

    tFVTN208 = TCut(TString::Format("(%f<=%s&&%s<=%f)&&(%f<=%s&&%s<=%f)&&(%f<=%s&&%s<=%f)&&!(%f<=%s&&%s<=%f)",
                                 minFidVolTN208Coords.Z(), "tVtxZ", "tVtxZ", maxFidVolTN208Coords.Z(),
                                 minFidVolTN208Coords.X(), "tVtxX", "tVtxX", maxFidVolTN208Coords.X(),
                                 minFidVolTN208Coords.Y(), "tVtxY", "tVtxY", maxFidVolTN208Coords.Y(),
                                 minSandCoords.Z(),   "tVtxZ", "tVtxZ", maxSandCoords.Z()
                              )
              );
    tFVTN208.SetName("True Fiducial Volume TN208 Cut");

    tOOFVTN208 = !tFVTN208;
    tOOFVTN208.SetName("True OOFVTN208 Cut");

    tLepMuMinus = TCut(TString::Format("tLeptonPDG==%d", pdg.kMuMinusPDG)) && tFV;
    tLepMuMinus.SetName("#mu^{-} Cut");

    tLepMuPlus = TCut(TString::Format("tLeptonPDG==%d", pdg.kMuPlusPDG)) && tFV;
    tLepMuPlus.SetName("#mu^{+} Cut");

    tLepPiPlus = TCut(TString::Format("tLeptonPDG==%d", pdg.kPiPlusPDG)) && tFV;
    tLepPiPlus.SetName("#pi^{+} Cut");

    tLepPiMinus = TCut(TString::Format("tLeptonPDG==%d", pdg.kPiMinusPDG)) && tFV;
    tLepPiMinus.SetName("#pi^{-} Cut");

    tLepProton = TCut(TString::Format("tLeptonPDG==%d", pdg.kProtonPDG)) && tFV;
    tLepProton.SetName("Proton Cut");

    tLepGamma = TCut(TString::Format("abs(tLeptonPDG)==abs(%d)", pdg.kGammaPDG)) && tFV;
    tLepGamma.SetName("#gamma Cut");

    tLepElectron = TCut(TString::Format("tLeptonPDG==%d", pdg.kElectronPDG)) && tFV;
    tLepElectron.SetName("Electron Cut");

    tLepPositron = TCut(TString::Format("tLeptonPDG==%d", pdg.kPositronPDG)) && tFV;
    tLepPositron.SetName("Positron Cut");

    tLepEMParticle = (TCut(TString::Format("abs(tLeptonPDG)==abs(%d)", pdg.kGammaPDG)) ||
                      TCut(TString::Format("abs(tLeptonPDG)==abs(%d)", pdg.kElectronPDG))) &&
                      tFV;
    tLepEMParticle.SetName("e^{#pm}/#gamma Cut");

    tLepOther = tFV &&
                TCut(TString::Format("abs(tLeptonPDG)!=abs(%d)", pdg.kMuMinusPDG)) &&
                TCut(TString::Format("abs(tLeptonPDG)!=abs(%d)", pdg.kPiPlusPDG)) &&
                TCut(TString::Format("abs(tLeptonPDG)!=abs(%d)", pdg.kElectronPDG)) &&
                TCut(TString::Format("abs(tLeptonPDG)!=abs(%d)", pdg.kGammaPDG)) &&
                TCut(TString::Format("tLeptonPDG!=%d" , pdg.kProtonPDG));
    tLepOther.SetName("Other Particle Cut");

    tLepSand = TCut(TString::Format("abs(tLeptonPDG)==abs(%d)&&(%f<=%s&&%s<=%f)",
                                     pdg.kMuMinusPDG, minSandCoords.Z(), "tVtxZ", "tVtxZ", maxSandCoords.Z()
                                   )
                   );
    tLepSand.SetName("Sand Muon Cut");

    tParNuMu = TCut(TString::Format("TrueNuPDGNom==%d", pdg.kNuMuPDG)) && tFV;
    tParNuMu.SetName("True #nu_{#mu} Cut");

    tParNuMubar = TCut(TString::Format("TrueNuPDGNom==%d", pdg.kNuMuBarPDG)) && tFV;
    tParNuMubar.SetName("True Anti-#nu_{#mu} Cut");

    tParNuEs = TCut(TString::Format("abs(TrueNuPDGNom)==abs(%d)", pdg.kNuEPDG)) && tFV;
    tParNuEs.SetName("True Anti-#nu_{e}+#nu_{e} Cut");

    tParOther = tFV &&
                TCut(TString::Format("abs(TrueNuPDGNom)!=abs(%d)", pdg.kNuMuPDG)) &&
                TCut(TString::Format("abs(TrueNuPDGNom)!=abs(%d)", pdg.kNuEPDG));
    tParOther.SetName("True Other FV Neutrino Cut");

    tParNuSand = TCut(TString::Format("abs(TrueNuPDGNom)==abs(%d)&&(%f<=%s&&%s<=%f)",
                                      pdg.kNuMuPDG, minSandCoords.Z(), "tVtxZ", "tVtxZ", maxSandCoords.Z()
                                     )
                     );
    tParNuSand.SetName("True Sand Neutrino Cut");

}

//**************************************************
TCut DefineCuts::AndTCuts(const TCut &cutA, const TCut &cutB) const
//**************************************************
{
    TCut cutAandB = cutA && cutB;
    cutAandB.SetName(TString::Format("(%s) && (%s)", cutA.GetName(), cutB.GetName()));
    return cutAandB;
}
