'''
MakeParameterPlotsForComparisonMaCh3
Produces plots that compare the BANFF fit results with the MaCH3 fit results
Usage:
    python MakeParameterPlotsForComparisonMaCh3 \
            postfitFileWithAllParameters \ ---> BANFF processed fit file
            mach3PostfitFile             \ ---> MaCH3 fit file
            outputfile                   \ ---> PDF name of output file
'''
#!/usr/bin/python
from ROOT import *
import sys
import math
from numpy import *
from array import array

gStyle.SetOptStat(0)
gStyle.SetLabelSize(0.055)
gStyle.SetPadBottomMargin(0.3)

gROOT.SetBatch(kTRUE)

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
  
    t2kStyle.SetLegendBorderSize(1);
    t2kStyle.SetLegendFont(FontStyle);
  
    # set the paper & margin sizes
    t2kStyle.SetPaperSize(20, 26);
    t2kStyle.SetPadTopMargin(0.1);
    t2kStyle.SetPadBottomMargin(0.33);# 0.15 in official
    t2kStyle.SetPadRightMargin(0.04); # 0.075 -> 0.13 for colz option
    t2kStyle.SetPadLeftMargin(0.11);#to include both large/small font options
  
    # Fonts, sizes, offsets
    t2kStyle.SetTextFont(FontStyle);
    t2kStyle.SetTextSize(0.08);
  
    t2kStyle.SetLabelFont(FontStyle, "x");
    t2kStyle.SetLabelFont(FontStyle, "y");
    t2kStyle.SetLabelFont(FontStyle, "z");
    t2kStyle.SetLabelFont(FontStyle, "t");
    t2kStyle.SetLabelSize(FontSizeLabel*1.45, "x");
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
    #t2kStyle.SetOptTitle(0);
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

# Check number of arguments
if (len(sys.argv) != 4):
    print 'Usage: python MakeParameterPlotsForComparisonMaCh3 \\'
    print '\t postfitFileWithAllParameters \\'
    print '\t mach3PostfitFile \\'
    print '\t outputfile.pdf'
else:
    print 'Processing BANFF and MaCH3 files'

# Load input files
infile1 = TFile(sys.argv[1]) #BANFF
infile2 = TFile(sys.argv[2]) #MaCH3

# Get BANFF postfit parameters, covariance, and errors
postfit_params = infile1.Get('postfit_params')
param_list     = infile1.Get('param_list')
postfit_cov    = infile1.Get('postfit_cov')
postfit_err    = TVectorD(postfit_params.GetNrows())

# Get MaCH3 parameters and errors
mach3_params = infile2.Get('postfit_params_HPD')
mach3_err    = infile2.Get('postfit_errors_HPD')

# Filling the BANFF postfit error vector
for i in xrange(0, postfit_err.GetNrows()):
    postfit_err[i] = math.sqrt(postfit_cov[i][i])

# The number of each type of parameter
NFLUX = 100
NOBS  = 556
NXSEC = 32
NPARAMS = NFLUX + NOBS + NXSEC

# The BANFF parameters are loaded in the order:
#     FLUX, OBSNORM, XSEC
# In order to match MaCH3, this needs to be:
#     FLUX, XSEC, OBSNORM
# Now moving the parameters around
banff_params = TVectorD(NPARAMS)
banff_errors = TVectorD(NPARAMS)
banff_names  = TObjArray(NPARAMS)

for i in range(0, NPARAMS):
    if (i < NFLUX):
        j = i
        banff_params[j] = postfit_params[i]
        banff_errors[j] = postfit_err[i]
        banff_names.AddAt(param_list.At(i), j)
    elif (NFLUX <= i) and (i < NFLUX+NOBS):
        j = i + NXSEC
        banff_params[j] = postfit_params[i]
        banff_errors[j] = postfit_err[i]
        banff_names.AddAt(param_list.At(i), j)
    elif (i >= NFLUX + NOBS):
        j = i - NOBS
        banff_params[j] = postfit_params[i]
        banff_errors[j] = postfit_err[i]
        banff_names.AddAt(param_list.At(i), j)

# Find BANFF parameter locations
# TODO: Add in checks for start/end of Flux and ObsNorm parameters
FEFQE    = -1
BERPA    = -1
CCDIS    = -1
FEFQELOC = -1
MAQELOC  = -1
BERPALOC = -1
CA5LOC   = -1
NCOTHLOC = -1
for i in xrange(0, banff_names.GetEntriesFast()):
    if banff_names[i] == 'FEFQE':
        FEFQELOC = i
        FEFQE    = i+1
    elif banff_names[i] == 'MAQE':
        MAQELOC = i
    elif banff_names[i] == 'BeRPA_A':
        BERPALOC = i
        BERPA    = i+1
    elif banff_names[i] == 'CA5':
        CA5LOC = i
    elif banff_names[i] == 'CC_DIS':
        CCDIS = i+1
    elif banff_names[i] == 'NC_other_far':
        NCOTHLOC = i
    else:
        continue

