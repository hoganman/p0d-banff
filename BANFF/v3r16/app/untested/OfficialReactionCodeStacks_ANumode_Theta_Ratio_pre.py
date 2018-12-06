#!/usr/bin/python
from ROOT import *
from array import array
import sys
import math

#gStyle.SetTitleSize(0.05)
#gStyle.SetPadBottomMargin(0.12)

#Turn off the warnings about the possible memory leaks caused by using the same
#name when projecting histograms.
gROOT.ProcessLine("gErrorIgnoreLevel = kError")

gROOT.SetBatch(kTRUE);

def FindPlotMax(dataPlot,mcPlot):

    dataBinContentPlusError = []
    mcBinContentPlusError = []

    for i in xrange(1, dataPlot.GetNbinsX() + 1):
        dataBinContentPlusError.append(dataPlot.GetBinContent(i) + dataPlot.GetBinError(i))
        mcBinContentPlusError.append(mcPlot.GetBinContent(i) + mcPlot.GetBinError(i))
        

    #Now, return the maximum of those two, plus 10% so that the histogram is
    #definitely big enough
    return max(max(dataBinContentPlusError),max(mcBinContentPlusError))*1.10

def SetT2KStyle(WhichStyle = 3, styleName = "T2K"):
    t2kStyle=TStyle(styleName, "T2K approved plots style");
  
    # -- WhichStyle --
    # 1 = presentation large fonts
    # 2 = presentation small fonts
    # 3 = publication/paper
  
    FontStyle = 22;
    FontSizeLabel = 0.035;
    FontSizeTitle = 0.05;
    YOffsetTitle = 1.3;
  
    if WhichStyle == 1:
        FontStyle = 42;
        FontSizeLabel = 0.05;
        FontSizeTitle = 0.065;
        YOffsetTitle = 1.19;
    elif WhichStyle == 2:
        FontStyle = 42;
        FontSizeLabel = 0.035;
        FontSizeTitle = 0.05;
        YOffsetTitle = 1.6;
    elif WhichStyle == 3:
        FontStyle = 132;
        FontSizeLabel = 0.05;#35
        FontSizeTitle = 0.071;#5
        YOffsetTitle = 0.79;#1.1
  
    # use plain black on white colors
    t2kStyle.SetFrameBorderMode(0);
    t2kStyle.SetCanvasBorderMode(0);
    t2kStyle.SetPadBorderMode(0);
    t2kStyle.SetCanvasBorderSize(0);
    t2kStyle.SetFrameBorderSize(0);
    t2kStyle.SetDrawBorder(0);
    t2kStyle.SetTitleBorderSize(0);
  
    t2kStyle.SetPadColor(0);
    t2kStyle.SetCanvasColor(0);
    t2kStyle.SetStatColor(0);
    t2kStyle.SetFillColor(0);
  
    t2kStyle.SetEndErrorSize(4);
    t2kStyle.SetStripDecimals(kFALSE);
  
    t2kStyle.SetLegendBorderSize(0);
    t2kStyle.SetLegendFont(FontStyle);
  
    # set the paper & margin sizes
    t2kStyle.SetPaperSize(20, 26);
    t2kStyle.SetPadTopMargin(0.05);
    t2kStyle.SetPadBottomMargin(0.15);# 0.15 in official
    t2kStyle.SetPadRightMargin(0.04); # 0.075 -> 0.13 for colz option
    t2kStyle.SetPadLeftMargin(0.11);#to include both large/small font options
  
    # Fonts, sizes, offsets
    t2kStyle.SetTextFont(FontStyle);
    t2kStyle.SetTextSize(0.10);#08
  
    t2kStyle.SetLabelFont(FontStyle, "x");
    t2kStyle.SetLabelFont(FontStyle, "y");
    t2kStyle.SetLabelFont(FontStyle, "z");
    t2kStyle.SetLabelFont(FontStyle, "t");
    t2kStyle.SetLabelSize(FontSizeLabel, "x");
    t2kStyle.SetLabelSize(FontSizeLabel, "y");
    t2kStyle.SetLabelSize(FontSizeLabel, "z");
    t2kStyle.SetLabelOffset(0.015, "x");
    t2kStyle.SetLabelOffset(0.015, "y");
    t2kStyle.SetLabelOffset(0.015, "z");
  
    t2kStyle.SetTitleFont(FontStyle, "x");
    t2kStyle.SetTitleFont(FontStyle, "y");
    t2kStyle.SetTitleFont(FontStyle, "z");
    t2kStyle.SetTitleFont(FontStyle, "t");
    t2kStyle.SetTitleSize(FontSizeTitle, "t");
    t2kStyle.SetTitleSize(FontSizeTitle, "y");
    t2kStyle.SetTitleSize(FontSizeTitle, "x");
    t2kStyle.SetTitleSize(FontSizeTitle, "z");
    t2kStyle.SetTitleOffset(1.1, "x"); #1.2 nominally
    t2kStyle.SetTitleOffset(YOffsetTitle, "y");
    t2kStyle.SetTitleOffset(1.2, "z");
  
    t2kStyle.SetTitleStyle(0);
    t2kStyle.SetTitleFontSize(0.08);#0.08
    t2kStyle.SetTitleFont(FontStyle, "pad");
    t2kStyle.SetTitleBorderSize(0);
    t2kStyle.SetTitleX(0.02);#5
    t2kStyle.SetTitleW(0.7);
  
    # use bold lines and markers
    t2kStyle.SetMarkerStyle(20);
    t2kStyle.SetHistLineWidth(2);
    t2kStyle.SetLineStyleString(2, "[12 12]"); # prescript dashes
  
    # get rid of X error bars and y error bar caps
