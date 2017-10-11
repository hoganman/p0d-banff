#!/usr/bin/python
from ROOT import *
import sys
import math

#The mcPlot needs to be the one with the biggest error bars.
def FindPlotMax(dataPlot,mcPlot,binsFromStart,binsFromEnd):

    dataBinContentPlusError = []
    mcBinContentPlusError = []

    for i in xrange(1 + binsFromStart, dataPlot.GetNbinsX() + 1 - binsFromEnd):
        dataBinContentPlusError.append(dataPlot.GetBinContent(i) + dataPlot.GetBinError(i))
        mcBinContentPlusError.append(mcPlot.GetBinContent(i) + mcPlot.GetBinError(i))
        

    #Now, return the maximum of those two, plus 10% so that the histogram is
    #definitely big enough
    return max(max(dataBinContentPlusError),max(mcBinContentPlusError))*1.10


#Take two inputs from command line.
firstInput = TFile(sys.argv[1])
secondInput = TFile(sys.argv[2])


#This holds the same name for use on the histogram title.
sampleNames = ["CC-0#pi","CC-1#pi","CC-Other"
        ,"#mu^{+} CC-1Track"
        ,"#mu^{+} CC-NTracks"
        ,"#mu^{-} CC-1Track"
        ,"#mu^{-} CC-NTracks"]

shortNames = ["cc0pi","cc1pi","ccother","anucc1track","anuccntracks","nucc1track","nuccntracks"]
shortObs = ["mumom","mucostheta"]

#The projection, x and y axis labels.
axisLabels = [["Muon momentum (MeV/c)","Events/(100 MeV/c)"]
        , ["Muon cos#theta","Events/0.01"]]

#Need to make sure plot over the right range.
#Indexed by sample and projection number.
xUserRange = [[[0.0,5000.0],[0.0,1.0]] #CC0pi
        ,[[0.0,5000.0],[0.0,1.0]] #CC1pi
        ,[[0.0,5000.0],[0.0,1.0]] #CCOther
        ,[[0.0,3600.0],[0.0,1.0]] #anu CC-1Track
        ,[[0.0,4800.0],[0.0,1.0]] #anu CC-NTrack
        ,[[0.0,2500.0],[0.0,1.0]] #nu CC-1Track.
        ,[[0.0,2500.0],[0.0,1.0]]] #nu CC-NTrack

#The normalization factor (need an extra factor of 100 for momentum, and 0.01
#for theta
normFactor = [100,0.01]

#On the right for momentum, on the left for cos theta.
#legendX = [[0.55,0.85],[0.15,0.45]]
#legendX = [[0.45,0.85],[0.15,0.55]]
legendX = [[0.35,0.85],[0.15,0.65]]

#Where to start and end bin consultation for max and min.
binCon = [[0,1],[1,0]]

#Set up the PDF output file.
c0 = TCanvas()
c0.Print("errorBandPlots_20150313.pdf[")

#Now loop over samples and axes and make the plots.
for si in xrange(0, len(sampleNames)):

    for oi in xrange(0, len(axisLabels)):

        dataPlotName = "data" + str(si) + str(oi)
        nomPlotName = "nomMC" + str(si) + str(oi)
        meanPlotName = "meanMC" + str(si) + str(oi)

        data = firstInput.Get(dataPlotName)
        data.SetLineColor(kBlack)
        data.SetMarkerStyle(20)

        nom1 = firstInput.Get(nomPlotName)
        mean1 = firstInput.Get(meanPlotName)
        mean1.SetFillColor(kBlue-4) #Blue.  Still Dark but a bit lighter so
        #points show up better.
        mean1.SetMarkerColor(kBlue-4)
        mean1.SetLineColor(kBlue-4)

        nom2 = secondInput.Get(nomPlotName)
        mean2 = secondInput.Get(meanPlotName)
        mean2.SetFillColor(kMagenta) #Lighter pink.
        mean2.SetMarkerColor(kMagenta)
        mean2.SetLineColor(kMagenta)

        #Now for all of them, go through and area normalize (divide both the
        #content and the error by the bin width. 
        for bi in xrange(1,data.GetXaxis().GetNbins() + 1):

            binDim = (data.GetXaxis().GetBinUpEdge(bi) -
                    data.GetXaxis().GetBinLowEdge(bi))/normFactor[oi]

            data.SetBinContent(bi,data.GetBinContent(bi)/binDim)
            data.SetBinError(bi,data.GetBinError(bi)/binDim)

            mean1.SetBinContent(bi,mean1.GetBinContent(bi)/binDim)
            mean1.SetBinError(bi,mean1.GetBinError(bi)/binDim)

            mean2.SetBinContent(bi,mean2.GetBinContent(bi)/binDim)
            mean2.SetBinError(bi,mean2.GetBinError(bi)/binDim)


        combinedPlot = THStack("combined",sampleNames[si]+";"+axisLabels[oi][0]+";"+axisLabels[oi][1])
        combinedPlot.Add(mean1,"E2") #Draw error bands.
        combinedPlot.Add(mean2,"E2") #Draw error bands.
        combinedPlot.Add(data,"P0E0") #Draw points and error bars, including empty bins
        
        c1 = TCanvas()
        c1.cd()
        c1.SetTicks(1,1)
        combinedPlot.Draw("NOSTACK")

        #Now, change the axis ranges.
        combinedPlot.GetXaxis().SetRangeUser(xUserRange[si][oi][0],xUserRange[si][oi][1])
        combinedPlot.SetMinimum(0.0)
        combinedPlot.SetMaximum(FindPlotMax(data,mean1,binCon[oi][0],binCon[oi][1]))
        
        if(si == 4 and oi == 0):
            combinedPlot.SetMaximum(combinedPlot.GetMaximum()-6)

        leg = TLegend(legendX[oi][0],0.70,legendX[oi][1],0.85)
        leg.SetFillColor(0)
        leg.AddEntry(data, "Data", "LEP")
        leg.AddEntry(mean2, "MC stat")
        leg.AddEntry(mean1, "MC stat + flux and xsec syst")
       # leg.AddEntry(mean2, "MC stat")
       # leg.AddEntry(mean1, "Detector syst + MC stat")
        leg.Draw()

        #The "preliminary"
        prelim = TPaveText(legendX[oi][0],0.55,legendX[oi][1],0.65, "NBNDC")
        prelim.AddText("PRELIMINARY")
        prelim.SetFillColor(0)
        prelim.SetBorderSize(0)
        prelim.Draw("")



        combinedPlot.GetXaxis().SetTitleSize(0.05)
        combinedPlot.GetXaxis().SetLabelSize(0.045)
        combinedPlot.GetYaxis().SetTitleSize(0.05)
        combinedPlot.GetYaxis().SetLabelSize(0.045)

        c1.Modified()
        c1.Update()
        
        c1.Print("errorBandPlots_20150313.pdf")
        c1.Print(shortNames[si] + "_" + shortObs[oi] + ".pdf")
        c1.Print(shortNames[si] + "_" + shortObs[oi] + ".png")


c0.Print("errorBandPlots_20150313.pdf]")
