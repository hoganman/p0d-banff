#!/usr/bin/python
from ROOT import *
import sys
import math
from numpy import * 
from array import array

gStyle.SetOptStat(0)
gROOT.SetBatch(kTRUE);


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
    t2kStyle.SetPadBottomMargin(0.41);# 0.15 in official
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

#Useage is python MakeParameterPlots.py postfitFileWithAllParameters outputFileName
inFile = TFile(sys.argv[1])

prefit_params = inFile.Get("prefit_params")
prefit_cov = inFile.Get("prefit_cov")
prefit_err = TVectorD(prefit_params.GetNrows())

postfit_params = inFile.Get("postfit_params")
postfit_cov = inFile.Get("postfit_cov")
postfit_err = TVectorD(postfit_params.GetNrows())

for i in xrange(0, prefit_err.GetNrows()):

    prefit_err[i] = math.sqrt(prefit_cov[i][i])
    postfit_err[i] = math.sqrt(postfit_cov[i][i])
    if i > 131:
        prefit_params[i] = prefit_params[i]/prefit_params[i]
        postfit_params[i] = postfit_params[i]/prefit_params[i]
        
#We just need to make one TH1D for each, then just change the range as needed
#to get the desired parameter subset.
prefitHist = TH1D(prefit_params)
postfitHist = TH1D(postfit_params)

for i in xrange(1, prefit_err.GetNrows() + 1):

    prefitHist.SetBinError(i,prefit_err[i-1])
    postfitHist.SetBinError(i,postfit_err[i-1])
    
##CCQE parameters
prefitHist.SetBinError(107,0.0)
prefitHist.SetBinError(108,0.0)
prefitHist.SetBinError(109,0.0)
prefitHist.SetBinError(110,0.0)
prefitHist.SetBinError(111,0.0)
    
#BeRPA_A
prefitHist.SetBinContent(115,prefitHist.GetBinContent(115)*1.69491525424)
postfitHist.SetBinContent(115,postfitHist.GetBinContent(115)*1.69491525424)
prefitHist.SetBinError(115,prefitHist.GetBinError(115)*1.69491525424)
postfitHist.SetBinError(115,postfitHist.GetBinError(115)*1.69491525424)
#BeRPA_B
prefitHist.SetBinContent(116,prefitHist.GetBinContent(116)*0.95238095238)
postfitHist.SetBinContent(116,postfitHist.GetBinContent(116)*0.95238095238)
prefitHist.SetBinError(116,prefitHist.GetBinError(116)*0.95238095238)
postfitHist.SetBinError(116,postfitHist.GetBinError(116)*0.95238095238)
#BeRPA_D
prefitHist.SetBinContent(117,prefitHist.GetBinContent(117)*0.88495575221)
postfitHist.SetBinContent(117,postfitHist.GetBinContent(117)*0.88495575221)
prefitHist.SetBinError(117,prefitHist.GetBinError(117)*0.88495575221)
postfitHist.SetBinError(117,postfitHist.GetBinError(117)*0.88495575221)
#BeRPA_E
prefitHist.SetBinContent(118,prefitHist.GetBinContent(118)*1.13636363636)
postfitHist.SetBinContent(118,postfitHist.GetBinContent(118)*1.13636363636)
prefitHist.SetBinError(118,prefitHist.GetBinError(118)*1.13636363636)
postfitHist.SetBinError(118,postfitHist.GetBinError(118)*1.13636363636)
#BeRPA_U
prefitHist.SetBinContent(119,prefitHist.GetBinContent(119)*0.83333333333)
postfitHist.SetBinContent(119,postfitHist.GetBinContent(119)*0.83333333333)
prefitHist.SetBinError(119,prefitHist.GetBinError(119)*0.83333333333)
postfitHist.SetBinError(119,postfitHist.GetBinError(119)*0.83333333333)
#CCDIS
prefitHist.SetBinContent(125,prefitHist.GetBinContent(125)+1)
postfitHist.SetBinContent(125,postfitHist.GetBinContent(125)+1)
#FSI
prefitHist.SetBinContent(101,prefitHist.GetBinContent(101)+1)
postfitHist.SetBinContent(101,postfitHist.GetBinContent(101)+1)
prefitHist.SetBinContent(102,prefitHist.GetBinContent(102)+1)
postfitHist.SetBinContent(102,postfitHist.GetBinContent(102)+1)
prefitHist.SetBinContent(103,prefitHist.GetBinContent(103)+1)
postfitHist.SetBinContent(103,postfitHist.GetBinContent(103)+1)
prefitHist.SetBinContent(104,prefitHist.GetBinContent(104)+1)
postfitHist.SetBinContent(104,postfitHist.GetBinContent(104)+1)
prefitHist.SetBinContent(105,prefitHist.GetBinContent(105)+1)
postfitHist.SetBinContent(105,postfitHist.GetBinContent(105)+1)
prefitHist.SetBinContent(106,prefitHist.GetBinContent(106)+1)
postfitHist.SetBinContent(106,postfitHist.GetBinContent(106)+1)