#    t2kStyle.SetErrorX(0.001);
  
    # do not display any of the standard histogram decorations
    t2kStyle.SetOptTitle(1);
    t2kStyle.SetOptStat(0);
    t2kStyle.SetOptFit(0);
  
    # put tick marks on top and RHS of plots
    t2kStyle.SetPadTickX(1);
    t2kStyle.SetPadTickY(1);
  
    # -- color --
    # functions blue
    t2kStyle.SetFuncColor(600-4);
  
    t2kStyle.SetFillColor(1); # make color fillings (not white)
    # - color setup for 2D -
    # - "cold"/ blue-ish -
    red = array('d',[0.00, 0.09, 0.18, 0.09, 0.00]);
    green = array('d',[0.01, 0.02, 0.39, 0.68, 0.97]);
    blue  = array('d',[0.17, 0.39, 0.62, 0.79, 0.97]);
    # - "warm" red-ish colors -
    #  red = array('d',[1.00, 1.00, 0.25]);
    #  green = array('d',[1.00, 0.00, 0.00]);
    #  blue = array('d',[0.00, 0.00, 0.00]);
  
    stops = array('d',[0.00, 0.02, 0.20, 0.60, 1.00]);
    NRGBs = 5;
    NCont = 255;
  
    TColor.CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    t2kStyle.SetNumberContours(NCont);
  
    # - Rainbow -
    #  t2kStyle.SetPalette(1);  # use the rainbow color set
  
    # -- axis --
    t2kStyle.SetStripDecimals(kFALSE); # don't do 1.0 -> 1
    #  ROOT.TGaxis.SetMaxDigits(3); # doesn't have an effect 
    # no supressed zeroes!
    t2kStyle.SetHistMinimumZero(kTRUE);
  
    return t2kStyle;

t2kStyle = SetT2KStyle()

gROOT.SetStyle("T2K");

inputFile = TFile(sys.argv[1])
dataFile = TFile(sys.argv[2])
dummyPage = TCanvas()
dummyPage.Print("reactionCodeStacksOfficial_Joint2016_anumode_prefit_costheta.pdf[")
#gStyle.SetErrorX(0.0000001)

#Set up the array of sample names.
#Anti-neutrino only.
sampleNames = ["fgd1anucc1tr","fgd1anuccntr","fgd1nucc1tr","fgd1nuccntr","fgd2anucc1tr","fgd2anuccntr","fgd2nucc1tr","fgd2nuccntr"]
fileNameRoots = ["fgd1anucc1tr","fgd1anuccntr","fgd1nucc1tr","fgd1nuccntr","fgd2anucc1tr","fgd2anuccntr","fgd2nucc1tr","fgd2nuccntr"]
#sampleNames = ["anuccqe","anuccnqe","nuccqe","nuccnqe"]
#fileNameRoots = ["acc1tr","accntr","cc1tr","ccntr"]
#stackHistos = ["CCQE","MEC", "CCRES", "CCCOH", "CCOTHER", "NCRES", "NCCOH", "NCOTHER"
#        ,"ANUCCQE", "ANUMEC", "ANUCCRES", "ANUCCCOH", "ANUCCOTHER", "ANUNCRES"
#        , "ANUNCCOH", "ANUNCOTHER"]

