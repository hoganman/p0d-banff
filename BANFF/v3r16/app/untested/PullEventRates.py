#!/usr/bin/python
#Reads in a user supplied throw file with nominal MC for many different weight
#combinations, and outputs a .tex file that can be made into a table.
#Usage: MakeEventRateType
from ROOT import *
import sys
from numpy import *
import glob

#gStyle.SetOptStat(0)
gROOT.SetBatch(kTRUE)

#Now grab the sample names from the sample name TObjString.
sampleNameList = ["fgd1cc0pi","fgd1cc1pi","fgd1ccOth","fgd1anucc1tr","fgd1anuccntr","fgd1nucc1tr","fgd1nuccntr","fgd2cc0pi","fgd2cc1pi","fgd2ccOth","fgd2anucc1tr","fgd2anuccntr","fgd2nucc1tr","fgd2nuccntr"]

nom = [16773.41,4357.82,3687.49,3565.30,1021.73,1267.46,1294.56,17206.43,3650.06,3584.55,3653.36,1063.78,1258.27,1240.33]
hist_final = []

fgd1cc0pi = TH1F(sampleNameList[0],sampleNameList[0],40,14500,17000)
fgd1cc1pi = TH1F(sampleNameList[1],sampleNameList[1],50,4000,6000)
fgd1ccOth = TH1F(sampleNameList[2],sampleNameList[2],50,3000,5000)
fgd1anucc1tr = TH1F(sampleNameList[3],sampleNameList[3],50,0,2000)
fgd1anuccntr = TH1F(sampleNameList[4],sampleNameList[4],50,0,2000)
fgd1nucc1tr = TH1F(sampleNameList[5],sampleNameList[5],50,0,2000)
fgd1nuccntr = TH1F(sampleNameList[6],sampleNameList[6],50,0,2000)

fgd2cc0pi = TH1F(sampleNameList[7],sampleNameList[7],40,14500,17000)
fgd2cc1pi = TH1F(sampleNameList[8],sampleNameList[8],50,3000,5000)
fgd2ccOth = TH1F(sampleNameList[9],sampleNameList[9],50,3000,5000)
fgd2anucc1tr = TH1F(sampleNameList[10],sampleNameList[10],50,0,2000)
fgd2anuccntr = TH1F(sampleNameList[11],sampleNameList[11],50,0,2000)
fgd2nucc1tr = TH1F(sampleNameList[12],sampleNameList[12],50,0,2000)
fgd2nuccntr = TH1F(sampleNameList[13],sampleNameList[13],50,0,2000)

hist_final.append(fgd1cc0pi)
hist_final.append(fgd1cc1pi)
hist_final.append(fgd1ccOth)
hist_final.append(fgd1anucc1tr)
hist_final.append(fgd1anuccntr)
hist_final.append(fgd1nucc1tr)
hist_final.append(fgd1nuccntr)
hist_final.append(fgd2cc0pi)
hist_final.append(fgd2cc1pi)
hist_final.append(fgd2ccOth)
hist_final.append(fgd2anucc1tr)
hist_final.append(fgd2anuccntr)
hist_final.append(fgd2nucc1tr)
hist_final.append(fgd2nuccntr)

inFile_pre = TFile("DataFit_2016_1p1h_MAQEH_PionSI_170622.root")
nomList = []
for sName in sampleNameList:
    #With all histograms added, append to the histogamList (of lists)
    nomList.append(inFile_pre.Get(sName + "_prefit"))

inFile = TFile("../toys/throws_obsnorm_170709.root")

for i in xrange(0, 1000):
    
    #Now for each sample, load in the corresponding list of THnD objects to fill
    #the table.
    histogramList = []
    
    for sName in sampleNameList:
        #With all histograms added, append to the histogamList (of lists)
        histogramList.append(inFile.Get(sName + "_predMC_"+str(i)))

    #Now loop through the samples and histograms and put the correct number in each
    #table entry.
    for i in xrange(0,len(sampleNameList)):
            hist_final[i].Fill(histogramList[i].Projection(1,0).GetSumOfWeights())
            nom[i] = nomList[i].Projection(1,0).GetSumOfWeights()
c1 = TCanvas()
c1.Print("event_number.pdf[")

for i in xrange(0,len(sampleNameList)):
    c1.cd()
    hist_final[i].SetLineWidth(3)
    hist_final[i].SetLineColor(kBlack)
    hist_final[i].Draw()
    c1.Modified()
    c1.Update()
    dataLine2 = TLine(nom[i],c1.GetUymin(),nom[i],c1.GetUymax())
    dataLine2.SetLineWidth(3)
    dataLine2.SetLineColor(kRed)
    dataLine2.Draw("same")
    c1.Modified()
    c1.Update()
    c1.Print("event_number.pdf")   
    
    #hist_final[i].Fit("gaus");
    #print sampleNameList[i], hist_final[i].GetFunction("gaus").GetParameter(2)
    
c1.Print("event_number.pdf]")

    
