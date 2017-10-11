#!/usr/bin/python

#Reads in a toy throw file with nominal MC, and pulls out the nominal MC and
#renames it to predMC_0 in order to make a file in the fake data format.
from ROOT import *
import sys

inFile = TFile(sys.argv[1])
outFile = TFile(sys.argv[2],"RECREATE")

#Get out the list of sample names.
sampleNames = inFile.Get("sampleNames")

sampleNameList = []

for i in xrange(0,sampleNames.GetEntries()):

    sampleNameList.append(sampleNames.At(i).GetString().Data())

for sName in sampleNameList:

    sampleHist = inFile.Get(sName +"_nomMC")

    outFile.cd()
    sampleHist.Write(sName + "_predMC_0")

#With all the histograms copied over, close it.
outFile.Close()