#stackHistos = ["#nu CCQE","#nu CC 2p-2h", "#nu CC Res 1#pi", "#nu CC Coh 1#pi",
#"#nu CC Other", "#nu NC Res 1#pi", "#nu NC Coh 1#pi", "#nu NC Other"
#,"#bar{#nu} CCQE", "#bar{#nu} CC 2p-2h", "#bar{#nu} CC Res 1#pi", "#bar{#nu} CC Coh 1#pi", "#bar{#nu} CC Other", "#bar{#nu} NC Res 1#pi", "#bar{#nu} NC Coh 1#pi", "#bar{#nu} NC Other"]

stackHistos = ["#nu CCQE","#nu non-CCQE", "#bar{#nu} CCQE","#bar{#nu} non-CCQE"]


latexTitles = ["#splitline{FGD 1 #bar{#nu}_{#mu}}{CC-1Track (NF)}","#splitline{FGD 1 #bar{#nu}_{#mu}}{CC-NTracks (NF)}","#splitline{FGD 1 #nu_{#mu}}{CC-1Track (NF)}","#splitline{FGD 1 #nu_{#mu}}{CC-NTracks (NF)}","#splitline{FGD 2 #bar{#nu}_{#mu}}{CC-1Track (NF)}","#splitline{FGD 2 #bar{#nu}_{#mu}}{CC-NTracks (NF)}","#splitline{FGD 2 #nu_{#mu}}{CC-1Track (NF)}","#splitline{FGD 2 #nu_{#mu}}{CC-NTracks (NF)}"]


#nu and nubar MEC never show up together, so can use the same colour.
#stackColours = [kBlue, kMagenta, kRed, kOrange + 3, kCyan, kRed-6,
#        kViolet+4, kMagenta, kRed, kOrange + 7, kCyan+3, kRed-1]

#stackColours = [kBlue, kMagenta, kRed, kCyan, kOrange + 1, kGray + 1,
#        kViolet + 4, kMagenta + 3, kRed + 3, kCyan + 3, kOrange+4, kGray+3]

#col1 = TColor(1001,136.0/255.0,204.0/255.0,238.0/255.0,"col1")

#gROOT.GetListOfColors().Print()
sron1 = TColor(1300,51.0/255.0,34.0/255.0,136.0/255.0, "sron1")
sron2 = TColor(1301,102.0/255.0,153.0/255.0,204.0/255.0, "sron2")
sron3 = TColor(1302,136.0/255.0,204.0/255.0,238.0/255.0, "sron3")
sron4 = TColor(1303,68.0/255.0,170.0/255.0,153.0/255.0, "sron4")
sron5 = TColor(1304,17.0/255.0,119.0/255.0,51.0/255.0, "sron5")
sron6 = TColor(1305,153.0/255.0,153.0/255.0,51.0/255.0, "sron6")
sron7 = TColor(1306,221.0/255.0,204.0/255.0,119.0/255.0, "sron7")
sron8 = TColor(1307,102.0/255.0,17.0/255.0,0.0/255.0, "sron8")
sron9 = TColor(1308,204.0/255.0,102.0/255.0,119.0/255.0, "sron9")
sron10 = TColor(1309,170.0/255.0,68.0/255.0,102.0/255.0, "sron10")
sron11 = TColor(1310,136.0/255.0,34.0/255.0,85.0/255.0, "sron11")
sron12 = TColor(1311,170.0/255.0,68.0/255.0,153.0/255.0, "sron12")

sron71 = TColor(1400,68.0/255.0,119.0/255.0,170.0/255.0,"sron71")
sron72 = TColor(1401,68.0/255.0,170.0/255.0,170.0/255.0,"sron72")
sron73 = TColor(1402,68.0/255.0,170.0/255.0,119.0/255.0,"sron73")
sron74 = TColor(1403,170.0/255.0,170.0/255.0,68.0/255.0,"sron74")
sron75 = TColor(1404,170.0/255.0,119.0/255.0,68.0/255.0,"sron75")
sron76 = TColor(1405,170.0/255.0,68.0/255.0,85.0/255.0,"sron76")
sron77 = TColor(1406,170.0/255.0,68.0/255.0,136.0/255.0,"sron77")

