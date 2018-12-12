#define DEFINECUTS_CXX

#include "DefineCuts.hxx"
ClassImp(DefineCuts)
#include"P0DBANFFInterface.hxx"
#include "HEPConstants.hxx"
#include <iostream>

//**************************************************
DefineCuts::~DefineCuts()
//**************************************************
{}

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

    muMinusCC1TrackSelection = TCut(TString::Format("SelectionNom==%d || SelectionNom==%d",
                samples.GetP0DWaterNuMuCC1Track(), samples.GetP0DAirNuMuCC1Track()));
    muMinusCC1TrackSelection.SetName("#mu^{-} CC1Track Selection Cut");

    muPlusInRHCCC1TrackSelection = TCut(TString::Format("SelectionNom==%d || SelectionNom==%d",
                samples.GetP0DWaterNuMuBarInAntiNuModeCC1Track(), samples.GetP0DAirNuMuBarInAntiNuModeCC1Track()));
    muPlusInRHCCC1TrackSelection.SetName("#mu^{+} in RHC CC1Track Selection Cut");

    muMinusBkgInRHCCC1TrackSelection = TCut(TString::Format("SelectionNom==%d || SelectionNom==%d",
                samples.GetP0DWaterNuMuBkgInAntiNuModeCC1Track(), samples.GetP0DAirNuMuBkgInAntiNuModeCC1Track()));
    muMinusBkgInRHCCC1TrackSelection.SetName("#mu^{-} Bkg in RHC CC1Track Selection Cut");

    muMinusCCNTracksSelection = TCut(TString::Format("SelectionNom==%d || SelectionNom==%d",
                samples.GetP0DWaterNuMuCCNTracks(), samples.GetP0DAirNuMuCCNTracks()));
    muMinusCCNTracksSelection.SetName("#mu^{-} CCNTracks Selection Cut");

    muPlusInRHCCCNTracksSelection = TCut(TString::Format("SelectionNom==%d || SelectionNom==%d",
                samples.GetP0DWaterNuMuBarInAntiNuModeCCNTracks(), samples.GetP0DAirNuMuBarInAntiNuModeCCNTracks()));
    muPlusInRHCCCNTracksSelection.SetName("#mu^{+} in RHC CCNTracks Selection Cut");

    muMinusBkgInRHCCCNTracksSelection = TCut(TString::Format("SelectionNom==%d || SelectionNom==%d",
                samples.GetP0DWaterNuMuBkgInAntiNuModeCCNTracks(), samples.GetP0DAirNuMuBkgInAntiNuModeCCNTracks()));
    muMinusBkgInRHCCCNTracksSelection.SetName("#mu^{-} Bkg in RHC CCNTracks Selection Cut");

    //due to exclusivity of the 1 and N track selections, CC-Inc is the sum of both
    muMinusCCSelection = TCut(TString::Format("SelectionNom==%d || SelectionNom==%d",
                samples.GetP0DWaterNuMuCC(), samples.GetP0DAirNuMuCC()))
                || muMinusCC1TrackSelection || muMinusCCNTracksSelection;
    muMinusCCSelection.SetName("#mu^{-} CC-Inc Selection Cut");

    //due to exclusivity of the 1 and N track selections, CC-Inc is the sum of both
    muPlusInRHCCCSelection = TCut(TString::Format("SelectionNom==%d || SelectionNom==%d",
                samples.GetP0DWaterNuMuBarInAntiNuModeCC(), samples.GetP0DAirNuMuBarInAntiNuModeCC()))
                || muPlusInRHCCC1TrackSelection || muPlusInRHCCCNTracksSelection;
    muPlusInRHCCCSelection.SetName("#mu^{+} in RHC CC-Inc Selection Cut");

    //due to exclusivity of the 1 and N track selections, CC-Inc is the sum of both
    muMinusBkgInRHCCCSelection = TCut(TString::Format("SelectionNom==%d || SelectionNom==%d",
                samples.GetP0DWaterNuMuBkgInAntiNuModeCC(), samples.GetP0DAirNuMuBkgInAntiNuModeCC()))
                || muMinusBkgInRHCCC1TrackSelection || muMinusBkgInRHCCCNTracksSelection;
    muMinusBkgInRHCCCSelection.SetName("#mu^{-} Bkg in RHC CC-Inc Selection Cut");

    anyP0DSelection = muMinusCCSelection || muMinusBkgInRHCCCSelection || muPlusInRHCCCSelection;
                      //|| muMinusCC1TrackSelection || muMinusBkgInRHCCC1TrackSelection || muPlusInRHCCC1TrackSelection
                      //|| muMinusCCNTracksSelection || muMinusBkgInRHCCCNTracksSelection || muPlusInRHCCCNTracksSelection;

    anyP0DSelection.SetName("Any P0D+TPC selection cut");

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

    tSand = TCut(TString::Format("%f<=tVtxZ&&tVtxZ<=%f", minSandCoords.Z(), maxSandCoords.Z()));
    tSand.SetName("Sand Muon Cut");

    tNEUTNuCCAny = TCut(TString::Format("0<tReactionCode&&tReactionCode<%d", pdg.kNEUTNuN_NC1PiZeroN));
    tNEUTNuCCAny.SetName("True NEUT Nu CC");

    tNEUTNuCCQE = TCut(TString::Format("tReactionCode==%d", pdg.kNEUTNu_CCQE));
    tNEUTNuCCQE.SetName("True NEUT Nu CC-QE");

    tNEUTNuCC2p2h = TCut(TString::Format("tReactionCode==%d", pdg.kNEUTNu_2p2h));
    tNEUTNuCC2p2h.SetName("True NEUT Nu CC-2p2h");

    tNEUTNuCC1pi = TCut(TString::Format("(%d<=tReactionCode&&tReactionCode<=%d)||tReactionCode==3",
                        pdg.kNEUTNuP_CC1PiPlusP, pdg.kNEUTNuO_CC1PiPlusO));
    tNEUTNuCC1pi.SetName("True NEUT Nu CC-1pi");

    tNEUTNuCCNpi = TCut(TString::Format("tReactionCode==%d", pdg.kNEUTNu_CCMultiPi));
    tNEUTNuCCNpi.SetName("True NEUT Nu CC-Npi");

    tNEUTNuCCDIS = TCut(TString::Format("tReactionCode==%d", pdg.kNEUTNu_CCDIS));
    tNEUTNuCCDIS.SetName("True NEUT Nu CC-DIS");

    tNEUTNuCCOther = TCut(TString::Format("tReactionCode==%d||tReactionCode==%d||tReactionCode==%d",
                          pdg.kNEUTNuN_CC1GammaP,pdg.kNEUTNuN_CC1EtaZeroP,pdg.kNEUTNuN_CC1KaPlusLambda));
    tNEUTNuCCOther.SetName("True NEUT Nu CC-Other");

    tNEUTNC  = TCut(TString::Format("abs(tReactionCode)>=abs(%d)", pdg.kNEUTNuN_NC1PiZeroN));
    tNEUTNC.SetName("True NEUT NC");

    tNEUTAntiNuCCAny = TCut(TString::Format("%d<tReactionCode&&tReactionCode<%d", pdg.kNEUTAntiNuN_NC1PiZeroN, pdg.kNEUTAntiNu_CCQE));
    tNEUTAntiNuCCAny.SetName("True NEUT Any Anti-nu");

    tNEUTAntiNuCCQE = TCut(TString::Format("tReactionCode==%d", pdg.kNEUTAntiNu_CCQE));
    tNEUTAntiNuCCQE.SetName("True NEUT Anti-nu CCQE");

    tNEUTAntiNuCC2p2h = TCut(TString::Format("tReactionCode==%d", pdg.kNEUTAntiNu_2p2h));
    tNEUTAntiNuCC2p2h.SetName("True NEUT Anti-nu CC-2p2h");

    tNEUTAntiNuCC1pi = TCut(TString::Format("(%d<=tReactionCode&&tReactionCode<=%d)||tReactionCode==-3",
                        pdg.kNEUTAntiNuO_CC1PiPlusO, pdg.kNEUTAntiNuP_CC1PiPlusP));
    tNEUTAntiNuCC1pi.SetName("True NEUT Anti-nu CC-1pi");

    tNEUTAntiNuCCNpi = TCut(TString::Format("tReactionCode==%d", pdg.kNEUTAntiNu_CCMultiPi));
    tNEUTAntiNuCCNpi.SetName("True NEUT Anti-nu CC-Npi");

    tNEUTAntiNuCCOther = TCut(TString::Format("tReactionCode==%d||tReactionCode==%d||tReactionCode==%d",
                                              pdg.kNEUTAntiNuN_CC1GammaP, pdg.kNEUTAntiNuN_CC1EtaZeroP,
                                              pdg.kNEUTAntiNuN_CC1KaPlusLambda));
    tNEUTAntiNuCCOther.SetName("True NEUT Anti-nu CC-Other");

    tNEUTAntiNuCCDIS = TCut(TString::Format("tReactionCode==%d", pdg.kNEUTAntiNu_CCDIS));
    tNEUTAntiNuCCDIS.SetName("True NEUT Anti-nu CC-DIS");

    tNEUTNuNonCCQE = TCut(TString::Format("%d < tReactionCode && tReactionCode < %d",
                                          pdg.kNEUTNu_CCQE, pdg.kNEUTNu_Invalid));
    tNEUTNuNonCCQE.SetName("True NEUT Nu non-CCQE");

    tNEUTAntiNuNonCCQE = TCut(TString::Format("%d < tReactionCode && tReactionCode < %d",
                                              pdg.kNEUTAntiNu_Invalid, pdg.kNEUTAntiNu_CCQE));
    tNEUTNuNonCCQE.SetName("True NEUT Nu Anti-nu non-CCQE");

    TCut tAntiNuBkgTopologyInNuMode = (muMinusCCSelection || muMinusBkgInRHCCCSelection ||
                                       muMinusCC1TrackSelection || muMinusBkgInRHCCC1TrackSelection ||
                                       muMinusCCNTracksSelection || muMinusBkgInRHCCCNTracksSelection)
                                      && TCut(TString::Format("NPrimaryParticles[%d]<=0", pdg.kMuon));
    TCut tNuBkgTopologyInAntiNuMode = (muPlusInRHCCCSelection || muPlusInRHCCC1TrackSelection || muPlusInRHCCCNTracksSelection)
                                      && TCut(TString::Format("NPrimaryParticles[%d]<=0", pdg.kAntiMuon));
    tBKGTopology = (tNuBkgTopologyInAntiNuMode || tAntiNuBkgTopologyInNuMode) && tFV;
    tBKGTopology.SetName("True BKG Topology");

    TCut tZeroMesonTopology = TCut(TString::Format("NPrimaryParticles[%d]==0", pdg.kMesons));
    tCC0PiTopology = anyP0DSelection && tZeroMesonTopology && tFV;
    tCC0PiTopology.SetName("True CC-0#pi Topology");

    TCut tCC1PiInNuModeTopology = (muMinusCCSelection || muMinusBkgInRHCCCSelection ||
                                   muMinusCC1TrackSelection || muMinusBkgInRHCCC1TrackSelection ||
                                   muMinusCCNTracksSelection || muMinusBkgInRHCCCNTracksSelection)
                                   && TCut(TString::Format("NPrimaryParticles[%d]==1", pdg.kPiPos));
    TCut tCC1PiInAntiNuModeTopology = (muPlusInRHCCCSelection || muPlusInRHCCC1TrackSelection || muPlusInRHCCCNTracksSelection)
                                      && TCut(TString::Format("NPrimaryParticles[%d]==1", pdg.kPiNeg));
    TCut tOneMesonTopology = TCut(TString::Format("NPrimaryParticles[%d]==1", pdg.kMesons));
    tCC1PiTopology = (tCC1PiInNuModeTopology || tCC1PiInAntiNuModeTopology) && tOneMesonTopology && tFV;
    tCC1PiTopology.SetName("True CC-1#pi Topology");

    tCCOtherTopology = anyP0DSelection && (     !(tNuBkgTopologyInAntiNuMode || tAntiNuBkgTopologyInNuMode)
                                             && !(tZeroMesonTopology)
                                             && !((tCC1PiInNuModeTopology || tCC1PiInAntiNuModeTopology) && tOneMesonTopology)
                                          )
                                       && tFV;
    tCCOtherTopology.SetName("True CC-Other Topology");

    tParNuMu = TCut(TString::Format("TrueNuPDGNom==%d&&tReactionCode>=%d", pdg.kNuMuPDG, pdg.kNEUTNu_CCQE)) && !tNEUTNC && tFV;
    tParNuMu.SetName("True #nu_{#mu} Cut");

    tParNuMubar = TCut(TString::Format("TrueNuPDGNom==%d&&tReactionCode<=%d", pdg.kNuMuBarPDG, pdg.kNEUTAntiNu_CCQE)) && !tNEUTNC && tFV;
    tParNuMubar.SetName("True Anti-#nu_{#mu} Cut");

    tParNuEs = TCut(TString::Format("abs(TrueNuPDGNom)==abs(%d)", pdg.kNuEPDG)) && !tNEUTNC && tFV;
    tParNuEs.SetName("True Anti-#nu_{e}+#nu_{e} Cut");

    tParNuSand = TCut(TString::Format("abs(TrueNuPDGNom)==abs(%d)&&(%f<=tVtxZ&&tVtxZ<=%f)",
                                      pdg.kNuMuPDG, minSandCoords.Z(), maxSandCoords.Z()));
    tParNuSand.SetName("True Sand Neutrino Cut");

}

