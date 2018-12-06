#!/usr/bin/python
from ROOT import *
import sys
import math
import os

gROOT.SetBatch(kTRUE);
#Loop through MaCh3 and BANFF fit ouptuts and plot prefit/postfit/data for the 2 fitters

sampleNames = ["fgd1cc0pi","fgd1cc1pi","fgd1ccOth","fgd1anucc1tr","fgd1anuccntr","fgd1nucc1tr","fgd1nuccntr","fgd2cc0pi","fgd2cc1pi","fgd2ccOth","fgd2anucc1tr","fgd2anuccntr","fgd2nucc1tr","fgd2nuccntr"]
gStyle.SetErrorX(0.0000001)

firstHisto = True
firstHistot = True
firstPrefit = True
firstPostfit = True
firstRatio = True
c = TCanvas("canv", "canv", 1024, 1024)

histMaCh3_temp = []
histMaCh3 = []
def LoopDirectory(direc):
    for key in direc.GetListOfKeys():
        name = key.GetName();
        ClassName = key.GetClassName();
        MAQE = "MAQE"
        if (not(MAQE in name)): 
            continue
        if (ClassName == "TDirectoryFile"):
            direc.cd(name)
            SubDir = gDirectory
            LoopDirectory(SubDir)

        data = "_data"
        norm = "_norm"
        if (data in name):
            continue
        if (not(norm in name)): 
            continue
        if (ClassName != "TGraphAsymmErrors"):
            continue
        print(name)
        c.cd()
        temp = key.ReadObj()
        histMaCh3_temp.append(temp)

infile_MaCh3 = TFile(sys.argv[2])
LoopDirectory(infile_MaCh3)
for i in range(6):
    print i 
    histMaCh3.append(histMaCh3_temp[i])
for i in range(12,16):
    print i 
    histMaCh3.append(histMaCh3_temp[i])
for i in range(20,24):
    print i 
    histMaCh3.append(histMaCh3_temp[i])
for i in range(6,12):
    print i 
    histMaCh3.append(histMaCh3_temp[i])
for i in range(16,20):
    print i 
    histMaCh3.append(histMaCh3_temp[i])
for i in range(24,28):
    print i 
    histMaCh3.append(histMaCh3_temp[i])
print(len(histMaCh3))

inputFile = TFile(sys.argv[1])

