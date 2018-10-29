#!/usr/bin/python
import sys
import math
from ROOT import *
from array import array

# Turn off the warnings about the possible memory leaks
#   caused by using the same name when projecting histograms
# Run in Batch mode
gROOT.ProcessLine('gErrorIgnoreLevel = kError')
gROOT.SetBatch(kTRUE)

if (len(sys.argv) != 4):
    print 'Usage:\n'
    print 'EnergyBreakdown_Plots.py mcfile.root datafile.root outfile.pdf'
    print '\tmcfile.root   ---> raw output from asimov fit'
    print '\tdatafile.root ---> raw output from data fit'
    print '\toutfile.pdf   ---> name of file where the plots written\n'
    exit()

mcfile    = TFile(sys.argv[1])
datafile  = TFile(sys.argv[2])
outfile   = str(sys.argv[3])
dummypage = TCanvas()
dummypage.Print(outfile + '[')

# Setup the array of sample names
# Only works for FHC samples, need to add in RHC samples
#   if desired in the future
samplenames = ['FGD1_numuCC_0pi', 'FGD1_numuCC_1pi', 'FGD1_numuCC_other',
               'FGD2_numuCC_0pi', 'FGD2_numuCC_1pi', 'FGD2_numuCC_other']

# Setup the array of reaction names
stackhistos = ['#nu CCQE', '#nu CC 2p2h', '#nu CC Res 1#pi', '#nu CC Coh 1#pi',
               '#nu CC Other', '#nu NC modes', '#bar{#nu} modes']

# Define some styling
gStyle.SetTitleBorderSize(0)

# Define colors for plotting
color1 = TColor(1001, 127./255., 201./255., 127./255., 'color1')
color2 = TColor(1002, 190./255., 174./255., 212./255., 'color2')
color3 = TColor(1003, 253./255., 192./255., 134./255., 'color3')
color4 = TColor(1004, 255./255., 255./255., 153./255., 'color4')
color5 = TColor(1005,  56./255., 108./255., 176./255., 'color5')
color6 = TColor(1006, 240./255.,   2./255., 127./255., 'color6')
color7 = TColor(1007, 191./255.,  91./255.,  23./255., 'color7')

# Using the 7 colour medium palette and setting fill style
#stackColors    = [kAzure-3, kAzure-2, kAzure-1, kAzure, kAzure+1, kAzure+2, kAzure+3]
stackColors    = [1001, 1002, 1003, 1004, 1005, 1006, 1007]
stackFillStyle = [1001, 1001, 1001, 1001, 1001, 1001, 1001]

# Takes an index, converts it to a reaction code, and returns the corresponding
#   index in the stackhistos list for the reaction code
def CorrespondingIndex(i):
    #First subtract 100 to get a reaction code.
    i = i -100

    # Turn reaction code into index for stackHistos
    if i == 1:                    # CCQE
        return 0
    elif i == 2:                  # 2p2h
        return 1
    elif (11 <= i) and (i <= 13): # 1pi resonance
        return 2
    elif i == 16:                 # 1pi coherent
        return 3
    elif (i > 16) and (i < 30):   # CC other
        return 4
    elif (i >= 31) and (i < 100): # NC modes
        return 5
    elif(-1 >= i) and (i > -100): # Antineutrino modes
        return 6
    else:
        print 'Bad value: ' + str(i)

# Takes a list of histograms and adds them together
def CombineHistograms(histlist):
    if (len(histlist) == 1):
        return histlist[0]
    else:
        tmphist = histlist[0]
        for index in range(1, len(histlist)-1):
            tmphist.Add(histlist[index])
        return tmphist

