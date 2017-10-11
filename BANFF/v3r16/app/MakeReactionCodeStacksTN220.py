#!/usr/bin/python
from ROOT import *
import sys
import math


gStyle.SetTitleAlign(33)
gStyle.SetTitleX(0.85)
gStyle.SetTitleY(0.85)

def FindPlotMax(dataPlot,mcPlot):

    dataBinContentPlusError = []
    mcBinContentPlusError = []

    for i in xrange(1, dataPlot.GetNbinsX() + 1):
        dataBinContentPlusError.append(dataPlot.GetBinContent(i) + dataPlot.GetBinError(i))
        mcBinContentPlusError.append(mcPlot.GetBinContent(i) + mcPlot.GetBinError(i))
        

    #Now, return the maximum of those two, plus 10% so that the histogram is
    #definitely big enough
    return max(max(dataBinContentPlusError),max(mcBinContentPlusError))*1.10


inputFile = TFile(sys.argv[1])
dataFile = TFile(sys.argv[2])
dummyPage = TCanvas("c","c",500,500)
dummyPage.Print("reactionCodeStacksTN220.pdf[")
gStyle.SetErrorX(0.0000001)
gStyle.SetHatchesLineWidth(4)
#Set up the array of sample names.
sampleNames = ["cc0pi","cc1pi","ccOth","anuccqe","anuccnqe","nuccqe","nuccnqe"]
stackHistos = ["#nu CCQE","#nu MEC", "#nu CCRES", "#nu CCCOH", "#nu CCOTHER", "#nu NCRES", "#nu NCCOH", "#nu NCOTHER" ,"#bar{#nu} CCQE", "#bar{#nu} MEC", "#bar{#nu} CCRES", "#bar{#nu} CCCOH", "#bar{#nu} CCOTHER", "#bar{#nu} NCRES", "#bar{#nu} NCCOH", "#bar{#nu} NCOTHER"]

#latexTitles = ["#nu_{#mu} CC-0#pi selection (#nu mode)","#nu_{#mu} CC-1#pi selection (#nu mode)","#nu_{#mu} CC-Other selection (#nu mode)","#bar{#nu}_{#mu} CC-1Track selection (#bar{#nu} mode)","#bar{#nu}_{#mu} CC-NTracks selection (#bar{#nu} mode)","#nu_{#mu} CC-1Track selection (#bar{#nu} mode)","#nu_{#mu} CC-NTracks selection (#bar{#nu} mode)"]

latexTitles = ["#nu_{#mu} CC-0#pi selection (FHC)","#nu_{#mu} CC-1#pi selection (FHC)","#nu_{#mu} CC-Other selection (FHC)","#bar{#nu}_{#mu} CC-1Track selection (RHC)","#bar{#nu}_{#mu} CC-NTracks selection (RHC)","#nu_{#mu} CC-1Track selection (RHC)","#nu_{#mu} CC-NTracks selection (RHC)"]


stackColours = [kBlue, kMagenta, kRed, kOrange + 1, kGray, kGreen + 2, kCyan,
        kRed-6, kBlue, kMagenta, kRed, kOrange + 1, kGray, kGreen + 2, kCyan,
        kRed-6]

#stackFillStyle = [3351, 3351, 3351, 3351, 3351, 3351, 3351, 3351, 3315, 3315,
#        3315, 3315, 3315, 3315, 3315, 3315]

#Try a more aggressive angle.
stackFillStyle = [3354, 3354, 3354, 3354, 3354, 3354, 3354, 3354, 3345, 3345,
        3345, 3345, 3345, 3345, 3345, 3345]



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


