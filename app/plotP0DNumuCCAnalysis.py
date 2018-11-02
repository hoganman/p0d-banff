#!/usr/bin/env python2

"""
This app takes the input Experiments from config/p0dNumuCCAnalysis
and plots the muon candidate information. Use this to compare to
plotRunSyst_New.py
"""

from os import getenv
import ROOT
from ROOT import gSystem
import sys

# toggle these to draw particular variables
DRAW_PMU = 1
DRAW_COSTHETAMU = 1

# Apply event + flux weight
APPLY_FLUX_WEIGHTS = 0
APPLY_EVENT_WEIGHTS = 0

RUNP0DWATERNUMUCCSELECTION = 1
RUNP0DAIRNUMUCCSELECTION = 1

# Display the ratio of Data/MC below histogram
SHOW_RATIO_PLOT_BELOW = 0

# cut the measured momentum
USE_MOMENTUM_CUT = 0
MOMENTUM_CUT_VALUE = '5000.'

# if the user wants to apply an addition set of cuts
ADDITIONAL_CUTS = 'accum_level[0]>3'

# Which categories to plot
HL2CATAGORIES =\
    {
            'particle': 1,
            'reaction': 1,
            'topology': 1
    }


def main(argv):
    """main"""
    LoadP0DBANFF()

    drawingToolsWaterIn, waterInExp = LoadWaterInExperiment()
    waterInMode = DrawingToolsExperimentPair(drawingToolsWaterIn, waterInExp,
                                             RUNP0DWATERNUMUCCSELECTION)

    # drawingToolsWaterOut, waterOutExp = LoadWaterOutExperiment()
    # waterOutMode = DrawingToolsExperimentPair(drawingToolsWaterOut, waterOutExp,
    #                                           RUNP0DAIRNUMUCCSELECTION)

    binningLocation = '%s/config/Binning.xml' % P0DBANFFROOT
    cosThetaMu_AnaBins = ROOT.AnalysisBins('CosTheta', binningLocation, XML)
    pMu_AnaBins = ROOT.AnalysisBins('Momentum', binningLocation, XML)

    # for p0dMode in (waterInMode, waterOutMode):
    for p0dMode in (waterInMode, ):
        if not p0dMode.Draw:
            continue
        print 'Draw: ', p0dMode
        for category, DrawStatus in HL2CATAGORIES.items():
            if not DrawStatus:
                continue
            print 'category: ', category

            if DRAW_PMU:
                # th1_pmu = ROOTTH1()
                # th1_pmu.
                Draw(p0dMode, category, pMu_AnaBins)

            if DRAW_COSTHETAMU:
                Draw(p0dMode, category, cosThetaMu_AnaBins)

    return


def LoadP0DBANFF():
    """Load in the necessary classes"""
    global P0DBANFFROOT
    P0DBANFFROOT = getenv('P0DBANFFROOT')
    global ENGINE
    try:
        ENGINE = ROOT.TXMLEngine()
    except Exception as exc:
        print type(exc)
        print "Unable to load ROOT"
        sys.exit(1)
    loadStatus = gSystem.Load("libP0DBANFF")
    if not (loadStatus == 1 or loadStatus == 0):
        print "Unable to load libP0DBANFF.so. gSystem.Load(\"libP0DBANFF\") returned", loadStatus
        sys.exit(1)
    global INTERFACE
    try:
        INTERFACE = ROOT.P0DBANFFInterface()
        INTERFACE.SetBatch(True)
        INTERFACE.GetThisStyle().SetOptStat(0000)
    except Exception as exc:
        print type(exc)
        print "unable to load libP0DBANFF.so"
        sys.exit(1)
    global XML
    XML = ROOT.XMLTools()


def LoadWaterInExperiment():
    """
    Create a DrawingTools and Experiment class for the
    Run2 and Run4 water-in periods
    """
    # This loads the necessary libraries for the Experiment class
    drawingTools = ROOT.DrawingTools()
    configFile = '%s/config/p0dNumuCCAnalysis/fhc_water-in.cfg' % P0DBANFFROOT
    waterInExp = ROOT.Experiment('waterIn', configFile)
    # The drawing options are set now
    drawingTools = ROOT.DrawingTools(waterInExp)
    return drawingTools, waterInExp


def LoadWaterOutExperiment():
    """
    Create a DrawingTools and Experiment class for the
    Run2, 3b, 3c, and 4 water-out periods
    """
    # This loads the necessary libraries for the Experiment class
    drawingTools = ROOT.DrawingTools()
    configFile = '%s/config/p0dNumuCCAnalysis/fhc_water-out.cfg' % P0DBANFFROOT
    waterOutExp = ROOT.Experiment('waterOut', configFile)
    # The drawing options are set now
    drawingTools = ROOT.DrawingTools(waterOutExp)
    return drawingTools, waterOutExp


class DrawingToolsExperimentPair(object):
    """
    A struct to contain both the DrawingTools and Experiment classes
    """

    def __init__(self, DrawingTools, Experiment, DrawStatus=True):
        self.DrawingTools = DrawingTools
        self.Experiment = Experiment
        self.Draw = DrawStatus


def Draw(p0dModePair, category, anaBins):
    """
    With the P0DModePair (DrawingToolsExperimentPair), draw the category (str)
    using the anaBins (AnalysisBins) as the input histogram bins
    """
    DrawingTools = p0dModePair.DrawingTools
    Experiment = p0dModePair.Experiment

    # Unfortunately, in order to get the flux weight index requies this command
    # DrawingTools::syst().GetSystematicWeightIndex('FluxWeight'), BUT
    # the SystematicsManager class does not work in CINT nor PyROOT.
    # I added a method called GetSystematicWeightIndex in DrawingToolsBase as a
    # bypass. Adding a try here in case the code is updated without this bypass
    try:
        fluxWeightIndex = DrawingTools.GetSystematicWeightIndex('FluxWeight')
    except Exception as exc:
        print 'There was a failure in plotP0DNumuCCAnalysis.py'
        print exc
        print 'The method DrawingTools.GetSystematicWeightIndex(\'FluxWeight\') failed'
        print 'This has happened because you are using a version of HighlandTools'
        print 'without a hack in DrawingToolsBase where the SystematicsManager::GetSystematicWeightIndex'
        print 'is reimplemented in DrawingToolsBase. Add these lines to'
        print 'your $HIGHLANDTOOLSROOT/src/DrawingToolsBase.hxx to fix this problem'
        print 'Int_t GetSystematicWeightIndex(const std::string &name){'
        print '    return syst().GetSystematicWeightIndex(name);'
        print '}'
        sys.exit(1)

    print 'fluxWeightIndex =', fluxWeightIndex
    # Draw (Experiment &exp, const std::string &var, int nbins, double *xbins, const std::string &categ="all", const std::string &cut="", const std::string &root_opt="", const std::string &opt="", double norm=-1., bool scale_errors=true)
    # if
    DrawingTools, Experiment
    return


if __name__ == "__main__":
    main(sys.argv[1:])
