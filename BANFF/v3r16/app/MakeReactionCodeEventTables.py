#!/usr/bin/python
from ROOT import *
import sys
import math

#Usage is
#python MakeReactionCodeEventTables.py fitOutputFile outputFile.tex
prefitFile = TFile(sys.argv[1])
postfitFile = TFile(sys.argv[2])
outFile = open(sys.argv[3],'w')

#These will be column headings, and must match the naming in the fit output.
sampleNames = ["cc0pi","cc1pi","ccOth","anuccqe","anuccnqe","nuccqe","nuccnqe"]


#These will be row headings.
rxnCodeGroups = ["CCQE","MEC", "CCRES", "CCCOH", "CCOTHER", "NCRES", "NCCOH", "NCOTHER"
        ,"ANUCCQE", "ANUMEC", "ANUCCRES", "ANUCCCOH", "ANUCCOTHER", "ANUNCRES"
        , "ANUNCCOH", "ANUNCOTHER"]


#Takes an index, converts to a reaction code, and returns the corresponding
#index in the stackHistos (and hence histosToStack) list to add this
#histogram's content to.
def CorrespondingIndex(i):

    #First subtract 100 to get a reaction code.
    i = i -100

    if i == 1:
        return 0
    
    elif i == 2:
        return 1

    elif (11 <= i) and (i <= 13):
        return 2

    elif i == 16:
        return 3

    elif (i > 16) and (i < 30):
        return 4

    elif (31 <= i) and (i <= 34):
        return 5

    elif i == 36:
        return 6

    elif (i > 36) and (i < 100):
        return 7

    elif i == -1:
        return 8

    elif i == -2:
        return 9

    elif (-13 <= i) and (i <= -11):
        return 10

    elif i == -16:
        return 11

    elif (i < -16) and (i > -30):
        return 12

    elif (-31 >= i) and (i >= -34):
        return 13

    elif i == -36:
        return 14

    elif (i < -36) and (i > -100):
        return 15

    else:
        print "Bad value: " + str(i)


#Make some 2D lists to store everything in.
prefitEvents = []
postfitEvents = []

#Loop through all the samples, prefit and postfit, assembling the event count
#for each of the reaction code groupings.
for sampleName in sampleNames:

    prefitEventsThisSample = []
    postfitEventsThisSample = []

    #Initialize the counters for each group of reaction codes to 0 for this
    #sample.
    for i in xrange(0, len(rxnCodeGroups)):
        prefitEventsThisSample.append(0.0)
        postfitEventsThisSample.append(0.0)


    #Now that we have something to add to, loop from 0 to 200, and if the
    #histogram exists, add its content to the correct sample group total.
    for i in xrange(0,200):
        if prefitFile.Get(sampleName + "_rxnPredMC_" + str(i)):
                prefitEventsThisSample[CorrespondingIndex(i)] += prefitFile.Get(sampleName + "_rxnPredMC_" + str(i)).Projection(1,0).GetSumOfWeights()

        if postfitFile.Get(sampleName + "_rxnPredMC_" + str(i)):
                postfitEventsThisSample[CorrespondingIndex(i)] += postfitFile.Get(sampleName + "_rxnPredMC_" + str(i)).Projection(1,0).GetSumOfWeights()

    #With everything added, sum them up to create a "total" entry right at the
    #end.
    prefitTotal = 0.0
    postfitTotal = 0.0
    for i in xrange(0,len(prefitEventsThisSample)):
        prefitTotal += prefitEventsThisSample[i]
        postfitTotal += postfitEventsThisSample[i]

    prefitEventsThisSample.append(prefitTotal)
    postfitEventsThisSample.append(postfitTotal)

    #At this point all groupings for this sample should be filled.  Append it
    #to the list for all samples.
    prefitEvents.append(prefitEventsThisSample)
    postfitEvents.append(postfitEventsThisSample)

#At this point we have everything.  So, add "Total" to the reaction code groups
#list.
rxnCodeGroups.append("Total")

#Start by printing the needed header information to the output file.
outFile.write("\\documentclass{article}\n")
outFile.write("\\usepackage{default}\n")
outFile.write("\\usepackage[paperwidth=11in, paperheight=3in,margin=0.25in]{geometry}")
outFile.write("\\begin{document}\n")
outFile.write("\\begin{centering}\n")

#There are 9 columns in total.
outFile.write("\\begin{tabular}{ | c || c | c || c | c || c | c || c | c || c | c || c | c || c | c |}\n")
outFile.write("\\hline\n")
outFile.write("Reaction Code Group & cc0pi prefit & cc0pi postfit & cc1pi prefit & cc1pi postfit & ccOth prefit & ccOth postfit & anuccqe prefit & anuccqe postfit & anuccnqe prefit & anuccnqe postfit & nuccqe prefit & nuccqe postfit & nuccnqe prefit & nuccnqe postfit \\\\ \\hline\n")

for i in xrange(0, len(rxnCodeGroups)):

    lineContent = rxnCodeGroups[i]

    for j in xrange(0, len(sampleNames)):

        prefitEntry = "{0:.2f}".format(round(prefitEvents[j][i],2))
        postfitEntry = "{0:.2f}".format(round(postfitEvents[j][i],2))

        lineContent += " & " + prefitEntry + " & " + postfitEntry

    #Now we will have reached the end of the line, so terminate it.
    lineContent += "\\\\ \\hline\n"

    #Now write it out to the file.
    outFile.write(lineContent)

outFile.write("\\end{tabular}\n")
outFile.write("\\end{centering}\n")
outFile.write("\\end{document}\n")
outFile.close()

