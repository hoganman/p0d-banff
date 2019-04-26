#!/usr/bin/env python
from os import getenv
import ROOT
from ROOT import gROOT, gSystem, kTRUE, TLine, TFile, TCanvas, TH1D, TH2D
from ROOT import THStack, TLegend, kBlack, TPad, gPad
import sys
import math.sqrt

MINMOMENTUM = 0.0
MAXMOMENTUM = 5000.0
INTERFACE = None


def FindPlotMax(dataPlot, mcPlot):

    dataBinContentPlusError = []
    mcBinContentPlusError = []

    for i in xrange(1, dataPlot.GetNbinsX() + 1):
        dataBinContentPlusError.append(
            dataPlot.GetBinContent(i) + dataPlot.GetBinError(i))
        mcBinContentPlusError.append(
            mcPlot.GetBinContent(i) + mcPlot.GetBinError(i))

    # Now, return the maximum of those two, plus 10% so that the histogram is
    # definitely big enough
    return max(max(dataBinContentPlusError), max(mcBinContentPlusError)) * 1.10


def CorrespondingIndex(rawReactionCode):
    """
    Takes an index, converts to a reaction code, and returns the corresponding
    index in the stackHistos (and hence histosToStack) list to add this
    histogram's content to.
    """
    # First subtract 100 to get a reaction code.
    reactionCode = rawReactionCode - 100
    # CCQE
    if reactionCode == 1:
        return 0
    # 2p2h
    elif reactionCode == 2:
        return 1
    # Single pion prod
    elif (11 <= reactionCode) and (reactionCode <= 13):
        return 2
    # Coherent pion prod
    elif reactionCode == 16:
        return 3
    # Multi pion
    elif (reactionCode > 16) and (reactionCode < 30):
        return 4
    # NC neutrino
    elif (reactionCode >= 31) and (reactionCode < 100):
        return 5
    # anti-neutrino any
    elif (-1 >= reactionCode) and (reactionCode > -100):
        return 6
    else:
        print "Bad value: " + str(reactionCode)


def LoadP0DBANFF():
    """Load in the necessary classes"""
    if len(getenv('P0DBANFFROOT')) <= 0:
        print 'P0DBANFFROOT NOT exported. Please export it now'
        sys.exit(1)
    loadStatus = gSystem.Load("libP0DBANFF")
    if not (loadStatus == 0 or loadStatus == 1):
        print "Unable to load libP0DBANFF.so. gSystem.Load(\"libP0DBANFF\") returned", loadStatus
        sys.exit(1)
    global INTERFACE
    try:
        INTERFACE = ROOT.P0DBANFFInterface()
        INTERFACE.SetBatch(True)
        INTERFACE.LoadColorBlindPalette()
        ROOT.gStyle.SetOptStat(0000)
    except Exception as exc:
        print type(exc)
        print "unable to load libP0DBANFF.so"
        sys.exit(1)
    # Turn off the warnings about the possible memory leaks caused by using the same
    # name when projecting histograms.
    gROOT.ProcessLine("gErrorIgnoreLevel = kError")
    gROOT.SetBatch(kTRUE)