histoTypes = ["ND280 NuMode Flux", "ND280 ANuMode Flux", "SK NuMode Flux","SK ANuMode Flux","FSI","XSec"]
filename = ["flux_ND280_NuMode_Flux", "flux_ND280_ANuMode_Flux", "flux_SK_NuMode_Flux","flux_SK_ANuMode_Flux","xsec_FSI","xsec_all"]
paramRange = [[0,24],[25,49],[50,74],[75,99],[100,105],[106,130]]
yRange = [[0.80,1.2],[0.80,1.2],[0.80,1.2],[0.80,1.2],[0.5,1.5],[0,2.05]]
binLabels = []
for i in xrange(0, postfit_params.GetNrows()):
    binLabels.append("")

binLabels[0] = "ND280 #nu_{#mu}, FHC"
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

for i in xrange(100,131):
    binLabels[i] = param_list.At(i).GetString().Data()

binLabels[100] = "FSI Inel. Low E"
binLabels[101] = "FSI Inel. High E"
binLabels[102] = "FSI #pi Prod."
binLabels[103] = "FSI #pi Abs."
binLabels[104] = "FSI Charge Ex. Low E"
binLabels[105] = "FSI Charge Ex. High E"
binLabels[106] = "MA_{QE}"
binLabels[107] = "pF_{C}"
binLabels[108] = "pF_{O}"
binLabels[109] = "2p2h Norm #nu"
binLabels[110] = "2p2h Norm #bar{#nu}"
binLabels[111] = "2p2h Norm C to O"
binLabels[112] = "2p2h Shape C"
binLabels[113] = "2p2h Shape O"
binLabels[114] = "BeRPA A"
binLabels[115] = "BeRPA B"
binLabels[116] = "BeRPA D"
binLabels[117] = "BeRPA E"
binLabels[118] = "BeRPA U"
binLabels[119] = "CA5"
binLabels[120] = "MA_{RES}"
binLabels[121] = "Iso Bkg"
binLabels[122] = "#nu_{e}/#nu_{#mu}"
binLabels[123] = "#bar{#nu}_{e}/#bar{#nu}_{#mu}"
binLabels[124] = "CC DIS"
binLabels[125] = "CC Coh C"
binLabels[126] = "CC Coh O"
binLabels[127] = "NC Coh"
binLabels[128] = "NC 1 #gamma"
binLabels[129] = "NC Other Near"
binLabels[130] = "NC Other Far"


