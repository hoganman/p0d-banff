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

# Get prefit parameters, covariance, and errors
prefit_params = inFile.Get("prefit_params")
prefit_cov    = inFile.Get("prefit_cov")
prefit_err    = TVectorD(prefit_params.GetNrows())

# Get postfit parameters, covariance, and errors
postfit_params = inFile.Get("postfit_params")
postfit_cov    = inFile.Get("postfit_cov")
postfit_err    = TVectorD(postfit_params.GetNrows())

# Set errors for prefit and postfit
for i in xrange(0, prefit_err.GetNrows()):
    prefit_err[i]  = sqrt(prefit_cov[i][i])
    postfit_err[i] = sqrt(postfit_cov[i][i])

#We just need to make one TH1D for each, then just change the range as needed
#to get the desired parameter subset.
prefitHist  = TH1D(prefit_params)
postfitHist = TH1D(postfit_params)

for i in xrange(1, prefit_err.GetNrows() + 1):
    prefitHist.SetBinError(i,prefit_err[i-1])
    postfitHist.SetBinError(i,postfit_err[i-1])

#Find parameter locations to eliminate some hardcoding of numbers
#Labels with 'RAW' in name are for range positions
#Other labels are for setting bin contents and errors
BERPA      = -1
CCDIS      = -1
FEFQE      = -1
NCOTHERRAW = -1
BERPARAW   = -1
FEFQERAW   = -1
MAQERAW    = -1
CA5RAW     = -1
for i in xrange(0, param_list.GetEntriesFast()):
    if param_list[i] == 'BeRPA_A':
        BERPA    = i+1
        BERPARAW = i
    elif param_list[i] == 'CC_DIS':
        CCDIS = i+1
    elif param_list[i] == 'FEFQE':
        FEFQE    = i+1
        FEFQERAW = i
    elif param_list[i] == 'NC_other_far':
        NCOTHERRAW = i
    elif param_list[i] == 'MAQE':
        MAQERAW = i
    elif param_list[i] == 'CA5':
        CA5RAW = i
    else:
        continue

#Print statements for debugging
#print 'BeRPA_A: ', BERPA, 'CC_DIS: ', CCDIS, 'FEFQE: ',  FEFQE, 'NC_other_far: ', NCOTHERRAW
#print 'BeRPA RAW: ', BERPARAW, 'FEFQE RAW: ', FEFQERAW, 'MAQE RAW: ', MAQERAW

#BeRPA_A
prefitHist.SetBinContent(BERPA,prefitHist.GetBinContent(BERPA)*1.69491525424)
prefitHist.SetBinError(BERPA,prefitHist.GetBinError(BERPA)*1.69491525424)
postfitHist.SetBinContent(BERPA,postfitHist.GetBinContent(BERPA)*1.69491525424)
postfitHist.SetBinError(BERPA,postfitHist.GetBinError(BERPA)*1.69491525424)

#BeRPA_B
prefitHist.SetBinContent(BERPA+1,prefitHist.GetBinContent(BERPA+1)*0.95238095238)
prefitHist.SetBinError(BERPA+1,prefitHist.GetBinError(BERPA+1)*0.95238095238)
postfitHist.SetBinContent(BERPA+1,postfitHist.GetBinContent(BERPA+1)*0.95238095238)
postfitHist.SetBinError(BERPA+1,postfitHist.GetBinError(BERPA+1)*0.95238095238)

#BeRPA_D
prefitHist.SetBinContent(BERPA+2,prefitHist.GetBinContent(BERPA+2)*0.88495575221)
prefitHist.SetBinError(BERPA+2,prefitHist.GetBinError(BERPA+2)*0.88495575221)
postfitHist.SetBinContent(BERPA+2,postfitHist.GetBinContent(BERPA+2)*0.88495575221)
postfitHist.SetBinError(BERPA+2,postfitHist.GetBinError(BERPA+2)*0.88495575221)

#BeRPA_E
prefitHist.SetBinContent(BERPA+3,prefitHist.GetBinContent(BERPA+3)*1.13636363636)
prefitHist.SetBinError(BERPA+3,prefitHist.GetBinError(BERPA+3)*1.13636363636)
postfitHist.SetBinContent(BERPA+3,postfitHist.GetBinContent(BERPA+3)*1.13636363636)
postfitHist.SetBinError(BERPA+3,postfitHist.GetBinError(BERPA+3)*1.13636363636)

#BeRPA_U
prefitHist.SetBinContent(BERPA+4,prefitHist.GetBinContent(BERPA+4)*0.83333333333)
prefitHist.SetBinError(BERPA+4,prefitHist.GetBinError(BERPA+4)*0.83333333333)
postfitHist.SetBinContent(BERPA+4,postfitHist.GetBinContent(BERPA+4)*0.83333333333)
postfitHist.SetBinError(BERPA+4,postfitHist.GetBinError(BERPA+4)*0.83333333333)

#CCDIS
prefitHist.SetBinContent(CCDIS,prefitHist.GetBinContent(CCDIS)+1)
postfitHist.SetBinContent(CCDIS,postfitHist.GetBinContent(CCDIS)+1)

#FSI
prefitHist.SetBinContent(FEFQE,prefitHist.GetBinContent(FEFQE)+1)
postfitHist.SetBinContent(FEFQE,postfitHist.GetBinContent(FEFQE)+1)
prefitHist.SetBinContent(FEFQE+1,prefitHist.GetBinContent(FEFQE+1)+1)
postfitHist.SetBinContent(FEFQE+1,postfitHist.GetBinContent(FEFQE+1)+1)
prefitHist.SetBinContent(FEFQE+2,prefitHist.GetBinContent(FEFQE+2)+1)
postfitHist.SetBinContent(FEFQE+2,postfitHist.GetBinContent(FEFQE+2)+1)
prefitHist.SetBinContent(FEFQE+3,prefitHist.GetBinContent(FEFQE+3)+1)
postfitHist.SetBinContent(FEFQE+3,postfitHist.GetBinContent(FEFQE+3)+1)
prefitHist.SetBinContent(FEFQE+4,prefitHist.GetBinContent(FEFQE+4)+1)
postfitHist.SetBinContent(FEFQE+4,postfitHist.GetBinContent(FEFQE+4)+1)

