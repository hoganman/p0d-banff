#!/bin/env python2

from __future__ import print_function
import ROOT
import numpy as np


def SaveCanvasAs(canvas, saveName):
    for saveFmt in ['root', 'png']:
        canvas.SaveAs('{}.{}'.format(saveName, saveFmt))


momentumCut = 'selmu_mom < 30000'

ROOT.gROOT.SetBatch()

MAX_ACCUM_LEVEL = '4'
CCInclusiveCut = 'accum_level > {}'.format(MAX_ACCUM_LEVEL)
CC1TrackCut = 'accum_level[][0] > {}'.format(MAX_ACCUM_LEVEL)
CCNTracksCut = 'accum_level[][1] > {}'.format(MAX_ACCUM_LEVEL)

cuts = {
        'NumuFHCCCInc': CCInclusiveCut,
        'NumuFHCCC1Track': CC1TrackCut,
        'NumuFHCCCNTracks': CCNTracksCut
        }
categories = ["reaction", "topology", "nutype", "target"]
# categories = ["particle", "nutype", "target"]
# categories = ["particle"]

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
        # 'nominal_mc': 'NOVARBIN NOW',
        'fluxtuned_systematicweighted': 'NOVARBIN'
        }

import ROOT
# open the config file
configFile = open('run4_water_out.cfg')
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
dataMCPOTRatio = 3.66E20/mcSample.GetPOTGoodBeamGoodND280()

canvas = None

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
                canvas = ROOT.TCanvas('canvas', '', 800, 600)
                canvas.cd()
                drawingTools.SetLegendPos(legendPositions[key])
                drawingTools.SetTitleX(kinematicTitles[key])
                outputName = '{}_{}_{}_{}'.format(cutName, key, category, optionName)
                print('output name =', outputName)
                canvas_1, pad1, pad2, hist = None, None, None, None
                if 'particle' in category:
                    canvas.cd()
                    pad1 = ROOT.TPad("pad1", "pad1", 0, 0.3, 1, 1.0)
                    pad1.SetBottomMargin(0)
                    pad1.Draw()
                    pad1.cd()
                drawingTools.Draw(mcSample, kinematicVar, nBins, bins, category, allCuts, "", optionStr, dataMCPOTRatio)
                if pad1:
                    hist = pad1.GetPrimitive(pad1.GetListOfPrimitives().At(2).GetName())
                else:
                    hist = canvas.GetPrimitive(canvas.GetListOfPrimitives().At(2).GetName())
                # hist.GetYaxis().SetTitleOffset(1.25*hist.GetYaxis().GetTitleOffset())
                # hist.GetYaxis().SetTitleSize(1.25*hist.GetYaxis().GetTitleSize())
                # hist.GetXaxis().SetTitleSize(1.35*hist.GetXaxis().GetTitleSize())
                # hist.GetYaxis().SetTitle('Events / bin / (3.66 x 10^{20} PoT)')
                # hist.GetXaxis().SetTitle(kinematicTitles[key])
                ROOT.gPad.Update()
                if 'particle' in category:
                    hist.GetYaxis().SetLabelSize(0.)
                    axis = ROOT.TGaxis(-5, 20, -5, 220, 20, 220, 510, "")
                    axis.SetLabelFont(43)
                    axis.SetLabelSize(15)
                    axis.Draw()

                    canvas.cd()
                    pad2 = ROOT.TPad("pad2", "pad2", 0, 0.05, 1, 0.3)
                    pad2.SetTopMargin(0)
                    pad2.SetBottomMargin(0.2)
                    pad2.SetGridx()
                    pad2.Draw()
                    pad2.cd()

                    ratio = ROOT.TH1D('ratio', '', hist.GetNbinsX(), hist.GetBinLowEdge(1), hist.GetBinLowEdge(hist.GetNbinsX()+1))
                    for histbin in range(1, hist.GetNbinsX()+1):
                        ratio.SetBinContent(histbin, hist.GetBinContent(histbin))
                    ratio.SetMinimum(0.0)
                    ratio.SetMaximum(3.0)
                    ratio.Sumw2()
                    ratio.SetStats(0)

                    canvas_1 = ROOT.TCanvas("canvas_1", "", 800, 600)
                    canvas_1.cd()
                    optionStr_1 = "NOVARBIN NOW"
                    drawingTools.Draw(mcSample, kinematicVar, nBins, bins, category, allCuts, "", optionStr_1, dataMCPOTRatio)
                    # for i in range(canvas_1.GetListOfPrimitives().GetSize()):
                    #     print(canvas_1.GetListOfPrimitives().At(i).GetName(), type(canvas_1.GetListOfPrimitives().At(i)))
                    _all = None  # canvas_1.GetPrimitive('_all_66_67')
                    for objindex in range(canvas_1.GetListOfPrimitives().GetSize()):
                        _all = canvas_1.GetListOfPrimitives().At(objindex)
                        if '_all' in _all.GetName():
                            break
                    nominal = ROOT.TH1D('nominal', '', _all.GetNbinsX(), _all.GetBinLowEdge(1), _all.GetBinLowEdge(_all.GetNbinsX()+1))
                    for histbin in range(1, _all.GetNbinsX()+1):
                        nominal.SetBinContent(histbin, _all.GetBinContent(histbin))
                    nominal.SetLineColor(ROOT.kYellow)
                    nominal.SetLineWidth(3)
                    nominal.SetLineStyle(9)
                    ratio.Divide(nominal)
                    ratio.SetMarkerStyle(21)
                    canvas.cd()
                    pad2.cd()
                    ratio.Draw("ep")
                    line = ROOT.TLine(ratio.GetXaxis().GetBinLowEdge(1), 1, ratio.GetXaxis().GetBinUpEdge(ratio.GetNbinsX()), 1)
                    line.SetLineColor(ROOT.kBlue)
                    line.SetLineWidth(3)
                    line.SetLineStyle(9)
                    line.Draw()
                    # Y axis ratio plot settings
                    ratio.GetYaxis().SetTitle("Ratio to Nom")
                    ratio.GetYaxis().SetNdivisions(505)
                    ratio.GetYaxis().SetLabelSize(15)
                    ratio.GetYaxis().SetTitleOffset(1.55)
                    ratio.GetYaxis().SetTitleSize(20)
                    ratio.GetYaxis().SetTitleFont(43)
                    ratio.GetYaxis().SetLabelFont(43)

                    # X axis ratio plot settings
                    ratio.GetXaxis().SetTitle(kinematicTitles[key])
                    ratio.GetXaxis().SetTitleSize(20)
                    ratio.GetXaxis().SetTitleFont(43)
                    ratio.GetXaxis().SetTitleOffset(4.)
                    ratio.GetXaxis().SetLabelFont(43)
                    ratio.GetXaxis().SetLabelSize(15)

                    pad2.Update()
                    canvas.cd()
                    pad1.cd()
                    nominal.Draw("SAME")
                    pad1_Legend = pad1.GetPrimitive('TPave')
                    pad1_Legend.AddEntry(nominal, 'Nominal MC', 'L')
                    pad1_Legend.Draw()
                    pad1.Update()
                canvas.Update()
                SaveCanvasAs(canvas, 'plots/Run4_Water/' + outputName)
                del pad1
                del pad2
                del canvas_1

# drawingTools.DrawRelativeErrors(mcFile.Get('all_syst'), 'selmu_mom*1e-3', 50., 0., 5., CC1TrackCut, '', 'SYS')

# mcFile.Close()
