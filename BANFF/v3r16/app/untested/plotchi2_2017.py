#!/usr/bin/python
import glob
from ROOT import *
from array import array
import sys
import math

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
        FontSizeLabel = 0.035;
        FontSizeTitle = 0.05;
        YOffsetTitle = 1.1;
  
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
    t2kStyle.SetTextSize(0.08);
  
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
    t2kStyle.SetTitleSize(FontSizeTitle, "y");
    t2kStyle.SetTitleSize(FontSizeTitle, "x");
    t2kStyle.SetTitleSize(FontSizeTitle, "z");
    t2kStyle.SetTitleOffset(1.2, "x");
    t2kStyle.SetTitleOffset(YOffsetTitle, "y");
    t2kStyle.SetTitleOffset(1.2, "z");
  
    t2kStyle.SetTitleStyle(0);
    t2kStyle.SetTitleFontSize(0.06);#0.08
    t2kStyle.SetTitleFont(FontStyle, "pad");
    t2kStyle.SetTitleBorderSize(0);
    t2kStyle.SetTitleX(0.1);
    t2kStyle.SetTitleW(0.8);
  
    # use bold lines and markers
    t2kStyle.SetMarkerStyle(20);
    t2kStyle.SetHistLineWidth(2);
    t2kStyle.SetLineStyleString(2, "[12 12]"); # postscript dashes
  
    # get rid of X error bars and y error bar caps
#    t2kStyle.SetErrorX(0.001);
  
    # do not display any of the standard histogram decorations
    t2kStyle.SetOptTitle(0);
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

gROOT.SetBatch(kTRUE);

filelist = glob.glob("/scratch/t/tanaka/T2K/simbien/BANFF_Output/170630_all/good/ToyFits_Asimov_20170628*.root")
filelist2 = glob.glob("/scratch/t/tanaka/T2K/simbien/BANFF_Output/170630_all/good/ToyFits_Asimov_20170628*.root")

outFile = TFile("chi2File_500Toys_201705.root","RECREATE")

#Create the PDF output file.
c1 = TCanvas()
c1.Print("chi2File_500Toys_201705.pdf[")

plotNames = ["total", "samples", "fgd1cc0pi", "fgd1cc1pi", "fgd1ccOth", "fgd1anuccqe",
"fgd1anuccnqe", "fgd1nuccqe", "fgd1nuccnqe","fgd2cc0pi", "fgd2cc1pi", "fgd2ccOth", "fgd2anuccqe",
"fgd2anuccnqe", "fgd2nuccqe", "fgd2nuccnqe", "prior", "flux", "xsec", "obsNorm"]

#[NBins,min, max]
plotLim = [[20,1400.0,3000.0], [30,1300.0,2800.0], [25,50.0,300.0], [25,50.0,300.0],
        [20,75.0,500.0], [30,0.0,300.0], [30,0.0,300.0],
        [30,0.0,300.0],[30,0.0,300.0],[30,50.0,300.0], [30,50.0,300.0],
        [30,5.0,400.0], [30,0.0,300.0], [30,0.0,300.0],
        [30,0.0,300.0],[30,0.0,300.0],[30,75.0,350.0],[30,0.0,60.0],[30,0.0,60.0],[40,0.0,300.0]]


#dataFitResults = [743.89, 697.30, 216.71, 167.96, 233.94, 11.315, 11.165,
#        26.33, 29.87, 46.59, 9.51, 7.66, 29.42]

#dataFitResults = [746.628,700.068,220.158,171.658,229.643,12.2696,11.301,
#        25.355,29.6832,46.5596,9.80448,7.55803,29.1971]

#The buggy fit result
#dataFitResults = [755.664,699.253,211.816,181.591,233.381,13.7872,14.0395,
#        22.4843,22.154,56.4111,12.5145,14.9064,28.9902]

#The current best fit.
#dataFitResults2 = [769.402,709.574,221.772,181.763,234.135,13.7862,14.124,
       # 22.1124,21.8817,59.8279,12.4922,17.7499,29.5858]
        
dataFitResults2 = [1895.37,1721.12,169.971,164.048,223.485,117.788,74.9824,66.9917,61.9967,164.399,162.619,168.82,129.103,78.9505,62.7941,75.1763,174.244,8.54581,11.3726,145.814]