gStyle.SetTitleBorderSize(0)

gStyle.SetTitleX(0.02)
gStyle.SetTitleY(0.95)
gStyle.SetTitleW(0.3)#2
gStyle.SetTitleH(0.07)#5

#gStyle.SetTitleFontSize(0.05)

#Align left, centered in y)
gStyle.SetTitleAlign(11)

stackColours = [1300,1302,1304,1308]

stackFillStyle = [1001,1001,1001,1001]


#Takes an index, converts to a reaction code, and returns the corresponding
#index in the stackHistos (and hence histosToStack) list to add this
#histogram's content to.
def CorrespondingIndex(i):

    #First subtract 100 to get a reaction code.
    i = i -100

    if i == 1:
        return 0

    elif (i > 1) and (i < 100):
        return 1

    elif i == -1:
        return 2

    elif (i < -1) and (i > -100):
        return 3
    else:
        print "Bad value: " + str(i)


#The graphs will be of the form <sampleName>_rxnPredMC_<rxnCode+100>
#So for each sample, we'll loop from 0-200, and if the THnD is not NULL when we
#try and get it, add it to a list for stacking.
#for sampleName in sampleNames:
for ii in xrange(0,len(sampleNames)):
    sampleName = sampleNames[ii]

    #Total the number of events for this sample going into the histogram.
    #(Remember: Sand is excluded here.)
    mcSampleTotal = 0.0

    #Now make the set of histograms to show, which combine several interaction
    #types.
    #Load in the data histogram and use it to assemble the TH2Ds for the MC, as
    #well as making it one of the histograms on the stack.
    dataHist = dataFile.Get(sampleName + "_data").Projection(1,0)

    dataMomProj = dataFile.Get(sampleName + "_data").Projection(1,0).ProjectionY("dataratio" + sampleName + "_px",1,dataHist.GetNbinsX()) 
    dataMomProj.SetDirectory(0)
    dataMomProj.SetTitle("")
   
    prefitMomProj = inputFile.Get(sampleName + "_prefit").Projection(1,0).ProjectionY("prefitratio" + sampleName + "_px",1,dataHist.GetNbinsX())
    prefitMomProj.SetDirectory(0)

    postfitMomProj = inputFile.Get(sampleName + "_postfit_0_0").Projection(1,0).ProjectionY("postfitratio" + sampleName + "_px",1,dataHist.GetNbinsX())
    postfitMomProj.SetDirectory(0)

    for ibx in xrange(1, dataMomProj.GetNbinsX() + 1):
        dataMomProj.SetBinError(ibx,ROOT.Math.sqrt(dataMomProj.GetBinContent(ibx))/prefitMomProj.GetBinContent(ibx))
        print "Data = "+str(dataMomProj.GetBinContent(ibx))+"  MC = "+str(prefitMomProj.GetBinContent(ibx))+" Ratio = "+str(dataMomProj.GetBinContent(ibx)/prefitMomProj.GetBinContent(ibx))
        dataMomProj.SetBinContent(ibx,dataMomProj.GetBinContent(ibx)/prefitMomProj.GetBinContent(ibx))

    dataMomProj.GetYaxis().SetRangeUser(0.75,1.25)
    if ii%4 == 0:
        dataMomProj.GetXaxis().SetRangeUser(0.6,1.0)
    elif ii%4 == 1:
         dataMomProj.GetXaxis().SetRangeUser(0.75,1.0)
    elif ii%4 == 2:
         dataMomProj.GetXaxis().SetRangeUser(0.7,1.0)
    elif ii%4 == 3:
         dataMomProj.GetXaxis().SetRangeUser(0.7,1.0)  
               
    dataMomProj.GetXaxis().SetTitle("cos(#theta_{#mu})")
    dataMomProj.GetYaxis().SetTitle("Data / Sim.")


    #There is only one data plot for each stack, so:
    #Momentum projection
    #Theta slices.
    #Assemble them here.
    dataOneDimPlots = []
    dataOneDimPlots.append(dataHist.ProjectionY("data" + sampleName + "_px",1,dataHist.GetNbinsX()))
    dataNXBins = dataHist.GetYaxis().GetNbins()
    dataxarray = dataHist.GetYaxis().GetXbins().GetArray()

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
                dataOneDimHist.GetXaxis().GetBinLowEdge(ibx))/0.01
            dataOneDimHist.SetBinError(ibx,
                    sqrt(dataOneDimHist.GetBinContent(ibx))/mpdim)
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

        oneDimPlotsThisReac.append(histosToStack[i].ProjectionY(nameBase + "_px",1,histosToStack[i].GetNbinsX()))
       
        stackTitles.append("cos(#theta) projection")
        #Area normalize this now.
        NXBins = histosToStack[i].GetYaxis().GetNbins()
        xarray = histosToStack[i].GetYaxis().GetXbins().GetArray()

        for ibx in xrange(1, NXBins + 1):
            mpdim = (histosToStack[i].GetYaxis().GetBinUpEdge(ibx) -
                histosToStack[i].GetYaxis().GetBinLowEdge(ibx))/0.01
            oneDimPlotsThisReac[0].SetBinContent(ibx,
                    oneDimPlotsThisReac[0].GetBinContent(ibx)/mpdim)
            


        mcSampleTotal += oneDimPlotsThisReac[0].GetSumOfWeights()
        #Now loop over all the theta bins, and make the theta slice plots.
