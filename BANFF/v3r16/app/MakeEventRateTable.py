#!/usr/bin/python
#Reads in a user supplied throw file with nominal MC for many different weight
#combinations, and outputs a .tex file that can be made into a table.
#Usage: MakeEventRateType
from ROOT import *
import sys

fileName = sys.argv[1]

inFile = TFile(fileName)

outFile = open(sys.argv[2],"w")

#Start by printing the needed header information to the output file.
outFile.write("\\documentclass{article}\n")
outFile.write("\\usepackage{default}\n")
outFile.write("\\usepackage[paperwidth=11in, paperheight=3in,margin=0.25in]{geometry}")
outFile.write("\\begin{document}\n")
outFile.write("\\begin{centering}\n")

#There are 9 columns in total.
outFile.write("\\begin{tabular}{ | c | c | c | c | c | c | c | c | c |}\n")
outFile.write("\\hline\n")
outFile.write("Sample & Raw MC & POT only & POT+flux & POT+xsec & POT+det & POT + flux + xsec & POT + flux + det & POT + flux + xsec + det \\\\ \\hline\n")

#Now grab the sample names from the sample name TObjString.
sampleNames =  inFile.Get("sampleNames")

sampleNameList = [];

for i in xrange(0,sampleNames.GetEntries()):

    sampleNameList.append(sampleNames.At(i).GetString().Data())

#Now for each sample, load in the corresponding list of THnD objects to fill
#the table.
histogramList = []

for sName in sampleNameList:

    thisHistList = []

    thisHistList.append(inFile.Get(sName + "_nomMC_withNoWeights"))
    thisHistList.append(inFile.Get(sName + "_nomMC_withPOTWeights"))
    thisHistList.append(inFile.Get(sName + "_nomMC_withNomFluxWeights"))
    thisHistList.append(inFile.Get(sName + "_nomMC_withNomXSecWeights"))
    thisHistList.append(inFile.Get(sName + "_nomMC_withNomDetWeights"))
    thisHistList.append(inFile.Get(sName + "_nomMC_withNomFluxAndXSecWeights"))
    thisHistList.append(inFile.Get(sName + "_nomMC_withNomFluxAndDetWeights"))
    thisHistList.append(inFile.Get(sName + "_nomMC"))

    #With all histograms added, append to the histogamList (of lists)
    histogramList.append(thisHistList)


#Now loop through the samples and histograms and put the correct number in each
#table entry.
for i in xrange(0,len(sampleNameList)):
    
    lineContent = sampleNameList[i]

    #Now loop through the histograms and add their values in too.
    for hist in histogramList[i]:

        val = hist.Projection(1,0).GetSumOfWeights()
        
        #Make it have 2 decimal places.
        stringVal = "{0:.2f}".format(round(val,2))
        
        lineContent += " &  " + stringVal

    #Having reached the end of the line, terminate it.
    lineContent += "\\\\ \\hline\n"

    #Now the line is complete.  Write it out to the output file.
    outFile.write(lineContent)

#With all lines of the table written, terminate the document and close the
#output file.
outFile.write("\\end{tabular}\n")
outFile.write("\\end{centering}\n")
outFile.write("\\end{document}\n")
outFile.close()


