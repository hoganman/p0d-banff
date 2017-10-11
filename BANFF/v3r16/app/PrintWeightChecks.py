#!/usr/bin/python
from ROOT import *
import sys

#Turn off the warnings about the possible memory leaks caused by using the same
#name when projecting the histograms.
gROOT.ProcessLine("gErrorIgnoreLevel = kError")

#Usage: python PrintWeightChecks.py output_of_Check2015XSecWeightsIndividually
inputFile = TFile(sys.argv[1])

sampleNames = ["cc0pi","cc1pi","ccOth","anuccqe","anuccnqe","nuccqe","nuccnqe"]
weightConfig =["no XSec","RFG","RFG + RPA","RFG + RPA + norms", 
        "RFG + RPA + norms + MAQE", "RFG + RPA + norms + MAQE + pF_C", "All"]
weightConfigNames = ["noXSec","RFG","RFGwRPA","RFGwRPAwNorms","RFGwRPAwNormswMAQE","RFGwRPAwNormswMAQEwpFC","all"]

#Loop through the different weight configurations.
for i in xrange(0, len(weightConfig)):

    #Print out this weight configuration.
    print weightConfig[i] +":\n"

    #Loop through the samples for each weight configuration.
    for j in xrange(0,len(sampleNames)):

        #Get the content of the histogram in the file.
        content = inputFile.Get(sampleNames[j]+"_predMC_"+weightConfigNames[i]).Projection(1,0).GetSumOfWeights()

        print sampleNames[j] + ": " + str(content)

    #Print a newline to separate the different types.
    print "\n"
