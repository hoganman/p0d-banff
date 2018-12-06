#!/usr/bin/python
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
    t2kStyle.SetPadTopMargin(0.1);
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
    t2kStyle.SetTitleOffset(1.14, "x");
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

gStyle.SetOptStat(0)
#gStyle.SetPadBottomMargin(0.1)
#gStyle.SetLabelSize(0.045)
#gStyle.SetTitleSize(0.045,"x")
gStyle.SetErrorX()
gStyle.SetPadRightMargin(0.1)

#Get rid of title border.
gStyle.SetTitleBorderSize(0)

gStyle.SetTitleX(0.06)
gStyle.SetTitleY(0.925)
gStyle.SetTitleW(0.4)
gStyle.SetTitleH(0.07)

#gStyle.SetTitleFontSize(0.05)

#Align left, centered in y)
gStyle.SetTitleAlign(11)


#Useage is python MakeParameterPlots.py postfitFileWithAllParameters outputFileName
inFile = TFile(sys.argv[1])

prefit_params = inFile.Get("prefit_params")
prefit_cov = inFile.Get("prefit_cov")
prefit_err = TVectorD(prefit_params.GetNrows())

postfit_params = inFile.Get("postfit_params")
postfit_cov = inFile.Get("postfit_cov")
postfit_err = TVectorD(postfit_params.GetNrows())
param_list = inFile.Get("param_list")

for i in xrange(0, prefit_err.GetNrows()):

    prefit_err[i] = TMath.Sqrt(prefit_cov[i][i])
    postfit_err[i] = TMath.Sqrt(postfit_cov[i][i])

#We just need to make one TH1D for each, then just change the range as needed
#to get the desired parameter subset.
#prefitHist = TH1D(prefit_params)
#postfitHist = TH1D(postfit_params)
#
#for i in xrange(1, prefit_err.GetNrows() + 1):
#
#    prefitHist.SetBinError(i,prefit_err[i-1])
#    postfitHist.SetBinError(i,postfit_err[i-1])

#histos = ["ND280 #nu_{#mu}, FHC",
#        "ND280 #bar{#nu}_{#mu}, FHC",
#        "ND280 #nu_{e}, FHC",
#        "ND280 #bar{#nu}_{e}, FHC",
#        "ND280 #nu_{#mu}, RHC",
#        "ND280 #bar{#nu}_{#mu}, RHC",
#        "ND280 #nu_{e}, RHC",
#        "ND280 #bar{#nu}_{e}, RHC",
#        "SK #nu_{#mu}, FHC",
#        "SK #bar{#nu}_{#mu}, FHC",
#        "SK #nu_{e}, FHC",
#        "SK #bar{#nu}_{e}, FHC",
#        "SK #nu_{#mu}, RHC",
#        "SK #bar{#nu}_{#mu}, RHC",
#        "SK #nu_{e}, RHC",
#        "SK #bar{#nu}_{e}, RHC"]


histos = ["ND280 #nu_{#mu}, #nu-mode",
        "ND280 #bar{#nu}_{#mu}, #nu-mode",
        "ND280 #nu_{e}, #nu-mode",
        "ND280 #bar{#nu}_{e}, #nu-mode",
        "ND280 #nu_{#mu}, #bar{#nu}-mode",
        "ND280 #bar{#nu}_{#mu}, #bar{#nu}-mode",
        "ND280 #nu_{e}, #bar{#nu}-mode",
        "ND280 #bar{#nu}_{e}, #bar{#nu}-mode",
        "SK #nu_{#mu}, #nu-mode",
        "SK #bar{#nu}_{#mu}, #nu-mode",
        "SK #nu_{e}, #nu-mode",
        "SK #bar{#nu}_{e}, #nu-mode",
        "SK #nu_{#mu}, #bar{#nu}-mode",
        "SK #bar{#nu}_{#mu}, #bar{#nu}-mode",
        "SK #nu_{e}, #bar{#nu}-mode",
        "SK #bar{#nu}_{e}, #bar{#nu}-mode"]

#histos = ["ND280 #nu_{#mu}, neutrino mode",
#        "ND280 #bar{#nu}_{#mu}, neutrino mode",
#        "ND280 #nu_{e}, neutrino mode",
#        "ND280 #bar{#nu}_{e}, neutrino mode",
#        "ND280 #nu_{#mu}, antineutrino mode",
#        "ND280 #bar{#nu}_{#mu}, antineutrino mode",
#        "ND280 #nu_{e}, antineutrino mode",
#        "ND280 #bar{#nu}_{e}, antineutrino mode",
#        "SK #nu_{#mu}, neutrino mode",
#        "SK #bar{#nu}_{#mu}, neutrino mode",
#        "SK #nu_{e}, neutrino mode",
#        "SK #bar{#nu}_{e}, neutrino mode",
#        "SK #nu_{#mu}, antineutrino mode",
#        "SK #bar{#nu}_{#mu}, antineutrino mode",
#        "SK #nu_{e}, antineutrino mode",
#        "SK #bar{#nu}_{e}, antineutrino mode"]

plotFileNames = ["nd_pf_numu_flux_parms",
        "nd_pf_numub_flux_parms",
        "nd_pf_nue_flux_parms",
        "nd_pf_nueb_flux_parms",
        "nd_nf_numu_flux_parms",
        "nd_nf_numub_flux_parms",
        "nd_nf_nue_flux_parms",
        "nd_nf_nueb_flux_parms",
        "sk_pf_numu_flux_parms",
        "sk_pf_numub_flux_parms",
        "sk_pf_nue_flux_parms",
        "sk_pf_nueb_flux_parms",
        "sk_nf_numu_flux_parms",
        "sk_nf_numub_flux_parms",
        "sk_nf_nue_flux_parms",
        "sk_nf_nueb_flux_parms"]