for i in xrange(0,len(sampleNames)):

    dataMomProj = (inputFile.Get(sampleNames[i] + "_data")).Projection(0) 
    dataMomProj.SetDirectory(0)
    
    prefitMomProj = (inputFile.Get(sampleNames[i] + "_prefit")).Projection(0)
    prefitMomProj.SetDirectory(0)

    postfitMomProj = (inputFile.Get(sampleNames[i] + "_postfit_0_0")).Projection(0)
    postfitMomProj.SetDirectory(0)

    dataThetaProj = (inputFile.Get(sampleNames[i] + "_data")).Projection(1) 
    dataThetaProj.SetDirectory(0)
    
    prefitThetaProj = (inputFile.Get(sampleNames[i] + "_prefit")).Projection(1)
    prefitThetaProj.SetDirectory(0)

    postfitThetaProj = (inputFile.Get(sampleNames[i] + "_postfit_0_0")).Projection(1)
    postfitThetaProj.SetDirectory(0)


    gStyle.SetOptStat(0)

    #Set up maximum momenta for the different histogram types.
    maxMom = 2000.0
    minTheta = 0.6
    if (i > 2 and i<7) or i>9:
        maxMom = 2000.0
        minTheta = 0.75

    #Do a stack of the momentum projections.
    mpStack = THStack("mpStack",sampleNames[i] + ": " + ";p_{#mu} (MeV/c);Events/(1 MeV/c)")
       
    for j in xrange(1,dataMomProj.GetNbinsX()+2):
         width = float(dataMomProj.GetXaxis().GetBinLowEdge(j+1)-dataMomProj.GetXaxis().GetBinLowEdge(j))
         dataMomProj.SetBinContent(j,dataMomProj.GetBinContent(j)/width)
         dataMomProj.SetBinError(j,dataMomProj.GetBinError(j)/width)
	 prefitMomProj.SetBinContent(j,prefitMomProj.GetBinContent(j)/width)
	 prefitMomProj.SetBinError(j,prefitMomProj.GetBinError(j)/width)
         postfitMomProj.SetBinContent(j,postfitMomProj.GetBinContent(j)/width)
         postfitMomProj.SetBinError(j,postfitMomProj.GetBinError(j)/width)

    #Set the data to black.
    dataMomProj.SetMarkerStyle(20)
    dataMomProj.SetMarkerColor(1)
    dataMomProj.SetLineColor(1)
    dataMomProj.SetLineWidth(2)
        
    #Draw points and error bars for this plot.
    mpStack.Add(dataMomProj,"P0E")
        
    #Set the pre-fit to blue
    prefitMomProj.SetLineColor(4)
    prefitMomProj.SetLineWidth(2)
    mpStack.Add(prefitMomProj,"HIST")

    #Postfit is black.
    postfitMomProj.SetLineColor(1)
    postfitMomProj.SetLineWidth(2)
    mpStack.Add(postfitMomProj,"HIST")

    cProj = TCanvas()
    cProj.cd()
    mpStack.Draw("NOSTACK")
    histMaCh3[i*2].Draw("SAME,P")
    #Now, set the x-axis maximum range.
    mpStack.GetXaxis().SetRangeUser(0,maxMom)

    cProj.Modified()
    cProj.Update()
    
    if(firstHisto):
        cProj0 = TCanvas()
        cProj0.cd()
        leg = TLegend(0.05,0.05,0.95,0.95)
        leg.SetFillColor(0)
        leg.AddEntry(dataMomProj, "Data","LEP")
        leg.AddEntry(prefitMomProj,"Prefit", "L")
        leg.AddEntry(postfitMomProj,"Postfit","L")
        leg.AddEntry(histMaCh3[i*2],"MaCh3 Postfit","LP")
        leg.Draw()
        cProj0.Print("momentumProjections.pdf(")
        cProj.Print("momentumProjections.pdf")
        cProj.Clear()
        firstHisto = False
    else:
        cProj.Print("momentumProjections.pdf")
        cProj.Clear()
        
    #Do a stack of the momentum projections.
    mpStack2 = THStack("mpStack",sampleNames[i] + ": " + ";cos#theta_{#mu};Events/(0.1)")
    
    for j in xrange(1,dataThetaProj.GetNbinsX()+2):
         width = float(dataThetaProj.GetXaxis().GetBinLowEdge(j+1)-dataThetaProj.GetXaxis().GetBinLowEdge(j))
         dataThetaProj.SetBinContent(j,dataThetaProj.GetBinContent(j)/width)
         prefitThetaProj.SetBinContent(j,prefitThetaProj.GetBinContent(j)/width)
         postfitThetaProj.SetBinContent(j,postfitThetaProj.GetBinContent(j)/width)
         dataThetaProj.SetBinError(j,dataThetaProj.GetBinError(j)/width)
         prefitThetaProj.SetBinError(j,prefitThetaProj.GetBinError(j)/width)
         postfitThetaProj.SetBinError(j,postfitThetaProj.GetBinError(j)/width)
    
    #Set the data to black.
    dataThetaProj.SetMarkerStyle(20)
    dataThetaProj.SetMarkerColor(1)
    dataThetaProj.SetLineColor(1)
    dataThetaProj.SetLineWidth(2)
        
    #Draw points and error bars for this plot.
    mpStack2.Add(dataThetaProj,"P0E")
        
    #Set the pre-fit to blue
    prefitThetaProj.SetLineColor(4)
    prefitThetaProj.SetLineWidth(2)
    mpStack2.Add(prefitThetaProj,"HIST")

    #Postfit is black.
    postfitThetaProj.SetLineColor(1)
    postfitThetaProj.SetLineWidth(2)
    mpStack2.Add(postfitThetaProj,"HIST")

    cProj = TCanvas()
    cProj.cd()
    mpStack2.Draw("NOSTACK")
    histMaCh3[i*2+1].Draw("SAME,P")
    #Now, set the x-axis maximum range.
    mpStack2.GetXaxis().SetRangeUser(minTheta,1)

    cProj.Modified()
    cProj.Update()
    
    if(firstHistot):
        cProj0 = TCanvas()
        cProj0.cd()
        leg = TLegend(0.05,0.05,0.95,0.95)
        leg.SetFillColor(0)
        leg.AddEntry(dataThetaProj, "Data","LEP")
        leg.AddEntry(prefitThetaProj,"Prefit", "L")
        leg.AddEntry(postfitThetaProj,"Postfit","L")
        leg.AddEntry(histMaCh3[i*2+1],"MaCh3 Postfit","LP")
        leg.Draw()
        cProj0.Print("thetaProjections.pdf(")
        cProj.Print("thetaProjections.pdf")
        firstHistot = False
    else:
        cProj.Print("thetaProjections.pdf")
    

c1 = TCanvas()
c1.cd()
c1.Print("momentumProjections.pdf]")
c1.Print("thetaProjections.pdf]")