binLabels[131] = "FGD 1 CC-0#pi, FHC"
binLabels[174] = "FGD 1 CC-1#pi, FHC"
binLabels[215] = "FGD 1 CC-Other, FHC"
binLabels[256] = "FGD 1 #bar{#nu}_{#mu} CC-1Track, RHC"
binLabels[297] = "FGD 1 #bar{#nu}_{#mu} CC-NTracks, RHC"
binLabels[333] = "FGD 1 #nu_{#mu} CC-1Track, RHC"
binLabels[364] = "FGD 1 #nu_{#mu} CC-NTracks, RHC"
binLabels[405] = "FGD 2 CC-0#pi, FHC"
binLabels[446] = "FGD 2 CC-1#pi, FHC"
binLabels[487] = "FGD 2 CC-Other, FHC"
binLabels[528] = "FGD 2 #bar{#nu}_{#mu} CC-1Track, RHC"
binLabels[569] = "FGD 2 #bar{#nu}_{#mu} CC-NTracks, RHC"
binLabels[606] = "FGD 2 #nu_{#mu} CC-1Track, RHC"
binLabels[647] = "FGD 2 #nu_{#mu} CC-NTracks, RHC"


#Set the fill types for the prefit and postfit histograms.  Will plot them as
#points, but this will propagate to the error bars.

#Prefit is red fill, red point that isn't seen.
prefitHist.SetFillColor(kRed)
prefitHist.SetFillStyle(3002)
prefitHist.SetLineColor(kRed)
prefitHist.SetMarkerColor(kRed)
prefitHist.SetMarkerStyle(7)
prefitHist.SetMarkerSize(0.5)


#Postfit is with blue squares
postfitHist.SetMarkerColor(4)
postfitHist.SetMarkerStyle(21)
postfitHist.SetMarkerSize(0.5)
postfitHist.SetLineWidth(2)

#line at 0
line = TLine(0,0,1000,0)
line.SetLineColor(kWhite)
line.SetLineStyle(2)
c1 = TCanvas()
c1.cd()
c1.Print(sys.argv[2] + "[")

for i in xrange(0, len(histoTypes)):

    stack = THStack("pStack",histoTypes[i])
    stack.Add(prefitHist,"E2P")
    stack.Add(postfitHist,"E1")
    
    c1.cd()
    stack.Draw("NOSTACK")
    line.Draw("SAME")

    #Set the range to the desired parameters that want (low end to topend + 1)
    stack.GetXaxis().SetRangeUser(paramRange[i][0],paramRange[i][1] + 1)
    stack.SetMinimum(yRange[i][0])
    stack.SetMaximum(yRange[i][1])

    leg = TLegend(0.12,0.8,0.32,0.94)
    leg.SetFillColor(0)
    leg.AddEntry(prefitHist, "prefit","FEP")
    leg.AddEntry(postfitHist,"postfit", "ELP")

    leg.Draw()

    for j in xrange(0, prefit_params.GetNrows()):
        stack.GetXaxis().SetBinLabel(j+1,binLabels[j])
    
    stack.GetXaxis().LabelsOption("v")

    c1.Modified()
    c1.Update()
    c1.Print(sys.argv[2])
    c1.Print(filename[i] + ".pdf")
    c1.Print(filename[i] + ".png")
    c1.Print(filename[i] + ".eps")

histoTypes = ["FSI parameters","CC0#pi parameters","BeRPA parameters","CC1#pi parameters","Other","Observable normalization FGD1","Observable normalization FGD2"]
filename = ["xsec_FSI","xsec_CC0pi","xsec_BeRPA","xsec_CC1pi","xsec_Other","osbnorm_FGD1","osbnorm_FGD2"]

paramRange = [[100,105],[106,113],[114,118],[119,121],[122,130],[131,131+273],[131+274,688]]
yRange = [[0.2,1.8],[0,2.1],[0,2.1],[0,2.1],[0,2.1],[0.5,1.5],[0.5,1.5]]

c1.cd()