//**************************************************
void DefineCuts::FillParticleSelections(const TString &name,
        const TString &title, const SampleId::SampleEnum &sampleID,
        const TCut &additionalCuts)
//**************************************************
{
    std::vector<PlottingSelectionInfo*>::iterator it;
    for(it = ParticleSelections.begin(); it != ParticleSelections.end(); ++it)
    {
        if(*it) delete *it;
    }
    ParticleSelections.clear();
    TCut all_nom_sel_cut = GetNominalSelectionCut(sampleID);
    if(TString(all_nom_sel_cut.GetTitle()).Length() == 0)
    {
        P0DBANFFInterface::Error(this, TString::Format("Unable to determine sample using SampleId = %d", sampleID));
        return;
    }

    ParticleSelections.resize(NMAXPARTICLESELECTIONS);
    if(TString(additionalCuts.GetTitle()).Length() > 0)
        all_nom_sel_cut = all_nom_sel_cut && additionalCuts;

    //Since cuts like tLepMuMinus have tFV in them, there is no need to add
    //extra checks on the FV X and Y since the only Z is different
    TCut tFV_Correction = "1";
    TCut tOOFV_Correction = tOOFV;
    if(TString(all_nom_sel_cut.GetTitle()).Contains(tFVTN208.GetTitle()))
    {
        tFV_Correction = TCut(TString::Format("(%f<=%s&&%s<=%f)",
                                 minFidVolTN208Coords.Z(), "tVtxZ",
                                 "tVtxZ", maxFidVolTN208Coords.Z()));
        tOOFV_Correction = tOOFVTN208;
    }

    UInt_t entry = 0;
    // all selection events
    PlottingSelectionInfo* all_nom_sel = new PlottingSelectionInfo(name, all_nom_sel_cut, title);
    ParticleSelections[entry++] = all_nom_sel;

    // mu minuses
    TCut muMinus_sel_cut = all_nom_sel_cut && tLepMuMinus && tFV_Correction;
    PlottingSelectionInfo* muMinus_sel = new PlottingSelectionInfo("muMinus_sel", muMinus_sel_cut, "#mu^{-}");
    ParticleSelections[entry++] = muMinus_sel;

    // mu plus
    TCut muPlus_sel_cut = all_nom_sel_cut && tLepMuPlus && tFV_Correction;
    PlottingSelectionInfo* muPlus_sel = new PlottingSelectionInfo("muPlus_sel", muPlus_sel_cut, "#mu^{+}");
    ParticleSelections[entry++] = muPlus_sel;

    // pi minus
    TCut piMinus_sel_cut = all_nom_sel_cut && tLepPiMinus && tFV_Correction;
    PlottingSelectionInfo* piMinus_sel = new PlottingSelectionInfo("piMinus_sel", piMinus_sel_cut, "#pi^{-}");
    ParticleSelections[entry++] = piMinus_sel;

    // pi plus
    TCut piPlus_sel_cut = all_nom_sel_cut && tLepPiPlus && tFV_Correction;
    PlottingSelectionInfo* piPlus_sel = new PlottingSelectionInfo("piPlus_sel", piPlus_sel_cut, "#pi^{+}");
    ParticleSelections[entry++] = piPlus_sel;

    // pi plus
    TCut proton_sel_cut = all_nom_sel_cut && tLepProton && tFV_Correction;
    PlottingSelectionInfo* proton_sel = new PlottingSelectionInfo("proton_sel", proton_sel_cut, "p");
    ParticleSelections[entry++] = proton_sel;

    // EM particles
    TCut em_sel_cut = all_nom_sel_cut && tLepEMParticle && tFV_Correction;
    PlottingSelectionInfo* em_sel = new PlottingSelectionInfo("em_sel", em_sel_cut, "e^{#pm}/#gamma");
    ParticleSelections[entry++] = em_sel;

    // other particles
    TCut other_sel_cut = all_nom_sel_cut && tLepOther && tFV_Correction;
    PlottingSelectionInfo* other_sel = new PlottingSelectionInfo("other_sel", other_sel_cut, "other");
    ParticleSelections[entry++] = other_sel;

    // OOFV
    TCut oofv_sel_cut = all_nom_sel_cut && tOOFV_Correction;
    PlottingSelectionInfo* oofv_sel = new PlottingSelectionInfo("oofv_sel", oofv_sel_cut, "OOFV");
    ParticleSelections[entry++] = oofv_sel;

    // sand muons
    TCut sandmu_sel_cut = all_nom_sel_cut && tSand;
    PlottingSelectionInfo* sandmu_sel = new PlottingSelectionInfo("sandmu_sel", sandmu_sel_cut, "Sand muons");
    ParticleSelections[entry++] = sandmu_sel;

    if(entry != NMAXPARTICLESELECTIONS)
    {
        P0DBANFFInterface::Error(this, "There is a mismatch between the number of cuts in DefineCuts::FillParticleSelections");
    }

}

