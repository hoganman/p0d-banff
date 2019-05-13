#!/bin/env python2

from __future__ import print_function
import ROOT
import numpy as np
import os


def SaveCanvasAs(canvas, saveName):
    for saveFmt in ['root', 'png']:
        canvas.SaveAs('{}.{}'.format(saveName, saveFmt))


momentumCut = 'selmu_mom < 30000'

ROOT.gROOT.SetBatch()
dataMCPOTRatio = 2.85E20/3.37046474629697031E+21  # drawingTools.GetPOTRatio(dataSample, mcSample)

CCInclusiveCut = 'accum_level > 5'
CC1TrackCut = 'accum_level[][0] > 5'
CCNTracksCut = 'accum_level[][1] > 5'

cuts = {
        'NumubarinRHCCCInc': CCInclusiveCut,
        'NumubarInRHCCC1Track': CC1TrackCut,
        'NumubarInRHCCCNTracks': CCNTracksCut
        }
categories = ["particle", "reaction", "topology", "nutype", "target"]
kinematicVariables = {
        'momentum': 'selmu_mom*1e-3',
        'costheta': 'selmu_dir[2]',
        'enu': 'nu_trueE*1e-3'
             }
kinematicTitles = {
        'momentum': 'Lepton Candidate Momentum [GeV/c]',
        'costheta': 'Lepton Candidate Track Angle [cos(#theta)]',
        'enu': 'True Neutrino Energy [GeV]'
        }

legendPositions = {
        'momentum': 'tr',
        'costheta': 'tl',
        'enu': 'tc'
        }

binning = {
        'momentum': np.array([0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 4.1, 4.2, 4.3, 4.4, 4.5, 4.6, 4.7, 4.8, 4.9, 5.0]),
        'costheta': np.array([0.0, 0.5, 0.52, 0.54, 0.56, 0.58, 0.6, 0.62, 0.64, 0.66, 0.68, 0.7, 0.72, 0.74, 0.76, 0.78, 0.8, 0.82, 0.84, 0.86, 0.88, 0.9, 0.92, 0.94, 0.9615, 0.98, 0.9925, 1.0]),
        'enu': np.array([i*0.1 for i in range(51)])
          }
options = {
        'nominal_mc': 'NOVARBIN NOW',
        'fluxtuned_systematicweighted': 'NOVARBIN'
        }

# open the config file
configFile = open(os.path.join(os.getenv('P0DBANFFROOT'), 'config/p0dAntiNumuCCMultiTrack/run7b-water.cfg'))
contents = configFile.readlines()[0].strip().split(' ')
configFile.close()
runName = contents[0]
del contents[0]
fileDict = dict()
for line in contents:
    items = line.split(':')
    key, value = items[0], items[1]
    fileDict[key] = value

mcFile = ROOT.TFile(fileDict['MC'])
dataFile = ROOT.TFile(fileDict['Data'])

drawingTools = ROOT.DrawingTools(mcFile.GetName())
mcSample = ROOT.DataSample(mcFile.GetName())
dataSample = ROOT.DataSample(dataFile.GetName())

canvas = ROOT.TCanvas("canvas", "", 800, 600)
canvas.cd()

# void Draw(DataSample& data, const string& name, int nbins, double* xbins, const string& categ = "all", const string& cut = "", const string& root_opt = "", const string& opt = "", double norm = -1, bool scale_errors = true)

for cutName, cutStr in cuts.iteritems():
    allCuts = '{} && {}'.format(cutStr, momentumCut)
    for key in kinematicVariables.keys():
        kinematicVar = kinematicVariables[key]
        bins = binning[key]
        nBins = len(bins) - 1
        for category in categories:
            for optionName, optionStr in options.iteritems():
                if 'nutype' in category or 'target' in category:
                    if 'enu' not in key:
                        continue
                    else:
                        optionStr = optionStr + " OVER"
                drawingTools.SetLegendPos(legendPositions[key])
                drawingTools.SetTitleX(kinematicTitles[key])
                outputName = '{}_{}_{}_{}'.format(cutName, key, category, optionName)
                print('output name =', outputName)
                drawingTools.Draw(mcSample, kinematicVar, nBins, bins, category, allCuts, "", optionStr, dataMCPOTRatio)
                hist = canvas.GetPrimitive(canvas.GetListOfPrimitives().At(1).GetName())
                hist.GetYaxis().SetTitleOffset(1.5*hist.GetYaxis().GetTitleOffset())
                hist.GetYaxis().SetTitle('Events / bin / (2.85 x 10^{20} PoT)')
                canvas.Update()
                SaveCanvasAs(canvas, outputName)

# drawingTools.DrawRelativeErrors(mcFile.Get('all_syst'), 'selmu_mom*1e-3', 50., 0., 5., CC1TrackCut, '', 'SYS')

# mcFile.Close()
