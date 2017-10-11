#!/usr/bin/python
from ROOT import *


inFile = TFile("obsNorm_2015_v2.root")
#outFile = TFile("obsNorm_2015_v2_NuMode.root", "RECREATE")
outFile = TFile("obsNorm_2015_v2_ANuMode.root", "RECREATE")

#The list of samples to include.
#sampleList = ["cc0pi","cc1pi","ccOth"]
sampleList = ["anuccqe","anuccnqe","nuccqe","nuccnqe"]

#The list of observables to include.
observableList = ["p","th"]

#The index to start copying at.
#startingIndex = 0
startingIndex = 210

#The last index to copy over.
#lastIndex = 209
lastIndex = 289

#Pull out the covariance and parameter values.
inCov = inFile.Get("obsNorm_cov")
inParams = inFile.Get("obsNorm_weights")

totalParams = (lastIndex - startingIndex) + 1 

outCov = TMatrixDSym(totalParams)
outParams = TVectorD(totalParams)

for i in xrange(startingIndex, lastIndex + 1):

    outParams[i-startingIndex] = inParams[i]

    for j in xrange(startingIndex, lastIndex + 1):
        outCov[i-startingIndex][j-startingIndex] = inCov[i][j]


#Write out the results.
outFile.cd()
outCov.Write("obsNorm_cov")
outParams.Write("obsNorm_weights")


for i in xrange(0, len(sampleList)):

    for j in xrange(0, len(observableList)):

        axisName = sampleList[i] + "_" + observableList[j] + "_axis"
        tmpAxis = inFile.Get(axisName) 

        outFile.cd()
        tmpAxis.Write(axisName)

outFile.Close()