//**************************************************
void DefineCuts::FillNeutrinoSelections(const TString &name,
        const TString &title, const SampleId::SampleEnum &sampleID,
        const TCut &additionalCuts)
//**************************************************
{
    std::vector<PlottingSelectionInfo*>::iterator it;
    for(it = NeutrinoSelections.begin(); it != NeutrinoSelections.end(); ++it)
    {
        if(*it) delete *it;
    }
    NeutrinoSelections.clear();
    TCut all_nom_sel_cut = GetNominalSelectionCut(sampleID);
    if(TString(all_nom_sel_cut.GetTitle()).Length() == 0)
    {
        P0DBANFFInterface::Error(this, TString::Format("Unable to determine sample using SampleId = %d", sampleID));
        return;
    }

    //Since cuts like tParNuMu have tFV in them, there is no need to add
    //extra checks on the FV X and Y since the only Z is different
    TCut tFV_Correction = "1";
    TCut tOOFV_Correction = tOOFV;
    if(TString(all_nom_sel_cut.GetTitle()).Contains(tFVTN208.GetTitle()))
    {
        tFV_Correction = TCut(TString::Format("(%f<=%s&&%s<=%f)",
                                 minFidVolTN208Coords.Z(), "tVtxZ",
                                 "tVtxZ", maxFidVolTN208Coords.Z()));
        tOOFV_Correction = tOOFVTN208;
    }


    NeutrinoSelections.resize(NMAXNEUTRINOSELECTIONS);
    if(TString(additionalCuts.GetTitle()).Length() > 0)
        all_nom_sel_cut = all_nom_sel_cut && additionalCuts;

    UInt_t entry = 0;
    // all selection events
    PlottingSelectionInfo* all_nom_sel = new PlottingSelectionInfo(name, all_nom_sel_cut, title);
    NeutrinoSelections[entry++] = all_nom_sel;

    //muon neutrinos
    TCut numu_cc_sel_cut = all_nom_sel_cut && tParNuMu && tFV_Correction;
    PlottingSelectionInfo* numu_cc_sel = new PlottingSelectionInfo("numu_cc_sel", numu_cc_sel_cut,
                                 "#nu_{#mu}");
    NeutrinoSelections[entry++] = numu_cc_sel;

    //muon antineutrinos
    TCut numubar_cc_sel_cut = all_nom_sel_cut && tParNuMubar && tFV_Correction;
    PlottingSelectionInfo* numubar_cc_sel = new PlottingSelectionInfo("numubar_cc_sel", numubar_cc_sel_cut,
                                    "#bar{#nu}_{#mu}");
    NeutrinoSelections[entry++] = numubar_cc_sel;

    //nue and nuebars
    TCut nuenuebar_cc_sel_cut = all_nom_sel_cut && tParNuEs && tFV_Correction;
    PlottingSelectionInfo* nuenuebar_cc_sel = new PlottingSelectionInfo("nuenuebar_cc_sel", nuenuebar_cc_sel_cut,
                                      "#nu_{e} + #bar{#nu}_{e}");
    NeutrinoSelections[entry++] = nuenuebar_cc_sel;

    //NC neutrinos
    TCut nc_sel_cut = all_nom_sel_cut && tNEUTNC && tFV_Correction;
    PlottingSelectionInfo* nc_sel = new PlottingSelectionInfo("nc_sel", nc_sel_cut, "NC");
    NeutrinoSelections[entry++] = nc_sel;

    // OOFV
    TCut oofv_sel_cut = all_nom_sel_cut && tOOFV_Correction;
    PlottingSelectionInfo* oofv_sel = new PlottingSelectionInfo("oofv_sel", oofv_sel_cut, "OOFV");
    NeutrinoSelections[entry++] = oofv_sel;

    // sand muons
    TCut sandmu_sel_cut = all_nom_sel_cut && tSand;
    PlottingSelectionInfo* sandmu_sel = new PlottingSelectionInfo("sandmu_sel", sandmu_sel_cut, "Sand muons");
    NeutrinoSelections[entry++] = sandmu_sel;

    if(entry != NMAXNEUTRINOSELECTIONS)
    {
        P0DBANFFInterface::Error(this, "There is a mismatch between the number of cuts in DefineCuts::FillNeutrinoSelections");
    }

}

