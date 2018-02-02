#!/usr/bin/python
from ROOT import *
import sys
import math

gStyle.SetOptStat(0)
gStyle.SetPadBottomMargin(0.3)
gStyle.SetLabelSize(0.055)

gROOT.SetBatch(kTRUE);

#Useage is python MakeParameterPlots.py postfitFileWithAllParameters outputFileName
inFile = TFile(sys.argv[1])

prefit_params = inFile.Get("prefit_params")
prefit_cov = inFile.Get("prefit_cov")
prefit_err = TVectorD(prefit_params.GetNrows())

postfit_params = inFile.Get("postfit_params")
postfit_cov = inFile.Get("postfit_cov")
postfit_err = TVectorD(postfit_params.GetNrows())

for i in xrange(0, prefit_err.GetNrows()):
    prefit_err[i]  = math.sqrt(prefit_cov[i][i])
    postfit_err[i] = math.sqrt(postfit_cov[i][i])


for i in xrange(0, param_list.GetEntries()):
    print(str(i)+": "+str(param_list.At(i).GetString().Data()))
    
#We just need to make one TH1D for each, then just change the range as needed
#to get the desired parameter subset.
prefitHist  = TH1D(prefit_params)
postfitHist = TH1D(postfit_params)

for i in xrange(1, prefit_err.GetNrows() + 1):
    prefitHist .SetBinError(i,prefit_err [i-1])
    postfitHist.SetBinError(i,postfit_err[i-1])

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


histoTypes = ["ND280 NuMode Flux", "ND280 ANuMode Flux", "SK NuMode Flux","SK ANuMode Flux","FSI","XSec"]
paramRange = [[0,24],[25,49],[50,74],[75,99],[100,105],[106,130]]
yRange = [[0.80,1.2],[0.80,1.2],[0.80,1.2],[0.80,1.2],[-1.0,1.2],[-0.5,2.05]]
binLabels = []
for i in xrange(0, postfit_params.GetNrows()):
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

for i in xrange(100,130):
    binLabels[i] = param_list.At(i).GetString().Data()

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

    leg = TLegend(0.12+0.3,0.84-0.05,0.42+0.3,0.98-0.05)
    leg.SetFillColor(0)
    leg.AddEntry(prefitHist, "Pre fit",  "FEP")
    leg.AddEntry(postfitHist,"Post fit",    "ELP")

    leg.Draw()

    for j in xrange(0, prefit_params.GetNrows()):
        stack.GetXaxis().SetBinLabel(j+1,binLabels[j])
    
    stack.GetXaxis().LabelsOption("v")

    c1.Modified()
    c1.Update()
    c1.Print(sys.argv[2])


histoTypes = ["SK FHC #nu_{#mu} Flux","FSI parameters","CCQE parameters","CC1#pi parameters"]
paramRange = [[50,60],[100,105],[106,108],[119,121]]
yRange = [[0.80,1.2],[-1.0,1.0],[0.7,1.4],[0.7,1.4]]

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

    stack = THStack("pStack",histoTypes[i])
    stack.Add(prefitHist,"E2P")
    stack.Add(postfitHist,"E1")
    postfitHist.SetMarkerSize(0.5)
    #stack.Add(postfitHist,"E1")
    
    c1.cd()
    stack.Draw("NOSTACK")
    line.Draw("SAME")

    #Set the range to the desired parameters that want (low end to topend + 1)
    stack.GetXaxis().SetRangeUser(paramRange[i][0],paramRange[i][1] + 1)
    stack.SetMinimum(yRange[i][0])
    stack.SetMaximum(yRange[i][1])

    leg = TLegend(0.12+0.3,0.84-0.05,0.5+0.3,0.98-0.05)
    leg.SetFillColor(0)
    leg.AddEntry(prefitHist, "Pre fit",  "FEP")
    leg.AddEntry(postfitHist,"Post fit", "ELP")

    leg.Draw()

    for j in xrange(0, prefit_params.GetNrows()):
        stack.GetXaxis().SetBinLabel(j+1,binLabels[j])
    
    stack.GetXaxis().LabelsOption("v")

    c1.Modified()
    c1.Update()
    c1.Print(sys.argv[2])

c1.Print(sys.argv[2] + "]")