# We just need to make one TH1D for each, then change the range as needed
# to get the desired parameter subset
banffHist = TH1D(banff_params)
mach3Hist = TH1D(mach3_params)

# Move MaCH3 FSI parameters to match up with BANFF
# Also, fill MaCH3 histogram
for i in xrange(1, mach3_params.GetNrows()+1):
    if (100 < i and i < 106):
        j = i + 27
        mach3Hist.SetBinContent(i, mach3_params[j-1])
        mach3Hist.SetBinError(i, mach3_err[j-1])
    elif (i > 105):
        j = i - 5
        mach3Hist.SetBinContent(i, mach3_params[j-1])
        mach3Hist.SetBinError(i, mach3_err[j-1])
    else:
        mach3Hist.SetBinContent(i, mach3_params[i-1])
        mach3Hist.SetBinError(i, mach3_err[i-1])

# Set BANFF errors
for i in xrange(1, banff_errors.GetNrows()+1):
    banffHist.SetBinError(i, banff_errors[i-1])

# Modify some of the BANFF parameters
# BeRPA parameters are adjusted to be fractional change
#BeRPA_A
banffHist.SetBinContent(BERPA, banffHist.GetBinContent(BERPA)*1.69491525424)
banffHist.SetBinError(BERPA, banffHist.GetBinError(BERPA)*1.69491525424)

#BeRPA_B
banffHist.SetBinContent(BERPA+1, banffHist.GetBinContent(BERPA+1)*0.95238095238)
banffHist.SetBinError(BERPA+1, banffHist.GetBinError(BERPA+1)*0.95238095238)

#BeRPA_D
banffHist.SetBinContent(BERPA+2, banffHist.GetBinContent(BERPA+2)*0.88495575221)
banffHist.SetBinError(BERPA+2, banffHist.GetBinError(BERPA+2)*0.88495575221)

#BeRPA_E
banffHist.SetBinContent(BERPA+3, banffHist.GetBinContent(BERPA+3)*1.13636363636)
banffHist.SetBinError(BERPA+3, banffHist.GetBinError(BERPA+3)*1.13636363636)

#BeRPA_U for both BANFF and MaCH3
# Set MaCH3 BeRPA_U to be equal to BANFF because it's a fixed parameter
banffHist.SetBinContent(BERPA+4, banffHist.GetBinContent(BERPA+4)*0.83333333333)
banffHist.SetBinError(BERPA+4, banffHist.GetBinError(BERPA+4)*0.83333333333)
mach3Hist.SetBinContent(118, banffHist.GetBinContent(BERPA+4))
mach3Hist.SetBinError(118, banffHist.GetBinError(BERPA+4))

# Move parameters centered around 0 to centered around 1
#CCDIS
banffHist.SetBinContent(CCDIS, banffHist.GetBinContent(CCDIS)+1)

#FSI
banffHist.SetBinContent(FEFQE,   banffHist.GetBinContent(FEFQE)   + 1)
banffHist.SetBinContent(FEFQE+1, banffHist.GetBinContent(FEFQE+1) + 1)
banffHist.SetBinContent(FEFQE+2, banffHist.GetBinContent(FEFQE+2) + 1)
banffHist.SetBinContent(FEFQE+3, banffHist.GetBinContent(FEFQE+3) + 1)
banffHist.SetBinContent(FEFQE+4, banffHist.GetBinContent(FEFQE+4) + 1)

# Set histo types, ranges, and bin labels
histoTypes = ["ND280 NuMode Flux", "ND280 ANuMode Flux", "SK NuMode Flux", 
              "SK ANuMode Flux", "FSI", "Cross-section"]
#filename   = ["comp_flux_ND280_NuMode_Flux", "comp_flux_ND280_ANuMode_Flux", 
#              "comp_flux_SK_NuMode_Flux", "comp_flux_SK_ANuMode_Flux", 
#              "comp_xsec_FSI", "comp_xsec_all"]
paramRange = [[0,24],     [25,49],    [50,74],    [75,99],    [FEFQELOC,FEFQELOC+4], [MAQELOC,NCOTHLOC]]
yRange     = [[0.80,1.2], [0.80,1.2], [0.80,1.2], [0.80,1.2], [0.0,2.2],             [0,2.05]]
binLabels  = []

for i in xrange(0, banff_params.GetNrows()):
    binLabels.append("")

binLabels[0]  = "ND280 #nu_{#mu}, FHC"
binLabels[11] = "ND280 #bar{#nu}_{#mu}, FHC"
binLabels[16] = "ND280 #nu_{e}, FHC"
binLabels[23] = "ND280 #bar{#nu}_{e}, FHC"
binLabels[25] = "ND280 #nu_{#mu}, RHC"
binLabels[30] = "ND280 #bar{#nu}_{#mu}, RHC"
binLabels[41] = "ND280 #nu_{e}, RHC"
binLabels[43] = "ND280 #bar{#nu}_{e}, RHC"
binLabels[50] = "SK #nu_{#mu}, FHC"
binLabels[61] = "SK #bar{#nu}_{#mu}, FHC"
binLabels[66] = "SK #nu_{e}, FHC"
binLabels[73] = "SK #bar{#nu}_{e}, FHC"
binLabels[75] = "SK #nu_{#mu}, RHC"
binLabels[80] = "SK #bar{#nu}_{#mu}, RHC"
binLabels[91] = "SK #nu_{e}, RHC"
binLabels[93] = "SK #bar{#nu}_{e}, RHC"