//**************************************************
void DefineCuts::FillNEUTNuSelections(const TString &name, const TString &title,
       const SampleId::SampleEnum &sampleID, const TCut &additionalCuts)
//**************************************************
{
    std::vector<PlottingSelectionInfo*>::iterator it;
    for(it = NEUTNuSelections.begin(); it != NEUTNuSelections.end(); ++it)
    {
        if(*it) delete *it;
    }
    NEUTNuSelections.clear();
    TCut all_nom_sel_cut = GetNominalSelectionCut(sampleID);
    if(TString(all_nom_sel_cut.GetTitle()).Length() == 0)
    {
        P0DBANFFInterface::Error(this, TString::Format("Unable to determine sample using SampleId = %d", sampleID));
        return;
    }

    TCut tFV_Correction = tFV;
    TCut tOOFV_Correction = tOOFV;
    if(TString(all_nom_sel_cut.GetTitle()).Contains(tFVTN208.GetTitle()))
    {
        tFV_Correction = tFVTN208;
        tOOFV_Correction = tOOFVTN208;
    }


    NEUTNuSelections.resize(NMAXNEUTNUSELECTIONS);
    if(TString(additionalCuts.GetTitle()).Length() > 0)
        all_nom_sel_cut = all_nom_sel_cut && additionalCuts;

    UInt_t entry = 0;
    // all selection events
    PlottingSelectionInfo* all_nom_sel = new PlottingSelectionInfo(name, all_nom_sel_cut, title);
    NEUTNuSelections[entry++] = all_nom_sel;

    //nu CCQE
    TCut ccqe_sel_cut = all_nom_sel_cut && tNEUTNuCCQE && tFV_Correction;
    PlottingSelectionInfo* ccqe_sel = new PlottingSelectionInfo("ccqe_sel", ccqe_sel_cut, "#nu CCQE");
    NEUTNuSelections[entry++] = ccqe_sel;

    TCut cc2p2h_sel_cut = all_nom_sel_cut && tNEUTNuCC2p2h && tFV_Correction;
    PlottingSelectionInfo* cc2p2h_sel = new PlottingSelectionInfo("cc2p2h_sel", cc2p2h_sel_cut, "#nu 2p2h");
    NEUTNuSelections[entry++] = cc2p2h_sel;

    TCut cc1p_sel_cut = all_nom_sel_cut && tNEUTNuCC1pi && tFV_Correction;
    PlottingSelectionInfo* cc1p_sel = new PlottingSelectionInfo("cc1p_sel", cc1p_sel_cut, "#nu CC-1#pi");
    NEUTNuSelections[entry++] = cc1p_sel;

    TCut ccnp_sel_cut = all_nom_sel_cut && tNEUTNuCCNpi && tFV_Correction;
    PlottingSelectionInfo* ccnp_sel = new PlottingSelectionInfo("ccnp_sel", ccnp_sel_cut, "#nu CC-N#pi");
    NEUTNuSelections[entry++] = ccnp_sel;

    TCut ccdis_sel_cut = all_nom_sel_cut && tNEUTNuCCDIS && tFV_Correction;
    PlottingSelectionInfo* ccdis_sel = new PlottingSelectionInfo("ccdis_sel", ccdis_sel_cut, "#nu CC-DIS");
    NEUTNuSelections[entry++] = ccdis_sel;

    TCut ccother_sel_cut = all_nom_sel_cut && tNEUTNuCCOther && tFV_Correction;
    PlottingSelectionInfo* ccother_sel = new PlottingSelectionInfo("ccother_sel", ccother_sel_cut, "#nu CC-Other");
    NEUTNuSelections[entry++] = ccother_sel;

    TCut nc_sel_cut = all_nom_sel_cut && tNEUTNC && tFV_Correction;
    PlottingSelectionInfo* nc_sel = new PlottingSelectionInfo("nc_sel", nc_sel_cut, "NC");
    NEUTNuSelections[entry++] = nc_sel;

    TCut antinu_sel_cut = all_nom_sel_cut && tNEUTAntiNuCCAny && tFV_Correction;
    PlottingSelectionInfo* antinu_sel = new PlottingSelectionInfo("antinu_sel", antinu_sel_cut, "#bar{#nu}");
    NEUTNuSelections[entry++] = antinu_sel;

    // OOFV
    TCut oofv_sel_cut = all_nom_sel_cut && tOOFV_Correction;
    PlottingSelectionInfo* oofv_sel = new PlottingSelectionInfo("oofv_sel", oofv_sel_cut, "OOFV");
    NEUTNuSelections[entry++] = oofv_sel;

    // sand muons
    TCut sandmu_sel_cut = all_nom_sel_cut && tSand;
    PlottingSelectionInfo* sandmu_sel = new PlottingSelectionInfo("sandmu_sel", sandmu_sel_cut, "Sand muons");
    NEUTNuSelections[entry++] = sandmu_sel;

    if(entry != NMAXNEUTNUSELECTIONS)
    {
        P0DBANFFInterface::Error(this, "There is a mismatch between the number of cuts in DefineCuts::FillNEUTNuSelections");
    }

}