def main(argv):

    P0DBANFFStyle = INTERFACE.GetThisStyle()
    INTERFACE.SetStyle(P0DBANFFStyle.GetName())
    OUTPUTFILENAME = "BANFFReactionCodeStacksOfficial_Joint2018_Numode_Prefit_P0DOnly_Momentum.pdf"
    inputFile = TFile(sys.argv[0])
    dataFile = TFile(sys.argv[1])
    dummyPage = TCanvas()
    dummyPage.Print(OUTPUTFILENAME + "[")
    sampleNames = [
        "P0D_Air_NuMu_CC_CC1Track", "P0D_Air_NuMu_CC_CCNTracks",
        "P0D_Water_NuMu_CC1Track", "P0D_Water_NuMu_CCNTracks"
    ]
    fileNameRoots = [
        "P0D_Air_NuMu_CC1Track", "P0D_Air_NuMu_CCNTracks",
        "P0D_Water_NuMu_CC1Track", "P0D_Water_NuMu_CCNTracks"
    ]

    stackHistos = [
        "#nu CCQE", "#nu CC 2p-2h", "#nu CC Res 1#pi", "#nu CC Coh 1#pi",
        "#nu CC Other", "#nu NC modes", "#bar{#nu} modes"
    ]

    P0DBANFFStyle.SetTitleBorderSize(0)
    P0DBANFFStyle.SetTitleX(0.02)
    P0DBANFFStyle.SetTitleY(0.95)
    P0DBANFFStyle.SetTitleW(0.3)
    P0DBANFFStyle.SetTitleH(0.07)

    # Align left, centered in y)
    P0DBANFFStyle.SetTitleAlign(11)

    stackColours = [1300, 1302, 1303, 1304, 1305, 1308, 1310]
    stackFillStyle = [1001, 1001, 1001, 1001, 1001, 1001, 1001]

    """
    The graphs will be of the form <sampleName>_rxnPredMC_<rxnCode+100>
    So for each sample, we'll loop from 0-200, and if the THnD is not NULL when we
    try and get it, add it to a list for stacking.
    for sampleName in sampleNames:
    """
    for ii in xrange(0, len(sampleNames)):
        sampleName = sampleNames[ii]

        # Total the number of events for this sample going into the histogram.
        # (Remember: Sand is excluded here.)
        mcSampleTotal = 0.0

        # Now make the set of histograms to show, which combine several interaction
        # types.
        # Load in the data histogram and use it to assemble the TH2Ds for the MC, as
        # well as making it one of the histograms on the stack.
        dataHist = dataFile.Get(sampleName + "_data").Projection(1, 0)

        dataMomProj = dataFile.Get(sampleName + "_data").Projection(
            1, 0).ProjectionX("dataratio" + sampleName + "_px", 1,
                              dataHist.GetNbinsX())
        dataMomProj.SetDirectory(0)
        dataMomProj.SetTitle("")

        prefitMomProj = inputFile.Get(sampleName + "_prefit").Projection(
            1, 0).ProjectionX("prefitratio" + sampleName + "_px", 1,
                              dataHist.GetNbinsX())
        prefitMomProj.SetDirectory(0)

        postfitMomProj = inputFile.Get(sampleName + "_postfit_0_0").Projection(
            1, 0).ProjectionX("postfitratio" + sampleName + "_px", 1,
                              dataHist.GetNbinsX())
        postfitMomProj.SetDirectory(0)

        for ibx in xrange(1, dataMomProj.GetNbinsX() + 1):
            dataMomProj.SetBinContent(
                ibx,
                dataMomProj.GetBinContent(ibx) / prefitMomProj.GetBinContent(ibx))
            dataMomProj.SetBinError(
                ibx,
                ROOT.Math.sqrt(dataMomProj.GetBinContent(ibx)) /
                prefitMomProj.GetBinContent(ibx))
            print "Data = " + str(
                dataMomProj.GetBinContent(ibx)) + "  MC = " + str(
                    prefitMomProj.GetBinContent(ibx)) + " Ratio = " + str(
                        dataMomProj.GetBinContent(ibx) /
                        prefitMomProj.GetBinContent(ibx))

        dataMomProj.GetYaxis().SetRangeUser(0.75, 1.25)
        dataMomProj.GetXaxis().SetRangeUser(MINMOMENTUM, MAXMOMENTUM)

        dataMomProj.GetXaxis().SetTitle("Reconstructed muon momentum (MeV/c)")
        dataMomProj.GetYaxis().SetTitle("Data / Sim.")

        # There is only one data plot for each stack, so:
        # Momentum projection
        # Assemble them here.
        dataOneDimPlots = []
        dataOneDimPlots.append(
            dataHist.ProjectionX("data" + sampleName + "_px", 1,
                                 dataHist.GetNbinsY()))
        dataNXBins = dataHist.GetXaxis().GetNbins()
        dataxarray = dataHist.GetXaxis().GetXbins().GetArray()

        # Now, loop through all the theta bins and add the slices in.
        for iby in xrange(1, dataHist.GetNbinsY() + 1):

            thetaLow = dataHist.GetYaxis().GetBinLowEdge(iby)
            thetaHigh = dataHist.GetYaxis().GetBinUpEdge(iby)

            thetaString = " < cos# theta < "
            thetaString = str(thetaLow) + thetaString + str(thetaHigh)
            dataOneDimPlots.append(
                TH1D("dataslice_" + sampleName + str(iby), thetaString, dataNXBins,
                     dataxarray))

            # Now, go through the x bins in this slice and fill the plot we
            # just created.
            for ibx in xrange(1, dataHist.GetNbinsX() + 1):

                # Get the bin dimensions for dividing purposes to area
                # normalize.
                dataOneDimPlots[len(dataOneDimPlots) - 1].SetBinContent(
                    ibx, dataHist.GetBinContent(ibx, iby))

        # Now that the dataOneDimPlots array is filled, loop through a area
        # normalize the bins.
        for dataOneDimHist in dataOneDimPlots:
            for ibx in xrange(1, dataOneDimHist.GetNbinsX() + 1):
                mpdim = (dataOneDimHist.GetXaxis().GetBinUpEdge(ibx) -
                         dataOneDimHist.GetXaxis().GetBinLowEdge(ibx)) / 100.0
                dataOneDimHist.SetBinError(
                    ibx,
                    math.sqrt(dataOneDimHist.GetBinContent(ibx)) / mpdim)
                dataOneDimHist.SetBinContent(
                    ibx,
                    dataOneDimHist.GetBinContent(ibx) / mpdim)

        histosToStack = []
        for histo in stackHistos:

            histosToStack.append(
                TH2D(sampleName + "_" + histo, sampleName + "_" + histo,
                     dataHist.GetXaxis().GetNbins(),
                     dataHist.GetXaxis().GetXbins().GetArray(),
                     dataHist.GetYaxis().GetNbins(),
                     dataHist.GetYaxis().GetXbins().GetArray()))

        # Now pull the histograms from the file, and add their content to the
        # relevant combined histogram.
        for i in xrange(0, 200):

            if inputFile.Get(sampleName + "_rxnPredMC_" + str(i)):

                histosToStack[CorrespondingIndex(i)].Add(
                    inputFile.Get(sampleName + "_rxnPredMC_" + str(i)).Projection(
                        1, 0))

        """
        # OK, so now we have an array of 2D histograms for this sample that we'd
        # like to stack.  Need to divide it up into individual ones.  Do Momentum
        # Projection, then momentum in theta slices.  Normalize by bin area.
        # For storing all the one dimensional plots for this sample.
        """
        oneDimPlots = []

        # Add nothing for the momentum projection, will append the thetaStrings as
        # needed.
        stackTitles = []
        for i in xrange(0, len(stackHistos)):

            # For storing all the one dimensional plots for this reaction code.
            oneDimPlotsThisReac = []
            nameBase = histosToStack[i].GetTitle()
            oneDimPlotsThisReac.append(histosToStack[i].ProjectionX(
                nameBase + "_px", 1, histosToStack[i].GetNbinsY()))

            stackTitles.append("Momentum projection")
            # Area normalize this now.
            NXBins = histosToStack[i].GetXaxis().GetNbins()

            for ibx in xrange(1, NXBins + 1):
                mpdim = (histosToStack[i].GetXaxis().GetBinUpEdge(ibx) -
                         histosToStack[i].GetXaxis().GetBinLowEdge(ibx)) / 100.0
                oneDimPlotsThisReac[0].SetBinContent(
                    ibx, oneDimPlotsThisReac[0].GetBinContent(ibx) / mpdim)

            mcSampleTotal += oneDimPlotsThisReac[0].GetSumOfWeights()
            # Now that the x bins are filled, loop on to the next histogram.

            # With all the y-slices done, oneDimPlotsThisReac is now full.  Append
            # it to oneDimPlots.
            oneDimPlots.append(oneDimPlotsThisReac)

        """"
        At this point in time, oneDimPlots has a momentum projection and theta
        slices for each reaction code grouping in this sample. So now we just need
        to make TStacks for them and print them out to PDF.
        The stack contains the plot from the same index of each entry of
        oneDimPlots.
        They should all have the same length, so use the first one to loop
        through (number of slices plus 1 full momenutm projection.)
        """
        for odi in xrange(0, len(oneDimPlots[0])):

            # For each one create a TStack
            sampleStack = THStack(
                "mpStack_" + sampleName + str(odi),
                ";Reconstructed muon momentum (MeV/c);Events/(100 MeV/c)")

            leg = TLegend(0.7, 0.15, 0.94, 0.93)
            leg.SetFillColor(0)

            # Make data the first entry.
            leg.AddEntry(dataOneDimPlots[odi], "Data", "LEP")
            dataOneDimPlots[odi].SetMarkerStyle(20)
            dataOneDimPlots[odi].SetMarkerColor(kBlack)
            dataOneDimPlots[odi].SetLineColor(kBlack)

            for odj in xrange(0, len(oneDimPlots)):

                # Apply the appropriate plot options from the beginning.
                oneDimPlots[odj][odi].SetFillColor(stackColours[odj])
                oneDimPlots[odj][odi].SetFillStyle(stackFillStyle[odj])
                oneDimPlots[odj][odi].SetMarkerStyle(1)  # Don't want P0DBANFFStyle to interfere.
                sampleStack.Add(oneDimPlots[odj][odi])
                print oneDimPlots[odj][odi].GetTitle() + " " + str(
                    oneDimPlots[odj][odi].GetSumOfWeights())

                leg.AddEntry(oneDimPlots[odj][odi], stackHistos[odj], "F")

            c1 = TCanvas()

            upperPad = TPad("upperPad", "upperPad", .05, .35, .96, .95)
            lowerPad = TPad("lowerPad", "lowerPad", .05, .1, .96, .35)

            upperPad.Draw()
            lowerPad.Draw()

            upperPad.cd()

            gPad.SetBottomMargin(1e-5)

            c1.SetTicks(1, 1)
            sampleStack.Draw("")

            dataOneDimPlots[odi].Draw("PEsame")
            sampleStack.SetMaximum(
                max(sampleStack.GetMaximum(),
                    FindPlotMax(dataOneDimPlots[odi], dataOneDimPlots[odi])))
            leg.Draw()
            sampleStack.GetXaxis().SetRangeUser(MINMOMENTUM, MAXMOMENTUM)
            sampleStack.GetXaxis().SetTitle("")
            sampleStack.GetXaxis().SetTickLength(0)
            sampleStack.GetXaxis().SetLabelSize(0)
            sampleStack.SetTitle("# nu-mode")

            lowerPad.cd()

            ROOT.gPad.SetTopMargin(1e-5)
            ROOT.gPad.SetBottomMargin(0.35)
            ROOT.gPad.SetTickx()
            dataMomProj.SetLineColor(2)
            dataMomProj.SetMarkerColor(2)
            dataMomProj.GetYaxis().SetNdivisions(8, 1)
            dataMomProj.GetYaxis().SetLabelSize(
                dataMomProj.GetYaxis().GetLabelSize() + 0.07)
            dataMomProj.GetYaxis().SetTitleSize(
                dataMomProj.GetYaxis().GetLabelSize() + 0.05)
            dataMomProj.GetYaxis().SetTitleOffset(0.335)
            dataMomProj.GetXaxis().SetLabelSize(
                dataMomProj.GetXaxis().GetLabelSize() + 0.08)
            dataMomProj.GetXaxis().SetTitleSize(
                dataMomProj.GetXaxis().GetLabelSize() + 0.04)
            dataMomProj.GetXaxis().SetTitleOffset(0.9)
            dataMomProj.GetXaxis().SetTickLength(0.04)
            dataMomProj.Draw("PE")
            line = TLine(0.0, 1.0, 5000.0, 1.0)
            line.SetLineWidth(2)
            line.SetLineColor(1)
            line.Draw("SAME")

            c1.Modified()
            c1.Update()
            c1.Print(OUTPUTFILENAME)
            outFileName = fileNameRoots[sampleNames.index(
                sampleName)] + "_mumom_rxn_postfit"
            c1.Print(outFileName + ".pdf")
            c1.Print(outFileName + ".png")
            c1.Print(outFileName + ".eps")

    dummyPage.Print(OUTPUTFILENAME + ']')


if __name__ == "__main__":
    main(sys.argv[1:])
    if INTERFACE:
        del INTERFACE
