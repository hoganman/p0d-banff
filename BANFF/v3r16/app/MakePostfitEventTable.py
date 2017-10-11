#!/usr/bin/python
from ROOT import *
import sys
import os

#This programs is just called with:
#python MakePostfitEventTable.py inputFileName outputFileName
#NB: outputFileName should be a .tex.  A pdf with the table will automatically
#be made.
#And it produces a table with a row for each sample, and columns for Data,
#Prefit, and Postfit event counts in each sample.

inFile = TFile(sys.argv[1])

outFile = open(sys.argv[2],"w")

#Start by printing the needed header information to the output file.
outFile.write("\\documentclass{article}\n")
outFile.write("\\usepackage{default}\n")
outFile.write("\\usepackage[paperwidth=3.5in, paperheight=2in,margin=0.25in]{geometry}")
outFile.write("\\begin{document}\n")
outFile.write("\\begin{centering}\n")

#There are 4 columns in total.
outFile.write("\\begin{tabular}{ | c | c | c | c | }\n")
outFile.write("\\hline\n")
outFile.write("Sample & Data & Prefit & Postfit  \\\\ \\hline\n")

#Now grab the sample names from the sample name TObjString.
#sampleNames =  inFile.Get("sampleNames")

#Unfortunately the fit does not currently save a list of sample names...
#perhaps should add it so can make general scripts instead of putting names in
#manually...
sampleNameList = ["cc0pi","cc1pi","ccOth","anuccqe","anuccnqe","nuccqe","nuccnqe"];

#for i in xrange(0,sampleNames.GetEntries()):
#
#    sampleNameList.append(sampleNames.At(i).GetString().Data())

#Lists to store the histograms in.
dataHists = []
prefitHists = []
postfitHists = []

for sName in sampleNameList:

    dataHists.append(inFile.Get(sName + "_data"))
    prefitHists.append(inFile.Get(sName + "_prefit"))
    postfitHists.append(inFile.Get(sName + "_postfit_0_0"))


#Now loop through the samples and histograms and put the correct number in each
#table entry.
for i in xrange(0,len(sampleNameList)):
    
    lineContent = sampleNameList[i]
    dataVal = dataHists[i].Projection(1,0).GetSumOfWeights()
    prefitVal = prefitHists[i].Projection(1,0).GetSumOfWeights()
    postfitVal = postfitHists[i].Projection(1,0).GetSumOfWeights()

    #Now round to two decimal places.
    dataStringVal = "{0:.2f}".format(round(dataVal,2))
    prefitStringVal = "{0:.2f}".format(round(prefitVal,2))
    postfitStringVal = "{0:.2f}".format(round(postfitVal,2))

    lineContent += " & " + dataStringVal
    lineContent += " & " + prefitStringVal
    lineContent += " & " + postfitStringVal

    #Terminate the line now.
    lineContent += "\\\\ \\hline\n"

    #Now the line is complete.  Write it out to the output file.
    outFile.write(lineContent)

#With all lines of the table written, terminate the document and close the
#output file.
outFile.write("\\end{tabular}\n")
outFile.write("\\end{centering}\n")
outFile.write("\\end{document}\n")
outFile.close()

os.system("pdflatex " + sys.argv[2])
os.system("pdflatex " + sys.argv[2])
