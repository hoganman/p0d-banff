#!/usr/bin/python
from ROOT import *
import glob

#Grab the files in a glob.
files = glob.glob("*BANFFInput.root")
inputHistoNames = ["cc0pi_predMC_0","cc1pi_predMC_0","ccOth_predMC_0","anuccqe_predMC_0","anuccnqe_predMC_0","nuccqe_predMC_0","nuccnqe_predMC_0"]
outputHistoRoot =  ["cc0pi_predMC_","cc1pi_predMC_","ccOth_predMC_","anuccqe_predMC_","anuccnqe_predMC_","nuccqe_predMC_","nuccnqe_predMC_"]

outputFile = TFile("FakeDataAsToys.root","RECREATE")

for i in xrange(0,len(files)):

    f = TFile(files[i])

    for j in xrange(0,len(inputHistoNames)):

        histo = f.Get(inputHistoNames[j])

        outputFile.cd()
        histo.Write(outputHistoRoot[j]+str(i))


    #With this file done, print out the file name so we know the order of the
    #toys.
    print "File " + str(i) + ": " + files[i]

#With all histograms read in and written out, close the output file.
outputFile.Close()