#        for iby in xrange(1, histosToStack[i].GetNbinsY() + 1):
#
#            thetaLow = histosToStack[i].GetYaxis().GetBinLowEdge(iby)
#            thetaHigh = histosToStack[i].GetYaxis().GetBinUpEdge(iby)
#
#            thetaString = " < cos#theta < "
#            thetaString = str(thetaLow) + thetaString + str(thetaHigh)
#            stackTitles.append(thetaString)
#            oneDimPlotsThisReac.append(TH1D("slice_" + nameBase +
#                str(iby),thetaString, NXBins, xarray))
#
#            #Now loop through the x bins for this y bin, and make the 1D
#            #histogram's xbin content equal to that.
#            for ibx in xrange(1, histosToStack[i].GetNbinsX() + 1):
#
#                #Get the bin dimensions for dividing purposes to area
#                #normalize.
#                mpdim = (histosToStack[i].GetXaxis().GetBinUpEdge(ibx) -
#                    histosToStack[i].GetXaxis().GetBinLowEdge(ibx))/100.0
#                oneDimPlotsThisReac[len(oneDimPlotsThisReac)-1].SetBinContent(ibx,
#                        histosToStack[i].GetBinContent(ibx,iby)/mpdim)

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
        sampleStack =  THStack("mpStack_" + sampleName + str(odi)
                ,";cos(#theta_{#mu});Events/(0.01 cos(#theta))")

        leg = TLegend(0.15,0.2,0.35,0.93)
        leg.SetFillColor(0)

        #Make data the first entry.
        leg.AddEntry(dataOneDimPlots[odi],"Data", "LEP")
        dataOneDimPlots[odi].SetMarkerStyle(20)
        dataOneDimPlots[odi].SetMarkerColor(kBlack)
        dataOneDimPlots[odi].SetLineColor(kBlack)

        for odj in xrange(0, len(oneDimPlots)):
            
            #Apply the appropriate plot options from the beginning.
            oneDimPlots[odj][odi].SetFillColor(stackColours[odj])
            oneDimPlots[odj][odi].SetFillStyle(stackFillStyle[odj])
            oneDimPlots[odj][odi].SetMarkerStyle(1) #Don't want T2KStyle to interfere.
            sampleStack.Add(oneDimPlots[odj][odi])
            print oneDimPlots[odj][odi].GetTitle() + " " + str(oneDimPlots[odj][odi].GetSumOfWeights())

            #Only add it to the legend if more than 1% of the events are that
            #type.
            if true:
            #if(oneDimPlots[odj][odi].GetSumOfWeights() >= 0.005*mcSampleTotal):
                leg.AddEntry(oneDimPlots[odj][odi],stackHistos[odj],"F")


        c1 = TCanvas()

        upperPad = TPad("upperPad", "upperPad", .05, .35, .96, .95);
        lowerPad = TPad("lowerPad", "lowerPad", .05, .1, .96, .35);

        upperPad.Draw()
        lowerPad.Draw()

        upperPad.cd()

        ROOT.gPad.SetBottomMargin(1e-5)

        c1.SetTicks(1,1)
        sampleStack.Draw("")

        dataOneDimPlots[odi].Draw("PEsame")
        #sampleStack.SetMaximum(max(sampleStack.GetMaximum(), dataOneDimPlots[odi].GetMaximum()))
        sampleStack.SetMaximum(max(sampleStack.GetMaximum(),FindPlotMax(dataOneDimPlots[odi],dataOneDimPlots[odi])))
        leg.Draw()
        if ii%4 == 0:
            sampleStack.GetXaxis().SetRangeUser(0.6,1.0)
        elif ii%4 == 1:
             sampleStack.GetXaxis().SetRangeUser(0.75,1.0)
        elif ii%4 == 2:
             sampleStack.GetXaxis().SetRangeUser(0.7,1.0)
        elif ii%4 == 3:
             sampleStack.GetXaxis().SetRangeUser(0.7,1.0)  
        sampleStack.GetXaxis().SetTitle("")
        sampleStack.GetXaxis().SetTickLength(0)
        sampleStack.GetXaxis().SetLabelSize(0)
        sampleStack.SetTitle("#bar{#nu}-mode")

        lowerPad.cd()

