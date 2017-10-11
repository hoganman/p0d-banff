#!/usr/bin/python
from ROOT import *
import sys

#Load in the file with the toys.
toyFile = TFile(sys.argv[1])

sampleNames = toyFile.Get("sampleNames")
sampleNameList = []

for i in xrange(0, sampleNames.GetEntries()):

    sampleNameList.append(sampleNames.At(i).GetString().Data())

dataHists = []

#Load in the data histograms for each sample.
for sample in sampleNameList:

    pDataHist = toyFile.Get(sample+"_data").Projection(0).Clone(sample+"_p_data")
    ctDataHist = toyFile.Get(sample+"_data").Projection(1).Clone(sample+"_ct_data")

    thisProjections = [pDataHist,ctDataHist]

    dataHists.append(thisProjections)



numberOfToys = toyFile.Get("nToys");
nToys = numberOfToys.GetUniqueID();


#Now load in all the toy prediction histograms, turning them into data/MC
#ratios as we go.
toyRatioHists = []
for i in xrange(0, nToys):

    thisRatioHists = []

    for j in xrange(0, len(sampleNameList)):

        pToyHist = toyFile.Get(sampleNameList[j]+"_predMC_"+str(i)).Projection(0).Clone(sampleNameList[j]+"p_predMC_"+str(i))
        ctToyHist = toyFile.Get(sampleNameList[j]+"_predMC_"+str(i)).Projection(1).Clone(sampleNameList[j]+"ct_predMC_"+str(i))
        
        pRatio = dataHists[j][0].Clone(sampleNameList[j]+"_p_ratio_"+str(i))
        ctRatio = dataHists[j][1].Clone(sampleNameList[j]+"_ct_ratio_"+str(i))
        
        pRatio.Divide(pToyHist)
        ctRatio.Divide(ctToyHist)

        thisObs = [pRatio,ctRatio]
        thisRatioHists.append(thisObs)

    toyRatioHists.append(thisRatioHists)


#Alright, now we have toyRatioHists[toyNumber][SampleNumber][ObservableNumber]
#Make a meanRatioHists list.

meanRatioHists = []

#Start it off with the first toy for each sample.
for j in xrange(0, len(sampleNameList)):

    pMeanHist = toyRatioHists[0][j][0].Clone(sampleNameList[j]+"_p_mean")
    ctMeanHist = toyRatioHists[0][j][1].Clone(sampleNameList[j]+"_ct_mean")

    thisMean = [pMeanHist,ctMeanHist]
    meanRatioHists.append(thisMean)

#Now, go through all the remaining toys and add in the ratio.
for i in xrange(1,nToys):

    for j in xrange(0, len(sampleNameList)):

        meanRatioHists[j][0].Add(toyRatioHists[i][j][0])
        meanRatioHists[j][1].Add(toyRatioHists[i][j][1])


#Divide by the number of toys to get the final mean.
for j in xrange(0, len(sampleNameList)):

    meanRatioHists[j][0].Scale(1.0/nToys)
    meanRatioHists[j][1].Scale(1.0/nToys)

    #Take this opportunity to set all bin errors to zero.
    for bi in xrange(1,meanRatioHists[j][0].GetNbinsX() + 1):
        meanRatioHists[j][0].SetBinError(bi,0.0)

    for bi in xrange(1,meanRatioHists[j][1].GetNbinsX() + 1):
        meanRatioHists[j][1].SetBinError(bi,0.0)




#Now that we have a mean, compute the RMS using the error.
for j in xrange(0, len(sampleNameList)):

    #Loop over the toys.
    for i in xrange(0, nToys):

        
        #Loop over each bin in the histogram.
        for bi in xrange(1,meanRatioHists[j][0].GetNbinsX() + 1):
            meanVal = meanRatioHists[j][0].GetBinContent(bi)
            toyVal = toyRatioHists[i][j][0].GetBinContent(bi)
            meanRatioHists[j][0].SetBinError(bi,meanRatioHists[j][0].GetBinError(bi)
                + pow(toyVal-meanVal,2))

        for bi in xrange(1,meanRatioHists[j][1].GetNbinsX() + 1):
            meanVal = meanRatioHists[j][1].GetBinContent(bi)
            toyVal = toyRatioHists[i][j][1].GetBinContent(bi)
            meanRatioHists[j][1].SetBinError(bi,meanRatioHists[j][1].GetBinError(bi)
                + pow(toyVal-meanVal,2))


#With all toys looped over, go through and finalize the bin errors by dividing
#by the number of toys and taking the square root.
for j in xrange(0, len(sampleNameList)):
  
    #Loop over each bin in the histogram.
    for bi in xrange(1,meanRatioHists[j][0].GetNbinsX() + 1):
        meanRatioHists[j][0].SetBinError(bi,sqrt(meanRatioHists[j][0].GetBinError(bi)/nToys))

    for bi in xrange(1,meanRatioHists[j][1].GetNbinsX() + 1):
        meanRatioHists[j][1].SetBinError(bi,sqrt(meanRatioHists[j][1].GetBinError(bi)/nToys))


#Now just loop through and plot the results.
gStyle.SetOptStat(0)
c1 = TCanvas()
c1.SetTicks(1,1)
c1.Print("dataMCRatiosWithUnc.pdf[")
for j in xrange(0, len(sampleNameList)):

    meanRatioHists[j][0].Draw()
    meanRatioHists[j][0].SetTitle("")
    meanRatioHists[j][0].GetXaxis().SetTitle("Muon momentum (MeV/c)")
    meanRatioHists[j][0].GetYaxis().SetTitle("Data/MC")
    meanRatioHists[j][0].SetMaximum(1.5)
    meanRatioHists[j][0].SetMinimum(0.5)
    xMax = meanRatioHists[j][0].GetXaxis().GetXmax()
    c1.SetLogx() #Use logarithmic x axis for momentum plots.
    c1.Modified()
    c1.Update()
#    lineAtOne = TLine(c1.GetUxmin(),1.0,c1.GetUxmax(),1.0)
#    lineAtOne.SetLineWidth(3)
#    lineAtOne.SetLineColor(kBlack)
#    lineAtOne.Draw("same")

    #In log scale, need to do this instead of a line.
    #Actually, this should work in both scales.
    lineAtOne = TF1("lineAtOne","1",0,xMax)
    lineAtOne.SetLineWidth(3)
    lineAtOne.SetLineColor(kBlack)
    lineAtOne.Draw("same")

    c1.Modified()
    c1.Update()
    c1.Print("dataMCRatiosWithUnc.pdf")
    c1.Clear()
    c1.SetLogx(0) #Set back to linear x-axis for cos(theta) plots.
    meanRatioHists[j][1].Draw()
    meanRatioHists[j][1].SetTitle("")
    meanRatioHists[j][1].GetXaxis().SetTitle("Muon cos#theta")
    meanRatioHists[j][1].GetYaxis().SetTitle("Data/MC")
    meanRatioHists[j][1].SetMaximum(1.5)
    meanRatioHists[j][1].SetMinimum(0.5)
    c1.Modified()
    c1.Update()
    lineAtOne = TLine(c1.GetUxmin(),1.0,c1.GetUxmax(),1.0)
    lineAtOne.SetLineWidth(3)
    lineAtOne.SetLineColor(kBlack)
    lineAtOne.Draw("same")
    c1.Modified()
    c1.Update()
    c1.Print("dataMCRatiosWithUnc.pdf")
    c1.Clear()
c1.Print("dataMCRatiosWithUnc.pdf]")