//**************************************************
void DefineCuts::FillNEUTAntiNuSelections(const TString &name, const TString &title,
       const SampleId::SampleEnum &sampleID, const TCut &additionalCuts)
//**************************************************
{
    std::vector<PlottingSelectionInfo*>::iterator it;
    for(it = NEUTAntiNuSelections.begin(); it != NEUTAntiNuSelections.end(); ++it)
    {
        if(*it) delete *it;
    }
    NEUTAntiNuSelections.clear();
    TCut all_nom_sel_cut = GetNominalSelectionCut(sampleID);
    if(TString(all_nom_sel_cut.GetTitle()).Length() == 0)
    {
        P0DBANFFInterface::Error(this, TString::Format("Unable to determine sample using SampleId = %d", sampleID));
        return;
    }

    TCut tFV_Correction = tFV;
    TCut tOOFV_Correction = tOOFV;
    if(TString(all_nom_sel_cut.GetTitle()).Contains(tFVTN208.GetTitle()))
    {
        tFV_Correction = tFVTN208;
        tOOFV_Correction = tOOFVTN208;
    }

    NEUTAntiNuSelections.resize(NMAXNEUTNUSELECTIONS);
    if(TString(additionalCuts.GetTitle()).Length() > 0)
        all_nom_sel_cut = all_nom_sel_cut && additionalCuts;

    UInt_t entry = 0;
    // all selection events
    PlottingSelectionInfo* all_nom_sel = new PlottingSelectionInfo(name, all_nom_sel_cut, title);
    NEUTAntiNuSelections[entry++] = all_nom_sel;

    TCut ccqe_sel_cut = all_nom_sel_cut && tNEUTAntiNuCCQE && tFV_Correction;
    PlottingSelectionInfo* ccqe_sel = new PlottingSelectionInfo("ccqe_sel", ccqe_sel_cut, "#bar{#nu} CCQE");
    NEUTAntiNuSelections[entry++] = ccqe_sel;

    TCut cc2p2h_sel_cut = all_nom_sel_cut && tNEUTAntiNuCC2p2h && tFV_Correction;
    PlottingSelectionInfo* cc2p2h_sel = new PlottingSelectionInfo("cc2p2h_sel", cc2p2h_sel_cut, "#bar{#nu} 2p2h");
    NEUTAntiNuSelections[entry++] = cc2p2h_sel;

    TCut cc1p_sel_cut = all_nom_sel_cut && tNEUTAntiNuCC1pi && tFV_Correction;
    PlottingSelectionInfo* cc1p_sel = new PlottingSelectionInfo("cc1p_sel", cc1p_sel_cut, "#bar{#nu} CC-1#pi");
    NEUTAntiNuSelections[entry++] = cc1p_sel;

    TCut ccnp_sel_cut = all_nom_sel_cut && tNEUTAntiNuCCNpi && tFV_Correction;
    PlottingSelectionInfo* ccnp_sel = new PlottingSelectionInfo("ccnp_sel", ccnp_sel_cut, "#bar{#nu} CC-N#pi");
    NEUTAntiNuSelections[entry++] = ccnp_sel;

    TCut ccdis_sel_cut = all_nom_sel_cut && tNEUTAntiNuCCDIS && tFV_Correction;
    PlottingSelectionInfo* ccdis_sel = new PlottingSelectionInfo("ccdis_sel", ccdis_sel_cut, "#bar{#nu} CC-DIS");
    NEUTAntiNuSelections[entry++] = ccdis_sel;

    TCut ccother_sel_cut = all_nom_sel_cut && tNEUTAntiNuCCOther && tFV_Correction;
    PlottingSelectionInfo* ccother_sel = new PlottingSelectionInfo("ccother_sel", ccother_sel_cut, "#bar{#nu} CC-Other");
    NEUTAntiNuSelections[entry++] = ccother_sel;

    TCut nc_sel_cut = all_nom_sel_cut && tNEUTNC && tFV_Correction;
    PlottingSelectionInfo* nc_sel = new PlottingSelectionInfo("nc_sel", nc_sel_cut, "NC");
    NEUTAntiNuSelections[entry++] = nc_sel;

    TCut nu_sel_cut = all_nom_sel_cut && tNEUTNuCCAny && tFV_Correction;
    PlottingSelectionInfo* nu_sel = new PlottingSelectionInfo("nu_sel", nu_sel_cut, "#nu");
    NEUTAntiNuSelections[entry++] = nu_sel;

    // OOFV
    TCut oofv_sel_cut = all_nom_sel_cut && tOOFV_Correction;
    PlottingSelectionInfo* oofv_sel = new PlottingSelectionInfo("oofv_sel", oofv_sel_cut, "OOFV");
    NEUTAntiNuSelections[entry++] = oofv_sel;

    // sand muons
    TCut sandmu_sel_cut = all_nom_sel_cut && tSand;
    PlottingSelectionInfo* sandmu_sel = new PlottingSelectionInfo("sandmu_sel", sandmu_sel_cut, "Sand muons");
    NEUTAntiNuSelections[entry++] = sandmu_sel;

    if(entry != NMAXNEUTANTINUSELECTIONS)
    {
        P0DBANFFInterface::Error(this, "There is a mismatch between the number of cuts in DefineCuts::FillNEUTAntiNuSelections");
    }

}