#The graphs will be of the form <sampleName>_rxnPredMC_<rxnCode+100>
#So for each sample, we'll loop from 0-200, and if the THnD is not NULL when we
#try and get it, add it to a list for stacking.
for sampleName in sampleNames:

    
    #Now make the set of histograms to show, which combine several interaction
    #types.
    #Load in the data histogram and use it to assemble the TH2Ds for the MC, as
    #well as making it one of the histograms on the stack.
    dataHist = dataFile.Get(sampleName + "_data").Projection(1,0)

    #There is only one data plot for each stack, so:
    #Momentum projection
    #Theta slices.
    #Assemble them here.
    dataOneDimPlots = []
    dataOneDimPlots.append(dataHist.ProjectionX("data" + sampleName + "_px",1,dataHist.GetNbinsY()))
    dataNXBins = dataHist.GetXaxis().GetNbins()
    dataxarray = dataHist.GetXaxis().GetXbins().GetArray()

    #Now, loop through all the theta bins and add the slices in.
    for iby in xrange(1, dataHist.GetNbinsY() + 1):

        thetaLow = dataHist.GetYaxis().GetBinLowEdge(iby)
        thetaHigh = dataHist.GetYaxis().GetBinUpEdge(iby)

        thetaString = " < cos#theta < "
        thetaString = str(thetaLow) + thetaString + str(thetaHigh)
        dataOneDimPlots.append(TH1D("dataslice_" + sampleName + str(iby),thetaString, dataNXBins, dataxarray))

        #Now, go through the x bins in this slice and fill the plot we
        #just created.
        for ibx in xrange(1, dataHist.GetNbinsX() + 1):

            #Get the bin dimensions for dividing purposes to area
            #normalize.
            dataOneDimPlots[len(dataOneDimPlots)-1].SetBinContent(ibx,
                    dataHist.GetBinContent(ibx,iby))


    #Now that the dataOneDimPlots array is filled, loop through a area
    #normalize the bins.
    for dataOneDimHist in dataOneDimPlots:
        for ibx in xrange(1, dataOneDimHist.GetNbinsX() + 1):
            mpdim = (dataOneDimHist.GetXaxis().GetBinUpEdge(ibx) -
                dataOneDimHist.GetXaxis().GetBinLowEdge(ibx))/100.0
            dataOneDimHist.SetBinContent(ibx,
                    dataOneDimHist.GetBinContent(ibx)/mpdim)
            
    histosToStack = []
    for histo in stackHistos:

        histosToStack.append(TH2D(sampleName + "_" + histo, sampleName + "_" +
            histo, dataHist.GetXaxis().GetNbins(),
            dataHist.GetXaxis().GetXbins().GetArray(),dataHist.GetYaxis().GetNbins(),dataHist.GetYaxis().GetXbins().GetArray()))

    #Now pull the histograms from the file, and add their content to the
    #relevant combined histogram. 
    for i in xrange(0,200):

        if inputFile.Get(sampleName + "_rxnPredMC_" + str(i)):

                histosToStack[CorrespondingIndex(i)].Add(inputFile.Get(sampleName + "_rxnPredMC_" + str(i)).Projection(1,0))



    #OK, so now we have an array of 2D histograms for this sample that we'd
    #like to stack.  Need to divide it up into individual ones.  Do Momentum
    #Projection, then momentum in theta slices.  Normalize by bin area.
    #For storing all the one dimensional plots for this sample.
    oneDimPlots = []
   
    #Add nothing for the momentum projection, will append the thetaStrings as
    #needed.
    stackTitles = []  
    for i in xrange(0, len(stackHistos)):

        #For storing all the one dimensional plots for this reaction code.
        oneDimPlotsThisReac = []
 
        nameBase = histosToStack[i].GetTitle()
        maxMom = 5000.0
        minTheta = 0.6

        if("nu" in sampleName):
            maxMom = 10000.0
            minTheta = -1.0

        oneDimPlotsThisReac.append(histosToStack[i].ProjectionX(nameBase + "_px",1,histosToStack[i].GetNbinsY()))
       
        stackTitles.append("p_proj")
        #Area normalize this now.
        NXBins = histosToStack[i].GetXaxis().GetNbins()
        xarray = histosToStack[i].GetXaxis().GetXbins().GetArray()

        for ibx in xrange(1, NXBins + 1):
            mpdim = (histosToStack[i].GetXaxis().GetBinUpEdge(ibx) -
                histosToStack[i].GetXaxis().GetBinLowEdge(ibx))/100.0
            oneDimPlotsThisReac[0].SetBinContent(ibx,
                    oneDimPlotsThisReac[0].GetBinContent(ibx)/mpdim)
            

        #Now loop over all the theta bins, and make the theta slice plots.
        for iby in xrange(1, histosToStack[i].GetNbinsY() + 1):

            thetaLow = histosToStack[i].GetYaxis().GetBinLowEdge(iby)
            thetaHigh = histosToStack[i].GetYaxis().GetBinUpEdge(iby)

            thetaString = " < cos#theta < "
            thetaString = str(thetaLow) + thetaString + str(thetaHigh)
            stackTitles.append(thetaString)
            oneDimPlotsThisReac.append(TH1D("slice_" + nameBase +
                str(iby),thetaString, NXBins, xarray))

            #Now loop through the x bins for this y bin, and make the 1D
            #histogram's xbin content equal to that.
            for ibx in xrange(1, histosToStack[i].GetNbinsX() + 1):

                #Get the bin dimensions for dividing purposes to area
                #normalize.
                mpdim = (histosToStack[i].GetXaxis().GetBinUpEdge(ibx) -
                    histosToStack[i].GetXaxis().GetBinLowEdge(ibx))/100.0
                oneDimPlotsThisReac[len(oneDimPlotsThisReac)-1].SetBinContent(ibx,
                        histosToStack[i].GetBinContent(ibx,iby)/mpdim)

            #Now that the x bins are filled, loop on to the next histogram.

        #With all the y-slices done, oneDimPlotsThisReac is now full.  Append
        #it to oneDimPlots.
        oneDimPlots.append(oneDimPlotsThisReac)


    #At this point in time, oneDimPlots has a momentum projection and theta
    #slices for each reaction code grouping in this sample. So now we just need
    #to make TStacks for them and print them out to PDF.
    #The stack contains the plot from the same index of each entry of
    #oneDimPlots.
    #They should all have the same length, so use the first one to loop
    #through (number of slices plus 1 full momenutm projection.)

    for odi in xrange(0, len(oneDimPlots[0])):
       
        #For each one create a TStack
        #sampleStack =  THStack("mpStack_" + sampleName + str(odi)
        #        ,sampleName + ": " + stackTitles[odi] + ";p_{#mu} (MeV/c);Events/(100 MeV/c)")

        #sampleStack =  THStack("mpStack_" + sampleName + str(odi)
        #        ,sampleName + ": " + stackTitles[odi])

        sampleStack =  THStack("mpStack_" + sampleName + str(odi)
                ,stackTitles[odi])




        leg = TLegend(0.30,0.10,0.70,0.80)
        leg.SetFillColor(0)

        #Make data the first entry.
        leg.AddEntry(dataOneDimPlots[odi],"Data", "LEP")
        dataOneDimPlots[odi].SetMarkerStyle(20)
        dataOneDimPlots[odi].SetMarkerColor(kBlack)
        dataOneDimPlots[odi].SetLineColor(kBlack)
        dataOneDimPlots[odi].SetLineWidth(3)

        for odj in xrange(0, len(oneDimPlots)):
            
            #Apply the appropriate plot options from the beginning.
            oneDimPlots[odj][odi].SetFillColor(stackColours[odj])
            oneDimPlots[odj][odi].SetFillStyle(stackFillStyle[odj])
            sampleStack.Add(oneDimPlots[odj][odi])
            leg.AddEntry(oneDimPlots[odj][odi],stackHistos[odj])


        c1 = TCanvas("c","c",500,500)
        c1.cd()
        c1.SetTicks(1,1)
        sampleStack.Draw("")
        dataOneDimPlots[odi].Draw("P0E0same")
        #sampleStack.SetMaximum(max(sampleStack.GetMaximum()+0.15*(sampleStack.GetMaximum()),dataOneDimPlots[odi].GetMaximum()+0.15*(dataOneDimPlots[odi].GetMaximum())))
        sampleStack.SetMaximum(max(sampleStack.GetMaximum(),FindPlotMax(dataOneDimPlots[odi],dataOneDimPlots[odi])))
    #leg.Draw()
        sampleStack.SetMinimum(0.0001)
        sampleStack.GetXaxis().SetRangeUser(0,maxMom)
        sampleStack.GetXaxis().SetNdivisions(505)
        sampleStack.GetXaxis().SetLabelSize(0.045)
        sampleStack.GetYaxis().SetLabelSize(0.045)
        c1.Modified()
        c1.Update()
        c1.Print("reactionCodeStacksTN220.pdf")
        
        #If we're on the very last plot, create another TCanvas and save the
        #legend to it.
        if odi == (len(oneDimPlots[0]) - 1):
            c2 = TCanvas("c2","c2",500,500)
            c2.cd()
            leg.Draw()
            title = TPaveText(0.10,0.80,0.90,0.95,"NBNDC")
            title.AddText(latexTitles[sampleNames.index(sampleName)])
            xaxisTitle = TPaveText(0.10,0.0,0.90,0.07,"NBNDC")
            xaxisTitle.SetFillColor(0)
            xaxisTitle.SetBorderSize(0)
            xaxisTitle.AddText("p_{#mu} (MeV/c)")
            yaxisTitle = TPaveText(0.90,0.10,1.0,0.80,"NBNDC")
            yaxisTitle.SetFillColor(0)
            yaxisTitle.SetBorderSize(0)
            yaxisText = yaxisTitle.AddText("Events/(100 MeV/c)")
            #yaxisTitle.SetTextAlign(11)
            yaxisText.SetTextAngle(90)
            yaxisText.SetTextSize(0.07)
            title.SetFillColor(0)
            title.SetBorderSize(0)
            title.Draw("")
            xaxisTitle.Draw("")
            yaxisTitle.Draw("")
            c2.Print("reactionCodeStacksTN220.pdf")


dummyPage = TCanvas("c","c",500,500)
dummyPage.Print("reactionCodeStacksTN220.pdf]")
