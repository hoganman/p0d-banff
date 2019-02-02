#!/usr/bin/env python

import ROOT
from ROOT import gSystem
import sys
from os import getenv, system


def Run(command):
    print command
    system(command)


class Kinematic(object):
    """stores the true and reco kinematic variables"""
    ISRESIDUAL = 1

    def __init__(self, recoVar="", trueVar="", isResidualVar=not ISRESIDUAL, histogramTuple=(), xAxisTitle="", minBinWidth=100):
        self.recoVar = "-x \"%s\"" % recoVar
        self.trueVar = "-t \"%s\"" % trueVar
        self.isResidualVar = "-r \"%d\"" % isResidualVar
        self.histogramTuple = "-h \"%s\"" % str(histogramTuple)
        self.xAxisTitle = "-T \"%s\"" % xAxisTitle
        self.minBinWidth = "-m \"%.3f\"" % minBinWidth

    def __str__(self):
        return " ".join([self.recoVar, self.trueVar, self.isResidualVar,
                         self.histogramTuple, self.xAxisTitle,
                         self.minBinWidth])


class SampleDef(object):
    """"""

    def __init__(self, name="", sampleID=0, cuts=""):
        self.name = name
        self.sampleID = sampleID
        self.cuts = cuts


class OneDimBinning(object):
    """Each instance of this class is run with the __str__ method"""
    EXECUTABLE = "%s/bin/oneDimResidualsWithSpecialErrorTreatment.exe" % (getenv("P0DBANFFROOT"))

    def __init__(self, sampleInputs=SampleDef(), kinematicInputs=Kinematic()):
        self.sampleID = "-i \"%d\"" % sampleInputs.sampleID
        self.outputName = "-o \"%s\"" % sampleInputs.name
        self.cuts = "-c \"%s\"" % sampleInputs.cuts
        self.kinematics = str(kinematicInputs)

    def __str__(self):
        return " ".join([OneDimBinning.EXECUTABLE, self.sampleID,
                         self.outputName, self.kinematics, self.cuts])


def main(argv):

    LoadP0DBANFF()

    momentum = Kinematic("LeptonMomNom*1e-3", "tLeptonMomentum*1e-3",
                         not Kinematic.ISRESIDUAL, (300, 0, 30),
                         "Muon Momentum [GeV/c]", 0.1)
    momentumTag = "Momentum"
    angle = Kinematic("TMath::ACos(LeptonCosNom)*TMath::RadToDeg()",
                      "TMath::RadToDeg()*(acos(tLeptonCosTheta)-acos(LeptonCosNom))",
                      Kinematic.ISRESIDUAL, (50, 0, 90), "Muon Angle [degrees]",
                      1.0)
    angleTag = "Theta"

    all_kinematics = [momentum, angle]

    samples = ROOT.SampleId()
    defCuts = ROOT.DefineCuts()
    neut_nuccqe = defCuts.tNEUTNuCCQE.GetTitle()
    neut_nuccany_notccqe = "%s&&!(%s)" % (defCuts.tNEUTNuCCAny.GetTitle(), neut_nuccqe)

    numuCC1TrackWater = SampleDef("numuCC1TrackWater", samples.GetP0DWaterNuMuCC1Track(), neut_nuccqe)
    numuCCNTracksWater = SampleDef("numuCCNTracksWater", samples.GetP0DAirNuMuCCNTracks(), neut_nuccany_notccqe)
    numuCC1TrackAir = SampleDef("numuCC1TrackAir", samples.GetP0DAirNuMuCC1Track(), neut_nuccqe)
    numuCCNTracksAir = SampleDef("numuCCNTracksAir", samples.GetP0DAirNuMuCCNTracks(), neut_nuccany_notccqe)

    neut_antinuccqe = defCuts.tNEUTAntiNuCCQE.GetTitle()
    neut_antinuccany_notccqe = "%s&&!(%s)" % (defCuts.tNEUTAntiNuCCAny.GetTitle(), neut_antinuccqe)
    numubarCC1TrackWater = SampleDef("numubarCC1TrackWater", samples.GetP0DWaterNuMuBarInAntiNuModeCC1Track(), neut_antinuccqe)
    numubarCCNTracksWater = SampleDef("numubarCCNTracksWater", samples.GetP0DWaterNuMuBarInAntiNuModeCCNTracks(), neut_antinuccany_notccqe)
    numubarCC1TrackAir = SampleDef("numubarCC1TrackAir", samples.GetP0DAirNuMuBarInAntiNuModeCC1Track(), neut_antinuccqe)
    numubarCCNTracksAir = SampleDef("numubarCCNTracksAir", samples.GetP0DAirNuMuBarInAntiNuModeCCNTracks(), neut_antinuccany_notccqe)

    numubkgCC1TrackWater = SampleDef("numubkgCC1TrackWater", samples.GetP0DWaterNuMuBkgInAntiNuModeCC1Track(), neut_nuccqe)
    numubkgCCNTracksWater = SampleDef("numubkgCCNTracksWater", samples.GetP0DWaterNuMuBkgInAntiNuModeCCNTracks(), neut_nuccany_notccqe)
    numubkgCC1TrackAir = SampleDef("numubkgCC1TrackAir", samples.GetP0DAirNuMuBkgInAntiNuModeCC1Track(), neut_nuccqe)
    numubkgCCNTracksAir = SampleDef("numubkgCCNTracksAir", samples.GetP0DAirNuMuBkgInAntiNuModeCCNTracks(), neut_nuccany_notccqe)

    numu_samples = [numuCC1TrackWater, numuCCNTracksWater, numuCC1TrackAir, numuCCNTracksAir]
    numubar_samples = [numubarCC1TrackWater, numubarCCNTracksWater, numubarCC1TrackAir, numubarCCNTracksAir]
    numubkg_samples = [numubkgCC1TrackWater, numubkgCCNTracksWater, numubkgCC1TrackAir, numubkgCCNTracksAir]
    all_samples = numu_samples + numubar_samples + numubkg_samples

    all_samples = numubkg_samples

    for kinematic in all_kinematics:
        kinematicTag = momentumTag if momentumTag in kinematic.trueVar else angleTag
        for sample in all_samples:
            sample_nameChange = SampleDef(sample.name + kinematicTag, sample.sampleID, sample.cuts)
            oneDimBinRecommendations = OneDimBinning(sample_nameChange, kinematic)
            Run(str(oneDimBinRecommendations))


def LoadP0DBANFF():
    """Load in the library"""
    if len(getenv('P0DBANFFROOT')) <= 0:
        print 'P0DBANFFROOT NOT exported. Please export it now'
        sys.exit(1)
    loadStatus = gSystem.Load("libP0DBANFF")
    if not (loadStatus == 0 or loadStatus == 1):
        print "Unable to load libP0DBANFF.so. gSystem.Load(\"libP0DBANFF\") returned", loadStatus
        sys.exit(1)


if __name__ == "__main__":
    main(sys.argv[1:])