for i in xrange(FEFQELOC,NCOTHLOC+1):
    binLabels[i] = banff_names.At(i).GetString().Data()

# Set the fill types for the MaCH3 and BANFF histograms.  Will plot them as
# points, but this will propagate to the error bars.

#MaCH3 is red fill, red point that isn't seen.
mach3Hist.SetFillColor(kRed)
mach3Hist.SetFillStyle(3002)
mach3Hist.SetLineColor(kRed)
mach3Hist.SetMarkerColor(kRed)
mach3Hist.SetMarkerStyle(7)
mach3Hist.SetMarkerSize(0.5)

#BANFF is with blue squares
banffHist.SetMarkerColor(4)
banffHist.SetMarkerStyle(21)
banffHist.SetMarkerSize(0.5)
banffHist.SetLineWidth(2)

c1 = TCanvas()
c1.cd()
c1.Print(sys.argv[3] + "[")

for i in xrange(0, len(histoTypes)):

    stack = THStack("pStack", histoTypes[i])
    stack.Add(banffHist, "E1")
    stack.Add(mach3Hist, "E2P")
    
    c1.cd()
    stack.Draw("NOSTACK")

    #Set the range to the desired parameters that want (low end to topend + 1)
    stack.GetXaxis().SetRangeUser(paramRange[i][0], paramRange[i][1] + 1)
    stack.SetMinimum(yRange[i][0])
    stack.SetMaximum(yRange[i][1])

    leg = TLegend(0.68,0.84,0.88,0.98)
    leg.SetFillColor(0)
    leg.AddEntry(mach3Hist, "MaCH3", "FEP")
    leg.AddEntry(banffHist, "BANFF", "ELP")
    leg.Draw()

    for j in xrange(paramRange[i][0], paramRange[i][1]+1):
        stack.GetXaxis().SetBinLabel(j+1, binLabels[j])
    
    stack.GetXaxis().LabelsOption("v")

    c1.Modified()
    c1.Update()
    c1.Print(sys.argv[3])
    #c1.Print(filename[i] + ".pdf")
    #c1.Print(filename[i] + ".png")
    #c1.Print(filename[i] + ".eps")

histoTypes = ["SK FHC #nu_{#mu} Flux", "FSI parameters", "CC0#pi parameters", 
              "BeRPA parameters", "CC1#pi parameters"]
paramRange = [[50,60],    [FEFQELOC,FEFQELOC+4], [MAQELOC,MAQELOC+7], [BERPALOC,BERPALOC+4], [CA5LOC,CA5LOC+2]]
yRange     = [[0.80,1.2], [0.0, 2.1],            [0.5,2.1],           [0.5,2.1],             [0.5,1.4]]

binLabels[50] = "0, 400 MeV"
binLabels[51] = "400, 500 Mev"
binLabels[52] = "500, 600 Mev"
binLabels[53] = "600, 700 Mev"
binLabels[54] = "700, 1000 Mev"
binLabels[55] = "1, 1.5 Gev"
binLabels[56] = "1.5, 2 Gev"
binLabels[57] = "2, 3.5 Gev"
binLabels[58] = "3.5, 5 Gev"
binLabels[59] = "5, 7 Gev"
binLabels[60] = "7, 30 Gev"
c1.cd()

for i in xrange(0, len(histoTypes)):

    stack = THStack("pStack", histoTypes[i])
    stack.Add(banffHist,"E1")
    stack.Add(mach3Hist, "E2P")
    
    banffHist.SetMarkerSize(0.5)

    c1.cd()
    stack.Draw("NOSTACK")

    #Set the range to the desired parameters that want (low end to topend + 1)
    stack.GetXaxis().SetRangeUser(paramRange[i][0], paramRange[i][1] + 1)
    stack.SetMinimum(yRange[i][0])
    stack.SetMaximum(yRange[i][1])

    leg = TLegend(0.68,0.84,0.88,0.98)
    leg.SetFillColor(0)
    leg.AddEntry(mach3Hist, "MaCh3", "FEP")
    leg.AddEntry(banffHist, "BANFF", "ELP")
    leg.Draw()

    for j in xrange(paramRange[i][0], paramRange[i][1]+1):
        stack.GetXaxis().SetBinLabel(j+1, binLabels[j])
    
    stack.GetXaxis().LabelsOption("v")

    c1.Modified()
    c1.Update()
    c1.Print(sys.argv[3])

c1.Print(sys.argv[3] + "]")