//**************************************************
void DefineCuts::FillTopologySelections(const TString &name, const TString &title,
       const SampleId::SampleEnum &sampleID, const TCut &additionalCuts)
//**************************************************
{
    std::vector<PlottingSelectionInfo*>::iterator it;
    for(it = TopologySelections.begin(); it != TopologySelections.end(); ++it)
    {
        if(*it) delete *it;
    }
    TopologySelections.clear();
    TCut all_nom_sel_cut = GetNominalSelectionCut(sampleID);
    if(TString(all_nom_sel_cut.GetTitle()).Length() == 0)
    {
        P0DBANFFInterface::Error(this, TString::Format("Unable to determine sample using SampleId = %d", sampleID));
        return;
    }

    TCut tFV_Correction = tFV;
    TCut tOOFV_Correction = tOOFV;
    if(TString(all_nom_sel_cut.GetTitle()).Contains(tFVTN208.GetTitle()))
    {
        tFV_Correction = tFVTN208;
        tOOFV_Correction = tOOFVTN208;
    }

    TopologySelections.resize(NMAXTOPOLOGYSELECTIONS);
    if(TString(additionalCuts.GetTitle()).Length() > 0)
        all_nom_sel_cut = all_nom_sel_cut && additionalCuts;

    UInt_t entry = 0;
    // all selection events
    PlottingSelectionInfo* all_nom_sel = new PlottingSelectionInfo(name, all_nom_sel_cut, title);
    TopologySelections[entry++] = all_nom_sel;

    TCut cc0pi_topology_sel_cut = all_nom_sel_cut && tCC0PiTopology && tFV_Correction;
    PlottingSelectionInfo* cc0pi_topology_sel = new PlottingSelectionInfo("cc0pi_topology_sel", cc0pi_topology_sel_cut, "CC-0#pi");
    TopologySelections[entry++] = cc0pi_topology_sel;

    TCut cc1pi_topology_sel_cut = all_nom_sel_cut && tCC1PiTopology && tFV_Correction;
    PlottingSelectionInfo* cc1pi_topology_sel = new PlottingSelectionInfo("cc1pi_topology_sel", cc1pi_topology_sel_cut, "CC-1#pi");
    TopologySelections[entry++] = cc1pi_topology_sel;

    TCut ccother_topology_sel_cut = all_nom_sel_cut && tCCOtherTopology && tFV_Correction;
    PlottingSelectionInfo* ccother_topology_sel = new PlottingSelectionInfo("ccother_topology_sel", ccother_topology_sel_cut, "CC-Other");
    TopologySelections[entry++] = ccother_topology_sel;

    TCut bkg_topology_sel_cut = all_nom_sel_cut && tBKGTopology && tFV_Correction;
    PlottingSelectionInfo* bkg_topology_sel = new PlottingSelectionInfo("bkg_topology_sel", bkg_topology_sel_cut, "BKG");
    TopologySelections[entry++] = bkg_topology_sel;

    // OOFV
    TCut oofv_sel_cut = all_nom_sel_cut && tOOFV_Correction;
    PlottingSelectionInfo* oofv_sel = new PlottingSelectionInfo("oofv_sel", oofv_sel_cut, "OOFV");
    TopologySelections[entry++] = oofv_sel;

    // sand muons
    TCut sandmu_sel_cut = all_nom_sel_cut && tSand;
    PlottingSelectionInfo* sandmu_sel = new PlottingSelectionInfo("sandmu_sel", sandmu_sel_cut, "Sand muons");
    TopologySelections[entry++] = sandmu_sel;

    if(entry != NMAXTOPOLOGYSELECTIONS)
    {
        P0DBANFFInterface::Error(this, "There is a mismatch between the number of cuts in DefineCuts::FillTopologySelections");
    }

}

