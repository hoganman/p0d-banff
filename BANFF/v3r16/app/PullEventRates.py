#!/usr/bin/python
#Reads in a user supplied throw file with nominal MC for many different weight
#combinations, and outputs a .tex file that can be made into a table.
#Usage: MakeEventRateType
from ROOT import *
import sys
from numpy import *
import glob
import mmap

gStyle.SetOptStat(111111)
gROOT.SetBatch(kTRUE)

#Now grab the sample names from the sample name TObjString.
sampleNameList = ["FGD1_numuCC_0pi","FGD1_numuCC_1pi","FGD1_numuCC_other","FGD2_numuCC_0pi","FGD2_numuCC_1pi","FGD2_numuCC_other","FGD1_anti-numuCC_QE","FGD1_anti-numuCC_nQE","FGD2_anti-numuCC_1_track","FGD2_anti-numuCC_N_tracks","FGD1_NuMuBkg_CCQE_in_AntiNu_Mode_","FGD1_NuMuBkg_CCnQE_in_AntiNu_Mode","FGD2_NuMuBkg_CCQE_in_AntiNu_Mode_","FGD2_NuMuBkg_CCnQE_in_AntiNu_Mode"]

nom  = [0,0,0,0,0,0,0,0,0,0,0,0,0,0] #Nominal event rates
mean = [0,0,0,0,0,0,0,0,0,0,0,0,0,0] #Value to centre histograms on
hist_final = [] #Array of final histograms for each sample

FGD1_numuCC_0pi = TH1F(sampleNameList[0],sampleNameList[0],30,mean[0]-2000,mean[0]+2000)
FGD1_numuCC_1pi = TH1F(sampleNameList[1],sampleNameList[1],30,mean[1]-400,mean[1]+400)
FGD1_numuCC_other = TH1F(sampleNameList[2],sampleNameList[2],30,mean[2]-400,mean[2]+400)
FGD2_numuCC_0pi = TH1F(sampleNameList[3],sampleNameList[3],30,mean[3]-600,mean[3]+600)
FGD2_numuCC_1pi = TH1F(sampleNameList[4],sampleNameList[4],30,mean[4]-400,mean[4]+400)
FGD2_numuCC_other = TH1F(sampleNameList[5],sampleNameList[5],30,mean[5]-400,mean[5]+400)
FGD1_antinumuCC_QE = TH1F(sampleNameList[6],sampleNameList[6],30,mean[6]-400,mean[6]+400)
FGD1_antinumuCC_nQE = TH1F(sampleNameList[7],sampleNameList[7],30,mean[7]-200,mean[7]+200)
FGD2_antinumuCC_1_track = TH1F(sampleNameList[8],sampleNameList[8],30,mean[8]-400,mean[8]+400)
FGD2_antinumuCC_N_tracks = TH1F(sampleNameList[9],sampleNameList[9],30,mean[9]-200,mean[9]+200)
FGD1_NuMuBkg_CCQE_in_AntiNu_Mode_ = TH1F(sampleNameList[10],sampleNameList[10],30,mean[10]-200,mean[10]+200)
FGD1_NuMuBkg_CCnQE_in_AntiNu_Mode = TH1F(sampleNameList[11],sampleNameList[11],30,mean[11]-200,mean[11]+200)
FGD2_NuMuBkg_CCQE_in_AntiNu_Mode_ = TH1F(sampleNameList[12],sampleNameList[12],30,mean[12]-200,mean[12]+200)
FGD2_NuMuBkg_CCnQE_in_AntiNu_Mode = TH1F(sampleNameList[13],sampleNameList[13],30,mean[13]-200,mean[13]+200)

hist_final.append(FGD1_numuCC_0pi)
hist_final.append(FGD1_numuCC_1pi)
hist_final.append(FGD1_numuCC_other)
hist_final.append(FGD2_numuCC_0pi)
hist_final.append(FGD2_numuCC_1pi)
hist_final.append(FGD2_numuCC_other)
hist_final.append(FGD1_antinumuCC_QE)
hist_final.append(FGD1_antinumuCC_nQE)
hist_final.append(FGD2_antinumuCC_1_track)
hist_final.append(FGD2_antinumuCC_N_tracks)
hist_final.append(FGD1_NuMuBkg_CCQE_in_AntiNu_Mode_)
hist_final.append(FGD1_NuMuBkg_CCnQE_in_AntiNu_Mode)
hist_final.append(FGD2_NuMuBkg_CCQE_in_AntiNu_Mode_)
hist_final.append(FGD2_NuMuBkg_CCnQE_in_AntiNu_Mode)

toyFile = TFile("/which/directory/toy_throws.root")

fitFileName = "/which/directory/MultiTrackRHC_v3r33_prefit.root"
fitFile = TFile(fitFileName)

nThrows = 2000
#Loop through toys
for i in xrange(0, nThrows):
    
    #Loop through samples
    for iName, sName in enumerate(sampleNameList):
        #Get toy histogram
        toyHist = toyFile.Get(sName + "_predMC_"+str(i))
        #Get nominal event rates
        if i == 0:
            prefitHist = fitFile.Get(sName + "_prefit")
            nom[iName] = prefitHist.Projection(1,0).GetSumOfWeights()

        #Fill histogram with the difference between the toy and nominal event rate
        hist_final[iName].Fill(toyHist.Projection(1,0).GetSumOfWeights() - nom[iName])

pdf_name = "output_file"
c1 = TCanvas()
c1.Print(pdf_name+".pdf[")

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
    c1.Print(pdf_name+".pdf")

c1.Print(pdf_name+".pdf]")
