#!/usr/bin/python
from ROOT import *
import sys
import math

gStyle.SetOptStat(0)
gStyle.SetPadBottomMargin(0.3)

#gStyle.SetPadLeftMargin(0.25)
gStyle.SetLabelSize(0.035)

gROOT.SetBatch(kTRUE);

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

    prefit_err[i] = math.sqrt(prefit_cov[i][i])
    postfit_err[i] = math.sqrt(postfit_cov[i][i])


#histoTypes = ["ND280 NuMode Flux", "ND280 ANuMode Flux", "SK NuMode Flux","SK ANuMode Flux","FSI","XSec"]
#paramRange = [[0,24],[25,49],[50,74],[75,99],[100,105],[106,121]]
#yRange = [[0.80,1.35],[0.80,1.35],[0.80,1.35],[0.80,1.35],[-1.0,1.2],[-1.0,2.25]]
histoTypes = ["Flux","XSec","ObsNorm FGD1","ObsNorm FGD2"]
paramRange = [[0,99],[100,130],[131,131+273],[131+274,postfit_params.GetNrows()-1]]
#yRange = [[0.80,1.35],[-1.0,2.25],[0.5,2.0]]
#Asher's y ranges.
yRange = [[0.8,1.2],[0.0,2.0],[0.5,1.5],[0.5,1.5]]


binLabels = []
binLabels_neworder = []
for i in xrange(0, postfit_params.GetNrows()):
    binLabels.append("")
    binLabels_neworder.append("")

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



#Now re-order everything to the same order as Asher for plotting.
prefit_params_neworder = TVectorD(prefit_params.GetNrows())
postfit_params_neworder = TVectorD(prefit_params.GetNrows())
prefit_err_neworder = TVectorD(prefit_params.GetNrows())
postfit_err_neworder = TVectorD(prefit_params.GetNrows())


#The new order moves FSI (originally 100-105) after the other XSec paramreters
#106-121 inclusive)
#So 106-121 become 100 to 115
#and 100 to 105 become 116 to 121.
for i in xrange(0,prefit_params.GetNrows()):

    #If it's a flux parameter, do nothing.
    if i < 100:
        prefit_params_neworder[i] = prefit_params[i]
        postfit_params_neworder[i] = postfit_params[i]
        prefit_err_neworder[i] = prefit_err[i]
        postfit_err_neworder[i] = postfit_err[i]
        binLabels_neworder[i] = binLabels[i]

    #Otherwise, if it's an FSI parameter, add 25 to get the new slot.
    elif i < 106:
        prefit_params_neworder[i+25] = prefit_params[i]
        postfit_params_neworder[i+25] = postfit_params[i]
        prefit_err_neworder[i+25] = prefit_err[i]
        postfit_err_neworder[i+25] = postfit_err[i]
        binLabels_neworder[i+25] = binLabels[i]

    #Otherwise, if it's another XSec parameter, subtract 6 to get the new slot.
    elif i < 131:
        prefit_params_neworder[i-6] = prefit_params[i]
        postfit_params_neworder[i-6] = postfit_params[i]
        prefit_err_neworder[i-6] = prefit_err[i]
        postfit_err_neworder[i-6] = postfit_err[i]
        binLabels_neworder[i-6] = binLabels[i]

    #Otherwise, it's an ObsNorm parameter, so copy it over into the same slot.
    else:
        prefit_params_neworder[i] = prefit_params[i]/prefit_params[i]
        postfit_params_neworder[i] = postfit_params[i]/prefit_params[i]
        prefit_err_neworder[i] = prefit_err[i]
        postfit_err_neworder[i] = postfit_err[i]
        binLabels_neworder[i] = binLabels[i]


#We just need to make one TH1D for each, then just change the range as needed
#to get the desired parameter subset.
prefitHist = TH1D(prefit_params_neworder)
postfitHist = TH1D(postfit_params_neworder)

for i in xrange(1, prefit_err_neworder.GetNrows() + 1):

    prefitHist.SetBinError(i,prefit_err_neworder[i-1])
    postfitHist.SetBinError(i,postfit_err_neworder[i-1])

#CCQE no prefit error
prefitHist.SetBinError(101,0.0)
prefitHist.SetBinError(102,0.0)
prefitHist.SetBinError(103,0.0)
prefitHist.SetBinError(104,0.0)
prefitHist.SetBinError(105,0.0)
prefitHist.SetBinError(106,0.0)
prefitHist.SetBinError(107,0.0)
prefitHist.SetBinError(108,0.0)