#        lowerPad.SetFillStyle(4000)
#        lowerPad.SetFillColor(0)
#        lowerPad.SetFrameFillStyle(4000)

        ROOT.gPad.SetTopMargin(1e-5)
        ROOT.gPad.SetBottomMargin(0.35)
        ROOT.gPad.SetTickx()
        dataMomProj.SetLineColor(2) 
        dataMomProj.SetMarkerColor(2) 
        dataMomProj.GetYaxis().SetNdivisions(8,1)
        dataMomProj.GetYaxis().SetLabelSize(dataMomProj.GetYaxis().GetLabelSize()+0.07)
        dataMomProj.GetYaxis().SetTitleSize(dataMomProj.GetYaxis().GetLabelSize()+0.05)
        dataMomProj.GetYaxis().SetTitleOffset(0.335)
        dataMomProj.GetXaxis().SetLabelSize(dataMomProj.GetXaxis().GetLabelSize()+0.08)
        dataMomProj.GetXaxis().SetTitleSize(dataMomProj.GetXaxis().GetLabelSize()+0.04)
        dataMomProj.GetXaxis().SetTitleOffset(0.9)
        dataMomProj.GetXaxis().SetTickLength(0.04)
        dataMomProj.Draw("PE") 
        if ii%4 == 0:
            dataMomProj.GetXaxis().SetRangeUser(0.6,1.0)
            line = TLine(0.6,1.0,1.0,1.0)
            line.SetLineWidth(2)
            line.SetLineColor(1)
            line.Draw("SAME")
        elif ii%4 == 1:
             dataMomProj.GetXaxis().SetRangeUser(0.75,1.0)
             line = TLine(0.75,1.0,1.0,1.0)
             line.SetLineWidth(2)
             line.SetLineColor(1)
             line.Draw("SAME")
        elif ii%4 == 2:
             dataMomProj.GetXaxis().SetRangeUser(0.7,1.0)
             line = TLine(0.7,1.0,1.0,1.0)
             line.SetLineWidth(2)
             line.SetLineColor(1)
             line.Draw("SAME")
        elif ii%4 == 3:
             dataMomProj.GetXaxis().SetRangeUser(0.7,1.0)
             line = TLine(0.7,1.0,1.0,1.0)
             line.SetLineWidth(2)
             line.SetLineColor(1)
             line.Draw("SAME") 

        #sampleStack.GetYaxis().SetTitleOffset(1.2)
        #The "preliminary"
        #The "preliminary"
#        prelim = TPaveText(0.44,0.75,0.74,0.85, "NBNDC")
#        prelim.SetTextFont(132)
#        prelim.SetTextSize(0.06)        
#        prelim.AddText(latexTitles[ii])
#        prelim.SetFillColor(0)
#        prelim.SetBorderSize(0)
#        prelim.Draw("")

        c1.Modified()
        c1.Update()
        c1.Print("reactionCodeStacksOfficial_Joint2016_anumode_prefit_costheta.pdf")
        outFileName = fileNameRoots[sampleNames.index(sampleName)] + "_mucostheta_rxn_prefit"
        c1.Print(outFileName + ".pdf")
        c1.Print(outFileName + ".png")
        c1.Print(outFileName + ".eps")

dummyPage.Print("reactionCodeStacksOfficial_Joint2016_anumode_prefit_costheta.pdf]")