for i in xrange(0, len(histoTypes)):

    stack = THStack("pStack",histoTypes[i])
    if i == 1:
        stack.Add(prefitHist,"HIST P")
        line = TLine(107,1,114,1)
        line.SetLineColor(kRed)
        line.SetLineStyle(2)
    else:
        stack.Add(prefitHist,"E2P")
    stack.Add(postfitHist,"E1")
    postfitHist.SetMarkerSize(0.5)
    #stack.Add(postfitHist,"E1")
    
    c1.cd()
    stack.Draw("NOSTACK")
    
    if i == 1 or i > 4:
        line.Draw("SAME")

    #Set the range to the desired parameters that want (low end to topend + 1)
    stack.GetXaxis().SetRangeUser(paramRange[i][0],paramRange[i][1] + 1)
    stack.SetMinimum(yRange[i][0])
    stack.SetMaximum(yRange[i][1])

    leg = TLegend(0.12,0.8,0.32,0.94)
    leg.SetFillColor(0)
    leg.AddEntry(prefitHist, "prefit","FEP")
    leg.AddEntry(postfitHist,"postfit", "ELP")

    leg.Draw()

    for j in xrange(100, prefit_params.GetNrows()):
        stack.GetXaxis().SetBinLabel(j+1,binLabels[j])
    
    if i<5:
        stack.GetXaxis().LabelsOption("v")
    else:
        stack.GetXaxis().LabelsOption("")
    c1.Modified()
    c1.Update()
    c1.Print(sys.argv[2])
    c1.Print(filename[i] + ".pdf")
    c1.Print(filename[i] + ".png")
    c1.Print(filename[i] + ".eps")

fluxfile = TFile("/home/simonbienstock/T2K/work/BANFF/v3r18/inputs/flux_covariance_banff_13av2.root")
flux_axis = []
flux_axis.append(fluxfile.Get("nd5_numode_numu_bins"))
flux_axis.append(fluxfile.Get("nd5_numode_numub_bins"))
flux_axis.append(fluxfile.Get("nd5_numode_nue_bins"))
flux_axis.append(fluxfile.Get("nd5_numode_nueb_bins"))
flux_axis.append(fluxfile.Get("nd5_anumode_numu_bins"))
flux_axis.append(fluxfile.Get("nd5_anumode_numub_bins"))
flux_axis.append(fluxfile.Get("nd5_anumode_nue_bins"))
flux_axis.append(fluxfile.Get("nd5_anumode_nueb_bins"))
flux_axis.append(fluxfile.Get("sk_numode_numu_bins"))
flux_axis.append(fluxfile.Get("sk_numode_numub_bins"))
flux_axis.append(fluxfile.Get("sk_numode_nue_bins"))
flux_axis.append(fluxfile.Get("sk_numode_nueb_bins"))
flux_axis.append(fluxfile.Get("sk_anumode_numu_bins"))
flux_axis.append(fluxfile.Get("sk_anumode_numub_bins"))
flux_axis.append(fluxfile.Get("sk_anumode_nue_bins"))
flux_axis.append(fluxfile.Get("sk_anumode_nueb_bins"))

histoTypes2 = ["ND280 FHC #nu_{#mu} Flux","ND280 FHC #bar{#nu}_{#mu} Flux","ND280 FHC #nu_{e} Flux","ND280 FHC #bar{#nu}_{e} Flux","ND280 RHC #nu_{#mu} Flux","ND280 RHC #bar{#nu}_{#mu} Flux","ND280 RHC #nu_{e} Flux","ND280 RHC #bar{#nu}_{e} Flux","SK FHC #nu_{#mu} Flux","SK FHC #bar{#nu}_{#mu} Flux","SK FHC #nu_{e} Flux","SK FHC #bar{#nu}_{e} Flux","SK RHC #nu_{#mu} Flux","SK RHC #bar{#nu}_{#mu} Flux","SK RHC #nu_{e} Flux","SK RHC #bar{#nu}_{e} Flux"]
filename = ["flux_ND280_FHC_numu_Flux","flux_ND280_FHC_numubar_Flux","flux_ND280_FHC_nue_Flux","flux_ND280_FHC_nuebar_Flux","flux_ND280_RHC_numu_Flux","flux_ND280_RHC_numubar_Flux","flux_ND280_RHC_nue_Flux","flux_ND280_RHC_nuebar_Flux","flux_SK_FHC_numu_Flux","flux_SK_FHC_numubar_Flux","flux_SK_FHC_nue_Flux","flux_SK_FHC_nuebar_Flux","flux_SK_RHC_numu_Flux","flux_SK_RHC_numubar_Flux","flux_SK_RHC_nue_Flux","flux_SK_RHC_nuebar_Flux"]
paramRange2 = [[0,10],[11,15],[16,22],[23,24],[25,29],[30,40],[41,42],[43,49],[50,60],[61,65],[66,72],[73,74],[75,79],[80,90],[91,92],[93,99]]
yRange2 = [[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3]]


