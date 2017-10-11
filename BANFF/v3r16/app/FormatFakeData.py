#!/usr/bin/python
import glob
from ROOT import *

#We're do all the data here.  Get a glob for each different file type:
fakeDataTypes = ["Combined_nonRelRPA","Combined_SF","q3cut_nonRelRPA","q3cut_RelRPA","GENIE","Combined_RelRPA"]
globList = []
histoNames = ["tuned_cc0pi","tuned_cc1pi","tuned_ccNpi","tuned_ccAnu0pi","tuned_ccAnuNpi","tuned_ccNu0pi","tuned_ccNuNpi"]
outHistoNames = ["cc0pi_predMC_0","cc1pi_predMC_0","ccOth_predMC_0","anuccqe_predMC_0","anuccnqe_predMC_0","nuccqe_predMC_0","nuccnqe_predMC_0"]


for fdt in fakeDataTypes:
    globList.append(glob.glob("*" + fdt + "*_FakeData*"))

for fdi in xrange(0,len(globList)):
    
    print "=================================="
    #Create a list to hold the histograms for this fake data set.
    histoList = []
    for i in xrange(0,len(globList[fdi])):

        print "Now adding file " + globList[fdi][i]


        currFile = TFile(globList[fdi][i])

        #If it's the first file, just grab the histogram and add it to the
        #list.
        if i == 0:

            #Only want to definitely grab the first 3 from whatever random file
            #happens to be first, because run5 has a different binning in its
            #histos than it does in the other files.
            for j in xrange(0,3):
                thisHist = currFile.Get(histoNames[j])
                thisHist.SetDirectory(0)
                histoList.append(thisHist)
                print histoNames[j] + " added to list."
        #Otherwise, get the current histogram and add it to the one in the
        #histoList.
        elif "Run5" not in globList[fdi][i]:
            for j in xrange(0,3):

                histoList[j].Add(currFile.Get(histoNames[j]))


        #Now, if this is the Run5 file, just append the histograms, and that's
        #the end of it.
        if "Run5" in globList[fdi][i]:

            for j in xrange(3,len(histoNames)):
                thisHist = currFile.Get(histoNames[j])
                thisHist.SetDirectory(0)
                histoList.append(thisHist)
                print histoNames[j] + " added to list."

    #Once we get to here we have finished looping over the histograms.
    #Create the output file.
    f = TFile("FakeData_" + fakeDataTypes[fdi] + ".root","RECREATE")
    f.cd()
    print "----------------------------"
    print "Event rates:"
    print "----------------------------"
    for i in xrange(0,len(histoList)):
        
        print outHistoNames[i] + ": " + str(histoList[i].GetSumOfWeights())
        histoList[i].Write(outHistoNames[i])

    #With all the histos saved, close the file.
    f.Close()


