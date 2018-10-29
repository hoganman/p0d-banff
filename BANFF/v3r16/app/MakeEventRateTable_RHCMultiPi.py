#!/usr/bin/python
#Reads in a user supplied throw file with nominal MC for many different weight
#combinations, and outputs a .tex file that can be made into a table.
#Usage: MakeEventRateType
from ROOT import *
import sys
from numpy import *

fileName = sys.argv[1]

inFile = TFile(fileName)

outFile = open(sys.argv[2],"w")

#Start by printing the needed header information to the output file.
outFile.write("\\documentclass{article}\n")
outFile.write("\\usepackage{default}\n")
outFile.write("\\usepackage{booktabs}\n")
outFile.write("\\usepackage[margin=0.25in]{geometry}")
outFile.write("\\begin{document}\n")
outFile.write("\\begin{centering}\n")

#There are 9 columns in total.
outFile.write("\\begin{tabular}{lccccccc}\n")
outFile.write("\\toprule\n")
outFile.write("Sample & Data  & Raw MC & POT only & POT+flux & POT+xsec & POT+det & Prefit \\\\ \\midrule\n")

#Now grab the sample names from the sample name TObjString.
sampleNameList = ["FGD1_numuCC_0pi"                    ,
                  "FGD1_numuCC_1pi"                    ,
                  "FGD1_numuCC_other"                  ,
                  "FGD2_numuCC_0pi"                    ,
                  "FGD2_numuCC_1pi"                    ,
                  "FGD2_numuCC_other"                  ,

                  "FGD1_anti-numuCC_0pi"               ,
                  "FGD1_anti-numuCC_1pi"               ,
                  "FGD1_anti-numuCC_other"             ,
                  "FGD2_anti-numuCC_0pi"               ,
                  "FGD2_anti-numuCC_1pi"               ,
                  "FGD2_anti-numuCC_other"             ,
                  
                  "FGD1_NuMuBkg_CC0pi_in_AntiNu_Mode"  ,
                  "FGD1_NuMuBkg_CC1pi_in_AntiNu_Mode"  ,
                  "FGD1_NuMuBkg_CCother_in_AntiNu_Mode",
                  "FGD2_NuMuBkg_CC0pi_in_AntiNu_Mode"  ,
                  "FGD2_NuMuBkg_CC1pi_in_AntiNu_Mode"  ,
                  "FGD2_NuMuBkg_CCother_in_AntiNu_Mode"]


sampleName = ["FHC FGD1 $\\nu_{\mu}$ CC $0\pi$"          ,
              "FHC FGD1 $\\nu_{\mu}$ CC $1\pi$"          ,
              "FHC FGD1 $\\nu_{\mu}$ CC Other"           ,
              "FHC FGD2 $\\nu_{\mu}$ CC $0\pi$"          ,
              "FHC FGD2 $\\nu_{\mu}$ CC $1\pi$"          ,
              "FHC FGD2 $\\nu_{\mu}$ CC Other"           ,

              "RHC FGD1 $\\bar{\\nu}_{\mu}$ CC CC $0\pi$",
              "RHC FGD1 $\\bar{\\nu}_{\mu}$ CC CC $1\pi$",
              "RHC FGD1 $\\bar{\\nu}_{\mu}$ CC CC Other" ,
              "RHC FGD2 $\\bar{\\nu}_{\mu}$ CC CC $0\pi$",
              "RHC FGD2 $\\bar{\\nu}_{\mu}$ CC CC $1\pi$",
              "RHC FGD2 $\\bar{\\nu}_{\mu}$ CC CC Other" ,

              "RHC FGD1 $\\nu_{\mu}$ CC $0\pi$"          ,
              "RHC FGD1 $\\nu_{\mu}$ CC $1\pi$"          ,
              "RHC FGD1 $\\nu_{\mu}$ CC Other"           ,
              "RHC FGD2 $\\nu_{\mu}$ CC $0\pi$"          ,
              "RHC FGD2 $\\nu_{\mu}$ CC $1\pi$"          ,
              "RHC FGD2 $\\nu_{\mu}$ CC Other"]

#Now for each sample, load in the corresponding list of THnD objects to fill
#the table.
histogramList = []