axisNames = ["nd5_numode_numu_bins",
            "nd5_numode_numub_bins",
            "nd5_numode_nue_bins",
            "nd5_numode_nueb_bins",
            "nd5_anumode_numu_bins",
            "nd5_anumode_numub_bins",
            "nd5_anumode_nue_bins",
            "nd5_anumode_nueb_bins",
            "sk_numode_numu_bins",
            "sk_numode_numub_bins",
            "sk_numode_nue_bins",
            "sk_numode_nueb_bins",
            "sk_anumode_numu_bins",
            "sk_anumode_numub_bins",
            "sk_anumode_nue_bins",
            "sk_anumode_nueb_bins"]

axes = []

for i in xrange(0,len(axisNames)):
    axes.append(inFile.Get(axisNames[i]))


startingParamIndex = 0
c1 = TCanvas()
c1.cd()
c1.SetTicks(1,1)
c1.Print(sys.argv[2] + "[")

for i in xrange(0,len(histos)):

    #Pull out the array first.  We want to change the first entry from 0 to
    #0.1, so the axis ranges work out when we want to draw histograms.
    xarray = axes[i].GetXbins().GetArray()
    xarray[0] = 0.1

    prefitHist = TH1D(axisNames[i]+"prefit",histos[i],axes[i].GetNbins(),xarray)
    postfitHist = TH1D(axisNames[i]+"postfit",histos[i],axes[i].GetNbins(),xarray)
    
    #Prefit is solid red fill, red point that isn't seen.
    prefitHist.SetFillStyle(1001)
    prefitHist.SetFillColor(2)
    #prefitHist.SetMarkerStyle(21)
    #prefitHist.SetMarkerColor(2)
    prefitHist.SetMarkerStyle(1)
    prefitHist.SetMarkerColor(2)
    prefitHist.SetLineColor(1)
    prefitHist.SetLineWidth(2)

    #Postfit is hatched blue background, with blue squares
    postfitHist.SetFillStyle(3244)
    postfitHist.SetFillColor(4)
    postfitHist.SetMarkerColor(4)
    postfitHist.SetMarkerStyle(21)
    postfitHist.SetLineColor(4)

    #Now, clone the prefitHist, because we need to plot it on top of itself
    #as a line, otherwise the line doesn't show.
    prefitHistClone = prefitHist.Clone()

    for j in xrange(0,axes[i].GetNbins()):

        prefitHist.SetBinContent(j+1,prefit_params[startingParamIndex + j])
        prefitHist.SetBinError(j+1,prefit_err[startingParamIndex +j])

        print str(prefitHist.GetXaxis().GetBinLowEdge(j+1)) + " " + str(prefitHist.GetXaxis().GetBinUpEdge(j+1))


        #Clone has zero error, since don't want it to have error bars showing.
        prefitHistClone.SetBinContent(j+1,prefit_params[startingParamIndex + j])
        prefitHistClone.SetBinError(j+1,0.0001)

        postfitHist.SetBinContent(j+1,postfit_params[startingParamIndex + j])
        postfitHist.SetBinError(j+1,postfit_err[startingParamIndex +j])

    #prefitHistClone should have a black marker, not a red one.
    prefitHistClone.SetMarkerColor(1)

    #With the histograms filled, increment startingParamIndex by the number of
    #bins in the axis we just considered.
    startingParamIndex += axes[i].GetNbins()


    #Before adding them to the stack, set the limits on each histogram to 0.1
    #to 30.0, as in the original plots.
    #This doesn't quite seem to work...
    #prefitHist.GetXaxis().SetLimits(0.1,30.0)
    #prefitHistClone.GetXaxis().SetLimits(0.1,30.0)
    #postfitHist.GetXaxis().SetLimits(0.1,30.0)
    #Try setting the low edge of the first bin to 0.1 for each.

    #Now, superimpose them and write them out.
    stack = THStack("pStack",histos[i]+";E_{#nu} (GeV);Flux Parameter Value")
    stack.Add(prefitHist,"E2")
    stack.Add(prefitHistClone,"L") 
    stack.Add(postfitHist,"E2P")
     
   
    c1.cd()
    c1.SetLogx()
    stack.Draw("NOSTACK")

    #Set the range to the desired parameters that want (low end to topend + 1)
#    stack.GetXaxis().SetRangeUser(paramRange[i][0],paramRange[i][1] + 1)
    stack.SetMinimum(0.5001)
    stack.SetMaximum(1.4)

    #The "preliminary"
    prelim = TPaveText(0.16,0.82,0.435,0.88, "NBNDC")
    #prelim.AddText("PRELIMINARY")
    prelim.SetFillColor(0)
    prelim.SetBorderSize(0)
    prelim.Draw("")


    leg = TLegend(0.16,0.20,0.85,0.43)
    leg.SetFillColor(0)
    leg.AddEntry(prefitHist, "Prior to ND280 Constraint","FL")
    leg.AddEntry(postfitHist,"After ND280 Constraint", "FEP")
    leg.SetBorderSize(0)
    leg.Draw()
    c1.Modified()
    c1.Update()
    c1.Print(sys.argv[2])
    c1.Print(plotFileNames[i]+".pdf")
    c1.Print(plotFileNames[i]+".eps")
    c1.Print(plotFileNames[i]+".png")


c1.Print(sys.argv[2] + "]")
	 