//**************************************************
void DefineCuts::FillNEUTCCQELikeSelections(const TString &name, const TString &title,
       const SampleId::SampleEnum &sampleID, const TCut &additionalCuts)
//**************************************************
{
    std::vector<PlottingSelectionInfo*>::iterator it;
    for(it = NEUTCCQELikeSelections.begin(); it != NEUTCCQELikeSelections.end(); ++it)
    {
        if(*it) delete *it;
    }
    NEUTCCQELikeSelections.clear();
    TCut all_nom_sel_cut = GetNominalSelectionCut(sampleID);
    if(TString(all_nom_sel_cut.GetTitle()).Length() == 0)
    {
        P0DBANFFInterface::Error(this, TString::Format("Unable to determine sample using SampleId = %d", sampleID));
        return;
    }

    TCut tFV_Correction = tFV;
    TCut tOOFV_Correction = tOOFV;
    if(TString(all_nom_sel_cut.GetTitle()).Contains(tFVTN208.GetTitle()))
    {
        tFV_Correction = tFVTN208;
        tOOFV_Correction = tOOFVTN208;
    }

    NEUTCCQELikeSelections.resize(NMAXNEUTCCQELIKESELECTIONS);
    if(TString(additionalCuts.GetTitle()).Length() > 0)
        all_nom_sel_cut = all_nom_sel_cut && additionalCuts;

    UInt_t entry = 0;
    // all selection events
    PlottingSelectionInfo* all_nom_sel = new PlottingSelectionInfo(name, all_nom_sel_cut, title);
    NEUTCCQELikeSelections[entry++] = all_nom_sel;

    TCut nu_ccqe_topology_sel_cut = all_nom_sel_cut && tNEUTNuCCQE && tFV_Correction;
    PlottingSelectionInfo* nu_ccqe_topology_sel = new PlottingSelectionInfo("nu_ccqe_topology_sel", nu_ccqe_topology_sel_cut, "#nu CCQE");
    NEUTCCQELikeSelections[entry++] = nu_ccqe_topology_sel;

    TCut non_nu_ccqe_topology_sel_cut = all_nom_sel_cut && tNEUTNuNonCCQE && tFV_Correction;
    PlottingSelectionInfo* non_nu_ccqe_topology_sel = new PlottingSelectionInfo("non_nu_ccqe_topology_sel", non_nu_ccqe_topology_sel_cut, "#nu non-CCQE");
    NEUTCCQELikeSelections[entry++] = non_nu_ccqe_topology_sel;

    TCut antinu_ccqe_topology_sel_cut = all_nom_sel_cut && tNEUTAntiNuCCQE && tFV_Correction;
    PlottingSelectionInfo* antinu_ccqe_topology_sel = new PlottingSelectionInfo("antinu_ccqe_topology_sel", antinu_ccqe_topology_sel_cut, "#bar{#nu} CCQE");
    NEUTCCQELikeSelections[entry++] = antinu_ccqe_topology_sel;

    TCut non_antinu_ccqe_topology_sel_cut = all_nom_sel_cut && tNEUTAntiNuNonCCQE && tFV_Correction;
    PlottingSelectionInfo* non_antinu_ccqe_topology_sel = new PlottingSelectionInfo("non_antinu_ccqe_topology_sel", non_antinu_ccqe_topology_sel_cut, "#bar{#nu} nonCCQE");
    NEUTCCQELikeSelections[entry++] = non_antinu_ccqe_topology_sel;

    // OOFV
    TCut oofv_sel_cut = all_nom_sel_cut && tOOFV_Correction;
    PlottingSelectionInfo* oofv_sel = new PlottingSelectionInfo("oofv_sel", oofv_sel_cut, "OOFV");
    NEUTCCQELikeSelections[entry++] = oofv_sel;

    // sand muons
    TCut sandmu_sel_cut = all_nom_sel_cut && tSand;
    PlottingSelectionInfo* sandmu_sel = new PlottingSelectionInfo("sandmu_sel", sandmu_sel_cut, "Sand muons");
    NEUTCCQELikeSelections[entry++] = sandmu_sel;

    if(entry != NMAXNEUTCCQELIKESELECTIONS)
    {
        P0DBANFFInterface::Error(this, "There is a mismatch between the number of cuts in DefineCuts::FillNEUTCCQELikeSelections");
    }

}
TCut DefineCuts::GetNominalSelectionCut(const SampleId::SampleEnum &sampleID) const
{
    if(sampleID == samples.GetP0DWaterNuMuCCEnum())                       return muMinusCCSelection;
    if(sampleID == samples.GetP0DWaterNuMuCC1TrackEnum())                 return muMinusCC1TrackSelection;
    if(sampleID == samples.GetP0DWaterNuMuCCNTracksEnum())                return muMinusCCNTracksSelection;
    if(sampleID == samples.GetP0DWaterNuMuBarInAntiNuModeCCEnum())        return muPlusInRHCCCSelection;
    if(sampleID == samples.GetP0DWaterNuMuBarInAntiNuModeCC1TrackEnum())  return muPlusInRHCCC1TrackSelection;
    if(sampleID == samples.GetP0DWaterNuMuBarInAntiNuModeCCNTracksEnum()) return muPlusInRHCCCNTracksSelection;
    if(sampleID == samples.GetP0DWaterNuMuBkgInAntiNuModeCCEnum())        return muMinusBkgInRHCCCSelection;
    if(sampleID == samples.GetP0DWaterNuMuBkgInAntiNuModeCC1TrackEnum())  return muMinusBkgInRHCCC1TrackSelection;
    if(sampleID == samples.GetP0DWaterNuMuBkgInAntiNuModeCCNTracksEnum()) return muMinusBkgInRHCCCNTracksSelection;
    if(sampleID == samples.GetP0DAirNuMuCCEnum())                         return muMinusCCSelection;
    if(sampleID == samples.GetP0DAirNuMuCC1TrackEnum())                   return muMinusCC1TrackSelection;
    if(sampleID == samples.GetP0DAirNuMuCCNTracksEnum())                  return muMinusCCNTracksSelection;
    if(sampleID == samples.GetP0DAirNuMuBarInAntiNuModeCCEnum())          return muPlusInRHCCCSelection;
    if(sampleID == samples.GetP0DAirNuMuBarInAntiNuModeCC1TrackEnum())    return muPlusInRHCCC1TrackSelection;
    if(sampleID == samples.GetP0DAirNuMuBarInAntiNuModeCCNTracksEnum())   return muPlusInRHCCCNTracksSelection;
    if(sampleID == samples.GetP0DAirNuMuBkgInAntiNuModeCCEnum())          return muMinusBkgInRHCCCSelection;
    if(sampleID == samples.GetP0DAirNuMuBkgInAntiNuModeCC1TrackEnum())    return muMinusBkgInRHCCC1TrackSelection;
    if(sampleID == samples.GetP0DAirNuMuBkgInAntiNuModeCCNTracksEnum())   return muMinusBkgInRHCCCNTracksSelection;

    return TCut();
}
