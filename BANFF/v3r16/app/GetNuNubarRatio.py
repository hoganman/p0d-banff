#!/usr/bin/python
from ROOT import *
import sys

toyFile = TFile(sys.argv[1])

#For CC-Inclusive
#nuSamples = ["cc0pi","cc1pi","ccOth"]
#anuSamples = ["anuccqe","anuccnqe"]

#For cc0pi.
nuSamples = ["cc0pi"]
anuSamples = ["anuccqe"]

ratios = []
ratioPlot = TH1D("ratio",";#bar{#nu}/#nu ratio;Number of toys",30,0.2,0.5)
for i in xrange(0,2000):

    #Compute the total event rate for the neutrino samples considering.
    nuSamplesRate = 0
    for sample in nuSamples:

        plot = toyFile.Get(sample+"_predMC_"+str(i))
        nuSamplesRate += plot.Projection(1,0).GetSumOfWeights()

    #Now do the same for the anti-neutrino samples.
    anuSamplesRate = 0
    for sample in anuSamples:

        plot = toyFile.Get(sample+"_predMC_"+str(i))
        anuSamplesRate += plot.Projection(1,0).GetSumOfWeights()


    #Now, compute a factor to normalize by POT. 
    #These are the MC POT numbers.  In the plots the MC has been normalized to
    #data POT, so the POT normalization should use data POT, not MC.
    #nuPOT = 9.215e+20 + 1.2015e+21 + 4.48e+20 + 2.63e+21 + 3.499e+21 + 3.4965e+21
    #nuBarPOT = 2.083e+21
    nuPOT = 3.55095e+19 + 4.2858e+19 + 2.146e+19 + 1.34821e+20 + 1.84438e+20 + 1.62818e+20
    nuBarPOT = 4.29779e+19

    print "PF POT: " + str(nuPOT)
    print "NF POT: " + str(nuBarPOT)

    #POTNORMNU/POTNORMANU = (nu/nuPOT)/(anu/anuPOT) = (nu/anu)*(anuPOT/nuPOT)
    #So define ratioPOT = (anuPOT/nuPOT)
    ratioPOT = nuBarPOT/nuPOT

    #Now store (nu/anu)*ratioPOT.
    ratios.append(1/((nuSamplesRate/anuSamplesRate)*ratioPOT))

    ratioPlot.Fill(1/((nuSamplesRate/anuSamplesRate)*ratioPOT))

#Now that we have all the ratios, compute the average and the standard
#deviation.

avgRatio = 0
for i in xrange(0, 2000):

    avgRatio += ratios[i]

#With the ratios summed, divide by the number of toys (2000)
avgRatio = avgRatio/2000.0

#Now, compute the RMS.
ratioRMS = 0
for i in xrange(0,2000):
    ratioRMS += pow(ratios[i] - avgRatio,2)

#Having summed the terms, divide by the number of toys and take the square
#root.
ratioRMS = sqrt(ratioRMS/2000.0)

#Print the result.
print "The ratio is " + str(avgRatio) +  " +/- " + str(ratioRMS)

c1 = TCanvas()
c1.cd()
ratioPlot.Draw()
c1.Print("ratioPlot.pdf")