# The graphs will be of the form <sampleName>_rxnPredMCTrueE_<rxnCode+100>
# So for each sample, we'll loop from 0-200, and if the THnD is not NULL when we
#   try and get it, add it to a list for stacking.
for ii in xrange(0, len(samplenames)):
    samplename = samplenames[ii]

    ccqehists = []
    mechists  = []
    reshists  = []
    cohhists  = []
    othrhists = []
    nchists   = []
    antihists = []
    for i in xrange(0, 200):
        plotname = samplename + '_rxnPredMCTrueE_' + str(i)
        if mcfile.Get(plotname):
            sorthists = CorrespondingIndex(i)
            hist = mcfile.Get(plotname).Projection(0)
            if (sorthists == 0):
                ccqehists.append(hist)
            elif (sorthists == 1):
                mechists.append(hist)
            elif (sorthists == 2):
                reshists.append(hist)
            elif (sorthists == 3):
                cohhists.append(hist)
            elif (sorthists == 4):
                othrhists.append(hist)
            elif (sorthists == 5):
                nchists.append(hist)
            elif (sorthists == 6):
                antihists.append(hist)
            else:
                print 'Not a valid index'

    histstostack = [CombineHistograms(ccqehists), CombineHistograms(mechists),
                    CombineHistograms(reshists), CombineHistograms(cohhists),
                    CombineHistograms(othrhists), CombineHistograms(nchists),
                    CombineHistograms(antihists)]


    # For storing all the one dimensional plots for this sample.
    oneDimPlots = []
   
    for i in xrange(0, len(stackhistos)):

        #For storing all the one dimensional plots for this reaction code.
        oneDimPlotsThisReac = []
 
        oneDimPlotsThisReac.append(histstostack[i])
       
        # oneDimPlotsThisReac is now full
        # Append it to oneDimPlots
        oneDimPlots.append(oneDimPlotsThisReac)

    # Plot reactions stacked on top of one another
    for odi in xrange(0, len(oneDimPlots[0])):
       
        # For each one create a TStack
        sampleStack =  THStack('mpStack_' + samplename + str(odi), ';True Neutrino Energy (GeV);Events/(100 MeV)')

        leg = TLegend(0.7,0.15,0.94,0.93)
        leg.SetFillColor(0)

        for odj in xrange(0, len(oneDimPlots)):
            # Apply the appropriate plot options from the beginning.
            oneDimPlots[odj][odi].SetFillColor(stackColors[odj])
            oneDimPlots[odj][odi].SetFillStyle(stackFillStyle[odj])
            oneDimPlots[odj][odi].SetMarkerStyle(1) #Don't want T2KStyle to interfere.
            sampleStack.Add(oneDimPlots[odj][odi])
            #print oneDimPlots[odj][odi].GetTitle() + ' ' + str(oneDimPlots[odj][odi].GetSumOfWeights())

            leg.AddEntry(oneDimPlots[odj][odi],stackhistos[odj],'F')

        c1 = TCanvas()

        c1.SetTicks(1,1)

        sampleStack.Draw('')

        leg.Draw()

        if (samplename[-3:] == '0pi') or (samplename[-3:] == '1pi'):
            sampleStack.GetXaxis().SetLimits(0, 3)
        else:
            sampleStack.GetXaxis().SetLimits(0,10)
        sampleStack.SetTitle(samplename)

        c1.Modified()
        c1.Update()
        c1.Print(outfile)

        #outFileName = sampleName + '_energy_rxn_postfit'
        #c1.Print(outFileName + '.pdf')
        #c1.Print(outFileName + '.png')
        #c1.Print(outFileName + '.eps')

    # Plot individual reactions separately
    for odi in xrange(0, len(histstostack)):
       
        # For each one create a TStack
        sampleStack =  THStack('mpStack_' + samplename + str(odi), ';True Neutrino Energy (GeV);Events/(100 MeV)')

        leg = TLegend(0.7,0.15,0.94,0.93)
        leg.SetFillColor(0)

        # Apply the appropriate plot options from the beginning.
        histstostack[odi].SetFillColor(stackColors[odi])
        histstostack[odi].SetFillStyle(stackFillStyle[odi])
        histstostack[odi].SetMarkerStyle(1) #Don't want T2KStyle to interfere.
        sampleStack.Add(histstostack[odi])
        #print histstostack[odi].GetTitle() + ' ' + str(histstostack[odi].GetSumOfWeights())

        leg.AddEntry(histstostack[odi], stackhistos[odi], 'F')

        c1 = TCanvas()

        c1.SetTicks(1,1)
        sampleStack.Draw('')

        leg.Draw()

        if (samplename[-3:] == '0pi') or (samplename[-3:] == '1pi'):
            sampleStack.GetXaxis().SetLimits(0, 3)
        else:
            sampleStack.GetXaxis().SetLimits(0,10)
        sampleStack.SetTitle(samplename)

        c1.Modified()
        c1.Update()
        c1.Print(outfile)

dummypage.Print(outfile + ']')

