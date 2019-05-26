#!/bin/env python2

from __future__ import print_function
import ROOT
import sys
import numpy as np
import optparse


def SaveCanvasAs(canvas, saveName):
    for saveFmt in ['root', 'png']:
        canvas.SaveAs('{}.{}'.format(saveName, saveFmt))


momentumCut = 'selmu_mom < 30000'
MAX_ACCUM_LEVEL = '4'
CCInclusiveCut = 'accum_level > {}'.format(MAX_ACCUM_LEVEL)
CC1TrackCut = 'accum_level[][0] > {}'.format(MAX_ACCUM_LEVEL)
CCNTracksCut = 'accum_level[][1] > {}'.format(MAX_ACCUM_LEVEL)

ROOT.gROOT.SetBatch()


def main(argv):

    parser = optparse.OptionParser()
    parser.add_option('-r', '--ratio', dest='ratio',
                      default=False, action='store_true',
                      help='Run make a ratio plot only')
    parser.add_option('-i', '--inclusive', dest='inclusive',
                      default=False, action='store_true',
                      help='Run CCInc only')
    parser.add_option('-o', '--oneTrack', dest='oneTrack',
                      default=False, action='store_true',
                      help='Run CC 1-track only')
    parser.add_option('-N', '--NTracks', dest='NTracks',
                      default=False, action='store_true',
                      help='Run CC n-track only')
    parser.add_option('-p', '--momentum', dest='momentum',
                      default=False, action='store_true',
                      help='Analyze momentum')
    parser.add_option('-q', '--costheta', dest='costheta',
                      default=False, action='store_true',
                      help='Analyze costheta')
    parser.add_option('-E', '--Enu', dest='Enu',
                      default=False, action='store_true',
                      help='Analyze Enu')
    parser.add_option('-n', '--nominal', dest='nominal',
                      default=False, action='store_true',
                      help='Turn off all weights')
    parser.add_option('-w', '--weighted', dest='weighted',
                      default=False, action='store_true',
                      help='Use all weights')
    options, args = parser.parse_args()

    cuts = {}
    if options.inclusive:
        cuts['NumuFHCCCInc'] = CCInclusiveCut
    elif options.oneTrack:
        cuts['NumuFHCCC1Track'] = CC1TrackCut
    elif options.NTracks:
        cuts['NumuFHCCCNTracks'] = CCNTracksCut
    else:
        print("Could not determine which samples, select Inc, 1-, or N-Tracks")
        options.print_help()
        return

    categories = ["particle"]
    if not options.ratio:
        categories = ["reaction", "topology", "nutype", "target"]

    kinematicVariables, kinematicTitles, legendPositions, binning = {}, {}, {}, {}
    if options.momentum:
        kinematicVariables['momentum'] = 'selmu_mom*1e-3'
        kinematicTitles['momentum'] = 'Lepton Candidate Momentum [GeV/c]'
        legendPositions['momentum'] = 'tr'
        binning['momentum'] = np.array([0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 4.1, 4.2, 4.3, 4.4, 4.5, 4.6, 4.7, 4.8, 4.9, 5.0])

    if options.costheta:
        kinematicVariables['costheta'] = 'selmu_dir[2]'
        kinematicTitles['costheta'] = 'Lepton Candidate Track Angle [cos(#theta)]'
        legendPositions['costheta'] = 'tl'
        binning['costheta'] = np.array([0.0, 0.5, 0.52, 0.54, 0.56, 0.58, 0.6, 0.62, 0.64, 0.66, 0.68, 0.7, 0.72, 0.74, 0.76, 0.78, 0.8, 0.82, 0.84, 0.86, 0.88, 0.9, 0.92, 0.94, 0.9615, 0.98, 0.9925, 1.0])
    if options.Enu:
        kinematicVariables['Enu'] = 'nu_trueE*1e-3'
        kinematicTitles['Enu'] = 'True Neutrino Energy [GeV]'
        legendPositions['Enu'] = 'tc'
        binning['Enu'] = np.array([i*0.10 for i in range(81)])
        if int(options.inclusive) + int(options.oneTrack) == 0:
            legendPositions['Enu'] = 'tr'
            binning['Enu'] = np.array([i*0.25 for i in range(61)])

    drawOptions = {}
    if options.nominal:
        drawOptions['nominal_mc'] = 'NOVARBIN NOW'
    if options.weighted:
        drawOptions['fluxtuned_systematicweighted'] = 'NOVARBIN'

    if options.ratio and int(options.nominal) + int(options.weighted) != 1:
        print('For ratio plots to work, you must only use one at a time: nominal or weighted')
        options.print_help()
        return
    if options.ratio and int(options.momentum) + int(options.costheta) + int(options.Enu) != 1:
        print('For ratio plots to work, you must only use one at a time: costheta, momentum or Enu')
        options.print_help()
        return
    if options.ratio and int(options.inclusive) + int(options.oneTrack) + int(options.NTracks) != 1:
        print('For ratio plots to work, you must only use one at a time: CCInc, CC 1track, or CC Ntracks')
        options.print_help()
        return

    # open the config file
    configFile = open('run4-water_MC_As_Data.cfg')
    contents = configFile.readlines()[0].strip().split(' ')
    configFile.close()
    del contents[0]
    fileDict = dict()
    for line in contents:
        items = line.split(':')
        key, value = items[0], items[1]
        fileDict[key] = value

    mcFile = ROOT.TFile(fileDict['MC'])

    drawingTools = ROOT.DrawingTools(mcFile.GetName())
    mcSample = ROOT.DataSample(mcFile.GetName())
    dataMCPOTRatio = 3.66E20/mcSample.GetPOTGoodBeamGoodND280()

    canvas = None
    # void Draw(DataSample& data, const string& name, int nbins, double* xbins, const string& categ = "all", const string& cut = "", const string& root_opt = "", const string& opt = "", double norm = -1, bool scale_errors = true)
    # drawingTools.DrawRelativeErrors(mcFile.Get('all_syst'), 'selmu_mom*1e-3', 50., 0., 5., CC1TrackCut, '', 'SYS')

    for cutName, cutStr in cuts.iteritems():
        allCuts = '{} && {}'.format(cutStr, momentumCut)
        for key in kinematicVariables.keys():
            kinematicVar = kinematicVariables[key]
            bins = binning[key]
            nBins = len(bins) - 1
            for category in categories:
                for optionName, optionStr in drawOptions.iteritems():
                    if 'Enu' in key:
                        optionStr = optionStr + " OVER"
                    canvas = ROOT.TCanvas('canvas', '', 800, 600)
                    canvas.cd()
                    outputName = '{}_{}_{}_{}'.format(cutName, key, category, optionName)
                    print('output name =', outputName)
                    canvas_1, pad1, pad2, hist = None, None, None, None
                    if options.ratio and 'particle' in category:
                        canvas.cd()
                        pad1 = ROOT.TPad("pad1", "pad1", 0, 0.3, 1, 1.0)
                        pad1.SetBottomMargin(0)
                        pad1.Draw()
                        pad1.cd()
                    drawingTools.SetLegendPos(legendPositions[key])
                    drawingTools.SetTitleX(kinematicTitles[key])
                    drawingTools.SetTitleY('Events / bin')
                    drawingTools.Draw(mcSample, kinematicVar, nBins, bins, category, allCuts, "", optionStr, dataMCPOTRatio)
                    if pad1:
                        hist = pad1.GetPrimitive(pad1.GetListOfPrimitives().At(2).GetName())
                    else:
                        hist = canvas.GetPrimitive(canvas.GetListOfPrimitives().At(2).GetName())
                        canvas.GetListOfPrimitives().At(1).GetYaxis().SetTitle('Events / bin')
                    # hist.GetYaxis().SetTitle('Events / bin')
                    # hist.GetYaxis().Draw()
                    # ROOT.gPad.Update()
                    if options.ratio and 'particle' in category:
                        hist.GetYaxis().SetLabelSize(0.)
                        hist.GetYaxis().SetTitle('Events / bin')
                        pad1.GetListOfPrimitives().At(1).GetYaxis().SetTitle('Events / bin')
                        axis = ROOT.TGaxis(-5, 20, -5, 220, 20, 220, 510, "")
                        axis.SetLabelFont(43)
                        axis.SetLabelSize(15)
                        axis.SetTitle("Events / bin")
                        axis.Draw()
                        ROOT.gPad.Update()
                        canvas.cd()
                        pad1.cd()
                        pad1.Update()
                        pad1.Draw()

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

                        drawingTools_1 = ROOT.DrawingTools(mcFile.GetName())
                        mcSample_1 = ROOT.DataSample(mcFile.GetName())
                        canvas_1 = ROOT.TCanvas("canvas_1", "", 800, 600)
                        canvas_1.cd()
                        pad1_1 = ROOT.TPad("pad1_1", "pad1_1", 0, 0.3, 1, 1.0)
                        pad1_1.SetBottomMargin(0)
                        pad1_1.Draw()
                        pad1_1.cd()
                        optionStr_1 = "NOVARBIN NOW"
                        drawingTools_1.Draw(mcSample_1, kinematicVar, nBins, bins, category, allCuts, "", optionStr_1, dataMCPOTRatio)
                        nominal = None
                        for objindex in range(pad1_1.GetListOfPrimitives().GetSize()):
                            print(pad1_1.GetListOfPrimitives().At(objindex))
                            if '_all' in pad1_1.GetListOfPrimitives().At(objindex).GetName():
                                nominal = pad1_1.GetListOfPrimitives().At(objindex)
                        canvas.cd()
                        pad2.cd()
                        nominal.SetLineColor(ROOT.kYellow)
                        nominal.SetLineWidth(3)
                        nominal.SetLineStyle(9)
                        ratio.Divide(nominal)
                        ratio.SetMarkerStyle(21)
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
                    canvas.cd()
                    canvas.Update()
                    # canvas.Draw()
                    SaveCanvasAs(canvas, 'plots/Run4_Water/{}/{}'.format(cutName, outputName))
    print('DONE!')
    sys.exit(0)


if __name__ == '__main__':
    main(sys.argv)
