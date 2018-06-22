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

    minFVcoords.SetXYZ(-836., -871., -2969.);
    maxFVcoords.SetXYZ(+765., +869., -1264.);

    const SampleId samples;
    const HEPConstants pdg;
    muMinusSelection = TCut(TString::Format("SelectionNom==%d", samples.GetP0DNuMuCC()));
    muPlusSelection = TCut(TString::Format("SelectionNom==%d", samples.GetP0DNuMuBarCC()));
    FV = TCut(TString::Format("(%f<=%s&&%s<=%f)&&(%f<=%s&&%s<=%f)&&(%f<=%s&&%s<=%f)",
                             minFVcoords.Z(), "vtxZ", "vtxZ", maxFVcoords.Z(),
                             minFVcoords.X(), "vtxX", "vtxX", maxFVcoords.X(),
                             minFVcoords.Y(), "vtxY", "vtxY", maxFVcoords.Y()
                             )
             );
    tFV = TCut(TString::Format("(%f<=%s&&%s<=%f)&&(%f<=%s&&%s<=%f)&&(%f<=%s&&%s<=%f)",
                             minFVcoords.Z(), "tVtxZ", "tVtxZ", maxFVcoords.Z(),
                             minFVcoords.X(), "tVtxX", "tVtxX", maxFVcoords.X(),
                             minFVcoords.Y(), "tVtxY", "tVtxY", maxFVcoords.Y()
                             )
              );
    tOOFV = !tFV;
    tLepMuMinus = TCut(TString::Format("tLeptonPDG==%d", pdg.kMuMinusPDG)) && tFV;
    tLepMuPlus = TCut(TString::Format("tLeptonPDG==%d", pdg.kMuPlusPDG)) && tFV;
    tLepPiPlus = TCut(TString::Format("tLeptonPDG==%d", pdg.kPiPlusPDG)) && tFV;
    tLepPiMinus = TCut(TString::Format("tLeptonPDG==%d", pdg.kPiMinusPDG)) && tFV;
    tLepProton = TCut(TString::Format("tLeptonPDG==%d", pdg.kProtonPDG)) && tFV;
    tLepGamma = TCut(TString::Format("abs(tLeptonPDG)==abs(%d)", pdg.kGammaPDG)) && tFV;
    tLepElectron = TCut(TString::Format("tLeptonPDG==%d", pdg.kElectronPDG)) && tFV;
    tLepPositron = TCut(TString::Format("tLeptonPDG==%d", pdg.kPositronPDG)) && tFV;
    tLepEMParticle = (tLepPositron || tLepElectron || tLepGamma) && tFV;
    tLepOther = tFV &&
                TCut(TString::Format("abs(tLeptonPDG)!=abs(%d)", pdg.kMuMinusPDG)) &&
                TCut(TString::Format("abs(tLeptonPDG)!=abs(%d)", pdg.kPiPlusPDG)) &&
                TCut(TString::Format("abs(tLeptonPDG)!=abs(%d)", pdg.kElectronPDG)) &&
                TCut(TString::Format(    "tLeptonPDG !=    %d" , pdg.kProtonPDG));
    tLepSand = TCut(TString::Format("abs(tLeptonPDG)==abs(%d) && tVtxZ <= -3500", pdg.kMuMinusPDG));
    tParNuMu = TCut(TString::Format("TrueNuPDGNom==%d", pdg.kNuMuPDG)) && tFV;
    tParNuMubar = TCut(TString::Format("TrueNuPDGNom==%d", pdg.kNuMuBarPDG)) && tFV;
    tParNuEs = TCut(TString::Format("abs(TrueNuPDGNom)==abs(%d)", pdg.kNuEPDG)) && tFV;
    tParOther = tFV &&
                TCut(TString::Format("abs(TrueNuPDGNom)!=abs(%d)", pdg.kNuMuPDG)) &&
                TCut(TString::Format("abs(TrueNuPDGNom)!=abs(%d)", pdg.kNuEPDG));


}