for sName in sampleNameList:

    thisHistList = []

    

    thisHistList.append(inFile.Get(sName + "_data"))
    thisHistList.append(inFile.Get(sName + "_prefit_withNoWeights"))
    thisHistList.append(inFile.Get(sName + "_prefit_withPOTWeights"))
    thisHistList.append(inFile.Get(sName + "_prefit_withNomFluxWeights"))
    thisHistList.append(inFile.Get(sName + "_prefit_withNomXSecWeights"))
    thisHistList.append(inFile.Get(sName + "_prefit_withNomDetWeights"))
    thisHistList.append(inFile.Get(sName + "_prefit"))

    #With all histograms added, append to the histogamList (of lists)
    histogramList.append(thisHistList)


#Now loop through the samples and histograms and put the correct number in each
#table entry.
total = zeros(len(histogramList[0]))
for i in xrange(0,len(sampleNameList)):
    
    lineContent = sampleName[i]

    #Now loop through the histograms and add their values in too.
    for j,hist in enumerate(histogramList[i]):

        val = hist.Projection(1,0).GetSumOfWeights()
        total[j]+=val
        #Make it have 2 decimal places.
        stringVal = "{0:.2f}".format(round(val,2))
        
        lineContent += " &  " + stringVal

    #Having reached the end of the line, terminate it.
    lineContent += "\\\\ \n"

    #Now the line is complete.  Write it out to the output file.
    outFile.write(lineContent)

lineContent = "\midrule\nTotal"
#Now line for total
for j,hist in enumerate(histogramList[i]):
    #Make it have 2 decimal places.
    stringVal = "{0:.2f}".format(round(total[j],2))
    lineContent += " &  " + stringVal

#Now the line is complete.  Write it out to the output file.
outFile.write(lineContent)

#With all lines of the table written, terminate the document and close the
#output file.
outFile.write("\\\\\\bottomrule\n\\end{tabular}\n")
outFile.write("\\end{centering}\n")

outFile.write("\\end{document}\n")
outFile.close()
#quit()

#outFile.write("\\begin{tabular}{| c | c | c | c |}\n")
#outFile.write("\\hline\n")
#outFile.write("Sample & Data & Postfit & Prefit \\\\ \\hline\n")
#              
##Now grab the sample names from the sample name TObjString.
#sampleNameList = ["fgd1cc0pi","fgd1cc1pi","fgd1ccOth","fgd1anucc1tr","fgd1anuccntr","fgd1nucc1tr","fgd1nuccntr","fgd2cc0pi","fgd2cc1pi","fgd2ccOth","fgd2anucc1tr","fgd2anuccntr","fgd2nucc1tr","fgd2nuccntr"]
#
#
##Now for each sample, load in the corresponding list of THnD objects to fill
##the table.
#histogramList = []
#
#for sName in sampleNameList:
#
#    thisHistList = []
#
#    thisHistList.append(inFile.Get(sName + "_data"))
#    thisHistList.append(inFile.Get(sName + "_postfit_0_0"))
#    thisHistList.append(inFile.Get(sName + "_prefit"))
#
#    #With all histograms added, append to the histogamList (of lists)
#    histogramList.append(thisHistList)
#
#
##Now loop through the samples and histograms and put the correct number in each
##table entry.
#total = zeros(len(histogramList[0]))
#for i in xrange(0,len(sampleNameList)):
#    
#    lineContent = sampleName[i]
#
#    #Now loop through the histograms and add their values in too.
#    for j,hist in enumerate(histogramList[i]):
#
#        val = hist.Projection(1,0).GetSumOfWeights()
#        total[j]+=val
#        #Make it have 2 decimal places.
#        stringVal = "{0:.2f}".format(round(val,2))
#        
#        lineContent += " &  " + stringVal
#
#    #Having reached the end of the line, terminate it.
#    lineContent += "\\\\ \\hline\n"
#
#    #Now the line is complete.  Write it out to the output file.
#    outFile.write(lineContent)
#
#lineContent = "Total"
##Now line for total
#for j,hist in enumerate(histogramList[i]):
#    #Make it have 2 decimal places.
#    stringVal = "{0:.2f}".format(round(total[j],2))
#    lineContent += " &  " + stringVal
#
##Now the line is complete.  Write it out to the output file.
#outFile.write(lineContent)
#
##With all lines of the table written, terminate the document and close the
##output file.
#outFile.write("\\end{tabular}\n")
#outFile.write("\\end{centering}\n")
#
#outFile.write("\\end{document}\n")
#outFile.close()


