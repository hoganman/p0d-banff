#!/usr/bin/python
from ROOT import *
import sys
import math

#Load in the preFit file and the toy file, and set up the output file.
preFitFile = TFile("preFitParameters.root")
#toyFile = TFile("full_100toys_20150125.root")
toyFile = TFile("full_300toys_20150202.root")
plotFile = TFile("checkToyPlots.root","RECREATE")

preFitParms = preFitFile.Get("FittedParameters0")
preFitCov = preFitFile.Get("FittedCovariance0")

goodFitHists = []
badFitHists = []
#badFitToys = [0, 10, 12, 13, 14, 16, 19, 1, 26, 32, 35, 3, 40, 54, 57, 58, 64, 75, 79, 81, 84, 87, 89, 91, 92, 98]
#failedFits = [46]

#badFitToys = [29,3,34,36,43,51,52,56,57,58,69,71,78,79,91,92]
badFitToys = []
failedFits = []

plotFile.cd()
for i in xrange(0,preFitParms.GetNrows()):
    
    priorVal = preFitParms(i)
    priorErr = math.sqrt(preFitCov(i,i))

    goodFitHists.append(TH1F("good"+str(i),"good"+str(i),120, priorVal -
        6*priorErr, priorVal + 6*priorErr))

    #badFitHists.append(TH1F("bad"+str(i),"bad"+str(i),120, priorVal -
    #    6*priorErr, priorVal + 6*priorErr))

#Now, for each toy in the toyFile, put its value in the appropriate histogram.
for i in xrange(0,300):

    if i in failedFits:
        continue

    flux_parms = toyFile.Get("flux_parms_" + str(i))
    xsec_parms = toyFile.Get("xsec_parms_" + str(i))
    obsNorm_parms = toyFile.Get("obsNorm_parms_" + str(i))

    if xsec_parms(18) < 0:
        print "Toy " + str(i) + " out of bounds"

    currentParam = 0
    for j in xrange(0,flux_parms.GetNrows()):

        if i in badFitToys:
            #badFitHists[currentParam].Fill(flux_parms(j))
            print "bad"
        else:
            goodFitHists[currentParam].Fill(flux_parms(j))

        currentParam += 1
    
    for j in xrange(0,xsec_parms.GetNrows()):

        if i in badFitToys:
            #badFitHists[currentParam].Fill(xsec_parms(j))
            print "bad"
        else:
            goodFitHists[currentParam].Fill(xsec_parms(j))

        currentParam += 1

    for j in xrange(0,obsNorm_parms.GetNrows()):

        if i in badFitToys:
            #badFitHists[currentParam].Fill(obsNorm_parms(j))
            print "bad"
        else:
            goodFitHists[currentParam].Fill(obsNorm_parms(j))

        currentParam += 1

#Save them to a PDF file.
firstPlot = True
for i in xrange(0,preFitParms.GetNrows()):

    c1 = TCanvas()
    c1.cd()
    goodFitHists[i].Draw()

    if firstPlot:
        c1.Print("checkToys.pdf(")
        firstPlot = False

    elif i != preFitParms.GetNrows() - 1:
        c1.Print("checkToys.pdf")

    else:
        c1.Print("checkToys.pdf)")

#    else:
#        c1.Print("checkToys.pdf")
#
#    c1.Clear()
#    #badFitHists[i].Draw()
#
#    if i == preFitParms.GetNrows() - 1:
#        c1.Print("checkToys.pdf)")
#    else:
#        c1.Print("checkToys.pdf")





#With all toys looped over, write out the output file.
plotFile.Write()