#Set histo types, ranges, and bin labels
histoTypes = ["ND280 NuMode Flux", "ND280 ANuMode Flux", "SK NuMode Flux","SK ANuMode Flux","FSI","XSec","XSec"]
paramRange = [[0,24],[25,49],[50,74],[75,99],[FEFQERAW,FEFQERAW+4],[MAQERAW,NCOTHERRAW],[MAQERAW,NCOTHERRAW]]
yRange = [[0.80,1.2],[0.80,1.2],[0.80,1.2],[0.80,1.2],[0,2.1],[0,2.1],[0,2.1]]
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

binLabels[100] = "FGD 1 CC-0#pi, FHC"
binLabels[143] = "FGD 1 CC-1#pi, FHC"
binLabels[184] = "FGD 1 CC-Other, FHC"
binLabels[225] = "FGD 1 #bar{#nu}_{#mu} CC-1Track, RHC"
binLabels[266] = "FGD 1 #bar{#nu}_{#mu} CC-NTracks, RHC"
binLabels[302] = "FGD 1 #nu_{#mu} CC-1Track, RHC"
binLabels[333] = "FGD 1 #nu_{#mu} CC-NTracks, RHC"
binLabels[374] = "FGD 2 CC-0#pi, FHC"
binLabels[415] = "FGD 2 CC-1#pi, FHC"
binLabels[456] = "FGD 2 CC-Other, FHC"
binLabels[497] = "FGD 2 #bar{#nu}_{#mu} CC-1Track, RHC"
binLabels[538] = "FGD 2 #bar{#nu}_{#mu} CC-NTracks, RHC"
binLabels[575] = "FGD 2 #nu_{#mu} CC-1Track, RHC"
binLabels[616] = "FGD 2 #nu_{#mu} CC-NTracks, RHC"

for i in xrange(FEFQERAW,NCOTHERRAW+1):
    binLabels[i] = param_list.At(i).GetString().Data()

#Set the fill types for the prefit and postfit histograms.
#Will plot them as points, but this will propagate to the error bars.

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

#Line at 0
line = TLine(0,0,1000,0)
line.SetLineColor(kWhite)
line.SetLineStyle(2)

c1 = TCanvas()
c1.cd()
c1.Print(sys.argv[2] + "[")

for i in xrange(0, len(histoTypes)):
    #CCQE parameters don't really have a prior,
    #so it doesn't make sense to have errors on these params:
    #MAQE, pF_C/O, 2p2h_norm_nu(bar), 2p2h_normCtoO, 2p2h_shape_C/O
    if i == 6:
        prefitHist.SetBinError(MAQERAW+1,0.0)
        prefitHist.SetBinError(MAQERAW+2,0.0)
        prefitHist.SetBinError(MAQERAW+3,0.0)
        prefitHist.SetBinError(MAQERAW+4,0.0)
        prefitHist.SetBinError(MAQERAW+5,0.0)
        prefitHist.SetBinError(MAQERAW+6,0.0)
        prefitHist.SetBinError(MAQERAW+7,0.0)
        prefitHist.SetBinError(MAQERAW+8,0.0)

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

    leg = TLegend(0.68,0.84,0.88,0.98)
    leg.SetFillColor(0)
    leg.AddEntry(prefitHist, "Prefit","FEP")
    leg.AddEntry(postfitHist,"Postfit", "ELP")

    leg.Draw()

    for j in xrange(0, prefit_params.GetNrows()):
        stack.GetXaxis().SetBinLabel(j+1,binLabels[j])
    
    stack.GetXaxis().LabelsOption("v")

    c1.Modified()
    c1.Update()
    c1.Print(sys.argv[2])


histoTypes = ["SK FHC #nu_{#mu} Flux","ObsNorm FGD1","ObsNorm FGD2","FSI parameters","CC0#pi parameters","BeRPA parameters","CC1#pi parameters"]
paramRange = [[50,60],[100,100+273],[100+274,FEFQERAW-1],[FEFQERAW,FEFQERAW+4],[MAQERAW,MAQERAW+7],[BERPARAW,BERPARAW+4],[CA5RAW,CA5RAW+2]]
yRange = [[0.80,1.2],[0.5,1.5],[0.5,1.5],[0.0,2.1],[0.5,2.1],[0.5,2.1],[0.5,1.4]]

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
    if i == 4:
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
    if i == 2:
        line.Draw("SAME")

    #Set the range to the desired parameters that want (low end to topend + 1)
    stack.GetXaxis().SetRangeUser(paramRange[i][0],paramRange[i][1] + 1)
    stack.SetMinimum(yRange[i][0])
    stack.SetMaximum(yRange[i][1])

    leg = TLegend(0.12,0.84,0.32,0.98)
    leg.SetFillColor(0)
    leg.AddEntry(prefitHist, "Prefit","FEP")
    leg.AddEntry(postfitHist,"Postfit", "ELP")
    
    leg.Draw()

    for j in xrange(0, prefit_params.GetNrows()):
        stack.GetXaxis().SetBinLabel(j+1,binLabels[j])
    
    stack.GetXaxis().LabelsOption("v")

    c1.Modified()
    c1.Update()
    c1.Print(sys.argv[2])

c1.Print(sys.argv[2] + "]")

