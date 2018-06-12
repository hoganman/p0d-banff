#define DEFINECUTS_CXX

#include "DefineCuts.hxx"
#include "TString.h"
#include "SampleId.hxx"
#include "HEPConstants.hxx"
ClassImp(DefineCuts)

//**************************************************
DefineCuts::DefineCuts()
//**************************************************
{

    minFVcoords.SetXYZ(-836., -871., -2969.);
    maxFVcoords.SetXYZ(-765., +869., -1264.);

    const SampleId samples;
    const HEPConstants pdg;
    muMinusSelection = TCut(TString::Format("tLeptonPDG==%u", samples.GetP0DNuMuCC()));
    muPlusSelection = TCut(TString::Format("tLeptonPDG==%u", samples.GetP0DNuMuBarCC()));
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
    tLepMuMinus = TCut(TString::Format("tLeptonPDG==%d", pdg.kMuMinusPDG));
    tLepMuPlus = TCut(TString::Format("tLeptonPDG==%d", pdg.kMuPlusPDG));
    tLepPiPlus = TCut(TString::Format("tLeptonPDG==%d", pdg.kPiPlusPDG));
    tLepPiMinus = TCut(TString::Format("tLeptonPDG==%d", pdg.kPiMinusPDG));
    tLepProton = TCut(TString::Format("tLeptonPDG==%d", pdg.kProtonPDG));
    tLepGamma = TCut(TString::Format("abs(tLeptonPDG)==abs(%d)", pdg.kGammaPDG));
    tLepElectron = TCut(TString::Format("tLeptonPDG==%d", pdg.kElectronPDG));
    tLepPositron = TCut(TString::Format("tLeptonPDG==%d", pdg.kPositronPDG));
    tLepEMParticle = (tLepPositron || tLepElectron || tLepGamma);
    tLepKaon = TCut(TString::Format("abs(tLeptonPDG)==abs(%d)", pdg.kKaPlusPDG));
    tLepOther = !(tLepMuPlus || tLepMuMinus || tLepPiPlus || tLepPiMinus || tLepEMParticle || tLepKaon);
    tParNuMu = TCut(TString::Format("TrueNuPDGNom==%d", pdg.kNuMuPDG));
    tParNuMubar = TCut(TString::Format("TrueNuPDGNom==%d", pdg.kNuMuBarPDG));
    tParNuEs = TCut(TString::Format("abs(TrueNuPDGNom)==abs(%d)", pdg.kNuEPDG));

}