#BeRPA_A
prefitHist.SetBinContent(109,prefitHist.GetBinContent(109)*1.69491525424)
postfitHist.SetBinContent(109,postfitHist.GetBinContent(109)*1.69491525424)
prefitHist.SetBinError(109,prefitHist.GetBinError(109)*1.69491525424)
postfitHist.SetBinError(109,postfitHist.GetBinError(109)*1.69491525424)
#BeRPA_B
prefitHist.SetBinContent(110,prefitHist.GetBinContent(110)*0.95238095238)
postfitHist.SetBinContent(110,postfitHist.GetBinContent(110)*0.95238095238)
prefitHist.SetBinError(110,prefitHist.GetBinError(110)*0.95238095238)
postfitHist.SetBinError(110,postfitHist.GetBinError(110)*0.95238095238)
#BeRPA_D
prefitHist.SetBinContent(111,prefitHist.GetBinContent(111)*0.88495575221)
postfitHist.SetBinContent(111,postfitHist.GetBinContent(111)*0.88495575221)
prefitHist.SetBinError(111,prefitHist.GetBinError(111)*0.88495575221)
postfitHist.SetBinError(111,postfitHist.GetBinError(111)*0.88495575221)
#BeRPA_E
prefitHist.SetBinContent(112,prefitHist.GetBinContent(112)*1.13636363636)
postfitHist.SetBinContent(112,postfitHist.GetBinContent(112)*1.13636363636)
prefitHist.SetBinError(112,prefitHist.GetBinError(112)*1.13636363636)
postfitHist.SetBinError(112,postfitHist.GetBinError(112)*1.13636363636)
#BeRPA_U
prefitHist.SetBinContent(113,prefitHist.GetBinContent(113)*0.83333333333)
postfitHist.SetBinContent(113,postfitHist.GetBinContent(113)*0.83333333333)
prefitHist.SetBinError(113,prefitHist.GetBinError(113)*0.83333333333)
postfitHist.SetBinError(113,postfitHist.GetBinError(113)*0.83333333333)
#CCDIS
prefitHist.SetBinContent(119,prefitHist.GetBinContent(119)+1)
postfitHist.SetBinContent(119,postfitHist.GetBinContent(119)+1)

#FSI
prefitHist.SetBinContent(126,prefitHist.GetBinContent(126)+1)
postfitHist.SetBinContent(126,postfitHist.GetBinContent(126)+1)
prefitHist.SetBinContent(127,prefitHist.GetBinContent(127)+1)
postfitHist.SetBinContent(127,postfitHist.GetBinContent(127)+1)
prefitHist.SetBinContent(128,prefitHist.GetBinContent(128)+1)
postfitHist.SetBinContent(128,postfitHist.GetBinContent(128)+1)
prefitHist.SetBinContent(129,prefitHist.GetBinContent(129)+1)
postfitHist.SetBinContent(129,postfitHist.GetBinContent(129)+1)
prefitHist.SetBinContent(130,prefitHist.GetBinContent(130)+1)
postfitHist.SetBinContent(130,postfitHist.GetBinContent(130)+1)
prefitHist.SetBinContent(131,prefitHist.GetBinContent(131)+1)
postfitHist.SetBinContent(131,postfitHist.GetBinContent(131)+1)

#Set the fill types for the prefit and postfit histograms.  Will plot them as
#points, but this will propagate to the error bars.

#Prefit is red fill, red point that isn't seen.
prefitHist.SetFillColor(kRed)
prefitHist.SetFillStyle(3001)
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

#c1 = TCanvas("c1","c1",500,500)
c1 = TCanvas()
c1.cd()
c1.SetTicks(1,1)
#Start with the legend.
#leg = TLegend(0.05,0.05,0.95,0.95)
#leg.SetFillColor(0)
#leg.AddEntry(prefitHist, "Prefit","FEP")
#leg.AddEntry(postfitHist,"Postfit", "FEP")
#leg.AddEntry(prefitHist, "All targets for MEC_C","FEP")
#leg.AddEntry(postfitHist,"Carbon targets only for MEC_C", "FEP")
#leg.AddEntry(prefitHist, "Released fit results","FEP")
#leg.AddEntry(postfitHist,"EB_O, pF_O, and CCCOH_O_0 fixed", "FEP")
#leg.Draw()
c1.Print(sys.argv[2] + "[")

for i in xrange(0, len(histoTypes)):

    #stack = THStack("pStack",histoTypes[i]+";Parameter ID;")
    stack = THStack("pStack",";;Variation rel. nominal")
    stack.Add(prefitHist,"E2P")
    stack.Add(postfitHist,"E1")
    
    c1.cd()
    stack.Draw("NOSTACK")

    #Set the range to the desired parameters that want (low end to topend + 1)
    stack.GetXaxis().SetRangeUser(paramRange[i][0],paramRange[i][1] + 1)
    stack.SetMinimum(yRange[i][0])
    stack.SetMaximum(yRange[i][1])

    leg = TLegend(0.15,0.86,0.35,0.995)
    leg.SetFillColor(0)
    leg.AddEntry(prefitHist, "Prefit","FEP")
    leg.AddEntry(postfitHist,"Postfit", "ELP")
    leg.Draw()

    for j in xrange(0, prefit_params.GetNrows()):
        stack.GetXaxis().SetBinLabel(j+1,binLabels_neworder[j])
    
    
    stack.GetXaxis().LabelsOption("v")
    #stack.GetYaxis().SetTitleOffset(1.4)
    c1.Modified()
    c1.Update()
    c1.Print(sys.argv[2])


c1.Print(sys.argv[2] + "]")






