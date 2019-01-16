#!/usr/bin/env python

import ROOT
from ROOT import gSystem
import sys
from os import getenv

INTERFACE = None


class Kinematic(object):
    """stores the true and reco kinematic variables"""
    ISRESIDUAL = 1

    def __init__(self, recoVar="", trueVar="", isResidualVar=(not ISRESIDUAL), histogramTuple=(), xAxisTitle="", minBinWidth=100):
        self.recoVar = "-x \"%s\"" % recoVar
        self.trueVar = "-t \"%s\"" % trueVar
        self.isResidualVar = "-r \"%d\"" % isResidualVar
        self.xAxisTitle = "-T \"%s\"" % xAxisTitle
        self.minBinWidth = "-m \"%.3f\"" % minBinWidth

    def __str__(self):
        return " ".join([self.recoVar, self.trueVar, self.isResidualVar,
                         self.xAxisTitle, self.minBinWidth])


class oneDimBinning(object):
    """Each instance of this class is run with the __str__ method"""
    EXECUTABLE = "%s/oneDimResidualsWithSpecialErrorTreatment.exe" % (getenv("P0DBANFFROOT"))

    def __init__(self, sampleID=0, outputName="", kinematicInputs=Kinematic(), cuts=""):
        self.sampleID = "-i \"%d\"" % sampleID
        self.outputName = "-o \"%s\"" % outputName
        self.kinematicInputs = str(kinematicInputs)
        self.cuts = "-c \"%s\"" % cuts

    def __str__(self):
        return " ".join([self.sampleID, self.outputName, self.kinematicInputs, self.cuts])


class sampleDef(object):
    """"""

    def __init__(self, sampleID=0, sampleCut=""):
        self.sampleID = sampleID
        self.sampleCut = sampleCut


def main(argv):

    LoadP0DBANFF()

    momentum = Kinematic("LeptonMomNom*1e-3", "tLeptonMomentum*1e-3",
                         not Kinematic.ISRESIDUAL, (300, 0, 30),
                         "Muon Momentum [GeV/c]", 0.1)
    angle = Kinematic("TMath::ACos(LeptonCosNom)*TMath::RadToDeg()",
                      "TMath::RadToDeg()*(acos(tLeptonCosTheta)-acos(LeptonCosNom))",
                      Kinematic.ISRESIDUAL, (50, 0, 90), "Muon Angle [degrees]",
                      1.0)
    print str(momentum)
    print str(angle)

    samples = ROOT.SampleId()
    neut_nuccqe = "tReactionCode==1"
    neut_nuccany_notccqe = "1<tReactionCode&&tReactionCode<31"
    neut_antinuccqe = "tReactionCode==-1"
    neut_antinuccany_notccqe = "-31<tReactionCode&&tReactionCode<-1"
    all_samples = []

    all_samples.append(sampleDef(samples.GetP0DWaterNuMuCC1Track(), neut_nuccqe))
    all_samples.append(sampleDef(samples.GetP0DAirNuMuCC1Track(), neut_nuccqe))
    all_samples.append(sampleDef(samples.GetP0DAirNuMuCCNTracks(), neut_nuccany_notccqe))
    all_samples.append(sampleDef(samples.GetP0DWaterNuMuCCNTracks(), neut_nuccany_notccqe))

    all_samples.append(sampleDef(samples.GetP0DWaterNuMuBarInAntiNuModeCC1Track(), neut_antinuccqe))
    all_samples.append(sampleDef(samples.GetP0DAirNuMuBarInAntiNuModeCC1Track(), neut_antinuccqe))
    all_samples.append(sampleDef(samples.GetP0DWaterNuMuBarInAntiNuModeCCNTracks(), neut_antinuccany_notccqe))
    all_samples.append(sampleDef(samples.GetP0DAirNuMuBarInAntiNuModeCCNTracks(), neut_antinuccany_notccqe))

    all_samples.append(sampleDef(samples.GetP0DWaterNuMuBkgInAntiNuModeCC1Track(), neut_nuccqe))
    all_samples.append(sampleDef(samples.GetP0DAirNuMuBkgInAntiNuModeCC1Track(), neut_nuccqe))
    all_samples.append(sampleDef(samples.GetP0DWaterNuMuBkgInAntiNuModeCCNTracks(), neut_nuccany_notccqe))
    all_samples.append(sampleDef(samples.GetP0DAirNuMuBkgInAntiNuModeCCNTracks(), neut_nuccany_notccqe))


def LoadP0DBANFF():
    """Load in the necessary classes"""
    if len(getenv('P0DBANFFROOT')) <= 0:
        print 'P0DBANFFROOT NOT exported. Please export it now'
        sys.exit(1)
    engine = ROOT.TXMLEngine()
    loadStatus = gSystem.Load("libP0DBANFF")
    if not (loadStatus == 0 or loadStatus == 1):
        print "Unable to load libP0DBANFF.so. gSystem.Load(\"libP0DBANFF\") returned", loadStatus
        sys.exit(1)
    global INTERFACE, CONFIGURATION
    try:
        INTERFACE = ROOT.P0DBANFFInterface()
        INTERFACE.SetBatch(True)
        ROOT.gStyle.SetOptStat(0000)
        INTERFACE.LoadColorBlindPalette()
    except Exception as exc:
        print type(exc)
        print "unable to load libP0DBANFF.so"
        sys.exit(1)
    del engine


if __name__ == "__main__":
    main(sys.argv[1:])
    if INTERFACE:
        del INTERFACE