vectorNames =["FittedMinimizedQuantity0", "FittedTotalSampleContribs0",
"FittedIndividualSampleContribs_0_0","FittedIndividualSampleContribs_0_0",
"FittedIndividualSampleContribs_0_0","FittedIndividualSampleContribs_0_0",
"FittedIndividualSampleContribs_0_0","FittedIndividualSampleContribs_0_0",
"FittedIndividualSampleContribs_0_0","FittedIndividualSampleContribs_0_0","FittedIndividualSampleContribs_0_0",
"FittedIndividualSampleContribs_0_0","FittedIndividualSampleContribs_0_0",
"FittedIndividualSampleContribs_0_0","FittedIndividualSampleContribs_0_0",
"FittedIndividualSampleContribs_0_0","FittedPriorTerm0","FittedPriorTerm0","FittedPriorTerm0","FittedPriorTerm0"]
vectorIndices = [0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,3,0,1,2]

outHists = []
outHists2 = []
for i in xrange(0,len(plotNames)):
    outHists.append(TH1D(plotNames[i],plotNames[i] +";#Delta#chi^{2};Number of toy experiments",plotLim[i][0],plotLim[i][1],plotLim[i][2]))
    outHists2.append(TH1D(plotNames[i]+str(2),plotNames[i] +";#Delta#chi^{2};Number of toy experiments",plotLim[i][0],plotLim[i][1],plotLim[i][2]))

TotalFits = 0
FitsWithChi2GTData = 0


#Loop through all the files in the filelist.
for fidx in xrange(0, len(filelist)):


    TotalFits += 1 

    #Otherwise, load the file.
    curr = TFile(filelist[fidx])
    curr2 = TFile(filelist2[fidx])

    #Now load in the vector for each plot.
    vectors = []
    vectors2 = []
    
    for vect in vectorNames:
        vectors.append(curr.Get(vect))
        vectors2.append(curr2.Get(vect))

    #Now, fill each histogram
    for i in xrange(0, len(outHists)):

        relevantVect = vectors[i]
        relevantVect2 = vectors2[i]
        outHists[i].Fill(relevantVect[vectorIndices[i]])
        outHists2[i].Fill(relevantVect2[vectorIndices[i]])

        #If the total value being added to the distribution is greater than the data
        #value, count it.
        if(i == 0):

            if(relevantVect[vectorIndices[i]] > dataFitResults2[i]):

                FitsWithChi2GTData += 1

#Can now print out the p value.
print "Total number of fits: " + str(TotalFits)
print "Number of fits with greater chi2 than data: " + str(FitsWithChi2GTData)
print "p-value = " + str(float(FitsWithChi2GTData)/float(TotalFits))


#Now write out to PDF:
for i in xrange(0, len(outHists)):

    c1.cd()

    outHists[i].SetLineWidth(3)
    outHists[i].SetLineColor(kBlack)
    outHists[i].Draw()
    c1.Modified()
    c1.Update()

#    st = outHists[i].FindObject("stats")
#    st.SetX1NDC(0.7)
#    st.SetX2NDC(0.9)
#    st.SetY1NDC(0.7)
#    st.SetY2NDC(0.9)

    c1.Modified()
    c1.Update()
    
    #Add a TLine in red from yMin to yMax at x = data fit result.
    #Unconstrained fit with slightly different prior in blue.
    dataLine2 = TLine(dataFitResults2[i],c1.GetUymin(),dataFitResults2[i],c1.GetUymax())
    dataLine2.SetLineWidth(3)
    dataLine2.SetLineColor(kRed)
    dataLine2.Draw("same")
    c1.Modified()
    c1.Update()

    leg = TLegend(0.58,0.71,0.93,0.91)
    leg.SetFillColor(0)
    leg.AddEntry(outHists[i], "Expected distribution","L")
    leg.AddEntry(dataLine2,"Data value", "L")
    leg.Draw()

    outHists[i].GetYaxis().SetTitleOffset(0.97)
    outHists[i].GetXaxis().SetTitleSize(0.045)
    outHists[i].GetYaxis().SetTitleSize(0.045)

    c1.Modified()
    c1.Update()

    name = plotNames[i];
    name += "500Toys_201705.eps"
    c1.Print(name);   
    name2 = plotNames[i];
    name2 += "500Toys_201705.png"
    c1.Print(name2);   
    name3 = plotNames[i];
    name3 += "500Toys_201705.pdf"
    c1.Print(name3);   
    c1.Print("chi2File_500Toys_201705.pdf")   


    full_integral = outHists[i].Integral()
    outHists[i].GetXaxis().SetRangeUser(dataFitResults2[i], outHists[i].GetXaxis().GetXmax())
    partial_integral = outHists[i].Integral()
    print "p-value " + plotNames[i] + " = " + str(partial_integral/full_integral)


#Close the PDF file.
c1.Print("chi2File_500Toys_201705.pdf]")

#With all histograms filled, write them to the ROOT file for now.
outFile.Write()
outFile.Close()