gStyle.SetPadBottomMargin(0.2);# 0.15 in official
gStyle.SetLegendBorderSize(1);

c1 = TCanvas()
c1.cd()

for i in xrange(0, len(histoTypes2)):
    
    nbin = int(paramRange2[i][1]-paramRange2[i][0])
    prefit_flux = TH1F("","",flux_axis[i].GetNbins(),flux_axis[i].GetBinLowEdge(1),flux_axis[i].GetBinUpEdge(flux_axis[i].GetNbins()))
    postfit_flux = TH1F("","",flux_axis[i].GetNbins(),flux_axis[i].GetBinLowEdge(1),flux_axis[i].GetBinUpEdge(flux_axis[i].GetNbins()))
    prefit_flux.GetXaxis().Set(flux_axis[i].GetNbins(),flux_axis[i].GetXbins().GetArray())
    postfit_flux.GetXaxis().Set(flux_axis[i].GetNbins(),flux_axis[i].GetXbins().GetArray())

    #Prefit is red fill, red point that isn't seen.
    prefit_flux.SetFillColor(kRed)
    prefit_flux.SetFillStyle(3002)
    prefit_flux.SetLineColor(kRed)
    prefit_flux.SetMarkerColor(kRed)
    prefit_flux.SetMarkerStyle(7)
    prefit_flux.SetMarkerSize(0.5)


    #Postfit is with blue squares
    postfit_flux.SetMarkerColor(4)
    postfit_flux.SetMarkerStyle(21)
    postfit_flux.SetMarkerSize(0.5)
    postfit_flux.SetLineWidth(2)

    for j in xrange(0,nbin+2):
        print int(paramRange2[i][0])+j, prefitHist.GetBinContent(int(paramRange2[i][0])+j)
        prefit_flux.SetBinContent(j,prefitHist.GetBinContent(int(paramRange2[i][0])+j))
        prefit_flux.SetBinError(j,prefitHist.GetBinError(int(paramRange2[i][0])+j))
        postfit_flux.SetBinContent(j,postfitHist.GetBinContent(int(paramRange2[i][0])+j))
        postfit_flux.SetBinError(j,postfitHist.GetBinError(int(paramRange2[i][0])+j))
        
    stack = THStack("pStack",histoTypes2[i])
    postfit_flux.GetXaxis().SetTitle("E_{#nu} (GeV)")
    prefit_flux.GetXaxis().SetTitle("E_{#nu} (GeV)")
    stack.Add(prefit_flux,"E2P")
    stack.Add(postfit_flux,"E1")
    postfit_flux.SetMarkerSize(0.5)

    
    c1.SetLogx()
    c1.cd()
    stack.Draw("NOSTACK")
    
    if i == 1 or i > 4:
        line.Draw("SAME")

    #Set the range to the desired parameters that want (low end to topend + 1)
    stack.SetMinimum(yRange2[i][0])
    stack.SetMaximum(yRange2[i][1])

    leg = TLegend(0.22,0.24,0.8,0.46)
    leg.SetFillColor(0)
    leg.SetBorderSize(0)
    leg.AddEntry(prefit_flux, "Prior to ND280 constraint","FEP")
    leg.AddEntry(postfit_flux,"After ND280 constraint", "ELP")

    leg.Draw()

    t =  TLatex()
    t.SetTextSize(0.06)
    t.DrawTextNDC(0.6,0.05,"Neutrino Energy (GeV)")
    
    c1.Modified()
    c1.Update()
    c1.Print(sys.argv[2])
    c1.Print(filename[i] + ".pdf")
    c1.Print(filename[i] + ".png")
    c1.Print(filename[i] + ".eps")

c1.Print(sys.argv[2] + "]")






